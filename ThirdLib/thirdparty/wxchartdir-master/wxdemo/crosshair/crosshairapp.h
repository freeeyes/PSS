/*
** Name:        crosshairapp.h
** Purpose:     Crosshair with Axis Labels sample application
** Author:      Ulrich Telle
** Created:     2018-05-09
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef CROSSHAIRAPP_H_
#define CROSSHAIRAPP_H_

/*!
 * Includes
 */

#include "wx/image.h"

#include "crosshair.h"

/*!
 * CrossHairApp class declaration
 */

class CrossHairApp: public wxApp
{    
  DECLARE_CLASS( CrossHairApp )
  DECLARE_EVENT_TABLE()

public:
  /// Constructor
  CrossHairApp();

  void Init();

  /// Initializes the application
  virtual bool OnInit();
};

/*!
 * Application instance declaration 
 */

DECLARE_APP(CrossHairApp)

#endif
    // CROSSHAIRAPP_H_
