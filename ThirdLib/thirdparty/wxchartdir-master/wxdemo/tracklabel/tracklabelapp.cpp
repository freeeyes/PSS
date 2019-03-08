/*
** Name:        tracklabelapp.cpp
** Purpose:     Track Label sample application
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

#include "tracklabelapp.h"

/*
 * Application instance implementation
 */

IMPLEMENT_APP( TrackLabelApp )

/*
 * TrackLabelApp type definition
 */

IMPLEMENT_CLASS( TrackLabelApp, wxApp )

/*
 * TrackLabelApp event table definition
 */

BEGIN_EVENT_TABLE( TrackLabelApp, wxApp )
END_EVENT_TABLE()

/*
 * Constructor for TrackLabelApp
 */

TrackLabelApp::TrackLabelApp()
{
  Init();
}


/*
 * Member initialisation
 */

void
TrackLabelApp::Init()
{
}

/*
 * Initialisation for TrackLabelApp
 */

bool
TrackLabelApp::OnInit()
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

  TrackLabel* trackLabelDialog = new TrackLabel(NULL);
  trackLabelDialog->Show();
  SetTopWindow(trackLabelDialog);

  return true;
}

/*
 * Cleanup for TrackLabelApp
 */

int
TrackLabelApp::OnExit()
{    
  return wxApp::OnExit();
}
