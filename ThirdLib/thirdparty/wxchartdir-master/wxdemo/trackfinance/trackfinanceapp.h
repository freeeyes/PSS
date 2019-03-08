/*
** Name:        trackfinanceapp.h
** Purpose:     Track Finance sample application
** Author:      Ulrich Telle
** Created:     2018-05-09
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef TRACKFINANCEAPP_H_
#define TRACKFINANCEAPP_H_

/*!
 * Includes
 */

#include "wx/image.h"

#include "trackfinance.h"

/*!
 * TrackFinanceApp class declaration
 */

class TrackFinanceApp: public wxApp
{    
  DECLARE_CLASS( TrackFinanceApp )
  DECLARE_EVENT_TABLE()

public:
  /// Constructor
  TrackFinanceApp();

  void Init();

  /// Initialises the application
  virtual bool OnInit();

  /// Called on exit
  virtual int OnExit();
};

/*!
 * Application instance declaration 
 */

DECLARE_APP(TrackFinanceApp)

#endif
    // TRACKFINANCEAPP_H_
