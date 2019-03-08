/*
** Name:        trackbox.cpp
** Purpose:     Track Box sample application
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

#include "trackbox.h"
#include "wxchartviewer.h"

#include <sstream>

using namespace std;

#include "mondrian.xpm"

/*
 * TrackBox type definition
 */

IMPLEMENT_DYNAMIC_CLASS( TrackBox, wxFrame )

/*
 * TrackBox event table definition
 */

BEGIN_EVENT_TABLE( TrackBox, wxFrame )

  EVT_MENU(wxID_EXIT, TrackBox::OnExitClick)
  EVT_CLOSE(TrackBox::OnCloseWindow)
  EVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA(ID_CHARTVIEWER, TrackBox::OnMouseMovePlotArea)

END_EVENT_TABLE()

/*
 * TrackBox constructors
 */

TrackBox::TrackBox()
{
  Init();
}

TrackBox::TrackBox( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create(parent, id, caption, pos, size, style);
}


/*
 * TrackBox creator
 */

bool
TrackBox::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
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
 * TrackBox destructor
 */

TrackBox::~TrackBox()
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
TrackBox::Init()
{
}

/*
 * Control creation for TrackBox
 */

void
TrackBox::CreateControls()
{    
  SetIcon(wxICON(mondrian)); 
  TrackBox* itemDialog1 = this;

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
TrackBox::DrawChart(wxChartViewer* viewer)
{
  // The data for the bar chart
  double data0[] = {100, 125, 245, 147, 67};
  double data1[] = {85, 156, 179, 211, 123};
  double data2[] = {97, 87, 56, 267, 157};
  const char* labels[] = {"Mon", "Tue", "Wed", "Thur", "Fri"};
  int noOfPoints = (int)(sizeof(data0)/sizeof(*data0));

  // Create a XYChart object of size 540 x 375 pixels
  XYChart* c = new XYChart(540, 375);

  // Add a title to the chart using 18 pts Times Bold Italic font
  c->addTitle("Average Weekly Network Load", "timesbi.ttf", 18);

  // Set the plotarea at (50, 55) and of 440 x 280 pixels in size. Use a vertical gradient color
  // from light blue (f9f9ff) to blue (6666ff) as background. Set border and grid lines to white
  // (ffffff).
  c->setPlotArea(50, 55, 440, 280, 
                 c->linearGradientColor(0, 55, 0, 335, 0xf9f9ff, 0x6666ff), -1, 0xffffff, 0xffffff);

  // Add a legend box at (50, 28) using horizontal layout. Use 10pts Arial Bold as font, with
  // transparent background.
  c->addLegend(50, 28, false, "arialbd.ttf", 10)->setBackground(Chart::Transparent);

  // Set the x axis labels
  c->xAxis()->setLabels(StringArray(labels, noOfPoints));

  // Draw the ticks between label positions (instead of at label positions)
  c->xAxis()->setTickOffset(0.5);

  // Set axis label style to 8pts Arial Bold
  c->xAxis()->setLabelStyle("arialbd.ttf", 8);
  c->yAxis()->setLabelStyle("arialbd.ttf", 8);

  // Set axis line width to 2 pixels
  c->xAxis()->setWidth(2);
  c->yAxis()->setWidth(2);

  // Add axis title
  c->yAxis()->setTitle("Throughput (MBytes Per Hour)");

  // Add a multi-bar layer with 3 data sets
  BarLayer* layer = c->addBarLayer(Chart::Side);
  layer->addDataSet(DoubleArray(data0, noOfPoints), 0xff0000, "Server #1");
  layer->addDataSet(DoubleArray(data1, noOfPoints), 0x00ff00, "Server #2");
  layer->addDataSet(DoubleArray(data2, noOfPoints), 0xff8800, "Server #3");

  // Set bar border to transparent. Use glass lighting effect with light direction from left.
 layer->setBorderColor(Chart::Transparent, Chart::glassEffect(Chart::NormalGlare, Chart::Left));

  // Configure the bars within a group to touch each others (no gap)
  layer->setBarGap(0.2, Chart::TouchBar);

  // Set the chart image to the wxChartViewer
  viewer->setChart(c);
}

// Draw the track box with legend
void
TrackBox::TrackBoxLegend(XYChart* c, int mouseX, int mouseY)
{
 // Clear the current dynamic layer and get the DrawArea object to draw on it.
  DrawArea* d = c->initDynamicLayer();

  // The plot area object
  PlotArea* plotArea = c->getPlotArea();

  // Get the data x-value that is nearest to the mouse
  double xValue = c->getNearestXValue(mouseX);

  // Compute the position of the box. This example assumes a label based x-axis, in which the
  // labeling spacing is one x-axis unit. So the left and right sides of the box is 0.5 unit from
  // the central x-value.
  int boxLeft = c->getXCoor(xValue - 0.5);
  int boxRight = c->getXCoor(xValue + 0.5);
  int boxTop = plotArea->getTopY();
  int boxBottom = plotArea->getBottomY();

  // Draw the track box
  d->rect(boxLeft, boxTop, boxRight, boxBottom, 0x000000, Chart::Transparent);

  // Container to hold the legend entries
  vector<string> legendEntries;

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

      // Build the legend entry, consist of the legend icon, the name and the data value.
      double dataValue = dataSet->getValue(xIndex);
      if ((dataValue != Chart::NoValue) && (dataSet->getDataColor() != (int)Chart::Transparent))
      {
        ostringstream legendEntry;
        legendEntry << dataSet->getLegendIcon() << " " << dataSet->getDataName() << ": " 
                    << c->formatValue(dataValue, "{value|P4}");
        legendEntries.push_back(legendEntry.str());
      }
    }
  }

  // Create the legend by joining the legend entries
  if (legendEntries.size() > 0)
  {
    ostringstream legend;
    legend << "<*block,bgColor=FFFFCC,edgeColor=000000,margin=5*><*font,underline=1*>"
           << c->xAxis()->getFormattedLabel(xValue) << "<*/font*>";
    for (int i = ((int)legendEntries.size()) - 1; i >= 0; --i)
    {
      legend << "<*br*>" << legendEntries[i];
    }
    legend << "<*/*>";

    // Display the legend at the bottom-right side of the mouse cursor, and make sure the legend
    // will not go outside the chart image.
    TTFText* t = d->text(legend.str().c_str(), "arialbd.ttf", 8);
    t->draw(min(mouseX + 12, c->getWidth() - t->getWidth()), min(mouseY + 18,
            c->getHeight() - t->getHeight()), 0x000000, Chart::TopLeft);
    t->destroy();
  }
}

// Draw track cursor when mouse is moving over plotarea
void
TrackBox::OnMouseMovePlotArea(wxCommandEvent& event)
{
  TrackBoxLegend((XYChart*) m_chartViewer->getChart(), m_chartViewer->getPlotAreaMouseX(),
                                                       m_chartViewer->getPlotAreaMouseY());
  m_chartViewer->updateDisplay();
    
  // Hide the track cursor when the mouse leaves the plot area
  m_chartViewer->removeDynamicLayer("mouseLeavePlotArea");
}

void
TrackBox::OnExitClick(wxCommandEvent& event)
{
  wxUnusedVar(event);
  Close(true);
}

/*
* wxEVT_CLOSE_WINDOW event handler for ID_HELLOWORLD
*/

void
TrackBox::OnCloseWindow(wxCloseEvent& event)
{
  event.Skip();
}

/*
 * Should we show tooltips?
 */

bool
TrackBox::ShowToolTips()
{
    return true;
}

/*
 * Get bitmap resources
 */

wxBitmap
TrackBox::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
  wxUnusedVar(name);
  return wxNullBitmap;
}

/*
 * Get icon resources
 */

wxIcon TrackBox::GetIconResource( const wxString& name )
{
  // Icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
}
