#include "stdafx.h"
#include "DlgInsertXmlElement.h"
#include "propgrid/propitem/SPropertyItem-Option.h"
#include "souidlgs.h"

namespace SOUI{
	DlgInsertXmlElement::DlgInsertXmlElement(pugi::xml_node xmlInitProp,SStringW strNodeName)
		:SHostDialog(_T("layout:UIDESIGNER_XML_INSERT_ELEMENT"))
		,m_xmlInitProp(xmlInitProp)
		,m_strNodeName(strNodeName)
	{
	}

	DlgInsertXmlElement::~DlgInsertXmlElement(void)
	{
	}

	BOOL DlgInsertXmlElement::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
	{
		m_tbProp = FindChildByID2<SToolBar>(R.id.tb_prop);
		m_propgrid = FindChildByName2<SPropertyGrid>(L"propgrid_element");
		InitPropGrid(m_strNodeName,L"");
		SRealWnd *pReal = FindChildByName2<SRealWnd>(L"xml_editor");
		pReal->GetRealHwnd();
		m_xmlEditor = (CScintillaWnd*)pReal->GetUserData();
		
		pugi::xml_node xmlContent = m_xmlInitProp.child(m_strNodeName).child(m_strNodeName);
		if(xmlContent)
		{
			pugi::xml_writer_buff buf;
			xmlContent.print(buf,L"", pugi::encoding_utf8);
			m_xmlDoc.load_buffer(buf.buffer(),buf.size()*2);
		}else
		{
			m_xmlDoc.root().append_child(S_CW2A(m_strNodeName,CP_UTF8));
			m_propgrid->EnumProp(&DlgInsertXmlElement::OnEnumPropItem,this);
		}
		spugi::xml_writer_buff buf;
		m_xmlDoc.root().print(buf);
		m_xmlEditor->InsertText(0,buf.buffer());

		m_bChanged = FALSE;
		SetTimer(100, 500);
		return TRUE;
	}

	BOOL DlgInsertXmlElement::OnEnumPropItem(IPropertyItem *pItem, void* opaque)
	{
		DlgInsertXmlElement * _this = (DlgInsertXmlElement*)opaque;
		if(pItem->HasValue() && pItem->GetType()!=PT_GROUP)
		{
			_this->OnGetValue(pItem,FALSE);
		}
		return TRUE;
	}

	void DlgInsertXmlElement::OnPropValueChanged(EventArgs *e)
	{
		EventPropGridValueChanged *e2=sobj_cast<EventPropGridValueChanged>(e);
		OnGetValue(e2->pItem,TRUE);
	}

	void DlgInsertXmlElement::OnTimer(UINT_PTR id)
	{
		if (id == 100)
		{
			if (m_bChanged)
			{
				SStringA buf = m_xmlEditor->GetWindowText();
				BOOL bValidXml = m_xmlDoc.load_buffer(buf.c_str(), buf.GetLength(), spugi::parse_default, spugi::encoding_utf8);
				m_propgrid->EnableWindow(bValidXml, TRUE);
				m_bChanged = FALSE;
			}
		}
		else
		{
			SetMsgHandled(FALSE);
		}
		
	}

	void DlgInsertXmlElement::OnGetValue(IPropertyItem *pItem,BOOL bInsertXml)
	{
		if(pItem->GetType() != PT_GROUP)
		{
			spugi::xml_node root = m_xmlDoc.root().first_child();
			SStringA name = S_CT2A(pItem->GetName2(),CP_UTF8);
			if(pItem->HasValue())
			{
				SStringA value = S_CT2A(pItem->GetValue(), CP_UTF8);
				if (pItem->GetType() == PT_OPTION)
				{
					SPropertyItemOption* pOption = ( SPropertyItemOption* )pItem;
					value = S_CT2A(pOption->Option2Value(pItem->GetValue()),CP_UTF8);
				}
				if(root.attribute(name))
					root.attribute(name).set_value(value);
				else
					root.append_attribute(name).set_value(value);
			}else
			{
				root.remove_attribute(name);
			}
		}

		if(bInsertXml)
		{
			spugi::xml_writer_buff buf;
			m_xmlDoc.root().print(buf);
			m_xmlEditor->SetSel(0,-1);
			m_xmlEditor->ReplaseSel(buf.buffer());
		}
	}

	void DlgInsertXmlElement::OnDestroy()
	{
		m_strXml = m_xmlEditor->GetWindowText();
		m_strXml += "\0x0a\0x0d";
		__super::OnDestroy();
	}

	SStringA DlgInsertXmlElement::GetXml() const
	{
		return m_strXml;
	}

	void DlgInsertXmlElement::OnCommand(UINT uNotifyCode, int nID, HWND wndCtl)
	{
		if(wndCtl == m_xmlEditor->m_hWnd && uNotifyCode == EN_CHANGE)
		{
			m_bChanged = TRUE;
		}else
		{
			SetMsgHandled(FALSE);
		}
	}

	void DlgInsertXmlElement::OnPropItemActive(EventArgs *e)
	{
		EventPropGridItemActive *e2=sobj_cast<EventPropGridItemActive>(e);
		FindChildByID(R.id.txt_prop_title)->SetWindowText(e2->pItem->GetTitle());
		FindChildByID(R.id.txt_prop_desc)->SetWindowText(e2->pItem->GetDescription());
	}

	void DlgInsertXmlElement::InitPropGrid(const SStringW & strNodeName,SStringW strParents)
	{
		pugi::xml_node xmlNode = m_xmlInitProp.child(strNodeName);
		if(xmlNode)
		{
			SStringW strParent = xmlNode.attribute(L"parent").as_string();
			if(!strParent.IsEmpty())
			{
				SStringWList lstParent;
				SplitString(strParent,L'|',lstParent);
				for(int i=0;i<lstParent.GetCount();i++)
				{
					if(strParents.Find(lstParent[i])==-1)
					{//use strParents to avoid round parent defined.
						strParents.Append(L",");
						strParents.Append(lstParent[i]);
						InitPropGrid(lstParent[i],strParents);
					}
				}
			}
		}
		m_propgrid->LoadFromXml(xmlNode.child(L"groups"));
	}

	void DlgInsertXmlElement::OnPropToolbarCmd(EventArgs *e)
	{
		EventToolBarCmd *e2=sobj_cast<EventToolBarCmd>(e);
		if(e2->nCmdId == 0)
		{
			m_propgrid->SetOrderType(SPropertyGrid::OT_GROUP);
			m_tbProp->SetItemCheck(0,TRUE);
			m_tbProp->SetItemCheck(1,FALSE);
		}else
		{
			m_propgrid->SetOrderType(SPropertyGrid::OT_NAME);
			m_tbProp->SetItemCheck(1,TRUE);
			m_tbProp->SetItemCheck(0,FALSE);
		}
	}

	void DlgInsertXmlElement::OnPropItemButtonClick(EventArgs *e)
	{
		EventPropGridItemButtonClick *e2 = sobj_cast<EventPropGridItemButtonClick>(e);
		SStringW strExType = e2->pItem->GetExtendType();

		if(strExType == _T("font"))
		{
			LOGFONT lf={0};
			CFontDialog fontDlg(&lf, CF_SCREENFONTS|CF_NOVERTFONTS);
			if(fontDlg.DoModal()== IDOK)
			{
				lf = fontDlg.m_lf;
				FontInfo fi;
				fi.strFaceName = lf.lfFaceName;
				fi.style.attr.cSize = abs(lf.lfHeight);
				fi.style.attr.byWeight = lf.lfWeight/4;
				fi.style.attr.byCharset = lf.lfCharSet;
				fi.style.attr.fItalic = lf.lfItalic;
				fi.style.attr.fUnderline = lf.lfUnderline;
				fi.style.attr.fStrike = lf.lfStrikeOut;
				if(lf.lfWeight == FW_BOLD)
				{
					fi.style.attr.fBold = 1;
					fi.style.attr.byWeight = 0;
				}
				SStringW strFontDesc = SFontPool::FontInfoToString(fi);
				e2->pItem->SetValue(strFontDesc);
			}
		}
	}

	void DlgInsertXmlElement::OnPropItemInplaceInit(EventArgs *e)
	{
		EventPropGridItemInplaceInit *e2 = sobj_cast<EventPropGridItemInplaceInit>(e);
		if(e2->pItem->IsClass(SPropertyItemOption::GetClassName()))
		{
			SComboBox *pCombox = (SComboBox*)e2->pInplaceWnd;
			if(e2->pItem->GetExtendType() == L"skin")
			{
				pCombox->InsertItem(-1,L"skin0",0,0);
				pCombox->InsertItem(-1,L"skin1",0,0);
				pCombox->InsertItem(-1,L"skin2",0,0);
			}else if(e2->pItem->GetExtendType() == L"class")
			{
				pCombox->InsertItem(-1,L"style0",0,0);
				pCombox->InsertItem(-1,L"style1",0,0);
				pCombox->InsertItem(-1,L"style2",0,0);
			}
		}
	}


}