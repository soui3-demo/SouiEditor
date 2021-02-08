#include "StdAfx.h"
#include "LayoutAdapter.h"

CLayoutAdapter::CLayoutAdapter(void)
{
}

CLayoutAdapter::~CLayoutAdapter(void)
{
}

int CLayoutAdapter::getCount()
{
	return m_lstIndex.size();
}

void CLayoutAdapter::getView(int position, SWindow * pItem, pugi::xml_node xmlTemplate)
{
	if(pItem->GetChildrenCount()==0)
	{
		pItem->InitFromXml(xmlTemplate);
	}
	SStatic *pTxtName = pItem->FindChildByID2<SStatic>(R.id.txt_name);
	FILEPAIR & fp = m_lstLayout[m_lstIndex[position]];
	pTxtName->SetWindowText(fp.first);
	SStatic *pTxtPath = pItem->FindChildByID2<SStatic>(R.id.txt_path);
	pTxtPath->SetWindowText(fp.second);
}

void CLayoutAdapter::SetFilter(SStringT strFilter)
{
	m_strFilter = strFilter;
	m_strFilter.MakeLower();
	m_lstIndex.clear();
	int index = 0;
	for(LAYOUTLIST::iterator it = m_lstLayout.begin();it!=m_lstLayout.end();it++,index++)
	{
		if(m_strFilter.IsEmpty() || it->first.Find(m_strFilter)!=-1 || it->second.Find(m_strFilter)!=-1)
		{
			m_lstIndex.push_back(index);
		}
	}
	notifyDataSetChanged();
}

bool CLayoutAdapter::OpenUires(SStringT strUires)
{
	if(!strUires.EndsWith(_T("\\uires.idx"),true))
		return false;

	m_lstLayout.clear();
	m_lstIndex.clear();
	m_strFilter.Empty();

	m_strDir = strUires.Left(strUires.GetLength()-9);
	pugi::xml_document xmlDoc;
	if(xmlDoc.load_file(strUires))
	{
		pugi::xml_node xmlType = xmlDoc.first_child().first_child();
		while(xmlType)
		{
			if(wcsicmp(xmlType.name(),L"layout")==0 || xmlType.attribute(L"layout").as_bool(false))
			{
				pugi::xml_node xmlFile = xmlType.child(L"file");
				while(xmlFile)
				{

					SStringT strName = S_CW2T(xmlFile.attribute(L"name").as_string());
					SStringT strPath = S_CW2T(xmlFile.attribute(L"path").as_string());
					m_lstLayout.push_back(std::make_pair(strName,strPath));
					xmlFile = xmlFile.next_sibling(L"file");
				}
			}
			xmlType = xmlType.next_sibling();
		}
	}
	SetFilter(_T(""));
	return true;
}

SStringT CLayoutAdapter::GetPath(int iItem) const
{
	const FILEPAIR & fp = m_lstLayout[m_lstIndex[iItem]];
	return m_strDir + fp.second;
}

SOUI::SStringT CLayoutAdapter::GetName(int iItem) const
{
	const FILEPAIR & fp = m_lstLayout[m_lstIndex[iItem]];
	return fp.first;
}

SOUI::SStringT CLayoutAdapter::GetDir() const
{
	return m_strDir;
}

void CLayoutAdapter::CloseUires()
{
	m_strFilter.Empty();
	m_strDir.Empty();
	m_lstIndex.clear();
	m_lstLayout.clear();
	notifyDataSetChanged();
}
