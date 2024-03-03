
// ScanViewer.h : main header file for the ScanViewer application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CScanViewerApp:
// See ScanViewer.cpp for the implementation of this class
//

class CScanViewerApp : public CWinApp
{
public:
	CScanViewerApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CScanViewerApp theApp;
