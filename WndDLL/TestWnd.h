#pragma once


// TestWnd

class TestWnd : public CWnd
{
	DECLARE_DYNAMIC(TestWnd)

public:
	TestWnd();
	virtual ~TestWnd();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg void OnPaint();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


