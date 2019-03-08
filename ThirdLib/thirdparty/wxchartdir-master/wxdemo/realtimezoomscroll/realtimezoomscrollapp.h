/*
** Name:        realtimezoomscrollapp.h
** Purpose:     Zoom & Scroll sample application
** Author:      Ulrich Telle
** Created:     2018-07-02
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef REALTIMEZOOMSCROLLAPP_H_
#define REALTIMEZOOMSCROLLAPP_H_

/*!
 * Includes
 */

#include "wx/image.h"
#include "realtimezoomscroll.h"

/*!
 * RealTimeZoomScrollApp class declaration
 */

class RealTimeZoomScrollApp: public wxApp
{    
  DECLARE_CLASS( RealTimeZoomScrollApp )
  DECLARE_EVENT_TABLE()

public:
  /// Constructor
  RealTimeZoomScrollApp();

  void Init();

  /// Initialises the application
  virtual bool OnInit();

  /// Called on exit
  virtual int OnExit();
};

/*!
 * Application instance declaration 
 */

DECLARE_APP(RealTimeZoomScrollApp)

#endif
    // REALTIMEZOOMSCROLLAPP_H_
