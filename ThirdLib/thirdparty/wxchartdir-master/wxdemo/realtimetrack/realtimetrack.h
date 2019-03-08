/*
** Name:        realtimetrack.h
** Purpose:     Realtime Chart with Track Line
** Author:      Ulrich Telle
** Created:     2018-07-02
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef REALTIMETRACK_H_
#define REALTIMETRACK_H_

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

#define ID_REALTIMETRACK 10000
#define ID_CHARTVIEWER   10001
#define ID_PLAY          10002
#define ID_PAUSE         10003
#define ID_UPDATE_PERIOD 10004
#define ID_DATA_TIMER    10005
#define ID_UPDATE_TIMER  10006

#define SYMBOL_REALTIMETRACK_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_REALTIMETRACK_TITLE _("Realtime Chart with Track Line")
#define SYMBOL_REALTIMETRACK_IDNAME ID_REALTIMETRACK
#define SYMBOL_REALTIMETRACK_SIZE wxSize(770, 320)
#define SYMBOL_REALTIMETRACK_POSITION wxDefaultPosition

// The number of samples per data series used in this demo
const int sampleSize = 240;

/*!
 * RealTimeTrack class declaration
 */

class RealTimeTrack: public wxFrame
{    
  DECLARE_CLASS( RealTimeTrack )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  RealTimeTrack();
  RealTimeTrack( wxWindow* parent, wxWindowID id = SYMBOL_REALTIMETRACK_IDNAME, const wxString& caption = SYMBOL_REALTIMETRACK_TITLE, const wxPoint& pos = SYMBOL_REALTIMETRACK_POSITION, const wxSize& size = SYMBOL_REALTIMETRACK_SIZE, long style = SYMBOL_REALTIMETRACK_STYLE );

  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_REALTIMETRACK_IDNAME, const wxString& caption = SYMBOL_REALTIMETRACK_TITLE, const wxPoint& pos = SYMBOL_REALTIMETRACK_POSITION, const wxSize& size = SYMBOL_REALTIMETRACK_SIZE, long style = SYMBOL_REALTIMETRACK_STYLE );

  /// Destructor
  ~RealTimeTrack();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  // The "Run" or "Freeze" button has been pressed
  void OnRunFreezeChanged(bool run);

  // The chart update timer interval has changed.
  void OnUpdatePeriodChanged(const wxString& value);

  // Get new data values
  void GetData();

  // Update the chart.
  void UpdateChart();
  
  // Draw the chart.
  void DrawChart();

  void TrackLineLegend(XYChart* c, int mouseX);

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_POINTER
  void OnPlayClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_IN
  void OnPauseClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_UPDATE_PERIOD
  void OnUpdatePeriodSelected( wxCommandEvent& event );

  void OnDataTimer(wxTimerEvent& event);
  void OnChartUpdateTimer(wxTimerEvent& event);

  void OnViewPortChanged(wxCommandEvent& event);
  void OnMouseMovePlotArea(wxCommandEvent& event);

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );

  /// Should we show tooltips?
  static bool ShowToolTips();

  wxToggleButton* m_playButton;
  wxToggleButton* m_pauseButton;
  wxColour        m_bgColour;

  wxChoice* m_updatePeriod;
  wxTextCtrl* m_alphaValue;  // Label to display the realtime value A
  wxTextCtrl* m_betaValue;   // Label to display the realtime value B
  wxTextCtrl* m_gammaValue;  // Label to display the realtime value C

  wxTimer* m_dataRateTimer;
  wxTimer* m_chartUpdateTimer;
    
  int    m_currentIndex;            // Index of the array position to which new values are added.
  double m_timeStamps[sampleSize];	// The timestamps for the data series
  double m_dataSeriesA[sampleSize];	// The values for the data series A
  double m_dataSeriesB[sampleSize];	// The values for the data series B
  double m_dataSeriesC[sampleSize];	// The values for the data series C

  wxDateTime m_nextDataTime;           // Used by the random number generator to generate realtime data.

  wxChartViewer* m_chartViewer;
};

#endif
    // REALTIMETRACK_H_
