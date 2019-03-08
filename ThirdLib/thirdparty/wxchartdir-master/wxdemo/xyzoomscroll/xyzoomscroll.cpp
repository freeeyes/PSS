/*
** Name:        xyzoomscroll.cpp
** Purpose:     XY Zooming and Scrolling
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

#include <wx/mstream.h>
#include <wx/wfstream.h>

#include "xyzoomscroll.h"
#include "hotspotdialog.h"

#include <sstream>

using namespace std;

#include "mondrian.xpm"

/*
 * XyZoomScroll type definition
 */

IMPLEMENT_CLASS( XyZoomScroll, wxFrame )

/*
 * XyZoomScroll event table definition
 */

BEGIN_EVENT_TABLE( XyZoomScroll, wxFrame )

  EVT_TOGGLEBUTTON( ID_POINTER, XyZoomScroll::OnPointerClick )
  EVT_TOGGLEBUTTON( wxID_ZOOM_IN, XyZoomScroll::OnZoomInClick )
  EVT_TOGGLEBUTTON( wxID_ZOOM_OUT, XyZoomScroll::OnZoomOutClick )
  EVT_BUTTON( wxID_SAVE, XyZoomScroll::OnSave )
  EVT_SLIDER( ID_SLIDER, XyZoomScroll::OnSliderUpdated )
  EVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA(ID_CHARTVIEWER, XyZoomScroll::OnMouseMovePlotArea)
  EVT_CHARTVIEWER_VIEWPORT_CHANGED(ID_CHARTVIEWER, XyZoomScroll::OnViewPortChanged)
  EVT_CHARTVIEWER_CLICKED(ID_CHARTVIEWER, XyZoomScroll::OnClickChart)

END_EVENT_TABLE()

/*
 * XyZoomScroll constructors
 */

XyZoomScroll::XyZoomScroll()
{
  Init();
}

XyZoomScroll::XyZoomScroll( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create( parent, id, caption, pos, size, style );
}


/*
 * XyZoomScroll creator
 */

bool
XyZoomScroll::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
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
 * XyZoomScroll destructor
 */

XyZoomScroll::~XyZoomScroll()
{
  if (m_chartViewer->getChart() != NULL)
  {
    delete m_chartViewer->getChart();
  }
  if (m_viewPortControl->getChart() != NULL)
  {
    delete m_viewPortControl->getChart();
  }
}

/*
 * Member initialisation
 */

void
XyZoomScroll::Init()
{
}

/*
 * Control creation for XyZoomScroll
 */

void
XyZoomScroll::CreateControls()
{    
  SetIcon(wxICON(mondrian));
  XyZoomScroll* itemFrame1 = this;

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
#if wxCHECK_VERSION(3,0,0)
  m_pointerButton->SetBitmap(GetBitmapResource("pointer.png"));
  m_pointerButton->SetBitmapMargins(10, 0);
#endif
  // Initially set the mouse to drag to scroll mode
  m_pointerButton->SetValue(true);
  itemStaticBoxSizer1->Add(m_pointerButton, 0, wxGROW|wxALL, 3);

  m_zoominButton = new wxToggleButton( itemPanel2, wxID_ZOOM_IN, _(" Zoom &In"), wxDefaultPosition, wxDefaultSize, wxBU_LEFT);
#if wxCHECK_VERSION(3,0,0)
  m_zoominButton->SetBitmap(GetBitmapResource("zoomin.png"));
  m_zoominButton->SetBitmapMargins(10, 0);
#endif
  m_zoominButton->SetValue(false);
  itemStaticBoxSizer1->Add(m_zoominButton, 0, wxGROW|wxALL, 3);

  m_zoomoutButton = new wxToggleButton( itemPanel2, wxID_ZOOM_OUT, _(" Zoom &Out"), wxDefaultPosition, wxDefaultSize, wxBU_LEFT);
#if wxCHECK_VERSION(3,0,0)
  m_zoomoutButton->SetBitmap(GetBitmapResource("zoomout.png"));
  m_zoomoutButton->SetBitmapMargins(10, 0);
#endif
  m_zoomoutButton->SetValue(false);
  itemStaticBoxSizer1->Add(m_zoomoutButton, 0, wxGROW|wxALL, 3);

  itemStaticBoxSizer1->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

  m_saveButton = new wxButton(itemPanel2, wxID_SAVE, _(" &Save"), wxDefaultPosition, wxDefaultSize, wxBU_LEFT);
#if wxCHECK_VERSION(3,0,0)
  m_saveButton->SetBitmap(GetBitmapResource("save.png"));
  m_saveButton->SetBitmapMargins(10, 0);
#endif
  itemStaticBoxSizer1->Add(m_saveButton, 0, wxGROW | wxALL, 3);

  itemStaticBoxSizer1->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

  wxStaticText* itemStaticText2 = new wxStaticText(itemStaticBoxSizer1->GetStaticBox(), wxID_STATIC, _("Zoom Level"), wxDefaultPosition, wxDefaultSize, 0);
  itemStaticBoxSizer1->Add(itemStaticText2, 0, wxALIGN_CENTER_HORIZONTAL | wxLEFT | wxRIGHT | wxTOP, 3);

  m_zoomBar = new wxSlider(itemStaticBoxSizer1->GetStaticBox(), ID_SLIDER, 100, 1, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_TOP);
  itemStaticBoxSizer1->Add(m_zoomBar, 0, wxALIGN_CENTER_HORIZONTAL | wxLEFT | wxRIGHT | wxBOTTOM, 3);

  itemStaticBoxSizer1->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

  m_viewPortControl = new wxViewPortControl(itemPanel2, ID_VIEWPORT, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxNO_BORDER);
  m_viewPortControl->SetMinSize(wxSize(120, 120));

  // Configure the CViewPortControl to use transparent black for region outside the viewport,
  // that is, to darken the outside region.
  m_viewPortControl->setViewPortExternalColor(0x7f000000);
  m_viewPortControl->setViewPortBorderColor(0x7fffffff);
  m_viewPortControl->setSelectionBorderColor(0x7fffffff);

  m_viewPortControl->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemStaticBoxSizer1->Add(m_viewPortControl, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 0);

  wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
  itemBoxSizer3->Add(itemBoxSizer8, 1, wxGROW|wxALL, 3);

  m_chartViewer = new wxChartViewer(itemPanel2, ID_CHARTVIEWER, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL|wxNO_BORDER);
  m_chartViewer->SetMinSize(wxSize(650, 350));

  // Set initial zoom/scroll direction
  m_chartViewer->setScrollDirection(Chart::DirectionHorizontalVertical);
  m_chartViewer->setZoomDirection(Chart::DirectionHorizontalVertical);

  // Enable mouse wheel zooming by setting the zoom ratio to 1.1 per wheel event
  m_chartViewer->setMouseWheelZoomRatio(1.1);

  // Load the data
  LoadData();

  // Trigger the ViewPortChanged event to draw the chart
  m_chartViewer->updateViewPort(true, true);

  // Draw and display the full chart in the wxViewPortControl
  DrawFullChart(m_viewPortControl, m_chartViewer);

  // Bind the wxChartViewer to the wxViewPortControl
  m_viewPortControl->setViewer(m_chartViewer);

  m_chartViewer->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
  itemBoxSizer8->Add(m_chartViewer, 1, wxGROW | wxALL, 3);
}

// Load the data
void
XyZoomScroll::LoadData()
{
  // For simplicity, in this demo, we just use hard coded data.
  double X0[] = {10, 15, 6, -12, 14, -8, 13, -3, 16, 12, 10.5, -7, 3, -10, -5, 2, 5};
  double Y0[] = {130, 150, 80, 110, -110, -105, -130, -15, -170, 125,  125, 60, 25, 150, 150, 15, 120};
  double X1[] = {6, 7, -4, 3.5, 7, 8, -9, -10, -12, 11, 8, -3, -2, 8, 4, -15, 15};
  double Y1[] = {65, -40, -40, 45, -70, -80, 80, 10, -100, 105, 60, 50, 20, 170, -25, 50, 75};
  double X2[] = {-10, -12, 11, 8, 6, 12, -4, 3.5, 7, 8, -9, 3, -13, 16, -7.5, -10, -15};
  double Y2[] = {65, -80, -40, 45, -70, -80, 80, 90, -100, 105, 60, -75, -150, -40, 120, -50, -30};

  dataX0.assign(X0, X0 + sizeof(X0) / sizeof(*X0));
  dataY0.assign(Y0, Y0 + sizeof(Y0) / sizeof(*Y0));
  dataX1.assign(X1, X1 + sizeof(X1) / sizeof(*X1));
  dataY1.assign(Y1, Y1 + sizeof(Y1) / sizeof(*Y1));
  dataX2.assign(X2, X2 + sizeof(X2) / sizeof(*X2));
  dataY2.assign(Y2, Y2 + sizeof(Y2) / sizeof(*Y2));
}

// The ViewPortChanged event handler. This event occurs if the user scrolls or zooms in
// or out the chart by dragging or clicking on the chart. It can also be triggered by
// calling WinChartViewer.updateViewPort.
void
XyZoomScroll::OnViewPortChanged(wxCommandEvent& event)
{
  // In addition to updating the chart, we may also need to update other controls that
  // changes based on the view port.
  UpdateControls(m_chartViewer);

  // Update chart and image map if necessary
  if (m_chartViewer->needUpdateChart())
  {
    DrawChart(m_chartViewer);
  }
  if (m_chartViewer->needUpdateImageMap())
  {
    UpdateImageMap(m_chartViewer);
  }
}

// Update other controls that may be affected by the viewport.
void
XyZoomScroll::UpdateControls(wxChartViewer* viewer)
{
  // Update the Zoom slider to reflect the current zoom level of the view port
  double smallerSide = viewer->getViewPortWidth() > viewer->getViewPortHeight()
                     ? viewer->getViewPortHeight() : viewer->getViewPortWidth();
  m_zoomBar->SetValue((int)(smallerSide * m_zoomBar->GetMax() + 0.5));
}

// Draw the chart and display it in the given viewer
void
XyZoomScroll::DrawChart(wxChartViewer* viewer)
{
  // Create an XYChart object 500 x 480 pixels in size, with the same background color
  // as the container
  XYChart* c = new XYChart(500, 480, (m_bgColour.Red() << 16) + (m_bgColour.Green() << 8) + m_bgColour.Blue());

  // Set the plotarea at (50, 40) and of size 400 x 400 pixels. Use light grey (c0c0c0)
  // horizontal and vertical grid lines. Set 4 quadrant coloring, where the colors of 
  // the quadrants alternate between lighter and deeper grey (dddddd/eeeeee). 
  c->setPlotArea(50, 40, 400, 400, -1, -1, -1, 0xc0c0c0, 0xc0c0c0)->set4QBgColor(0xdddddd, 0xeeeeee, 0xdddddd, 0xeeeeee, 0x000000);

  // Enable clipping mode to clip the part of the data that is outside the plot area.
  c->setClipping();

  // Set 4 quadrant mode, with both x and y axes symetrical around the origin
  c->setAxisAtOrigin(Chart::XYAxisAtOrigin, Chart::XAxisSymmetric + Chart::YAxisSymmetric);

  // Add a legend box at (450, 40) (top right corner of the chart) with vertical layout
  // and 8 pts Arial Bold font. Set the background color to semi-transparent grey.
  LegendBox* legendBox = c->addLegend(450, 40, true, "arialbd.ttf", 8);
  legendBox->setAlignment(Chart::TopRight);
  legendBox->setBackground(0x40dddddd);

  // Add a titles to axes
  c->xAxis()->setTitle("Alpha Index");
  c->yAxis()->setTitle("Beta Index");

  // Set axes width to 2 pixels
  c->xAxis()->setWidth(2);
  c->yAxis()->setWidth(2);

  // The default ChartDirector settings has a denser y-axis grid spacing and less-dense
  // x-axis grid spacing. In this demo, we want the tick spacing to be symmetrical.
  // We use around 50 pixels between major ticks and 25 pixels between minor ticks.
  c->xAxis()->setTickDensity(50, 25);
  c->yAxis()->setTickDensity(50, 25);

  // In this example, we represent the data by scatter points. If you want to represent
  // the data by somethings else (lines, bars, areas, floating boxes, etc), just modify
  // the code below to use the layer type of your choice. 

  // Add scatter layer, using 11 pixels red (ff33333) X shape symbols
  c->addScatterLayer(vectorToArray(dataX0), vectorToArray(dataY0), "Group A", Chart::Cross2Shape(), 11, 0xff3333);

  // Add scatter layer, using 11 pixels green (33ff33) circle symbols
  c->addScatterLayer(vectorToArray(dataX1), vectorToArray(dataY1), "Group B", Chart::CircleShape, 11,  0x33ff33);

  // Add scatter layer, using 11 pixels blue (3333ff) triangle symbols
  c->addScatterLayer(vectorToArray(dataX2), vectorToArray(dataY2), "Group C", Chart::TriangleSymbol, 11, 0x3333ff);

  // In this example, we have not explicitly configured the full x and y range. In this case, 
  // the first time syncLinearAxisWithViewPort is called, ChartDirector will auto-scale the axis
  // and assume the resulting range is the full range. In subsequent calls, ChartDirector will 
  // set the axis range based on the view port and the full range.
  viewer->syncLinearAxisWithViewPort("x", c->xAxis());
  viewer->syncLinearAxisWithViewPort("y", c->yAxis());
    
  // We need to update the track line too. If the mouse is moving on the chart (eg. if 
  // the user drags the mouse on the chart to scroll it), the track line will be updated
  // in the MouseMovePlotArea event. Otherwise, we need to update the track line here.
  if ((!viewer->isInMouseMoveEvent()) && viewer->isMouseOnPlotArea())
  {
    CrossHair(c, viewer->getPlotAreaMouseX(), viewer->getPlotAreaMouseY());
  }

  // Set the chart image to the wxChartViewer
  delete viewer->getChart();
  viewer->setChart(c);
}

// Draw the full thumbnail chart and display it in the given CViewPortControl
void
XyZoomScroll::DrawFullChart(wxViewPortControl* vpc, wxChartViewer* viewer)
{
  // Create an XYChart object 110 x 110 pixels in size
  XYChart* c = new XYChart(110, 110);

  // Set the plotarea to cover the entire chart. Disable grid lines by setting their colors
  // to transparent. Set 4 quadrant coloring, where the colors of the quadrants alternate
  // between lighter and deeper grey (dddddd/eeeeee).
  c->setPlotArea(0, 0, c->getWidth() - 1, c->getHeight() - 1, -1, -1, -1, Chart::Transparent,
        Chart::Transparent)->set4QBgColor(0xdddddd, 0xeeeeee, 0xdddddd, 0xeeeeee, 0x000000);

  // Set 4 quadrant mode, with both x and y axes symetrical around the origin
  c->setAxisAtOrigin(Chart::XYAxisAtOrigin, Chart::XAxisSymmetric + Chart::YAxisSymmetric);

  // The x and y axis scales reflect the full range of the view port
  c->xAxis()->setLinearScale(viewer->getValueAtViewPort("x", 0), viewer->getValueAtViewPort("x", 1), Chart::NoValue);
  c->yAxis()->setLinearScale(viewer->getValueAtViewPort("y", 0), viewer->getValueAtViewPort("y", 1), Chart::NoValue);

  // Add scatter layer, using 11 pixels red (ff33333) X shape symbols
  c->addScatterLayer(vectorToArray(dataX0), vectorToArray(dataY0), "", Chart::Cross2Shape(), 3, 0xff3333, 0xff3333);

  // Add scatter layer, using 11 pixels green (33ff33) circle symbols
  c->addScatterLayer(vectorToArray(dataX1), vectorToArray(dataY1), "", Chart::CircleShape, 3, 0x33ff33, 0x33ff33);

  // Add scatter layer, using 11 pixels blue (3333ff) triangle symbols
  c->addScatterLayer(vectorToArray(dataX2), vectorToArray(dataY2), "", Chart::TriangleSymbol, 3, 0x3333ff, 0x3333ff);

  // Set the chart image to the WinChartViewer
  vpc->setChart(c);
}

// Update the image map
void
XyZoomScroll::UpdateImageMap(wxChartViewer* viewer)
{
  if (0 == viewer->getImageMapHandler())
  {
    // no existing image map - creates a new one
    viewer->setImageMap(viewer->getChart()->getHTMLImageMap("clickable", "",
            "title='[{dataSetName}] Alpha = {x}, Beta = {value}'"));
  }
}

// User clicks on the wxChartViewer
void
XyZoomScroll::OnClickChart(wxCommandEvent& event)
{
  ImageMapHandler* handler = m_chartViewer->getImageMapHandler();
  if (0 != handler)
  {
    // Query the ImageMapHandler to see if the mouse is on a clickable hot spot. We
    // consider the hot spot as clickable if its href ("path") parameter is not empty.
    const char *path = handler->getValue("path");
    if ((0 != path) && (0 != *path))
    {
      // In this sample code, we just show all hot spot parameters.
      HotspotDialog hs(this);
      hs.SetData(handler);
      hs.ShowModal();
    }
  }
}

// The Pointer, Zoom In or Zoom out button is pressed
void
XyZoomScroll::OnMouseUsageChanged(int mouseUsage)
{
  m_chartViewer->setMouseUsage(mouseUsage);
}

/*
 * Should we show tooltips?
 */

bool
XyZoomScroll::ShowToolTips()
{
  return true;
}

/*
 * Get bitmap resources
 */

wxBitmap
XyZoomScroll::GetBitmapResource( const wxString& name )
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
XyZoomScroll::GetIconResource( const wxString& name )
{
  // Icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
}

/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for ID_POINTER
 */

void
XyZoomScroll::OnPointerClick( wxCommandEvent& event )
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
XyZoomScroll::OnZoomInClick( wxCommandEvent& event )
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
XyZoomScroll::OnZoomOutClick( wxCommandEvent& event )
{
  m_pointerButton->SetValue(false);
  m_zoominButton->SetValue(false);
  m_zoomoutButton->SetValue(true);
  OnMouseUsageChanged(Chart::MouseUsageZoomOut);
}

// The Save button is pressed
void XyZoomScroll::OnSave(wxCommandEvent& event)
{
  wxFileDialog saveFileDialog(this, _("Save graphics file"), wxS(""), wxS("chartdirector_demo"),
      wxS("PNG (*.png)|*.png|JPG (*.jpg)|*.jpg|GIF (*.gif)|*.gif|BMP (*.bmp)|*.bmp|SVG (*.svg)|*.svg|PDF (*.pdf)|*.pdf"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
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

/*
 * wxEVT_COMMAND_SLIDER_UPDATED event handler for ID_SLIDER
 */

void
XyZoomScroll::OnSliderUpdated( wxCommandEvent& event )
{
  int value = m_zoomBar->GetValue();
  OnZoomBarChanged(value);
}

// User moves the Zoom slider control
void
XyZoomScroll::OnZoomBarChanged(int value)
{
  if (!m_chartViewer->isInViewPortChangedEvent())
  {
    // Remember the center point
    double centerX = m_chartViewer->getViewPortLeft() + m_chartViewer->getViewPortWidth() / 2;
    double centerY = m_chartViewer->getViewPortTop() + m_chartViewer->getViewPortHeight() / 2;

    // Aspect ratio and zoom factor
    double aspectRatio = m_chartViewer->getViewPortWidth() / m_chartViewer->getViewPortHeight();
    double zoomTo = ((double)value) / m_zoomBar->GetMax();

    // Zoom by adjusting ViewPortWidth and ViewPortHeight while maintaining the aspect ratio
    m_chartViewer->setViewPortWidth(zoomTo * ((aspectRatio < 1) ? 1 : aspectRatio));
    m_chartViewer->setViewPortHeight(zoomTo * ((aspectRatio > 1) ? 1 : (1 / aspectRatio)));

    // Adjust ViewPortLeft and ViewPortTop to keep center point unchanged
    m_chartViewer->setViewPortLeft(centerX - m_chartViewer->getViewPortWidth() / 2);
    m_chartViewer->setViewPortTop(centerY - m_chartViewer->getViewPortHeight() / 2);

    // Update the chart image only, but no need to update the image map.
    m_chartViewer->updateViewPort(true, false);
  }
}

// Draw track cursor when mouse is moving over plotarea, and update image map if necessary
void
XyZoomScroll::OnMouseMovePlotArea(wxCommandEvent& event)
{
  // Draw crosshair track cursor
  CrossHair((XYChart *)m_chartViewer->getChart(), m_chartViewer->getPlotAreaMouseX(), m_chartViewer->getPlotAreaMouseY());
  m_chartViewer->updateDisplay();

  // Hide the track cursor when the mouse leaves the plot area
  m_chartViewer->removeDynamicLayer(wxS("mouseLeavePlotArea"));

  // Update image map if necessary. If the mouse is still dragging, the chart is still 
  // updating and not confirmed, so there is no need to set up the image map.
  if (!m_chartViewer->isMouseDragging())
  {
    UpdateImageMap(m_chartViewer);
  }
}

// Draw cross hair cursor with axis labels
void
XyZoomScroll::CrossHair(XYChart* c, int mouseX, int mouseY)
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
  ylabel << "<*block,bgColor=FFFFDD,margin=3,edgeColor=000000*>" << c->formatValue(c->getYValue(mouseY, c->yAxis()), "{value|P4}") << "<*/*>";
  TTFText *t = d->text(ylabel.str().c_str(), "arialbd.ttf", 8);
  t->draw(plotArea->getLeftX() - 5, mouseY, 0x000000, Chart::Right);
  t->destroy();

  // Draw x-axis label
  ostringstream xlabel;
  xlabel << "<*block,bgColor=FFFFDD,margin=3,edgeColor=000000*>" << c->formatValue(c->getXValue(mouseX), "{value|P4}") << "<*/*>";
  t = d->text(xlabel.str().c_str(), "arialbd.ttf", 8);
  t->draw(mouseX, plotArea->getBottomY() + 5, 0x000000, Chart::Top);
  t->destroy();
}

// Convert std::vector to a DoubleArray
DoubleArray
XyZoomScroll::vectorToArray(std::vector<double> &v)
{
  return (v.size() == 0) ? DoubleArray() : DoubleArray(&(v[0]), v.size());
}
