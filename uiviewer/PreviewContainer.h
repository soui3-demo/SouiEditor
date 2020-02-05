#pragma once
#include "PreviewHost.h"

class CPreviewContainer : public SNativeWnd,IListener
{
	CPreviewHost	m_previewHost;
public:
	CPreviewContainer(LPCTSTR pszLayoutId, HWND hEditor = NULL);
	~CPreviewContainer(void);

protected:
	virtual void OnEditorExit() override;

protected:
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnScroll(int nBar,UINT nSBCode, UINT nPos, HWND hScrollBar);
	void OnHScroll(UINT nSBCode, UINT nPos, HWND hScrollBar);
	void OnVScroll(UINT nSBCode, UINT nPos, HWND hScrollBar);
	void OnClose();
	void OnPaint(HDC hdc);
	BEGIN_MSG_MAP_EX(CPreviewContainer)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_HSCROLL(OnHScroll)
		MSG_WM_VSCROLL(OnVScroll)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_PAINT(OnPaint)
		CHAIN_MSG_MAP(SNativeWnd)
	END_MSG_MAP()


};

