// TestWnd.cpp : implementation file
//

#include "pch.h"
#include "WndDLL.h"
#include "TestWnd.h"


// TestWnd

IMPLEMENT_DYNAMIC(TestWnd, CWnd)

TestWnd::TestWnd()
{

}

TestWnd::~TestWnd()
{
}


BEGIN_MESSAGE_MAP(TestWnd, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// TestWnd message handlers




BOOL TestWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class

	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


void TestWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages
	CRect rc;
	GetClientRect(&rc);
	CBrush brush, * old_brush;
	brush.CreateSolidBrush(RGB(100, 100, 0));
	old_brush = dc.SelectObject(&brush);

	dc.Rectangle(rc);

	dc.SelectObject(old_brush);
	brush.DeleteObject();

	
}


BOOL TestWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
	cs.cx = 200;
	cs.cy = 200;

	return CWnd::PreCreateWindow(cs);
}
