/*
** Name:        wxchartviewer.cpp
** Purpose:     wxChartViewer defines
** Author:      Ulrich Telle
** Created:     2018-05-09
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

/// \file wxchartviewer.cpp Implementation of the \b wxChartViewer component

#include "wxchartviewer.h"
#include <time.h>

#include <wx/dcclient.h>
#include <wx/mstream.h>
#include <wx/log.h>
#include <wx/graphics.h>
#include <wx/image.h>

// Utility functions

// ChartDirector uses Chart::chartTime, while wxWidgets uses wxDateTime,
// therefore functions are needed to convert from one to another

// Convert from wxDateTime to chartTime
double
wxChartViewer::ConvertDateTimeToChartTime(wxDateTime dt)
{
  return Chart::chartTime(dt.GetYear(), dt.GetMonth() + 1, dt.GetDay(), dt.GetHour(), dt.GetMinute(), dt.GetSecond()) + dt.GetMillisecond() / 1000.0;
}

// Convert from chartTime to wxDateTime
wxDateTime
wxChartViewer::ConvertChartTimeToDateTime(double ct)
{
  double ymdhms = floor(ct);
  int ms = (int) (floor((ct - ymdhms) * 1000));
  int ymd = Chart::getChartYMD(ymdhms);
  int hms = (int) fmod(ymdhms, 86400);

  wxDateTime date((unsigned short)(ymd % 100), (wxDateTime::Month)((ymd % 10000) / 100 - 1), (unsigned short)(ymd / 10000),
                  (unsigned short)(hms / 3600), (unsigned short)((hms % 3600) / 60), (unsigned short)(hms % 60), (unsigned short)ms);
  return date;
}

/*
** wxChartViewer
*/



#if wxCHECK_VERSION(3,0,0)
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_CLICKED
wxDEFINE_EVENT(wxEVT_CHARTVIEWER_CLICKED, wxCommandEvent);
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_VIEWPORT_CHANGED
wxDEFINE_EVENT(wxEVT_CHARTVIEWER_VIEWPORT_CHANGED, wxCommandEvent);
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_MOUSEMOVE_CHART
wxDEFINE_EVENT(wxEVT_CHARTVIEWER_MOUSEMOVE_CHART, wxCommandEvent);
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_MOUSELEAVE_CHART
wxDEFINE_EVENT(wxEVT_CHARTVIEWER_MOUSELEAVE_CHART, wxCommandEvent);
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA
wxDEFINE_EVENT(wxEVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA, wxCommandEvent);
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_MOUSELEAVE_PLOTAREA
wxDEFINE_EVENT(wxEVT_CHARTVIEWER_MOUSELEAVE_PLOTAREA, wxCommandEvent);
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_MOUSEWHEEL
wxDEFINE_EVENT(wxEVT_CHARTVIEWER_MOUSEWHEEL, wxCommandEvent);
#else
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_CLICKED
DEFINE_EVENT_TYPE(wxEVT_CHARTVIEWER_CLICKED);
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_VIEWPORT_CHANGED
DEFINE_EVENT_TYPE(wxEVT_CHARTVIEWER_VIEWPORT_CHANGED);
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_MOUSEMOVE_CHART
DEFINE_EVENT_TYPE(wxEVT_CHARTVIEWER_MOUSEMOVE_CHART);
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_MOUSELEAVE_CHART
DEFINE_EVENT_TYPE(wxEVT_CHARTVIEWER_MOUSELEAVE_CHART);
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA
DEFINE_EVENT_TYPE(wxEVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA);
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_MOUSELEAVE_PLOTAREA
DEFINE_EVENT_TYPE(wxEVT_CHARTVIEWER_MOUSELEAVE_PLOTAREA);
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_MOUSEWHEEL
DEFINE_EVENT_TYPE(wxEVT_CHARTVIEWER_MOUSEWHEEL);
#endif

// Build in mouse cursors for zooming and scrolling support

static wxCursor& GetZoomInCursor();
static wxCursor& GetZoomOutCursor();
static wxCursor& GetNoZoomCursor();
static wxCursor& GetNoMove2DCursor();
static wxCursor& GetNoMoveHorizCursor();
static wxCursor& GetNoMoveVertCursor();

// Constants used in m_delayChartUpdate
enum { NO_DELAY, NEED_DELAY, NEED_UPDATE };
enum { ID_TIMER_HOLD = 10001, ID_TIMER_DELAY = 10002 };

BEGIN_EVENT_TABLE(wxChartViewer, wxPanel)

  EVT_PAINT(wxChartViewer::OnPaint)
  EVT_MOTION(wxChartViewer::OnMouseMove)
  EVT_LEAVE_WINDOW(wxChartViewer::OnLeaveEvent)
  EVT_LEFT_DOWN(wxChartViewer::OnMousePressEvent)
  EVT_LEFT_UP(wxChartViewer::OnMouseReleaseEvent)
  EVT_MOUSEWHEEL(wxChartViewer::OnWheelEvent)
  EVT_MOUSE_CAPTURE_LOST(wxChartViewer::OnMouseCaptureLost)

  EVT_TIMER(ID_TIMER_HOLD, wxChartViewer::OnHoldTimerEvent)
  EVT_TIMER(ID_TIMER_DELAY, wxChartViewer::OnDelayTimerEvent)

END_EVENT_TABLE()

wxChartViewer::wxChartViewer(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
  : wxPanel(parent, id, pos, size, style, name)
{
  // current chart and hot spot tester
  m_currentChart = 0;
  m_hotSpotTester = 0;

  // initialize chart configuration
  m_selectBoxLineColor = wxColour(0, 0, 0);
  m_selectBoxLineWidth = 2;
#if wxCHECK_VERSION(3,0,0)
  m_selectBoxLineStyle = wxPENSTYLE_SOLID;
#else
  m_selectBoxLineStyle = wxSOLID;
#endif
  m_mouseUsage = Chart::MouseUsageDefault;
  m_zoomDirection = Chart::DirectionHorizontal;
  m_zoomInRatio = 2;
  m_zoomOutRatio = 0.5;
  m_mouseWheelZoomRatio = 1;
  m_scrollDirection = Chart::DirectionHorizontal;
  m_minDragAmount = 5;
  m_updateInterval = 20;

  // current state of the mouse
  m_isOnPlotArea = false;
  m_isPlotAreaMouseDown = false;
  m_isDragScrolling = false;
  m_currentHotSpot = -1;
  m_isClickable = false;
  m_isMouseTracking = false;
  m_isInMouseMove = false;

  // chart update rate support
  m_needUpdateChart = false;
  m_needUpdateImageMap = false;
  m_holdTimerActive = false;
  m_holdTimer.SetOwner(this, ID_TIMER_HOLD);
  m_isInViewPortChanged = false;
  m_delayUpdateChart = NO_DELAY;
  m_delayedChart = 0;
  m_lastMouseMove = 0;
  m_delayedMouseEvent = 0;
  m_delayedMouseEventTimer.SetOwner(this, ID_TIMER_DELAY);
  m_delayImageMapUpdate = false;

  // track cursor support
  m_autoHideMsg = wxString(wxS(""));
  m_currentMouseX = -1;
  m_currentMouseY = -1;
  m_isInMouseMovePlotArea = false;

  // selection rectangle
  m_rectVisible = false;
  m_currentRect = wxRect(0, 0, 0, 0);

  m_vpControl = NULL;
  m_reentrantGuard = false;
}

wxChartViewer::~wxChartViewer()
{
  delete m_hotSpotTester;
  if (m_delayedMouseEvent != NULL)
  {
    delete m_delayedMouseEvent;
  }
}

void
wxChartViewer::OnPaint(wxPaintEvent& evt)
{
  wxPaintDC dc(this);
  if (m_chartBitmap.IsOk())
  {
    dc.DrawBitmap(m_chartBitmap, 0, 0);
    SetRectVisible(m_rectVisible);
  }
}

void
wxChartViewer::PaintNow(BaseChart* c)
{
  wxClientDC dc(this);
//  wxGCDC gdc(dc);
  Render(dc, c);
}

void
wxChartViewer::Render(wxDC& dc, BaseChart* c)
{
  c->setOutputOptions("alpha=3");
  MemBlock m = c->makeChart(Chart::BMP);
  wxMemoryInputStream in(m.data, m.len);
  wxBitmap bmp(wxImage(in, wxBITMAP_TYPE_BMP));
  m_chartBitmap = bmp;
  dc.DrawBitmap(bmp, 0, 0);
  SetRectVisible(m_rectVisible);
}

/////////////////////////////////////////////////////////////////////////////
// wxChartViewer message handlers

// MouseMove event handler
void wxChartViewer::OnMouseMove(wxMouseEvent& event)
{
  if (!event.LeftIsDown() && m_rectVisible)
  {
    SetRectVisible(false);
  }
  // Enable mouse tracking to detect mouse leave events
  m_isMouseTracking = true;

  // TODO
  //emit mouseMove(event);

  // On Windows, mouse events can by-pass the event queue. If there are too many mouse events,
  // the event queue may not get processed, preventing other controls from updating. If two mouse
  // events are less than 10ms apart, there is a risk of too many mouse events. So we repost the
  // mouse event as a timer event that is queued up normally, allowing the queue to get processed.
  unsigned int timeBetweenMouseMove = (((unsigned int)clock()) - m_lastMouseMove) * 1000 / CLOCKS_PER_SEC ;
  if ((m_delayedMouseEvent != NULL && (timeBetweenMouseMove < 250)) || (timeBetweenMouseMove < 10))
  {
    if (m_delayedMouseEvent == NULL)
    {
      m_delayedMouseEventTimer.Start(1);
    }
    else
    {
      delete m_delayedMouseEvent;
    }

    m_delayedMouseEvent = (wxMouseEvent*) event.Clone();
  }
  else
  {
    CommitMouseMove(event);
  }
  OnSetCursor();
  event.Skip();
}

// The method that actually performs MouseMove event processing
void
wxChartViewer::CommitMouseMove(wxMouseEvent& event)
{
  // Remember the mouse coordinates for later use
  m_currentMouseX = event.GetX();
  m_currentMouseY = event.GetY();

  // The chart can be updated more than once during mouse move. For example, it can update due to
  // drag to scroll, and also due to drawing track cursor. So we delay updating the display until
  // all all events has occured.
  m_delayUpdateChart = NEED_DELAY;
  m_isInMouseMove = true;

  // Check if mouse is dragging on the plot area
  m_isOnPlotArea = m_isPlotAreaMouseDown || inPlotArea(event.GetX(), event.GetY());
  if (m_isPlotAreaMouseDown)
  {
    OnPlotAreaMouseDrag(event);
  }

  // Emit mouseMoveChart
  wxCommandEvent mmcEvent(wxEVT_CHARTVIEWER_MOUSEMOVE_CHART);
  mmcEvent.SetId(GetId());
  mmcEvent.SetEventObject(this);
#if wxCHECK_VERSION(3,0,0)
  HandleWindowEvent(mmcEvent);
#else
  GetEventHandler()->ProcessEvent(mmcEvent);
#endif

  if (inExtendedPlotArea(event.GetX(), event.GetY()))
  {
    // Mouse is in extended plot area, emit mouseMovePlotArea
    m_isInMouseMovePlotArea = true;
    wxCommandEvent mmpEvent(wxEVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA);
    mmpEvent.SetId(GetId());
    mmpEvent.SetEventObject(this);
#if wxCHECK_VERSION(3,0,0)
    HandleWindowEvent(mmpEvent);
#else
    GetEventHandler()->ProcessEvent(mmpEvent);
#endif
  }
  else if (m_isInMouseMovePlotArea)
  {
    // Mouse was in extended plot area, but is not in it now, so emit mouseLeavePlotArea
    m_isInMouseMovePlotArea = false;
    wxCommandEvent mlpEvent(wxEVT_CHARTVIEWER_MOUSELEAVE_PLOTAREA);
    mlpEvent.SetId(GetId());
    mlpEvent.SetEventObject(this);
#if wxCHECK_VERSION(3,0,0)
    HandleWindowEvent(mlpEvent);
#else
    GetEventHandler()->ProcessEvent(mlpEvent);
#endif
    ApplyAutoHide(wxS("mouseleaveplotarea"));
  }

  // Show hot spot tool tips if necessary
  if (m_delayImageMapUpdate)
  {
    m_delayImageMapUpdate = false;
    if (!m_isPlotAreaMouseDown)
    {
      updateViewPort(false, true);
    }
  }

  if (event.ButtonIsDown(wxMOUSE_BTN_ANY))
  {
    // Hide tool tips if mouse button is pressed.
    UnsetToolTip();
  }
  else
  {
    // Use the ChartDirector ImageMapHandler to determine if the mouse is over a hot spot
    int hotSpotNo = 0;
    if (0 != m_hotSpotTester)
    {
      hotSpotNo = m_hotSpotTester->getHotSpot(event.GetX(), event.GetY());
    }

    // If the mouse is in the same hot spot since the last mouse move event, there is no need
    // to update the tool tip.
    if (hotSpotNo != m_currentHotSpot)
    {
      // Hot spot has changed - update tool tip text
      m_currentHotSpot = hotSpotNo;

      if (hotSpotNo == 0)
      {
        // Mouse is not actually on handler hot spot - use default tool tip text and reset
        // the clickable flag.
        SetToolTip(m_defaultToolTip);
        m_isClickable = false;
      }
      else
      {
        // Mouse is on a hot spot. In this implementation, we consider the hot spot as
        // clickable if its href ("path") parameter is not empty.
        const char *path = m_hotSpotTester->getValue("path");
        m_isClickable = ((0 != path) && (0 != *path));
        SetToolTip(wxString::FromUTF8(m_hotSpotTester->getValue("title")));
      }
    }
  }

  // Cancel the delayed mouse event if any
  if (m_delayedMouseEvent != NULL)
  {
    m_delayedMouseEventTimer.Stop();
    delete m_delayedMouseEvent;
    m_delayedMouseEvent = NULL;
  }

  // Can update chart now
  CommitUpdateChart();
  m_isInMouseMove = false;

  m_lastMouseMove = (unsigned int) clock();
}

// Delayed MouseMove event handler
void
wxChartViewer::OnDelayedMouseMove()
{
  if (m_delayedMouseEvent != NULL)
  {
    CommitMouseMove(*m_delayedMouseEvent);
  }
}

void
wxChartViewer::OnLeaveEvent(wxMouseEvent& event)
{
  // Process delayed mouse move, if any
  OnDelayedMouseMove();

  // Mouse tracking is no longer active
  m_isMouseTracking = false;

  if (m_isInMouseMovePlotArea)
  {
    // Mouse was in extended plot area, but is not in it now, so emit mouseLeavePlotArea
    m_isInMouseMovePlotArea = false;
    wxCommandEvent mlpEvent(wxEVT_CHARTVIEWER_MOUSELEAVE_PLOTAREA);
    mlpEvent.SetId(GetId());
    mlpEvent.SetEventObject(this);
#if wxCHECK_VERSION(3,0,0)
    HandleWindowEvent(mlpEvent);
#else
    GetEventHandler()->ProcessEvent(mlpEvent);
#endif
    ApplyAutoHide(wxS("mouseleaveplotarea"));
  }

  // emit mouseLeaveChart
  wxCommandEvent mlcEvent(wxEVT_CHARTVIEWER_MOUSELEAVE_CHART);
  mlcEvent.SetId(GetId());
  mlcEvent.SetEventObject(this);
#if wxCHECK_VERSION(3,0,0)
  HandleWindowEvent(mlcEvent);
#else
  GetEventHandler()->ProcessEvent(mlcEvent);
#endif
  ApplyAutoHide(wxS("mouseleavechart"));
  event.Skip();
}

// MouseWheel handler
void
wxChartViewer::OnWheelEvent(wxMouseEvent& event)
{
  // Process delayed mouse move, if any
  OnDelayedMouseMove();

  // emit mouseWheel event
  wxCommandEvent mwEvent(wxEVT_CHARTVIEWER_MOUSEWHEEL);
  mwEvent.SetId(GetId());
  mwEvent.SetEventObject(this);
  mwEvent.SetInt(event.GetWheelRotation());
#if wxCHECK_VERSION(3,0,0)
  HandleWindowEvent(mwEvent);
#else
  GetEventHandler()->ProcessEvent(mwEvent);
#endif

  // Process the mouse wheel only if the mouse is over the plot area
  bool hasMouseWheelZoom = isMouseOnPlotArea() &&
                           onMouseWheelZoom(getPlotAreaMouseX(), getPlotAreaMouseY(), event.GetWheelRotation());
  event.Skip();
}

bool
wxChartViewer::onMouseWheelZoom(int x, int y, int zDelta)
{
  // Zoom ratio = 1 means no zooming
  if (m_mouseWheelZoomRatio == 1)
  {
    return false;
  }

  // X and Y zoom ratios
  double rx = 1;
  double ry = 1;
  if (getZoomDirection() != Chart::DirectionVertical)
  {
    rx = (zDelta > 0) ? m_mouseWheelZoomRatio : 1 / m_mouseWheelZoomRatio;
  }
  if (getZoomDirection() != Chart::DirectionHorizontal)
  {
    ry = (zDelta > 0) ? m_mouseWheelZoomRatio : 1 / m_mouseWheelZoomRatio;
  }

  // Do the zooming
  if (zoomAround(x, y, rx, ry))
  {
    updateViewPort(true, true);
  }

  return true;
}

// Change the mouse cursor.
void
wxChartViewer::OnSetCursor()
{
  if (m_isDragScrolling)
  {
    switch (m_scrollDirection)
    {
      case Chart::DirectionHorizontal:
        SetCursor(GetNoMoveHorizCursor());
        break;
      case Chart::DirectionVertical:
        SetCursor(GetNoMoveVertCursor());
        break;
      default :
        SetCursor(GetNoMove2DCursor());
        break;
    }

    return;
  }

  if (m_isOnPlotArea)
  {
    switch (m_mouseUsage)
    {
      case Chart::MouseUsageZoomIn:
        if (canZoomIn(m_zoomDirection))
        {
          SetCursor(GetZoomInCursor());
        }
        else
        {
          SetCursor(GetNoZoomCursor());
        }
        return;
      case Chart::MouseUsageZoomOut:
        if (canZoomOut(m_zoomDirection))
        {
          SetCursor(GetZoomOutCursor());
        }
        else
        {
          SetCursor(GetNoZoomCursor());
        }
        return;
      }
  }

  if (m_isClickable)
  {
    SetCursor(wxCursor(wxCURSOR_HAND));
  }
  else
  {
    SetCursor(wxCursor(wxCURSOR_ARROW));
  }
}

// Mouse capture lost event handler
void
wxChartViewer::OnMouseCaptureLost(wxMouseCaptureLostEvent& event)
{
  // TODO: Any action required?
  // wxLogDebug("*** OnMouseCaptureLost ***");
}

// Mouse button down event.
void
wxChartViewer::OnMousePressEvent(wxMouseEvent& event)
{
  OnDelayedMouseMove();

  if (inPlotArea(event.GetX(), event.GetY()) && (m_mouseUsage != Chart::MouseUsageDefault))
  {
    // Mouse usage is for drag to zoom/scroll. Capture the mouse to prepare for dragging and
    // save the mouse down position to draw the selection rectangle.
    if (!HasCapture())
    {
      CaptureMouse();
    }
    m_isPlotAreaMouseDown = true;
    m_plotAreaMouseDownXPos = event.GetX();
    m_plotAreaMouseDownYPos = event.GetY();
    startDrag();
  }
  event.Skip();
}

// Mouse button up event.
void
wxChartViewer::OnMouseReleaseEvent(wxMouseEvent& event)
{
  OnDelayedMouseMove();
  if (m_isPlotAreaMouseDown)
  {
    // Release the mouse capture.
    if (HasCapture())
    {
      ReleaseMouse();
    }
    m_isPlotAreaMouseDown = false;
    SetRectVisible(false);
    bool hasUpdate = false;

    switch (m_mouseUsage)
    {
      case Chart::MouseUsageZoomIn:
        if (canZoomIn(m_zoomDirection))
        {
          if (IsDrag(m_zoomDirection, event))
          {
            // Zoom to the drag selection rectangle.
            hasUpdate = zoomTo(m_zoomDirection, m_plotAreaMouseDownXPos, m_plotAreaMouseDownYPos, event.GetX(), event.GetY());
          }
          else
          {
            // User just click on a point. Zoom-in around the mouse cursor position.
            hasUpdate = zoomAt(m_zoomDirection, event.GetX(), event.GetY(), m_zoomInRatio);
          }
        }
        break;
      case Chart::MouseUsageZoomOut:
        // Zoom out around the mouse cursor position.
        if (canZoomOut(m_zoomDirection))
        {
          hasUpdate = zoomAt(m_zoomDirection, event.GetX(), event.GetY(), m_zoomOutRatio);
        }
        break;
      default :
        if (m_isDragScrolling)
        {
          // Drag to scroll. We can update the image map now as scrolling has finished.
          updateViewPort(false, true);
        }
        else
        {
          // Is not zooming or scrolling, so is just a normal click event.
          wxCommandEvent clickEvent(wxEVT_CHARTVIEWER_CLICKED);
          clickEvent.SetId(GetId());
          clickEvent.SetEventObject(this);
#if wxCHECK_VERSION(3,0,0)
          HandleWindowEvent(clickEvent);
#else
          GetEventHandler()->ProcessEvent(clickEvent);
#endif
        }
        break;
    }

    m_isDragScrolling = false;
    if (hasUpdate)
    {
      // View port has changed - update it.
      updateViewPort(true, true);
    }
  }
  else
  {
    if (m_rectVisible)
    {
      SetRectVisible(false);
    }
    wxCommandEvent clickEvent(wxEVT_CHARTVIEWER_CLICKED);
    clickEvent.SetId(GetId());
    clickEvent.SetEventObject(this);
#if wxCHECK_VERSION(3,0,0)
    HandleWindowEvent(clickEvent);
#else
    GetEventHandler()->ProcessEvent(clickEvent);
#endif
  }

  OnSetCursor();
  event.Skip();
}

// Chart hold timer.
void
wxChartViewer::OnHoldTimerEvent(wxTimerEvent& event)
{
  if (m_holdTimerActive)
  {
    m_holdTimer.Stop();
    m_holdTimerActive = false;

    // If has pending chart update request, handles them now.
    if (m_needUpdateChart || m_needUpdateImageMap)
    {
      updateViewPort(m_needUpdateChart, m_needUpdateImageMap);
    }
  }
}

// Chart hold timer.
void
wxChartViewer::OnDelayTimerEvent(wxTimerEvent& event)
{
  if (m_delayedMouseEvent != NULL)
  {
    // Is a delayed mouse move event
    OnDelayedMouseMove();
  }
}

////////////////////////////////////////////////////////////////////////
// wxChartViewer properties

// Set the chart to the control
void
wxChartViewer::setChart(BaseChart* c)
{
  m_currentChart = c;
  setImageMap(0);

  if (0 != c)
  {
    commitPendingSyncAxis(c);
    if (m_delayUpdateChart != NO_DELAY)
    {
      c->makeChart();
    }
  }

  updateDisplay();
}

// Get back the same BaseChart pointer provided by the previous setChart call.
BaseChart*
wxChartViewer::getChart()
{
  return m_currentChart;
}

// Set the wxViewPortControl to be associated with this control
void
wxChartViewer::setViewPortControl(wxViewPortControl* vpc)
{
  if (m_reentrantGuard)
  {
    return;
  }
  m_reentrantGuard = true;

  if (0 != m_vpControl)
  {
    m_vpControl->setViewer(0);
  }
  m_vpControl = vpc;
  if (0 != m_vpControl)
  {
    m_vpControl->setViewer(this);
  }

  m_reentrantGuard = false;
}


// Get the wxViewPortControl that is associated with this control
wxViewPortControl*
wxChartViewer::getViewPortControl()
{
  return m_vpControl;
}

// Set image map used by the chart
void
wxChartViewer::setImageMap(const char* imageMap)
{
  // Delete the existing ImageMapHandler
  if (0 != m_hotSpotTester)
  {
    delete m_hotSpotTester;
  }
  m_currentHotSpot = -1;
  m_isClickable = false;

  // Create a new ImageMapHandler to represent the image map
  if ((0 == imageMap) || (0 == *imageMap))
  {
    m_hotSpotTester = 0;
  }
  else
  {
    m_hotSpotTester = new ImageMapHandler(imageMap);
  }
}

// Get the image map handler for the chart
ImageMapHandler*
wxChartViewer::getImageMapHandler()
{
  return m_hotSpotTester;
}

// Set the default tool tip to use
void
wxChartViewer::setDefaultToolTip(const wxString& text)
{
  m_defaultToolTip = text;
}

// Set the border width of the selection box
void
wxChartViewer::setSelectionBorderWidth(int width)
{
  m_selectBoxLineWidth = width;
}

// Get the border with of the selection box.
int
wxChartViewer::getSelectionBorderWidth()
{
  return m_selectBoxLineWidth;
}

// Set the border color of the selection box
void
wxChartViewer::setSelectionBorderColor(const wxColour& color)
{
  m_selectBoxLineColor = color;
}

// Get the border color of the selection box.
wxColour
wxChartViewer::getSelectionBorderColor()
{
  return m_selectBoxLineColor;
}

#if wxCHECK_VERSION(3,0,0)
// Set the border style of the selection box
void
wxChartViewer::setSelectionBorderStyle(wxPenStyle style)
{
  m_selectBoxLineStyle = style;
}

// Get the border style of the selection box
wxPenStyle
wxChartViewer::getSelectionBorderStyle()
{
  return m_selectBoxLineStyle;
}
#else
// Set the border style of the selection box
void
wxChartViewer::setSelectionBorderStyle(int style)
{
  m_selectBoxLineStyle = style;
}

// Get the border style of the selection box
int
wxChartViewer::getSelectionBorderStyle()
{
  return m_selectBoxLineStyle;
}
#endif

// Set the mouse usage mode
void
wxChartViewer::setMouseUsage(int mouseUsage)
{
  m_mouseUsage = mouseUsage;
}

// Get the mouse usage mode
int
wxChartViewer::getMouseUsage()
{
  return m_mouseUsage;
}

// Set the zoom direction
void
wxChartViewer::setZoomDirection(int direction)
{
  m_zoomDirection = direction;
}

// Get the zoom direction
int
wxChartViewer::getZoomDirection()
{
  return m_zoomDirection;
}

// Set the scroll direction
void
wxChartViewer::setScrollDirection(int direction)
{
  m_scrollDirection = direction;
}

// Get the scroll direction
int
wxChartViewer::getScrollDirection()
{
  return m_scrollDirection;
}

// Set the zoom-in ratio for mouse click zoom-in
void
wxChartViewer::setZoomInRatio(double ratio)
{
  m_zoomInRatio = ratio;
}

// Get the zoom-in ratio for mouse click zoom-in
double
wxChartViewer::getZoomInRatio()
{
  return m_zoomInRatio;
}

// Set the zoom-out ratio
void
wxChartViewer::setZoomOutRatio(double ratio)
{
  m_zoomOutRatio = ratio;
}

// Get the zoom-out ratio
double
wxChartViewer::getZoomOutRatio()
{
  return m_zoomOutRatio;
}

// Set the mouse wheel zoom ratio 
void
wxChartViewer::setMouseWheelZoomRatio(double ratio)
{
	m_mouseWheelZoomRatio = ratio;
}

// Get the mouse wheel zoom ratio 
double
wxChartViewer::getMouseWheelZoomRatio()
{
	return m_mouseWheelZoomRatio;	
}

// Set the minimum mouse drag before the dragging is considered as real. This is to avoid small
// mouse vibrations triggering a mouse drag.
void
wxChartViewer::setMinimumDrag(int offset)
{
  m_minDragAmount = offset;
}

// Get the minimum mouse drag before the dragging is considered as real.
int
wxChartViewer::getMinimumDrag()
{
  return m_minDragAmount;
}

// Set the minimum interval between ViewPortChanged events. This is to avoid the chart being
// updated too frequently. (Default is 20ms between chart updates.) Multiple update events
// arrived during the interval will be merged into one chart update and executed at the end
// of the interval.
void
wxChartViewer::setUpdateInterval(int interval)
{
  m_updateInterval = interval;
}

// Get the minimum interval between ViewPortChanged events.
int
wxChartViewer::getUpdateInterval()
{
  return m_updateInterval;
}

// Check if there is a pending chart update request.
bool
wxChartViewer::needUpdateChart()
{
  return m_needUpdateChart;
}

// Check if there is a pending image map update request.
bool
wxChartViewer::needUpdateImageMap()
{
  return m_needUpdateImageMap;
}

// Get the current mouse x coordinate when used in a mouse move event handler
int
wxChartViewer::getChartMouseX()
{
  int ret = m_currentMouseX;
  if (ret < 0)
  {
    ret = getPlotAreaLeft() + getPlotAreaWidth();
  }
  return ret;
}

// Get the current mouse y coordinate when used in a mouse move event handler
int
wxChartViewer::getChartMouseY()
{
  int ret = m_currentMouseY;
  if (ret < 0)
  {
    ret = getPlotAreaTop() + getPlotAreaHeight();
  }
  return ret;
}

// Get the current mouse x coordinate bounded to the plot area when used in a mouse event handler
int
wxChartViewer::getPlotAreaMouseX()
{
  int ret = getChartMouseX();
  if (ret < getPlotAreaLeft())
  {
    ret = getPlotAreaLeft();
  }
  if (ret > getPlotAreaLeft() + getPlotAreaWidth())
  {
    ret = getPlotAreaLeft() + getPlotAreaWidth();
  }
  return ret;
}

// Get the current mouse y coordinate bounded to the plot area when used in a mouse event handler
int
wxChartViewer::getPlotAreaMouseY()
{
  int ret = getChartMouseY();
  if (ret < getPlotAreaTop())
  {
    ret = getPlotAreaTop();
  }
  if (ret > getPlotAreaTop() + getPlotAreaHeight())
  {
    ret = getPlotAreaTop() + getPlotAreaHeight();
  }
  return ret;
}

// Check if mouse is on the extended plot area
bool
wxChartViewer::isMouseOnPlotArea()
{
  return ((m_isMouseTracking) ? inExtendedPlotArea(getChartMouseX(), getChartMouseY()) : false);
}

// Check if mouse is dragging to scroll or to select the zoom rectangle
bool
wxChartViewer::isMouseDragging()
{
  return m_isPlotAreaMouseDown;
}

// Check if is currently processing a mouse move event
bool
wxChartViewer::isInMouseMoveEvent()
{
  return m_isInMouseMove;
}

// Check if is currently processing a view port changed event
bool
wxChartViewer::isInViewPortChangedEvent()
{
  return m_isInViewPortChanged;
}

////////////////////////////////////////////////////////////////////////
// wxChartViewer methods

// Update the display
void
wxChartViewer::updateDisplay()
{
  if (m_delayUpdateChart == NO_DELAY)
  {
    CommitUpdateChart();
  }
  else
  {
    m_delayUpdateChart = NEED_UPDATE;
    delete m_delayedChart;
    m_delayedChart = (0 != m_currentChart) ? new BaseChart(m_currentChart) : 0;
  }
}

// Commit chart to display
void
wxChartViewer::CommitUpdateChart()
{
  if (m_delayUpdateChart == NEED_DELAY)
  {
    // No actual update occur
    m_delayUpdateChart = NO_DELAY;
    return;
  }

  // Get the image and metrics for the chart
  BaseChart* c = (m_delayUpdateChart == NEED_UPDATE) ? m_delayedChart : m_currentChart;
  const char* chartMetrics = 0;

  if (NULL != c)
  {
    PaintNow(c);
    SetMinSize(wxSize(c->getDrawArea()->getWidth(), c->getDrawArea()->getHeight()));

    // Get chart metrics
    chartMetrics = c->getChartMetrics();
  }

  // Set the chart metrics and clear the image map
  setChartMetrics(chartMetrics);

  // Any delayed chart has been committed
  m_delayUpdateChart = NO_DELAY;
  delete m_delayedChart;
  m_delayedChart = 0;
}

// Set the message used to remove the dynamic layer
void
wxChartViewer::removeDynamicLayer(const wxString& msg)
{
  m_autoHideMsg = msg.Lower();
  if (m_autoHideMsg.IsSameAs(wxS("now")))
  {
    ApplyAutoHide(msg);
  }
}

// Attempt to hide the dynamic layer using the specified message
void
wxChartViewer::ApplyAutoHide(const wxString& msg)
{
  if (m_autoHideMsg == msg)
  {
    if (0 != m_currentChart)
    {
      m_currentChart->removeDynamicLayer();
    }
    m_autoHideMsg = wxEmptyString;
    updateDisplay();
  }
}

// Set selection rectangle position and size
void
wxChartViewer::SetSelectionRect(int x, int y, int width, int height)
{
  // width < 0 is interpreted as the rectangle extends to the left or x.
  // height < 0 is interpreted as the rectangle extends to above y.
  if (width < 0)
  {
    x -= (width = -width);
  }
  if (height < 0)
  {
    y -= (height = -height);
  }
  m_currentRect = wxRect(x, y, width-1, height-1);
}

// Show/hide selection rectangle
void
wxChartViewer::SetRectVisible(bool b)
{
  m_rectVisible = b;

  if (m_currentRect.GetWidth() > 0 && m_currentRect.GetHeight() > 0)
  {
    wxClientDC dc(this);
    PrepareDC(dc);
    wxDCOverlay overlaydc(m_overlay, &dc);
    overlaydc.Clear();
    if (b)
    {
      wxRect rect(m_currentRect.x, m_currentRect.y, m_currentRect.width, m_currentRect.height);
      wxPen pen(m_selectBoxLineColor, m_selectBoxLineWidth, m_selectBoxLineStyle);
      pen.SetCap(wxCAP_PROJECTING);

      dc.SetPen(pen);
      dc.SetBrush(*wxTRANSPARENT_BRUSH);
      dc.DrawRectangle(rect);
    }
  }

  if (!b)
  {
    m_overlay.Reset();
    m_currentRect = wxRect(0, 0, 0, 0);
  }
}

// Determines if the mouse is dragging.
bool
wxChartViewer::IsDrag(int direction, wxMouseEvent& event)
{
  // We only consider the mouse is dragging it is has dragged more than m_minDragAmount. This is
  // to avoid small mouse vibrations triggering a mouse drag.
  int spanX = abs(event.GetX() - m_plotAreaMouseDownXPos);
  int spanY = abs(event.GetY() - m_plotAreaMouseDownYPos);
  return ((direction != Chart::DirectionVertical) && (spanX >= m_minDragAmount)) ||
         ((direction != Chart::DirectionHorizontal) && (spanY >= m_minDragAmount));
}

// Process mouse dragging over the plot area
void
wxChartViewer::OnPlotAreaMouseDrag(wxMouseEvent& event)
{
  switch (m_mouseUsage)
  {
    case Chart::MouseUsageZoomIn:
    {
      // Mouse is used for zoom in. Draw the selection rectangle if necessary.
      bool isDragZoom = event.LeftIsDown() && canZoomIn(m_zoomDirection) && IsDrag(m_zoomDirection, event);
      if (isDragZoom)
      {
        int spanX = m_plotAreaMouseDownXPos - event.GetX();
        int spanY = m_plotAreaMouseDownYPos - event.GetY();
        switch (m_zoomDirection)
        {
          case Chart::DirectionHorizontal:
            SetSelectionRect(event.GetX(), getPlotAreaTop(), spanX, getPlotAreaHeight());
            break;
          case Chart::DirectionVertical:
            SetSelectionRect(getPlotAreaLeft(), event.GetY(), getPlotAreaWidth(), spanY);
            break;
          default:
            SetSelectionRect(event.GetX(), event.GetY(), spanX, spanY);
            break;
        }
      }
      SetRectVisible(isDragZoom);
      break;
    }
    case Chart::MouseUsageScroll:
    {
      // Mouse is used for drag scrolling. Scroll and update the view port.
      if (m_isDragScrolling || IsDrag(m_scrollDirection, event))
      {
        m_isDragScrolling = true;
        if (dragTo(m_scrollDirection, event.GetX() - m_plotAreaMouseDownXPos, event.GetY() - m_plotAreaMouseDownYPos))
        {
          updateViewPort(true, false);
        }
      }
    }
  }
}

// Trigger the ViewPortChanged event
void
wxChartViewer::updateViewPort(bool needUpdateChart, bool needUpdateImageMap)
{
  // Already updating, no need to update again
  if (m_isInViewPortChanged)
  {
    return;
  }

  // Merge the current update requests with any pending requests.
  m_needUpdateChart = m_needUpdateChart || needUpdateChart;
  m_needUpdateImageMap = needUpdateImageMap;

  // Hold timer has not expired, so do not update chart immediately. Keep the requests pending.
  if (m_holdTimerActive)
  {
    return;
  }

  // The chart can be updated more than once during mouse move. For example, it can update due to
  // drag to scroll, and also due to drawing track cursor. So we delay updating the display until
  // all all updates has occured.
  int hasDelayUpdate = (m_delayUpdateChart != NO_DELAY);
  if (!hasDelayUpdate)
  {
    m_delayUpdateChart = NEED_DELAY;
  }

  // Can trigger the ViewPortChanged event.
  validateViewPort();
  m_isInViewPortChanged = true;
  wxCommandEvent vpcEvent(wxEVT_CHARTVIEWER_VIEWPORT_CHANGED);
  vpcEvent.SetId(GetId());
  vpcEvent.SetEventObject(this);
#if wxCHECK_VERSION(3,0,0)
  HandleWindowEvent(vpcEvent);
#else
  GetEventHandler()->ProcessEvent(vpcEvent);
#endif

  m_isInViewPortChanged = false;

  if (m_needUpdateChart && (0 != m_vpControl))
  {
    m_vpControl->updateDisplay();
  }

  // Can update chart now
  if (!hasDelayUpdate)
  {
    CommitUpdateChart();
  }

  // Clear any pending updates.
  m_needUpdateChart = false;
  m_needUpdateImageMap = false;

  // Set hold timer to prevent multiple chart updates within a short period.
  if (m_updateInterval > 0)
  {
    m_holdTimerActive = true;
    m_holdTimer.Start(m_updateInterval);
  }
}

////////////////////////////////////////////////////////////////////////
// Build in mouse cursors for zooming and scrolling support

static const unsigned int zoomInCursorA[] =
{
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xff3ff8ff,
0xff0fe0ff,
0xff07c0ff,
0xff07c0ff,
0xff0380ff,
0xff0380ff,
0xff0380ff,
0xff0380ff,
0xff0380ff,
0xff07c0ff,
0xff07c0ff,
0xff01e0ff,
0xff30f8ff,
0x7ff0ffff,
0x3ff8ffff,
0x1ffcffff,
0x0ffeffff,
0x0fffffff,
0x9fffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff
};

static const unsigned int zoomInCursorB[] =
{
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00c00700,
0x00f01f00,
0x00f01e00,
0x00f83e00,
0x00f83e00,
0x00183000,
0x00f83e00,
0x00f83e00,
0x00f01e00,
0x00f01f00,
0x00c00700,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000
};

static const unsigned int zoomOutCursorA[] =
{
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xff3ff8ff,
0xff0fe0ff,
0xff07c0ff,
0xff07c0ff,
0xff0380ff,
0xff0380ff,
0xff0380ff,
0xff0380ff,
0xff0380ff,
0xff07c0ff,
0xff07c0ff,
0xff01e0ff,
0xff30f8ff,
0x7ff0ffff,
0x3ff8ffff,
0x1ffcffff,
0x0ffeffff,
0x0fffffff,
0x9fffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff
};

static const unsigned int zoomOutCursorB[] =
{
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00c00700,
0x00f01f00,
0x00f01f00,
0x00f83f00,
0x00f83f00,
0x00183000,
0x00f83f00,
0x00f83f00,
0x00f01f00,
0x00f01f00,
0x00c00700,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000
};

static const unsigned int noZoomCursorA[] =
{
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xff3ff8ff,
0xff0fe0ff,
0xff07c0ff,
0xff07c0ff,
0xff0380ff,
0xff0380ff,
0xff0380ff,
0xff0380ff,
0xff0380ff,
0xff07c0ff,
0xff07c0ff,
0xff01e0ff,
0xff30f8ff,
0x7ff0ffff,
0x3ff8ffff,
0x1ffcffff,
0x0ffeffff,
0x0fffffff,
0x9fffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff
};

static const unsigned int noZoomCursorB[] =
{
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00c00700,
0x00f01f00,
0x00f01f00,
0x00f83f00,
0x00f83f00,
0x00f83f00,
0x00f83f00,
0x00f83f00,
0x00f01f00,
0x00f01f00,
0x00c00700,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000
};

static const unsigned int noMove2DCursorA[] =
{
0xffffffff,
0xffffffff,
0xffffffff,
0xff7ffeff,
0xff3ffcff,
0xff1ff8ff,
0xff0ff0ff,
0xff07e0ff,
0xff03c0ff,
0xff03c0ff,
0xfffc3fff,
0x7ffc3ffe,
0x3ffc3ffc,
0x1f7c3ef8,
0x0f3c3cf0,
0x071c38e0,
0x071c38e0,
0x0f3c3cf0,
0x1f7c3ef8,
0x3ffc3ffc,
0x7ffc3ffe,
0xfffc3fff,
0xff03c0ff,
0xff03c0ff,
0xff07e0ff,
0xff0ff0ff,
0xff1ff8ff,
0xff3ffcff,
0xff7ffeff,
0xffffffff,
0xffffffff,
0xffffffff
};

static const unsigned int noMove2DCursorB[] =
{
0x00000000,
0x00000000,
0x00000000,
0x00800100,
0x00400200,
0x00200400,
0x00100800,
0x00081000,
0x00042000,
0x00fc3f00,
0x0003c000,
0x80024001,
0x40024002,
0x20824104,
0x10424208,
0x08224410,
0x08224410,
0x10424208,
0x20824104,
0x40024002,
0x80024001,
0x0003c000,
0x00fc3f00,
0x00042000,
0x00081000,
0x00100800,
0x00200400,
0x00400200,
0x00800100,
0x00000000,
0x00000000,
0x00000000
};

static const unsigned int noMoveHorizCursorA[] =
{
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xfffc3fff,
0x7ffc3ffe,
0x3ffc3ffc,
0x1f7c3ef8,
0x0f3c3cf0,
0x071c38e0,
0x071c38e0,
0x0f3c3cf0,
0x1f7c3ef8,
0x3ffc3ffc,
0x7ffc3ffe,
0xfffc3fff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff,
0xffffffff
};

static const unsigned int noMoveHorizCursorB[] =
{
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x0003c000,
0x80024001,
0x40024002,
0x20824104,
0x10424208,
0x08224410,
0x08224410,
0x10424208,
0x20824104,
0x40024002,
0x80024001,
0x0003c000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000,
0x00000000
};

static const unsigned int noMoveVertCursorA[] =
{
0xffffffff,
0xffffffff,
0xffffffff,
0xff7ffeff,
0xff3ffcff,
0xff1ff8ff,
0xff0ff0ff,
0xff07e0ff,
0xff03c0ff,
0xff03c0ff,
0xffffffff,
0xffffffff,
0xffffffff,
0xff7ffeff,
0xff3ffcff,
0xff1ff8ff,
0xff1ff8ff,
0xff3ffcff,
0xff7ffeff,
0xffffffff,
0xffffffff,
0xffffffff,
0xff03c0ff,
0xff03c0ff,
0xff07e0ff,
0xff0ff0ff,
0xff1ff8ff,
0xff3ffcff,
0xff7ffeff,
0xffffffff,
0xffffffff,
0xffffffff
};

static const unsigned int noMoveVertCursorB[] =
{
0x00000000,
0x00000000,
0x00000000,
0x00800100,
0x00400200,
0x00200400,
0x00100800,
0x00081000,
0x00042000,
0x00fc3f00,
0x00000000,
0x00000000,
0x00000000,
0x00800100,
0x00400200,
0x00200400,
0x00200400,
0x00400200,
0x00800100,
0x00000000,
0x00000000,
0x00000000,
0x00fc3f00,
0x00042000,
0x00081000,
0x00100800,
0x00200400,
0x00400200,
0x00800100,
0x00000000,
0x00000000,
0x00000000
};

static wxCursor* hZoomInCursor = 0;
static wxCursor* hZoomOutCursor = 0;
static wxCursor* hNoZoomCursor = 0;
static wxCursor* hNoMove2DCursor = 0;
static wxCursor* hNoMoveHorizCursor = 0;
static wxCursor* hNoMoveVertCursor = 0;

/// Internal class to destruct static cursors
class FreeCursors
{
public:
  ~FreeCursors()
  {
    delete hZoomInCursor;
    delete hZoomOutCursor;
    delete hNoZoomCursor;
    delete hNoMove2DCursor;
    delete hNoMoveHorizCursor;
    delete hNoMoveVertCursor;
  }
} dummyFreeCursorObj; ///< Dummy object for destructing static cursors

// For monomchrome bitmaps wxWidgets expects bit patterns in XBM format.
// Therefore the bit order has to be reversed.
static void
reverseBitOrder(unsigned int& value)
{
  unsigned char* buffer= (unsigned char*) &value;
  for (int j = 0; j < 4; ++j)
  {
    unsigned char val = buffer[j];
    unsigned char reversed = 0;

    for (int bit = 0; bit < 8; bit++)
    {
      reversed <<= 1;
      reversed |= (unsigned char)(val & 0x01);
      val >>= 1;
    }
    buffer[j] = reversed;
  }
}

static wxCursor*
createCursor(const unsigned int* andPlane, const unsigned int* orPlane, int hotX, int hotY)
{
  // The cursor bitmaps are in and/xor plane formats used in the Win32 CreateCursor
  // function. We need to change it to the bitmap/mask format used by wxWidgets.
  unsigned int buffer[32];
  for (int i = 0; i < 32; ++i)
  {
    buffer[i] = (~orPlane[i] & ~andPlane[i]);
    reverseBitOrder(buffer[i]);
  }
  wxBitmap csrBits((char*) buffer, 32, 32);
  for (int i = 0; i < 32; ++i)
  {
    buffer[i] = ~(orPlane[i] | ~andPlane[i]);
    reverseBitOrder(buffer[i]);
  }
  wxBitmap csrMask((char*) buffer, 32, 32);

  csrBits.SetMask(new wxMask(csrMask));
  wxImage csrImage = csrBits.ConvertToImage();
  csrImage.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, hotX);
  csrImage.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, hotY);

  return new wxCursor(csrImage);
}

static wxCursor&
GetZoomInCursor()
{
  if (0 == hZoomInCursor)
  {
    hZoomInCursor = createCursor(zoomInCursorA, zoomInCursorB, 15, 15);
  }
  return *hZoomInCursor;
}

static wxCursor&
GetZoomOutCursor()
{
  if (0 == hZoomOutCursor)
  {
    hZoomOutCursor = createCursor(zoomOutCursorA, zoomOutCursorB, 15, 15);
  }
  return *hZoomOutCursor;
}

static wxCursor&
GetNoZoomCursor()
{
  if (0 == hNoZoomCursor)
  {
    hNoZoomCursor = createCursor(noZoomCursorA, noZoomCursorB, 15, 15);
  }
  return *hNoZoomCursor;
}

static wxCursor&
GetNoMove2DCursor()
{
  if (0 == hNoMove2DCursor)
  {
    hNoMove2DCursor = createCursor(noMove2DCursorA, noMove2DCursorB, 15, 15);
  }
  return *hNoMove2DCursor;
}

static wxCursor&
GetNoMoveHorizCursor()
{
  if (0 == hNoMoveHorizCursor)
  {
    hNoMoveHorizCursor = createCursor(noMoveHorizCursorA, noMoveHorizCursorB, 15, 15);
  }
  return *hNoMoveHorizCursor;
}

static wxCursor&
GetNoMoveVertCursor()
{
  if (0 == hNoMoveVertCursor)
  {
    hNoMoveVertCursor = createCursor(noMoveVertCursorA, noMoveVertCursorB, 15, 15);
  }
  return *hNoMoveVertCursor;
}


////////////////////////////////////////////////////////////////////////
//
// wxViewPortControl
//
////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(wxViewPortControl, wxPanel)

  EVT_PAINT(wxViewPortControl::OnPaint)
  EVT_MOTION(wxViewPortControl::OnMouseMoveEvent)
  //EVT_LEAVE_WINDOW(wxViewPortControl::OnLeaveEvent)
  EVT_LEFT_DOWN(wxViewPortControl::OnMousePressEvent)
  EVT_LEFT_UP(wxViewPortControl::OnMouseReleaseEvent)
  EVT_MOUSEWHEEL(wxViewPortControl::OnMouseWheelEvent)
  EVT_MOUSE_CAPTURE_LOST(wxViewPortControl::OnMouseCaptureLost)

END_EVENT_TABLE()

wxViewPortControl::wxViewPortControl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name)
  : wxPanel(parent, id, pos, size, style, name)
{
  m_chartViewer = 0;
  m_chart = 0;
  m_reentrantGuard = false;

  m_mouseDownX = 0;
  m_mouseDownY = 0;
  m_isPlotAreaMouseDown = false;
}

// Free	resources
wxViewPortControl::~wxViewPortControl()
{
}

// Set the wxChartViewer to be associated with this control
void
wxViewPortControl::setViewer(wxChartViewer* viewer)
{
  if (m_reentrantGuard)
  {
    return;
  }

  m_reentrantGuard = true;
  if (0 != m_chartViewer)
  {
    m_chartViewer->setViewPortControl(0);
  }
  m_chartViewer = viewer;
  setViewPortManager(viewer);
  if (0 != m_chartViewer)
  {
    m_chartViewer->setViewPortControl(this);
  }
  m_reentrantGuard = false;

  updateDisplay();
}

// Get back	the	same wxChartViewer pointer provided	by the previous	setViewer call.
wxChartViewer*
wxViewPortControl::getViewer()
{
  return m_chartViewer;
}

// Set the chart to	be displayed in the	control
void
wxViewPortControl::setChart(BaseChart* c)
{
  m_chart = c;
  ViewPortControlBase::setChart(c);
  updateDisplay();
}

// Get back	the	same BaseChart pointer provided	by the previous	setChart call.
BaseChart*
wxViewPortControl::getChart()
{
  return m_chart;
}

// Convert from mouse x-coordinate to image x-coordinate
double
wxViewPortControl::ToImageX(int x)
{
  // In this version, no conversion is done. It is assumed the control does not stretch or shrink
  // the image and does not provide any additional margin to offset the image.
  return x;
}

// Convert from mouse x-coordinate to image x-coordinate
double
wxViewPortControl::ToImageY(int y)
{
  // In this version, no conversion is done. It is assumed the control does not stretch or shrink
  // the image and does not provide any additional margin to offset the image.
  return y;
}

void
wxViewPortControl::OnPaint(wxPaintEvent& evt)
{
  wxPaintDC dc(this);
  BaseChart* c = m_chart;
  if (c != NULL)
  {
    Render(dc, c);
  }
}

void
wxViewPortControl::PaintNow(BaseChart* c)
{
  if (c != NULL)
  {
    wxClientDC dc(this);
    //  wxGCDC gdc(dc);
    Render(dc, c);
  }
}

void
wxViewPortControl::Render(wxDC& dc, BaseChart* c)
{
  MemBlock m = c->makeChart(Chart::BMP);
  wxMemoryInputStream in(m.data, m.len);
  wxBitmap bmp(wxImage(in, wxBITMAP_TYPE_BMP));
  dc.DrawBitmap(bmp, 0, 0);
}

// Display the chart
void
wxViewPortControl::PaintDisplay()
{   
  BaseChart* c = getChart();
  if (c != NULL)
  {
    PaintNow(c);
    SetMinSize(wxSize(c->getWidth(), c->getHeight()));
  }
}

// Handle the ViewPortChanged event from the associated wxChartViewer
void
wxViewPortControl::OnViewPortChanged()
{
  updateDisplay();
}

// Mouse capture lost event handler
void
wxViewPortControl::OnMouseCaptureLost(wxMouseCaptureLostEvent& event)
{
  // TODO: Any action required?
}

// Mouse button down event.
void
wxViewPortControl::OnMousePressEvent(wxMouseEvent& event)
{
  event.Skip();
  if (!event.ButtonIsDown(wxMOUSE_BTN_LEFT))
  {
    return;
  }

  // Enable mouse capture for drag support
  if (!HasCapture())
  {
    CaptureMouse();
  }

  // Remember current mouse position
  m_mouseDownX = event.GetX();
  m_mouseDownY = event.GetY();
  m_isPlotAreaMouseDown = true;

  // Get the wxChartViewer zoom/scroll state to determine which type of mouse action is allowed
  SyncState();

  // Handle the mouse down event
  handleMouseDown(ToImageX(event.GetX()), ToImageY(event.GetY()));

  // Update the chart viewer if the viewport has changed
  UpdateChartViewerIfNecessary();
//  event.Skip();
}

// MouseMove event handler
void
wxViewPortControl::OnMouseMoveEvent(wxMouseEvent& event)
{
  //if (m_isPlotAreaMouseDown)
  {
    // Get the wxChartViewer zoom/scroll state to determine which type of mouse action is allowed
    SyncState();

    // Handle the mouse move event
    handleMouseMove(ToImageX(event.GetX()), ToImageY(event.GetY()), IsDrag(event));

    // Update the chart viewer if the viewport has changed
    UpdateChartViewerIfNecessary();
  }

  // Update the mouse cursor
  UpdateCursor(getCursor());

  // Update the display
  if (needUpdateDisplay())
  {
    PaintDisplay();
  }
  event.Skip();
}

// Mouse button up event.
void
wxViewPortControl::OnMouseReleaseEvent(wxMouseEvent& event)
{
  event.Skip();
  if (!event.LeftUp())
  {
    return;
  }

  if (HasCapture())
  {
    ReleaseMouse();
  }
  m_isPlotAreaMouseDown = false;

  // Get the wxChartViewer zoom/scroll state to determine which type of mouse action is allowed
  SyncState();

  // Handle the mouse up event
  handleMouseUp(ToImageX(event.GetX()), ToImageY(event.GetY()));

  // Update the chart viewer if the viewport has changed
  UpdateChartViewerIfNecessary();
}

// MouseWheel handler
void
wxViewPortControl::OnMouseWheelEvent(wxMouseEvent& event)
{
  event.Skip();
  // Process the mouse wheel only if the mouse is over the plot area
  if ((NULL == m_chartViewer) || (!isOnPlotArea(event.GetX(), event.GetY())))
  {
//    event.Skip();
  }
  else
  {
    // Ask the wxChartViewer to zoom around the center of the chart
    int x = m_chartViewer->getPlotAreaLeft() + m_chartViewer->getPlotAreaWidth() / 2;
    int y = m_chartViewer->getPlotAreaTop() + m_chartViewer->getPlotAreaHeight() / 2;
    if (!m_chartViewer->onMouseWheelZoom(x, y, event.GetWheelRotation()))
    {
//      event.Skip();
    }
  }
}

// Get the wxChartViewer zoom/scroll state
void
wxViewPortControl::SyncState()
{
  wxChartViewer* viewer = getViewer();
  if (NULL != viewer)
  {
    setZoomScrollDirection(viewer->getZoomDirection(), viewer->getScrollDirection());
  }
}

// Determines if the mouse is dragging.
bool
wxViewPortControl::IsDrag(wxMouseEvent& event)
{
  wxChartViewer* viewer = getViewer();
  if (NULL == viewer)
  {
    return false;
  }

  int minimumDrag = viewer->getMinimumDrag();
  int moveX = abs(m_mouseDownX - event.GetX());
  int moveY = abs(m_mouseDownY - event.GetY());
  return (moveX >= minimumDrag) || (moveY >= minimumDrag);
}

// Update the display
void
wxViewPortControl::updateDisplay()
{
  paintViewPort();
  PaintDisplay();
}

// Update the CChartViewer if the viewport has changed
void
wxViewPortControl::UpdateChartViewerIfNecessary()
{
  wxChartViewer* viewer = getViewer();
  if (NULL == viewer)
  {
    return;
  }

  if (needUpdateChart() || needUpdateImageMap())
  {
    viewer->updateViewPort(needUpdateChart(), needUpdateImageMap());
  }
}

// Update the mouse cursor
void
wxViewPortControl::UpdateCursor(int position)
{
  switch (position)
  {
    case Chart::Left:
    case Chart::Right:
      SetCursor(wxCursor(wxCURSOR_SIZEWE));
      break;
    case Chart::Top:
    case Chart::Bottom:
      SetCursor(wxCursor(wxCURSOR_SIZENS));
      break;
    case Chart::TopLeft:
    case Chart::BottomRight:
      SetCursor(wxCursor(wxCURSOR_SIZENWSE));
      break;
    case Chart::TopRight:
    case Chart::BottomLeft:
      SetCursor(wxCursor(wxCURSOR_SIZENESW));
      break;
    default:
      SetCursor(wxCursor(wxCURSOR_ARROW));
      break;
  }
}
