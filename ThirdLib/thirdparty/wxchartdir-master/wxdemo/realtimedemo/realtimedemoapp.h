/*
** Name:        realtimedemoapp.h
** Purpose:     Simple Realtime Chart
** Author:      Ulrich Telle
** Created:     2018-07-02
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef REALTIMEDEMOAPP_H_
#define REALTIMEDEMOAPP_H_

/*!
 * Includes
 */

#include "wx/image.h"
#include "realtimedemo.h"

/*!
 * RealTimeDemoApp class declaration
 */

class RealTimeDemoApp: public wxApp
{    
  DECLARE_CLASS( RealTimeDemoApp )
  DECLARE_EVENT_TABLE()

public:
  /// Constructor
  RealTimeDemoApp();

  void Init();

  /// Initialises the application
  virtual bool OnInit();

  /// Called on exit
  virtual int OnExit();
};

/*!
 * Application instance declaration 
 */

DECLARE_APP(RealTimeDemoApp)

#endif
    // REALTIMEDEMOAPP_H_
