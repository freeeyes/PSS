// DlgClientConnect.cpp : implementation file
//

#include "stdafx.h"
#include "PSS_ClientManager.h"
#include "DlgClientConnect.h"


// CDlgClientConnect dialog

IMPLEMENT_DYNAMIC(CDlgClientConnect, CDialog)

CDlgClientConnect::CDlgClientConnect(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgClientConnect::IDD, pParent)
{
    m_pTcpClientConnect = NULL;
}

CDlgClientConnect::~CDlgClientConnect()
{
}

void CDlgClientConnect::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT1, m_txtClientConnectCount);
    DDX_Control(pDX, IDC_EDIT2, m_txtCpu);
    DDX_Control(pDX, IDC_EDIT5, m_txtMemory);
    DDX_Control(pDX, IDC_LIST1, m_lcClientConnectHistory);
    DDX_Control(pDX, IDC_EDIT3, m_txtDebugState);
    DDX_Control(pDX, IDC_RADIO2, m_btnDebug);
    DDX_Control(pDX, IDC_EDIT7, m_txtFlowIn);
    DDX_Control(pDX, IDC_EDIT9, m_txtFlowOut);
    DDX_Control(pDX, IDC_EDIT10, m_txtMaxConnectCount);
    DDX_Control(pDX, IDC_EDIT14, m_txtPacketFileName);
}


BEGIN_MESSAGE_MAP(CDlgClientConnect, CDialog)
    ON_BN_CLICKED(IDC_BUTTON1, &CDlgClientConnect::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON6, &CDlgClientConnect::OnBnClickedButton6)
    ON_BN_CLICKED(IDC_BUTTON5, &CDlgClientConnect::OnBnClickedButton5)
    ON_BN_CLICKED(IDC_BUTTON4, &CDlgClientConnect::OnBnClickedButton4)
    ON_BN_CLICKED(IDC_BUTTON12, &CDlgClientConnect::OnBnClickedButton12)
    ON_BN_CLICKED(IDC_BUTTON14, &CDlgClientConnect::OnBnClickedButton14)
    ON_BN_CLICKED(IDC_BUTTON15, &CDlgClientConnect::OnBnClickedButton15)
END_MESSAGE_MAP()

CString CDlgClientConnect::GetPageTitle()
{
    return _T("客户端链接管理模块");
}

void CDlgClientConnect::SetTcpClientConnect( CTcpClientConnect* pTcpClientConnect )
{
    m_pTcpClientConnect = pTcpClientConnect;
}


// CDlgClientConnect message handlers

void CDlgClientConnect::OnBnClickedButton1()
{
    // TODO: Add your control notification handler code here
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
        MessageBox(_T(MESSAGE_SENDERROR), _T(MESSAGE_TITLE_ERROR), MB_OK);
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

        CString strTemp;
        strTemp.Format(_T("%d"), nConnectCount);
        m_txtClientConnectCount.SetWindowText(strTemp);
        strTemp.Format(_T("%d"), nMaxConnectCount);
        m_txtMaxConnectCount.SetWindowText(strTemp);
    }

    //获得CPU和内存占用量
    sprintf_s(szCommand, 100, "%s ShowCurrProcessInfo -a&", m_pTcpClientConnect->GetKey());
    nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    nRecvLen = 10 * 1024;
    blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if(blState == false)
    {
        MessageBox(_T(MESSAGE_SENDERROR), _T(MESSAGE_TITLE_ERROR), MB_OK);
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

        CString strCpuRote;
        strCpuRote.Format(_T("%f"), (float)nCpuRote/100.0f);
        m_txtCpu.SetWindowText(strCpuRote);

        CString strMemoryRote;
        strMemoryRote.Format(_T("%f"), (float)nMemoryRote/(1000.0f*1000.0f));
        m_txtMemory.SetWindowText(strMemoryRote);

        if(cFlow == 0)
        {
            m_txtFlowIn.SetWindowText(_T("关闭"));
            m_txtFlowOut.SetWindowText(_T("关闭"));
        }
        else
        {
            CString strFlowIn;
            CString strFlowOut;
            strFlowIn.Format(_T("%d"), nFlowIn);
            m_txtFlowIn.SetWindowText(strFlowIn);
            strFlowOut.Format(_T("%d"), nFlowOut);
            m_txtFlowOut.SetWindowText(strFlowOut);
        }
    }

}

BOOL CDlgClientConnect::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_lcClientConnectHistory.InsertColumn(0, _T("IP地址"), LVCFMT_CENTER, 100);
    m_lcClientConnectHistory.InsertColumn(1, _T("单位时间链接次数"), LVCFMT_CENTER, 150);
    m_lcClientConnectHistory.InsertColumn(2, _T("总链接数"), LVCFMT_CENTER, 100);
    m_lcClientConnectHistory.InsertColumn(3, _T("最后链接时间"), LVCFMT_CENTER, 200);

    m_txtPacketFileName.SetWindowText(_T("FileTestCfg.xml"));

    return TRUE;
}

void CDlgClientConnect::OnBnClickedButton6()
{
    m_lcClientConnectHistory.DeleteAllItems();

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
        MessageBox(_T(MESSAGE_SENDERROR), _T(MESSAGE_TITLE_ERROR), MB_OK);
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
            wchar_t szIP[200]    = {'\0'};
            CString strIPCount;
            CString strIPAllCount;
            wchar_t szIPTime[200]    = {'\0'};

            int nSrcLen = MultiByteToWideChar(CP_ACP, 0, IPAccount.m_szIP, -1, NULL, 0);
            int nDecLen = MultiByteToWideChar(CP_ACP, 0, IPAccount.m_szIP, -1, szIP, 200);

            strIPCount.Format(_T("%d"), IPAccount.m_nCount);
            strIPAllCount.Format(_T("%d"), IPAccount.m_nAllCount);

            nSrcLen = MultiByteToWideChar(CP_ACP, 0, IPAccount.m_szDate, -1, NULL, 0);
            nDecLen = MultiByteToWideChar(CP_ACP, 0, IPAccount.m_szDate, -1, szIPTime, 200);

            m_lcClientConnectHistory.InsertItem(i, szIP);
            m_lcClientConnectHistory.SetItemText(i, 1, strIPCount);
            m_lcClientConnectHistory.SetItemText(i, 2, strIPAllCount);
            m_lcClientConnectHistory.SetItemText(i, 3, szIPTime);

            objvecIPAccount.push_back(IPAccount);
        }

    }
}

void CDlgClientConnect::OnBnClickedButton5()
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
        MessageBox(_T(MESSAGE_SENDERROR), _T(MESSAGE_TITLE_ERROR), MB_OK);
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
            m_txtDebugState.SetWindowText(_T("DEBUG日志关闭"));
        }
        else
        {
            m_txtDebugState.SetWindowText(_T("DEBUG日志打开"));
        }
    }
}

void CDlgClientConnect::OnBnClickedButton4()
{
    //设置DEBUG状态
    int nType = 0;

    switch(GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2))
    {
    case IDC_RADIO1:
        nType = 0;
        break;

    case IDC_RADIO2:
        nType = 1;
        break;
    }

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
        MessageBox(_T(MESSAGE_SENDERROR), _T(MESSAGE_TITLE_ERROR), MB_OK);
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
            MessageBox(_T(MESSAGE_RESULT_SUCCESS), _T(MESSAGE_TITLE_ERROR), MB_OK);
            OnBnClickedButton5();
        }
        else
        {
            MessageBox(_T(MESSAGE_RESULT_FAIL), _T(MESSAGE_TITLE_SUCCESS), MB_OK);
        }
    }
}

void CDlgClientConnect::OnBnClickedButton12()
{
    //重新设置上限
    int nMaxConnectCount = 0;
    CString strMaxConnectCount;
    char szMaxConnectCount[20] = {'\0'};
    m_txtMaxConnectCount.GetWindowText(strMaxConnectCount);

    int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strMaxConnectCount, strMaxConnectCount.GetLength(), NULL, 0, NULL, NULL);
    int nDecLen = WideCharToMultiByte(CP_ACP, 0, strMaxConnectCount, nSrcLen, szMaxConnectCount, 20, NULL,NULL);
    szMaxConnectCount[nDecLen] = '\0';

    nMaxConnectCount = atoi(szMaxConnectCount);

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
        MessageBox(_T(MESSAGE_SENDERROR), _T(MESSAGE_TITLE_ERROR), MB_OK);
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
            MessageBox(_T(MESSAGE_RESULT_SUCCESS), _T(MESSAGE_TITLE_ERROR), MB_OK);
        }
        else
        {
            MessageBox(_T(MESSAGE_RESULT_FAIL), _T(MESSAGE_TITLE_SUCCESS), MB_OK);
        }
    }
}


void CDlgClientConnect::OnBnClickedButton14()
{
    //启动文件
    char szPacketFileName[200] = { '\0' };
    CString strPacketFileName;
    m_txtPacketFileName.GetWindowText(strPacketFileName);

    if (strPacketFileName.GetLength() == 0)
    {
        MessageBox(_T("请输出文件路径以及文件名"), _T(MESSAGE_TITLE_ERROR), MB_OK);
        return;
    }

    int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strPacketFileName, strPacketFileName.GetLength(), NULL, 0, NULL, NULL);
    int nDecLen = WideCharToMultiByte(CP_ACP, 0, strPacketFileName, nSrcLen, szPacketFileName, 20, NULL, NULL);
    szPacketFileName[nDecLen] = '\0';

    char szSendMessage[200] = { '\0' };
    char szCommand[100] = { '\0' };
    sprintf_s(szCommand, 100, "%s TestFileStart -f %s&", m_pTcpClientConnect->GetKey(), szPacketFileName);
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[100 * 1024] = { '\0' };
    int nRecvLen = 100 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if (blState == false)
    {
        MessageBox(_T(MESSAGE_SENDERROR), _T(MESSAGE_TITLE_ERROR), MB_OK);
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
            CString strTxt;
            int nData = 0;
            wchar_t szTemp[1024]        = { '\0' };
            char szCommandDesc[1024] = { '\0' };
            memcpy_s(&nData, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);
            wsprintf(szTemp, _T("TimeInterval:[%d]&"), nData);
            strTxt += szTemp;
            memcpy_s(&nData, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);
            wsprintf(szTemp, _T("ProcolCount:[%d]&"), nData);
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
                wchar_t szDesc[1024] = { '\0' };

                int nSrcLen = MultiByteToWideChar(CP_UTF8, 0, szCommandDesc, -1, NULL, 0);
                int nDecLen = MultiByteToWideChar(CP_UTF8, 0, szCommandDesc, -1, szDesc, 1024);

                wsprintf(szTemp, _T("CommandDesc:[%s]&"), szDesc);
                strTxt += szTemp;
            }

            MessageBox(strTxt, _T(MESSAGE_TITLE_SUCCESS), MB_OK);
        }
        else
        {
            MessageBox(_T(MESSAGE_RESULT_FAIL), _T(MESSAGE_TITLE_ERROR), MB_OK);
        }
    }
}


void CDlgClientConnect::OnBnClickedButton15()
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
        MessageBox(_T(MESSAGE_SENDERROR), _T(MESSAGE_TITLE_ERROR), MB_OK);
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
            MessageBox(_T(MESSAGE_RESULT_SUCCESS), _T(MESSAGE_TITLE_ERROR), MB_OK);
        }
        else
        {
            MessageBox(_T(MESSAGE_RESULT_FAIL), _T(MESSAGE_TITLE_SUCCESS), MB_OK);
        }
    }
}
