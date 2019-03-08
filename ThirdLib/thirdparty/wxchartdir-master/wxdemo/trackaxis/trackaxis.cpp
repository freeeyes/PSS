/*
** Name:        trackaxis.cpp
** Purpose:     Track Axis sample application
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

#include "trackaxis.h"
#include "wxchartviewer.h"

#include <sstream>

using namespace std;

#include "mondrian.xpm"

/*
 * TrackAxis type definition
 */

IMPLEMENT_DYNAMIC_CLASS( TrackAxis, wxFrame )

/*
 * TrackAxis event table definition
 */

BEGIN_EVENT_TABLE( TrackAxis, wxFrame )

  EVT_MENU(wxID_EXIT, TrackAxis::OnExitClick)
  EVT_CLOSE(TrackAxis::OnCloseWindow)
  EVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA(ID_CHARTVIEWER, TrackAxis::OnMouseMovePlotArea)

END_EVENT_TABLE()

/*
 * TrackAxis constructors
 */

TrackAxis::TrackAxis()
{
  Init();
}

TrackAxis::TrackAxis( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create(parent, id, caption, pos, size, style);
}


/*
 * TrackAxis creator
 */

bool
TrackAxis::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
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
 * TrackAxis destructor
 */

TrackAxis::~TrackAxis()
{
  if (m_chartViewer != NULL)
  {
    delete m_chartViewer->getChart();
  }
}

/*
 * Member initialisation
 */

void
TrackAxis::Init()
{
}

/*
 * Control creation for TrackAxis
 */

void
TrackAxis::CreateControls()
{    
  SetIcon(wxICON(mondrian)); 
  TrackAxis* itemDialog1 = this;

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  itemDialog1->SetSizer(itemBoxSizer2);

  m_chartViewer = new wxChartViewer( itemDialog1, ID_CHARTVIEWER);
  // Output the chart
  DrawChart(m_chartViewer);

  m_chartViewer->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemBoxSizer2->Add(m_chartViewer, 1, wxGROW|wxALL, 3);
}

// Draw the chart and display it in the given viewer
void
TrackAxis::DrawChart(wxChartViewer* viewer)
{
  // Data for the chart as 2 random data series
  RanSeries r(127);
  DoubleArray data0 = r.getSeries(180, 10, -1.5, 1.5);
  DoubleArray data1 = r.getSeries(180, 150, -15, 15);
  DoubleArray timeStamps = r.getDateSeries(180, Chart::chartTime(2011, 1, 1), 86400);

  // Create a XYChart object of size 670 x 400 pixels
  XYChart* c = new XYChart(670, 400);

  // Add a title to the chart using 18 pts Times New Roman Bold Italic font
  c->addTitle("Plasma Stabilizer Energy Usage", "timesbi.ttf", 18);

  // Set the plotarea at (50, 55) with width 100 pixels less than chart width, and height 90 pixels
  // less than chart height. Use a vertical gradient from light blue (f0f6ff) to sky blue (a0c0ff)
  // as background. Set border to transparent and grid lines to white (ffffff).
  c->setPlotArea(50, 55, c->getWidth() - 100, c->getHeight() - 90,
                 c->linearGradientColor(0, 55, 0, c->getHeight() - 35, 0xf0f6ff, 0xa0c0ff),
                 -1, Chart::Transparent, 0xffffff, 0xffffff);

  // Add a legend box at (50, 25) using horizontal layout. Use 10pts Arial Bold as font. Set the
  // background and border color to Transparent.
  c->addLegend(50, 25, false, "arialbd.ttf", 10)->setBackground(Chart::Transparent);

  // Set axis label style to 8pts Arial Bold
  c->xAxis()->setLabelStyle("arialbd.ttf", 8);
  c->yAxis()->setLabelStyle("arialbd.ttf", 8);
  c->yAxis2()->setLabelStyle("arialbd.ttf", 8);

  // Set the axis stem to transparent
  c->xAxis()->setColors(Chart::Transparent);
  c->yAxis()->setColors(Chart::Transparent);
  c->yAxis2()->setColors(Chart::Transparent);

  // Configure x-axis label format
  c->xAxis()->setMultiFormat(Chart::StartOfYearFilter(), "{value|mm/yyyy} ",
                             Chart::StartOfMonthFilter(), "{value|mm}");

  // Add axis title using 10pts Arial Bold Italic font
  c->yAxis()->setTitle("Power Usage (Watt)", "arialbi.ttf", 10);
  c->yAxis2()->setTitle("Effective Load (kg)", "arialbi.ttf", 10);

  // Add a line layer to the chart using a line width of 2 pixels.
  LineLayer* layer = c->addLineLayer();
  layer->setLineWidth(2);

  // Add 2 data series to the line layer
  layer->setXData(timeStamps);
  layer->addDataSet(data0, 0xcc0000, "Power Usage");
  layer->addDataSet(data1, 0x008800, "Effective Load")->setUseYAxis2();

  // Set the chart image to the wxChartViewer
  viewer->setChart(c);
}

// Draw track line with axis labels
void
TrackAxis::TrackLineAxis(XYChart *c, int mouseX)
{
  // Clear the current dynamic layer and get the DrawArea object to draw on it.
  DrawArea* d = c->initDynamicLayer();

  // The plot area object
  PlotArea* plotArea = c->getPlotArea();

  // Get the data x-value that is nearest to the mouse, and find its pixel coordinate.
  double xValue = c->getNearestXValue(mouseX);
  int xCoor = c->getXCoor(xValue);

  // The vertical track line is drawn up to the highest data point (the point with smallest
  // y-coordinate). We need to iterate all datasets in all layers to determine where it is.
  int minY = plotArea->getBottomY();

  // Iterate through all layers to find the highest data point
  for(int i = 0; i < c->getLayerCount(); ++i)
  {
    Layer* layer = c->getLayerByZ(i);

    // The data array index of the x-value
    int xIndex = layer->getXIndexOf(xValue);

    // Iterate through all the data sets in the layer
    for(int j = 0; j < layer->getDataSetCount(); ++j)
    {
      DataSet* dataSet = layer->getDataSetByZ(j);

      double dataPoint = dataSet->getPosition(xIndex);
      if ((dataPoint != Chart::NoValue) && (dataSet->getDataColor() != (int)Chart::Transparent))
      {
        minY = min(minY, c->getYCoor(dataPoint, dataSet->getUseYAxis()));
      }
    }
  }

  // Draw a vertical track line at the x-position up to the highest data point.
  d->vline(max(minY, plotArea->getTopY()), plotArea->getBottomY() + 6, xCoor, d->dashLineColor(0x000000, 0x0101));

  // Draw a label on the x-axis to show the track line position
  ostringstream xlabel;
  xlabel << "<*font,bgColor=000000*> " << c->xAxis()->getFormattedLabel(xValue, "mmm dd, yyyy")
         << " <*/font*>";
  TTFText* t = d->text(xlabel.str().c_str(), "arialbd.ttf", 8);
  t->draw(xCoor, plotArea->getBottomY() + 6, 0xffffff, Chart::Top);
  t->destroy();

  // Iterate through all layers to build the legend array
  for(int i = 0; i < c->getLayerCount(); ++i)
  {
    Layer* layer = c->getLayerByZ(i);

    // The data array index of the x-value
    int xIndex = layer->getXIndexOf(xValue);

    // Iterate through all the data sets in the layer
    for(int j = 0; j < layer->getDataSetCount(); ++j)
    {
      DataSet* dataSet = layer->getDataSetByZ(j);

      // The positional value, axis binding, pixel coordinate and color of the data point.
      double dataPoint = dataSet->getPosition(xIndex);
      Axis* yAxis = dataSet->getUseYAxis();
      int yCoor = c->getYCoor(dataPoint, yAxis);
      int color = dataSet->getDataColor();

      // Draw the axis label only for visible data points of named data sets
      if ((dataPoint != Chart::NoValue) && (color != (int)Chart::Transparent) &&
          (yCoor >= plotArea->getTopY()) && (yCoor <= plotArea->getBottomY()))
      {
        // The axis label consists of 3 parts - a track dot for the data point, an axis label,
        // and a line joining the track dot to the axis label.

        // Draw the line first. The end point of the line at the axis label side depends on
        // whether the label is at the left or right side of the axis (that is, on whether the
        // axis is on the left or right side of the plot area).
        int xPos = yAxis->getX() + ((yAxis->getAlignment() == Chart::Left) ? -4 : 4);
        d->hline(xCoor, xPos, yCoor, d->dashLineColor(color, 0x0101));

        // Draw the track dot
        d->circle(xCoor, yCoor, 4, 4, color, color);

        // Draw the axis label. If the axis is on the left side of the plot area, the labels
        // should right aligned to the axis, and vice versa.
        ostringstream axisLabel;
        axisLabel << "<*font,bgColor=" << hex << color << "*> "
                  << c->formatValue(dataPoint, "{value|P4}") << " <*/font*>";
        t = d->text(axisLabel.str().c_str(), "arialbd.ttf", 8);
        t->draw(xPos, yCoor, 0xffffff,
                ((yAxis->getAlignment() == Chart::Left) ? Chart::Right : Chart::Left));
        t->destroy();
      }
    }
  }
}

// Draw track cursor when mouse is moving over plotarea
void
TrackAxis::OnMouseMovePlotArea(wxCommandEvent& event)
{
  TrackLineAxis((XYChart*) m_chartViewer->getChart(), m_chartViewer->getPlotAreaMouseX());
  m_chartViewer->updateDisplay();
    
  // Hide the track cursor when the mouse leaves the plot area
  m_chartViewer->removeDynamicLayer("mouseLeavePlotArea");
}

void
TrackAxis::OnExitClick(wxCommandEvent& event)
{
  wxUnusedVar(event);
  Close(true);
}

/*
* wxEVT_CLOSE_WINDOW event handler for ID_HELLOWORLD
*/

void
TrackAxis::OnCloseWindow(wxCloseEvent& event)
{
  event.Skip();
}

/*
 * Should we show tooltips?
 */

bool
TrackAxis::ShowToolTips()
{
  return true;
}

/*
 * Get bitmap resources
 */

wxBitmap
TrackAxis::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
}

/*
 * Get icon resources
 */

wxIcon
TrackAxis::GetIconResource( const wxString& name )
{
    // Icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
}
