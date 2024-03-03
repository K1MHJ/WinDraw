
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "pch.h"
#include "framework.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_CLOSE()
	ON_MESSAGE(UM_RUNWORK, &CMainFrame::OnRunWork)
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_runThread = false;
	m_quitThread = false;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(
		NULL,
		NULL, 
		WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 0, 0), 
		this,0))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
	m_wndView.SetDocument(&m_doc);
	return 0;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::RunWorkerThread()
{
	m_runThread = true;
	m_pAnimateThread = ::AfxBeginThread(AnimateThreadFunc, this, THREAD_PRIORITY_NORMAL,
		0, CREATE_SUSPENDED, NULL);
	if (m_pAnimateThread)
	{
		m_pAnimateThread->m_pMainWnd = this;
		m_pAnimateThread->m_bAutoDelete = TRUE;
		// スレッド処理の開始
		m_pAnimateThread->ResumeThread();
	}
}

UINT CMainFrame::AnimateThreadFunc(void* pParam)
{
	CMainFrame* pView = dynamic_cast<CMainFrame*>(reinterpret_cast<CWnd*>(pParam));
	if (pView)
	{
		pView->AnimateThreadFunc();
	}
	return 123;
}


void CMainFrame::OnClose()
{
	TRACE(L"Close");
	
	CWnd::OnClose();
}
LRESULT CMainFrame::OnRunWork(WPARAM wParam, LPARAM lParam)
{
	RunWorkerThread();
	return 0;
}

void CMainFrame::OnDestroy()
{
	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	if (m_runThread) {
		m_quitThread = true;
		while (WaitForSingleObject(m_pAnimateThread->m_hThread, 100) == WAIT_TIMEOUT)
		{
			//// フリーズしないようにする
			//MSG msg;
			//while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			//{
			//	::TranslateMessage(&msg);
			//	::DispatchMessage(&msg);
			//}
		}
	}
	//delete m_pAnimateThread;
}


void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	m_wndView.MoveWindow(0, 0, cx, cy);
}
void CMainFrame::AnimateThreadFunc()
{
	m_doc.SetUp();
	FRECT rc;
	rc.left = rc.top = -20;
	rc.right = 20;
	rc.bottom = 20;

	for (int i = -2; i < 3; ++i) {
		for (int j = -2; j < 3; ++j) {
			if (m_quitThread) break;
			FRECT r = rc;
			r.left += i * 50;
			r.right += i * 50;
			r.top += j * 50;
			r.bottom += j * 50;
			m_doc.AddScan(&r);
		}
	}
	m_wndView.SetupView();

	
	m_wndView.DisplayFit();
	//Sleep(1000);
	//m_wndView.DisplayScan(0);//zero scan
	//Sleep(1000);
	//m_wndView.DisplayScan(1);//next scan
	//Sleep(1000);
	//m_wndView.DisplayScan(-1);//preview scan
	//Sleep(1000);
	//m_wndView.DisplayFit();
	//Sleep(1000);
	for (auto scan : m_doc.GetScans()) {
		scan->Status(SCAN_STAT::Cutting);
		m_wndView.BlinkOn();
		Sleep(1000);
		if (m_quitThread) break;
		/*for (int i = 0; i <= 100; i += 10) {
			if (m_quitThread) break;
			scan->Processing(i);
			Sleep(500);
		}*/
		m_wndView.BlinkOff();
		scan->Status(SCAN_STAT::Done);
		m_wndView.UpdateProgress();
		
	}
	m_doc.Delete();
	m_runThread = false;
}