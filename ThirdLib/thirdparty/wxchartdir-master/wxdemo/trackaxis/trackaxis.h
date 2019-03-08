/*
** Name:        trackaxisapp.h
** Purpose:     Track Axis sample application
** Author:      Ulrich Telle
** Created:     2018-05-09
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef TRACKAXIS_H_
#define TRACKAXIS_H_

/*!
 * Includes
 */

#include "wxchartviewer.h"

/*!
 * Control identifiers
 */

#define ID_TRACKAXIS   10000
#define ID_CHARTVIEWER 10001
#define SYMBOL_TRACKAXIS_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_TRACKAXIS_TITLE _("Track Line with Axis Labels")
#define SYMBOL_TRACKAXIS_IDNAME ID_TRACKAXIS
#define SYMBOL_TRACKAXIS_SIZE wxSize(400, 300)
#define SYMBOL_TRACKAXIS_POSITION wxDefaultPosition

/*!
 * TrackAxis class declaration
 */

class TrackAxis: public wxFrame
{    
  DECLARE_DYNAMIC_CLASS( TrackAxis )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  TrackAxis();
  TrackAxis( wxWindow* parent, wxWindowID id = SYMBOL_TRACKAXIS_IDNAME, const wxString& caption = SYMBOL_TRACKAXIS_TITLE, const wxPoint& pos = SYMBOL_TRACKAXIS_POSITION, const wxSize& size = SYMBOL_TRACKAXIS_SIZE, long style = SYMBOL_TRACKAXIS_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_TRACKAXIS_IDNAME, const wxString& caption = SYMBOL_TRACKAXIS_TITLE, const wxPoint& pos = SYMBOL_TRACKAXIS_POSITION, const wxSize& size = SYMBOL_TRACKAXIS_SIZE, long style = SYMBOL_TRACKAXIS_STYLE );

  /// Destructor
  ~TrackAxis();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  void DrawChart(wxChartViewer* viewer);          // Draw chart
  void TrackLineAxis(XYChart* c, int mouseX);     // Draw Track Cursor
  void OnMouseMovePlotArea(wxCommandEvent& event);

  void OnExitClick(wxCommandEvent& event);

  /// wxEVT_CLOSE_WINDOW event handler for ID_TRACKAXIS
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
    // _TRACKAXIS_H_
