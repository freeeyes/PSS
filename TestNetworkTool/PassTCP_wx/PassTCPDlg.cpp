/***************************************************************
 * Name:      PassTCPDlg.cpp
 * Purpose:   Code for Application Frame
 * Author:    Smith ()
 * Created:   2019-02-17
 * Copyright: Smith ()
 * License:
 **************************************************************/

#include "PassTCPDlg.h"
#include <wx/msgdlg.h>

#include "macro.h"
#include "PacketDlg.h"



//(*InternalHeaders(CPassTCPDlg)
#include <wx/bitmap.h>
#include <wx/icon.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)




//线程执行
int  ThreadProc(void* argv)
{

    CClientTcpSocket* pClientTcpSocket = (CClientTcpSocket* )argv;
    pClientTcpSocket->Run();

    return 0;
}

int  ThreadUDPProc(void* argv)
{

    CClientUdpSocket* pClientUdpSocket = (CClientUdpSocket* )argv;
    pClientUdpSocket->Run();

    return 0;
}




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

//(*IdInit(CPassTCPDlg)
const long CPassTCPDlg::ID_STATICBOX1 = wxNewId();
const long CPassTCPDlg::ID_STATICTEXT1 = wxNewId();
const long CPassTCPDlg::ID_STATICTEXT2 = wxNewId();
const long CPassTCPDlg::ID_STATICBOX2 = wxNewId();
const long CPassTCPDlg::ID_STATICTEXT3 = wxNewId();
const long CPassTCPDlg::ID_STATICTEXT4 = wxNewId();
const long CPassTCPDlg::ID_STATICTEXT5 = wxNewId();
const long CPassTCPDlg::ID_STATICBOX3 = wxNewId();
const long CPassTCPDlg::ID_STATICTEXT6 = wxNewId();
const long CPassTCPDlg::ID_TEXTCTRL1 = wxNewId();
const long CPassTCPDlg::ID_CHECKBOX1 = wxNewId();
const long CPassTCPDlg::ID_BUTTON_LuaAdvance = wxNewId();
const long CPassTCPDlg::ID_BUTTON_StatTest = wxNewId();
const long CPassTCPDlg::ID_BUTTON_StopTest = wxNewId();
const long CPassTCPDlg::ID_BUTTON_ExportTestData = wxNewId();
const long CPassTCPDlg::ID_BUTTON_SetRadomPacket = wxNewId();
const long CPassTCPDlg::ID_TEXTCTRL2 = wxNewId();
const long CPassTCPDlg::ID_TEXTCTRL3 = wxNewId();
const long CPassTCPDlg::ID_TEXTCTRL4 = wxNewId();
const long CPassTCPDlg::ID_TEXTCTRL5 = wxNewId();
const long CPassTCPDlg::ID_TEXTCTRL6 = wxNewId();
const long CPassTCPDlg::ID_STATICTEXT7 = wxNewId();
const long CPassTCPDlg::ID_STATICTEXT8 = wxNewId();
const long CPassTCPDlg::ID_STATICTEXT9 = wxNewId();
const long CPassTCPDlg::ID_STATICTEXT10 = wxNewId();
const long CPassTCPDlg::ID_TEXTCTRL7 = wxNewId();
const long CPassTCPDlg::ID_TEXTCTRL8 = wxNewId();
const long CPassTCPDlg::ID_TEXTCTRL9 = wxNewId();
const long CPassTCPDlg::ID_TEXTCTRL10 = wxNewId();
const long CPassTCPDlg::ID_CHECKBOX2 = wxNewId();
const long CPassTCPDlg::ID_CHECKBOX3 = wxNewId();
const long CPassTCPDlg::ID_CHECKBOX4 = wxNewId();
const long CPassTCPDlg::ID_CHECKBOX5 = wxNewId();
const long CPassTCPDlg::ID_CHECKBOX6 = wxNewId();
const long CPassTCPDlg::ID_CHECKBOX7 = wxNewId();
const long CPassTCPDlg::ID_CHECKBOX8 = wxNewId();
const long CPassTCPDlg::ID_RADIOBOX1 = wxNewId();
const long CPassTCPDlg::ID_STATICTEXT11 = wxNewId();
const long CPassTCPDlg::ID_STATICTEXT12 = wxNewId();
const long CPassTCPDlg::ID_STATICTEXT13 = wxNewId();
const long CPassTCPDlg::ID_STATICTEXT14 = wxNewId();
const long CPassTCPDlg::ID_STATICTEXT15 = wxNewId();
const long CPassTCPDlg::ID_STATICTEXT16 = wxNewId();
const long CPassTCPDlg::ID_TEXTCTRL11 = wxNewId();
const long CPassTCPDlg::ID_TEXTCTRL12 = wxNewId();
const long CPassTCPDlg::ID_TEXTCTRL13 = wxNewId();
const long CPassTCPDlg::ID_TEXTCTRL14 = wxNewId();
const long CPassTCPDlg::ID_TEXTCTRL15 = wxNewId();
const long CPassTCPDlg::ID_TEXTCTRL16 = wxNewId();
const long CPassTCPDlg::ID_STATICTEXT17 = wxNewId();
const long CPassTCPDlg::ID_STATICTEXT18 = wxNewId();
const long CPassTCPDlg::ID_STATICTEXT19 = wxNewId();
const long CPassTCPDlg::ID_TEXTCTRL17 = wxNewId();
const long CPassTCPDlg::ID_TEXTCTRL18 = wxNewId();
const long CPassTCPDlg::ID_TEXTCTRL19 = wxNewId();
const long CPassTCPDlg::ID_STATICTEXT20 = wxNewId();
const long CPassTCPDlg::ID_TEXTCTRL20 = wxNewId();
const long CPassTCPDlg::ID_STATICTEXT21 = wxNewId();
const long CPassTCPDlg::ID_TEXTCTRL21 = wxNewId();
const long CPassTCPDlg::ID_STATICTEXT22 = wxNewId();
const long CPassTCPDlg::ID_CHOICE1 = wxNewId();
const long CPassTCPDlg::ID_TEXTCTRL22 = wxNewId();
const long CPassTCPDlg::ID_PANEL1 = wxNewId();
const long CPassTCPDlg::ID_TIMER_Test = wxNewId();
//*)

BEGIN_EVENT_TABLE(CPassTCPDlg,wxFrame)
    //(*EventTable(CPassTCPDlg)
    //*)
END_EVENT_TABLE()

CPassTCPDlg::CPassTCPDlg(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(CPassTCPDlg)
    Create(parent, id, wxT("TCP压力测试通用工具"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
    SetClientSize(wxSize(769,510));
    {
        wxIcon FrameIcon;
        FrameIcon.CopyFromBitmap(wxBitmap(wxImage(_T("../../../Icon.jpg"))));
        SetIcon(FrameIcon);
    }
    Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(224,216), wxSize(752,514), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    StaticBox1 = new wxStaticBox(Panel1, ID_STATICBOX1, wxT("参数配置"), wxPoint(0,0), wxSize(768,296), 0, _T("ID_STATICBOX1"));
    StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT1, wxT("服务器IP"), wxPoint(24,22), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    StaticText2 = new wxStaticText(Panel1, ID_STATICTEXT2, wxT("压测线程数"), wxPoint(24,52), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    StaticBox2 = new wxStaticBox(Panel1, ID_STATICBOX2, wxT("压力测试参数"), wxPoint(8,288), wxSize(760,232), 0, _T("ID_STATICBOX2"));
    StaticText3 = new wxStaticText(Panel1, ID_STATICTEXT3, wxT("连接间延迟毫秒"), wxPoint(24,80), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    StaticText4 = new wxStaticText(Panel1, ID_STATICTEXT4, wxT("数据包间隔"), wxPoint(24,108), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    StaticText5 = new wxStaticText(Panel1, ID_STATICTEXT5, wxT("设置发包总数，如果无限设置为零"), wxPoint(24,140), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    StaticBox3 = new wxStaticBox(Panel1, ID_STATICBOX3, wxT("高级模式(LUA文件控制模式)"), wxPoint(16,160), wxSize(736,88), 0, _T("ID_STATICBOX3"));
    StaticText6 = new wxStaticText(Panel1, ID_STATICTEXT6, wxT("Lua控制文件路径"), wxPoint(40,188), wxDefaultSize, 0, _T("ID_STATICTEXT6"));
    TextCtrl_LuaControlFilePath = new wxTextCtrl(Panel1, ID_TEXTCTRL1, wxEmptyString, wxPoint(168,188), wxSize(576,24), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    CheckBoxLuaAdvance = new wxCheckBox(Panel1, ID_CHECKBOX1, wxT("启用高级模式"), wxPoint(32,216), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
    CheckBoxLuaAdvance->SetValue(false);
    Button_LuaAdvance = new wxButton(Panel1, ID_BUTTON_LuaAdvance, wxT("打开文件夹"), wxPoint(160,216), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_LuaAdvance"));
    Button_StatTest = new wxButton(Panel1, ID_BUTTON_StatTest, wxT("开始压测"), wxPoint(32,248), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_StatTest"));
    Button_StopTest = new wxButton(Panel1, ID_BUTTON_StopTest, wxT("停止压测"), wxPoint(200,248), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_StopTest"));
    Button_ExportTestData = new wxButton(Panel1, ID_BUTTON_ExportTestData, wxT("导出压测数据文档"), wxPoint(352,248), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_ExportTestData"));
    Button_SetRadomPacket = new wxButton(Panel1, ID_BUTTON_SetRadomPacket, wxT("设置随机数据包序列"), wxPoint(528,248), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON_SetRadomPacket"));
    TextCtrl_ServerIP = new wxTextCtrl(Panel1, ID_TEXTCTRL2, wxT("127.0.0.1"), wxPoint(120,20), wxSize(-1,24), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
    TextCtrl_ServerIP->SetMaxLength(15);
    TextCtrl_TestThreadCount = new wxTextCtrl(Panel1, ID_TEXTCTRL3, wxT("1"), wxPoint(120,48), wxSize(-1,24), 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
    TextCtrl_SocketInterval = new wxTextCtrl(Panel1, ID_TEXTCTRL4, wxT("0"), wxPoint(120,76), wxSize(-1,24), 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
    TextCtrl_PacketTimewait = new wxTextCtrl(Panel1, ID_TEXTCTRL5, wxT("0"), wxPoint(120,104), wxSize(-1,24), 0, wxDefaultValidator, _T("ID_TEXTCTRL5"));
    TextCtrl_PacketSendMax = new wxTextCtrl(Panel1, ID_TEXTCTRL6, wxT("0"), wxPoint(232,136), wxSize(184,24), 0, wxDefaultValidator, _T("ID_TEXTCTRL6"));
    StaticText7 = new wxStaticText(Panel1, ID_STATICTEXT7, wxT("服务器端口"), wxPoint(232,22), wxDefaultSize, 0, _T("ID_STATICTEXT7"));
    StaticText8 = new wxStaticText(Panel1, ID_STATICTEXT8, wxT("接收超时秒数"), wxPoint(232,48), wxDefaultSize, 0, _T("ID_STATICTEXT8"));
    StaticText9 = new wxStaticText(Panel1, ID_STATICTEXT9, wxT("接收字节限定"), wxPoint(232,72), wxDefaultSize, 0, _T("ID_STATICTEXT9"));
    StaticText10 = new wxStaticText(Panel1, ID_STATICTEXT10, wxT("UDP接收端口"), wxPoint(236,104), wxDefaultSize, 0, _T("ID_STATICTEXT10"));
    TextCtrl_ServerPort = new wxTextCtrl(Panel1, ID_TEXTCTRL7, wxT("10002"), wxPoint(320,20), wxSize(-1,24), 0, wxDefaultValidator, _T("ID_TEXTCTRL7"));
    TextCtrl_ServerPort->SetMaxLength(5);
    TextCtrl_RecvTimeOut = new wxTextCtrl(Panel1, ID_TEXTCTRL8, wxT("1000"), wxPoint(320,48), wxSize(-1,24), 0, wxDefaultValidator, _T("ID_TEXTCTRL8"));
    TextCtrl_RecvLength = new wxTextCtrl(Panel1, ID_TEXTCTRL9, wxT("12"), wxPoint(320,76), wxSize(-1,24), 0, wxDefaultValidator, _T("ID_TEXTCTRL9"));
    TextCtrl_ClientUdpPort = new wxTextCtrl(Panel1, ID_TEXTCTRL10, wxT("20002"), wxPoint(320,104), wxSize(-1,24), 0, wxDefaultValidator, _T("ID_TEXTCTRL10"));
    CheckBox_IsAlwayConnect = new wxCheckBox(Panel1, ID_CHECKBOX2, wxT("是否开启长连接"), wxPoint(448,22), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
    CheckBox_IsAlwayConnect->SetValue(false);
    CheckBox_RadomaDelay = new wxCheckBox(Panel1, ID_CHECKBOX3, wxT("是否开启随机延时"), wxPoint(600,22), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX3"));
    CheckBox_RadomaDelay->SetValue(false);
    CheckBox_IsRecv = new wxCheckBox(Panel1, ID_CHECKBOX4, wxT("是否接收应答包"), wxPoint(448,48), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX4"));
    CheckBox_IsRecv->SetValue(true);
    CheckBox_IsBroken = new wxCheckBox(Panel1, ID_CHECKBOX5, wxT("是否开启断线自动重连"), wxPoint(600,48), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX5"));
    CheckBox_IsBroken->SetValue(true);
    CheckBox_RadomSendCount = new wxCheckBox(Panel1, ID_CHECKBOX6, wxT("是否启用随机数据包数(随机1-10个数据包)"), wxPoint(448,72), wxSize(256,17), 0, wxDefaultValidator, _T("ID_CHECKBOX6"));
    CheckBox_RadomSendCount->SetValue(false);
    CheckBoxIsWriteLog = new wxCheckBox(Panel1, ID_CHECKBOX7, wxT("是否记录数据日志"), wxPoint(448,96), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX7"));
    CheckBoxIsWriteLog->SetValue(false);
    CheckBox_SendOne = new wxCheckBox(Panel1, ID_CHECKBOX8, wxT("只发送一个包"), wxPoint(600,96), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX8"));
    CheckBox_SendOne->SetValue(false);
    wxString __wxRadioBoxChoices_1[3] =
    {
        wxT("TCP链接"),
        wxT("UDP链接"),
        wxT("WebSocket链接")
    };
    RadioBox_nRadio = new wxRadioBox(Panel1, ID_RADIOBOX1, wxEmptyString, wxPoint(448,112), wxSize(304,48), 3, __wxRadioBoxChoices_1, 1, wxRA_VERTICAL|wxFULL_REPAINT_ON_RESIZE, wxDefaultValidator, _T("ID_RADIOBOX1"));
    StaticText11 = new wxStaticText(Panel1, ID_STATICTEXT11, wxT("连接成功数"), wxPoint(32,312), wxDefaultSize, 0, _T("ID_STATICTEXT11"));
    StaticText12 = new wxStaticText(Panel1, ID_STATICTEXT12, wxT("发送成功数"), wxPoint(32,344), wxDefaultSize, 0, _T("ID_STATICTEXT12"));
    StaticText13 = new wxStaticText(Panel1, ID_STATICTEXT13, wxT("接收成功数"), wxPoint(32,376), wxDefaultSize, 0, _T("ID_STATICTEXT13"));
    StaticText14 = new wxStaticText(Panel1, ID_STATICTEXT14, wxT("当前连接数"), wxPoint(32,406), wxDefaultSize, 0, _T("ID_STATICTEXT14"));
    StaticText15 = new wxStaticText(Panel1, ID_STATICTEXT15, wxT("最小响应时间"), wxPoint(32,436), wxDefaultSize, 0, _T("ID_STATICTEXT15"));
    StaticText16 = new wxStaticText(Panel1, ID_STATICTEXT16, wxT("发送字节数"), wxPoint(32,466), wxDefaultSize, 0, _T("ID_STATICTEXT16"));
    TextCtrl_SuccessConnect = new wxTextCtrl(Panel1, ID_TEXTCTRL11, wxEmptyString, wxPoint(112,308), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL11"));
    TextCtrl_SuccessSend = new wxTextCtrl(Panel1, ID_TEXTCTRL12, wxEmptyString, wxPoint(112,340), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL12"));
    TextCtrl_SuccessRecv = new wxTextCtrl(Panel1, ID_TEXTCTRL13, wxEmptyString, wxPoint(112,372), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL13"));
    TextCtrl_CurrConnect = new wxTextCtrl(Panel1, ID_TEXTCTRL14, wxEmptyString, wxPoint(112,402), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL14"));
    TextCtrl_MinTime = new wxTextCtrl(Panel1, ID_TEXTCTRL15, wxEmptyString, wxPoint(112,432), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL15"));
    TextCtrl_SendByteCount = new wxTextCtrl(Panel1, ID_TEXTCTRL16, wxEmptyString, wxPoint(112,464), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL16"));
    StaticText17 = new wxStaticText(Panel1, ID_STATICTEXT17, wxT("连接失败数"), wxPoint(240,312), wxDefaultSize, 0, _T("ID_STATICTEXT17"));
    StaticText18 = new wxStaticText(Panel1, ID_STATICTEXT18, wxT("发送失败数"), wxPoint(240,344), wxDefaultSize, 0, _T("ID_STATICTEXT18"));
    StaticText19 = new wxStaticText(Panel1, ID_STATICTEXT19, wxT("接收失败数"), wxPoint(240,376), wxDefaultSize, 0, _T("ID_STATICTEXT19"));
    TextCtrl_FailConnect = new wxTextCtrl(Panel1, ID_TEXTCTRL17, wxEmptyString, wxPoint(320,308), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL17"));
    TextCtrl_FailSend = new wxTextCtrl(Panel1, ID_TEXTCTRL18, wxEmptyString, wxPoint(320,340), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL18"));
    TextCtrl_FailRecv = new wxTextCtrl(Panel1, ID_TEXTCTRL19, wxEmptyString, wxPoint(320,372), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL19"));
    StaticText20 = new wxStaticText(Panel1, ID_STATICTEXT20, wxT("最大响应时间"), wxPoint(240,436), wxDefaultSize, 0, _T("ID_STATICTEXT20"));
    TextCtrl_MaxTime = new wxTextCtrl(Panel1, ID_TEXTCTRL20, wxEmptyString, wxPoint(320,432), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL20"));
    StaticText21 = new wxStaticText(Panel1, ID_STATICTEXT21, wxT("接收字节数"), wxPoint(240,466), wxDefaultSize, 0, _T("ID_STATICTEXT21"));
    TextCtrl_RecvByteCount = new wxTextCtrl(Panel1, ID_TEXTCTRL21, wxEmptyString, wxPoint(320,464), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL21"));
    StaticText22 = new wxStaticText(Panel1, ID_STATICTEXT22, wxT("压测发送二进制串"), wxPoint(448,312), wxDefaultSize, 0, _T("ID_STATICTEXT22"));
    Choice_SendBuffStyle = new wxChoice(Panel1, ID_CHOICE1, wxPoint(560,308), wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
    Choice_SendBuffStyle->SetSelection( Choice_SendBuffStyle->Append(wxT("二进制模式")) );
    Choice_SendBuffStyle->Append(wxT("文本模式"));
    TextCtrl_SendText = new wxTextCtrl(Panel1, ID_TEXTCTRL22, wxT("01 00 00 10 08 00 00 00 46 45 45 45 45 59 45 53 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 be cd aa 8f 3c 01 00 00"), wxPoint(448,336), wxSize(288,152), wxTE_MULTILINE, wxDefaultValidator, _T("ID_TEXTCTRL22"));
    DirDialog_Lua = new wxDirDialog(this, wxT("打开目录"), wxEmptyString, wxDD_DEFAULT_STYLE|wxDD_DIR_MUST_EXIST|wxDD_CHANGE_DIR, wxDefaultPosition, wxDefaultSize, _T("wxDirDialog"));
    Timer_Test.SetOwner(this, ID_TIMER_Test);
    Timer_Test.Start(1000, false);

    Connect(ID_BUTTON_LuaAdvance,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CPassTCPDlg::OnButton_LuaAdvanceClick);
    Connect(ID_BUTTON_StatTest,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CPassTCPDlg::OnButton_StatTestClick);
    Connect(ID_BUTTON_StopTest,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CPassTCPDlg::OnButton_StopTestClick);
    Connect(ID_BUTTON_ExportTestData,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CPassTCPDlg::OnButton_ExportTestDataClick);
    Connect(ID_BUTTON_SetRadomPacket,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&CPassTCPDlg::OnButton_SetRadomPacketClick);
    Connect(ID_TEXTCTRL2,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&CPassTCPDlg::OnTextCtrl_ServerIPText);
    Connect(ID_CHOICE1,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&CPassTCPDlg::OnChoice_SendBuffStyleSelect);
    Connect(ID_TIMER_Test,wxEVT_TIMER,(wxObjectEventFunction)&CPassTCPDlg::OnTimer_TestTrigger);
    Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&CPassTCPDlg::OnClose);
    //*)
}

CPassTCPDlg::~CPassTCPDlg()
{
    //(*Destroy(CPassTCPDlg)
    //*)
}

void CPassTCPDlg::OnQuit(wxCommandEvent& event)
{
    Close();
}

void CPassTCPDlg::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxT("PassTCP，1.0 版，压力测试工具\r\nCopyright freeeeyes (C) 2013\r\n");
    wxMessageBox(msg, wxT("关于 PassTCP"), wxOK | wxICON_INFORMATION, this);

}

void CPassTCPDlg::OnButton_LuaAdvanceClick(wxCommandEvent& event)
{
    //打开Lua文件夹
    //首先判断高级选项是否已经打开

    if(this->CheckBoxLuaAdvance->IsChecked()   ==false)
    {
        wxString msg = wxT("请先选择启用高级模式开关。");
        wxMessageBox(msg, wxT("关于 PassTCP"), wxOK | wxICON_INFORMATION, this);
        return;
    }

    if  (this->DirDialog_Lua->ShowModal()    ==wxID_OK)
    {
        wxString    LuaDirPath  =this->DirDialog_Lua->GetPath();
        this->TextCtrl_LuaControlFilePath->SetValue(LuaDirPath);
    }

}

void CPassTCPDlg::OnButton_StatTestClick(wxCommandEvent& event)
{
    //开始压测代码在这里
    char* pSendData = NULL;
    ClearResult();
    Close();


    //m_txtThreadCount.GetWindowText(strData);
    int nThreadCount = wxAtoi(this->TextCtrl_TestThreadCount->GetValue());
    //m_txtSendCount.GetWindowText(strData);
    int nAllSendCount = wxAtoi(this->TextCtrl_PacketSendMax->GetValue());

    //根据线程数计算发送量
    int nThreadSendCount = nAllSendCount / nThreadCount;

    for(int i = 0; i < nThreadCount; i++)
    {
        //读取线程信息
        _Socket_Info* pSocket_Info             = new _Socket_Info();
        _Socket_State_Info* pSocket_State_Info = new _Socket_State_Info();

        ENUM_TYPE_PROTOCOL emType = ENUM_PROTOCOL_TCP;
        //默认TCP类型，0是TCP，1是UDP



        switch(this->RadioBox_nRadio->GetSelection())
        {
        case 0:  //TCP
            {
                pSocket_Info->m_nConnectType = 0;
                emType = ENUM_PROTOCOL_TCP;

                if(m_pLogic == NULL)
                {
                    CNomalLogic* pNomalLogic = new CNomalLogic();
                    m_pLogic = (CBaseDataLogic* )pNomalLogic;
                    pSocket_Info->m_pLogic = m_pLogic;
                }
                else
                {
                    pSocket_Info->m_pLogic = m_pLogic;
                }

                break;
            }

        case 1:   //UDP
            {
                pSocket_Info->m_nConnectType = 1;
                emType = ENUM_PROTOCOL_UDP;

                if(m_pLogic == NULL)
                {
                    CNomalLogic* pNomalLogic = new CNomalLogic();
                    m_pLogic =  (CBaseDataLogic* )pNomalLogic;
                    pSocket_Info->m_pLogic = m_pLogic;
                }
                else
                {
                    pSocket_Info->m_pLogic = m_pLogic;
                }

                break;
            }

        case 2:     //WebSocket
            {
                pSocket_Info->m_nConnectType = 0;
                emType = ENUM_PROTOCOL_WEBSOCKET;

                if(m_pLogic == NULL)
                {
                    CWebSocketLogic* pWebSocketLogic = new CWebSocketLogic();
                    pSocket_Info->m_pLogic = (CBaseDataLogic* )pWebSocketLogic;
                }
                else
                {
                    pSocket_Info->m_pLogic = m_pLogic;
                }

                break;
            }
        }


        //std::string stlstring = std::string(strData.mb_str());
        wxString    m_szSerevrIP    =this->TextCtrl_ServerIP->GetValue();
        strncpy(pSocket_Info->m_szSerevrIP, (const char*)m_szSerevrIP.mb_str(), 16);
        int nDecLen =this->TextCtrl_ServerIP->GetValue().length();
        pSocket_Info->m_szSerevrIP[nDecLen] = '\0';

        pSocket_Info->m_nPort = wxAtoi(this->TextCtrl_ServerPort->GetValue());
        pSocket_Info->m_nRecvTimeout = wxAtoi(this->TextCtrl_RecvTimeOut->GetValue());
        pSocket_Info->m_nDelaySecond = wxAtoi(this->TextCtrl_SocketInterval->GetValue());
        pSocket_Info->m_pLogic->SetRecvLength(wxAtoi(this->TextCtrl_RecvLength->GetValue()));
        pSocket_Info->m_nUdpClientPort = wxAtoi(this->TextCtrl_ClientUdpPort->GetValue());
        pSocket_Info->m_nPacketTimewait = wxAtoi(this->TextCtrl_PacketTimewait->GetValue());


        if(nThreadSendCount > 0)
        {
            if(i == nThreadCount - 1 && nThreadCount > 1)
            {
                pSocket_Info->m_nSendCount = nThreadSendCount + ( nAllSendCount % nThreadSendCount);
            }
            else
            {
                pSocket_Info->m_nSendCount = nThreadSendCount;
            }
        }

        if(this->CheckBoxLuaAdvance->IsChecked() == true)
        {
            //如果是Lua文件模式，直接初始化一个100k的数据块
            //然后根据脚本去组织发送数据
            pSocket_Info->m_pLogic->InitSendSize(100 * MAX_BUFF_1024);
        }
        else
        {

            std::string SendText    =std::string(this->TextCtrl_SendText->GetValue());
            int nBufferSize =SendText.length();
            pSendData = new char[nBufferSize];
            strncpy(pSendData, (const char*)SendText.c_str(), nBufferSize);

            pSendData[nBufferSize] = '\0';



            if(this->Choice_SendBuffStyle->GetSelection() == 0)
            {
                if(emType != ENUM_PROTOCOL_WEBSOCKET)
                {
                    nDecLen = strlen(pSendData);

                    //如果是二进制模式
                    CConvertBuffer objConvertBuffer;
                    //获得要转换的数据块大小
                    pSocket_Info->m_pLogic->InitSendSize(objConvertBuffer.GetBufferSize(pSendData, nDecLen));
                    //将数据串转换成二进制串
                    int nSendLen = nDecLen;
                    objConvertBuffer.Convertstr2charArray(pSendData, strlen(pSendData),
                                                          (unsigned char*)pSocket_Info->m_pLogic->GetSendData(), nSendLen);
                }
                else
                {
                    //如果是webSocket模式
                    char szOriData[100 * MAX_BUFF_1024] = {'\0'};
                    //如果是二进制模式
                    CConvertBuffer objConvertBuffer;
                    //获得要转换的数据块大小
                    pSocket_Info->m_pLogic->InitSendSize(100 * MAX_BUFF_1024);
                    //将数据串转换成二进制串
                    int nSendLen = 100 * MAX_BUFF_1024;
                    objConvertBuffer.Convertstr2charArray(pSendData, strlen(pSendData),
                                                          (unsigned char*)szOriData, nSendLen);

                    pSocket_Info->m_pLogic->SetSendBuff(szOriData, nSendLen);
                }
            }
            else
            {
                if(emType != ENUM_PROTOCOL_WEBSOCKET)
                {
                    //如果是文本模式
                    pSocket_Info->m_pLogic->InitSendSize(nDecLen);
                    memcpy(pSocket_Info->m_pLogic->GetSendData(),  pSendData, nDecLen);
                }
                else
                {
                    //如果是webSocket模式
                    pSocket_Info->m_pLogic->InitSendSize(100 * MAX_BUFF_1024);
                    pSocket_Info->m_pLogic->SetSendBuff(pSendData, nDecLen);
                }
            }

            delete[] pSendData;

        }

        pSocket_Info->m_blIsAlwayConnect = this->CheckBox_IsAlwayConnect->IsChecked();

        pSocket_Info->m_blIsRadomaDelay = this->CheckBox_RadomaDelay->IsChecked();
        pSocket_Info->m_blIsRecv = this->CheckBox_IsRecv->IsChecked();

        pSocket_Info->m_blIsBroken = this->CheckBox_IsBroken->IsChecked();

        pSocket_Info->m_blIsSendCount = this->CheckBox_RadomSendCount->IsChecked();
        pSocket_Info->m_blIsWriteFile = this->CheckBoxIsWriteLog->IsChecked();
        pSocket_Info->m_blIsSendOne = this->CheckBox_SendOne->IsChecked();
        pSocket_Info->m_blLuaAdvance = this->CheckBoxLuaAdvance->IsChecked();

        //是否开启高级模式
        if(this->CheckBoxLuaAdvance->IsChecked() == true)
        {
            //如果打开高级模式，则读取文件

            wxString    m_txtLuaFilePath    =this->TextCtrl_LuaControlFilePath->GetValue();

            if(m_txtLuaFilePath.length() == 0)
            {
                wxMessageBox(wxT("您已经开启了高级模式，必须设置必要的Lua文件名。"), this->GetTitle(), wxOK | wxICON_INFORMATION, this);
                delete pSocket_Info;
                delete pSocket_State_Info;
                return;
            }

            std::string LuaFilePathText    =std::string(m_txtLuaFilePath);
            int nBufferSize =LuaFilePathText.length();
            strncpy(pSocket_Info->m_szLuaFileName, (const char*)LuaFilePathText.c_str(), nBufferSize);
            pSocket_Info->m_szLuaFileName[nDecLen] = '\0';

        }


        if(pSocket_Info->m_nConnectType == 0)
        {
            //TCP发送
            CClientTcpSocket* pClientTcpSocket = new CClientTcpSocket();
            pClientTcpSocket->SetSocketThread(pSocket_Info, pSocket_State_Info);
            pClientTcpSocket->SetThreadID(i);


            std::thread thread_tcp(ThreadProc, pClientTcpSocket);

            //DWORD  ThreadID = 0;
            //CreateThread(NULL, NULL, ThreadProc, (LPVOID)pClientTcpSocket, NULL, &ThreadID);

            m_vecClientTcpSocket.push_back(pClientTcpSocket);
        }
        else
        {
            //UDP发送
            CClientUdpSocket* pClientUdpSocket = new CClientUdpSocket();
            pClientUdpSocket->SetSocketThread(pSocket_Info, pSocket_State_Info);

            //DWORD  ThreadID = 0;
            //CreateThread(NULL, NULL, ThreadUDPProc, (LPVOID)pClientUdpSocket, NULL, &ThreadID);

            std::thread thread_udp(ThreadUDPProc, pClientUdpSocket);

            m_vecClientUdpSocket.push_back(pClientUdpSocket);
        }

        m_tmBegin = wxDateTime::Now ();
        m_blIsRun = true;
    }
}

void CPassTCPDlg::OnButton_StopTestClick(wxCommandEvent& event)
{
    //停止压测
    int nCount = (int)m_vecClientUdpSocket.size();

    for(int i = 0; i < nCount; i++)
    {
        CClientUdpSocket* pClientUdpSocket = (CClientUdpSocket* )m_vecClientUdpSocket[i];

        if(NULL != pClientUdpSocket)
        {
            pClientUdpSocket->Stop();
        }
    }

    nCount = (int)m_vecClientTcpSocket.size();

    for(int i = 0; i < nCount; i++)
    {
        CClientTcpSocket* pClientTcpSocket = (CClientTcpSocket* )m_vecClientTcpSocket[i];

        if(NULL != pClientTcpSocket)
        {
            pClientTcpSocket->Stop();
        }
    }

    m_tmEnd = wxDateTime::Now ();
    m_blIsRun = false;

}



void CPassTCPDlg::OnButton_ExportTestDataClick(wxCommandEvent& event)
{
    //写入压测报告
    FILE* pFile = NULL;
    char szFileName[20];
    sprintf(szFileName, "StressTest.log");
    pFile   =fopen( szFileName, "a+");

    if(pFile == NULL)
    {
        wxMessageBox(wxT("导出压测报告失败，文件不存"), this->GetTitle(), wxOK | wxICON_INFORMATION, this);
        return;
    }

    char szLogText[1024] = {'\0'};

    sprintf(szLogText,  "=============================================\n");
    fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

    //默认TCP类型，0是TCP，1是UDP

    switch(this->RadioBox_nRadio->GetSelection())
    {
    case 0:
        sprintf(szLogText,  "压测类型:TCP\n");
        break;

    case 1:
        sprintf(szLogText,  "压测类型:UDP\n");
        break;

    case 2:
        sprintf(szLogText,  "压测类型:WebSocket\n");
        break;
    }

    fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);


    std::string    m_szSerevrIP    =std::string(this->TextCtrl_ServerIP->GetValue());
    int    m_nPort    =wxAtoi(this->TextCtrl_ServerPort->GetValue());

    sprintf(szLogText,  "压测IP:%s, 压测端口:%d.\n", m_szSerevrIP.c_str(), m_nPort);


    //CString strBeginTime = m_tmBegin.Format("%Y-%m-%d %H:%M:%S");
    sprintf(szLogText,  "压测开始时间为: %04d-%02d-%02d %02d:%02d:%02d\n", m_tmBegin.GetYear(), m_tmBegin.GetMonth(), m_tmBegin.GetDay(), m_tmBegin.GetHour(), m_tmBegin.GetMinute(), m_tmBegin.GetSecond());
    fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

    if(m_blIsRun == true)
    {
        wxDateTime tmNow = wxDateTime::Now();

        //CString strNowTime = tmNow.Format("%Y-%m-%d %H:%M:%S");
        sprintf(szLogText,  "压测结束时间为: %04d-%02d-%02d %02d:%02d:%02d\n", tmNow.GetYear(), tmNow.GetMonth(), tmNow.GetDay(), tmNow.GetHour(), tmNow.GetMinute(), tmNow.GetSecond());
        fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);
    }
    else
    {
        //CString strEndTime = m_tmEnd.Format("%Y-%m-%d %H:%M:%S");
        sprintf(szLogText,  "压测结束时间为: %04d-%02d-%02d %02d:%02d:%02d\n", m_tmEnd.GetYear(), m_tmEnd.GetMonth(), m_tmEnd.GetDay(), m_tmEnd.GetHour(), m_tmEnd.GetMinute(), m_tmEnd.GetSecond());
        fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);
    }


    int m_txtThreadCount = wxAtoi(this->TextCtrl_TestThreadCount->GetValue());
    int m_txtSuccessConnect = wxAtoi(this->TextCtrl_SuccessConnect->GetValue());
    int m_txtSuccessSend = wxAtoi(this->TextCtrl_SuccessSend->GetValue());
    int m_txtSuccessRecv = wxAtoi(this->TextCtrl_SuccessRecv->GetValue());
    int m_txtSendByteCount = wxAtoi(this->TextCtrl_SendByteCount->GetValue());
    int m_txtRecvByteCount = wxAtoi(this->TextCtrl_RecvByteCount->GetValue());
    int m_txtFailConnect    =wxAtoi(this->TextCtrl_FailConnect->GetValue());
    int m_txtFailSend = wxAtoi(this->TextCtrl_FailSend->GetValue());
    int m_txtFailRecv = wxAtoi(this->TextCtrl_FailRecv->GetValue());
    int m_txtMinTime = wxAtoi(this->TextCtrl_MinTime->GetValue());
    int m_txtMaxTime = wxAtoi(this->TextCtrl_MaxTime->GetValue());


    sprintf(szLogText,  "压测线程数:%d\n", m_txtThreadCount);
    fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

    sprintf(szLogText,  "创建成功连接数:%d\n",m_txtSuccessConnect);
    fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

    sprintf(szLogText,  "成功发送数据包数:%d\n", m_txtSuccessSend);
    fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

    sprintf(szLogText,  "成功接收数据包数:%d\n", m_txtSuccessRecv);
    fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

    sprintf(szLogText,  "发送字节数:%d\n", m_txtSendByteCount);
    fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

    sprintf(szLogText,  "接收字节数:%d\n", m_txtRecvByteCount);
    fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);


    sprintf(szLogText,  "连接失败数:%d\n",m_txtFailConnect);
    fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

    sprintf(szLogText,  "发送失败数据包数:%d\n", m_txtFailSend);
    fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

    sprintf(szLogText,  "接收失败数据包数:%d\n", m_txtFailRecv);
    fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

    sprintf(szLogText,  "最小单包响应时间:%d毫秒\n", m_txtFailRecv);
    fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

    sprintf(szLogText,  "最大单包响应时间:%d毫秒\n", m_txtMaxTime);
    fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);


    //连接成功百分比
    float fRote = 0.0f;
    int nSucccess = wxAtoi(this->TextCtrl_SuccessConnect->GetValue());
    int nFail     = wxAtoi(this->TextCtrl_FailConnect->GetValue());


    if(nSucccess + nFail == 0)
    {
        fRote = 0.0f;
    }
    else
    {
        fRote = (float)nSucccess/(nSucccess + nFail);
    }

    sprintf(szLogText,  "连接成功百分比:%f%%\n", fRote*100.0);
    fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

    //发送成功百分比
    nSucccess = wxAtoi(this->TextCtrl_SuccessSend->GetValue());
    nFail     =  wxAtoi(this->TextCtrl_FailSend->GetValue());

    if(nSucccess + nFail == 0)
    {
        fRote = 0.0f;
    }
    else
    {
        fRote = (float)nSucccess/(nSucccess + nFail);
    }

    sprintf(szLogText,  "发送数据包成功百分比:%f%%\n", fRote*100.0);
    fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

    //接收成功百分比
    nSucccess =  wxAtoi(this->TextCtrl_SuccessRecv->GetValue());
    nFail     = wxAtoi(this->TextCtrl_FailRecv->GetValue());

    if(nSucccess + nFail == 0)
    {
        fRote = 0.0f;
    }
    else
    {
        fRote = (float)nSucccess/(nSucccess + nFail);
    }

    sprintf(szLogText,  "接收数据包成功百分比:%f%%\n", fRote*100.0);
    fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

    sprintf(szLogText,  "=============================================\n");
    fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

    fclose(pFile);

    wxMessageBox(wxT("导出压测报告成功"), this->GetTitle(), wxOK | wxICON_INFORMATION, this);

}

void CPassTCPDlg::OnButton_SetRadomPacketClick(wxCommandEvent& event)
{
    //设置随机数据包序列
    CPacketDlg* objPacketDlg;
    objPacketDlg    =new CPacketDlg(this);


    switch(this->RadioBox_nRadio->GetSelection ())
    {
    case 0:
        {
            if(m_pLogic != NULL)
            {
                delete m_pLogic;
            }

            CNomalLogic* pNomalLogic = new CNomalLogic();
            m_pLogic = (CBaseDataLogic* )pNomalLogic;
            break;
        }

    case 1:
        {
            if(m_pLogic != NULL)
            {
                delete m_pLogic;
            }

            CNomalLogic* pNomalLogic = new CNomalLogic();
            m_pLogic = (CBaseDataLogic* )pNomalLogic;
            break;
        }

    case 2:
        {
            if(m_pLogic != NULL)
            {
                delete m_pLogic;
            }

            CWebSocketLogic* pWebSocketLogic = new CWebSocketLogic();
            m_pLogic = (CBaseDataLogic* )pWebSocketLogic;
            break;
        }
    }

    objPacketDlg->SetBaseDataLogic(m_pLogic);

    objPacketDlg->ShowModal();


}









void CPassTCPDlg::ClearResult()
{

    this->TextCtrl_SuccessConnect->SetValue(wxT("0"));
    this->TextCtrl_SuccessSend->SetValue(wxT("0"));
    this->TextCtrl_SuccessRecv->SetValue(wxT("0"));
    this->TextCtrl_CurrConnect->SetValue(wxT("0"));
    this->TextCtrl_FailConnect->SetValue(wxT("0"));
    this->TextCtrl_FailRecv->SetValue(wxT("0"));
    this->TextCtrl_FailSend->SetValue(wxT("0"));
    this->TextCtrl_FailConnect->SetValue(wxT("0"));
    this->TextCtrl_SendByteCount->SetValue(wxT("0"));
    this->TextCtrl_RecvByteCount->SetValue(wxT("0"));

}




void CPassTCPDlg::OnClose(wxCloseEvent& event)
{
    int nCount = m_vecClientUdpSocket.size();

    for(int i = 0; i < nCount; i++)
    {
        CClientUdpSocket* pClientUdpSocket = (CClientUdpSocket* )m_vecClientUdpSocket[i];

        if(NULL != pClientUdpSocket)
        {
            pClientUdpSocket->Stop();

            int dwSleep = 100;
            //Sleep(dwSleep);
            std::this_thread::sleep_for(std::chrono::milliseconds(dwSleep*1000));


            delete pClientUdpSocket;
            pClientUdpSocket = NULL;
        }
    }

    m_vecClientUdpSocket.clear();

    nCount = m_vecClientTcpSocket.size();

    for(int i = 0; i < nCount; i++)
    {
        CClientTcpSocket* pClientTcpSocket = (CClientTcpSocket* )m_vecClientTcpSocket[i];

        if(NULL != pClientTcpSocket)
        {
            pClientTcpSocket->Stop();


            int dwSleep = 100;
            //Sleep(dwSleep);
            std::this_thread::sleep_for(std::chrono::milliseconds(dwSleep*1000));

            delete pClientTcpSocket;
            pClientTcpSocket = NULL;
        }
    }

    m_vecClientTcpSocket.clear();

    if( m_pLogic != NULL)
    {
        if(m_pLogic->m_nClassTye == 1)
        {
            delete (CNomalLogic* )m_pLogic;
        }
        else
        {
            delete (CWebSocketLogic* )m_pLogic;
        }
    }

    this->Destroy();

}

void CPassTCPDlg::OnTimer_TestTrigger(wxTimerEvent& event)
{
    //if(nIDEvent == 1)
    {
        int nSuccessConnect = 0;
        int nSuccessSend    = 0;
        int nSuccessRecv    = 0;
        int ntCurrConnect   = 0;
        int nFailConnect    = 0;
        int nFailSend       = 0;
        int nFailRecv       = 0;
        int nCurrConnect    = 0;
        int nSendByteCount  = 0;
        int nRecvByteCount  = 0;
        int nMinTime        = 0;
        int nMaxTime        = 0;

        int nConnectType = 0;

        switch(this->RadioBox_nRadio->GetSelection ())
        {
        case 0:
            nConnectType = 0;
            break;

        case 1:
            nConnectType = 1;
            break;

        case 2:
            nConnectType = 0;
            break;
        }

        if(nConnectType == 0)
        {
            for(int i = 0; i < (int)m_vecClientTcpSocket.size(); i++)
            {
                CClientTcpSocket* pClientTcpSocket = (CClientTcpSocket* )m_vecClientTcpSocket[i];

                if(NULL != pClientTcpSocket)
                {
                    _Socket_State_Info* pSocket_State_Info = pClientTcpSocket->GetStateInfo();

                    if(NULL != pSocket_State_Info)
                    {
                        nSuccessConnect += pSocket_State_Info->m_nSuccessConnect;
                        nSuccessSend    += pSocket_State_Info->m_nSuccessSend;
                        nSuccessRecv    += pSocket_State_Info->m_nSuccessRecv;
                        nFailConnect    += pSocket_State_Info->m_nFailConnect;
                        nFailSend       += pSocket_State_Info->m_nFailSend;
                        nFailRecv       += pSocket_State_Info->m_nFailRecv;
                        nCurrConnect    += pSocket_State_Info->m_nCurrectSocket;
                        nSendByteCount  += pSocket_State_Info->m_nSendByteCount;
                        nRecvByteCount  += pSocket_State_Info->m_nRecvByteCount;

                        if(i == 0)
                        {
                            nMinTime = pSocket_State_Info->m_nMinRecvTime;
                            nMaxTime = pSocket_State_Info->m_nMaxRecvTime;
                        }
                        else
                        {
                            if(nMinTime > pSocket_State_Info->m_nMinRecvTime)
                            {
                                nMinTime = pSocket_State_Info->m_nMinRecvTime;
                            }

                            if(nMaxTime < pSocket_State_Info->m_nMaxRecvTime)
                            {
                                nMaxTime = pSocket_State_Info->m_nMaxRecvTime;
                            }
                        }
                    }
                }
            }

            //显示相关数据

            this->TextCtrl_SuccessConnect->SetValue( wxString::Format(wxT("%i"),nSuccessConnect));
            this->TextCtrl_SuccessSend->SetValue( wxString::Format(wxT("%i"),nSuccessSend));
            this->TextCtrl_SuccessRecv->SetValue( wxString::Format(wxT("%i"),nSuccessRecv));
            this->TextCtrl_CurrConnect->SetValue( wxString::Format(wxT("%i"),ntCurrConnect));
            this->TextCtrl_FailConnect->SetValue( wxString::Format(wxT("%i"),nFailConnect));
            this->TextCtrl_FailRecv->SetValue( wxString::Format(wxT("%i"),nFailRecv));
            this->TextCtrl_FailSend->SetValue( wxString::Format(wxT("%i"),nFailSend));
            this->TextCtrl_SendByteCount->SetValue( wxString::Format(wxT("%i"),nSendByteCount));
            this->TextCtrl_RecvByteCount->SetValue( wxString::Format(wxT("%i"),nRecvByteCount));

            this->TextCtrl_MinTime->SetValue( wxString::Format(wxT("%i"),nMinTime));
            this->TextCtrl_MaxTime->SetValue( wxString::Format(wxT("%i"),nMaxTime));

        }
        else
        {
            for(int i = 0; i < (int)m_vecClientUdpSocket.size(); i++)
            {
                CClientUdpSocket* pClientUdpSocket = (CClientUdpSocket* )m_vecClientUdpSocket[i];

                if(NULL != pClientUdpSocket)
                {
                    _Socket_State_Info* pSocket_State_Info = pClientUdpSocket->GetStateInfo();

                    if(NULL != pSocket_State_Info)
                    {
                        nSuccessConnect += pSocket_State_Info->m_nSuccessConnect;
                        nSuccessSend    += pSocket_State_Info->m_nSuccessSend;
                        nSuccessRecv    += pSocket_State_Info->m_nSuccessRecv;
                        nFailConnect    += pSocket_State_Info->m_nFailConnect;
                        nFailSend       += pSocket_State_Info->m_nFailSend;
                        nFailRecv       += pSocket_State_Info->m_nFailRecv;
                        nCurrConnect    += pSocket_State_Info->m_nCurrectSocket;
                    }

                    if(i == 0)
                    {
                        nMinTime = pSocket_State_Info->m_nMinRecvTime;
                        nMaxTime = pSocket_State_Info->m_nMaxRecvTime;
                    }
                    else
                    {
                        if(nMinTime > pSocket_State_Info->m_nMinRecvTime)
                        {
                            nMinTime = pSocket_State_Info->m_nMinRecvTime;
                        }

                        if(nMaxTime < pSocket_State_Info->m_nMaxRecvTime)
                        {
                            nMaxTime = pSocket_State_Info->m_nMaxRecvTime;
                        }
                    }
                }
            }

            //显示相关数据

            this->TextCtrl_SuccessConnect->SetValue( wxString::Format(wxT("%i"),nSuccessConnect));
            this->TextCtrl_SuccessSend->SetValue( wxString::Format(wxT("%i"),nSuccessSend));
            this->TextCtrl_SuccessRecv->SetValue( wxString::Format(wxT("%i"),nSuccessRecv));
            this->TextCtrl_CurrConnect->SetValue( wxString::Format(wxT("%i"),ntCurrConnect));
            this->TextCtrl_FailConnect->SetValue( wxString::Format(wxT("%i"),nFailConnect));
            this->TextCtrl_FailRecv->SetValue( wxString::Format(wxT("%i"),nFailRecv));
            this->TextCtrl_FailSend->SetValue( wxString::Format(wxT("%i"),nFailSend));

            this->TextCtrl_MinTime->SetValue( wxString::Format(wxT("%i"),nMinTime));
            this->TextCtrl_MaxTime->SetValue( wxString::Format(wxT("%i"),nMaxTime));


        }
    }

    //event.Skip();



}

void CPassTCPDlg::OnChoice_SendBuffStyleSelect(wxCommandEvent& event)
{
    //文本编制模式，根据用户的选项，变化文本的内容

    int nCurrTextStyle = this->Choice_SendBuffStyle->GetSelection();

    if(nCurrTextStyle != m_nCurrTextStyle)
    {
        if(nCurrTextStyle == 0)  //讲文本转化为二进制
        {
            //当风格发生改变的时候变化

            std::string  m_reSendText =std::string(this->TextCtrl_SendText->GetValue());
            ///m_txtPacketData.GetWindowText(strData);
            int nSrcLen = m_reSendText.length();
            int nBufferSize = nSrcLen;
            int nDecLen = nSrcLen ;

            char* pSendData = new char[nBufferSize + 1];
            strncpy(pSendData, (const char*)m_reSendText.c_str(), nBufferSize);
            pSendData[nBufferSize] = '\0';


            if(nDecLen <= 0)
            {
                //如果没有内容就不转换
                m_nCurrTextStyle = nCurrTextStyle;
                //SetRichTextColor(COLOR_TEXT_BULE);
                return;
            }

            //转化后的字符串
            int nConvertSize = (nBufferSize + 1)*4;
            char* pErSendBuff = new char[nConvertSize];
            memset(pErSendBuff, 0, nConvertSize);

            for(int i = 0; i < nDecLen; i++)
            {
                char szLog[4] = {'\0'};

                if( i != nDecLen - 1)
                {
                    sprintf(szLog,  "%02X ", (unsigned char)pSendData[i]);
                }
                else
                {
                    sprintf(szLog,  "%02X", (unsigned char)pSendData[i]);
                }

                if(i == 0)
                {
                    sprintf(pErSendBuff,  "%s%s", pErSendBuff, szLog);
                }
                else
                {
                    sprintf(pErSendBuff,  "%s %s", pErSendBuff, szLog);
                }
            }

            //将转换后的二进制，显示在文本框内
            //          wchar_t *pwText = new wchar_t[nConvertSize];
            //
            //          nSrcLen = MultiByteToWideChar (CP_ACP, 0, pErSendBuff, -1, NULL, 0);
            //          nBufferSize = MultiByteToWideChar (CP_ACP, 0, pErSendBuff, -1, pwText, nSrcLen);
            //          pwText[nBufferSize] = '\0';
            //
            //          m_reSendText.SetWindowText(pwText);
            //

            this->TextCtrl_SendText->SetValue(wxString(pErSendBuff ));



            delete[] pSendData;
            delete[] pErSendBuff;

            //SetRichTextColor(COLOR_TEXT_BULE);
        }
        else
        {
            //将二进制转化成文本

            std::string  m_reSendText =std::string(this->TextCtrl_SendText->GetValue());
            ///m_txtPacketData.GetWindowText(strData);
            int nSrcLen = m_reSendText.length();
            int nBufferSize = nSrcLen;

            char* pSendData = new char[nBufferSize + 1];
            strncpy(pSendData, (const char*)m_reSendText.c_str(), nBufferSize);
            pSendData[nBufferSize] = '\0';

            int nDecLen = nSrcLen ;

            if(nDecLen <= 0)
            {
                //如果没有内容就不转换
                m_nCurrTextStyle = nCurrTextStyle;
                //SetRichTextColor(COLOR_TEXT_RED);
                return;
            }

            int nTextSize = 0;

            if(nDecLen % 3 != 0)
            {
                nTextSize = nDecLen / 3 + 2;
            }
            else
            {
                nTextSize = nDecLen / 3 + 1;
            }

            char* pTextData = new char[nTextSize];
            memset(pTextData, 0, nTextSize);

            CConvertBuffer objConvertBuffer;
            //将数据串转换成二进制串
            objConvertBuffer.Convertstr2charArray(pSendData, strlen(pSendData), (unsigned char*)pTextData, nTextSize);

            //将转换后的二进制，显示在文本框内

            this->TextCtrl_SendText->SetValue(wxString(pTextData ));


            delete[] pSendData;
            delete[] pTextData;

            //SetRichTextColor(COLOR_TEXT_RED);
        }

        m_nCurrTextStyle = nCurrTextStyle;

    }


}

void CPassTCPDlg::OnButton1Click(wxCommandEvent& event)
{
}

void CPassTCPDlg::OnTextCtrl_ServerIPText(wxCommandEvent& event)
{
}
