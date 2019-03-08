/*
** Name:        trackaxisapp.h
** Purpose:     Track Axis sample application
** Author:      Ulrich Telle
** Created:     2018-05-09
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef TRACKAXISAPP_H_
#define TRACKAXISAPP_H_

/*!
 * Includes
 */

#include "wx/image.h"

#include "trackaxis.h"

/*!
 * TrackAxisApp class declaration
 */

class TrackAxisApp: public wxApp
{    
  DECLARE_CLASS( TrackAxisApp )
  DECLARE_EVENT_TABLE()

public:
  /// Constructor
  TrackAxisApp();

  void Init();

  /// Initialises the application
  virtual bool OnInit();

  /// Called on exit
  virtual int OnExit();
};

/*!
 * Application instance declaration 
 */

DECLARE_APP(TrackAxisApp)

#endif
    // TRACKAXISAPP_H_
