/*
** Name:        realtimeviewportapp.h
** Purpose:     Real-Time Chart with Viewport Control
** Author:      Ulrich Telle
** Created:     2018-07-02
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef REALTIMEVIEWPORTAPP_H_
#define REALTIMEVIEWPORTAPP_H_

/*!
 * Includes
 */

#include "wx/image.h"
#include "realtimeviewport.h"

/*!
 * RealTimeViewPortApp class declaration
 */

class RealTimeViewPortApp: public wxApp
{    
  DECLARE_CLASS( RealTimeViewPortApp )
  DECLARE_EVENT_TABLE()

public:
  /// Constructor
  RealTimeViewPortApp();

  void Init();

  /// Initialises the application
  virtual bool OnInit();

  /// Called on exit
  virtual int OnExit();
};

/*!
 * Application instance declaration 
 */

DECLARE_APP(RealTimeViewPortApp)

#endif
    // REALTIMEVIEWPORTAPP_H_
