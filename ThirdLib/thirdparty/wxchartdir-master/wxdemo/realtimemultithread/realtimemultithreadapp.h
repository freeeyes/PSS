/*
** Name:        realtimemultithreadapp.h
** Purpose:     Multithreading Real-Time Chart with Zoom/Scroll and Track Line
** Author:      Ulrich Telle
** Created:     2018-07-02
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef REALTIMEMULTITHREADAPP_H_
#define REALTIMEMULTITHREADAPP_H_

/*!
 * Includes
 */

#include "wx/image.h"
#include "realtimemultithread.h"

/*!
 * RealTimeMultiThreadApp class declaration
 */

class RealTimeMultiThreadApp: public wxApp
{    
  DECLARE_CLASS( RealTimeMultiThreadApp )
  DECLARE_EVENT_TABLE()

public:
  /// Constructor
  RealTimeMultiThreadApp();

  void Init();

  /// Initialises the application
  virtual bool OnInit();

  /// Called on exit
  virtual int OnExit();
};

/*!
 * Application instance declaration 
 */

DECLARE_APP(RealTimeMultiThreadApp)

#endif
    // REALTIMEMULTITHREADAPP_H_
