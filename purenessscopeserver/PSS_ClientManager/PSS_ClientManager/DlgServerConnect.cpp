// DlgServerConnect.cpp : implementation file
//

#include "stdafx.h"
#include "PSS_ClientManager.h"
#include "DlgServerConnect.h"


// CDlgServerConnect dialog

IMPLEMENT_DYNAMIC(CDlgServerConnect, CDialog)

CDlgServerConnect::CDlgServerConnect(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgServerConnect::IDD, pParent)
{

}

CDlgServerConnect::~CDlgServerConnect()
{
}

void CDlgServerConnect::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_lcServerConnect);
    DDX_Control(pDX, IDC_EDIT1, m_txtServerID);
    DDX_Control(pDX, IDC_LIST2, m_lcServerListen);
    DDX_Control(pDX, IDC_IPADDRESS1, m_txtListenIP);
    DDX_Control(pDX, IDC_EDIT4, m_txtListenPort);
    DDX_Control(pDX, IDC_COMBO1, m_cbListenType);
    DDX_Control(pDX, IDC_EDIT13, m_txtPacketParseID);
}


BEGIN_MESSAGE_MAP(CDlgServerConnect, CDialog)
    ON_BN_CLICKED(IDC_BUTTON2, &CDlgServerConnect::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON1, &CDlgServerConnect::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON8, &CDlgServerConnect::OnBnClickedButton8)
    ON_BN_CLICKED(IDC_BUTTON10, &CDlgServerConnect::OnBnClickedButton10)
    ON_BN_CLICKED(IDC_BUTTON13, &CDlgServerConnect::OnBnClickedButton13)
END_MESSAGE_MAP()

CString CDlgServerConnect::GetPageTitle()
{
    return _T("服务器间链接信息");
}


// CDlgServerConnect message handlers

void CDlgServerConnect::OnBnClickedButton2()
{
    //查看TCP
    m_lcServerConnect.DeleteAllItems();

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
        MessageBox(_T(MESSAGE_SENDERROR), _T(MESSAGE_TITLE_ERROR), MB_OK);
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
            wchar_t szzTCPIP[50]      = {'\0'};
            wchar_t szzUpdateTime[30] = {'\0'};
            CString strConnectID;
            CString strSendPacketCount;
            CString strRecvPacketCount;
            CString strSendSize;
            CString strRecvSize;
            CString strCreateTime;
            CString strAliveSecond;
            CString strLogicCount;

            int nSrcLen = MultiByteToWideChar(CP_ACP, 0, ClientConnectInfo.m_szIP, -1, NULL, 0);
            int nDecLen = MultiByteToWideChar(CP_ACP, 0, ClientConnectInfo.m_szIP, -1, szzTCPIP, 50);

            nSrcLen = MultiByteToWideChar(CP_ACP, 0, szUpdateTime, -1, NULL, 0);
            nDecLen = MultiByteToWideChar(CP_ACP, 0, szUpdateTime, -1, szzUpdateTime, 30);

            strConnectID.Format(_T("%d"), ClientConnectInfo.m_nConnectID);
            strSendPacketCount.Format(_T("%d"), ClientConnectInfo.m_nSendCount);
            strRecvPacketCount.Format(_T("%d"), ClientConnectInfo.m_nRecvCount);
            strSendSize.Format(_T("%d"), ClientConnectInfo.m_nAllSendSize);
            strRecvSize.Format(_T("%d"), ClientConnectInfo.m_nAllRecvSize);
            strCreateTime.Format(_T("%d"), ClientConnectInfo.m_nBeginTime);
            strAliveSecond.Format(_T("%d"), ClientConnectInfo.m_nAliveTime);
            strLogicCount.Format(_T("%d"), ClientConnectInfo.m_nRecvQueueCount);

            m_lcServerConnect.InsertItem(i, strConnectID);
            m_lcServerConnect.SetItemText(i, 1, szzTCPIP);

            if(nState == 0)
            {
                m_lcServerConnect.SetItemText(i, 2, _T("链接已存在"));
            }
            else
            {
                m_lcServerConnect.SetItemText(i, 2, _T("链接不存在"));
            }

            m_lcServerConnect.SetItemText(i, 3, strSendPacketCount);
            m_lcServerConnect.SetItemText(i, 4, strRecvPacketCount);
            m_lcServerConnect.SetItemText(i, 5, strSendSize);
            m_lcServerConnect.SetItemText(i, 6, strRecvSize);
            m_lcServerConnect.SetItemText(i, 7, szzUpdateTime);
            m_lcServerConnect.SetItemText(i, 8, strAliveSecond);

            objvecClientConnectInfo.push_back(ClientConnectInfo);
        }
    }
}

void CDlgServerConnect::SetTcpClientConnect( CTcpClientConnect* pTcpClientConnect )
{
    m_pTcpClientConnect = pTcpClientConnect;
}

BOOL CDlgServerConnect::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_lcServerConnect.InsertColumn(0, _T("ID"), LVCFMT_CENTER, 100);
    m_lcServerConnect.InsertColumn(1, _T("IP地址"), LVCFMT_CENTER, 100);
    m_lcServerConnect.InsertColumn(2, _T("连接状态"), LVCFMT_CENTER, 80);
    m_lcServerConnect.InsertColumn(3, _T("发送包数"), LVCFMT_CENTER, 80);
    m_lcServerConnect.InsertColumn(4, _T("接收包数"), LVCFMT_CENTER, 80);
    m_lcServerConnect.InsertColumn(5, _T("发送字节"), LVCFMT_CENTER, 80);
    m_lcServerConnect.InsertColumn(6, _T("接收字节"), LVCFMT_CENTER, 80);
    m_lcServerConnect.InsertColumn(7, _T("创建时间"), LVCFMT_CENTER, 80);
    m_lcServerConnect.InsertColumn(8, _T("存活秒数"), LVCFMT_CENTER, 80);

    m_lcServerListen.InsertColumn(0, _T("监听IP"), LVCFMT_CENTER, 200);
    m_lcServerListen.InsertColumn(1, _T("监听端口"), LVCFMT_CENTER, 100);

    m_cbListenType.AddString(_T("IPv4"));
    m_cbListenType.AddString(_T("IPv6"));
    m_cbListenType.SetCurSel(0);
    m_txtListenPort.SetWindowText(_T("0"));

    return TRUE;
}

void CDlgServerConnect::OnBnClickedButton1()
{
    // TODO: 在此添加控件通知处理程序代码
    CString strServerID;
    char szServerIP[30] = {'\0'};
    m_txtServerID.GetWindowText(strServerID);

    int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strServerID, strServerID.GetLength(), NULL, 0, NULL, NULL);
    int nDecLen = WideCharToMultiByte(CP_ACP, 0, strServerID, nSrcLen, szServerIP, 30, NULL,NULL);

    m_lcServerConnect.DeleteAllItems();

    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s ReConnectServer -s %s&", m_pTcpClientConnect->GetKey(), szServerIP);
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
            wchar_t szzTCPIP[50]      = {'\0'};
            wchar_t szzUpdateTime[30] = {'\0'};
            CString strConnectID;
            CString strSendPacketCount;
            CString strRecvPacketCount;
            CString strSendSize;
            CString strRecvSize;
            CString strCreateTime;
            CString strAliveSecond;
            CString strLogicCount;

            int nSrcLen = MultiByteToWideChar(CP_ACP, 0, ClientConnectInfo.m_szIP, -1, NULL, 0);
            int nDecLen = MultiByteToWideChar(CP_ACP, 0, ClientConnectInfo.m_szIP, -1, szzTCPIP, 50);

            nSrcLen = MultiByteToWideChar(CP_ACP, 0, szUpdateTime, -1, NULL, 0);
            nDecLen = MultiByteToWideChar(CP_ACP, 0, szUpdateTime, -1, szzUpdateTime, 30);

            strConnectID.Format(_T("%d"), ClientConnectInfo.m_nConnectID);
            strSendPacketCount.Format(_T("%d"), ClientConnectInfo.m_nSendCount);
            strRecvPacketCount.Format(_T("%d"), ClientConnectInfo.m_nRecvCount);
            strSendSize.Format(_T("%d"), ClientConnectInfo.m_nAllSendSize);
            strRecvSize.Format(_T("%d"), ClientConnectInfo.m_nAllRecvSize);
            strCreateTime.Format(_T("%d"), ClientConnectInfo.m_nBeginTime);
            strAliveSecond.Format(_T("%d"), ClientConnectInfo.m_nAliveTime);
            strLogicCount.Format(_T("%d"), ClientConnectInfo.m_nRecvQueueCount);

            m_lcServerConnect.InsertItem(i, strConnectID);
            m_lcServerConnect.SetItemText(i, 1, szzTCPIP);

            if(nState == 0)
            {
                m_lcServerConnect.SetItemText(i, 2, _T("链接已存在"));
            }
            else
            {
                m_lcServerConnect.SetItemText(i, 2, _T("链接不存在"));
            }

            m_lcServerConnect.SetItemText(i, 3, strSendPacketCount);
            m_lcServerConnect.SetItemText(i, 4, strRecvPacketCount);
            m_lcServerConnect.SetItemText(i, 5, strSendSize);
            m_lcServerConnect.SetItemText(i, 6, strRecvSize);
            m_lcServerConnect.SetItemText(i, 7, szzUpdateTime);
            m_lcServerConnect.SetItemText(i, 8, strAliveSecond);

            objvecClientConnectInfo.push_back(ClientConnectInfo);
        }
    }

    MessageBox(_T(MESSAGE_RESULT_SUCCESS), _T(MESSAGE_TITLE_ERROR), MB_OK);
}

void CDlgServerConnect::OnBnClickedButton8()
{
    //查看所有已有的监听端口信息
    m_lcServerListen.DeleteAllItems();

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
        MessageBox(_T(MESSAGE_SENDERROR), _T(MESSAGE_TITLE_ERROR), MB_OK);
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
            wchar_t szzTCPIP[50]      = {'\0'};
            CString strPort;

            int nSrcLen = MultiByteToWideChar(CP_ACP, 0, szListenIP, -1, NULL, 0);
            int nDecLen = MultiByteToWideChar(CP_ACP, 0,szListenIP, -1, szzTCPIP, 50);

            strPort.Format(_T("%d"), nListenPort);

            m_lcServerListen.InsertItem(i, szzTCPIP);
            m_lcServerListen.SetItemText(i, 1, strPort);
        }
    }

    MessageBox(_T(MESSAGE_RESULT_SUCCESS), _T(MESSAGE_TITLE_ERROR), MB_OK);
}

void CDlgServerConnect::OnBnClickedButton10()
{
    //添加新监控
    char szIP[20] = {'\0'};
    CString strIP;
    m_txtListenIP.GetWindowText(strIP);

    int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strIP, strIP.GetLength(), NULL, 0, NULL, NULL);
    int nDecLen = WideCharToMultiByte(CP_ACP, 0, strIP, nSrcLen, szIP, 20, NULL,NULL);
    szIP[nDecLen] = '\0';

    CString strPort;
    m_txtListenPort.GetWindowText(strPort);
    int nPort = _ttoi(strPort);

    int nType = 1;

    if(m_cbListenType.GetCurSel() == 0)
    {
        nType = 1;
    }
    else
    {
        nType = 2;
    }

    CString strPacketParseID;
    m_txtPacketParseID.GetWindowText(strPacketParseID);
    int nPacketParseID = _ttoi(strPacketParseID);

    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s AddListen -i %s -p %d -t %d -n %d&", m_pTcpClientConnect->GetKey(), szIP, nPort, nType, nPacketParseID);
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
        int nStrLen       = 0;
        int nPos          = 0;
        int nResult       = 0;
        int nState        = 0;
        memcpy_s(&nResult, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);

        if(nResult == 1)
        {
            MessageBox(_T(MESSAGE_RESULT_FAIL), _T(MESSAGE_TITLE_SUCCESS), MB_OK);
        }
        else
        {
            MessageBox(_T(MESSAGE_RESULT_SUCCESS), _T(MESSAGE_TITLE_SUCCESS), MB_OK);
        }

    }
}

void CDlgServerConnect::OnBnClickedButton13()
{
    //删除一个已有监控
    char szIP[20] = {'\0'};
    CString strIP;
    m_txtListenIP.GetWindowText(strIP);

    int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strIP, strIP.GetLength(), NULL, 0, NULL, NULL);
    int nDecLen = WideCharToMultiByte(CP_ACP, 0, strIP, nSrcLen, szIP, 20, NULL,NULL);
    szIP[nDecLen] = '\0';

    CString strPort;
    m_txtListenPort.GetWindowText(strPort);
    int nPort = _ttoi(strPort);

    int nType = 1;

    if(m_cbListenType.GetCurSel() == 0)
    {
        nType = 1;
    }
    else
    {
        nType = 2;
    }

    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s DelListen -i %s -p %d -t %d &", m_pTcpClientConnect->GetKey(), szIP, nPort, nType);
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
        int nStrLen       = 0;
        int nPos          = 0;
        int nResult       = 0;
        int nState        = 0;
        memcpy_s(&nResult, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);

        if(nResult == 1)
        {
            MessageBox(_T(MESSAGE_RESULT_FAIL), _T(MESSAGE_TITLE_SUCCESS), MB_OK);
        }
        else
        {
            MessageBox(_T(MESSAGE_RESULT_SUCCESS), _T(MESSAGE_TITLE_SUCCESS), MB_OK);
        }

    }
}
