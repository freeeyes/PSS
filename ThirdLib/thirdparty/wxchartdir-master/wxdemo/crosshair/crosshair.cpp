/*
** Name:        crosshair.cpp
** Purpose:     Crosshair with Axis Labels sample application
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

#include "crosshair.h"
#include "wxchartviewer.h"

#include <sstream>

using namespace std;

#include "mondrian.xpm"

// CrossHair type definition

IMPLEMENT_DYNAMIC_CLASS( CrossHair, wxFrame )

// CrossHair event table definition

BEGIN_EVENT_TABLE( CrossHair, wxFrame )

  EVT_MENU(wxID_EXIT, CrossHair::OnExitClick)
  EVT_CLOSE(CrossHair::OnCloseWindow)
  EVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA(ID_CHARTVIEWER, CrossHair::OnMouseMovePlotArea)

END_EVENT_TABLE()

// CrossHair constructors

CrossHair::CrossHair()
{
  Init();
}

CrossHair::CrossHair( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create(parent, id, caption, pos, size, style);
}

// CrossHair creator
bool
CrossHair::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
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

// CrossHair destructor
CrossHair::~CrossHair()
{
  if (m_chartViewer != NULL)
  {
    delete m_chartViewer->getChart();
  }
}

// Member initialisation
void
CrossHair::Init()
{
  m_chartViewer = NULL;
}

// Control creation for CrossHair
void
CrossHair::CreateControls()
{    
  SetIcon(wxICON(mondrian)); 
  CrossHair* itemDialog1 = this;

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  itemDialog1->SetSizer(itemBoxSizer2);

  m_chartViewer = new wxChartViewer( itemDialog1, ID_CHARTVIEWER, wxDefaultPosition, wxSize(520,490));
  m_chartViewer->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemBoxSizer2->Add(m_chartViewer, 1, wxGROW | wxALL, 3);

  // Output the chart
  DrawChart(m_chartViewer);
}

// Draw the chart and display it in the given viewer
void
CrossHair::DrawChart(wxChartViewer* viewer)
{
  // The XY data of the first data series
  double dataX[] = {50, 55, 37, 24, 42, 49, 63, 72, 83, 59};
  double dataY[] = {3.6, 2.8, 2.5, 2.3, 3.8, 3.0, 3.8, 5.0, 6.0, 3.3};
  int noOfPoints = (int)(sizeof(dataX) / sizeof(*dataX));

  // Create a XYChart object of size 520 x 490 pixels
  XYChart* c = new XYChart(520, 490);

  // Set the plotarea at (60, 40) and of size 450 x 400 pixels, with white background and a light
  // grey border (0xc0c0c0). Turn on both horizontal and vertical grid lines with light grey color
  // (0xc0c0c0)
  c->setPlotArea(60, 40, 450, 400, 0xffffff, -1, 0xc0c0c0, 0xc0c0c0, -1);

  // Add a title to the chart using 18 point Times Bold Itatic font.
  c->addTitle("      Chemical X Thermal Conductivity", "timesbi.ttf", 18);

  // Add titles to the axes using 12 pts Arial Bold Italic font
  c->yAxis()->setTitle("Thermal Conductivity (W/K)", "arialbi.ttf", 12);
  c->xAxis()->setTitle("Concentration (g/liter)", "arialbi.ttf", 12);

  // Set the axes line width to 3 pixels
  c->yAxis()->setWidth(3);
  c->xAxis()->setWidth(3);

  // Add a scatter layer using (dataX, dataY)
  ScatterLayer* scatterLayer = c->addScatterLayer(DoubleArray(dataX, noOfPoints), 
  DoubleArray(dataY, noOfPoints), "", Chart::GlassSphereShape, 13, 0xcc0000);

  // Tooltip for the scatter layer
  scatterLayer->setHTMLImageMap("", "", "title='Concentration: {x} g/liter, Conductivity: {value} W/K'");

  // Add a trend line layer for (dataX, dataY)
  TrendLayer* trendLayer = c->addTrendLayer(DoubleArray(dataX, noOfPoints), 
                                               DoubleArray(dataY, noOfPoints), 0xcc0000);

  // Set the line width to 3 pixels
  trendLayer->setLineWidth(3);

  // Add a 95% confidence band for the line
  trendLayer->addConfidenceBand(0.95, 0x806666ff);

  // Add a 95% confidence band (prediction band) for the points
  trendLayer->addPredictionBand(0.95, 0x8066ff66);

  // Tool tip for the trend layer
  trendLayer->setHTMLImageMap("", "", "title='Trend Line: y = {slope|P4} x + {intercept|P4}'");

  // Add a legend box at (60, 35) (top of the chart) with horizontal layout. Use 10 pts Arial Bold
  // Italic font. Set the background and border color to Transparent and use line style legend
  // icons.
  LegendBox* legendBox = c->addLegend(60, 35, false, "arialbi.ttf", 9);
  legendBox->setBackground(Chart::Transparent);
  legendBox->setLineStyleKey(true);

  // Add entries to the legend box
  legendBox->addKey("95% Line Confidence", 0x806666ff);
  legendBox->addKey("95% Point Confidence", 0x8066ff66);

  ostringstream trendLegend;
  trendLegend << "Trend Line: y = " << c->formatValue(trendLayer->getSlope(), "{value|4}") << " x";
  trendLegend << " + " << c->formatValue(trendLayer->getIntercept(), "{value|4}");
  legendBox->addKey(trendLegend.str().c_str(), 0xcc0000, 3);

  // Set the chart image to the wxChartViewer
  viewer->setChart(c);

  // Include tool tip for the chart
  viewer->setImageMap(c->getHTMLImageMap(""));
}

// Draw track cursor when mouse is moving over plotarea
void
CrossHair::OnMouseMovePlotArea(wxCommandEvent& evt)
{
  Crosshair((XYChart*) m_chartViewer->getChart(),
            m_chartViewer->getPlotAreaMouseX(),
            m_chartViewer->getPlotAreaMouseY());
  m_chartViewer->updateDisplay();
    
  // Hide the track cursor when the mouse leaves the plot area
  m_chartViewer->removeDynamicLayer(wxS("mouseLeavePlotArea"));
}

// Draw cross hair cursor with axis labels
void
CrossHair::Crosshair(XYChart* c, int mouseX, int mouseY)
{
  // Clear the current dynamic layer and get the DrawArea object to draw on it.
  DrawArea* d = c->initDynamicLayer();

  // The plot area object
  PlotArea* plotArea = c->getPlotArea();

  // Draw a vertical line and a horizontal line as the cross hair
  d->vline(plotArea->getTopY(), plotArea->getBottomY(), mouseX, d->dashLineColor(0x000000, 0x0101));
  d->hline(plotArea->getLeftX(), plotArea->getRightX(), mouseY, d->dashLineColor(0x000000, 0x0101));

  // Draw y-axis label
  ostringstream ylabel;
  ylabel << "<*block,bgColor=FFFFDD,margin=3,edgeColor=000000*>" 
         << c->formatValue(c->getYValue(mouseY, c->yAxis()), "{value|P4}") << "<*/*>";
  TTFText* t = d->text(ylabel.str().c_str(), "arialbd.ttf", 8);
  t->draw(plotArea->getLeftX() - 5, mouseY, 0x000000, Chart::Right);
  t->destroy();

  // Draw x-axis label
  ostringstream xlabel;
  xlabel << "<*block,bgColor=FFFFDD,margin=3,edgeColor=000000*>" 
         << c->formatValue(c->getXValue(mouseX), "{value|P4}") << "<*/*>";
  t = d->text(xlabel.str().c_str(), "arialbd.ttf", 8);
  t->draw(mouseX, plotArea->getBottomY() + 5, 0x000000, Chart::Top);
  t->destroy();
}

void
CrossHair::OnExitClick(wxCommandEvent& event)
{
  wxUnusedVar(event);
  Close(true);
}

// wxEVT_CLOSE_WINDOW event handler for ID_CROSSHAIR
void
CrossHair::OnCloseWindow(wxCloseEvent& event)
{
  event.Skip();
}

// Should we show tooltips?
bool
CrossHair::ShowToolTips()
{
    return true;
}

// Get bitmap resources
wxBitmap
CrossHair::GetBitmapResource( const wxString& name )
{
    wxUnusedVar(name);
    return wxNullBitmap;
}

// Get icon resources
wxIcon
CrossHair::GetIconResource( const wxString& name )
{
    // Icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
}
