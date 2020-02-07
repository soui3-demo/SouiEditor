// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

struct IListener{
	virtual void OnEditorExit() = 0;
	virtual void OnResize() = 0;
};

class CPreviewHost : public SHostWnd
{
public:
	CPreviewHost(IListener *pListener,LPCTSTR pszLayoutId, HWND hEditor = NULL);
	~CPreviewHost();

protected:
	virtual BOOL OnLoadLayoutFromResourceID(const SStringT & resId) override;

protected:
	void GetSwndIndex(SWindow *pWnd,SList<int> &lstIndex);
	void SelectCtrlByTag(SStringA strTag);

	void OnSwndInitFinish(EventArgs *e);

	SWindow * m_pHover;
	SWindow * m_pSel;
	BOOL	  m_bSOUIWnd;
	BOOL	  m_bVirtualRoot;
	HWND	  m_hEditor;
	
protected:
	//soui消息
	EVENT_MAP_BEGIN()
		EVENT_HANDLER(EventSwndInitFinish::EventID, OnSwndInitFinish)
	EVENT_MAP_END()

	LRESULT OnMouseEvent(UINT uMsg, WPARAM wParam, LPARAM lParam);

	BOOL OnCopyData(HWND wnd, PCOPYDATASTRUCT pCopyDataStruct);
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnSize(UINT nType, CSize size);
	void OnExitSizeMove();
	//HostWnd真实窗口消息处理
	BEGIN_MSG_MAP_EX(CPreviewHost)
		MSG_WM_COPYDATA(OnCopyData)
		MSG_WM_CREATE(OnCreate)
		//MSG_WM_SIZE(OnSize)
		MSG_WM_EXITSIZEMOVE(OnExitSizeMove)
		MESSAGE_RANGE_HANDLER_EX(WM_MOUSEFIRST, WM_MOUSELAST, OnMouseEvent)
		CHAIN_MSG_MAP(SHostWnd)
	END_MSG_MAP()

	SStringA	m_utf8Buffer;

	IListener * m_pListener;
};
