/***************************************************************
 * Name:      PurenessLineMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    smith ()
 * Created:   2019-02-22
 * Copyright: smith ()
 * License:
 **************************************************************/

#include "PurenessLineMain.h"
#include <wx/msgdlg.h>



//(*InternalHeaders(PurenessLineFrame)
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

//(*IdInit(PurenessLineFrame)
const long PurenessLineFrame::ID_STATICBOX1 = wxNewId();
const long PurenessLineFrame::ID_STATICBOX2 = wxNewId();
const long PurenessLineFrame::ID_STATICTEXT1 = wxNewId();
const long PurenessLineFrame::ID_TEXTCTRL1 = wxNewId();
const long PurenessLineFrame::ID_STATICTEXT2 = wxNewId();
const long PurenessLineFrame::ID_TEXTCTRL2 = wxNewId();
const long PurenessLineFrame::ID_BUTTON1 = wxNewId();
const long PurenessLineFrame::ID_BUTTON2 = wxNewId();
const long PurenessLineFrame::ID_PANEL1 = wxNewId();
const long PurenessLineFrame::ID_TIMER1 = wxNewId();
const long PurenessLineFrame::ID_TIMER2 = wxNewId();
//*)

const long PurenessLineFrame::ID_CHARTPANEL_CPU = wxNewId();
const long PurenessLineFrame::ID_CHARTPANEL_MEM = wxNewId();

BEGIN_EVENT_TABLE(PurenessLineFrame,wxFrame)
    //(*EventTable(PurenessLineFrame)
    //*)

    EVT_CHARTVIEWER_VIEWPORT_CHANGED(ID_CHARTPANEL_CPU, PurenessLineFrame::OnViewPortChanged)
    EVT_CHARTVIEWER_VIEWPORT_CHANGED(ID_CHARTPANEL_MEM, PurenessLineFrame::OnViewPortChanged)
END_EVENT_TABLE()

PurenessLineFrame::PurenessLineFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(PurenessLineFrame)
    Create(parent, id, wxT("PurenessLine Client"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("id"));
    SetClientSize(wxSize(780,493));
    {
    	wxIcon FrameIcon;
    	FrameIcon.CopyFromBitmap(wxBitmap(wxImage(_T("/data/develop/Lib/Network/PSS-master/Icon.jpg"))));
    	SetIcon(FrameIcon);
    }
    Panel1 = new wxPanel(this, ID_PANEL1, wxPoint(216,72), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    StaticBox1 = new wxStaticBox(Panel1, ID_STATICBOX1, wxT("CPU监控"), wxPoint(8,8), wxSize(536,224), 0, _T("ID_STATICBOX1"));
    StaticBox2 = new wxStaticBox(Panel1, ID_STATICBOX2, wxT("内存监控"), wxPoint(8,240), wxSize(536,248), 0, _T("ID_STATICBOX2"));
    StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT1, wxT("服务器IP地址"), wxPoint(552,24), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    TextCtrl_ServerIP = new wxTextCtrl(Panel1, ID_TEXTCTRL1, wxEmptyString, wxPoint(640,16), wxSize(128,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    TextCtrl_ServerIP->SetMaxLength(16);
    StaticText2 = new wxStaticText(Panel1, ID_STATICTEXT2, wxT("服务器端口"), wxPoint(552,72), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    TextCtrl_ServerPort = new wxTextCtrl(Panel1, ID_TEXTCTRL2, wxEmptyString, wxPoint(640,64), wxSize(128,35), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
    TextCtrl_ServerPort->SetMaxLength(5);
    Button_StartMonitor = new wxButton(Panel1, ID_BUTTON1, wxT("开始监听"), wxPoint(552,120), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    Button_StopMonitor = new wxButton(Panel1, ID_BUTTON2, wxT("取消监听"), wxPoint(680,120), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    Timer_chartUpdateTimer.SetOwner(this, ID_TIMER1);
    Timer_chartUpdateTimer.Start(1000, false);
    Timer_dataRateTimer.SetOwner(this, ID_TIMER2);
    Timer_dataRateTimer.Start(1000, false);

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PurenessLineFrame::OnButton_StartMonitorClick);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&PurenessLineFrame::OnButton_StopMonitorClick);
    Connect(ID_TIMER1,wxEVT_TIMER,(wxObjectEventFunction)&PurenessLineFrame::OnTimer_chartUpdateTimerTrigger);
    Connect(ID_TIMER2,wxEVT_TIMER,(wxObjectEventFunction)&PurenessLineFrame::OnTimer_dataRateTimerTrigger);
    //*)

    this->Button_StartMonitor->Enable(true);
    this->Button_StopMonitor->Enable(false);


    InitializeSocketEnvironment();

    m_sckServer =new CSockWrap(SOCK_STREAM);


    m_cvCPU = new wxChartViewer(Panel1, ID_CHARTPANEL_CPU, wxPoint(16,32),wxSize(520,194), wxTAB_TRAVERSAL|wxNO_BORDER);
    m_cvCPU->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);

    m_cvMemory = new wxChartViewer(Panel1, ID_CHARTPANEL_MEM, wxPoint(16,264),wxSize(520,218), wxTAB_TRAVERSAL|wxNO_BORDER);
    m_cvMemory->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    // itemBoxSizer8->Add(m_chartViewer, 1, wxGROW | wxALL, 3);


    // Set up the data acquisition mechanism. In this demo, we just use a timer to get a
    // sample every 250ms.
    this->Timer_dataRateTimer.Start(gs_dataInterval);

    // Set up the chart update timer
    this->Timer_chartUpdateTimer.Start(1000);


    this->Init();
}

PurenessLineFrame::~PurenessLineFrame()
{
    //(*Destroy(PurenessLineFrame)
    //*)
    Timer_chartUpdateTimer.Stop();
    Timer_dataRateTimer.Stop();

    if (m_cvCPU != NULL)
    {
        delete m_cvCPU;
    }
    if (m_cvMemory != NULL)
    {
        delete m_cvMemory;
    }

    if (m_sckServer != NULL)
    {
        delete m_sckServer;
    }

    FreeSocketEnvironment();
}

void PurenessLineFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void PurenessLineFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, wxT("Welcome to..."));
}

void PurenessLineFrame::OnButton_StartMonitorClick(wxCommandEvent& event)
{



    std::string    strServerIP    =std::string(this->TextCtrl_ServerIP->GetValue());
    int nPort = wxAtoi(this->TextCtrl_ServerPort->GetValue());

    if(strServerIP == wxT("") || nPort == 0)
    {
        wxString msg = wxT("请输入完整的服务器IP地址和端口信息。");
        wxMessageBox(msg, wxT("链接信息"), wxOK | wxICON_INFORMATION, this);
        return;
    }


    m_sckServer->SetAddress(strServerIP.c_str(), nPort);
    m_sckServer->Reopen(true);
    if  (GetLastSocketError() !=0)
    {
        wxString msg = wxT("无法链接指定的服务器。");
        wxMessageBox(msg, wxT("链接信息"), wxOK | wxICON_INFORMATION, this);
        return  ;
    }

    m_blState = true;

    this->Button_StartMonitor->Enable(false);
    this->Button_StopMonitor->Enable(true);


    //OnRunFreezeChanged(true);

}

void PurenessLineFrame::OnButton_StopMonitorClick(wxCommandEvent& event)
{
    this->Button_StartMonitor->Enable(true);
    this->Button_StopMonitor->Enable(false);
    //OnRunFreezeChanged(false);

}

void PurenessLineFrame::OnTimer_dataRateTimerTrigger(wxTimerEvent& event)
{


}

void PurenessLineFrame::OnTimer_chartUpdateTimerTrigger(wxTimerEvent& event)
{
    m_cvCPU->updateViewPort(true, true);
}



void PurenessLineFrame::Init()
{
    m_blState = false;

    //初始化列表
    for(int i = 0; i < MAX_DATA_COUNT; i++)
    {
        m_dbCPU[i]    = 0.0;
        m_dbMemory[i] = 0.0;
    }

    //设置随机种子
    InitRandom();

    //绘制相应的数据图表
    drawChart();
}

bool PurenessLineFrame::GetServerStateInfo(_ServerRunInfo& objServerRunInfo)
{
    if(m_blState == false)
    {
        return false;
    }

    //拼接发送协议
    char szSendDate[100] = {'\0'};
    char szCommand[100]  = {'\0'};
   // size_t stSendEnd = 0;

    sprintf(szCommand,  "ShowCurrProcessInfo -a");
    int nAllLen = (int)strlen(szCommand);
    memcpy(&szSendDate[0], (const void*)&nAllLen, sizeof(int));
    memcpy(&szSendDate[4], (const void*)&szCommand, nAllLen);

    //发送数据
    //int nSend = m_sckServer.Send(szSendDate, nAllLen + 4);
    transresult_t   rt  =m_sckServer->Send(szSendDate, nAllLen + 4);

    if  (rt.nresult !=0)
    {
        wxString msg = wxT("向服务器发送查询指令失败！！！");
        //wxMessageBox(msg, wxT("发送查询指令"), wxOK | wxICON_INFORMATION, this);
        return  false;
    }

    int nSend = rt.nbytes;
    if(nSend != nAllLen + 4)
    {
        return false;
    }

    //接受数据
    char szBuffRecv[200] = {'\0'};
    //m_sckServer.Receive(szBuffRecv, 200);
    m_sckServer->Recv(szBuffRecv, 200);

    if  (rt.nresult !=0)
    {
        wxString msg = wxT("接收服务器数据失败！！！");
        //wxMessageBox(msg, wxT("接收数据"), wxOK | wxICON_INFORMATION, this);
        return  false;
    }


   // int nStrLen              = 0;
    int nPos                 = 4;
   // int nClientCount         = 0;
    //包处理线程
    memcpy(&objServerRunInfo.m_nCPU, &szBuffRecv[nPos], sizeof(int));
    nPos += sizeof(int);
    memcpy(&objServerRunInfo.m_nMemorySize, &szBuffRecv[nPos], sizeof(int));
    nPos += sizeof(int);

    return true;


}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void PurenessLineFrame::OnViewPortChanged(wxCommandEvent& event)
{
    _ServerRunInfo objServerRunInfo;
    if(GetServerStateInfo(objServerRunInfo) == TRUE)
    {
        //设置数据
        //double dbData = (double)RandomValue(1, 100);
        shiftData(m_dbCPU, MAX_DATA_COUNT, (double)objServerRunInfo.m_nCPU/100.0);

        //dbData = (double)RandomValue(1, 100);
        shiftData(m_dbMemory, MAX_DATA_COUNT, (double)objServerRunInfo.m_nMemorySize/(1000*1000));

        //绘制相应的数据图表
        drawChart();
    }
    else
    {
        //设置数据
        //double dbData = 0.0;
        shiftData(m_dbCPU, MAX_DATA_COUNT, (double)objServerRunInfo.m_nCPU/100.0);

        //dbData = 0.0;
        shiftData(m_dbMemory, MAX_DATA_COUNT, (double)objServerRunInfo.m_nMemorySize/(1000*1000));

        //绘制相应的数据图表
        drawChart();
    }
}




void PurenessLineFrame::drawChart()
{
    const char* szLabels[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24" };

    XYChart* pXYCPU    = NULL;
    XYChart* pXYMemory = NULL;

    //设置默认窗体大小(CPU)
    pXYCPU = new XYChart(520, 194, 0xeeeeff, 0x000000, 1);
    pXYCPU->setRoundedFrame();

    //m_cvCPU = new wxChartViewer(Panel1, ID_CHARTPANEL_CPU, wxPoint(16,32),wxSize(520,194), wxTAB_TRAVERSAL|wxNO_BORDER);
    //m_cvMemory = new wxChartViewer(Panel1, ID_CHARTPANEL_MEM, wxPoint(16,264),wxSize(520,218), wxTAB_TRAVERSAL|wxNO_BORDER);

    //设置X轴下标
    pXYCPU->xAxis()->setLabels(StringArray(szLabels, (int)(sizeof(szLabels) / sizeof(szLabels[0]))));

    //设置位置
    pXYCPU->setPlotArea(12, 18, 520, 194, 0x000200, -1, -1, 0x337f59);

    //获得一个新的线层
    LineLayer* pLayer = pXYCPU->addLineLayer();

    if(NULL != pLayer)
    {
        pLayer->setLineWidth(2);
        pLayer->addDataSet(DoubleArray(m_dbCPU, (int)(sizeof(m_dbCPU) / sizeof(m_dbCPU[0]))), 0xff0000, "Server #1");
    }

    //将制定的Chart对象绑定给指定控件
    m_cvCPU->setChart(pXYCPU);
    delete pXYCPU;

    //设置默认窗体大小(Memory)
    pXYMemory = new XYChart(520, 218, 0xeeeeff, 0x000000, 1);
    pXYMemory->setRoundedFrame();

    //设置X轴下标
    pXYMemory->xAxis()->setLabels(StringArray(szLabels, (int)(sizeof(szLabels) / sizeof(szLabels[0]))));

    //设置位置
    pXYMemory->setPlotArea(12, 18, 520, 194, 0x000200, -1, -1, 0x337f59);

    //获得一个新的线层
    pLayer = pXYMemory->addLineLayer();

    if(NULL != pLayer)
    {
        pLayer->setLineWidth(2);
        pLayer->addDataSet(DoubleArray(m_dbMemory, (int)(sizeof(m_dbMemory) / sizeof(m_dbMemory[0]))), 0xff0000, "Server #1");
    }

    //将制定的Chart对象绑定给指定控件
    m_cvMemory->setChart(pXYMemory);
    delete pXYMemory;
}

void PurenessLineFrame::shiftData(double *data, int len, double newValue)
{
    memmove(data, data + 1, sizeof(*data) * (len - 1));
    data[len - 1] = newValue;
}





















