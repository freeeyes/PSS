/*
** Name:        helloworldapp.cpp
** Purpose:     Hello World sample application
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

#include "helloworldapp.h"

/*
 * Application instance implementation
 */

IMPLEMENT_APP( HelloWorldApp )

/*
 * HelloWorldApp type definition
 */

IMPLEMENT_CLASS( HelloWorldApp, wxApp )

/*
 * HelloWorldApp event table definition
 */

BEGIN_EVENT_TABLE( HelloWorldApp, wxApp )
END_EVENT_TABLE()

/*
 * Constructor for HelloWorldApp
 */

HelloWorldApp::HelloWorldApp()
{
  Init();
}

/*
 * Member initialisation
 */

void
HelloWorldApp::Init()
{
}

/*
 * Initialisation for HelloWorldApp
 */

bool
HelloWorldApp::OnInit()
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

  HelloWorld* helloWorldDialog = new HelloWorld(NULL);
  helloWorldDialog->Show();
  SetTopWindow(helloWorldDialog);

  return true;
}

/*
 * Cleanup for HelloWorldApp
 */

int
HelloWorldApp::OnExit()
{    
  return wxApp::OnExit();
}
