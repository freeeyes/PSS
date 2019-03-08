/*
** Name:        trackboxapp.h
** Purpose:     Track Box sample application
** Author:      Ulrich Telle
** Created:     2018-05-09
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef TRACKBOXAPP_H_
#define TRACKBOXAPP_H_

/*!
 * Includes
 */

#include "wx/image.h"

#include "trackbox.h"

/*!
 * TrackBoxApp class declaration
 */

class TrackBoxApp: public wxApp
{    
  DECLARE_CLASS( TrackBoxApp )
  DECLARE_EVENT_TABLE()

public:
  /// Constructor
  TrackBoxApp();

  void Init();

  /// Initialises the application
  virtual bool OnInit();

  /// Called on exit
  virtual int OnExit();
};

/*!
 * Application instance declaration 
 */

DECLARE_APP(TrackBoxApp)

#endif
    // TRACKBOXAPP_H_
