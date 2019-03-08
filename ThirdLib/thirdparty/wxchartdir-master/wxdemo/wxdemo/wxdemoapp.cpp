/*
** Name:        wxdemoapp.cpp
** Purpose:     wxWidgets Demo sample application
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

#include "wxdemoapp.h"

/*
 * Application instance implementation
 */

IMPLEMENT_APP( WxDemoApp )

/*
 * WxDemoApp type definition
 */

IMPLEMENT_CLASS( WxDemoApp, wxApp )

/*
 * WxDemoApp event table definition
 */

BEGIN_EVENT_TABLE( WxDemoApp, wxApp )
END_EVENT_TABLE()

/*
 * Constructor for WxDemoApp
 */

WxDemoApp::WxDemoApp()
{
  Init();
}

/*
 * Member initialisation
 */

void
WxDemoApp::Init()
{
}

/*
 * Initialisation for WxDemoApp
 */

bool
WxDemoApp::OnInit()
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

  WxDemo* wxDemoDialog = new WxDemo(NULL);
  wxDemoDialog->Show();
  SetTopWindow(wxDemoDialog);

  return true;
}

/*
 * Cleanup for WxDemoApp
 */

int
WxDemoApp::OnExit()
{    
  return wxApp::OnExit();
}
