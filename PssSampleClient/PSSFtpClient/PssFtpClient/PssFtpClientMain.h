/***************************************************************
 * Name:      PssFtpClientMain.h
 * Purpose:   Defines Application Frame
 * Author:    smith ()
 * Created:   2019-02-21
 * Copyright: smith ()
 * License:
 **************************************************************/

#ifndef PSSFTPCLIENTMAIN_H
#define PSSFTPCLIENTMAIN_H
#include <vector>

#include "CNetOperation.h"

//(*Headers(PssFtpClientFrame)
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/listctrl.h>
#include <wx/panel.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)







struct _ClientFTPInfo
{
	char szServerIP[MAX_BUFF_50];
	int  nServerPort;
	char szUserName[MAX_BUFF_50];
	char szUserPass[MAX_BUFF_50];

	_ClientFTPInfo()
	{
		szServerIP[0] = '\0';
		nServerPort   = 0;
		szUserName[0] = '\0';
		szUserPass[0] = '\0';
	}
};

struct _DownloadFileInfo
{
	char szLocalPath[MAX_BUFF_500];
	char szFileName[MAX_BUFF_500];
	char szRemotePath[MAX_BUFF_500];
	int  nSize;
};

class PssFtpClientFrame: public wxFrame
{
    public:

        PssFtpClientFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~PssFtpClientFrame();

    private:

        //(*Handlers(PssFtpClientFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnButton_LoginClick(wxCommandEvent& event);
        void OnButton_LogoutClick(wxCommandEvent& event);
        void OnButton_OpenClick(wxCommandEvent& event);
        void OnButton_DownloadClick(wxCommandEvent& event);
        void OnButton5Click(wxCommandEvent& event);
        void OnButton_UploadClick(wxCommandEvent& event);
        //*)

        //(*Identifiers(PssFtpClientFrame)
        static const long ID_STATICBOX1;
        static const long ID_STATICTEXT1;
        static const long ID_TEXTCTRL1;
        static const long ID_STATICTEXT2;
        static const long ID_TEXTCTRL2;
        static const long ID_STATICTEXT3;
        static const long ID_STATICTEXT4;
        static const long ID_TEXTCTRL3;
        static const long ID_TEXTCTRL4;
        static const long ID_BUTTON1;
        static const long ID_BUTTON2;
        static const long ID_STATICTEXT5;
        static const long ID_TEXTCTRL5;
        static const long ID_STATICTEXT6;
        static const long ID_TEXTCTRL6;
        static const long ID_BUTTON3;
        static const long ID_BUTTON4;
        static const long ID_STATICTEXT7;
        static const long ID_TEXTCTRL7;
        static const long ID_BUTTON5;
        static const long ID_LISTCTRL1;
        static const long ID_PANEL1;
        //*)

        //(*Declarations(PssFtpClientFrame)
        wxButton* Button_Download;
        wxButton* Button_Login;
        wxButton* Button_Logout;
        wxButton* Button_Open;
        wxButton* Button_Upload;
        wxListCtrl* ListCtrl_FileList;
        wxPanel* Panel1;
        wxStaticBox* StaticBox1;
        wxStaticText* StaticText1;
        wxStaticText* StaticText2;
        wxStaticText* StaticText3;
        wxStaticText* StaticText4;
        wxStaticText* StaticText5;
        wxStaticText* StaticText6;
        wxStaticText* StaticText7;
        wxTextCtrl* TextCtrl_Password;
        wxTextCtrl* TextCtrl_RemotePath;
        wxTextCtrl* TextCtrl_SavePath;
        wxTextCtrl* TextCtrl_ServerIP;
        wxTextCtrl* TextCtrl_ServerPort;
        wxTextCtrl* TextCtrl_UploadFilename;
        wxTextCtrl* TextCtrl_Username;
        //*)

        DECLARE_EVENT_TABLE()


private:

	void Init();
/*	void Close();
	bool Conn();

	bool Send_Login();
	bool Send_Logout();
	bool Send_FileList(const char* pRemotePath);
	bool Send_Download(const char* pLocalPath, const char* pFileName, const char* pRemotePath, int nIndex, int nSize, int& nBockCount);
	bool Send_Upload(const char* pLocalPath, const char* pFileName, const char* pRemotePath);
*/

private:
	CNetOperation       *pNetOperation;
	_ClientFTPInfo m_ClientFTPInfo;

public:
	void DownLoadListFile();

};

#endif // PSSFTPCLIENTMAIN_H
