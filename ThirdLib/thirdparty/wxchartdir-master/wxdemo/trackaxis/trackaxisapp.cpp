/*
** Name:        trackaxisapp.cpp
** Purpose:     Track Axis sample application
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

#include "trackaxisapp.h"

/*
 * Application instance implementation
 */

IMPLEMENT_APP( TrackAxisApp )

/*
 * TrackAxisApp type definition
 */

IMPLEMENT_CLASS( TrackAxisApp, wxApp )


/*
 * TrackAxisApp event table definition
 */

BEGIN_EVENT_TABLE( TrackAxisApp, wxApp )
END_EVENT_TABLE()

/*
 * Constructor for TrackAxisApp
 */

TrackAxisApp::TrackAxisApp()
{
  Init();
}


/*
 * Member initialisation
 */

void TrackAxisApp::Init()
{
}

/*
 * Initialisation for TrackAxisApp
 */

bool TrackAxisApp::OnInit()
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

  TrackAxis* trackAxisDialog = new TrackAxis(NULL);
  trackAxisDialog->Show();
  SetTopWindow(trackAxisDialog);

  return true;
}


/*
 * Cleanup for TrackAxisApp
 */

int TrackAxisApp::OnExit()
{    
  return wxApp::OnExit();
}

