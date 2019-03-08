/*
** Name:        tracklabel.cpp
** Purpose:     Track Label sample application
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

#include "tracklabel.h"
#include "wxchartviewer.h"

#include <sstream>

using namespace std;

#include "mondrian.xpm"

/*
 * TrackLabel type definition
 */

IMPLEMENT_DYNAMIC_CLASS( TrackLabel, wxFrame )

/*
 * TrackLabel event table definition
 */

BEGIN_EVENT_TABLE( TrackLabel, wxFrame )

  EVT_MENU(wxID_EXIT, TrackLabel::OnExitClick)
  EVT_CLOSE(TrackLabel::OnCloseWindow)
  EVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA(ID_CHARTVIEWER, TrackLabel::OnMouseMovePlotArea)

END_EVENT_TABLE()

/*
 * TrackLabel constructors
 */

TrackLabel::TrackLabel()
{
  Init();
}

TrackLabel::TrackLabel( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create(parent, id, caption, pos, size, style);
}

/*
 * TrackLabel creator
 */

bool
TrackLabel::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
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
 * TrackLabel destructor
 */

TrackLabel::~TrackLabel()
{
  if (m_chartViewer != NULL)
  {
    delete m_chartViewer->getChart();
  }
}

/*
 * Member initialisation
 */

void TrackLabel::Init()
{
}

/*
 * Control creation for TrackLabel
 */

void
TrackLabel::CreateControls()
{    
  SetIcon(wxICON(mondrian)); 
  TrackLabel* itemDialog1 = this;

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
TrackLabel::DrawChart(wxChartViewer* viewer)
{
  // Data for the chart as 3 random data series
  RanSeries r(127);
  DoubleArray data0 = r.getSeries(100, 100, -15, 15);
  DoubleArray data1 = r.getSeries(100, 150, -15, 15);
  DoubleArray data2 = r.getSeries(100, 200, -15, 15);
  DoubleArray timeStamps = r.getDateSeries(100, Chart::chartTime(2011, 1, 1), 86400);

  // Create a XYChart object of size 640 x 400 pixels
  XYChart* c = new XYChart(640, 400);

  // Add a title to the chart using 18 pts Times New Roman Bold Italic font
  c->addTitle("    Product Line Global Revenue", "timesbi.ttf", 18);

  // Set the plotarea at (50, 55) with width 70 pixels less than chart width, and height 90 pixels
  // less than chart height. Use a vertical gradient from light blue (f0f6ff) to sky blue (a0c0ff)
  // as background. Set border to transparent and grid lines to white (ffffff).
  c->setPlotArea(50, 55, c->getWidth() - 70, c->getHeight() - 90,
                 c->linearGradientColor(0, 55, 0, c->getHeight() - 35, 0xf0f6ff, 0xa0c0ff),
                 -1, Chart::Transparent, 0xffffff, 0xffffff);

  // Add a legend box at (50, 25) using horizontal layout. Use 10pts Arial Bold as font. Set the
  // background and border color to Transparent.
  c->addLegend(50, 25, false, "arialbd.ttf", 10)->setBackground(Chart::Transparent);

  // Set axis label style to 8pts Arial Bold
  c->xAxis()->setLabelStyle("arialbd.ttf", 8);
  c->yAxis()->setLabelStyle("arialbd.ttf", 8);

  // Set the axis stem to transparent
  c->xAxis()->setColors(Chart::Transparent);
  c->yAxis()->setColors(Chart::Transparent);

  // Configure x-axis label format
  c->xAxis()->setMultiFormat(Chart::StartOfYearFilter(), "{value|mm/yyyy} ",
                             Chart::StartOfMonthFilter(), "{value|mm}");

  // Add axis title using 10pts Arial Bold Italic font
  c->yAxis()->setTitle("USD millions", "arialbi.ttf", 10);

  // Add a line layer to the chart using a line width of 2 pixels.
  LineLayer* layer = c->addLineLayer();
  layer->setLineWidth(2);

  // Add 3 data series to the line layer
  layer->setXData(timeStamps);
  layer->addDataSet(data0, 0xff3333, "Alpha");
  layer->addDataSet(data1, 0x008800, "Beta");
  layer->addDataSet(data2, 0x3333cc, "Gamma");

  // Set the chart image to the wxChartViewer
  viewer->setChart(c);
}

// Draw track line with data labels
void
TrackLabel::TrackLineLabel(XYChart* c, int mouseX)
{
  // Clear the current dynamic layer and get the DrawArea object to draw on it.
  DrawArea* d = c->initDynamicLayer();

  // The plot area object
  PlotArea* plotArea = c->getPlotArea();

  // Get the data x-value that is nearest to the mouse, and find its pixel coordinate.
  double xValue = c->getNearestXValue(mouseX);
  int xCoor = c->getXCoor(xValue);

  // Draw a vertical track line at the x-position
  d->vline(plotArea->getTopY(), plotArea->getBottomY(), xCoor,
           d->dashLineColor(0x000000, 0x0101));

  // Draw a label on the x-axis to show the track line position.
  ostringstream xlabel;
  xlabel << "<*font,bgColor=000000*> " << c->xAxis()->getFormattedLabel(xValue, "mmm dd, yyyy")
         << " <*/font*>";
  TTFText* t = d->text(xlabel.str().c_str(), "arialbd.ttf", 8);

  // Restrict the x-pixel position of the label to make sure it stays inside the chart image.
  int xLabelPos = max(0, min(xCoor - t->getWidth() / 2, c->getWidth() - t->getWidth()));
  t->draw(xLabelPos, plotArea->getBottomY() + 6, 0xffffff);
  t->destroy();

  // Iterate through all layers to draw the data labels
  for (int i = 0; i < c->getLayerCount(); ++i)
  {
    Layer* layer = c->getLayerByZ(i);

    // The data array index of the x-value
    int xIndex = layer->getXIndexOf(xValue);

    // Iterate through all the data sets in the layer
    for (int j = 0; j < layer->getDataSetCount(); ++j)
    {
      DataSet* dataSet = layer->getDataSetByZ(j);
      const char* dataSetName = dataSet->getDataName();

      // Get the color, name and position of the data label
      int color = dataSet->getDataColor();
      int yCoor = c->getYCoor(dataSet->getPosition(xIndex), dataSet->getUseYAxis());

      // Draw a track dot with a label next to it for visible data points in the plot area
      if ((yCoor >= plotArea->getTopY()) && (yCoor <= plotArea->getBottomY()) &&
          (color != (int)Chart::Transparent) && dataSetName && *dataSetName)
      {
        d->circle(xCoor, yCoor, 4, 4, color, color);

        ostringstream label;
        label << "<*font,bgColor=" << hex << color << "*> "
              << c->formatValue(dataSet->getValue(xIndex), "{value|P4}") << " <*font*>";
        t = d->text(label.str().c_str(), "arialbd.ttf", 8);

        // Draw the label on the right side of the dot if the mouse is on the left side the
        // chart, and vice versa. This ensures the label will not go outside the chart image.
        if (xCoor <= (plotArea->getLeftX() + plotArea->getRightX()) / 2)
        {
          t->draw(xCoor + 5, yCoor, 0xffffff, Chart::Left);
        }
        else
        {
          t->draw(xCoor - 5, yCoor, 0xffffff, Chart::Right);
        }
        t->destroy();
      }
    }
  }
}

// Draw track cursor when mouse is moving over plotarea
void
TrackLabel::OnMouseMovePlotArea(wxCommandEvent& event)
{
  TrackLineLabel((XYChart*) m_chartViewer->getChart(), m_chartViewer->getPlotAreaMouseX());
  m_chartViewer->updateDisplay();
    
  // Hide the track cursor when the mouse leaves the plot area
  m_chartViewer->removeDynamicLayer("mouseLeavePlotArea");
}

void
TrackLabel::OnExitClick(wxCommandEvent& event)
{
  wxUnusedVar(event);
  Close(true);
}

/*
* wxEVT_CLOSE_WINDOW event handler for ID_HELLOWORLD
*/

void TrackLabel::OnCloseWindow(wxCloseEvent& event)
{
  event.Skip();
}

/*
 * Should we show tooltips?
 */

bool
TrackLabel::ShowToolTips()
{
  return true;
}

/*
 * Get bitmap resources
 */

wxBitmap
TrackLabel::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
  wxUnusedVar(name);
  return wxNullBitmap;
}

/*
 * Get icon resources
 */

wxIcon
TrackLabel::GetIconResource( const wxString& name )
{
  // Icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
}
