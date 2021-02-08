// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PreviewHost.h"	
#include "../uieditor/Global.h"
#include "Adapter.h"
#include "helper/SplitString.h"
#include <MMSystem.h>

#pragma comment(lib, "winmm.lib")


static DWORD		g_dwLastMsgTime;

void (CALLBACK cb_HeartTimer)(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	if ((GetTickCount() - g_dwLastMsgTime) > 5000)
	{
		ExitProcess(0);
	}
}

CPreviewHost::CPreviewHost(IListener *pListener,LPCTSTR pszLayoutId, HWND hEditor/* = NULL*/)
: SHostWnd(pszLayoutId)
,m_pHover(NULL)
,m_pSel(NULL)
,m_bVirtualRoot(FALSE)
,m_bSOUIWnd(FALSE)
,m_hOwner(hEditor)
,m_pListener(pListener)
{
}

CPreviewHost::~CPreviewHost()
{
}

LRESULT CPreviewHost::OnMouseEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(GetKeyState(VK_MENU)&0x80)
	{
		SetMsgHandled(FALSE);
		return TRUE;
	}else
	{
		CPoint pt(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
		if(uMsg == WM_LBUTTONDOWN)
		{//tell editor the clicked window id.
			m_pHover->SetVisible(FALSE);
			m_pSel->SetVisible(FALSE);
			SWND swnd = SwndFromPoint(pt,true);
			m_pSel->SetVisible(TRUE);
			m_pHover->SetVisible(TRUE);
			if(swnd)
			{
				SWindow *p = SWindowMgr::GetWindow(swnd);
				CRect rcSel = p->GetWindowRect();
				m_pSel->Move(rcSel);
				SList<int> lstIndex;
				GetSwndIndex(p,lstIndex);
				if(m_bVirtualRoot)
				{
					lstIndex.RemoveHead();
				}else
				{
					//lstIndex.AddHead(0);
				}
				if(m_hOwner)
				{
					COPYDATASTRUCT cds;
					cds.dwData = select_id;
					cds.cbData = sizeof(int)*lstIndex.GetCount();
					int *pData = new int[lstIndex.GetCount()];
					int i=0;
					SPOSITION pos = lstIndex.GetHeadPosition();
					while(pos)
					{
						pData[i++]=lstIndex.GetNext(pos);
					}
					cds.lpData = pData;
					::SendMessage(m_hOwner,WM_COPYDATA,(WPARAM)m_hWnd,(LPARAM)&cds);
					delete []pData;
				}
			}
		}else if(uMsg == WM_MOUSEMOVE)
		{//highlight the hovered window
			m_pHover->SetVisible(FALSE);
			SWND swnd = SwndFromPoint(pt,true);
			m_pHover->SetVisible(TRUE);
			if(swnd)
			{
				SWindow *p = SWindowMgr::GetWindow(swnd);
				CRect rc = p->GetWindowRect();
				m_pHover->Move(rc);
			}else
			{
				m_pHover->Move(0,0,0,0);
			}
		}

		return TRUE;
	}
}

void CPreviewHost::SelectCtrlByTag(SStringA strTag)
{
	SStringAList strPair;
	int level_deep = SplitString(strTag, ',', strPair);
	
	SWindow *pRoot = this->GetRoot();
	SWindow *pChild = pRoot;
	int i = 0;
	if (m_bSOUIWnd)
	{
		i = 2;
	}
	else if (!m_bVirtualRoot)
	{
		i = 1;
	}
	for (; i<level_deep; i++)
	{
		int child_index = atoi(strPair.GetAt(i));
		pChild = pChild->GetChild(child_index+1);
		if (!pChild)
			break;

		if (i == level_deep - 1)
		{
			CRect rcSel = pChild->GetWindowRect();
			m_pSel->Move(rcSel);
			m_pSel->SetVisible(TRUE);
		}
	}	
}

void CPreviewHost::GetSwndIndex(SWindow *pWnd,SList<int> &lstIndex)
{
	int idx = 0;
	SWindow *pPrev = pWnd->GetWindow(GSW_PREVSIBLING);
	while(pPrev)
	{
		idx++;
		pPrev = pPrev->GetWindow(GSW_PREVSIBLING);
	}
	lstIndex.AddHead(idx);
	SWindow *pParent = pWnd->GetParent();
	if(pParent)
	{
		GetSwndIndex(pParent,lstIndex);
	}
}

BOOL CPreviewHost::OnLoadLayoutFromResourceID(const SStringT & resId)
{
	if(resId.IsEmpty())
		return FALSE;
	pugi::xml_document xmlDoc;
	BOOL bLoaded=FALSE;
	if(!m_utf8Buffer.IsEmpty())
	{
		bLoaded = xmlDoc.load_buffer(m_utf8Buffer.c_str(),m_utf8Buffer.GetLength(),pugi::parse_default,pugi::encoding_utf8);
	}else
	{
		bLoaded =LOADXML(xmlDoc,m_strXmlLayout);
	}
	if(bLoaded)
	{
		pugi::xml_node xmlSoui = xmlDoc.child(L"SOUI");
		if(xmlSoui)
		{
			m_bSOUIWnd = TRUE;
			xmlSoui.attribute(L"translucent").set_value(0);
			xmlSoui.attribute(L"wndType").set_value(L"normal");
			return InitFromXml(xmlDoc.child(L"SOUI"));
		}
		else
		{//include element.
			const WCHAR ksz_include_host[] = L"<soui  margin=\"2,2,2,2\" resizable=\"1\">"\
				L"<root width=\"800\" height=\"600\" colorBkgnd=\"#888888\"/>"\
				L"</soui>";
			pugi::xml_document xmlDoc2;
			xmlDoc2.load_buffer(ksz_include_host,sizeof(ksz_include_host),pugi::parse_default,pugi::encoding_wchar);
			pugi::xml_node xmlInclude = xmlDoc.child(L"include");
			if (xmlInclude)
			{
				int inc_width = xmlInclude.attribute(_T("width")).as_int();
				int inc_height = xmlInclude.attribute(_T("height")).as_int();
				pugi::xml_node doc_root = xmlDoc2.child(_T("soui")).child(_T("root"));
				doc_root.attribute(_T("width")).set_value(inc_width);
				doc_root.attribute(_T("height")).set_value(inc_height);
				
				InitFromXml(xmlDoc2.first_child());
				CreateChildren(xmlInclude);
				return TRUE;
			}else
			{
				m_bVirtualRoot = TRUE;
				pugi::xml_node xmlRoot2=xmlDoc2.first_child().first_child();
				xmlRoot2.append_copy(xmlDoc.first_child());
				return InitFromXml(xmlDoc2.first_child());
			}
		}
	}else
	{
		SASSERT_FMTA(FALSE,"Load layout [%s] Failed",S_CT2A(m_strXmlLayout));
		return FALSE;
	}
}

BOOL CPreviewHost::OnCopyData(HWND wnd, PCOPYDATASTRUCT pCopyDataStruct)
{
	if(pCopyDataStruct->dwData == update_buf_id)
	{
		m_utf8Buffer = SStringA((const char*)pCopyDataStruct->lpData,pCopyDataStruct->cbData);
		Reload();
	}else if(pCopyDataStruct->dwData == update_file_id)
	{
		Reload();
	}
	else if (pCopyDataStruct->dwData == exitviewer_id)
	{
		m_pListener->OnEditorExit();
	}
	else if (pCopyDataStruct->dwData == selctrl_id)
	{
		SStringA strTag = SStringA((const char*)pCopyDataStruct->lpData, pCopyDataStruct->cbData);
		SelectCtrlByTag(strTag);
	}else if(pCopyDataStruct->dwData == setviewerpos_id)
	{
		POINT * pt = (POINT*)pCopyDataStruct->lpData;
		m_pListener->OnRePos(pt);
	}
	return TRUE;
}

int CPreviewHost::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	int nRet = SHostWnd::OnCreate(lpCreateStruct);
	if(nRet==0)
	{
		const WCHAR *pszXmlSel=L"<window float=\"1\" msgTransparent=\"1\" margin=\"2,2,2,2\" colorBorder=\"#ff0000\"/>";
		m_pSel = CreateChildren(pszXmlSel);

		const WCHAR *pszXml=L"<window float=\"1\" msgTransparent=\"1\" margin=\"1,1,1,1\" colorBorder=\"#0000ff\"/>";
		m_pHover = CreateChildren(pszXml);
	}

	if(!m_hOwner)
	{
		m_hOwner = FindWindowEx(NULL,NULL,ksz_editor_cls,ksz_editor_wnd);
	}
	if(!m_hOwner)
	{
		m_hOwner = FindWindowEx(NULL,NULL,ksz_uipreview_cls,ksz_uipreview_wnd);
	}
	if (m_hOwner)
	{
		g_dwLastMsgTime = GetTickCount();
		::PostMessage(m_hOwner, kmsg_viewer_create, (WPARAM)m_hWnd, 0);
		::timeSetEvent(1000, 1, cb_HeartTimer, 0, TIME_PERIODIC);
	}
	return nRet;
}

void CPreviewHost::OnSwndInitFinish(EventArgs *e)
{
	SWindow *pRet = sobj_cast<SWindow>(e->sender);
	if(pRet)
	{
		if (pRet->IsClass(SMCListView::GetClassName()))
		{
			CBaseMcAdapterFix *mcAdapter = new CBaseMcAdapterFix();
			((SMCListView*)pRet)->SetAdapter(mcAdapter);
			mcAdapter->Release();
		}
		if (pRet->IsClass(SListView::GetClassName()))
		{
			CBaseAdapterFix *listAdapter = new CBaseAdapterFix();
			((SListView*)pRet)->SetAdapter(listAdapter);
			listAdapter->Release();
		}
		if (pRet->IsClass(STileView::GetClassName()))
		{
			CBaseAdapterFix *listAdapter = new CBaseAdapterFix();
			((STileView*)pRet)->SetAdapter(listAdapter);
			listAdapter->Release();
		}
		if(pRet->IsClass(STreeView::GetClassName()))
		{//todo:

		}
	}
}

void CPreviewHost::OnSize(UINT nType, CSize size)
{
	SHostWnd::OnSize(nType,size);
}

void CPreviewHost::OnExitSizeMove()
{
	m_pListener->OnResize();
}

LRESULT CPreviewHost::OnHeart(UINT uMsg, WPARAM wp, LPARAM lp)
{
	g_dwLastMsgTime = GetTickCount();
	return 0;
}

void CPreviewHost::OnDestroy()
{
	if(m_hOwner)
	{
		
	}
	SetMsgHandled(FALSE);
}

void CPreviewHost::Reload()
{
	CRect rcWnd = GetWindowRect();
	OnDestroy();
	CREATESTRUCT cs;
	cs.cx = 0;
	cs.cy = 0;
	OnCreate(&cs);

	OnSize(0,rcWnd.Size());
	//todo: keep tab visible pages same as before.

}
