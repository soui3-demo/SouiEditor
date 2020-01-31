#include "stdafx.h"
#include "SUnkWnd.h"

namespace SOUI{
	SUnkWnd::SUnkWnd(void)
	{
	}

	SUnkWnd::~SUnkWnd(void)
	{
	}

	HRESULT SUnkWnd::SetAttribute(const SStringW & strAttribName, const SStringW & strValue, BOOL bLoading/*=FALSE*/)
	{
		HRESULT hRet = SWindow::SetAttribute(strAttribName,strValue,bLoading);
		m_strAttrs+=S_CW2T(SStringW().Format(L" %s=\"%s\"",strAttribName,strValue));
		return hRet;
	}

	void SUnkWnd::SetOriClassName(const SStringW & strOriCls)
	{
		m_strOriCls = S_CW2T(strOriCls);
	}

	SStringT SUnkWnd::GetWindowText(BOOL bRawText/*=FALSE*/)
	{
		return SStringT().Format(_T("<%s%s>"),m_strOriCls,m_strAttrs);
	}

}