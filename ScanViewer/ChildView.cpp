
// ChildView.cpp : implementation of the CChildView class
//

#include "pch.h"
#include "framework.h"
#include "ScanViewer.h"
#include "ChildView.h"
#include <algorithm>

#define IDT_TEST 1

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_CLOSE()
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
	CRect rc;
	GetClientRect(rc);
	int half;
	half = std::min(rc.Width(), rc.Height())/2;
	CPoint center = rc.CenterPoint();
	rc.left = center.x - half;
	rc.top = center.y - half;
	rc.right = center.x + half;
	rc.bottom = center.y + half;
	
	dc.StretchBlt(rc.left, rc.top, rc.Width(), rc.Height(), &dcMem, 0, 0, 800, 800, SRCCOPY);

	// Do not call CWnd::OnPaint() for painting messages
}
void CALLBACK CChildView::MyTimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	::KillTimer(hwnd, IDT_TEST);
	
}
DWORD WINAPI CChildView::MyThreadFunction(LPVOID lpParam)
{
	CChildView& view = *(CChildView*)lpParam;
	view.Draw(0);
	Sleep(2000);
	view.Draw(1);
	return 0;
}
int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	UINT interval = 1 * 1000;
	//SetTimer(IDT_TEST, interval, (TIMERPROC)MyTimerProc);
	SetTimer(IDT_TEST, interval, NULL);
	 
	CBrush cBrush;
	CBrush* pOldBrush;

	CDC* pDC;
	pDC = GetDC();
	pBitmap = new CBitmap();
	pBitmap->CreateCompatibleBitmap(pDC, 800, 800);
	dcMem.CreateCompatibleDC(pDC);
	pOldBitmap = dcMem.SelectObject(pBitmap);

	cBrush.CreateSolidBrush(RGB(100, 100, 255));
	pOldBrush = dcMem.SelectObject(&cBrush);
	dcMem.PatBlt(0, 0, 800, 800, PATCOPY);

	dcMem.SelectObject(pOldBrush);
	cBrush.DeleteObject();
	ReleaseDC(pDC);
	return 0;
}


void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == IDT_TEST) {
		KillTimer(IDT_TEST);
		hThread = CreateThread(
			NULL,                   // default security attributes
			0,                      // use default stack size  
			MyThreadFunction,       // thread function name
			this,          // argument to thread function 
			0,                      // use default creation flags 
			&dwThreadId);   // returns the thread identifier 

	}
	CWnd::OnTimer(nIDEvent);
}


void CChildView::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	BOOL err = KillTimer(IDT_TEST);
	if (!err) {
	}
	CWnd::OnClose();
}
void CChildView::Draw(int n)
{
	CBrush cBrush;
	CBrush* pOldBrush;

	if (n == 0) {
		cBrush.CreateSolidBrush(RGB(100, 100, 255));
	}
	else {
		cBrush.CreateSolidBrush(RGB(10, 10, 21));
	}
	cBrush.CreateSolidBrush(RGB(100, 100, 255));
	pOldBrush = dcMem.SelectObject(&cBrush);
	dcMem.PatBlt(0, 0, 800, 800, PATCOPY);

	dcMem.SelectObject(pOldBrush);
	cBrush.DeleteObject();
	
}

void CChildView::OnDestroy()
{
	CWnd::OnDestroy();
	dcMem.SelectObject(pOldBitmap);
	pBitmap->DeleteObject();
	dcMem.DeleteDC();
}
