/***************************************************************
 * Name:      PSS_ClientManagerMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    smith ()
 * Created:   2019-02-21
 * Copyright: smith ()
 * License:
 **************************************************************/

#include "PSS_ClientManagerMain.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(PSS_ClientManagerFrame)
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

//(*IdInit(PSS_ClientManagerFrame)
const long PSS_ClientManagerFrame::ID_STATICBOX1 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICBOX2 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT1 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT2 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL1 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL2 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT3 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL3 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON1 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON2 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON3 = wxNewId();
const long PSS_ClientManagerFrame::ID_LISTBOX1 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON4 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON5 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT4 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL4 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT5 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL5 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT6 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL6 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT7 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL7 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON6 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON7 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON8 = wxNewId();
const long PSS_ClientManagerFrame::ID_LISTCTRL1 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL38 = wxNewId();
const long PSS_ClientManagerFrame::ID_PANEL1 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICBOX3 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICBOX4 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON9 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT8 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL8 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT9 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL9 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT10 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL10 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON10 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT11 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL11 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON11 = wxNewId();
const long PSS_ClientManagerFrame::ID_RADIOBOX1 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT12 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL12 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON12 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON13 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT13 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL13 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT14 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL14 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON14 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT15 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL15 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON15 = wxNewId();
const long PSS_ClientManagerFrame::ID_LISTCTRL2 = wxNewId();
const long PSS_ClientManagerFrame::ID_PANEL2 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICBOX5 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT16 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL16 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON16 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON17 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT17 = wxNewId();
const long PSS_ClientManagerFrame::ID_LISTCTRL3 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON18 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON19 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON20 = wxNewId();
const long PSS_ClientManagerFrame::ID_LISTCTRL4 = wxNewId();
const long PSS_ClientManagerFrame::ID_PANEL3 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICBOX6 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT18 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL17 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON21 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON22 = wxNewId();
const long PSS_ClientManagerFrame::ID_LISTCTRL5 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT19 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL18 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON23 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON24 = wxNewId();
const long PSS_ClientManagerFrame::ID_LISTCTRL6 = wxNewId();
const long PSS_ClientManagerFrame::ID_PANEL4 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICBOX7 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON25 = wxNewId();
const long PSS_ClientManagerFrame::ID_LISTCTRL7 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICBOX8 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT20 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL19 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT21 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL20 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT22 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON26 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON27 = wxNewId();
const long PSS_ClientManagerFrame::ID_CHECKBOX1 = wxNewId();
const long PSS_ClientManagerFrame::ID_LISTCTRL8 = wxNewId();
const long PSS_ClientManagerFrame::ID_RADIOBOX2 = wxNewId();
const long PSS_ClientManagerFrame::ID_LISTCTRL9 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON28 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON29 = wxNewId();
const long PSS_ClientManagerFrame::ID_PANEL5 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICBOX9 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON30 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT23 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL21 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON31 = wxNewId();
const long PSS_ClientManagerFrame::ID_LISTCTRL10 = wxNewId();
const long PSS_ClientManagerFrame::ID_LISTCTRL11 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON32 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICBOX10 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT24 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT25 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT26 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL22 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL23 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT27 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL24 = wxNewId();
const long PSS_ClientManagerFrame::ID_COMBOBOX1 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON33 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON34 = wxNewId();
const long PSS_ClientManagerFrame::ID_PANEL6 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICBOX11 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT28 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL25 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT29 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL26 = wxNewId();
const long PSS_ClientManagerFrame::ID_RADIOBOX3 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON35 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON36 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON37 = wxNewId();
const long PSS_ClientManagerFrame::ID_LISTCTRL12 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT30 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT31 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL27 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL28 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON38 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON39 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON40 = wxNewId();
const long PSS_ClientManagerFrame::ID_LISTCTRL13 = wxNewId();
const long PSS_ClientManagerFrame::ID_PANEL7 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICBOX12 = wxNewId();
const long PSS_ClientManagerFrame::ID_LISTCTRL14 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT32 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT33 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL29 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON41 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL30 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL31 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL32 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON42 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON43 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON44 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT35 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT36 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL33 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT37 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL34 = wxNewId();
const long PSS_ClientManagerFrame::ID_PANEL8 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICBOX13 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICBOX14 = wxNewId();
const long PSS_ClientManagerFrame::ID_LISTCTRL15 = wxNewId();
const long PSS_ClientManagerFrame::ID_CHECKBOX2 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT34 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL35 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT38 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT39 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL36 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT40 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT41 = wxNewId();
const long PSS_ClientManagerFrame::ID_TEXTCTRL37 = wxNewId();
const long PSS_ClientManagerFrame::ID_STATICTEXT42 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON45 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON46 = wxNewId();
const long PSS_ClientManagerFrame::ID_LISTCTRL16 = wxNewId();
const long PSS_ClientManagerFrame::ID_LISTCTRL17 = wxNewId();
const long PSS_ClientManagerFrame::ID_BUTTON47 = wxNewId();
const long PSS_ClientManagerFrame::ID_PANEL9 = wxNewId();
const long PSS_ClientManagerFrame::ID_NOTEBOOK1 = wxNewId();
const long PSS_ClientManagerFrame::ID_TIMER1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(PSS_ClientManagerFrame,wxFrame)
    //(*EventTable(PSS_ClientManagerFrame)
    //*)
END_EVENT_TABLE()

PSS_ClientManagerFrame::PSS_ClientManagerFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(PSS_ClientManagerFrame)
    Create(parent, id, wxT("PSS_ClientManager"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxMINIMIZE_BOX, _T("id"));
    SetClientSize(wxSize(1003,506));
    {
        wxIcon FrameIcon;
        FrameIcon.CopyFromBitmap(wxBitmap(wxImage(_T("../../../Icon.jpg"))));
        SetIcon(FrameIcon);
    }
    Notebook1 = new wxNotebook(this, ID_NOTEBOOK1, wxPoint(0,-16), wxSize(1240,506), 0, _T("ID_NOTEBOOK1"));
    Panel1 = new wxPanel(Notebook1, ID_PANEL1, wxDefaultPosition, wxSize(899,461), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    StaticBox1 = new wxStaticBox(Panel1, ID_STATICBOX1, wxT("服务器管理接口信息"), wxPoint(8,8), wxSize(832,224), 0, _T("ID_STATICBOX1"));
    StaticBox2 = new wxStaticBox(Panel1, ID_STATICBOX2, wxT("模块信息"), wxPoint(8,232), wxSize(832,224), 0, _T("ID_STATICBOX2"));
    StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT1, wxT("服务器IP"), wxPoint(16,40), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    StaticText2 = new wxStaticText(Panel1, ID_STATICTEXT2, wxT("服务器密钥"), wxPoint(16,80), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    TextCtrl_ServerIP = new wxTextCtrl(Panel1, ID_TEXTCTRL1, wxT("127.0.0.1"), wxPoint(88,32), wxSize(114,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    TextCtrl_ServerIP->SetMaxLength(16);
    TextCtrl_ServerKey = new wxTextCtrl(Panel1, ID_TEXTCTRL2, wxT("freeeyes"), wxPoint(88,72), wxSize(114,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
    StaticText3 = new wxStaticText(Panel1, ID_STATICTEXT3, wxT("服务器端口"), wxPoint(216,40), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    TextCtrl_ServerPort = new wxTextCtrl(Panel1, ID_TEXTCTRL3, wxT("10010"), wxPoint(296,32), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
    TextCtrl_ServerPort->SetMaxLength(5);
    Button_Set = new wxButton(Panel1, ID_BUTTON1, wxT("设置"), wxPoint(400,32), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    Button_GetServerstate = new wxButton(Panel1, ID_BUTTON2, wxT("获得服务器状态"), wxPoint(496,32), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    Button_CloseServer = new wxButton(Panel1, ID_BUTTON3, wxT("关闭服务器(远程服务器必须允许关闭才能生效)"), wxPoint(216,72), wxSize(392,35), 0, wxDefaultValidator, _T("ID_BUTTON3"));
    ListBox_ConfigList = new wxListBox(Panel1, ID_LISTBOX1, wxPoint(616,72), wxSize(136,152), 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX1"));
    ListBox_ConfigList->SetSelection( ListBox_ConfigList->Append(wxT("Windows PSS")) );
    ListBox_ConfigList->Append(wxT("Linux PSS"));
    Button_LoadConfigfile = new wxButton(Panel1, ID_BUTTON4, wxT("从配置文件加载"), wxPoint(616,32), wxSize(136,34), 0, wxDefaultValidator, _T("ID_BUTTON4"));
    Button_ShowAllModule = new wxButton(Panel1, ID_BUTTON5, wxT("显示所有模块"), wxPoint(24,264), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON5"));
    StaticText4 = new wxStaticText(Panel1, ID_STATICTEXT4, wxT("模块路径"), wxPoint(136,272), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    TextCtrl_ModulePath = new wxTextCtrl(Panel1, ID_TEXTCTRL4, wxEmptyString, wxPoint(200,264), wxSize(160,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
    StaticText5 = new wxStaticText(Panel1, ID_STATICTEXT5, wxT("文件名称"), wxPoint(368,272), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    TextCtrl_ModuleFilename = new wxTextCtrl(Panel1, ID_TEXTCTRL5, wxEmptyString, wxPoint(432,264), wxSize(210,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL5"));
    StaticText6 = new wxStaticText(Panel1, ID_STATICTEXT6, wxT("模块名称"), wxPoint(24,312), wxDefaultSize, 0, _T("ID_STATICTEXT6"));
    TextCtrl_ModuleName = new wxTextCtrl(Panel1, ID_TEXTCTRL6, wxEmptyString, wxPoint(88,304), wxSize(168,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL6"));
    StaticText7 = new wxStaticText(Panel1, ID_STATICTEXT7, wxT("模块参数"), wxPoint(264,312), wxDefaultSize, 0, _T("ID_STATICTEXT7"));
    TextCtrl_ModuleParam = new wxTextCtrl(Panel1, ID_TEXTCTRL7, wxEmptyString, wxPoint(328,304), wxSize(218,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL7"));
    Button_LoadModule = new wxButton(Panel1, ID_BUTTON6, wxT("加载"), wxPoint(664,264), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON6"));
    Button_UnloadModule = new wxButton(Panel1, ID_BUTTON7, wxT("卸载"), wxPoint(560,304), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON7"));
    Button_ReloadModule = new wxButton(Panel1, ID_BUTTON8, wxT("重载"), wxPoint(664,304), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON8"));
    ListCtrl_ModuleList = new wxListCtrl(Panel1, ID_LISTCTRL1, wxPoint(16,344), wxSize(736,112), wxLC_REPORT|wxLC_SINGLE_SEL, wxDefaultValidator, _T("ID_LISTCTRL1"));
    TextCtrl_ServerInfo = new wxTextCtrl(Panel1, ID_TEXTCTRL38, wxEmptyString, wxPoint(16,112), wxSize(592,112), wxTE_MULTILINE|wxTE_RICH|wxTE_RICH2, wxDefaultValidator, _T("ID_TEXTCTRL38"));
    Panel2 = new wxPanel(Notebook1, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
    StaticBox3 = new wxStaticBox(Panel2, ID_STATICBOX3, wxT("链接信息历史记录"), wxPoint(16,248), wxSize(808,200), 0, _T("ID_STATICBOX3"));
    StaticBox4 = new wxStaticBox(Panel2, ID_STATICBOX4, wxT("链接信息"), wxPoint(16,8), wxSize(808,240), 0, _T("ID_STATICBOX4"));
    Button_GetCurrClientCount = new wxButton(Panel2, ID_BUTTON9, wxT("获得当前客户端链接个数"), wxPoint(32,32), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON9"));
    StaticText8 = new wxStaticText(Panel2, ID_STATICTEXT8, wxT("当前链接数量"), wxPoint(216,40), wxDefaultSize, 0, _T("ID_STATICTEXT8"));
    TextCtrl_CurrClientCount = new wxTextCtrl(Panel2, ID_TEXTCTRL8, wxEmptyString, wxPoint(304,32), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL8"));
    TextCtrl_CurrClientCount->Disable();
    StaticText9 = new wxStaticText(Panel2, ID_STATICTEXT9, wxT("当前流量(入)"), wxPoint(416,40), wxDefaultSize, 0, _T("ID_STATICTEXT9"));
    TextCtrl_CurrFlowIn = new wxTextCtrl(Panel2, ID_TEXTCTRL9, wxEmptyString, wxPoint(504,32), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL9"));
    TextCtrl_CurrFlowIn->Disable();
    StaticText10 = new wxStaticText(Panel2, ID_STATICTEXT10, wxT("当前流量(出)"), wxPoint(616,40), wxDefaultSize, 0, _T("ID_STATICTEXT10"));
    TextCtrl_CurrFlowOut = new wxTextCtrl(Panel2, ID_TEXTCTRL10, wxEmptyString, wxPoint(704,32), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL10"));
    TextCtrl_CurrFlowOut->Disable();
    Button_SetLinkLimit = new wxButton(Panel2, ID_BUTTON10, wxT("重新设置上限"), wxPoint(32,72), wxSize(168,34), 0, wxDefaultValidator, _T("ID_BUTTON10"));
    StaticText11 = new wxStaticText(Panel2, ID_STATICTEXT11, wxT("链接总数上限"), wxPoint(216,80), wxDefaultSize, 0, _T("ID_STATICTEXT11"));
    TextCtrl_LinkUpperLimit = new wxTextCtrl(Panel2, ID_TEXTCTRL11, wxEmptyString, wxPoint(304,72), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL11"));
    Button_SetDebug = new wxButton(Panel2, ID_BUTTON11, wxT("设置DEBUG"), wxPoint(32,112), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON11"));
    wxString __wxRadioBoxChoices_1[2] =
    {
        wxT("DEBUG开启"),
        wxT("DEBUG关闭")
    };
    RadioBox_DebugState = new wxRadioBox(Panel2, ID_RADIOBOX1, wxEmptyString, wxPoint(144,96), wxSize(224,56), 2, __wxRadioBoxChoices_1, 1, wxBORDER_NONE, wxDefaultValidator, _T("ID_RADIOBOX1"));
    RadioBox_DebugState->SetSelection(0);
    StaticText12 = new wxStaticText(Panel2, ID_STATICTEXT12, wxT("获得DEBUG状态"), wxPoint(504,120), wxDefaultSize, 0, _T("ID_STATICTEXT12"));
    TextCtrl_CurrDebugStat = new wxTextCtrl(Panel2, ID_TEXTCTRL12, wxEmptyString, wxPoint(616,112), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL12"));
    TextCtrl_CurrDebugStat->Disable();
    Button_GetDebugState = new wxButton(Panel2, ID_BUTTON12, wxT("获取DEBUG"), wxPoint(400,112), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON12"));
    Button_StartFile = new wxButton(Panel2, ID_BUTTON13, wxT("启动文件"), wxPoint(32,160), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON13"));
    StaticText13 = new wxStaticText(Panel2, ID_STATICTEXT13, wxT("当前CPU消耗"), wxPoint(40,208), wxDefaultSize, 0, _T("ID_STATICTEXT13"));
    TextCtrl_CurrCpuOccupy = new wxTextCtrl(Panel2, ID_TEXTCTRL13, wxEmptyString, wxPoint(136,200), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL13"));
    TextCtrl_CurrCpuOccupy->Disable();
    StaticText14 = new wxStaticText(Panel2, ID_STATICTEXT14, wxT("当前内存消耗"), wxPoint(256,208), wxDefaultSize, 0, _T("ID_STATICTEXT14"));
    TextCtrl_CurrMemOccupy = new wxTextCtrl(Panel2, ID_TEXTCTRL14, wxEmptyString, wxPoint(360,200), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL14"));
    TextCtrl_CurrMemOccupy->Disable();
    Button_StopFile = new wxButton(Panel2, ID_BUTTON14, wxT("停止文件"), wxPoint(136,160), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON14"));
    StaticText15 = new wxStaticText(Panel2, ID_STATICTEXT15, wxT("数据包文件路径"), wxPoint(256,168), wxDefaultSize, 0, _T("ID_STATICTEXT15"));
    TextCtrl_PacketFile = new wxTextCtrl(Panel2, ID_TEXTCTRL15, wxT("FileTestCfg.xml"), wxPoint(360,160), wxSize(440,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL15"));
    Button_GetLinkHistory = new wxButton(Panel2, ID_BUTTON15, wxT("获得当前链接历史"), wxPoint(24,280), wxSize(224,32), 0, wxDefaultValidator, _T("ID_BUTTON15"));
    ListCtrl_LinkHistory = new wxListCtrl(Panel2, ID_LISTCTRL2, wxPoint(24,320), wxSize(776,120), wxLC_REPORT, wxDefaultValidator, _T("ID_LISTCTRL2"));
    Panel3 = new wxPanel(Notebook1, ID_PANEL3, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL3"));
    StaticBox5 = new wxStaticBox(Panel3, ID_STATICBOX5, wxT("模块调用信息"), wxPoint(8,8), wxSize(824,448), 0, _T("ID_STATICBOX5"));
    StaticText16 = new wxStaticText(Panel3, ID_STATICTEXT16, wxT("命令ID"), wxPoint(24,40), wxDefaultSize, 0, _T("ID_STATICTEXT16"));
    TextCtrl_ModuleID = new wxTextCtrl(Panel3, ID_TEXTCTRL16, wxEmptyString, wxPoint(88,32), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL16"));
    Button_ShowModuleCmd = new wxButton(Panel3, ID_BUTTON16, wxT("查看命令"), wxPoint(232,32), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON16"));
    Button_ShowAllCmd = new wxButton(Panel3, ID_BUTTON17, wxT("查看全部"), wxPoint(360,32), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON17"));
    Button_ShowAllCmd->Disable();
    StaticText17 = new wxStaticText(Panel3, ID_STATICTEXT17, wxT("命令ID格式为十六进制，比如0X1000"), wxPoint(456,40), wxDefaultSize, 0, _T("ID_STATICTEXT17"));
    ListCtrl_ModuleInfo = new wxListCtrl(Panel3, ID_LISTCTRL3, wxPoint(24,80), wxSize(792,168), wxLC_REPORT, wxDefaultValidator, _T("ID_LISTCTRL3"));
    Button_ShowTimeoutCmd = new wxButton(Panel3, ID_BUTTON18, wxT("查看超时命令"), wxPoint(40,256), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON18"));
    Button_ClearTimeoutCmd = new wxButton(Panel3, ID_BUTTON19, wxT("清除超时命令"), wxPoint(176,256), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON19"));
    Button_SaveSmdHistory = new wxButton(Panel3, ID_BUTTON20, wxT("保存命令日志"), wxPoint(312,256), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON20"));
    ListCtrl_CommandTimeout = new wxListCtrl(Panel3, ID_LISTCTRL4, wxPoint(24,304), wxSize(792,144), wxLC_REPORT, wxDefaultValidator, _T("ID_LISTCTRL4"));
    Panel4 = new wxPanel(Notebook1, ID_PANEL4, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL4"));
    StaticBox6 = new wxStaticBox(Panel4, ID_STATICBOX6, wxT("链接管理"), wxPoint(8,8), wxSize(824,448), 0, _T("ID_STATICBOX6"));
    StaticText18 = new wxStaticText(Panel4, ID_STATICTEXT18, wxT("要停止的客户端链接ID"), wxPoint(24,32), wxDefaultSize, 0, _T("ID_STATICTEXT18"));
    TextCtrl_ClientID = new wxTextCtrl(Panel4, ID_TEXTCTRL17, wxEmptyString, wxPoint(168,24), wxSize(120,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL17"));
    Button_DisconnectClient = new wxButton(Panel4, ID_BUTTON21, wxT("断开链接"), wxPoint(304,24), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON21"));
    Button_ShowClient = new wxButton(Panel4, ID_BUTTON22, wxT("查看TCP"), wxPoint(408,24), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON22"));
    ListCtrl_ClientTcpInfo = new wxListCtrl(Panel4, ID_LISTCTRL5, wxPoint(24,64), wxSize(792,192), wxLC_REPORT, wxDefaultValidator, _T("ID_LISTCTRL5"));
    StaticText19 = new wxStaticText(Panel4, ID_STATICTEXT19, wxT("当前日志等级"), wxPoint(16,272), wxDefaultSize, 0, _T("ID_STATICTEXT19"));
    TextCtrl_LogLevel = new wxTextCtrl(Panel4, ID_TEXTCTRL18, wxT("1"), wxPoint(112,264), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL18"));
    Button_SetLogLevel = new wxButton(Panel4, ID_BUTTON23, wxT("设置日志等级"), wxPoint(232,264), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON23"));
    Button_ShowCurrLoginfo = new wxButton(Panel4, ID_BUTTON24, wxT("查看当前日志项"), wxPoint(352,264), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON24"));
    ListCtrl_LogInfo = new wxListCtrl(Panel4, ID_LISTCTRL6, wxPoint(24,312), wxSize(792,136), wxLC_REPORT, wxDefaultValidator, _T("ID_LISTCTRL6"));
    Panel5 = new wxPanel(Notebook1, ID_PANEL5, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL5"));
    StaticBox7 = new wxStaticBox(Panel5, ID_STATICBOX7, wxT("工作线程状态"), wxPoint(8,8), wxSize(832,448), 0, _T("ID_STATICBOX7"));
    Button_ShowWorkThreadStat = new wxButton(Panel5, ID_BUTTON25, wxT("查看工作线程状态"), wxPoint(24,32), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON25"));
    ListCtrl_WorkThreadInfo = new wxListCtrl(Panel5, ID_LISTCTRL7, wxPoint(24,72), wxSize(808,160), wxLC_REPORT, wxDefaultValidator, _T("ID_LISTCTRL7"));
    StaticBox8 = new wxStaticBox(Panel5, ID_STATICBOX8, wxT("设置监控参数"), wxPoint(16,240), wxSize(816,216), 0, _T("ID_STATICBOX8"));
    StaticText20 = new wxStaticText(Panel5, ID_STATICTEXT20, wxT("当前工作线程个数"), wxPoint(20,264), wxDefaultSize, 0, _T("ID_STATICTEXT20"));
    TextCtrl_ThreadCount = new wxTextCtrl(Panel5, ID_TEXTCTRL19, wxT("3"), wxPoint(128,256), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL19"));
    StaticText21 = new wxStaticText(Panel5, ID_STATICTEXT21, wxT("检查间隔"), wxPoint(240,264), wxDefaultSize, 0, _T("ID_STATICTEXT21"));
    TextCtrl_CheckTime = new wxTextCtrl(Panel5, ID_TEXTCTRL20, wxT("60"), wxPoint(304,256), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL20"));
    StaticText22 = new wxStaticText(Panel5, ID_STATICTEXT22, wxT("秒"), wxPoint(408,264), wxDefaultSize, 0, _T("ID_STATICTEXT22"));
    Button_StartMonitor = new wxButton(Panel5, ID_BUTTON26, wxT("开始监控"), wxPoint(480,256), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON26"));
    Button_StopMonitor = new wxButton(Panel5, ID_BUTTON27, wxT("停止监控"), wxPoint(584,256), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON27"));
    CheckBox_Voice = new wxCheckBox(Panel5, ID_CHECKBOX1, wxT("开启声音报警"), wxPoint(680,260), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
    CheckBox_Voice->SetValue(false);
    ListCtrl_CheckLog = new wxListCtrl(Panel5, ID_LISTCTRL8, wxPoint(24,296), wxSize(448,152), wxLC_REPORT, wxDefaultValidator, _T("ID_LISTCTRL8"));
    wxString __wxRadioBoxChoices_2[2] =
    {
        wxT("监控当前服务器"),
        wxT("监控列表服务器")
    };
    RadioBox_MonitorServer = new wxRadioBox(Panel5, ID_RADIOBOX2, wxEmptyString, wxPoint(480,280), wxSize(336,48), 2, __wxRadioBoxChoices_2, 1, 0, wxDefaultValidator, _T("ID_RADIOBOX2"));
    RadioBox_MonitorServer->SetSelection(0);
    ListCtrl_ServerList = new wxListCtrl(Panel5, ID_LISTCTRL9, wxPoint(480,336), wxSize(344,72), wxLC_REPORT, wxDefaultValidator, _T("ID_LISTCTRL9"));
    Button_LoadFile = new wxButton(Panel5, ID_BUTTON28, wxT("从文件中加载"), wxPoint(480,416), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON28"));
    Button_ClearLoad = new wxButton(Panel5, ID_BUTTON29, wxT("清除加载"), wxPoint(736,416), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON29"));
    Panel6 = new wxPanel(Notebook1, ID_PANEL6, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL6"));
    StaticBox9 = new wxStaticBox(Panel6, ID_STATICBOX9, wxT("服务器间链接管理"), wxPoint(0,8), wxSize(856,440), 0, _T("ID_STATICBOX9"));
    Button_ShowServerLinkInfo = new wxButton(Panel6, ID_BUTTON30, wxT("查看服务器间链接信息"), wxPoint(16,32), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON30"));
    StaticText23 = new wxStaticText(Panel6, ID_STATICTEXT23, wxT("服务器ID"), wxPoint(16,76), wxDefaultSize, 0, _T("ID_STATICTEXT23"));
    TextCtrl_ServerID = new wxTextCtrl(Panel6, ID_TEXTCTRL21, wxEmptyString, wxPoint(72,72), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL21"));
    Button_ReconnectServer = new wxButton(Panel6, ID_BUTTON31, wxT("重新连接"), wxPoint(176,72), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON31"));
    ListCtrl_ServerConnect = new wxListCtrl(Panel6, ID_LISTCTRL10, wxPoint(16,112), wxSize(824,136), wxLC_REPORT, wxDefaultValidator, _T("ID_LISTCTRL10"));
    ListCtrl_ServerLisen = new wxListCtrl(Panel6, ID_LISTCTRL11, wxPoint(16,296), wxSize(328,136), wxLC_REPORT, wxDefaultValidator, _T("ID_LISTCTRL11"));
    Button_ShowPSSLisenInfo = new wxButton(Panel6, ID_BUTTON32, wxT("查看PSS监听信息"), wxPoint(8,256), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON32"));
    StaticBox10 = new wxStaticBox(Panel6, ID_STATICBOX10, wxT("动态添加和关闭监听端口"), wxPoint(368,256), wxSize(472,184), 0, _T("ID_STATICBOX10"));
    StaticText24 = new wxStaticText(Panel6, ID_STATICTEXT24, wxT("监听IP"), wxPoint(392,288), wxDefaultSize, 0, _T("ID_STATICTEXT24"));
    StaticText25 = new wxStaticText(Panel6, ID_STATICTEXT25, wxT("监听端口"), wxPoint(392,328), wxDefaultSize, 0, _T("ID_STATICTEXT25"));
    StaticText26 = new wxStaticText(Panel6, ID_STATICTEXT26, wxT("IP类型"), wxPoint(392,368), wxDefaultSize, 0, _T("ID_STATICTEXT26"));
    TextCtrl_LisenIP = new wxTextCtrl(Panel6, ID_TEXTCTRL22, wxEmptyString, wxPoint(456,280), wxSize(136,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL22"));
    TextCtrl_LisenIP->SetMaxLength(16);
    TextCtrl_LisenPort = new wxTextCtrl(Panel6, ID_TEXTCTRL23, wxT("0"), wxPoint(456,320), wxSize(136,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL23"));
    TextCtrl_LisenPort->SetMaxLength(5);
    StaticText27 = new wxStaticText(Panel6, ID_STATICTEXT27, wxT("解析模块 ID"), wxPoint(600,328), wxDefaultSize, 0, _T("ID_STATICTEXT27"));
    TextCtrl_PacketParseID = new wxTextCtrl(Panel6, ID_TEXTCTRL24, wxEmptyString, wxPoint(688,320), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL24"));
    ComboBox_ListenType = new wxComboBox(Panel6, ID_COMBOBOX1, wxEmptyString, wxPoint(456,360), wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("ID_COMBOBOX1"));
    ComboBox_ListenType->SetSelection( ComboBox_ListenType->Append(wxT("IPv4")) );
    ComboBox_ListenType->Append(wxT("IPv6"));
    Button_InsertNewMonitor = new wxButton(Panel6, ID_BUTTON33, wxT("添加新监听"), wxPoint(392,400), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON33"));
    Button_DeleteOldMonitor = new wxButton(Panel6, ID_BUTTON34, wxT("停止已有监听"), wxPoint(560,400), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON34"));
    Panel7 = new wxPanel(Notebook1, ID_PANEL7, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL7"));
    StaticBox11 = new wxStaticBox(Panel7, ID_STATICBOX11, wxT("IP封禁管理"), wxPoint(8,8), wxSize(792,440), 0, _T("ID_STATICBOX11"));
    StaticText28 = new wxStaticText(Panel7, ID_STATICTEXT28, wxT("要封禁的IP地址(支持*)"), wxPoint(24,40), wxDefaultSize, 0, _T("ID_STATICTEXT28"));
    TextCtrl_ForbidenIP = new wxTextCtrl(Panel7, ID_TEXTCTRL25, wxEmptyString, wxPoint(160,32), wxSize(128,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL25"));
    TextCtrl_ForbidenIP->SetMaxLength(16);
    StaticText29 = new wxStaticText(Panel7, ID_STATICTEXT29, wxT("要封禁的秒数"), wxPoint(312,40), wxDefaultSize, 0, _T("ID_STATICTEXT29"));
    TextCtrl_ForbidenSeconds = new wxTextCtrl(Panel7, ID_TEXTCTRL26, wxEmptyString, wxPoint(392,32), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL26"));
    wxString __wxRadioBoxChoices_3[2] =
    {
        wxT("时段封禁"),
        wxT("永久封禁")
    };
    RadioBoxm_TimeForbiden = new wxRadioBox(Panel7, ID_RADIOBOX3, wxEmptyString, wxPoint(16,48), wxSize(208,40), 2, __wxRadioBoxChoices_3, 1, wxBORDER_NONE, wxDefaultValidator, _T("ID_RADIOBOX3"));
    RadioBoxm_TimeForbiden->SetSelection(0);
    Button_InsertForbiden = new wxButton(Panel7, ID_BUTTON35, wxT("添加封禁"), wxPoint(24,96), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON35"));
    Button_ReleaseForbiden = new wxButton(Panel7, ID_BUTTON36, wxT("解除封禁"), wxPoint(144,96), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON36"));
    Button_ShowForbiden = new wxButton(Panel7, ID_BUTTON37, wxT("查看列表"), wxPoint(272,96), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON37"));
    ListCtrl_ForbidenList = new wxListCtrl(Panel7, ID_LISTCTRL12, wxPoint(24,136), wxSize(768,128), wxLC_REPORT, wxDefaultValidator, _T("ID_LISTCTRL12"));
    StaticText30 = new wxStaticText(Panel7, ID_STATICTEXT30, wxT("要查看的连接别名"), wxPoint(16,280), wxDefaultSize, 0, _T("ID_STATICTEXT30"));
    StaticText31 = new wxStaticText(Panel7, ID_STATICTEXT31, wxT("ConnectID"), wxPoint(16,312), wxDefaultSize, 0, _T("ID_STATICTEXT31"));
    TextCtrl_NickName = new wxTextCtrl(Panel7, ID_TEXTCTRL27, wxEmptyString, wxPoint(128,272), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL27"));
    TextCtrl_ConnectID = new wxTextCtrl(Panel7, ID_TEXTCTRL28, wxEmptyString, wxPoint(128,312), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL28"));
    Button_SearchNickname = new wxButton(Panel7, ID_BUTTON38, wxT("查询别名"), wxPoint(240,272), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON38"));
    Button_OpenLog = new wxButton(Panel7, ID_BUTTON39, wxT("打开日志"), wxPoint(240,312), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON39"));
    Button_Closelog = new wxButton(Panel7, ID_BUTTON40, wxT("关闭日志"), wxPoint(336,312), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON40"));
    ListCtrl_NickInfo = new wxListCtrl(Panel7, ID_LISTCTRL13, wxPoint(16,352), wxSize(776,104), wxLC_REPORT, wxDefaultValidator, _T("ID_LISTCTRL13"));
    Panel8 = new wxPanel(Notebook1, ID_PANEL8, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL8"));
    StaticBox12 = new wxStaticBox(Panel8, ID_STATICBOX12, wxT("设置跟踪的IP"), wxPoint(8,8), wxSize(816,184), 0, _T("ID_STATICBOX12"));
    ListCtrl_TrackIPHistory = new wxListCtrl(Panel8, ID_LISTCTRL14, wxPoint(8,200), wxSize(816,256), wxLC_REPORT, wxDefaultValidator, _T("ID_LISTCTRL14"));
    StaticText32 = new wxStaticText(Panel8, ID_STATICTEXT32, wxT("要查询的ConnectID"), wxPoint(24,40), wxDefaultSize, 0, _T("ID_STATICTEXT32"));
    StaticText33 = new wxStaticText(Panel8, ID_STATICTEXT33, wxT("染色的IP地址"), wxPoint(24,80), wxDefaultSize, 0, _T("ID_STATICTEXT33"));
    TextCtr_TrackConnectID = new wxTextCtrl(Panel8, ID_TEXTCTRL29, wxEmptyString, wxPoint(152,32), wxSize(136,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL29"));
    Button_GetTrackSourceIP = new wxButton(Panel8, ID_BUTTON41, wxT("查询IP来源"), wxPoint(304,32), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON41"));
    TextCtrl_IPInfo = new wxTextCtrl(Panel8, ID_TEXTCTRL30, wxEmptyString, wxPoint(408,32), wxSize(136,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL30"));
    TextCtrl_IPInfo->Disable();
    TextCtr_TrackIP = new wxTextCtrl(Panel8, ID_TEXTCTRL31, wxT("127.0.0.1"), wxPoint(152,72), wxSize(136,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL31"));
    TextCtrl_TrackIPCount = new wxTextCtrl(Panel8, ID_TEXTCTRL32, wxT("1"), wxPoint(408,72), wxSize(136,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL32"));
    Button_SetTrackCommandID = new wxButton(Panel8, ID_BUTTON42, wxT("设置CommandID"), wxPoint(560,112), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON42"));
    Button_SetTrackIP = new wxButton(Panel8, ID_BUTTON43, wxT("设置IP"), wxPoint(560,72), wxSize(128,35), 0, wxDefaultValidator, _T("ID_BUTTON43"));
    Button_ShowTrackCommandID = new wxButton(Panel8, ID_BUTTON44, wxT("染色的CommandID信息"), wxPoint(16,152), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON44"));
    StaticText35 = new wxStaticText(Panel8, ID_STATICTEXT35, wxT("染色数据个数"), wxPoint(312,80), wxDefaultSize, 0, _T("ID_STATICTEXT35"));
    StaticText36 = new wxStaticText(Panel8, ID_STATICTEXT36, wxT("染色的CommandID"), wxPoint(24,120), wxDefaultSize, 0, _T("ID_STATICTEXT36"));
    TextCtrl_TrackCommandID = new wxTextCtrl(Panel8, ID_TEXTCTRL33, wxT("0x1000"), wxPoint(152,112), wxSize(136,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL33"));
    StaticText37 = new wxStaticText(Panel8, ID_STATICTEXT37, wxT("染色数据个数"), wxPoint(312,120), wxDefaultSize, 0, _T("ID_STATICTEXT37"));
    TextCtrl_TracCommandCount = new wxTextCtrl(Panel8, ID_TEXTCTRL34, wxT("1"), wxPoint(408,112), wxSize(136,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL34"));
    Panel9 = new wxPanel(Notebook1, ID_PANEL9, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL9"));
    StaticBox13 = new wxStaticBox(Panel9, ID_STATICBOX13, wxT("静态"), wxPoint(8,8), wxSize(864,224), 0, _T("ID_STATICBOX13"));
    StaticBox14 = new wxStaticBox(Panel9, ID_STATICBOX14, wxT("静态"), wxPoint(8,240), wxSize(864,216), 0, _T("ID_STATICBOX14"));
    ListCtrl_WorkThreadAI = new wxListCtrl(Panel9, ID_LISTCTRL15, wxPoint(16,40), wxSize(600,184), wxLC_REPORT|wxLC_SINGLE_SEL, wxDefaultValidator, _T("ID_LISTCTRL15"));
    CheckBox_WorkThread = new wxCheckBox(Panel9, ID_CHECKBOX2, wxT("WorkThreadAI是否开启"), wxPoint(632,40), wxSize(184,24), 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
    CheckBox_WorkThread->SetValue(false);
    StaticText34 = new wxStaticText(Panel9, ID_STATICTEXT34, wxT("超时时间"), wxPoint(632,72), wxDefaultSize, 0, _T("ID_STATICTEXT34"));
    TextCtrl_WorkThreadDisposeTime = new wxTextCtrl(Panel9, ID_TEXTCTRL35, wxT("40"), wxPoint(704,64), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL35"));
    StaticText38 = new wxStaticText(Panel9, ID_STATICTEXT38, wxT("毫秒"), wxPoint(816,72), wxDefaultSize, 0, _T("ID_STATICTEXT38"));
    StaticText39 = new wxStaticText(Panel9, ID_STATICTEXT39, wxT("时间区间"), wxPoint(632,112), wxDefaultSize, 0, _T("ID_STATICTEXT39"));
    TextCtrl_WorkThreadCheckTime = new wxTextCtrl(Panel9, ID_TEXTCTRL36, wxT("30"), wxPoint(704,104), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL36"));
    StaticText40 = new wxStaticText(Panel9, ID_STATICTEXT40, wxT("秒"), wxPoint(816,112), wxDefaultSize, 0, _T("ID_STATICTEXT40"));
    StaticText41 = new wxStaticText(Panel9, ID_STATICTEXT41, wxT("中断时间"), wxPoint(632,152), wxDefaultSize, 0, _T("ID_STATICTEXT41"));
    TextCtrl_WorkThreadStopTime = new wxTextCtrl(Panel9, ID_TEXTCTRL37, wxT("30"), wxPoint(704,144), wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL37"));
    StaticText42 = new wxStaticText(Panel9, ID_STATICTEXT42, wxT("秒"), wxPoint(816,152), wxDefaultSize, 0, _T("ID_STATICTEXT42"));
    Button_ResetWorkThread = new wxButton(Panel9, ID_BUTTON45, wxT("重新设置"), wxPoint(632,192), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON45"));
    Button_GetWorkThreadParam = new wxButton(Panel9, ID_BUTTON46, wxT("工作线程AI参数"), wxPoint(720,192), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON46"));
    ListCtr_WorkThreadTimeout = new wxListCtrl(Panel9, ID_LISTCTRL16, wxPoint(16,264), wxSize(408,184), wxLC_REPORT|wxLC_SINGLE_SEL, wxDefaultValidator, _T("ID_LISTCTRL16"));
    ListCtrl_WorkThreadForbiden = new wxListCtrl(Panel9, ID_LISTCTRL17, wxPoint(440,264), wxSize(280,184), wxLC_REPORT|wxLC_SINGLE_SEL, wxDefaultValidator, _T("ID_LISTCTRL17"));
    Button_GetWorkThreadPool = new wxButton(Panel9, ID_BUTTON47, wxT("查询当前AI数据池"), wxPoint(728,408), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON47"));
    Notebook1->AddPage(Panel1, wxT("客户端主模块管理"), false);
    Notebook1->AddPage(Panel2, wxT("客户端鏈接管理模块"), false);
    Notebook1->AddPage(Panel3, wxT("客户端主模块运行信息"), false);
    Notebook1->AddPage(Panel4, wxT("客户端链接管理"), false);
    Notebook1->AddPage(Panel5, wxT("工作线程状态"), false);
    Notebook1->AddPage(Panel6, wxT("服务器间链接信息"), false);
    Notebook1->AddPage(Panel7, wxT("IP封禁管理"), false);
    Notebook1->AddPage(Panel8, wxT("数据染色"), false);
    Notebook1->AddPage(Panel9, wxT("工作线程AI相关"), false);
    Timer_WorkThread.SetOwner(this, ID_TIMER1);
    Timer_WorkThread.Start(60000, false);
    Center();

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_SetClick);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_GetServerstateClick);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_CloseServerClick);
    Connect(ID_LISTBOX1,wxEVT_COMMAND_LISTBOX_SELECTED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnListBox_ConfigListSelect);
    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_LoadConfigfileClick);
    Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_ShowAllModuleClick);
    Connect(ID_TEXTCTRL6,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnTextCtrl_ModuleNmaeText);
    Connect(ID_BUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_LoadModuleClick);
    Connect(ID_BUTTON7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_UnloadModuleClick);
    Connect(ID_BUTTON8,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_ReloadModuleClick);
    Connect(ID_LISTCTRL1,wxEVT_COMMAND_LIST_ITEM_SELECTED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnListCtrl_ModuleListItemSelect);
    Connect(ID_BUTTON9,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_GetCurrClientCountClick);
    Connect(ID_TEXTCTRL8,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnTextCtrl_CurrLinkCountText);
    Connect(ID_BUTTON10,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_SetLinkLimitClick);
    Connect(ID_BUTTON11,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_SetDebugClick);
    Connect(ID_BUTTON12,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_GetDebugStateClick);
    Connect(ID_BUTTON13,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_StartFileClick);
    Connect(ID_BUTTON14,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_StopFileClick);
    Connect(ID_BUTTON15,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_GetLinkHistoryClick);
    Connect(ID_TEXTCTRL16,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnTextCtrl_ModuleIDText);
    Connect(ID_BUTTON16,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_ShowModuleCmdClick);
    Connect(ID_BUTTON17,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_ShowAllCmdClick);
    Connect(ID_BUTTON18,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_ShowTimeoutCmdClick);
    Connect(ID_BUTTON19,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_ClearTimeoutCmdClick);
    Connect(ID_BUTTON20,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_SaveSmdHistoryClick);
    Connect(ID_BUTTON21,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_DisconnectClientClick);
    Connect(ID_BUTTON22,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_ShowClientClick);
    Connect(ID_BUTTON23,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_SetLogLevelClick);
    Connect(ID_BUTTON24,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_ShowCurrLoginfoClick);
    Connect(ID_BUTTON25,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_ShowWorkThreadStatClick);
    Connect(ID_BUTTON26,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_StartMonitorClick);
    Connect(ID_BUTTON27,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_StopMonitorClick);
    Connect(ID_BUTTON28,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_LoadFileClick);
    Connect(ID_BUTTON29,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_ClearLoadClick);
    Connect(ID_BUTTON30,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_ShowServerLinkInfoClick);
    Connect(ID_BUTTON31,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_ReconnectServerClick);
    Connect(ID_BUTTON32,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_ShowPSSLisenInfoClick);
    Connect(ID_BUTTON33,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_InsertNewMonitorClick);
    Connect(ID_BUTTON34,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_DeleteOldMonitorClick);
    Connect(ID_BUTTON35,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_InsertForbidenClick);
    Connect(ID_BUTTON36,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_ReleaseForbidenClick);
    Connect(ID_BUTTON37,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_ShowForbidenClick);
    Connect(ID_BUTTON38,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_SearchNicknameClick);
    Connect(ID_BUTTON39,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_OpenLogClick);
    Connect(ID_BUTTON40,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_CloselogClick);
    Connect(ID_BUTTON41,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_GetTrackSourceIPClick);
    Connect(ID_BUTTON42,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_SetTrackCommandIDClick);
    Connect(ID_BUTTON43,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_SetTrackIPClick);
    Connect(ID_BUTTON44,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_ShowTrackCommandIDClick);
    Connect(ID_BUTTON45,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_ResetWorkThreadClick);
    Connect(ID_BUTTON46,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_GetWorkThreadParamClick);
    Connect(ID_BUTTON47,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnButton_GetWorkThreadPoolClick);
    Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&PSS_ClientManagerFrame::OnTimer_WorkThreadTrigger);
    //*)





    this->ListCtrl_ModuleList->InsertColumn(0, wxT("当前已加载模块"), wxLIST_FORMAT_CENTER, 120);
    this->ListCtrl_ModuleList->InsertColumn(1, wxT("模块名称"), wxLIST_FORMAT_CENTER, 80);
    this->ListCtrl_ModuleList->InsertColumn(2, wxT("模块路径"), wxLIST_FORMAT_CENTER, 80);
    this->ListCtrl_ModuleList->InsertColumn(3, wxT("模块参数"), wxLIST_FORMAT_CENTER, 80);
    this->ListCtrl_ModuleList->InsertColumn(4, wxT("模块描述"), wxLIST_FORMAT_CENTER, 80);
    this->ListCtrl_ModuleList->InsertColumn(5, wxT("加载时间"), wxLIST_FORMAT_CENTER, 150);
    this->ListCtrl_ModuleList->InsertColumn(6, wxT("当前状态"), wxLIST_FORMAT_CENTER, 80);
    this->ListCtrl_ModuleList->InsertColumn(7, wxT("状态ID"), wxLIST_FORMAT_CENTER, 80);


    this->ListCtrl_LinkHistory->InsertColumn(0, wxT("IP地址"), wxLIST_FORMAT_CENTER, 100);
    this->ListCtrl_LinkHistory->InsertColumn(1, wxT("单位时间链接次数"), wxLIST_FORMAT_CENTER, 150);
    this->ListCtrl_LinkHistory->InsertColumn(2, wxT("总链接数"), wxLIST_FORMAT_CENTER, 100);
    this->ListCtrl_LinkHistory->InsertColumn(3, wxT("最后链接时间"), wxLIST_FORMAT_CENTER, 200);



    this->ListCtrl_ModuleInfo->InsertColumn(0, wxT("模块名称"), wxLIST_FORMAT_CENTER, 200);
    this->ListCtrl_ModuleInfo->InsertColumn(1, wxT("命令ID"), wxLIST_FORMAT_CENTER, 100);
    this->ListCtrl_ModuleInfo->InsertColumn(2, wxT("调用次数"), wxLIST_FORMAT_CENTER, 100);
    this->ListCtrl_ModuleInfo->InsertColumn(3, wxT("时间消耗"), wxLIST_FORMAT_CENTER, 200);

    this->ListCtrl_CommandTimeout->InsertColumn(0, wxT("命令ID"), wxLIST_FORMAT_CENTER, 100);
    this->ListCtrl_CommandTimeout->InsertColumn(1, wxT("发生时间"), wxLIST_FORMAT_CENTER, 200);
    this->ListCtrl_CommandTimeout->InsertColumn(2, wxT("超时时间"), wxLIST_FORMAT_CENTER, 100);




    this->ListCtrl_ClientTcpInfo->InsertColumn(0, wxT("IP地址"), wxLIST_FORMAT_CENTER, 100);
    this->ListCtrl_ClientTcpInfo->InsertColumn(1, wxT("Socketfd"), wxLIST_FORMAT_CENTER, 80);
    this->ListCtrl_ClientTcpInfo->InsertColumn(2, wxT("发送包数"), wxLIST_FORMAT_CENTER, 80);
    this->ListCtrl_ClientTcpInfo->InsertColumn(3, wxT("接收包数"), wxLIST_FORMAT_CENTER, 80);
    this->ListCtrl_ClientTcpInfo->InsertColumn(4, wxT("发送字节"), wxLIST_FORMAT_CENTER, 80);
    this->ListCtrl_ClientTcpInfo->InsertColumn(5, wxT("接收字节"), wxLIST_FORMAT_CENTER, 80);
    this->ListCtrl_ClientTcpInfo->InsertColumn(6, wxT("创建时间"), wxLIST_FORMAT_CENTER, 80);
    this->ListCtrl_ClientTcpInfo->InsertColumn(7, wxT("存活秒数"), wxLIST_FORMAT_CENTER, 80);
    this->ListCtrl_ClientTcpInfo->InsertColumn(8, wxT("逻辑处理包数"), wxLIST_FORMAT_CENTER, 120);

    this->ListCtrl_LogInfo->InsertColumn(0, wxT("日志ID"), wxLIST_FORMAT_CENTER, 100);
    this->ListCtrl_LogInfo->InsertColumn(1, wxT("日志前缀"), wxLIST_FORMAT_CENTER, 100);
    this->ListCtrl_LogInfo->InsertColumn(2, wxT("日志名称"), wxLIST_FORMAT_CENTER, 100);
    this->ListCtrl_LogInfo->InsertColumn(3, wxT("打印or写文件"), wxLIST_FORMAT_CENTER, 100);
    this->ListCtrl_LogInfo->InsertColumn(4, wxT("日志等级"), wxLIST_FORMAT_CENTER, 100);





    this->ListCtrl_WorkThreadInfo->InsertColumn(0, wxT("线程名称"),  wxLIST_FORMAT_CENTER, 100);
    this->ListCtrl_WorkThreadInfo->InsertColumn(1, wxT("线程ID"),  wxLIST_FORMAT_CENTER, 100);
    this->ListCtrl_WorkThreadInfo->InsertColumn(2, wxT("线程创建时间"),  wxLIST_FORMAT_CENTER, 100);
    this->ListCtrl_WorkThreadInfo->InsertColumn(3, wxT("最后执行时间"),  wxLIST_FORMAT_CENTER, 100);
    this->ListCtrl_WorkThreadInfo->InsertColumn(4, wxT("处理包数量"),  wxLIST_FORMAT_CENTER, 80);
    this->ListCtrl_WorkThreadInfo->InsertColumn(5, wxT("当前处理ID"),  wxLIST_FORMAT_CENTER, 80);
    this->ListCtrl_WorkThreadInfo->InsertColumn(6, wxT("平均耗时"),  wxLIST_FORMAT_CENTER, 80);
    this->ListCtrl_WorkThreadInfo->InsertColumn(7, wxT("单位处理量"),  wxLIST_FORMAT_CENTER, 80);



    this->ListCtrl_ServerConnect->InsertColumn(0, wxT("ID"), wxLIST_FORMAT_CENTER, 100);
    this->ListCtrl_ServerConnect->InsertColumn(1, wxT("IP地址"), wxLIST_FORMAT_CENTER, 100);
    this->ListCtrl_ServerConnect->InsertColumn(2, wxT("连接状态"), wxLIST_FORMAT_CENTER, 80);
    this->ListCtrl_ServerConnect->InsertColumn(3, wxT("发送包数"), wxLIST_FORMAT_CENTER, 80);
    this->ListCtrl_ServerConnect->InsertColumn(4, wxT("接收包数"), wxLIST_FORMAT_CENTER, 80);
    this->ListCtrl_ServerConnect->InsertColumn(5, wxT("发送字节"), wxLIST_FORMAT_CENTER, 80);
    this->ListCtrl_ServerConnect->InsertColumn(6, wxT("接收字节"), wxLIST_FORMAT_CENTER, 80);
    this->ListCtrl_ServerConnect->InsertColumn(7, wxT("创建时间"), wxLIST_FORMAT_CENTER, 80);
    this->ListCtrl_ServerConnect->InsertColumn(8, wxT("存活秒数"), wxLIST_FORMAT_CENTER, 80);

    this->ListCtrl_ServerLisen->InsertColumn(0, wxT("监听IP"), wxLIST_FORMAT_CENTER, 200);
    this->ListCtrl_ServerLisen->InsertColumn(1, wxT("监听端口"), wxLIST_FORMAT_CENTER, 100);





    this->ListCtrl_ForbidenList->InsertColumn(0, wxT("IP地址"), wxLIST_FORMAT_CENTER, 150);
    this->ListCtrl_ForbidenList->InsertColumn(1, wxT("封禁类型"), wxLIST_FORMAT_CENTER, 100);
    this->ListCtrl_ForbidenList->InsertColumn(2, wxT("封禁开始时间"), wxLIST_FORMAT_CENTER, 200);
    this->ListCtrl_ForbidenList->InsertColumn(3, wxT("封禁持续秒数"), wxLIST_FORMAT_CENTER, 100);

    this->ListCtrl_NickInfo->InsertColumn(0, wxT("ConnectID"), wxLIST_FORMAT_CENTER, 100);
    this->ListCtrl_NickInfo->InsertColumn(1, wxT("别名"), wxLIST_FORMAT_CENTER, 150);
    this->ListCtrl_NickInfo->InsertColumn(2, wxT("客户端IP"), wxLIST_FORMAT_CENTER, 100);
    this->ListCtrl_NickInfo->InsertColumn(3, wxT("端口"), wxLIST_FORMAT_CENTER, 50);
    this->ListCtrl_NickInfo->InsertColumn(4, wxT("日志状态"), wxLIST_FORMAT_CENTER, 50);


    this->ListCtrl_TrackIPHistory->InsertColumn(0, wxT("命令ID"), wxLIST_FORMAT_CENTER, 150);
    this->ListCtrl_TrackIPHistory->InsertColumn(1, wxT("当前已染色包数"), wxLIST_FORMAT_CENTER, 100);
    this->ListCtrl_TrackIPHistory->InsertColumn(2, wxT("最大染色包数"), wxLIST_FORMAT_CENTER, 100);






    this->ListCtrl_WorkThreadAI->InsertColumn(0, wxT("线程ID"), wxLIST_FORMAT_CENTER, 100);
    this->ListCtrl_WorkThreadAI->InsertColumn(1, wxT("AI开关"), wxLIST_FORMAT_CENTER, 100);
    this->ListCtrl_WorkThreadAI->InsertColumn(2, wxT("处理超时时间"), wxLIST_FORMAT_CENTER, 100);
    this->ListCtrl_WorkThreadAI->InsertColumn(3, wxT("判断时间区间"), wxLIST_FORMAT_CENTER, 100);
    this->ListCtrl_WorkThreadAI->InsertColumn(4, wxT("区间内超时次数"), wxLIST_FORMAT_CENTER, 100);
    this->ListCtrl_WorkThreadAI->InsertColumn(5, wxT("命令中断时间"), wxLIST_FORMAT_CENTER, 100);

    this->ListCtr_WorkThreadTimeout->InsertColumn(0, wxT("线程ID"), wxLIST_FORMAT_CENTER, 50);
    this->ListCtr_WorkThreadTimeout->InsertColumn(1, wxT("命令ID"), wxLIST_FORMAT_CENTER, 50);
    this->ListCtr_WorkThreadTimeout->InsertColumn(2, wxT("时间"), wxLIST_FORMAT_CENTER, 120);
    this->ListCtr_WorkThreadTimeout->InsertColumn(3, wxT("执行时间"), wxLIST_FORMAT_CENTER, 50);

    this->ListCtrl_WorkThreadForbiden->InsertColumn(0, wxT("线程ID"), wxLIST_FORMAT_CENTER, 50);
    this->ListCtrl_WorkThreadForbiden->InsertColumn(1, wxT("命令ID"), wxLIST_FORMAT_CENTER, 50);
    this->ListCtrl_WorkThreadForbiden->InsertColumn(2, wxT("到期时间"), wxLIST_FORMAT_CENTER, 120);

    this->Init();

}

PSS_ClientManagerFrame::~PSS_ClientManagerFrame()
{
    //(*Destroy(PSS_ClientManagerFrame)
    //*)

    delete  m_pTcpClientConnect;
}


void PSS_ClientManagerFrame::Init()
{


    //初始化TCP链接

    InitializeSocketEnvironment();

    //    m_sckClient =new CSockWrap(SOCK_STREAM);

    this->LoadConfigFile();
    this->ChangeConfigFile();

    std::string    strServerIP    =std::string(this->TextCtrl_ServerIP->GetValue());
    std::string    strConsoleKey    =std::string(this->TextCtrl_ServerKey->GetValue());
    int nPort = wxAtoi(this->TextCtrl_ServerPort->GetValue());

    m_pTcpClientConnect =new    CTcpClientConnect();
    m_pTcpClientConnect->Init(strServerIP.c_str(), nPort, strConsoleKey.c_str());

    m_blTimeRun         = false;


}


void PSS_ClientManagerFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void PSS_ClientManagerFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, wxT("Welcome to..."));
}

void PSS_ClientManagerFrame::OnButton_ReloadModuleClick(wxCommandEvent& event)
{

    //重载模块
    wxCSConv cvGBK( wxT("GBK") );
    std::string    strModuleName    =std::string( cvGBK.cWX2MB( this->TextCtrl_ModuleName->GetValue().c_str() ) );

    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s ReLoadModule %s&", m_pTcpClientConnect->GetKey(), strModuleName.c_str());
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[10 * 1024] = {'\0'};
    int nRecvLen = 10 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        int nStrLen      = 0;
        int nPos         = 0;
        int nResult      = 0;
        memcpy_s(&nResult, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);

        if(nResult == 0)
        {
            wxMessageBox(wxT(MESSAGE_RESULT_SUCCESS), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        }
        else
        {
            wxMessageBox(wxT(MESSAGE_RESULT_FAIL), wxT(MESSAGE_RESULT_FAIL), wxOK | wxICON_INFORMATION, this);
        }
    }

    this->OnButton_ShowAllModuleClick(event);


}

void PSS_ClientManagerFrame::OnButton_UnloadModuleClick(wxCommandEvent& event)
{

    //卸载模块
    wxCSConv cvGBK( wxT("GBK") );
    std::string    strModuleName    =std::string( cvGBK.cWX2MB( this->TextCtrl_ModuleName->GetValue().c_str() ) );

    if(strModuleName.length() == 0)
    {
        wxMessageBox(wxT(MESSAGE_INSERT_NULL), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }

    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s UnLoadModule %s&", m_pTcpClientConnect->GetKey(), strModuleName.c_str());
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[10 * 1024] = {'\0'};
    int nRecvLen = 10 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        int nStrLen      = 0;
        int nPos         = 0;
        int nResult      = 0;
        memcpy_s(&nResult, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);

        if(nResult == 0)
        {
            wxMessageBox(wxT(MESSAGE_RESULT_SUCCESS), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        }
        else
        {
            wxMessageBox(wxT(MESSAGE_RESULT_FAIL), wxT(MESSAGE_TITLE_SUCCESS), wxOK | wxICON_INFORMATION, this);
        }
    }

    this->OnButton_ShowAllModuleClick(event);

}

void PSS_ClientManagerFrame::OnTextCtrl_ModuleNmaeText(wxCommandEvent& event)
{
}

void PSS_ClientManagerFrame::OnButton_CloseServerClick(wxCommandEvent& event)
{
    // TODO: 关闭服务器
    if (wxMessageBox(wxT(MESSAGE_IS_CLOSE_SERVER), wxT(MESSAGE_TITLE_INFO), wxOK | wxICON_INFORMATION, this) == wxOK)
    {
        char szSendMessage[200] = {'\0'};
        char szCommand[100]     = {'\0'};
        sprintf_s(szCommand, 100, "%s ServerClose -a&", m_pTcpClientConnect->GetKey());
        int nSendLen = (int)strlen(szCommand);

        memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

        char szRecvBuff[10 * 1024] = {'\0'};
        int nRecvLen = 10 * 1024;
        m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);
        wxMessageBox(wxT(MESSAGE_IS_CLOSE_OVER), wxT(MESSAGE_TITLE_INFO), wxOK | wxICON_INFORMATION, this);
    }

}



void PSS_ClientManagerFrame::ChangeConfigFile()
{
    //设置相关属性

    int nSel = this->ListBox_ConfigList->GetSelection();

    if  ((nSel < 0) || (nSel >= m_vecConfigInfo.size()  ))
    {
        return;
    }

    this->TextCtrl_ServerIP->SetValue(wxString(m_vecConfigInfo[nSel].m_szIP));
    this->TextCtrl_ServerPort->SetValue(wxString::Format(wxT("%d"),m_vecConfigInfo[nSel].m_nPort));
    this->TextCtrl_ServerKey->SetValue(wxString(m_vecConfigInfo[nSel].m_szMagicCode));

}


void PSS_ClientManagerFrame::OnListBox_ConfigListSelect(wxCommandEvent& event)
{
    this->ChangeConfigFile();

}

void PSS_ClientManagerFrame::OnListCtrl_ModuleListItemSelect(wxListEvent& event)
{
    int nIndex  =  this->ListCtrl_ModuleList->GetNextItem(-1,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);

    if(nIndex != -1)
    {
        this->TextCtrl_ModulePath->SetValue(this->ListCtrl_ModuleList->GetItemText(nIndex, 2));
        this->TextCtrl_ModuleFilename->SetValue(this->ListCtrl_ModuleList->GetItemText(nIndex, 0));

        this->TextCtrl_ModuleName->SetValue(this->ListCtrl_ModuleList->GetItemText(nIndex, 1));
        this->TextCtrl_ModuleParam->SetValue(this->ListCtrl_ModuleList->GetItemText(nIndex, 3));
    }

}

void PSS_ClientManagerFrame::OnButton_GetCurrLinkCountClick(wxCommandEvent& event)
{
}

void PSS_ClientManagerFrame::OnButton_SetLinkLimitClick(wxCommandEvent& event)
{
    //重新设置上限
    int nMaxConnectCount = wxAtoi(this->TextCtrl_LinkUpperLimit->GetValue());


    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s SetMaxConnectCount -n %d &", m_pTcpClientConnect->GetKey(), nMaxConnectCount);
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[100 * 1024] = {'\0'};
    int nRecvLen = 100 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        int nStrLen       = 0;
        int nPos          = 0;
        int nOPState      = 0;
        memcpy_s(&nOPState, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);

        if(nOPState == 0)
        {
            wxMessageBox(wxT(MESSAGE_RESULT_SUCCESS), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        }
        else
        {
            wxMessageBox(wxT(MESSAGE_RESULT_FAIL), wxT(MESSAGE_TITLE_SUCCESS), wxOK | wxICON_INFORMATION, this);
        }
    }

}

void PSS_ClientManagerFrame::OnButton_SetDebugClick(wxCommandEvent& event)
{

    //设置DEBUG状态
    int nType = this->RadioBox_DebugState->GetSelection();


    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s SetDebug -s %d &", m_pTcpClientConnect->GetKey(), nType);
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[10 * 1024] = {'\0'};
    int nRecvLen = 10 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
    }
    else
    {
        int nStrLen      = 0;
        int nPos         = 0;
        int nResult      = 0;
        memcpy_s(&nResult, sizeof(int), &szRecvBuff[nPos], sizeof(char));
        nPos += sizeof(int);

        if(nResult == 0)
        {
            wxMessageBox(wxT(MESSAGE_RESULT_SUCCESS), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
            this->OnButton_GetDebugStateClick(event);
            // OnBnClickedButton5();
        }
        else
        {
            wxMessageBox(wxT(MESSAGE_RESULT_FAIL), wxT(MESSAGE_TITLE_SUCCESS), wxOK | wxICON_INFORMATION, this);
        }
    }


}

void PSS_ClientManagerFrame::OnButton_GetDebugStateClick(wxCommandEvent& event)
{
    //获得DEBUG状态
    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s ShowDebug -a&", m_pTcpClientConnect->GetKey());
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[10 * 1024] = {'\0'};
    int nRecvLen = 10 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
    }
    else
    {
        int nStrLen      = 0;
        int nPos         = 0;
        int nResult      = 0;
        memcpy_s(&nResult, sizeof(int), &szRecvBuff[nPos], sizeof(char));
        nPos += sizeof(int);

        if(nResult == 0)
        {
            this->TextCtrl_CurrDebugStat->SetValue(wxT("DEBUG日志关闭"));
        }
        else
        {
            this->TextCtrl_CurrDebugStat->SetValue(wxT("DEBUG日志打开"));
        }
    }

}

void PSS_ClientManagerFrame::OnButton_StartFileClick(wxCommandEvent& event)
{
    //启动文件
    wxCSConv cvGBK( wxT("GBK") );
    std::string    strPacketFileName    =std::string( cvGBK.cWX2MB( this->TextCtrl_PacketFile->GetValue().c_str() ) );

    char szSendMessage[200] = { '\0' };
    char szCommand[100] = { '\0' };
    sprintf_s(szCommand, 100, "%s TestFileStart -f %s&", m_pTcpClientConnect->GetKey(), strPacketFileName.c_str());
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[100 * 1024] = { '\0' };
    int nRecvLen = 100 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if (blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        int nStrLen = 0;
        int nPos = 0;
        int nOPState = 0;
        memcpy_s(&nOPState, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);

        if (nOPState == 0)
        {
            wxString strTxt;
            int nData = 0;
            char szTemp[1024]        = { '\0' };
            char szCommandDesc[1024] = { '\0' };
            memcpy_s(&nData, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);
            sprintf(szTemp, ("TimeInterval:[%d]&"), nData);
            strTxt += szTemp;
            memcpy_s(&nData, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);
            sprintf(szTemp, ("ProcolCount:[%d]&"), nData);
            strTxt += szTemp;
            memcpy_s(&nData, sizeof(short), &szRecvBuff[nPos], sizeof(short));
            nPos += sizeof(short);
            int nCount = nData;

            for (int i = 0; i < nCount; i++)
            {
                memcpy_s(&nData, sizeof(int), &szRecvBuff[nPos], sizeof(int));
                nPos += sizeof(int);
                memcpy_s(&szCommandDesc, nData, &szRecvBuff[nPos], nData);
                szCommandDesc[nData] = '\0';
                nPos += nData;
                //wchar_t szDesc[1024] = { '\0' };

                //int nSrcLen = MultiByteToWideChar(CP_UTF8, 0, szCommandDesc, -1, NULL, 0);
                //int nDecLen = MultiByteToWideChar(CP_UTF8, 0, szCommandDesc, -1, szDesc, 1024);

                sprintf(szTemp, ("CommandDesc:[%s]&"), szCommandDesc);
                strTxt += szTemp;
            }

            wxMessageBox(wxString(strTxt), wxT(MESSAGE_TITLE_SUCCESS), wxOK | wxICON_INFORMATION, this);
        }
        else
        {
            wxMessageBox(wxT(MESSAGE_RESULT_FAIL), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        }
    }

}

void PSS_ClientManagerFrame::OnButton_StopFileClick(wxCommandEvent& event)
{

    //停止测试
    char szSendMessage[200] = { '\0' };
    char szCommand[100] = { '\0' };
    sprintf_s(szCommand, 100, "%s TestFileStop -a&", m_pTcpClientConnect->GetKey());
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[100 * 1024] = { '\0' };
    int nRecvLen = 100 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if (blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        int nStrLen = 0;
        int nPos = 0;
        int nOPState = 0;
        memcpy_s(&nOPState, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);

        if (nOPState == 0)
        {
            wxMessageBox(wxT(MESSAGE_RESULT_SUCCESS), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        }
        else
        {
            wxMessageBox(wxT(MESSAGE_RESULT_FAIL), wxT(MESSAGE_TITLE_SUCCESS), wxOK | wxICON_INFORMATION, this);
        }
    }
}

void PSS_ClientManagerFrame::OnButton_GetLinkHistoryClick(wxCommandEvent& event)
{
    this->ListCtrl_LinkHistory->DeleteAllItems();

    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s ShowConnectHistory -a&", m_pTcpClientConnect->GetKey());
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[100 * 1024] = {'\0'};
    int nRecvLen = 100 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        vecIPAccount objvecIPAccount;

        int nStrLen       = 0;
        int nPos          = 0;
        int nTCPCount     = 0;
        memcpy_s(&nTCPCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);

        for(int i = 0; i < nTCPCount; i++)
        {
            //开始还原数据结构
            _IPAccount IPAccount;

            memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
            nPos += sizeof(char);

            memcpy_s(IPAccount.m_szIP, nStrLen, &szRecvBuff[nPos], nStrLen);
            nPos += nStrLen;
            IPAccount.m_szIP[nStrLen] = '\0';

            memcpy_s(&IPAccount.m_nCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            memcpy_s(&IPAccount.m_nAllCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
            nPos += sizeof(char);

            memcpy_s(IPAccount.m_szDate, nStrLen, &szRecvBuff[nPos], nStrLen);
            nPos += nStrLen;
            IPAccount.m_szDate[nStrLen] = '\0';

            //显示在界面上

            this->ListCtrl_LinkHistory->InsertItem(i, wxString(IPAccount.m_szIP));
            this->ListCtrl_LinkHistory->InsertItem(i, wxString::Format(wxT("%d"),IPAccount.m_nCount));
            this->ListCtrl_LinkHistory->InsertItem(i, wxString::Format(wxT("%d"),IPAccount.m_nAllCount));
            this->ListCtrl_LinkHistory->InsertItem(i, wxString(IPAccount.m_szDate));

            objvecIPAccount.push_back(IPAccount);
        }

    }

}

void PSS_ClientManagerFrame::OnTextCtrl_CurrLinkCountText(wxCommandEvent& event)
{
}

void PSS_ClientManagerFrame::OnButton_GetCurrClientCountClick(wxCommandEvent& event)
{
    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s ClientCount -cp&", m_pTcpClientConnect->GetKey());
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[10 * 1024] = {'\0'};
    int nRecvLen = 10 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        int nStrLen          = 0;
        int nPos             = 0;
        int nConnectCount    = 0;
        int nFreeCount       = 0;
        int nMaxConnectCount = 0;
        memcpy_s(&nConnectCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);
        memcpy_s(&nFreeCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);
        memcpy_s(&nMaxConnectCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);

        this->TextCtrl_LinkUpperLimit->SetValue(wxString::Format(wxT("%d"),nMaxConnectCount));
        this->TextCtrl_CurrClientCount->SetValue(wxString::Format(wxT("%d"),nConnectCount));

    }

    //获得CPU和内存占用量
    sprintf_s(szCommand, 100, "%s ShowCurrProcessInfo -a&", m_pTcpClientConnect->GetKey());
    nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    nRecvLen = 10 * 1024;
    blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        int  nStrLen       = 0;
        int  nPos          = 0;
        int  nCpuRote      = 0;
        int  nMemoryRote   = 0;
        char cFlow         = 0;
        int  nFlowIn       = 0;
        int  nFlowOut      = 0;

        memcpy_s(&nCpuRote, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);
        memcpy_s(&nMemoryRote, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);
        memcpy_s(&cFlow, sizeof(char), &szRecvBuff[nPos], sizeof(char));
        nPos += sizeof(char);
        memcpy_s(&nFlowIn, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);
        memcpy_s(&nFlowOut, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);


        this->TextCtrl_CurrCpuOccupy->SetValue(wxString::Format(wxT("%f"), (float)nCpuRote/100.0f));
        this->TextCtrl_CurrMemOccupy->SetValue(wxString::Format(wxT("%f"), (float)nMemoryRote/(1000.0f*1000.0f)));



        if(cFlow == 0)
        {
            this->TextCtrl_CurrFlowIn->SetValue(wxString("关闭"));
            this->TextCtrl_CurrFlowOut->SetValue(wxString("关闭"));
        }
        else
        {
            this->TextCtrl_CurrFlowIn->SetValue(wxString::Format(wxT("%d"), nFlowIn));
            this->TextCtrl_CurrFlowOut->SetValue(wxString::Format(wxT("%d"),nFlowOut));
        }
    }

}

void PSS_ClientManagerFrame::OnTextCtrl_ModuleIDText(wxCommandEvent& event)
{
}

void PSS_ClientManagerFrame::OnButton_ShowModuleCmdClick(wxCommandEvent& event)
{
    //查看指定命令
    wxCSConv cvGBK( wxT("GBK") );
    std::string    strCommandID    =std::string( cvGBK.cWX2MB( this->TextCtrl_ModuleID->GetValue().c_str() ) );

    if(strCommandID.length() == 0)
    {
        wxMessageBox(wxT(MESSAGE_INSERT_NULL), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }

    this->ListCtrl_ModuleInfo->DeleteAllItems();

    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s CommandInfo %s&", m_pTcpClientConnect->GetKey(), strCommandID.c_str());
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[10 * 1024] = {'\0'};
    int nRecvLen = 10 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        vecCommandInfo objvecCommandInfo;

        int nIndex            = 0;
        int nStrLen           = 0;
        int nPos              = 0;
        int nCommandCount     = 0;
        int nCommandInfoCount = 0;
        memcpy_s(&nCommandCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);
        memcpy(&nCommandInfoCount, &szRecvBuff[nPos], sizeof(short));
        nPos += sizeof(short);

        for(int i = 0; i < nCommandInfoCount; i++)
        {
            //开始还原数据结构
            _CommandInfo CommandInfo;
            /*
            memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
            nPos += sizeof(char);

            memcpy_s(CommandInfo.szModuleName, nStrLen, &szRecvBuff[nPos], nStrLen);
            nPos += nStrLen;
            CommandInfo.szModuleName[nStrLen] = '\0';
            */

            int nCommandID = 0;
            memcpy_s(&nCommandID, sizeof(short), &szRecvBuff[nPos], sizeof(short));
            nPos += sizeof(short);
            sprintf_s(CommandInfo.szCommandID, 200, "0x%04x", nCommandID);

            memcpy_s(&CommandInfo.m_nCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            memcpy_s(&CommandInfo.m_nTimeCost, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);


            this->ListCtrl_ModuleInfo->InsertItem(nIndex, wxString(CommandInfo.szModuleName));
            this->ListCtrl_ModuleInfo->SetItem(nIndex, 1, wxString(CommandInfo.szCommandID));
            this->ListCtrl_ModuleInfo->SetItem(nIndex, 2, wxString::Format(wxT("%d"),CommandInfo.m_nCount));
            this->ListCtrl_ModuleInfo->SetItem(nIndex, 3, wxString::Format(wxT("%d"),CommandInfo.m_nTimeCost));


            nIndex++;

            objvecCommandInfo.push_back(CommandInfo);
        }
    }

}

void PSS_ClientManagerFrame::OnButton_ShowAllCmdClick(wxCommandEvent& event)
{
    //查看全部
    this->ListCtrl_ModuleInfo->DeleteAllItems();

    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s ShowAllCommand -a&", m_pTcpClientConnect->GetKey());
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[10 * 1024] = {'\0'};
    int nRecvLen = 10 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        vecCommandInfo objvecCommandInfo;

        int nIndex        = 0;
        int nStrLen       = 0;
        int nPos          = 0;
        int nModuleCount  = 0;
        int nCommandCount = 0;
        memcpy_s(&nModuleCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);

        for(int i = 0; i < nModuleCount; i++)
        {

            memcpy(&nCommandCount, &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            for(int j = 0; j < nCommandCount; j++)
            {
                //开始还原数据结构
                _CommandInfo CommandInfo;
                /*
                memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
                nPos += sizeof(char);

                memcpy_s(CommandInfo.szModuleName, nStrLen, &szRecvBuff[nPos], nStrLen);
                nPos += nStrLen;
                CommandInfo.szModuleName[nStrLen] = '\0';
                */

                int nCommandID = 0;
                memcpy_s(&nCommandID, sizeof(short), &szRecvBuff[nPos], sizeof(short));
                nPos += sizeof(short);
                sprintf_s(CommandInfo.szCommandID, 200, "0x%04x", nCommandID);

                memcpy_s(&CommandInfo.m_nCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
                nPos += sizeof(int);

                memcpy_s(&CommandInfo.m_nTimeCost, sizeof(int), &szRecvBuff[nPos], sizeof(int));
                nPos += sizeof(int);

                this->ListCtrl_ModuleInfo->InsertItem(nIndex, wxString(CommandInfo.szModuleName));
                this->ListCtrl_ModuleInfo->SetItem(nIndex, 1, wxString(CommandInfo.szCommandID));
                this->ListCtrl_ModuleInfo->SetItem(nIndex, 2, wxString::Format(wxT("%d"),CommandInfo.m_nCount));
                this->ListCtrl_ModuleInfo->SetItem(nIndex, 3, wxString::Format(wxT("%d"),CommandInfo.m_nTimeCost));

                nIndex++;

                objvecCommandInfo.push_back(CommandInfo);
            }
        }
    }
}

void PSS_ClientManagerFrame::OnButton_ShowTimeoutCmdClick(wxCommandEvent& event)
{
    //查看全部
    this->ListCtrl_CommandTimeout->DeleteAllItems();

    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    char szCommandID[100]   = {'\0'};
    sprintf_s(szCommand, 100, "%s CommandTimeout -a&", m_pTcpClientConnect->GetKey());
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[10 * 1024] = {'\0'};
    int nRecvLen = 10 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        int nIndex        = 0;
        int nStrLen       = 0;
        int nPos          = 0;
        int nModuleCount  = 0;

        memcpy_s(&nModuleCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);

        for(int i = 0; i < nModuleCount; i++)
        {
            //开始还原数据结构
            _CommandTimeOut objCommandTimeOut;
            memcpy_s(&objCommandTimeOut.nCommandID, sizeof(short), &szRecvBuff[nPos], sizeof(short));
            nPos += sizeof(short);
            sprintf_s(szCommandID, 100, "0x%04x", objCommandTimeOut.nCommandID);

            char szUpdateTime[30] = {'\0'};
            memcpy_s(&objCommandTimeOut.nTime, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);
            struct tm tmDate;
            time_t newRawTime = objCommandTimeOut.nTime;
            localtime_s(&tmDate, &newRawTime);
            sprintf_s(szUpdateTime, 30, "%04d-%02d-%02d %02d:%02d:%02d", tmDate.tm_year + 1900,
                      tmDate.tm_mon + 1,
                      tmDate.tm_mday,
                      tmDate.tm_hour,
                      tmDate.tm_min,
                      tmDate.tm_sec);


            memcpy_s(&objCommandTimeOut.nTimeOutTime, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            this->ListCtrl_CommandTimeout->InsertItem(nIndex, wxString(szCommandID));
            this->ListCtrl_CommandTimeout->SetItem(nIndex, 1, wxString(szUpdateTime));
            this->ListCtrl_CommandTimeout->SetItem(nIndex, 2, wxString::Format(wxT("%d"),objCommandTimeOut.nTimeOutTime));


            nIndex++;
        }
    }

}

void PSS_ClientManagerFrame::OnButton_ClearTimeoutCmdClick(wxCommandEvent& event)
{

    //清除所有超时记录
    this->ListCtrl_CommandTimeout->DeleteAllItems();

    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    char szCommandID[100]   = {'\0'};
    sprintf_s(szCommand, 100, "%s CommandTimeoutclr -a&", m_pTcpClientConnect->GetKey());
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[10 * 1024] = {'\0'};
    int nRecvLen = 10 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        wxMessageBox(wxT(MESSAGE_RESULT_SUCCESS), wxT(MESSAGE_TITLE_SUCCESS), wxOK | wxICON_INFORMATION, this);
        return;
    }

}

void PSS_ClientManagerFrame::OnButton_SaveSmdHistoryClick(wxCommandEvent& event)
{
    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    char szCommandID[100]   = {'\0'};
    sprintf_s(szCommand, 100, "%s CommandDataLog -a&", m_pTcpClientConnect->GetKey());
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[10 * 1024] = {'\0'};
    int nRecvLen = 10 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        wxMessageBox(wxT(MESSAGE_RESULT_SUCCESS), wxT(MESSAGE_TITLE_SUCCESS), wxOK | wxICON_INFORMATION, this);
        return;
    }
}

void PSS_ClientManagerFrame::OnButton_DisconnectClientClick(wxCommandEvent& event)
{


    //断开链接
    int szScoketFD = wxAtoi(this->TextCtrl_ClientID->GetValue());

    if(szScoketFD == 0)
    {
        wxMessageBox(wxT(MESSAGE_INSERT_NULL), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }

    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s CloseClient %d&", m_pTcpClientConnect->GetKey(), szScoketFD);
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[100 * 1024] = {'\0'};
    int nRecvLen = 100 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        int nStrLen       = 0;
        int nPos          = 0;
        int nOPState      = 0;
        memcpy_s(&nOPState, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);

        memcpy_s(&nOPState, sizeof(char), &szRecvBuff[4], sizeof(char));

        if(nOPState == 0)
        {
            wxMessageBox(wxT(MESSAGE_RESULT_SUCCESS), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        }
        else
        {
            wxMessageBox(wxT(MESSAGE_RESULT_FAIL), wxT(MESSAGE_TITLE_SUCCESS), wxOK | wxICON_INFORMATION, this);
        }
    }

}

void PSS_ClientManagerFrame::OnButton_ShowClientClick(wxCommandEvent& event)
{

    //查看TCP
    this->ListCtrl_LogInfo->DeleteAllItems();

    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s ConnectClient -a&", m_pTcpClientConnect->GetKey());
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[100 * 1024] = {'\0'};
    int nRecvLen = 100 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        vecClientConnectInfo objvecClientConnectInfo;

        int nStrLen       = 0;
        int nPos          = 0;
        int nTCPCount     = 0;
        memcpy_s(&nTCPCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);

        for(int i = 0; i < nTCPCount; i++)
        {
            //开始还原数据结构
            _ClientConnectInfo ClientConnectInfo;

            memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
            nPos += sizeof(char);

            memcpy_s(ClientConnectInfo.m_szIP, nStrLen, &szRecvBuff[nPos], nStrLen);
            nPos += nStrLen;
            ClientConnectInfo.m_szIP[nStrLen] = '\0';

            memcpy_s(&ClientConnectInfo.m_nConnectID, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            memcpy_s(&ClientConnectInfo.m_nRecvCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            memcpy_s(&ClientConnectInfo.m_nSendCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            memcpy_s(&ClientConnectInfo.m_nAllRecvSize, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            memcpy_s(&ClientConnectInfo.m_nAllSendSize, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            char szUpdateTime[30] = {'\0'};
            memcpy_s(&ClientConnectInfo.m_nBeginTime, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);
            struct tm tmDate;
            time_t newRawTime = ClientConnectInfo.m_nBeginTime;
            localtime_s(&tmDate, &newRawTime);
            sprintf_s(szUpdateTime, 30, "%04d-%02d-%02d %02d:%02d:%02d", tmDate.tm_year + 1900,
                      tmDate.tm_mon + 1,
                      tmDate.tm_mday,
                      tmDate.tm_hour,
                      tmDate.tm_min,
                      tmDate.tm_sec);

            memcpy_s(&ClientConnectInfo.m_nAliveTime, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            memcpy_s(&ClientConnectInfo.m_nRecvQueueCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            memcpy_s(&ClientConnectInfo.m_nRecvQueueTimeCost, sizeof(long long), &szRecvBuff[nPos], sizeof(long long));
            nPos += sizeof(long long);

            memcpy_s(&ClientConnectInfo.m_nSendQueueTimeCost, sizeof(long long), &szRecvBuff[nPos], sizeof(long long));
            nPos += sizeof(long long);

            //显示在界面上

            this->ListCtrl_LogInfo->InsertItem(i, wxString(ClientConnectInfo.m_szIP));
            this->ListCtrl_LogInfo->SetItem(i, 1, wxString::Format(wxT("%d"), ClientConnectInfo.m_nConnectID));
            this->ListCtrl_LogInfo->SetItem(i, 2, wxString::Format(wxT("%d"), ClientConnectInfo.m_nSendCount));
            this->ListCtrl_LogInfo->SetItem(i, 3, wxString::Format(wxT("%d"), ClientConnectInfo.m_nRecvCount));
            this->ListCtrl_LogInfo->SetItem(i, 4, wxString::Format(wxT("%d"), ClientConnectInfo.m_nAllSendSize));
            this->ListCtrl_LogInfo->SetItem(i, 5, wxString::Format(wxT("%d"), ClientConnectInfo.m_nAllRecvSize));
            this->ListCtrl_LogInfo->SetItem(i, 6, wxString(szUpdateTime));
            this->ListCtrl_LogInfo->SetItem(i, 7, wxString::Format(wxT("%d"), ClientConnectInfo.m_nAliveTime));
            this->ListCtrl_LogInfo->SetItem(i, 8, wxString::Format(wxT("%d"), ClientConnectInfo.m_nRecvQueueCount));

            objvecClientConnectInfo.push_back(ClientConnectInfo);
        }
    }

}

void PSS_ClientManagerFrame::OnButton_SetLogLevelClick(wxCommandEvent& event)
{

    //设置当前日志等级
    int nLevel = wxAtoi(this->TextCtrl_LogLevel->GetValue());

    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s SetLogLevel -l %d&", m_pTcpClientConnect->GetKey(), nLevel);
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[100 * 1024] = {'\0'};
    int nRecvLen = 100 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        int nPos          = 0;
        int nResult       = 0;

        memcpy_s(&nResult, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);

        if(nResult == 0)
        {
            //设置日志等级成功
            wxMessageBox(wxString("设置日志等级成功"), wxT(MESSAGE_TITLE_SUCCESS), wxOK | wxICON_INFORMATION, this);
        }
    }

}

void PSS_ClientManagerFrame::OnButton_ShowCurrLoginfoClick(wxCommandEvent& event)
{
    //查看当前日志项
    this->ListCtrl_LogInfo->DeleteAllItems();

    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s GetLogInfo -a&", m_pTcpClientConnect->GetKey());
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[100 * 1024] = {'\0'};
    int nRecvLen = 100 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        int nStrLen       = 0;
        int nPos          = 0;
        int nTCPCount     = 0;
        int nLogCount     = 0;
        int nCurrLogLevel = 0;


        memcpy_s(&nLogCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);

        memcpy_s(&nCurrLogLevel, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);


        this->TextCtrl_LogLevel->SetValue(wxString::Format(wxT("%d"), nCurrLogLevel));


        for(int i = 0; i < nLogCount; i++)
        {
            int  nLogID = 0;
            char szServerName[300] = {'\0'};
            char szLogName[300]    = {'\0'};
            int  nDisplay          = 0;
            int  nLogLevel         = 0;
            memcpy_s(&nLogID, sizeof(short), &szRecvBuff[nPos], sizeof(short));
            nPos += sizeof(short);

            memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
            nPos += sizeof(char);

            memcpy_s(szServerName, nStrLen, &szRecvBuff[nPos], nStrLen);
            nPos += nStrLen;
            szServerName[nStrLen] = '\0';

            memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
            nPos += sizeof(char);

            memcpy_s(szLogName, nStrLen, &szRecvBuff[nPos], nStrLen);
            nPos += nStrLen;
            szLogName[nStrLen] = '\0';

            memcpy_s(&nDisplay, sizeof(char), &szRecvBuff[nPos], sizeof(char));
            nPos += sizeof(char);

            //显示在界面上
            this->ListCtrl_LogInfo->InsertItem(i, wxString::Format(wxT("%d"), nLogID));
            this->ListCtrl_LogInfo->SetItem(i, 1, wxString(szServerName));
            this->ListCtrl_LogInfo->SetItem(i, 2, wxString(szLogName));

            if(nDisplay == 0)
            {
                this->ListCtrl_LogInfo->SetItem(i, 3, wxString("写文件"));
            }
            else
            {
                this->ListCtrl_LogInfo->SetItem(i, 3, wxString("写屏幕"));
            }

            memcpy_s(&nLogLevel, sizeof(short), &szRecvBuff[nPos], sizeof(short));
            nPos += sizeof(short);
            this->ListCtrl_LogInfo->SetItem(i, 4, wxString::Format(wxT("%d"), nLogLevel));
        }
    }


}

void PSS_ClientManagerFrame::OnButton_StartMonitorClick(wxCommandEvent& event)
{

    //开始监控
    int nCheckThreadCount = wxAtoi(this->TextCtrl_ThreadCount->GetValue());
    int nTimeInterval = wxAtoi(this->TextCtrl_CheckTime->GetValue());

    if(m_blTimeRun == true)
    {
        //如果正在监控，先停止上一次的监控
        this->Timer_WorkThread.Stop();
    }

    //开始监控的相关准备工作
    if(this->RadioBox_MonitorServer->GetSelection()    ==0)
    {
        //当前监控
        m_vecCheckServerInfo.clear();

        _CheckServerInfo* pCheckServerInfo = new _CheckServerInfo();

        pCheckServerInfo->m_objTcpClientConnect.Init(m_pTcpClientConnect->GetServerIP(), m_pTcpClientConnect->GetServerPort(), m_pTcpClientConnect->GetServerKey());
        pCheckServerInfo->m_nCheckThreadCount = nCheckThreadCount;
        pCheckServerInfo->m_pWorkThreadInfo   = new _WorkThreadInfo[nCheckThreadCount];
        m_vecCheckServerInfo.push_back(pCheckServerInfo);
    }
    else
    {
        //群体监控

    }

    this->ListCtrl_CheckLog->DeleteAllItems();

    wxDateTime  tm  =wxDateTime::Now();

    this->Timer_WorkThread.Start(nTimeInterval * 1000);
    m_blTimeRun = true;

    this->ListCtrl_CheckLog->InsertItem(this->ListCtrl_CheckLog->GetItemCount(),tm.Format(wxString("[%Y-%m-%d %X]"))+ wxT("开始监控..."));
    this->ListCtrl_CheckLog->Refresh();

}

void PSS_ClientManagerFrame::OnButton_StopMonitorClick(wxCommandEvent& event)
{
    //停止监控
    this->Timer_WorkThread.Stop();
    m_blTimeRun = false;

    ClearCheckServerInfo();


    wxDateTime  tm  =wxDateTime::Now();
    this->ListCtrl_CheckLog->InsertItem(this->ListCtrl_CheckLog->GetItemCount(),tm.Format(wxString("[%Y-%m-%d %X]"))+ wxT("停止监控..."));

    int nCount = this->ListCtrl_CheckLog->GetItemCount();

    if (nCount > 0)
    {
        this->ListCtrl_CheckLog->SetItemState(nCount - 1, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
    }

}

void PSS_ClientManagerFrame::OnTimer_WorkThreadTrigger(wxTimerEvent& event)
{


    for(int i = 0; i < (int)m_vecCheckServerInfo.size(); i++)
    {
        CheckWorkThread(m_vecCheckServerInfo[i]);
    }

}




void PSS_ClientManagerFrame::CheckWorkThread(_CheckServerInfo* pCheckServerInfo)
{
    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s WorkThreadState -s&", m_pTcpClientConnect->GetKey());
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[100 * 1024] = {'\0'};
    int nRecvLen = 100 * 1024;
    bool blState = pCheckServerInfo->m_objTcpClientConnect.SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        vecWorkThreadInfo objvecWorkThreadInfo;

        int nStrLen       = 0;
        int nPos          = 0;
        int nThreadCount  = 0;
        memcpy_s(&nThreadCount, sizeof(char), &szRecvBuff[nPos], sizeof(char));
        nPos += sizeof(char);

        if(nThreadCount != pCheckServerInfo->m_nCheckThreadCount)
        {
            char szError[1024]     = {'\0'};
            wchar_t szzError[1024] = {'\0'};

            sprintf_s(szError,1024, "[%s:%d]监控线程数不符，当前服务器线程数[%d]，当前监控线程数[%d]", pCheckServerInfo->m_objTcpClientConnect.GetServerIP(), pCheckServerInfo->m_objTcpClientConnect.GetServerPort(), nThreadCount, pCheckServerInfo->m_nCheckThreadCount);


            this->ListCtrl_CheckLog->InsertItem(this->ListCtrl_CheckLog->GetItemCount(),wxString(szError));

            int nCount = this->ListCtrl_CheckLog->GetItemCount();

            if (nCount > 0)
            {
                this->ListCtrl_CheckLog->SetItemState(nCount - 1, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
            }

            BeepAlarm();

            return;
        }

        //接收到的所有工作线程信息
        for(int i = 0; i < nThreadCount; i++)
        {
            //开始还原数据结构
            _WorkThreadInfo WorkThreadInfo;
            sprintf_s(WorkThreadInfo.m_szThreadName, 50, "工作线程");

            memcpy_s(&WorkThreadInfo.m_nThreadID, sizeof(char), &szRecvBuff[nPos], sizeof(char));
            nPos += sizeof(char);

            char szUpdateTime[30] = {'\0'};
            memcpy_s(&WorkThreadInfo.m_nUpdateTime, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);
            struct tm tmDate;
            time_t newRawTime = WorkThreadInfo.m_nUpdateTime;
            localtime_s(&tmDate, &newRawTime);
            sprintf_s(szUpdateTime, 30, "%04d-%02d-%02d %02d:%02d:%02d", tmDate.tm_year + 1900,
                      tmDate.tm_mon + 1,
                      tmDate.tm_mday,
                      tmDate.tm_hour,
                      tmDate.tm_min,
                      tmDate.tm_sec);

            char szCreateTime[30] = {'\0'};
            memcpy_s(&WorkThreadInfo.m_nCreateTime, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);
            struct tm tmDate1;
            time_t newRawTime1 = WorkThreadInfo.m_nCreateTime;
            localtime_s(&tmDate1, &newRawTime1);
            sprintf_s(szCreateTime, 30, "%04d-%02d-%02d %02d:%02d:%02d", tmDate.tm_year + 1900,
                      tmDate1.tm_mon + 1,
                      tmDate1.tm_mday,
                      tmDate1.tm_hour,
                      tmDate1.tm_min,
                      tmDate1.tm_sec);

            memcpy_s(&WorkThreadInfo.m_nState, sizeof(char), &szRecvBuff[nPos], sizeof(char));
            nPos += sizeof(char);

            memcpy_s(&WorkThreadInfo.m_nRecvPacketCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            memcpy_s(&WorkThreadInfo.m_nCommandID, sizeof(short), &szRecvBuff[nPos], sizeof(short));
            nPos += sizeof(short);

            memcpy_s(&WorkThreadInfo.m_nPacketTime, sizeof(short), &szRecvBuff[nPos], sizeof(short));
            nPos += sizeof(short);

            memcpy_s(&WorkThreadInfo.m_nCurrPacketCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            objvecWorkThreadInfo.push_back(WorkThreadInfo);
        }

        for(int i = 0; i < pCheckServerInfo->m_nCheckThreadCount; i++)
        {
            if(pCheckServerInfo->m_pWorkThreadInfo[i].m_nCreateTime == 0)
            {
                //第一次调用，只拷贝
                pCheckServerInfo->m_pWorkThreadInfo[i] = objvecWorkThreadInfo[i];
            }
            else
            {
                //如果是已有数据，进行比较判定
                if(pCheckServerInfo->m_pWorkThreadInfo[i].m_nCreateTime != objvecWorkThreadInfo[i].m_nCreateTime)
                {
                    char szError[1024]     = {'\0'};
                    char szThreadTime[100] = {'\0'};
                    char szCurrTime[100]   = {'\0'};
                    wchar_t szzError[1024] = {'\0'};

                    struct tm tmDate;
                    time_t newRawTime = pCheckServerInfo->m_pWorkThreadInfo[i].m_nCreateTime;
                    localtime_s(&tmDate, &newRawTime);
                    sprintf_s(szThreadTime, 30, "%04d-%02d-%02d %02d:%02d:%02d", tmDate.tm_year + 1900,
                              tmDate.tm_mon + 1,
                              tmDate.tm_mday,
                              tmDate.tm_hour,
                              tmDate.tm_min,
                              tmDate.tm_sec);

                    struct tm tmDate1;
                    time_t newRawTime1 = objvecWorkThreadInfo[i].m_nCreateTime;
                    localtime_s(&tmDate1, &newRawTime1);
                    sprintf_s(szCurrTime, 30, "%04d-%02d-%02d %02d:%02d:%02d", tmDate1.tm_year + 1900,
                              tmDate1.tm_mon + 1,
                              tmDate1.tm_mday,
                              tmDate1.tm_hour,
                              tmDate1.tm_min,
                              tmDate1.tm_sec);

                    sprintf_s(szError,1024, "[%s:%d]原线程时间是[%s], 当前线程重启时间[%s]", pCheckServerInfo->m_objTcpClientConnect.GetServerIP(), pCheckServerInfo->m_objTcpClientConnect.GetServerPort(), szThreadTime, szCurrTime);

                    this->ListCtrl_CheckLog->InsertItem(this->ListCtrl_CheckLog->GetItemCount(),wxString(szError));

                    int nCount = this->ListCtrl_CheckLog->GetItemCount();

                    if (nCount > 0)
                    {
                        this->ListCtrl_CheckLog->SetItemState(nCount - 1, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
                    }


                    BeepAlarm();
                }
            }
        }
    }
}


void PSS_ClientManagerFrame::BeepAlarm()
{

    if(this->CheckBox_Voice->GetValue() == true)
    {
        for(int i = 0; i < 5; i++)
        {
            //Beep(1000, 1000);
        }
    }
}



void PSS_ClientManagerFrame::OnButton_ShowServerLinkInfoClick(wxCommandEvent& event)
{

    //查看TCP
    this->ListCtrl_ServerConnect->DeleteAllItems();

    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s ServerConnectTCP -a&", m_pTcpClientConnect->GetKey());
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[100 * 1024] = {'\0'};
    int nRecvLen = 100 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        vecClientConnectInfo objvecClientConnectInfo;

        int nStrLen       = 0;
        int nPos          = 0;
        int nTCPCount     = 0;
        int nState        = 0;
        memcpy_s(&nTCPCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);

        for(int i = 0; i < nTCPCount; i++)
        {
            //开始还原数据结构
            _ClientConnectInfo ClientConnectInfo;

            memcpy_s(&nState, sizeof(int), &szRecvBuff[nPos], sizeof(char));
            nPos += sizeof(char);

            memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
            nPos += sizeof(char);

            memcpy_s(ClientConnectInfo.m_szIP, nStrLen, &szRecvBuff[nPos], nStrLen);
            nPos += nStrLen;
            ClientConnectInfo.m_szIP[nStrLen] = '\0';

            memcpy_s(&ClientConnectInfo.m_nConnectID, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            memcpy_s(&ClientConnectInfo.m_nRecvCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            memcpy_s(&ClientConnectInfo.m_nSendCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            memcpy_s(&ClientConnectInfo.m_nAllRecvSize, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            memcpy_s(&ClientConnectInfo.m_nAllSendSize, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            char szUpdateTime[30] = {'\0'};
            memcpy_s(&ClientConnectInfo.m_nBeginTime, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);
            struct tm tmDate;
            time_t newRawTime = ClientConnectInfo.m_nBeginTime;
            localtime_s(&tmDate, &newRawTime);
            sprintf_s(szUpdateTime, 30, "%04d-%02d-%02d %02d:%02d:%02d", tmDate.tm_year + 1900,
                      tmDate.tm_mon + 1,
                      tmDate.tm_mday,
                      tmDate.tm_hour,
                      tmDate.tm_min,
                      tmDate.tm_sec);

            memcpy_s(&ClientConnectInfo.m_nAliveTime, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            memcpy_s(&ClientConnectInfo.m_nRecvQueueCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            //memcpy_s(&ClientConnectInfo.m_nRecvQueueTimeCost, sizeof(long long), &szRecvBuff[nPos], sizeof(long long));
            nPos += sizeof(long long);

            //memcpy_s(&ClientConnectInfo.m_nSendQueueTimeCost, sizeof(long long), &szRecvBuff[nPos], sizeof(long long));
            nPos += sizeof(long long);

            //显示在界面上
            this->ListCtrl_ServerConnect->InsertItem(i, wxString::Format(_T("%d"), ClientConnectInfo.m_nConnectID));
            this->ListCtrl_ServerConnect->SetItem(i, 1, wxString(ClientConnectInfo.m_szIP));

            if(nState == 0)
            {
                this->ListCtrl_ServerConnect->SetItem(i, 2, wxT("链接已存在"));
            }
            else
            {
                this->ListCtrl_ServerConnect->SetItem(i, 2, wxT("链接不存在"));
            }

            this->ListCtrl_ServerConnect->SetItem(i, 3, wxString::Format(_T("%d"), ClientConnectInfo.m_nSendCount));
            this->ListCtrl_ServerConnect->SetItem(i, 4, wxString::Format(_T("%d"), ClientConnectInfo.m_nRecvCount));
            this->ListCtrl_ServerConnect->SetItem(i, 5, wxString::Format(_T("%d"), ClientConnectInfo.m_nAllSendSize));
            this->ListCtrl_ServerConnect->SetItem(i, 6, wxString::Format(_T("%d"), ClientConnectInfo.m_nAllRecvSize));
            this->ListCtrl_ServerConnect->SetItem(i, 7, wxString(szUpdateTime));
            this->ListCtrl_ServerConnect->SetItem(i, 8, wxString::Format(_T("%d"), ClientConnectInfo.m_nAliveTime));

            objvecClientConnectInfo.push_back(ClientConnectInfo);
        }
    }

}

void PSS_ClientManagerFrame::OnButton_ReconnectServerClick(wxCommandEvent& event)
{

    std::string    strServerID    =std::string(this->TextCtrl_ServerID->GetValue());

    this->ListCtrl_ServerConnect->DeleteAllItems();

    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s ReConnectServer -s %s&", m_pTcpClientConnect->GetKey(), strServerID.c_str());
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[100 * 1024] = {'\0'};
    int nRecvLen = 100 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        vecClientConnectInfo objvecClientConnectInfo;

        int nStrLen       = 0;
        int nPos          = 0;
        int nTCPCount     = 0;
        int nState        = 0;
        memcpy_s(&nTCPCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);

        for(int i = 0; i < nTCPCount; i++)
        {
            //开始还原数据结构
            _ClientConnectInfo ClientConnectInfo;

            memcpy_s(&nState, sizeof(int), &szRecvBuff[nPos], sizeof(char));
            nPos += sizeof(char);

            memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
            nPos += sizeof(char);

            memcpy_s(ClientConnectInfo.m_szIP, nStrLen, &szRecvBuff[nPos], nStrLen);
            nPos += nStrLen;
            ClientConnectInfo.m_szIP[nStrLen] = '\0';

            memcpy_s(&ClientConnectInfo.m_nConnectID, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            memcpy_s(&ClientConnectInfo.m_nRecvCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            memcpy_s(&ClientConnectInfo.m_nSendCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            memcpy_s(&ClientConnectInfo.m_nAllRecvSize, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            memcpy_s(&ClientConnectInfo.m_nAllSendSize, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            char szUpdateTime[30] = {'\0'};
            memcpy_s(&ClientConnectInfo.m_nBeginTime, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);
            struct tm tmDate;
            time_t newRawTime = ClientConnectInfo.m_nBeginTime;
            localtime_s(&tmDate, &newRawTime);
            sprintf_s(szUpdateTime, 30, "%04d-%02d-%02d %02d:%02d:%02d", tmDate.tm_year + 1900,
                      tmDate.tm_mon + 1,
                      tmDate.tm_mday,
                      tmDate.tm_hour,
                      tmDate.tm_min,
                      tmDate.tm_sec);

            memcpy_s(&ClientConnectInfo.m_nAliveTime, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            memcpy_s(&ClientConnectInfo.m_nRecvQueueCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            //memcpy_s(&ClientConnectInfo.m_nRecvQueueTimeCost, sizeof(long long), &szRecvBuff[nPos], sizeof(long long));
            nPos += sizeof(long long);

            //memcpy_s(&ClientConnectInfo.m_nSendQueueTimeCost, sizeof(long long), &szRecvBuff[nPos], sizeof(long long));
            nPos += sizeof(long long);

            //显示在界面上
            this->ListCtrl_ServerConnect->InsertItem(i, wxString::Format(_T("%d"), ClientConnectInfo.m_nConnectID));
            this->ListCtrl_ServerConnect->SetItem(i, 1, wxString(ClientConnectInfo.m_szIP));

            if(nState == 0)
            {
                this->ListCtrl_ServerConnect->SetItem(i, 2, wxT("链接已存在"));
            }
            else
            {
                this->ListCtrl_ServerConnect->SetItem(i, 2, wxT("链接不存在"));
            }

            this->ListCtrl_ServerConnect->SetItem(i, 3, wxString::Format(_T("%d"), ClientConnectInfo.m_nSendCount));
            this->ListCtrl_ServerConnect->SetItem(i, 4, wxString::Format(_T("%d"), ClientConnectInfo.m_nRecvCount));
            this->ListCtrl_ServerConnect->SetItem(i, 5, wxString::Format(_T("%d"), ClientConnectInfo.m_nAllSendSize));
            this->ListCtrl_ServerConnect->SetItem(i, 6, wxString::Format(_T("%d"), ClientConnectInfo.m_nAllRecvSize));
            this->ListCtrl_ServerConnect->SetItem(i, 7, wxString(szUpdateTime));
            this->ListCtrl_ServerConnect->SetItem(i, 8, wxString::Format(_T("%d"), ClientConnectInfo.m_nAliveTime));


            objvecClientConnectInfo.push_back(ClientConnectInfo);
        }
    }

    wxMessageBox(wxT(MESSAGE_RESULT_SUCCESS), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);

}

void PSS_ClientManagerFrame::OnButton_ShowPSSLisenInfoClick(wxCommandEvent& event)
{
    //查看所有已有的监听端口信息
    this->ListCtrl_ServerLisen->DeleteAllItems();

    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s ShowListen -a&", m_pTcpClientConnect->GetKey());
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[100 * 1024] = {'\0'};
    int nRecvLen = 100 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        int nStrLen       = 0;
        int nPos          = 0;
        int nTCPCount     = 0;
        int nState        = 0;
        memcpy_s(&nTCPCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);

        for(int i = 0; i < nTCPCount; i++)
        {
            //开始还原数据结构
            char szListenIP[30] = {'\0'};
            int  nIPLen         = 0;
            int  nListenPort    = 0;

            memcpy_s(&nIPLen, sizeof(int), &szRecvBuff[nPos], sizeof(char));
            nPos += sizeof(char);

            memcpy_s(szListenIP, 30, &szRecvBuff[nPos], sizeof(char)*nIPLen);
            nPos += sizeof(char)*nIPLen;

            memcpy_s(&nListenPort, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            //显示在界面上
            this->ListCtrl_ServerLisen->InsertItem(i, wxString(szListenIP));
            this->ListCtrl_ServerLisen->SetItem(i, 1, wxString::Format(wxT("%d"), nListenPort));
        }
    }

    wxMessageBox(wxT(MESSAGE_RESULT_SUCCESS), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);

}

void PSS_ClientManagerFrame::OnButton_InsertNewMonitorClick(wxCommandEvent& event)
{

    //添加新监控
    std::string    strIP    =std::string(this->TextCtrl_LisenIP->GetValue());
    int nPort = wxAtoi(this->TextCtrl_LisenPort->GetValue());

    int nType = 1;

    if(this->ComboBox_ListenType->GetSelection() == 0)
    {
        nType = 1;
    }
    else
    {
        nType = 2;
    }

    int nPacketParseID = wxAtoi(this->TextCtrl_PacketParseID->GetValue());

    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s AddListen -i %s -p %d -t %d -n %d&", m_pTcpClientConnect->GetKey(), strIP.c_str(), nPort, nType, nPacketParseID);
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[100 * 1024] = {'\0'};
    int nRecvLen = 100 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        int nStrLen       = 0;
        int nPos          = 0;
        int nResult       = 0;
        int nState        = 0;
        memcpy_s(&nResult, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);

        if(nResult == 1)
        {
            wxMessageBox(wxT(MESSAGE_RESULT_FAIL), wxT(MESSAGE_TITLE_SUCCESS), wxOK | wxICON_INFORMATION, this);
        }
        else
        {
            wxMessageBox(wxT(MESSAGE_RESULT_SUCCESS), wxT(MESSAGE_TITLE_SUCCESS), wxOK | wxICON_INFORMATION, this);
        }

    }

}

void PSS_ClientManagerFrame::OnButton_DeleteOldMonitorClick(wxCommandEvent& event)
{

    //删除一个已有监控
    std::string    strIP    =std::string(this->TextCtrl_LisenIP->GetValue());
    int nPort = wxAtoi(this->TextCtrl_LisenPort->GetValue());

    int nType = 1;

    if(this->ComboBox_ListenType->GetSelection() == 0)
    {
        nType = 1;
    }
    else
    {
        nType = 2;
    }


    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s DelListen -i %s -p %d -t %d &", m_pTcpClientConnect->GetKey(), strIP.c_str(), nPort, nType);
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[100 * 1024] = {'\0'};
    int nRecvLen = 100 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        int nStrLen       = 0;
        int nPos          = 0;
        int nResult       = 0;
        int nState        = 0;
        memcpy_s(&nResult, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);

        if(nResult == 1)
        {
            wxMessageBox(wxT(MESSAGE_RESULT_FAIL), wxT(MESSAGE_TITLE_SUCCESS), wxOK | wxICON_INFORMATION, this);
        }
        else
        {
            wxMessageBox(wxT(MESSAGE_RESULT_SUCCESS), wxT(MESSAGE_TITLE_SUCCESS), wxOK | wxICON_INFORMATION, this);
        }

    }

}

void PSS_ClientManagerFrame::OnButton_ShowWorkThreadStatClick(wxCommandEvent& event)
{
    //查看工作线程信息
    this->ListCtrl_WorkThreadInfo->DeleteAllItems();

    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s WorkThreadState -s&", m_pTcpClientConnect->GetKey());
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[100 * 1024] = {'\0'};
    int nRecvLen = 100 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        vecWorkThreadInfo objvecWorkThreadInfo;

        int nStrLen       = 0;
        int nPos          = 0;
        int nThreadCount  = 0;
        memcpy_s(&nThreadCount, sizeof(char), &szRecvBuff[nPos], sizeof(char));
        nPos += sizeof(char);

        for(int i = 0; i < nThreadCount; i++)
        {
            //开始还原数据结构
            _WorkThreadInfo WorkThreadInfo;
            sprintf_s(WorkThreadInfo.m_szThreadName, 50, "工作线程");

            memcpy_s(&WorkThreadInfo.m_nThreadID, sizeof(char), &szRecvBuff[nPos], sizeof(char));
            nPos += sizeof(char);

            char szUpdateTime[30] = {'\0'};
            memcpy_s(&WorkThreadInfo.m_nUpdateTime, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);
            struct tm tmDate;
            time_t newRawTime = WorkThreadInfo.m_nUpdateTime;
            localtime_s(&tmDate, &newRawTime);
            sprintf_s(szUpdateTime, 30, "%04d-%02d-%02d %02d:%02d:%02d", tmDate.tm_year + 1900,
                      tmDate.tm_mon + 1,
                      tmDate.tm_mday,
                      tmDate.tm_hour,
                      tmDate.tm_min,
                      tmDate.tm_sec);

            char szCreateTime[30] = {'\0'};
            memcpy_s(&WorkThreadInfo.m_nCreateTime, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);
            struct tm tmDate1;
            time_t newRawTime1 = WorkThreadInfo.m_nCreateTime;
            localtime_s(&tmDate1, &newRawTime1);
            sprintf_s(szCreateTime, 30, "%04d-%02d-%02d %02d:%02d:%02d", tmDate.tm_year + 1900,
                      tmDate1.tm_mon + 1,
                      tmDate1.tm_mday,
                      tmDate1.tm_hour,
                      tmDate1.tm_min,
                      tmDate1.tm_sec);

            memcpy_s(&WorkThreadInfo.m_nState, sizeof(char), &szRecvBuff[nPos], sizeof(char));
            nPos += sizeof(char);

            memcpy_s(&WorkThreadInfo.m_nRecvPacketCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            memcpy_s(&WorkThreadInfo.m_nCommandID, sizeof(short), &szRecvBuff[nPos], sizeof(short));
            nPos += sizeof(short);

            memcpy_s(&WorkThreadInfo.m_nPacketTime, sizeof(short), &szRecvBuff[nPos], sizeof(short));
            nPos += sizeof(short);

            memcpy_s(&WorkThreadInfo.m_nCurrPacketCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            //显示在界面上

            this->ListCtrl_WorkThreadInfo->InsertItem(i, wxT("工作线程"));
            this->ListCtrl_WorkThreadInfo->SetItem(i, 1,wxString::Format(wxT("%d"),WorkThreadInfo.m_nThreadID) );
            this->ListCtrl_WorkThreadInfo->SetItem(i, 2,wxString(szCreateTime) );
            this->ListCtrl_WorkThreadInfo->SetItem(i, 3,wxString(szUpdateTime) );
            this->ListCtrl_WorkThreadInfo->SetItem(i, 4,wxString::Format(wxT("%d"),WorkThreadInfo.m_nRecvPacketCount) );
            this->ListCtrl_WorkThreadInfo->SetItem(i, 5,wxString::Format(wxT("%d"),WorkThreadInfo.m_nCommandID) );
            this->ListCtrl_WorkThreadInfo->SetItem(i, 6,wxString::Format(wxT("%d"),WorkThreadInfo.m_nPacketTime) );
            this->ListCtrl_WorkThreadInfo->SetItem(i, 7,wxString::Format(wxT("%d"),WorkThreadInfo.m_nCurrPacketCount) );


            objvecWorkThreadInfo.push_back(WorkThreadInfo);
        }

    }
}


void PSS_ClientManagerFrame::OnButton_InsertForbidenClick(wxCommandEvent& event)
{


    //添加封禁
    std::string    strIP    =std::string(this->TextCtrl_ForbidenIP->GetValue());
    std::string    strSeconds    =std::string(this->TextCtrl_ForbidenSeconds->GetValue());
    int nType = RadioBoxm_TimeForbiden->GetSelection();


    if(strIP.length() == 0)
    {
        wxMessageBox(wxT(MESSAGE_INSERT_NULL), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }


    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s ForbiddenIP -c %s -t %d -s %s &", m_pTcpClientConnect->GetKey(), strIP.c_str(), nType, strSeconds.c_str());
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[10 * 1024] = {'\0'};
    int nRecvLen = 10 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
    }
    else
    {
        int nStrLen      = 0;
        int nPos         = 0;
        int nResult      = 0;
        memcpy_s(&nResult, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);

        if(nResult == 0)
        {
            wxMessageBox(wxT(MESSAGE_RESULT_SUCCESS), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        }
        else
        {
            wxMessageBox(wxT(MESSAGE_RESULT_FAIL), wxT(MESSAGE_TITLE_SUCCESS), wxOK | wxICON_INFORMATION, this);
            this->OnButton_ShowForbidenClick(event);
        }
    }

}

void PSS_ClientManagerFrame::OnButton_ReleaseForbidenClick(wxCommandEvent& event)
{
    //解除封禁
    std::string    strIP    =std::string(this->TextCtrl_ForbidenIP->GetValue());

    if(strIP.length() == 0)
    {
        wxMessageBox(wxT(MESSAGE_INSERT_NULL), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }


    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s LiftedIP %s&", m_pTcpClientConnect->GetKey(), strIP.c_str());
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[10 * 1024] = {'\0'};
    int nRecvLen = 10 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
    }
    else
    {
        int nStrLen      = 0;
        int nPos         = 0;
        int nResult      = 0;
        memcpy_s(&nResult, sizeof(char), &szRecvBuff[nPos], sizeof(char));
        nPos += sizeof(char);

        if(nResult == 0)
        {
            wxMessageBox(wxT(MESSAGE_RESULT_SUCCESS), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        }
        else
        {
            wxMessageBox(wxT(MESSAGE_RESULT_FAIL), wxT(MESSAGE_TITLE_SUCCESS), wxOK | wxICON_INFORMATION, this);
            this->OnButton_ShowForbidenClick(event);
        }
    }

}

void PSS_ClientManagerFrame::OnButton_ShowForbidenClick(wxCommandEvent& event)
{

    //查看列表
    this->ListCtrl_ForbidenList->DeleteAllItems();

    vecForbiddenIP objvecForbiddenIP;

    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s ShowForbiddenIP -a&", m_pTcpClientConnect->GetKey());
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[10 * 1024] = {'\0'};
    int nRecvLen = 10 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        int nStrLen        = 0;
        int nPos           = 0;
        int nForbidenCount = 0;
        memcpy_s(&nForbidenCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);

        for(int i = 0; i < nForbidenCount; i++)
        {
            //开始还原数据结构
            _ForbiddenIP ForbiddenIP;

            memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
            nPos += sizeof(char);

            memcpy_s(ForbiddenIP.m_szIP, nStrLen, &szRecvBuff[nPos], nStrLen);
            nPos += nStrLen;
            ForbiddenIP.m_szIP[nStrLen] = '\0';

            memcpy_s(&ForbiddenIP.m_nType, sizeof(char), &szRecvBuff[nPos], sizeof(char));
            nPos += sizeof(char);

            char szUpdateTime[30] = {'\0'};
            memcpy_s(&ForbiddenIP.m_nBeginTime, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);
            struct tm tmDate;
            time_t newRawTime = ForbiddenIP.m_nBeginTime;
            localtime_s(&tmDate, &newRawTime);
            sprintf_s(szUpdateTime, 30, "%04d-%02d-%02d %02d:%02d:%02d", tmDate.tm_year + 1900,
                      tmDate.tm_mon + 1,
                      tmDate.tm_mday,
                      tmDate.tm_hour,
                      tmDate.tm_min,
                      tmDate.tm_sec);

            memcpy_s(&ForbiddenIP.m_nSecond, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            //显示在桌面上
            wxString strType;

            if(ForbiddenIP.m_nType == 0)
            {
                strType = wxT("永久封禁");
            }
            else
            {
                strType = wxT("时段封禁");
            }

            this->ListCtrl_ForbidenList->InsertItem(i, wxString(ForbiddenIP.m_szIP));
            this->ListCtrl_ForbidenList->SetItem(i, 1, strType);
            this->ListCtrl_ForbidenList->SetItem(i, 2, wxString(szUpdateTime));
            this->ListCtrl_ForbidenList->SetItem(i, 3, wxString::Format(wxT("%d"),ForbiddenIP.m_nSecond));

            objvecForbiddenIP.push_back(ForbiddenIP);
        }
    }


}

void PSS_ClientManagerFrame::OnButton_SearchNicknameClick(wxCommandEvent& event)
{


    //查询别名
    this->ListCtrl_NickInfo->DeleteAllItems();

    std::string    strNickName    =std::string(this->TextCtrl_NickName->GetValue());


    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s GetNickNameInfo -n %s&", m_pTcpClientConnect->GetKey(), strNickName.c_str());
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[10 * 1024] = {'\0'};
    int nRecvLen = 10 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        int nStrLen        = 0;
        int nPos           = 0;
        int nForbidenCount = 0;
        memcpy_s(&nForbidenCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);

        for(int i = 0; i < nForbidenCount; i++)
        {
            //开始还原数据结构
            _ClientNameInfo ClientNameInfo;

            memcpy_s(&ClientNameInfo.m_nConnectID, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
            nPos += sizeof(char);

            memcpy_s(ClientNameInfo.m_szClientIP, nStrLen, &szRecvBuff[nPos], nStrLen);
            nPos += nStrLen;
            ClientNameInfo.m_szClientIP[nStrLen] = '\0';

            memcpy_s(&ClientNameInfo.m_nPort, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
            nPos += sizeof(char);

            memcpy_s(ClientNameInfo.m_szName, nStrLen, &szRecvBuff[nPos], nStrLen);
            nPos += nStrLen;
            ClientNameInfo.m_szName[nStrLen] = '\0';

            memcpy_s(&ClientNameInfo.m_nLog, sizeof(char), &szRecvBuff[nPos], sizeof(char));
            nPos += sizeof(char);

            //显示在桌面上
            this->ListCtrl_NickInfo->InsertItem(i, wxString::Format(wxT("%d"),ClientNameInfo.m_nConnectID));
            this->ListCtrl_NickInfo->SetItem(i, 1, wxString(ClientNameInfo.m_szName));
            this->ListCtrl_NickInfo->SetItem(i, 2, wxString(ClientNameInfo.m_szClientIP));
            this->ListCtrl_NickInfo->SetItem(i, 3, wxString::Format(wxT("%d"),ClientNameInfo.m_nPort));

            if(ClientNameInfo.m_nLog == 0)
            {
                this->ListCtrl_NickInfo->SetItem(i, 4, wxT("未开启"));
            }
            else
            {
                this->ListCtrl_NickInfo->SetItem(i, 4, wxT("开启"));
            }
        }
    }

}


bool PSS_ClientManagerFrame::SendSetLog( int nConnectID, bool blFlag )
{
    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    int  nResult            = 0;

    if(blFlag == false)
    {
        sprintf_s(szCommand, 100, "%s SetConnectLog -n %d -f 0 &", m_pTcpClientConnect->GetKey(), nConnectID);
    }
    else
    {
        sprintf_s(szCommand, 100, "%s SetConnectLog -n %d -f 1 &", m_pTcpClientConnect->GetKey(), nConnectID);
    }

    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[10 * 1024] = {'\0'};
    int nRecvLen = 10 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return false;
    }
    else
    {
        int nStrLen        = 0;
        int nPos           = 0;
        int nForbidenCount = 0;
        memcpy_s(&nResult, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);

        wxMessageBox(wxT(MESSAGE_RESULT_SUCCESS), wxT(MESSAGE_TITLE_SUCCESS), wxOK | wxICON_INFORMATION, this);
        return true;
    }
}



void PSS_ClientManagerFrame::OnButton_OpenLogClick(wxCommandEvent& event)
{

    //打开日志
    int nConnectID = wxAtoi(this->TextCtrl_ConnectID->GetValue());
    SendSetLog(nConnectID, true);

}

void PSS_ClientManagerFrame::OnButton_CloselogClick(wxCommandEvent& event)
{
    //关闭日志
    int nConnectID = wxAtoi(this->TextCtrl_ConnectID->GetValue());
    SendSetLog(nConnectID, false);
}

void PSS_ClientManagerFrame::OnButton41Click(wxCommandEvent& event)
{
}

void PSS_ClientManagerFrame::OnButton_GetTrackSourceIPClick(wxCommandEvent& event)
{
    //根据ConnectID获得相关的数据

    //得到ConnectID
    int nConnectID = wxAtoi(this->TextCtr_TrackConnectID->GetValue());


    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s GetConnectIPInfo -s %d&", m_pTcpClientConnect->GetKey(), nConnectID);
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[10 * 1024] = {'\0'};
    int nRecvLen = 10 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
    }
    else
    {
        int nStrLen      = 0;
        int nPos         = 0;
        int nResult      = 0;
        memcpy_s(&nResult, sizeof(short), &szRecvBuff[nPos], sizeof(short));
        nPos += sizeof(short);

        if(nResult == 1)
        {
            this->TextCtrl_IPInfo->SetValue(wxT(""));
            wxMessageBox(wxString("当前ConnectID无连接信息。"), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        }
        else
        {
            char szIP[20]         = {'\0'};
            int  nPort            = 0;

            memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
            nPos += sizeof(char);

            //获得IP地址
            memcpy_s(szIP, nStrLen, &szRecvBuff[nPos], nStrLen);
            nPos += nStrLen;
            szIP[nStrLen] = '\0';

            //得到端口
            memcpy_s(&nPort, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            this->TextCtrl_IPInfo->SetValue(wxString::Format(wxT("%s:%d"), szIP, nPort));
        }

    }


}

void PSS_ClientManagerFrame::OnButton_SetTrackIPClick(wxCommandEvent& event)
{

    //设置IP
    std::string    strIP   =std::string(this->TextCtr_TrackIP->GetValue());
    std::string    strIPCount   =std::string(this->TextCtrl_TrackIPCount->GetValue());


    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s SetTrackIP -i %s -c %s &", m_pTcpClientConnect->GetKey(), strIP.c_str(), strIPCount.c_str());
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[10 * 1024] = {'\0'};
    int nRecvLen = 10 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
    }
    else
    {
        int nStrLen      = 0;
        int nPos         = 0;
        int nResult      = 0;
        memcpy_s(&nResult, sizeof(char), &szRecvBuff[nPos], sizeof(char));
        nPos += sizeof(char);

        if(nResult == 0)
        {
            wxMessageBox(wxT(MESSAGE_RESULT_SUCCESS), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        }
        else
        {
            wxMessageBox(wxT(MESSAGE_RESULT_FAIL), wxT(MESSAGE_TITLE_SUCCESS), wxOK | wxICON_INFORMATION, this);
        }
    }

}


void PSS_ClientManagerFrame::OnButton_SetTrackCommandIDClick(wxCommandEvent& event)
{
    //设置IP
    std::string    strCommandID   =std::string(this->TextCtrl_TrackCommandID->GetValue());
    std::string    strCommandIDCount   =std::string(this->TextCtrl_TracCommandCount->GetValue());


    if (strCommandID.length() == 0)
    {
        wxMessageBox(wxT(MESSAGE_INSERT_NULL), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }

    char szSendMessage[200] = { '\0' };
    char szCommand[100] = { '\0' };
    sprintf_s(szCommand, 100, "%s SetTrackCommand -i %s -c %s &", m_pTcpClientConnect->GetKey(), strCommandID.c_str(), strCommandIDCount.c_str());
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[10 * 1024] = { '\0' };
    int nRecvLen = 10 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if (blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
    }
    else
    {
        int nStrLen = 0;
        int nPos = 0;
        int nResult = 0;
        memcpy_s(&nResult, sizeof(char), &szRecvBuff[nPos], sizeof(char));
        nPos += sizeof(char);

        if (nResult == 0)
        {
            wxMessageBox(wxT(MESSAGE_RESULT_SUCCESS), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        }
        else
        {
            wxMessageBox(wxT(MESSAGE_RESULT_FAIL), wxT(MESSAGE_TITLE_SUCCESS), wxOK | wxICON_INFORMATION, this);
        }
    }

}

void PSS_ClientManagerFrame::OnButton_ShowTrackCommandIDClick(wxCommandEvent& event)
{

    //显示命令染色状态
    this->ListCtrl_TrackIPHistory->DeleteAllItems();

    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s GetTrackCommandInfo -a&", m_pTcpClientConnect->GetKey());
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[10 * 1024] = {'\0'};
    int nRecvLen = 10 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
    }
    else
    {
        int nStrLen      = 0;
        int nPos         = 0;
        int nCount       = 0;
        memcpy_s(&nCount, sizeof(char), &szRecvBuff[nPos], sizeof(char));
        nPos += sizeof(char);

        for(int i = 0; i < nCount; i++)
        {
            //开始还原数据结构
            short nCommandID = 0;
            short nCurrCount = 0;
            short nMaxCount  = 0;

            //获得命令ID
            memcpy_s(&nCommandID, sizeof(short), &szRecvBuff[nPos], sizeof(short));
            nPos += sizeof(short);

            //获得当前已染色包数量
            memcpy_s(&nCurrCount, sizeof(short), &szRecvBuff[nPos], sizeof(short));
            nPos += sizeof(short);

            //获得需要染色包总数
            memcpy_s(&nMaxCount, sizeof(short), &szRecvBuff[nPos], sizeof(short));
            nPos += sizeof(short);

            //显示在桌面上
            this->ListCtrl_TrackIPHistory->InsertItem(i, wxString("0x")+wxString::Format(wxT("%04x"), nCommandID));
            this->ListCtrl_TrackIPHistory->SetItem(i, 1, wxString::Format(_T("%d"), nCurrCount));
            this->ListCtrl_TrackIPHistory->SetItem(i, 2, wxString::Format(_T("%d"), nMaxCount));
        }
    }

}

void PSS_ClientManagerFrame::OnButton_GetWorkThreadParamClick(wxCommandEvent& event)
{
    //得到所有工作进程的AI配置
    this->ListCtrl_WorkThreadAI->DeleteAllItems();

    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s GetWorkThreadAI -a&", m_pTcpClientConnect->GetKey());
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[100 * 1024] = {'\0'};
    int nRecvLen = 100 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        int nStrLen       = 0;
        int nPos          = 0;
        int nThreadCount  = 0;
        memcpy_s(&nThreadCount, sizeof(short), &szRecvBuff[nPos], sizeof(short));
        nPos += sizeof(short);

        for(int i = 0; i < nThreadCount; i++)
        {
            int nThreadID       = 0;
            int nAI             = 0;
            int nDisposeTime    = 0;
            int nWTCheckTime    = 0;
            int nWTTimeoutCount = 0;
            int nWTStopTime     = 0;

            memcpy_s(&nThreadID, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);
            memcpy_s(&nAI, sizeof(char), &szRecvBuff[nPos], sizeof(char));
            nPos += sizeof(char);
            memcpy_s(&nDisposeTime, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);
            memcpy_s(&nWTCheckTime, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);
            memcpy_s(&nWTTimeoutCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);
            memcpy_s(&nWTStopTime, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            //显示出来

            this->ListCtrl_WorkThreadAI->InsertItem(i, wxString::Format(wxT("%d"), nThreadID));

            if(nAI == 0)
            {
                this->ListCtrl_WorkThreadAI->SetItem(i, 1, wxT("AI关闭"));
            }
            else
            {
                this->ListCtrl_WorkThreadAI->SetItem(i, 1, wxT("AI运行中"));
            }

            ;

            this->ListCtrl_WorkThreadAI->SetItem(i, 2, wxString::Format(_T("%d"), nDisposeTime));

            this->ListCtrl_WorkThreadAI->SetItem(i, 3, wxString::Format(_T("%d"), nWTCheckTime));

            this->ListCtrl_WorkThreadAI->SetItem(i, 4, wxString::Format(_T("%d"), nWTTimeoutCount));

            this->ListCtrl_WorkThreadAI->SetItem(i, 5, wxString::Format(_T("%d"), nWTStopTime));
        }
    }


}


void PSS_ClientManagerFrame::OnButton_GetWorkThreadPoolClick(wxCommandEvent& event)
{


    //查询当前AI池中的数据
    this->ListCtr_WorkThreadTimeout->DeleteAllItems();
    this->ListCtrl_WorkThreadForbiden->DeleteAllItems();

    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s GetWorkThreadTO -a&", m_pTcpClientConnect->GetKey());
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[100 * 1024] = {'\0'};
    int nRecvLen = 100 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        int nStrLen       = 0;
        int nPos          = 0;
        int nThreadCount  = 0;
        memcpy_s(&nThreadCount, sizeof(short), &szRecvBuff[nPos], sizeof(short));
        nPos += sizeof(short);

        for(int i = 0; i < nThreadCount; i++)
        {
            int nThreadID    = 0;
            int nCommandID   = 0;
            int nSecond      = 0;
            int nDisposeTime = 0;

            memcpy_s(&nThreadID, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            memcpy_s(&nCommandID, sizeof(short), &szRecvBuff[nPos], sizeof(short));
            nPos += sizeof(short);

            memcpy_s(&nSecond, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            memcpy_s(&nDisposeTime, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            //显示出来
            this->ListCtr_WorkThreadTimeout->InsertItem(i, wxString::Format(_T("%d"), nThreadID));
            this->ListCtr_WorkThreadTimeout->SetItem(i, 1, wxString::Format(_T("0x%04x"), nCommandID));
            time_t time2 = nSecond;
            struct tm tm2;
            localtime_s(&tm2, &time2);

            this->ListCtr_WorkThreadTimeout->SetItem(i, 2, wxString::Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), tm2.tm_year + 1900, tm2.tm_mon + 1, tm2.tm_mday, tm2.tm_hour, tm2.tm_min, tm2.tm_sec));
            this->ListCtr_WorkThreadTimeout->SetItem(i, 3, wxString::Format(_T("%d"), nDisposeTime));
        }

        //显示封禁的
        memcpy_s(&nThreadCount, sizeof(short), &szRecvBuff[nPos], sizeof(short));
        nPos += sizeof(short);

        for(int i = 0; i < nThreadCount; i++)
        {
            int nThreadID    = 0;
            int nCommandID   = 0;
            int nSecond      = 0;

            memcpy_s(&nThreadID, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            memcpy_s(&nCommandID, sizeof(short), &szRecvBuff[nPos], sizeof(short));
            nPos += sizeof(short);

            memcpy_s(&nSecond, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            //显示出来
            this->ListCtrl_WorkThreadForbiden->InsertItem(i, wxString::Format(wxT("%d"), nThreadID));            ;
            this->ListCtrl_WorkThreadForbiden->SetItem(i, 1, wxString::Format(wxT("0x%04x"), nCommandID));

            time_t time2 = nSecond;
            struct tm tm2;
            localtime_s(&tm2, &time2);

            this->ListCtrl_WorkThreadForbiden->SetItem(i, 2, wxString::Format(wxT("%04d-%02d-%02d %02d:%02d:%02d"), tm2.tm_year + 1900,tm2.tm_mon + 1, tm2.tm_mday, tm2.tm_hour, tm2.tm_min, tm2.tm_sec));
        }
    }



}



void PSS_ClientManagerFrame::OnButton_SetClick(wxCommandEvent& event)
{

    std::string    strServerIP    =std::string(this->TextCtrl_ServerIP->GetValue());
    std::string    strConsoleKey    =std::string(this->TextCtrl_ServerKey->GetValue());
    int nPort = wxAtoi(this->TextCtrl_ServerPort->GetValue());

    if(strServerIP.length() == 0 || strConsoleKey.length() == 0  || nPort == 0)
    {
        wxMessageBox(wxT(MESSAGE_INSERT_NULL), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }

    m_pTcpClientConnect->Init(strServerIP.c_str(), nPort, strConsoleKey.c_str());

    wxMessageBox(wxT(MESSAGE_TITLE_CONNECT), wxT(MESSAGE_TITLE_SUCCESS), wxOK | wxICON_INFORMATION, this);


}

void PSS_ClientManagerFrame::OnButton_GetServerstateClick(wxCommandEvent& event)
{

    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s ShowServerInfo -a&", m_pTcpClientConnect->GetKey());
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[10 * 1024] = {'\0'};
    int nRecvLen = 10 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        //处理发送的数据
        int nPos = 0;

        int nTempLen             = 0;
        int nSerevrID            = 0;
        char szServerName[20]    = {'\0'};
        char szServerVersion[20] = {'\0'};
        int nModuleCount         = 0;
        int nWorkThreadCount     = 0;
        char szPacketVersion[20] = {'\0'};
        int nCharOrder           = 0;
        int nByteOrder           = 0;

        memcpy_s(&nSerevrID, 2, &szRecvBuff[nPos], sizeof(short));
        nPos += sizeof(short);
        memcpy_s(&nTempLen, 1, &szRecvBuff[nPos], sizeof(char));
        nPos += sizeof(char);
        memcpy_s(&szServerName, nTempLen, &szRecvBuff[nPos], nTempLen);
        nPos += nTempLen;
        memcpy_s(&nTempLen, 1, &szRecvBuff[nPos], sizeof(char));
        nPos += sizeof(char);
        memcpy_s(&szServerVersion, nTempLen, &szRecvBuff[nPos], nTempLen);
        nPos += nTempLen;
        memcpy_s(&nModuleCount, 2, &szRecvBuff[nPos], sizeof(short));
        nPos += sizeof(short);
        memcpy_s(&nWorkThreadCount, 2, &szRecvBuff[nPos], sizeof(short));
        nPos += sizeof(short);
        memcpy_s(&nTempLen, 1, &szRecvBuff[nPos], sizeof(char));
        nPos += sizeof(char);
        memcpy_s(&szPacketVersion, nTempLen, &szRecvBuff[nPos], nTempLen);
        nPos += nTempLen;
        memcpy_s(&nCharOrder, 1, &szRecvBuff[nPos], sizeof(char));
        nPos += sizeof(char);
        memcpy_s(&nByteOrder, 1, &szRecvBuff[nPos], sizeof(char));
        nPos += sizeof(char);

        char szCharOrder[20] = {'\0'};

        if(nCharOrder == 0)
        {
            sprintf_s(szCharOrder, 100, "小端");
        }
        else
        {
            sprintf_s(szCharOrder, 100, "大端");
        }

        char szByteOrder[20] = {'\0'};

        if(nByteOrder == 0)
        {
            sprintf_s(szByteOrder, 100, "主机字节序");
        }
        else
        {
            sprintf_s(szByteOrder, 100, "网络字节序");
        }

        char szText[2048] = {'\0'};
        sprintf_s(szText,2048, "服务器ID:%d.\r\n服务器名称:%s.\r\n服务器版本:%s.\r\n服务器加载模块数:%d.\r\n服务器工作线程数:%d\r\n服务器包解析版本号:%s\r\n服务器大小端:%s\r\n服务器字符序:%s.",
                  nSerevrID, szServerName, szServerVersion, nModuleCount, nWorkThreadCount, szPacketVersion, szCharOrder, szByteOrder);

        //显示内容

        this->TextCtrl_ServerInfo->SetValue(wxString(szText));

    }

}



void PSS_ClientManagerFrame::OnButton_ShowAllModuleClick(wxCommandEvent& event)
{

    this->ListCtrl_ModuleList->DeleteAllItems();

    vecModuleInfo objvecModuleInfo;

    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s ShowModule -a&", m_pTcpClientConnect->GetKey());
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[10 * 1024] = {'\0'};
    int nRecvLen = 10 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        int nStrLen      = 0;
        int nPos         = 0;
        int nModuleCount = 0;
        memcpy_s(&nModuleCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);

        for(int i = 0; i < nModuleCount; i++)
        {
            //开始还原数据结构
            _ModuleInfo ModuleInfo;
            memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
            nPos += sizeof(char);

            memcpy_s(ModuleInfo.szModuleName, nStrLen, &szRecvBuff[nPos], nStrLen);
            nPos += nStrLen;
            ModuleInfo.szModuleName[nStrLen] = '\0';

            memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
            nPos += sizeof(char);

            memcpy_s(ModuleInfo.szModuleFile, nStrLen, &szRecvBuff[nPos], nStrLen);
            nPos += nStrLen;
            ModuleInfo.szModuleFile[nStrLen] = '\0';

            memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
            nPos += sizeof(char);

            memcpy_s(ModuleInfo.szModulePath, nStrLen, &szRecvBuff[nPos], nStrLen);
            nPos += nStrLen;
            ModuleInfo.szModulePath[nStrLen] = '\0';

            memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
            nPos += sizeof(char);

            memcpy_s(ModuleInfo.szModuleParam, nStrLen, &szRecvBuff[nPos], nStrLen);
            nPos += nStrLen;
            ModuleInfo.szModuleParam[nStrLen] = '\0';

            memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
            nPos += sizeof(char);

            memcpy_s(ModuleInfo.szModuleDesc, nStrLen, &szRecvBuff[nPos], nStrLen);
            nPos += nStrLen;
            ModuleInfo.szModuleDesc[nStrLen] = '\0';

            memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
            nPos += sizeof(char);

            memcpy_s(ModuleInfo.szModuleCreateDate, nStrLen, &szRecvBuff[nPos], nStrLen);
            nPos += nStrLen;
            ModuleInfo.szModuleCreateDate[nStrLen] = '\0';

            memcpy_s(&ModuleInfo.nModuleState, sizeof(char), &szRecvBuff[nPos], sizeof(char));
            nPos += sizeof(char);

            memcpy_s(&ModuleInfo.nModuleID, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            //显示在桌面上
            this->ListCtrl_ModuleList->InsertItem(i, ModuleInfo.szModuleFile);
            this->ListCtrl_ModuleList->SetItem(i, 1, ModuleInfo.szModuleName);
            this->ListCtrl_ModuleList->SetItem(i, 2, ModuleInfo.szModulePath);
            this->ListCtrl_ModuleList->SetItem(i, 3, ModuleInfo.szModuleParam);
            this->ListCtrl_ModuleList->SetItem(i, 4, ModuleInfo.szModuleDesc);
            this->ListCtrl_ModuleList->SetItem(i, 5, ModuleInfo.szModuleCreateDate);


            if(ModuleInfo.nModuleState == 0)
            {
                this->ListCtrl_ModuleList->SetItem(i, 6, wxT("正常"));
            }
            else
            {
                this->ListCtrl_ModuleList->SetItem(i, 6, wxT("异常"));
            }

            this->ListCtrl_ModuleList->SetItem(i, 7, wxString::Format(wxT("%d"),ModuleInfo.nModuleID));

            objvecModuleInfo.push_back(ModuleInfo);
        }
    }


}

void PSS_ClientManagerFrame::OnButton_LoadModuleClick(wxCommandEvent& event)
{
    //加载指定模块

    wxCSConv cvGBK( wxT("GBK") );
    std::string    strModulePath    =std::string( cvGBK.cWX2MB( this->TextCtrl_ModulePath->GetValue().c_str() ) );
    std::string    strModuleFile    =std::string( cvGBK.cWX2MB( this->TextCtrl_ModuleFilename->GetValue().c_str() ) );
    std::string    strModuleParam    =std::string( cvGBK.cWX2MB( this->TextCtrl_ModuleParam->GetValue().c_str() ) );


    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s LoadModule %s,%s,%s&", m_pTcpClientConnect->GetKey(), strModulePath.c_str(), strModuleFile.c_str(), strModuleParam.c_str());
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[10 * 1024] = {'\0'};
    int nRecvLen = 10 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        int nStrLen      = 0;
        int nPos         = 0;
        int nResult      = 0;
        memcpy_s(&nResult, sizeof(int), &szRecvBuff[nPos], sizeof(char));
        nPos += sizeof(int);

        if(nResult == 0)
        {
            wxMessageBox(wxT(MESSAGE_RESULT_SUCCESS), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        }
        else
        {
            wxMessageBox(wxT(MESSAGE_RESULT_FAIL), wxT(MESSAGE_TITLE_SUCCESS), wxOK | wxICON_INFORMATION, this);
        }
    }

    //OnBnClickedButton3();
    this->OnButton_ShowAllModuleClick(event);

}




void PSS_ClientManagerFrame::LoadConfigFile()
{
    //从配置文件加载
    m_vecConfigInfo.clear();

    bool blState = m_objXmlOpeation.Init(CONFIG_NAME);

    if(false == blState)
    {
        wxMessageBox(wxT("读取配置文件错误，请确认当前目录下Config.xml文件存在"), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }

    TiXmlElement* pNextTiXmlElementName      = NULL;
    TiXmlElement* pNextTiXmlElementIP        = NULL;
    TiXmlElement* pNextTiXmlElementPort      = NULL;
    TiXmlElement* pNextTiXmlElementMagicCode = NULL;
    char* pData                              = NULL;

    while(true)
    {
        _ConfigInfo objConfigInfo;

        pData = m_objXmlOpeation.GetData("ServerInfo", "servername", pNextTiXmlElementName);

        if(pData != NULL)
        {
            sprintf_s(objConfigInfo.m_szServerName, sizeof(objConfigInfo.m_szServerName), "%s", pData);
        }
        else
        {
            break;
        }

        pData = m_objXmlOpeation.GetData("ServerInfo", "sIP", pNextTiXmlElementIP);

        if(pData != NULL)
        {
            sprintf_s(objConfigInfo.m_szIP, sizeof(objConfigInfo.m_szIP), "%s", pData);
        }
        else
        {
            break;
        }

        pData = m_objXmlOpeation.GetData("ServerInfo", "sport", pNextTiXmlElementPort);

        if(pData != NULL)
        {
            objConfigInfo.m_nPort = atoi(pData);
        }
        else
        {
            break;
        }

        pData = m_objXmlOpeation.GetData("ServerInfo", "MagicCode", pNextTiXmlElementMagicCode);

        if(pData != NULL)
        {
            sprintf_s(objConfigInfo.m_szMagicCode, sizeof(objConfigInfo.m_szMagicCode), "%s", pData);
        }
        else
        {
            break;
        }

        m_vecConfigInfo.push_back(objConfigInfo);
    }

    m_objXmlOpeation.Close();


    ShowConfigList();


}

void PSS_ClientManagerFrame::OnButton_LoadConfigfileClick(wxCommandEvent& event)
{
    LoadConfigFile();
    ChangeConfigFile();
}


void PSS_ClientManagerFrame::ShowConfigList()
{

    this->ListBox_ConfigList->Clear();
    //wchar_t szTemp[1024] = {'\0'};

    for(int i = 0; i < (int)m_vecConfigInfo.size(); i++)
    {
        //int nSrcLen = MultiByteToWideChar(CP_ACP, 0, m_vecConfigInfo[i].m_szServerName, strlen(m_vecConfigInfo[i].m_szServerName), NULL, 0);
        //int nDecLen = MultiByteToWideChar(CP_ACP, 0, m_vecConfigInfo[i].m_szServerName, nSrcLen, szTemp, 1024);
        //szTemp[nDecLen] = '\0';

        //m_lbConfig.InsertString(i, szTemp);
        wxString    strServerName   =wxString(m_vecConfigInfo[i].m_szServerName);
        this->ListBox_ConfigList->InsertItems(1,&strServerName,i);

    }

    if  (m_vecConfigInfo.size() >0)
    {
        this->ListBox_ConfigList->SetSelection(0);
    }

}



void PSS_ClientManagerFrame::OnButton_LoadFileClick(wxCommandEvent& event)
{

    //从XML文件中加载
    m_vecCheckServer.clear();

    CXmlOpeation objXmlOpeation;

    bool blState = objXmlOpeation.Init(CHECKSERVER_FILE);

    if(false == blState)
    {
        wxMessageBox(wxString("读取配置文件错误，请确认当前目录下Config.xml文件存在"), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }

    TiXmlElement* pNextTiXmlElementIP        = NULL;
    TiXmlElement* pNextTiXmlElementPort      = NULL;
    TiXmlElement* pNextTiXmlElementMagicCode = NULL;
    TiXmlElement* pNextTiXmlElementTCount    = NULL;
    char* pData                              = NULL;

    while(true)
    {
        _CheckServer objCheckServer;

        pData = objXmlOpeation.GetData("ServerInfo", "sIP", pNextTiXmlElementIP);

        if(pData != NULL)
        {
            sprintf_s(objCheckServer.m_szIP, sizeof(objCheckServer.m_szIP), "%s", pData);
        }
        else
        {
            break;
        }

        pData = objXmlOpeation.GetData("ServerInfo", "sport", pNextTiXmlElementPort);

        if(pData != NULL)
        {
            objCheckServer.m_nPort = atoi(pData);
        }
        else
        {
            break;
        }

        pData = objXmlOpeation.GetData("ServerInfo", "MagicCode", pNextTiXmlElementMagicCode);

        if(pData != NULL)
        {
            sprintf_s(objCheckServer.m_szMagicCode, sizeof(objCheckServer.m_szMagicCode), "%s", pData);
        }
        else
        {
            break;
        }

        pData = objXmlOpeation.GetData("ServerInfo", "ThreadCount", pNextTiXmlElementTCount);

        if(pData != NULL)
        {
            objCheckServer.m_nThreadCount = atoi(pData);
        }
        else
        {
            break;
        }

        m_vecCheckServer.push_back(objCheckServer);
    }

    objXmlOpeation.Close();

    ShowGroupServerList();

}

void PSS_ClientManagerFrame::OnButton_ClearLoadClick(wxCommandEvent& event)
{

    //清除加载
    m_vecCheckServer.clear();

    ShowGroupServerList();

}


void PSS_ClientManagerFrame::ShowGroupServerList()
{
    this->ListCtrl_ServerList->DeleteAllItems();


    for(int i = 0; i < (int)m_vecCheckServer.size(); i++)
    {
        char    szLog[500]  = {'\0'};
        sprintf_s(szLog, 500, "[%s:%d]监控线程数[%d]", m_vecCheckServer[i].m_szIP, m_vecCheckServer[i].m_nPort, m_vecCheckServer[i].m_nThreadCount);

        this->ListCtrl_ServerList->InsertItem(this->ListCtrl_ServerList->GetItemCount(),wxString(szLog));

    }

}

void PSS_ClientManagerFrame::ClearCheckServerInfo()
{
    for(int i = 0; i < (int)m_vecCheckServerInfo.size(); i++)
    {
        _CheckServerInfo* pCheckServerInfo = (_CheckServerInfo*)(m_vecCheckServerInfo[i]);

        if(NULL != pCheckServerInfo)
        {
            delete pCheckServerInfo;
        }
    }

    m_vecCheckServerInfo.clear();
}

void PSS_ClientManagerFrame::OnButton_ResetWorkThreadClick(wxCommandEvent& event)
{
    //重置工作线程AI参数
    int nDisposeTime = wxAtoi(this->TextCtrl_WorkThreadDisposeTime->GetValue());

    if(nDisposeTime <= 0)
    {
        wxMessageBox(wxString("处理时间不能是0"), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }


    int nCheckTime =  wxAtoi(this->TextCtrl_WorkThreadCheckTime->GetValue());

    if(nCheckTime <= 0)
    {
        wxMessageBox(wxString("时间区间不能是0"), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }


    int nStopTime = wxAtoi(this->TextCtrl_WorkThreadStopTime->GetValue());

    if(nStopTime <= 0)
    {
        wxMessageBox(wxString("暂停时间不能是0"), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }

    int nAI = 0;

    if(this->CheckBox_WorkThread->GetValue() == true)
    {
        nAI = 1;
    }

    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s SetWorkThreadAI -i %d,%d,%d,%d&", m_pTcpClientConnect->GetKey(), nAI, nDisposeTime, nCheckTime, nStopTime);
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[100 * 1024] = {'\0'};
    int nRecvLen = 100 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        wxMessageBox(wxT(MESSAGE_SENDERROR), wxT(MESSAGE_TITLE_ERROR), wxOK | wxICON_INFORMATION, this);
        return;
    }
    else
    {
        int nStrLen       = 0;
        int nPos          = 0;

        wxMessageBox(wxString("线程AI参数设置成功"), wxT(MESSAGE_TITLE_SUCCESS), wxOK | wxICON_INFORMATION, this);
        return;
    }


}

