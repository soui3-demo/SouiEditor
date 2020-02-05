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

	::EndPaint(m_hWnd, &ps);

	if (g_pMainDlg && g_pMainDlg->m_hViewer)
	{
		g_pMainDlg->Desiner_TabSelChanged(NULL);
	}
}

void CDesignWnd::RefreshDesignLayout()
{
	HWND hChild = GetWindow(m_hWnd,GW_CHILD);
	if(hChild)
	{
		CRect rc;
		GetClientRect(&rc);
		::SetWindowPos(hChild,NULL,0,0,rc.Width(),rc.Height(),SWP_NOZORDER);
	}
}

void CDesignWnd::OnSize(UINT nType, CSize size)
{
	RefreshDesignLayout();
}
