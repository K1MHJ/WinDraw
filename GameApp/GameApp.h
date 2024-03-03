
// GameApp.h : main header file for the GameApp application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CGameAppApp:
// See GameApp.cpp for the implementation of this class
//

class CGameAppApp : public CWinApp
{
public:
	CGameAppApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGameAppApp theApp;
