
// PSS_ClientManager.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CPSS_ClientManagerApp:
// See PSS_ClientManager.cpp for the implementation of this class
//

class CPSS_ClientManagerApp : public CWinAppEx
{
public:
	CPSS_ClientManagerApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CPSS_ClientManagerApp theApp;