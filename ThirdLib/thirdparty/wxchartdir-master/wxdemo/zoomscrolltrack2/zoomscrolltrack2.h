/*
** Name:        zoomscrolltrack2.h
** Purpose:     Zooming and Scrolling with Track Line (2)
** Author:      Ulrich Telle
** Created:     2018-07-02
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef ZOOMSCROLLTRACK2_H_
#define ZOOMSCROLLTRACK2_H_

/*!
 * Includes
 */

#include "wx/frame.h"
#include "wx/tglbtn.h"
#include "wx/scrolbar.h"
#include "wx/datectrl.h"
#include "wx/dateevt.h"

#include "wxchartviewer.h"
#include "chartdir.h"

/*!
 * Control identifiers
 */

#define ID_ZOOMSCROLLTRACK2 10000
#define ID_CHARTVIEWER      10001
#define ID_POINTER          10002
#define ID_SCROLLBAR        10003
#define ID_DATE_START       10004
#define ID_DATE_END         10005
#define SYMBOL_ZOOMSCROLLTRACK2_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_ZOOMSCROLLTRACK2_TITLE _("Zooming and Scrolling with Track Line (2)")
#define SYMBOL_ZOOMSCROLLTRACK2_IDNAME ID_ZOOMSCROLLTRACK2
#define SYMBOL_ZOOMSCROLLTRACK2_SIZE wxSize(700, 300)
#define SYMBOL_ZOOMSCROLLTRACK2_POSITION wxDefaultPosition

/*!
 * ZoomScrollTrack2 class declaration
 */

class ZoomScrollTrack2: public wxFrame
{    
  DECLARE_CLASS( ZoomScrollTrack2 )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  ZoomScrollTrack2();
  ZoomScrollTrack2( wxWindow* parent, wxWindowID id = SYMBOL_ZOOMSCROLLTRACK2_IDNAME, const wxString& caption = SYMBOL_ZOOMSCROLLTRACK2_TITLE, const wxPoint& pos = SYMBOL_ZOOMSCROLLTRACK2_POSITION, const wxSize& size = SYMBOL_ZOOMSCROLLTRACK2_SIZE, long style = SYMBOL_ZOOMSCROLLTRACK2_STYLE );

  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_ZOOMSCROLLTRACK2_IDNAME, const wxString& caption = SYMBOL_ZOOMSCROLLTRACK2_TITLE, const wxPoint& pos = SYMBOL_ZOOMSCROLLTRACK2_POSITION, const wxSize& size = SYMBOL_ZOOMSCROLLTRACK2_SIZE, long style = SYMBOL_ZOOMSCROLLTRACK2_STYLE );

  /// Destructor
  ~ZoomScrollTrack2();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  void LoadData();                                // Load data into data arrays

  void InitChartViewer(wxChartViewer* viewer);    // Initialize the wxChartViewer

  void UpdateControls(wxChartViewer* viewer);

  void DrawChart(wxChartViewer* viewer);          // Draw chart

  void TrackLineLabel(XYChart *c, int mouseX);   // Draw Track Cursor

  // Event handler declarations

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_POINTER
  void OnPointerClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_IN
  void OnZoomInClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_OUT
  void OnZoomOutClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_SCROLLBAR_UPDATED event handler for ID_SCROLLBAR
  void OnScrollbarUpdated(wxScrollEvent& event);

  /// wxEVT_DATE_CHANGED event handler for ID_DATE_START
  void OnStartDateChanged(wxDateEvent& event);

  /// wxEVT_DATE_CHANGED event handler for ID_DATE_END
  void OnEndDateChanged(wxDateEvent& event);

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

  wxToggleButton*   m_pointerButton;
  wxToggleButton*   m_zoominButton;
  wxToggleButton*   m_zoomoutButton;
  wxColour          m_bgColour;
  wxDatePickerCtrl* m_startDate;
  wxDatePickerCtrl* m_endDate;
  wxScrollBar*      m_scrollBar;

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
    // ZOOMSCROLLTRACK2_H_
