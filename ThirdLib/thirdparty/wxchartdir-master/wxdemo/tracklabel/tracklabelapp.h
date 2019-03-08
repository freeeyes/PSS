/*
** Name:        tracklabelapp.h
** Purpose:     Track Axis sample application
** Author:      Ulrich Telle
** Created:     2018-05-09
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef TRACKLABELAPP_H_
#define TRACKLABELAPP_H_

/*!
 * Includes
 */

#include "wx/image.h"

#include "tracklabel.h"

/*!
 * TrackLabelApp class declaration
 */

class TrackLabelApp: public wxApp
{    
  DECLARE_CLASS( TrackLabelApp )
  DECLARE_EVENT_TABLE()

public:
  /// Constructor
  TrackLabelApp();

  void Init();

  /// Initialises the application
  virtual bool OnInit();

  /// Called on exit
  virtual int OnExit();
};

/*!
 * Application instance declaration 
 */

DECLARE_APP(TrackLabelApp)

#endif
    // TRACKLABELAPP_H_
