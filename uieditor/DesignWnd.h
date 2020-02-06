#pragma once

#include <core/SNativeWnd.h>



// CScintillaWnd
class CDesignWnd : public SNativeWnd
{
public:
	CDesignWnd();
	virtual ~CDesignWnd();
	BOOL CreateWnd(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, HWND hParentWnd, UINT nID,HINSTANCE hInst);

protected:
	void InitDesignWnd(void);

	LRESULT OnNotify(int idCtrl, LPNMHDR pnmh);
	void OnPaint(HDC dc);
	void RefreshDesignLayout();
	void OnSize(UINT nType, CSize size);
	BOOL OnEraseBkgnd(HDC dc);

	BEGIN_MSG_MAP_EX(CDesignWnd)
		MSG_OCM_NOTIFY(OnNotify)
		MSG_WM_PAINT(OnPaint)
		MSG_WM_ERASEBKGND(OnEraseBkgnd)
		MSG_WM_SIZE(OnSize)
		CHAIN_MSG_MAP(SNativeWnd)
	END_MSG_MAP()

	HBITMAP		m_bmpLogo;
};
