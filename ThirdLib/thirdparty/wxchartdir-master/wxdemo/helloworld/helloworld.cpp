/*
** Name:        helloworld.cpp
** Purpose:     Hello World sample application
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

#include "helloworld.h"
#include "wxchartviewer.h"

#include "mondrian.xpm"

/*
 * HelloWorld type definition
 */

IMPLEMENT_DYNAMIC_CLASS( HelloWorld, wxFrame )

/*
 * HelloWorld event table definition
 */

BEGIN_EVENT_TABLE( HelloWorld, wxFrame )

  EVT_MENU(wxID_EXIT, HelloWorld::OnExitClick)
  EVT_CLOSE(HelloWorld::OnCloseWindow)

END_EVENT_TABLE()

/*
 * HelloWorld constructors
 */

HelloWorld::HelloWorld()
{
  Init();
}

HelloWorld::HelloWorld( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create(parent, id, caption, pos, size, style);
}

/*
 * HelloWorld creator
 */

bool
HelloWorld::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
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
 * HelloWorld destructor
 */

HelloWorld::~HelloWorld()
{
}

/*
 * Member initialisation
 */

void
HelloWorld::Init()
{
  // Draw Chart and set to wxChartViewer

  // The data for the bar chart
  double data[] = { 85, 156, 179.5, 211, 123 };

  // The labels for the bar chart
  const char* labels[] = { "Mon", "Tue", "Wed", "Thu", "Fri" };

  // Create a XYChart object of size 250 x 250 pixels
  m_chart = new XYChart(250, 250);

  // Set the plotarea at (30, 20) and of size 200 x 200 pixels
  m_chart->setPlotArea(30, 20, 200, 200);

  // Add a bar chart layer using the given data
  m_chart->addBarLayer(DoubleArray(data, sizeof(data) / sizeof(data[0])));

  // Set the labels on the x axis.
  m_chart->xAxis()->setLabels(StringArray(labels, sizeof(labels) / sizeof(labels[0])));
}

/*
 * Control creation for HelloWorld
 */

void
HelloWorld::CreateControls()
{    
  SetIcon(wxICON(mondrian)); 
  HelloWorld* itemDialog1 = this;

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  itemDialog1->SetSizer(itemBoxSizer2);

  wxChartViewer* chartViewer = new wxChartViewer( itemDialog1 /*, ID_PANEL, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL*/ );
  chartViewer->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemBoxSizer2->Add(chartViewer, 1, wxGROW | wxALL, 3);

  // Output the chart
  chartViewer->setChart(m_chart);

  // Include tool tip for the chart
  chartViewer->setImageMap(m_chart->getHTMLImageMap("", "", "title='{xLabel}: US${value}K'"));
}

void
HelloWorld::OnExitClick(wxCommandEvent& event)
{
  wxUnusedVar(event);
  Close(true);
}

/*
* wxEVT_CLOSE_WINDOW event handler for ID_HELLOWORLD
*/

void
HelloWorld::OnCloseWindow(wxCloseEvent& event)
{
  delete m_chart;
  event.Skip();
}

/*
 * Should we show tooltips?
 */

bool
HelloWorld::ShowToolTips()
{
  return true;
}

/*
 * Get bitmap resources
 */

wxBitmap
HelloWorld::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
  wxUnusedVar(name);
  return wxNullBitmap;
}

/*
 * Get icon resources
 */

wxIcon
HelloWorld::GetIconResource( const wxString& name )
{
  // Icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
}
