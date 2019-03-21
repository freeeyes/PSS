/***************************************************************
 * Name:      PSS_ClientManagerMain.h
 * Purpose:   Defines Application Frame
 * Author:    smith ()
 * Created:   2019-02-21
 * Copyright: smith ()
 * License:
 **************************************************************/

#ifndef PSS_CLIENTMANAGERMAIN_H
#define PSS_CLIENTMANAGERMAIN_H

//(*Headers(PSS_ClientManagerFrame)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/combobox.h>
#include <wx/frame.h>
#include <wx/listbox.h>
#include <wx/listctrl.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/radiobox.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
//*)

#include <stdio.h>

#include <wchar.h>
#include <vector>


#include "XmlOpeation.h"

#include "macro.h"
#include "sock_wrap.h"


#include "ClientDefine.h"
#include "TcpClientConnect.h"

using namespace std;


struct _IPAccount
{
    char           m_szIP[50];   //当前链接地址
    int            m_nCount;              //当前链接次数
    int            m_nAllCount;           //指定IP链接次数总和
    int            m_nMinute;             //当前分钟数
    char           m_szDate[50]; //最后更新时间

    _IPAccount()
    {
        m_szIP[0]    = '\0';
        m_nCount     = 0;
        m_nAllCount  = 0;
        m_szDate[0]  = '\0';
    }
};

typedef vector<_IPAccount> vecIPAccount;
typedef vector<_ClientConnectInfo> vecClientConnectInfo;






#define CONFIG_NAME "./Config.xml"

//PSS远程服务器配置信息
struct _ServerInfo
{
    int  m_nSerevrID;
    char m_szServerName[20];
    char m_szServerVersion[20];
    int  m_nModuleCount;
    int  m_nWorkThreadCount;
    char m_szPacketVersion[20];

    _ServerInfo()
    {
        m_nSerevrID          = 0;
        m_szServerName[0]    = '\0';
        m_szServerVersion[0] = '\0';
        m_nModuleCount       = 0;
        m_nWorkThreadCount   = 0;
        m_szPacketVersion[0] = '\0';
    }
};

//PSS模块加载信息
struct _ModuleInfo
{
    char szModuleName[200];
    char szModuleFile[200];
    char szModulePath[200];
    char szModuleParam[200];
    char szModuleDesc[200];
    char szModuleCreateDate[200];
    int  nModuleState;
    int  nModuleID;

    _ModuleInfo()
    {
        szModuleName[0]       = '\0';
        szModuleFile[0]       = '\0';
        szModulePath[0]       = '\0';
        szModuleParam[0]      = '\0';
        szModuleDesc[0]       = '\0';
        szModuleCreateDate[0] = '\0';
        nModuleState          = 0;
        nModuleID             = 0;
    }
};

//PSS配置文件信息
struct _ConfigInfo
{
    char m_szServerName[200];
    char m_szIP[50];
    int  m_nPort;
    char m_szMagicCode[300];

    _ConfigInfo()
    {
        m_szServerName[0] = '\0';
        m_szIP[0]         = '\0';
        m_nPort           = 0;
        m_szMagicCode[0]  = '\0';
    }
};

struct _CommandInfo
{
    char szModuleName[200];
    char szCommandID[200];
    int  m_nCount;                     //当前命令被调用的次数
    int  m_nTimeCost;                  //当前命令总时间消耗

    _CommandInfo()
    {
        szModuleName[0] = '\0';
        szCommandID[0]  = '\0';
        m_nCount        = 0;
        m_nTimeCost     = 0;
    }
};

struct _CommandTimeOut
{
    short          nCommandID;                  //命令的ID
    int            nTime;                       //发生时间
    int            nTimeOutTime;                //超时时间

    _CommandTimeOut()
    {
        nCommandID   = 0;
        nTime        = 0;
        nTimeOutTime = 0;
    }
};

typedef vector<_CommandInfo> vecCommandInfo;

typedef vector<_ConfigInfo> vecConfigInfo;

typedef vector<_ModuleInfo> vecModuleInfo;


#define CHECKSERVER_FILE "./CheckServer.xml"

//监控线程信息
struct _WorkThreadInfo
{
    char                m_szThreadName[50];   //线程名称
    int                 m_nThreadID;         //线程ID
    int                 m_nUpdateTime;       //线程最后处理数据的时间
    int                 m_nCreateTime;       //线程最后处理数据的时间
    int                 m_nState;            //0为没有数据正在处理，1为正在处理接受数据。
    int                 m_nRecvPacketCount;  //此线程下接收包的处理个数
    int                 m_nSendPacketCount;  //此线程下发送包的处理个数
    int                 m_nCommandID;        //当前处理的信令ID
    int                 m_nPacketTime;       //平均数据包处理时间
    int                 m_nCurrPacketCount;  //单位时间内的包处理数量

    _WorkThreadInfo()
    {
        m_nThreadID        = 0;
        m_nUpdateTime      = 0;
        m_nCreateTime      = 0;
        m_nState           = 0;
        m_nRecvPacketCount = 0;
        m_nSendPacketCount = 0;
        m_nCommandID       = 0;
        m_nPacketTime      = 0;
        m_nCurrPacketCount = 0;
    }
};

typedef vector<_WorkThreadInfo> vecWorkThreadInfo;

//监控服务器需要加载的信息
struct _CheckServerInfo
{
public:
    CTcpClientConnect m_objTcpClientConnect;
    int               m_nCheckThreadCount;
    _WorkThreadInfo*  m_pWorkThreadInfo;

    _CheckServerInfo()
    {
        m_nCheckThreadCount = 0;
        m_pWorkThreadInfo   = NULL;
    }

    ~_CheckServerInfo()
    {
        if(NULL != m_pWorkThreadInfo)
        {
            delete[] m_pWorkThreadInfo;
            m_pWorkThreadInfo = NULL;
        }
    }
};

typedef vector<_CheckServerInfo*> vecCheckServerInfo;

//XML中记载的服务器的配置信息
struct _CheckServer
{
public:
    char m_szIP[50];
    int  m_nPort;
    char m_szMagicCode[300];
    int  m_nThreadCount;

    _CheckServer()
    {
        m_szIP[0]        = '\0';
        m_nPort          = 0;
        m_szMagicCode[0] = '\0';
        m_nThreadCount   = 0;
    }
};

typedef vector<_CheckServer> vecCheckServer;

struct _ForbiddenIP
{
    char  m_szIP[50];
    int   m_nType;
    int   m_nBeginTime;
    int   m_nSecond;

    _ForbiddenIP()
    {
        m_szIP[0]    = '\0';
        m_nType      = 0;
        m_nBeginTime = 0;
        m_nSecond    = 0;
    }
};


//链接别名映射信息(用于PSS_ClientManager管理)
struct _ClientNameInfo
{
    char m_szName[100];      //连接别名
    char m_szClientIP[20];   //客户端的IP地址
    int  m_nPort;            //客户端的端口
    int  m_nConnectID;       //连接ID
    int  m_nLog;             //Log标记位

    _ClientNameInfo()
    {
        m_szName[0]     = '\0';
        m_szClientIP[0] = '\0';
        m_nPort         = 0;
        m_nConnectID    = 0;
        m_nLog          = 0;
    }
};
typedef vector<_ClientNameInfo> vecClientNameInfo;

typedef vector<_ForbiddenIP> vecForbiddenIP;
typedef vector<_ClientConnectInfo> vecClientConnectInfo;



using namespace std;



class PSS_ClientManagerFrame: public wxFrame
{
public:

    PSS_ClientManagerFrame(wxWindow* parent,wxWindowID id = -1);
    virtual ~PSS_ClientManagerFrame();

private:

    //(*Handlers(PSS_ClientManagerFrame)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnButton_ReloadModuleClick(wxCommandEvent& event);
    void OnButton_UnloadModuleClick(wxCommandEvent& event);
    void OnTextCtrl_ModuleNmaeText(wxCommandEvent& event);
    void OnButton_CloseServerClick(wxCommandEvent& event);
    void OnListBox_ConfigListSelect(wxCommandEvent& event);
    void OnListCtrl_ModuleListItemSelect(wxListEvent& event);
    void OnButton_GetCurrLinkCountClick(wxCommandEvent& event);
    void OnButton_SetLinkLimitClick(wxCommandEvent& event);
    void OnButton_SetDebugClick(wxCommandEvent& event);
    void OnButton_GetDebugStateClick(wxCommandEvent& event);
    void OnButton_StartFileClick(wxCommandEvent& event);
    void OnButton_StopFileClick(wxCommandEvent& event);
    void OnButton_GetLinkHistoryClick(wxCommandEvent& event);
    void OnTextCtrl_CurrLinkCountText(wxCommandEvent& event);
    void OnButton_GetCurrClientCountClick(wxCommandEvent& event);
    void OnTextCtrl_ModuleIDText(wxCommandEvent& event);
    void OnButton_ShowModuleCmdClick(wxCommandEvent& event);
    void OnButton_ShowAllCmdClick(wxCommandEvent& event);
    void OnButton_ShowTimeoutCmdClick(wxCommandEvent& event);
    void OnButton_ClearTimeoutCmdClick(wxCommandEvent& event);
    void OnButton_SaveSmdHistoryClick(wxCommandEvent& event);
    void OnButton_DisconnectClientClick(wxCommandEvent& event);
    void OnButton_ShowClientClick(wxCommandEvent& event);
    void OnButton_SetLogLevelClick(wxCommandEvent& event);
    void OnButton_ShowCurrLoginfoClick(wxCommandEvent& event);
    void OnButton_StartMonitorClick(wxCommandEvent& event);
    void OnButton_StopMonitorClick(wxCommandEvent& event);
    void OnTimer_WorkThreadTrigger(wxTimerEvent& event);
    void OnButton_ShowServerLinkInfoClick(wxCommandEvent& event);
    void OnButton_ReconnectServerClick(wxCommandEvent& event);
    void OnButton_ShowPSSLisenInfoClick(wxCommandEvent& event);
    void OnButton_InsertNewMonitorClick(wxCommandEvent& event);
    void OnButton_DeleteOldMonitorClick(wxCommandEvent& event);
    void OnButton_ShowWorkThreadStatClick(wxCommandEvent& event);
    void OnButton_ShowWorkThreadStatClick1(wxCommandEvent& event);
    void OnButton_InsertForbidenClick(wxCommandEvent& event);
    void OnButton_ReleaseForbidenClick(wxCommandEvent& event);
    void OnButton_ShowForbidenClick(wxCommandEvent& event);
    void OnButton_SearchNicknameClick(wxCommandEvent& event);
    void OnButton_OpenLogClick(wxCommandEvent& event);
    void OnButton_CloselogClick(wxCommandEvent& event);
    void OnButton41Click(wxCommandEvent& event);
    void OnButton_GetTrackSourceIPClick(wxCommandEvent& event);
    void OnButton_SetTrackIPClick(wxCommandEvent& event);
    void OnButton_SetTrackCommandIDClick(wxCommandEvent& event);
    void OnButton_ShowTrackCommandIDClick(wxCommandEvent& event);
    void OnButton_GetWorkThreadParamClick(wxCommandEvent& event);
    void OnButton_GetWorkThreadPoolClick(wxCommandEvent& event);
    void OnListCtrl_CheckLogBeginDrag(wxListEvent& event);
    void OnListCtrl_LinkHistoryBeginDrag(wxListEvent& event);
    void OnButton_SetClick(wxCommandEvent& event);
    void OnButton_GetServerstateClick(wxCommandEvent& event);
    void OnButton_ShowAllModuleClick(wxCommandEvent& event);
    void OnButton_LoadModuleClick(wxCommandEvent& event);
    void OnButton_LoadConfigfileClick(wxCommandEvent& event);
    void OnButton_LoadFileClick(wxCommandEvent& event);
    void OnButton_ClearLoadClick(wxCommandEvent& event);
    void OnButton_ResetWorkThreadClick(wxCommandEvent& event);
    //*)

    //(*Identifiers(PSS_ClientManagerFrame)
    static const long ID_STATICBOX1;
    static const long ID_STATICBOX2;
    static const long ID_STATICTEXT1;
    static const long ID_STATICTEXT2;
    static const long ID_TEXTCTRL1;
    static const long ID_TEXTCTRL2;
    static const long ID_STATICTEXT3;
    static const long ID_TEXTCTRL3;
    static const long ID_BUTTON1;
    static const long ID_BUTTON2;
    static const long ID_BUTTON3;
    static const long ID_LISTBOX1;
    static const long ID_BUTTON4;
    static const long ID_BUTTON5;
    static const long ID_STATICTEXT4;
    static const long ID_TEXTCTRL4;
    static const long ID_STATICTEXT5;
    static const long ID_TEXTCTRL5;
    static const long ID_STATICTEXT6;
    static const long ID_TEXTCTRL6;
    static const long ID_STATICTEXT7;
    static const long ID_TEXTCTRL7;
    static const long ID_BUTTON6;
    static const long ID_BUTTON7;
    static const long ID_BUTTON8;
    static const long ID_LISTCTRL1;
    static const long ID_TEXTCTRL38;
    static const long ID_PANEL1;
    static const long ID_STATICBOX3;
    static const long ID_STATICBOX4;
    static const long ID_BUTTON9;
    static const long ID_STATICTEXT8;
    static const long ID_TEXTCTRL8;
    static const long ID_STATICTEXT9;
    static const long ID_TEXTCTRL9;
    static const long ID_STATICTEXT10;
    static const long ID_TEXTCTRL10;
    static const long ID_BUTTON10;
    static const long ID_STATICTEXT11;
    static const long ID_TEXTCTRL11;
    static const long ID_BUTTON11;
    static const long ID_RADIOBOX1;
    static const long ID_STATICTEXT12;
    static const long ID_TEXTCTRL12;
    static const long ID_BUTTON12;
    static const long ID_BUTTON13;
    static const long ID_STATICTEXT13;
    static const long ID_TEXTCTRL13;
    static const long ID_STATICTEXT14;
    static const long ID_TEXTCTRL14;
    static const long ID_BUTTON14;
    static const long ID_STATICTEXT15;
    static const long ID_TEXTCTRL15;
    static const long ID_BUTTON15;
    static const long ID_LISTCTRL2;
    static const long ID_PANEL2;
    static const long ID_STATICBOX5;
    static const long ID_STATICTEXT16;
    static const long ID_TEXTCTRL16;
    static const long ID_BUTTON16;
    static const long ID_BUTTON17;
    static const long ID_STATICTEXT17;
    static const long ID_LISTCTRL3;
    static const long ID_BUTTON18;
    static const long ID_BUTTON19;
    static const long ID_BUTTON20;
    static const long ID_LISTCTRL4;
    static const long ID_PANEL3;
    static const long ID_STATICBOX6;
    static const long ID_STATICTEXT18;
    static const long ID_TEXTCTRL17;
    static const long ID_BUTTON21;
    static const long ID_BUTTON22;
    static const long ID_LISTCTRL5;
    static const long ID_STATICTEXT19;
    static const long ID_TEXTCTRL18;
    static const long ID_BUTTON23;
    static const long ID_BUTTON24;
    static const long ID_LISTCTRL6;
    static const long ID_PANEL4;
    static const long ID_STATICBOX7;
    static const long ID_BUTTON25;
    static const long ID_LISTCTRL7;
    static const long ID_STATICBOX8;
    static const long ID_STATICTEXT20;
    static const long ID_TEXTCTRL19;
    static const long ID_STATICTEXT21;
    static const long ID_TEXTCTRL20;
    static const long ID_STATICTEXT22;
    static const long ID_BUTTON26;
    static const long ID_BUTTON27;
    static const long ID_CHECKBOX1;
    static const long ID_LISTCTRL8;
    static const long ID_RADIOBOX2;
    static const long ID_LISTCTRL9;
    static const long ID_BUTTON28;
    static const long ID_BUTTON29;
    static const long ID_PANEL5;
    static const long ID_STATICBOX9;
    static const long ID_BUTTON30;
    static const long ID_STATICTEXT23;
    static const long ID_TEXTCTRL21;
    static const long ID_BUTTON31;
    static const long ID_LISTCTRL10;
    static const long ID_LISTCTRL11;
    static const long ID_BUTTON32;
    static const long ID_STATICBOX10;
    static const long ID_STATICTEXT24;
    static const long ID_STATICTEXT25;
    static const long ID_STATICTEXT26;
    static const long ID_TEXTCTRL22;
    static const long ID_TEXTCTRL23;
    static const long ID_STATICTEXT27;
    static const long ID_TEXTCTRL24;
    static const long ID_COMBOBOX1;
    static const long ID_BUTTON33;
    static const long ID_BUTTON34;
    static const long ID_PANEL6;
    static const long ID_STATICBOX11;
    static const long ID_STATICTEXT28;
    static const long ID_TEXTCTRL25;
    static const long ID_STATICTEXT29;
    static const long ID_TEXTCTRL26;
    static const long ID_RADIOBOX3;
    static const long ID_BUTTON35;
    static const long ID_BUTTON36;
    static const long ID_BUTTON37;
    static const long ID_LISTCTRL12;
    static const long ID_STATICTEXT30;
    static const long ID_STATICTEXT31;
    static const long ID_TEXTCTRL27;
    static const long ID_TEXTCTRL28;
    static const long ID_BUTTON38;
    static const long ID_BUTTON39;
    static const long ID_BUTTON40;
    static const long ID_LISTCTRL13;
    static const long ID_PANEL7;
    static const long ID_STATICBOX12;
    static const long ID_LISTCTRL14;
    static const long ID_STATICTEXT32;
    static const long ID_STATICTEXT33;
    static const long ID_TEXTCTRL29;
    static const long ID_BUTTON41;
    static const long ID_TEXTCTRL30;
    static const long ID_TEXTCTRL31;
    static const long ID_TEXTCTRL32;
    static const long ID_BUTTON42;
    static const long ID_BUTTON43;
    static const long ID_BUTTON44;
    static const long ID_STATICTEXT35;
    static const long ID_STATICTEXT36;
    static const long ID_TEXTCTRL33;
    static const long ID_STATICTEXT37;
    static const long ID_TEXTCTRL34;
    static const long ID_PANEL8;
    static const long ID_STATICBOX13;
    static const long ID_STATICBOX14;
    static const long ID_LISTCTRL15;
    static const long ID_CHECKBOX2;
    static const long ID_STATICTEXT34;
    static const long ID_TEXTCTRL35;
    static const long ID_STATICTEXT38;
    static const long ID_STATICTEXT39;
    static const long ID_TEXTCTRL36;
    static const long ID_STATICTEXT40;
    static const long ID_STATICTEXT41;
    static const long ID_TEXTCTRL37;
    static const long ID_STATICTEXT42;
    static const long ID_BUTTON45;
    static const long ID_BUTTON46;
    static const long ID_LISTCTRL16;
    static const long ID_LISTCTRL17;
    static const long ID_BUTTON47;
    static const long ID_PANEL9;
    static const long ID_NOTEBOOK1;
    static const long ID_TIMER1;
    //*)

    //(*Declarations(PSS_ClientManagerFrame)
    wxButton* Button_ClearLoad;
    wxButton* Button_ClearTimeoutCmd;
    wxButton* Button_CloseServer;
    wxButton* Button_Closelog;
    wxButton* Button_DeleteOldMonitor;
    wxButton* Button_DisconnectClient;
    wxButton* Button_GetCurrClientCount;
    wxButton* Button_GetDebugState;
    wxButton* Button_GetLinkHistory;
    wxButton* Button_GetServerstate;
    wxButton* Button_GetTrackSourceIP;
    wxButton* Button_GetWorkThreadParam;
    wxButton* Button_GetWorkThreadPool;
    wxButton* Button_InsertForbiden;
    wxButton* Button_InsertNewMonitor;
    wxButton* Button_LoadConfigfile;
    wxButton* Button_LoadFile;
    wxButton* Button_LoadModule;
    wxButton* Button_OpenLog;
    wxButton* Button_ReconnectServer;
    wxButton* Button_ReleaseForbiden;
    wxButton* Button_ReloadModule;
    wxButton* Button_ResetWorkThread;
    wxButton* Button_SaveSmdHistory;
    wxButton* Button_SearchNickname;
    wxButton* Button_Set;
    wxButton* Button_SetDebug;
    wxButton* Button_SetLinkLimit;
    wxButton* Button_SetLogLevel;
    wxButton* Button_SetTrackCommandID;
    wxButton* Button_SetTrackIP;
    wxButton* Button_ShowAllCmd;
    wxButton* Button_ShowAllModule;
    wxButton* Button_ShowClient;
    wxButton* Button_ShowCurrLoginfo;
    wxButton* Button_ShowForbiden;
    wxButton* Button_ShowModuleCmd;
    wxButton* Button_ShowPSSLisenInfo;
    wxButton* Button_ShowServerLinkInfo;
    wxButton* Button_ShowTimeoutCmd;
    wxButton* Button_ShowTrackCommandID;
    wxButton* Button_ShowWorkThreadStat;
    wxButton* Button_StartFile;
    wxButton* Button_StartMonitor;
    wxButton* Button_StopFile;
    wxButton* Button_StopMonitor;
    wxButton* Button_UnloadModule;
    wxCheckBox* CheckBox_Voice;
    wxCheckBox* CheckBox_WorkThread;
    wxComboBox* ComboBox_ListenType;
    wxListBox* ListBox_ConfigList;
    wxListCtrl* ListCtr_WorkThreadTimeout;
    wxListCtrl* ListCtrl_CheckLog;
    wxListCtrl* ListCtrl_ClientTcpInfo;
    wxListCtrl* ListCtrl_CommandTimeout;
    wxListCtrl* ListCtrl_ForbidenList;
    wxListCtrl* ListCtrl_LinkHistory;
    wxListCtrl* ListCtrl_LogInfo;
    wxListCtrl* ListCtrl_ModuleInfo;
    wxListCtrl* ListCtrl_ModuleList;
    wxListCtrl* ListCtrl_NickInfo;
    wxListCtrl* ListCtrl_ServerConnect;
    wxListCtrl* ListCtrl_ServerLisen;
    wxListCtrl* ListCtrl_ServerList;
    wxListCtrl* ListCtrl_TrackIPHistory;
    wxListCtrl* ListCtrl_WorkThreadAI;
    wxListCtrl* ListCtrl_WorkThreadForbiden;
    wxListCtrl* ListCtrl_WorkThreadInfo;
    wxNotebook* Notebook1;
    wxPanel* Panel1;
    wxPanel* Panel2;
    wxPanel* Panel3;
    wxPanel* Panel4;
    wxPanel* Panel5;
    wxPanel* Panel6;
    wxPanel* Panel7;
    wxPanel* Panel8;
    wxPanel* Panel9;
    wxRadioBox* RadioBox_DebugState;
    wxRadioBox* RadioBox_MonitorServer;
    wxRadioBox* RadioBoxm_TimeForbiden;
    wxStaticBox* StaticBox10;
    wxStaticBox* StaticBox11;
    wxStaticBox* StaticBox12;
    wxStaticBox* StaticBox13;
    wxStaticBox* StaticBox14;
    wxStaticBox* StaticBox1;
    wxStaticBox* StaticBox2;
    wxStaticBox* StaticBox3;
    wxStaticBox* StaticBox4;
    wxStaticBox* StaticBox5;
    wxStaticBox* StaticBox6;
    wxStaticBox* StaticBox7;
    wxStaticBox* StaticBox8;
    wxStaticBox* StaticBox9;
    wxStaticText* StaticText10;
    wxStaticText* StaticText11;
    wxStaticText* StaticText12;
    wxStaticText* StaticText13;
    wxStaticText* StaticText14;
    wxStaticText* StaticText15;
    wxStaticText* StaticText16;
    wxStaticText* StaticText17;
    wxStaticText* StaticText18;
    wxStaticText* StaticText19;
    wxStaticText* StaticText1;
    wxStaticText* StaticText20;
    wxStaticText* StaticText21;
    wxStaticText* StaticText22;
    wxStaticText* StaticText23;
    wxStaticText* StaticText24;
    wxStaticText* StaticText25;
    wxStaticText* StaticText26;
    wxStaticText* StaticText27;
    wxStaticText* StaticText28;
    wxStaticText* StaticText29;
    wxStaticText* StaticText2;
    wxStaticText* StaticText30;
    wxStaticText* StaticText31;
    wxStaticText* StaticText32;
    wxStaticText* StaticText33;
    wxStaticText* StaticText34;
    wxStaticText* StaticText35;
    wxStaticText* StaticText36;
    wxStaticText* StaticText37;
    wxStaticText* StaticText38;
    wxStaticText* StaticText39;
    wxStaticText* StaticText3;
    wxStaticText* StaticText40;
    wxStaticText* StaticText41;
    wxStaticText* StaticText42;
    wxStaticText* StaticText4;
    wxStaticText* StaticText5;
    wxStaticText* StaticText6;
    wxStaticText* StaticText7;
    wxStaticText* StaticText8;
    wxStaticText* StaticText9;
    wxTextCtrl* TextCtr_TrackConnectID;
    wxTextCtrl* TextCtr_TrackIP;
    wxTextCtrl* TextCtrl_CheckTime;
    wxTextCtrl* TextCtrl_ClientID;
    wxTextCtrl* TextCtrl_ConnectID;
    wxTextCtrl* TextCtrl_CurrClientCount;
    wxTextCtrl* TextCtrl_CurrCpuOccupy;
    wxTextCtrl* TextCtrl_CurrDebugStat;
    wxTextCtrl* TextCtrl_CurrFlowIn;
    wxTextCtrl* TextCtrl_CurrFlowOut;
    wxTextCtrl* TextCtrl_CurrMemOccupy;
    wxTextCtrl* TextCtrl_ForbidenIP;
    wxTextCtrl* TextCtrl_ForbidenSeconds;
    wxTextCtrl* TextCtrl_IPInfo;
    wxTextCtrl* TextCtrl_LinkUpperLimit;
    wxTextCtrl* TextCtrl_LisenIP;
    wxTextCtrl* TextCtrl_LisenPort;
    wxTextCtrl* TextCtrl_LogLevel;
    wxTextCtrl* TextCtrl_ModuleFilename;
    wxTextCtrl* TextCtrl_ModuleID;
    wxTextCtrl* TextCtrl_ModuleName;
    wxTextCtrl* TextCtrl_ModuleParam;
    wxTextCtrl* TextCtrl_ModulePath;
    wxTextCtrl* TextCtrl_NickName;
    wxTextCtrl* TextCtrl_PacketFile;
    wxTextCtrl* TextCtrl_PacketParseID;
    wxTextCtrl* TextCtrl_ServerID;
    wxTextCtrl* TextCtrl_ServerIP;
    wxTextCtrl* TextCtrl_ServerInfo;
    wxTextCtrl* TextCtrl_ServerKey;
    wxTextCtrl* TextCtrl_ServerPort;
    wxTextCtrl* TextCtrl_ThreadCount;
    wxTextCtrl* TextCtrl_TracCommandCount;
    wxTextCtrl* TextCtrl_TrackCommandID;
    wxTextCtrl* TextCtrl_TrackIPCount;
    wxTextCtrl* TextCtrl_WorkThreadCheckTime;
    wxTextCtrl* TextCtrl_WorkThreadDisposeTime;
    wxTextCtrl* TextCtrl_WorkThreadStopTime;
    wxTimer Timer_WorkThread;
    //*)

    DECLARE_EVENT_TABLE()

private:
    void ChangeConfigFile();
    void LoadConfigFile();
    void ShowConfigList();

public:

    void SetTcpClientConnect(CTcpClientConnect* pTcpClientConnect);
    void CheckWorkThread(_CheckServerInfo* pCheckServerInfo);
    void BeepAlarm();
    void ShowGroupServerList();
    void ClearCheckServerInfo();

private:
    bool SendSetLog(int nConnectID, bool blFlag);


private:
    bool               m_blTimeRun;

    //集群监控参数
    vecCheckServerInfo m_vecCheckServerInfo;
    vecCheckServer     m_vecCheckServer;


private:
    CXmlOpeation       m_objXmlOpeation;
    vecConfigInfo      m_vecConfigInfo;

private:
    void Init();

    CTcpClientConnect    m_TcpClientConnect;
    CTcpClientConnect* m_pTcpClientConnect;

};

#endif // PSS_CLIENTMANAGERMAIN_H
