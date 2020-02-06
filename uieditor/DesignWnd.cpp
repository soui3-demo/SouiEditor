// ScintillaWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "DesignWnd.h"
#include <shlwapi.h>

#include "MainDlg.h"


extern CMainDlg* g_pMainDlg;

#define STR_DESIGNWNDCLS _T("DesignWnd")
//////////////////////////////////////////////////////////////////////////


CDesignWnd::CDesignWnd():m_bmpLogo(NULL)
{
}

CDesignWnd::~CDesignWnd()
{
	if(m_bmpLogo)
	{
		DeleteObject(m_bmpLogo);
	}
}

static HBITMAP CreateGDIBitmap( int nWid,int nHei,void ** ppBits )
{
	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(bmi));
	bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth       = nWid;
	bmi.bmiHeader.biHeight      = -nHei; // top-down image 
	bmi.bmiHeader.biPlanes      = 1;
	bmi.bmiHeader.biBitCount    = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage   = 0;

	HDC hdc=GetDC(NULL);
	HBITMAP hBmp=CreateDIBSection(hdc,&bmi,DIB_RGB_COLORS,ppBits,0,0);
	ReleaseDC(NULL,hdc);
	return hBmp;
}

static HBITMAP BmpFromIBitmap( int nWid,int nHei ,const LPVOID pBits/*=NULL*/)
{
	LPVOID pBmpBits=NULL;
	HBITMAP hBmp = CreateGDIBitmap(nWid,nHei,&pBmpBits);
	if(hBmp)
	{
		const int stride = nWid*4;
		if(pBits)
		{
			memcpy(pBmpBits,pBits,stride*nHei);
		}else
		{
			memset(pBmpBits,0,stride*nHei);
		}
	}
	return hBmp;
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
	
	IBitmap *pBmp = SApplication::getSingletonPtr()->LoadImage(_T("img"),_T("png_logo"));
	if(pBmp)
	{
		const LPVOID pBits = pBmp->GetPixelBits();
		m_bmpLogo = BmpFromIBitmap(pBmp->Width(),pBmp->Height(),pBits);
		pBmp->Release();
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

	{
		if (m_bmpLogo)
		{
			BITMAP bm;
			GetObject(m_bmpLogo, sizeof(bm), &bm);

			CRect rc;
			GetClientRect(&rc);

			HBRUSH hBkBrush = CreateSolidBrush(RGB(59, 59, 62));
			HGDIOBJ oldBrush = SelectObject(dc, hBkBrush);
			Rectangle(dc, 0, 0, rc.Width(), rc.Height());
			SelectObject(dc, oldBrush);
			
			BITMAPINFO bmi;
			memset(&bmi, 0, sizeof(bmi));
			bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmi.bmiHeader.biWidth = bm.bmWidth;
			bmi.bmiHeader.biHeight = -bm.bmHeight; // top-down image
			bmi.bmiHeader.biPlanes = 1;
			bmi.bmiHeader.biBitCount = 32;
			bmi.bmiHeader.biCompression = BI_RGB;
			bmi.bmiHeader.biSizeImage = bm.bmWidth*bm.bmHeight * 4;

			int dstW = rc.Width(), dstH = rc.Height();
			int xpos = 0, ypos = 0;
			if (bm.bmWidth > bm.bmHeight)
			{
				dstH = (float)dstW / bm.bmWidth * dstH;
			}else
			{
				dstW = (float)bm.bmWidth / bm.bmHeight * dstH;
				xpos = (rc.Width() - dstW) / 2;
			}
			
			SetStretchBltMode(dc, HALFTONE);
			StretchDIBits(dc, xpos, ypos, dstW, dstH, 0, 0, bm.bmWidth, bm.bmHeight, bm.bmBits, &bmi, DIB_RGB_COLORS, SRCCOPY);
		}
	}
	::EndPaint(m_hWnd, &ps);

	if (g_pMainDlg && g_pMainDlg->m_hViewer)
	{
		g_pMainDlg->Desiner_TabSelChanged(NULL);
	}
}

void CDesignWnd::RefreshDesignLayout()
{
	HWND hChild = GetWindow(m_hWnd,GW_CHILD);
	if ( hChild )
	{
		CRect rc;
		GetClientRect(&rc);
		::SetWindowPos(hChild,NULL,0,0,rc.Width(),rc.Height(),SWP_NOZORDER);
	}else
	{
		Invalidate();
	}
}

void CDesignWnd::OnSize(UINT nType, CSize size)
{
	RefreshDesignLayout();
}

BOOL CDesignWnd::OnEraseBkgnd(HDC dc)
{
	return TRUE;
}
