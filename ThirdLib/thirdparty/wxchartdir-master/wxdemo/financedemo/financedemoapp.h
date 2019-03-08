/*
** Name:        financedemoapp.h
** Purpose:     Financial Chart Demonstration
** Author:      Ulrich Telle
** Created:     2018-07-01
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef FINANCEDEMOAPP_H_
#define FINANCEDEMOAPP_H_

/*!
 * Includes
 */

#include "wx/image.h"

#include "financedemo.h"

/*!
 * FinanceDemoApp class declaration
 */

class FinanceDemoApp: public wxApp
{    
  DECLARE_CLASS( FinanceDemoApp )
  DECLARE_EVENT_TABLE()

public:
  /// Constructor
  FinanceDemoApp();

  void Init();

  /// Initialises the application
  virtual bool OnInit();

  /// Called on exit
  virtual int OnExit();
};

/*!
 * Application instance declaration 
 */

DECLARE_APP(FinanceDemoApp)

#endif
    // FINANCEDEMOAPP_H_
