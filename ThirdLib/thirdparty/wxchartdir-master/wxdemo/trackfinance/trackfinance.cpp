/*
** Name:        trackfinance.cpp
** Purpose:     Track Finance sample application
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

#include "trackfinance.h"
#include "wxchartviewer.h"

#include "FinanceChart.h"

#include <sstream>

using namespace std;

#include "mondrian.xpm"

/*
 * TrackFinance type definition
 */

IMPLEMENT_DYNAMIC_CLASS( TrackFinance, wxFrame )

/*
 * TrackFinance event table definition
 */

BEGIN_EVENT_TABLE( TrackFinance, wxFrame )

  EVT_MENU(wxID_EXIT, TrackFinance::OnExitClick)
  EVT_CLOSE(TrackFinance::OnCloseWindow)
  EVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA(ID_CHARTVIEWER, TrackFinance::OnMouseMovePlotArea)

END_EVENT_TABLE()

/*
 * TrackFinance constructors
 */

TrackFinance::TrackFinance()
{
  Init();
}

TrackFinance::TrackFinance( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create(parent, id, caption, pos, size, style);
}

/*
 * TrackFinance creator
 */

bool
TrackFinance::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
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
 * TrackFinance destructor
 */

TrackFinance::~TrackFinance()
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
TrackFinance::Init()
{
}

/*
 * Control creation for TrackFinance
 */

void
TrackFinance::CreateControls()
{    
  SetIcon(wxICON(mondrian)); 
  TrackFinance* itemDialog1 = this;

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
TrackFinance::DrawChart(wxChartViewer* viewer)
{
  // Create a finance chart demo containing 100 days of data
  int noOfDays = 100;

  // To compute moving averages starting from the first day, we need to get extra data points before
  // the first day
  int extraDays = 30;

  // In this exammple, we use a random number generator utility to simulate the data. We set up the
  // random table to create 6 cols x (noOfDays + extraDays) rows, using 9 as the seed.
  RanTable rantable(9, 6, noOfDays + extraDays);

  // Set the 1st col to be the timeStamp, starting from Sep 4, 2011, with each row representing one
  // day, and counting week days only (jump over Sat and Sun)
  rantable.setDateCol(0, Chart::chartTime(2011, 9, 4), 86400, true);

  // Set the 2nd, 3rd, 4th and 5th columns to be high, low, open and close data. The open value
  // starts from 100, and the daily change is random from -5 to 5.
  rantable.setHLOCCols(1, 100, -5, 5);

  // Set the 6th column as the vol data from 5 to 25 million
  rantable.setCol(5, 50000000, 250000000);

  // Now we read the data from the table into arrays
  DoubleArray timeStamps = rantable.getCol(0);
  DoubleArray highData = rantable.getCol(1);
  DoubleArray lowData = rantable.getCol(2);
  DoubleArray openData = rantable.getCol(3);
  DoubleArray closeData = rantable.getCol(4);
  DoubleArray volData = rantable.getCol(5);

  // Create a FinanceChart object of width 720 pixels
  FinanceChart* c = new FinanceChart(720);

  // Add a title to the chart
  c->addTitle("Finance Chart Demonstration");

  // Disable default legend box, as we are using dynamic legend
  c->setLegendStyle("normal", 8, Chart::Transparent, Chart::Transparent);

  // Set the data into the finance chart object
  c->setData(timeStamps, highData, lowData, openData, closeData, volData, extraDays);

  // Add the main chart with 240 pixels in height
  c->addMainChart(240);

  // Add a 10 period simple moving average to the main chart, using brown color
  c->addSimpleMovingAvg(10, 0x663300);

  // Add a 20 period simple moving average to the main chart, using purple color
  c->addSimpleMovingAvg(20, 0x9900ff);

  // Add candlestick symbols to the main chart, using green/red for up/down days
  c->addCandleStick(0x00ff00, 0xff0000);

  // Add 20 days bollinger band to the main chart, using light blue (9999ff) as the border and
  // semi-transparent blue (c06666ff) as the fill color
  c->addBollingerBand(20, 2, 0x9999ff, 0xc06666ff);

  // Add a 75 pixels volume bars sub-chart to the bottom of the main chart, using green/red/grey for
  // up/down/flat days
  c->addVolBars(75, 0x99ff99, 0xff9999, 0x808080);

  // Append a 14-days RSI indicator chart (75 pixels high) after the main chart. The main RSI line
  // is purple (800080). Set threshold region to +/- 20 (that is, RSI = 50 +/- 25). The upper/lower
  // threshold regions will be filled with red (ff0000)/blue (0000ff).
  c->addRSI(75, 14, 0x800080, 20, 0xff0000, 0x0000ff);

  // Append a MACD(26, 12) indicator chart (75 pixels high) after the main chart, using 9 days for
  // computing divergence.
  c->addMACD(75, 26, 12, 9, 0x0000ff, 0xff00ff, 0x008000);

  // Include track line with legend for the latest data values
  DrawTrackFinance(c, ((XYChart*) c->getChart(0))->getPlotArea()->getRightX());
    
  // Set the chart image to the wxChartViewer
  viewer->setChart(c);
}

// Draw finance chart track line with legend
void
TrackFinance::DrawTrackFinance(MultiChart* m, int mouseX)
{
  // Clear the current dynamic layer and get the DrawArea object to draw on it.
  DrawArea* d = m->initDynamicLayer();

  // It is possible for a FinanceChart to be empty, so we need to check for it.
  if (m->getChartCount() == 0)
  {
    return;
  }

  // Get the data x-value that is nearest to the mouse
  int xValue = (int)(((XYChart* ) m->getChart(0))->getNearestXValue(mouseX));

  // Iterate the XY charts (main price chart and indicator charts) in the FinanceChart
  XYChart* c = 0;
  for(int i = 0; i < m->getChartCount(); ++i)
  {
    c = (XYChart *)m->getChart(i);

    // Variables to hold the legend entries
    ostringstream ohlcLegend;
    vector<string> legendEntries;

    // Iterate through all layers to find the highest data point
    for(int j = 0; j < c->getLayerCount(); ++j)
    {
      Layer* layer = c->getLayerByZ(j);
      int xIndex = layer->getXIndexOf(xValue);
      int dataSetCount = layer->getDataSetCount();

      // In a FinanceChart, only layers showing OHLC data can have 4 data sets
      if (dataSetCount == 4)
      {
        double highValue = layer->getDataSet(0)->getValue(xIndex);
        double lowValue = layer->getDataSet(1)->getValue(xIndex);
        double openValue = layer->getDataSet(2)->getValue(xIndex);
        double closeValue = layer->getDataSet(3)->getValue(xIndex);

        if (closeValue != Chart::NoValue)
        {
          // Build the OHLC legend
          ohlcLegend << "      <*block*>";
          ohlcLegend << "Open: " << c->formatValue(openValue, "{value|P4}");
          ohlcLegend << ", High: " << c->formatValue(highValue, "{value|P4}"); 
          ohlcLegend << ", Low: " << c->formatValue(lowValue, "{value|P4}"); 
          ohlcLegend << ", Close: " << c->formatValue(closeValue, "{value|P4}");

          // We also draw an upward or downward triangle for up and down days and the %
          // change
          double lastCloseValue = layer->getDataSet(3)->getValue(xIndex - 1);
          if (lastCloseValue != Chart::NoValue)
          {
            double change = closeValue - lastCloseValue;
            double percent = change * 100 / closeValue;
            string symbol = (change >= 0) ?
                            "<*font,color=008800*><*img=@triangle,width=8,color=008800*>" :
                            "<*font,color=CC0000*><*img=@invertedtriangle,width=8,color=CC0000*>";

            ohlcLegend << "  " << symbol << " " << c->formatValue(change, "{value|P4}");
            ohlcLegend << " (" << c->formatValue(percent, "{value|2}") << "%)<*/font*>";
          }

          ohlcLegend << "<*/*>";
        }
      }
      else
      {
        // Iterate through all the data sets in the layer
        for(int k = 0; k < layer->getDataSetCount(); ++k)
        {
          DataSet* dataSet = layer->getDataSetByZ(k);

          string name = dataSet->getDataName();
          double value = dataSet->getValue(xIndex);
          if ((0 != name.size()) && (value != Chart::NoValue))
          {
            // In a FinanceChart, the data set name consists of the indicator name and its
            // latest value. It is like "Vol: 123M" or "RSI (14): 55.34". As we are
            // generating the values dynamically, we need to extract the indictor name
            // out, and also the volume unit (if any).

            // The volume unit
            string unitChar;

            // The indicator name is the part of the name up to the colon character.
            int delimiterPosition = (int)name.find(':');
            if ((int)name.npos != delimiterPosition)
            {
              // The unit, if any, is the trailing non-digit character(s).
              int lastDigitPos = (int)name.find_last_of("0123456789");
              if (((int)name.npos != lastDigitPos) && (lastDigitPos + 1 < (int)name.size())
                                && (lastDigitPos > delimiterPosition))
              {
                unitChar = name.substr(lastDigitPos + 1);
              }

              name.resize(delimiterPosition);
            }

            // In a FinanceChart, if there are two data sets, it must be representing a
            // range.
            if (dataSetCount == 2)
            {
              // We show both values in the range in a single legend entry
              value = layer->getDataSet(0)->getValue(xIndex);
              double value2 = layer->getDataSet(1)->getValue(xIndex);
              name = name + ": " + c->formatValue(min(value, value2), "{value|P3}");
              name = name + " - " + c->formatValue(max(value, value2), "{value|P3}");
            }
            else
            {
              // In a FinanceChart, only the layer for volume bars has 3 data sets for
              // up/down/flat days
              if (dataSetCount == 3)
              {
                // The actual volume is the sum of the 3 data sets.
                value = layer->getDataSet(0)->getValue(xIndex) + 
                        layer->getDataSet(1)->getValue(xIndex) +
                        layer->getDataSet(2)->getValue(xIndex);
              }

              // Create the legend entry
              name = name + ": " + c->formatValue(value, "{value|P3}") + unitChar;
            }

            // Build the legend entry, consist of a colored square box and the name (with
            // the data value in it).
            ostringstream legendEntry;
            legendEntry << "<*block*><*img=@square,width=8,edgeColor=000000,color=" 
                        << hex << dataSet->getDataColor() << "*> " << name << "<*/*>";
            legendEntries.push_back(legendEntry.str());
          }
        }
      }
    }

    // Get the plot area position relative to the entire FinanceChart
    PlotArea* plotArea = c->getPlotArea();
    int plotAreaLeftX = plotArea->getLeftX() + c->getAbsOffsetX();
    int plotAreaTopY = plotArea->getTopY() + c->getAbsOffsetY();

    // The legend begins with the date label, then the ohlcLegend (if any), and then the
    // entries for the indicators.
    ostringstream legendText;
    legendText << "<*block,valign=top,maxWidth=" << (plotArea->getWidth() - 5) 
               << "*><*font=arialbd.ttf*>[" << c->xAxis()->getFormattedLabel(xValue, "mmm dd, yyyy")
               << "]<*/font*>" << ohlcLegend.str();
    for (int i = ((int)legendEntries.size()) - 1; i >= 0; --i)
    {
      legendText << "      " << legendEntries[i];
    }
    legendText << "<*/*>";

    // Draw a vertical track line at the x-position
    d->vline(plotAreaTopY, plotAreaTopY + plotArea->getHeight(),
             c->getXCoor(xValue) + c->getAbsOffsetX(), d->dashLineColor(0x000000, 0x0101));

    // Display the legend on the top of the plot area
    TTFText* t = d->text(legendText.str().c_str(), "arial.ttf", 8);
    t->draw(plotAreaLeftX + 5, plotAreaTopY + 3, 0x000000, Chart::TopLeft);
    t->destroy();
  }
}

// Draw track cursor when mouse is moving over plotarea
void
TrackFinance::OnMouseMovePlotArea(wxCommandEvent& event)
{
  DrawTrackFinance((MultiChart*) m_chartViewer->getChart(), m_chartViewer->getPlotAreaMouseX());
  m_chartViewer->updateDisplay();
}

void
TrackFinance::OnExitClick(wxCommandEvent& event)
{
  wxUnusedVar(event);
  Close(true);
}

/*
* wxEVT_CLOSE_WINDOW event handler for ID_HELLOWORLD
*/

void
TrackFinance::OnCloseWindow(wxCloseEvent& event)
{
  event.Skip();
}

/*
 * Should we show tooltips?
 */

bool
TrackFinance::ShowToolTips()
{
  return true;
}

/*
 * Get bitmap resources
 */

wxBitmap
TrackFinance::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
  wxUnusedVar(name);
  return wxNullBitmap;
}

/*
 * Get icon resources
 */

wxIcon
TrackFinance::GetIconResource( const wxString& name )
{
  // Icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
}
