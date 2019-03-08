/*
** Name:        zoomscrolltrack2app.h
** Purpose:     Zooming and Scrolling with Track Line (2)
** Author:      Ulrich Telle
** Created:     2018-07-02
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef ZOOMSCROLLTRACK2APP_H_
#define ZOOMSCROLLTRACK2APP_H_

/*!
 * Includes
 */

#include "wx/image.h"
#include "zoomscrolltrack2.h"

/*!
 * ZoomScrollTrack2App class declaration
 */

class ZoomScrollTrack2App: public wxApp
{    
  DECLARE_CLASS( ZoomScrollTrack2App )
  DECLARE_EVENT_TABLE()

public:
  /// Constructor
  ZoomScrollTrack2App();

  void Init();

  /// Initialises the application
  virtual bool OnInit();

  /// Called on exit
  virtual int OnExit();
};

/*!
 * Application instance declaration 
 */

DECLARE_APP(ZoomScrollTrack2App)

#endif
    // ZOOMSCROLLTRACK2APP_H_
