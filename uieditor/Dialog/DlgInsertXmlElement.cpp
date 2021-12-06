#include "stdafx.h"
#include "DlgInsertXmlElement.h"
#include "propgrid/propitem/SPropertyItem-Option.h"

namespace SOUI{
	DlgInsertXmlElement::DlgInsertXmlElement(pugi::xml_node xmlInitProp)
		:SHostDialog(_T("layout:UIDESIGNER_XML_INSERT_ELEMENT"))
		,m_xmlInitProp(xmlInitProp)
	{
	}

	DlgInsertXmlElement::~DlgInsertXmlElement(void)
	{
	}

	BOOL DlgInsertXmlElement::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
	{
		m_propgrid = FindChildByName2<SPropertyGrid>(L"propgrid_element");
		m_propgrid->LoadFromXml(m_xmlInitProp.first_child());
		SRealWnd *pReal = FindChildByName2<SRealWnd>(L"xml_editor");
		pReal->GetRealHwnd();
		m_xmlEditor = (CScintillaWnd*)pReal->GetUserData();
		
		m_xmlDoc.root().append_child(S_CW2A(m_xmlInitProp.name(),CP_UTF8));

		m_propgrid->EnumProp(&DlgInsertXmlElement::OnEnumPropItem,this);

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
					value = S_CT2A(pOption->Value2Option(pItem->GetValue()),CP_UTF8);
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


}