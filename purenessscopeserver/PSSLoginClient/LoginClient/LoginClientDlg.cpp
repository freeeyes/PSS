// LoginClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LoginClient.h"
#include "LoginClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//线程执行
DWORD WINAPI ThreadProc(LPVOID argv)
{
	CLoginClientDlg* pLoginClientDlg = (CLoginClientDlg *)argv;
	if(NULL != pLoginClientDlg)
	{
		pLoginClientDlg->Send_Multiple_Login();
	}

	return 0;
}


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


// CLoginClientDlg 对话框




CLoginClientDlg::CLoginClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLoginClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_txtServerIP);
	DDX_Control(pDX, IDC_EDIT3, m_txtServerPort);
	DDX_Control(pDX, IDC_EDIT4, m_txtUserName);
	DDX_Control(pDX, IDC_EDIT5, m_txtUserPass);
	DDX_Control(pDX, IDC_EDIT6, m_txtIDFrom);
	DDX_Control(pDX, IDC_EDIT7, m_txtIDTo);
	DDX_Control(pDX, IDC_LIST1, m_lcServer);
	DDX_Control(pDX, IDC_EDIT1, m_txtClientCost);
	DDX_Control(pDX, IDC_EDIT8, m_txtSeachUserID);
	DDX_Control(pDX, IDC_EDIT9, m_txtUserID);
	DDX_Control(pDX, IDC_EDIT10, m_txtUserLife);
	DDX_Control(pDX, IDC_EDIT11, m_txtUserMagic);
}

BEGIN_MESSAGE_MAP(CLoginClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CLoginClientDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CLoginClientDlg::OnBnClickedButton2)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON3, &CLoginClientDlg::OnBnClickedButton3)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON4, &CLoginClientDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CLoginClientDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CLoginClientDlg 消息处理程序

BOOL CLoginClientDlg::OnInitDialog()
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

	Init();

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CLoginClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLoginClientDlg::OnPaint()
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
HCURSOR CLoginClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLoginClientDlg::OnBnClickedButton1()
{
	//单一测试
	CString strData;

	//获得相关服务器连接信息
	m_txtServerIP.GetWindowText(strData);
	int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
	int nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, m_objServerInfo.m_szServerIP, MAX_BUFF_50, NULL,NULL);
	m_objServerInfo.m_szServerIP[nDecLen] = '\0';

	m_txtServerPort.GetWindowText(strData);
	m_objServerInfo.m_nServerPort = _ttoi((LPCTSTR)strData);

	//获得相关测试信息
	m_txtUserName.GetWindowText(strData);
	nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
	nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, m_objLoginClient.m_szUserName, MAX_BUFF_50, NULL,NULL);
	m_objLoginClient.m_szUserName[nDecLen] = '\0';

	m_txtUserPass.GetWindowText(strData);
	nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
	nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, m_objLoginClient.m_szUserPass, MAX_BUFF_50, NULL,NULL);
	m_objLoginClient.m_szUserPass[nDecLen] = '\0';

	m_txtIDFrom.GetWindowText(strData);
	m_objLoginClient.m_nUserIDFrom = _ttoi((LPCTSTR)strData);

	m_txtIDTo.GetWindowText(strData);
	m_objLoginClient.m_nUserIDTo = _ttoi((LPCTSTR)strData);

	//判断socket是否已经连接，如果有连接则断开
	if(m_sckClient != INVALID_SOCKET)
	{
		Close();
	}

	if(Connect() == false)
	{
		return;
	}

	Send_Single_Login();

	Close();
}

void CLoginClientDlg::OnBnClickedButton2()
{
	//压力测试
	CString strData;

	//获得相关服务器连接信息
	m_txtServerIP.GetWindowText(strData);
	int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
	int nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, m_objServerInfo.m_szServerIP, MAX_BUFF_50, NULL,NULL);
	m_objServerInfo.m_szServerIP[nDecLen] = '\0';

	m_txtServerPort.GetWindowText(strData);
	m_objServerInfo.m_nServerPort = _ttoi((LPCTSTR)strData);

	//获得相关测试信息
	m_txtUserName.GetWindowText(strData);
	nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
	nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, m_objLoginClient.m_szUserName, MAX_BUFF_50, NULL,NULL);
	m_objLoginClient.m_szUserName[nDecLen] = '\0';

	m_txtUserPass.GetWindowText(strData);
	nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
	nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, m_objLoginClient.m_szUserPass, MAX_BUFF_50, NULL,NULL);
	m_objLoginClient.m_szUserPass[nDecLen] = '\0';

	m_txtIDFrom.GetWindowText(strData);
	m_objLoginClient.m_nUserIDFrom = _ttoi((LPCTSTR)strData);

	m_txtIDTo.GetWindowText(strData);
	m_objLoginClient.m_nUserIDTo = _ttoi((LPCTSTR)strData);

	m_blMultiple = true;
	m_nSendCount = 0;

	//初始化要发送的数据
	m_vecLoginInfo.clear();
	m_lcServer.SetRedraw(FALSE);
	m_lcServer.DeleteAllItems();
	for (int i = m_objLoginClient.m_nUserIDFrom; i < m_objLoginClient.m_nUserIDTo; i++)
	{
		_LoginInfo objLoginInfo;
		sprintf_s(objLoginInfo.m_szUserName, MAX_BUFF_50, "%s%d", m_objLoginClient.m_szUserName, i);
		sprintf_s(objLoginInfo.m_szUserPass, MAX_BUFF_50, "%s", m_objLoginClient.m_szUserPass);
		m_vecLoginInfo.push_back(objLoginInfo);

		// 插入新项
		wchar_t wszUserName[MAX_BUFF_50] = { '\0' };
		wchar_t wszUserPass[MAX_BUFF_50] = { '\0' };

		int nSrcLen = MultiByteToWideChar(CP_ACP, 0, objLoginInfo.m_szUserName, -1, NULL, 0);
		int nDecLen = MultiByteToWideChar(CP_ACP, 0, objLoginInfo.m_szUserName, -1, wszUserName, MAX_BUFF_50);

		nSrcLen = MultiByteToWideChar(CP_ACP, 0, objLoginInfo.m_szUserPass, -1, NULL, 0);
		nDecLen = MultiByteToWideChar(CP_ACP, 0, objLoginInfo.m_szUserPass, -1, wszUserPass, MAX_BUFF_50);

		int itemIndex = m_lcServer.GetItemCount();
		m_lcServer.InsertItem(itemIndex, wszUserName);
		m_lcServer.SetItemText(itemIndex, 1, wszUserPass);
		m_lcServer.SetItemText(itemIndex, 2, L"0");
		m_lcServer.SetItemText(itemIndex, 3, L"0");
		m_lcServer.SetItemText(itemIndex, 4, L"0");
	}
	m_lcServer.SetRedraw(TRUE);

	DWORD  ThreadID = 0;
	CreateThread(NULL, NULL, ThreadProc, (LPVOID)this, NULL, &ThreadID);
}

void CLoginClientDlg::Init()
{
	m_txtServerIP.SetWindowText(_T("127.0.0.1"));
	m_txtServerPort.SetWindowText(_T("10002"));
	m_txtUserName.SetWindowText(_T("freeeyes"));
	m_txtUserPass.SetWindowText(_T("123456"));
	m_txtIDFrom.SetWindowText(_T("1"));
	m_txtIDTo.SetWindowText(_T("5"));
	m_txtSeachUserID.SetWindowText(_T("1001"));

	m_lcServer.InsertColumn(0, _T("用户名"), LVCFMT_CENTER, 100);
	m_lcServer.InsertColumn(1, _T("密码"), LVCFMT_CENTER, 100);
	m_lcServer.InsertColumn(2, _T("发送次数"), LVCFMT_CENTER, 100);
	m_lcServer.InsertColumn(3, _T("成功"), LVCFMT_CENTER, 100);
	m_lcServer.InsertColumn(4, _T("失败"), LVCFMT_CENTER, 100);

	DWORD dwStyle = m_lcServer.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	//dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	dwStyle |= LVS_EX_DOUBLEBUFFER; // 使用双缓冲
	m_lcServer.SetExtendedStyle(dwStyle); //设置扩展风格

	//初始化TCP链接
	WSADATA wsaData;
	int nErr = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(nErr != 0)
	{
		MessageBox(_T("本机socket库加载失败，请检查本机socket库版本"), _T("错误信息"), MB_OK);
	}

	m_blMultiple = false;
	m_nSendCount = 0;

	srand(unsigned(time(0)));
}

bool CLoginClientDlg::Connect()
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

void CLoginClientDlg::Close()
{
	closesocket(m_sckClient);
	m_sckClient = INVALID_SOCKET;
}

void CLoginClientDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//判断socket是否已经连接，如果有连接则断开
	if(m_blMultiple == true)
	{
		m_blMultiple = false; 
		Sleep(1);
	}

	if(m_sckClient != INVALID_SOCKET)
	{
		Close();
	}

	CDialog::OnClose();
}

bool CLoginClientDlg::Send_Login(_LoginInfo& objLoginInfo)
{
	int nCommand = COMMAND_LOGIN;
	char szSendBuff[MAX_BUFF_500] = {'\0'};
	if(m_sckClient == INVALID_SOCKET)
	{
		return false;
	}

	short sVersion = 1;
	int nPos = 0;
	int nLen = 1 + 1 + (int)strlen(m_objLoginClient.m_szUserName) + (int)strlen(m_objLoginClient.m_szUserPass);
	char szSession[32] = {'\0'};
	sprintf_s(szSession, 32, "FREEETES");

	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&sVersion, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nCommand, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nLen, sizeof(int));
	nPos += sizeof(int);
	memcpy_s(&szSendBuff[nPos], sizeof(char)*32, (char*)szSession, sizeof(char)*32);
	nPos += sizeof(char)*32;

	int nStrLen = (int)strlen(m_objLoginClient.m_szUserName);
	memcpy_s(&szSendBuff[nPos], sizeof(char), (char*)&nStrLen, sizeof(char));
	nPos += sizeof(char);
	memcpy_s(&szSendBuff[nPos], nStrLen, (char*)m_objLoginClient.m_szUserName, nStrLen);
	nPos += nStrLen;

	nStrLen = (int)strlen(m_objLoginClient.m_szUserPass);
	memcpy_s(&szSendBuff[nPos], sizeof(char), (char*)&nStrLen, sizeof(char));
	nPos += sizeof(char);
	memcpy_s(&szSendBuff[nPos], nStrLen, (char*)m_objLoginClient.m_szUserPass, nStrLen);
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
			objLoginInfo.m_nServerFail++;
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

	objLoginInfo.m_nSendCount++;

	//先接收四字节的数据包长度
	char szRecvLength[4] = {'\0'};
	nCurrRecvLen = recv(m_sckClient, (char* )szRecvLength, 4, 0);
	if(nCurrRecvLen != 4)
	{
		DWORD dwError = GetLastError();
		MessageBox(_T("远程服务器接收数据失败"), _T("错误信息"), MB_OK);
		objLoginInfo.m_nServerFail++;
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
			objLoginInfo.m_nServerFail++;
			return false;
		}
		else
		{
			nRecvBegin += nCurrRecvLen;
		}
	}

	int nRecvCommandID = 0;
	int nRet           = 0;
	nPos               = 0;

	memcpy_s((char*)&nRecvCommandID,  sizeof(short), &pRecvBuff[nPos], sizeof(short));
	nPos += sizeof(short);
	memcpy_s((char*)&nRet,  sizeof(int), &pRecvBuff[nPos], sizeof(int));
	nPos += sizeof(int);

	if(nRet == LOGIN_SUCCESS)
	{
		objLoginInfo.m_nServerSuccess++;
	}
	else 
	{
		objLoginInfo.m_nServerFail++;
	}

	delete[] pRecvBuff;

	return true;
}

bool CLoginClientDlg::Send_Single_Login()
{
	m_vecLoginInfo.clear();

	_LoginInfo objLoginInfo;

	sprintf_s(objLoginInfo.m_szUserName, MAX_BUFF_50, "%s", m_objLoginClient.m_szUserName);
	sprintf_s(objLoginInfo.m_szUserPass, MAX_BUFF_50, "%s", m_objLoginClient.m_szUserPass);

	m_vecLoginInfo.push_back(objLoginInfo);

	// 插入新项
	m_lcServer.SetRedraw(FALSE);
	m_lcServer.DeleteAllItems();

	wchar_t wszUserName[MAX_BUFF_50] = { '\0' };
	wchar_t wszUserPass[MAX_BUFF_50] = { '\0' };
	int nSrcLen = MultiByteToWideChar(CP_ACP, 0, objLoginInfo.m_szUserName, -1, NULL, 0);
	int nDecLen = MultiByteToWideChar(CP_ACP, 0, objLoginInfo.m_szUserName, -1, wszUserName, MAX_BUFF_50);
	nSrcLen = MultiByteToWideChar(CP_ACP, 0, objLoginInfo.m_szUserPass, -1, NULL, 0);
	nDecLen = MultiByteToWideChar(CP_ACP, 0, objLoginInfo.m_szUserPass, -1, wszUserPass, MAX_BUFF_50);

	m_lcServer.InsertItem(0, wszUserName);
	m_lcServer.SetItemText(0, 1, wszUserPass);
	m_lcServer.SetItemText(0, 2, L"0");
	m_lcServer.SetItemText(0, 3, L"0");
	m_lcServer.SetItemText(0, 4, L"0");

	m_lcServer.SetRedraw(TRUE);

	Send_Login(m_vecLoginInfo[0]);
	updateItemData(objLoginInfo, 0);

	return true;
}

bool CLoginClientDlg::Send_Multiple_Login()
{
	if(m_sckClient != INVALID_SOCKET)
	{
		Close();
	}

	if(Connect() == false)
	{
		return false;
	}

	// 设置定时器去计算每秒大约发送的请求数
	m_nLastSecondSendCount = 0;
	m_nSendCount = 0;
	SetTimer(1, 1000, NULL);

	ULONG lulSendCount = 0;
	while(m_blMultiple)
	{
		int id = Random(m_objLoginClient.m_nUserIDFrom, m_objLoginClient.m_nUserIDTo);
		int nIndex = id - m_objLoginClient.m_nUserIDFrom;
		_LoginInfo& rdLoginInfo = m_vecLoginInfo[nIndex];
	
		Send_Login(rdLoginInfo);
		updateItemData(rdLoginInfo, nIndex);

		++m_nSendCount;
	}
	KillTimer(1);

	m_blMultiple = false;
	Close();

	return true;
}

bool CLoginClientDlg::Send_UserInfo( int nUserID )
{
	int nCommand = COMMAND_USERINFO;
	char szSendBuff[MAX_BUFF_500] = {'\0'};
	if(m_sckClient == INVALID_SOCKET)
	{
		return false;
	}

	short sVersion = 1;
	int nPos = 0;
	int nLen = 2 + 4;
	char szSession[32] = {'\0'};
	sprintf_s(szSession, 32, "FREEETES");

	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&sVersion, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nCommand, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nLen, sizeof(int));
	nPos += sizeof(int);
	memcpy_s(&szSendBuff[nPos], sizeof(char)*32, (char*)szSession, sizeof(char)*32);
	nPos += sizeof(char)*32;

	memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nUserID, sizeof(int));
	nPos += sizeof(int);

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
	int nRet           = 0;
	nPos               = 0;
	int nReUserID      = 0;
	int nLife          = 0;
	int nMagic         = 0;

	memcpy_s((char*)&nRecvCommandID,  sizeof(short), &pRecvBuff[nPos], sizeof(short));
	nPos += sizeof(short);
	memcpy_s((char*)&nRet,  sizeof(int), &pRecvBuff[nPos], sizeof(int));
	nPos += sizeof(int);

	memcpy_s((char*)&nReUserID,  sizeof(int), &pRecvBuff[nPos], sizeof(int));
	nPos += sizeof(int);
	memcpy_s((char*)&nLife,  sizeof(int), &pRecvBuff[nPos], sizeof(int));
	nPos += sizeof(int);
	memcpy_s((char*)&nMagic,  sizeof(int), &pRecvBuff[nPos], sizeof(int));
	nPos += sizeof(int);

	CString strData;
	strData.Format(_T("%d"), nReUserID);
	m_txtUserID.SetWindowText(strData);
	strData.Format(_T("%d"), nLife);
	m_txtUserLife.SetWindowText(strData);
	strData.Format(_T("%d"), nMagic);
	m_txtUserMagic.SetWindowText(strData);

	delete[] pRecvBuff;

	return true;
}

bool CLoginClientDlg::Send_SetUserInfo( int nUserID, int nLife, int nMagic )
{
	int nCommand = COMMAND_SET_USERINFO;
	char szSendBuff[MAX_BUFF_500] = {'\0'};
	if(m_sckClient == INVALID_SOCKET)
	{
		return false;
	}

	short sVersion = 1;
	int nPos = 0;
	int nLen = 2 + 4 + 4 + 4;
	char szSession[32] = {'\0'};
	sprintf_s(szSession, 32, "FREEEYES");

	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nCommand, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nCommand, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nLen, sizeof(int));
	nPos += sizeof(int);
	memcpy_s(&szSendBuff[nPos], sizeof(char)*32, (char*)szSession, sizeof(char)*32);
	nPos += sizeof(char)*32;

	memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nUserID, sizeof(int));
	nPos += sizeof(int);
	memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nLife, sizeof(int));
	nPos += sizeof(int);
	memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nMagic, sizeof(int));
	nPos += sizeof(int);

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
	int nRet           = 0;
	nPos               = 0;

	memcpy_s((char*)&nRecvCommandID,  sizeof(short), &pRecvBuff[nPos], sizeof(short));
	nPos += sizeof(short);
	memcpy_s((char*)&nRet,  sizeof(int), &pRecvBuff[nPos], sizeof(int));
	nPos += sizeof(int);

	if(nRet == LOGIN_SUCCESS)
	{
		MessageBox(_T("设置用户信息成功"), _T("提示信息"), MB_OK);
	}
	else
	{
		MessageBox(_T("设置用户信息失败"), _T("提示信息"), MB_OK);
	}

	delete[] pRecvBuff;
	return true;
}

// 更新指定项测试数据
void CLoginClientDlg::updateItemData(_LoginInfo& objLoginInfo, const int nItemIndex)
{
	// 更新List列表项
	//add by @单调
	m_lcServer.SetRedraw(FALSE);
	CString strData;

	strData.Format(_T("%d"), objLoginInfo.m_nSendCount);
	m_lcServer.SetItemText(nItemIndex, 2, strData);

	strData.Format(_T("%d"), objLoginInfo.m_nServerSuccess);
	m_lcServer.SetItemText(nItemIndex, 3, strData);

	strData.Format(_T("%d"), objLoginInfo.m_nServerFail);
	m_lcServer.SetItemText(nItemIndex, 4, strData);
	m_lcServer.SetRedraw(TRUE);
}



void CLoginClientDlg::OnBnClickedButton3()
{
	//停止压测
	m_blMultiple = false;

	KillTimer(1);

	m_txtClientCost.SetWindowText(L"");
}

int CLoginClientDlg::Random( int nStart, int nEnd )
{
	return nStart + (nEnd - nStart)*rand()/(RAND_MAX + 1);
}

void CLoginClientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case 1:
		CString steData;
		steData.Format(_T("每秒约发送[%d]个请求"), m_nSendCount - m_nLastSecondSendCount);
		m_nLastSecondSendCount = m_nSendCount;
		m_txtClientCost.SetWindowText(steData);
	}

	CDialog::OnTimer(nIDEvent);
}

void CLoginClientDlg::OnBnClickedButton4()
{
	char szSeachUserID[MAX_BUFF_50] = {'\0'};
	// 测试获得用户信息
	CString strData;

	//获得相关服务器连接信息
	m_txtServerIP.GetWindowText(strData);
	int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
	int nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, m_objServerInfo.m_szServerIP, MAX_BUFF_50, NULL,NULL);
	m_objServerInfo.m_szServerIP[nDecLen] = '\0';

	m_txtServerPort.GetWindowText(strData);
	m_objServerInfo.m_nServerPort = _ttoi((LPCTSTR)strData);

	//获得相关用户ID
	m_txtSeachUserID.GetWindowText(strData);
	nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
	nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, szSeachUserID, MAX_BUFF_50, NULL,NULL);
	szSeachUserID[nDecLen] = '\0';

	int nSeachUserID = atoi(szSeachUserID);

	if(Connect() == false)
	{
		return;
	}

	Send_UserInfo(nSeachUserID);

	Close();
}


void CLoginClientDlg::OnBnClickedButton5()
{
	char szTemp[MAX_BUFF_50] = {'\0'};
	// 测试获得用户信息
	CString strData;

	//获得相关服务器连接信息
	m_txtServerIP.GetWindowText(strData);
	int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
	int nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, m_objServerInfo.m_szServerIP, MAX_BUFF_50, NULL,NULL);
	m_objServerInfo.m_szServerIP[nDecLen] = '\0';

	m_txtServerPort.GetWindowText(strData);
	m_objServerInfo.m_nServerPort = _ttoi((LPCTSTR)strData);

	//获得相关用户信息
	m_txtUserID.GetWindowText(strData);
	nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
	nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, szTemp, MAX_BUFF_50, NULL,NULL);
	szTemp[nDecLen] = '\0';
	int nUserID = atoi(szTemp);
	m_txtUserLife.GetWindowText(strData);
	nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
	nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, szTemp, MAX_BUFF_50, NULL,NULL);
	szTemp[nDecLen] = '\0';
	int nLife = atoi(szTemp);
	m_txtUserMagic.GetWindowText(strData);
	nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
	nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, szTemp, MAX_BUFF_50, NULL,NULL);
	szTemp[nDecLen] = '\0';
	int nMagic = atoi(szTemp);

	if(Connect() == false)
	{
		return;
	}

	Send_SetUserInfo(nUserID, nLife, nMagic);

	Close();
}