/*
** Name:        trackfinanceapp.cpp
** Purpose:     Track Finance sample application
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

#include "trackfinanceapp.h"

/*
 * Application instance implementation
 */

IMPLEMENT_APP( TrackFinanceApp )

/*
 * TrackFinanceApp type definition
 */

IMPLEMENT_CLASS( TrackFinanceApp, wxApp )

/*
 * TrackFinanceApp event table definition
 */

BEGIN_EVENT_TABLE( TrackFinanceApp, wxApp )
END_EVENT_TABLE()

/*
 * Constructor for TrackFinanceApp
 */

TrackFinanceApp::TrackFinanceApp()
{
  Init();
}

/*
 * Member initialisation
 */

void
TrackFinanceApp::Init()
{
}

/*
 * Initialisation for TrackFinanceApp
 */

bool
TrackFinanceApp::OnInit()
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

  TrackFinance* trackFinanceDialog = new TrackFinance(NULL);
  trackFinanceDialog->Show();
  SetTopWindow(trackFinanceDialog);

  return true;
}

/*
 * Cleanup for TrackFinanceApp
 */

int
TrackFinanceApp::OnExit()
{    
  return wxApp::OnExit();
}
