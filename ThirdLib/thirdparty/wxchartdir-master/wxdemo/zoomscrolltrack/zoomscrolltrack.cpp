/*
** Name:        zoomscrolltrack.cpp
** Purpose:     Zooming and Scrolling with Track Line (1)
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

#include "zoomscrolltrack.h"

#include <sstream>

using namespace std;

#include "mondrian.xpm"

/*
 * ZoomScrollTrack type definition
 */

IMPLEMENT_CLASS( ZoomScrollTrack, wxFrame )

/*
 * ZoomScrollTrack event table definition
 */

BEGIN_EVENT_TABLE( ZoomScrollTrack, wxFrame )

  EVT_TOGGLEBUTTON( ID_POINTER, ZoomScrollTrack::OnPointerClick )
  EVT_TOGGLEBUTTON( wxID_ZOOM_IN, ZoomScrollTrack::OnZoomInClick )
  EVT_TOGGLEBUTTON( wxID_ZOOM_OUT, ZoomScrollTrack::OnZoomOutClick )
  EVT_COMMAND_SCROLL(ID_SCROLLBAR, ZoomScrollTrack::OnScrollbarUpdated)
  EVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA(ID_CHARTVIEWER, ZoomScrollTrack::OnMouseMovePlotArea)
  EVT_CHARTVIEWER_VIEWPORT_CHANGED(ID_CHARTVIEWER, ZoomScrollTrack::OnViewPortChanged)
  EVT_CHARTVIEWER_MOUSEWHEEL(ID_CHARTVIEWER, ZoomScrollTrack::OnMouseWheelChart)

END_EVENT_TABLE()

/*
 * ZoomScrollTrack constructors
 */

ZoomScrollTrack::ZoomScrollTrack()
{
  Init();
}

ZoomScrollTrack::ZoomScrollTrack( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create( parent, id, caption, pos, size, style );
}

/*
 * ZoomScrollTrack creator
 */

bool
ZoomScrollTrack::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
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
 * ZoomScrollTrack destructor
 */

ZoomScrollTrack::~ZoomScrollTrack()
{
  delete m_ranSeries;
  if (m_chartViewer->getChart() != NULL)
  {
    delete m_chartViewer->getChart();
  }
}

/*
 * Member initialisation
 */

void
ZoomScrollTrack::Init()
{
}

/*
 * Control creation for ZoomScrollTrack
 */

void
ZoomScrollTrack::CreateControls()
{    
  SetIcon(wxICON(mondrian));
  ZoomScrollTrack* itemFrame1 = this;

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

  wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer3->Add(itemBoxSizer8, 1, wxGROW|wxALL, 3);

  m_chartViewer = new wxChartViewer(itemPanel2, ID_CHARTVIEWER, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxNO_BORDER);
  m_chartViewer->SetMinSize(wxSize(650, 350));

  m_chartViewer->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemBoxSizer8->Add(m_chartViewer, 1, wxGROW | wxALL, 3);

  m_scrollBar = new wxScrollBar(itemPanel2, ID_SCROLLBAR, wxDefaultPosition, wxDefaultSize, wxSB_HORIZONTAL);
  m_scrollBar->SetScrollbar(0, 0, 1000000000, 200000000);
  itemBoxSizer8->Add(m_scrollBar, 0, wxGROW | wxALL, 3);

  // Load the data
  LoadData();

  // Initialize the wxChartViewer
  InitChartViewer(m_chartViewer);

  // Trigger the ViewPortChanged event to draw the chart
  m_chartViewer->updateViewPort(true, true);
}

// Load the data
void
ZoomScrollTrack::LoadData()
{
  // In this example, we just use random numbers as data.
  m_ranSeries = new RanSeries(127);
  m_timeStamps = m_ranSeries->getDateSeries(1827, Chart::chartTime(2007, 1, 1), 86400);
  m_dataSeriesA = m_ranSeries->getSeries(1827, 150, -10, 10);
  m_dataSeriesB = m_ranSeries->getSeries(1827, 200, -10, 10);
  m_dataSeriesC = m_ranSeries->getSeries(1827, 250, -8, 8);
}

// Initialize the wxChartViewer
void
ZoomScrollTrack::InitChartViewer(wxChartViewer* viewer)
{
  // Set the full x range to be the duration of the data
  viewer->setFullRange("x", m_timeStamps[0], m_timeStamps[m_timeStamps.len - 1]);

  // Initialize the view port to show the latest 20% of the time range
  viewer->setViewPortWidth(0.2);
  viewer->setViewPortLeft(1 - viewer->getViewPortWidth());

  // Set the maximum zoom to 10 points
  viewer->setZoomInWidthLimit(10.0 / m_timeStamps.len);

  viewer->setMouseUsage(Chart::MouseUsageScroll);
}

// The ViewPortChanged event handler. This event occurs if the user scrolls or zooms in
// or out the chart by dragging or clicking on the chart. It can also be triggered by
// calling WinChartViewer.updateViewPort.
void ZoomScrollTrack::OnViewPortChanged(wxCommandEvent& event)
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
ZoomScrollTrack::UpdateControls(wxChartViewer* viewer)
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
ZoomScrollTrack::DrawChart(wxChartViewer* viewer)
{
  // Get the start date and end date that are visible on the chart.
  double viewPortStartDate = viewer->getValueAtViewPort("x", viewer->getViewPortLeft());
  double viewPortEndDate = viewer->getValueAtViewPort("x", viewer->getViewPortLeft() + viewer->getViewPortWidth());

  // Get the array indexes that corresponds to the visible start and end dates
  int startIndex = (int)floor(Chart::bSearch(m_timeStamps, viewPortStartDate));
  int endIndex = (int)ceil(Chart::bSearch(m_timeStamps, viewPortEndDate));
  int noOfPoints = endIndex - startIndex + 1;

  // Extract the part of the data array that are visible.
  DoubleArray viewPortTimeStamps = DoubleArray(m_timeStamps.data + startIndex, noOfPoints);
  DoubleArray viewPortDataSeriesA = DoubleArray(m_dataSeriesA.data + startIndex, noOfPoints);
  DoubleArray viewPortDataSeriesB = DoubleArray(m_dataSeriesB.data + startIndex, noOfPoints);
  DoubleArray viewPortDataSeriesC = DoubleArray(m_dataSeriesC.data + startIndex, noOfPoints);

  // At this stage, we have extracted the visible data. We can use those data to plot the chart.

  ///////////////////////////////////////////////////////////////////////////////////////
  // Configure overall chart appearance.
  ///////////////////////////////////////////////////////////////////////////////////////

  // Create an XYChart object of size 650 x 350 pixels, with a white (ffffff) background and grey
  // (aaaaaa) border
  XYChart* c = new XYChart(650, 350, 0xffffff, 0xaaaaaa);

  // Set the plotarea at (55, 55) with width 90 pixels less than chart width, and height 90 pixels
  // less than chart height. Use a vertical gradient from light blue (f0f6ff) to sky blue (a0c0ff)
  // as background. Set border to transparent and grid lines to white (ffffff).
  c->setPlotArea(55, 55, c->getWidth() - 105, c->getHeight() - 90, c->linearGradientColor(0, 55, 0,
    c->getHeight() - 35, 0xf0f6ff, 0xa0c0ff), -1, Chart::Transparent, 0xffffff, 0xffffff);

  // As the data can lie outside the plotarea in a zoomed chart, we need enable clipping.
  c->setClipping();

  // Add a title to the chart using 18 pts Times New Roman Bold Italic font
  c->addTitle("   Zooming and Scrolling with Track Line (1)", "timesbi.ttf", 18);

  // Set legend icon style to use line style icon, sized for 8pt font
  c->getLegend()->setLineStyleKey();
  c->getLegend()->setFontSize(8);

  // Set the axis stem to transparent
  c->xAxis()->setColors(Chart::Transparent);
  c->yAxis()->setColors(Chart::Transparent);

  // Add axis title using 10pts Arial Bold Italic font
  c->yAxis()->setTitle("Ionic Temperature (C)", "arialbi.ttf", 10);

  ///////////////////////////////////////////////////////////////////////////////////////
  // Add data to chart
  ///////////////////////////////////////////////////////////////////////////////////////

  // In this example, we represent the data by lines. You may modify the code below to use other
  // representations (areas, scatter plot, etc).

  // Add a line layer for the lines, using a line width of 2 pixels
  LineLayer *layer = c->addLineLayer();
  layer->setLineWidth(2);

  // In this demo, we do not have too many data points. In real code, the chart may contain a lot
  // of data points when fully zoomed out - much more than the number of horizontal pixels in this
  // plot area. So it is a good idea to use fast line mode.
  layer->setFastLineMode();

  // Now we add the 3 data series to a line layer, using the color red (ff0000), green
  // (00cc00) and blue (0000ff)
  layer->setXData(viewPortTimeStamps);
  layer->addDataSet(viewPortDataSeriesA, 0xff3333, "Alpha");
  layer->addDataSet(viewPortDataSeriesB, 0x008800, "Beta");
  layer->addDataSet(viewPortDataSeriesC, 0x3333CC, "Gamma");

  ///////////////////////////////////////////////////////////////////////////////////////
  // Configure axis scale and labelling
  ///////////////////////////////////////////////////////////////////////////////////////

  // Set the x-axis as a date/time axis with the scale according to the view port x range.
  viewer->syncDateAxisWithViewPort("x", c->xAxis());

  // In this demo, the time range can be from a few years to a few days. We demonstrate how to set
  // up different date/time format based on the time range.

  // If all ticks are yearly aligned, then we use "yyyy" as the label format.
  c->xAxis()->setFormatCondition("align", 360 * 86400);
  c->xAxis()->setLabelFormat("{value|yyyy}");

  // If all ticks are monthly aligned, then we use "mmm yyyy" in bold font as the first
  // label of a year, and "mmm" for other labels.
  c->xAxis()->setFormatCondition("align", 30 * 86400);
  c->xAxis()->setMultiFormat(Chart::StartOfYearFilter(), "<*font=bold*>{value|mmm yyyy}",
    Chart::AllPassFilter(), "{value|mmm}");

  // If all ticks are daily algined, then we use "mmm dd<*br*>yyyy" in bold font as the
  // first label of a year, and "mmm dd" in bold font as the first label of a month, and
  // "dd" for other labels.
  c->xAxis()->setFormatCondition("align", 86400);
  c->xAxis()->setMultiFormat(Chart::StartOfYearFilter(),
    "<*block,halign=left*><*font=bold*>{value|mmm dd<*br*>yyyy}",
    Chart::StartOfMonthFilter(), "<*font=bold*>{value|mmm dd}");
  c->xAxis()->setMultiFormat(Chart::AllPassFilter(), "{value|dd}");

  // For all other cases (sub-daily ticks), use "hh:nn<*br*>mmm dd" for the first label of
  // a day, and "hh:nn" for other labels.
  c->xAxis()->setFormatCondition("else");
  c->xAxis()->setMultiFormat(Chart::StartOfDayFilter(),
    "<*font=bold*>{value|hh:nn<*br*>mmm dd}", Chart::AllPassFilter(), "{value|hh:nn}");

  ///////////////////////////////////////////////////////////////////////////////////////
  // Output the chart
  ///////////////////////////////////////////////////////////////////////////////////////

  // We need to update the track line too. If the mouse is moving on the chart (eg. if 
  // the user drags the mouse on the chart to scroll it), the track line will be updated
  // in the MouseMovePlotArea event. Otherwise, we need to update the track line here.
  if (!viewer->isInMouseMoveEvent())
  {
    TrackLineLegend(c, (0 == viewer->getChart()) ? c->getPlotArea()->getRightX()
                                                 : viewer->getPlotAreaMouseX());
  }

  ///////////////////////////////////////////////////////////////////////////////////////
  // Output the chart
  ///////////////////////////////////////////////////////////////////////////////////////

  delete viewer->getChart();
  viewer->setChart(c);
}

// The Pointer, Zoom In or Zoom out button is pressed
void
ZoomScrollTrack::OnMouseUsageChanged(int mouseUsage)
{
  m_chartViewer->setMouseUsage(mouseUsage);
}

// User clicks on the the horizontal scroll bar
void
ZoomScrollTrack::OnScrollBarChanged(int value)
{
  if (!m_chartViewer->isInViewPortChangedEvent())
  {
    // Set the view port based on the scroll bar
    int scrollBarLen = m_scrollBar->GetRange();
    m_chartViewer->setViewPortLeft(value / (double)scrollBarLen);

    // Update the chart display without updating the image maps. (We can delay updating
    // the image map until scrolling is completed and the chart display is stable.)
    m_chartViewer->updateViewPort(true, false);
  }
}

// When the mouse enters the chart, we will generate an image map for hot spots and tooltips
// support if it has not already been generated.
void
ZoomScrollTrack::OnMouseWheelChart(wxCommandEvent& event)
{
  // Process the mouse wheel only if the mouse is over the plot area
  if (!m_chartViewer->isMouseOnPlotArea())
  {
    event.Skip();
    return;
  }

  // We zoom in or out by 10% depending on the mouse wheel direction.
  double newVpWidth = m_chartViewer->getViewPortWidth() * (event.GetInt() > 0 ? 0.9 : 1 / 0.9);
  double newVpHeight = m_chartViewer->getViewPortHeight() * (event.GetInt() > 0 ? 0.9 : 1 / 0.9);

  // We do not zoom beyond the zoom width or height limits.
  if (newVpWidth > m_chartViewer->getZoomOutWidthLimit())
    newVpWidth = m_chartViewer->getZoomOutWidthLimit();
  if (newVpWidth < m_chartViewer->getZoomInWidthLimit())
    newVpWidth = m_chartViewer->getZoomInWidthLimit();

  if (newVpHeight, m_chartViewer->getZoomOutHeightLimit())
    newVpHeight, m_chartViewer->getZoomOutHeightLimit();
  if (newVpHeight < m_chartViewer->getZoomInHeightLimit())
    newVpHeight = m_chartViewer->getZoomInHeightLimit();

  if ((newVpWidth != m_chartViewer->getViewPortWidth()) ||
      (newVpHeight != m_chartViewer->getViewPortHeight()))
  {
    // Set the view port position and size so that the point under the mouse remains under
    // the mouse after zooming.

    double deltaX = (m_chartViewer->getPlotAreaMouseX() - m_chartViewer->getPlotAreaLeft()) *
      (m_chartViewer->getViewPortWidth() - newVpWidth) / m_chartViewer->getPlotAreaWidth();
    m_chartViewer->setViewPortLeft(m_chartViewer->getViewPortLeft() + deltaX);
    m_chartViewer->setViewPortWidth(newVpWidth);

    double deltaY = (m_chartViewer->getPlotAreaMouseY() - m_chartViewer->getPlotAreaTop()) *
      (m_chartViewer->getViewPortHeight() - newVpHeight) / m_chartViewer->getPlotAreaHeight();
    m_chartViewer->setViewPortTop(m_chartViewer->getViewPortTop() + deltaY);
    m_chartViewer->setViewPortHeight(newVpHeight);

    m_chartViewer->updateViewPort(true, false);
  }
}

// Draw track cursor when mouse is moving over plotarea
void
ZoomScrollTrack::OnMouseMovePlotArea(wxCommandEvent& event)
{
  TrackLineLegend((XYChart*) m_chartViewer->getChart(), m_chartViewer->getPlotAreaMouseX());
  m_chartViewer->updateDisplay();
}

// Draw track line with data labels
void
ZoomScrollTrack::TrackLineLegend(XYChart* c, int mouseX)
{
  // Clear the current dynamic layer and get the DrawArea object to draw on it.
  DrawArea* d = c->initDynamicLayer();

  // The plot area object
  PlotArea* plotArea = c->getPlotArea();

  // Get the data x-value that is nearest to the mouse, and find its pixel coordinate.
  double xValue = c->getNearestXValue(mouseX);
  int xCoor = c->getXCoor(xValue);

  // Draw a vertical track line at the x-position
  d->vline(plotArea->getTopY(), plotArea->getBottomY(), xCoor, d->dashLineColor(0x000000, 0x0101));

  // Container to hold the legend entries
  vector<string> legendEntries;

  // Iterate through all layers to build the legend array
  for (int i = 0; i < c->getLayerCount(); ++i)
  {
    Layer *layer = c->getLayerByZ(i);

    // The data array index of the x-value
    int xIndex = layer->getXIndexOf(xValue);

    // Iterate through all the data sets in the layer
    for (int j = 0; j < layer->getDataSetCount(); ++j) {
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
             << c->xAxis()->getFormattedLabel(xValue, "mmm dd, yyyy") << "]<*/*>";
  for (int i = ((int)legendEntries.size()) - 1; i >= 0; --i)
    legendText << "        " << legendEntries[i];

  // Display the legend on the top of the plot area
  TTFText *t = d->text(legendText.str().c_str(), "arial.ttf", 8);
  t->draw(plotArea->getLeftX() + 5, plotArea->getTopY() - 3, 0x000000, Chart::BottomLeft);
  t->destroy();
}

/*
 * Should we show tooltips?
 */

bool
ZoomScrollTrack::ShowToolTips()
{
  return true;
}

/*
 * Get bitmap resources
 */

wxBitmap
ZoomScrollTrack::GetBitmapResource( const wxString& name )
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
  return wxNullBitmap;
}

/*
 * Get icon resources
 */

wxIcon
ZoomScrollTrack::GetIconResource( const wxString& name )
{
  // Icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
}

/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_POINTER
 */

void
ZoomScrollTrack::OnPointerClick( wxCommandEvent& event )
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
ZoomScrollTrack::OnZoomInClick( wxCommandEvent& event )
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
ZoomScrollTrack::OnZoomOutClick( wxCommandEvent& event )
{
  m_pointerButton->SetValue(false);
  m_zoominButton->SetValue(false);
  m_zoomoutButton->SetValue(true);
  OnMouseUsageChanged(Chart::MouseUsageZoomOut);
}

/*
* wxEVT_COMMAND_SCROLLBAR_UPDATED event handler for ID_SCROLLBAR
*/

void
ZoomScrollTrack::OnScrollbarUpdated(wxScrollEvent& event)
{
  int value = event.GetPosition();
  bool enabled = m_scrollBar->IsEnabled();
  OnScrollBarChanged(value);
}
