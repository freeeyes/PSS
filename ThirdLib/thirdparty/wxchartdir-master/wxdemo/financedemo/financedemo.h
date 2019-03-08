/*
** Name:        financedemo.h
** Purpose:     Financial Chart Demonstration
** Author:      Ulrich Telle
** Created:     2018-07-01
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef FINANCEDEMO_H_
#define FINANCEDEMO_H_

/*!
 * Includes
 */

#include "wx/frame.h"

#include "wxchartviewer.h"

/*!
 * Control identifiers
 */

#define ID_FINANCEDEMO      10000
#define ID_TICKER_SYMBOL    10001
#define ID_COMPARE_WITH     10002
#define ID_TIME_PERIOD      10003
#define ID_CHART_SIZE       10004
#define ID_SHOW_VOLUME_BAR  10005
#define ID_PARABOLIC_SAR    10006
#define ID_LOG_SCALE        10007
#define ID_PERCENTAGE_SCALE 10008
#define ID_CHART_TYPE       10009
#define ID_PRICE_BAND       10010
#define ID_MOVING_AVG1      10011
#define ID_VALUE_AVG1       10012
#define ID_MOVING_AVG2      10013
#define ID_VALUE_AVG2       10014
#define ID_TECH_INDICATOR1  10015
#define ID_TECH_INDICATOR2  10016
#define ID_TECH_INDICATOR3  10017
#define ID_TECH_INDICATOR4  10018
#define ID_CHARTVIEWER      10019
#define SYMBOL_FINANCEDEMO_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_FINANCEDEMO_TITLE _("ChartDirector Interactive Financial Chart Demonstration")
#define SYMBOL_FINANCEDEMO_IDNAME ID_FINANCEDEMO
#define SYMBOL_FINANCEDEMO_SIZE wxSize(1000, 575)
#define SYMBOL_FINANCEDEMO_POSITION wxDefaultPosition

/*!
 * FinanceDemo class declaration
 */

class FinanceDemo: public wxFrame
{    
  DECLARE_CLASS( FinanceDemo )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  FinanceDemo();
  FinanceDemo( wxWindow* parent, wxWindowID id = SYMBOL_FINANCEDEMO_IDNAME, const wxString& caption = SYMBOL_FINANCEDEMO_TITLE, const wxPoint& pos = SYMBOL_FINANCEDEMO_POSITION, const wxSize& size = SYMBOL_FINANCEDEMO_SIZE, long style = SYMBOL_FINANCEDEMO_STYLE );

  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_FINANCEDEMO_IDNAME, const wxString& caption = SYMBOL_FINANCEDEMO_TITLE, const wxPoint& pos = SYMBOL_FINANCEDEMO_POSITION, const wxSize& size = SYMBOL_FINANCEDEMO_SIZE, long style = SYMBOL_FINANCEDEMO_STYLE );

  /// Destructor
  ~FinanceDemo();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  // Event handler declarations

  /// wxEVT_COMMAND_TEXT_ENTER event handler for ID_TICKER_SYMBOL
  void OnTickerSymbolEnter( wxCommandEvent& event );

  /// wxEVT_COMMAND_TEXT_ENTER event handler for ID_COMPARE_WITH
  void OnCompareWithEnter( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_TIME_PERIOD
  void OnTimePeriodSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHART_SIZE
  void OnChartSizeSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_SHOW_VOLUME_BAR
  void OnShowVolumeBarClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_PARABOLIC_SAR
  void OnParabolicSarClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_LOG_SCALE
  void OnLogScaleClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_PERCENTAGE_SCALE
  void OnPercentageScaleClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHART_TYPE
  void OnChartTypeSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_PRICE_BAND
  void OnPriceBandSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_MOVING_AVG1/2
  void OnMovingAvgSelected( wxCommandEvent& event );

  /// wxEVT_COMMAND_TEXT_ENTER event handler for ID_VALUE_AVG1/2
  void OnMovingAvgUpdated( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_TECH_INDICATOR1..4
  void OnTechIndicatorSelected( wxCommandEvent& event );

  // Member function declarations

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );

  /// Should we show tooltips?
  static bool ShowToolTips();

  virtual void DrawChart(wxChartViewer* viewer);

private:
  // Routines to get data into the data arrays
  virtual bool GetData(const wxString& ticker, const wxDateTime& startDate, const wxDateTime& endDate, int durationInDays, int extraPoints);
  virtual void Get15MinData(const wxString& ticker, const wxDateTime& startDate, const wxDateTime& endDate);
  virtual void GetDailyData(const wxString &ticker, const wxDateTime& startDate, const wxDateTime& endDate);
  virtual void GetWeeklyData(const wxString &ticker, const wxDateTime& startDate, const wxDateTime& endDate);
  virtual void GetMonthlyData(const wxString &ticker, const wxDateTime& startDate, const wxDateTime& endDate);

  // Utilities to aggregate data if the data source do not already have aggregated data
  virtual void ConvertDailyToWeeklyData();
  virtual void ConvertDailyToMonthlyData();
  virtual void AggregateData(ArrayMath& aggregator);

  // In the demo, we just use random numbers for data.
  virtual void GenerateRandomData(const wxString& ticker, const wxDateTime& startDate, const wxDateTime& endDate, int resolution);

  // TimeStamps, volume, high, low, open and close data
  wxString m_tickerKey;
  double* m_timeStamps;
  double* m_volData;
  double* m_highData;
  double* m_lowData;
  double* m_openData;
  double* m_closeData;
  int m_noOfPoints;

  // An extra data series to compare with the close data
  wxString m_compareKey;
  double* m_compareData;
  int m_compareDataLen;

  // The resolution of the data in seconds. 1 day = 86400 seconds.
  int m_resolution;

  // The moving average periods
  int m_avgPeriod1;
  int m_avgPeriod2;

  wxArrayString m_timePeriodStrings;
  wxArrayInt    m_timePeriods;
  wxArrayString m_chartSizeStrings;
  wxArrayString m_chartSizes;
  wxArrayString m_chartTypeStrings;
  wxArrayString m_chartTypes;
  wxArrayString m_priceBandStrings;
  wxArrayString m_priceBands;
  wxArrayString m_movingAvgStrings;
  wxArrayString m_movingAvgs;
  wxArrayString m_techIndicatorStrings;
  wxArrayString m_techIndicators;

  // The user interface

  wxTextCtrl* m_tickerSymbol;
  wxTextCtrl* m_compareWith;
  wxChoice* m_timeRange;
  wxChoice* m_chartSize;
  wxCheckBox* m_volumeBars;
  wxCheckBox* m_parabolicSar;
  wxCheckBox* m_logScale;
  wxCheckBox* m_percentageScale;
  wxChoice* m_chartType;
  wxChoice* m_priceBand;
  wxChoice* m_avgType1;
  wxTextCtrl* m_movAvg1;
  wxChoice* m_avgType2;
  wxTextCtrl* m_movAvg2;
  wxChoice* m_techIndicator1;
  wxChoice* m_techIndicator2;
  wxChoice* m_techIndicator3;
  wxChoice* m_techIndicator4;

  int m_indicator1;
  int m_indicator2;
  int m_indicator3;
  int m_indicator4;

  wxChartViewer* m_chartViewer;
};

#endif
    // FINANCEDEMO_H_
