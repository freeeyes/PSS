/*
** Name:        simplezoomscroll.cpp
** Purpose:     Simple Zooming and Scrolling
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

#include "simplezoomscroll.h"

#include "mondrian.xpm"

/*
 * SimpleZoomScroll type definition
 */

IMPLEMENT_CLASS( SimpleZoomScroll, wxFrame )

/*
 * SimpleZoomScroll event table definition
 */

BEGIN_EVENT_TABLE( SimpleZoomScroll, wxFrame )

  EVT_TOGGLEBUTTON( ID_POINTER, SimpleZoomScroll::OnPointerClick )
  EVT_TOGGLEBUTTON( wxID_ZOOM_IN, SimpleZoomScroll::OnZoomInClick )
  EVT_TOGGLEBUTTON( wxID_ZOOM_OUT, SimpleZoomScroll::OnZoomOutClick )
  EVT_CHARTVIEWER_VIEWPORT_CHANGED(ID_CHARTVIEWER, SimpleZoomScroll::OnViewPortChanged)
  EVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA(ID_CHARTVIEWER, SimpleZoomScroll::OnMouseMovePlotArea)

END_EVENT_TABLE()

/*
 * SimpleZoomScroll constructors
 */

SimpleZoomScroll::SimpleZoomScroll()
{
  Init();
}

SimpleZoomScroll::SimpleZoomScroll( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create( parent, id, caption, pos, size, style );
}

/*
 * SimpleZoomScroll creator
 */

bool
SimpleZoomScroll::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
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
 * SimpleZoomScroll destructor
 */

SimpleZoomScroll::~SimpleZoomScroll()
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
SimpleZoomScroll::Init()
{
}

/*
 * Control creation for SimpleZoomScroll
 */

void
SimpleZoomScroll::CreateControls()
{    
  SetIcon(wxICON(mondrian));
  SimpleZoomScroll* itemFrame1 = this;

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

  // Load the data
  LoadData();

  // Initialize the wxChartViewer
  InitChartViewer(m_chartViewer);

  // Trigger the ViewPortChanged event to draw the chart
  m_chartViewer->updateViewPort(true, true);

  m_chartViewer->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemBoxSizer8->Add(m_chartViewer, 1, wxGROW | wxALL, 3);
}

// Load the data
void
SimpleZoomScroll::LoadData()
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
SimpleZoomScroll::InitChartViewer(wxChartViewer* viewer)
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
void
SimpleZoomScroll::OnViewPortChanged(wxCommandEvent& event)
{
  if (m_chartViewer->needUpdateChart())
  {
    DrawChart(m_chartViewer);
  }
  if (m_chartViewer->needUpdateImageMap())
  {
    UpdateImageMap(m_chartViewer);
  }
}

void
SimpleZoomScroll::OnMouseMovePlotArea(wxCommandEvent& event)
{
  m_chartViewer->updateDisplay();
}

// Draw the chart and display it in the given viewer
void
SimpleZoomScroll::DrawChart(wxChartViewer* viewer)
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

  // Create an XYChart object 600 x 300 pixels in size, with pale blue (0xf0f0ff) background,
  // black (000000) rounded border, 1 pixel raised effect.
  XYChart* c = new XYChart(600, 300, 0xf0f0ff, 0, 1);
  //wxColour bgColor = GetBackgroundColour();
  c->setRoundedFrame((m_bgColour.Red() << 16) + (m_bgColour.Green() << 8) + m_bgColour.Blue());

  // Set the plotarea at (52, 60) and of size 520 x 205 pixels. Use white (ffffff) background.
  // Enable both horizontal and vertical grids by setting their colors to grey (cccccc). Set
  // clipping mode to clip the data lines to the plot area.
  c->setPlotArea(52, 60, 520, 205, 0xffffff, -1, -1, 0xcccccc, 0xcccccc);

  // As the data can lie outside the plotarea in a zoomed chart, we need to enable clipping.
  c->setClipping();

  // Add a top title to the chart using 15 pts Times New Roman Bold Italic font, with a light blue
  // (ccccff) background, black (000000) border, and a glass like raised effect.
  c->addTitle("Simple Zooming and Scrolling", "timesbi.ttf", 15)->setBackground(0xccccff, 0x0, Chart::glassEffect());

  // Add a legend box at the top of the plot area with 9pts Arial Bold font with flow layout.
  c->addLegend(50, 33, false, "arialbd.ttf", 9)->setBackground(Chart::Transparent, Chart::Transparent);

  // Set axes width to 2 pixels
  c->yAxis()->setWidth(2);
  c->xAxis()->setWidth(2);

  // Add a title to the y-axis
  c->yAxis()->setTitle("Price (USD)", "arialbd.ttf", 9);

  ///////////////////////////////////////////////////////////////////////////////////////
  // Add data to chart
  ///////////////////////////////////////////////////////////////////////////////////////

  // In this example, we represent the data by lines. You may modify the code below to use other
  // representations (areas, scatter plot, etc).

  // Add a line layer for the lines, using a line width of 2 pixels
  LineLayer* layer = c->addLineLayer();
  layer->setLineWidth(2);

  // In this demo, we do not have too many data points. In real code, the chart may contain a lot
  // of data points when fully zoomed out - much more than the number of horizontal pixels in this
  // plot area. So it is a good idea to use fast line mode.
  layer->setFastLineMode();

  // Now we add the 3 data series to a line layer, using the color red (ff0000), green
  // (00cc00) and blue (0000ff)
  layer->setXData(viewPortTimeStamps);
  layer->addDataSet(viewPortDataSeriesA, 0xff0000, "Product Alpha");
  layer->addDataSet(viewPortDataSeriesB, 0x00cc00, "Product Beta");
  layer->addDataSet(viewPortDataSeriesC, 0x0000ff, "Product Gamma");

  ///////////////////////////////////////////////////////////////////////////////////////
  // Configure axis scale and labelling
  ///////////////////////////////////////////////////////////////////////////////////////

  // Set the x-axis as a date/time axis with the scale according to the view port x range.
  viewer->syncDateAxisWithViewPort("x", c->xAxis());

  // In this demo, we rely on ChartDirector to auto-label the axis. We ask ChartDirector to ensure
  // the x-axis labels are at least 75 pixels apart to avoid too many labels.
  c->xAxis()->setTickDensity(75);

  ///////////////////////////////////////////////////////////////////////////////////////
  // Output the chart
  ///////////////////////////////////////////////////////////////////////////////////////

  delete viewer->getChart();
  viewer->setChart(c);
}

// Update the image map
void
SimpleZoomScroll::UpdateImageMap(wxChartViewer* viewer)
{
  // Include tool tip for the chart
  if (0 == viewer->getImageMapHandler())
  {
    viewer->setImageMap(viewer->getChart()->getHTMLImageMap("", "", "title='[{dataSetName}] {x|mmm dd, yyyy}: USD {value|2}'"));
  }
}

// The Pointer, Zoom In or Zoom out button is pressed
void
SimpleZoomScroll::OnMouseUsageChanged(int mouseUsage)
{
  m_chartViewer->setMouseUsage(mouseUsage);
}

/*
 * Should we show tooltips?
 */

bool
SimpleZoomScroll::ShowToolTips()
{
  return true;
}

/*
 * Get bitmap resources
 */

wxBitmap
SimpleZoomScroll::GetBitmapResource( const wxString& name )
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
SimpleZoomScroll::GetIconResource( const wxString& name )
{
  // Icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
}

/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_POINTER
 */

void
SimpleZoomScroll::OnPointerClick( wxCommandEvent& event )
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
SimpleZoomScroll::OnZoomInClick( wxCommandEvent& event )
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
SimpleZoomScroll::OnZoomOutClick( wxCommandEvent& event )
{
  m_pointerButton->SetValue(false);
  m_zoominButton->SetValue(false);
  m_zoomoutButton->SetValue(true);
  OnMouseUsageChanged(Chart::MouseUsageZoomOut);
}
