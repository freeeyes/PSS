/*
** Name:        realtimemultithread.h
** Purpose:     Multithreading Real-Time Chart with Zoom/Scroll and Track Line
** Author:      Ulrich Telle
** Created:     2018-07-02
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef REALTIMEMULTITHREAD_H_
#define REALTIMEMULTITHREAD_H_

/*!
 * Includes
 */

#include "wx/frame.h"
#include "wx/tglbtn.h"
#include "wx/scrolbar.h"

#include "wxchartviewer.h"
#include "chartdir.h"
#include "wxdoublebufferedqueue.h"
#include "randomwalk.h"

/*!
 * Control identifiers
 */

#define ID_REALTIMEMULTITHREAD 10000
#define ID_CHARTVIEWER         10001
#define ID_POINTER             10002
#define ID_SCROLLBAR           10003
#define ID_UPDATE_TIMER        10004
#define SYMBOL_REALTIMEMULTITHREAD_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_REALTIMEMULTITHREAD_TITLE _("Multithreading Real-Time Chart with Zoom/Scroll and Track Line")
#define SYMBOL_REALTIMEMULTITHREAD_IDNAME ID_REALTIMEMULTITHREAD
#define SYMBOL_REALTIMEMULTITHREAD_SIZE wxSize(800, 420)
#define SYMBOL_REALTIMEMULTITHREAD_POSITION wxDefaultPosition

/*!
 * RealTimeMultiThread class declaration
 */

class RealTimeMultiThread: public wxFrame
{    
  DECLARE_CLASS( RealTimeMultiThread )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  RealTimeMultiThread();
   RealTimeMultiThread( wxWindow* parent, wxWindowID id = SYMBOL_REALTIMEMULTITHREAD_IDNAME, const wxString& caption = SYMBOL_REALTIMEMULTITHREAD_TITLE, const wxPoint& pos = SYMBOL_REALTIMEMULTITHREAD_POSITION, const wxSize& size = SYMBOL_REALTIMEMULTITHREAD_SIZE, long style = SYMBOL_REALTIMEMULTITHREAD_STYLE );

  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_REALTIMEMULTITHREAD_IDNAME, const wxString& caption = SYMBOL_REALTIMEMULTITHREAD_TITLE, const wxPoint& pos = SYMBOL_REALTIMEMULTITHREAD_POSITION, const wxSize& size = SYMBOL_REALTIMEMULTITHREAD_SIZE, long style = SYMBOL_REALTIMEMULTITHREAD_STYLE );

  /// Destructor
  ~RealTimeMultiThread();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  void DrawChart(wxChartViewer* viewer);          // Draw chart

  double TrackLineLabel(XYChart* c, int mouseX);    // Draw track cursor

  void UpdateControls(wxChartViewer* viewer);

////@begin RealTimeMultiThread event handler declarations

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_POINTER
  void OnPointerClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_IN
  void OnZoomInClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_OUT
  void OnZoomOutClick( wxCommandEvent& event );

  void OnSave(wxCommandEvent& event);

  /// wxEVT_COMMAND_SCROLLBAR_UPDATED event handler for ID_SCROLLBAR
  void OnScrollbarUpdated(wxScrollEvent& event);

  void OnChartUpdateTimer(wxTimerEvent& event);

  void OnMouseUsageChanged(int mouseUsage);

  void OnViewPortChanged(wxCommandEvent& event);

  void OnScrollBarChanged(int value);

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
  static void OnData(void* self, double elapsedTime, double series0, double series1);

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

  // The initial full range is set to 60 seconds of data.
  static const int initialFullRange = 60;

  // The maximum zoom in is 5 seconds.
  static const int zoomInLimit = 5;

  // If the track cursor is at the end of the data series, we will automatic move the track
  // line when new data arrives.
  double m_trackLineEndPos;
  bool m_trackLineIsAtEnd;

  double m_timeStamps[sampleSize];	// The timestamps for the data series
  double m_dataSeriesA[sampleSize];	// The values for the data series A
  double m_dataSeriesB[sampleSize];	// The values for the data series B

  int m_currentIndex;                 // Index of the array position to which new values are added.

  /// User interface
  wxToggleButton* m_pointerButton;
  wxToggleButton* m_zoominButton;
  wxToggleButton* m_zoomoutButton;
  wxButton*       m_saveButton;
  wxColour        m_bgColour;
  wxScrollBar*    m_scrollBar;

  wxTimer* m_chartUpdateTimer;

  wxChartViewer* m_chartViewer;
};

#endif
    // REALTIMEMULTITHREAD_H_
