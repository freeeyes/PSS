/*
** Name:        simplezoomscrollapp.h
** Purpose:     Simple Zooming and Scrolling
** Author:      Ulrich Telle
** Created:     2018-07-02
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef SIMPLEZOOMSCROLLAPP_H_
#define SIMPLEZOOMSCROLLAPP_H_

/*!
 * Includes
 */

#include "wx/image.h"
#include "simplezoomscroll.h"

/*!
 * SimpleZoomScrollApp class declaration
 */

class SimpleZoomScrollApp: public wxApp
{    
  DECLARE_CLASS( SimpleZoomScrollApp )
  DECLARE_EVENT_TABLE()

public:
  /// Constructor
  SimpleZoomScrollApp();

  void Init();

  /// Initialises the application
  virtual bool OnInit();

  /// Called on exit
  virtual int OnExit();
};

/*!
 * Application instance declaration 
 */

DECLARE_APP(SimpleZoomScrollApp)

#endif
    // SIMPLEZOOMSCROLLAPP_H_
