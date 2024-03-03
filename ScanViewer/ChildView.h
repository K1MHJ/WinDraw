
// ChildView.h : interface of the CChildView class
//


#pragma once


// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:
	HANDLE hThread;
	DWORD dwThreadId;
// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	static void CALLBACK MyTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
	static DWORD WINAPI MyThreadFunction(LPVOID lpParam);
private:
	void Draw(int n);
	
	CDC dcMem;
	CBitmap* pOldBitmap;
	CBitmap* pBitmap;

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
};

