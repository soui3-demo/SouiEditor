#include "stdafx.h"
#include "DlgResMgr.h"
#include "CDebug.h"

#include "DlgInput.h"
#include "ResManger.h"
#include "helpapi.h"


namespace SOUI
{
	SResMgrDlg::SResMgrDlg(SStringT strPath,ResManger* pResFileManger) :SHostDialog(_T("layout:xml_res_mgr")),m_pResFileManger(pResFileManger)
	{
		m_strProPath = strPath.Mid(0, strPath.ReverseFind(_T('\\')));
	}

	BOOL SResMgrDlg::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
	{
		m_lbResType = (SListBox *)FindChildByName(L"NAME_UIDESIGNER_LB_ZYLX");
		m_lbRes = (SListBox *)FindChildByName(L"NAME_UIDESIGNER_LB_ZY");

		m_pEdit = (SEdit *)FindChildByName(L"NAME_UIDESIGNER_edit_SEARCH");

		m_imgView = (SImgCanvas *)FindChildByName(L"NAME_UIDESIGNER_IMG_VIEW");
		m_txtImageSize = (SStatic *)FindChildByName(L"sel_image_size");

		LRESULT lr = m_pEdit->SSendMessage(EM_SETEVENTMASK, 0, ENM_CHANGE);
		m_pEdit->GetEventSet()->subscribeEvent(EventRENotify::EventID, Subscriber(&SResMgrDlg::OnReNotify, this));


		m_lbResType->GetEventSet()->subscribeEvent(EVT_LB_SELCHANGED, Subscriber(&SResMgrDlg::OnLbResTypeSelChanged, this));
		m_lbRes->GetEventSet()->subscribeEvent(EVT_LB_SELCHANGED, Subscriber(&SResMgrDlg::OnLbResSelChanged, this));

		InitResType();

		return TRUE;
	}

	void SResMgrDlg::InitResType()
	{
		pugi::xml_node xmlNode = m_pResFileManger->m_xmlNodeUiRes.child(L"resource").first_child();

		while (xmlNode)
		{
			if (xmlNode.type() != pugi::node_element)
			{
				xmlNode = xmlNode.next_sibling();
				continue;
			}

			m_lbResType->AddString(xmlNode.name());

			xmlNode = xmlNode.next_sibling();
		}
	}

	bool SResMgrDlg::OnLbResTypeSelChanged(EventArgs *pEvtBase)
	{
		EventLBSelChanged *pEvt = (EventLBSelChanged*)pEvtBase;
		SListBox *listbox = (SListBox*)pEvt->sender;
		pugi::xml_node xmlNode;

		{
			SStringT strText = listbox->GetText(pEvt->nNewSel);
			m_lbRes->DeleteAll();
			xmlNode = m_pResFileManger->m_xmlNodeUiRes.child(L"resource").child(strText).first_child();
			while (xmlNode)
			{
				if (xmlNode.type() != pugi::node_element)
				{
					xmlNode = xmlNode.next_sibling();
					continue;
				}
				m_lbRes->AddString(xmlNode.attribute(L"path").value(), -1);

				xmlNode = xmlNode.next_sibling();
			}
		}

		return true;
	}

	bool SResMgrDlg::OnLbResSelChanged(EventArgs *pEvtBase)
	{
		m_imgView->Clear();
		m_txtImageSize->SetWindowText(L"");

		EventLBSelChanged *pEvt = (EventLBSelChanged*)pEvtBase;
		SListBox *listbox = (SListBox*)pEvt->sender;
		pugi::xml_node xmlNode;
		if (pEvt->nNewSel == -1)
		{
			return false;
		}
		SStringT strPath = listbox->GetText(pEvt->nNewSel);

		ShowImage(strPath);
		return true;
	}

	void SResMgrDlg::OnOK()
	{
		m_pResFileManger->SaveRes();
		EndDialog(IDOK);
	}


	void SResMgrDlg::OnNewResType()
	{
		SStringT strName;
		SDlgInput dlg;
		if (IDOK != dlg.DoModal(m_hWnd))
		{
			return;
		}

		strName = dlg.m_strValue;
		pugi::xml_node xmlNode = m_pResFileManger->m_xmlNodeUiRes.child(L"resource");
		if (xmlNode.child(strName))
		{
			CDebug::Debug(_T("该资源类型已存在！"));
			return;
		}

		xmlNode = xmlNode.append_child(strName);
		int n = m_lbResType->AddString(strName);

		m_lbResType->SetCurSel(n,TRUE);
		m_lbResType->EnsureVisible(n);
	}

	void SResMgrDlg::OnDelResType()
	{
		int iSel = m_lbResType->GetCurSel();

		if (iSel < 0)
		{
			CDebug::Debug(_T("请选择资源类型"));
			return;
		}

		if (iSel == 0)
		{
			CDebug::Debug(_T("不能选择内置皮肤类型"));
			return;
		}

		if (m_lbRes->GetCount() > 0)
		{
			CDebug::Debug(_T("请先删除资源"));
			return;
		}

		pugi::xml_node xmlNode = m_pResFileManger->m_xmlNodeUiRes.child(L"resource").child(ListBox_GetCurSelText(m_lbResType));
		if (xmlNode)
		{
			xmlNode.parent().remove_child(xmlNode);
			m_lbResType->DeleteString(iSel);
		}
	}
	
	void SResMgrDlg::OnAddResource()
	{
		if (m_lbResType->GetCurSel() < 0)
		{
			CDebug::Debug(_T("请选择资源类型"));
			return;
		}

		SStringT strResType = ListBox_GetCurSelText(m_lbResType);
		if (m_lbResType->GetCurSel() <= 3)
		{
			CDebug::Debug(_T("不能选择") + strResType + _T("类型"));
			return;
		}

		CFileDialogEx OpenDlg(TRUE, m_strProPath, NULL, NULL, 6, _T("所有文件 (*.*)\0*.*\0\0"));
		if (IDOK == OpenDlg.DoModal())
		{
			SStringT strFileName = OpenDlg.m_szFileName;
			int n = strFileName.Find(m_strProPath);
			if (n != 0)
			{
				SMessageBox(NULL, _T("请将资源放到uires目录下"), _T("提示"), MB_OK);
				return;
			}

			SStringT strFile = strFileName.Mid(m_strProPath.GetLength() + 1);

			pugi::xml_node xmlNode = m_pResFileManger->m_xmlNodeUiRes.child(L"resource").child(strResType).first_child();
			pugi::xml_node xmlNewNode;

			while (xmlNode)
			{
				if (xmlNode.type() != pugi::node_element)
				{
					xmlNode = xmlNode.next_sibling();
					continue;
				}

				if (strFile.CompareNoCase(xmlNode.attribute(L"path").value()) == 0)
				{
					break;
				}

				xmlNode = xmlNode.next_sibling();
			}

			if (!xmlNode)
			{
				// 如果该不存在该类型的资源，则添加	
				xmlNewNode = m_pResFileManger->m_xmlNodeUiRes.child(L"resource").child(strResType).append_child(L"file");

				SStringT strResName = GetFilename(strFile);
				strResName.Replace(_T("\\"), _T("_"));
				strResName.Replace(_T("."), _T("_"));

				xmlNewNode.append_attribute(L"name").set_value(strResName);
				xmlNewNode.append_attribute(L"path").set_value(strFile);

				SStringT *strData = new SStringT(strResName);
				m_lbRes->AddString(strFile, -1, (LPARAM)strData);
			}

			//定位到资源
			{
				SStringT strResText;
				for (int i = 0; i < m_lbRes->GetCount(); i++)
				{
					strResText = m_lbRes->GetText(i);
					if (strResText.Compare(strFile) != 0)
						continue;

					m_lbRes->SetCurSel(i);

					EventLBSelChanged evt(m_lbRes);
					evt.nOldSel = 0;
					evt.nNewSel = i;
					m_lbRes->FireEvent(evt);
					m_lbRes->EnsureVisible(i);

					m_lbRes->SetFocus();

					break;
				}	// end for
			}
		}
	}
	void SResMgrDlg::OnRemoveResource()
	{
		if (m_lbResType->GetCurSel() == 0)
		{
			CDebug::Debug(_T("内置资源不能删除"));
			return;
		}

		SStringT strResText = ListBox_GetCurSelText(m_lbRes);
		if (strResText.IsEmpty())
		{
			return;
		}

		SStringT strResType = ListBox_GetCurSelText(m_lbResType);
		if (strResType.IsEmpty())
		{
			return;
		}

		int nResult = SMessageBox(NULL, _T("确定要删除资源吗?"), _T("提示"), MB_OKCANCEL);
		if (nResult != 1)
		{
			return;
		}

		pugi::xml_node xmlNode = m_pResFileManger->m_xmlNodeUiRes.child(L"resource").child(strResType).first_child();
		while (xmlNode)
		{
			if (xmlNode.type() != pugi::node_element)
			{
				xmlNode = xmlNode.next_sibling();
				continue;
			}
			if (strResText.CompareNoCase(xmlNode.attribute(L"path").value()) == 0)
			{

				pugi::xml_node xmlNodeP = xmlNode.parent();
				xmlNodeP.remove_child(xmlNode);
				m_lbRes->DeleteString(m_lbRes->GetCurSel());
				return;
			}
			xmlNode = xmlNode.next_sibling();
		}

	}


	SStringT SResMgrDlg::ListBox_GetCurSelText(SListBox * lb)
	{
		SStringT s(_T(""));
		int n = lb->GetCurSel();

		if (n < 0)
		{
			return s;
		}

		return lb->GetText(n);
	}

	bool SResMgrDlg::OnReNotify(EventArgs *pEvt)
	{
		EventRENotify *pReEvt = (EventRENotify*)pEvt;
		if (pReEvt->iNotify == EN_CHANGE)
		{
			SStringT strValue = m_pEdit->GetWindowText();
			m_lbRes->DeleteAll();

			{
				pugi::xml_node  xmlNode = m_pResFileManger->m_xmlNodeUiRes.child(L"resource").child(ListBox_GetCurSelText(m_lbResType)).first_child();
				while (xmlNode)
				{
					if (xmlNode.type() != pugi::node_element)
					{
						xmlNode = xmlNode.next_sibling();
						continue;
					}
					SStringT strPath = xmlNode.attribute(L"path").value();

					if (strPath.Find(strValue) >= 0)
					{
						SStringT *strData = new SStringT(xmlNode.attribute(L"name").value());
						m_lbRes->AddString(strPath, -1, (LPARAM)strData);
					}

					xmlNode = xmlNode.next_sibling();
				}
			}

		}
		return true;
	}

	void SResMgrDlg::ShowImage(const SStringT& strImgname)
	{
		SStringT strImgPath = m_strProPath + _T("\\") + strImgname;

		m_imgView->Clear();
		m_imgView->AddFile(strImgPath);
		CSize imgSize = m_imgView->GetImgInfo();
		SStringT strSize;
		strSize.Format(L"%d * %d", imgSize.cx, imgSize.cy);
		m_txtImageSize->SetWindowText(strSize);
	}

}


