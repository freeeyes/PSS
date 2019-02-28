// DlgClientWorkThread.cpp : implementation file
//

#include "stdafx.h"
#include "PSS_ClientManager.h"
#include "DlgClientWorkThread.h"


// CDlgClientWorkThread dialog

IMPLEMENT_DYNAMIC(CDlgClientWorkThread, CDialog)

CDlgClientWorkThread::CDlgClientWorkThread(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgClientWorkThread::IDD, pParent)
{
}

CDlgClientWorkThread::~CDlgClientWorkThread()
{
    ClearCheckServerInfo();
}

void CDlgClientWorkThread::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_lcWorkThreadInfo);
    DDX_Control(pDX, IDC_EDIT1, m_txtThreadCount);
    DDX_Control(pDX, IDC_EDIT2, m_txtCheckTime);
    DDX_Control(pDX, IDC_LIST2, m_lbCheckLog);
    DDX_Control(pDX, IDC_CHECK1, m_btnVoice);
    DDX_Control(pDX, IDC_LIST3, m_lbServerList);
}


BEGIN_MESSAGE_MAP(CDlgClientWorkThread, CDialog)
    ON_BN_CLICKED(IDC_BUTTON1, &CDlgClientWorkThread::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &CDlgClientWorkThread::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON8, &CDlgClientWorkThread::OnBnClickedButton8)
    ON_WM_TIMER()
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_BUTTON3, &CDlgClientWorkThread::OnBnClickedButton3)
    ON_BN_CLICKED(IDC_BUTTON4, &CDlgClientWorkThread::OnBnClickedButton4)
END_MESSAGE_MAP()

CString CDlgClientWorkThread::GetPageTitle()
{
    return _T("工作线程状态");
}


// CDlgClientWorkThread message handlers

void CDlgClientWorkThread::OnBnClickedButton1()
{
    //查看工作线程信息
    m_lcWorkThreadInfo.DeleteAllItems();

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
        MessageBox(_T(MESSAGE_SENDERROR), _T(MESSAGE_TITLE_ERROR), MB_OK);
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
            wchar_t szzTheadName[50]  = {'\0'};
            wchar_t szzUpdateTime[30] = {'\0'};
            wchar_t szzCreateTime[30] = {'\0'};
            CString strThreadID;
            CString strSendPacketCount;
            CString strRecvPacketCount;
            CString strCurrCommandID;
            CString strPacketTime;
            CString strAvgDisposeCount;

            int nSrcLen = MultiByteToWideChar(CP_ACP, 0, WorkThreadInfo.m_szThreadName, -1, NULL, 0);
            int nDecLen = MultiByteToWideChar(CP_ACP, 0, WorkThreadInfo.m_szThreadName, -1, szzTheadName, 50);

            nSrcLen = MultiByteToWideChar(CP_ACP, 0, szUpdateTime, -1, NULL, 0);
            nDecLen = MultiByteToWideChar(CP_ACP, 0, szUpdateTime, -1, szzUpdateTime, 50);

            nSrcLen = MultiByteToWideChar(CP_ACP, 0, szCreateTime, -1, NULL, 0);
            nDecLen = MultiByteToWideChar(CP_ACP, 0, szCreateTime, -1, szzCreateTime, 50);

            strThreadID.Format(_T("%d"), WorkThreadInfo.m_nThreadID);
            strRecvPacketCount.Format(_T("%d"), WorkThreadInfo.m_nRecvPacketCount);
            strSendPacketCount.Format(_T("%d"), WorkThreadInfo.m_nSendPacketCount);
            strCurrCommandID.Format(_T("%d"), WorkThreadInfo.m_nCommandID);
            strPacketTime.Format(_T("%d"), WorkThreadInfo.m_nPacketTime);
            strAvgDisposeCount.Format(_T("%d"), WorkThreadInfo.m_nCurrPacketCount);

            m_lcWorkThreadInfo.InsertItem(i, _T("工作线程"));
            m_lcWorkThreadInfo.SetItemText(i, 1, strThreadID);
            m_lcWorkThreadInfo.SetItemText(i, 2, szzCreateTime);
            m_lcWorkThreadInfo.SetItemText(i, 3, szzUpdateTime);
            m_lcWorkThreadInfo.SetItemText(i, 4, strRecvPacketCount);
            m_lcWorkThreadInfo.SetItemText(i, 5, strCurrCommandID);
            m_lcWorkThreadInfo.SetItemText(i, 6, strPacketTime);
            m_lcWorkThreadInfo.SetItemText(i, 7, strAvgDisposeCount);

            objvecWorkThreadInfo.push_back(WorkThreadInfo);
        }
    }
}

void CDlgClientWorkThread::SetTcpClientConnect( CTcpClientConnect* pTcpClientConnect )
{
    m_pTcpClientConnect = pTcpClientConnect;
}

BOOL CDlgClientWorkThread::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_lcWorkThreadInfo.InsertColumn(0, _T("线程名称"), LVCFMT_CENTER, 100);
    m_lcWorkThreadInfo.InsertColumn(1, _T("线程ID"), LVCFMT_CENTER, 100);
    m_lcWorkThreadInfo.InsertColumn(2, _T("线程创建时间"), LVCFMT_CENTER, 100);
    m_lcWorkThreadInfo.InsertColumn(3, _T("最后执行时间"), LVCFMT_CENTER, 100);
    m_lcWorkThreadInfo.InsertColumn(4, _T("处理包数量"), LVCFMT_CENTER, 80);
    m_lcWorkThreadInfo.InsertColumn(5, _T("当前处理ID"), LVCFMT_CENTER, 80);
    m_lcWorkThreadInfo.InsertColumn(6, _T("平均耗时"), LVCFMT_CENTER, 80);
    m_lcWorkThreadInfo.InsertColumn(7, _T("单位处理量"), LVCFMT_CENTER, 80);

    m_txtThreadCount.SetWindowText(_T("3"));
    m_txtCheckTime.SetWindowText(_T("60"));

    m_blTimeRun         = false;

    ((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(TRUE);

    return TRUE;
}

void CDlgClientWorkThread::OnBnClickedButton2()
{
    //开始监控
    ClearCheckServerInfo();

    CString strData;
    m_txtThreadCount.GetWindowText(strData);
    int nCheckThreadCount = _ttoi(strData);
    m_txtCheckTime.GetWindowText(strData);
    int nTimeInterval = _ttoi(strData);

    if(m_blTimeRun == true)
    {
        //如果正在监控，先停止上一次的监控
        KillTimer(1);
    }

    //开始监控的相关准备工作
    if(((CButton*)GetDlgItem(IDC_RADIO2))->GetCheck() == TRUE)
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


    m_lbCheckLog.ResetContent();

    SetTimer(1, nTimeInterval * 1000, NULL);
    m_blTimeRun = true;

    CString strTImeNow;

    CTime tm = CTime::GetCurrentTime();
    strTImeNow = tm.Format(_T("[%Y-%m-%d %X]"));
    strData = strTImeNow + _T("开始监控...");

    m_lbCheckLog.AddString(strData);
}

void CDlgClientWorkThread::OnBnClickedButton8()
{
    //停止监控
    KillTimer(1);
    m_blTimeRun = false;

    ClearCheckServerInfo();

    CString strData;
    CString strTImeNow;

    CTime tm = CTime::GetCurrentTime();
    strTImeNow = tm.Format(_T("[%Y-%m-%d %X]"));
    strData = strTImeNow + _T("停止监控...");

    m_lbCheckLog.AddString(strData);

    int nCount = m_lbCheckLog.GetCount();

    if (nCount > 0)
    {
        m_lbCheckLog.SetCurSel(nCount - 1);
    }
}

void CDlgClientWorkThread::OnTimer(UINT_PTR nIDEvent)
{
    //定时器
    switch(nIDEvent)
    {
    case 1:
        {
            for(int i = 0; i < (int)m_vecCheckServerInfo.size(); i++)
            {
                CheckWorkThread(m_vecCheckServerInfo[i]);
            }

            break;
        }
    }

    CDialog::OnTimer(nIDEvent);
}

void CDlgClientWorkThread::CheckWorkThread(_CheckServerInfo* pCheckServerInfo)
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
        MessageBox(_T(MESSAGE_SENDERROR), _T(MESSAGE_TITLE_ERROR), MB_OK);
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

            sprintf_s(szError, "[%s:%d]监控线程数不符，当前服务器线程数[%d]，当前监控线程数[%d]", pCheckServerInfo->m_objTcpClientConnect.GetServerIP(), pCheckServerInfo->m_objTcpClientConnect.GetServerPort(), nThreadCount, pCheckServerInfo->m_nCheckThreadCount);

            int nSrcLen = MultiByteToWideChar(CP_ACP, 0, szError, -1, NULL, 0);
            int nDecLen = MultiByteToWideChar(CP_ACP, 0, szError, -1, szzError, 1024);

            m_lbCheckLog.AddString(szzError);

            int nCount = m_lbCheckLog.GetCount();

            if (nCount > 0)
            {
                m_lbCheckLog.SetCurSel(nCount - 1);
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

                    sprintf_s(szError, "[%s:%d]原线程时间是[%s], 当前线程重启时间[%s]", pCheckServerInfo->m_objTcpClientConnect.GetServerIP(), pCheckServerInfo->m_objTcpClientConnect.GetServerPort(), szThreadTime, szCurrTime);

                    int nSrcLen = MultiByteToWideChar(CP_ACP, 0, szError, -1, NULL, 0);
                    int nDecLen = MultiByteToWideChar(CP_ACP, 0, szError, -1, szzError, 1024);

                    m_lbCheckLog.AddString(szzError);

                    int nCount = m_lbCheckLog.GetCount();

                    if (nCount > 0)
                    {
                        m_lbCheckLog.SetCurSel(nCount - 1);
                    }

                    BeepAlarm();
                }
            }
        }
    }
}

void CDlgClientWorkThread::OnClose()
{

    CDialog::OnClose();
}

void CDlgClientWorkThread::BeepAlarm()
{
    if(m_btnVoice.GetCheck() == BST_CHECKED)
    {
        for(int i = 0; i < 5; i++)
        {
            Beep(1000, 1000);
        }
    }
}
void CDlgClientWorkThread::OnBnClickedButton3()
{
    //从XML文件中加载
    m_vecCheckServer.clear();

    CXmlOpeation objXmlOpeation;

    bool blState = objXmlOpeation.Init(CHECKSERVER_FILE);

    if(false == blState)
    {
        MessageBox(_T("读取配置文件错误，请确认当前目录下Config.xml文件存在"), _T(MESSAGE_TITLE_ERROR), MB_OK);
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

void CDlgClientWorkThread::OnBnClickedButton4()
{
    //清除加载
    m_vecCheckServer.clear();

    ShowGroupServerList();
}

void CDlgClientWorkThread::ShowGroupServerList()
{
    m_lbServerList.ResetContent();

    for(int i = 0; i < (int)m_vecCheckServer.size(); i++)
    {
        char    szLog[500]  = {'\0'};
        wchar_t szzLog[500] = {'\0'};

        sprintf_s(szLog, 500, "[%s:%d]监控线程数[%d]", m_vecCheckServer[i].m_szIP, m_vecCheckServer[i].m_nPort, m_vecCheckServer[i].m_nThreadCount);

        int nSrcLen = MultiByteToWideChar(CP_ACP, 0, szLog, -1, NULL, 0);
        int nDecLen = MultiByteToWideChar(CP_ACP, 0, szLog, -1, szzLog, 500);

        m_lbServerList.AddString(szzLog);
    }

}

void CDlgClientWorkThread::ClearCheckServerInfo()
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