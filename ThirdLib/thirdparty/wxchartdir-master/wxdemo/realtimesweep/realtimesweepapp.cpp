/*
** Name:        realtimesweepapp.cpp
** Purpose:     Real-Time Sweep Chart
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

#include "realtimesweepapp.h"

/*
 * Application instance implementation
 */

IMPLEMENT_APP( RealTimeSweepApp )

/*
 * RealTimeSweepApp type definition
 */

IMPLEMENT_CLASS( RealTimeSweepApp, wxApp )

/*
 * RealTimeSweepApp event table definition
 */

BEGIN_EVENT_TABLE( RealTimeSweepApp, wxApp )
END_EVENT_TABLE()

/*
 * Constructor for RealTimeSweepApp
 */

RealTimeSweepApp::RealTimeSweepApp()
{
  Init();
}


/*
 * Member initialisation
 */

void RealTimeSweepApp::Init()
{
}

/*
 * Initialisation for RealTimeSweepApp
 */

bool
RealTimeSweepApp::OnInit()
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
	RealTimeSweep* mainWindow = new RealTimeSweep(NULL);
	mainWindow->Show(true);

  return true;
}

/*
 * Cleanup for RealTimeSweepApp
 */

int
RealTimeSweepApp::OnExit()
{    
	return wxApp::OnExit();
}
