/*
** Name:        trackboxapp.cpp
** Purpose:     Track Box sample application
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

#include "trackboxapp.h"

/*
 * Application instance implementation
 */

IMPLEMENT_APP( TrackBoxApp )

/*
 * TrackBoxApp type definition
 */

IMPLEMENT_CLASS( TrackBoxApp, wxApp )

/*
 * TrackBoxApp event table definition
 */

BEGIN_EVENT_TABLE( TrackBoxApp, wxApp )
END_EVENT_TABLE()

/*
 * Constructor for TrackBoxApp
 */

TrackBoxApp::TrackBoxApp()
{
  Init();
}

/*
 * Member initialisation
 */

void
TrackBoxApp::Init()
{
}

/*
 * Initialisation for TrackBoxApp
 */

bool
TrackBoxApp::OnInit()
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

  TrackBox* trackBoxDialog = new TrackBox(NULL);
  trackBoxDialog->Show();
  SetTopWindow(trackBoxDialog);

  return true;
}

/*
 * Cleanup for TrackBoxApp
 */

int
TrackBoxApp::OnExit()
{    
  return wxApp::OnExit();
}
