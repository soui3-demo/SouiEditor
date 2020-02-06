// dui-demo.cpp : main source file
//

#include "stdafx.h"
#include "MainDlg.h"
#include "SImageBtnEx.h"
#include "SouiRealWndHandler.h"
#include "SImgCanvas.h"
#include "CmdLine.h"
#include <helper/SAppDir.h>
#include "Global.h"
#include "../ExtendCtrls/SCtrlsRegister.h"


//从PE文件加载，注意从文件加载路径位置
#ifdef _DEBUG
	#define RES_TYPE 1   //从文件中加载资源
#else
	#define RES_TYPE 1  //从PE资源中加载UI资源
#endif

#ifdef _DEBUG
#define SYS_NAMED_RESOURCE _T("soui-sys-resourced.dll")
#else
#define SYS_NAMED_RESOURCE _T("soui-sys-resource.dll")
#endif
	
//定义唯一的一个R,UIRES对象,ROBJ_IN_CPP是resource.h中定义的宏。
ROBJ_IN_CPP

SStringT g_CurDir;
void RegisterExtendControl(SApplication *theApp);


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int /*nCmdShow*/)
{
    HRESULT hRes = OleInitialize(NULL);
    SASSERT(SUCCEEDED(hRes));

    int nRet = 0;
    
    SComMgr *pComMgr = new SComMgr;

    //将程序的运行路径修改到项目所在目录所在的目录

	TCHAR szCurrentDir[MAX_PATH] = { 0 };
#ifndef _DEBUG
    GetModuleFileName(NULL, szCurrentDir, sizeof(szCurrentDir));
    LPTSTR lpInsertPos = _tcsrchr(szCurrentDir, _T('\\'));
	lpInsertPos[1] = 0;
    _tcscpy(lpInsertPos + 1, _T("..\\SouiEditor"));
	SetCurrentDirectory(szCurrentDir);
#endif
	GetCurrentDirectory(MAX_PATH,szCurrentDir);
	g_CurDir = szCurrentDir;
	g_CurDir += _T("\\");
    {

		Scintilla_RegisterClasses(hInstance);

        BOOL bLoaded=FALSE;
        SAutoRefPtr<SOUI::IImgDecoderFactory> pImgDecoderFactory;
        SAutoRefPtr<SOUI::IRenderFactory> pRenderFactory;

		bLoaded = pComMgr->CreateRender_Skia((IObjRef**)&pRenderFactory);
        //bLoaded = pComMgr->CreateRender_GDI((IObjRef**)&pRenderFactory);
        SASSERT_FMT(bLoaded,_T("load interface [render] failed!"));
        bLoaded=pComMgr->CreateImgDecoder((IObjRef**)&pImgDecoderFactory);
        SASSERT_FMT(bLoaded,_T("load interface [%s] failed!"),_T("imgdecoder"));

        pRenderFactory->SetImgDecoderFactory(pImgDecoderFactory);
		SouiEditorApp *theApp = new SouiEditorApp(pRenderFactory, hInstance,ksz_editor_cls);

		theApp->RegisterWindowClass<SImageBtnEx>();

		theApp->RegisterWindowClass<SListBoxDrop>();
		theApp->RegisterWindowClass<CDropWnd>();

		theApp->RegisterWindowClass<SPropertyGrid>();//注册属性表控件
		theApp->RegisterWindowClass<SImgCanvas>();


		// 注册扩展控件
		SCtrlsRegister::RegisterCtrls(theApp);

        //从DLL加载系统资源
        HMODULE hModSysResource = LoadLibrary(SYS_NAMED_RESOURCE);
        if (hModSysResource)
        {
            SAutoRefPtr<IResProvider> sysResProvider;
            CreateResProvider(RES_PE, (IObjRef**)&sysResProvider);
            sysResProvider->Init((WPARAM)hModSysResource, 0);
            theApp->LoadSystemNamedResource(sysResProvider);
            FreeLibrary(hModSysResource);
        }else
        {
            SASSERT(0);
        }

        SAutoRefPtr<IResProvider>   pResProvider;
#if (RES_TYPE == 0)
        CreateResProvider(RES_FILE, (IObjRef**)&pResProvider);
        if (!pResProvider->Init((LPARAM)_T("uires"), 0))
        {
            SASSERT(0);
            return 1;
        }
#else 
        CreateResProvider(RES_PE, (IObjRef**)&pResProvider);
        pResProvider->Init((WPARAM)hInstance, 0);
#endif

		theApp->InitXmlNamedID(namedXmlID, ARRAYSIZE(namedXmlID),TRUE);
        theApp->AddResProvider(pResProvider);

		//读取自定义消息框布局
		int ret = 0;
		pugi::xml_document xmlDoc;
		if (!theApp->LoadXmlDocment(xmlDoc, _T("LAYOUT:xml_messagebox")) || !SetMsgTemplate(xmlDoc.child(L"SOUI")))
			ret = -1;
		if (ret == -1)
			SMessageBox(NULL, _T("【消息框皮肤】读取失败"), _T("提示"), 0);

		//设置真窗口处理接口
		CSouiRealWndHandler * pRealWndHandler = new CSouiRealWndHandler();
		theApp->SetRealWndHandler(pRealWndHandler);
		pRealWndHandler->Release();

        // BLOCK: Run application
        {
			CCmdLine cmdLine(GetCommandLine());
            CMainDlg dlgMain;
			if (cmdLine.GetParamCount() > 1)
				dlgMain.m_cmdWorkspaceFile = cmdLine.GetParam(1);
            dlgMain.Create(GetActiveWindow(), WS_CLIPCHILDREN | WS_TABSTOP | WS_POPUP, (DWORD)0);
			SetWindowText(dlgMain.m_hWnd,ksz_editor_wnd);
            dlgMain.SendMessage(WM_INITDIALOG);
            dlgMain.CenterWindow(dlgMain.m_hWnd);
            dlgMain.ShowWindow(SW_SHOWNORMAL);
            nRet = theApp->Run(dlgMain.m_hWnd);
        }

        delete theApp;
    }
    
    delete pComMgr;
    
	Scintilla_ReleaseResources();

    OleUninitialize();
    return nRet;
}
