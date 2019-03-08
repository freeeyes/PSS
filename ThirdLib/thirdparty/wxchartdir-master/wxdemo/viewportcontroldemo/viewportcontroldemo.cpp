/*
** Name:        viewportcontroldemo.cpp
** Purpose:     Zooming and Scrolling with Viewport Control
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

#include "viewportcontroldemo.h"

#include <sstream>
#include <vector>

using namespace std;

#include "mondrian.xpm"

/*
 * ViewportControlDemo type definition
 */

IMPLEMENT_CLASS( ViewportControlDemo, wxFrame )

/*
 * ViewportControlDemo event table definition
 */

BEGIN_EVENT_TABLE( ViewportControlDemo, wxFrame )

  EVT_TOGGLEBUTTON( ID_POINTER, ViewportControlDemo::OnPointerClick )
  EVT_TOGGLEBUTTON( wxID_ZOOM_IN, ViewportControlDemo::OnZoomInClick )
  EVT_TOGGLEBUTTON( wxID_ZOOM_OUT, ViewportControlDemo::OnZoomOutClick )
  EVT_BUTTON( wxID_SAVE, ViewportControlDemo::OnSave )
  EVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA(ID_CHARTVIEWER, ViewportControlDemo::OnMouseMovePlotArea)
  EVT_CHARTVIEWER_VIEWPORT_CHANGED(ID_CHARTVIEWER, ViewportControlDemo::OnViewPortChanged)

END_EVENT_TABLE()

/*
 * ViewportControlDemo constructors
 */

ViewportControlDemo::ViewportControlDemo()
{
  Init();
}

ViewportControlDemo::ViewportControlDemo( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create( parent, id, caption, pos, size, style );
}

/*
 * ViewportControlDemo creator
 */

bool
ViewportControlDemo::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
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
 * ViewportControlDemo destructor
 */

ViewportControlDemo::~ViewportControlDemo()
{
  delete m_ranSeries;
  if (m_viewPortControl->getChart() != NULL)
  {
    delete m_viewPortControl->getChart();
  }
  if (m_chartViewer->getChart() != NULL)
  {
    delete m_chartViewer->getChart();
  }
}

/*
 * Member initialisation
 */

void
ViewportControlDemo::Init()
{
}

/*
 * Control creation for ViewportControlDemo
 */

void
ViewportControlDemo::CreateControls()
{    
  SetIcon(wxICON(mondrian));
  ViewportControlDemo* itemFrame1 = this;

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

  m_chartViewer = new wxChartViewer(itemPanel2, ID_CHARTVIEWER, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxNO_BORDER);
  m_chartViewer->SetMinSize(wxSize(660, 400));

  m_viewPortControl = new wxViewPortControl(itemPanel2, ID_VIEWPORT, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxNO_BORDER);
  m_viewPortControl->SetMinSize(wxSize(660, 70));

  // Load the data
  LoadData();

  // Initialize the wxChartViewer
  InitChartViewer(m_chartViewer);

  // Trigger the ViewPortChanged event to draw the chart
  m_chartViewer->updateViewPort(true, true);

  // Draw and display the full chart in the CViewPortControl
  DrawFullChart(m_viewPortControl, m_chartViewer);

  // Bind the CChartViewer to the CViewPortControl
  m_viewPortControl->setViewer(m_chartViewer);

  m_chartViewer->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemBoxSizer8->Add(m_chartViewer, 0, wxGROW | wxALL, 3);

  m_viewPortControl->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemBoxSizer8->Add(m_viewPortControl, 0, wxGROW | wxALL, 3);
}

// Load the data
void
ViewportControlDemo::LoadData()
{
  // In this example, we just use random numbers as data.
  m_ranSeries = new RanSeries(127);
  m_timeStamps = m_ranSeries->getDateSeries(1827, Chart::chartTime(2010, 1, 1), 86400);
  m_dataSeriesA = m_ranSeries->getSeries(1827, 150, -10, 10);
  m_dataSeriesB = m_ranSeries->getSeries(1827, 200, -10, 10);
  m_dataSeriesC = m_ranSeries->getSeries(1827, 250, -8, 8);
}

// Initialize the wxChartViewer
void
ViewportControlDemo::InitChartViewer(wxChartViewer* viewer)
{
  // Set the full x range to be the duration of the data
  viewer->setFullRange("x", m_timeStamps[0], m_timeStamps[m_timeStamps.len - 1]);

  // Initialize the view port to show the latest 20% of the time range
  viewer->setViewPortWidth(0.2);
  viewer->setViewPortLeft(1 - viewer->getViewPortWidth());

  // Set the maximum zoom to 10 points
  viewer->setZoomInWidthLimit(10.0 / m_timeStamps.len);
  viewer->setMouseWheelZoomRatio(1.1);

  // Initially set the mouse to drag to scroll mode.
  viewer->setMouseUsage(Chart::MouseUsageScroll);
}

// The ViewPortChanged event handler. This event occurs if the user scrolls or zooms in
// or out the chart by dragging or clicking on the chart. It can also be triggered by
// calling WinChartViewer.updateViewPort.
void
ViewportControlDemo::OnViewPortChanged(wxCommandEvent& event)
{
  // Update chart if necessary
  if (m_chartViewer->needUpdateChart())
  {
    DrawChart(m_chartViewer);
  }
}

// Draw the chart and display it in the given viewer
void
ViewportControlDemo::DrawChart(wxChartViewer* viewer)
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

  // Create an XYChart object of size 640 x 400 pixels
  XYChart* c = new XYChart(640, 400);

  // Set the plotarea at (55, 55) with width 80 pixels less than chart width, and height 92 pixels
  // less than chart height. Use a vertical gradient from light blue (f0f6ff) to sky blue (a0c0ff)
  // as background. Set border to transparent and grid lines to white (ffffff).
  c->setPlotArea(55, 55, c->getWidth() - 85, c->getHeight() - 92, c->linearGradientColor(0, 55, 0,
        c->getHeight() - 35, 0xf0f6ff, 0xa0c0ff), -1, Chart::Transparent, 0xffffff, 0xffffff);

  // As the data can lie outside the plotarea in a zoomed chart, we need enable clipping.
  c->setClipping();

  // Add a title box using dark grey (0x333333) 15pt Arial Bold font
  c->addTitle("   Zooming and Scrolling with Viewport Control", "arialbd.ttf", 15, 0x333333);

	// Set legend icon style to use line style icon, sized for 10pt font
  c->getLegend()->setLineStyleKey();
  c->getLegend()->setFontSize(10);

  // Set the x and y axis stems to transparent and the label font to 10pt Arial
  c->xAxis()->setColors(Chart::Transparent);
  c->yAxis()->setColors(Chart::Transparent);
  c->xAxis()->setLabelStyle("arial.ttf", 10);
  c->yAxis()->setLabelStyle("arial.ttf", 10);

  // Add axis title using 10pt Arial Bold font
  c->yAxis()->setTitle("Ionic Temperature (C)", "arialbd.ttf", 10);

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

  // Now we add the 3 data series to a line layer, using the color red (0xff3333), green
  // (0x008800) and blue (0x3333cc)
  layer->setXData(viewPortTimeStamps);
  layer->addDataSet(viewPortDataSeriesA, 0xff3333, "Alpha");
  layer->addDataSet(viewPortDataSeriesB, 0x008800, "Beta");
  layer->addDataSet(viewPortDataSeriesC, 0x3333cc, "Gamma");

  ///////////////////////////////////////////////////////////////////////////////////////
  // Configure axis scale and labelling
  ///////////////////////////////////////////////////////////////////////////////////////

  // Set the x-axis as a date/time axis with the scale according to the view port x range.
  viewer->syncDateAxisWithViewPort("x", c->xAxis());

  // For the automatic y-axis labels, set the minimum spacing to 30 pixels.
  c->yAxis()->setTickDensity(30);

  // In this demo, the time range can be from a few years to a few days. We demonstrate how to
  // set up different date/time format based on the time range.

  // If all ticks are yearly aligned, then we use "yyyy" as the label format.
  c->xAxis()->setFormatCondition("align", 360 * 86400);
  c->xAxis()->setLabelFormat("{value|yyyy}");

  // If all ticks are monthly aligned, then we use "mmm yyyy" in bold font as the first
  // label of a year, and "mmm" for other labels.
  c->xAxis()->setFormatCondition("align", 30 * 86400);
  c->xAxis()->setMultiFormat(Chart::StartOfYearFilter(), "<*font=bold*>{value|mmm<*br*>yyyy}",
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
  c->xAxis()->setMultiFormat(Chart::StartOfDayFilter(), "<*font=bold*>{value|hh:nn<*br*>mmm dd}",
        Chart::AllPassFilter(), "{value|hh:nn}");

  ///////////////////////////////////////////////////////////////////////////////////////
  // Output the chart
  ///////////////////////////////////////////////////////////////////////////////////////
    
  // We need to update the track line too. If the mouse is moving on the chart (eg. if 
  // the user drags the mouse on the chart to scroll it), the track line will be updated
  // in the MouseMovePlotArea event. Otherwise, we need to update the track line here.
  if (!viewer->isInMouseMoveEvent()) 
  {
    TrackLineLegend(c, (0 == viewer->getChart()) ? c->getPlotArea()->getRightX() : viewer->getPlotAreaMouseX()); 
  }
  delete viewer->getChart();
  viewer->setChart(c);
}

void
ViewportControlDemo::DrawFullChart(wxViewPortControl* vpc, wxChartViewer* viewer)
{
  // Create an XYChart object of size 640 x 70 pixels
  XYChart* c = new XYChart(640, 70);

  // Set the plotarea with the same horizontal position as that in the main chart for alignment.
  c->setPlotArea(55, 5, c->getWidth() - 85, c->getHeight() - 11, 0xc0d8ff, -1, 0x888888, Chart::Transparent, 0xffffff);

  // Set the x axis stem to transparent and the label font to 10pt Arial
  c->xAxis()->setColors(Chart::Transparent);
  c->xAxis()->setLabelStyle("arial.ttf", 10);

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
  layer->setXData(m_timeStamps);
  layer->addDataSet(m_dataSeriesA, 0xff3333);
  layer->addDataSet(m_dataSeriesB, 0x008800);
  layer->addDataSet(m_dataSeriesC, 0x3333cc);

  // The x axis scales should reflect the full range of the view port
  c->xAxis()->setDateScale(viewer->getValueAtViewPort("x", 0), viewer->getValueAtViewPort("x", 1));

  // For the automatic x-axis labels, set the minimum spacing to 75 pixels.
  c->xAxis()->setTickDensity(75);

  // For the auto-scaled y-axis, as we hide the labels, we can disable axis rounding. This can
  // make the axis scale fit the data tighter.
  c->yAxis()->setRounding(false, false);

  // Output the chart
  vpc->setChart(c);
}

// The Pointer, Zoom In or Zoom out button is pressed
void ViewportControlDemo::OnMouseUsageChanged(int mouseUsage)
{
  m_chartViewer->setMouseUsage(mouseUsage);
}

// The Save button is pressed
void ViewportControlDemo::OnSave(wxCommandEvent& event)
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

// Draw track cursor when mouse is moving over plotarea
void
ViewportControlDemo::OnMouseMovePlotArea(wxCommandEvent& event)
{
  TrackLineLegend((XYChart *)m_chartViewer->getChart(), m_chartViewer->getPlotAreaMouseX());
  m_chartViewer->updateDisplay();
}

// Draw track line with data labels
void
ViewportControlDemo::TrackLineLegend(XYChart *c, int mouseX)
{
  // Clear the current dynamic layer and get the DrawArea object to draw on it.
  DrawArea* d = c->initDynamicLayer();

  // The plot area object
  PlotArea* plotArea = c->getPlotArea();

  // Get the data x-value that is nearest to the mouse, and find its pixel coordinate.
  double xValue = c->getNearestXValue(mouseX);
  int xCoor = c->getXCoor(xValue);

  // Draw a vertical track line at the x-position
  d->vline(plotArea->getTopY(), plotArea->getBottomY(), xCoor, 0xaaaaaa);

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
      if (dataName && *dataName && (color != Chart::Transparent))
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
          d->circle(xCoor, yCoor, 5, 5, color, color);
        }
      }
    }
  }

  // Create the legend by joining the legend entries
  ostringstream legendText;
  legendText << "<*block,maxWidth=" << plotArea->getWidth() << "*><*block*><*font=arialbd.ttf*>["
        << c->xAxis()->getFormattedLabel(xValue, "mm/dd/yyyy") << "]<*/*>";
  for (int i = ((int)legendEntries.size()) - 1; i >= 0; --i)
    legendText << "        " << legendEntries[i];

  // Display the legend on the top of the plot area
  TTFText *t = d->text(legendText.str().c_str(), "arialbd.ttf", 10);
  t->draw(plotArea->getLeftX() + 5, plotArea->getTopY() - 3, 0x333333, Chart::BottomLeft);
  t->destroy();
}

/*
 * Should we show tooltips?
 */

bool
ViewportControlDemo::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap
ViewportControlDemo::GetBitmapResource( const wxString& name )
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
ViewportControlDemo::GetIconResource( const wxString& name )
{
  // Icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
}

/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_POINTER
 */

void
ViewportControlDemo::OnPointerClick( wxCommandEvent& event )
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
ViewportControlDemo::OnZoomInClick( wxCommandEvent& event )
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
ViewportControlDemo::OnZoomOutClick( wxCommandEvent& event )
{
  m_pointerButton->SetValue(false);
  m_zoominButton->SetValue(false);
  m_zoomoutButton->SetValue(true);
  OnMouseUsageChanged(Chart::MouseUsageZoomOut);
}
