#pragma once
#include "propgrid/SPropertyGrid.h"
#include "ScintillaWnd.h"
#include "spugixml/pugixml.hpp"

namespace SOUI{
	class DlgInsertXmlElement : public SHostDialog
	{
	public:
		DlgInsertXmlElement(pugi::xml_node xmlInitProp,SStringW strNodeName);
		~DlgInsertXmlElement(void);

	public:
		SStringA GetXml() const;
	protected:
		void OnGetValue(IPropertyItem *pItem,BOOL bInsertXml);

		void OnPropValueChanged(EventArgs *e);
		void OnPropItemActive(EventArgs *e);

		void OnBtnOrderByGroup();
		void OnBtnOrderByName();

		EVENT_MAP_BEGIN()
			EVENT_HANDLER(EventPropGridValueChanged::EventID,OnPropValueChanged)
			EVENT_HANDLER(EventPropGridItemActive::EventID,OnPropItemActive)
			EVENT_ID_COMMAND(R.id.btn_order_by_group,OnBtnOrderByGroup)
			EVENT_ID_COMMAND(R.id.btn_order_by_name,OnBtnOrderByName)
		EVENT_MAP_END()

		BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
		void OnTimer(UINT_PTR id);
		void OnDestroy();
		void OnCommand(UINT uNotifyCode, int nID, HWND wndCtl);

		BEGIN_MSG_MAP_EX(DlgInsertXmlElement)
			MSG_WM_DESTROY(OnDestroy)
			MSG_WM_INITDIALOG(OnInitDialog)
			MSG_WM_TIMER(OnTimer)
			MSG_WM_COMMAND(OnCommand)
			CHAIN_MSG_MAP(SHostDialog)
		END_MSG_MAP()

	private:
		void InitPropGrid(SStringW strNodeName,SStringW strParents);

		static BOOL OnEnumPropItem(IPropertyItem *pItem, void* opaque);
		pugi::xml_node m_xmlInitProp;
		SStringW m_strNodeName;

		spugi::xml_document m_xmlDoc;

		SPropertyGrid * m_propgrid;
		CScintillaWnd * m_xmlEditor;
		BOOL			m_bChanged;
		SStringA		m_strXml;
	};
}

