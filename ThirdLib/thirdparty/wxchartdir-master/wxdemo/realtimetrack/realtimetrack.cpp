/*
** Name:        realtimetrack.cpp
** Purpose:     Realtime Chart with Track Line
** Author:      Ulrich Telle
** Created:     2018-07-02
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

#include "realtimetrack.h"

#include <sstream>
#include <vector>

using namespace std;

#include "mondrian.xpm"

static const int gs_dataInterval = 250;

/*
 * RealTimeTrack type definition
 */

IMPLEMENT_CLASS( RealTimeTrack, wxFrame )

/*
 * RealTimeTrack event table definition
 */

BEGIN_EVENT_TABLE( RealTimeTrack, wxFrame )

  EVT_TOGGLEBUTTON( ID_PLAY, RealTimeTrack::OnPlayClick )
  EVT_TOGGLEBUTTON( ID_PAUSE, RealTimeTrack::OnPauseClick )
  EVT_CHOICE( ID_UPDATE_PERIOD, RealTimeTrack::OnUpdatePeriodSelected )
    
  EVT_TIMER(ID_DATA_TIMER, RealTimeTrack::OnDataTimer)
  EVT_TIMER(ID_UPDATE_TIMER, RealTimeTrack::OnChartUpdateTimer)
  EVT_CHARTVIEWER_VIEWPORT_CHANGED(ID_CHARTVIEWER, RealTimeTrack::OnViewPortChanged)
  EVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA(ID_CHARTVIEWER, RealTimeTrack::OnMouseMovePlotArea)

END_EVENT_TABLE()

/*
 * RealTimeTrack constructors
 */

RealTimeTrack::RealTimeTrack()
{
  Init();
}

RealTimeTrack::RealTimeTrack( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create( parent, id, caption, pos, size, style );
}


/*
 * RealTimeTrack creator
 */

bool RealTimeTrack::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
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
 * RealTimeTrack destructor
 */

RealTimeTrack::~RealTimeTrack()
{
  m_dataRateTimer->Stop();
  m_chartUpdateTimer->Stop();
  delete m_dataRateTimer;
  delete m_chartUpdateTimer;

  if (m_chartViewer->getChart() != NULL)
  {
    delete m_chartViewer->getChart();
  }
}

/*
 * Member initialisation
 */

void
RealTimeTrack::Init()
{
  m_updatePeriod = NULL;
  m_alphaValue = NULL;
  m_betaValue = NULL;
  m_gammaValue = NULL;
}

/*
 * Control creation for RealTimeTrack
 */

void
RealTimeTrack::CreateControls()
{    
  SetIcon(wxICON(mondrian));
  RealTimeTrack* itemFrame1 = this;

  wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxVERTICAL);
  itemFrame1->SetSizer(itemBoxSizer1);

  wxPanel* itemPanel2 = new wxPanel( itemFrame1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
  itemPanel2->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemBoxSizer1->Add(itemPanel2, 1, wxGROW|wxALL, 0);
  m_bgColour = itemPanel2->GetBackgroundColour();

  wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
  itemPanel2->SetSizer(itemBoxSizer3);

  wxStaticBox* itemStaticBoxSizer1Static = new wxStaticBox(itemPanel2, wxID_ANY, wxEmptyString);
  wxStaticBoxSizer* itemStaticBoxSizer1 = new wxStaticBoxSizer(itemStaticBoxSizer1Static, wxVERTICAL);
  itemBoxSizer3->Add(itemStaticBoxSizer1, 0, wxGROW | wxALL, 3);

  m_playButton = new wxToggleButton( itemPanel2, ID_PLAY, _(" &Run"), wxDefaultPosition, wxDefaultSize, wxBU_LEFT );
  m_playButton->SetBitmap(GetBitmapResource("play.png"));
  m_playButton->SetBitmapMargins(10, 0);
  // Initially set the mouse to drag to scroll mode
  m_playButton->SetValue(true);
  itemStaticBoxSizer1->Add(m_playButton, 0, wxGROW|wxALL, 3);

  m_pauseButton = new wxToggleButton( itemPanel2, ID_PAUSE, _(" &Freeze"), wxDefaultPosition, wxDefaultSize, wxBU_LEFT);
  m_pauseButton->SetBitmap(GetBitmapResource("pause.png"));
  m_pauseButton->SetBitmapMargins(10, 0);
  m_pauseButton->SetValue(false);
  itemStaticBoxSizer1->Add(m_pauseButton, 0, wxGROW|wxALL, 3);

  itemStaticBoxSizer1->Add(5, 5, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  wxStaticText* itemStaticText4 = new wxStaticText( itemStaticBoxSizer1->GetStaticBox(), wxID_STATIC, _("Update Period (ms)"), wxDefaultPosition, wxDefaultSize, 0 );
  itemStaticBoxSizer1->Add(itemStaticText4, 0, wxALIGN_LEFT|wxALL, 5);

  wxArrayString m_updatePeriodStrings;
  m_updatePeriodStrings.Add("250");
  m_updatePeriodStrings.Add("500");
  m_updatePeriodStrings.Add("750");
  m_updatePeriodStrings.Add("1000");
  m_updatePeriodStrings.Add("1250");
  m_updatePeriodStrings.Add("1500");
  m_updatePeriodStrings.Add("1750");
  m_updatePeriodStrings.Add("2000");
  m_updatePeriod = new wxChoice( itemStaticBoxSizer1->GetStaticBox(), ID_UPDATE_PERIOD, wxDefaultPosition, wxDefaultSize, m_updatePeriodStrings, 0 );
  m_updatePeriod->SetStringSelection("1000");
  itemStaticBoxSizer1->Add(m_updatePeriod, 0, wxGROW|wxALL, 3);

  itemStaticBoxSizer1->Add(5, 5, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  wxFlexGridSizer* itemFlexGridSizer3 = new wxFlexGridSizer(0, 2, 0, 0);
  itemStaticBoxSizer1->Add(itemFlexGridSizer3, 0, wxGROW|wxALL, 3);

  wxStaticText* itemStaticText5 = new wxStaticText( itemStaticBoxSizer1->GetStaticBox(), wxID_STATIC, _("Alpha"), wxDefaultPosition, wxDefaultSize, 0 );
  itemFlexGridSizer3->Add(itemStaticText5, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 3);

  m_alphaValue = new wxTextCtrl( itemStaticBoxSizer1->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(60, -1), wxTE_READONLY|wxSTATIC_BORDER );
  m_alphaValue->Enable(false);
  itemFlexGridSizer3->Add(m_alphaValue, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 3);

  wxStaticText* itemStaticText7 = new wxStaticText( itemStaticBoxSizer1->GetStaticBox(), wxID_STATIC, _("Beta"), wxDefaultPosition, wxDefaultSize, 0 );
  itemFlexGridSizer3->Add(itemStaticText7, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 3);

  m_betaValue = new wxTextCtrl( itemStaticBoxSizer1->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(60, -1), wxTE_READONLY|wxSTATIC_BORDER );
  m_betaValue->Enable(false);
  itemFlexGridSizer3->Add(m_betaValue, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 3);

  wxStaticText* itemStaticText9 = new wxStaticText( itemStaticBoxSizer1->GetStaticBox(), wxID_STATIC, _("Gamma"), wxDefaultPosition, wxDefaultSize, 0 );
  itemFlexGridSizer3->Add(itemStaticText9, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 3);

  m_gammaValue = new wxTextCtrl( itemStaticBoxSizer1->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(60, -1), wxTE_READONLY|wxSTATIC_BORDER );
  m_gammaValue->Enable(false);
  itemFlexGridSizer3->Add(m_gammaValue, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 3);

  wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer3->Add(itemBoxSizer8, 1, wxGROW|wxALL, 3);

  m_chartViewer = new wxChartViewer(itemPanel2, ID_CHARTVIEWER, wxDefaultPosition, wxSize(600,270), wxTAB_TRAVERSAL|wxNO_BORDER);

  m_chartViewer->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemBoxSizer8->Add(m_chartViewer, 1, wxGROW | wxALL, 3);

  // Clear data arrays to Chart::NoValue
  for (int i = 0; i < sampleSize; ++i)
  {
    m_timeStamps[i] = m_dataSeriesA[i] = m_dataSeriesB[i] = m_dataSeriesC[i] = Chart::NoValue;
  }
  m_currentIndex = 0;

  // Set m_nextDataTime to the current time. It is used by the real time random number
  // generator so it knows what timestamp should be used for the next data point.
  m_nextDataTime = wxDateTime::Now();

  // Set up the data acquisition mechanism. In this demo, we just use a timer to get a
  // sample every 250ms.
  m_dataRateTimer = new wxTimer(this, ID_DATA_TIMER);
  m_dataRateTimer->Start(gs_dataInterval);

  // Set up the chart update timer
  m_chartUpdateTimer = new wxTimer(this, ID_UPDATE_TIMER);
  m_chartUpdateTimer->Start(1000);
}

// The ViewPortChanged event handler. This event occurs if the user scrolls or zooms in
// or out the chart by dragging or clicking on the chart. It can also be triggered by
// calling WinChartViewer.updateViewPort.
void
RealTimeTrack::OnViewPortChanged(wxCommandEvent& event)
{
  if (m_chartViewer->needUpdateChart())
  {
    DrawChart();
  }
}

// Update the chart. Instead of drawing the chart directly, we call updateViewPort, which
// will trigger a ViewPortChanged signal. We update the chart in the signal handler
// "drawChart". This can take advantage of the built-in rate control in wxChartViewer to
// ensure a smooth user interface, even for extremely high update rate. (See the
// documentation on wxChartViewer.setUpdateInterval).
void
RealTimeTrack::UpdateChart()
{
  m_chartViewer->updateViewPort(true, false);
}

// Draw the chart and display it in the given viewer
void
RealTimeTrack::DrawChart()
{
  // Create an XYChart object 600 x 270 pixels in size, with light grey (f4f4f4) 
  // background, black (000000) border, 1 pixel raised effect, and with a rounded frame.
  XYChart* c = new XYChart(600, 270, 0xf4f4f4, 0x000000, 1);
  c->setRoundedFrame((m_bgColour.Red() << 16) + (m_bgColour.Green() << 8) + m_bgColour.Blue());

  // Set the plotarea at (55, 55) and of size 520 x 185 pixels. Use white (ffffff) 
  // background. Enable both horizontal and vertical grids by setting their colors to 
  // grey (cccccc). Set clipping mode to clip the data lines to the plot area.
  c->setPlotArea(55, 55, 520, 185, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);
  c->setClipping();

  // Add a title to the chart using 15 pts Times New Roman Bold Italic font, with a light
  // grey (dddddd) background, black (000000) border, and a glass like raised effect.
  c->addTitle("Field Intensity at Observation Satellite", "timesbi.ttf", 15
  )->setBackground(0xdddddd, 0x000000, Chart::glassEffect());

  // Set the reference font size of the legend box
  c->getLegend()->setFontSize(8);

  // Configure the y-axis with a 10pts Arial Bold axis title
  c->yAxis()->setTitle("Intensity (V/m)", "arialbd.ttf", 10);

  // Configure the x-axis to auto-scale with at least 75 pixels between major tick and 
  // 15  pixels between minor ticks. This shows more minor grid lines on the chart.
  c->xAxis()->setTickDensity(75, 15);

  // Set the axes width to 2 pixels
  c->xAxis()->setWidth(2);
  c->yAxis()->setWidth(2);

  // Now we add the data to the chart. 
  double firstTime = m_timeStamps[0];
  if (firstTime != Chart::NoValue)
  {
    // Set up the x-axis to show the time range in the data buffer
    c->xAxis()->setDateScale(firstTime, firstTime + gs_dataInterval * sampleSize / 1000);

    // Set the x-axis label format
    c->xAxis()->setLabelFormat("{value|hh:nn:ss}");

    // Create a line layer to plot the lines
    LineLayer *layer = c->addLineLayer();

    // The x-coordinates are the timeStamps.
    layer->setXData(DoubleArray(m_timeStamps, sampleSize));

    // The 3 data series are used to draw 3 lines.
    layer->addDataSet(DoubleArray(m_dataSeriesA, sampleSize), 0xff0000, "Alpha");
    layer->addDataSet(DoubleArray(m_dataSeriesB, sampleSize), 0x00cc00, "Beta");
    layer->addDataSet(DoubleArray(m_dataSeriesC, sampleSize), 0x0000ff, "Gamma");
  }

  // Include track line with legend. If the mouse is on the plot area, show the track 
  // line with legend at the mouse position; otherwise, show them for the latest data
  // values (that is, at the rightmost position).
  TrackLineLegend(c, m_chartViewer->isMouseOnPlotArea() ? m_chartViewer->getPlotAreaMouseX() :
    c->getPlotArea()->getRightX());


  if (m_chartViewer->getChart() != NULL)
  {
    delete m_chartViewer->getChart();
  }
  // Set the chart image to the WinChartViewer
  m_chartViewer->setChart(c);
}

// Draw the track line with legend
void
RealTimeTrack::TrackLineLegend(XYChart* c, int mouseX)
{
  // Clear the current dynamic layer and get the DrawArea object to draw on it.
  DrawArea *d = c->initDynamicLayer();

  // The plot area object
  PlotArea *plotArea = c->getPlotArea();

  // Get the data x-value that is nearest to the mouse, and find its pixel coordinate.
  double xValue = c->getNearestXValue(mouseX);
  int xCoor = c->getXCoor(xValue);

  // Draw a vertical track line at the x-position
  d->vline(plotArea->getTopY(), plotArea->getBottomY(), xCoor, d->dashLineColor(0x000000, 0x0101));

  // Container to hold the legend entries
  std::vector<string> legendEntries;

  // Iterate through all layers to build the legend array
  for (int i = 0; i < c->getLayerCount(); ++i)
  {
    Layer *layer = c->getLayerByZ(i);

    // The data array index of the x-value
    int xIndex = layer->getXIndexOf(xValue);

    // Iterate through all the data sets in the layer
    for (int j = 0; j < layer->getDataSetCount(); ++j)
    {
      DataSet *dataSet = layer->getDataSetByZ(j);

      // We are only interested in visible data sets with names
      const char *dataName = dataSet->getDataName();
      int color = dataSet->getDataColor();
      if (dataName && *dataName && (color != (int)Chart::Transparent))
      {
        // Build the legend entry, consist of the legend icon, name and data value.
        double dataValue = dataSet->getValue(xIndex);
        ostringstream legendEntry;
        legendEntry << "<*block*>" << dataSet->getLegendIcon() << " " << dataName << ": " <<
          ((dataValue == Chart::NoValue) ? "N/A" : c->formatValue(dataValue, "{value|P4}"))
          << "<*/*>";
        legendEntries.push_back(legendEntry.str());

        // Draw a track dot for data points within the plot area
        int yCoor = c->getYCoor(dataSet->getPosition(xIndex), dataSet->getUseYAxis());
        if ((yCoor >= plotArea->getTopY()) && (yCoor <= plotArea->getBottomY()))
        {
          d->circle(xCoor, yCoor, 4, 4, color, color);
        }
      }
    }
  }

  // Create the legend by joining the legend entries
  ostringstream legendText;
  legendText << "<*block,maxWidth=" << plotArea->getWidth() << "*><*block*><*font=arialbd.ttf*>["
    << c->xAxis()->getFormattedLabel(xValue, "hh:nn:ss") << "]<*/*>";
  for (int i = ((int)legendEntries.size()) - 1; i >= 0; --i)
  {
    legendText << "        " << legendEntries[i];
  }

  // Display the legend on the top of the plot area
  TTFText *t = d->text(legendText.str().c_str(), "arial.ttf", 8);
  t->draw(plotArea->getLeftX() + 5, plotArea->getTopY() - 3, 0x000000, Chart::BottomLeft);
  t->destroy();
}

/*
 * Should we show tooltips?
 */

bool
RealTimeTrack::ShowToolTips()
{
  return true;
}

/*
 * Get bitmap resources
 */

wxBitmap
RealTimeTrack::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
  wxUnusedVar(name);
  if (name == wxT("play.png"))
  {
    wxBitmap bitmap(wxT("play.png"), wxBITMAP_TYPE_PNG);
    return bitmap;
  }
  else if (name == wxT("pause.png"))
  {
    wxBitmap bitmap(wxT("pause.png"), wxBITMAP_TYPE_PNG);
    return bitmap;
  }
  return wxNullBitmap;
}

/*
 * Get icon resources
 */

wxIcon
RealTimeTrack::GetIconResource( const wxString& name )
{
  // Icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
}

/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_PLAY
 */

void
RealTimeTrack::OnPlayClick( wxCommandEvent& event )
{
  m_playButton->SetValue(true);
  m_pauseButton->SetValue(false);
  OnRunFreezeChanged(true);
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_PAUSE
 */

void
RealTimeTrack::OnPauseClick( wxCommandEvent& event )
{
  m_playButton->SetValue(false);
  m_pauseButton->SetValue(true);
  OnRunFreezeChanged(false);
}

// The Run or Freeze button is pressed
void RealTimeTrack::OnRunFreezeChanged(bool run)
{
  if (run)
  {
    m_chartUpdateTimer->Start();
  }
  else
  {
    m_chartUpdateTimer->Stop();
  }
}

/*
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_UPDATE_PERIOD
 */

void
RealTimeTrack::OnUpdatePeriodSelected( wxCommandEvent& event )
{
  OnUpdatePeriodChanged(m_updatePeriod->GetString(m_updatePeriod->GetSelection()));
}

// User changes the chart update period
void RealTimeTrack::OnUpdatePeriodChanged(const wxString& text)
{
  long interval;
  text.ToLong(&interval);
  m_chartUpdateTimer->Start(interval);
}

void RealTimeTrack::OnChartUpdateTimer(wxTimerEvent& event)
{
  UpdateChart();
}

void RealTimeTrack::OnDataTimer(wxTimerEvent& event)
{
  GetData();
}

// A utility to shift a new data value into a data array
static void
ShiftData(double* data, int len, double newValue)
{
  memmove(data, data+1, sizeof(*data) * (len - 1));
  data[len-1] = newValue;
}

// Shift new data values into the real time data series
void
RealTimeTrack::GetData()
{
  // The current time
  wxDateTime now = wxDateTime::Now();

  // This is our formula for the random number generator
  do
  {
    // We need the currentTime in millisecond resolution
    double currentTime = Chart::chartTime2(m_nextDataTime.GetTicks())
                         + m_nextDataTime.GetMillisecond() / 1000.0;

    // Get a data sample
    double p = currentTime * 4;
    double dataA = 20 + cos(p * 129241) * 10 + 1 / (cos(p) * cos(p) + 0.01);
    double dataB = 150 + 100 * sin(p / 27.7) * sin(p / 10.1);
    double dataC = 150 + 100 * cos(p / 6.7) * cos(p / 11.9);

    // After obtaining the new values, we need to update the data arrays.
    if (m_currentIndex < sampleSize)
    {
      // Store the new values in the current index position, and increment the index.
      m_dataSeriesA[m_currentIndex] = dataA;
      m_dataSeriesB[m_currentIndex] = dataB;
      m_dataSeriesC[m_currentIndex] = dataC;
      m_timeStamps[m_currentIndex] = currentTime;
      ++m_currentIndex;
    }
    else
    {
      // The data arrays are full. Shift the arrays and store the values at the end.
      ShiftData(m_dataSeriesA, sampleSize, dataA);
      ShiftData(m_dataSeriesB, sampleSize, dataB);
      ShiftData(m_dataSeriesC, sampleSize, dataC);
      ShiftData(m_timeStamps, sampleSize, currentTime);
    }


    m_nextDataTime.Add(wxTimeSpan(0, 0, 0, gs_dataInterval));
  }
  while (m_nextDataTime < now);

  // We provide some visual feedback to the latest numbers generated, so you can see the
  // data being generated.
  m_alphaValue->SetValue(wxString::Format("%.2f", m_dataSeriesA[m_currentIndex -1]));
  m_betaValue->SetValue(wxString::Format("%.2f", m_dataSeriesB[m_currentIndex -1]));
  m_gammaValue->SetValue(wxString::Format("%.2f", m_dataSeriesC[m_currentIndex -1]));
}

// Draw track cursor when mouse is moving over plotarea
void RealTimeTrack::OnMouseMovePlotArea(wxCommandEvent& event)
{
  TrackLineLegend((XYChart*) m_chartViewer->getChart(), m_chartViewer->getPlotAreaMouseX());
  m_chartViewer->updateDisplay();
}
