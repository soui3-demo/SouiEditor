// ScintillaWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "DesignWnd.h"
#include <shlwapi.h>

#include "MainDlg.h"


extern CMainDlg* g_pMainDlg;

#define STR_DESIGNWNDCLS _T("DesignWnd")
//////////////////////////////////////////////////////////////////////////


CDesignWnd::CDesignWnd()
{
}

CDesignWnd::~CDesignWnd()
{
}


BOOL CDesignWnd::CreateWnd(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, HWND hParent, UINT nID, HINSTANCE hInst)
{
	HWND hWnd = Create(lpszWindowName, dwStyle, 0, 
		rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
		hParent, NULL);
	
	if (!hWnd)
	{
		int err = GetLastError();
		return FALSE;
	}
	InitDesignWnd();
	return TRUE;
}

void CDesignWnd::InitDesignWnd(void)
{
	SetMsgHandled(TRUE);
}

LRESULT CDesignWnd::OnNotify(int idCtrl, LPNMHDR pnmh)
{
	if (pnmh->hwndFrom != m_hWnd) return 0;
	LRESULT lRet = 0;
	
	return lRet;
}

void CDesignWnd::OnPaint(HDC dc)
{
	PAINTSTRUCT ps;
	dc = ::BeginPaint(m_hWnd, &ps);

	CRect rc;
	GetClientRect(rc);
	HDC hMemDC = CreateCompatibleDC(dc);
	HBITMAP hBitmap = CreateCompatibleBitmap(dc, rc.Width(), rc.Height());
	
	HBRUSH hBoxBrush = CreateSolidBrush(RGB(204, 204, 204));
	HBRUSH brWhite = CreateSolidBrush(RGB(255, 255, 255));
	HPEN hPenNone = CreatePen(PS_NULL, 1, RGB(255, 255, 255));
	
	SelectObject(hMemDC, hBitmap);
	HGDIOBJ hOldBursh = SelectObject(hMemDC, brWhite);
	HGDIOBJ hOldPen = SelectObject(hMemDC, hPenNone);
	
	Rectangle(hMemDC, 0, 0, rc.right, rc.bottom);	

	SelectObject(hMemDC, hBoxBrush);
	const int boxsize = 12;
	int line = 0;
	for (int i = 0; i < rc.Height(); i += boxsize)
	{
		for (int j = 0; j < rc.Width(); j += boxsize * 2)
		{
			int start = 0;
			if (line & 1)
				start = boxsize;
			Rectangle(hMemDC, start + j, i, start + j + boxsize, i + boxsize);
		}
		line++;
	}
	::BitBlt(dc, 0, 0, rc.Width(), rc.Height(), hMemDC, 0, 0, SRCCOPY);

	::DeleteDC(hMemDC);
	::DeleteObject(hBitmap);
	::DeleteObject(hBoxBrush);
	::DeleteObject(brWhite);
	::DeleteObject(hPenNone);
	::EndPaint(m_hWnd, &ps);
}

void CDesignWnd::RefreshDesignLayout()
{
	Invalidate();
	if (g_pMainDlg && g_pMainDlg->m_hViewer)
	{
		g_pMainDlg->Desiner_TabSelChanged(NULL);
	}
}
void CDesignWnd::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if (bShow)
		RefreshDesignLayout();
}

void CDesignWnd::OnSize(UINT nType, CSize size)
{
	RefreshDesignLayout();
}
