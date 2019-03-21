/***************************************************************
 * Name:      PassTCPDlg.h
 * Purpose:   Defines Application Frame
 * Author:    Smith ()
 * Created:   2019-02-17
 * Copyright: Smith ()
 * License:
 **************************************************************/

#ifndef CPASSTCPDLG_H
#define CPASSTCPDLG_H

//(*Headers(CPassTCPDlg)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/dirdlg.h>
#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/radiobox.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
//*)



#include "ClientTcpSocket.h"
#include "ClientUdpSocket.h"


using namespace std;

typedef vector<CClientTcpSocket* > vecClientTcpSocket;
typedef vector<CClientUdpSocket* > vecClientUdpSocket;



class CPassTCPDlg: public wxFrame
{
public:

    CPassTCPDlg(wxWindow* parent,wxWindowID id = -1);
    virtual ~CPassTCPDlg();

private:

    //(*Handlers(CPassTCPDlg)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnButton_LuaAdvanceClick(wxCommandEvent& event);
    void OnButton_StatTestClick(wxCommandEvent& event);
    void OnButton_StopTestClick(wxCommandEvent& event);
    void OnButton_ExportTestDataClick(wxCommandEvent& event);
    void OnButton_SetRadomPacketClick(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
    void OnTimer_TestTrigger(wxTimerEvent& event);
    void OnChoice_SendBuffStyleSelect(wxCommandEvent& event);
    void OnButton1Click(wxCommandEvent& event);
    void OnTextCtrl_ServerIPText(wxCommandEvent& event);
    //*)

    //(*Identifiers(CPassTCPDlg)
    static const long ID_STATICBOX1;
    static const long ID_STATICTEXT1;
    static const long ID_STATICTEXT2;
    static const long ID_STATICBOX2;
    static const long ID_STATICTEXT3;
    static const long ID_STATICTEXT4;
    static const long ID_STATICTEXT5;
    static const long ID_STATICBOX3;
    static const long ID_STATICTEXT6;
    static const long ID_TEXTCTRL1;
    static const long ID_CHECKBOX1;
    static const long ID_BUTTON_LuaAdvance;
    static const long ID_BUTTON_StatTest;
    static const long ID_BUTTON_StopTest;
    static const long ID_BUTTON_ExportTestData;
    static const long ID_BUTTON_SetRadomPacket;
    static const long ID_TEXTCTRL2;
    static const long ID_TEXTCTRL3;
    static const long ID_TEXTCTRL4;
    static const long ID_TEXTCTRL5;
    static const long ID_TEXTCTRL6;
    static const long ID_STATICTEXT7;
    static const long ID_STATICTEXT8;
    static const long ID_STATICTEXT9;
    static const long ID_STATICTEXT10;
    static const long ID_TEXTCTRL7;
    static const long ID_TEXTCTRL8;
    static const long ID_TEXTCTRL9;
    static const long ID_TEXTCTRL10;
    static const long ID_CHECKBOX2;
    static const long ID_CHECKBOX3;
    static const long ID_CHECKBOX4;
    static const long ID_CHECKBOX5;
    static const long ID_CHECKBOX6;
    static const long ID_CHECKBOX7;
    static const long ID_CHECKBOX8;
    static const long ID_RADIOBOX1;
    static const long ID_STATICTEXT11;
    static const long ID_STATICTEXT12;
    static const long ID_STATICTEXT13;
    static const long ID_STATICTEXT14;
    static const long ID_STATICTEXT15;
    static const long ID_STATICTEXT16;
    static const long ID_TEXTCTRL11;
    static const long ID_TEXTCTRL12;
    static const long ID_TEXTCTRL13;
    static const long ID_TEXTCTRL14;
    static const long ID_TEXTCTRL15;
    static const long ID_TEXTCTRL16;
    static const long ID_STATICTEXT17;
    static const long ID_STATICTEXT18;
    static const long ID_STATICTEXT19;
    static const long ID_TEXTCTRL17;
    static const long ID_TEXTCTRL18;
    static const long ID_TEXTCTRL19;
    static const long ID_STATICTEXT20;
    static const long ID_TEXTCTRL20;
    static const long ID_STATICTEXT21;
    static const long ID_TEXTCTRL21;
    static const long ID_STATICTEXT22;
    static const long ID_CHOICE1;
    static const long ID_TEXTCTRL22;
    static const long ID_PANEL1;
    static const long ID_TIMER_Test;
    //*)

    //(*Declarations(CPassTCPDlg)
    wxButton* Button_ExportTestData;
    wxButton* Button_LuaAdvance;
    wxButton* Button_SetRadomPacket;
    wxButton* Button_StatTest;
    wxButton* Button_StopTest;
    wxCheckBox* CheckBoxIsWriteLog;
    wxCheckBox* CheckBoxLuaAdvance;
    wxCheckBox* CheckBox_IsAlwayConnect;
    wxCheckBox* CheckBox_IsBroken;
    wxCheckBox* CheckBox_IsRecv;
    wxCheckBox* CheckBox_RadomSendCount;
    wxCheckBox* CheckBox_RadomaDelay;
    wxCheckBox* CheckBox_SendOne;
    wxChoice* Choice_SendBuffStyle;
    wxDirDialog* DirDialog_Lua;
    wxPanel* Panel1;
    wxRadioBox* RadioBox_nRadio;
    wxStaticBox* StaticBox1;
    wxStaticBox* StaticBox2;
    wxStaticBox* StaticBox3;
    wxStaticText* StaticText10;
    wxStaticText* StaticText11;
    wxStaticText* StaticText12;
    wxStaticText* StaticText13;
    wxStaticText* StaticText14;
    wxStaticText* StaticText15;
    wxStaticText* StaticText16;
    wxStaticText* StaticText17;
    wxStaticText* StaticText18;
    wxStaticText* StaticText19;
    wxStaticText* StaticText1;
    wxStaticText* StaticText20;
    wxStaticText* StaticText21;
    wxStaticText* StaticText22;
    wxStaticText* StaticText2;
    wxStaticText* StaticText3;
    wxStaticText* StaticText4;
    wxStaticText* StaticText5;
    wxStaticText* StaticText6;
    wxStaticText* StaticText7;
    wxStaticText* StaticText8;
    wxStaticText* StaticText9;
    wxTextCtrl* TextCtrl_ClientUdpPort;
    wxTextCtrl* TextCtrl_CurrConnect;
    wxTextCtrl* TextCtrl_FailConnect;
    wxTextCtrl* TextCtrl_FailRecv;
    wxTextCtrl* TextCtrl_FailSend;
    wxTextCtrl* TextCtrl_LuaControlFilePath;
    wxTextCtrl* TextCtrl_MaxTime;
    wxTextCtrl* TextCtrl_MinTime;
    wxTextCtrl* TextCtrl_PacketSendMax;
    wxTextCtrl* TextCtrl_PacketTimewait;
    wxTextCtrl* TextCtrl_RecvByteCount;
    wxTextCtrl* TextCtrl_RecvLength;
    wxTextCtrl* TextCtrl_RecvTimeOut;
    wxTextCtrl* TextCtrl_SendByteCount;
    wxTextCtrl* TextCtrl_SendText;
    wxTextCtrl* TextCtrl_ServerIP;
    wxTextCtrl* TextCtrl_ServerPort;
    wxTextCtrl* TextCtrl_SocketInterval;
    wxTextCtrl* TextCtrl_SuccessConnect;
    wxTextCtrl* TextCtrl_SuccessRecv;
    wxTextCtrl* TextCtrl_SuccessSend;
    wxTextCtrl* TextCtrl_TestThreadCount;
    wxTimer Timer_Test;
    //*)

    DECLARE_EVENT_TABLE()

private:
    void ClearResult();

private:

    vecClientTcpSocket m_vecClientTcpSocket;
    vecClientUdpSocket m_vecClientUdpSocket;

    wxDateTime m_tmBegin;             //压测开始时间
    wxDateTime m_tmEnd;               //压测停止时间

    bool  m_blIsRun;             //压测是否在进行
    int   m_nCurrTextStyle;      //当前文本风格
    CBaseDataLogic* m_pLogic;    //是否设置了最新的逻辑包

};

#endif // CPassTCPDlg_H
