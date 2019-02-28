// PlugClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PlugClient.h"
#include "PlugClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CPlugClientDlg 对话框
//线程执行
DWORD WINAPI ThreadProc(LPVOID argv)
{
	CPlugClientDlg* pLoginClientDlg = (CPlugClientDlg *)argv;
	if(NULL != pLoginClientDlg)
	{
		pLoginClientDlg->Send_Multiple_Plug();
	}

	return 0;
}



CPlugClientDlg::CPlugClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlugClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPlugClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_txtServerIP);
	DDX_Control(pDX, IDC_EDIT2, m_txtServerPort);
	DDX_Control(pDX, IDC_EDIT3, m_txtText);
	DDX_Control(pDX, IDC_EDIT4, m_txtReturnText);
	DDX_Control(pDX, IDC_EDIT5, m_txtSendCount);
	DDX_Control(pDX, IDC_EDIT7, m_txtRecvCount);
}

BEGIN_MESSAGE_MAP(CPlugClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CPlugClientDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPlugClientDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CPlugClientDlg::OnBnClickedButton3)
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CPlugClientDlg 消息处理程序

BOOL CPlugClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	Init();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPlugClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPlugClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPlugClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPlugClientDlg::Init()
{
	m_txtServerIP.SetWindowText(_T("127.0.0.1"));
	m_txtServerPort.SetWindowText(_T("10002"));
	m_txtText.SetWindowText(_T("freeeyes，在天际，随云聚云散，梦起梦逝，雁过留影。"));

	//初始化TCP链接
	WSADATA wsaData;
	int nErr = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(nErr != 0)
	{
		MessageBox(_T("本机socket库加载失败，请检查本机socket库版本"), _T("错误信息"), MB_OK);
	}

	m_blIsRun = false;
}


void CPlugClientDlg::OnBnClickedButton1()
{
	CString strData;

	//获得相关服务器连接信息
	m_txtServerIP.GetWindowText(strData);
	int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
	int nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, m_objServerInfo.m_szServerIP, MAX_BUFF_50, NULL,NULL);
	m_objServerInfo.m_szServerIP[nDecLen] = '\0';

	m_txtServerPort.GetWindowText(strData);
	m_objServerInfo.m_nServerPort = _ttoi((LPCTSTR)strData);

	//判断socket是否已经连接，如果有连接则断开
	if(m_sckClient != INVALID_SOCKET)
	{
		Close();
	}

	if(Connect() == false)
	{
		return;
	}

	char szText[MAX_BUFF_200] = {'\0'};

	m_txtText.GetWindowText(strData);
	nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
	nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, szText, MAX_BUFF_200, NULL,NULL);
	szText[nDecLen] = '\0';

	Send_Plug(szText);


	Close();
}

bool CPlugClientDlg::Connect()
{
	//socket创建的准备工作
	struct sockaddr_in sockaddr;

	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port   = htons(m_objServerInfo.m_nServerPort);
	sockaddr.sin_addr.S_un.S_addr = inet_addr(m_objServerInfo.m_szServerIP);

	m_sckClient = socket(AF_INET, SOCK_STREAM, 0);

	DWORD TimeOut = (DWORD)1000;
	::setsockopt(m_sckClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&TimeOut, sizeof(TimeOut));

	int nErr = connect(m_sckClient, (SOCKADDR*)&sockaddr, sizeof(SOCKADDR));
	if(0 != nErr)
	{
		//连接失败
		DWORD dwError = GetLastError();
		MessageBox(_T("连接远程服务器失败"), _T("错误信息"), MB_OK);
		return false;
	}

	return true;
}

bool CPlugClientDlg::Send_Plug(const char* pText, bool blIsDisPlay)
{
	//发送数据
	int nCommand = COMMAND_PLUGA;
	char szSendBuff[MAX_BUFF_500] = {'\0'};
	if(m_sckClient == INVALID_SOCKET)
	{
		return false;
	}

	int nVersion = 1;
	int nPos = 0;
	int nLen = (int)strlen(pText) + 2;
	char szSession[32] = {'\0'};

	sprintf_s(szSession, 32, "FREEEYES");

	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nVersion, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nCommand, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nLen, sizeof(int));
	nPos += sizeof(int);
	memcpy_s(&szSendBuff[nPos], sizeof(char)*32, (char*)szSession, sizeof(char)*32);
	nPos += sizeof(char)*32;

	int nStrLen = (int)strlen(pText);
	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nStrLen, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], nStrLen, (char* )pText, nStrLen);
	nPos += nStrLen;

	int nTotalSendLen = nPos;
	int nBeginSend    = 0;
	int nCurrSendLen  = 0;
	bool blSendFlag   = false;
	int nBeginRecv    = 0;
	int nCurrRecvLen  = 0;
	bool blRecvFlag   = false;
	while(true)
	{
		nCurrSendLen = send(m_sckClient, szSendBuff + nBeginSend, nTotalSendLen, 0);
		if(nCurrSendLen <= 0)
		{
			DWORD dwError = GetLastError();
			MessageBox(_T("远程服务器发送数据失败"), _T("错误信息"), MB_OK);
			return false;
		}
		else
		{
			nTotalSendLen -= nCurrSendLen;
			if(nTotalSendLen == 0)
			{
				//发送完成
				blSendFlag = true;
				break;
			}
			else
			{
				nBeginSend += nCurrSendLen;
			}
		}
	}

	//接收返回数据
	//先接收四字节的数据包长度
	char szRecvLength[4] = {'\0'};
	nCurrRecvLen = recv(m_sckClient, (char* )szRecvLength, 4, 0);
	if(nCurrRecvLen != 4)
	{
		DWORD dwError = GetLastError();
		MessageBox(_T("远程服务器接收数据失败"), _T("错误信息"), MB_OK);
		return false;
	}

	int nRecvLength = 0;
	memcpy_s(&nRecvLength, sizeof(int), szRecvLength, sizeof(int));
	char* pRecvBuff = new char[nRecvLength];
	int nRecvBegin  = 0;

	while(true)
	{
		if(nRecvLength - nRecvBegin == 0)
		{
			break;
		}

		//如果发送成功了，则处理接收数据
		nCurrRecvLen = recv(m_sckClient, (char* )pRecvBuff + nRecvBegin, nRecvLength - nRecvBegin, 0);
		if(nCurrRecvLen <= 0)
		{
			delete[] pRecvBuff;
			DWORD dwError = GetLastError();
			MessageBox(_T("远程服务器发送数据失败"), _T("错误信息"), MB_OK);
			return false;
		}
		else
		{
			nRecvBegin += nCurrRecvLen;
		}
	}

	int nRecvCommandID = 0;
	int nRetA          = 0;
	int nRetB          = 0;
	nPos               = 0;


	memcpy_s((char*)&nRecvCommandID,  sizeof(short), &pRecvBuff[nPos], sizeof(short));
	nPos += sizeof(short);
	memcpy_s((char*)&nRetB,  sizeof(int), &pRecvBuff[nPos], sizeof(int));
	nPos += sizeof(int);
	memcpy_s((char*)&nRetA,  sizeof(int), &pRecvBuff[nPos], sizeof(int));
	nPos += sizeof(int);

	if(blIsDisPlay == true)
	{
		char    szReturnText[MAX_BUFF_50]  = {'\0'};
		wchar_t wszReturnText[MAX_BUFF_50] = {'\0'};
		sprintf_s(szReturnText, MAX_BUFF_50, "PlugA:%d,PlugB:%d.", nRetA, nRetB);

		CString steData;
		steData.Format(_T("接收回应包字节数:%d,PlugA应答:%d,PlugB应答:%d."), nRecvLength + sizeof(int), nRetA, nRetB);
		m_txtReturnText.SetWindowText(steData);
	}

	return true;
}

void CPlugClientDlg::Close()
{
	closesocket(m_sckClient);
	m_sckClient = INVALID_SOCKET;
}
void CPlugClientDlg::OnBnClickedButton2()
{
	//压测
	SetTimer(1, 1000, NULL);

	DWORD  ThreadID = 0;
	CreateThread(NULL, NULL, ThreadProc, (LPVOID)this, NULL, &ThreadID);
}

void CPlugClientDlg::OnBnClickedButton3()
{
	m_blIsRun = false;
}

bool CPlugClientDlg::Send_Multiple_Plug()
{
	//开始压测
	CString strData;

	//获得相关服务器连接信息
	m_txtServerIP.GetWindowText(strData);
	int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
	int nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, m_objServerInfo.m_szServerIP, MAX_BUFF_50, NULL,NULL);
	m_objServerInfo.m_szServerIP[nDecLen] = '\0';

	m_txtServerPort.GetWindowText(strData);
	m_objServerInfo.m_nServerPort = _ttoi((LPCTSTR)strData);

	//判断socket是否已经连接，如果有连接则断开
	if(m_sckClient != INVALID_SOCKET)
	{
		Close();
	}

	if(Connect() == false)
	{
		return false;
	}

	char szText[MAX_BUFF_200] = {'\0'};

	m_txtText.GetWindowText(strData);
	nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
	nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, szText, MAX_BUFF_200, NULL,NULL);
	szText[nDecLen] = '\0';

	m_blIsRun = true;
	m_objPassTest.Init();

	while(m_blIsRun)
	{
		bool blState = Send_Plug(szText, false);
		if(true == blState)
		{
			m_objPassTest.m_nSendCount++;
			m_objPassTest.m_nRecvCount++;
		}
	}

	Close();
	KillTimer(1);

	Show_Send_List();

	return true;
}

void CPlugClientDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(m_blIsRun == true)
	{
		m_blIsRun = false;
		Sleep(1);
	}

	CDialog::OnClose();
}

void CPlugClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case 1:
		Show_Send_List();
	}


	CDialog::OnTimer(nIDEvent);
}

void CPlugClientDlg::Show_Send_List()
{
	CString strData;

	strData.Format(_T("%d"), m_objPassTest.m_nSendCount);
	m_txtSendCount.SetWindowText(strData);
	strData.Format(_T("%d"), m_objPassTest.m_nRecvCount);
	m_txtRecvCount.SetWindowText(strData);
}