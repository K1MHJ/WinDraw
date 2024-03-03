
// GameAppView.cpp : implementation of the CGameAppView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GameApp.h"
#endif

#include "GameAppDoc.h"
#include "GameAppView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGameAppView

IMPLEMENT_DYNCREATE(CGameAppView, CView)

BEGIN_MESSAGE_MAP(CGameAppView, CView)
	ON_MESSAGE(WM_USER_COMPLETE_LOAD_XML, &CGameAppView::OnCompleteAnimate)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CGameAppView construction/destruction

CGameAppView::CGameAppView() noexcept
{
	// TODO: add construction code here
	m_quitThread = false;
}

CGameAppView::~CGameAppView()
{
}

BOOL CGameAppView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGameAppView drawing

void CGameAppView::OnDraw(CDC* pDC)
{
	CGameAppDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	//  First get a pointer to the document
	//  Save the current state of the device context
	int nDCSave = pDC->SaveDC();

	CRect rc;
	GetClientRect(&rc);
	int half;
	half = min(rc.Width(), rc.Height()) / 2;
	CPoint center = rc.CenterPoint();
	rc.left = center.x - half;
	rc.top = center.y - half;
	rc.right = center.x + half;
	rc.bottom = center.y + half;

	pDC->StretchBlt(rc.left, rc.top, rc.Width(), rc.Height(), &dcMem, 0, 0, 800, 800, SRCCOPY);
	
	//  Restore the device context settings
	pDC->RestoreDC(nDCSave);
}


// CGameAppView diagnostics

#ifdef _DEBUG
void CGameAppView::AssertValid() const
{
	CView::AssertValid();
}

void CGameAppView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGameAppDoc* CGameAppView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGameAppDoc)));
	return (CGameAppDoc*)m_pDocument;
}
#endif //_DEBUG


// CGameAppView message handlers


void CGameAppView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
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

	// 処理スレッド関数を実行
	m_pAnimateThread = ::AfxBeginThread(AnimateThreadFunc, this, THREAD_PRIORITY_NORMAL,
		0, CREATE_SUSPENDED, NULL);
	if (m_pAnimateThread)
	{
		m_pAnimateThread->m_pMainWnd = this;
		m_pAnimateThread->m_bAutoDelete = FALSE;

		// スレッド処理の開始
		m_pAnimateThread->ResumeThread();
	}
}

UINT CGameAppView::AnimateThreadFunc(void* pParam)
{
	CGameAppView* pView = dynamic_cast<CGameAppView*>(reinterpret_cast<CWnd*>(pParam));
	if (pView)
	{
		pView->AnimateThreadFunc();
	}
	return 123;
}
void CGameAppView::AnimateThreadFunc()
{
	// スレッド本体の処理
	//AfxGetMainWnd()->SendMessage(WM_USER_COMPLETE_LOAD_XML, (WPARAM)&firstIndex, (LPARAM)firstFilePath.GetString());

	for (int i = 0;!m_quitThread;++i) {
		CBrush cBrush;
		CBrush* pOldBrush;
		cBrush.CreateSolidBrush(i%2==0?RGB(100, 100, 100):RGB(200,100,100));
		pOldBrush = dcMem.SelectObject(&cBrush);
		dcMem.PatBlt(0, 0, 800, 800, PATCOPY);
		dcMem.SelectObject(pOldBrush);
		cBrush.DeleteObject();
		Invalidate();
		UpdateWindow();
		Sleep(5000);
	}
	
}

void CGameAppView::OnClose()
{
	// TODO: Add your message handler code here and/or call default
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
	delete m_pAnimateThread;

	CView::OnClose();
}
LRESULT CGameAppView::OnCompleteAnimate(WPARAM wParam, LPARAM lParam)
{
	return 0;
}