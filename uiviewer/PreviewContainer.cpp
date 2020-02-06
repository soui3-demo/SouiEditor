#include "stdafx.h"
#include "PreviewContainer.h"

CPreviewContainer::CPreviewContainer(LPCTSTR pszLayoutId, HWND hEditor)
:m_previewHost(this,pszLayoutId,hEditor)
{
}

CPreviewContainer::~CPreviewContainer(void)
{
}

int CPreviewContainer::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	SCROLLINFO si={0};
	si.cbSize = sizeof(si);
	si.fMask=SIF_RANGE|SIF_PAGE;
	si.nMin=0;
	si.nMax=4096;
	si.nPage=100;
	SetScrollInfo(m_hWnd,SB_HORZ,&si,FALSE);
	SetScrollInfo(m_hWnd,SB_VERT,&si,FALSE);

	m_previewHost.Create(m_hWnd,WS_CHILD|WS_VISIBLE,0,0,0,0,0);
	m_previewHost.SetWindowPos(NULL,0,0,0,0,SWP_NOZORDER|SWP_NOSIZE);
	return 0;
}

void CPreviewContainer::OnHScroll(UINT nSBCode, UINT nPos, HWND hScrollBar)
{
	OnScroll(SB_HORZ,nSBCode,nPos,hScrollBar);
}

void CPreviewContainer::OnVScroll(UINT nSBCode, UINT nPos, HWND hScrollBar)
{
	OnScroll(SB_VERT,nSBCode,nPos,hScrollBar);
}

void CPreviewContainer::OnScroll(int nBar,UINT nSBCode, UINT nPos, HWND hScrollBar)
{
	SCROLLINFO si={sizeof(si),SIF_ALL,0};
	GetScrollInfo(m_hWnd,nBar,&si);
	int nNewPos = si.nPos;
	switch(nSBCode)
	{
	case SB_THUMBTRACK:
		nNewPos=nPos;
		break;
	case SB_LINEDOWN:
		nNewPos++;
		break;
	case SB_LINEUP:
		nNewPos--;
		break;
	case SB_PAGEUP:
		nNewPos-=si.nPage;
		break;
	case SB_PAGEDOWN:
		nNewPos+=si.nPage;
		break;
	}
	nNewPos = smin(nNewPos,si.nMax-si.nPage);
	nNewPos = smax(0,nNewPos);
	SetScrollPos(m_hWnd,nBar,nNewPos,FALSE);

	int x = GetScrollPos(m_hWnd,SB_HORZ);
	int y = GetScrollPos(m_hWnd,SB_VERT);
	m_previewHost.SetWindowPos(0,-x,-y,0,0,SWP_NOZORDER|SWP_NOSIZE);
}

void CPreviewContainer::OnClose()
{
	DestroyWindow();
	PostQuitMessage(0);
}

void CPreviewContainer::OnEditorExit()
{
	OnClose();
}

void CPreviewContainer::OnPaint(HDC hdc)
{
	PAINTSTRUCT ps;
	hdc = BeginPaint(m_hWnd,&ps);
	CRect rc;
	GetClientRect(rc);

	HBRUSH hBoxBrush = CreateSolidBrush(RGB(204, 204, 204));
	HBRUSH brWhite = CreateSolidBrush(RGB(255, 255, 255));
	HPEN hPenNone = CreatePen(PS_NULL, 1, RGB(255, 255, 255));

	HGDIOBJ hOldBursh = SelectObject(hdc, brWhite);
	HGDIOBJ hOldPen = SelectObject(hdc, hPenNone);

	Rectangle(hdc, 0, 0, rc.right, rc.bottom);	

	SelectObject(hdc, hBoxBrush);
	const int boxsize = 12;
	int line = 0;
	for (int i = 0; i < rc.Height(); i += boxsize)
	{
		for (int j = 0; j < rc.Width(); j += boxsize * 2)
		{
			int start = 0;
			if (line & 1)
				start = boxsize;
			Rectangle(hdc, start + j, i, start + j + boxsize, i + boxsize);
		}
		line++;
	}
	SelectObject(hdc,hOldPen);
	SelectObject(hdc,hOldBursh);
	::DeleteObject(hBoxBrush);
	::DeleteObject(brWhite);
	::DeleteObject(hPenNone);

	EndPaint(m_hWnd,&ps);
}
