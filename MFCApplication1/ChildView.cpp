
// ChildView.cpp : implementation of the CChildView class
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef HRESULT(__stdcall* LPFNDLLFUNC1)(DWORD);

HINSTANCE hDLL;               // Handle to DLL
LPFNDLLFUNC1 loadWndFunc;    // Function pointer
HRESULT hrReturnVal;


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
	if (NULL != hDLL)
		FreeLibrary(hDLL);
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CWnd::OnPaint() for painting messages
}



int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	hDLL = LoadLibrary(L"WndDLL.dll");
	if (NULL != hDLL)
	{
		loadWndFunc = (LPFNDLLFUNC1)GetProcAddress(hDLL, "LoadWnd");
		if (NULL != loadWndFunc)
		{
			HWND hwnd = this->GetSafeHwnd();
			// call the function
			hrReturnVal = loadWndFunc((DWORD)hwnd);
		}
		else
		{
			// report the error
			hrReturnVal = ERROR_DELAY_LOAD_FAILED;
		}
		
	}
	else
	{
		hrReturnVal = ERROR_DELAY_LOAD_FAILED;
	}

	return 0;
}


void CChildView::OnDestroy()
{
	CWnd::OnDestroy();
	if (NULL != hDLL)
	{
		loadWndFunc = (LPFNDLLFUNC1)GetProcAddress(hDLL, "CloseWnd");
		if (NULL != loadWndFunc)
		{
			HWND hwnd = this->GetSafeHwnd();
			// call the function
			hrReturnVal = loadWndFunc((DWORD)hwnd);
		}
	}
	
}
