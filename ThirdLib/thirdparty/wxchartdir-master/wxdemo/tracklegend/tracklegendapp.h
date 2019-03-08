/*
** Name:        tracklegendapp.h
** Purpose:     Track Legend sample application
** Author:      Ulrich Telle
** Created:     2018-05-09
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef TRACKLEGENDAPP_H_
#define TRACKLEGENDAPP_H_

/*!
 * Includes
 */

#include "wx/image.h"

#include "tracklegend.h"

/*!
 * TrackLegendApp class declaration
 */

class TrackLegendApp: public wxApp
{    
  DECLARE_CLASS( TrackLegendApp )
  DECLARE_EVENT_TABLE()

public:
  /// Constructor
  TrackLegendApp();

  void Init();

  /// Initialises the application
  virtual bool OnInit();

  /// Called on exit
  virtual int OnExit();
};

/*!
 * Application instance declaration 
 */

DECLARE_APP(TrackLegendApp)

#endif
    // TRACKLEGENDAPP_H_
