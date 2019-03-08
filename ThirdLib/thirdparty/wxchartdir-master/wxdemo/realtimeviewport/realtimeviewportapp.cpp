/*
** Name:        realtimeviewportapp.cpp
** Purpose:     Simple Zoom & Scroll sample application
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

#include "realtimeviewportapp.h"

/*
 * Application instance implementation
 */

IMPLEMENT_APP( RealTimeViewPortApp )

/*
 * RealTimeViewPortApp type definition
 */

IMPLEMENT_CLASS( RealTimeViewPortApp, wxApp )

/*
 * RealTimeViewPortApp event table definition
 */

BEGIN_EVENT_TABLE( RealTimeViewPortApp, wxApp )
END_EVENT_TABLE()

/*
 * Constructor for RealTimeViewPortApp
 */

RealTimeViewPortApp::RealTimeViewPortApp()
{
  Init();
}


/*
 * Member initialisation
 */

void
RealTimeViewPortApp::Init()
{
}

/*
 * Initialisation for RealTimeViewPortApp
 */

bool
RealTimeViewPortApp::OnInit()
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
  RealTimeViewPort* mainWindow = new RealTimeViewPort(NULL);
	mainWindow->Show(true);

  return true;
}

/*
 * Cleanup for RealTimeViewPortApp
 */

int RealTimeViewPortApp::OnExit()
{    
	return wxApp::OnExit();
}
