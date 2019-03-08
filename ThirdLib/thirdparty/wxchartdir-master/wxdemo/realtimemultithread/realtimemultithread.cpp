/*
** Name:        realtimemultithread.h
** Purpose:     Multithreading Real-Time Chart with Zoom/Scroll and Track Line
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

#include "realtimemultithread.h"

#include <sstream>

using namespace std;

#include "mondrian.xpm"

/*
 * RealTimeMultiThread type definition
 */

IMPLEMENT_CLASS( RealTimeMultiThread, wxFrame )

/*
 * RealTimeMultiThread event table definition
 */

BEGIN_EVENT_TABLE( RealTimeMultiThread, wxFrame )

  EVT_TOGGLEBUTTON( ID_POINTER, RealTimeMultiThread::OnPointerClick )
  EVT_TOGGLEBUTTON( wxID_ZOOM_IN, RealTimeMultiThread::OnZoomInClick )
  EVT_TOGGLEBUTTON( wxID_ZOOM_OUT, RealTimeMultiThread::OnZoomOutClick )
  EVT_COMMAND_SCROLL(ID_SCROLLBAR, RealTimeMultiThread::OnScrollbarUpdated)
  EVT_BUTTON(wxID_SAVE, RealTimeMultiThread::OnSave)
  EVT_TIMER(ID_UPDATE_TIMER, RealTimeMultiThread::OnChartUpdateTimer)
  EVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA(ID_CHARTVIEWER, RealTimeMultiThread::OnMouseMovePlotArea)
  EVT_CHARTVIEWER_VIEWPORT_CHANGED(ID_CHARTVIEWER, RealTimeMultiThread::OnViewPortChanged)

END_EVENT_TABLE()

/*
 * RealTimeMultiThread constructors
 */

RealTimeMultiThread::RealTimeMultiThread()
{
  Init();
}

RealTimeMultiThread::RealTimeMultiThread( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create( parent, id, caption, pos, size, style );
}

/*
 * RealTimeMultiThread creator
 */

bool RealTimeMultiThread::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
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
 * RealTimeMultiThread destructor
 */

RealTimeMultiThread::~RealTimeMultiThread()
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
RealTimeMultiThread::Init()
{
}

/*
 * Control creation for RealTimeMultiThread
 */

void
RealTimeMultiThread::CreateControls()
{    
  SetIcon(wxICON(mondrian));
  RealTimeMultiThread* itemFrame1 = this;

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
  itemBoxSizer3->Add(itemBoxSizer8, 1, wxGROW|wxALL, 3);

  m_chartViewer = new wxChartViewer(itemPanel2, ID_CHARTVIEWER, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxNO_BORDER);
  m_chartViewer->SetMinSize(wxSize(650, 350));

  m_chartViewer->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemBoxSizer8->Add(m_chartViewer, 1, wxGROW | wxALL, 3);

  m_scrollBar = new wxScrollBar(itemPanel2, ID_SCROLLBAR, wxDefaultPosition, wxDefaultSize, wxSB_HORIZONTAL);
  m_scrollBar->SetScrollbar(0, 0, 1000000000, 200000000);
  itemBoxSizer8->Add(m_scrollBar, 0, wxGROW | wxALL, 3);

  // Initialize member variables
  m_currentIndex = 0;

  // Initially, auto-move the track line to make it follow the data series
  m_trackLineEndPos = 0;
  m_trackLineIsAtEnd = true;

  // Enable mouse wheel zooming by setting the zoom ratio to 1.1 per wheel event
  m_chartViewer->setMouseWheelZoomRatio(1.1);

  // Start the random data generator
  m_dataSource = new RandomWalk(OnData, this);
  m_dataSource->Create();
  m_dataSource->Run();

  // Set up the chart update timer
  m_chartUpdateTimer = new wxTimer(this, ID_UPDATE_TIMER);
  m_chartUpdateTimer->Start(100);
}

// The ViewPortChanged event handler. This event occurs if the user scrolls or zooms in
// or out the chart by dragging or clicking on the chart. It can also be triggered by
// calling WinChartViewer.updateViewPort.
void
RealTimeMultiThread::OnViewPortChanged(wxCommandEvent& event)
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
RealTimeMultiThread::UpdateControls(wxChartViewer* viewer)
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
RealTimeMultiThread::DrawChart(wxChartViewer* viewer)
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

    // Keep track of the latest available data at chart plotting time
    m_trackLineEndPos = m_timeStamps[m_currentIndex - 1];
  }

  // At this stage, we have extracted the visible data. We can use those data to plot the chart.

  //================================================================================
  // Configure overall chart appearance.
  //================================================================================

  // Create an XYChart object of size 640 x 350 pixels
  XYChart* c = new XYChart(640, 350);

  // Set the plotarea at (55, 50) with width 80 pixels less than chart width, and height 80 pixels
  // less than chart height. Use a vertical gradient from light blue (f0f6ff) to sky blue (a0c0ff)
  // as background. Set border to transparent and grid lines to white (ffffff).
  c->setPlotArea(55, 50, c->getWidth() - 85, c->getHeight() - 80, c->linearGradientColor(0, 50, 0,
    c->getHeight() - 35, 0xf0f6ff, 0xa0c0ff), -1, Chart::Transparent, 0xffffff, 0xffffff);

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
  c->yAxis()->setLabelStyle("arial.ttf", 10);

  // Set the y-axis tick length to 0 to disable the tick and put the labels closer to the axis.
  c->yAxis()->setTickLength(0);

  // Add axis title using 12pt Arial Bold Italic font
  c->yAxis()->setTitle("Ionic Temperature (C)", "arialbd.ttf", 12);

  //================================================================================
  // Add data to chart
  //================================================================================

  // In this example, we represent the data by lines. You may modify the code below to use other
  // representations (areas, scatter plot, etc).

  // Add a line layer for the lines, using a line width of 2 pixels
  LineLayer* layer = c->addLineLayer();
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

  //================================================================================
  // Output the chart
  //================================================================================

  // We need to update the track line too. If the mouse is moving on the chart (eg. if
  // the user drags the mouse on the chart to scroll it), the track line will be updated
  // in the MouseMovePlotArea event. Otherwise, we need to update the track line here.
  if (!viewer->isInMouseMoveEvent())
  {
    TrackLineLabel(c, m_trackLineIsAtEnd ? c->getWidth() : viewer->getPlotAreaMouseX());
  }

  // Set the chart image to the wxChartViewer
  delete viewer->getChart();
  viewer->setChart(c);
}

// The Pointer, Zoom In or Zoom out button is pressed
void
RealTimeMultiThread::OnMouseUsageChanged(int mouseUsage)
{
  m_chartViewer->setMouseUsage(mouseUsage);
}

// User clicks on the the horizontal scroll bar
void
RealTimeMultiThread::OnScrollBarChanged(int value)
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
RealTimeMultiThread::OnMouseMovePlotArea(wxCommandEvent& event)
{
  double trackLinePos = TrackLineLabel((XYChart*) m_chartViewer->getChart(), m_chartViewer->getPlotAreaMouseX());
  m_trackLineIsAtEnd = (m_currentIndex <= 0) || (trackLinePos == m_trackLineEndPos);
  m_chartViewer->updateDisplay();
}

// Draw the track line with data point labels
double
RealTimeMultiThread::TrackLineLabel(XYChart *c, int mouseX)
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
  xlabel << "<*font,bgColor=000000*> " << c->xAxis()->getFormattedLabel(xValue + 0.00499, "hh:nn:ss.ff") << " <*/font*>";
  TTFText *t = d->text(xlabel.str().c_str(), "arialbd.ttf", 10);

  // Restrict the x-pixel position of the label to make sure it stays inside the chart image.
  int xLabelPos = max(0, min(xCoor - t->getWidth() / 2, c->getWidth() - t->getWidth()));
  t->draw(xLabelPos, plotArea->getBottomY() + 6, 0xffffff);
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
          t->draw(xCoor + 6, yCoor, 0xffffff, Chart::Left);
        else
          t->draw(xCoor - 6, yCoor, 0xffffff, Chart::Right);

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
RealTimeMultiThread::ShowToolTips()
{
  return true;
}

/*
 * Get bitmap resources
 */

wxBitmap
RealTimeMultiThread::GetBitmapResource( const wxString& name )
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
RealTimeMultiThread::GetIconResource( const wxString& name )
{
  // Icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_POINTER
 */

void
RealTimeMultiThread::OnPointerClick( wxCommandEvent& event )
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
RealTimeMultiThread::OnZoomInClick( wxCommandEvent& event )
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
RealTimeMultiThread::OnZoomOutClick( wxCommandEvent& event )
{
  m_pointerButton->SetValue(false);
  m_zoominButton->SetValue(false);
  m_zoomoutButton->SetValue(true);
  OnMouseUsageChanged(Chart::MouseUsageZoomOut);
}

// The Save button is pressed
void RealTimeMultiThread::OnSave(wxCommandEvent& event)
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
RealTimeMultiThread::OnScrollbarUpdated(wxScrollEvent& event)
{
  int value = event.GetPosition();
  bool enabled = m_scrollBar->IsEnabled();
  OnScrollBarChanged(value);
}

void
RealTimeMultiThread::OnChartUpdateTimer(wxTimerEvent& event)
{
  wxChartViewer* viewer = m_chartViewer;

  // Enables auto scroll if the viewport is showing the latest data before the update
  bool autoScroll = (m_currentIndex > 0) && (0.001 + viewer->getValueAtViewPort("x",
    viewer->getViewPortLeft() + viewer->getViewPortWidth()) >= m_timeStamps[m_currentIndex - 1]);

  // Get new data from the queue and append them to the data arrays
  int count;
  DataPacket* packets;
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

    // Remove data older than the time range to leave space for new data. The data removed must
    // be at least equal to the packet count.
    int originalIndex = m_currentIndex;
    m_currentIndex = sampleSize * 95 / 100 - 1;

    if (m_currentIndex > sampleSize - count)
      m_currentIndex = sampleSize - count;

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
RealTimeMultiThread::OnData(void* self, double elapsedTime, double series0, double series1)
{
  // Copy the data into a structure and store it in the queue.
  DataPacket packet;
  packet.elapsedTime = elapsedTime;
  packet.series0 = series0;
  packet.series1 = series1;

  ((RealTimeMultiThread*) self)->m_buffer.Put(packet);
}
