#include "stdafx.h"
#include "XmlEditor.h"
#include "core/SWnd.h"
#include "MainDlg.h"
#include "Global.h"
#include "SysdataMgr.h"
#include <helper/SAppDir.h>
#include <control/SRealWnd.h>

//编辑界面时XML窗口只显示选择控件的XML文本
//#define  ONLYSHOWSELXML

#define  MARGIN 20

namespace SOUI{


CXmlEditor::CXmlEditor(CMainDlg *pMainDlg)
{
	m_pMainDlg = pMainDlg;
}


CXmlEditor::~CXmlEditor()
{

}

BOOL CXmlEditor::CloseProject()
{
	m_strXmlFile =
	m_strProPath = SStringT();

	m_pScintillaWnd->SendEditor(SCI_CLEARALL);
	m_pScintillaWnd->SetDirty(false);
	m_treeXmlStruct->RemoveAllItems();

	return TRUE;
}

void CXmlEditor::StartPreviewProcess()
{
	SAppDir appdir(NULL);
	SStringT binDir = appdir.AppDir();
#ifdef _DEBUG
	SStringT strPreviewExePath = binDir + _T("\\uiviewerd.exe");
#else
	SStringT strPreviewExePath = binDir + _T("\\uiviewer.exe");
#endif

	TCHAR buffer[32] = { 0 };
	SStringT strCommandLine = strPreviewExePath + _T(" ");
	strCommandLine += _T("\"");
	strCommandLine += m_strProPath;
	strCommandLine += _T("\" ");
	strCommandLine += m_strLayoutName;
	strCommandLine += _T(" ");
	strCommandLine += _ltot((long)m_pMainDlg->m_hWnd, buffer, 10);
	strCommandLine += _T(" ");
	strCommandLine += _ltot((long)m_pViewerHost->GetRealHwnd(), buffer, 10);
	
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	::ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(STARTUPINFO);
	si.wShowWindow = SW_SHOWNORMAL;
	si.dwFlags = STARTF_USESHOWWINDOW;

	if (!CreateProcess(NULL, (LPTSTR)strCommandLine.c_str(), 
		NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
	{
		int err = GetLastError();
		return;
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}
	
BOOL CXmlEditor::LoadXml(SStringT strFileName, SStringT layoutName)
{
	m_pMainDlg->SendMsgToViewer(exitviewer_id, NULL, 0);

	m_bChanged = FALSE;
	m_strXmlFile = strFileName;
	m_strLayoutName = layoutName;
	m_pScintillaWnd->SendMessage(SCI_SETREADONLY, 0, 0);
	SStringT strPath = m_strProPath + _T("\\") + strFileName;
	m_pScintillaWnd->OpenFile(strPath);

	if(!layoutName.IsEmpty())
	{
		m_xml_editor->ShowPane(0);
		StartPreviewProcess();
	}else
	{
		m_xml_editor->HidePane(0);
	}

	return TRUE;
}

void CXmlEditor::ReloadLayout()
{
	SStringA strUtf8=m_pScintillaWnd->GetWindowText();
	bool bXml=m_xmlParser.loadUtf8(strUtf8,strUtf8.GetLength());
	m_treeXmlStruct->RemoveAllItems();

	spugi::xml_node root = m_xmlParser.first_child();
	UpdateXmlStruct(root, STVI_ROOT);
	m_treeXmlStruct->Invalidate();
	if(bXml)
	{
		if(m_strLayoutName.StartsWith(L"layout:",true))
			m_pMainDlg->SendMsgToViewer(update_buf_id,strUtf8.c_str(),strUtf8.GetLength());
	}
	else
		SLOG_INFO("error xml");
}


//保存当前打开的布局文件
bool CXmlEditor::SaveFile()
{
	if (m_strXmlFile.IsEmpty())
	{
		return false;
	}
	return m_pScintillaWnd->SaveFile(m_strProPath + _T("\\")+ m_strXmlFile);
}

bool CXmlEditor::UpdateXmlStruct(spugi::xml_node xmlNode, HSTREEITEM item,int iSib)
{
	if (!xmlNode)
	{
		return false;
	}
	int count = 0;

	if(xmlNode.type() != spugi::node_element)
		return false;
	HSTREEITEM itemNew = m_treeXmlStruct->InsertItem(S_CA2T(xmlNode.name()), item);
	m_treeXmlStruct->SetItemData(itemNew,iSib);

	int iChild = 0;
	spugi::xml_node nodeChild = xmlNode.first_child();
	while (nodeChild)
	{
		bool bOK = UpdateXmlStruct(nodeChild,itemNew,iChild);
		if(bOK) iChild++;

		nodeChild = nodeChild.next_sibling();
	}
	return true;
}

// 响应窗口结构中点击选中界面元素
void CXmlEditor::OnTCSelChanged(EventArgs *pEvt)
{
	EventTCSelChanged *evt = (EventTCSelChanged*)pEvt;
	HSTREEITEM item = evt->hNewSel;
	int nLevel = 0;
	while(item)
	{
		nLevel++;
		item = m_treeXmlStruct->GetParentItem(item);
	}
	
	int *idx = new int[nLevel];
	item = evt->hNewSel;
	int iLevel = nLevel-1;
	while(item)
	{
		int iSib = m_treeXmlStruct->GetItemData(item);
		idx[iLevel] = iSib;
		item = m_treeXmlStruct->GetParentItem(item);
		iLevel--;
	}
	SelectCtrlByOrder(idx,nLevel);
	delete []idx;
}


void CXmlEditor::SelectCtrlByOrder(int *pOrder,int nLen)
{
	SStringA str;
	for(int i=0;i<nLen;i++)
	{
		str += SStringA().Format("%d,",pOrder[i]);
	}
	SLOG_INFO("order:"<<str);
	NodeRange range = m_xmlParser.getNodePos(pOrder,nLen);
	if(range.begin!=-1)
	{
		int begin = range.begin;
		int end = range._break!=-1?range._break:range.end;
		if(end != -1)
		{
			m_pScintillaWnd->SetSel(begin,end);
		}
	}
	m_pMainDlg->SendMsgToViewer(selctrl_id,pOrder,nLen*sizeof(int));
}

void CXmlEditor::OnCommand(UINT uNotifyCode, int nID, HWND wndCtl)
{
	if(wndCtl == m_pScintillaWnd->m_hWnd && uNotifyCode == EN_CHANGE)
	{
		if(!m_bChanged)
		{
			m_bChanged = TRUE;
			::SetTimer(m_pMainDlg->m_hWnd,300,200,NULL);
		}
	}else
	{
		SetMsgHandled(FALSE);
	}
}

void CXmlEditor::Init(SWindow * pRoot,CScintillaWnd::IListener *pListener)
{
	m_treeXmlStruct = pRoot->FindChildByName2<STreeCtrl>(L"uidesigner_wnd_xmltree");
	m_xml_editor = pRoot->FindChildByName2<SSplitWnd>("xml_editor");
	m_pViewerHost = pRoot->FindChildByName2<SRealWnd>(L"uidesigner_wnd_layout");
	m_pViewerHost->GetRealHwnd();
	SRealWnd *pRealWnd = pRoot->FindChildByName2<SRealWnd>(L"uidesigner_scintilla");
	m_pScintillaWnd = (CScintillaWnd*)pRealWnd->GetUserData();
	if (m_pScintillaWnd)
	{
		m_pScintillaWnd->SendEditor(SCI_SETMODEVENTMASK,SC_MOD_INSERTTEXT|SC_MOD_DELETETEXT);
		m_pScintillaWnd->SetListener(pListener);
	}
}


BOOL CXmlEditor::OnCopyData(HWND wnd, PCOPYDATASTRUCT pCopyDataStruct)
{
	if (pCopyDataStruct->dwData == select_id)
	{
		bool bRootLayout = m_xmlParser.isRootLayout();
		int  nCount = pCopyDataStruct->cbData / sizeof(int);
		int *pOrder = new int[nCount+1];
		pOrder[0]=0;
		memcpy(pOrder+1,pCopyDataStruct->lpData,pCopyDataStruct->cbData);
		if(bRootLayout)
			SelectCtrlByOrder(pOrder,nCount+1);
		else
			SelectCtrlByOrder(pOrder+1,nCount);
		delete []pOrder;
	}

	return TRUE;
}

bool CXmlEditor::isDirty() const
{
	return m_pScintillaWnd->isDirty();
}

void CXmlEditor::SetProjectPath(const SStringT & strProjPath)
{
	m_strProPath = strProjPath;
	TCHAR *s = m_strProPath.GetBuffer(m_strProPath.GetLength());
	PathRemoveFileSpec(s);
	m_strProPath.ReleaseBuffer();
}

void CXmlEditor::InsertText(SStringA strText)
{
	m_pScintillaWnd->ReplaseSel(strText.c_str());
}

void CXmlEditor::OnTimer(UINT_PTR id)
{
	if(id == 300)
	{
		m_bChanged = FALSE;
		ReloadLayout();
		::KillTimer(m_pMainDlg->m_hWnd,id);
	}else
	{
		SetMsgHandled(FALSE);
	}
}

}
