/*
** Name:        tracklegend.h
** Purpose:     Track Legend sample application
** Author:      Ulrich Telle
** Created:     2018-05-09
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef TRACKLEGEND_H_
#define TRACKLEGEND_H_

/*!
 * Includes
 */

#include "wxchartviewer.h"
#include "chartdir.h"

/*!
 * Control identifiers
 */

#define ID_TRACKLEGEND 10000
#define ID_CHARTVIEWER 10001
#define SYMBOL_TRACKLEGEND_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_TRACKLEGEND_TITLE _("Track Line with Legend")
#define SYMBOL_TRACKLEGEND_IDNAME ID_TRACKLEGEND
#define SYMBOL_TRACKLEGEND_SIZE wxSize(400, 300)
#define SYMBOL_TRACKLEGEND_POSITION wxDefaultPosition

/*!
 * TrackLegend class declaration
 */

class TrackLegend: public wxFrame
{    
  DECLARE_DYNAMIC_CLASS( TrackLegend )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  TrackLegend();
  TrackLegend( wxWindow* parent, wxWindowID id = SYMBOL_TRACKLEGEND_IDNAME, const wxString& caption = SYMBOL_TRACKLEGEND_TITLE, const wxPoint& pos = SYMBOL_TRACKLEGEND_POSITION, const wxSize& size = SYMBOL_TRACKLEGEND_SIZE, long style = SYMBOL_TRACKLEGEND_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_TRACKLEGEND_IDNAME, const wxString& caption = SYMBOL_TRACKLEGEND_TITLE, const wxPoint& pos = SYMBOL_TRACKLEGEND_POSITION, const wxSize& size = SYMBOL_TRACKLEGEND_SIZE, long style = SYMBOL_TRACKLEGEND_STYLE );

  /// Destructor
  ~TrackLegend();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  void DrawChart(wxChartViewer* viewer);          // Draw chart
  void TrackLineLegend(XYChart* c, int mouseX);   // Draw Track Cursor
  void OnMouseMovePlotArea(wxCommandEvent& event);

  void OnExitClick(wxCommandEvent& event);

  /// wxEVT_CLOSE_WINDOW event handler for ID_TRACKLEGEND
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
    // TRACKLEGEND_H_
