/*
** Name:        hotspotdialog.h
** Purpose:     Hot Spot Dialog
** Author:      Ulrich Telle
** Created:     2018-05-15
** Copyright:   (C) 2018, Ulrich Telle
** License:     LGPL - 3.0 + WITH WxWindows - exception - 3.1
*/

#ifndef HOTSPOTDIALOG_H_
#define HOTSPOTDIALOG_H_

/*!
 * Includes
 */

#include "wx/listctrl.h"

#include "chartdir.h"

/*!
 * Control identifiers
 */

#define ID_HOTSPOTDIALOG 10000
#define ID_LISTCTRL      10001
#define SYMBOL_HOTSPOTDIALOG_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX|wxTAB_TRAVERSAL
#define SYMBOL_HOTSPOTDIALOG_TITLE _("Hotspot Dialog")
#define SYMBOL_HOTSPOTDIALOG_IDNAME ID_HOTSPOTDIALOG
#define SYMBOL_HOTSPOTDIALOG_SIZE wxSize(400, 300)
#define SYMBOL_HOTSPOTDIALOG_POSITION wxDefaultPosition

/*!
 * HotspotDialog class declaration
 */

class HotspotDialog: public wxDialog
{    
  DECLARE_DYNAMIC_CLASS( HotspotDialog )
  DECLARE_EVENT_TABLE()

public:
  /// Constructors
  HotspotDialog();
  HotspotDialog( wxWindow* parent, wxWindowID id = SYMBOL_HOTSPOTDIALOG_IDNAME, const wxString& caption = SYMBOL_HOTSPOTDIALOG_TITLE, const wxPoint& pos = SYMBOL_HOTSPOTDIALOG_POSITION, const wxSize& size = SYMBOL_HOTSPOTDIALOG_SIZE, long style = SYMBOL_HOTSPOTDIALOG_STYLE );

  /// Creation
  bool Create( wxWindow* parent, wxWindowID id = SYMBOL_HOTSPOTDIALOG_IDNAME, const wxString& caption = SYMBOL_HOTSPOTDIALOG_TITLE, const wxPoint& pos = SYMBOL_HOTSPOTDIALOG_POSITION, const wxSize& size = SYMBOL_HOTSPOTDIALOG_SIZE, long style = SYMBOL_HOTSPOTDIALOG_STYLE );

  /// Destructor
  ~HotspotDialog();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  void SetData(ImageMapHandler* hotSpotData);

  // HotspotDialog event handler declarations

  /// wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
  void OnOkClick( wxCommandEvent& event );

  // HotspotDialog member function declarations

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource( const wxString& name );

  /// Retrieves icon resources
  wxIcon GetIconResource( const wxString& name );

  /// Should we show tooltips?
  static bool ShowToolTips();

  // HotspotDialog member variables
    wxListView* m_propList;
};

#endif
    // HOTSPOTDIALOG_H_
