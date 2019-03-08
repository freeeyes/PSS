/*
** Name:        realtimesweepapp.cpp
** Purpose:     Real-Time Sweep Chart
** Author:      Ulrich Telle
** Created:     2018-05-09
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

#include "realtimesweep.h"

#include <sstream>

using namespace std;

#include "mondrian.xpm"

static const int gs_dataInterval = 250;

/*
 * RealTimeSweep type definition
 */

IMPLEMENT_CLASS( RealTimeSweep, wxFrame )

/*
 * RealTimeSweep event table definition
 */

BEGIN_EVENT_TABLE( RealTimeSweep, wxFrame )

  EVT_SIZE(RealTimeSweep::OnResizeEvent)
  EVT_TIMER(ID_UPDATE_TIMER, RealTimeSweep::OnChartUpdateTimer)
  EVT_CHARTVIEWER_VIEWPORT_CHANGED(ID_CHARTVIEWER, RealTimeSweep::OnViewPortChanged)

END_EVENT_TABLE()

/*
 * RealTimeSweep constructors
 */

RealTimeSweep::RealTimeSweep()
{
  Init();
}

RealTimeSweep::RealTimeSweep( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create( parent, id, caption, pos, size, style );
}

/*
 * RealTimeSweep creator
 */

bool
RealTimeSweep::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
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
 * RealTimeSweep destructor
 */

RealTimeSweep::~RealTimeSweep()
{
  m_chartUpdateTimer->Stop();
  delete m_chartUpdateTimer;

  if (m_chartViewer->getChart() != NULL)
  {
    delete m_chartViewer->getChart();
  }
  m_dataSource->Delete();
}

/*
 * Member initialisation
 */

void
RealTimeSweep::Init()
{
}

/*
 * Control creation for RealTimeSweep
 */

void
RealTimeSweep::CreateControls()
{    
  SetIcon(wxICON(mondrian));
  RealTimeSweep* itemFrame1 = this;

  wxBoxSizer* itemBoxSizer1 = new wxBoxSizer(wxVERTICAL);
  itemFrame1->SetSizer(itemBoxSizer1);

  wxPanel* itemPanel2 = new wxPanel( itemFrame1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER|wxTAB_TRAVERSAL );
  itemPanel2->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemBoxSizer1->Add(itemPanel2, 1, wxGROW|wxALL, 0);
  m_bgColour = itemPanel2->GetBackgroundColour();

  wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
  itemPanel2->SetSizer(itemBoxSizer3);

  wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer3->Add(itemBoxSizer8, 1, wxGROW|wxALL, 0);

  m_chartViewer = new wxChartViewer(itemPanel2, ID_CHARTVIEWER, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxNO_BORDER);
  m_chartViewer->SetMinSize(wxSize(650, 350));

  m_chartViewer->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemBoxSizer8->Add(m_chartViewer, 1, wxGROW | wxALL, 0);

  // Initialize member variables
  m_currentIndex = 0;

  // Start the random data generator
  m_dataSource = new RandomWave(OnData, this);
  m_dataSource->Create();
  m_dataSource->Run();

  // Set up the chart update timer
  m_chartUpdateTimer = new wxTimer(this, ID_UPDATE_TIMER);
  m_chartUpdateTimer->Start(100);
  sw.Start();
}

// Update the chart if the window size is changed
void
RealTimeSweep::OnResizeEvent(wxSizeEvent& event)
{
  wxDateTime now = wxDateTime::Now();
  wxRect r = event.GetRect();
  wxSize s = event.GetSize();
  m_chartViewer->updateViewPort(true, false);
  event.Skip();
}

// The ViewPortChanged event handler. This event occurs if the user scrolls or zooms in
// or out the chart by dragging or clicking on the chart. It can also be triggered by
// calling WinChartViewer.updateViewPort.
void
RealTimeSweep::OnViewPortChanged(wxCommandEvent& event)
{
  // Update the chart if necessary
  if (m_chartViewer->needUpdateChart())
  {
    DrawChart(m_chartViewer);
  }
}

// Draw the chart and display it in the given viewer
void
RealTimeSweep::DrawChart(wxChartViewer* viewer)
{
  // Have not started collecting data ???
  if (m_currentIndex <= 0)
    return;

  //================================================================================
  // Configure overall chart appearance.
  //================================================================================

  // Create an XYChart object extending to the container boundary, with a minimum of 300 x 150
  XYChart* c = new XYChart(std::max(300, viewer->GetParent()->GetSize().GetWidth()),
                           std::max(150, viewer->GetParent()->GetSize().GetHeight()));
  SetMinSize(ClientToWindowSize(wxSize(300, 150)));

  // Set the plotarea at (0, 0) with width 1 pixel less than chart width, and height 20 pixels
  // less than chart height. Use a vertical gradient from light blue (f0f6ff) to sky blue (a0c0ff)
  // as background. Set border to transparent and grid lines to white (ffffff).
  c->setPlotArea(0, 0, c->getWidth() - 1, c->getHeight() - 20, c->linearGradientColor(0, 0, 0,
    c->getHeight() - 20, 0xf0f6ff, 0xa0c0ff), -1, Chart::Transparent, 0xffffff, 0xffffff);

  // In our code, we can overdraw the line slightly, so we clip it to the plot area.
  c->setClipping();

  // Add a legend box at the right side using horizontal layout. Use 10pt Arial Bold as font.
  // Set the background and border color to Transparent and use line style legend key.
  LegendBox *b = c->addLegend(c->getWidth() - 1, 10, false, "arialbd.ttf", 10);
  b->setBackground(Chart::Transparent);
  b->setAlignment(Chart::Right);
  b->setLineStyleKey();

  // Set the x and y axis stems to transparent and the label font to 10pt Arial
  c->xAxis()->setColors(Chart::Transparent);
  c->yAxis()->setColors(Chart::Transparent);
  c->xAxis()->setLabelStyle("arial.ttf", 10);
  c->yAxis()->setLabelStyle("arial.ttf", 10, 0x336699);

  // Configure the y-axis label to be inside the plot area and above the horizontal grid lines
  c->yAxis()->setLabelGap(-1);
  c->yAxis()->setMargin(20);
  c->yAxis()->setLabelAlignment(1);

  // Configure the x-axis labels to be to the left of the vertical grid lines
  c->xAxis()->setLabelAlignment(1);

  //================================================================================
  // Add data to chart
  //================================================================================

  // We only need data form the latest time range.
  double startTime = m_timeStamps[m_currentIndex - 1] - timeRange;
  int startIndex = (int)ceil(Chart::bSearch(DoubleArray(m_timeStamps, m_currentIndex), startTime) - 0.1);

  // For a sweep chart, if the data overflow the right border (which should be a multiple
  // of the time range), it will wrap back to the left.
  double wrapTime = floor(startTime / timeRange + 1) * timeRange;
  double wrapIndex = Chart::bSearch(DoubleArray(m_timeStamps, m_currentIndex), wrapTime);

  // First, draw the part of the line from the start time to the wrap time.

  LineLayer *layerA = c->addLineLayer();
  layerA->setLineWidth(2);
  layerA->setFastLineMode();

  int lenA = (int)ceil(wrapIndex) - startIndex + 1;

  // The x-coordinates needs to be normalized to 0 - time_range.
  layerA->setXData(ArrayMath(DoubleArray(m_timeStamps + startIndex, lenA)).sub(
    wrapTime - timeRange).result());

  layerA->addDataSet(DoubleArray(m_channel1 + startIndex, lenA), 0xff0000, "Channel 1");
  layerA->addDataSet(DoubleArray(m_channel2 + startIndex, lenA), 0x00cc00, "Channel 2");

  // Second, draw the part of the line from the wrap time to the latest time.

  LineLayer *layerB = c->addLineLayer();
  layerB->setLineWidth(2);
  layerB->setFastLineMode();

  int wrapStart = (int)floor(wrapIndex);
  int lenB = m_currentIndex - wrapStart;

  // The x-coordinates need to be adjusted to 0 - time_range.
  layerB->setXData(ArrayMath(DoubleArray(m_timeStamps + wrapStart, lenB)).sub(wrapTime).result());

  layerB->addDataSet(DoubleArray(m_channel1 + wrapStart, lenB), 0xff0000, "Channel 1");
  layerB->addDataSet(DoubleArray(m_channel2 + wrapStart, lenB), 0x00cc00, "Channel 2");

  // Disable legend for the second part to avoid duplication with the first part.
  layerB->setLegend(Chart::NoLegend);

  // Finally, we add symbols and labels for the vertical sweep line.

  // The layerB contains the latest data. Add a vertical line at the latest position.
  Mark* m = c->xAxis()->addMark(layerB->getXPosition(lenB - 1), -1);
  m->setMarkColor(0x0000ff, Chart::Transparent, Chart::Transparent);
  m->setDrawOnTop(false);

  // Add symbols for each data series at the latest position
  for (int i = 0; i < layerB->getDataSetCount(); ++i)
  {
    // Add the symbol
    DataSet *dataSet = layerB->getDataSetByZ(i);
    double lastX = layerB->getXPosition(lenB - 1);
    double lastY = dataSet->getPosition(lenB - 1);
    Layer *labelLayer = c->addScatterLayer(DoubleArray(&lastX, 1), DoubleArray(&lastY, 1),
      "", Chart::CircleSymbol, 9, dataSet->getDataColor(), Chart::SameAsMainColor);
    labelLayer->moveFront();

    // Add the label
    char labelFormat[1024];
    sprintf(labelFormat, "<*font,bgColor=%x*> {value|P4} <*/font*>", dataSet->getDataColor());
    labelLayer->setDataLabelFormat(labelFormat);

    // The label style
    TextBox *t = labelLayer->setDataLabelStyle("arialbd.ttf", 10, 0xffffff);
    bool isOnLeft = lastX <= timeRange / 2;
    t->setAlignment(isOnLeft ? Chart::Left : Chart::Right);
    t->setMargin(isOnLeft ? 5 : 0, isOnLeft ? 0 : 5, 0, 0);
  }

  //================================================================================
  // Configure axis scale and labelling
  //================================================================================

  c->xAxis()->setLinearScale(0, timeRange);

  // For the automatic axis labels, set the minimum spacing to 75/40 pixels for the x/y axis.
  c->xAxis()->setTickDensity(75);
  c->yAxis()->setTickDensity(40);

  // Set the auto-scale margin to 0.05, and the zero affinity to 0.6
  c->yAxis()->setAutoScale(0.05, 0.05, 0.6);

  //================================================================================
  // Output the chart
  //================================================================================

  // Set the chart image to the CChartViewer
  if (m_chartViewer->getChart() != NULL)
  {
    delete m_chartViewer->getChart();
  }
  // Set the chart image to the WinChartViewer
  m_chartViewer->setChart(c);
}

/*
 * Should we show tooltips?
 */

bool
RealTimeSweep::ShowToolTips()
{
  return true;
}

/*
 * Get bitmap resources
 */

wxBitmap
RealTimeSweep::GetBitmapResource( const wxString& name )
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
RealTimeSweep::GetIconResource( const wxString& name )
{
    // Icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
}

// Handles realtime data from RandomWalk. The RandomWalk will call this method from its own thread.
// This is a static method.
void
RealTimeSweep::OnData(void* self, double elapsedTime, double series0, double series1)
{
  // Copy the data into a structure and store it in the queue.
  DataPacket packet;
  packet.elapsedTime = elapsedTime;
  packet.series0 = series0;
  packet.series1 = series1;

  ((RealTimeSweep*) self)->buffer.Put(packet);
}

void
RealTimeSweep::OnChartUpdateTimer(wxTimerEvent& event)
{
  // Get new data from the queue and append them to the data arrays
  int count;
  DataPacket* packets;
  if ((count = buffer.Get(&packets)) <= 0)
    return;

  // if data arrays have insufficient space, we need to remove some old data.
  if (m_currentIndex + count >= sampleSize)
  {
    // For safety, we check if the queue contains too much data than the entire data arrays. If
    // this is the case, we only use the latest data to completely fill the data arrays.
    if (count > sampleSize)
    {
      packets += count - sampleSize;
      count = sampleSize;
    }

    // Remove data older than the time range to leave space for new data. The data removed must
    // be at least equal to the packet count.
    int originalIndex = m_currentIndex;
    if (m_currentIndex > 0)
    {
      m_currentIndex -= (int)(Chart::bSearch(DoubleArray(m_timeStamps, m_currentIndex),
        m_timeStamps[m_currentIndex - 1] - timeRange));
    }
    if (m_currentIndex > sampleSize - count)
      m_currentIndex = sampleSize - count;

    for (int i = 0; i < m_currentIndex; ++i)
    {
      int srcIndex = i + originalIndex - m_currentIndex;
      m_timeStamps[i] = m_timeStamps[srcIndex];
      m_channel1[i] = m_channel1[srcIndex];
      m_channel2[i] = m_channel2[srcIndex];
    }
  }

  // Append the data from the queue to the data arrays
  for (int n = 0; n < count; ++n)
  {
    m_timeStamps[m_currentIndex] = packets[n].elapsedTime;
    m_channel1[m_currentIndex] = packets[n].series0;
    m_channel2[m_currentIndex] = packets[n].series1;
    ++m_currentIndex;
  }

  m_chartViewer->updateViewPort(true, false);
}
