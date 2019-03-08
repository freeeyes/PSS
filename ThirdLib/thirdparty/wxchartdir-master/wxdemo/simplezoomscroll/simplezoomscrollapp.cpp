/*
** Name:        simplezoomscrollapp.cpp
** Purpose:     Simple Zooming and Scrolling
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

#include "simplezoomscrollapp.h"

/*
 * Application instance implementation
 */

IMPLEMENT_APP( SimpleZoomScrollApp )

/*
 * SimpleZoomScrollApp type definition
 */

IMPLEMENT_CLASS( SimpleZoomScrollApp, wxApp )

/*
 * SimpleZoomScrollApp event table definition
 */

BEGIN_EVENT_TABLE( SimpleZoomScrollApp, wxApp )
END_EVENT_TABLE()

/*
 * Constructor for SimpleZoomScrollApp
 */

SimpleZoomScrollApp::SimpleZoomScrollApp()
{
  Init();
}

/*
 * Member initialisation
 */

void
SimpleZoomScrollApp::Init()
{
}

/*
 * Initialisation for SimpleZoomScrollApp
 */

bool
SimpleZoomScrollApp::OnInit()
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
	SimpleZoomScroll* mainWindow = new SimpleZoomScroll(NULL);
	mainWindow->Show(true);

  return true;
}

/*
 * Cleanup for SimpleZoomScrollApp
 */

int
SimpleZoomScrollApp::OnExit()
{    
	return wxApp::OnExit();
}
