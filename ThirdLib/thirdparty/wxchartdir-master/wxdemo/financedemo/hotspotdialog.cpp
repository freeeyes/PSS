/*
** Name:        hotspotdialog.cpp
** Purpose:     Hot Spot Dialog
** Author:      Ulrich Telle
** Created:     2018-05-15
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

#include "wx/imaglist.h"

#include "hotspotdialog.h"

/*
 * HotspotDialog type definition
 */

IMPLEMENT_DYNAMIC_CLASS( HotspotDialog, wxDialog )

/*
 * HotspotDialog event table definition
 */

BEGIN_EVENT_TABLE( HotspotDialog, wxDialog )

  EVT_BUTTON( wxID_OK, HotspotDialog::OnOkClick )

END_EVENT_TABLE()

/*
 * HotspotDialog constructors
 */

HotspotDialog::HotspotDialog()
{
  Init();
}

HotspotDialog::HotspotDialog( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  Init();
  Create(parent, id, caption, pos, size, style);
}


/*
 * HotspotDialog creator
 */

bool
HotspotDialog::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
  SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY|wxWS_EX_BLOCK_EVENTS);
  wxDialog::Create( parent, id, caption, pos, size, style );

  CreateControls();
  if (GetSizer())
  {
    GetSizer()->SetSizeHints(this);
  }
  Centre();

  return true;
}


/*
 * HotspotDialog destructor
 */

HotspotDialog::~HotspotDialog()
{
}


/*
 * Member initialisation
 */

void
HotspotDialog::Init()
{
}


/*
 * Control creation for HotspotDialog
 */

void
HotspotDialog::CreateControls()
{    
  HotspotDialog* itemDialog1 = this;

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  itemDialog1->SetSizer(itemBoxSizer2);

  m_propList = new wxListView(itemDialog1, ID_LISTCTRL, wxDefaultPosition, wxSize(200, -1), wxLC_REPORT | wxLC_SINGLE_SEL);
  itemBoxSizer2->Add(m_propList, 1, wxGROW|wxALL, 3);

  wxButton* itemButton2 = new wxButton( itemDialog1, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer2->Add(itemButton2, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  m_propList->InsertColumn(0, wxS("Parameter"));
  m_propList->InsertColumn(1, wxS("Value"), wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE_USEHEADER);
}


/*
 * Should we show tooltips?
 */

bool
HotspotDialog::ShowToolTips()
{
  return true;
}

/*
 * Get bitmap resources
 */

wxBitmap
HotspotDialog::GetBitmapResource( const wxString& name )
{
  // Bitmap retrieval
  wxUnusedVar(name);
  return wxNullBitmap;
}

/*
 * Get icon resources
 */

wxIcon
HotspotDialog::GetIconResource( const wxString& name )
{
  // Icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
}


/*
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void
HotspotDialog::OnOkClick( wxCommandEvent& event )
{
  EndModal(wxID_OK);
}

void
HotspotDialog::SetData(ImageMapHandler* hotSpotData)
{
  if (0 != hotSpotData)
  {
    for (int i = 0; 0 != hotSpotData->getKey(i); ++i)
    {
      wxString attr = wxString::FromUTF8(hotSpotData->getKey(i));
      wxString value = wxString::FromUTF8(hotSpotData->getValue(i));
      int tmp = m_propList->InsertItem(i, attr);
      m_propList->SetItem(tmp, 1, value);
    }
    m_propList->SetColumnWidth(0, wxLIST_AUTOSIZE);
  }
}
