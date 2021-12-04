#pragma once
#include "core/shostwnd.h"
#include "core/smsgloop.h"
#include "core/SHostDialog.h"
#include "control/SRichEdit.h"
#include "SImgCanvas.h"

class ResManger;

namespace SOUI
{

	class SResMgrDlg: public SHostDialog
	{
	public:
		SResMgrDlg(SStringT strPath,ResManger* pResFileManger);

		~SResMgrDlg(void) { }


	protected:
		void OnNewResType();
		void OnDelResType();
		void OnAddResource();		
		void OnRemoveResource();

		bool OnLbResTypeSelChanged(EventArgs *pEvtBase);
		bool OnLbResSelChanged(EventArgs *pEvtBase);
		bool OnReNotify(EventArgs *pEvt);

		void OnOK();

		EVENT_MAP_BEGIN()
			EVENT_NAME_COMMAND(L"NAME_UIDESIGNER_btn_ZYLX_new", OnNewResType)
			EVENT_NAME_COMMAND(L"NAME_UIDESIGNER_btn_ZYLX_DEL", OnDelResType)
			EVENT_NAME_COMMAND(L"NAME_UIDESIGNER_btn_ZY_NEW", OnAddResource)
			EVENT_NAME_COMMAND(L"NAME_UIDESIGNER_btn_ZY_DEL", OnRemoveResource)

			EVENT_NAME_COMMAND(L"btnOK", OnOK)
		EVENT_MAP_END()

		BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);

		BEGIN_MSG_MAP_EX(SResMgrDlg)
			MSG_WM_INITDIALOG(OnInitDialog)
			CHAIN_MSG_MAP(SHostDialog)
		END_MSG_MAP()

	protected:
		 void InitResType();

         SStringT ListBox_GetCurSelText(SListBox * lb);

		 void ShowImage(const SStringT& strImgname);

	private:

	    SListBox *m_lbResType;  //资源类型
		SListBox *m_lbRes;  //资源

		ResManger* m_pResFileManger;	//所有资源文件的管理

		SStringT m_strProPath;

		SEdit *m_pEdit;

		SImgCanvas *m_imgView;
		SStatic *m_txtImageSize;	//当前图像的大小
	};

}