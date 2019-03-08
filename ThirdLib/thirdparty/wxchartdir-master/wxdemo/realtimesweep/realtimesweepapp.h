/*
** Name:        realtimesweepapp.h
** Purpose:     Real-Time Sweep Chart
** Author:      Ulrich Telle
** Created:     2018-07-02
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef REALTIMESWEEPAPP_H_
#define REALTIMESWEEPAPP_H_

/*!
 * Includes
 */

#include "wx/image.h"
#include "realtimesweep.h"

/*!
 * RealTimeSweepApp class declaration
 */

class RealTimeSweepApp: public wxApp
{    
  DECLARE_CLASS( RealTimeSweepApp )
  DECLARE_EVENT_TABLE()

public:
  /// Constructor
  RealTimeSweepApp();

  void Init();

  /// Initialises the application
  virtual bool OnInit();

  /// Called on exit
  virtual int OnExit();
};

/*!
 * Application instance declaration 
 */

DECLARE_APP(RealTimeSweepApp)

#endif
    // REALTIMESWEEPAPP_H_
