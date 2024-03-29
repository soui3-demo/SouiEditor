//stamp:1d7eeb2b142bf901
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
				xml_messagebox = _T("LAYOUT:xml_messagebox");
				XML_MAINWND = _T("LAYOUT:XML_MAINWND");
				xml_mainwnd_left = _T("LAYOUT:xml_mainwnd_left");
				xml_mainwnd_right = _T("LAYOUT:xml_mainwnd_right");
				xml_uidesigner_main = _T("LAYOUT:xml_uidesigner_main");
				xml_res_mgr = _T("LAYOUT:xml_res_mgr");
				UIDESIGNER_XML_NEW_LAYOUT = _T("LAYOUT:UIDESIGNER_XML_NEW_LAYOUT");
				UIDESIGNER_XML_NEW_SKIN = _T("LAYOUT:UIDESIGNER_XML_NEW_SKIN");
				UIDESIGNER_XML_INPUT = _T("LAYOUT:UIDESIGNER_XML_INPUT");
				UIDESIGNER_XML_ABOUT = _T("LAYOUT:UIDESIGNER_XML_ABOUT");
				UIDESIGNER_XML_INSERT_ELEMENT = _T("LAYOUT:UIDESIGNER_XML_INSERT_ELEMENT");
			}
			const TCHAR * xml_messagebox;
			const TCHAR * XML_MAINWND;
			const TCHAR * xml_mainwnd_left;
			const TCHAR * xml_mainwnd_right;
			const TCHAR * xml_uidesigner_main;
			const TCHAR * xml_res_mgr;
			const TCHAR * UIDESIGNER_XML_NEW_LAYOUT;
			const TCHAR * UIDESIGNER_XML_NEW_SKIN;
			const TCHAR * UIDESIGNER_XML_INPUT;
			const TCHAR * UIDESIGNER_XML_ABOUT;
			const TCHAR * UIDESIGNER_XML_INSERT_ELEMENT;
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
		class _IMG{
			public:
			_IMG(){
				icon_main = _T("IMG:icon_main");
				caption_line = _T("IMG:caption_line");
				png_tool_button = _T("IMG:png_tool_button");
				png_prop_switch = _T("IMG:png_prop_switch");
				png_tab_skin = _T("IMG:png_tab_skin");
				png_btn_blue = _T("IMG:png_btn_blue");
				png_app_button = _T("IMG:png_app_button");
				default_scrollbar = _T("IMG:default.scrollbar");
				open_menu_png = _T("IMG:open_menu_png");
				tree_toggle_png = _T("IMG:tree_toggle_png");
				menu_skin_png = _T("IMG:menu_skin_png");
				menu_sep_png = _T("IMG:menu_sep_png");
				png_logo = _T("IMG:png_logo");
				png_toolbar_state = _T("IMG:png_toolbar_state");
				png_toolbar_prop = _T("IMG:png_toolbar_prop");
				png_btn_help = _T("IMG:png_btn_help");
			}
			const TCHAR * icon_main;
			const TCHAR * caption_line;
			const TCHAR * png_tool_button;
			const TCHAR * png_prop_switch;
			const TCHAR * png_tab_skin;
			const TCHAR * png_btn_blue;
			const TCHAR * png_app_button;
			const TCHAR * default_scrollbar;
			const TCHAR * open_menu_png;
			const TCHAR * tree_toggle_png;
			const TCHAR * menu_skin_png;
			const TCHAR * menu_sep_png;
			const TCHAR * png_logo;
			const TCHAR * png_toolbar_state;
			const TCHAR * png_toolbar_prop;
			const TCHAR * png_btn_help;
		}IMG;
		class _ICON{
			public:
			_ICON(){
				ICON_logo = _T("ICON:ICON_logo");
			}
			const TCHAR * ICON_logo;
		}ICON;
		class _smenu{
			public:
			_smenu(){
				menu_layoutfile = _T("smenu:menu_layoutfile");
				menu_recent = _T("smenu:menu_recent");
			}
			const TCHAR * menu_layoutfile;
			const TCHAR * menu_recent;
		}smenu;
	};
	const SNamedID::NAMEDVALUE namedXmlID[]={
		{L"NAME_UIDESIGNER_IMG_VIEW",65582},
		{L"NAME_UIDESIGNER_LB_ZYLX",65574},
		{L"NAME_UIDESIGNER_NEW_SKIN_LB",65592},
		{L"NAME_UIDESIGNER_btn_OK",65589},
		{L"NAME_UIDESIGNER_btn_ZYLX_DEL",65573},
		{L"NAME_UIDESIGNER_btn_ZYLX_new",65572},
		{L"NAME_UIDESIGNER_btn_ZY_DEL",65576},
		{L"NAME_UIDESIGNER_btn_ZY_NEW",65575},
		{L"NAME_UIDESIGNER_btn_ZY_NEWALL",65577},
		{L"NAME_UIDESIGNER_btn_close",65585},
		{L"NAME_UIDESIGNER_btn_dlg",65588},
		{L"NAME_UIDESIGNER_edit_SEARCH",65578},
		{L"NAME_UIDESIGNER_main_icon",65571},
		{L"NAME_UIDESIGNER_split_col",65561},
		{L"UI_main_caption",65546},
		{L"_name_start",65535},
		{L"apptitle",65547},
		{L"btnCancel",65591},
		{L"btnOK",65584},
		{L"btnSwitch",65542},
		{L"btn_close",65548},
		{L"btn_help",65552},
		{L"btn_max",65549},
		{L"btn_min",65551},
		{L"btn_restore",65550},
		{L"button1st",65543},
		{L"button2nd",65544},
		{L"button3rd",65545},
		{L"chk_autosave",65560},
		{L"edtInput",65593},
		{L"img_icon_exclamation",65540},
		{L"img_icon_information",65538},
		{L"img_icon_question",65539},
		{L"item_icon",65568},
		{L"item_text",65569},
		{L"lv_res_path",65579},
		{L"lv_tb_skin",65567},
		{L"lv_tb_widget",65570},
		{L"msgicon",65537},
		{L"msgtext",65541},
		{L"msgtitle",65536},
		{L"new_layout_path",65587},
		{L"new_layout_resname",65586},
		{L"propgrid_element",65595},
		{L"sel_image_size",65583},
		{L"tb_prop",65594},
		{L"toolbar_btn_Close",65555},
		{L"toolbar_btn_NewInclude",65557},
		{L"toolbar_btn_NewLayout",65556},
		{L"toolbar_btn_Open",65553},
		{L"toolbar_btn_recent",65554},
		{L"toolbar_btn_resmgr",65559},
		{L"toolbar_btn_savexml",65558},
		{L"txt_name",65580},
		{L"txt_path",65581},
		{L"txt_prop_desc",65597},
		{L"txt_prop_title",65596},
		{L"txt_title",65590},
		{L"uidesigner_scintilla",100000},
		{L"uidesigner_wnd_layout",100000},
		{L"uidesigner_wnd_xmltree",65565},
		{L"workspace_tab",65562},
		{L"workspace_tree",65563},
		{L"workspace_xmlfile_lb",65564},
		{L"xml_editor",65566}		};
	class _R{
	public:
		class _name{
		public:
		_name(){
			NAME_UIDESIGNER_IMG_VIEW = namedXmlID[0].strName;
			NAME_UIDESIGNER_LB_ZYLX = namedXmlID[1].strName;
			NAME_UIDESIGNER_NEW_SKIN_LB = namedXmlID[2].strName;
			NAME_UIDESIGNER_btn_OK = namedXmlID[3].strName;
			NAME_UIDESIGNER_btn_ZYLX_DEL = namedXmlID[4].strName;
			NAME_UIDESIGNER_btn_ZYLX_new = namedXmlID[5].strName;
			NAME_UIDESIGNER_btn_ZY_DEL = namedXmlID[6].strName;
			NAME_UIDESIGNER_btn_ZY_NEW = namedXmlID[7].strName;
			NAME_UIDESIGNER_btn_ZY_NEWALL = namedXmlID[8].strName;
			NAME_UIDESIGNER_btn_close = namedXmlID[9].strName;
			NAME_UIDESIGNER_btn_dlg = namedXmlID[10].strName;
			NAME_UIDESIGNER_edit_SEARCH = namedXmlID[11].strName;
			NAME_UIDESIGNER_main_icon = namedXmlID[12].strName;
			NAME_UIDESIGNER_split_col = namedXmlID[13].strName;
			UI_main_caption = namedXmlID[14].strName;
			_name_start = namedXmlID[15].strName;
			apptitle = namedXmlID[16].strName;
			btnCancel = namedXmlID[17].strName;
			btnOK = namedXmlID[18].strName;
			btnSwitch = namedXmlID[19].strName;
			btn_close = namedXmlID[20].strName;
			btn_help = namedXmlID[21].strName;
			btn_max = namedXmlID[22].strName;
			btn_min = namedXmlID[23].strName;
			btn_restore = namedXmlID[24].strName;
			button1st = namedXmlID[25].strName;
			button2nd = namedXmlID[26].strName;
			button3rd = namedXmlID[27].strName;
			chk_autosave = namedXmlID[28].strName;
			edtInput = namedXmlID[29].strName;
			img_icon_exclamation = namedXmlID[30].strName;
			img_icon_information = namedXmlID[31].strName;
			img_icon_question = namedXmlID[32].strName;
			item_icon = namedXmlID[33].strName;
			item_text = namedXmlID[34].strName;
			lv_res_path = namedXmlID[35].strName;
			lv_tb_skin = namedXmlID[36].strName;
			lv_tb_widget = namedXmlID[37].strName;
			msgicon = namedXmlID[38].strName;
			msgtext = namedXmlID[39].strName;
			msgtitle = namedXmlID[40].strName;
			new_layout_path = namedXmlID[41].strName;
			new_layout_resname = namedXmlID[42].strName;
			propgrid_element = namedXmlID[43].strName;
			sel_image_size = namedXmlID[44].strName;
			tb_prop = namedXmlID[45].strName;
			toolbar_btn_Close = namedXmlID[46].strName;
			toolbar_btn_NewInclude = namedXmlID[47].strName;
			toolbar_btn_NewLayout = namedXmlID[48].strName;
			toolbar_btn_Open = namedXmlID[49].strName;
			toolbar_btn_recent = namedXmlID[50].strName;
			toolbar_btn_resmgr = namedXmlID[51].strName;
			toolbar_btn_savexml = namedXmlID[52].strName;
			txt_name = namedXmlID[53].strName;
			txt_path = namedXmlID[54].strName;
			txt_prop_desc = namedXmlID[55].strName;
			txt_prop_title = namedXmlID[56].strName;
			txt_title = namedXmlID[57].strName;
			uidesigner_scintilla = namedXmlID[58].strName;
			uidesigner_wnd_layout = namedXmlID[59].strName;
			uidesigner_wnd_xmltree = namedXmlID[60].strName;
			workspace_tab = namedXmlID[61].strName;
			workspace_tree = namedXmlID[62].strName;
			workspace_xmlfile_lb = namedXmlID[63].strName;
			xml_editor = namedXmlID[64].strName;
		}
		 const wchar_t * NAME_UIDESIGNER_IMG_VIEW;
		 const wchar_t * NAME_UIDESIGNER_LB_ZYLX;
		 const wchar_t * NAME_UIDESIGNER_NEW_SKIN_LB;
		 const wchar_t * NAME_UIDESIGNER_btn_OK;
		 const wchar_t * NAME_UIDESIGNER_btn_ZYLX_DEL;
		 const wchar_t * NAME_UIDESIGNER_btn_ZYLX_new;
		 const wchar_t * NAME_UIDESIGNER_btn_ZY_DEL;
		 const wchar_t * NAME_UIDESIGNER_btn_ZY_NEW;
		 const wchar_t * NAME_UIDESIGNER_btn_ZY_NEWALL;
		 const wchar_t * NAME_UIDESIGNER_btn_close;
		 const wchar_t * NAME_UIDESIGNER_btn_dlg;
		 const wchar_t * NAME_UIDESIGNER_edit_SEARCH;
		 const wchar_t * NAME_UIDESIGNER_main_icon;
		 const wchar_t * NAME_UIDESIGNER_split_col;
		 const wchar_t * UI_main_caption;
		 const wchar_t * _name_start;
		 const wchar_t * apptitle;
		 const wchar_t * btnCancel;
		 const wchar_t * btnOK;
		 const wchar_t * btnSwitch;
		 const wchar_t * btn_close;
		 const wchar_t * btn_help;
		 const wchar_t * btn_max;
		 const wchar_t * btn_min;
		 const wchar_t * btn_restore;
		 const wchar_t * button1st;
		 const wchar_t * button2nd;
		 const wchar_t * button3rd;
		 const wchar_t * chk_autosave;
		 const wchar_t * edtInput;
		 const wchar_t * img_icon_exclamation;
		 const wchar_t * img_icon_information;
		 const wchar_t * img_icon_question;
		 const wchar_t * item_icon;
		 const wchar_t * item_text;
		 const wchar_t * lv_res_path;
		 const wchar_t * lv_tb_skin;
		 const wchar_t * lv_tb_widget;
		 const wchar_t * msgicon;
		 const wchar_t * msgtext;
		 const wchar_t * msgtitle;
		 const wchar_t * new_layout_path;
		 const wchar_t * new_layout_resname;
		 const wchar_t * propgrid_element;
		 const wchar_t * sel_image_size;
		 const wchar_t * tb_prop;
		 const wchar_t * toolbar_btn_Close;
		 const wchar_t * toolbar_btn_NewInclude;
		 const wchar_t * toolbar_btn_NewLayout;
		 const wchar_t * toolbar_btn_Open;
		 const wchar_t * toolbar_btn_recent;
		 const wchar_t * toolbar_btn_resmgr;
		 const wchar_t * toolbar_btn_savexml;
		 const wchar_t * txt_name;
		 const wchar_t * txt_path;
		 const wchar_t * txt_prop_desc;
		 const wchar_t * txt_prop_title;
		 const wchar_t * txt_title;
		 const wchar_t * uidesigner_scintilla;
		 const wchar_t * uidesigner_wnd_layout;
		 const wchar_t * uidesigner_wnd_xmltree;
		 const wchar_t * workspace_tab;
		 const wchar_t * workspace_tree;
		 const wchar_t * workspace_xmlfile_lb;
		 const wchar_t * xml_editor;
		}name;

		class _id{
		public:
		const static int NAME_UIDESIGNER_IMG_VIEW	=	65582;
		const static int NAME_UIDESIGNER_LB_ZYLX	=	65574;
		const static int NAME_UIDESIGNER_NEW_SKIN_LB	=	65592;
		const static int NAME_UIDESIGNER_btn_OK	=	65589;
		const static int NAME_UIDESIGNER_btn_ZYLX_DEL	=	65573;
		const static int NAME_UIDESIGNER_btn_ZYLX_new	=	65572;
		const static int NAME_UIDESIGNER_btn_ZY_DEL	=	65576;
		const static int NAME_UIDESIGNER_btn_ZY_NEW	=	65575;
		const static int NAME_UIDESIGNER_btn_ZY_NEWALL	=	65577;
		const static int NAME_UIDESIGNER_btn_close	=	65585;
		const static int NAME_UIDESIGNER_btn_dlg	=	65588;
		const static int NAME_UIDESIGNER_edit_SEARCH	=	65578;
		const static int NAME_UIDESIGNER_main_icon	=	65571;
		const static int NAME_UIDESIGNER_split_col	=	65561;
		const static int UI_main_caption	=	65546;
		const static int _name_start	=	65535;
		const static int apptitle	=	65547;
		const static int btnCancel	=	65591;
		const static int btnOK	=	65584;
		const static int btnSwitch	=	65542;
		const static int btn_close	=	65548;
		const static int btn_help	=	65552;
		const static int btn_max	=	65549;
		const static int btn_min	=	65551;
		const static int btn_restore	=	65550;
		const static int button1st	=	65543;
		const static int button2nd	=	65544;
		const static int button3rd	=	65545;
		const static int chk_autosave	=	65560;
		const static int edtInput	=	65593;
		const static int img_icon_exclamation	=	65540;
		const static int img_icon_information	=	65538;
		const static int img_icon_question	=	65539;
		const static int item_icon	=	65568;
		const static int item_text	=	65569;
		const static int lv_res_path	=	65579;
		const static int lv_tb_skin	=	65567;
		const static int lv_tb_widget	=	65570;
		const static int msgicon	=	65537;
		const static int msgtext	=	65541;
		const static int msgtitle	=	65536;
		const static int new_layout_path	=	65587;
		const static int new_layout_resname	=	65586;
		const static int propgrid_element	=	65595;
		const static int sel_image_size	=	65583;
		const static int tb_prop	=	65594;
		const static int toolbar_btn_Close	=	65555;
		const static int toolbar_btn_NewInclude	=	65557;
		const static int toolbar_btn_NewLayout	=	65556;
		const static int toolbar_btn_Open	=	65553;
		const static int toolbar_btn_recent	=	65554;
		const static int toolbar_btn_resmgr	=	65559;
		const static int toolbar_btn_savexml	=	65558;
		const static int txt_name	=	65580;
		const static int txt_path	=	65581;
		const static int txt_prop_desc	=	65597;
		const static int txt_prop_title	=	65596;
		const static int txt_title	=	65590;
		const static int uidesigner_scintilla	=	100000;
		const static int uidesigner_wnd_layout	=	100000;
		const static int uidesigner_wnd_xmltree	=	65565;
		const static int workspace_tab	=	65562;
		const static int workspace_tree	=	65563;
		const static int workspace_xmlfile_lb	=	65564;
		const static int xml_editor	=	65566;
		}id;

		class _string{
		public:
		const static int openincode	=	0;
		const static int title	=	1;
		const static int ver	=	2;
		}string;

		class _color{
		public:
		const static int bkBlack1	=	0;
		const static int bkBlack2	=	1;
		const static int black	=	2;
		const static int blue	=	3;
		const static int gray	=	4;
		const static int green	=	5;
		const static int imgprviewbg	=	6;
		const static int itemHover	=	7;
		const static int itemSelected	=	8;
		const static int red	=	9;
		const static int white	=	10;
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
