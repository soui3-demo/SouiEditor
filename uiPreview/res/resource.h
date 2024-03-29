//stamp:0932f40c6fc58764
/*<------------------------------------------------------------------------------------------------->*/
/*该文件由uiresbuilder生成，请不要手动修改*/
/*<------------------------------------------------------------------------------------------------->*/
#pragma once
#include <res.mgr/snamedvalue.h>
#define ROBJ_IN_CPP \
namespace SOUI \
{\
    const _R R;\
    const _UIRES UIRES;\
}
namespace SOUI
{
	class _UIRES{
		public:
		class _UIDEF{
			public:
			_UIDEF(){
				XML_INIT = _T("UIDEF:XML_INIT");
			}
			const TCHAR * XML_INIT;
		}UIDEF;
		class _LAYOUT{
			public:
			_LAYOUT(){
				XML_MAINWND = _T("LAYOUT:XML_MAINWND");
				dlg_about = _T("LAYOUT:dlg_about");
			}
			const TCHAR * XML_MAINWND;
			const TCHAR * dlg_about;
		}LAYOUT;
		class _values{
			public:
			_values(){
				string = _T("values:string");
				color = _T("values:color");
				skin = _T("values:skin");
			}
			const TCHAR * string;
			const TCHAR * color;
			const TCHAR * skin;
		}values;
		class _img{
			public:
			_img(){
				png_item_bk = _T("img:png_item_bk");
			}
			const TCHAR * png_item_bk;
		}img;
		class _ICON{
			public:
			_ICON(){
				ICON_LOGO = _T("ICON:ICON_LOGO");
			}
			const TCHAR * ICON_LOGO;
		}ICON;
	};
	const SNamedID::NAMEDVALUE namedXmlID[]={
		{L"_name_start",65535},
		{L"btn_about",65536},
		{L"btn_close",65538},
		{L"btn_close_uires",65540},
		{L"btn_min",65537},
		{L"btn_open_uires",65539},
		{L"edit_filter",65541},
		{L"lv_layout",65542},
		{L"txt_name",65543},
		{L"txt_path",65544}		};
	class _R{
	public:
		class _name{
		public:
		_name(){
			_name_start = namedXmlID[0].strName;
			btn_about = namedXmlID[1].strName;
			btn_close = namedXmlID[2].strName;
			btn_close_uires = namedXmlID[3].strName;
			btn_min = namedXmlID[4].strName;
			btn_open_uires = namedXmlID[5].strName;
			edit_filter = namedXmlID[6].strName;
			lv_layout = namedXmlID[7].strName;
			txt_name = namedXmlID[8].strName;
			txt_path = namedXmlID[9].strName;
		}
		 const wchar_t * _name_start;
		 const wchar_t * btn_about;
		 const wchar_t * btn_close;
		 const wchar_t * btn_close_uires;
		 const wchar_t * btn_min;
		 const wchar_t * btn_open_uires;
		 const wchar_t * edit_filter;
		 const wchar_t * lv_layout;
		 const wchar_t * txt_name;
		 const wchar_t * txt_path;
		}name;

		class _id{
		public:
		const static int _name_start	=	65535;
		const static int btn_about	=	65536;
		const static int btn_close	=	65538;
		const static int btn_close_uires	=	65540;
		const static int btn_min	=	65537;
		const static int btn_open_uires	=	65539;
		const static int edit_filter	=	65541;
		const static int lv_layout	=	65542;
		const static int txt_name	=	65543;
		const static int txt_path	=	65544;
		}id;

		class _string{
		public:
		const static int about	=	0;
		const static int title	=	1;
		const static int ver	=	2;
		}string;

		class _color{
		public:
		const static int blue	=	0;
		const static int gray	=	1;
		const static int green	=	2;
		const static int red	=	3;
		const static int white	=	4;
		}color;

	};

#ifdef R_IN_CPP
	 extern const _R R;
	 extern const _UIRES UIRES;
#else
	 extern const __declspec(selectany) _R & R = _R();
	 extern const __declspec(selectany) _UIRES & UIRES = _UIRES();
#endif//R_IN_CPP
}
