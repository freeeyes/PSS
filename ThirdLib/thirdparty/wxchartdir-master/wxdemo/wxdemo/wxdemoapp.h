/*
** Name:        wxdemoapp.h
** Purpose:     wxWidgets Demo sample application
** Author:      Ulrich Telle
** Created:     2018-05-09
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef WXDEMOAPP_H_
#define WXDEMOAPP_H_

/*!
 * Includes
 */

#include "wx/image.h"

#include "wxdemo.h"

/*!
 * WxDemoApp class declaration
 */

class WxDemoApp: public wxApp
{    
  DECLARE_CLASS( WxDemoApp )
  DECLARE_EVENT_TABLE()

public:
  /// Constructor
  WxDemoApp();

  void Init();

  /// Initialises the application
  virtual bool OnInit();

  /// Called on exit
  virtual int OnExit();
};

/*!
 * Application instance declaration 
 */

DECLARE_APP(WxDemoApp)

#endif
    // WXDEMOAPP_H_
