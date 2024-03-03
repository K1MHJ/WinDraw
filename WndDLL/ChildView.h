
// ChildView.h : interface of the CChildView class
//
#pragma once

#include "MainDoc.h"
#include <map>
#include <vector>


class CTotalView : public CBitmap
{
public:
	CTotalView() {
		//this->CreateBitmap(_size, _size, nPlanes, nBitCount, Buffer);
	}
	static const int PxlSize = 200;
	int PixelSize() const { return PxlSize; }
	const UINT nPlanes = 1;
	const UINT nBitCount = 32;
	//PBYTE Buffer[_size * _size * 4];
};
class CScanView : public CBitmap
{
public:
	CScanView() {
		//this->CreateBitmap(_size, _size, nPlanes, nBitCount, Buffer);
	}
	int PixelSize() const { return PxlSize; }
	static const int PxlSize = 200;
	const UINT nPlanes = 1;
	const UINT nBitCount = 32;
	//PBYTE Buffer[_size * _size * 4];
};
class CChildView : public CWnd
{
	enum class COLOR{
		BACKGROUND,
		NOTCUT,
		PROCESSING,
		MAX
	};
	// Construction
public:
	CChildView();

	// Attributes
public:
	CDC dcMem;
	const int MemSize = 400;
	void SetDocument(CReadableDoc* doc) {
		m_pDoc = doc;
	}

	void SetupView();
	void DisplayScan(int incr);
	void DisplayFit();
	void Display();
	void BlinkOn();
	void BlinkOff();
	void UpdateProgress();
private:
	bool m_FullShow;
	bool m_EachScanShow;
	int  m_EachScanNo;

	void DrawToMemDC();
	CBitmap* pOldBitmap;
	CBitmap* pBitmap;
	CReadableDoc* m_pDoc;
	const CReadableDoc* GetDocument() {
		return m_pDoc;
	}
	// Operations
	CBrush m_colors[(int)COLOR::MAX + 1];
	CBrush* GetStockBrush(COLOR type);

	CTotalView m_totalview;
	CTotalView m_progressview;
	std::vector<CScanView*> m_scanviews;
	std::vector<CRect> m_blinkingAreas;
	UINT m_blinkingCount;
public:

	// Overrides
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	// Implementation
public:
	virtual ~CChildView();
	
	// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

