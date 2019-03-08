/*
** Name:        realtimemeasureapp.h
** Purpose:     Real-Time Chart with Measurement Cursors
** Author:      Ulrich Telle
** Created:     2018-07-02
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef REALTIMEMEASURE_H_
#define REALTIMEMEASURE_H_

/*!
 * Includes
 */

#include "wx/frame.h"
#include "wx/tglbtn.h"
#include "wx/scrolbar.h"

#include "wxchartviewer.h"
//#include "chartdir.h"

#include <vector>
#include <map>

/*!
 * Control identifiers
 */

#define ID_REALTIMEMEASURE 10000
#define ID_CHARTVIEWER     10001
#define ID_POINTER         10002
#define ID_TRACKLINE1      10003
#define ID_TRACKLINE2      10004
#define ID_SCROLLBAR       10005
#define ID_DATA_TIMER      10006
#define ID_UPDATE_TIMER    10007

#define SYMBOL_REALTIMEMEASURE_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_REALTIMEMEASURE_TITLE _("Real-Time Chart with Measurement Cursors")
#define SYMBOL_REALTIMEMEASURE_IDNAME ID_REALTIMEMEASURE
#define SYMBOL_REALTIMEMEASURE_SIZE wxSize(800, 420)
#define SYMBOL_REALTIMEMEASURE_POSITION wxDefaultPosition

/*!
 * RealTimeMeasure class declaration
 */

class RealTimeMeasure: public wxFrame
{    
  DECLARE_CLASS( RealTimeMeasure )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  RealTimeMeasure();
  RealTimeMeasure( wxWindow* parent, wxWindowID id = SYMBOL_REALTIMEMEASURE_IDNAME, const wxString& caption = SYMBOL_REALTIMEMEASURE_TITLE, const wxPoint& pos = SYMBOL_REALTIMEMEASURE_POSITION, const wxSize& size = SYMBOL_REALTIMEMEASURE_SIZE, long style = SYMBOL_REALTIMEMEASURE_STYLE );

  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_REALTIMEMEASURE_IDNAME, const wxString& caption = SYMBOL_REALTIMEMEASURE_TITLE, const wxPoint& pos = SYMBOL_REALTIMEMEASURE_POSITION, const wxSize& size = SYMBOL_REALTIMEMEASURE_SIZE, long style = SYMBOL_REALTIMEMEASURE_STYLE );

  /// Destructor
  ~RealTimeMeasure();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  void DrawChart(wxChartViewer* viewer);          // Draw chart

  // Draw track lines
  void TrackLineLabel(XYChart* c);
  void DrawTrackLine(XYChart* c, int i, std::map<std::string, double> &log);
  void DrawTrackDiff(XYChart* c, std::map<std::string, double> &log0, std::map<std::string, double> &log1);

  void UpdateControls(wxChartViewer* viewer);

  // Event handler declarations

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_POINTER
  void OnPointerClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_IN
  void OnZoomInClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_OUT
  void OnZoomOutClick( wxCommandEvent& event );

  void OnSave(wxCommandEvent& event);

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_TRACKLINE1
  void OnTrackline1Click(wxCommandEvent& event);

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_TRACKLINE2
  void OnTrackline2Click(wxCommandEvent& event);

  /// wxEVT_COMMAND_SCROLLBAR_UPDATED event handler for ID_SCROLLBAR
  void OnScrollbarUpdated(wxScrollEvent& event);

  /// Event handler for "get data" event
  void OnDataTimer(wxTimerEvent& event);

  /// Event handler for "chart update" event
  void OnChartUpdateTimer(wxTimerEvent& event);

  /// Event handler for mouse usage change
  void OnMouseUsageChanged(int mouseUsage);

  /// Event handler for viewport change event
  void OnViewPortChanged(wxCommandEvent& event);

  /// Event handler for scrollbar position changed
  void OnScrollBarChanged(int value);

  /// Event handler for mouse move over plot area
  void OnMouseMovePlotArea(wxCommandEvent& event);

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );

  /// Should we show tooltips?
  static bool ShowToolTips();

private:
  // The number of samples per data series used in this demo
  static const int sampleSize = 10000;

  // The initial full range is set to 60 seconds of data.
  static const int initialFullRange = 60;

  // The maximum zoom in is 10 seconds.
  static const int zoomInLimit = 10;

  double m_timeStamps[sampleSize];	// The timestamps for the data series
  double m_dataSeriesA[sampleSize];	// The values for the data series A
  double m_dataSeriesB[sampleSize];	// The values for the data series B

  int m_currentIndex;               // Index of the array position to which new values are added.

  wxDateTime m_nextDataTime;        // Used by the random number generator to generate realtime data.

  double m_firstChartTime;          // The first chart time
  double m_chartTimeLimit;          // The last chart time

  // The position of the track lines
  std::vector<int> m_trackLinePos;

  // Keep track of the mouse for dragging the track lines
  int m_nearestTrackLine;
  int m_nearestDistance;

  // User interface
  wxToggleButton* m_pointerButton;
  wxToggleButton* m_zoominButton;
  wxToggleButton* m_zoomoutButton;
  wxButton*       m_saveButton;
  wxCheckBox*     m_trackLine1;
  wxCheckBox*     m_trackLine2;

  wxColour        m_bgColour;
  wxScrollBar*    m_scrollBar;

  wxTimer* m_dataRateTimer;
  wxTimer* m_chartUpdateTimer;

  wxChartViewer* m_chartViewer;
};

#endif
    // REALTIMEMEASURE_H_
