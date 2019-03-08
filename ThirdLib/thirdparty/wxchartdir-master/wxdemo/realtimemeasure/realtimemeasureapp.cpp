/*
** Name:        realtimemeasureapp.cpp
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

#include "realtimemeasureapp.h"

/*
 * Application instance implementation
 */

IMPLEMENT_APP( RealTimeMeasureApp )

/*
 * RealTimeMeasureApp type definition
 */

IMPLEMENT_CLASS( RealTimeMeasureApp, wxApp )

/*
 * RealTimeMeasureApp event table definition
 */

BEGIN_EVENT_TABLE( RealTimeMeasureApp, wxApp )
END_EVENT_TABLE()

/*
 * Constructor for RealTimeMeasureApp
 */

RealTimeMeasureApp::RealTimeMeasureApp()
{
  Init();
}


/*
 * Member initialisation
 */

void
RealTimeMeasureApp::Init()
{
}

/*
 * Initialisation for RealTimeMeasureApp
 */

bool
RealTimeMeasureApp::OnInit()
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
	RealTimeMeasure* mainWindow = new RealTimeMeasure(NULL);
	mainWindow->Show(true);

  return true;
}

/*
 * Cleanup for RealTimeMeasureApp
 */

int
RealTimeMeasureApp::OnExit()
{    
	return wxApp::OnExit();
}
