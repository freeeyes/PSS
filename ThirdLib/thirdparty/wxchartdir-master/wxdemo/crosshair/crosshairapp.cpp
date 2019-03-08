/*
** Name:        crosshairapp.cpp
** Purpose:     Crosshair with Axis Labels sample application
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

#include "crosshairapp.h"

// Application instance implementation

IMPLEMENT_APP( CrossHairApp )

// CrossHairApp type definition

IMPLEMENT_CLASS( CrossHairApp, wxApp )


// CrossHairApp event table definition

BEGIN_EVENT_TABLE( CrossHairApp, wxApp )
END_EVENT_TABLE()

// Constructor for CrossHairApp

CrossHairApp::CrossHairApp()
{
  Init();
}


// Member initialisation
void
CrossHairApp::Init()
{
}

// Initialisation for CrossHairApp
bool
CrossHairApp::OnInit()
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

  CrossHair* crossHairDialog = new CrossHair(NULL);
  crossHairDialog->Show();
  SetTopWindow(crossHairDialog);

  return true;
}
