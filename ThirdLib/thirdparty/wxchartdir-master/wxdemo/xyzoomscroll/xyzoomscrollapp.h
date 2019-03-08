/*
** Name:        simplezoomscrollapp.h
** Purpose:     XY Zooming and Scrolling
** Author:      Ulrich Telle
** Created:     2018-07-02
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef XYZOOMSCROLLAPP_H_
#define XYZOOMSCROLLAPP_H_

/*!
 * Includes
 */

#include "wx/image.h"
#include "xyzoomscroll.h"

/*!
 * XyZoomScrollApp class declaration
 */

class XyZoomScrollApp: public wxApp
{    
  DECLARE_CLASS( XyZoomScrollApp )
  DECLARE_EVENT_TABLE()

public:
  /// Constructor
  XyZoomScrollApp();

  void Init();

  /// Initialises the application
  virtual bool OnInit();

  /// Called on exit
  virtual int OnExit();
};

/*!
 * Application instance declaration 
 */

DECLARE_APP(XyZoomScrollApp)

#endif
    // XYZOOMSCROLLAPP_H_
