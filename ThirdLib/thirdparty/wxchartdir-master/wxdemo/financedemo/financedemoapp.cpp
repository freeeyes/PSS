/*
** Name:        financedemoapp.cpp
** Purpose:     Financial Chart Demonstration
** Author:      Ulrich Telle
** Created:     2018-07-01
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

#include "financedemoapp.h"

/*
 * Application instance implementation
 */

IMPLEMENT_APP( FinanceDemoApp )

/*
 * FinanceDemoApp type definition
 */

IMPLEMENT_CLASS( FinanceDemoApp, wxApp )

/*
 * FinanceDemoApp event table definition
 */

BEGIN_EVENT_TABLE( FinanceDemoApp, wxApp )
END_EVENT_TABLE()

/*
 * Constructor for FinanceDemoApp
 */

FinanceDemoApp::FinanceDemoApp()
{
  Init();
}

/*
 * Member initialisation
 */

void FinanceDemoApp::Init()
{
}

/*
 * Initialisation for FinanceDemoApp
 */

bool FinanceDemoApp::OnInit()
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

  FinanceDemo* financeDemoDialog = new FinanceDemo(NULL);
  financeDemoDialog->Show();
  SetTopWindow(financeDemoDialog);

  return true;
}

/*
 * Cleanup for FinanceDemoApp
 */

int FinanceDemoApp::OnExit()
{    
  return wxApp::OnExit();
}
