/*
** Name:        realtimezoomscroll.h
** Purpose:     Realtime Chart with Zoom/Scroll and Track Line
** Author:      Ulrich Telle
** Created:     2018-05-09
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef REALTIMEZOOMSCROLL_H_
#define REALTIMEZOOMSCROLL_H_

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

#define ID_REALTIMEZOOMSCROLL 10000
#define ID_CHARTVIEWER        10001
#define ID_POINTER            10002
#define ID_SCROLLBAR          10003

#define ID_UPDATE_PERIOD      10004
#define ID_DATA_TIMER         10005
#define ID_UPDATE_TIMER       10006

#define SYMBOL_REALTIMEZOOMSCROLL_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_REALTIMEZOOMSCROLL_TITLE _("Realtime Chart with Zoom/Scroll and Track Line")
#define SYMBOL_REALTIMEZOOMSCROLL_IDNAME ID_REALTIMEZOOMSCROLL
#define SYMBOL_REALTIMEZOOMSCROLL_SIZE wxSize(800, 420)
#define SYMBOL_REALTIMEZOOMSCROLL_POSITION wxDefaultPosition

/*!
 * RealTimeZoomScroll class declaration
 */

class RealTimeZoomScroll: public wxFrame
{    
  DECLARE_CLASS( RealTimeZoomScroll )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  RealTimeZoomScroll();
  RealTimeZoomScroll( wxWindow* parent, wxWindowID id = SYMBOL_REALTIMEZOOMSCROLL_IDNAME, const wxString& caption = SYMBOL_REALTIMEZOOMSCROLL_TITLE, const wxPoint& pos = SYMBOL_REALTIMEZOOMSCROLL_POSITION, const wxSize& size = SYMBOL_REALTIMEZOOMSCROLL_SIZE, long style = SYMBOL_REALTIMEZOOMSCROLL_STYLE );

  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_REALTIMEZOOMSCROLL_IDNAME, const wxString& caption = SYMBOL_REALTIMEZOOMSCROLL_TITLE, const wxPoint& pos = SYMBOL_REALTIMEZOOMSCROLL_POSITION, const wxSize& size = SYMBOL_REALTIMEZOOMSCROLL_SIZE, long style = SYMBOL_REALTIMEZOOMSCROLL_STYLE );

  /// Destructor
  ~RealTimeZoomScroll();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  void DrawChart(wxChartViewer* viewer);           // Draw chart
  void TrackLineLabel(XYChart* c, int mouseX);    // Draw track cursor
  void UpdateControls(wxChartViewer* viewer);      // Update other controls as viewport changes

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_POINTER
  void OnPointerClick(wxCommandEvent& event);

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_IN
  void OnZoomInClick(wxCommandEvent& event);

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_OUT
  void OnZoomOutClick(wxCommandEvent& event);

  void OnSave(wxCommandEvent& event);

  /// wxEVT_COMMAND_SCROLLBAR_UPDATED event handler for ID_SCROLLBAR
  void OnScrollbarUpdated(wxScrollEvent& event);

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_UPDATE_PERIOD
  void OnUpdatePeriodSelected( wxCommandEvent& event );

  void OnMouseUsageChanged(int mouseUsage);

  void OnViewPortChanged(wxCommandEvent& event);

  void OnScrollBarChanged(int value);

  void OnMouseWheelChart(wxCommandEvent& event);

  void OnMouseMovePlotArea(wxCommandEvent& event);

  void OnUpdatePeriodChanged(const wxString& value);// The chart update timer interval has changed.

  void OnDataTimer(wxTimerEvent& event);
  void OnChartUpdateTimer(wxTimerEvent& event);

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
  wxScrollBar*    m_scrollBar;

  wxChoice* m_updatePeriod;
  wxTextCtrl* m_alphaValue;                   // Label to display the realtime value A
  wxTextCtrl* m_betaValue;                   // Label to display the realtime value B
  wxTextCtrl* m_gammaValue;                   // Label to display the realtime value C

  wxTimer* m_dataRateTimer;
  wxTimer* m_chartUpdateTimer;
    
  // The number of samples per data series used in this demo
  static const int sampleSize = 10000;

  // The initial full range is set to 60 seconds of data.
  static const int initialFullRange = 60;

  // The maximum zoom in is 10 seconds.
  static const int zoomInLimit = 10;

  double m_timeStamps[sampleSize];	// The timestamps for the data series
  double m_dataSeriesA[sampleSize];	// The values for the data series A
  double m_dataSeriesB[sampleSize];	// The values for the data series B
  double m_dataSeriesC[sampleSize];	// The values for the data series C

  int m_currentIndex;                 // Index of the array position to which new values are added.

  wxDateTime m_nextDataTime;           // Used by the random number generator to generate realtime data.

  wxChartViewer* m_chartViewer;
};

#endif
    // REALTIMEZOOMSCROLL_H_
