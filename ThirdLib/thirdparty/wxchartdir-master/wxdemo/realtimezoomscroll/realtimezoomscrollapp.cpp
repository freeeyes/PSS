/*
** Name:        realtimezoomscrollapp.cpp
** Purpose:     Realtime Chart with Zoom/Scroll and Track Line
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

#include "realtimezoomscrollapp.h"

/*
 * Application instance implementation
 */

IMPLEMENT_APP( RealTimeZoomScrollApp )

/*
 * RealTimeZoomScrollApp type definition
 */

IMPLEMENT_CLASS( RealTimeZoomScrollApp, wxApp )

/*
 * RealTimeZoomScrollApp event table definition
 */

BEGIN_EVENT_TABLE( RealTimeZoomScrollApp, wxApp )
END_EVENT_TABLE()

/*
 * Constructor for RealTimeZoomScrollApp
 */

RealTimeZoomScrollApp::RealTimeZoomScrollApp()
{
  Init();
}

/*
 * Member initialisation
 */

void
RealTimeZoomScrollApp::Init()
{
}

/*
 * Initialisation for RealTimeZoomScrollApp
 */

bool
RealTimeZoomScrollApp::OnInit()
{    
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
	RealTimeZoomScroll* mainWindow = new RealTimeZoomScroll(NULL);
	mainWindow->Show(true);

  return true;
}

/*
 * Cleanup for RealTimeZoomScrollApp
 */

int
RealTimeZoomScrollApp::OnExit()
{    
	return wxApp::OnExit();
}
