#ifndef CPACKETDLGG_H
#define CPACKETDLGG_H

//(*Headers(CPacketDlg)
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/panel.h>
#include <wx/radiobox.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)


#include "define.h"


class CPacketDlg: public wxDialog
{
public:

    CPacketDlg(wxWindow* parent,wxWindowID id=wxID_ANY);
    virtual ~CPacketDlg();

    //(*Declarations(CPacketDlg)
    wxButton* Button_AddPacket;
    wxButton* Button_CloseDialog;
    wxButton* Button_DeletePacketItem;
    wxButton* Button_LoadPacketFile;
    wxButton* Button_PreviewSendResult;
    wxButton* Button_SavePacketToFile;
    wxListBox* ListBox_PacketList;
    wxListBox* ListBox_PacketPreviewList;
    wxPanel* Panel1;
    wxRadioBox* RadioBox_PacketType;
    wxStaticBox* StaticBox1;
    wxStaticBox* StaticBox2;
    wxStaticText* StaticText1;
    wxStaticText* StaticText2;
    wxStaticText* StaticText3;
    wxTextCtrl* TextCtrl_PacketData;
    wxTextCtrl* TextCtrl_RecvLength;
    //*)

protected:

    //(*Identifiers(CPacketDlg)
    static const long ID_STATICBOX1;
    static const long ID_STATICBOX2;
    static const long ID_STATICTEXT2;
    static const long ID_TEXTCTRL_PacketData;
    static const long ID_BUTTON_AddPacket;
    static const long ID_RADIOBOX_PacketType;
    static const long ID_STATICTEXT3;
    static const long ID_TEXTCTRL_RecvLength;
    static const long ID_BUTTON_SavePacketToFile;
    static const long ID_BUTTON_LoadPacketFile;
    static const long ID_BUTTON4;
    static const long ID_BUTTON_PreviewSendResult;
    static const long ID_BUTTON_CloseDialog;
    static const long ID_LISTBOX_PacketList;
    static const long ID_LISTBOX_PacketPreviewList;
    static const long ID_PANEL1;
    //*)

private:

    //(*Handlers(CPacketDlg)
    void OnPanel1Paint(wxPaintEvent& event);
    void OnButton_AddPacketClick(wxCommandEvent& event);
    void OnButton_SavePacketToFileClick(wxCommandEvent& event);
    void OnButton_LoadPacketFileClick(wxCommandEvent& event);
    void OnButton_DeletePacketItemClick(wxCommandEvent& event);
    void OnButton_PreviewSendResultClick(wxCommandEvent& event);
    void OnButton_CloseDialogClick(wxCommandEvent& event);
    //*)

    DECLARE_EVENT_TABLE()


public:
    void SetBaseDataLogic(CBaseDataLogic* pBaseDataLogic);

private:
    void ShowPacketList();

private:
    CBaseDataLogic* m_pBaseDataLogic;


};

#endif
