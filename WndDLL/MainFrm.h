
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "ChildView.h"
#include "MainDoc.h"

#define UM_RUNWORK 123


class CMainFrame : public CWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();
	virtual ~CMainFrame();
protected:
	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

	// Implementation
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


	void RunWorkerThread();
private:
	CWinThread* m_pAnimateThread;
	CMainDoc m_doc;
public:
	bool m_runThread;
	bool m_quitThread;
	static UINT AnimateThreadFunc(void* pParam);
	void        AnimateThreadFunc();


	CChildView    m_wndView;
	
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);

public:
	afx_msg void OnClose();
	afx_msg LRESULT OnRunWork(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


