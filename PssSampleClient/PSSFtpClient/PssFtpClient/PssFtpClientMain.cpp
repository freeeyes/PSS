/***************************************************************
 * Name:      PssFtpClientMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    smith ()
 * Created:   2019-02-21
 * Copyright: smith ()
 * License:
 **************************************************************/

#include "PssFtpClientMain.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(PssFtpClientFrame)
#include <wx/bitmap.h>
#include <wx/icon.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << wxT("-Windows");
#elif defined(__UNIX__)
        wxbuild << wxT("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << wxT("-Unicode build");
#else
        wxbuild << wxT("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}



//(*IdInit(PssFtpClientFrame)
const long PssFtpClientFrame::ID_STATICBOX1 = wxNewId();
const long PssFtpClientFrame::ID_STATICTEXT1 = wxNewId();
const long PssFtpClientFrame::ID_TEXTCTRL1 = wxNewId();
const long PssFtpClientFrame::ID_STATICTEXT2 = wxNewId();
const long PssFtpClientFrame::ID_TEXTCTRL2 = wxNewId();
const long PssFtpClientFrame::ID_STATICTEXT3 = wxNewId();
const long PssFtpClientFrame::ID_STATICTEXT4 = wxNewId();
const long PssFtpClientFrame::ID_TEXTCTRL3 = wxNewId();
const long PssFtpClientFrame::ID_TEXTCTRL4 = wxNewId();
const long PssFtpClientFrame::ID_BUTTON1 = wxNewId();
const long PssFtpClientFrame::ID_BUTTON2 = wxNewId();
const long PssFtpClientFrame::ID_STATICTEXT5 = wxNewId();
const long PssFtpClientFrame::ID_TEXTCTRL5 = wxNewId();
const long PssFtpClientFrame::ID_STATICTEXT6 = wxNewId();
const long PssFtpClientFrame::ID_TEXTCTRL6 = wxNewId();
const long PssFtpClientFrame::ID_BUTTON3 = wxNewId();
const long PssFtpClientFrame::ID_BUTTON4 = wxNewId();
const long PssFtpClientFrame::ID_STATICTEXT7 = wxNewId();
const long PssFtpClientFrame::ID_TEXTCTRL7 = wxNewId();
const long PssFtpClientFrame::ID_BUTTON5 = wxNewId();
const long PssFtpClientFrame::ID_LISTCTRL1 = wxNewId();
const long PssFtpClientFrame::ID_PANEL1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(PssFtpClientFrame,wxFrame)
    //(*EventTable(PssFtpClientFrame)
    //*)
END_EVENT_TABLE()

PssFtpClientFrame::PssFtpClientFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(PssFtpClientFrame)
    Create(parent, id, wxT("PSS FTP客户端"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
    SetClientSize(wxSize(747,418));
    {
    	wxIcon FrameIcon;
    	FrameIcon.CopyFromBitmap(wxBitmap(wxImage(_T("./Icon.jpg"))));
    	SetIcon(FrameIcon);
    }
    Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(168,120), wxSize(744,418), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    StaticBox1 = new wxStaticBox(Panel1, ID_STATICBOX1, wxT("FTP配置"), wxPoint(8,0), wxSize(728,104), 0, _T("ID_STATICBOX1"));
    StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT1, wxT("服务器IP"), wxPoint(16,32), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    TextCtrl_ServerIP = new wxTextCtrl(Panel1, ID_TEXTCTRL1, wxT("127.0.0.1"), wxPoint(88,24), wxSize(136,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    StaticText2 = new wxStaticText(Panel1, ID_STATICTEXT2, wxT("服务器端口"), wxPoint(248,32), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    TextCtrl_ServerPort = new wxTextCtrl(Panel1, ID_TEXTCTRL2, wxT("10002"), wxPoint(328,24), wxSize(136,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
    StaticText3 = new wxStaticText(Panel1, ID_STATICTEXT3, wxT("用户名"), wxPoint(16,72), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    StaticText4 = new wxStaticText(Panel1, ID_STATICTEXT4, wxT("密码"), wxPoint(248,72), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    TextCtrl_Username = new wxTextCtrl(Panel1, ID_TEXTCTRL3, wxT("freeeyes"), wxPoint(88,64), wxSize(136,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
    TextCtrl_Password = new wxTextCtrl(Panel1, ID_TEXTCTRL4, wxT("1234"), wxPoint(328,64), wxSize(136,35), wxTE_PASSWORD, wxDefaultValidator, _T("ID_TEXTCTRL4"));
    Button_Login = new wxButton(Panel1, ID_BUTTON1, wxT("登陆"), wxPoint(488,24), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    Button_Logout = new wxButton(Panel1, ID_BUTTON2, wxT("退出"), wxPoint(488,64), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    Button_Logout->Disable();
    StaticText5 = new wxStaticText(Panel1, ID_STATICTEXT5, wxT("保存路径"), wxPoint(16,120), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    TextCtrl_SavePath = new wxTextCtrl(Panel1, ID_TEXTCTRL5, wxT("./"), wxPoint(88,112), wxSize(192,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL5"));
    StaticText6 = new wxStaticText(Panel1, ID_STATICTEXT6, wxT("远程路径"), wxPoint(288,120), wxDefaultSize, 0, _T("ID_STATICTEXT6"));
    TextCtrl_RemotePath = new wxTextCtrl(Panel1, ID_TEXTCTRL6, wxT("./"), wxPoint(344,112), wxSize(194,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL6"));
    Button_Open = new wxButton(Panel1, ID_BUTTON3, wxT("打开"), wxPoint(552,112), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    Button_Download = new wxButton(Panel1, ID_BUTTON4, wxT("下载"), wxPoint(648,112), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
    StaticText7 = new wxStaticText(Panel1, ID_STATICTEXT7, wxT("上传文件名"), wxPoint(16,168), wxDefaultSize, 0, _T("ID_STATICTEXT7"));
    TextCtrl_UploadFilename = new wxTextCtrl(Panel1, ID_TEXTCTRL7, wxEmptyString, wxPoint(88,160), wxSize(184,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL7"));
    Button_Upload = new wxButton(Panel1, ID_BUTTON5, wxT("上传"), wxPoint(288,160), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON5"));
    ListCtrl_FileList = new wxListCtrl(Panel1, ID_LISTCTRL1, wxPoint(16,200), wxSize(712,208), wxLC_LIST|wxLC_REPORT, wxDefaultValidator, _T("ID_LISTCTRL1"));

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PssFtpClientFrame::OnButton_LoginClick);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PssFtpClientFrame::OnButton_LogoutClick);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PssFtpClientFrame::OnButton_OpenClick);
    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PssFtpClientFrame::OnButton_DownloadClick);
    Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PssFtpClientFrame::OnButton_UploadClick);
    //*)


    this->Init();
}

PssFtpClientFrame::~PssFtpClientFrame()
{
    //(*Destroy(PssFtpClientFrame)
    //*)
    delete pNetOperation;
}

void PssFtpClientFrame::OnQuit(wxCommandEvent& event)
{
    pNetOperation->Close();
}




void PssFtpClientFrame::Init()
{

    this->ListCtrl_FileList->InsertColumn(0, wxT("文件名称"), wxLIST_FORMAT_CENTER, 300);
	this->ListCtrl_FileList->InsertColumn(1, wxT("属性"), wxLIST_FORMAT_CENTER, 50);
	this->ListCtrl_FileList->InsertColumn(2, wxT("大小"), wxLIST_FORMAT_CENTER, 150);

	

	//初始化TCP链接
    pNetOperation =new CNetOperation(SOCK_STREAM);

}

void PssFtpClientFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, wxT("Welcome to..."));
}


void PssFtpClientFrame::OnButton_LogoutClick(wxCommandEvent& event)
{
	//设置登出行为
    wxCSConv cvGBK( wxT("GBK") );
    std::string    szRemotePath    =std::string( cvGBK.cWX2MB( this->TextCtrl_RemotePath->GetValue().c_str() ) );
    std::string    strUserName    =std::string( cvGBK.cWX2MB( this->TextCtrl_Username->GetValue().c_str() ) );
//
    _LoginInfo objLoginInfo;
	pNetOperation->Send_Logout(objLoginInfo,COMMAND_FTP_LOGOUT,strUserName);
	pNetOperation->Close();
	wxMessageBox(wxT("登出服务器成功。"), wxT("提示信息"), wxOK | wxICON_INFORMATION, this);
	this->Button_Login->Enable(true);
	this->Button_Logout->Enable(false);

}

void PssFtpClientFrame::OnButton_OpenClick(wxCommandEvent& event)
{

    //打开远程目录,并获得列表

    wxCSConv cvGBK( wxT("GBK") );
    std::string    szRemotePath    =std::string( cvGBK.cWX2MB( this->TextCtrl_RemotePath->GetValue().c_str() ) );
    std::string    strUserName    =std::string( cvGBK.cWX2MB( this->TextCtrl_Username->GetValue().c_str() ) );

	pNetOperation->Send_FileList(strUserName,szRemotePath);

}
void PssFtpClientFrame::OnButton_DownloadClick(wxCommandEvent& event)
{

	//启动后台线程下载
   // std::thread thread_download(&PssFtpClientFrame::DownLoadListFile,this);
  //  thread_download.detach(); // 等待线程结束

    this->DownLoadListFile();

}


void PssFtpClientFrame::OnButton_UploadClick(wxCommandEvent& event)
{
//上传文件

    wxCSConv cvGBK( wxT("GBK") );
    std::string    strRomoteFilePath    =std::string( cvGBK.cWX2MB( this->TextCtrl_RemotePath->GetValue().c_str() ) );
    std::string    strLocalPath    =std::string( cvGBK.cWX2MB( this->TextCtrl_SavePath->GetValue().c_str() ) );
    std::string    strLocalFile    =std::string( cvGBK.cWX2MB( this->TextCtrl_UploadFilename->GetValue().c_str() ) );
    std::string    strRemoteFile    =strRomoteFilePath+std::string("/")+strLocalFile;
    std::string    strUserName    =std::string( cvGBK.cWX2MB( this->TextCtrl_Username->GetValue().c_str() ) );

	pNetOperation->Send_Upload(strUserName,strRomoteFilePath,strLocalPath, strLocalFile);
}


void PssFtpClientFrame::OnButton_LoginClick(wxCommandEvent& event)
{
    
    std::string    strServerIP    =std::string(this->TextCtrl_ServerIP->GetValue());
    int nPort = wxAtoi(this->TextCtrl_ServerPort->GetValue());

    memcpy(m_ClientFTPInfo.szServerIP,strServerIP.c_str(),strServerIP.length());
    m_ClientFTPInfo.szServerIP[strServerIP.length()] = '\0';
    m_ClientFTPInfo.nServerPort =nPort;

    if(strServerIP == wxT("") || nPort == 0)
    {
        wxString msg = wxT("请输入完整的服务器IP地址和端口信息。");
        wxMessageBox(msg, wxT("服务器信息"), wxOK | wxICON_INFORMATION, this);
        return;
    }

	//获得相关测试信息
    wxCSConv cvGBK( wxT("GBK") );
    std::string    strUserName    =std::string( cvGBK.cWX2MB( this->TextCtrl_Username->GetValue().c_str() ) );
    std::string    strUserPass    =std::string( cvGBK.cWX2MB( this->TextCtrl_Password->GetValue().c_str() ) );
    


    memcpy(m_ClientFTPInfo.szUserName,strUserName.c_str(),strUserName.length());
    memcpy(m_ClientFTPInfo.szUserPass,strUserPass.c_str(),strUserPass.length());

	m_ClientFTPInfo.szUserName[strUserName.length()] = '\0';
	m_ClientFTPInfo.szUserPass[strUserPass.length()] = '\0';


	//判断socket是否已经连接，如果有连接则断开
	if(pNetOperation->Conn(strServerIP,nPort,1000) == false)
	{
		return;
	}

    _LoginInfo objLoginInfo;
	if(pNetOperation->Send_Login(objLoginInfo,COMMAND_FTP_LOGIN,strUserName,strUserPass) ==true )
	{
        wxMessageBox(wxT("连接远程服务器成功"), wxT("提示信息"), wxOK | wxICON_INFORMATION, this);
        this->Button_Login->Enable(false);
        this->Button_Logout->Enable(true);
	}
	else
	{
		pNetOperation->Close();
	}

}

void PssFtpClientFrame::DownLoadListFile()
{
    this->Button_Download->Enable(false);

	//下载文件
	for(int i = 0; i < this->ListCtrl_FileList->GetItemCount(); i++)
	{
		if( this->ListCtrl_FileList->GetItemState(i, wxLIST_STATE_SELECTED) == wxLIST_STATE_SELECTED )
		{
			//选中了下载文件
			int nBufferCount = 0;

			//得到文件名
            wxCSConv cvGBK( wxT("GBK") );
            std::string      strData =std::string( cvGBK.cWX2MB( this->TextCtrl_RemotePath->GetValue().c_str() ) );

			std::string      strLocalPath=std::string( cvGBK.cWX2MB( this->TextCtrl_SavePath->GetValue().c_str() ) );
			std::string     strPathName = std::string( cvGBK.cWX2MB( this->ListCtrl_FileList->GetItemText(i, 0).c_str() ) );
			std::string      strRomoteFilePath = strData + strPathName;

            std::string    strUserName    =std::string( cvGBK.cWX2MB( this->TextCtrl_Username->GetValue().c_str() ) );
            std::string    strUserPass    =std::string( cvGBK.cWX2MB( this->TextCtrl_Password->GetValue().c_str() ) );

			_DownloadFileInfo objDownloadFileInfo;

			//这里要注意，如果要设置更大的数据块，需要修改PSS的数据包最大大小的配置文件
			//默认PSS支持最大单数据包是20K，如果需要可以改的更大
			int nSize  = MAX_BUFF_10240;

			sprintf_s(objDownloadFileInfo.szLocalPath, MAX_BUFF_500, "%s", strLocalPath.c_str());
			sprintf_s(objDownloadFileInfo.szFileName, MAX_BUFF_500, "%s", strPathName.c_str());
			sprintf_s(objDownloadFileInfo.szRemotePath, MAX_BUFF_500, "%s", strRomoteFilePath.c_str());



			objDownloadFileInfo.nSize = nSize;

			int nIndex = 0;

			bool blState = pNetOperation->Send_Download(strUserName,strRomoteFilePath,strLocalPath, strPathName, nIndex, nSize, nBufferCount);
            
			if(blState == false)
			{
				wxMessageBox(wxT("远程文件下载失败"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
				continue;
			}

			for(int i = 1; i < nBufferCount; i++)
			{
                blState = pNetOperation->Send_Download(strUserName,strRomoteFilePath,strLocalPath, strPathName, i, nSize, nBufferCount);
            				
                if(blState == false)
				{
					wxMessageBox(wxT("远程文件下载失败"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
					break;
				}
			}
		}
	}

	wxMessageBox(wxT("下载文件完成"), wxT("提示信息"), wxOK | wxICON_INFORMATION, this);
	this->Button_Download->Enable(true);

}


