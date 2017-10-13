// DlgClientModule.cpp : implementation file
//

#include "stdafx.h"
#include "PSS_ClientManager.h"
#include "DlgClientModule.h"

IMPLEMENT_DYNAMIC(CDlgClientModule, CDialog)

CDlgClientModule::CDlgClientModule(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgClientModule::IDD, pParent)
{
    m_pTcpClientConnect = NULL;
}

CDlgClientModule::~CDlgClientModule()
{
}

void CDlgClientModule::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT1, m_txtModuleID);
    DDX_Control(pDX, IDC_LIST1, m_lcModuleInfo);
    DDX_Control(pDX, IDC_LIST2, m_lcCommandTimeout);
}


BEGIN_MESSAGE_MAP(CDlgClientModule, CDialog)
    ON_BN_CLICKED(IDC_BUTTON2, &CDlgClientModule::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON1, &CDlgClientModule::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON8, &CDlgClientModule::OnBnClickedButton8)
    ON_BN_CLICKED(IDC_BUTTON9, &CDlgClientModule::OnBnClickedButton9)
    ON_BN_CLICKED(IDC_BUTTON10, &CDlgClientModule::OnBnClickedButton10)
END_MESSAGE_MAP()

CString CDlgClientModule::GetPageTitle()
{
    return _T("客户端主模块运行信息");
}


// CDlgClientModule message handlers

void CDlgClientModule::OnBnClickedButton2()
{
    //查看全部
    m_lcModuleInfo.DeleteAllItems();

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
        MessageBox(_T(MESSAGE_SENDERROR), _T(MESSAGE_TITLE_ERROR), MB_OK);
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

                wchar_t szModuleName[200] = {'\0'};
                wchar_t szCommandName[200] = {'\0'};
                CString strCommandCount;
                CString strCommandTimeCost;

                int nSrcLen = MultiByteToWideChar(CP_ACP, 0, CommandInfo.szModuleName, -1, NULL, 0);
                int nDecLen = MultiByteToWideChar(CP_ACP, 0, CommandInfo.szModuleName, -1, szModuleName, 200);

                nSrcLen = MultiByteToWideChar(CP_ACP, 0, CommandInfo.szCommandID, -1, NULL, 0);
                nDecLen = MultiByteToWideChar(CP_ACP, 0, CommandInfo.szCommandID, -1, szCommandName, 200);

                strCommandCount.Format(_T("%d"), CommandInfo.m_nCount);
                strCommandTimeCost.Format(_T("%d"), CommandInfo.m_nTimeCost);

                m_lcModuleInfo.InsertItem(nIndex, szModuleName);
                m_lcModuleInfo.SetItemText(nIndex, 1, szCommandName);
                m_lcModuleInfo.SetItemText(nIndex, 2, strCommandCount);
                m_lcModuleInfo.SetItemText(nIndex, 3, strCommandTimeCost);

                nIndex++;

                objvecCommandInfo.push_back(CommandInfo);
            }
        }
    }
}

void CDlgClientModule::OnBnClickedButton1()
{
    //查看指定命令
    CString strCommandID;
    char szCommandID[20] = {'\0'};

    m_txtModuleID.GetWindowText(strCommandID);

    int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strCommandID, strCommandID.GetLength(), NULL, 0, NULL, NULL);
    int nDecLen = WideCharToMultiByte(CP_ACP, 0, strCommandID, nSrcLen, szCommandID, 20, NULL,NULL);
    szCommandID[nDecLen] = '\0';

    if(strlen(szCommandID) == 0)
    {
        MessageBox(_T(MESSAGE_INSERT_NULL), _T(MESSAGE_TITLE_ERROR), MB_OK);
        return;
    }

    m_lcModuleInfo.DeleteAllItems();

    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s CommandInfo %s&", m_pTcpClientConnect->GetKey(), szCommandID);
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[10 * 1024] = {'\0'};
    int nRecvLen = 10 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        MessageBox(_T(MESSAGE_SENDERROR), _T(MESSAGE_TITLE_ERROR), MB_OK);
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

            wchar_t szModuleName[200] = {'\0'};
            wchar_t szCommandName[200] = {'\0'};
            CString strCommandCount;
            CString strCommandTimeCost;

            int nSrcLen = MultiByteToWideChar(CP_ACP, 0, CommandInfo.szModuleName, -1, NULL, 0);
            int nDecLen = MultiByteToWideChar(CP_ACP, 0, CommandInfo.szModuleName, -1, szModuleName, 200);

            nSrcLen = MultiByteToWideChar(CP_ACP, 0, CommandInfo.szCommandID, -1, NULL, 0);
            nDecLen = MultiByteToWideChar(CP_ACP, 0, CommandInfo.szCommandID, -1, szCommandName, 200);

            strCommandCount.Format(_T("%d"), CommandInfo.m_nCount);
            strCommandTimeCost.Format(_T("%d"), CommandInfo.m_nTimeCost);

            m_lcModuleInfo.InsertItem(nIndex, szModuleName);
            m_lcModuleInfo.SetItemText(nIndex, 1, szCommandName);
            m_lcModuleInfo.SetItemText(nIndex, 2, strCommandCount);
            m_lcModuleInfo.SetItemText(nIndex, 3, strCommandTimeCost);

            nIndex++;

            objvecCommandInfo.push_back(CommandInfo);
        }
    }
}

void CDlgClientModule::SetTcpClientConnect( CTcpClientConnect* pTcpClientConnect )
{
    m_pTcpClientConnect = pTcpClientConnect;
}

BOOL CDlgClientModule::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_lcModuleInfo.InsertColumn(0, _T("模块名称"), LVCFMT_CENTER, 200);
    m_lcModuleInfo.InsertColumn(1, _T("命令ID"), LVCFMT_CENTER, 100);
    m_lcModuleInfo.InsertColumn(2, _T("调用次数"), LVCFMT_CENTER, 100);
    m_lcModuleInfo.InsertColumn(3, _T("时间消耗"), LVCFMT_CENTER, 200);

    m_lcCommandTimeout.InsertColumn(0, _T("命令ID"), LVCFMT_CENTER, 100);
    m_lcCommandTimeout.InsertColumn(1, _T("发生时间"), LVCFMT_CENTER, 200);
    m_lcCommandTimeout.InsertColumn(2, _T("超时时间"), LVCFMT_CENTER, 100);

    return TRUE;
}

void CDlgClientModule::OnBnClickedButton8()
{
    //查看全部
    m_lcCommandTimeout.DeleteAllItems();

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
        MessageBox(_T(MESSAGE_SENDERROR), _T(MESSAGE_TITLE_ERROR), MB_OK);
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

            wchar_t szzCommandID[200] = {'\0'};
            wchar_t szzUpdateTime[200] = {'\0'};
            CString strTimeout;

            int nSrcLen = MultiByteToWideChar(CP_ACP, 0, szCommandID, -1, NULL, 0);
            int nDecLen = MultiByteToWideChar(CP_ACP, 0, szCommandID, -1, szzCommandID, 200);

            nSrcLen = MultiByteToWideChar(CP_ACP, 0, szUpdateTime, -1, NULL, 0);
            nDecLen = MultiByteToWideChar(CP_ACP, 0, szUpdateTime, -1, szzUpdateTime, 50);

            strTimeout.Format(_T("%d"),objCommandTimeOut.nTimeOutTime);

            m_lcCommandTimeout.InsertItem(nIndex, szzCommandID);
            m_lcCommandTimeout.SetItemText(nIndex, 1, szzCommandID);
            m_lcCommandTimeout.SetItemText(nIndex, 2, strTimeout);

            nIndex++;
        }
    }
}

void CDlgClientModule::OnBnClickedButton9()
{
    //清除所有超时记录
    m_lcCommandTimeout.DeleteAllItems();

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
        MessageBox(_T(MESSAGE_SENDERROR), _T(MESSAGE_TITLE_ERROR), MB_OK);
        return;
    }
    else
    {
        MessageBox(_T(MESSAGE_RESULT_SUCCESS), _T(MESSAGE_TITLE_SUCCESS), MB_OK);
        return;
    }
}

void CDlgClientModule::OnBnClickedButton10()
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
        MessageBox(_T(MESSAGE_SENDERROR), _T(MESSAGE_TITLE_ERROR), MB_OK);
        return;
    }
    else
    {
        MessageBox(_T(MESSAGE_RESULT_SUCCESS), _T(MESSAGE_TITLE_SUCCESS), MB_OK);
        return;
    }
}
