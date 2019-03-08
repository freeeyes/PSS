/*
** Name:        financedemo.cpp
** Purpose:     Financial Chart Demonstration
** Author:      Ulrich Telle
** Created:     2018-07-01
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "financedemo.h"
#include "FinanceChart.h"

#include "mondrian.xpm"

/*
 * FinanceDemo type definition
 */

IMPLEMENT_CLASS( FinanceDemo, wxFrame )

/*
 * FinanceDemo event table definition
 */

BEGIN_EVENT_TABLE( FinanceDemo, wxFrame )

  EVT_TEXT_ENTER( ID_TICKER_SYMBOL, FinanceDemo::OnTickerSymbolEnter )
  EVT_TEXT_ENTER( ID_COMPARE_WITH, FinanceDemo::OnCompareWithEnter )
  EVT_CHOICE( ID_TIME_PERIOD, FinanceDemo::OnTimePeriodSelected )
  EVT_CHOICE( ID_CHART_SIZE, FinanceDemo::OnChartSizeSelected )
  EVT_CHECKBOX( ID_SHOW_VOLUME_BAR, FinanceDemo::OnShowVolumeBarClick )
  EVT_CHECKBOX( ID_PARABOLIC_SAR, FinanceDemo::OnParabolicSarClick )
  EVT_CHECKBOX( ID_LOG_SCALE, FinanceDemo::OnLogScaleClick )
  EVT_CHECKBOX( ID_PERCENTAGE_SCALE, FinanceDemo::OnPercentageScaleClick )
  EVT_CHOICE( ID_CHART_TYPE, FinanceDemo::OnChartTypeSelected )
  EVT_CHOICE( ID_PRICE_BAND, FinanceDemo::OnPriceBandSelected )
  EVT_CHOICE( ID_MOVING_AVG1, FinanceDemo::OnMovingAvgSelected )
  EVT_CHOICE( ID_MOVING_AVG2, FinanceDemo::OnMovingAvgSelected)
  EVT_TEXT_ENTER( ID_VALUE_AVG1, FinanceDemo::OnMovingAvgUpdated)
  EVT_TEXT_ENTER( ID_VALUE_AVG2, FinanceDemo::OnMovingAvgUpdated )
  EVT_CHOICE( ID_TECH_INDICATOR1, FinanceDemo::OnTechIndicatorSelected )
  EVT_CHOICE( ID_TECH_INDICATOR2, FinanceDemo::OnTechIndicatorSelected )
  EVT_CHOICE( ID_TECH_INDICATOR3, FinanceDemo::OnTechIndicatorSelected )
  EVT_CHOICE( ID_TECH_INDICATOR4, FinanceDemo::OnTechIndicatorSelected )

END_EVENT_TABLE()

/*
 * FinanceDemo constructors
 */

FinanceDemo::FinanceDemo()
{
  Init();
}

FinanceDemo::FinanceDemo( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create( parent, id, caption, pos, size, style );
}

/*
 * FinanceDemo creator
 */

bool
FinanceDemo::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  wxFrame::Create( parent, id, caption, pos, size, style );

  CreateControls();
  if (GetSizer())
  {
    GetSizer()->SetSizeHints(this);
  }
  Centre();

  return true;
}

/*
 * FinanceDemo destructor
 */

FinanceDemo::~FinanceDemo()
{
  delete[] m_timeStamps;
  delete[] m_highData;
  delete[] m_lowData;
  delete[] m_openData;
  delete[] m_closeData;
  delete[] m_volData;
  delete[] m_compareData; 
}

/*
 * Member initialisation
 */

void
FinanceDemo::Init()
{
  m_tickerSymbol = NULL;
  m_compareWith = NULL;
  m_timeRange = NULL;
  m_chartSize = NULL;
  m_volumeBars = NULL;
  m_parabolicSar = NULL;
  m_logScale = NULL;
  m_percentageScale = NULL;
  m_chartType = NULL;
  m_priceBand = NULL;
  m_avgType1 = NULL;
  m_movAvg1 = NULL;
  m_avgType2 = NULL;
  m_movAvg2 = NULL;
  m_techIndicator1 = NULL;
  m_techIndicator2 = NULL;
  m_techIndicator3 = NULL;
  m_techIndicator4 = NULL;

  // Initialize member variables

  m_noOfPoints = 0;
  m_timeStamps = 0;
  m_highData = 0;
  m_lowData = 0;
  m_openData = 0;
  m_closeData = 0;
  m_volData = 0;

  m_resolution = 86400;

  m_compareData = 0;
  m_compareDataLen = 0;

  m_timePeriodStrings.Add(_("1 day"));
  m_timePeriodStrings.Add(_("2 days"));
  m_timePeriodStrings.Add(_("5 days"));
  m_timePeriodStrings.Add(_("10 days"));
  m_timePeriodStrings.Add(_("1 month"));
  m_timePeriodStrings.Add(_("2 months"));
  m_timePeriodStrings.Add(_("3 months"));
  m_timePeriodStrings.Add(_("6 months"));
  m_timePeriodStrings.Add(_("1 year"));
  m_timePeriodStrings.Add(_("2 years"));
  m_timePeriodStrings.Add(_("3 years"));
  m_timePeriodStrings.Add(_("4 years"));
  m_timePeriodStrings.Add(_("5 years"));
  m_timePeriodStrings.Add(_("10 years"));

  m_timePeriods.Add(1);
  m_timePeriods.Add(2);
  m_timePeriods.Add(5);
  m_timePeriods.Add(10);
  m_timePeriods.Add(30);
  m_timePeriods.Add(60);
  m_timePeriods.Add(90);
  m_timePeriods.Add(180);
  m_timePeriods.Add(360);
  m_timePeriods.Add(720);
  m_timePeriods.Add(1080);
  m_timePeriods.Add(1440);
  m_timePeriods.Add(1800);
  m_timePeriods.Add(3600);

  m_chartSizeStrings.Add(_("Small"));
  m_chartSizeStrings.Add(_("Medium"));
  m_chartSizeStrings.Add(_("Large"));
  m_chartSizeStrings.Add(_("Huge"));

  m_chartSizes.Add(wxS("S"));
  m_chartSizes.Add(wxS("M"));
  m_chartSizes.Add(wxS("L"));
  m_chartSizes.Add(wxS("H"));

  m_chartTypeStrings.Add(_("None"));
  m_chartTypeStrings.Add(_("CandleStick"));
  m_chartTypeStrings.Add(_("Closing Price"));
  m_chartTypeStrings.Add(_("Median Price"));
  m_chartTypeStrings.Add(_("OHLC"));
  m_chartTypeStrings.Add(_("Typical Price"));
  m_chartTypeStrings.Add(_("Weighted Close"));

  m_chartTypes.Add(wxS("None"));
  m_chartTypes.Add(wxS("CandleStick"));
  m_chartTypes.Add(wxS("Close"));
  m_chartTypes.Add(wxS("Median"));
  m_chartTypes.Add(wxS("OHLC"));
  m_chartTypes.Add(wxS("TP"));
  m_chartTypes.Add(wxS("WC"));

  m_priceBandStrings.Add(_("None"));
  m_priceBandStrings.Add(_("Bollinger Band"));
  m_priceBandStrings.Add(_("Donchain Channel"));
  m_priceBandStrings.Add(_("Envelop (SMA 20 +/- 10%)"));

  m_priceBands.Add(wxS("None"));
  m_priceBands.Add(wxS("BB"));
  m_priceBands.Add(wxS("DC"));
  m_priceBands.Add(wxS("Envelop"));

  m_movingAvgStrings.Add(_("None"));
  m_movingAvgStrings.Add(_("Simple"));
  m_movingAvgStrings.Add(_("Exponential"));
  m_movingAvgStrings.Add(_("Triangular"));
  m_movingAvgStrings.Add(_("Weighted"));

  m_movingAvgs.Add(wxS("None"));
  m_movingAvgs.Add(wxS("SMA"));
  m_movingAvgs.Add(wxS("EMA"));
  m_movingAvgs.Add(wxS("TMA"));
  m_movingAvgs.Add(wxS("WMA"));

  m_techIndicatorStrings.Add(_("None"));
  m_techIndicatorStrings.Add(_("Accumulation/Distribution"));
  m_techIndicatorStrings.Add(_("Aroon Oscillator"));
  m_techIndicatorStrings.Add(_("Aroon Up/Down"));
  m_techIndicatorStrings.Add(_("Avg Directional Index"));
  m_techIndicatorStrings.Add(_("Avg True Range"));
  m_techIndicatorStrings.Add(_("Bollinger Band Width"));
  m_techIndicatorStrings.Add(_("Chaikin Money Flow"));
  m_techIndicatorStrings.Add(_("Chaikin Oscillator"));
  m_techIndicatorStrings.Add(_("Chaikin Volatility"));
  m_techIndicatorStrings.Add(_("Close Location Value"));
  m_techIndicatorStrings.Add(_("Commodity Channel Index"));
  m_techIndicatorStrings.Add(_("Detrended Price Osc"));
  m_techIndicatorStrings.Add(_("Donchian Channel Width"));
  m_techIndicatorStrings.Add(_("Ease of Movement"));
  m_techIndicatorStrings.Add(_("Fast Stochastic"));
  m_techIndicatorStrings.Add(_("MACD"));
  m_techIndicatorStrings.Add(_("Mass Index"));
  m_techIndicatorStrings.Add(_("Momentum"));
  m_techIndicatorStrings.Add(_("Money Flow Index"));
  m_techIndicatorStrings.Add(_("Neg Volume Index"));
  m_techIndicatorStrings.Add(_("On Balance Volume"));
  m_techIndicatorStrings.Add(_("Performance"));
  m_techIndicatorStrings.Add(_("% Price Oscillator"));
  m_techIndicatorStrings.Add(_("% Volume Oscillator"));
  m_techIndicatorStrings.Add(_("Pos Volume Index"));
  m_techIndicatorStrings.Add(_("Price Volume Trend"));
  m_techIndicatorStrings.Add(_("Rate of Change"));
  m_techIndicatorStrings.Add(_("RSI"));
  m_techIndicatorStrings.Add(_("Slow Stochastic"));
  m_techIndicatorStrings.Add(_("StochRSI"));
  m_techIndicatorStrings.Add(_("TRIX"));
  m_techIndicatorStrings.Add(_("Ultimate Oscillator"));
  m_techIndicatorStrings.Add(_("Volume"));
  m_techIndicatorStrings.Add(_("William's %R"));

  m_techIndicators.Add(wxS("None"));
  m_techIndicators.Add(wxS("AccDist"));
  m_techIndicators.Add(wxS("AroonOsc"));
  m_techIndicators.Add(wxS("Aroon"));
  m_techIndicators.Add(wxS("ADX"));
  m_techIndicators.Add(wxS("ATR"));
  m_techIndicators.Add(wxS("BBW"));
  m_techIndicators.Add(wxS("CMF"));
  m_techIndicators.Add(wxS("COscillator"));
  m_techIndicators.Add(wxS("CVolatility"));
  m_techIndicators.Add(wxS("CLV"));
  m_techIndicators.Add(wxS("CCI"));
  m_techIndicators.Add(wxS("DPO"));
  m_techIndicators.Add(wxS("DCW"));
  m_techIndicators.Add(wxS("EMV"));
  m_techIndicators.Add(wxS("FStoch"));
  m_techIndicators.Add(wxS("MACD"));
  m_techIndicators.Add(wxS("MDX"));
  m_techIndicators.Add(wxS("Momentum"));
  m_techIndicators.Add(wxS("MFI"));
  m_techIndicators.Add(wxS("NVI"));
  m_techIndicators.Add(wxS("OBV"));
  m_techIndicators.Add(wxS("Performance"));
  m_techIndicators.Add(wxS("PPO"));
  m_techIndicators.Add(wxS("PVO"));
  m_techIndicators.Add(wxS("PVI"));
  m_techIndicators.Add(wxS("PVT"));
  m_techIndicators.Add(wxS("ROC"));
  m_techIndicators.Add(wxS("RSI"));
  m_techIndicators.Add(wxS("SStoch"));
  m_techIndicators.Add(wxS("StochRSI"));
  m_techIndicators.Add(wxS("TRIX"));
  m_techIndicators.Add(wxS("UO"));
  m_techIndicators.Add(wxS("Vol"));
  m_techIndicators.Add(wxS("WilliamR"));

  m_indicator1 = 28;
  m_indicator2 = 16;
  m_indicator3 = 0;
  m_indicator4 = 0;
}


/*
 * Control creation for FinanceDemo
 */

void
FinanceDemo::CreateControls()
{    
  SetIcon(wxICON(mondrian));
  FinanceDemo* itemFrame1 = this;

  wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxVERTICAL);
  itemFrame1->SetSizer(itemBoxSizer1);

  wxPanel* itemPanel2 = new wxPanel( itemFrame1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
  itemPanel2->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemBoxSizer1->Add(itemPanel2, 1, wxGROW|wxALL, 0);

  wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
  itemPanel2->SetSizer(itemBoxSizer3);

  wxStaticBox* itemStaticBoxSizer4Static = new wxStaticBox(itemPanel2, wxID_ANY, wxEmptyString);
  wxStaticBoxSizer* itemStaticBoxSizer4 = new wxStaticBoxSizer(itemStaticBoxSizer4Static, wxVERTICAL);
  itemBoxSizer3->Add(itemStaticBoxSizer4, 0, wxGROW|wxALL, 3);

  wxStaticText* itemStaticText5 = new wxStaticText( itemStaticBoxSizer4->GetStaticBox(), wxID_STATIC, _("Ticker Symbol"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticBoxSizer4->Add(itemStaticText5, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 3);

  m_tickerSymbol = new wxTextCtrl( itemStaticBoxSizer4->GetStaticBox(), ID_TICKER_SYMBOL, wxS("ASE.SYMBOL"), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
  itemStaticBoxSizer4->Add(m_tickerSymbol, 0, wxGROW|wxALL, 3);

  wxStaticText* itemStaticText7 = new wxStaticText( itemStaticBoxSizer4->GetStaticBox(), wxID_STATIC, _("Compare With"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticBoxSizer4->Add(itemStaticText7, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

  m_compareWith = new wxTextCtrl( itemStaticBoxSizer4->GetStaticBox(), ID_COMPARE_WITH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
  itemStaticBoxSizer4->Add(m_compareWith, 0, wxGROW|wxALL, 3);

  wxStaticText* itemStaticText9 = new wxStaticText( itemStaticBoxSizer4->GetStaticBox(), wxID_STATIC, _("Time Period"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticBoxSizer4->Add(itemStaticText9, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 3);

  m_timeRange = new wxChoice( itemStaticBoxSizer4->GetStaticBox(), ID_TIME_PERIOD, wxDefaultPosition, wxDefaultSize, m_timePeriodStrings, 0 );
  m_timeRange->SetSelection(7);
  itemStaticBoxSizer4->Add(m_timeRange, 0, wxGROW|wxALL, 3);

  wxStaticText* itemStaticText11 = new wxStaticText( itemStaticBoxSizer4->GetStaticBox(), wxID_STATIC, _("Chart Size"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticBoxSizer4->Add(itemStaticText11, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 3);

  m_chartSize = new wxChoice( itemStaticBoxSizer4->GetStaticBox(), ID_CHART_SIZE, wxDefaultPosition, wxDefaultSize, m_chartSizeStrings, 0 );
  m_chartSize->SetSelection(2);
  itemStaticBoxSizer4->Add(m_chartSize, 0, wxGROW|wxALL, 3);

  m_volumeBars = new wxCheckBox( itemStaticBoxSizer4->GetStaticBox(), ID_SHOW_VOLUME_BAR, _("Show Volume Bars"), wxDefaultPosition, wxDefaultSize, 0 );
  m_volumeBars->SetValue(true);
  itemStaticBoxSizer4->Add(m_volumeBars, 0, wxALIGN_LEFT|wxALL, 3);

  m_parabolicSar = new wxCheckBox( itemStaticBoxSizer4->GetStaticBox(), ID_PARABOLIC_SAR, _("Parabolic SAR"), wxDefaultPosition, wxDefaultSize, 0 );
  m_parabolicSar->SetValue(false);
  itemStaticBoxSizer4->Add(m_parabolicSar, 0, wxALIGN_LEFT|wxALL, 3);

  m_logScale = new wxCheckBox( itemStaticBoxSizer4->GetStaticBox(), ID_LOG_SCALE, _("Log Scale"), wxDefaultPosition, wxDefaultSize, 0 );
  m_logScale->SetValue(false);
  itemStaticBoxSizer4->Add(m_logScale, 0, wxALIGN_LEFT|wxALL, 3);

  m_percentageScale = new wxCheckBox( itemStaticBoxSizer4->GetStaticBox(), ID_PERCENTAGE_SCALE, _("Percentage Scale"), wxDefaultPosition, wxDefaultSize, 0 );
  m_percentageScale->SetValue(false);
  itemStaticBoxSizer4->Add(m_percentageScale, 0, wxALIGN_LEFT|wxALL, 3);

  wxStaticText* itemStaticText17 = new wxStaticText( itemStaticBoxSizer4->GetStaticBox(), wxID_STATIC, _("Chart Type"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticBoxSizer4->Add(itemStaticText17, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 3);

  m_chartType = new wxChoice( itemStaticBoxSizer4->GetStaticBox(), ID_CHART_TYPE, wxDefaultPosition, wxDefaultSize, m_chartTypeStrings, 0 );
  m_chartType->SetSelection(1);
  itemStaticBoxSizer4->Add(m_chartType, 0, wxGROW|wxALL, 3);

  wxStaticText* itemStaticText19 = new wxStaticText( itemStaticBoxSizer4->GetStaticBox(), wxID_STATIC, _("Price Band"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticBoxSizer4->Add(itemStaticText19, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 3);

  m_priceBand = new wxChoice( itemStaticBoxSizer4->GetStaticBox(), ID_PRICE_BAND, wxDefaultPosition, wxDefaultSize, m_priceBandStrings, 0 );
  m_priceBand->SetSelection(1);
  itemStaticBoxSizer4->Add(m_priceBand, 0, wxGROW|wxALL, 3);

  wxStaticText* itemStaticText21 = new wxStaticText( itemStaticBoxSizer4->GetStaticBox(), wxID_STATIC, _("Moving Averages"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticBoxSizer4->Add(itemStaticText21, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 3);

  wxBoxSizer* itemBoxSizer22 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer4->Add(itemBoxSizer22, 0, wxGROW|wxALL, 0);

  m_avgType1 = new wxChoice( itemStaticBoxSizer4->GetStaticBox(), ID_MOVING_AVG1, wxDefaultPosition, wxDefaultSize, m_movingAvgStrings, 0 );
  m_avgType1->SetSelection(1);
  itemBoxSizer22->Add(m_avgType1, 1, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 3);

  m_movAvg1 = new wxTextCtrl( itemStaticBoxSizer4->GetStaticBox(), ID_VALUE_AVG1, wxS("5"), wxDefaultPosition, wxSize(50, -1), wxTE_PROCESS_ENTER );
  itemBoxSizer22->Add(m_movAvg1, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxTOP, 3);

  wxBoxSizer* itemBoxSizer25 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer4->Add(itemBoxSizer25, 0, wxGROW|wxALL, 0);

  m_avgType2 = new wxChoice( itemStaticBoxSizer4->GetStaticBox(), ID_MOVING_AVG2, wxDefaultPosition, wxDefaultSize, m_movingAvgStrings, 0 );
  m_avgType2->SetSelection(1);
  itemBoxSizer25->Add(m_avgType2, 1, wxALIGN_CENTER_VERTICAL|wxALL, 3);

  m_movAvg2 = new wxTextCtrl( itemStaticBoxSizer4->GetStaticBox(), ID_VALUE_AVG2, wxS("20"), wxDefaultPosition, wxSize(50, -1), wxTE_PROCESS_ENTER );
  itemBoxSizer25->Add(m_movAvg2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);

  wxStaticText* itemStaticText28 = new wxStaticText( itemStaticBoxSizer4->GetStaticBox(), wxID_STATIC, _("Technical Indicators"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticBoxSizer4->Add(itemStaticText28, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 3);

  m_techIndicator1 = new wxChoice( itemStaticBoxSizer4->GetStaticBox(), ID_TECH_INDICATOR1, wxDefaultPosition, wxDefaultSize, m_techIndicatorStrings, 0 );
  m_techIndicator1->SetSelection(m_indicator1);
  itemStaticBoxSizer4->Add(m_techIndicator1, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 3);

  m_techIndicator2 = new wxChoice( itemStaticBoxSizer4->GetStaticBox(), ID_TECH_INDICATOR2, wxDefaultPosition, wxDefaultSize, m_techIndicatorStrings, 0 );
  m_techIndicator2->SetSelection(m_indicator2);
  itemStaticBoxSizer4->Add(m_techIndicator2, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 3);

  m_techIndicator3 = new wxChoice( itemStaticBoxSizer4->GetStaticBox(), ID_TECH_INDICATOR3, wxDefaultPosition, wxDefaultSize, m_techIndicatorStrings, 0 );
  m_techIndicator3->SetSelection(m_indicator3);
  itemStaticBoxSizer4->Add(m_techIndicator3, 0, wxGROW|wxLEFT|wxRIGHT|wxTOP, 3);

  m_techIndicator4 = new wxChoice( itemStaticBoxSizer4->GetStaticBox(), ID_TECH_INDICATOR4, wxDefaultPosition, wxDefaultSize, m_techIndicatorStrings, 0 );
  m_techIndicator4->SetSelection(m_indicator4);
  itemStaticBoxSizer4->Add(m_techIndicator4, 0, wxGROW|wxALL, 3);

  wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer3->Add(itemBoxSizer8, 1, wxGROW | wxALL, 0);

  m_chartViewer = new wxChartViewer(itemPanel2, ID_CHARTVIEWER, wxDefaultPosition, wxSize(660,350), wxTAB_TRAVERSAL | wxNO_BORDER);
  m_chartViewer->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemBoxSizer8->Add(m_chartViewer, 1, wxGROW | wxALL, 3);

  // Update the chart
  DrawChart(m_chartViewer);
}


/*
 * Should we show tooltips?
 */

bool
FinanceDemo::ShowToolTips()
{
  return true;
}

/*
 * Get bitmap resources
 */

wxBitmap
FinanceDemo::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
}

/*
 * Get icon resources
 */

wxIcon
FinanceDemo::GetIconResource( const wxString& name )
{
    // Icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
}


/*
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_TECH_INDICATOR1..4
 */

void
FinanceDemo::OnTechIndicatorSelected( wxCommandEvent& event )
{
  DrawChart(m_chartViewer);
}


/*
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_MOVING_AVG2
 */

void
FinanceDemo::OnMovingAvgSelected( wxCommandEvent& event )
{
  DrawChart(m_chartViewer);
}


/*
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_PRICE_BAND
 */

void
FinanceDemo::OnPriceBandSelected( wxCommandEvent& event )
{
  DrawChart(m_chartViewer);
}


/*
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHART_TYPE
 */

void
FinanceDemo::OnChartTypeSelected( wxCommandEvent& event )
{
  DrawChart(m_chartViewer);
}


/*
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_CHART_SIZE
 */

void
FinanceDemo::OnChartSizeSelected( wxCommandEvent& event )
{
  DrawChart(m_chartViewer);
  Refresh();
}


/*
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_SHOW_VOLUME_BAR
 */

void
FinanceDemo::OnShowVolumeBarClick( wxCommandEvent& event )
{
  DrawChart(m_chartViewer);
}


/*
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_PARABOLIC_SAR
 */

void
FinanceDemo::OnParabolicSarClick( wxCommandEvent& event )
{
  DrawChart(m_chartViewer);
}


/*
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_LOG_SCALE
 */

void
FinanceDemo::OnLogScaleClick( wxCommandEvent& event )
{
  DrawChart(m_chartViewer);
}


/*
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_PERCENTAGE_SCALE
 */

void
FinanceDemo::OnPercentageScaleClick( wxCommandEvent& event )
{
  DrawChart(m_chartViewer);
}


/*
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_TIME_PERIOD
 */

void
FinanceDemo::OnTimePeriodSelected( wxCommandEvent& event )
{
  DrawChart(m_chartViewer);
}


/*
 * wxEVT_COMMAND_TEXT_ENTER event handler for ID_COMPARE_WITH
 */

void
FinanceDemo::OnCompareWithEnter( wxCommandEvent& event )
{
  wxString compareKey = m_compareWith->GetValue();
  if (!m_compareKey.IsSameAs(compareKey))
  {
    DrawChart(m_chartViewer);
  }
}


/*
 * wxEVT_COMMAND_TEXT_ENTER event handler for ID_TICKER_SYMBOL
 */

void
FinanceDemo::OnTickerSymbolEnter( wxCommandEvent& event )
{
  wxString tickerKey = m_tickerSymbol->GetValue();
  if (!m_tickerKey.IsSameAs(tickerKey))
  {
    DrawChart(m_chartViewer);
  }
}


/*
 * wxEVT_COMMAND_TEXT_ENTER event handler for ID_VALUE_AVG1
 */

void
FinanceDemo::OnMovingAvgUpdated( wxCommandEvent& event )
{
  long avgValue = 0;
  if (!m_movAvg1->GetValue().ToLong(&avgValue))
  {
    m_movAvg1->SetValue(wxS("0"));
  }
  int new_avgPeriod1 = avgValue;

  if (!m_movAvg2->GetValue().ToLong(&avgValue))
  {
    m_movAvg2->SetValue(wxS("0"));
  }
  int new_avgPeriod2 = avgValue;

  if (new_avgPeriod1 != m_avgPeriod1 || new_avgPeriod2 != m_avgPeriod2)
  {
    DrawChart(m_chartViewer);
  }
}

/////////////////////////////////////////////////////////////////////////////
// Data source

/// <summary>
/// Get the timeStamps, highData, lowData, openData, closeData and volData.
/// </summary>
/// <param name="ticker">The ticker symbol for the data series.</param>
/// <param name="startDate">The starting date/time for the data series.</param>
/// <param name="endDate">The ending date/time for the data series.</param>
/// <param name="durationInDays">The number of trading days to get.</param>
/// <param name="extraPoints">The extra leading data points needed in order to
/// compute moving averages.</param>
/// <returns>True if successfully obtain the data, otherwise false.</returns>
bool
FinanceDemo::GetData(const wxString& ticker, const wxDateTime& startDate, const wxDateTime& endDate, int durationInDays, int extraPoints)
{
  // This method should return false if the ticker symbol is invalid. In this
  // sample code, as we are using a random number generator for the data, all
  // ticker symbol is allowed, but we still assumed an empty symbol is invalid.
  if (ticker.IsEmpty())
  {
    return false;
  }

  // In this demo, we can get 15 min, daily, weekly or monthly data depending on
  // the time range.
  wxDateTime adjustedStartDate = startDate;
  m_resolution = 86400;
  if (durationInDays <= 10)
  {
    // 10 days or less, we assume 15 minute data points are available
    m_resolution = 900;

    // We need to adjust the startDate backwards for the extraPoints. We assume
    // 6.5 hours trading time per day, and 5 trading days per week.
    double dataPointsPerDay = 6.5 * 3600 / m_resolution;
    adjustedStartDate.Subtract(wxDateSpan(0, 0, 0, (int)(extraPoints / dataPointsPerDay * 7 / 5 + 2.9999999)));

    // Get the required 15 min data
    Get15MinData(ticker, adjustedStartDate, endDate);
  }
  else if (durationInDays >= 4.5 * 360)
  {
    // 4 years or more - use monthly data points.
    m_resolution = 30 * 86400;

    // Adjust startDate backwards to cater for extraPoints
    adjustedStartDate.Subtract(wxDateSpan(0,extraPoints));

    // Get the required monthly data
    GetMonthlyData(ticker, adjustedStartDate, endDate);
  }
  else if (durationInDays >= 1.5 * 360)
  {
    // 1 year or more - use weekly points.
    m_resolution = 7 * 86400;

    //Note that we need to add extra points by shifting the starting weeks backwards
    adjustedStartDate.Subtract(wxDateSpan(0,0,0,extraPoints * 7 + 6));

    // Get the required weekly data
    GetWeeklyData(ticker, adjustedStartDate, endDate);
  }
  else
  {
    // Default - use daily points
    m_resolution = 86400;

    // Adjust startDate backwards to cater for extraPoints. We multiply the days
    // by 7/5 as we assume 1 week has 5 trading days.
    adjustedStartDate.Subtract(wxDateSpan(0, 0, 0, (extraPoints * 7 + 4) / 5 + 2));

    // Get the required daily data
    GetDailyData(ticker, adjustedStartDate, endDate);
  }

  return true;
}

/// <summary>
/// Get 15 minutes data series into the timeStamps, highData, lowData, openData, closeData
/// and volData arrays.
/// </summary>
/// <param name="startDate">The starting date/time for the data series.</param>
/// <param name="endDate">The ending date/time for the data series.</param>
void
FinanceDemo::Get15MinData(const wxString& ticker, const wxDateTime& startDate, const wxDateTime& endDate)
{
  // In this demo, we use a random number generator to generate the data. In practice,
  // you may get the data from a database or by other means. If you do not have 15
  // minute data, you may modify the "drawChart" method below to not using 15 minute
  // data.
  GenerateRandomData(ticker, startDate, endDate, 900);
}

/// <summary>
/// Get daily data series into the timeStamps, highData, lowData, openData, closeData and
/// volData arrays.
/// </summary>
/// <param name="startDate">The starting date/time for the data series.</param>
/// <param name="endDate">The ending date/time for the data series.</param>
void
FinanceDemo::GetDailyData(const wxString &ticker, const wxDateTime& startDate, const wxDateTime& endDate)
{
  //
  // In this demo, we use a random number generator to generate the data. In practice,
  // you may get the data from a database or by other means. Replace the code below
  // with your own data acquisition code.
  //

  GenerateRandomData(ticker, startDate, endDate, 86400);
}

/// <summary>
/// Get weekly data series into the timeStamps, highData, lowData, openData, closeData and
/// volData arrays.
/// </summary>
/// <param name="startDate">The starting date/time for the data series.</param>
/// <param name="endDate">The ending date/time for the data series.</param>
void
FinanceDemo::GetWeeklyData(const wxString &ticker, const wxDateTime& startDate, const wxDateTime& endDate)
{
  // In this demo, we use a random number generator to generate the data. In practice,
  // you may get the data from a database or by other means. If you do not have weekly
  // data, you may call "getDailyData" to get daily data first, and then call
  // "convertDailyToWeeklyData" to convert it to weekly data, like:
  //
  //      getDailyData(ticker, startDate, endDate);
  //      convertDailyToWeeklyData();
  GenerateRandomData(ticker, startDate, endDate, 86400 * 7);
}

/// <summary>
/// Get monthly data series into the timeStamps, highData, lowData, openData, closeData and
/// volData arrays.
/// </summary>
/// <param name="startDate">The starting date/time for the data series.</param>
/// <param name="endDate">The ending date/time for the data series.</param>
void
FinanceDemo::GetMonthlyData(const wxString &ticker, const wxDateTime& startDate, const wxDateTime& endDate)
{
  // In this demo, we use a random number generator to generate the data. In practice,
  // you may get the data from a database or by other means. If you do not have monthly
  // data, you may call "getDailyData" to get daily data first, and then call
  // "convertDailyToMonthlyData" to convert it to monthly data, like:
  //
  //      getDailyData(ticker, startDate, endDate);
  //      convertDailyToMonthlyData();
  GenerateRandomData(ticker, startDate, endDate, 86400 * 30);
}

/// <summary>
/// A random number generator designed to generate realistic financial data.
/// </summary>
/// <param name="startDate">The starting date/time for the data series.</param>
/// <param name="endDate">The ending date/time for the data series.</param>
/// <param name="resolution">The period of the data series.</param>
void
FinanceDemo::GenerateRandomData(const wxString& ticker, const wxDateTime& startDate, const wxDateTime& endDate, int resolution)
{
  // free the previous data arrays
  delete[] m_timeStamps;
  delete[] m_highData;
  delete[] m_lowData;
  delete[] m_openData;
  delete[] m_closeData;
  delete[] m_volData;

  // The financial simulator
  FinanceSimulator db((const char*) ticker.ToUTF8(), wxChartViewer::ConvertDateTimeToChartTime(startDate), wxChartViewer::ConvertDateTimeToChartTime(endDate), resolution);

  // Allocate the data arrays
  m_noOfPoints = db.getTimeStamps().len;
  m_timeStamps = new double[m_noOfPoints];
  m_highData = new double[m_noOfPoints];
  m_lowData = new double[m_noOfPoints];
  m_openData = new double[m_noOfPoints];
  m_closeData = new double[m_noOfPoints];
  m_volData = new double[m_noOfPoints];

  // Copy data to the data arrays
  memcpy(m_timeStamps, db.getTimeStamps().data, m_noOfPoints * sizeof(double));
  memcpy(m_highData, db.getHighData().data, m_noOfPoints * sizeof(double));
  memcpy(m_lowData, db.getLowData().data, m_noOfPoints * sizeof(double));
  memcpy(m_openData, db.getOpenData().data, m_noOfPoints * sizeof(double));
  memcpy(m_closeData, db.getCloseData().data, m_noOfPoints * sizeof(double));
  memcpy(m_volData, db.getVolData().data, m_noOfPoints * sizeof(double));
}

/// <summary>
/// A utility to convert daily to weekly data.
/// </summary>
void
FinanceDemo::ConvertDailyToWeeklyData()
{
  AggregateData(ArrayMath(DoubleArray(m_timeStamps, m_noOfPoints)).selectStartOfWeek());
}

/// <summary>
/// A utility to convert daily to monthly data.
/// </summary>
void
FinanceDemo::ConvertDailyToMonthlyData()
{
  AggregateData(ArrayMath(DoubleArray(m_timeStamps, m_noOfPoints)).selectStartOfMonth());
}

/// <summary>
/// An internal method used to aggregate daily data.
/// </summary>
void
FinanceDemo::AggregateData(ArrayMath& aggregator)
{
  aggregator.aggregate(DoubleArray(m_highData, m_noOfPoints), Chart::AggregateMax);
  aggregator.aggregate(DoubleArray(m_lowData, m_noOfPoints), Chart::AggregateMin);
  aggregator.aggregate(DoubleArray(m_openData, m_noOfPoints), Chart::AggregateFirst);
  aggregator.aggregate(DoubleArray(m_closeData, m_noOfPoints), Chart::AggregateLast);
  aggregator.aggregate(DoubleArray(m_volData, m_noOfPoints), Chart::AggregateSum);
  m_noOfPoints = aggregator.aggregate(DoubleArray(m_timeStamps, m_noOfPoints), Chart::AggregateFirst).len;
}

/////////////////////////////////////////////////////////////////////////////
// Chart Creation

/// <summary>
/// Add a moving average line to the FinanceChart object.
/// </summary>
/// <param name="m">The FinanceChart object to add the line to.</param>
/// <param name="avgType">The moving average type (SMA/EMA/TMA/WMA).</param>
/// <param name="avgPeriod">The moving average period.</param>
/// <param name="color">The color of the line.</param>
static LineLayer*
AddMovingAvg(FinanceChart* m, const wxString& avgType, int avgPeriod, int color)
{
  if (avgPeriod > 1)
  {
    if (avgType.IsSameAs(wxS("SMA")))
      return m->addSimpleMovingAvg(avgPeriod, color);
    else if (avgType.IsSameAs(wxS("EMA")))
      return m->addExpMovingAvg(avgPeriod, color);
    else if (avgType.IsSameAs(wxS("TMA")))
      return m->addTriMovingAvg(avgPeriod, color);
    else if (avgType.IsSameAs(wxS("WMA")))
      return m->addWeightedMovingAvg(avgPeriod, color);
  }

  return 0;
}

/// <summary>
/// Add an indicator chart to the FinanceChart object. In this demo example, the indicator
/// parameters (such as the period used to compute RSI, colors of the lines, etc.) are hard
/// coded to commonly used values. You are welcome to design a more complex user interface
/// to allow users to set the parameters.
/// </summary>
/// <param name="m">The FinanceChart object to add the line to.</param>
/// <param name="indicator">The selected indicator.</param>
/// <param name="height">Height of the chart in pixels</param>
static XYChart*
AddIndicator(FinanceChart* m, const wxString& indicator, int height)
{
  if (indicator.IsSameAs(wxS("RSI")))
    return m->addRSI(height, 14, 0x800080, 20, 0xff6666, 0x6666ff);
  else if (indicator.IsSameAs(wxS("StochRSI")))
    return m->addStochRSI(height, 14, 0x800080, 30, 0xff6666, 0x6666ff);
  else if (indicator.IsSameAs(wxS("MACD")))
    return m->addMACD(height, 26, 12, 9, 0xff, 0xff00ff, 0x8000);
  else if (indicator.IsSameAs(wxS("FStoch")))
    return m->addFastStochastic(height, 14, 3, 0x6060, 0x606000);
  else if (indicator.IsSameAs(wxS("SStoch")))
    return m->addSlowStochastic(height, 14, 3, 0x6060, 0x606000);
  else if (indicator.IsSameAs(wxS("ATR")))
    return m->addATR(height, 14, 0x808080, 0xff);
  else if (indicator.IsSameAs(wxS("ADX")))
    return m->addADX(height, 14, 0x8000, 0x800000, 0x80);
  else if (indicator.IsSameAs(wxS("DCW")))
    return m->addDonchianWidth(height, 20, 0xff);
  else if (indicator.IsSameAs(wxS("BBW")))
    return m->addBollingerWidth(height, 20, 2, 0xff);
  else if (indicator.IsSameAs(wxS("DPO")))
    return m->addDPO(height, 20, 0xff);
  else if (indicator.IsSameAs(wxS("PVT")))
    return m->addPVT(height, 0xff);
  else if (indicator.IsSameAs(wxS("Momentum")))
    return m->addMomentum(height, 12, 0xff);
  else if (indicator.IsSameAs(wxS("Performance")))
    return m->addPerformance(height, 0xff);
  else if (indicator.IsSameAs(wxS("ROC")))
    return m->addROC(height, 12, 0xff);
  else if (indicator.IsSameAs(wxS("OBV")))
    return m->addOBV(height, 0xff);
  else if (indicator.IsSameAs(wxS("AccDist")))
    return m->addAccDist(height, 0xff);
  else if (indicator.IsSameAs(wxS("CLV")))
    return m->addCLV(height, 0xff);
  else if (indicator.IsSameAs(wxS("WilliamR")))
    return m->addWilliamR(height, 14, 0x800080, 30, 0xff6666, 0x6666ff);
  else if (indicator.IsSameAs(wxS("Aroon")))
    return m->addAroon(height, 14, 0x339933, 0x333399);
  else if (indicator.IsSameAs(wxS("AroonOsc")))
    return m->addAroonOsc(height, 14, 0xff);
  else if (indicator.IsSameAs(wxS("CCI")))
    return m->addCCI(height, 20, 0x800080, 100, 0xff6666, 0x6666ff);
  else if (indicator.IsSameAs(wxS("EMV")))
    return m->addEaseOfMovement(height, 9, 0x6060, 0x606000);
  else if (indicator.IsSameAs(wxS("MDX")))
    return m->addMassIndex(height, 0x800080, 0xff6666, 0x6666ff);
  else if (indicator.IsSameAs(wxS("CVolatility")))
    return m->addChaikinVolatility(height, 10, 10, 0xff);
  else if (indicator.IsSameAs(wxS("COscillator")))
    return m->addChaikinOscillator(height, 0xff);
  else if (indicator.IsSameAs(wxS("CMF")))
    return m->addChaikinMoneyFlow(height, 21, 0x8000);
  else if (indicator.IsSameAs(wxS("NVI")))
    return m->addNVI(height, 255, 0xff, 0x883333);
  else if (indicator.IsSameAs(wxS("PVI")))
    return m->addPVI(height, 255, 0xff, 0x883333);
  else if (indicator.IsSameAs(wxS("MFI")))
    return m->addMFI(height, 14, 0x800080, 30, 0xff6666, 0x6666ff);
  else if (indicator.IsSameAs(wxS("PVO")))
    return m->addPVO(height, 26, 12, 9, 0xff, 0xff00ff, 0x8000);
  else if (indicator.IsSameAs(wxS("PPO")))
    return m->addPPO(height, 26, 12, 9, 0xff, 0xff00ff, 0x8000);
  else if (indicator.IsSameAs(wxS("UO")))
    return m->addUltimateOscillator(height, 7, 14, 28, 0x800080, 20, 0xff6666, 0x6666ff);
  else if (indicator.IsSameAs(wxS("Vol")))
    return m->addVolIndicator(height, 0x99ff99, 0xff9999, 0xc0c0c0);
  else if (indicator.IsSameAs(wxS("TRIX")))
    return m->addTRIX(height, 12, 0xff);
  else
    return 0;
}

/// <summary>
/// Creates a dummy chart to show an error message.
/// </summary>
/// <param name="viewer">The WinChartViewer to display the error message.</param>
/// <param name="msg">The error message</param>
static void
ErrorMessage(wxChartViewer* viewer, const wxString& msg)
{
  MultiChart m(400, 200);
  m.addTitle(Chart::Center, (const char*) msg.ToUTF8(), "Arial", 10)->setMaxWidth(m.getWidth());
  viewer->setChart(&m);
}

/// <summary>
/// Draw the chart according to user selection and display it in the ChartViewer.
/// </summary>
/// <param name="viewer">The ChartViewer object to display the chart.</param>
void
FinanceDemo::DrawChart(wxChartViewer* viewer)
{
  // In this demo, we just assume we plot up to the latest time. So endDate is now.
  wxDateTime endDate = wxDateTime::Now();

  // If the trading day has not yet started (before 9:30am), or if the end date is on
  // on Sat or Sun, we set the end date to 4:00pm of the last trading day
  while ((endDate.FormatISOTime() < wxString(wxS("09:30:00")))
      || (endDate.GetWeekDay() == wxDateTime::Sat)
      || (endDate.GetWeekDay() == wxDateTime::Sun))
  {
    endDate.Subtract(wxDateSpan(0, 0, 0, 1));
    endDate.SetHour(16).SetMinute(0).SetSecond(0).SetMillisecond(0);
  }

  // The duration selected by the user
  int durationInDays = m_timePeriods[m_timeRange->GetSelection()];

  // Compute the start date by subtracting the duration from the end date.
  wxDateTime startDate = endDate;
  startDate.SetHour(0).SetMinute(0).SetSecond(0).SetMillisecond(0);
  if (durationInDays >= 30)
  {
    // More or equal to 30 days - so we use months as the unit
    startDate.Subtract(wxDateSpan(0, durationInDays / 30));
    startDate.SetDay(1);
  }
  else
  {
    // Less than 30 days - use day as the unit. The starting point of the axis is
    // always at the start of the day (9:30am). Note that we use trading days, so
    // we skip Sat and Sun in counting the days.
    for (int i = 1; i < durationInDays; ++i)
    {
      if (startDate.GetWeekDay() == wxDateTime::Mon)
      {
        startDate.Subtract(wxDateSpan(0, 0, 0, 3));
      }
      else
      {
        startDate.Subtract(wxDateSpan(0,0,0,1));
      }
    }
  }

  // The first moving average period selected by the user.
  long avg1Value = 0;
  m_movAvg1->GetValue().ToLong(&avg1Value);
  m_avgPeriod1 = (avg1Value < 0) ? 0 : (avg1Value > 300) ? 300 : avg1Value;

  // The second moving average period selected by the user.
  long avg2Value = 0;
  m_movAvg2->GetValue().ToLong(&avg2Value);
  m_avgPeriod2 = (avg2Value < 0) ? 0 : (avg2Value > 300) ? 300 : avg2Value;

  // We need extra leading data points in order to compute moving averages.
  int extraPoints = (m_avgPeriod1 > m_avgPeriod2) ? m_avgPeriod1 : m_avgPeriod2;
  if (extraPoints < 25)
  {
    extraPoints = 25;
  }

  // Get the data series to compare with, if any.
  m_compareKey = m_compareWith->GetValue();
  delete[] m_compareData;
  m_compareData = 0;
  if (GetData(m_compareKey, startDate, endDate, durationInDays, extraPoints))
  {
    m_compareData = m_closeData;
    m_compareDataLen = m_noOfPoints;
    m_closeData = 0;
  }

  // The data series we want to get.
  m_tickerKey = m_tickerSymbol->GetValue();
  if (!GetData(m_tickerKey, startDate, endDate, durationInDays, extraPoints))
  {
    ErrorMessage(viewer, wxS("Please enter a valid ticker symbol"));
    return;
  }

  // We now confirm the actual number of extra points (data points that are before
  // the start date) as inferred using actual data from the database.
  double startChartTime = wxChartViewer::ConvertDateTimeToChartTime(startDate);
  for (extraPoints = 0; extraPoints < m_noOfPoints; ++extraPoints)
  {
    if (m_timeStamps[extraPoints] >= startChartTime)
      break;
  }

  // Check if there is any valid data
  if (extraPoints >= m_noOfPoints)
  {
    // No data - just display the no data message.
    ErrorMessage(viewer, wxS("No data available for the specified time period"));
    return;
  }

  // In some finance chart presentation style, even if the data for the latest day
  // is not fully available, the axis for the entire day will still be drawn, where
  // no data will appear near the end of the axis.
  int extraTrailingPoints = 0;
  if (m_resolution <= 86400)
  {
    // Add extra points to the axis until it reaches the end of the day. The end
    // of day is assumed to be 16:00 (it depends on the stock exchange).
    double lastTime = m_timeStamps[m_noOfPoints - 1];
    extraTrailingPoints = (int)((16 * 3600 - fmod(lastTime, 86400)) / m_resolution);
    if (extraTrailingPoints > 0)
    {
      double *extendedTimeStamps = new double[m_noOfPoints + extraTrailingPoints];
      memcpy(extendedTimeStamps, m_timeStamps, sizeof(double) * m_noOfPoints);
      for (int i = 0; i < extraTrailingPoints; ++i)
        extendedTimeStamps[m_noOfPoints + i] = lastTime + m_resolution * (i + 1);
      delete[] m_timeStamps;
      m_timeStamps = extendedTimeStamps;
    }
  }

  // At this stage, all data is available. We can draw the chart as according to
  // user input.

  // Determine the chart size. In this demo, user can select 4 different chart sizes.
  // Default is the large chart size.
  int width = 780;
  int mainHeight = 255;
  int indicatorHeight = 80;

  wxString selectedSize = m_chartSizes[m_chartSize->GetSelection()];
  if (selectedSize.IsSameAs(wxS("S")))
  {
    // Small chart size
    width = 450;
    mainHeight = 160;
    indicatorHeight = 60;
  }
  else if (selectedSize.IsSameAs(wxS("M")))
  {
    // Medium chart size
    width = 620;
    mainHeight = 215;
    indicatorHeight = 70;
  }
  else if (selectedSize.IsSameAs(wxS("H")))
  {
    // Huge chart size
    width = 1000;
    mainHeight = 320;
    indicatorHeight = 90;
  }

  // Create the chart object using the selected size
  FinanceChart m(width);

  // Set the data into the chart object
  m.setData(DoubleArray(m_timeStamps, m_noOfPoints + extraTrailingPoints),
            DoubleArray(m_highData, m_noOfPoints), DoubleArray(m_lowData, m_noOfPoints),
            DoubleArray(m_openData, m_noOfPoints), DoubleArray(m_closeData, m_noOfPoints),
            DoubleArray(m_volData, m_noOfPoints), extraPoints);

  // We configure the title of the chart. In this demo chart design, we put the
  // company name as the top line of the title with left alignment.
  m.addPlotAreaTitle(Chart::TopLeft, (const char*) m_tickerKey.ToUTF8());

  // We displays the current date as well as the data resolution on the next line.
  const char* resolutionText = "";
  if (m_resolution == 30 * 86400)
    resolutionText = "Monthly";
  else if (m_resolution == 7 * 86400)
    resolutionText = "Weekly";
  else if (m_resolution == 86400)
    resolutionText = "Daily";
  else if (m_resolution == 900)
    resolutionText = "15-min";

  char buffer[1024];
  sprintf(buffer, "<*font=arial.ttf,size=8*>%s - %s chart", 
    m.formatValue(wxChartViewer::ConvertDateTimeToChartTime(wxDateTime::Now()), "mmm dd, yyyy"), resolutionText);
  m.addPlotAreaTitle(Chart::BottomLeft, buffer);

  // A copyright message at the bottom left corner the title area
  m.addPlotAreaTitle(Chart::BottomRight,
    "<*font=arial.ttf,size=8*>(c) Advanced Software Engineering");

  // Add the first techical indicator according. In this demo, we draw the first
  // indicator on top of the main chart.
  m_indicator1 = m_techIndicator1->GetSelection();
  AddIndicator(&m, m_techIndicators[m_indicator1], indicatorHeight);

  // Add the main chart
  m.addMainChart(mainHeight);

  // Set log or linear scale according to user preference
  m.setLogScale(m_logScale->IsChecked());

  // Set axis labels to show data values or percentage change to user preference
  if (m_percentageScale->IsChecked())
    m.setPercentageAxis();

  // Draw the main chart depending on the chart type the user has selected
  wxString selectedType = m_chartTypes[m_chartType->GetSelection()];
  if (selectedType.IsSameAs(wxS("Close")))
    m.addCloseLine(0x000040);
  else if (selectedType.IsSameAs(wxS("TP")))
    m.addTypicalPrice(0x000040);
  else if (selectedType.IsSameAs(wxS("WC")))
    m.addWeightedClose(0x000040);
  else if (selectedType.IsSameAs(wxS("Median")))
    m.addMedianPrice(0x000040);

  // Add comparison line if there is data for comparison
  if (m_compareData != 0) {
    if (m_compareDataLen > extraPoints) {
      m.addComparison(DoubleArray(m_compareData, m_compareDataLen), 0x0000ff, (const char*) m_compareKey.ToUTF8());
    }
  }

  // Add moving average lines.
  AddMovingAvg(&m, m_movingAvgs[m_avgType1->GetSelection()], m_avgPeriod1, 0x663300);
  AddMovingAvg(&m, m_movingAvgs[m_avgType2->GetSelection()], m_avgPeriod2, 0x9900ff);

  // Draw the main chart if the user has selected CandleStick or OHLC. We
  // draw it here to make sure it is drawn behind the moving average lines
  // (that is, the moving average lines stay on top.)
  if (selectedType.IsSameAs(wxS("CandleStick")))
    m.addCandleStick(0x33ff33, 0xff3333);
  else if (selectedType.IsSameAs(wxS("OHLC")))
    m.addHLOC(0x8000, 0x800000);

  // Add parabolic SAR if necessary
  if (m_parabolicSar->IsChecked())
    m.addParabolicSAR(0.02, 0.02, 0.2, Chart::DiamondShape, 5, 0x008800, 0x000000);

  // Add price band/channel/envelop to the chart according to user selection
  wxString selectedBand = m_priceBands[m_priceBand->GetSelection()];
  if (selectedBand.IsSameAs(wxS("BB")))
    m.addBollingerBand(20, 2, 0x9999ff, 0xc06666ff);
  else if (selectedBand.IsSameAs(wxS("DC")))
    m.addDonchianChannel(20, 0x9999ff, 0xc06666ff);
  else if (selectedBand.IsSameAs(wxS("Envelop")))
    m.addEnvelop(20, 0.1, 0x9999ff, 0xc06666ff);

  // Add volume bars to the main chart if necessary
  if (m_volumeBars->IsChecked())
    m.addVolBars(indicatorHeight, 0x99ff99, 0xff9999, 0xc0c0c0);

  // Add additional indicators as according to user selection.
  m_indicator2 = m_techIndicator2->GetSelection();
  m_indicator3 = m_techIndicator3->GetSelection();
  m_indicator4 = m_techIndicator4->GetSelection();
  AddIndicator(&m, m_techIndicators[m_indicator2], indicatorHeight);
  AddIndicator(&m, m_techIndicators[m_indicator3], indicatorHeight);
  AddIndicator(&m, m_techIndicators[m_indicator4], indicatorHeight);

  // Set the chart to the viewer
  viewer->setChart(&m);

  // Set image map (for tool tips) to the viewer
  sprintf(buffer, "title='%s {value|P}'", m.getToolTipDateFormat());
  viewer->setImageMap(m.getHTMLImageMap("", "", buffer));

  // Resize and refresh window
  if (GetSizer())
  {
    GetSizer()->SetSizeHints(this);
  }
  Refresh();
}
