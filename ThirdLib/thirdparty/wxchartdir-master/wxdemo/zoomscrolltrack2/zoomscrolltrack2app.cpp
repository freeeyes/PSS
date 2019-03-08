/*
** Name:        zoomscrolltrack2app.cpp
** Purpose:     Zooming and Scrolling with Track Line (2)
** Author:      Ulrich Telle
** Created:     2018-07-02
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

#include "zoomscrolltrack2app.h"

/*
 * Application instance implementation
 */

IMPLEMENT_APP( ZoomScrollTrack2App )

/*
 * ZoomScrollTrack2App type definition
 */

IMPLEMENT_CLASS( ZoomScrollTrack2App, wxApp )

/*
 * ZoomScrollTrack2App event table definition
 */

BEGIN_EVENT_TABLE( ZoomScrollTrack2App, wxApp )
END_EVENT_TABLE()

/*
 * Constructor for ZoomScrollTrack2App
 */

ZoomScrollTrack2App::ZoomScrollTrack2App()
{
  Init();
}

/*
 * Member initialisation
 */

void
ZoomScrollTrack2App::Init()
{
}

/*
 * Initialisation for ZoomScrollTrack2App
 */

bool
ZoomScrollTrack2App::OnInit()
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
	ZoomScrollTrack2* mainWindow = new ZoomScrollTrack2(NULL);
	mainWindow->Show(true);

  return true;
}

/*
 * Cleanup for ZoomScrollTrack2App
 */

int
ZoomScrollTrack2App::OnExit()
{    
	return wxApp::OnExit();
}
