/*
** Name:        viewportcontroldemoapp.cpp
** Purpose:     Zooming and Scrolling with Viewport Control
** Author:      Ulrich Telle
** Created:     2018-05-09
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef VIEWPORTCONTROLDEMO_H_
#define VIEWPORTCONTROLDEMO_H_

/*!
 * Includes
 */

#include "wx/frame.h"
#include "wx/tglbtn.h"

#include "wxchartviewer.h"

/*!
 * Control identifiers
 */

#define ID_VIEWPORTCONTROLDEMO 10000
#define ID_CHARTVIEWER         10001
#define ID_VIEWPORT            10002
#define ID_POINTER             10003
#define SYMBOL_VIEWPORTCONTROLDEMO_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_VIEWPORTCONTROLDEMO_TITLE _("Zooming and Scrolling with Viewport Control")
#define SYMBOL_VIEWPORTCONTROLDEMO_IDNAME ID_VIEWPORTCONTROLDEMO
#define SYMBOL_VIEWPORTCONTROLDEMO_SIZE wxSize(800, 510)
#define SYMBOL_VIEWPORTCONTROLDEMO_POSITION wxDefaultPosition

/*!
 * ViewportControlDemo class declaration
 */

class ViewportControlDemo: public wxFrame
{    
  DECLARE_CLASS( ViewportControlDemo )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  ViewportControlDemo();
  ViewportControlDemo( wxWindow* parent, wxWindowID id = SYMBOL_VIEWPORTCONTROLDEMO_IDNAME, const wxString& caption = SYMBOL_VIEWPORTCONTROLDEMO_TITLE, const wxPoint& pos = SYMBOL_VIEWPORTCONTROLDEMO_POSITION, const wxSize& size = SYMBOL_VIEWPORTCONTROLDEMO_SIZE, long style = SYMBOL_VIEWPORTCONTROLDEMO_STYLE );

  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_VIEWPORTCONTROLDEMO_IDNAME, const wxString& caption = SYMBOL_VIEWPORTCONTROLDEMO_TITLE, const wxPoint& pos = SYMBOL_VIEWPORTCONTROLDEMO_POSITION, const wxSize& size = SYMBOL_VIEWPORTCONTROLDEMO_SIZE, long style = SYMBOL_VIEWPORTCONTROLDEMO_STYLE );

  /// Destructor
  ~ViewportControlDemo();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  void LoadData();                                // Load data into data arrays
  void InitChartViewer(wxChartViewer* viewer);     // Initialize the wxChartViewer
  void DrawChart(wxChartViewer* viewer);           // Draw chart
  void DrawFullChart(wxViewPortControl* vpc, wxChartViewer* viewer);   // Draw full chart
  void TrackLineLegend(XYChart* c, int mouseX);   // Draw Track Cursor

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_POINTER
  void OnPointerClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_IN
  void OnZoomInClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_OUT
  void OnZoomOutClick( wxCommandEvent& event );

  void OnViewPortChanged(wxCommandEvent& event);
  void OnMouseMovePlotArea(wxCommandEvent& event);
  void OnMouseUsageChanged(int mouseUsage);
  void OnSave(wxCommandEvent& event);

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );

  /// Should we show tooltips?
  static bool ShowToolTips();

  wxToggleButton* m_pointerButton;
  wxToggleButton* m_zoominButton;
  wxToggleButton* m_zoomoutButton;
  wxButton*       m_saveButton;
  wxColour        m_bgColour;

  // Data arrays for the scrollable / zoomable chart.
  // - In this demo, we just use a RanTable object to generate random data for the chart.
  RanSeries* m_ranSeries;
  DoubleArray m_timeStamps;
  DoubleArray m_dataSeriesA;
  DoubleArray m_dataSeriesB;
  DoubleArray m_dataSeriesC;

  wxChartViewer* m_chartViewer;
  wxViewPortControl* m_viewPortControl;
};

#endif
    // VIEWPORTCONTROLDEMO_H_
