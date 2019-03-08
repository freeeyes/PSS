/*
** Name:        realtimeviewport.cpp
** Purpose:     Real-Time Chart with Viewport Control
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

#include "realtimeviewport.h"

#include <sstream>

using namespace std;

#include "mondrian.xpm"

/*
 * RealTimeViewPort type definition
 */

IMPLEMENT_CLASS( RealTimeViewPort, wxFrame )

/*
 * RealTimeViewPort event table definition
 */

BEGIN_EVENT_TABLE( RealTimeViewPort, wxFrame )

  EVT_TOGGLEBUTTON( ID_POINTER, RealTimeViewPort::OnPointerClick )
  EVT_TOGGLEBUTTON( wxID_ZOOM_IN, RealTimeViewPort::OnZoomInClick )
  EVT_TOGGLEBUTTON( wxID_ZOOM_OUT, RealTimeViewPort::OnZoomOutClick )
  EVT_BUTTON( wxID_SAVE, RealTimeViewPort::OnSave )
  EVT_TIMER(ID_UPDATE_TIMER, RealTimeViewPort::OnChartUpdateTimer)
  EVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA(ID_CHARTVIEWER, RealTimeViewPort::OnMouseMovePlotArea)
  EVT_CHARTVIEWER_VIEWPORT_CHANGED(ID_CHARTVIEWER, RealTimeViewPort::OnViewPortChanged)

END_EVENT_TABLE()

/*
 * RealTimeViewPort constructors
 */

RealTimeViewPort::RealTimeViewPort()
{
  Init();
}

RealTimeViewPort::RealTimeViewPort( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create( parent, id, caption, pos, size, style );
}


/*
 * RealTimeViewPort creator
 */

bool RealTimeViewPort::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
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
 * RealTimeViewPort destructor
 */

RealTimeViewPort::~RealTimeViewPort()
{
  m_chartUpdateTimer->Stop();
  delete m_chartUpdateTimer;

  if (m_viewPortControl->getChart() != NULL)
  {
    delete m_viewPortControl->getChart();
  }
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
RealTimeViewPort::Init()
{
}

/*
 * Control creation for RealTimeViewPort
 */

void
RealTimeViewPort::CreateControls()
{    
  SetIcon(wxICON(mondrian));
  RealTimeViewPort* itemFrame1 = this;

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

  wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer3->Add(itemBoxSizer8, 1, wxGROW|wxALL, 0);

  m_chartViewer = new wxChartViewer(itemPanel2, ID_CHARTVIEWER, wxDefaultPosition, wxSize(640,350), wxTAB_TRAVERSAL|wxNO_BORDER);
  m_chartViewer->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemBoxSizer8->Add(m_chartViewer, 0, wxGROW | wxALL, 3);

  m_viewPortControl = new wxViewPortControl(itemPanel2, ID_VIEWPORT, wxDefaultPosition, wxSize(640,60), wxTAB_TRAVERSAL | wxNO_BORDER);
  m_viewPortControl->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemBoxSizer8->Add(m_viewPortControl, 0, wxGROW | wxALL, 3);

  // Initialize member variables
  m_currentIndex = 0;

  // Initially, auto-move the track line to make it follow the data series
  trackLineEndPos = 0;
  trackLineIsAtEnd = true;

  // Enable mouse wheel zooming by setting the zoom ratio to 1.1 per wheel event
  m_chartViewer->setMouseWheelZoomRatio(1.1);

  // Configure the initial viewport
  m_chartViewer->setViewPortWidth(initialVisibleRange / (double)initialFullRange);

  // Start the random data generator
  m_dataSource = new RandomWalk(OnData, this);
  m_dataSource->Create();
  m_dataSource->Run();

  // Set up the chart update timer
  m_chartUpdateTimer = new wxTimer(this, ID_UPDATE_TIMER);
  m_chartUpdateTimer->Start(100);

  // Bind the CChartViewer to the CViewPortControl
  m_viewPortControl->setViewer(m_chartViewer);
}

// The ViewPortChanged event handler. This event occurs if the user scrolls or zooms in
// or out the chart by dragging or clicking on the chart. It can also be triggered by
// calling WinChartViewer.updateViewPort.
void
RealTimeViewPort::OnViewPortChanged(wxCommandEvent& event)
{
  // Update chart if necessary
  if (m_chartViewer->needUpdateChart())
  {
    DrawChart(m_chartViewer);
  }
  // Update the full chart
  DrawFullChart(m_viewPortControl);
}

// Draw the chart and display it in the given viewer
void
RealTimeViewPort::DrawChart(wxChartViewer* viewer)
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
    if (m_timeStamps[endIndex] >= viewPortStartDate)
    {
      viewPortTimeStamps = DoubleArray(m_timeStamps + startIndex, noOfPoints);
      viewPortDataSeriesA = DoubleArray(m_dataSeriesA + startIndex, noOfPoints);
      viewPortDataSeriesB = DoubleArray(m_dataSeriesB + startIndex, noOfPoints);
    }

    // Keep track of the latest available data at chart plotting time
    trackLineEndPos = m_timeStamps[m_currentIndex - 1];
  }

  // At this stage, we have extracted the visible data. We can use those data to plot the chart.

  //================================================================================
  // Configure overall chart appearance.
  //================================================================================

  // Create an XYChart object of size 640 x 350 pixels
  XYChart* c = new XYChart(640, 350);

  // Set the plotarea at (20, 30) with width 41 pixels less than chart width, and height 50 pixels
  // less than chart height. Use a vertical gradient from light blue (f0f6ff) to sky blue (a0c0ff)
  // as background. Set border to transparent and grid lines to white (ffffff).
  c->setPlotArea(20, 30, c->getWidth() - 41, c->getHeight() - 50, c->linearGradientColor(0, 30, 0,
    c->getHeight() - 50, 0xf0f6ff, 0xa0c0ff), -1, Chart::Transparent, 0xffffff, 0xffffff);

  // As the data can lie outside the plotarea in a zoomed chart, we need enable clipping.
  c->setClipping();

  // Add a title to the chart using 18pt Arial font
  c->addTitle("   Multithreading Real-Time Chart", "arial.ttf", 18);

  // Add a legend box at (55, 25) using horizontal layout. Use 10pt Arial Bold as font. Set the
  // background and border color to transparent and use line style legend key.
  LegendBox *b = c->addLegend(55, 25, false, "arialbd.ttf", 10);
  b->setBackground(Chart::Transparent);
  b->setLineStyleKey();

  // Set the x and y axis stems to transparent and the label font to 10pt Arial
  c->xAxis()->setColors(Chart::Transparent);
  c->yAxis()->setColors(Chart::Transparent);
  c->xAxis()->setLabelStyle("arial.ttf", 10);
  c->yAxis()->setLabelStyle("arial.ttf", 10, 0x336699);

  // Set the y-axis tick length to 0 to disable the tick and put the labels closer to the axis.
  c->yAxis()->setTickLength(0);

  // Add axis title using 10pt Arial Bold Italic font
  c->yAxis()->setTitle("Ionic Temperature (C)", "arialbd.ttf", 10);

  // Configure the y-axis label to be inside the plot area and above the horizontal grid lines
  c->yAxis()->setLabelGap(-1);
  c->yAxis()->setLabelAlignment(1);
  c->yAxis()->setMargin(20);

  // Configure the x-axis labels to be to the left of the vertical grid lines
  c->xAxis()->setLabelAlignment(1);

  //================================================================================
  // Add data to chart
  //================================================================================

  // In this example, we represent the data by lines. You may modify the code below to use other
  // representations (areas, scatter plot, etc).

  // Add a line layer for the lines, using a line width of 2 pixels
  LineLayer *layer = c->addLineLayer();
  layer->setLineWidth(2);
  layer->setFastLineMode();

  // Now we add the 2 data series to the line layer with red (ff0000) and green (00cc00) colors
  layer->setXData(viewPortTimeStamps);
  layer->addDataSet(viewPortDataSeriesA, 0xff0000, "Alpha");
  layer->addDataSet(viewPortDataSeriesB, 0x00cc00, "Beta");

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
  c->xAxis()->setLabelFormat("{value|hh:nn:ss}");

  // We make sure the tick increment must be at least 1 second.
  c->xAxis()->setMinTickInc(1);

  // Set the auto-scale margin to 0.05, and the zero affinity to 0.6
  c->yAxis()->setAutoScale(0.05, 0.05, 0.6);

  ///////////////////////////////////////////////////////////////////////////////////////
  // Output the chart
  ///////////////////////////////////////////////////////////////////////////////////////
    
  // We need to update the track line too. If the mouse is moving on the chart (eg. if
  // the user drags the mouse on the chart to scroll it), the track line will be updated
  // in the MouseMovePlotArea event. Otherwise, we need to update the track line here.
  if (!viewer->isInMouseMoveEvent())
    TrackLineLabel(c, trackLineIsAtEnd ? c->getWidth() : viewer->getPlotAreaMouseX());

  delete viewer->getChart();
  viewer->setChart(c);
}

void
RealTimeViewPort::DrawFullChart(wxViewPortControl* vpc)
{
  // Create an XYChart object of size 640 x 60 pixels
  XYChart* c = new XYChart(640, 60);

  // Set the plotarea with the same horizontal position as that in the main chart for alignment.
  c->setPlotArea(20, 0, c->getWidth() - 41, c->getHeight() - 1, 0xc0d8ff, -1, 0xcccccc, Chart::Transparent, 0xffffff);

  // Set the x axis stem to transparent and the label font to 10pt Arial
  c->xAxis()->setColors(Chart::Transparent);
  c->xAxis()->setLabelStyle("Arial", 10);

  // Put the x-axis labels inside the plot area by setting a negative label gap. Use
  // setLabelAlignment to put the label at the right side of the tick.
  c->xAxis()->setLabelGap(-1);
  c->xAxis()->setLabelAlignment(1);

  // Set the y axis stem and labels to transparent (that is, hide the labels)
  c->yAxis()->setColors(Chart::Transparent, Chart::Transparent);

  // Add a line layer for the lines with fast line mode enabled
  LineLayer *layer = c->addLineLayer();
  layer->setFastLineMode();

  // Now we add the 3 data series to a line layer, using the color red (0xff3333), green
  // (0x008800) and blue (0x3333cc)
  layer->setXData(DoubleArray(m_timeStamps, m_currentIndex));
  layer->addDataSet(DoubleArray(m_dataSeriesA, m_currentIndex), 0xff3333);
  layer->addDataSet(DoubleArray(m_dataSeriesB, m_currentIndex), 0x008800);

  // The x axis scales should reflect the full range of the view port
  c->xAxis()->setDateScale(vpc->getViewer()->getValueAtViewPort("x", 0),
    vpc->getViewer()->getValueAtViewPort("x", 1));
  c->xAxis()->setLabelFormat("{value|nn:ss}");

  // For the automatic x-axis labels, set the minimum spacing to 75 pixels.
  c->xAxis()->setTickDensity(75);

  // For the auto-scaled y-axis, as we hide the labels, we can disable axis rounding. This can
  // make the axis scale fit the data tighter.
  c->yAxis()->setRounding(false, false);

  // Output the chart
  delete vpc->getChart();
  vpc->setChart(c);
}

// The Pointer, Zoom In or Zoom out button is pressed
void
RealTimeViewPort::OnMouseUsageChanged(int mouseUsage)
{
  m_chartViewer->setMouseUsage(mouseUsage);
}

// The Save button is pressed
void
RealTimeViewPort::OnSave(wxCommandEvent& event)
{
  wxFileDialog saveFileDialog(this, _("Save graphics file"), "", "chartdirector_demo",
      "PNG (*.png)|*.png|JPG (*.jpg)|*.jpg|GIF (*.gif)|*.gif|BMP (*.bmp)|*.bmp|SVG (*.svg)|*.svg|PDF (*.pdf)|*.pdf", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
  if (saveFileDialog.ShowModal() == wxID_CANCEL)
    return;     // the user changed idea...

  // save the current contents in the file;
  wxString fileName = saveFileDialog.GetPath();
  if (!fileName.IsEmpty())
  {
    BaseChart* c = m_chartViewer->getChart();
    if (0 != c)
    {
      c->makeChart(fileName.ToUTF8());
    }
  }
}

// Draw track cursor when mouse is moving over plotarea
void RealTimeViewPort::OnMouseMovePlotArea(wxCommandEvent& event)
{
  double trackLinePos = TrackLineLabel((XYChart *) m_chartViewer->getChart(), m_chartViewer->getPlotAreaMouseX());
  trackLineIsAtEnd = (m_currentIndex <= 0) || (trackLinePos == trackLineEndPos);

  m_chartViewer->updateDisplay();
}

// Draw the track line with data point labels
double
RealTimeViewPort::TrackLineLabel(XYChart *c, int mouseX)
{
  // Clear the current dynamic layer and get the DrawArea object to draw on it.
  DrawArea* d = c->initDynamicLayer();

  // The plot area object
  PlotArea* plotArea = c->getPlotArea();

  // Get the data x-value that is nearest to the mouse, and find its pixel coordinate.
  double xValue = c->getNearestXValue(mouseX);
  int xCoor = c->getXCoor(xValue);
  if (xCoor < plotArea->getLeftX())
    return xValue;

  // Draw a vertical track line at the x-position
  d->vline(plotArea->getTopY(), plotArea->getBottomY(), xCoor, 0x888888);

  // Draw a label on the x-axis to show the track line position.
  ostringstream xlabel;
  xlabel << "<*font,bgColor=000000*> " << c->xAxis()->getFormattedLabel(xValue + 0.00499,
    "hh:nn:ss.ff") << " <*/font*>";
  TTFText *t = d->text(xlabel.str().c_str(), "arialbd.ttf", 10);

  // Restrict the x-pixel position of the label to make sure it stays inside the chart image.
  int xLabelPos = max(0, min(xCoor - t->getWidth() / 2, c->getWidth() - t->getWidth()));
  t->draw(xLabelPos, plotArea->getBottomY() + 2, 0xffffff);
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
      if ((yCoor >= plotArea->getTopY()) && (yCoor <= plotArea->getBottomY()) && (color !=
        Chart::Transparent) && dataSetName && *dataSetName)
      {
        d->circle(xCoor, yCoor, 4, 4, color, color);

        ostringstream label;
        label << "<*font,bgColor=" << hex << color << "*> "
          << c->formatValue(dataSet->getValue(xIndex), "{value|P4}") << " <*font*>";
        t = d->text(label.str().c_str(), "arialbd.ttf", 10);

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

  return xValue;
}

/*
 * Should we show tooltips?
 */

bool
RealTimeViewPort::ShowToolTips()
{
  return true;
}

/*
 * Get bitmap resources
 */

wxBitmap
RealTimeViewPort::GetBitmapResource( const wxString& name )
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
RealTimeViewPort::GetIconResource( const wxString& name )
{
  // Icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
}

/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_POINTER
 */

void
RealTimeViewPort::OnPointerClick( wxCommandEvent& event )
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
RealTimeViewPort::OnZoomInClick( wxCommandEvent& event )
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
RealTimeViewPort::OnZoomOutClick( wxCommandEvent& event )
{
  m_pointerButton->SetValue(false);
  m_zoominButton->SetValue(false);
  m_zoomoutButton->SetValue(true);
  OnMouseUsageChanged(Chart::MouseUsageZoomOut);
}

void
RealTimeViewPort::OnChartUpdateTimer(wxTimerEvent& event)
{
  wxChartViewer* viewer = m_chartViewer;

  // Enables auto scroll if the viewport is showing the latest data before the update
  bool autoScroll = (m_currentIndex > 0) && (0.001 + viewer->getValueAtViewPort("x",
    viewer->getViewPortLeft() + viewer->getViewPortWidth()) >= m_timeStamps[m_currentIndex - 1]);

  //
  // Get new data from the queue and append them to the data arrays
  //
  int count;
  DataPacket *packets;
  if ((count = m_buffer.Get(&packets)) <= 0)
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

    // Remove oldest data to leave space for new data. To avoid frequent removal, we ensure at
    // least 5% empty space available after removal.
    int originalIndex = m_currentIndex;
    m_currentIndex = sampleSize * 95 / 100 - 1;
    if (m_currentIndex > sampleSize - count)
    {
      m_currentIndex = sampleSize - count;
    }

    for (int i = 0; i < m_currentIndex; ++i)
    {
      int srcIndex = i + originalIndex - m_currentIndex;
      m_timeStamps[i] = m_timeStamps[srcIndex];
      m_dataSeriesA[i] = m_dataSeriesA[srcIndex];
      m_dataSeriesB[i] = m_dataSeriesB[srcIndex];
    }
  }

  // Append the data from the queue to the data arrays
  for (int n = 0; n < count; ++n)
  {
    m_timeStamps[m_currentIndex] = packets[n].elapsedTime;
    m_dataSeriesA[m_currentIndex] = packets[n].series0;
    m_dataSeriesB[m_currentIndex] = packets[n].series1;
    ++m_currentIndex;
  }

  // As we added more data, we may need to update the full range of the viewport.

  double startDate = m_timeStamps[0];
  double endDate = m_timeStamps[m_currentIndex - 1];

  // Use the initialFullRange (which is 60 seconds in this demo) if this is sufficient.
  double duration = endDate - startDate;
  if (duration < initialFullRange)
    endDate = startDate + initialFullRange;

  // Update the new full data range to include the latest data
  bool axisScaleHasChanged = viewer->updateFullRangeH("x", startDate, endDate, Chart::KeepVisibleRange);

  if (autoScroll)
  {
    // Scroll the viewport if necessary to display the latest data
    double viewPortEndPos = viewer->getViewPortAtValue("x", m_timeStamps[m_currentIndex - 1]);
    if (viewPortEndPos > viewer->getViewPortLeft() + viewer->getViewPortWidth())
    {
      viewer->setViewPortLeft(viewPortEndPos - viewer->getViewPortWidth());
      axisScaleHasChanged = true;
    }
  }

  // Set the zoom in limit as a ratio to the full range
  viewer->setZoomInWidthLimit(zoomInLimit / (viewer->getValueAtViewPort("x", 1) - viewer->getValueAtViewPort("x", 0)));

  // Trigger the viewPortChanged event. Updates the chart if the axis scale has changed
  // (scrolling or zooming) or if new data are added to the existing axis scale.
  viewer->updateViewPort(axisScaleHasChanged || (duration < initialFullRange), false);
}

// Handles realtime data from RandomWalk. The RandomWalk will call this method from its own thread.
// This is a static method.
void
RealTimeViewPort::OnData(void* self, double elapsedTime, double series0, double series1)
{
  // Copy the data into a structure and store it in the queue.
  DataPacket packet;
  packet.elapsedTime = elapsedTime;
  packet.series0 = series0;
  packet.series1 = series1;

  ((RealTimeViewPort*) self)->m_buffer.Put(packet);
}
