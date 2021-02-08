#pragma once
#include <vector>
#include <helper/SAdapterBase.h>

class CLayoutAdapter : public SAdapterBase
{
	typedef std::pair<SStringT,SStringT> FILEPAIR;
	typedef std::vector<FILEPAIR> LAYOUTLIST;
	typedef std::vector<int> INDEXLIST;
	LAYOUTLIST m_lstLayout;
	INDEXLIST  m_lstIndex;
	SStringT m_strFilter;
	SStringT m_strDir;
public:
	CLayoutAdapter(void);
	~CLayoutAdapter(void);

	void SetFilter(SStringT strFilter);
	bool OpenUires(SStringT strUires);
	void CloseUires();
	SStringT GetPath(int iItem) const;
	SStringT GetName(int iItem) const;
	SStringT GetDir() const;
protected:
	virtual int getCount();

	virtual void getView(int position, SWindow * pItem, pugi::xml_node xmlTemplate);

};
