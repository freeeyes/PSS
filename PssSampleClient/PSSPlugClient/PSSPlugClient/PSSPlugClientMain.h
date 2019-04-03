/***************************************************************
 * Name:      PSSPlugClientMain.h
 * Purpose:   Defines Application Frame
 * Author:    smith ()
 * Created:   2019-02-22
 * Copyright: smith ()
 * License:
 **************************************************************/

#ifndef PSSPLUGCLIENTMAIN_H
#define PSSPLUGCLIENTMAIN_H

//(*Headers(PSSPlugClientFrame)
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
//*)

#include "CNetOperation.h"



class PSSPlugClientFrame: public wxFrame
{
    public:

        PSSPlugClientFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~PSSPlugClientFrame();

    private:

        //(*Handlers(PSSPlugClientFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnButton_SendDataClick(wxCommandEvent& event);
        void OnButton_StartTestClick(wxCommandEvent& event);
        void OnButton_StopTestClick(wxCommandEvent& event);
        void OnTimer_ShowDataTrigger(wxTimerEvent& event);
        //*)

        //(*Identifiers(PSSPlugClientFrame)
        static const long ID_STATICBOX1;
        static const long ID_STATICBOX2;
        static const long ID_STATICTEXT1;
        static const long ID_TEXTCTRL1;
        static const long ID_STATICTEXT2;
        static const long ID_TEXTCTRL2;
        static const long ID_STATICTEXT3;
        static const long ID_TEXTCTRL3;
        static const long ID_STATICTEXT4;
        static const long ID_TEXTCTRL4;
        static const long ID_BUTTON1;
        static const long ID_BUTTON2;
        static const long ID_BUTTON3;
        static const long ID_STATICTEXT5;
        static const long ID_TEXTCTRL5;
        static const long ID_STATICTEXT6;
        static const long ID_TEXTCTRL6;
        static const long ID_PANEL1;
        static const long ID_TIMER1;
        //*)

        //(*Declarations(PSSPlugClientFrame)
        wxButton* Button_SendData;
        wxButton* Button_StartTest;
        wxButton* Button_StopTest;
        wxPanel* Panel1;
        wxStaticBox* StaticBox1;
        wxStaticBox* StaticBox2;
        wxStaticText* StaticText1;
        wxStaticText* StaticText2;
        wxStaticText* StaticText3;
        wxStaticText* StaticText4;
        wxStaticText* StaticText5;
        wxStaticText* StaticText6;
        wxTextCtrl* TextCtrl_RecvCount;
        wxTextCtrl* TextCtrl_SendCount;
        wxTextCtrl* TextCtrl_SendData;
        wxTextCtrl* TextCtrl_ServerIP;
        wxTextCtrl* TextCtrl_ServerPort;
        wxTextCtrl* TextCtrl_ServerResponse;
        wxTimer Timer_ShowData;
        //*)

        DECLARE_EVENT_TABLE()


        private:
                void Init();
                bool Conn();
                
                void SendData(char* ParamText);

                void Show_Send_List();

        public:
                bool Send_Multiple_Plug();

private:
        _ServerInfo  m_objServerInfo;
        _PassTest    m_objPassTest;
        //CSockWrap       *m_sckClient;
        CNetOperation       *pNetOperation;
        bool         m_blIsRun;         //是否在进行压力测试

};

#endif // PSSPLUGCLIENTMAIN_H
