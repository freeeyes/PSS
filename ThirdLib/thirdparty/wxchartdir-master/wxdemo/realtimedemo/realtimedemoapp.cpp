/*
** Name:        realtimedemoapp.cpp
** Purpose:     Simple Zoom & Scroll sample application
** Author:      Ulrich Telle
** Created:     2018-05-09
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
//#include <vld.h>

#include "realtimedemoapp.h"

/*
 * Application instance implementation
 */

IMPLEMENT_APP( RealTimeDemoApp )

/*
 * RealTimeDemoApp type definition
 */

IMPLEMENT_CLASS( RealTimeDemoApp, wxApp )

/*
 * RealTimeDemoApp event table definition
 */

BEGIN_EVENT_TABLE( RealTimeDemoApp, wxApp )
END_EVENT_TABLE()

/*
 * Constructor for RealTimeDemoApp
 */

RealTimeDemoApp::RealTimeDemoApp()
{
  Init();
}

/*
 * Member initialisation
 */

void
RealTimeDemoApp::Init()
{
}

/*
 * Initialisation for RealTimeDemoApp
 */

bool
RealTimeDemoApp::OnInit()
{    
	// Remove the comment markers above and below this block
	// to make permanent changes to the code.

#if wxUSE_XPM
	wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
	wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
	wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
	wxImage::AddHandler(new wxGIFHandler);
#endif
	RealTimeDemo* mainWindow = new RealTimeDemo(NULL);
	mainWindow->Show(true);
////@end RealTimeDemoApp initialisation

  return true;
}

/*
 * Cleanup for RealTimeDemoApp
 */

int
RealTimeDemoApp::OnExit()
{    
	return wxApp::OnExit();
}
