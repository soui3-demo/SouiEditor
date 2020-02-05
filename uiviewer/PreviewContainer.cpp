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
	GetClientRect(&rc);
	::FillRect(hdc,&rc,(HBRUSH)GetStockObject(DKGRAY_BRUSH));
	EndPaint(m_hWnd,&ps);
}
