#pragma once
#include "propgrid/SPropertyGrid.h"
#include "ScintillaWnd.h"
#include "spugixml/pugixml.hpp"

namespace SOUI{
	class DlgInsertXmlElement : public SHostDialog, CScintillaWnd::IListener
	{
	public:
		DlgInsertXmlElement(void);
		~DlgInsertXmlElement(void);

	public:
		SStringA GetXml() const;
	protected:
		void OnGetValue(IPropertyItem *pItem,BOOL bInsertXml);

		void OnPropValueChanged(EventArgs *e);
		EVENT_MAP_BEGIN()
			EVENT_HANDLER(EventPropGridValueChanged::EventID,OnPropValueChanged)
		EVENT_MAP_END()

		BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
		void OnTimer(UINT_PTR id);
		void OnDestroy();
		BEGIN_MSG_MAP_EX(DlgInsertXmlElement)
			MSG_WM_DESTROY(OnDestroy)
			MSG_WM_INITDIALOG(OnInitDialog)
			MSG_WM_TIMER(OnTimer)
			CHAIN_MSG_MAP(SHostDialog)
		END_MSG_MAP()


	protected:
		virtual void onScintillaChange();

		virtual void onScintillaSave(LPCTSTR pszFileName);

	public:
		pugi::xml_node m_xmlInitProp;

	private:
		static BOOL OnEnumPropItem(IPropertyItem *pItem, void* opaque);


		spugi::xml_document m_xmlDoc;

		SPropertyGrid * m_propgrid;
		CScintillaWnd * m_xmlEditor;
		BOOL			m_bChanged;
		SStringA		m_strXml;
	};
}

