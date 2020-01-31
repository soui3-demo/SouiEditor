#pragma once

namespace SOUI{

	class SUnkWnd : public SStatic
	{
		SOUI_CLASS_NAME(SUnkWnd,L"unknown")
	public:
		SUnkWnd(void);
		~SUnkWnd(void);
		void SetOriClassName(const SStringW & strOriCls);

		virtual HRESULT SetAttribute(  
			const SOUI::SStringW & strAttribName, 
			const SOUI::SStringW &  strValue,
			BOOL     bLoading=FALSE);
		virtual SStringT GetWindowText(BOOL bRawText=FALSE);
	private:
		SStringT m_strAttrs;
		SStringT m_strOriCls;
	};

}
