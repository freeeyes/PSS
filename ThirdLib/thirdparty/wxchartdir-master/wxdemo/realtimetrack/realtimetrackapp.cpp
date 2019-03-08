/*
** Name:        realtimetrackapp.cpp
** Purpose:     Realtime Chart with Track Line
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

#include "realtimetrackapp.h"

/*
 * Application instance implementation
 */

IMPLEMENT_APP( RealTimeTrackApp )

/*
 * RealTimeTrackApp type definition
 */

IMPLEMENT_CLASS( RealTimeTrackApp, wxApp )

/*
 * RealTimeTrackApp event table definition
 */

BEGIN_EVENT_TABLE( RealTimeTrackApp, wxApp )
END_EVENT_TABLE()

/*
 * Constructor for RealTimeTrackApp
 */

RealTimeTrackApp::RealTimeTrackApp()
{
  Init();
}

/*
 * Member initialisation
 */

void
RealTimeTrackApp::Init()
{
}

/*
 * Initialisation for RealTimeTrackApp
 */

bool
RealTimeTrackApp::OnInit()
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
	RealTimeTrack* mainWindow = new RealTimeTrack(NULL);
	mainWindow->Show(true);
////@end RealTimeTrackApp initialisation

  return true;
}

/*
 * Cleanup for RealTimeTrackApp
 */

int
RealTimeTrackApp::OnExit()
{    
	return wxApp::OnExit();
}
