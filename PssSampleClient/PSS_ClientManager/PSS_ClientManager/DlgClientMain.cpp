// DlgClientMain.cpp : implementation file
//

#include "stdafx.h"
#include "PSS_ClientManager.h"
#include "DlgClientMain.h"


// CDlgClientMain dialog

IMPLEMENT_DYNAMIC(CDlgClientMain, CDialog)

CDlgClientMain::CDlgClientMain(CWnd* pParent /*=NULL*/)
    : CDialog(CDlgClientMain::IDD, pParent)
{
    m_pTcpClientConnect = NULL;
}

CDlgClientMain::~CDlgClientMain()
{
}

void CDlgClientMain::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT1, m_txtServerIP);
    DDX_Control(pDX, IDC_EDIT2, m_txtServerPort);
    DDX_Control(pDX, IDC_EDIT3, m_txtServerInfo);
    DDX_Control(pDX, IDC_EDIT4, m_txtModuleName);
    DDX_Control(pDX, IDC_LIST1, m_lcModuleList);
    DDX_Control(pDX, IDC_EDIT6, m_txtKey);
    DDX_Control(pDX, IDC_EDIT8, m_txtModuleFile);
    DDX_Control(pDX, IDC_LIST3, m_lbConfig);
    DDX_Control(pDX, IDC_EDIT11, m_txtModuleParam);
    DDX_Control(pDX, IDC_EDIT12, m_txtModuleFileName);
}


BEGIN_MESSAGE_MAP(CDlgClientMain, CDialog)
    ON_BN_CLICKED(IDC_BUTTON1, &CDlgClientMain::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &CDlgClientMain::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON3, &CDlgClientMain::OnBnClickedButton3)
    ON_BN_CLICKED(IDC_BUTTON4, &CDlgClientMain::OnBnClickedButton4)
    ON_BN_CLICKED(IDC_BUTTON5, &CDlgClientMain::OnBnClickedButton5)
    ON_BN_CLICKED(IDC_BUTTON7, &CDlgClientMain::OnBnClickedButton7)
    ON_BN_CLICKED(IDC_BUTTON11, &CDlgClientMain::OnBnClickedButton11)
    ON_LBN_SELCHANGE(IDC_LIST3, &CDlgClientMain::OnLbnSelchangeList3)
    ON_BN_CLICKED(IDC_BUTTON12, &CDlgClientMain::OnBnClickedButton12)
    ON_NOTIFY(NM_CLICK, IDC_LIST1, &CDlgClientMain::OnNMClickList1)
END_MESSAGE_MAP()

CString CDlgClientMain::GetPageTitle()
{
    return _T("客户端主模块管理");
}


// CDlgClientMain message handlers

void CDlgClientMain::OnBnClickedButton1()
{
    // TODO: Add your control notification handler code here
    CString strServerIP;
    CString strServerPort;
    CString strConsoleKey;

    char szSeverIP[20]     = {'\0'};
    char szServerPort[20]  = {'\0'};
    char szConsoleKey[100] = {'\0'};

    m_txtServerIP.GetWindowText(strServerIP);
    m_txtServerPort.GetWindowText(strServerPort);
    m_txtKey.GetWindowText(strConsoleKey);

    int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strServerIP, strServerIP.GetLength(), NULL, 0, NULL, NULL);
    int nDecLen = WideCharToMultiByte(CP_ACP, 0, strServerIP, nSrcLen, szSeverIP, 20, NULL,NULL);
    szSeverIP[nDecLen] = '\0';

    nSrcLen = WideCharToMultiByte(CP_ACP, 0, strServerPort, strServerPort.GetLength(), NULL, 0, NULL, NULL);
    nDecLen = WideCharToMultiByte(CP_ACP, 0, strServerPort, nSrcLen, szServerPort, 20, NULL,NULL);
    szServerPort[nDecLen] = '\0';

    nSrcLen = WideCharToMultiByte(CP_ACP, 0, strConsoleKey, strConsoleKey.GetLength(), NULL, 0, NULL, NULL);
    nDecLen = WideCharToMultiByte(CP_ACP, 0, strConsoleKey, nSrcLen, szConsoleKey, 100, NULL,NULL);
    szConsoleKey[nDecLen] = '\0';

    if(strlen(szSeverIP) == 0 || strlen(szServerPort) == 0  || strlen(szConsoleKey) == 0)
    {
        MessageBox(_T(MESSAGE_INSERT_NULL), _T(MESSAGE_TITLE_ERROR), MB_OK);
        return;
    }

    int nPort = atoi(szServerPort);

    m_pTcpClientConnect->Init(szSeverIP, nPort, szConsoleKey);

    MessageBox(_T(MESSAGE_TITLE_CONNECT), _T(MESSAGE_TITLE_SUCCESS), MB_OK);
}

void CDlgClientMain::SetTcpClientConnect( CTcpClientConnect* pTcpClientConnect )
{
    m_pTcpClientConnect = pTcpClientConnect;
}

void CDlgClientMain::OnBnClickedButton2()
{
    // TODO: Add your control notification handler code here
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
        MessageBox(_T(MESSAGE_SENDERROR), _T(MESSAGE_TITLE_ERROR), MB_OK);
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
            sprintf_s(szCharOrder, "小端");
        }
        else
        {
            sprintf_s(szCharOrder, "大端");
        }

        char szByteOrder[20] = {'\0'};

        if(nByteOrder == 0)
        {
            sprintf_s(szByteOrder, "主机字节序");
        }
        else
        {
            sprintf_s(szByteOrder, "网络字节序");
        }

        char szText[2048] = {'\0'};
        sprintf_s(szText, "服务器ID:%d.\r\n服务器名称:%s.\r\n服务器版本:%s.\r\n服务器加载模块数:%d.\r\n服务器工作线程数:%d\r\n服务器包解析版本号:%s\r\n服务器大小端:%s\r\n服务器字符序:%s.",
                  nSerevrID, szServerName, szServerVersion, nModuleCount, nWorkThreadCount, szPacketVersion, szCharOrder, szByteOrder);

        //显示内容
        wchar_t szTemp[2048] = {'\0'};

        int nSrcLen = MultiByteToWideChar(CP_ACP, 0, szText, -1, NULL, 0);
        int nDecLen = MultiByteToWideChar(CP_ACP, 0, szText, -1, szTemp, 2048);
        szTemp[nDecLen] = '\0';
        m_txtServerInfo.SetWindowText((LPCTSTR)szTemp);
    }

}

BOOL CDlgClientMain::OnInitDialog()
{
    CDialog::OnInitDialog();

    //m_txtServerIP.SetWindowText(_T("127.0.0.1"));
    //m_txtServerPort.SetWindowText(_T("10010"));
    //m_txtKey.SetWindowText(_T("freeeyes"));

    OnBnClickedButton11();

    OnLbnSelchangeList3();

    m_lcModuleList.InsertColumn(0, _T("当前已加载模块"), LVCFMT_CENTER, 120);
    m_lcModuleList.InsertColumn(1, _T("模块名称"), LVCFMT_CENTER, 80);
    m_lcModuleList.InsertColumn(2, _T("模块路径"), LVCFMT_CENTER, 80);
    m_lcModuleList.InsertColumn(3, _T("模块参数"), LVCFMT_CENTER, 80);
    m_lcModuleList.InsertColumn(4, _T("模块描述"), LVCFMT_CENTER, 80);
    m_lcModuleList.InsertColumn(5, _T("加载时间"), LVCFMT_CENTER, 150);
    m_lcModuleList.InsertColumn(6, _T("当前状态"), LVCFMT_CENTER, 80);
    m_lcModuleList.InsertColumn(7, _T("状态ID"), LVCFMT_CENTER, 80);

    DWORD dwStyle = m_lcModuleList.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
    m_lcModuleList.SetExtendedStyle(dwStyle); //设置扩展风格

    CString strServerIP;
    CString strServerPort;
    CString strConsoleKey;

    char szSeverIP[20] = { '\0' };
    char szServerPort[20] = { '\0' };
    char szConsoleKey[100] = { '\0' };

    m_txtServerIP.GetWindowText(strServerIP);
    m_txtServerPort.GetWindowText(strServerPort);
    m_txtKey.GetWindowText(strConsoleKey);

    int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strServerIP, strServerIP.GetLength(), NULL, 0, NULL, NULL);
    int nDecLen = WideCharToMultiByte(CP_ACP, 0, strServerIP, nSrcLen, szSeverIP, 20, NULL, NULL);
    szSeverIP[nDecLen] = '\0';

    nSrcLen = WideCharToMultiByte(CP_ACP, 0, strServerPort, strServerPort.GetLength(), NULL, 0, NULL, NULL);
    nDecLen = WideCharToMultiByte(CP_ACP, 0, strServerPort, nSrcLen, szServerPort, 20, NULL, NULL);
    szServerPort[nDecLen] = '\0';

    nSrcLen = WideCharToMultiByte(CP_ACP, 0, strConsoleKey, strConsoleKey.GetLength(), NULL, 0, NULL, NULL);
    nDecLen = WideCharToMultiByte(CP_ACP, 0, strConsoleKey, nSrcLen, szConsoleKey, 100, NULL, NULL);
    szConsoleKey[nDecLen] = '\0';

    int nPort = atoi(szServerPort);

    m_pTcpClientConnect->Init(szSeverIP, nPort, szConsoleKey);


    return TRUE;
}

void CDlgClientMain::OnBnClickedButton3()
{
    // TODO: Add your control notification handler code here
    m_lcModuleList.DeleteAllItems();

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
        MessageBox(_T(MESSAGE_SENDERROR), _T(MESSAGE_TITLE_ERROR), MB_OK);
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
            wchar_t szModuleFile[200]    = {'\0'};
            wchar_t szModuleName[200]    = {'\0'};
            wchar_t szModulePath[200]    = {'\0'};
            wchar_t szModuleParam[200]   = {'\0'};
            wchar_t szModuleDesc[200]    = {'\0'};
            wchar_t szModuleTime[200]    = {'\0'};

            int nSrcLen = MultiByteToWideChar(CP_ACP, 0, ModuleInfo.szModuleFile, -1, NULL, 0);
            int nDecLen = MultiByteToWideChar(CP_ACP, 0, ModuleInfo.szModuleFile, -1, szModuleFile, 200);

            nSrcLen = MultiByteToWideChar(CP_ACP, 0, ModuleInfo.szModuleName, -1, NULL, 0);
            nDecLen = MultiByteToWideChar(CP_ACP, 0, ModuleInfo.szModuleName, -1, szModuleName, 200);

            nSrcLen = MultiByteToWideChar(CP_ACP, 0, ModuleInfo.szModulePath, -1, NULL, 0);
            nDecLen = MultiByteToWideChar(CP_ACP, 0, ModuleInfo.szModulePath, -1, szModulePath, 200);

            nSrcLen = MultiByteToWideChar(CP_ACP, 0, ModuleInfo.szModuleParam, -1, NULL, 0);
            nDecLen = MultiByteToWideChar(CP_ACP, 0, ModuleInfo.szModuleParam, -1, szModuleParam, 200);

            nSrcLen = MultiByteToWideChar(CP_ACP, 0, ModuleInfo.szModuleDesc, -1, NULL, 0);
            nDecLen = MultiByteToWideChar(CP_ACP, 0, ModuleInfo.szModuleDesc, -1, szModuleDesc, 200);

            nSrcLen = MultiByteToWideChar(CP_ACP, 0, ModuleInfo.szModuleCreateDate, -1, NULL, 0);
            nDecLen = MultiByteToWideChar(CP_ACP, 0, ModuleInfo.szModuleCreateDate, -1, szModuleTime, 200);

            m_lcModuleList.InsertItem(i, szModuleFile);
            m_lcModuleList.SetItemText(i, 1, szModuleName);
            m_lcModuleList.SetItemText(i, 2, szModulePath);
            m_lcModuleList.SetItemText(i, 3, szModuleParam);
            m_lcModuleList.SetItemText(i, 4, szModuleDesc);
            m_lcModuleList.SetItemText(i, 5, szModuleTime);

            CString strData;

            if(ModuleInfo.nModuleState == 0)
            {
                m_lcModuleList.SetItemText(i, 6, _T("正常"));
            }
            else
            {
                m_lcModuleList.SetItemText(i, 6, _T("异常"));
            }

            strData.Format(_T("%d"), ModuleInfo.nModuleID);
            m_lcModuleList.SetItemText(i, 7, strData);

            objvecModuleInfo.push_back(ModuleInfo);
        }
    }
}

void CDlgClientMain::OnBnClickedButton4()
{
    //卸载模块
    CString strModuleName;

    char szModuleName[100]        = {'\0'};

    m_txtModuleName.GetWindowText(strModuleName);

    int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strModuleName, strModuleName.GetLength(), NULL, 0, NULL, NULL);
    int nDecLen = WideCharToMultiByte(CP_ACP, 0, strModuleName, nSrcLen, szModuleName, 100, NULL,NULL);
    szModuleName[nDecLen] = '\0';

    if(strlen(szModuleName) == 0)
    {
        MessageBox(_T(MESSAGE_INSERT_NULL), _T(MESSAGE_TITLE_ERROR), MB_OK);
        return;
    }

    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s UnLoadModule %s&", m_pTcpClientConnect->GetKey(), szModuleName);
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
        int nStrLen      = 0;
        int nPos         = 0;
        int nResult      = 0;
        memcpy_s(&nResult, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);

        if(nResult == 0)
        {
            MessageBox(_T(MESSAGE_RESULT_SUCCESS), _T(MESSAGE_TITLE_ERROR), MB_OK);
        }
        else
        {
            MessageBox(_T(MESSAGE_RESULT_FAIL), _T(MESSAGE_TITLE_SUCCESS), MB_OK);
        }
    }

    OnBnClickedButton3();
}

void CDlgClientMain::OnBnClickedButton5()
{
    //重载模块
    CString strModuleName;

    char szModuleName[100]        = {'\0'};

    m_txtModuleName.GetWindowText(strModuleName);

    int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strModuleName, strModuleName.GetLength(), NULL, 0, NULL, NULL);
    int nDecLen = WideCharToMultiByte(CP_ACP, 0, strModuleName, nSrcLen, szModuleName, 100, NULL,NULL);
    szModuleName[nDecLen] = '\0';

    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s ReLoadModule %s&", m_pTcpClientConnect->GetKey(), szModuleName);
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
        int nStrLen      = 0;
        int nPos         = 0;
        int nResult      = 0;
        memcpy_s(&nResult, sizeof(int), &szRecvBuff[nPos], sizeof(int));
        nPos += sizeof(int);

        if(nResult == 0)
        {
            MessageBox(_T(MESSAGE_RESULT_SUCCESS), _T(MESSAGE_TITLE_ERROR), MB_OK);
        }
        else
        {
            MessageBox(_T(MESSAGE_RESULT_FAIL), _T(MESSAGE_TITLE_SUCCESS), MB_OK);
        }
    }

    OnBnClickedButton3();
}

void CDlgClientMain::OnBnClickedButton7()
{
    //加载指定模块
    CString strModulePath;
    CString strModuleFile;
    CString strModuleParam;

    char szModulePath[100]        = {'\0'};
    char szModuleFile[100]        = {'\0'};
    char szModuleParam[200]       = {'\0'};

    m_txtModuleFile.GetWindowText(strModulePath);
    m_txtModuleFileName.GetWindowText(strModuleFile);
    m_txtModuleParam.GetWindowText(strModuleParam);

    int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strModulePath, strModulePath.GetLength(), NULL, 0, NULL, NULL);
    int nDecLen = WideCharToMultiByte(CP_ACP, 0, strModulePath, nSrcLen, szModulePath, 100, NULL,NULL);
    szModulePath[nDecLen] = '\0';

    nSrcLen = WideCharToMultiByte(CP_ACP, 0, strModuleFile, strModuleFile.GetLength(), NULL, 0, NULL, NULL);
    nDecLen = WideCharToMultiByte(CP_ACP, 0, strModuleFile, nSrcLen, szModuleFile, 100, NULL,NULL);
    szModuleFile[nDecLen] = '\0';

    nSrcLen = WideCharToMultiByte(CP_ACP, 0, strModuleParam, strModuleParam.GetLength(), NULL, 0, NULL, NULL);
    nDecLen = WideCharToMultiByte(CP_ACP, 0, strModuleParam, nSrcLen, szModuleParam, 100, NULL,NULL);
    szModuleParam[nDecLen] = '\0';

    char szSendMessage[200] = {'\0'};
    char szCommand[100]     = {'\0'};
    sprintf_s(szCommand, 100, "%s LoadModule %s,%s,%s&", m_pTcpClientConnect->GetKey(), szModulePath, szModuleFile, szModuleParam);
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
        int nStrLen      = 0;
        int nPos         = 0;
        int nResult      = 0;
        memcpy_s(&nResult, sizeof(int), &szRecvBuff[nPos], sizeof(char));
        nPos += sizeof(int);

        if(nResult == 0)
        {
            MessageBox(_T(MESSAGE_RESULT_SUCCESS), _T(MESSAGE_TITLE_ERROR), MB_OK);
        }
        else
        {
            MessageBox(_T(MESSAGE_RESULT_FAIL), _T(MESSAGE_TITLE_SUCCESS), MB_OK);
        }
    }

    OnBnClickedButton3();
}

void CDlgClientMain::OnBnClickedButton11()
{
    //从配置文件加载
    m_vecConfigInfo.clear();

    bool blState = m_objXmlOpeation.Init(CONFIG_NAME);

    if(false == blState)
    {
        MessageBox(_T("读取配置文件错误，请确认当前目录下Config.xml文件存在"), _T(MESSAGE_TITLE_ERROR), MB_OK);
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

    m_lbConfig.SetCurSel(0);
}

void CDlgClientMain::ShowConfigList()
{
    m_lbConfig.ResetContent();
    wchar_t szTemp[1024] = {'\0'};

    for(int i = 0; i < (int)m_vecConfigInfo.size(); i++)
    {
        int nSrcLen = MultiByteToWideChar(CP_ACP, 0, m_vecConfigInfo[i].m_szServerName, strlen(m_vecConfigInfo[i].m_szServerName), NULL, 0);
        int nDecLen = MultiByteToWideChar(CP_ACP, 0, m_vecConfigInfo[i].m_szServerName, nSrcLen, szTemp, 1024);
        szTemp[nDecLen] = '\0';

        m_lbConfig.InsertString(i, szTemp);
    }
}
void CDlgClientMain::OnLbnSelchangeList3()
{
    //设置相关属性
    int nSel = m_lbConfig.GetCurSel();

    if(nSel < 0)
    {
        return;
    }

    wchar_t szTemp[1024] = {'\0'};

    int nSrcLen = MultiByteToWideChar(CP_ACP, 0, m_vecConfigInfo[nSel].m_szIP, strlen(m_vecConfigInfo[nSel].m_szIP), NULL, 0);
    int nDecLen = MultiByteToWideChar(CP_ACP, 0, m_vecConfigInfo[nSel].m_szIP, nSrcLen, szTemp, 1024);
    szTemp[nDecLen] = '\0';
    m_txtServerIP.SetWindowText(szTemp);

    CString strData;
    strData.Format(_T("%d"), m_vecConfigInfo[nSel].m_nPort);
    m_txtServerPort.SetWindowText(strData);

    nSrcLen = MultiByteToWideChar(CP_ACP, 0, m_vecConfigInfo[nSel].m_szMagicCode, strlen(m_vecConfigInfo[nSel].m_szMagicCode), NULL, 0);
    nDecLen = MultiByteToWideChar(CP_ACP, 0, m_vecConfigInfo[nSel].m_szMagicCode, nSrcLen, szTemp, 1024);
    szTemp[nDecLen] = '\0';
    m_txtKey.SetWindowText(szTemp);

}


void CDlgClientMain::OnBnClickedButton12()
{
    // TODO: 关闭服务器
    if(MessageBox(_T(MESSAGE_IS_CLOSE_SERVER), _T(MESSAGE_TITLE_INFO), MB_OKCANCEL) == IDOK)
    {
        char szSendMessage[200] = {'\0'};
        char szCommand[100]     = {'\0'};
        sprintf_s(szCommand, 100, "%s ServerClose -a&", m_pTcpClientConnect->GetKey());
        int nSendLen = (int)strlen(szCommand);

        memcpy_s(&szSendMessage, 200, &szCommand, nSendLen);

        char szRecvBuff[10 * 1024] = {'\0'};
        int nRecvLen = 10 * 1024;
        m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen, (char*)szRecvBuff, nRecvLen);
        MessageBox(_T(MESSAGE_IS_CLOSE_OVER), _T(MESSAGE_TITLE_INFO), MB_OK);
    }
}




void CDlgClientMain::OnNMClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

    POSITION ps = m_lcModuleList.GetFirstSelectedItemPosition();
    int nIndex  = m_lcModuleList.GetNextSelectedItem(ps);

    if(nIndex != -1)
    {
        CString strModulePath;
        CString strModuleName;
        CString strModuleFile;
        CString strModuleParam;

        m_txtModuleFile.SetWindowText(m_lcModuleList.GetItemText(nIndex, 2));
        m_txtModuleFileName.SetWindowText(m_lcModuleList.GetItemText(nIndex, 0));
        m_txtModuleName.SetWindowText(m_lcModuleList.GetItemText(nIndex, 1));
        m_txtModuleParam.SetWindowText(m_lcModuleList.GetItemText(nIndex, 3));
    }

    *pResult = 0;
}
