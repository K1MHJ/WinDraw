
// GameAppView.h : interface of the CGameAppView class
//

#pragma once


class CGameAppView : public CView
{
protected: // create from serialization only
	CGameAppView() noexcept;
	DECLARE_DYNCREATE(CGameAppView)
private:
	CWinThread* m_pAnimateThread;
public:
	bool m_quitThread;
	static UINT AnimateThreadFunc(void* pParam);
	void        AnimateThreadFunc();
// Attributes
public:
	CGameAppDoc* GetDocument() const;

	CDC dcMem;
	CBitmap* pOldBitmap;
	CBitmap* pBitmap;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	

	virtual ~CGameAppView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg LRESULT OnCompleteAnimate(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();
};

#ifndef _DEBUG  // debug version in GameAppView.cpp
inline CGameAppDoc* CGameAppView::GetDocument() const
   { return reinterpret_cast<CGameAppDoc*>(m_pDocument); }
#endif

