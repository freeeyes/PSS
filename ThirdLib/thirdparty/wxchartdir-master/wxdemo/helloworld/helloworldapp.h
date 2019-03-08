/*
** Name:        helloworldapp.h
** Purpose:     Hello World sample application
** Author:      Ulrich Telle
** Created:     2018-05-09
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef HELLOWORLDAPP_H_
#define HELLOWORLDAPP_H_

/*!
 * Includes
 */

#include "wx/image.h"

#include "helloworld.h"

/*!
 * HelloWorldApp class declaration
 */

class HelloWorldApp: public wxApp
{    
  DECLARE_CLASS( HelloWorldApp )
  DECLARE_EVENT_TABLE()

public:
  /// Constructor
  HelloWorldApp();

  void Init();

  /// Initialises the application
  virtual bool OnInit();

  /// Called on exit
  virtual int OnExit();
};

/*!
 * Application instance declaration 
 */

DECLARE_APP(HelloWorldApp)

#endif
    // HELLOWORLDAPP_H_
