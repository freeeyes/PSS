/*
** Name:        realtimezoomscroll.cpp
** Purpose:     Realtime Chart with Zoom/Scroll and Track Line
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

#include "realtimezoomscroll.h"

#include <sstream>

using namespace std;

#include "mondrian.xpm"

static const int gs_dataInterval = 250;

/*
 * RealTimeZoomScroll type definition
 */

IMPLEMENT_CLASS( RealTimeZoomScroll, wxFrame )

/*
 * RealTimeZoomScroll event table definition
 */

BEGIN_EVENT_TABLE( RealTimeZoomScroll, wxFrame )

  EVT_TOGGLEBUTTON(ID_POINTER, RealTimeZoomScroll::OnPointerClick)
  EVT_TOGGLEBUTTON(wxID_ZOOM_IN, RealTimeZoomScroll::OnZoomInClick)
  EVT_TOGGLEBUTTON(wxID_ZOOM_OUT, RealTimeZoomScroll::OnZoomOutClick)
  EVT_BUTTON(wxID_SAVE, RealTimeZoomScroll::OnSave)
  EVT_COMMAND_SCROLL(ID_SCROLLBAR, RealTimeZoomScroll::OnScrollbarUpdated)
  EVT_CHOICE( ID_UPDATE_PERIOD, RealTimeZoomScroll::OnUpdatePeriodSelected )
  EVT_TIMER(ID_DATA_TIMER, RealTimeZoomScroll::OnDataTimer)
  EVT_TIMER(ID_UPDATE_TIMER, RealTimeZoomScroll::OnChartUpdateTimer)
  EVT_CHARTVIEWER_VIEWPORT_CHANGED(ID_CHARTVIEWER, RealTimeZoomScroll::OnViewPortChanged)
  EVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA(ID_CHARTVIEWER, RealTimeZoomScroll::OnMouseMovePlotArea)

END_EVENT_TABLE()

/*
 * RealTimeZoomScroll constructors
 */

RealTimeZoomScroll::RealTimeZoomScroll()
{
  Init();
}

RealTimeZoomScroll::RealTimeZoomScroll( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create( parent, id, caption, pos, size, style );
}


/*
 * RealTimeZoomScroll creator
 */

bool
RealTimeZoomScroll::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
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
 * RealTimeZoomScroll destructor
 */

RealTimeZoomScroll::~RealTimeZoomScroll()
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
RealTimeZoomScroll::Init()
{
  m_updatePeriod = NULL;
  m_alphaValue = NULL;
  m_betaValue = NULL;
  m_gammaValue = NULL;
}

/*
 * Control creation for RealTimeZoomScroll
 */

void
RealTimeZoomScroll::CreateControls()
{    
  SetIcon(wxICON(mondrian));
  RealTimeZoomScroll* itemFrame1 = this;

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

  m_pointerButton = new wxToggleButton(itemPanel2, ID_POINTER, _(" &Pointer"), wxDefaultPosition, wxDefaultSize, wxBU_LEFT);
  m_pointerButton->SetBitmap(GetBitmapResource("pointer.png"));
  m_pointerButton->SetBitmapMargins(10, 0);
  // Initially set the mouse to drag to scroll mode
  m_pointerButton->SetValue(true);
  itemStaticBoxSizer1->Add(m_pointerButton, 0, wxGROW | wxALL, 3);

  m_zoominButton = new wxToggleButton(itemPanel2, wxID_ZOOM_IN, _(" Zoom &In"), wxDefaultPosition, wxDefaultSize, wxBU_LEFT);
  m_zoominButton->SetBitmap(GetBitmapResource("zoomin.png"));
  m_zoominButton->SetBitmapMargins(10, 0);
  m_zoominButton->SetValue(false);
  itemStaticBoxSizer1->Add(m_zoominButton, 0, wxGROW | wxALL, 3);

  m_zoomoutButton = new wxToggleButton(itemPanel2, wxID_ZOOM_OUT, _(" Zoom &Out"), wxDefaultPosition, wxDefaultSize, wxBU_LEFT);
  m_zoomoutButton->SetBitmap(GetBitmapResource("zoomout.png"));
  m_zoomoutButton->SetBitmapMargins(10, 0);
  m_zoomoutButton->SetValue(false);
  itemStaticBoxSizer1->Add(m_zoomoutButton, 0, wxGROW | wxALL, 3);

  itemStaticBoxSizer1->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

  m_saveButton = new wxButton(itemPanel2, wxID_SAVE, _(" &Save"), wxDefaultPosition, wxDefaultSize, wxBU_LEFT);
  m_saveButton->SetBitmap(GetBitmapResource("save.png"));
  m_saveButton->SetBitmapMargins(10, 0);
  itemStaticBoxSizer1->Add(m_saveButton, 0, wxGROW | wxALL, 3);

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

  m_chartViewer = new wxChartViewer(itemPanel2, ID_CHARTVIEWER, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxNO_BORDER);
  m_chartViewer->SetMinSize(wxSize(650, 350));

  m_chartViewer->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemBoxSizer8->Add(m_chartViewer, 1, wxGROW | wxALL, 3);

  // Enable mouse wheel zooming by setting the zoom ratio to 1.1 per wheel event
  m_chartViewer->setMouseWheelZoomRatio(1.1);

  m_scrollBar = new wxScrollBar(itemPanel2, ID_SCROLLBAR, wxDefaultPosition, wxDefaultSize, wxSB_HORIZONTAL);
  m_scrollBar->SetScrollbar(0, 0, 1000000000, 200000000);
  itemBoxSizer8->Add(m_scrollBar, 0, wxGROW | wxALL, 3);

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

// The data acquisition routine. In this demo, this is invoked every 250ms.
void
RealTimeZoomScroll::OnDataTimer(wxTimerEvent& event)
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
    double dataA = 20 + cos(p * 129241) * 10 + 1 / (cos(p) * cos(p) + 0.01);
    double dataB = 150 + 100 * sin(p / 27.7) * sin(p / 10.1);
    double dataC = 150 + 100 * cos(p / 6.7) * cos(p / 11.9);

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
        m_dataSeriesC[i] = m_dataSeriesC[srcIndex];
      }
    }

    // Store the new values in the current index position, and increment the index.
    m_timeStamps[m_currentIndex] = currentTime;
    m_dataSeriesA[m_currentIndex] = dataA;
    m_dataSeriesB[m_currentIndex] = dataB;
    m_dataSeriesC[m_currentIndex] = dataC;
    ++m_currentIndex;

    m_nextDataTime.Add(wxTimeSpan(0, 0, 0, gs_dataInterval));
  } while (m_nextDataTime < now);

  // We provide some visual feedback to the latest numbers generated, so you can see the
  // data being generated.
  m_alphaValue->SetValue(wxString::Format("%.2f", m_dataSeriesA[m_currentIndex - 1]));
  m_betaValue->SetValue(wxString::Format("%.2f", m_dataSeriesB[m_currentIndex - 1]));
  m_gammaValue->SetValue(wxString::Format("%.2f", m_dataSeriesC[m_currentIndex - 1]));
}

// The ViewPortChanged event handler. This event occurs if the user scrolls or zooms in
// or out the chart by dragging or clicking on the chart. It can also be triggered by
// calling WinChartViewer.updateViewPort.
void
RealTimeZoomScroll::OnViewPortChanged(wxCommandEvent& event)
{
  // In addition to updating the chart, we may also need to update other controls that
  // changes based on the view port.
  UpdateControls(m_chartViewer);

  if (m_chartViewer->needUpdateChart())
  {
    DrawChart(m_chartViewer);
  }
}

// Update controls in the user interface when the view port changed
void
RealTimeZoomScroll::UpdateControls(wxChartViewer* viewer)
{
  // The logical length of the scrollbar. It can be any large value. The actual value does
  // not matter.
  const int scrollBarLen = 1000000000;

  // Update the horizontal scroll bar
  //  m_scrollBar->setPageStep((int)ceil(viewer->getViewPortWidth() * scrollBarLen));
  //  m_scrollBar->setSingleStep(min(scrollBarLen / 100, m_scrollBar->GetPageSize()));
  //  m_scrollBar->setRange(0, scrollBarLen - m_HScrollBar->pageStep());
  //  m_scrollBar->SetValue((int)(0.5 + viewer->getViewPortLeft() * scrollBarLen));
  int position = (int)(0.5 + viewer->getViewPortLeft() * scrollBarLen);
  int thumbSize = (int)ceil(viewer->getViewPortWidth() * scrollBarLen);
  int range = scrollBarLen /*- m_scrollBar->GetPageSize()*/;
  int pageSize = thumbSize;
  m_scrollBar->SetScrollbar(position, thumbSize, range, pageSize);
  m_scrollBar->Enable(viewer->getViewPortWidth() < 1);
}

// Draw chart
void
RealTimeZoomScroll::DrawChart(wxChartViewer* viewer)
{
  // Get the start date and end date that are visible on the chart.
  double viewPortStartDate = viewer->getValueAtViewPort("x", viewer->getViewPortLeft());
  double viewPortEndDate = viewer->getValueAtViewPort("x", viewer->getViewPortLeft() + viewer->getViewPortWidth());

  // Extract the part of the data arrays that are visible.
  DoubleArray viewPortTimeStamps;
  DoubleArray viewPortDataSeriesA;
  DoubleArray viewPortDataSeriesB;
  DoubleArray viewPortDataSeriesC;

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
    viewPortDataSeriesC = DoubleArray(m_dataSeriesC + startIndex, noOfPoints);
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
  c->addTitle("    Realtime Chart with Zoom/Scroll and Track Line", "arial.ttf", 18);

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
  LineLayer *layer = c->addLineLayer();
  layer->setLineWidth(2);
  layer->setFastLineMode();

  // Now we add the 3 data series to a line layer, using the color red (ff0000), green (00cc00)
  // and blue (0000ff)
  layer->setXData(viewPortTimeStamps);
  layer->addDataSet(viewPortDataSeriesA, 0xff0000, "Alpha");
  layer->addDataSet(viewPortDataSeriesB, 0x00cc00, "Beta");
  layer->addDataSet(viewPortDataSeriesC, 0x0000ff, "Gamma");

  //================================================================================
  // Configure axis scale and labelling
  //================================================================================

  // Set the x-axis as a date/time axis with the scale according to the view port x range.
  if (m_currentIndex > 0)
    c->xAxis()->setDateScale(viewPortStartDate, viewPortEndDate);

  // For the automatic axis labels, set the minimum spacing to 75/30 pixels for the x/y axis.
  c->xAxis()->setTickDensity(75);
  c->yAxis()->setTickDensity(30);

  // In this demo, the time range can be from many hours to a few seconds. We can need to define
  // the date/time format the various cases.

  // If all ticks are hour algined, we use "hh:nn<*br*>mmm dd" in bold font as the first label of
  // the Day, and "hh:nn" for other labels.
  c->xAxis()->setFormatCondition("align", 3600);
  c->xAxis()->setMultiFormat(Chart::StartOfDayFilter(), "<*font=bold*>{value|hh:nn<*br*>mmm dd}",
    Chart::AllPassFilter(), "{value|hh:nn}");

  // If all ticks are minute algined, then we use "hh:nn" as the label format.
  c->xAxis()->setFormatCondition("align", 60);
  c->xAxis()->setLabelFormat("{value|hh:nn}");

  // If all other cases, we use "hh:nn:ss" as the label format.
  c->xAxis()->setFormatCondition("else");
  c->xAxis()->setLabelFormat("{value|hh:nn:ss}");

  // We make sure the tick increment must be at least 1 second.
  c->xAxis()->setMinTickInc(1);

  //================================================================================
  // Output the chart
  //================================================================================

  // We need to update the track line too. If the mouse is moving on the chart (eg. if 
  // the user drags the mouse on the chart to scroll it), the track line will be updated
  // in the MouseMovePlotArea event. Otherwise, we need to update the track line here.
  if (!viewer->isInMouseMoveEvent())
  {
    wxLogDebug("DrawChart: update tracklinelabel");
    TrackLineLabel(c, (0 == viewer->getChart()) ? c->getPlotArea()->getRightX() : viewer->getPlotAreaMouseX());
  }

  // Set the chart image to the wxChartViewer
  delete viewer->getChart();
  viewer->setChart(c);
}

// Draw track cursor when mouse is moving over plotarea
void
RealTimeZoomScroll::OnMouseMovePlotArea(wxCommandEvent& event)
{
  wxLogDebug("OnMouseMovePlotArea: update tracklinelabel");
  TrackLineLabel((XYChart*) m_chartViewer->getChart(), m_chartViewer->getPlotAreaMouseX());
  m_chartViewer->updateDisplay();
}

// Draw the track line with data point labels
void
RealTimeZoomScroll::TrackLineLabel(XYChart *c, int mouseX)
{
  // Clear the current dynamic layer and get the DrawArea object to draw on it.
  DrawArea* d = c->initDynamicLayer();

  // The plot area object
  PlotArea* plotArea = c->getPlotArea();

  // Get the data x-value that is nearest to the mouse, and find its pixel coordinate.
  double xValue = c->getNearestXValue(mouseX);
  int xCoor = c->getXCoor(xValue);
  if (xCoor < plotArea->getLeftX())
    return;

  // Draw a vertical track line at the x-position
  d->vline(plotArea->getTopY(), plotArea->getBottomY(), xCoor, 0x888888);

  // Draw a label on the x-axis to show the track line position.
  ostringstream xlabel;
  xlabel << "<*font,bgColor=000000*> " << c->xAxis()->getFormattedLabel(xValue, "hh:nn:ss.ff")
    << " <*/font*>";
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
}

// The Pointer, Zoom In or Zoom out button is pressed
void
RealTimeZoomScroll::OnMouseUsageChanged(int mouseUsage)
{
  m_chartViewer->setMouseUsage(mouseUsage);
}

// User clicks on the the horizontal scroll bar
void
RealTimeZoomScroll::OnScrollBarChanged(int value)
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

/*
 * Should we show tooltips?
 */

bool
RealTimeZoomScroll::ShowToolTips()
{
  return true;
}

/*
 * Get bitmap resources
 */

wxBitmap
RealTimeZoomScroll::GetBitmapResource( const wxString& name )
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
RealTimeZoomScroll::GetIconResource( const wxString& name )
{
  // Icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
}

/*
* wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_POINTER
*/

void
RealTimeZoomScroll::OnPointerClick(wxCommandEvent& event)
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
RealTimeZoomScroll::OnZoomInClick(wxCommandEvent& event)
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
RealTimeZoomScroll::OnZoomOutClick(wxCommandEvent& event)
{
  m_pointerButton->SetValue(false);
  m_zoominButton->SetValue(false);
  m_zoomoutButton->SetValue(true);
  OnMouseUsageChanged(Chart::MouseUsageZoomOut);
}

// The Save button is pressed
void
RealTimeZoomScroll::OnSave(wxCommandEvent& event)
{
  wxFileDialog saveFileDialog(this, _("Save graphics file"), "", "chartdirector_demo",
    "PNG (*.png)|*.png|JPG (*.jpg)|*.jpg|GIF (*.gif)|*.gif|BMP (*.bmp)|*.bmp|SVG (*.svg)|*.svg|PDF (*.pdf)|*.pdf", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
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
RealTimeZoomScroll::OnScrollbarUpdated(wxScrollEvent& event)
{
  int value = event.GetPosition();
  bool enabled = m_scrollBar->IsEnabled();
  OnScrollBarChanged(value);
}

/*
 * wxEVT_COMMAND_CHOICE_SELECTED event handler for ID_UPDATE_PERIOD
 */

void
RealTimeZoomScroll::OnUpdatePeriodSelected( wxCommandEvent& event )
{
  OnUpdatePeriodChanged(m_updatePeriod->GetString(m_updatePeriod->GetSelection()));
}

// User changes the chart update period
void
RealTimeZoomScroll::OnUpdatePeriodChanged(const wxString& text)
{
  long interval;
  text.ToLong(&interval);
  m_chartUpdateTimer->Start(interval);
}

void
RealTimeZoomScroll::OnChartUpdateTimer(wxTimerEvent& event)
{
  if (m_currentIndex >= 0)
  {
    // As we added more data, we may need to update the full range of the viewport.
    double startDate = m_timeStamps[0];
    double endDate = m_timeStamps[m_currentIndex - 1];

    // Use the initialFullRange (which is 60 seconds in this demo) if this is sufficient.
    double duration = endDate - startDate;
    if (duration < initialFullRange)
      endDate = startDate + initialFullRange;

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
