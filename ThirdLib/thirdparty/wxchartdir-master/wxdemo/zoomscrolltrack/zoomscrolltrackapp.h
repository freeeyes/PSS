/*
** Name:        zoomscrolltrackapp.h
** Purpose:     Zooming and Scrolling with Track Line (1)
** Author:      Ulrich Telle
** Created:     2018-05-09
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef ZOOMSCROLLTRACKAPP_H_
#define ZOOMSCROLLTRACKAPP_H_

/*!
 * Includes
 */

#include "wx/image.h"
#include "zoomscrolltrack.h"

/*!
 * ZoomScrollTrackApp class declaration
 */

class ZoomScrollTrackApp: public wxApp
{    
  DECLARE_CLASS( ZoomScrollTrackApp )
  DECLARE_EVENT_TABLE()

public:
  /// Constructor
  ZoomScrollTrackApp();

  void Init();

  /// Initialises the application
  virtual bool OnInit();

  /// Called on exit
  virtual int OnExit();
};

/*!
 * Application instance declaration 
 */

DECLARE_APP(ZoomScrollTrackApp)

#endif
    // ZOOMSCROLLTRACKAPP_H_
