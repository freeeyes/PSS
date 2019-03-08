/*
** Name:        wxchartviewer.h
** Purpose:     wxChartViewer class declaration
** Author:      Ulrich Telle
** Created:     2018-05-09
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

/// \file wxchartviewer.h Interface of the \b wxChartViewer component

/** \mainpage wxChartDir

\section intro What is wxChartDir?

\b wxChartDir provides components for the integration and use of the commercial C++ library
<a href="https://www.advsofteng.com/index.html"><b>ChartDirector</b></a>, a universal chart component
developed by <b>Advanced Software Engineering Ltd</b>, in
<a href="https://www.wxwidgets.org/"><b>wxWidgets</b></a> based applications.

At first glance it may seem to contradict the open source philosophy to support a commercial
library, when open source libraries for charting (like
<a href="https://github.com/iwbnwif/wxFreeChart"><b>wxFreeChart</b></a> or
<a href="https://github.com/wxIshiko/wxCharts"><b>wxCharts</b></a>) are available.
However, \b ChartDirector is an extremely versatile, stable and affordable charting library.

\b ChartDirector supports many different chart types like pie, donut, bar, line, spline,
step line, trend line, curve-fitting, inter-line filling, area, band, scatter, bubble,
floating box, box-whisker, waterfall, contour, heat map, surface, vector, finance, gantt,
radar, polar, rose, pyramid, cone, funnel and more. Charts can be created as vector graphics
in PDF and SVG and as raster graphics in PNG, JPG, GIF and BMP. Please visit the
<a href="https://www.advsofteng.com/gallery.html"><b>ChartDirector gallery</b></a>
to get an impression of the capabilities of this library.

\b wxChartDir includes the following charting controls:

<dl>
<dt>wxChartViewer</dt>
<dd>a wxWidgets control that inherits from \b wxPanel and \b ViewPortManager.

A viewport can be imagined as a rectangular window of an underlying rectangular surface.
For example, a chart that has 10 years of data can be imagined as a very long chart.
If one only displays one of the years, it can be said that the viewport covers only
10% of the underlying chart.

With the viewport concept, scrolling can be handled as moving the viewport,
while zooming in and out can be handled as changing the viewport size.
</dd>
<dt>wxViewPortControl</dt>
<dd>a wxWidgets control that inherits from \b wxPanel and from \b ViewPortControlBase.

A viewport control is a user interface element to let the user visualize and manipulate the
viewport managed by the ViewPortManager.

In typical usage, the viewport control displays an "overall chart" that shows the full data range,
and connects to the ViewPortManager (wxChartViewer). It can then include a rectangle on the overall
chart to represent the viewport in the ViewPortManager.
The region outside the rectangle can be dimmed out to highlight the viewport region.
If the viewport is updated (such as if the user uses the mouse wheel to zoom in the chart),
the rectangle will also update automatically.
</dd>
</dl>

Additionally, \b wxChartDir contains wxWidgets based versions of all sample applications coming with
\b ChartDirector and of the <a href="https://www.advsofteng.com/tutorials/extra.html">extra samples</a>
available from the \b ChartDirector website. \b ChartDirector itself contains versions for QT and MFC.

\note While the source code of the components wxChartViewer and wxViewPortControl as well as that
of the wxWidgets based standard samples is licensed under the
<a href="https://www.wxwidgets.org/about/licence/"><b>wxWindows license</b></a>
(resp LGPL - 3.0 + WITH WxWindows - exception - 3.1), the extra samples contain code that is restricted
by the following terms:
> You may use and modify the code in this file in your application,
> provided the code and its modifications are used only in conjunction
> with ChartDirector. Usage of this software is subject to the terms
> and conditions of the ChartDirector license.
A copy of the \b ChartDirector license is included in the \b wxChartDir distribution.

\section version Version history

<dl>

<dt><b>1.0.0</b> - <i>July 2018</i></dt>
<dd>
First public release
</dd>

</dl>

\author Ulrich Telle (ulrich DOT telle AT gmx DOT de)

\section ackn Acknowledgments

Kudos to <b>Peter Kwan</b> from <b>Advanced Software Engineering Ltd</b> for
his excellent support during the development of this component.

*/

#ifndef WXCHARTDIR_WXCHARTVIEWER_H
#define WXCHARTDIR_WXCHARTVIEWER_H

// wxWidgets headers

// wxChartViewer headers
#include "wxchartviewer_defs.h"
#include "wxchartviewer_version.h"

#include <wx/bitmap.h>
#include <wx/event.h>
#include <wx/panel.h>
#include <wx/timer.h>
#include <wx/graphics.h>
#include <wx/overlay.h>

#include "chartdir.h"

#ifdef CD_NAMESPACE
namespace CD_NAMESPACE
{
#endif

namespace Chart
{
  // Mouse usage mode constants
  enum
  {
    MouseUsageDefault = 0,
    MouseUsageScroll  = 1,
    MouseUsageZoomIn  = 3,
    MouseUsageZoomOut = 4
  };
}

#ifdef CD_NAMESPACE
}
#endif

// Forward declarations
class WXDLLIMPEXP_FWD_WXCHARTVIEWER wxViewPortControl;

/// Class representing a ChartDirector chart viewer control.
/**
 * wxChartViewer is a wxWidgets control that inherits from wxPanel and from ViewPortManager.
 * It is used in wxWidgets applications for displaying chart images and to handle mouse interactions.
 */
class WXDLLIMPEXP_WXCHARTVIEWER wxChartViewer : public wxPanel, public ViewPortManager
{
public:
  /// Constructor for a chart viewer control.
  /**
   * \param parent The parent window.
   * \param id An identifier for the panel. wxID_ANY is taken to mean a default.
   * \param pos The panel position. The value wxDefaultPosition indicates a default position, chosen by either the windowing system or wxWidgets, depending on platform. 
   * \param size The panel size. The value wxDefaultSize indicates a default size, chosen by either the windowing system or wxWidgets, depending on platform. 
   * \param style The window style.
   * \param name Window name.
   */
  wxChartViewer(wxWindow* parent = 0, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name = wxPanelNameStr);

  /// Destructor for a chart viewer control.
  ~wxChartViewer();

  /// Sets a BaseChart object for display.
  /**
   * wxChartViewer stores the BaseChart pointer internally so it can later be retrieved with wxChartViewer::getChart.
   * However, wxChartViewer does not use the stored pointer itself. So it is perfectly safe to immediately delete the
   * BaseChart after calling wxChartViewer::setChart, or to use a BaseChart that is allocated on the stack for this method.
   *
   * \param chart The BaseChart object to be displayed.
   */
  virtual void setChart(BaseChart* chart);

  /// Gets the BaseChart pointer provided by the previous wxChartViewer::setChart method call.
  /**
   * Gets the BaseChart pointer provided by the previous wxChartViewer::setChart method call.
   *
   * \return  The BaseChart pointer.
   */
  virtual BaseChart* getChart();

  /// Sets the image map that defines the hot spots on the chart image.
  /**
   * By using an image map, a wxChartViewer object can be configured to display tool tips specific
   * to the hot spots when the mouse is over the hot spots. The mouse cursor can also change to a
   * "hand" shape to provide visual feedback for clickable hot spots. A clickable hot spot is
   * defined as a hot spot with a non-empty path attribute.
   *
   * <a href="https://www.advsofteng.com/doc/cdcppdoc/BaseChart.getHTMLImageMap.htm">BaseChart.getHTMLImageMap</a>
   * can be used to generated image maps for charts automatically.
   *
   * The ImageMapHandler object, accessible via wxChartViewer::getImageMapHandler, can be used to
   * determine if the mouse cursor is over a hot spot and to retrieve the hot spot parameters.
   *
   * \param imageMap The image map that defines the hot spots.
   */
  virtual void setImageMap(const char* imageMap);

  /// Gets the ImageMapHandler object for the current image map.
  /**
   * \return The ImageMapHandler object that represents the image map of the chart.
   */
  virtual ImageMapHandler* getImageMapHandler();

  /// Sets the default tool tip to use when the mouse is over the wxChartViewer but not over any hot spot.
  /**
   * \param text The default tool tip text to use.
   */
  virtual void setDefaultToolTip(const wxString& text);

  /// Sets the viewport control to be associated with this chart viewer control.
  /**
   * \param vpc The viewport control to be associated with this chart viewer.
   */
  virtual void setViewPortControl(wxViewPortControl* vpc);

  /// Get the viewport control associated with this chart viewer control.
  /**
   * \return A pointer to the viewport control associated with this chart viewer.
   */
  virtual wxViewPortControl* getViewPortControl();

  /// Sets the border width of the zoom selection box.
  /**
   * \param width The border width in pixels.
   */
  virtual void setSelectionBorderWidth(int width);

  /// Gets the border width of the zoom selection box.
  /**
   * \return The border width of the zoom selection box in pixels.
   */
  virtual int getSelectionBorderWidth();

  /// Sets the border color of the zoom selection box as a wxColour value.
  /**
   * \param color The border color as a wxColour value.
   */
  virtual void setSelectionBorderColor(const wxColour& color);

  /// Gets the border color of the zoom selection box.
  /**
   * \return The border color of the zoom selection box as a wxColour value.
   */
  virtual wxColour getSelectionBorderColor();

#if wxCHECK_VERSION(3,0,0)
  /// Sets the border style of the zoom selection box.
  /**
   * \param style The border style of the zoom selection box.
   */
  virtual void setSelectionBorderStyle(wxPenStyle style);

  /// Gets the border style of the zoom selection box.
  /**
   * \return The border style of the zoom selection box.
   */
  virtual wxPenStyle getSelectionBorderStyle();
#else
  /// Sets the border style of the zoom selection box.
  /**
  * \param style The border style of the zoom selection box.
  */
  virtual void setSelectionBorderStyle(int style);

  /// Gets the border style of the zoom selection box.
  /**
  * \return The border style of the zoom selection box.
  */
  virtual int getSelectionBorderStyle();
#endif

  /// Sets the mouse usage mode.
  /**
   * The following table describes the supported mouse usage modes and the mouse behaviour in each mode.
   *
   * <table>
   * <tr><th width="19%">Constant</th><th width="19%">Value</th><th width="60%">Description</th></tr>
   * <tr>
   *   <td valign="top"><a href="https://www.advsofteng.com/doc/cdcppdoc/Chart.MouseUsageDefault.htm">MouseUsageDefault</a></td>
   *   <td valign="top">0</td>
   *   <td>This represents the default mouse behaviour, which is to support tool tips and hot spot events as defined by the image map.
   *       <br><br>See wxChartViewer::setImageMap on how to define an image map for a chart.</td>
   * </tr>
   * <tr>
   *   <td valign="top"><a href="https://www.advsofteng.com/doc/cdcppdoc/Chart.MouseUsageScroll.htm">MouseUsageScroll</a></td>
   *   <td valign="top">1</td><td>The mouse is used for drag scrolling the chart.<br><br>
   * When the mouse drags on the plot area, the mouse cursor will change to a scroll cursor, the viewport coordinates will shift to reflect the amount dragged, and viewPortChanged signals will be emitted.<br><br>
   * If the mouse is not dragging inside the plot area, its behaviour is the same as the default mouse behaviour.</td>
   * </tr>
   * <tr>
   *   <td valign="top"><a href="https://www.advsofteng.com/doc/cdcppdoc/Chart.MouseUsageZoomIn.htm">MouseUsageZoomIn</a></td>
   *   <td valign="top">3</td>
   *   <td>The mouse is used for zoom in actions.<br><br>
   * When the mouse is inside the plot area, the following behaviour will apply:
   * <ul>
   * <li>The mouse cursor will change to a zoom in cursor (a magnifying glass cursor with a '+' sign inside).</li>
   * <li>When the mouse drags on the plot area, a selection rectangle will be visible. When the mouse button is subsequently released, the viewport coordinates will be updated to reflect the selection rectangle, and a viewPortChanged signal will be emitted.</li>
   * <li>When the mouse clicks on the plot area without dragging, the viewport will be updated to reflect zooming into the clicked point by a ratio specified in wxChartViewer::setZoomInRatio, and a viewPortChanged signal will be emitted. </li>
   * </ul>
   * If the mouse is not inside the plot area, its behaviour is the same as the default mouse behaviour.</td>
   * </tr>
   * <tr>
   *   <td valign="top"><a href="https://www.advsofteng.com/doc/cdcppdoc/Chart.MouseUsageZoomOut.htm">MouseUsageZoomOut</a></td>
   *   <td valign="top">4</td>
   *   <td>The mouse is used for zoom out actions.<br><br>
   * When the mouse is inside the plot area, the following behaviour will apply:
   * <ul>
   * <li>The mouse cursor will change to a zoom out cursor (a magnifying glass cursor with a '-' sign inside).</li>
   * <li>When the mouse clicks on the plot area , the viewport will be updated to reflect zooming out from the clicked point by a ratio specified in wxChartViewer::setZoomOutRatio, and a viewPortChanged signal will be emitted. </li>
   * </ul>
   * If the mouse is not inside the plot area, its behaviour is the same as the default mouse behaviour.</td>
   * </tr>
   * </table>
   * \param mouseUsage The mouse usage mode of the control, which must be one of the predefined constants as shown in the table above.
   */
  virtual void setMouseUsage(int mouseUsage);

  /// Gets the mouse usage mode.
  /**
   * \return An integer representing the mouse usage mode. See wxChartViewer::setMouseUsage for the meaning of this value.
   */
  virtual int getMouseUsage();

  /// Sets the zoom direction for mouse zoom in/out actions.
  /**
   * In zoom operations for charts, it is common to restrict zooming to one direction only.
   * For example, in a chart plotting temperature against time, zooming in from a yearly chart
   * to a daily chart may mean the time axis (x-axis) is zoomed in by 36500%, while the
   * temperature axis (y-axis) may be auto-scaled and not zoomed in at all. The user may be
   * surprise if you also zoom in the y-axis by 36500%, in which case the chart may become
   * hardly readable.
   *
   * This method allows you to determine the zoom direction for mouse zoom actions
   * (see wxChartViewer::setMouseUsage), specified using the following predefined constants:
   *
   * <table>
   *  <tr><th width="19%">Constant</th><th width="19%%">Value</th><th width="60%">Description</th></tr>
   *  <tr>
   *    <td><a href="https://www.advsofteng.com/doc/cdcppdoc/Chart.DirectionHorizontal.htm">DirectionHorizontal</a></td>
   *    <td>0</td>
   *    <td>Zoom actions apply to horizontal direction only.</td>
   *  </tr>
   *  <tr>
   *    <td><a href="https://www.advsofteng.com/doc/cdcppdoc/Chart.DirectionVertical.htm">DirectionVertical</a></td>
   *    <td>1</td>
   *    <td>Zoom actions apply to vertical direction only.</td>
   *  </tr>
   *  <tr>
   *    <td><a href="https://www.advsofteng.com/doc/cdcppdoc/Chart.DirectionHorizontalVertical.htm">DirectionHorizontalVertical</a></td>
   *    <td>2</td><td>Zoom actions apply to both horizontal and vertical directions.</td>
   *  </tr>
   *  </table>
   *
   * \param direction The zoom direction for mouse zoom actions.
   */
  virtual void setZoomDirection(int direction);

  /// Gets the zoom direction for mouse zoom in/out actions.
  /**
   * Please refer to wxChartViewer::setZoomDirection for the supported directions.
   *
   * \return  An integer representing the zoom direction.
   */
  virtual int getZoomDirection();

  /// Sets the scroll direction for mouse scroll actions.
  /**
   * Please refer to wxChartViewer::setZoomDirection for the supported directions.
   *
   * \param direction The scroll direction for mouse scroll actions.
   */
  virtual void setScrollDirection(int direction);

  /// Gets the scroll direction for mouse scroll actions.
  /**
   * Please refer to wxChartViewer::setZoomDirection for the supported directions.
   *
   * \return  An integer representing the scroll direction.
   */
  virtual int getScrollDirection();

  /// Sets the magnification ratio for each zoom in click of the mouse.
  /**
   * For zoom in, the ratio should be greater than 1. The default is 2. 
   *
   * \param ratio The zoom in ratio.
   */
  virtual void setZoomInRatio(double ratio);

  /// Gets the magnification ratio for each zoom in click of the mouse.
  /**
   * For zoom in, the ratio should be greater than 1. The default is 2. 
   *
   * \return The magnification ratio. 
   */
  virtual double getZoomInRatio();

  /// Sets the magnification ratio for each zoom out click of the mouse.
  /**
   * For zoom out, the ratio should be less than 1. The default is 0.5.
   *
   * \param ratio The zoom out ratio.
   */
  virtual void setZoomOutRatio(double ratio);

  /// Gets the magnification ratio for each zoom out click of the mouse.
  /**
   * For zoom out, the ratio should be less than 1. The default is 0.5.
   *
   * \return The magnification ratio.
   */
  virtual double getZoomOutRatio();

  /// Sets the magnification ratio for each mouse wheel event.
  /**
   * The magnification ratio is applied when the mouse wheel rolls towards the screen.
   * The reciprocal of the ratio is applied when the mouse wheel rolls away from the screen.
   * A ratio larger than 1 means to zoom in when the mouse wheel rolls towards the screen,
   * and to zoom out when the mouse wheel rolls away from the screen. A ratio less than 1
   * means the opposite behaviour. A ratio equal to 1 disables mouse wheel zooming.
   *
   * The mouse wheel causes zooming to occur, it will trigger the wxEVT_CHARTVIEWER_VIEWPORT_CHANGED message.
   *
   * \param ratio The magnification ratio.
   */
  virtual void setMouseWheelZoomRatio(double ratio);

  /// Gets the magnification ratio for each mouse wheel event.
  /**
   * Please refer to wxChartViewer::setMouseWheelZoomRatio for the usage of this ratio.
   *
   * \return The magnification ratio.
   */
  virtual double getMouseWheelZoomRatio();

  /// Sets the minimum mouse drag distance before it is considered as an intentional drag.
  /**
   * A mouse drag occurs if the mouse button is down while the mouse moves.
   * However, sometimes a user may want to click on a point, but due to mouse
   * vibrations, the mouse moves by a small amount while the button is still down.
   * This may then be interpreted as a mouse drag.
   *
   * This method determines the minimum drag distance in pixels before it is considered
   * as an intentional drag. The default is 5 pixels. Non-intentional drags will be handled
   * as mouse clicks instead of drags.
   *
   * \param offset The minimum mouse drag distance in pixels before it is considered as an intentional drag.
   */
  virtual void setMinimumDrag(int offset);

  /// Gets the minimum mouse drag distance before it is considered as an intentional drag.
  /**
   * \return  The minimum mouse drag distance in pixels before it is considered as an intentional drag. See wxChartViewer::setMinimumDrag for the meaning of this value.
   */
  virtual int getMinimumDrag();

  /// Sets the minimum gap interval between two viewPortChanged signals in milliseconds.
  /**
   * Some GUI designs may be capable of generate a lot of viewPortChanged events rapidly.
   * For example, an external scroll bar may update the viewport and call wxChartViewer::updateViewPort
   * rapidly and repeatedly when it is scrolling. As the chart is usually updated in the main GUI thread,
   * updating too rapidly without a gap interval in between may cause other GUI elements to become sluggish.
   *
   * This method specifies the minimum gap interval between two viewPortChanged signals. The default is 20ms.
   *
   * If a viewPortChanged event needs to be emitted during the gap interval, the signal will be delayed and
   * emitted after the gap interval.
   *
   * If multiple viewPortChanged events needs to be emitted during the gap interval, they will be merged into
   * one event, and be emitted after the gap interval.
   *
   * \param interval The minimum gap interval between two viewPortChanged signals in milliseconds.
   */
  virtual void setUpdateInterval(int interval);

  /// Gets the minimum gap interval between two wxEVT_CHARTVIEWER_VIEWPORT_CHANGED events in milliseconds.
  /**
   * \return  The minimum gap interval between two viewPortChanged signals in milliseconds.
   *          See wxChartViewer::setUpdateInterval for the meaning of this value.
   */
  virtual int getUpdateInterval();

  /// Determines if the current wxEVT_CHARTVIEWER_VIEWPORT_CHANGED event requires chart update.
  /**
   * The event handler for a wxEVT_CHARTVIEWER_VIEWPORT_CHANGED event can use this method to determine
   * if chart update is necessary.
   *
   * This method is only valid during handling of the wxEVT_CHARTVIEWER_VIEWPORT_CHANGED event.
   * Its behaviour is undefined at other times.
   *
   * \return A boolean value that indicates if the wxEVT_CHARTVIEWER_VIEWPORT_CHANGED event requires chart update.
   */
  virtual bool needUpdateChart();

  /// Determines if the current viewPortChanged signal requires image map update.
  /**
   * The event handler for a wxEVT_CHARTVIEWER_VIEWPORT_CHANGED signal can use this method
   * to determine if image map update is necessary.
   *
   * This method is only valid during handling of the wxEVT_CHARTVIEWER_VIEWPORT_CHANGED event.
   * Its behaviour is undefined at other times.
   *
   * \return A boolean value that indicates if the viewPortChanged signal requires image map update.
   */
  virtual bool needUpdateImageMap();

  /// Checks if the mouse cursor is on the extended plot area.
  /**
   * See wxChartViewer::setPlotAreaMouseMargin on what is the extended plot area.
   *
   * \return  True if the mouse is on the extended plot area, otherwise false.
   */
  virtual bool isMouseOnPlotArea();

  /// Checks if the current code is executing in a wxEVT_CHARTVIEWER_MOUSEMOVE_CHART or wxEVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA event.
  /**
   * \return True if the current code is executing in a wxEVT_CHARTVIEWER_MOUSEMOVE_CHART message or wxEVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA message, otherwise false. 
   */
  virtual bool isInMouseMoveEvent();

  /// Checks if the current code is executing in a ViewPortChanged event.
  /**
   * This is to help preventing "event looping" in complicated GUI designs in which multiple
   * controls can update the viewport and indirectly update each others.
   *
   * For example, suppose in a scrollable chart design, the x-axis can be scrolled by both a
   * scroll bar control and a date/time picker control. That means both scroll bar and date/time
   * picker events may update the viewport.
   *
   * The following sequence of events may happen:
   *
   * <ul>
   * <li>User changes the start date on the x-axis using the date/time picker control.</li>
   * <li>The viewport is updated, so QChartViewer.updateViewPort is called, triggering a CVN_ViewPortChanged message.</li>
   * <li>In the ViewPortChanged event handler, the scroll bar position needs to be updated to reflect that the viewport has moved.</li>
   * <li>As the scroll bar has changed, a scroll bar event occurs. The scroll bar event handler may update the viewport and call QChartViewer.updateViewPort again.</li>
   * </ul>
   *
   * The above is potentially an infinite loop as the ViewPortChanged event will keep firing again
   * and again. To avoid this, wxChartViewer::updateViewPort will perform no action if it is called
   * within wxEVT_CHARTVIEWER_VIEWPORT_CHANGED event. So the last step above cannot actually trigger
   * another wxEVT_CHARTVIEWER_VIEWPORT_CHANGED event. This avoids the infinite loop.
   *
   * Internally, wxChartViewer::updateViewPort uses this method to check if it is called within a
   * wxEVT_CHARTVIEWER_VIEWPORT_CHANGED event, and ignores the request if it is already in the event.
   * This method allows external code to also change behaviour in a wxEVT_CHARTVIEWER_VIEWPORT_CHANGED event.
   *
   * \return True if the current code is executing in a wxEVT_CHARTVIEWER_VIEWPORT_CHANGED message, otherwise false.
   */
  virtual bool isInViewPortChangedEvent();

  /// Checks if the mouse is dragging on the plot area.
  /**
   * A mouse is considered to be dragging on the plot area if the mouse is on the plot area when
   * the mouse button is down, and the mouse button still has not been released.
   *
   * \return True if the mouse is dragging on the plot area, otherwise false.
   */
  virtual bool isMouseDragging();

  /// Triggers the wxEVT_CHARTVIEWER_VIEWPORT_CHANGED event
  /**
   * wxChartViewer may emit wxEVT_CHARTVIEWER_VIEWPORT_CHANGED events when the mouse clicks or
   * drags on the plot area (see wxChartViewer::setMouseUsage). wxChartViewer also allows external
   * objects (such as scroll bars, navigation buttons, timers, etc) to modify the viewport. This
   * method allows external objects to request wxChartViewer to emit wxEVT_CHARTVIEWER_VIEWPORT_CHANGED events.
   *
   * Many controls are able to update the viewport continuously as they are used. For example,
   * a scroll bar may update the viewport continuously as the mouse drags on it. For these cases,
   * the chart display may be updated continuously as the viewport changes. However, the image map
   * does not need to update until the scrolling is completed (when the mouse button is released
   * on the scroll bar and the mouse moves over the chart surface).
   *
   * The needUpdateChart argument and needUpdateImageMap argument allows one to specify
   * if the event requires a chart update, and/or an image map update.
   *
   * For charts with a lot of hot spots and tool tips, the CPU power needed to handle the image map
   * may not be negligible. Removing unnecessary image map updates may increase the update rate
   * when the chart is being updated continuously.
   *
   * \param needUpdateChart A boolean value to indicate if the chart needs to be updated.
   * \param needUpdateImageMap A boolean value to indicate if the image map needs to be updated.
   */
  virtual void updateViewPort(bool needUpdateChart, bool needUpdateImageMap);

  /// Updates the display.
  // Request wxChartViewer to redisplay the chart
  /**
   * If a chart is assigned to a wxChartViewer for display, the wxChartViewer will automatically
   * display it. If the chart is subsequently modified (eg. by drawing additional things in its
   * dynamic layer - see <a href="https://www.advsofteng.com/doc/cdcppdoc/BaseChart.initDynamicLayer.htm">BaseChart.initDynamicLayer</a>),
   * this method can be used to notify the wxChartViewer that the modification has been completed
   * so that it can update the display. 
   */
  virtual void updateDisplay();

  /// Handles mouse wheel zooming
  /**
   * \param x x-coordinate of the mouse cursor position
   * \param y y-coordinate of the mouse cursor position
   * \param zDelta The sign of zDelta determines the zoom direction (i.e. zoom in or zoom out)
   * \return
   */
  virtual bool onMouseWheelZoom(int x, int y, int zDelta);

  /// Set the message used to remove the dynamic layer
  /**
   * The dynamic layer (see <a href="https://www.advsofteng.com/doc/cdcppdoc/BaseChart.initDynamicLayer.htm">BaseChart.initDynamicLayer</a>)
   * is commonly used for drawing track cursors. For these type of usages, it is often necessary
   * to clear the dynamic layer when the mouse cursor leaves the plot area. The removeDynamicLayer
   * method can be used in the dynamic layer drawing code to automatically clear the dynamic layer
   * when the specified event occurs. This avoids the need to set up a separate event handler to
   * call <a href="https://www.advsofteng.com/doc/cdcppdoc/BaseChart.removeDynamicLayer.htm">BaseChart.removeDynamicLayer</a>.
   *
   * In the current version of ChartDirector, this method only supports the eventId
   * "MouseLeaveChart" and "MouseLeavePlotArea".
   *
   * This method only applies to the first occurrence of the specified event after calling this method.
   * If this method is called multiple times before the event occurs, only the last call has effect.
   * To cancel this method, use an empty string as the eventId.
   *
   * \param msg The event that causes BaseChart.removeDynamicLayer to be called.
   */
  virtual void removeDynamicLayer(const wxString& msg);

  /// Gets the mouse x-coordinate in the chart pixel coordinate system.
  /**
   * This method is only valid when called inside the chart or plot area mouse events.
   *
   * \return The mouse x-coordinate in the chart pixel coordinate system.
   */
  virtual int getChartMouseX();

  /// Gets the mouse y-coordinate in the chart pixel coordinate system.
  /**
   * This method is only valid when called inside the chart or plot area mouse events.
   *
   * \return The mouse x-coordinate in the chart pixel coordinate system.
   */
  virtual int getChartMouseY();

  /// Gets the mouse x-coordinate on the plot area in the chart pixel coordinate system.
  /**
   * If the mouse cursor is not on the plot area, but is on the extended plot area
   * (see wxChartViewer::setPlotAreaMouseMargin), this method will behave as if the
   * mouse cursor is on the nearest plot area edge.
   *
   * If the mouse cursor is not even on the extended plot area mouse region, this method will
   * return the last coordinate when the mouse cursor was still on the extended plot area.
   *
   * This method is only valid when called inside the chart or plot area mouse events.
   *
   * \return The mouse x-coordinate in the chart pixel coordinate system. 
   */
  virtual int getPlotAreaMouseX();

  /// Gets the mouse y-coordinate on the plot area in the chart pixel coordinate system.
  /**
   * If the mouse cursor is not on the plot area, but is on the extended plot area
   * (see wxChartViewer::setPlotAreaMouseMargin), this method will behave as if the
   * mouse cursor is on the nearest plot area edge.
   *
   * If the mouse cursor is not even on the extended plot area mouse region, this method will
   * return the last coordinate when the mouse cursor was still on the extended plot area.
   * 
   * This method is only valid when called inside the chart or plot area mouse events.
   *
   * \return The mouse y-coordinate in the chart pixel coordinate system. 
   */
  virtual int getPlotAreaMouseY();

  /// Convert from wxDateTime to chartTime
  /**
   * Many functions in the ChartDirector API accept dates/times as parameters, in which
   * the dates/times are represented as the seconds elapsed since 1-1-0001 00:00:00.
   *
   * This function allows to convert a wxDateTime value to the corresponding chartTime value.
   *
   * \param dt wxDateTime value to be converted to ChartDirector's chartTime
   * \return ChartDirector's chartTime value corresponding to wxDateTime dt
   */
  static double ConvertDateTimeToChartTime(wxDateTime dt);

  /// Convert from chartTime to wxDateTime
  /**
   * Many functions in the ChartDirector API accept dates/times as parameters, in which
   * the dates/times are represented as the seconds elapsed since 1-1-0001 00:00:00.
   *
   * This function allows to convert a chartTime value to the corresponding wxDateTime value.
   *
   * \param ct chartTime value to be converted to a wxDateTime value
   * \return wxDateTime value corresponding to chartTime ct
   */
  static wxDateTime ConvertChartTimeToDateTime(double ct);

  DECLARE_EVENT_TABLE()

protected:
  // wxChartViewer event handlers

  /// Handles mouse move events.
  /**
   * \param evt Mouse event
   */
  void OnMouseMove(wxMouseEvent &evt);

  /// Handles the OnPaint event.
  /**
   * \param evt Paint event
   */
  void OnPaint(wxPaintEvent & evt);

  /// Paints a chart immediately on screen.
  /**
   * \param chart chart object to be painted on screen
   */
  void PaintNow(BaseChart* chart);

  /// Renders a chart to a device context.
  /**
   * \param dc screen device context
   * \param chart chart object to be rendered on screen
   */
  void Render(wxDC& dc, BaseChart* chart);

  /// Handles a delayed mouse move event.
  /**
   */
  virtual void OnDelayedMouseMove();

  /// Handles a mouse capture lost event.
  /**
   * \param evt Mouse capture lost event
   */
  virtual void OnMouseCaptureLost(wxMouseCaptureLostEvent& evt);

  /// Handles a left mouse button down event.
  /**
   * \param evt Mouse event
   */
  virtual void OnMousePressEvent(wxMouseEvent& evt);

  /// Handles a left mouse button up event.
  /**
   * \param evt Mouse event
   */
  virtual void OnMouseReleaseEvent(wxMouseEvent& evt);

  /// Handles a control leave event.
  /**
   * \param evt Mouse event
   */
  virtual void OnLeaveEvent(wxMouseEvent& evt);

  /// Handles a mouse wheel event.
  /**
   * \param evt Mouse event
   */
  virtual void OnWheelEvent(wxMouseEvent& evt);

  /// Handles a hold timer event.
  /**
   * \param evt Timer event
   */
  virtual void OnHoldTimerEvent (wxTimerEvent& evt);

  /// Handles a delay timer event.
  /**
   * \param evt Timer event
   */
  virtual void OnDelayTimerEvent(wxTimerEvent& evt);

  /// Handles a set cursor event.
  /**
   */
  virtual void OnSetCursor();

private:
  /// wxChartViewer configurable properties
  BaseChart*       m_currentChart;             ///< Current BaseChart object
  ImageMapHandler* m_hotSpotTester;            ///< ImageMapHander representing the image map
  wxString         m_defaultToolTip;           ///< Default tool tip text
  wxColour         m_selectBoxLineColor;       ///< Selectiom box border color
  int              m_selectBoxLineWidth;       ///< Selectiom box border width
#if wxCHECK_VERSION(3,0,0)
  wxPenStyle       m_selectBoxLineStyle;       ///< Selection box pen style
#else
  int              m_selectBoxLineStyle;       ///< Selection box pen style
#endif

  int    m_mouseUsage;             ///< Mouse usage mode
  int    m_zoomDirection;          ///< Zoom direction
  int    m_scrollDirection;        ///< Scroll direction
  double m_zoomInRatio;            ///< Click zoom in ratio
  double m_zoomOutRatio;           ///< Click zoom out ratio
  double m_mouseWheelZoomRatio;    ///< Mouse wheel zoom ratio
  int    m_minDragAmount;          ///< Minimum drag amount
  int    m_updateInterval;         ///< Minimum interval between chart updates
  bool   m_needUpdateChart;        ///< Has pending chart update request
  bool   m_needUpdateImageMap;     ///< Has pending image map udpate request

  /// Keep track of mouse states

  int  m_currentHotSpot;           ///< The hot spot under the mouse cursor.
  bool m_isClickable;              ///< Mouse is over a clickable hot spot.
  bool m_isOnPlotArea;             ///< Mouse is over the plot area.
  bool m_isPlotAreaMouseDown;      ///< Mouse left button is down in the plot area.
  bool m_isDragScrolling;          ///< Is current dragging scrolling the chart.
  bool m_isMouseTracking;          ///< Is tracking mouse leave event
  bool m_isInMouseMove;            ///< Is in mouse moeve event handler

  // Dragging support

  int  m_plotAreaMouseDownXPos;    ///< The starting x coordinate of the mouse drag.
  int  m_plotAreaMouseDownYPos;    ///< The starting y coordinate of the mouse drag.

  /// Checks if mouse is dragging.
  bool IsDrag(int direction, wxMouseEvent& event);

  /// Handles mouse dragging.
  void OnPlotAreaMouseDrag(wxMouseEvent& point);

  // Selection rectangle

  bool   m_rectVisible; ///< Current selection box is visible on screen
  wxRect m_currentRect; ///< Current selection box

  /// Sets position and dimension of selection box.
  void SetSelectionRect(int x, int y, int width, int height);

  /// Shows or hides the selection box.
  void SetRectVisible(bool b);

  // Chart update rate control

  bool       m_holdTimerActive;               ///< Delay chart update to limit update frequency
  int        m_holdTimerId;                   ///< The id of the hold timer.
  wxTimer    m_holdTimer;
  bool       m_isInViewPortChanged;           ///< Flag to avoid infinite update loops

  int        m_delayUpdateChart;              ///< Delay chart update until the mouse event is completed
  BaseChart* m_delayedChart;

  /// Commits updating the chart.
  void CommitUpdateChart();

  unsigned int  m_lastMouseMove;              ///< The timestamp of the last mouse move event.
  wxMouseEvent* m_delayedMouseEvent;	        ///< The delayed mouse move event.
  int           m_delayedMouseEventTimerId;   ///< The id of the delayed mouse move timer.
  wxTimer       m_delayedMouseEventTimer;
  void CommitMouseMove(wxMouseEvent& event);  ///< Raise the delayed mouse move event.

  bool m_delayImageMapUpdate;	                ///< Delay image map update until mouse moves on plot area

  // Track Cursor support
  int m_currentMouseX;                        ///< Get the mouse x-pixel coordinate
  int m_currentMouseY;                        ///< Get the mouse y-pixel coordinate
  int m_isInMouseMovePlotArea;              	///< flag to indicate if is in a mouse move plot area event.

  // Dynamic Layer support
  wxString m_autoHideMsg;                     ///< The message that will trigger removing the dynamic layer.

  /// Attempts to remove the dynamic layer with the given message.
  void ApplyAutoHide(const wxString& msg);

  // wxViewPortControl support
  wxViewPortControl* m_vpControl;             ///< Associated wxViewPortControl      
  bool               m_reentrantGuard;				///< Prevents infinite calling loops

  wxBitmap  m_chartBitmap;  ///< Chart converted to a wxBitmap
  wxOverlay m_overlay;      ///< Overlay for drawing the selection box on the overlay device context
};

/// Class representing a ChartDirector viewport control.
/**
 * wxViewPortControl is a wxWidgets control that inherits from wxPanel and from
 * <a href="https://www.advsofteng.com/doc/cdcppdoc/ViewPortControlBase.htm">ViewPortControlBase</a>.
 * It is used in wxWidgets applications for displaying charts images and to handle mouse interactions. 
 */
class wxViewPortControl : public wxPanel, public ViewPortControlBase
{
public:
  /// Constructor for a ChartDirector viewport.
  /**
   * \param parent The parent window.
   * \param id An identifier for the panel. wxID_ANY is taken to mean a default.
   * \param pos The panel position. The value wxDefaultPosition indicates a default position, chosen by either the windowing system or wxWidgets, depending on platform.
   * \param size The panel size. The value wxDefaultSize indicates a default size, chosen by either the windowing system or wxWidgets, depending on platform.
   * \param style The window style.
   * \param name Window name.
   */
  wxViewPortControl(wxWindow* parent = 0, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, const wxString& name = wxPanelNameStr);

  /// Destructor for a ChartDirector viewport.
  ~wxViewPortControl();

  /// Sets a BaseChart object for displaying in the control.
  /**
   * The chart usually is an overall chart that includes the full data range.
   *
   * \param chart The BaseChart object to be displayed.
   */
  virtual void setChart(BaseChart* chart);

  /// Gets the BaseChart object associated with the wxViewPortControl.
  /**
   * \return The BaseChart pointer. 
   */
  virtual BaseChart* getChart();

  /// Associates a wxChartViewer object with this wxViewPortControl object.
  /**
   * \param viewer The wxChartViewer object to be associated with this wxViewPortControl object.
   */
  virtual void setViewer(wxChartViewer* viewer);

  /// Gets the wxChartViewer object associated with this viewport control.
  /**
   * \return The wxChartViewer pointer associated with this wxViewPortControl.
   */
  virtual wxChartViewer* getViewer();

  /// Requests the viewport control to update its contents.
  /**
   * Normally, the wxViewPortControl display will automatically update if the viewport of the
   * associated wxChartViewer object is changed. So calling updateDisplay is not necessary.
   * However, if the display needs to be updated for other reasons, such as if the viewport
   * border color has been modified, this method can be used to notify the wxViewPortControl
   * that the modification has been completed so that it can update the display. 
   */
  virtual void updateDisplay();

  DECLARE_EVENT_TABLE()

protected:
  /// Handles the OnPaint event.
  /**
  * \param evt Paint event
  */
  void OnPaint(wxPaintEvent & evt);

  /// Paints a chart immediately on screen.
  /**
  * \param chart chart object to be painted on screen
  */
  void PaintNow(BaseChart* chart);

  /// Renders the chart on the device context.
  /**
  * \param dc screen device context
  * \param chart chart object to be rendered on screen
  */
  void Render(wxDC& dc, BaseChart* chart);

  // wxChartViewer event handlers
  /// Handles mouse move events.
  /**
  * \param evt Mouse event
  */
  virtual void OnMouseMoveEvent(wxMouseEvent& evt);

  /// Handles left mouse button down events.
  /**
   * \param evt Mouse event
   */
  virtual void OnMousePressEvent(wxMouseEvent& evt);

  /// Handles left mouse button up events.
  /**
   * \param evt Mouse event
   */
  virtual void OnMouseReleaseEvent(wxMouseEvent& evt);

  /// Handles mouse wheel events.
  /**
   * \param evt Mouse wheel event
   */
  virtual void OnMouseWheelEvent(wxMouseEvent& evt);

  /// Handles mouse capture lost events.
  /**
   * \param evt Mouse capture lost event
   */
  virtual void OnMouseCaptureLost(wxMouseCaptureLostEvent& evt);

private:
  /// Handles viewport changed events.
  virtual void OnViewPortChanged();

  /// Check if mouse is dragging
  bool IsDrag(wxMouseEvent& event);

  /// Paints the display
  void PaintDisplay();

  /// Synchronize the wxViewPortControl with wxChartViewer
  void SyncState();

  /// Update the mouse cursor
  void UpdateCursor(int position);

  /// Updates wxChartViewer if viewport has changed
  void UpdateChartViewerIfNecessary();

  /// Converts from mouse to image x-coordinate
  double ToImageX(int x);

  /// Converts from mouse to image y-coordinate
  double ToImageY(int y);

  wxChartViewer* m_chartViewer;           ///< Associated wxChartViewer
  BaseChart*     m_chart;                 ///< BaseChart object displayed in the control
  bool           m_reentrantGuard;        ///< Prevents infinite calling loops

  int  m_mouseDownX;                      ///< Current mouse x coordinates
  int  m_mouseDownY;                      ///< Current mouse y coordinates
  bool m_isPlotAreaMouseDown;             ///< Mouse left button is down in the plot area.
};

#if wxCHECK_VERSION(3,0,0)
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_CLICKED
wxDECLARE_EXPORTED_EVENT(WXDLLIMPEXP_WXCHARTVIEWER, wxEVT_CHARTVIEWER_CLICKED, wxCommandEvent);
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_VIEWPORT_CHANGED
wxDECLARE_EXPORTED_EVENT(WXDLLIMPEXP_WXCHARTVIEWER, wxEVT_CHARTVIEWER_VIEWPORT_CHANGED, wxCommandEvent);
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_MOUSEMOVE_CHART
wxDECLARE_EXPORTED_EVENT(WXDLLIMPEXP_WXCHARTVIEWER, wxEVT_CHARTVIEWER_MOUSEMOVE_CHART, wxCommandEvent);
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_MOUSELEAVE_CHART
wxDECLARE_EXPORTED_EVENT(WXDLLIMPEXP_WXCHARTVIEWER, wxEVT_CHARTVIEWER_MOUSELEAVE_CHART, wxCommandEvent);
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA
wxDECLARE_EXPORTED_EVENT(WXDLLIMPEXP_WXCHARTVIEWER, wxEVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA, wxCommandEvent);
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_MOUSELEAVE_PLOTAREA
wxDECLARE_EXPORTED_EVENT(WXDLLIMPEXP_WXCHARTVIEWER, wxEVT_CHARTVIEWER_MOUSELEAVE_PLOTAREA, wxCommandEvent);
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_MOUSEWHEEL
wxDECLARE_EXPORTED_EVENT(WXDLLIMPEXP_WXCHARTVIEWER, wxEVT_CHARTVIEWER_MOUSEWHEEL, wxCommandEvent);
#else
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_CLICKED
extern WXDLLIMPEXP_WXCHARTVIEWER const wxEventType wxEVT_CHARTVIEWER_CLICKED;
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_VIEWPORT_CHANGED
extern WXDLLIMPEXP_WXCHARTVIEWER const wxEventType wxEVT_CHARTVIEWER_VIEWPORT_CHANGED;
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_MOUSEMOVE_CHART
extern WXDLLIMPEXP_WXCHARTVIEWER const wxEventType wxEVT_CHARTVIEWER_MOUSEMOVE_CHART;
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_MOUSELEAVE_CHART
extern WXDLLIMPEXP_WXCHARTVIEWER const wxEventType wxEVT_CHARTVIEWER_MOUSELEAVE_CHART;
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA
extern WXDLLIMPEXP_WXCHARTVIEWER const wxEventType wxEVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA;
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_MOUSELEAVE_PLOTAREA
extern WXDLLIMPEXP_WXCHARTVIEWER const wxEventType wxEVT_CHARTVIEWER_MOUSELEAVE_PLOTAREA;
/// Declaration of chart viewer event wxEVT_CHARTVIEWER_MOUSEWHEEL
extern WXDLLIMPEXP_WXCHARTVIEWER const wxEventType wxEVT_CHARTVIEWER_MOUSEWHEEL;
#endif

/// Event emitted on left mouse button clicked on chart
#define EVT_CHARTVIEWER_CLICKED(winid, fn) \
  wx__DECLARE_EVT1(wxEVT_CHARTVIEWER_CLICKED, winid, wxCommandEventHandler(fn))
/// Event emitted when the viewport changed
#define EVT_CHARTVIEWER_VIEWPORT_CHANGED(winid, fn) \
  wx__DECLARE_EVT1(wxEVT_CHARTVIEWER_VIEWPORT_CHANGED, winid, wxCommandEventHandler(fn))
/// Event emitted on mouse moves over the chart area
#define EVT_CHARTVIEWER_MOUSEMOVE_CHART(winid, fn) \
  wx__DECLARE_EVT1(wxEVT_CHARTVIEWER_MOUSEMOVE_CHART, winid, wxCommandEventHandler(fn))
/// Event emitted when the mouse cursor leaves the chart area
#define EVT_CHARTVIEWER_MOUSELEAVE_CHART(winid, fn) \
  wx__DECLARE_EVT1(wxEVT_CHARTVIEWER_MOUSELEAVE_CHART, winid, wxCommandEventHandler(fn))
/// Event emitted on mouse moves over the plot area of a chart
#define EVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA(winid, fn) \
  wx__DECLARE_EVT1(wxEVT_CHARTVIEWER_MOUSEMOVE_PLOTAREA, winid, wxCommandEventHandler(fn))
/// Event emitted when the mouse cursor leaves the plot area of a chart
#define EVT_CHARTVIEWER_MOUSELEAVE_PLOTAREA(winid, fn) \
  wx__DECLARE_EVT1(wxEVT_CHARTVIEWER_MOUSELEAVE_PLOTAREA, winid, wxCommandEventHandler(fn))
/// Event emitted on mouse wheel activities
#define EVT_CHARTVIEWER_MOUSEWHEEL(winid, fn) \
  wx__DECLARE_EVT1(wxEVT_CHARTVIEWER_MOUSEWHEEL, winid, wxCommandEventHandler(fn))

#endif // WXCHARTDIR_WXCHARTVIEWER_H
