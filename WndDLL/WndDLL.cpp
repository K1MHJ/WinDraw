// WndDLL.cpp : Defines the initialization routines for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "WndDLL.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CWndDLLApp

BEGIN_MESSAGE_MAP(CWndDLLApp, CWinApp)
END_MESSAGE_MAP()


// CWndDLLApp construction

CWndDLLApp::CWndDLLApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CWndDLLApp object

CWndDLLApp theApp;


// CWndDLLApp initialization

BOOL CWndDLLApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


CMainFrame m_pMainWnd;

extern "C" __declspec(dllexport) HRESULT __stdcall LoadWnd(DWORD param)
{
	TRACE(L"Load\n");
	HWND hWnd = (HWND)param;
	CWnd* pCWnd = CWnd::FromHandle(hWnd);

	CWnd* pFrame = &m_pMainWnd;
	if (!pFrame)
		return 0;

	pFrame->Create(
		NULL,
		NULL,
		WS_CHILD | WS_VISIBLE,
		CRect(0,0,800,600),
		pCWnd,
		0);

	// The one and only window has been initialized, so show and update it
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	pFrame->SendMessage(UM_RUNWORK);
	
	return 1;
}
extern "C" __declspec(dllexport) HRESULT __stdcall CloseWnd(DWORD)
{
	TRACE(L"Close\n");
	return 0;
}