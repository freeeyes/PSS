/*
** Name:        realtimemeasure.cpp
** Purpose:     Real-Time Chart with Measurement Cursors
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

#include "realtimemeasure.h"

#include <sstream>

using namespace std;

#include "mondrian.xpm"

/*
 * RealTimeMeasure type definition
 */

static const int gs_dataInterval = 250;
static const int gs_chartUpdateInterval = 250;

// The mouse can drag the track line if it is within the GrabDistance to the line.
static const int gs_grabDistance = 8;

IMPLEMENT_CLASS( RealTimeMeasure, wxFrame )

/*
 * RealTimeMeasure event table definition
 */

BEGIN_EVENT_TABLE( RealTimeMeasure, wxFrame )

  EVT_TOGGLEBUTTON( ID_POINTER, RealTimeMeasure::OnPointerClick )
  EVT_TOGGLEBUTTON( wxID_ZOOM_IN, RealTimeMeasure::OnZoomInClick )
  EVT_TOGGLEBUTTON( wxID_ZOOM_OUT, RealTimeMeasure::OnZoomOutClick )
  EVT_BUTTON(wxID_SAVE, RealTimeMeasure::OnSave)
  EVT_CHECKBOX(ID_TRACKLINE1, RealTimeMeasure::OnTrackline1Click)
  EVT_CHECKBOX(ID_TRACKLINE2, RealTimeMeasure::OnTrackline2Click)
  EVT_COMMAND_SCROLL(ID_SCROLLBAR, RealTimeMeasure::OnScrollbarUpdated)

  EVT_TIMER(ID_DATA_TIMER, RealTimeMeasure::OnDataTimer)
  EVT_TIMER(ID_UPDATE_TIMER, RealTimeMeasure::OnChartUpdateTimer)

  EVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA(ID_CHARTVIEWER, RealTimeMeasure::OnMouseMovePlotArea)
  EVT_CHARTVIEWER_VIEWPORT_CHANGED(ID_CHARTVIEWER, RealTimeMeasure::OnViewPortChanged)

END_EVENT_TABLE()

/*
 * RealTimeMeasure constructors
 */

RealTimeMeasure::RealTimeMeasure()
{
  Init();
}

RealTimeMeasure::RealTimeMeasure( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create( parent, id, caption, pos, size, style );
}


/*
 * RealTimeMeasure creator
 */

bool
RealTimeMeasure::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
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
 * RealTimeMeasure destructor
 */

RealTimeMeasure::~RealTimeMeasure()
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
RealTimeMeasure::Init()
{
  m_trackLine1 = NULL;
  m_trackLine2 = NULL;
}


/*
 * Control creation for RealTimeMeasure
 */

void
RealTimeMeasure::CreateControls()
{    
  SetIcon(wxICON(mondrian));
  RealTimeMeasure* itemFrame1 = this;

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

  m_pointerButton = new wxToggleButton( itemPanel2, ID_POINTER, _(" &Pointer"), wxDefaultPosition, wxDefaultSize, wxBU_LEFT );
  m_pointerButton->SetBitmap(GetBitmapResource("pointer.png"));
  m_pointerButton->SetBitmapMargins(10, 0);
  // Initially set the mouse to drag to scroll mode
  m_pointerButton->SetValue(true);
  itemStaticBoxSizer1->Add(m_pointerButton, 0, wxGROW|wxALL, 3);

  m_zoominButton = new wxToggleButton( itemPanel2, wxID_ZOOM_IN, _(" Zoom &In"), wxDefaultPosition, wxDefaultSize, wxBU_LEFT);
  m_zoominButton->SetBitmap(GetBitmapResource("zoomin.png"));
  m_zoominButton->SetBitmapMargins(10, 0);
  m_zoominButton->SetValue(false);
  itemStaticBoxSizer1->Add(m_zoominButton, 0, wxGROW|wxALL, 3);

  m_zoomoutButton = new wxToggleButton( itemPanel2, wxID_ZOOM_OUT, _(" Zoom &Out"), wxDefaultPosition, wxDefaultSize, wxBU_LEFT);
  m_zoomoutButton->SetBitmap(GetBitmapResource("zoomout.png"));
  m_zoomoutButton->SetBitmapMargins(10, 0);
  m_zoomoutButton->SetValue(false);
  itemStaticBoxSizer1->Add(m_zoomoutButton, 0, wxGROW|wxALL, 3);

  itemStaticBoxSizer1->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

  m_saveButton = new wxButton(itemPanel2, wxID_SAVE, _(" &Save"), wxDefaultPosition, wxDefaultSize, wxBU_LEFT);
  m_saveButton->SetBitmap(GetBitmapResource("save.png"));
  m_saveButton->SetBitmapMargins(10, 0);
  itemStaticBoxSizer1->Add(m_saveButton, 0, wxGROW | wxALL, 3);

  itemStaticBoxSizer1->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

  m_trackLine1 = new wxCheckBox(itemStaticBoxSizer1->GetStaticBox(), ID_TRACKLINE1, _("Track Line 1"), wxDefaultPosition, wxDefaultSize, 0);
  m_trackLine1->SetValue(true);
  itemStaticBoxSizer1->Add(m_trackLine1, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

  m_trackLine2 = new wxCheckBox(itemStaticBoxSizer1->GetStaticBox(), ID_TRACKLINE2, _("Track Line 2"), wxDefaultPosition, wxDefaultSize, 0);
  m_trackLine2->SetValue(true);
  itemStaticBoxSizer1->Add(m_trackLine2, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

  wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer3->Add(itemBoxSizer8, 1, wxGROW|wxALL, 3);

  m_chartViewer = new wxChartViewer(itemPanel2, ID_CHARTVIEWER, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxNO_BORDER);
  m_chartViewer->SetMinSize(wxSize(650, 350));

  m_chartViewer->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemBoxSizer8->Add(m_chartViewer, 1, wxGROW | wxALL, 3);

  m_scrollBar = new wxScrollBar(itemPanel2, ID_SCROLLBAR, wxDefaultPosition, wxDefaultSize, wxSB_HORIZONTAL);
  m_scrollBar->SetScrollbar(0, 0, 1000000000, 200000000);
  itemBoxSizer8->Add(m_scrollBar, 0, wxGROW | wxALL, 3);

  // Clear data arrays to Chart::NoValue
  for (int i = 0; i < sampleSize; ++i)
  {
    m_timeStamps[i] = m_dataSeriesA[i] = m_dataSeriesB[i] = Chart::NoValue;
  }
  m_currentIndex = 0;
  m_firstChartTime = m_chartTimeLimit = Chart::NoValue;

  // Variables to keep track of the mouse for dragging track lines
  m_nearestTrackLine = -1;
  m_nearestDistance = 0;

  // Set m_nextDataTime to the current time. It is used by the real time random number
  // generator so it knows what timestamp should be used for the next data point.
  m_nextDataTime = wxDateTime::Now();

  // Enable mouse wheel zooming by setting the zoom ratio to 1.1 per wheel event
  m_chartViewer->setMouseWheelZoomRatio(1.1);

  // Set m_nextDataTime to the current time. It is used by the real time random number
  // generator so it knows what timestamp should be used for the next data point.
  m_nextDataTime = wxDateTime::Now();

  // Set up the data acquisition mechanism. In this demo, we just use a timer to get a
  // sample every 250ms.
  m_dataRateTimer = new wxTimer(this, ID_DATA_TIMER);
  m_dataRateTimer->Start(gs_dataInterval);

  // Set up the chart update timer
  m_chartUpdateTimer = new wxTimer(this, ID_UPDATE_TIMER);
  m_chartUpdateTimer->Start(gs_chartUpdateInterval);
}

// The ViewPortChanged event handler. This event occurs if the user scrolls or zooms in
// or out the chart by dragging or clicking on the chart. It can also be triggered by
// calling WinChartViewer.updateViewPort.
void
RealTimeMeasure::OnViewPortChanged(wxCommandEvent& event)
{
  // In addition to updating the chart, we may also need to update other controls that
  // changes based on the view port.
  UpdateControls(m_chartViewer);

  if (m_chartViewer->needUpdateChart())
  {
    DrawChart(m_chartViewer);
  }
  m_scrollBar->Enable(m_chartViewer->getViewPortWidth() < 1);
}

// Update controls in the user interface when the view port changed
void
RealTimeMeasure::UpdateControls(wxChartViewer* viewer)
{
  // The logical length of the scrollbar. It can be any large value. The actual value does
  // not matter.
  const int scrollBarLen = 1000000000;

  // Update the horizontal scroll bar
  int position = (int)(0.5 + viewer->getViewPortLeft() * scrollBarLen);
  int thumbSize = (int) ceil(viewer->getViewPortWidth() * scrollBarLen);
  int range = scrollBarLen /*- m_scrollBar->GetPageSize()*/;
  int pageSize = thumbSize;
  m_scrollBar->SetScrollbar(position, thumbSize, range, pageSize);
  m_scrollBar->Enable(viewer->getViewPortWidth() < 1);
}

// Draw the chart and display it in the given viewer
void
RealTimeMeasure::DrawChart(wxChartViewer* viewer)
{
  // Get the start date and end date that are visible on the chart.
  double viewPortStartDate = viewer->getValueAtViewPort("x", viewer->getViewPortLeft());
  double viewPortEndDate = viewer->getValueAtViewPort("x", viewer->getViewPortLeft() + viewer->getViewPortWidth());

  // Extract the part of the data arrays that are visible.
  DoubleArray viewPortTimeStamps;
  DoubleArray viewPortDataSeriesA;
  DoubleArray viewPortDataSeriesB;

  if (m_currentIndex > 0)
  {
    // Get the array indexes that corresponds to the visible start and end dates
    int startIndex = (int)floor(Chart::bSearch(DoubleArray(m_timeStamps, m_currentIndex), viewPortStartDate));
    int endIndex = (int)ceil(Chart::bSearch(DoubleArray(m_timeStamps, m_currentIndex), viewPortEndDate));
    int noOfPoints = endIndex - startIndex + 1;

    // Extract the visible data
    viewPortTimeStamps = DoubleArray(m_timeStamps + startIndex, noOfPoints);
    viewPortDataSeriesA = DoubleArray(m_dataSeriesA + startIndex, noOfPoints);
    viewPortDataSeriesB = DoubleArray(m_dataSeriesB + startIndex, noOfPoints);
    m_chartTimeLimit = m_timeStamps[m_currentIndex - 1];
  }

  // At this stage, we have extracted the visible data. We can use those data to plot the chart.

  //================================================================================
  // Configure overall chart appearance.
  //================================================================================

  // Create an XYChart object of size 640 x 350 pixels
  XYChart *c = new XYChart(640, 350);

  // Set the position, size and colors of the plot area
  c->setPlotArea(23, 33, c->getWidth() - 41, c->getHeight() - 53, c->linearGradientColor(0, 33, 0,
    c->getHeight() - 53, 0xf0f6ff, 0xa0c0ff), -1, Chart::Transparent, 0xffffff, 0xffffff);

  // As the data can lie outside the plotarea in a zoomed chart, we need enable clipping.
  c->setClipping();

  // Add a title to the chart using 18pt Arial font
  c->addTitle("Real-Time Chart with Measurement Cursors", "arial.ttf", 18);

  // Add a legend box at (60, 28) using horizontal layout. Use 10pt Arial Bold as font. Set the
  // background and border color to transparent and use line style legend key.
  LegendBox *b = c->addLegend(60, 28, false, "arialbd.ttf", 10);
  b->setBackground(Chart::Transparent);
  b->setLineStyleKey();

  // Set the x and y axis stems to transparent and the label font to 10pt Arial
  c->xAxis()->setColors(Chart::Transparent);
  c->yAxis()->setColors(Chart::Transparent);
  c->xAxis()->setLabelStyle("arial.ttf", 10);
  c->yAxis()->setLabelStyle("arialbd.ttf", 10, 0x336699);

  // Set the y-axis tick length to 0 to disable the tick and put the labels closer to the axis.
  c->yAxis()->setLabelGap(-1);
  c->yAxis()->setLabelAlignment(1);
  c->yAxis()->setTickLength(0);
  c->yAxis()->setMargin(20);

  // Add axis title using 12pt Arial Bold Italic font
  c->yAxis()->setTitle("Ionic Temperature (C)", "arialbd.ttf", 12);

  // Configure the x-axis tick length to 1 to put the labels closer to the axis.
  c->xAxis()->setTickLength(1);

  //================================================================================
  // Add data to chart
  //================================================================================

  // In this example, we represent the data by lines. You may modify the code below to use other
  // representations (areas, scatter plot, etc).

  // Add a line layer for the lines, using a line width of 2 pixels
  LineLayer *layer = c->addLineLayer();
  layer->setLineWidth(2);
  layer->setFastLineMode();

  // Now we add the 3 data series to a line layer, using the color red (ff0000), green (00cc00)
  // and blue (0000ff)
  layer->setXData(viewPortTimeStamps);
  layer->addDataSet(viewPortDataSeriesA, 0x00cc00, "Alpha");
  layer->addDataSet(viewPortDataSeriesB, 0x0000ff, "Beta");

  //================================================================================
  // Configure axis scale and labelling
  //================================================================================

  // Set the x-axis as a date/time axis with the scale according to the view port x range.
  if (m_currentIndex > 0)
    c->xAxis()->setDateScale(viewPortStartDate, viewPortEndDate);

  // For the automatic axis labels, set the minimum spacing to 75/30 pixels for the x/y axis.
  c->xAxis()->setTickDensity(75);
  c->yAxis()->setTickDensity(30);

  // We use "hh:nn:ss" as the axis label format.
  c->xAxis()->setLabelFormat("{value|nn:ss}");

  // We make sure the tick increment must be at least 1 second.
  c->xAxis()->setMinTickInc(1);

  //================================================================================
  // Output the chart
  //================================================================================

  // We need to update the track line too.
  TrackLineLabel(c);

  // Set the chart image to the wxChartViewer
  delete viewer->getChart();
  viewer->setChart(c);
}

// The Pointer, Zoom In or Zoom out button is pressed
void
RealTimeMeasure::OnMouseUsageChanged(int mouseUsage)
{
  m_chartViewer->setMouseUsage(mouseUsage);
}

// User clicks on the the horizontal scroll bar
void
RealTimeMeasure::OnScrollBarChanged(int value)
{
  if (!m_chartViewer->isInViewPortChangedEvent())
  {
    // Set the view port based on the scroll bar
    int scrollBarLen = m_scrollBar->GetRange()/* + m_scrollBar->GetPageSize()*/;
    m_chartViewer->setViewPortLeft(value / (double)scrollBarLen);

    // Update the chart display without updating the image maps. (We can delay updating
    // the image map until scrolling is completed and the chart display is stable.)
    m_chartViewer->updateViewPort(true, false);
  }
}

// Draw track cursor when mouse is moving over plotarea
void
RealTimeMeasure::OnMouseMovePlotArea(wxCommandEvent& event)
{
  // Mouse can drag the track lines if it is in scroll mode
  int mouseUsage = m_chartViewer->getMouseUsage();
  if (((mouseUsage != Chart::MouseUsageScroll) && (mouseUsage != Chart::MouseUsageDefault)) || (m_trackLinePos.size() == 0))
    return;

  int mouseX = m_chartViewer->getChartMouseX();

  wxMouseState mouseState = wxGetMouseState();

  // Check if mouse button is down
  if (mouseState.LeftIsDown())
  {
    // If mouse is near track line, then it is dragging the track line
    if (m_nearestTrackLine >= 0)
    {
      XYChart* c = (XYChart *)m_chartViewer->getChart();
      PlotArea* p = c->getPlotArea();

      // move the track line while ensuring the track line is in the plot area
      m_trackLinePos[m_nearestTrackLine] = (std::min)(p->getRightX(), (std::max)(p->getLeftX(), mouseX - m_nearestDistance));

      // repaint the track lines
      TrackLineLabel(c);
      m_chartViewer->updateDisplay();
    }
  }
  else
  {
    // Check which track line is nearest to the mouse
    m_nearestTrackLine = -1;
    m_nearestDistance = gs_grabDistance + 1;
    for (int i = 0; i < (int)m_trackLinePos.size(); ++i)
    {
      if (abs(mouseX - m_trackLinePos[i]) < abs(m_nearestDistance))
      {
        m_nearestTrackLine = i;
        m_nearestDistance = mouseX - m_trackLinePos[i];
      }
    }

    // If mouse is near the track line, it is used to drag the line, so disable drag to scroll.
    m_chartViewer->setMouseUsage((m_nearestTrackLine >= 0) ? Chart::MouseUsageDefault : Chart::MouseUsageScroll);
  }
}

// Draw the track line with data point labels
void
RealTimeMeasure::TrackLineLabel(XYChart* c)
{
  // Clear the current dynamic layer and get the DrawArea object to draw on it.
  c->initDynamicLayer();

  // In this example, we have two track lines.
  const int trackLineCount = 2;

  if (m_trackLinePos.size() == 0)
  {
    // Initialize the track line position by distributing them on the plot area
    PlotArea *p = c->getPlotArea();
    for (int i = 0; i < trackLineCount; ++i)
    {
      m_trackLinePos.push_back(p->getLeftX() + (int)(p->getWidth() * (i + 0.5) / trackLineCount));
    }
  }

  // Record the positions with the track lines
  std::map<std::string, double> trackLineLog[trackLineCount];

  // Draw the track lines if enabled
  if (m_trackLine1->IsChecked())
  {
    DrawTrackLine(c, m_trackLinePos[0], trackLineLog[0]);
  }
  if (m_trackLine2->IsChecked())
  {
    DrawTrackLine(c, m_trackLinePos[1], trackLineLog[1]);
  }

  // Draw the differences between the first two track lines
  DrawTrackDiff(c, trackLineLog[0], trackLineLog[1]);
}

void RealTimeMeasure::DrawTrackLine(XYChart* c, int lineX, std::map<std::string, double> &log)
{
  // The drawarea and plotarea objects
  DrawArea* d = c->getDrawArea();
  PlotArea* plotArea = c->getPlotArea();

  // Get the data x-value that is nearest to the track line, and find its pixel coordinate.
  double xValue = c->getNearestXValue(lineX);
  int xCoor = c->getXCoor(xValue);

  // Draw empty track line if it is ahead of the data
  if ((m_currentIndex <= 0) || ((xCoor < lineX) && (xValue >= m_chartTimeLimit)))
  {
    d->vline(plotArea->getTopY(), plotArea->getBottomY(), lineX, 0x888888);
    return;
  }

  // Draw a vertical track line at the x-position
  d->vline(plotArea->getTopY(), plotArea->getBottomY(), xCoor, 0x888888);

  // Draw a label on the x-axis to show the track line position.
  ostringstream xlabel;
  xlabel << "<*font,bgColor=000000*> " << c->xAxis()->getFormattedLabel(xValue, "nn:ss.ff")
    << " <*/font*>";
  TTFText *t = d->text(xlabel.str().c_str(), "arialbd.ttf", 10);
  log["x"] = xValue;

  // Restrict the x-pixel position of the label to make sure it stays inside the chart image.
  int xLabelPos = max(0, min(xCoor - t->getWidth() / 2, c->getWidth() - t->getWidth()));
  t->draw(xLabelPos, plotArea->getBottomY() + 3, 0xffffff);
  t->destroy();

  // Iterate through all layers to draw the data labels
  for (int i = 0; i < c->getLayerCount(); ++i)
  {
    Layer *layer = c->getLayerByZ(i);

    // The data array index of the x-value
    int xIndex = layer->getXIndexOf(xValue);

    // Iterate through all the data sets in the layer
    for (int j = 0; j < layer->getDataSetCount(); ++j)
    {
      DataSet *dataSet = layer->getDataSetByZ(j);
      const char *dataSetName = dataSet->getDataName();

      // Get the color, name and position of the data label
      int color = dataSet->getDataColor();
      int yCoor = c->getYCoor(dataSet->getPosition(xIndex), dataSet->getUseYAxis());

      // Draw a track dot with a label next to it for visible data points in the plot area
      if ((yCoor >= plotArea->getTopY()) && (yCoor <= plotArea->getBottomY()) &&
          (color != Chart::Transparent) && dataSetName && *dataSetName)
      {
        d->circle(xCoor, yCoor, 4, 4, color, color);

        ostringstream label;
        label << "<*font,bgColor=" << hex << color << "*> "
              << c->formatValue(dataSet->getValue(xIndex), "{value|P4}") << " <*font*>";
        t = d->text(label.str().c_str(), "arialbd.ttf", 10);
        log[dataSetName] = dataSet->getValue(xIndex);

        // Draw the label on the right side of the dot if the mouse is on the left side the
        // chart, and vice versa. This ensures the label will not go outside the chart image.
        if (xCoor <= (plotArea->getLeftX() + plotArea->getRightX()) / 2)
        {
          t->draw(xCoor + 6, yCoor, 0xffffff, Chart::Left);
        }
        else
        {
          t->draw(xCoor - 6, yCoor, 0xffffff, Chart::Right);
        }
        t->destroy();
      }
    }
  }
}

// Draw the differences between the track lines
void
RealTimeMeasure::DrawTrackDiff(XYChart *c, std::map<std::string, double> &log0, std::map<std::string, double> &log1)
{
  std::map<std::string, double>::iterator x0 = log0.find("x");
  std::map<std::string, double>::iterator x1 = log1.find("x");
  if ((x0 == log0.end()) || (x1 == log1.end()))
    return;  // Not all track lines have data

             // Two columns in the table
  ostringstream leftCol;
  ostringstream rightCol;

  leftCol << "Change in x: ";
  rightCol << c->formatValue(x1->second - x0->second, "{value|2}");

  // Iterate through all layers to draw the data labels
  for (int i = 0; i < c->getLayerCount(); ++i)
  {
    Layer *layer = c->getLayerByZ(i);

    // Iterate through all the data sets in the layer
    for (int j = 0; j < layer->getDataSetCount(); ++j)
    {
      const char *dataSetName = layer->getDataSet(j)->getDataName();
      std::map<std::string, double>::iterator i0 = log0.find(dataSetName);
      std::map<std::string, double>::iterator i1 = log1.find(dataSetName);
      if ((i0 == log0.end()) || (i1 == log1.end()))
        continue;
      leftCol << "\nChange in " << dataSetName << ": ";
      rightCol << "\n" << c->formatValue(i1->second - i0->second, "{value|2}");
    }
  }

  ostringstream table;
  table << "<*block,bgColor=80ffffff,margin=4*><*block*>" << leftCol.str()
    << "<*/*><*block,halign=right*>" << rightCol.str() << "<*/*><*/*>";

  TTFText *t = c->getDrawArea()->text(table.str().c_str(), "arial.ttf", 10);
  t->draw(c->getPlotArea()->getRightX() - t->getWidth(), c->getPlotArea()->getTopY(), 0x000000);
  t->destroy();
}

/*
 * Should we show tooltips?
 */

bool
RealTimeMeasure::ShowToolTips()
{
  return true;
}

/*
 * Get bitmap resources
 */

wxBitmap
RealTimeMeasure::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
  wxUnusedVar(name);
  if (name == wxT("pointer.png"))
  {
    wxBitmap bitmap(wxT("pointer.png"), wxBITMAP_TYPE_PNG);
    return bitmap;
  }
  else if (name == wxT("zoomin.png"))
  {
    wxBitmap bitmap(wxT("zoomin.png"), wxBITMAP_TYPE_PNG);
    return bitmap;
  }
  else if (name == wxT("zoomout.png"))
  {
    wxBitmap bitmap(wxT("zoomout.png"), wxBITMAP_TYPE_PNG);
    return bitmap;
  }
  else if (name == wxT("save.png"))
  {
    wxBitmap bitmap(wxT("save.png"), wxBITMAP_TYPE_PNG);
    return bitmap;
  }
  return wxNullBitmap;
}

/*
 * Get icon resources
 */

wxIcon
RealTimeMeasure::GetIconResource( const wxString& name )
{
  // Icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
}

/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_POINTER
 */

void
RealTimeMeasure::OnPointerClick( wxCommandEvent& event )
{
  m_pointerButton->SetValue(true);
  m_zoominButton->SetValue(false);
  m_zoomoutButton->SetValue(false);
  OnMouseUsageChanged(Chart::MouseUsageScroll);
}

/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_ZOOM_IN
 */

void
RealTimeMeasure::OnZoomInClick( wxCommandEvent& event )
{
  m_pointerButton->SetValue(false);
  m_zoominButton->SetValue(true);
  m_zoomoutButton->SetValue(false);
  OnMouseUsageChanged(Chart::MouseUsageZoomIn);
}

/*
 * wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_OUT
 */

void
RealTimeMeasure::OnZoomOutClick( wxCommandEvent& event )
{
  m_pointerButton->SetValue(false);
  m_zoominButton->SetValue(false);
  m_zoomoutButton->SetValue(true);
  OnMouseUsageChanged(Chart::MouseUsageZoomOut);
}

// The Save button is pressed
void
RealTimeMeasure::OnSave(wxCommandEvent& event)
{
  wxFileDialog saveFileDialog(this, _("Save graphics file"), "", "chartdirector_demo",
                              "PNG (*.png)|*.png|JPG (*.jpg)|*.jpg|GIF (*.gif)|*.gif|BMP (*.bmp)|*.bmp|SVG (*.svg)|*.svg|PDF (*.pdf)|*.pdf",
                              wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
  if (saveFileDialog.ShowModal() == wxID_CANCEL)
    return;     // the user changed idea...

  // save the current contents in the file;
  wxString fileName = saveFileDialog.GetPath();
  if (!fileName.IsEmpty())
  {
    // Save the chart
    BaseChart* c = m_chartViewer->getChart();
    if (0 != c)
    {
      c->makeChart(fileName.ToUTF8());
    }
  }
}

/*
* wxEVT_COMMAND_SCROLLBAR_UPDATED event handler for ID_SCROLLBAR
*/

void
RealTimeMeasure::OnScrollbarUpdated(wxScrollEvent& event)
{
  int value = event.GetPosition();
  bool enabled = m_scrollBar->IsEnabled();
  OnScrollBarChanged(value);
}

void
RealTimeMeasure::OnDataTimer(wxTimerEvent& event)
{
  // The current time
  wxDateTime now = wxDateTime::Now();

  // This is our formula for the random number generator
  do
  {
    // We need the currentTime in millisecond resolution
    double currentTime = Chart::chartTime2(m_nextDataTime.GetTicks()) + m_nextDataTime.GetMillisecond() / 1000.0;

    // Get a data sample
    double p = currentTime * 4;
    double dataA = 150 + 100 * sin(p / 27.7) * sin(p / 10.1);
    double dataB = 150 + 100 * cos(p / 6.7) * cos(p / 11.9);

    // In this demo, if the data arrays are full, the oldest 5% of data are discarded.
    if (m_currentIndex >= sampleSize)
    {
      m_currentIndex = sampleSize * 95 / 100 - 1;

      for (int i = 0; i < m_currentIndex; ++i)
      {
        int srcIndex = i + sampleSize - m_currentIndex;
        m_timeStamps[i] = m_timeStamps[srcIndex];
        m_dataSeriesA[i] = m_dataSeriesA[srcIndex];
        m_dataSeriesB[i] = m_dataSeriesB[srcIndex];
      }
    }

    // Remember the first timestamps to compute the elapsed time
    if (m_firstChartTime == Chart::NoValue)
      m_firstChartTime = currentTime;

    // Store the new values in the current index position, and increment the index.
    m_timeStamps[m_currentIndex] = currentTime - m_firstChartTime;
    m_dataSeriesA[m_currentIndex] = dataA;
    m_dataSeriesB[m_currentIndex] = dataB;
    ++m_currentIndex;

    m_nextDataTime.Add(wxTimeSpan(0, 0, 0, gs_dataInterval));
  }
  while (m_nextDataTime < now);
}

void
RealTimeMeasure::OnChartUpdateTimer(wxTimerEvent& event)
{
  if (m_currentIndex > 0)
  {
    // As we added more data, we may need to update the full range of the viewport.
    double startDate = m_timeStamps[0];
    double endDate = m_timeStamps[m_currentIndex - 1];

    // Use the initialFullRange (which is 60 seconds in this demo) if this is sufficient.
    double duration = endDate - startDate;
    if (duration < initialFullRange)
    {
      endDate = startDate + initialFullRange;
    }

    // Update the full range to reflect the actual duration of the data. In this case,
    // if the view port is viewing the latest data, we will scroll the view port as new
    // data are added. If the view port is viewing historical data, we would keep the
    // axis scale unchanged to keep the chart stable.
    int updateType = Chart::ScrollWithMax;
    if (m_chartViewer->getViewPortLeft() + m_chartViewer->getViewPortWidth() < 0.999)
    {
      updateType = Chart::KeepVisibleRange;
    }
    bool scaleHasChanged = m_chartViewer->updateFullRangeH("x", startDate, endDate, updateType);

    // Set the zoom in limit as a ratio to the full range
    m_chartViewer->setZoomInWidthLimit(zoomInLimit / (m_chartViewer->getValueAtViewPort("x", 1) - m_chartViewer->getValueAtViewPort("x", 0)));

    // Trigger the viewPortChanged event to update the display if the axis scale has changed
    // or if new data are added to the existing axis scale.
    if (scaleHasChanged || (duration < initialFullRange))
    {
      m_chartViewer->updateViewPort(true, false);
    }
  }
}

/*
* wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_TRACKLINE1
*/

void
RealTimeMeasure::OnTrackline1Click(wxCommandEvent& event)
{
  // Simply redraw the track lines
  TrackLineLabel((XYChart*) m_chartViewer->getChart());
  m_chartViewer->updateDisplay();
}

/*
* wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_TRACKLINE2
*/

void
RealTimeMeasure::OnTrackline2Click(wxCommandEvent& event)
{
  // Simply redraw the track lines
  TrackLineLabel((XYChart*) m_chartViewer->getChart());
  m_chartViewer->updateDisplay();
}
