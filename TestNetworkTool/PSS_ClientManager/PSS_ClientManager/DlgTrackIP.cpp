// DlgTrackIP.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PSS_ClientManager.h"
#include "DlgTrackIP.h"


// CDlgTrackIP �Ի���

IMPLEMENT_DYNAMIC(CDlgTrackIP, CDialog)

CDlgTrackIP::CDlgTrackIP(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgTrackIP::IDD, pParent)
{

}

CDlgTrackIP::~CDlgTrackIP()
{
}

void CDlgTrackIP::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT1, m_txtTrackIP);
    DDX_Control(pDX, IDC_LIST1, m_lcTrackIPHistory);
    DDX_Control(pDX, IDC_EDIT2, m_txtConnectID);
    DDX_Control(pDX, IDC_EDIT7, m_txtIPInfo);
    DDX_Control(pDX, IDC_EDIT13, m_txtTrackIPCount);
    DDX_Control(pDX, IDC_EDIT15, m_txtTraceCommandID);
    DDX_Control(pDX, IDC_EDIT16, m_txtTracCommandCount);
}


BEGIN_MESSAGE_MAP(CDlgTrackIP, CDialog)
    ON_BN_CLICKED(IDC_BUTTON1, &CDlgTrackIP::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &CDlgTrackIP::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON8, &CDlgTrackIP::OnBnClickedButton8)
    ON_BN_CLICKED(IDC_BUTTON6, &CDlgTrackIP::OnBnClickedButton6)
END_MESSAGE_MAP()

CString CDlgTrackIP::GetPageTitle()
{
    return _T("����Ⱦɫ");
}

void CDlgTrackIP::SetTcpClientConnect( CTcpClientConnect* pTcpClientConnect )
{
    m_pTcpClientConnect = pTcpClientConnect;
}

// CDlgTrackIP ��Ϣ�������

BOOL CDlgTrackIP::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_lcTrackIPHistory.InsertColumn(0, _T("����ID"), LVCFMT_CENTER, 150);
    m_lcTrackIPHistory.InsertColumn(1, _T("��ǰ��Ⱦɫ����"), LVCFMT_CENTER, 100);
    m_lcTrackIPHistory.InsertColumn(2, _T("���Ⱦɫ����"), LVCFMT_CENTER, 100);

    m_txtTrackIP.SetWindowText(_T("127.0.0.1"));
    m_txtTrackIPCount.SetWindowText(_T("1"));
    m_txtTraceCommandID.SetWindowText(_T("0x1000"));
    m_txtTracCommandCount.SetWindowText(_T("1"));

    DWORD dwStyle = m_lcTrackIPHistory.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
    dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
    m_lcTrackIPHistory.SetExtendedStyle(dwStyle); //������չ���

    return TRUE;
}

void CDlgTrackIP::OnBnClickedButton1()
{
    //����IP
    char szIP[20]      = {'\0'};
    char szIPCount[10] = { '\0' };
    CString strIP;
    CString strIPCount;

    m_txtTrackIP.GetWindowText(strIP);
    m_txtTrackIPCount.GetWindowText(strIPCount);

    int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strIP, strIP.GetLength(), NULL, 0, NULL, NULL);
    int nDecLen = WideCharToMultiByte(CP_ACP, 0, strIP, nSrcLen, szIP, 20, NULL,NULL);
    szIP[nDecLen] = '\0';

    nSrcLen = WideCharToMultiByte(CP_ACP, 0, strIP, strIPCount.GetLength(), NULL, 0, NULL, NULL);
    nDecLen = WideCharToMultiByte(CP_ACP, 0, strIPCount, nSrcLen, szIPCount, 20, NULL, NULL);
    szIPCount[nDecLen] = '\0';

    if(strlen(szIP) == 0)
    {
        MessageBox(_T(MESSAGE_INSERT_NULL), _T(MESSAGE_TITLE_ERROR), MB_OK);
        return;
    }

    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s SetTrackIP -i %s -c %s &", m_pTcpClientConnect->GetKey(), szIP, szIPCount);
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
        memcpy_s(&nResult, sizeof(char), &szRecvBuff[nPos], sizeof(char));
        nPos += sizeof(char);

        if(nResult == 0)
        {
            MessageBox(_T(MESSAGE_RESULT_SUCCESS), _T(MESSAGE_TITLE_ERROR), MB_OK);
        }
        else
        {
            MessageBox(_T(MESSAGE_RESULT_FAIL), _T(MESSAGE_TITLE_SUCCESS), MB_OK);
        }
    }
}

void CDlgTrackIP::OnBnClickedButton2()
{
    //����IP
    char szCommandID[20] = { '\0' };
    char szCommandIDCount[10] = { '\0' };
    CString strCommandID;
    CString strCommandIDCount;

    m_txtTraceCommandID.GetWindowText(strCommandID);
    m_txtTracCommandCount.GetWindowText(strCommandIDCount);

    int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strCommandID, strCommandID.GetLength(), NULL, 0, NULL, NULL);
    int nDecLen = WideCharToMultiByte(CP_ACP, 0, strCommandID, nSrcLen, szCommandID, 20, NULL, NULL);
    szCommandID[nDecLen] = '\0';

    nSrcLen = WideCharToMultiByte(CP_ACP, 0, strCommandIDCount, strCommandIDCount.GetLength(), NULL, 0, NULL, NULL);
    nDecLen = WideCharToMultiByte(CP_ACP, 0, strCommandIDCount, nSrcLen, szCommandIDCount, 20, NULL, NULL);
    szCommandIDCount[nDecLen] = '\0';

    if (strlen(szCommandID) == 0)
    {
        MessageBox(_T(MESSAGE_INSERT_NULL), _T(MESSAGE_TITLE_ERROR), MB_OK);
        return;
    }

    char szSendMessage[200] = { '\0' };
    char szCommand[100] = { '\0' };
    sprintf_s(szCommand, 100, "%s SetTrackCommand -i %s -c %s &", m_pTcpClientConnect->GetKey(), szCommandID, szCommandIDCount);
    int nSendLen = (int)strlen(szCommand);

    memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

    char szRecvBuff[10 * 1024] = { '\0' };
    int nRecvLen = 10 * 1024;
    bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);

    if (blState == false)
    {
        MessageBox(_T(MESSAGE_SENDERROR), _T(MESSAGE_TITLE_ERROR), MB_OK);
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
            MessageBox(_T(MESSAGE_RESULT_SUCCESS), _T(MESSAGE_TITLE_ERROR), MB_OK);
        }
        else
        {
            MessageBox(_T(MESSAGE_RESULT_FAIL), _T(MESSAGE_TITLE_SUCCESS), MB_OK);
        }
    }
}

void CDlgTrackIP::OnBnClickedButton8()
{
    //��ʾ����Ⱦɫ״̬
    m_lcTrackIPHistory.DeleteAllItems();

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
        MessageBox(_T(MESSAGE_SENDERROR), _T(MESSAGE_TITLE_ERROR), MB_OK);
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
            //��ʼ��ԭ���ݽṹ
            short nCommandID = 0;
            short nCurrCount = 0;
            short nMaxCount  = 0;

            //�������ID
            memcpy_s(&nCommandID, sizeof(short), &szRecvBuff[nPos], sizeof(short));
            nPos += sizeof(short);

            //��õ�ǰ��Ⱦɫ������
            memcpy_s(&nCurrCount, sizeof(short), &szRecvBuff[nPos], sizeof(short));
            nPos += sizeof(short);

            //�����ҪȾɫ������
            memcpy_s(&nMaxCount, sizeof(short), &szRecvBuff[nPos], sizeof(short));
            nPos += sizeof(short);

            //��ʾ��������
            CString strCommandID;
            CString strCurrCount;
            CString strMaxCount;

            strCommandID.Format(_T("%04x"), nCommandID);
            strCommandID = (CString)"0x" + strCommandID;

            strCurrCount.Format(_T("%d"), nCurrCount);
            strMaxCount.Format(_T("%d"), nMaxCount);

            m_lcTrackIPHistory.InsertItem(i, strCommandID);
            m_lcTrackIPHistory.SetItemText(i, 1, strCurrCount);
            m_lcTrackIPHistory.SetItemText(i, 2, strMaxCount);
        }
    }
}

void CDlgTrackIP::OnBnClickedButton6()
{
    //����ConnectID�����ص�����

    //�õ�ConnectID
    int nConnectID = 0;
    CString strData;

    m_txtConnectID.GetWindowText(strData);

    nConnectID = _ttoi(strData);

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
        MessageBox(_T(MESSAGE_SENDERROR), _T(MESSAGE_TITLE_ERROR), MB_OK);
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
            m_txtIPInfo.SetWindowText(_T(""));
            MessageBox(_T("��ǰConnectID��������Ϣ��"), _T(MESSAGE_TITLE_ERROR), MB_OK);
        }
        else
        {
            char szIP[20]         = {'\0'};
            int  nPort            = 0;

            memcpy_s(&nStrLen, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            //���IP��ַ
            memcpy_s(szIP, nStrLen, &szRecvBuff[nPos], nStrLen);
            nPos += nStrLen;
            szIP[nStrLen] = '\0';

            //�õ��˿�
            memcpy_s(&nPort, sizeof(int), &szRecvBuff[nPos], sizeof(int));
            nPos += sizeof(int);

            char szData[100] = {'\0'};
            sprintf_s(szData, 100, "%s:%d", szIP, nPort);

            wchar_t swzIP[200]         = {'\0'};

            int nSrcLen = MultiByteToWideChar(CP_ACP, 0, szData, -1, NULL, 0);
            int nDecLen = MultiByteToWideChar(CP_ACP, 0, szData, -1, swzIP, 200);

            m_txtIPInfo.SetWindowText(swzIP);
        }

    }
}
