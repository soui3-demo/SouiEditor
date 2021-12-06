#pragma once


class CSysDataMgr : public SOUI::SSingleton<CSysDataMgr>
{
public:
	struct CtrlAttrItem
	{
		SStringT attrname;
		pugi::xml_document* attrdoc;
		CtrlAttrItem() { ; }
		CtrlAttrItem(SStringT name, pugi::xml_node xmlnode)
		{
			attrname = name;
			attrdoc = new pugi::xml_document();
			attrdoc->append_copy(xmlnode);
		}
	};

	typedef SArray<CtrlAttrItem> CTRL_ATTR_VALUE;

	CSysDataMgr();
	~CSysDataMgr();

	// 读取系统数据, 控件属性表
	bool LoadSysData(LPCTSTR cfgDir);
	
	SStringT GetConfigDir() const;
	void InitProperty();

	void InitSkinProp();

	void InitComAttr(pugi::xml_node NodeCom, pugi::xml_node cNode, CTRL_ATTR_VALUE & arrControlStyle);

	void InitCtrlProperty(pugi::xml_node NodeCom, pugi::xml_node NodeCtrl, CTRL_ATTR_VALUE* arr_attr);

	void InitCtrlDef();

	// 获取控件名称列表
	SStringA GetCtrlAutos();

	// 获取指定控件的自动完成字串
	SStringA GetCtrlAttrAutos(SStringT ctrlname);

	pugi::xml_node getCtrlDefNode();

	pugi::xml_node getSkinPropNode();
public:
	pugi::xml_document m_xmlDocProperty;	//property.xml文件doc

	pugi::xml_document m_xmlCtrlDef;

	pugi::xml_document m_xmlSkinProp;
private:
	static int textCmp(const void * p1, const void*p2)
	{
		const SStringT *tag1 = (const SStringT*)p1;
		const SStringT *tag2 = (const SStringT*)p2;
		return tag1->Compare(*tag2);
	}
	static int CtrlAttrCmp(const void * p1, const void*p2)
	{
		const CtrlAttrItem *tag1 = (const CtrlAttrItem*)p1;
		const CtrlAttrItem *tag2 = (const CtrlAttrItem*)p2;
		return tag1->attrname.Compare(tag2->attrname);
	}
	static int CtrlAttrCmpNoCase(const void * p1, const void*p2)
	{
		const CtrlAttrItem *tag1 = (const CtrlAttrItem*)p1;
		const CtrlAttrItem *tag2 = (const CtrlAttrItem*)p2;
		return tag1->attrname.CompareNoCase(tag2->attrname);
	}

	SStringT m_strConfigDir;

	// 控件属性
	SMap<SStringT, CTRL_ATTR_VALUE*> m_mapControl;
	// 控件基本样式
	CTRL_ATTR_VALUE m_arrControlStyle;
	// ColorMask
	CTRL_ATTR_VALUE m_arrColorMask;
};