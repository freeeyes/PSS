/*
** Name:        realtimetrackapp.h
** Purpose:     Realtime Chart with Track Line
** Author:      Ulrich Telle
** Created:     2018-07-02
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef REALTIMETRACKAPP_H_
#define REALTIMETRACKAPP_H_

/*!
 * Includes
 */

#include "wx/image.h"
#include "realtimetrack.h"

/*!
 * RealTimeTrackApp class declaration
 */

class RealTimeTrackApp: public wxApp
{    
  DECLARE_CLASS( RealTimeTrackApp )
  DECLARE_EVENT_TABLE()

public:
  /// Constructor
  RealTimeTrackApp();

  void Init();

  /// Initialises the application
  virtual bool OnInit();

  /// Called on exit
  virtual int OnExit();
};

/*!
 * Application instance declaration 
 */

DECLARE_APP(RealTimeTrackApp)

#endif
    // REALTIMETRACKAPP_H_
