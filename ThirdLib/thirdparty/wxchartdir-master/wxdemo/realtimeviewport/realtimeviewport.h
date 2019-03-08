/*
** Name:        realtimeviewport.h
** Purpose:     Real-Time Chart with Viewport Control
** Author:      Ulrich Telle
** Created:     2018-07-02
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef REALTIMEVIEWPORT_H_
#define REALTIMEVIEWPORT_H_

/*!
 * Includes
 */

#include "wx/frame.h"
#include "wx/tglbtn.h"

#include "wxchartviewer.h"
#include "chartdir.h"
#include "randomwalk.h"
#include "wxdoublebufferedqueue.h"

/*!
 * Control identifiers
 */

#define ID_REALTIMEVIEWPORT 10000
#define ID_CHARTVIEWER      10001
#define ID_VIEWPORT         10002
#define ID_POINTER          10003
#define ID_UPDATE_TIMER     10004
#define SYMBOL_REALTIMEVIEWPORT_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_REALTIMEVIEWPORT_TITLE _("Real-Time Chart with Viewport Control")
#define SYMBOL_REALTIMEVIEWPORT_IDNAME ID_REALTIMEVIEWPORT
#define SYMBOL_REALTIMEVIEWPORT_SIZE wxSize(600, 250)
#define SYMBOL_REALTIMEVIEWPORT_POSITION wxDefaultPosition

/*!
 * RealTimeViewPort class declaration
 */

class RealTimeViewPort: public wxFrame
{    
  DECLARE_CLASS( RealTimeViewPort )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  RealTimeViewPort();
  RealTimeViewPort( wxWindow* parent, wxWindowID id = SYMBOL_REALTIMEVIEWPORT_IDNAME, const wxString& caption = SYMBOL_REALTIMEVIEWPORT_TITLE, const wxPoint& pos = SYMBOL_REALTIMEVIEWPORT_POSITION, const wxSize& size = SYMBOL_REALTIMEVIEWPORT_SIZE, long style = SYMBOL_REALTIMEVIEWPORT_STYLE );

  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_REALTIMEVIEWPORT_IDNAME, const wxString& caption = SYMBOL_REALTIMEVIEWPORT_TITLE, const wxPoint& pos = SYMBOL_REALTIMEVIEWPORT_POSITION, const wxSize& size = SYMBOL_REALTIMEVIEWPORT_SIZE, long style = SYMBOL_REALTIMEVIEWPORT_STYLE );

  /// Destructor
  ~RealTimeViewPort();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  // Draw chart
  void DrawChart(wxChartViewer* viewer);
  
  // Draw full chart
  void DrawFullChart(wxViewPortControl* vpc);
  
  // Draw track cursor
  double TrackLineLabel(XYChart* c, int mouseX);

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_POINTER
  void OnPointerClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_IN
  void OnZoomInClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_OUT
  void OnZoomOutClick( wxCommandEvent& event );

  void OnChartUpdateTimer(wxTimerEvent& event);

  void OnMouseUsageChanged(int mouseUsage);
  void OnSave(wxCommandEvent& event);
  void OnViewPortChanged(wxCommandEvent& event);
  void OnMouseMovePlotArea(wxCommandEvent& event);

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );

  /// Should we show tooltips?
  static bool ShowToolTips();

private:
  // The random data source
  RandomWalk* m_dataSource;

  // Handles callback from the RandomWalk
  static void OnData(void *self, double elapsedTime, double series0, double series1);

  // A thread-safe queue with minimal read/write contention
  struct DataPacket
  {
    double elapsedTime;
    double series0;
    double series1;
  };
  wxDoubleBufferedQueue<DataPacket> m_buffer;

  // The number of samples per data series used in this demo
  static const int sampleSize = 10000;

  // The full range is initialized to 180 seconds. It will extend when more data are available.
  static const int initialFullRange = 180;

  // The visible range is initialized to 30 seconds.
  static const int initialVisibleRange = 30;

  // The maximum zoom in is 5 seconds.
  static const int zoomInLimit = 5;

  // If the track cursor is at the end of the data series, we will automatic move the track
  // line when new data arrives.
  double trackLineEndPos;
  bool trackLineIsAtEnd;

  double m_timeStamps[sampleSize];        // The timestamps for the data series
  double m_dataSeriesA[sampleSize];       // The values for the data series A
  double m_dataSeriesB[sampleSize];       // The values for the data series B

  int m_currentIndex;                     // Index of the array position to which new values are added.

  wxToggleButton* m_pointerButton;
  wxToggleButton* m_zoominButton;
  wxToggleButton* m_zoomoutButton;
  wxButton*       m_saveButton;
  wxColour        m_bgColour;

  wxTimer* m_chartUpdateTimer;

  wxChartViewer* m_chartViewer;
  wxViewPortControl* m_viewPortControl;
};

#endif
    // REALTIMEVIEWPORT_H_
