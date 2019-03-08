/*
** Name:        viewportcontroldemoapp.h
** Purpose:     Zooming and Scrolling with Viewport Control sample application
** Author:      Ulrich Telle
** Created:     2018-07-02
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef VIEWPORTCONTROLDEMOAPP_H_
#define VIEWPORTCONTROLDEMOAPP_H_

/*!
 * Includes
 */

#include "wx/image.h"
#include "viewportcontroldemo.h"

/*!
 * ViewportControlDemoApp class declaration
 */

class ViewportControlDemoApp: public wxApp
{    
  DECLARE_CLASS( ViewportControlDemoApp )
  DECLARE_EVENT_TABLE()

public:
  /// Constructor
  ViewportControlDemoApp();

  void Init();

  /// Initialises the application
  virtual bool OnInit();

  /// Called on exit
  virtual int OnExit();
};

/*!
 * Application instance declaration 
 */

DECLARE_APP(ViewportControlDemoApp)

#endif
    // VIEWPORTCONTROLDEMOAPP_H_
