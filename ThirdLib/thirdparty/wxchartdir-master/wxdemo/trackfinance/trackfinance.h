/*
** Name:        trackfinance.h
** Purpose:     Track Finance sample application
** Author:      Ulrich Telle
** Created:     2018-05-09
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef TRACKFINANCE_H_
#define TRACKFINANCE_H_

/*!
 * Includes
 */

#include "wxchartviewer.h"
#include "chartdir.h"

/*!
 * Control identifiers
 */

#define ID_TRACKFINANCE 10000
#define ID_CHARTVIEWER  10001
#define SYMBOL_TRACKFINANCE_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_TRACKFINANCE_TITLE _("Finance Chart Track Line")
#define SYMBOL_TRACKFINANCE_IDNAME ID_TRACKFINANCE
#define SYMBOL_TRACKFINANCE_SIZE wxSize(400, 300)
#define SYMBOL_TRACKFINANCE_POSITION wxDefaultPosition

/*!
 * TrackFinance class declaration
 */

class TrackFinance: public wxFrame
{    
  DECLARE_DYNAMIC_CLASS( TrackFinance )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  TrackFinance();
  TrackFinance( wxWindow* parent, wxWindowID id = SYMBOL_TRACKFINANCE_IDNAME, const wxString& caption = SYMBOL_TRACKFINANCE_TITLE, const wxPoint& pos = SYMBOL_TRACKFINANCE_POSITION, const wxSize& size = SYMBOL_TRACKFINANCE_SIZE, long style = SYMBOL_TRACKFINANCE_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_TRACKFINANCE_IDNAME, const wxString& caption = SYMBOL_TRACKFINANCE_TITLE, const wxPoint& pos = SYMBOL_TRACKFINANCE_POSITION, const wxSize& size = SYMBOL_TRACKFINANCE_SIZE, long style = SYMBOL_TRACKFINANCE_STYLE );

  /// Destructor
  ~TrackFinance();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  void DrawChart(wxChartViewer* viewer);           // Draw chart
  void DrawTrackFinance(MultiChart* m, int mouseX);    // Draw Track Cursor
  void OnMouseMovePlotArea(wxCommandEvent& event);

  void OnExitClick(wxCommandEvent& event);

  /// wxEVT_CLOSE_WINDOW event handler for ID_TRACKFINANCE
  void OnCloseWindow(wxCloseEvent& event);

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );

  /// Should we show tooltips?
  static bool ShowToolTips();

  wxChartViewer* m_chartViewer;
};

#endif
    // TRACKFINANCE_H_
