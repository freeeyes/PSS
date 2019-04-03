/***************************************************************
 * Name:      PSSLoginClientMain.h
 * Purpose:   Defines Application Frame
 * Author:    smith ()
 * Created:   2019-02-21
 * Copyright: smith ()
 * License:
 **************************************************************/

#ifndef PSSLOGINCLIENTMAIN_H
#define PSSLOGINCLIENTMAIN_H

//(*Headers(PSSLoginClientFrame)
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/listctrl.h>
#include <wx/panel.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
//*)

#include <vector>


#include "CNetOperation.h"


using namespace std;

class PSSLoginClientFrame: public wxFrame
{
    public:

        PSSLoginClientFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~PSSLoginClientFrame();

    private:

        //(*Handlers(PSSLoginClientFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnClose(wxCloseEvent& event);
        void OnButton_StartTestClick(wxCommandEvent& event);
        void OnButton_StopTestClick(wxCommandEvent& event);
        void OnButton_SetParamClick(wxCommandEvent& event);
        void OnButton_UserID_OnceTestClick(wxCommandEvent& event);
        void OnButton_LoginOnceTestClick(wxCommandEvent& event);
        void OnTimer_MainFormTrigger(wxTimerEvent& event);
        //*)

        //(*Identifiers(PSSLoginClientFrame)
        static const long ID_STATICBOX1;
        static const long ID_STATICBOX2;
        static const long ID_STATICBOX3;
        static const long ID_STATICTEXT1;
        static const long ID_TEXTCTRL1;
        static const long ID_STATICTEXT2;
        static const long ID_STATICTEXT3;
        static const long ID_STATICTEXT4;
        static const long ID_STATICTEXT5;
        static const long ID_TEXTCTRL2;
        static const long ID_TEXTCTRL3;
        static const long ID_TEXTCTRL4;
        static const long ID_STATICTEXT6;
        static const long ID_TEXTCTRL5;
        static const long ID_TEXTCTRL6;
        static const long ID_STATICTEXT7;
        static const long ID_TEXTCTRL7;
        static const long ID_BUTTON1;
        static const long ID_BUTTON2;
        static const long ID_BUTTON3;
        static const long ID_STATICTEXT8;
        static const long ID_TEXTCTRL8;
        static const long ID_BUTTON4;
        static const long ID_STATICTEXT9;
        static const long ID_TEXTCTRL9;
        static const long ID_STATICTEXT10;
        static const long ID_STATICTEXT11;
        static const long ID_TEXTCTRL10;
        static const long ID_TEXTCTRL11;
        static const long ID_BUTTON5;
        static const long ID_LISTCTRL1;
        static const long ID_PANEL1;
        static const long ID_TIMER1;
        //*)

        //(*Declarations(PSSLoginClientFrame)
        wxButton* Button_LoginOnceTest;
        wxButton* Button_SetParam;
        wxButton* Button_StartTest;
        wxButton* Button_StopTest;
        wxButton* Button_UserID_OnceTest;
        wxListCtrl* ListCtrl_Result;
        wxPanel* Panel1;
        wxStaticBox* StaticBox1;
        wxStaticBox* StaticBox2;
        wxStaticBox* StaticBox3;
        wxStaticText* StaticText10;
        wxStaticText* StaticText11;
        wxStaticText* StaticText1;
        wxStaticText* StaticText2;
        wxStaticText* StaticText3;
        wxStaticText* StaticText4;
        wxStaticText* StaticText5;
        wxStaticText* StaticText6;
        wxStaticText* StaticText7;
        wxStaticText* StaticText8;
        wxStaticText* StaticText9;
        wxTextCtrl* TextCtrl_Life;
        wxTextCtrl* TextCtrl_Magic;
        wxTextCtrl* TextCtrl_Password;
        wxTextCtrl* TextCtrl_RadomEnd;
        wxTextCtrl* TextCtrl_RadomStart;
        wxTextCtrl* TextCtrl_ServerIP;
        wxTextCtrl* TextCtrl_ServerPort;
        wxTextCtrl* TextCtrl_TimeUsed;
        wxTextCtrl* TextCtrl_UserID_Once;
        wxTextCtrl* TextCtrl_UserID_Set;
        wxTextCtrl* TextCtrl_Username;
        wxTimer Timer_MainForm;
        //*)

        DECLARE_EVENT_TABLE()




public:
	bool Send_Multiple_Login();

private:
	void Init();

	bool Conn();
	int  Random(int nStart, int nEnd);


	bool Send_Single_Login();


	// 更新指定项测试数据
	void updateItemData(_LoginInfo& objLoginInfo, const int nItemIndex);



private:
	CNetOperation       *pNetOperation;


private:
	vecLoginInfo m_vecLoginInfo;
	_ServerInfo  m_objServerInfo;
	_LoginClient m_objLoginClient;
	//CSockWrap       *m_sckClient;
	bool         m_blMultiple;

	int	m_nLastSecondSendCount;
	int          m_nSendCount;



};

#endif // PSSLOGINCLIENTMAIN_H
