/*
** Name:        trackbox.h
** Purpose:     Track Box sample application
** Author:      Ulrich Telle
** Created:     2018-05-09
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef TRACKBOX_H_
#define TRACKBOX_H_

/*!
 * Includes
 */

#include "wxchartviewer.h"
#include "chartdir.h"

/*!
 * Control identifiers
 */

#define ID_TRACKBOX    10000
#define ID_CHARTVIEWER 10001
#define SYMBOL_TRACKBOX_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_TRACKBOX_TITLE _("Track Box with Legend")
#define SYMBOL_TRACKBOX_IDNAME ID_TRACKBOX
#define SYMBOL_TRACKBOX_SIZE wxSize(400, 300)
#define SYMBOL_TRACKBOX_POSITION wxDefaultPosition

/*!
 * TrackBox class declaration
 */

class TrackBox: public wxFrame
{    
  DECLARE_DYNAMIC_CLASS( TrackBox )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  TrackBox();
  TrackBox( wxWindow* parent, wxWindowID id = SYMBOL_TRACKBOX_IDNAME, const wxString& caption = SYMBOL_TRACKBOX_TITLE, const wxPoint& pos = SYMBOL_TRACKBOX_POSITION, const wxSize& size = SYMBOL_TRACKBOX_SIZE, long style = SYMBOL_TRACKBOX_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_TRACKBOX_IDNAME, const wxString& caption = SYMBOL_TRACKBOX_TITLE, const wxPoint& pos = SYMBOL_TRACKBOX_POSITION, const wxSize& size = SYMBOL_TRACKBOX_SIZE, long style = SYMBOL_TRACKBOX_STYLE );

  /// Destructor
  ~TrackBox();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  void DrawChart(wxChartViewer* viewer);                      // Draw chart
  void TrackBoxLegend(XYChart* c, int mouseX, int mouseY);    // Draw Track Cursor
  void OnMouseMovePlotArea(wxCommandEvent& event);

  void OnExitClick(wxCommandEvent& event);

  /// wxEVT_CLOSE_WINDOW event handler for ID_TRACKBOX
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
    // TRACKBOX_H_
