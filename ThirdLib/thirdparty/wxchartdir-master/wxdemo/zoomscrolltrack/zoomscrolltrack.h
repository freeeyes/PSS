/*
** Name:        zoomscrolltrack.h
** Purpose:     Zooming and Scrolling with Track Line (1)
** Author:      Ulrich Telle
** Created:     2018-05-09
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef ZOOMSCROLLTRACK_H_
#define ZOOMSCROLLTRACK_H_

/*!
 * Includes
 */

#include "wx/frame.h"
#include "wx/tglbtn.h"
#include "wx/scrolbar.h"

#include "wxchartviewer.h"
#include "chartdir.h"

/*!
 * Control identifiers
 */

#define ID_ZOOMSCROLLTRACK 10000
#define ID_CHARTVIEWER     10001
#define ID_POINTER         10002
#define ID_SCROLLBAR       10003
#define SYMBOL_ZOOMSCROLLTRACK_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_ZOOMSCROLLTRACK_TITLE _("Zooming and Scrolling with Track Line (1)")
#define SYMBOL_ZOOMSCROLLTRACK_IDNAME ID_ZOOMSCROLLTRACK
#define SYMBOL_ZOOMSCROLLTRACK_SIZE wxSize(800, 420)
#define SYMBOL_ZOOMSCROLLTRACK_POSITION wxDefaultPosition

/*!
 * ZoomScrollTrack class declaration
 */

class ZoomScrollTrack: public wxFrame
{    
  DECLARE_CLASS( ZoomScrollTrack )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  ZoomScrollTrack();
  ZoomScrollTrack( wxWindow* parent, wxWindowID id = SYMBOL_ZOOMSCROLLTRACK_IDNAME, const wxString& caption = SYMBOL_ZOOMSCROLLTRACK_TITLE, const wxPoint& pos = SYMBOL_ZOOMSCROLLTRACK_POSITION, const wxSize& size = SYMBOL_ZOOMSCROLLTRACK_SIZE, long style = SYMBOL_ZOOMSCROLLTRACK_STYLE );

  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_ZOOMSCROLLTRACK_IDNAME, const wxString& caption = SYMBOL_ZOOMSCROLLTRACK_TITLE, const wxPoint& pos = SYMBOL_ZOOMSCROLLTRACK_POSITION, const wxSize& size = SYMBOL_ZOOMSCROLLTRACK_SIZE, long style = SYMBOL_ZOOMSCROLLTRACK_STYLE );

  /// Destructor
  ~ZoomScrollTrack();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  void LoadData();                                // Load data into data arrays
  void InitChartViewer(wxChartViewer* viewer);    // Initialize the wxChartViewer
  void UpdateControls(wxChartViewer* viewer);
  void DrawChart(wxChartViewer* viewer);          // Draw chart
  void TrackLineLegend(XYChart *c, int mouseX);   // Draw Track Cursor

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_POINTER
  void OnPointerClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_IN
  void OnZoomInClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_OUT
  void OnZoomOutClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_SCROLLBAR_UPDATED event handler for ID_SCROLLBAR
  void OnScrollbarUpdated(wxScrollEvent& event);

  void OnMouseUsageChanged(int mouseUsage);
  void OnViewPortChanged(wxCommandEvent& event);
  void OnScrollBarChanged(int value);
  void OnMouseWheelChart(wxCommandEvent& event);
  void OnMouseMovePlotArea(wxCommandEvent& event);

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );

  /// Should we show tooltips?
  static bool ShowToolTips();

  wxToggleButton* m_pointerButton;
  wxToggleButton* m_zoominButton;
  wxToggleButton* m_zoomoutButton;
  wxColour        m_bgColour;
  wxScrollBar*    m_scrollBar;

  // Data arrays for the scrollable / zoomable chart.
  // - In this demo, we just use a RanTable object to generate random data for the chart.
  RanSeries*  m_ranSeries;
  DoubleArray m_timeStamps;
  DoubleArray m_dataSeriesA;
  DoubleArray m_dataSeriesB;
  DoubleArray m_dataSeriesC;

  wxChartViewer* m_chartViewer;
};

#endif
    // ZOOMSCROLLTRACK_H_
