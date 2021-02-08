#pragma once
#include "ThreadObject.h"

class CMainDlg;
class CPathMonitor : public CThreadObject
{
	CMainDlg * m_pMainDlg;
	std::tstring m_strPath;
public:
	CPathMonitor(CMainDlg *pMainDlg);
	~CPathMonitor(void);

	void SetPath(LPCTSTR pszPath);
protected:
	virtual UINT Run() ;
};
