/***************************************************************
 * Name:      PSSPlugClientMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    smith ()
 * Created:   2019-02-22
 * Copyright: smith ()
 * License:
 **************************************************************/

#include "PSSPlugClientMain.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(PSSPlugClientFrame)
#include <wx/bitmap.h>
#include <wx/icon.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat
{
    short_f, long_f
};

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


//(*IdInit(PSSPlugClientFrame)
const long PSSPlugClientFrame::ID_STATICBOX1 = wxNewId();
const long PSSPlugClientFrame::ID_STATICBOX2 = wxNewId();
const long PSSPlugClientFrame::ID_STATICTEXT1 = wxNewId();
const long PSSPlugClientFrame::ID_TEXTCTRL1 = wxNewId();
const long PSSPlugClientFrame::ID_STATICTEXT2 = wxNewId();
const long PSSPlugClientFrame::ID_TEXTCTRL2 = wxNewId();
const long PSSPlugClientFrame::ID_STATICTEXT3 = wxNewId();
const long PSSPlugClientFrame::ID_TEXTCTRL3 = wxNewId();
const long PSSPlugClientFrame::ID_STATICTEXT4 = wxNewId();
const long PSSPlugClientFrame::ID_TEXTCTRL4 = wxNewId();
const long PSSPlugClientFrame::ID_BUTTON1 = wxNewId();
const long PSSPlugClientFrame::ID_BUTTON2 = wxNewId();
const long PSSPlugClientFrame::ID_BUTTON3 = wxNewId();
const long PSSPlugClientFrame::ID_STATICTEXT5 = wxNewId();
const long PSSPlugClientFrame::ID_TEXTCTRL5 = wxNewId();
const long PSSPlugClientFrame::ID_STATICTEXT6 = wxNewId();
const long PSSPlugClientFrame::ID_TEXTCTRL6 = wxNewId();
const long PSSPlugClientFrame::ID_PANEL1 = wxNewId();
const long PSSPlugClientFrame::ID_TIMER1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(PSSPlugClientFrame,wxFrame)
    //(*EventTable(PSSPlugClientFrame)
    //*)
END_EVENT_TABLE()

PSSPlugClientFrame::PSSPlugClientFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(PSSPlugClientFrame)
    Create(parent, id, wxT("插件间调用测试工具"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
    SetClientSize(wxSize(559,345));
    {
    	wxIcon FrameIcon;
    	FrameIcon.CopyFromBitmap(wxBitmap(wxImage(_T("/data/develop/Lib/Network/PSS-master/Icon.jpg"))));
    	SetIcon(FrameIcon);
    }
    Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(256,192), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    StaticBox1 = new wxStaticBox(Panel1, ID_STATICBOX1, wxT("测试信息"), wxPoint(8,8), wxSize(544,160), 0, _T("ID_STATICBOX1"));
    StaticBox2 = new wxStaticBox(Panel1, ID_STATICBOX2, wxT("压力测试"), wxPoint(8,176), wxSize(544,160), 0, _T("ID_STATICBOX2"));
    StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT1, wxT("服务器IP"), wxPoint(32,40), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    TextCtrl_ServerIP = new wxTextCtrl(Panel1, ID_TEXTCTRL1, wxT("127.0.0.1"), wxPoint(96,32), wxSize(152,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    TextCtrl_ServerIP->SetMaxLength(16);
    StaticText2 = new wxStaticText(Panel1, ID_STATICTEXT2, wxT("服务器端口"), wxPoint(272,40), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    TextCtrl_ServerPort = new wxTextCtrl(Panel1, ID_TEXTCTRL2, wxT("10002"), wxPoint(352,32), wxSize(-1,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
    StaticText3 = new wxStaticText(Panel1, ID_STATICTEXT3, wxT("内容"), wxPoint(32,80), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    TextCtrl_SendData = new wxTextCtrl(Panel1, ID_TEXTCTRL3, wxT("freeeyes，在天际，随云聚云散，梦起梦逝，雁过留影。"), wxPoint(96,72), wxSize(352,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
    StaticText4 = new wxStaticText(Panel1, ID_STATICTEXT4, wxT("回应"), wxPoint(32,120), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    TextCtrl_ServerResponse = new wxTextCtrl(Panel1, ID_TEXTCTRL4, wxEmptyString, wxPoint(96,112), wxSize(352,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
    TextCtrl_ServerResponse->Disable();
    Button_SendData = new wxButton(Panel1, ID_BUTTON1, wxT("发送"), wxPoint(456,72), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    Button_StartTest = new wxButton(Panel1, ID_BUTTON2, wxT("压力测试开始"), wxPoint(24,208), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    Button_StopTest = new wxButton(Panel1, ID_BUTTON3, wxT("压力测试结束"), wxPoint(136,208), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    StaticText5 = new wxStaticText(Panel1, ID_STATICTEXT5, wxT("已发送数据包"), wxPoint(32,264), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    TextCtrl_SendCount = new wxTextCtrl(Panel1, ID_TEXTCTRL5, wxEmptyString, wxPoint(136,256), wxSize(-1,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL5"));
    StaticText6 = new wxStaticText(Panel1, ID_STATICTEXT6, wxT("已接收数据包"), wxPoint(32,304), wxDefaultSize, 0, _T("ID_STATICTEXT6"));
    TextCtrl_RecvCount = new wxTextCtrl(Panel1, ID_TEXTCTRL6, wxEmptyString, wxPoint(136,296), wxSize(-1,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL6"));
    Timer_ShowData.SetOwner(this, ID_TIMER1);
    Timer_ShowData.Start(1000, false);

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSSPlugClientFrame::OnButton_SendDataClick);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSSPlugClientFrame::OnButton_StartTestClick);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSSPlugClientFrame::OnButton_StopTestClick);
    Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&PSSPlugClientFrame::OnTimer_ShowDataTrigger);
    //*)



    InitializeSocketEnvironment();

    m_sckClient =new CSockWrap(SOCK_STREAM);

}

PSSPlugClientFrame::~PSSPlugClientFrame()
{
    //(*Destroy(PSSPlugClientFrame)
    //*)
}

void PSSPlugClientFrame::OnQuit(wxCommandEvent& event)
{
    if (m_sckClient != NULL)
    {
        m_sckClient->Close();
        delete m_sckClient;
    }
    Close();
}

void PSSPlugClientFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, wxT("Welcome to..."));
}






void PSSPlugClientFrame::OnButton_SendDataClick(wxCommandEvent& event)
{
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

     wxCSConv cvGBK( wxT("GBK") );
    //return string( cvGBK.cWX2MB( this->TextCtrl_SendData->GetValue().c_str() ) );
     //wxPrintf("%s\n",this->TextCtrl_SendData->GetValue());

    std::string    strSendData    =std::string( cvGBK.cWX2MB( this->TextCtrl_SendData->GetValue().c_str() ) );

    if(strSendData.length() ==0)
    {
        wxMessageBox(wxT("测试数据不能为空。"), wxT("测试数据"), wxOK | wxICON_INFORMATION, this);
        return;
    };

    if(this->Conn( ) == false)
    {
        return;
    }

    char szText[MAX_BUFF_200] = {'\0'};
    int nDecLen = strSendData.length();
    memcpy(szText,strSendData.c_str(),strSendData.length());

    szText[nDecLen] = '\0';

    Send_Plug(szText);

    this->Close();

}

void PSSPlugClientFrame::OnButton_StartTestClick(wxCommandEvent& event)
{
    //压测
    //SetTimer(1, 1000, NULL);

    //DWORD  ThreadID = 0;
    //CreateThread(NULL, NULL, ThreadProc, (LPVOID)this, NULL, &ThreadID);
    if(this->Conn( ) == false)
    {
        return;
    }

    this->Timer_ShowData.Start(1000);

    std::thread thread_test(&PSSPlugClientFrame::Send_Multiple_Plug,this);

    //thread_test.join(); // 等待线程结束
    thread_test.detach(); // 等待线程结束

}

void PSSPlugClientFrame::OnButton_StopTestClick(wxCommandEvent& event)
{
    //this->Init();
    m_blIsRun = false;
}

void PSSPlugClientFrame::OnTimer_ShowDataTrigger(wxTimerEvent& event)
{
    Show_Send_List();
}



void PSSPlugClientFrame::Init()
{
    m_blIsRun = false;
    this->Timer_ShowData.Stop();
}




bool PSSPlugClientFrame::Conn()
{

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

bool PSSPlugClientFrame::Send_Plug(const char* pText, bool blIsDisPlay)
{
    //发送数据
    int nCommand = COMMAND_PLUGA;
    char szSendBuff[MAX_BUFF_500] = {'\0'};
    if(IsValidSocketHandle(m_sckClient->GetHandle()) )
    {
        return false;
    }

    int nVersion = 1;
    int nPos = 0;
    int nLen = (int)strlen(pText) + 2;
    char szSession[32] = {'\0'};

    sprintf_s(szSession, 32, "FREEEYES");

    memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nVersion, sizeof(short));
    nPos += sizeof(short);
    memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nCommand, sizeof(short));
    nPos += sizeof(short);
    memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nLen, sizeof(int));
    nPos += sizeof(int);
    memcpy_s(&szSendBuff[nPos], sizeof(char)*32, (char*)szSession, sizeof(char)*32);
    nPos += sizeof(char)*32;

    int nStrLen = (int)strlen(pText);
    memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nStrLen, sizeof(short));
    nPos += sizeof(short);
    memcpy_s(&szSendBuff[nPos], nStrLen, (char* )pText, nStrLen);
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


        //nCurrSendLen = this->m_sckClient->Send( szSendBuff + nBeginSend, nTotalSendLen);
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

    //接收返回数据
    //先接收四字节的数据包长度
    char szRecvLength[4] = {'\0'};
    // nCurrRecvLen = recv(m_sckClient, (char* )szRecvLength, 4, 0);

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
    int nRetA          = 0;
    int nRetB          = 0;
    nPos               = 0;


    memcpy_s((char*)&nRecvCommandID,  sizeof(short), &pRecvBuff[nPos], sizeof(short));
    nPos += sizeof(short);
    memcpy_s((char*)&nRetB,  sizeof(int), &pRecvBuff[nPos], sizeof(int));
    nPos += sizeof(int);
    memcpy_s((char*)&nRetA,  sizeof(int), &pRecvBuff[nPos], sizeof(int));
    nPos += sizeof(int);

    if(blIsDisPlay == true)
    {
        char    szReturnText[MAX_BUFF_50]  = {'\0'};
        wchar_t wszReturnText[MAX_BUFF_50] = {'\0'};
        sprintf_s(szReturnText, MAX_BUFF_50, "PlugA:%d,PlugB:%d.", nRetA, nRetB);

        this->TextCtrl_ServerResponse->SetValue( wxString::Format(wxT("接收回应包字节数:%d,PlugA应答:%d,PlugB应答:%d."),nRecvLength + sizeof(int), nRetA, nRetB));


    }

    return true;
}

void PSSPlugClientFrame::Close()
{
    m_sckClient->Close();
}




bool PSSPlugClientFrame::Send_Multiple_Plug()
{


    std::string    strServerIP    =std::string(this->TextCtrl_ServerIP->GetValue());

    int nPort = wxAtoi(this->TextCtrl_ServerPort->GetValue());

    memcpy(m_objServerInfo.m_szServerIP,strServerIP.c_str(),strServerIP.length());
    m_objServerInfo.m_szServerIP[strServerIP.length()] = '\0';

    m_objServerInfo.m_nServerPort =nPort;

    if(strServerIP == wxT("") || nPort == 0)
    {
        wxString msg = wxT("请输入完整的服务器IP地址和端口信息。");
        wxMessageBox(msg, wxT("服务器信息"), wxOK | wxICON_INFORMATION, this);
        return false;
    }

    //std::string    strSendData    =std::string(this->TextCtrl_SendData->GetValue());

    wxCSConv cvGBK( wxT("GBK") );
    std::string    strSendData    =std::string( cvGBK.cWX2MB( this->TextCtrl_SendData->GetValue() ) );

    if(strSendData.length()==0)
    {
        wxMessageBox(wxT("测试数据不能为空。"), wxT("测试数据"), wxOK | wxICON_INFORMATION, this);
        return false;
    };


    if(this->Conn( ) == false)
    {
        return false;
    }

    char szText[MAX_BUFF_200] = {'\0'};
    int nDecLen = strSendData.length();
    memcpy(szText,strSendData.c_str(),strSendData.length());

    szText[nDecLen] = '\0';

    Send_Plug(szText);


    m_blIsRun = true;
    m_objPassTest.Init();

    while(m_blIsRun)
    {
        bool blState = Send_Plug(szText, false);
        if(true == blState)
        {
            m_objPassTest.m_nSendCount++;
            m_objPassTest.m_nRecvCount++;
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(100*10));

    }


    this->Close();
    this->Timer_ShowData.Stop();


    Show_Send_List();

    return  true;

}



void PSSPlugClientFrame::Show_Send_List()
{
    this->TextCtrl_SendCount->SetValue( wxString::Format(wxT("%d"),m_objPassTest.m_nSendCount));
    this->TextCtrl_RecvCount->SetValue( wxString::Format(wxT("%d"),m_objPassTest.m_nRecvCount));

}
