/*
** Name:        viewportcontroldemoapp.cpp
** Purpose:     Zooming and Scrolling with Viewport Control
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

#include "viewportcontroldemoapp.h"

/*
 * Application instance implementation
 */

IMPLEMENT_APP( ViewportControlDemoApp )

/*
 * ViewportControlDemoApp type definition
 */

IMPLEMENT_CLASS( ViewportControlDemoApp, wxApp )

/*
 * ViewportControlDemoApp event table definition
 */

BEGIN_EVENT_TABLE( ViewportControlDemoApp, wxApp )
END_EVENT_TABLE()

/*
 * Constructor for ViewportControlDemoApp
 */

ViewportControlDemoApp::ViewportControlDemoApp()
{
  Init();
}

/*
 * Member initialisation
 */

void
ViewportControlDemoApp::Init()
{
}

/*
 * Initialisation for ViewportControlDemoApp
 */

bool
ViewportControlDemoApp::OnInit()
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
  ViewportControlDemo* mainWindow = new ViewportControlDemo(NULL);
	mainWindow->Show(true);
////@end ViewportControlDemoApp initialisation

  return true;
}

/*
 * Cleanup for ViewportControlDemoApp
 */

int
ViewportControlDemoApp::OnExit()
{    
	return wxApp::OnExit();
}
