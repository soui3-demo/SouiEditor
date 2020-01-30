#include "stdafx.h"
#include "SCtrlsRegister.h"
//À©Õ¹¿Ø¼þ
#include "stabctrl2.h"
#include "SButtonEx.h"
#include "simagemaskwnd.h"
#include "SFreeMoveWindow.h"
#include "SVscrollbar.h"
#include "SSkinNewScrollBar.h"
#include "gif/SSkinGif.h"
#include "gif/SSkinAPNG.h"
#include "gif/SGifPlayer.h"
#include "SScrollText.h"
#include "ExtendSkins.h"
#include "ExtendCtrls.h"
#include "SRatingBar.h"
#include "SListCtrlEx.h"
#include "SIPAddressCtrl.h"
#include "STurn3DView.h"
#include "SRadioBox2.h"
#include "SMcListViewEx/SHeaderCtrlEx.h"
#include <SGroupList.h>
#include <SChromeTabCtrl.h>
#include <SScrollText.h>

namespace SOUI{


void SCtrlsRegister::RegisterCtrls(SApplication *theApp)
{
	theApp->RegisterWindowClass<STabPage2>();//×¢²áSTabPage2
	theApp->RegisterWindowClass<STabCtrl2>();//×¢²áSTabCtrl2
	theApp->RegisterWindowClass<SHeaderCtrlEx>();//×¢²áSTabCtrl2
	theApp->RegisterWindowClass<SChromeTabCtrl>();//×¢²áChromeTabCtrl
	theApp->RegisterWindowClass<SImageMaskWnd>();//×¢²áSImageMaskWnd
	theApp->RegisterWindowClass<SButtonEx>();
	theApp->RegisterWindowClass<SWindowEx>();
	theApp->RegisterWindowClass<SGifPlayer>();//×¢²ágif
	theApp->RegisterWindowClass<SRatingBar>();
	theApp->RegisterWindowClass<SIPAddressCtrl>();
	theApp->RegisterWindowClass<STurn3dView>();
	theApp->RegisterWindowClass<SRadioBox2>();
	theApp->RegisterWindowClass<SGroupList>();
	theApp->RegisterWindowClass<SScrollText>();


	//extened skins
	theApp->RegisterSkinClass<SColorMask>();
	theApp->RegisterSkinClass<SSkinVScrollbar>();
	theApp->RegisterSkinClass<SSkinNewScrollbar>();
	theApp->RegisterSkinClass<SSkinGif>();
	theApp->RegisterSkinClass<SSkinAPNG>();
}


}