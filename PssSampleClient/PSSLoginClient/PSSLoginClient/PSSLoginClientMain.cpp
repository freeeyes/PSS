/***************************************************************
 * Name:      PSSLoginClientMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    smith ()
 * Created:   2019-02-21
 * Copyright: smith ()
 * License:
 **************************************************************/

#include "PSSLoginClientMain.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(PSSLoginClientFrame)
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
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}




//(*IdInit(PSSLoginClientFrame)
const long PSSLoginClientFrame::ID_STATICBOX1 = wxNewId();
const long PSSLoginClientFrame::ID_STATICBOX2 = wxNewId();
const long PSSLoginClientFrame::ID_STATICBOX3 = wxNewId();
const long PSSLoginClientFrame::ID_STATICTEXT1 = wxNewId();
const long PSSLoginClientFrame::ID_TEXTCTRL1 = wxNewId();
const long PSSLoginClientFrame::ID_STATICTEXT2 = wxNewId();
const long PSSLoginClientFrame::ID_STATICTEXT3 = wxNewId();
const long PSSLoginClientFrame::ID_STATICTEXT4 = wxNewId();
const long PSSLoginClientFrame::ID_STATICTEXT5 = wxNewId();
const long PSSLoginClientFrame::ID_TEXTCTRL2 = wxNewId();
const long PSSLoginClientFrame::ID_TEXTCTRL3 = wxNewId();
const long PSSLoginClientFrame::ID_TEXTCTRL4 = wxNewId();
const long PSSLoginClientFrame::ID_STATICTEXT6 = wxNewId();
const long PSSLoginClientFrame::ID_TEXTCTRL5 = wxNewId();
const long PSSLoginClientFrame::ID_TEXTCTRL6 = wxNewId();
const long PSSLoginClientFrame::ID_STATICTEXT7 = wxNewId();
const long PSSLoginClientFrame::ID_TEXTCTRL7 = wxNewId();
const long PSSLoginClientFrame::ID_BUTTON1 = wxNewId();
const long PSSLoginClientFrame::ID_BUTTON2 = wxNewId();
const long PSSLoginClientFrame::ID_BUTTON3 = wxNewId();
const long PSSLoginClientFrame::ID_STATICTEXT8 = wxNewId();
const long PSSLoginClientFrame::ID_TEXTCTRL8 = wxNewId();
const long PSSLoginClientFrame::ID_BUTTON4 = wxNewId();
const long PSSLoginClientFrame::ID_STATICTEXT9 = wxNewId();
const long PSSLoginClientFrame::ID_TEXTCTRL9 = wxNewId();
const long PSSLoginClientFrame::ID_STATICTEXT10 = wxNewId();
const long PSSLoginClientFrame::ID_STATICTEXT11 = wxNewId();
const long PSSLoginClientFrame::ID_TEXTCTRL10 = wxNewId();
const long PSSLoginClientFrame::ID_TEXTCTRL11 = wxNewId();
const long PSSLoginClientFrame::ID_BUTTON5 = wxNewId();
const long PSSLoginClientFrame::ID_LISTCTRL1 = wxNewId();
const long PSSLoginClientFrame::ID_PANEL1 = wxNewId();
const long PSSLoginClientFrame::ID_TIMER1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(PSSLoginClientFrame,wxFrame)
    //(*EventTable(PSSLoginClientFrame)
    //*)
END_EVENT_TABLE()

PSSLoginClientFrame::PSSLoginClientFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(PSSLoginClientFrame)
    Create(parent, id, wxT("二级缓冲测试客户端"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
    SetClientSize(wxSize(643,496));
    {
    	wxIcon FrameIcon;
    	FrameIcon.CopyFromBitmap(wxBitmap(wxImage(_T("/data/develop/Lib/Network/PSS-master/Icon.jpg"))));
    	SetIcon(FrameIcon);
    }
    Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(128,112), wxSize(643,496), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    StaticBox1 = new wxStaticBox(Panel1, ID_STATICBOX1, wxT("登陆测试"), wxPoint(8,8), wxSize(624,160), 0, _T("ID_STATICBOX1"));
    StaticBox2 = new wxStaticBox(Panel1, ID_STATICBOX2, wxT("用户信息"), wxPoint(8,176), wxSize(624,128), 0, _T("ID_STATICBOX2"));
    StaticBox3 = new wxStaticBox(Panel1, ID_STATICBOX3, wxT("测试结果"), wxPoint(8,304), wxSize(624,136), 0, _T("ID_STATICBOX3"));
    StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT1, wxT("时间消耗"), wxPoint(8,456), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    TextCtrl_TimeUsed = new wxTextCtrl(Panel1, ID_TEXTCTRL1, wxEmptyString, wxPoint(72,448), wxSize(256,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    StaticText2 = new wxStaticText(Panel1, ID_STATICTEXT2, wxT("服务器IP"), wxPoint(24,40), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    StaticText3 = new wxStaticText(Panel1, ID_STATICTEXT3, wxT("用户名"), wxPoint(24,80), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    StaticText4 = new wxStaticText(Panel1, ID_STATICTEXT4, wxT("随机从"), wxPoint(24,120), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    StaticText5 = new wxStaticText(Panel1, ID_STATICTEXT5, wxT("服务器端口"), wxPoint(232,40), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    TextCtrl_ServerPort = new wxTextCtrl(Panel1, ID_TEXTCTRL2, wxT("10002"), wxPoint(304,32), wxSize(128,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
    TextCtrl_ServerIP = new wxTextCtrl(Panel1, ID_TEXTCTRL3, wxT("127.0.0.1"), wxPoint(88,32), wxSize(128,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
    TextCtrl_Username = new wxTextCtrl(Panel1, ID_TEXTCTRL4, wxT("freeeyes"), wxPoint(88,72), wxSize(128,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
    StaticText6 = new wxStaticText(Panel1, ID_STATICTEXT6, wxT("密码"), wxPoint(232,80), wxDefaultSize, 0, _T("ID_STATICTEXT6"));
    TextCtrl_Password = new wxTextCtrl(Panel1, ID_TEXTCTRL5, wxT("123456"), wxPoint(304,72), wxSize(128,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL5"));
    TextCtrl_RadomStart = new wxTextCtrl(Panel1, ID_TEXTCTRL6, wxT("1"), wxPoint(88,112), wxSize(128,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL6"));
    StaticText7 = new wxStaticText(Panel1, ID_STATICTEXT7, wxT("到"), wxPoint(232,120), wxDefaultSize, 0, _T("ID_STATICTEXT7"));
    TextCtrl_RadomEnd = new wxTextCtrl(Panel1, ID_TEXTCTRL7, wxT("5"), wxPoint(304,112), wxSize(128,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL7"));
    Button_LoginOnceTest = new wxButton(Panel1, ID_BUTTON1, wxT("单一测试"), wxPoint(448,72), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    Button_StartTest = new wxButton(Panel1, ID_BUTTON2, wxT("压力测试"), wxPoint(448,112), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    Button_StopTest = new wxButton(Panel1, ID_BUTTON3, wxT("停止测试"), wxPoint(544,112), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    StaticText8 = new wxStaticText(Panel1, ID_STATICTEXT8, wxT("用户ID"), wxPoint(24,208), wxDefaultSize, 0, _T("ID_STATICTEXT8"));
    TextCtrl_UserID_Once = new wxTextCtrl(Panel1, ID_TEXTCTRL8, wxT("1001"), wxPoint(88,200), wxSize(128,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL8"));
    Button_UserID_OnceTest = new wxButton(Panel1, ID_BUTTON4, wxT("单一测试"), wxPoint(232,200), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
    StaticText9 = new wxStaticText(Panel1, ID_STATICTEXT9, wxT("用户ID"), wxPoint(24,248), wxDefaultSize, 0, _T("ID_STATICTEXT9"));
    TextCtrl_UserID_Set = new wxTextCtrl(Panel1, ID_TEXTCTRL9, wxEmptyString, wxPoint(88,248), wxSize(120,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL9"));
    StaticText10 = new wxStaticText(Panel1, ID_STATICTEXT10, wxT("Life"), wxPoint(232,256), wxDefaultSize, 0, _T("ID_STATICTEXT10"));
    StaticText11 = new wxStaticText(Panel1, ID_STATICTEXT11, wxT("Magic"), wxPoint(392,256), wxDefaultSize, 0, _T("ID_STATICTEXT11"));
    TextCtrl_Life = new wxTextCtrl(Panel1, ID_TEXTCTRL10, wxEmptyString, wxPoint(264,248), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL10"));
    TextCtrl_Magic = new wxTextCtrl(Panel1, ID_TEXTCTRL11, wxEmptyString, wxPoint(432,248), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL11"));
    Button_SetParam = new wxButton(Panel1, ID_BUTTON5, wxT("设置数值"), wxPoint(544,248), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON5"));
    ListCtrl_Result = new wxListCtrl(Panel1, ID_LISTCTRL1, wxPoint(24,328), wxSize(600,104), wxLC_LIST|wxLC_REPORT, wxDefaultValidator, _T("ID_LISTCTRL1"));
    Timer_MainForm.SetOwner(this, ID_TIMER1);

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSSLoginClientFrame::OnButton_LoginOnceTestClick);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSSLoginClientFrame::OnButton_StartTestClick);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSSLoginClientFrame::OnButton_StopTestClick);
    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSSLoginClientFrame::OnButton_UserID_OnceTestClick);
    Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSSLoginClientFrame::OnButton_SetParamClick);
    Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&PSSLoginClientFrame::OnTimer_MainFormTrigger);
    //*)


    Init();
}

PSSLoginClientFrame::~PSSLoginClientFrame()
{
    //(*Destroy(PSSLoginClientFrame)
    //*)
}

void PSSLoginClientFrame::OnQuit(wxCommandEvent& event)
{
    if (m_sckClient != NULL)
    {
        m_sckClient->Close();
        delete m_sckClient;
    }

    if(m_blMultiple == true)
	{
		m_blMultiple = false;
	}

    Close();

}

void PSSLoginClientFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, wxT("Welcome to..."));
}


void PSSLoginClientFrame::OnButton_StartTestClick(wxCommandEvent& event)
{

	//压力测试

	//获得相关服务器连接信息
    std::string    strServerIP    =std::string(this->TextCtrl_ServerIP->GetValue());
    int nPort = wxAtoi(this->TextCtrl_ServerPort->GetValue());

    memcpy(m_objServerInfo.m_szServerIP,strServerIP.c_str(),strServerIP.length());
    m_objServerInfo.m_szServerIP[strServerIP.length()] = '\0';
    m_objServerInfo.m_nServerPort =nPort;

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
    //std::string    strUserIDFrom    =std::string( cvGBK.cWX2MB( this->TextCtrl_RadomStart->GetValue().c_str() ) );
    //std::string    strUserIDTo    =std::string( cvGBK.cWX2MB( this->TextCtrl_RadomEnd->GetValue().c_str() ) );

    memcpy(m_objLoginClient.m_szUserName,strUserNmae.c_str(),strUserNmae.length());
    memcpy(m_objLoginClient.m_szUserPass,strUserPass.c_str(),strUserPass.length());
	m_objLoginClient.m_szUserName[strUserNmae.length()] = '\0';
	m_objLoginClient.m_szUserPass[strUserPass.length()] = '\0';

	m_objLoginClient.m_nUserIDFrom = wxAtoi(this->TextCtrl_RadomStart->GetValue());
	m_objLoginClient.m_nUserIDTo = wxAtoi(this->TextCtrl_RadomEnd->GetValue());


	m_blMultiple = true;
	m_nSendCount = 0;

	//初始化要发送的数据
	m_vecLoginInfo.clear();
	this->ListCtrl_Result->DeleteAllItems();

	for (int i = m_objLoginClient.m_nUserIDFrom; i < m_objLoginClient.m_nUserIDTo; i++)
	{
		_LoginInfo objLoginInfo;
		sprintf_s(objLoginInfo.m_szUserName, MAX_BUFF_50, "%s%d", m_objLoginClient.m_szUserName, i);
		sprintf_s(objLoginInfo.m_szUserPass, MAX_BUFF_50, "%s", m_objLoginClient.m_szUserPass);
		m_vecLoginInfo.push_back(objLoginInfo);

		// 插入新项

//		wchar_t wszUserName[MAX_BUFF_50] = { '\0' };
//		wchar_t wszUserPass[MAX_BUFF_50] = { '\0' };
//
//		int nSrcLen = MultiByteToWideChar(CP_ACP, 0, objLoginInfo.m_szUserName, -1, NULL, 0);
//		int nDecLen = MultiByteToWideChar(CP_ACP, 0, objLoginInfo.m_szUserName, -1, wszUserName, MAX_BUFF_50);
//
//		nSrcLen = MultiByteToWideChar(CP_ACP, 0, objLoginInfo.m_szUserPass, -1, NULL, 0);
//		nDecLen = MultiByteToWideChar(CP_ACP, 0, objLoginInfo.m_szUserPass, -1, wszUserPass, MAX_BUFF_50);

		int itemIndex = this->ListCtrl_Result->GetItemCount();
		this->ListCtrl_Result->InsertItem(itemIndex, wxString(objLoginInfo.m_szUserName));
		this->ListCtrl_Result->SetItem(itemIndex, 1, wxString(objLoginInfo.m_szUserPass));
		this->ListCtrl_Result->SetItem(itemIndex, 2, wxT("0"));
		this->ListCtrl_Result->SetItem(itemIndex, 3, wxT("0"));
		this->ListCtrl_Result->SetItem(itemIndex, 4, wxT("0"));
	}

    std::thread thread_test(&PSSLoginClientFrame::Send_Multiple_Login,this);
    thread_test.detach(); // 等待线程结束


}

void PSSLoginClientFrame::OnButton_StopTestClick(wxCommandEvent& event)
{
	//停止压测
	m_blMultiple = false;

	this->Timer_MainForm.Stop();
	this->TextCtrl_TimeUsed->SetValue(wxT(""));

}


int PSSLoginClientFrame::Random( int nStart, int nEnd )
{
	return nStart + (nEnd - nStart)*rand()/(RAND_MAX + 1);
}




void PSSLoginClientFrame::OnButton_SetParamClick(wxCommandEvent& event)
{

	//获得相关服务器连接信息
    std::string    strServerIP    =std::string(this->TextCtrl_ServerIP->GetValue());
    int nPort = wxAtoi(this->TextCtrl_ServerPort->GetValue());

    memcpy(m_objServerInfo.m_szServerIP,strServerIP.c_str(),strServerIP.length());
    m_objServerInfo.m_szServerIP[strServerIP.length()] = '\0';
    m_objServerInfo.m_nServerPort =nPort;

    if(strServerIP == wxT("") || nPort == 0)
    {
        wxString msg = wxT("请输入完整的服务器IP地址和端口信息。");
        wxMessageBox(msg, wxT("服务器信息"), wxOK | wxICON_INFORMATION, this);
        return;
    }

	//获得相关用户信息
	int nUserID =  wxAtoi(this->TextCtrl_UserID_Set->GetValue());
	int nLife = wxAtoi(this->TextCtrl_Life->GetValue());
	int nMagic = wxAtoi(this->TextCtrl_Magic->GetValue());


	if(Conn() == false)
	{
		return;
	}

	Send_SetUserInfo(nUserID, nLife, nMagic);

	Close();

}

void PSSLoginClientFrame::OnButton_UserID_OnceTestClick(wxCommandEvent& event)
{

	//获得相关服务器连接信息
	std::string    strServerIP    =std::string(this->TextCtrl_ServerIP->GetValue());
    int nPort = wxAtoi(this->TextCtrl_ServerPort->GetValue());

    memcpy(m_objServerInfo.m_szServerIP,strServerIP.c_str(),strServerIP.length());
    m_objServerInfo.m_szServerIP[strServerIP.length()] = '\0';
    m_objServerInfo.m_nServerPort =nPort;

    if(strServerIP == wxT("") || nPort == 0)
    {
        wxString msg = wxT("请输入完整的服务器IP地址和端口信息。");
        wxMessageBox(msg, wxT("服务器信息"), wxOK | wxICON_INFORMATION, this);
        return;
    }

	//获得相关用户信息
	int nSeachUserID = wxAtoi(this->TextCtrl_UserID_Once->GetValue());



	if(Conn() == false)
	{
		return;
	}

	Send_UserInfo(nSeachUserID);

	Close();


}

void PSSLoginClientFrame::OnButton_LoginOnceTestClick(wxCommandEvent& event)
{


	//单一测试

	//获得相关服务器连接信息
	std::string    strServerIP    =std::string(this->TextCtrl_ServerIP->GetValue());
    int nPort = wxAtoi(this->TextCtrl_ServerPort->GetValue());

    memcpy(m_objServerInfo.m_szServerIP,strServerIP.c_str(),strServerIP.length());
    m_objServerInfo.m_szServerIP[strServerIP.length()] = '\0';
    m_objServerInfo.m_nServerPort =nPort;

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
    //std::string    strUserIDFrom    =std::string( cvGBK.cWX2MB( this->TextCtrl_RadomStart->GetValue().c_str() ) );
    //std::string    strUserIDTo    =std::string( cvGBK.cWX2MB( this->TextCtrl_RadomEnd->GetValue().c_str() ) );

    memcpy(m_objLoginClient.m_szUserName,strUserNmae.c_str(),strUserNmae.length());
    memcpy(m_objLoginClient.m_szUserPass,strUserPass.c_str(),strUserPass.length());
	m_objLoginClient.m_szUserName[strUserNmae.length()] = '\0';
	m_objLoginClient.m_szUserPass[strUserPass.length()] = '\0';

	m_objLoginClient.m_nUserIDFrom = wxAtoi(this->TextCtrl_RadomStart->GetValue());
	m_objLoginClient.m_nUserIDTo = wxAtoi(this->TextCtrl_RadomEnd->GetValue());



	//判断socket是否已经连接，如果有连接则断开
    if(IsValidSocketHandle(m_sckClient->GetHandle()) )
    {
        Close();
    }


	if(Conn() == false)
	{
		return;
	}

	Send_Single_Login();

	Close();

}




void PSSLoginClientFrame::Init()
{

	this->ListCtrl_Result->InsertColumn(0, wxT("用户名"), wxLIST_FORMAT_CENTER, 100);
	this->ListCtrl_Result->InsertColumn(1, wxT("密码"), wxLIST_FORMAT_CENTER, 100);
	this->ListCtrl_Result->InsertColumn(2, wxT("发送次数"), wxLIST_FORMAT_CENTER, 100);
	this->ListCtrl_Result->InsertColumn(3, wxT("成功"), wxLIST_FORMAT_CENTER, 100);
	this->ListCtrl_Result->InsertColumn(4, wxT("失败"), wxLIST_FORMAT_CENTER, 100);


	//初始化TCP链接

    InitializeSocketEnvironment();

    m_sckClient =new CSockWrap(SOCK_STREAM);

	m_blMultiple = false;
	m_nSendCount = 0;

	srand(unsigned(time(0)));
}

bool PSSLoginClientFrame::Conn()
{
	//socket创建的准备工作
    std::string    strServerIP    =std::string(this->TextCtrl_ServerIP->GetValue());
    int nPort = wxAtoi(this->TextCtrl_ServerPort->GetValue());

    if(strServerIP == wxT("") || nPort == 0)
    {
        wxString msg = wxT("请输入完整的服务器IP地址和端口信息。");
        wxMessageBox(msg, wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
        return  false;
    }

    m_sckClient->SetAddress(m_objServerInfo.m_szServerIP, m_objServerInfo.m_nServerPort);
    int TimeOut = 1000;
    m_sckClient->SetTimeOut(TimeOut,-1,-1);

    m_sckClient->Reopen(true);
    if  (GetLastSocketError() !=0)
    {
        wxMessageBox(wxT("连接远程服务器失败。"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
        return  false;
    }

    return true;


}

void PSSLoginClientFrame::Close()
{
    if(IsValidSocketHandle(m_sckClient->GetHandle()) )
    {
            m_sckClient->Close();
    }

}



bool PSSLoginClientFrame::Send_Login(_LoginInfo& objLoginInfo)
{
	int nCommand = COMMAND_LOGIN;
	char szSendBuff[MAX_BUFF_500] = {'\0'};
	if(m_sckClient == INVALID_SOCKET)
	{
		return false;
	}

	short sVersion = 1;
	int nPos = 0;
	int nLen = 1 + 1 + (int)strlen(m_objLoginClient.m_szUserName) + (int)strlen(m_objLoginClient.m_szUserPass);
	char szSession[32] = {'\0'};
	sprintf_s(szSession, 32, "FREEETES");

	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&sVersion, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nCommand, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nLen, sizeof(int));
	nPos += sizeof(int);
	memcpy_s(&szSendBuff[nPos], sizeof(char)*32, (char*)szSession, sizeof(char)*32);
	nPos += sizeof(char)*32;

	int nStrLen = (int)strlen(m_objLoginClient.m_szUserName);
	memcpy_s(&szSendBuff[nPos], sizeof(char), (char*)&nStrLen, sizeof(char));
	nPos += sizeof(char);
	memcpy_s(&szSendBuff[nPos], nStrLen, (char*)m_objLoginClient.m_szUserName, nStrLen);
	nPos += nStrLen;

	nStrLen = (int)strlen(m_objLoginClient.m_szUserPass);
	memcpy_s(&szSendBuff[nPos], sizeof(char), (char*)&nStrLen, sizeof(char));
	nPos += sizeof(char);
	memcpy_s(&szSendBuff[nPos], nStrLen, (char*)m_objLoginClient.m_szUserPass, nStrLen);
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
			objLoginInfo.m_nServerFail++;
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

	objLoginInfo.m_nSendCount++;

	//先接收四字节的数据包长度
	char szRecvLength[4] = {'\0'};
	//nCurrRecvLen = recv(m_sckClient, (char* )szRecvLength, 4, 0);

    transresult_t  rt  =m_sckClient->Recv( (char* )szRecvLength, 4);
    nCurrRecvLen    =rt.nbytes;


	if(nCurrRecvLen != 4)
	{
        int dwError = GetLastSocketError();
        wxMessageBox(wxT("远程服务器接收数据失败"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
		objLoginInfo.m_nServerFail++;
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
		//nCurrRecvLen = recv(m_sckClient, (char* )pRecvBuff + nRecvBegin, nRecvLength - nRecvBegin, 0);
        transresult_t  rt  =m_sckClient->Recv( (char* )pRecvBuff + nRecvBegin, nRecvLength - nRecvBegin);
        nCurrRecvLen    =rt.nbytes;

		if(nCurrRecvLen <= 0)
		{
            delete[] pRecvBuff;
            int dwError = GetLastSocketError();
            wxMessageBox(wxT("远程服务器发送数据失败"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
			objLoginInfo.m_nServerFail++;
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

	if(nRet == LOGIN_SUCCESS)
	{
		objLoginInfo.m_nServerSuccess++;
	}
	else
	{
		objLoginInfo.m_nServerFail++;
	}

	delete[] pRecvBuff;

	return true;
}

bool PSSLoginClientFrame::Send_Single_Login()
{
	m_vecLoginInfo.clear();

	_LoginInfo objLoginInfo;

	sprintf_s(objLoginInfo.m_szUserName, MAX_BUFF_50, "%s", m_objLoginClient.m_szUserName);
	sprintf_s(objLoginInfo.m_szUserPass, MAX_BUFF_50, "%s", m_objLoginClient.m_szUserPass);

	m_vecLoginInfo.push_back(objLoginInfo);

	// 插入新项
		// 插入新项

		int itemIndex = this->ListCtrl_Result->GetItemCount();
		this->ListCtrl_Result->InsertItem(itemIndex, wxString(objLoginInfo.m_szUserName));
		this->ListCtrl_Result->SetItem(itemIndex, 1, wxString(objLoginInfo.m_szUserPass));
		this->ListCtrl_Result->SetItem(itemIndex, 2, wxT("0"));
		this->ListCtrl_Result->SetItem(itemIndex, 3, wxT("0"));
		this->ListCtrl_Result->SetItem(itemIndex, 4, wxT("0"));



	Send_Login(m_vecLoginInfo[0]);
	updateItemData(objLoginInfo, 0);

	return true;
}

bool PSSLoginClientFrame::Send_Multiple_Login()
{
	if(m_sckClient != INVALID_SOCKET)
	{
		Close();
	}

	if(Conn() == false)
	{
		return false;
	}

	// 设置定时器去计算每秒大约发送的请求数
	m_nLastSecondSendCount = 0;
	m_nSendCount = 0;
	//SetTimer(1, 1000, NULL);


	unsigned long  lulSendCount = 0;
	while(m_blMultiple)
	{
		int id = Random(m_objLoginClient.m_nUserIDFrom, m_objLoginClient.m_nUserIDTo);
		int nIndex = id - m_objLoginClient.m_nUserIDFrom;
		_LoginInfo& rdLoginInfo = m_vecLoginInfo[nIndex];

		Send_Login(rdLoginInfo);
		updateItemData(rdLoginInfo, nIndex);

		++m_nSendCount;
	}
	this->Timer_MainForm.Stop();
	//KillTimer(1);

	m_blMultiple = false;
	Close();

	return true;
}

bool PSSLoginClientFrame::Send_UserInfo( int nUserID )
{
	int nCommand = COMMAND_USERINFO;
	char szSendBuff[MAX_BUFF_500] = {'\0'};
   if(IsValidSocketHandle(m_sckClient->GetHandle()) )
    {
        return false;
    }

	short sVersion = 1;
	int nPos = 0;
	int nLen = 2 + 4;
	char szSession[32] = {'\0'};
	sprintf_s(szSession, 32, "FREEETES");

	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&sVersion, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nCommand, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nLen, sizeof(int));
	nPos += sizeof(int);
	memcpy_s(&szSendBuff[nPos], sizeof(char)*32, (char*)szSession, sizeof(char)*32);
	nPos += sizeof(char)*32;

	memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nUserID, sizeof(int));
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
		//nCurrRecvLen = recv(m_sckClient, (char* )pRecvBuff + nRecvBegin, nRecvLength - nRecvBegin, 0);

        transresult_t  rt  =m_sckClient->Recv( (char* )pRecvBuff + nRecvBegin, nRecvLength - nRecvBegin);
        nCurrRecvLen    =rt.nbytes;


		if(nCurrRecvLen <= 0)
		{
            delete[] pRecvBuff;
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
	int nReUserID      = 0;
	int nLife          = 0;
	int nMagic         = 0;

	memcpy_s((char*)&nRecvCommandID,  sizeof(short), &pRecvBuff[nPos], sizeof(short));
	nPos += sizeof(short);
	memcpy_s((char*)&nRet,  sizeof(int), &pRecvBuff[nPos], sizeof(int));
	nPos += sizeof(int);

	memcpy_s((char*)&nReUserID,  sizeof(int), &pRecvBuff[nPos], sizeof(int));
	nPos += sizeof(int);
	memcpy_s((char*)&nLife,  sizeof(int), &pRecvBuff[nPos], sizeof(int));
	nPos += sizeof(int);
	memcpy_s((char*)&nMagic,  sizeof(int), &pRecvBuff[nPos], sizeof(int));
	nPos += sizeof(int);

	this->TextCtrl_UserID_Set->SetValue(wxString::Format(wxT("%d"),nReUserID));
	this->TextCtrl_Life->SetValue(wxString::Format(wxT("%d"),nLife));
    this->TextCtrl_Magic->SetValue(wxString::Format(wxT("%d"),nMagic));

	delete[] pRecvBuff;

	return true;
}

bool PSSLoginClientFrame::Send_SetUserInfo( int nUserID, int nLife, int nMagic )
{
	int nCommand = COMMAND_SET_USERINFO;
	char szSendBuff[MAX_BUFF_500] = {'\0'};
   if(IsValidSocketHandle(m_sckClient->GetHandle()) )
    {
        return false;
    }

	short sVersion = 1;
	int nPos = 0;
	int nLen = 2 + 4 + 4 + 4;
	char szSession[32] = {'\0'};
	sprintf_s(szSession, 32, "FREEEYES");

	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nCommand, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nCommand, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nLen, sizeof(int));
	nPos += sizeof(int);
	memcpy_s(&szSendBuff[nPos], sizeof(char)*32, (char*)szSession, sizeof(char)*32);
	nPos += sizeof(char)*32;

	memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nUserID, sizeof(int));
	nPos += sizeof(int);
	memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nLife, sizeof(int));
	nPos += sizeof(int);
	memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nMagic, sizeof(int));
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
            delete[] pRecvBuff;
            int dwError = GetLastSocketError();
            wxMessageBox(wxT("远程服务器发送数据失败"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
            return false;
        }

	}

	int nRecvCommandID = 0;
	int nRet           = 0;
	nPos               = 0;

	memcpy_s((char*)&nRecvCommandID,  sizeof(short), &pRecvBuff[nPos], sizeof(short));
	nPos += sizeof(short);
	memcpy_s((char*)&nRet,  sizeof(int), &pRecvBuff[nPos], sizeof(int));
	nPos += sizeof(int);

	if(nRet == LOGIN_SUCCESS)
	{
		wxMessageBox(wxT("设置用户信息成功"), wxT("提示信息"), wxOK | wxICON_INFORMATION, this);
	}
	else
	{
		wxMessageBox(wxT(""), wxT("提示信息"), wxOK | wxICON_INFORMATION, this);
	}

	delete[] pRecvBuff;
	return true;
}

// 更新指定项测试数据
void PSSLoginClientFrame::updateItemData(_LoginInfo& objLoginInfo, const int nItemIndex)
{
	// 更新List列表项
	//add by @单调

		this->ListCtrl_Result->SetItem(nItemIndex, 2, wxString::Format(wxT("%d"),objLoginInfo.m_nSendCount));
		this->ListCtrl_Result->SetItem(nItemIndex, 3, wxString::Format(wxT("%d"),objLoginInfo.m_nServerSuccess));
		this->ListCtrl_Result->SetItem(nItemIndex, 4, wxString::Format(wxT("%d"),objLoginInfo.m_nServerFail));

}



void PSSLoginClientFrame::OnTimer_MainFormTrigger(wxTimerEvent& event)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	this->TextCtrl_TimeUsed->SetValue(wxString::Format(wxT("每秒约发送[%d]个请求"),m_nSendCount - m_nLastSecondSendCount));


}
