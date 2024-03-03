// WndDLL.h : main header file for the WndDLL DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CWndDLLApp
// See WndDLL.cpp for the implementation of this class
//

class CWndDLLApp : public CWinApp
{
public:
	CWndDLLApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
