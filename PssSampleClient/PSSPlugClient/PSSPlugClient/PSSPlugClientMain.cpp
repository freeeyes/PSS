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
    	FrameIcon.CopyFromBitmap(wxBitmap(wxImage(_T("./Icon.jpg"))));
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

    this->Init();
    
}

PSSPlugClientFrame::~PSSPlugClientFrame()
{
    //(*Destroy(PSSPlugClientFrame)
    //*)
    delete pNetOperation;
}

void PSSPlugClientFrame::OnQuit(wxCommandEvent& event)
{
    pNetOperation->Close();
    Close();
}

void PSSPlugClientFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, wxT("Welcome to..."));
}




void PSSPlugClientFrame::SendData(char* ParamText)
{
    if(this->Conn( ) == false)
    {
        return;
    }
    
    wxCSConv cvGBK( wxT("GBK") );     
    std::string    strSendData    =std::string( cvGBK.cWX2MB( this->TextCtrl_SendData->GetValue().c_str() ) );
    if(strSendData.length() ==0)
    {
        wxMessageBox(wxT("测试数据不能为空。"), wxT("测试数据"), wxOK | wxICON_INFORMATION, this);
        return;
    };

    int nDecLen = strSendData.length();
    memcpy(ParamText,strSendData.c_str(),strSendData.length());

    ParamText[nDecLen] = '\0';

    short nRecvCommandID = 0;
    int nRetA          = 0;
    int nRetB          = 0;
    pNetOperation->Send_Plug(ParamText,nRecvCommandID,nRetB,nRetA);
}

void PSSPlugClientFrame::OnButton_SendDataClick(wxCommandEvent& event)
{
    char szText[MAX_BUFF_200] = {'\0'};
    this->SendData(szText);
    pNetOperation->Close();

}

void PSSPlugClientFrame::OnButton_StartTestClick(wxCommandEvent& event)
{
    //压测
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
    //初始化TCP链接
    pNetOperation =new CNetOperation(SOCK_STREAM);

    m_blIsRun = false;
    this->Timer_ShowData.Stop();
}


bool PSSPlugClientFrame::Conn()
{
    std::string    strServerIP    =std::string(this->TextCtrl_ServerIP->GetValue());
    int nPort = wxAtoi(this->TextCtrl_ServerPort->GetValue());
    
    memcpy(m_objServerInfo.m_szServerIP,strServerIP.c_str(),strServerIP.length());
    m_objServerInfo.m_szServerIP[strServerIP.length()] = '\0';
    m_objServerInfo.m_nServerPort =nPort;

    if(strServerIP == wxT("") || nPort == 0)
    {
        wxString msg = wxT("请输入完整的服务器IP地址和端口信息。");
        wxMessageBox(msg, wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
        return  false;
    }

	if(pNetOperation->Conn(strServerIP,nPort,1000) == false)
	{
        wxMessageBox(wxT("连接远程服务器失败。"), wxT("错误信息"), wxOK | wxICON_INFORMATION, this);
        return  false;
	}
       
    return true;

}

bool PSSPlugClientFrame::Send_Multiple_Plug()
{
    char szText[MAX_BUFF_200] = {'\0'};
    this->SendData(szText);


    m_blIsRun = true;
    m_objPassTest.Init();

    while(m_blIsRun)
    {
        short nRecvCommandID = 0;
        int nRetA          = 0;
        int nRetB          = 0;
        bool blState =pNetOperation->Send_Plug(szText,nRecvCommandID,nRetB,nRetA);
    
    
//        if(blIsDisPlay == true)
//        {
//            char    szReturnText[MAX_BUFF_50]  = {'\0'};
//            wchar_t wszReturnText[MAX_BUFF_50] = {'\0'};
//            sprintf_s(szReturnText, MAX_BUFF_50, "PlugA:%d,PlugB:%d.", nRetA, nRetB);
//            this->TextCtrl_ServerResponse->SetValue( wxString::Format(wxT("接收回应包字节数:%d,PlugA应答:%d,PlugB应答:%d."),nRecvLength + sizeof(int), nRetA, nRetB));
//        }

    
        if(true == blState)
        {
            m_objPassTest.m_nSendCount++;
            m_objPassTest.m_nRecvCount++;
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(100*10));

    }

    pNetOperation->Close();
    this->Timer_ShowData.Stop();


    Show_Send_List();

    return  true;

}



void PSSPlugClientFrame::Show_Send_List()
{
    this->TextCtrl_SendCount->SetValue( wxString::Format(wxT("%d"),m_objPassTest.m_nSendCount));
    this->TextCtrl_RecvCount->SetValue( wxString::Format(wxT("%d"),m_objPassTest.m_nRecvCount));

}
