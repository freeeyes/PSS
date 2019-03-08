/*
** Name:        tracklegendapp.cpp
** Purpose:     Track Legend sample application
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

#include "tracklegendapp.h"

/*
 * Application instance implementation
 */

IMPLEMENT_APP( TrackLegendApp )

/*
 * TrackLegendApp type definition
 */

IMPLEMENT_CLASS( TrackLegendApp, wxApp )

/*
 * TrackLegendApp event table definition
 */

BEGIN_EVENT_TABLE( TrackLegendApp, wxApp )
END_EVENT_TABLE()

/*
 * Constructor for TrackLegendApp
 */

TrackLegendApp::TrackLegendApp()
{
  Init();
}

/*
 * Member initialisation
 */

void TrackLegendApp::Init()
{
}

/*
 * Initialisation for TrackLegendApp
 */

bool
TrackLegendApp::OnInit()
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

  TrackLegend* trackLegendDialog = new TrackLegend(NULL);
  trackLegendDialog->Show();
  SetTopWindow(trackLegendDialog);

  return true;
}

/*
 * Cleanup for TrackLegendApp
 */

int
TrackLegendApp::OnExit()
{    
  return wxApp::OnExit();
}
