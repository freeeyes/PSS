/*
** Name:        zoomscrolltrackapp.cpp
** Purpose:     Zooming and Scrolling with Track Line (1)
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

#include "zoomscrolltrackapp.h"

/*
 * Application instance implementation
 */

IMPLEMENT_APP( ZoomScrollTrackApp )

/*
 * ZoomScrollTrackApp type definition
 */

IMPLEMENT_CLASS( ZoomScrollTrackApp, wxApp )

/*
 * ZoomScrollTrackApp event table definition
 */

BEGIN_EVENT_TABLE( ZoomScrollTrackApp, wxApp )
END_EVENT_TABLE()

/*
 * Constructor for ZoomScrollTrackApp
 */

ZoomScrollTrackApp::ZoomScrollTrackApp()
{
  Init();
}

/*
 * Member initialisation
 */

void
ZoomScrollTrackApp::Init()
{
}

/*
 * Initialisation for ZoomScrollTrackApp
 */

bool
ZoomScrollTrackApp::OnInit()
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
	ZoomScrollTrack* mainWindow = new ZoomScrollTrack(NULL);
	mainWindow->Show(true);
////@end ZoomScrollTrackApp initialisation

  return true;
}

/*
 * Cleanup for ZoomScrollTrackApp
 */

int
ZoomScrollTrackApp::OnExit()
{    
	return wxApp::OnExit();
}
