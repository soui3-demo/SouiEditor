// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"
#include "helpapi.h"
#include "CDebug.h"
#include "Dialog/DlgNewLayout.h"
#include "Dialog/DlgAbout.h"
#include "SysdataMgr.h"
#include <vector>
#include <algorithm>
#include <helper/SMenu.h>
#include <helper/SMenuEx.h>
#include "SImgCanvas.h"
#include "Global.h"
#include "Dialog/DlgResMgr.h"
#include "Dialog/DlgInsertXmlElement.h"

extern SStringT g_CurDir;

#define TIMERID_HEART		   101

#define UIRES_FILE	L"uires.idx"
//////////////////////////////////////////////////////////////////////////
CMainDlg* g_pMainDlg = NULL;
CSysDataMgr g_SysDataMgr;
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CMainDlg::CMainDlg() : SHostWnd(_T("LAYOUT:XML_MAINWND")),m_pDesigner(NULL),m_bAutoSave(TRUE)
{
	m_hViewer = NULL;
}

CMainDlg::~CMainDlg()
{
}

BOOL CMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	g_pMainDlg = this;
	m_strOrigTitle = GETSTRING(R.string.title);

	m_bIsOpen = FALSE;

	FindChildByID(R.id.chk_autosave)->SetCheck(m_bAutoSave);
	m_treePro = FindChildByName2<STreeCtrl>(L"workspace_tree");
	m_treePro->SetListener(this);
	m_lbWorkSpaceXml = FindChildByName2<SListBox>(L"workspace_xmlfile_lb");
	m_staticAppTitle = FindChildByName2<SStatic>(L"apptitle");
	m_btn_recentFile = FindChildByName2<SButton>(L"toolbar_btn_recent");

	m_textNodenum = FindChildByName2<SStatic>(L"uidesigner_nodenum");
	m_textCtrlTypename = FindChildByName2<SStatic>(L"uidesigner_CtrlTypename");
	
	m_edtDesc = FindChildByName2<SRichEdit>(L"uidesigner_DescText");

	m_tabWorkspace = FindChildByName2<STabCtrl>(L"workspace_tab");

	m_lvSkin = FindChildByName2<SListView>(L"lv_tb_skin");
	m_lvWidget = FindChildByName2<SListView>(L"lv_tb_widget");

	m_RecentFileMenu.LoadMenu(UIRES.smenu.menu_recent);
	//======================================================================
	m_pDesigner = new CXmlEditor(this);
	m_pDesigner->Init(this,this);

	SStringT strCfgDir = g_CurDir + "Config";
	SApplication::getSingleton().SetFilePrefix(strCfgDir);
	g_SysDataMgr.LoadSysData(strCfgDir);

	{//init control listbox
		CWidgetTBAdapter * pAdapter = new CWidgetTBAdapter(g_SysDataMgr.getCtrlDefNode(),this);
		m_lvWidget->SetAdapter(pAdapter);
		pAdapter->Release();
	}
	{//init skin listbox
		CSkinTBAdapter * pAdapter = new CSkinTBAdapter(g_SysDataMgr.getSkinPropNode(),this);
		m_lvSkin->SetAdapter(pAdapter);
		pAdapter->Release();
	}


	HRESULT hr = ::RegisterDragDrop(m_hWnd, GetDropTarget());
	RegisterDragDrop(m_treePro->GetSwnd(), new CDropTarget(this));
	SWindow* MainWnd = FindChildByName2<SWindow>("UI_main_caption");
	if (MainWnd)
		RegisterDragDrop(MainWnd->GetSwnd(), new CDropTarget(this));

	LoadAppCfg();


	return 0;
}

void CMainDlg::LoadAppCfg()
{
	pugi::xml_document xmldoc;
	pugi::xml_parse_result result = xmldoc.load_file(g_CurDir + L"sEditor.cfg");
	if (result)
	{
		m_vecRecentFile.clear();
		pugi::xml_node rootNode = xmldoc.child(L"root");
		if (!rootNode)
			return;

		m_bAutoSave = rootNode.attribute(L"autoSave").as_bool(true);

		pugi::xml_node recentNode = rootNode.child(L"recent_file");
		pugi::xml_node recentItem = recentNode.first_child();
		while (recentItem)
		{
			SStringT filepath = recentItem.attribute(L"path").as_string();
			m_vecRecentFile.push_back(filepath);

			recentItem = recentItem.next_sibling();
		}
	}

	do 
	{
		;
	} while (m_RecentFileMenu.DeleteMenu(0, MF_BYCOMMAND | MF_BYPOSITION));
	
	UINT MenuId = MenuId_Start;
	for (std::vector<SStringT>::iterator it = m_vecRecentFile.begin();it != m_vecRecentFile.end();it++)
	{
		m_RecentFileMenu.InsertMenu(0, MF_BYCOMMAND|MF_BYPOSITION, MenuId++, (*it) + L"  ");
	}
}

void CMainDlg::SaveAppCfg()
{
	pugi::xml_document xmlDoc;
	pugi::xml_node nodeRoot = xmlDoc.append_child(L"root");
	nodeRoot.append_attribute(L"autoSave").set_value(m_bAutoSave);
	// 声明
	pugi::xml_node pre = xmlDoc.prepend_child(pugi::node_declaration);
	pre.append_attribute(L"version") = "1.0";
	pre.append_attribute(L"encoding") = "utf-8";

	// 注释节点1
	pugi::xml_node nodeCommentStudents = nodeRoot.append_child(pugi::node_comment);
	nodeCommentStudents.set_value(L"SouiEditor Config");
	// 普通节点1
	pugi::xml_node nodeRecents = nodeRoot.append_child(L"recent_file");
	for (std::vector<SStringT>::iterator it = m_vecRecentFile.begin();it != m_vecRecentFile.end();it++)
	{
		pugi::xml_node nodeRecent = nodeRecents.append_child(L"item");
		nodeRecent.append_attribute(L"path").set_value(*it);
	}

	xmlDoc.save_file(g_CurDir + L"sEditor.cfg");
}

bool CMainDlg::OnTreeproContextMenu(CPoint pt)
{
	CPoint pt2 = pt;
	ClientToScreen(&pt2);

	HSTREEITEM Item = m_treePro->HitTest(pt);
	if (!Item) return false;

	SMenuEx menu;
	menu.LoadMenu(UIRES.smenu.menu_layoutfile);
	int cmd = menu.TrackPopupMenu(TPM_RETURNCMD, pt2.x, pt2.y, m_hWnd);

	return false;
}

void CMainDlg::OnClose()
{
	if (m_bIsOpen)
	{
		CloseProject();
	}

	SNativeWnd::DestroyWindow();
}

void CMainDlg::OnMaximize()
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
}
void CMainDlg::OnRestore()
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE);
}
void CMainDlg::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void CMainDlg::OnSize(UINT nType, CSize size)
{
	SetMsgHandled(FALSE);
	
	SWindow *pBtnMax = FindChildByName(L"btn_max");
	SWindow *pBtnRestore = FindChildByName(L"btn_restore");
	if(!pBtnMax || !pBtnRestore) return;
	
	if (nType == SIZE_MAXIMIZED)
	{
		pBtnRestore->SetVisible(TRUE);
		pBtnMax->SetVisible(FALSE);
	}
	else if (nType == SIZE_RESTORED)
	{
		pBtnRestore->SetVisible(FALSE);
		pBtnMax->SetVisible(TRUE);
	}
}

void CMainDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	__super::OnShowWindow(bShow, nStatus);
	if (bShow && !m_cmdWorkspaceFile.IsEmpty())
	{
		OpenProject(m_cmdWorkspaceFile);
		m_bIsOpen = TRUE;
		m_cmdWorkspaceFile.Empty();
	}
}

void CMainDlg::OnTimer(UINT_PTR timeID)
{
	if (timeID == TIMERID_HEART)
	{
		::SendMessage(m_hViewer, kmsg_heart, (WPARAM)0, (LPARAM)0);
	}
	else
	{
		SetMsgHandled(FALSE);
	}
}

void CMainDlg::OnCommand(UINT uNotifyCode, int nID, HWND wndCtl)
{
	if (uNotifyCode == 0)
	{
		if (nID >= MenuId_Start && nID <= (MenuId_Start + m_vecRecentFile.size()))
		{
			if (m_bIsOpen)
			{
				CDebug::Debug(_T("请先关闭当前工程"));
				return;
			}
			OpenProject(m_vecRecentFile[nID - MenuId_Start]);
		}
	}
}


void CMainDlg::OnBtnOpen()
{
	if (m_bIsOpen)
	{
		CDebug::Debug(_T("请关闭当前工程后再打开新的"));
		return;
	}
	CFileDialogEx OpenDlg(TRUE, NULL, _T("idx"), _T("uires.idx"), 6, _T("soui skin index(*.idx)\0*.idx\0All files (*.*)\0*.*\0\0"));
	if (IDOK == OpenDlg.DoModal())
	{
		OpenProject(OpenDlg.m_szFileName);
	}
}

void CMainDlg::OnBtnClose()
{
	if (!m_bIsOpen || m_strProPath.IsEmpty())
		return;

	CloseProject();

	LoadAppCfg();
}


void CMainDlg::CloseProject()
{
	m_xmlDocUiRes.reset();
	m_pDesigner->CloseProject();
	m_treePro->RemoveAllItems();
	m_lbWorkSpaceXml->DeleteAll();
	m_UIResFileMgr.ReleaseUIRes();	

	m_lvSkin->SetVisible(FALSE,TRUE);
	m_lvWidget->SetVisible(FALSE,TRUE);

	SendMsgToViewer(exitviewer_id, NULL, 0);
	m_bIsOpen = FALSE;
	m_staticAppTitle->SetWindowText(m_strOrigTitle);
}

//打开工程
void CMainDlg::OpenProject(SStringT strFileName)
{
	m_treePro->RemoveAllItems();
	m_lbWorkSpaceXml->DeleteAll();
	m_xmlDocUiRes.reset();
	
	SStringT strFile = strFileName;
	int n = strFileName.ReverseFind(_T('\\'));
	m_strUiresPath = strFileName;
	m_strProPath = strFileName.Mid(0, n);
	
	if (!FileIsExist(strFileName))
	{
		SStringT notestr;
		notestr.Format(_T("未能打开 %s, \n是否从最近列表中移除对它的引用?"), strFileName);
		if (SMessageBox(NULL, notestr, _T("提示"), MB_YESNO) == IDYES)
		{
			std::vector<SStringT>::iterator it = std::find(m_vecRecentFile.begin(), m_vecRecentFile.end(), strFileName);
			if (it != m_vecRecentFile.end())
			{				
				m_vecRecentFile.erase(it);
				SaveAppCfg();
				LoadAppCfg();
			}
		}
		
		return;
	}

	if(!m_xmlDocUiRes.load_file(strFileName, pugi::parse_full))
	{
		CDebug::Debug(_T("open project file failed"));
		return;
	}
	m_strUIResFile = strFileName;
	m_pDesigner->SetProjectPath(strFileName);


	pugi::xml_node xmlType = m_xmlDocUiRes.child(L"resource").first_child();
	while(xmlType)
	{
		SStringT nodeName = S_CW2T(xmlType.name());
		if(xmlType.attribute(L"layout").as_bool(false) 
			|| nodeName.CompareNoCase(_T("layout"))==0)
		{//layout node
			pugi::xml_node xmlFile = xmlType.first_child();
			while(xmlFile)
			{
				SStringT strName = S_CW2T(xmlFile.attribute(L"name").value());
				SStringT *strPath = new SStringT(S_CW2T(xmlFile.attribute(L"path").value()));
				HSTREEITEM item = m_treePro->InsertItem(strName);  //strName = "xml_mainwnd"
				m_treePro->SetItemText(item, nodeName + _T(":") + strName);
				m_treePro->SetItemData(item, (LPARAM)strPath);  //strpath = "xml\dlg_maing.xml"
				xmlFile = xmlFile.next_sibling();
			}
		}
		xmlType = xmlType.next_sibling();
	}

	m_UIResFileMgr.LoadUIResFromFile(m_strUiresPath);

	std::vector<SStringT> vecTemp;
	SPOSITION pos = m_UIResFileMgr.m_mapXmlFile.GetStartPosition();
	while (pos)
	{
		const SMap<SStringT, SStringT>::CPair* item = m_UIResFileMgr.m_mapXmlFile.GetAt(pos);
		SStringT strLayoutName = _T("LAYOUT");
		strLayoutName.MakeLower();
		if (item->m_key.Find(strLayoutName + L":") == -1)
		{
			vecTemp.push_back(item->m_key);
		}
		m_UIResFileMgr.m_mapXmlFile.GetNext(pos);
	}

	m_lbWorkSpaceXml->AddString(UIRES_FILE);
	std::sort(vecTemp.begin(), vecTemp.end(), SortSStringNoCase);
	std::vector<SStringT>::iterator it = vecTemp.begin();
	for (; it != vecTemp.end(); it++)
	{
		m_lbWorkSpaceXml->AddString(*it);
	}


	m_bIsOpen = TRUE;

	if (std::find(m_vecRecentFile.begin(), m_vecRecentFile.end(), strFileName) == m_vecRecentFile.end())
	{
		m_vecRecentFile.push_back(strFileName);
		if (m_vecRecentFile.size() > 10)
			m_vecRecentFile.erase(m_vecRecentFile.begin());
		SaveAppCfg();
	}
}

void CMainDlg::ReloadWorkspaceUIRes()
{
	if (!m_bIsOpen)
		return;

	m_UIResFileMgr.LoadUIResFromFile(m_strUiresPath);
}

void CMainDlg::OnBtnSave()
{
	m_pDesigner->SaveFile();
}

void CMainDlg::OnBtnNewLayout()
{
	if (m_strProPath.IsEmpty())
	{
		CDebug::Debug(_T("请先打开工程"));
		return;
	}
	SDlgNewLayout DlgNewDialog(_T("layout:UIDESIGNER_XML_NEW_LAYOUT"), m_strProPath);
	if (IDOK == DlgNewDialog.DoModal(m_hWnd))
	{
		CopyFile(CSysDataMgr::getSingleton().GetConfigDir() + ("\\LayoutTmpl\\Dialog.xml"), DlgNewDialog.m_strPath, FALSE);
		NewLayout(DlgNewDialog.m_strName, DlgNewDialog.m_strPath);

		SStringT *strShortPath = new SStringT(DlgNewDialog.m_strPath.Mid(m_strProPath.GetLength() + 1));

		//将文件名插入工程列表
		HSTREEITEM item = m_treePro->InsertItem(DlgNewDialog.m_strName);  //strName = "xml_mainwnd"
		m_treePro->SetItemText(item, DlgNewDialog.m_strName);
		m_treePro->SetItemData(item, (LPARAM)strShortPath);  //strpath = "xml\dlg_maing.xml"
		m_treePro->Invalidate();
	}
}

void CMainDlg::OnBtnNewInclude()
{
	if (m_strProPath.IsEmpty())
	{
		CDebug::Debug(_T("请先打开工程"));
		return;
	}
	SDlgNewLayout DlgNewDialog(_T("layout:UIDESIGNER_XML_NEW_LAYOUT"), m_strProPath);
	if (IDOK == DlgNewDialog.DoModal(m_hWnd))
	{
		CopyFile(CSysDataMgr::getSingleton().GetConfigDir() + _T("\\LayoutTmpl\\Include.xml"), DlgNewDialog.m_strPath, FALSE);
		NewLayout(DlgNewDialog.m_strName, DlgNewDialog.m_strPath);

		SStringT *strShortPath = new SStringT(DlgNewDialog.m_strPath.Mid(m_strProPath.GetLength() + 1));

		//将文件名插入工程列表
		HSTREEITEM item = m_treePro->InsertItem(DlgNewDialog.m_strName);  //strName = "xml_mainwnd"
		m_treePro->SetItemText(item, DlgNewDialog.m_strName);
		m_treePro->SetItemData(item, (LPARAM)strShortPath);  //strpath = "xml\dlg_maing.xml"
		m_treePro->Invalidate();

	}
}


void CMainDlg::OnBtnResMgr()
{
	if (m_strProPath.IsEmpty())
	{
		CDebug::Debug(_T("请先打开工程"));
		return;
	}

	if (m_strUIResFile.IsEmpty())
	{
		return;
	}
	SResMgrDlg DlgSkin(m_strUIResFile, &m_UIResFileMgr);
	if (DlgSkin.DoModal(m_hWnd) == IDOK)
	{
		ReloadWorkspaceUIRes();
		RefreshRes();
	}
}

void CMainDlg::OnBtnAbout()
{
	SDlgAbout dlg;
	dlg.DoModal(m_hWnd);
}

void CMainDlg::OnBtnRecentFile()
{
	CRect rect = m_btn_recentFile->GetWindowRect();
	ClientToScreen(&rect);

	m_RecentFileMenu.TrackPopupMenu(0, rect.left, rect.bottom, m_hWnd);
}

void CMainDlg::OnTreeItemDbClick(EventArgs *pEvtBase)
{
	EventTCDbClick *pEvt = (EventTCDbClick*)pEvtBase;
	STreeCtrl *tree = (STreeCtrl*)pEvt->sender;

	SStringT *s = (SStringT*)tree->GetItemData(pEvt->hItem);
	SStringT strLayoutName;
	tree->GetItemText(pEvt->hItem, strLayoutName);

	if(m_pDesigner->isDirty())
	{
		if(m_bAutoSave) 
			OnBtnSave();
		else
		{
			if(SMessageBox(m_hWnd,_T("当前文件还没有保存，现在保存吗？"),_T("提示"),MB_OKCANCEL)==IDOK)
			{
				OnBtnSave();
			}else
			{
				return;
			}
		}
	}
	m_pDesigner->LoadXml(*s, strLayoutName);
	m_lvWidget->SetVisible(TRUE,TRUE);
	m_lvSkin->SetVisible(FALSE,TRUE);
}


// 双击打开文件
void CMainDlg::OnWorkspaceXMLDbClick(EventArgs * pEvtBase)
{
	EventLBDbClick *pEvt = (EventLBDbClick*)pEvtBase;
	SListBox *listbox = (SListBox*)pEvt->sender;

	if (pEvt->nCurSel != -1)
	{
		SStringT strText;
		strText = listbox->GetText(pEvt->nCurSel);

		SStringT filename;
		//查找此XML对应的文件
		SMap<SStringT, SStringT>::CPair *p = m_UIResFileMgr.m_mapXmlFile.Lookup(strText);  //查找
		if (p == NULL)
		{
			filename = UIRES_FILE;
		}
		else
			filename += p->m_value;

		if(m_pDesigner->isDirty())
		{
			if(m_bAutoSave) 
				OnBtnSave();
			else
			{
				if(SMessageBox(m_hWnd,_T("当前文件还没有保存，现在保存吗？"),_T("提示"),MB_OKCANCEL)==IDOK)
				{
					OnBtnSave();
				}else
				{
					return;
				}
			}
		}
		m_pDesigner->LoadXml(filename,SStringT());

		m_lvWidget->SetVisible(FALSE,TRUE);
		BOOL bSkin = filename.EndsWith(_T("skin.xml"));
		m_lvSkin->SetVisible(bSkin,TRUE);

	}
}


void CMainDlg::SendMsgToViewer(int msgid, const void* pMsgData, int msglen)
{
	if (m_hViewer)
	{
		char* pData = NULL;
		COPYDATASTRUCT cds;
		cds.dwData = msgid;
		cds.cbData = msglen;
		cds.lpData = pData;
		if (msglen > 0)
		{
			char* pData = new char[msglen];
			memcpy(pData, pMsgData, msglen);
			cds.lpData = pData;
		}
		::SendMessage(m_hViewer, WM_COPYDATA, (WPARAM)m_hWnd, (LPARAM)&cds);
		if (pData)
			delete[] pData;
	}
}


LRESULT CMainDlg::OnCreateViewer(UINT uMsg,WPARAM wp,LPARAM lp)
{
	m_hViewer = (HWND)wp;
	if (m_hViewer)
	{
		SNativeWnd::SetTimer(TIMERID_HEART, 1000);
	}else
	{
		SNativeWnd::KillTimer(TIMERID_HEART);
	}
	return 0;
}

BOOL CMainDlg::NewLayout(SStringT strResName, SStringT strPath)
{
	SStringT strShortPath = strPath.Mid(m_strProPath.GetLength() + 1);

	pugi::xml_node xmlNode = m_xmlDocUiRes.child(_T("resource")).child(_T("LAYOUT"));

	if (xmlNode)
	{
		pugi::xml_node Child = xmlNode.append_child(_T("file"));
		Child.append_attribute(_T("name")).set_value(strResName);
		Child.append_attribute(_T("path")).set_value(strShortPath);

		m_xmlDocUiRes.save_file(m_strUIResFile);
	}

	return TRUE;
}


void CMainDlg::RefreshRes()
{
	m_xmlDocUiRes.load_file(m_strUIResFile, pugi::parse_full);
}


void CMainDlg::OnDeleteItem(STreeCtrl *pTreeCtrl,HSTREEITEM hItem,LPARAM lParam)
{
	SStringT * pPath = (SStringT*)lParam;
	delete pPath;
}


void CMainDlg::onScintillaSave(LPCTSTR pszFileName)
{
	m_UIResFileMgr.onXmlSave(pszFileName);
}

BOOL CMainDlg::OnDrop(LPCTSTR pszName)
{
	SStringT strName(pszName);
	if(!strName.EndsWith(_T("idx")))
		return FALSE;
	OpenProject(pszName);
	return TRUE;
}

void CMainDlg::OnInsertWidget(CWidgetTBAdapter::IconInfo *info)
{
	m_pDesigner->InsertText(info->strContent);
}

void CMainDlg::OnInertSkin(CSkinTBAdapter::IconInfo * info)
{
	SStringT skinName = info->strTxt;

	DlgInsertXmlElement dlg(g_SysDataMgr.getSkinPropNode().child(L"skins"),skinName);
	if(IDOK==dlg.DoModal())
	{
		m_pDesigner->InsertText(dlg.GetXml());
	}
}

void CMainDlg::OnAutoCheck(EventArgs *e)
{
	EventSwndStateChanged *e2=sobj_cast<EventSwndStateChanged>(e);
	if(e2->CheckState(WndState_Check))
	{
		m_bAutoSave = e2->dwNewState&WndState_Check;
	}
}
