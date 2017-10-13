// DlgWorkThreadAI.cpp : 实现文件
//

#include "stdafx.h"
#include "PSS_ClientManager.h"
#include "DlgWorkThreadAI.h"


// CDlgWorkThreadAI 对话框

IMPLEMENT_DYNAMIC(CDlgWorkThreadAI, CDialog)

CDlgWorkThreadAI::CDlgWorkThreadAI(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgWorkThreadAI::IDD, pParent)
{

}

CDlgWorkThreadAI::~CDlgWorkThreadAI()
{
}

void CDlgWorkThreadAI::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_lcWorkThreadAI);
    DDX_Control(pDX, IDC_LIST2, m_lcTimeout);
    DDX_Control(pDX, IDC_LIST4, m_lcForbiden);
    DDX_Control(pDX, IDC_EDIT1, m_txtDisposeTime);
    DDX_Control(pDX, IDC_EDIT2, m_txtCheckTime);
    DDX_Control(pDX, IDC_EDIT3, m_txtStopTime);
    DDX_Control(pDX, IDC_CHECK1, m_btnAI);
}


BEGIN_MESSAGE_MAP(CDlgWorkThreadAI, CDialog)
    ON_BN_CLICKED(IDC_BUTTON1, &CDlgWorkThreadAI::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &CDlgWorkThreadAI::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON3, &CDlgWorkThreadAI::OnBnClickedButton3)
END_MESSAGE_MAP()

CString CDlgWorkThreadAI::GetPageTitle()
{
    return _T("工作线程AI相关");
}

void CDlgWorkThreadAI::SetTcpClientConnect( CTcpClientConnect* pTcpClientConnect )
{
    m_pTcpClientConnect = pTcpClientConnect;
}

// CDlgWorkThreadAI 消息处理程序

void CDlgWorkThreadAI::OnBnClickedButton1()
{
    //得到所有工作进程的AI配置
    m_lcWorkThreadAI.DeleteAllItems();

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
        MessageBox(_T(MESSAGE_SENDERROR), _T(MESSAGE_TITLE_ERROR), MB_OK);
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
            CString strData;
            strData.Format(_T("%d"), nThreadID);
            m_lcWorkThreadAI.InsertItem(i, strData);

            if(nAI == 0)
            {
                strData = _T("AI关闭");
                m_lcWorkThreadAI.SetItemText(i, 1, strData);
            }
            else
            {
                strData = _T("AI运行中");
                m_lcWorkThreadAI.SetItemText(i, 1, strData);
            }

            strData.Format(_T("%d"), nDisposeTime);
            m_lcWorkThreadAI.SetItemText(i, 2, strData);
            strData.Format(_T("%d"), nWTCheckTime);
            m_lcWorkThreadAI.SetItemText(i, 3, strData);
            strData.Format(_T("%d"), nWTTimeoutCount);
            m_lcWorkThreadAI.SetItemText(i, 4, strData);
            strData.Format(_T("%d"), nWTStopTime);
            m_lcWorkThreadAI.SetItemText(i, 5, strData);
        }
    }
}

BOOL CDlgWorkThreadAI::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_lcWorkThreadAI.InsertColumn(0, _T("线程ID"), LVCFMT_CENTER, 100);
    m_lcWorkThreadAI.InsertColumn(1, _T("AI开关"), LVCFMT_CENTER, 100);
    m_lcWorkThreadAI.InsertColumn(2, _T("处理超时时间"), LVCFMT_CENTER, 100);
    m_lcWorkThreadAI.InsertColumn(3, _T("判断时间区间"), LVCFMT_CENTER, 100);
    m_lcWorkThreadAI.InsertColumn(4, _T("区间内超时次数"), LVCFMT_CENTER, 100);
    m_lcWorkThreadAI.InsertColumn(5, _T("命令中断时间"), LVCFMT_CENTER, 100);

    m_lcTimeout.InsertColumn(0, _T("线程ID"), LVCFMT_CENTER, 50);
    m_lcTimeout.InsertColumn(1, _T("命令ID"), LVCFMT_CENTER, 50);
    m_lcTimeout.InsertColumn(2, _T("时间"), LVCFMT_CENTER, 120);
    m_lcTimeout.InsertColumn(3, _T("执行时间"), LVCFMT_CENTER, 50);

    m_lcForbiden.InsertColumn(0, _T("线程ID"), LVCFMT_CENTER, 50);
    m_lcForbiden.InsertColumn(1, _T("命令ID"), LVCFMT_CENTER, 50);
    m_lcForbiden.InsertColumn(2, _T("到期时间"), LVCFMT_CENTER, 120);

    DWORD dwStyle = m_lcWorkThreadAI.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
    m_lcWorkThreadAI.SetExtendedStyle(dwStyle); //设置扩展风格

    dwStyle = m_lcTimeout.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
    m_lcTimeout.SetExtendedStyle(dwStyle); //设置扩展风格

    dwStyle = m_lcForbiden.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
    m_lcForbiden.SetExtendedStyle(dwStyle); //设置扩展风格

    m_txtDisposeTime.SetWindowText(_T("40"));
    m_txtCheckTime.SetWindowText(_T("30"));
    m_txtStopTime.SetWindowText(_T("30"));

    return TRUE;
}

void CDlgWorkThreadAI::OnBnClickedButton2()
{
    //查询当前AI池中的数据
    m_lcTimeout.DeleteAllItems();
    m_lcForbiden.DeleteAllItems();

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
        MessageBox(_T(MESSAGE_SENDERROR), _T(MESSAGE_TITLE_ERROR), MB_OK);
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
            CString strData;
            strData.Format(_T("%d"), nThreadID);
            m_lcTimeout.InsertItem(i, strData);

            strData.Format(_T("0x%04x"), nCommandID);
            m_lcTimeout.SetItemText(i, 1, strData);
            time_t time2 = nSecond;
            struct tm tm2;
            localtime_s(&tm2, &time2);

            strData.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), tm2.tm_year + 1900,
                           tm2.tm_mon + 1, tm2.tm_mday, tm2.tm_hour, tm2.tm_min, tm2.tm_sec);
            m_lcTimeout.SetItemText(i, 2, strData);
            strData.Format(_T("%d"), nDisposeTime);
            m_lcTimeout.SetItemText(i, 3, strData);
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
            CString strData;
            strData.Format(_T("%d"), nThreadID);
            m_lcForbiden.InsertItem(i, strData);

            strData.Format(_T("0x%04x"), nCommandID);
            m_lcForbiden.SetItemText(i, 1, strData);

            time_t time2 = nSecond;
            struct tm tm2;
            localtime_s(&tm2, &time2);

            strData.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), tm2.tm_year + 1900,
                           tm2.tm_mon + 1, tm2.tm_mday, tm2.tm_hour, tm2.tm_min, tm2.tm_sec);
            m_lcForbiden.SetItemText(i, 2, strData);
        }
    }

}

void CDlgWorkThreadAI::OnBnClickedButton3()
{
    //重置工作线程AI参数
    CString strData;
    m_txtDisposeTime.GetWindowText(strData);
    int nDisposeTime = _ttoi(strData);

    if(nDisposeTime <= 0)
    {
        MessageBox(_T("处理时间不能是0"), _T(MESSAGE_TITLE_ERROR), MB_OK);
        return;
    }

    m_txtCheckTime.GetWindowText(strData);
    int nCheckTime = _ttoi(strData);

    if(nCheckTime <= 0)
    {
        MessageBox(_T("时间区间不能是0"), _T(MESSAGE_TITLE_ERROR), MB_OK);
        return;
    }

    m_txtStopTime.GetWindowText(strData);
    int nStopTime = _ttoi(strData);

    if(nStopTime <= 0)
    {
        MessageBox(_T("暂停时间不能是0"), _T(MESSAGE_TITLE_ERROR), MB_OK);
        return;
    }

    int nAI = 0;

    if(m_btnAI.GetCheck() == TRUE)
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
        MessageBox(_T(MESSAGE_SENDERROR), _T(MESSAGE_TITLE_ERROR), MB_OK);
        return;
    }
    else
    {
        int nStrLen       = 0;
        int nPos          = 0;

        MessageBox(_T("线程AI参数设置成功"), _T(MESSAGE_TITLE_SUCCESS), MB_OK);
        return;
    }

}
