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
    	FrameIcon.CopyFromBitmap(wxBitmap(wxImage(_T("/data/develop/Lib/Network/PSS-master/Icon.jpg"))));
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


    Init();
}

PssFtpClientFrame::~PssFtpClientFrame()
{
    //(*Destroy(PssFtpClientFrame)
    //*)
}

void PssFtpClientFrame::OnQuit(wxCommandEvent& event)
{

    Close();
}
void PssFtpClientFrame::Close()
{
    if(IsValidSocketHandle(m_sckClient->GetHandle()) )
    {
            m_sckClient->Close();
    }
}



void PssFtpClientFrame::Init()
{

    this->ListCtrl_FileList->InsertColumn(0, wxT("文件名称"), wxLIST_FORMAT_CENTER, 300);
	this->ListCtrl_FileList->InsertColumn(1, wxT("属性"), wxLIST_FORMAT_CENTER, 50);
	this->ListCtrl_FileList->InsertColumn(2, wxT("大小"), wxLIST_FORMAT_CENTER, 150);


	//初始化TCP链接

    InitializeSocketEnvironment();

    m_sckClient =new CSockWrap(SOCK_STREAM);

}

void PssFtpClientFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, wxT("Welcome to..."));
}


void PssFtpClientFrame::OnButton_LogoutClick(wxCommandEvent& event)
{
	//设置登出行为
	Send_Logout();
	Close();
	wxMessageBox(wxT("登出服务器成功。"), wxT("提示信息"), wxOK | wxICON_INFORMATION, this);
	this->Button_Login->Enable(true);
	this->Button_Logout->Enable(false);

}

void PssFtpClientFrame::OnButton_OpenClick(wxCommandEvent& event)
{

    //打开远程目录,并获得列表

    wxCSConv cvGBK( wxT("GBK") );
    std::string    szRemotePath    =std::string( cvGBK.cWX2MB( this->TextCtrl_RemotePath->GetValue().c_str() ) );

	Send_FileList(szRemotePath.c_str());

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

	Send_Upload(strLocalPath.c_str(), strLocalFile.c_str(), strRemoteFile.c_str());
}






void PssFtpClientFrame::OnButton_LoginClick(wxCommandEvent& event)
{

	// TODO: 在此添加控件通知处理程序代码

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
    std::string    strUserNmae    =std::string( cvGBK.cWX2MB( this->TextCtrl_Username->GetValue().c_str() ) );
    std::string    strUserPass    =std::string( cvGBK.cWX2MB( this->TextCtrl_Password->GetValue().c_str() ) );

    memcpy(m_ClientFTPInfo.szUserName,strUserNmae.c_str(),strUserNmae.length());
    memcpy(m_ClientFTPInfo.szUserPass,strUserPass.c_str(),strUserPass.length());

	m_ClientFTPInfo.szUserName[strUserNmae.length()] = '\0';
	m_ClientFTPInfo.szUserPass[strUserPass.length()] = '\0';



	//判断socket是否已经连接，如果有连接则断开
    if(IsValidSocketHandle(m_sckClient->GetHandle()) )
    {
        Close();
    }

	if(Conn() == false)
	{
		return;
	}

	if(true == Send_Login())
	{
        wxMessageBox(wxT("连接远程服务器成功"), wxT("提示信息"), wxOK | wxICON_INFORMATION, this);
        this->Button_Login->Enable(false);
        this->Button_Logout->Enable(true);
	}
	else
	{
		Close();
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


			_DownloadFileInfo objDownloadFileInfo;

			//这里要注意，如果要设置更大的数据块，需要修改PSS的数据包最大大小的配置文件
			//默认PSS支持最大单数据包是20K，如果需要可以改的更大
			int nSize  = MAX_BUFF_10240;

			sprintf_s(objDownloadFileInfo.szLocalPath, MAX_BUFF_500, "%s", strLocalPath.c_str());
			sprintf_s(objDownloadFileInfo.szFileName, MAX_BUFF_500, "%s", strPathName.c_str());
			sprintf_s(objDownloadFileInfo.szRemotePath, MAX_BUFF_500, "%s", strRomoteFilePath.c_str());



			objDownloadFileInfo.nSize = nSize;

			int nIndex = 0;

			bool blState = Send_Download(strLocalPath.c_str(), strPathName.c_str(), strRomoteFilePath.c_str(), nIndex, nSize, nBufferCount);
			if(blState == false)
			{
				wxMessageBox(wxT("远程文件下载失败"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
				continue;
			}

			for(int i = 1; i < nBufferCount; i++)
			{
				blState = Send_Download(strLocalPath.c_str(), strPathName.c_str(), strRomoteFilePath.c_str(), i, nSize, nBufferCount);
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

bool PssFtpClientFrame::Conn()
{
	//socket创建的准备工作

    m_sckClient->SetAddress(m_ClientFTPInfo.szServerIP, m_ClientFTPInfo.nServerPort);
    int TimeOut = 100;
    m_sckClient->SetTimeOut(TimeOut,-1,-1);

    m_sckClient->Reopen(true);
    if  (GetLastSocketError() !=0)
    {
        wxMessageBox(wxT("连接远程服务器失败。"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
        return  false;
    }

    return true;

}


bool PssFtpClientFrame::Send_Login()
{
	int nCommand = COMMAND_LOGIN;
	char szSendBuff[MAX_BUFF_500] = {'\0'};
	if(m_sckClient == INVALID_SOCKET)
	{
		return false;
	}

	short sVersion = 1;
	int nPos       = 0;
	int nLen       = 1 + 1 + (int)strlen(m_ClientFTPInfo.szUserName) + (int)strlen(m_ClientFTPInfo.szUserPass);
	char szSession[32] = {'\0'};

	sprintf_s(szSession, 32, "FREEEYES");

	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&sVersion, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nCommand, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nLen, sizeof(int));
	nPos += sizeof(int);
	memcpy_s(&szSendBuff[nPos], sizeof(char)*32, (char*)szSession, sizeof(char)*32);
	nPos += sizeof(char)*32;

	int nStrLen = (int)strlen(m_ClientFTPInfo.szUserName);
	memcpy_s(&szSendBuff[nPos], sizeof(char), (char*)&nStrLen, sizeof(char));
	nPos += sizeof(char);
	memcpy_s(&szSendBuff[nPos], nStrLen, (char*)m_ClientFTPInfo.szUserName, nStrLen);
	nPos += nStrLen;

	nStrLen = (int)strlen(m_ClientFTPInfo.szUserPass);
	memcpy_s(&szSendBuff[nPos], sizeof(char), (char*)&nStrLen, sizeof(char));
	nPos += sizeof(char);
	memcpy_s(&szSendBuff[nPos], nStrLen, (char*)m_ClientFTPInfo.szUserPass, nStrLen);
	nPos += nStrLen;

	int nTotalSendLen = nPos;
	int nBeginSend    = 0;
	int nCurrSendLen  = 0;
	bool blSendFlag   = false;
	int nBeginRecv    = 0;
	int nCurrRecvLen  = 0;
	bool blRecvFlag   = false;
	while(true)
	{
        transresult_t   rt  =m_sckClient->Send(szSendBuff + nBeginSend, nTotalSendLen);
        nCurrSendLen    =rt.nbytes;
		if(nCurrSendLen <= 0)
		{
            int dwError = GetLastSocketError();
            wxMessageBox(wxT("远程服务器发送数据失败"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
			return false;
		}
		else
		{
			nTotalSendLen -= nCurrSendLen;
			if(nTotalSendLen == 0)
			{
				//发送完成
				blSendFlag = true;
				break;
			}
			else
			{
				nBeginSend += nCurrSendLen;
			}
		}
	}

	//先接收四字节的数据包长度
	char szRecvLength[4] = {'\0'};
    transresult_t  rt  =m_sckClient->Recv( (char* )szRecvLength, 4);
    nCurrRecvLen    =rt.nbytes;

	if(nCurrRecvLen != 4)
	{
        int dwError = GetLastSocketError();
        wxMessageBox(wxT("远程服务器接收数据失败"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
		return false;
	}
	int nRecvLength = 0;
	memcpy_s(&nRecvLength, sizeof(int), szRecvLength, sizeof(int));
	char* pRecvBuff = new char[nRecvLength];
	int nRecvBegin  = 0;

	while(true)
	{
		if(nRecvLength - nRecvBegin == 0)
		{
			break;
		}

		//如果发送成功了，则处理接收数据
        transresult_t  rt  =m_sckClient->Recv( (char* )pRecvBuff + nRecvBegin, nRecvLength - nRecvBegin);
        nCurrRecvLen    =rt.nbytes;

		if(nCurrRecvLen <= 0)
		{
            int dwError = GetLastSocketError();
            wxMessageBox(wxT("远程服务器发送数据失败"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
			return false;
		}
		else
		{
			nRecvBegin += nCurrRecvLen;
		}
	}

	int nRecvCommandID = 0;
	int nRet           = 0;
	nPos               = 0;

	memcpy_s((char*)&nRecvCommandID,  sizeof(short), &pRecvBuff[nPos], sizeof(short));
	nPos += sizeof(short);
	memcpy_s((char*)&nRet,  sizeof(int), &pRecvBuff[nPos], sizeof(int));
	nPos += sizeof(int);

	if(nRet == LOGIN_FAIL_NOEXIST)
	{
		wxMessageBox(wxT("用户名不存在"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
		return false;
	}
	else if(nRet == LOGIN_FAIL_ONLINE)
	{
		wxMessageBox(wxT("此用户正在线，不能登录"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
		return false;
	}
	else if(nRet == LOGIN_FAIL_PASSWORD)
	{
		wxMessageBox(wxT(""), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
		return false;
	}

	return true;
}

bool PssFtpClientFrame::Send_Logout()
{
	int nCommand = COMMAND_LOGOUT;
	char szSendBuff[MAX_BUFF_500] = {'\0'};
	if(m_sckClient == INVALID_SOCKET)
	{
		return false;
	}

	short sVersion = 1;
	int nPos       = 0;
	int nLen       = 1 + (int)strlen(m_ClientFTPInfo.szUserName);
	char szSession[32] = {'\0'};

	sprintf_s(szSession, 32, "FREEEYES");

	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&sVersion, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nCommand, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nLen, sizeof(int));
	nPos += sizeof(int);
	memcpy_s(&szSendBuff[nPos], sizeof(char)*32, (char*)szSession, sizeof(char)*32);
	nPos += sizeof(char)*32;

	int nStrLen = (int)strlen(m_ClientFTPInfo.szUserName);
	memcpy_s(&szSendBuff[nPos], sizeof(char), (char*)&nStrLen, sizeof(char));
	nPos += sizeof(char);
	memcpy_s(&szSendBuff[nPos], nStrLen, (char*)m_ClientFTPInfo.szUserName, nStrLen);
	nPos += nStrLen;

	int nTotalSendLen = nPos;
	int nBeginSend    = 0;
	int nCurrSendLen  = 0;
	bool blSendFlag   = false;
	int nBeginRecv    = 0;
	int nCurrRecvLen  = 0;
	bool blRecvFlag   = false;
	while(true)
	{
		//nCurrSendLen = send(m_sckClient, szSendBuff + nBeginSend, nTotalSendLen, 0);

        transresult_t   rt  =m_sckClient->Send(szSendBuff + nBeginSend, nTotalSendLen);
        nCurrSendLen    =rt.nbytes;

        if(nCurrSendLen <= 0)
		{
            int dwError = GetLastSocketError();
            wxMessageBox(wxT("远程服务器发送数据失败"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
			return false;
		}
		else
		{
			nTotalSendLen -= nCurrSendLen;
			if(nTotalSendLen == 0)
			{
				//发送完成
				blSendFlag = true;
				break;
			}
			else
			{
				nBeginSend += nCurrSendLen;
			}
		}
	}

	//先接收四字节的数据包长度
	char szRecvLength[4] = {'\0'};
	//nCurrRecvLen = recv(m_sckClient, (char* )szRecvLength, 4, 0);

    transresult_t  rt  =m_sckClient->Recv( (char* )szRecvLength, 4);
    nCurrRecvLen    =rt.nbytes;

	if(nCurrRecvLen != 4)
	{
        int dwError = GetLastSocketError();
        wxMessageBox(wxT("远程服务器接收数据失败"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
		return false;
	}

	int nRecvLength = 0;
	memcpy_s(&nRecvLength, sizeof(int), szRecvLength, sizeof(int));
	char* pRecvBuff = new char[nRecvLength];
	int nRecvBegin  = 0;

	while(true)
	{
		if(nRecvLength - nRecvBegin == 0)
		{
			break;
		}

		//如果发送成功了，则处理接收数据
        transresult_t  rt  =m_sckClient->Recv( (char* )pRecvBuff + nRecvBegin, nRecvLength - nRecvBegin);
        nCurrRecvLen    =rt.nbytes;

		if(nCurrRecvLen <= 0)
		{
            int dwError = GetLastSocketError();
            wxMessageBox(wxT("远程服务器发送数据失败"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
			return false;
		}
		else
		{
			nRecvBegin += nCurrRecvLen;
		}
	}

	int nRecvCommandID = 0;
	int nRet           = 0;
	nPos               = 0;

	memcpy_s((char*)&nRecvCommandID,  sizeof(short), &pRecvBuff[nPos], sizeof(short));
	nPos += sizeof(short);
	memcpy_s((char*)&nRet,  sizeof(int), &pRecvBuff[nPos], sizeof(int));
	nPos += sizeof(int);

	return true;
}

bool PssFtpClientFrame::Send_FileList(const char* pRemotePath)
{
	this->ListCtrl_FileList->DeleteAllItems();
	int nCommand = COMMAND_FILELIST;
	char szSendBuff[MAX_BUFF_500] = {'\0'};
   if(IsValidSocketHandle(m_sckClient->GetHandle()) )
    {
        return false;
    }

	short sVersion = 1;
	int nPos       = 0;
	int nLen       = 1 + 2 + (int)strlen(m_ClientFTPInfo.szUserName) + (int)strlen(pRemotePath);
	char szSession[32] = {'\0'};

	sprintf_s(szSession, 32, "FREEEYES");

	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&sVersion, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nCommand, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nLen, sizeof(int));
	nPos += sizeof(int);
	memcpy_s(&szSendBuff[nPos], sizeof(char)*32, (char*)szSession, sizeof(char)*32);
	nPos += sizeof(char)*32;

	int nStrLen = (int)strlen(m_ClientFTPInfo.szUserName);
	memcpy_s(&szSendBuff[nPos], sizeof(char), (char*)&nStrLen, sizeof(char));
	nPos += sizeof(char);
	memcpy_s(&szSendBuff[nPos], nStrLen, (char*)m_ClientFTPInfo.szUserName, nStrLen);
	nPos += nStrLen;

	nStrLen = (int)strlen(pRemotePath);
	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nStrLen, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], nStrLen, (char*)pRemotePath, nStrLen);
	nPos += nStrLen;

	int nTotalSendLen = nPos;
	int nBeginSend    = 0;
	int nCurrSendLen  = 0;
	bool blSendFlag   = false;
	int nBeginRecv    = 0;
	int nCurrRecvLen  = 0;
	bool blRecvFlag   = false;
	while(true)
	{
        transresult_t   rt  =m_sckClient->Send(szSendBuff + nBeginSend, nTotalSendLen);
        nCurrSendLen    =rt.nbytes;

		if(nCurrSendLen <= 0)
		{
            int dwError = GetLastSocketError();
            wxMessageBox(wxT("远程服务器发送数据失败"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
			return false;
		}
		else
		{
			nTotalSendLen -= nCurrSendLen;
			if(nTotalSendLen == 0)
			{
				//发送完成
				blSendFlag = true;
				break;
			}
			else
			{
				nBeginSend += nCurrSendLen;
			}
		}
	}

	//先接收四字节的数据包长度
	char szRecvLength[4] = {'\0'};

    transresult_t  rt  =m_sckClient->Recv( (char* )szRecvLength, 4);
    nCurrRecvLen    =rt.nbytes;

	if(nCurrRecvLen != 4)
	{
        int dwError = GetLastSocketError();
        wxMessageBox(wxT("远程服务器接收数据失败"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
		return false;
	}


	int nRecvLength = 0;
	memcpy_s(&nRecvLength, sizeof(int), szRecvLength, sizeof(int));
	char* pRecvBuff = new char[nRecvLength];
	int nRecvBegin  = 0;

	while(true)
	{
		if(nRecvLength - nRecvBegin == 0)
		{
			break;
		}

		//如果发送成功了，则处理接收数据
        transresult_t  rt  =m_sckClient->Recv( (char* )pRecvBuff + nRecvBegin, nRecvLength - nRecvBegin);
        nCurrRecvLen    =rt.nbytes;

		if(nCurrRecvLen <= 0)
		{
            int dwError = GetLastSocketError();
            wxMessageBox(wxT("远程服务器发送数据失败"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
			return false;
		}
		else
		{
			nRecvBegin += nCurrRecvLen;
		}
	}

	int nRecvCommandID = 0;
	int nRet           = 0;
	nPos               = 0;

	memcpy_s((char*)&nRecvCommandID,  sizeof(short), &pRecvBuff[nPos], sizeof(short));
	nPos += sizeof(short);
	memcpy_s((char*)&nRet,  sizeof(int), &pRecvBuff[nPos], sizeof(int));
	nPos += sizeof(int);

	if(nRet == OP_OK)
	{
		int nCount = 0;
		memcpy_s((char*)&nCount, sizeof(int), &pRecvBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		for(int i = 0; i < nCount; i++)
		{
			int nFileNameLen = 0;
			char szFileName[MAX_BUFF_500] = {'\0'};
			int nFileType = IS_FILE;
			int nFileSize = 0;
			memcpy_s((char*)&nFileNameLen, sizeof(char), &pRecvBuff[nPos], sizeof(char));
			nPos += sizeof(char);
			memcpy_s((char*)&szFileName, nFileNameLen, &pRecvBuff[nPos], nFileNameLen);
			nPos += nFileNameLen;
			memcpy_s((char*)&nFileType, sizeof(int), &pRecvBuff[nPos], sizeof(int));
			nPos += sizeof(int);
			memcpy_s((char*)&nFileSize, sizeof(int), &pRecvBuff[nPos], sizeof(int));
			nPos += sizeof(int);

			//加入显示

			this->ListCtrl_FileList->InsertItem(i, wxString(szFileName));
			if(nFileType == IS_FILE)
			{
				this->ListCtrl_FileList->SetItem(i, 1, wxT("文件"));
			}
			else
			{
				this->ListCtrl_FileList->SetItem(i, 1, wxT("文件夹"));
			}
			this->ListCtrl_FileList->SetItem(i, 2, wxString::Format(wxT("%d"),nFileSize));



		}
	}

	return true;
}

bool PssFtpClientFrame::Send_Download(const char* pLocalPath, const char* pFileName, const char* pRemotePath, int nIndex, int nSize, int& nBockCount)
{
	int nCommand = COMMAND_FILE_DOWNLOAD;
	char szSendBuff[MAX_BUFF_500] = {'\0'};
   if(IsValidSocketHandle(m_sckClient->GetHandle()) )
    {
        return false;
    }


	short sVersion = 1;
	int nPos       = 0;
	int nLen       = 2 + 1 + 8 + (int)strlen(m_ClientFTPInfo.szUserName) + (int)strlen(pRemotePath);
	char szSession[32] = {'\0'};

	sprintf_s(szSession, 32, "FREEEYES");

	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&sVersion, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nCommand, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nLen, sizeof(int));
	nPos += sizeof(int);
	memcpy_s(&szSendBuff[nPos], sizeof(char)*32, (char*)szSession, sizeof(char)*32);
	nPos += sizeof(char)*32;

	int nStrLen = (int)strlen(m_ClientFTPInfo.szUserName);
	memcpy_s(&szSendBuff[nPos], sizeof(char), (char*)&nStrLen, sizeof(char));
	nPos += sizeof(char);
	memcpy_s(&szSendBuff[nPos], nStrLen, (char*)m_ClientFTPInfo.szUserName, nStrLen);
	nPos += nStrLen;

	nStrLen = (int)strlen(pRemotePath);
	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nStrLen, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], nStrLen, (char*)pRemotePath, nStrLen);
	nPos += nStrLen;

	memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nSize, sizeof(int));
	nPos += sizeof(int);
	memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nIndex, sizeof(int));
	nPos += sizeof(int);

	int nTotalSendLen = nPos;
	int nBeginSend    = 0;
	int nCurrSendLen  = 0;
	bool blSendFlag   = false;
	int nBeginRecv    = 0;
	int nCurrRecvLen  = 0;
	bool blRecvFlag   = false;
	while(true)
	{
        transresult_t   rt  =m_sckClient->Send(szSendBuff + nBeginSend, nTotalSendLen);
        nCurrSendLen    =rt.nbytes;
        if(nCurrSendLen <= 0)
        {
            int dwError = GetLastSocketError();
            wxMessageBox(wxT("远程服务器发送数据失败"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
            return false;
        }
		else
		{
			nTotalSendLen -= nCurrSendLen;
			if(nTotalSendLen == 0)
			{
				//发送完成
				blSendFlag = true;
				break;
			}
			else
			{
				nBeginSend += nCurrSendLen;
			}
		}
	}

	//先接收四字节的数据包长度
	char szRecvLength[4] = {'\0'};

    transresult_t  rt  =m_sckClient->Recv( (char* )szRecvLength, 4);
    nCurrRecvLen    =rt.nbytes;

    if(nCurrRecvLen != 4)
    {
        int dwError = GetLastSocketError();
        wxMessageBox(wxT("远程服务器接收数据失败"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
        return false;
    }


	int nRecvLength = 0;
	memcpy_s(&nRecvLength, sizeof(int), szRecvLength, sizeof(int));
	char* pRecvBuff = new char[nRecvLength];
	int nRecvBegin  = 0;

	while(true)
	{
		if(nRecvLength - nRecvBegin == 0)
		{
			break;
		}

		//如果发送成功了，则处理接收数据
        transresult_t  rt  =m_sckClient->Recv( (char* )pRecvBuff + nRecvBegin, nRecvLength - nRecvBegin);
        nCurrRecvLen    =rt.nbytes;

        if(nCurrRecvLen <= 0)
        {
            int dwError = GetLastSocketError();
            wxMessageBox(wxT("远程服务器发送数据失败"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
            return false;
        }
		else
		{
			nRecvBegin += nCurrRecvLen;
		}
	}

	int nRecvCommandID = 0;
	int nRet           = 0;
	nPos               = 0;

	memcpy_s((char*)&nRecvCommandID,  sizeof(short), &pRecvBuff[nPos], sizeof(short));
	nPos += sizeof(short);
	memcpy_s((char*)&nRet,  sizeof(int), &pRecvBuff[nPos], sizeof(int));
	nPos += sizeof(int);

	if(nRet == OP_OK)
	{
		//得到文件块，并写入文件
		char* pBuffer = new char[nSize];
		int nFileBlockCount     = 0;
		int nFileCurrIndex      = 0;
		int nFileCurrBufferSize = 0;

		memcpy_s((char*)&nBockCount,  sizeof(int), &pRecvBuff[nPos], sizeof(int));
		nPos += sizeof(int);
		memcpy_s((char*)&nFileCurrIndex,  sizeof(int), &pRecvBuff[nPos], sizeof(int));
		nPos += sizeof(int);
		memcpy_s((char*)&nFileCurrBufferSize,  sizeof(int), &pRecvBuff[nPos], sizeof(int));
		nPos += sizeof(int);
		memcpy_s((char*)pBuffer, nFileCurrBufferSize, &pRecvBuff[nPos], nFileCurrBufferSize);
		nPos += nFileCurrBufferSize;

		char szLocalFilePath[MAX_BUFF_500] = {'\0'};
		sprintf_s(szLocalFilePath, MAX_BUFF_500, "%s%s", pLocalPath, pFileName);

		//写入文件
		if(nFileCurrIndex == 0)
		{
			//如果是初始包，则删除当前文件。
			remove(szLocalFilePath);
		}

		FILE* pFile = NULL;
		fopen_s(&pFile, szLocalFilePath, "ab+");
		if(pFile == NULL)
		{
			wxMessageBox(wxT(""), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
			delete[] pBuffer;
			return false;
		}

		fwrite(pBuffer, sizeof(char), nFileCurrBufferSize, pFile);
		fclose(pFile);
		delete[] pBuffer;
	}
	else
	{
		wxMessageBox(wxT("远程文件获得失败"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
		return false;
	}

	return true;
}

bool PssFtpClientFrame::Send_Upload( const char* pLocalPath, const char* pFileName, const char* pRemotePath )
{
	int nBufferSize = MAX_BUFF_10240;
	int nCommand = COMMAND_FILE_UPLOAD;
	char szSendBuff[MAX_BUFF_10240 + MAX_BUFF_500] = {'\0'};
   if(IsValidSocketHandle(m_sckClient->GetHandle()) )
    {
        return false;
    }


	char szLoaclFilePath[MAX_BUFF_500] = {'\0'};
	sprintf_s(szLoaclFilePath, MAX_BUFF_500, "%s%s", pLocalPath, pFileName);

	//得到当前文件大小
	FILE* pFile = NULL;
	fopen_s(&pFile, szLoaclFilePath, "rb");
	if(NULL == pFile)
	{
		return false;
	}

	fseek(pFile, 0L, SEEK_END);
	int nFileLen        = (int)ftell(pFile);
	int nBlockCount     = 0;
	int nLastBufferSize = 0;

	if(nFileLen % nBufferSize != 0)
	{
		nBlockCount     = nFileLen / nBufferSize + 1;
		nLastBufferSize = nFileLen % nBufferSize;
	}
	else
	{
		nBlockCount     = nFileLen / nBufferSize;
		nLastBufferSize = nBufferSize;
	}

	fclose(pFile);
	pFile = NULL;

	for(int i = 0; i < nBlockCount; i++)
	{
		fopen_s(&pFile, szLoaclFilePath, "rb");
		if(NULL == pFile)
		{
			return false;
		}

		int nPos = 0;
		char szBuffer[MAX_BUFF_10240] = {'\0'};
		if(i != nBlockCount - 1)
		{
			//如果不是最后一个块
			fseek(pFile, (long)(i * nBufferSize), SEEK_CUR);

			int nReadSize = fread((char* )szBuffer, sizeof(char), nBufferSize, pFile);

			int nLen = 2 + 1 + 12 + (int)strlen(m_ClientFTPInfo.szUserName) + (int)strlen(pRemotePath) + nBufferSize;

			short sVersion = 1;
			char szSession[32] = {'\0'};

			sprintf_s(szSession, 32, "FREEEYES");
			memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&sVersion, sizeof(short));
			nPos += sizeof(short);
			memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nCommand, sizeof(short));
			nPos += sizeof(short);
			memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nLen, sizeof(int));
			nPos += sizeof(int);
			memcpy_s(&szSendBuff[nPos], sizeof(char)*32, (char*)szSession, sizeof(char)*32);
			nPos += sizeof(char)*32;

			int nStrLen = (int)strlen(m_ClientFTPInfo.szUserName);
			memcpy_s(&szSendBuff[nPos], sizeof(char), (char*)&nStrLen, sizeof(char));
			nPos += sizeof(char);
			memcpy_s(&szSendBuff[nPos], nStrLen, (char*)m_ClientFTPInfo.szUserName, nStrLen);
			nPos += nStrLen;

			nStrLen = (int)strlen(pRemotePath);
			memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nStrLen, sizeof(short));
			nPos += sizeof(short);
			memcpy_s(&szSendBuff[nPos], nStrLen, (char*)pRemotePath, nStrLen);
			nPos += nStrLen;

			memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nBufferSize, sizeof(int));
			nPos += sizeof(int);
			memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&i, sizeof(int));
			nPos += sizeof(int);

			memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nBufferSize, sizeof(int));
			nPos += sizeof(int);
			memcpy_s(&szSendBuff[nPos], nBufferSize, (char*)szBuffer, nBufferSize);
			nPos += nBufferSize;
		}
		else
		{
			//如果是最后一个块
			fseek(pFile, (long)(i * nBufferSize), SEEK_CUR);

			int nReadSize = fread((char* )szBuffer, sizeof(char), nLastBufferSize, pFile);

			int nLen = 2 + 1 + 12 + (int)strlen(m_ClientFTPInfo.szUserName) + (int)strlen(pRemotePath) + nLastBufferSize;

			short sVersion = 1;
			char szSession[32] = {'\0'};

			sprintf_s(szSession, 32, "FREEEYES");
			memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&sVersion, sizeof(short));
			nPos += sizeof(short);
			memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nCommand, sizeof(short));
			nPos += sizeof(short);
			memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nLen, sizeof(int));
			nPos += sizeof(int);
			memcpy_s(&szSendBuff[nPos], sizeof(char)*32, (char*)szSession, sizeof(char)*32);
			nPos += sizeof(char)*32;

			int nStrLen = (int)strlen(m_ClientFTPInfo.szUserName);
			memcpy_s(&szSendBuff[nPos], sizeof(char), (char*)&nStrLen, sizeof(char));
			nPos += sizeof(char);
			memcpy_s(&szSendBuff[nPos], nStrLen, (char*)m_ClientFTPInfo.szUserName, nStrLen);
			nPos += nStrLen;

			nStrLen = (int)strlen(pRemotePath);
			memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nStrLen, sizeof(short));
			nPos += sizeof(short);
			memcpy_s(&szSendBuff[nPos], nStrLen, (char*)pRemotePath, nStrLen);
			nPos += nStrLen;

			memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nLastBufferSize, sizeof(int));
			nPos += sizeof(int);
			memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&i, sizeof(int));
			nPos += sizeof(int);

			memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nLastBufferSize, sizeof(int));
			nPos += sizeof(int);
			memcpy_s(&szSendBuff[nPos], nLastBufferSize, (char*)szBuffer, nLastBufferSize);
			nPos += nLastBufferSize;
		}

		int nTotalSendLen = nPos;
		int nBeginSend    = 0;
		int nCurrSendLen  = 0;
		bool blSendFlag   = false;
		int nBeginRecv    = 0;
		int nCurrRecvLen  = 0;
		bool blRecvFlag   = false;
		while(true)
		{
            transresult_t   rt  =m_sckClient->Send(szSendBuff + nBeginSend, nTotalSendLen);
            nCurrSendLen    =rt.nbytes;
            if(nCurrSendLen <= 0)
            {
                int dwError = GetLastSocketError();
                wxMessageBox(wxT("远程服务器接收数据失败"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
                fclose(pFile);
                return false;
            }
			else
			{
				nTotalSendLen -= nCurrSendLen;
				if(nTotalSendLen == 0)
				{
					//发送完成
					blSendFlag = true;
					break;
				}
				else
				{
					nBeginSend += nCurrSendLen;
				}
			}
		}

		//先接收四字节的数据包长度
		char szRecvLength[4] = {'\0'};
        transresult_t  rt  =m_sckClient->Recv( (char* )szRecvLength, 4);
        nCurrRecvLen    =rt.nbytes;

        if(nCurrRecvLen != 4)
        {
            int dwError = GetLastSocketError();
            wxMessageBox(wxT("远程服务器接收数据失败"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
            fclose(pFile);
            return false;
        }


		int nRecvLength = 0;
		memcpy_s(&nRecvLength, sizeof(int), szRecvLength, sizeof(int));
		char* pRecvBuff = new char[nRecvLength];
		int nRecvBegin  = 0;

		while(true)
		{
			if(nRecvLength - nRecvBegin == 0)
			{
				break;
			}

			//如果发送成功了，则处理接收数据
            transresult_t  rt  =m_sckClient->Recv( (char* )pRecvBuff + nRecvBegin, nRecvLength - nRecvBegin);
            nCurrRecvLen    =rt.nbytes;

            if(nCurrRecvLen <= 0)
            {
                int dwError = GetLastSocketError();
                wxMessageBox(wxT("远程服务器发送数据失败"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
                fclose(pFile);
                return false;
            }
			else
			{
				nRecvBegin += nCurrRecvLen;
			}
		}

		int nRecvCommandID = 0;
		int nRet           = 0;
		nPos               = 0;

		memcpy_s((char*)&nRecvCommandID,  sizeof(short), &pRecvBuff[nPos], sizeof(short));
		nPos += sizeof(short);
		memcpy_s((char*)&nRet,  sizeof(int), &pRecvBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		if(nRet != OP_OK)
		{
			int dwError = GetLastSocketError();
			wxMessageBox(wxT("远程服务器接收数据失败"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
			fclose(pFile);
			return false;
		}

		fclose(pFile);
	}

    wxMessageBox(wxT("上传文件成功"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);

	return true;
}
