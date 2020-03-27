#pragma once
#include "ScintillaWnd.h"
#include "Scintilla.h"
#include "layout/SouiLayout.h"
#include "layout/SLinearLayout.h"
#include <map>

class CSysDataMgr;
extern CSysDataMgr g_SysDataMgr;

namespace SOUI
{
	class SPropertyGrid;

	class SDesignerView
	{
	public:
		static BOOL LoadConfig(pugi::xml_document &doc,const SStringT & cfgFile);

		SDesignerView(SHostDialog *pMainHost, STreeCtrl *pTreeXmlStruct);
		~SDesignerView();

		long GetWindowUserData(SWindow *pWnd);

		//SMoveWnd* m_CurSelCtrl;  //用户当前选择的控件
		SList<SWindow*> m_CopyList;

		//保存当前打开的布局文件
		bool SaveLayoutFile();
		BOOL SaveAll();

		//关闭当前打开的布局文件
		BOOL CloseLayoutFile();

		// 打开工程
		BOOL OpenProject(SStringT strFileName);
		BOOL CloseProject();
		

		BOOL InsertLayoutToMap(SStringT);
		void StartPreviewProcess();

		BOOL NewLayout(SStringT strResName, SStringT strPath);  //新建dialog 或include布局

		BOOL LoadLayout(SStringT strFileName, SStringT layoutName);   //加载布局

		BOOL ReloadLayout(BOOL bClearSel = FALSE);

		void SelectCtrlByIndex(int index, bool bReCreatePropGrid = true);
		void SelectCtrlByOrder(int *pOrder,int nLen,HSTREEITEM hFrom = 0);


		//重命名每一个控件的名字
		void RenameChildeWnd(pugi::xml_node xmlNode);
		void RenameWnd(pugi::xml_node xmlNode, BOOL force = FALSE);
		void RenameAllLayoutWnd();
		void RemoveWndName(pugi::xml_node xmlNode, BOOL bClear, SStringT strFileName = _T(""));

		//调试用
		void Debug(pugi::xml_node);
		void Debug(SStringT str);
		SStringT Debug1(pugi::xml_node);

		SStringT NodeToStr(pugi::xml_node xmlNode);

		void SetCurrentCtrl(pugi::xml_node xmlNode, long index); //设置当前选中的控件

		SStringW GetPosFromLayout(SouiLayoutParam *pLayoutParam, INT nPosIndex);

		//通过控件的属性值找到该控件对应的xml节点
		pugi::xml_node FindNodeByAttr(pugi::xml_node NodeRoot, SStringT attrName, SStringT attrValue);

		void BindXmlcodeWnd(SWindow *pXmlTextCtrl);   //绑定界面代码编辑窗口

		void ShowNoteInSciwnd();		

		void InitProperty(SStatic* textCtrl, SWindow *pPropertyContainer);   //初始化属性列表
		void InitCtrlProperty(pugi::xml_node NodeCom, pugi::xml_node NodeCtrl);

		void CreatePropGrid(SStringT strCtrlType);
		void UpdatePropGrid(pugi::xml_node xmlNode);

		bool OnPropGridValueChanged(EventArgs *pEvt);
		bool OnPropGridItemClick(EventArgs *pEvt);
		bool OnPropGridItemActive(EventArgs *pEvt);
		bool OnTCSelChanged(EventArgs *pEvt);

		BOOL bIsContainerCtrl(SStringT strCtrlName); //判断控件是否是容器控件

		void SaveEditorCaretPos();

		void RestoreEditorCaretPos();

		void LocateControlXML();

		void AddCodeToEditor(CScintillaWnd* pSciWnd);  //复制xml代码到代码编辑器
		void GetCodeFromEditor();
		void GetCodeFromEditor(CScintillaWnd* pSciWnd);//从代码编辑器获取xml代码

		void SetSelCtrlNode(pugi::xml_node xmlNode);

		int InitXMLStruct(pugi::xml_node xmlNode, HSTREEITEM item);
		BOOL GoToXmlStructItem(int data, HSTREEITEM item);
		BOOL SDesignerView::GetTC_CtrlNodeTag(HSTREEITEM fromItem, HSTREEITEM item, SStringA& strTag);

		void DeleteCtrl();
		void Preview();
		void unPreview();

		void RefreshRes();
		void ShowZYGLDlg();
		void ShowYSGLDlg();

		int GetIndexData();

		SWindow* FindChildByUserData(SWindow* pWnd, int data);

		void TrimXmlNodeTextBlank(pugi::xml_node xmlNode);

	public:
		SAutoRefPtr<IFont> m_defFont;
	
		bool	 m_bXmlResLoadOK;
		int		 m_nSciCaretPos;		//代码编辑窗口光标位置

		BOOL     m_bChange;    //文件是否被修改,如果被修改需要保存
		BOOL     m_bPage;      //是否为页文件,否则为窗口文件
		CPoint   m_downPt;     //按下的位置
		INT      m_nState;     //是否正在进行创建控件的鼠标动作； 1:是；0:否

		SStringW *m_strxml;
		pugi::xml_node m_curSelXmlNode;   //当前选中的控件的xmlnode 
		pugi::xml_node m_curPropertyXmlNode;   //当前属性控件显示的xmlnode 
		pugi::xml_document m_xmlSelCtrlDoc;//鼠标选择控件列表要创建的控件的xml
		pugi::xml_node m_xmlSelCtrlNode;  //鼠标选择控件列表要创建的控件的xml

		SMap<SStringT, pugi::xml_document *> m_mapLayoutFile;
		SMap<SStringT, SMap<int, SStringT>* > m_mapIncludeReplace;    //保存每一个布局文件对应的include map;

		SStringT m_strUIResFile;   //C:\demos\MyTest\uires\uires.idx
		SStringT m_strProPath;     //C:\demos\MyTest\uires\

		SStringT m_strCurLayoutName; //当前打开的窗体名  XML_MAINDLG
		SStringT m_strCurLayoutXmlFile; //当前打开的窗体文件名  xml\main.xml
		SStringT m_strCurFileEditor; //当前代码编辑器打开代码对应的文件  xml\main.xml

		pugi::xml_node m_CurrentLayoutNode;		//当前正在编辑布局的XML文档结点

		SMap<SStringT, pugi::xml_document*> m_mapCtrlProperty;//所有控件的属性列表 <Button, xmlnode> <Check, xmlNode>
		SWindow *m_pPropertyContainer;     //属性面板父窗口
		SStatic* m_textCtrlTypename;	//显示选择的控件类型

		CScintillaWnd *m_pScintillaWnd;	//XML代码编辑窗口

		SStringT m_strCurrentCtrlType; //当前选中的控件类型 "button" "check"等等
		SPropertyGrid *m_pPropgrid;    //属性面板

		SHostDialog* m_pMainHost;

		pugi::xml_document m_xmlDocUiRes;  //uires文件

		SList<SStringT> m_lstSouiProperty;   //SOUI节点的属性列表  在property.xml hostwnd节点中定义
		SList<SStringT> m_lstRootProperty;   //Root节点的属性列表 
		SList<SStringT> m_lstContainerCtrl;  //容器控件列表，非容器控件，上面不能摆放控件 在Ctrl.xml中定义

		STreeCtrl *m_treeXmlStruct; //显示xml文档结构的tree控件

		int			m_CurSelCtrlIndex;
		HSTREEITEM	m_CurSelCtrlItem;
		int			m_ndata;	//这个值用来标识xmlnode的每一个节点，节点属性为data,xmlnode的这个属性值是唯一的;
		HSTREEITEM	m_rootItem;// root window item.
	};
}