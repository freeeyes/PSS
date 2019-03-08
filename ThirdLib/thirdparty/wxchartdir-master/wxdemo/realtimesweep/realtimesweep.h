/*
** Name:        realtimesweepapp.h
** Purpose:     Real-Time Sweep Chart
** Author:      Ulrich Telle
** Created:     2018-07-02
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef REALTIMESWEEP_H_
#define REALTIMESWEEP_H_

/*!
 * Includes
 */

#include "wx/frame.h"
#include "wx/tglbtn.h"
#include "wx/scrolbar.h"

#include "wxchartviewer.h"
#include "chartdir.h"
#include "wxdoublebufferedqueue.h"
#include "randomwave.h"

/*!
 * Control identifiers
 */

#define ID_REALTIMESWEEP 10000
#define ID_CHARTVIEWER   10001
#define ID_DATA_TIMER    10002
#define ID_UPDATE_TIMER  10003

#define SYMBOL_REALTIMESWEEP_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_REALTIMESWEEP_TITLE _("Real-Time Sweep Chart")
#define SYMBOL_REALTIMESWEEP_IDNAME ID_REALTIMESWEEP
#define SYMBOL_REALTIMESWEEP_SIZE wxSize(770, 320)
#define SYMBOL_REALTIMESWEEP_POSITION wxDefaultPosition

// The number of samples per data series used in this demo
const int sampleSize = 240;

/*!
 * RealTimeSweep class declaration
 */

class RealTimeSweep: public wxFrame
{    
  DECLARE_CLASS( RealTimeSweep )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  RealTimeSweep();
  RealTimeSweep( wxWindow* parent, wxWindowID id = SYMBOL_REALTIMESWEEP_IDNAME, const wxString& caption = SYMBOL_REALTIMESWEEP_TITLE, const wxPoint& pos = SYMBOL_REALTIMESWEEP_POSITION, const wxSize& size = SYMBOL_REALTIMESWEEP_SIZE, long style = SYMBOL_REALTIMESWEEP_STYLE );

  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_REALTIMESWEEP_IDNAME, const wxString& caption = SYMBOL_REALTIMESWEEP_TITLE, const wxPoint& pos = SYMBOL_REALTIMESWEEP_POSITION, const wxSize& size = SYMBOL_REALTIMESWEEP_SIZE, long style = SYMBOL_REALTIMESWEEP_STYLE );

  /// Destructor
  ~RealTimeSweep();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  // Draw chart
  void DrawChart(wxChartViewer* viewer);

  // Handle resize event
  void OnResizeEvent(wxSizeEvent& event);

  void OnChartUpdateTimer(wxTimerEvent& event);

  void OnViewPortChanged(wxCommandEvent& event);

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );

  /// Should we show tooltips?
  static bool ShowToolTips();

private:
  // The random data source
  RandomWave* m_dataSource;

  // Handles callback from the RandomWalk
  static void OnData(void* self, double elapsedTime, double series0, double series1);

  // A thread-safe queue with minimal read/write contention
  struct DataPacket
  {
    double elapsedTime;
    double series0;
    double series1;
  };
  wxDoubleBufferedQueue<DataPacket> buffer;

  // The number of samples per data series used in this demo
  static const int sampleSize = 10000;

  // The time range of the sweep chart
  static const int timeRange = 60;

  double m_timeStamps[sampleSize];	// The timestamps for the data series
  double m_channel1[sampleSize];	// The values for the data series A
  double m_channel2[sampleSize];	// The values for the data series B

  int m_currentIndex;                 // Index of the array position to which new values are added.

    wxColour        m_bgColour;
    wxTimer* m_chartUpdateTimer;
    
    wxDateTime m_nextDataTime;           // Used by the random number generator to generate realtime data.

    wxChartViewer* m_chartViewer;
    
    bool m_inResize;
    wxStopWatch sw;
};

#endif
    // REALTIMESWEEP_H_
