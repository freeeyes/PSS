/*
** Name:        xyzoomscrollapp.cpp
** Purpose:     XY Zooming and Scrolling
** Author:      Ulrich Telle
** Created:     2018-05-09
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef XYZOOMSCROLL_H_
#define XYZOOMSCROLL_H_

/*!
 * Includes
 */

#include "wx/frame.h"
#include "wx/tglbtn.h"

#include "wxchartviewer.h"

#include <vector>

/*!
 * Control identifiers
 */

#define ID_XYZOOMSCROLL 10000
#define ID_CHARTVIEWER  10001
#define ID_POINTER      10002
#define ID_SLIDER       10003
#define ID_VIEWPORT     10004
#define SYMBOL_XYZOOMSCROLL_STYLE wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_XYZOOMSCROLL_TITLE _("XY Zooming and Scrolling")
#define SYMBOL_XYZOOMSCROLL_IDNAME ID_XYZOOMSCROLL
#define SYMBOL_XYZOOMSCROLL_SIZE wxSize(650, 520)
#define SYMBOL_XYZOOMSCROLL_POSITION wxDefaultPosition

/*!
 * XYZoomScroll class declaration
 */

class XyZoomScroll: public wxFrame
{    
  DECLARE_CLASS( XyZoomScroll )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  XyZoomScroll();
  XyZoomScroll( wxWindow* parent, wxWindowID id = SYMBOL_XYZOOMSCROLL_IDNAME, const wxString& caption = SYMBOL_XYZOOMSCROLL_TITLE, const wxPoint& pos = SYMBOL_XYZOOMSCROLL_POSITION, const wxSize& size = SYMBOL_XYZOOMSCROLL_SIZE, long style = SYMBOL_XYZOOMSCROLL_STYLE );

  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_XYZOOMSCROLL_IDNAME, const wxString& caption = SYMBOL_XYZOOMSCROLL_TITLE, const wxPoint& pos = SYMBOL_XYZOOMSCROLL_POSITION, const wxSize& size = SYMBOL_XYZOOMSCROLL_SIZE, long style = SYMBOL_XYZOOMSCROLL_STYLE );

  /// Destructor
  ~XyZoomScroll();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  // Load data into data arrays
  void LoadData();

  // Draw chart
  void DrawChart(wxChartViewer* viewer);
  void DrawFullChart(wxViewPortControl* vpc, wxChartViewer* viewer);
  void CrossHair(XYChart* c, int mouseX, int mouseY);

  // Update image map
  void UpdateImageMap(wxChartViewer* viewer);

  // Update other controls when viewport changed
  void UpdateControls(wxChartViewer* viewer);

  // utility to convert std::vector to a DoubleArray
  DoubleArray vectorToArray(std::vector<double> &v);

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for ID_POINTER
  void OnPointerClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_IN
  void OnZoomInClick( wxCommandEvent& event );

  /// wxEVT_COMMAND_CHECKBOX_CLICKED event handler for wxID_ZOOM_OUT
  void OnZoomOutClick( wxCommandEvent& event );

  void OnSave(wxCommandEvent& event);

  /// wxEVT_COMMAND_SLIDER_UPDATED event handler for ID_SLIDER
  void OnSliderUpdated( wxCommandEvent& event );

  void OnClickChart(wxCommandEvent& event);

  void OnZoomBarChanged(int value);

  void OnMouseMovePlotArea(wxCommandEvent& event);
  void OnMouseUsageChanged(int mouseUsage);
  void OnViewPortChanged(wxCommandEvent& event);

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );

  /// Should we show tooltips?
  static bool ShowToolTips();

  wxToggleButton* m_pointerButton;
  wxToggleButton* m_zoominButton;
  wxToggleButton* m_zoomoutButton;
  wxButton*       m_saveButton;
  wxColour        m_bgColour;
  wxSlider*       m_zoomBar;

  // Data arrays for the scrollable / zoomable chart.
  std::vector<double> dataX0;
  std::vector<double> dataY0;
  std::vector<double> dataX1;
  std::vector<double> dataY1;
  std::vector<double> dataX2;
  std::vector<double> dataY2;

  wxChartViewer* m_chartViewer;
  wxViewPortControl* m_viewPortControl;
};

#endif
    // XYZOOMSCROLL_H_
