// PssFtpClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PssFtpClient.h"
#include "PssFtpClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//线程执行
DWORD WINAPI ThreadProc(LPVOID argv)
{
	CPssFtpClientDlg* pPssFtpClientDlg = (CPssFtpClientDlg *)argv;
	if(NULL != pPssFtpClientDlg)
	{
		pPssFtpClientDlg->DownLoadListFile();
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


// CPssFtpClientDlg 对话框




CPssFtpClientDlg::CPssFtpClientDlg(CWnd* pParent /*=NULL*/)
: CDialog(CPssFtpClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPssFtpClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_txtServerIP);
	DDX_Control(pDX, IDC_EDIT2, m_txtServerPort);
	DDX_Control(pDX, IDC_EDIT3, m_txtUserName);
	DDX_Control(pDX, IDC_EDIT4, m_txtUserPass);
	DDX_Control(pDX, IDC_LIST1, m_lcPath);
	DDX_Control(pDX, IDC_BUTTON1, m_btnLogin);
	DDX_Control(pDX, IDC_BUTTON2, m_btnLogout);
	DDX_Control(pDX, IDC_EDIT5, m_txtLocalPath);
	DDX_Control(pDX, IDC_EDIT6, m_txtRemotePath);
	DDX_Control(pDX, IDC_BUTTON4, m_btnDownLoadFile);
	DDX_Control(pDX, IDC_EDIT7, m_txtUpFileName);
}

BEGIN_MESSAGE_MAP(CPssFtpClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CPssFtpClientDlg::OnBnClickedButton1)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON2, &CPssFtpClientDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CPssFtpClientDlg::OnBnClickedButton3)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CPssFtpClientDlg::OnNMClickList1)
	ON_BN_CLICKED(IDC_BUTTON4, &CPssFtpClientDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CPssFtpClientDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CPssFtpClientDlg 消息处理程序

BOOL CPssFtpClientDlg::OnInitDialog()
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

void CPssFtpClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPssFtpClientDlg::OnPaint()
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
HCURSOR CPssFtpClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPssFtpClientDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strData;

	m_txtServerIP.GetWindowText(strData);
	int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
	int nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, m_ClientFTPInfo.szServerIP, MAX_BUFF_50, NULL,NULL);
	m_ClientFTPInfo.szServerIP[nDecLen] = '\0';

	m_txtServerPort.GetWindowText(strData);
	m_ClientFTPInfo.nServerPort = _ttoi((LPCTSTR)strData);

	m_txtUserName.GetWindowText(strData);
	nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
	nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, m_ClientFTPInfo.szUserName, MAX_BUFF_50, NULL,NULL);
	m_ClientFTPInfo.szUserName[nDecLen] = '\0';

	m_txtUserPass.GetWindowText(strData);
	nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
	nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, m_ClientFTPInfo.szUserPass, MAX_BUFF_50, NULL,NULL);
	m_ClientFTPInfo.szUserPass[nDecLen] = '\0';

	//判断socket是否已经连接，如果有连接则断开
	if(m_sckClient != INVALID_SOCKET)
	{
		Close();
	}

	if(Connect() == false)
	{
		return;
	}

	if(true == Send_Login())
	{
		MessageBox(_T("连接远程服务器成功"), _T("提示信息"), MB_OK);
		m_btnLogin.EnableWindow(FALSE);
		m_btnLogout.EnableWindow(TRUE);
	}
	else
	{
		Close();
	}
}

void CPssFtpClientDlg::OnBnClickedButton2()
{
	//设置登出行为
	Send_Logout();
	Close();
	MessageBox(_T("登出服务器成功"), _T("提示信息"), MB_OK);
	m_btnLogin.EnableWindow(TRUE);
	m_btnLogout.EnableWindow(FALSE);
}

void CPssFtpClientDlg::Init()
{
	m_txtServerIP.SetWindowText(_T("127.0.0.1"));
	m_txtServerPort.SetWindowText(_T("10002"));
	m_txtUserName.SetWindowText(_T("freeeyes"));
	m_txtUserPass.SetWindowText(_T("1234"));

	m_lcPath.InsertColumn(0, _T("文件名称"), LVCFMT_CENTER, 300);
	m_lcPath.InsertColumn(1, _T("属性"), LVCFMT_CENTER, 50);
	m_lcPath.InsertColumn(2, _T("大小"), LVCFMT_CENTER, 150);

	DWORD dwStyle = m_lcPath.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	m_lcPath.SetExtendedStyle(dwStyle); //设置扩展风格

	//初始化TCP链接
	WSADATA wsaData;
	int nErr = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(nErr != 0)
	{
		MessageBox(_T("本机socket库加载失败，请检查本机socket库版本"), _T("错误信息"), MB_OK);
	}

	m_txtLocalPath.SetWindowText(_T("./"));
	m_txtRemotePath.SetWindowText(_T("./"));

	m_btnLogin.EnableWindow(TRUE);
	m_btnLogout.EnableWindow(FALSE);

	m_sckClient = INVALID_SOCKET;
}

void CPssFtpClientDlg::Close()
{
	closesocket(m_sckClient);
	m_sckClient = INVALID_SOCKET;
}

bool CPssFtpClientDlg::Connect()
{
	//socket创建的准备工作
	struct sockaddr_in sockaddr;

	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port   = htons(m_ClientFTPInfo.nServerPort);
	sockaddr.sin_addr.S_un.S_addr = inet_addr(m_ClientFTPInfo.szServerIP);

	m_sckClient = socket(AF_INET, SOCK_STREAM, 0);

	DWORD TimeOut = (DWORD)100;
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

void CPssFtpClientDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//判断socket是否已经连接，如果有连接则断开
	if(m_sckClient != INVALID_SOCKET)
	{
		Close();
	}

	CDialog::OnClose();
}

bool CPssFtpClientDlg::Send_Login()
{
	int nCommand = COMMAND_LOGIN;
	char szSendBuff[MAX_BUFF_500] = {'\0'};
	if(m_sckClient == INVALID_SOCKET)
	{
		return false;
	}

	short sVersion = 1;
	int nPos       = 0;
	int nLen       = 1 + 1 + (int)strlen(m_ClientFTPInfo.szUserName) + (int)strlen(m_ClientFTPInfo.szUserPass);
	char szSession[32] = {'\0'};

	sprintf_s(szSession, 32, "FREEEYES");

	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&sVersion, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nCommand, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nLen, sizeof(int));
	nPos += sizeof(int);
	memcpy_s(&szSendBuff[nPos], sizeof(char)*32, (char*)szSession, sizeof(char)*32);
	nPos += sizeof(char)*32;

	int nStrLen = (int)strlen(m_ClientFTPInfo.szUserName);
	memcpy_s(&szSendBuff[nPos], sizeof(char), (char*)&nStrLen, sizeof(char));
	nPos += sizeof(char);
	memcpy_s(&szSendBuff[nPos], nStrLen, (char*)m_ClientFTPInfo.szUserName, nStrLen);
	nPos += nStrLen;

	nStrLen = (int)strlen(m_ClientFTPInfo.szUserPass);
	memcpy_s(&szSendBuff[nPos], sizeof(char), (char*)&nStrLen, sizeof(char));
	nPos += sizeof(char);
	memcpy_s(&szSendBuff[nPos], nStrLen, (char*)m_ClientFTPInfo.szUserPass, nStrLen);
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

	if(nRet == LOGIN_FAIL_NOEXIST)
	{
		MessageBox(_T("用户名不存在"), _T("错误信息"), MB_OK);
		return false;
	}
	else if(nRet == LOGIN_FAIL_ONLINE)
	{
		MessageBox(_T("此用户正在线，不能登录"), _T("错误信息"), MB_OK);
		return false;
	}
	else if(nRet == LOGIN_FAIL_PASSWORD)
	{
		MessageBox(_T("用户密码不正确"), _T("错误信息"), MB_OK);
		return false;
	}

	return true;
}

bool CPssFtpClientDlg::Send_Logout()
{
	int nCommand = COMMAND_LOGOUT;
	char szSendBuff[MAX_BUFF_500] = {'\0'};
	if(m_sckClient == INVALID_SOCKET)
	{
		return false;
	}

	short sVersion = 1;
	int nPos       = 0;
	int nLen       = 1 + (int)strlen(m_ClientFTPInfo.szUserName);
	char szSession[32] = {'\0'};

	sprintf_s(szSession, 32, "FREEEYES");

	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&sVersion, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nCommand, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nLen, sizeof(int));
	nPos += sizeof(int);
	memcpy_s(&szSendBuff[nPos], sizeof(char)*32, (char*)szSession, sizeof(char)*32);
	nPos += sizeof(char)*32;

	int nStrLen = (int)strlen(m_ClientFTPInfo.szUserName);
	memcpy_s(&szSendBuff[nPos], sizeof(char), (char*)&nStrLen, sizeof(char));
	nPos += sizeof(char);
	memcpy_s(&szSendBuff[nPos], nStrLen, (char*)m_ClientFTPInfo.szUserName, nStrLen);
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

	return true;
}

bool CPssFtpClientDlg::Send_FileList(const char* pRemotePath)
{
	m_lcPath.DeleteAllItems();
	int nCommand = COMMAND_FILELIST;
	char szSendBuff[MAX_BUFF_500] = {'\0'};
	if(m_sckClient == INVALID_SOCKET)
	{
		return false;
	}

	short sVersion = 1;
	int nPos       = 0;
	int nLen       = 1 + 2 + (int)strlen(m_ClientFTPInfo.szUserName) + (int)strlen(pRemotePath);
	char szSession[32] = {'\0'};

	sprintf_s(szSession, 32, "FREEEYES");

	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&sVersion, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nCommand, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nLen, sizeof(int));
	nPos += sizeof(int);
	memcpy_s(&szSendBuff[nPos], sizeof(char)*32, (char*)szSession, sizeof(char)*32);
	nPos += sizeof(char)*32;

	int nStrLen = (int)strlen(m_ClientFTPInfo.szUserName);
	memcpy_s(&szSendBuff[nPos], sizeof(char), (char*)&nStrLen, sizeof(char));
	nPos += sizeof(char);
	memcpy_s(&szSendBuff[nPos], nStrLen, (char*)m_ClientFTPInfo.szUserName, nStrLen);
	nPos += nStrLen;

	nStrLen = (int)strlen(pRemotePath);
	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nStrLen, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], nStrLen, (char*)pRemotePath, nStrLen);
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

	if(nRet == OP_OK)
	{
		int nCount = 0;
		memcpy_s((char*)&nCount, sizeof(int), &pRecvBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		for(int i = 0; i < nCount; i++)
		{
			int nFileNameLen = 0;
			char szFileName[MAX_BUFF_500] = {'\0'};
			int nFileType = IS_FILE;
			int nFileSize = 0;
			memcpy_s((char*)&nFileNameLen, sizeof(char), &pRecvBuff[nPos], sizeof(char));
			nPos += sizeof(char);
			memcpy_s((char*)&szFileName, nFileNameLen, &pRecvBuff[nPos], nFileNameLen);
			nPos += nFileNameLen;
			memcpy_s((char*)&nFileType, sizeof(int), &pRecvBuff[nPos], sizeof(int));
			nPos += sizeof(int);
			memcpy_s((char*)&nFileSize, sizeof(int), &pRecvBuff[nPos], sizeof(int));
			nPos += sizeof(int);

			//加入显示
			CString strData;
			wchar_t wszFileName[MAX_BUFF_500]    = {'\0'};

			int nSrcLen = MultiByteToWideChar(CP_ACP, 0, szFileName, -1, NULL, 0);
			int nDecLen = MultiByteToWideChar(CP_ACP, 0, szFileName, -1, wszFileName, MAX_BUFF_500);

			m_lcPath.InsertItem(i, wszFileName);
			if(nFileType == IS_FILE)
			{
				m_lcPath.SetItemText(i, 1, _T("文件"));
			}
			else
			{
				m_lcPath.SetItemText(i, 1, _T("文件夹"));
			}
			strData.Format(_T("%d"), nFileSize);
			m_lcPath.SetItemText(i, 2, strData);
		}
	}

	return true;
}

bool CPssFtpClientDlg::Send_Download(const char* pLocalPath, const char* pFileName, const char* pRemotePath, int nIndex, int nSize, int& nBockCount)
{
	int nCommand = COMMAND_FILE_DOWNLOAD;
	char szSendBuff[MAX_BUFF_500] = {'\0'};
	if(m_sckClient == INVALID_SOCKET)
	{
		return false;
	}

	short sVersion = 1;
	int nPos       = 0;
	int nLen       = 2 + 1 + 8 + (int)strlen(m_ClientFTPInfo.szUserName) + (int)strlen(pRemotePath);
	char szSession[32] = {'\0'};

	sprintf_s(szSession, 32, "FREEEYES");

	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&sVersion, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nCommand, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nLen, sizeof(int));
	nPos += sizeof(int);
	memcpy_s(&szSendBuff[nPos], sizeof(char)*32, (char*)szSession, sizeof(char)*32);
	nPos += sizeof(char)*32;

	int nStrLen = (int)strlen(m_ClientFTPInfo.szUserName);
	memcpy_s(&szSendBuff[nPos], sizeof(char), (char*)&nStrLen, sizeof(char));
	nPos += sizeof(char);
	memcpy_s(&szSendBuff[nPos], nStrLen, (char*)m_ClientFTPInfo.szUserName, nStrLen);
	nPos += nStrLen;

	nStrLen = (int)strlen(pRemotePath);
	memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nStrLen, sizeof(short));
	nPos += sizeof(short);
	memcpy_s(&szSendBuff[nPos], nStrLen, (char*)pRemotePath, nStrLen);
	nPos += nStrLen;

	memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nSize, sizeof(int));
	nPos += sizeof(int);
	memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nIndex, sizeof(int));
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
			MessageBox(_T("远程服务器接收数据失败"), _T("错误信息"), MB_OK);
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
			DWORD dwError = GetLastError();
			MessageBox(_T("远程服务器接收数据失败"), _T("错误信息"), MB_OK);
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

	if(nRet == OP_OK)
	{
		//得到文件块，并写入文件
		char* pBuffer = new char[nSize];
		int nFileBlockCount     = 0;
		int nFileCurrIndex      = 0;
		int nFileCurrBufferSize = 0;

		memcpy_s((char*)&nBockCount,  sizeof(int), &pRecvBuff[nPos], sizeof(int));
		nPos += sizeof(int);
		memcpy_s((char*)&nFileCurrIndex,  sizeof(int), &pRecvBuff[nPos], sizeof(int));
		nPos += sizeof(int);
		memcpy_s((char*)&nFileCurrBufferSize,  sizeof(int), &pRecvBuff[nPos], sizeof(int));
		nPos += sizeof(int);
		memcpy_s((char*)pBuffer, nFileCurrBufferSize, &pRecvBuff[nPos], nFileCurrBufferSize);
		nPos += nFileCurrBufferSize;

		char szLocalFilePath[MAX_BUFF_500] = {'\0'};
		sprintf_s(szLocalFilePath, MAX_BUFF_500, "%s%s", pLocalPath, pFileName);

		//写入文件
		if(nFileCurrIndex == 0)
		{
			//如果是初始包，则删除当前文件。
			remove(szLocalFilePath);
		}

		FILE* pFile = NULL;
		fopen_s(&pFile, szLocalFilePath, "ab+");
		if(pFile == NULL)
		{
			MessageBox(_T("写入本地文件失败"), _T("错误信息"), MB_OK);
			delete[] pBuffer;
			return false;
		}

		fwrite(pBuffer, sizeof(char), nFileCurrBufferSize, pFile);
		fclose(pFile);
		delete[] pBuffer;
	}
	else
	{
		MessageBox(_T("远程文件获得失败"), _T("错误信息"), MB_OK);
		return false;
	}

	return true;
}

bool CPssFtpClientDlg::Send_Upload( const char* pLocalPath, const char* pFileName, const char* pRemotePath )
{
	int nBufferSize = MAX_BUFF_10240;
	int nCommand = COMMAND_FILE_UPLOAD;
	char szSendBuff[MAX_BUFF_10240 + MAX_BUFF_500] = {'\0'};
	if(m_sckClient == INVALID_SOCKET)
	{
		return false;
	}

	char szLoaclFilePath[MAX_BUFF_500] = {'\0'};
	sprintf_s(szLoaclFilePath, MAX_BUFF_500, "%s%s", pLocalPath, pFileName);

	//得到当前文件大小
	FILE* pFile = NULL;
	fopen_s(&pFile, szLoaclFilePath, "rb");
	if(NULL == pFile)
	{
		return false;
	}

	fseek(pFile, 0L, SEEK_END);
	int nFileLen        = (int)ftell(pFile);
	int nBlockCount     = 0;
	int nLastBufferSize = 0;

	if(nFileLen % nBufferSize != 0)
	{
		nBlockCount     = nFileLen / nBufferSize + 1;
		nLastBufferSize = nFileLen % nBufferSize;
	}
	else
	{
		nBlockCount     = nFileLen / nBufferSize;
		nLastBufferSize = nBufferSize;
	}

	fclose(pFile);
	pFile = NULL;

	for(int i = 0; i < nBlockCount; i++)
	{
		fopen_s(&pFile, szLoaclFilePath, "rb");
		if(NULL == pFile)
		{
			return false;
		}

		int nPos = 0;
		char szBuffer[MAX_BUFF_10240] = {'\0'};
		if(i != nBlockCount - 1)
		{
			//如果不是最后一个块
			fseek(pFile, (long)(i * nBufferSize), SEEK_CUR);

			int nReadSize = fread((char* )szBuffer, sizeof(char), nBufferSize, pFile);

			int nLen = 2 + 1 + 12 + (int)strlen(m_ClientFTPInfo.szUserName) + (int)strlen(pRemotePath) + nBufferSize;

			short sVersion = 1;
			char szSession[32] = {'\0'};

			sprintf_s(szSession, 32, "FREEEYES");
			memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&sVersion, sizeof(short));
			nPos += sizeof(short);
			memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nCommand, sizeof(short));
			nPos += sizeof(short);
			memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nLen, sizeof(int));
			nPos += sizeof(int);
			memcpy_s(&szSendBuff[nPos], sizeof(char)*32, (char*)szSession, sizeof(char)*32);
			nPos += sizeof(char)*32;

			int nStrLen = (int)strlen(m_ClientFTPInfo.szUserName);
			memcpy_s(&szSendBuff[nPos], sizeof(char), (char*)&nStrLen, sizeof(char));
			nPos += sizeof(char);
			memcpy_s(&szSendBuff[nPos], nStrLen, (char*)m_ClientFTPInfo.szUserName, nStrLen);
			nPos += nStrLen;

			nStrLen = (int)strlen(pRemotePath);
			memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nStrLen, sizeof(short));
			nPos += sizeof(short);
			memcpy_s(&szSendBuff[nPos], nStrLen, (char*)pRemotePath, nStrLen);
			nPos += nStrLen;

			memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nBufferSize, sizeof(int));
			nPos += sizeof(int);
			memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&i, sizeof(int));
			nPos += sizeof(int);

			memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nBufferSize, sizeof(int));
			nPos += sizeof(int);
			memcpy_s(&szSendBuff[nPos], nBufferSize, (char*)szBuffer, nBufferSize);
			nPos += nBufferSize;
		}
		else
		{
			//如果是最后一个块
			fseek(pFile, (long)(i * nBufferSize), SEEK_CUR);

			int nReadSize = fread((char* )szBuffer, sizeof(char), nLastBufferSize, pFile);

			int nLen = 2 + 1 + 12 + (int)strlen(m_ClientFTPInfo.szUserName) + (int)strlen(pRemotePath) + nLastBufferSize;

			short sVersion = 1;
			char szSession[32] = {'\0'};

			sprintf_s(szSession, 32, "FREEEYES");
			memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&sVersion, sizeof(short));
			nPos += sizeof(short);
			memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nCommand, sizeof(short));
			nPos += sizeof(short);
			memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nLen, sizeof(int));
			nPos += sizeof(int);
			memcpy_s(&szSendBuff[nPos], sizeof(char)*32, (char*)szSession, sizeof(char)*32);
			nPos += sizeof(char)*32;

			int nStrLen = (int)strlen(m_ClientFTPInfo.szUserName);
			memcpy_s(&szSendBuff[nPos], sizeof(char), (char*)&nStrLen, sizeof(char));
			nPos += sizeof(char);
			memcpy_s(&szSendBuff[nPos], nStrLen, (char*)m_ClientFTPInfo.szUserName, nStrLen);
			nPos += nStrLen;

			nStrLen = (int)strlen(pRemotePath);
			memcpy_s(&szSendBuff[nPos], sizeof(short), (char*)&nStrLen, sizeof(short));
			nPos += sizeof(short);
			memcpy_s(&szSendBuff[nPos], nStrLen, (char*)pRemotePath, nStrLen);
			nPos += nStrLen;

			memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nLastBufferSize, sizeof(int));
			nPos += sizeof(int);
			memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&i, sizeof(int));
			nPos += sizeof(int);

			memcpy_s(&szSendBuff[nPos], sizeof(int), (char*)&nLastBufferSize, sizeof(int));
			nPos += sizeof(int);
			memcpy_s(&szSendBuff[nPos], nLastBufferSize, (char*)szBuffer, nLastBufferSize);
			nPos += nLastBufferSize;
		}

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
				MessageBox(_T("远程服务器接收数据失败"), _T("错误信息"), MB_OK);
				fclose(pFile);
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
			fclose(pFile);
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
				DWORD dwError = GetLastError();
				MessageBox(_T("远程服务器发送数据失败"), _T("错误信息"), MB_OK);
				fclose(pFile);
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

		if(nRet != OP_OK)
		{
			DWORD dwError = GetLastError();
			MessageBox(_T("远程服务器接收数据失败"), _T("错误信息"), MB_OK);
			fclose(pFile);
			return false;
		}

		fclose(pFile);
	}

	MessageBox(_T("上传文件成功"), _T("错误信息"), MB_OK);

	return true;
}

void CPssFtpClientDlg::OnBnClickedButton3()
{
	//打开远程目录,并获得列表
	CString strData;
	char szRemotePath[MAX_BUFF_500] = {'\0'};

	m_txtRemotePath.GetWindowText(strData);
	int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
	int nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, szRemotePath, MAX_BUFF_500, NULL,NULL);
	szRemotePath[nDecLen] = '\0';

	Send_FileList(szRemotePath);
}

void CPssFtpClientDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	HD_NOTIFY *phdn = (HD_NOTIFY *)pNMHDR;

	//点击的行号
	int nRow = phdn->iItem;

	CString StrPathName = m_lcPath.GetItemText(nRow, 0);
	CString StrPathType = m_lcPath.GetItemText(nRow, 1);

	if(wcscmp((LPCTSTR)StrPathType, _T("文件夹")) == 0)
	{
		//如果是文件夹，则更新m_txtRemotePath
		CString strData;
		m_txtRemotePath.GetWindowText(strData);
		strData += StrPathName + _T("/");
		m_txtRemotePath.SetWindowText(strData);
	}

	if(StrPathType )

		*pResult = 0;
}

void CPssFtpClientDlg::OnBnClickedButton4()
{
	//启动后台线程下载
	DWORD  ThreadID = 0;
	CreateThread(NULL, NULL, ThreadProc, (LPVOID)this, NULL, &ThreadID);
}

void CPssFtpClientDlg::DownLoadListFile()
{
	m_btnDownLoadFile.EnableWindow(FALSE);
	//下载文件
	for(int i = 0; i < m_lcPath.GetItemCount(); i++)
	{
		if( m_lcPath.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED || m_lcPath.GetCheck(i))
		{
			//选中了下载文件
			int nBufferCount = 0;
			char szFileName[MAX_BUFF_500]   = {'\0'};
			char szRemotePath[MAX_BUFF_500] = {'\0'};
			char szLocalPath[MAX_BUFF_500]  = {'\0'};

			//得到文件名
			CString strData;
			CString strRomoteFilePath;
			CString strLocalPath;
			CString StrPathName = m_lcPath.GetItemText(i, 0);
			m_txtRemotePath.GetWindowText(strData);
			strRomoteFilePath = strData + StrPathName;

			int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strRomoteFilePath, strRomoteFilePath.GetLength(), NULL, 0, NULL, NULL);
			int nDecLen = WideCharToMultiByte(CP_ACP, 0, strRomoteFilePath, nSrcLen, szRemotePath, MAX_BUFF_500, NULL, NULL);
			szRemotePath[nDecLen] = '\0';

			nSrcLen = WideCharToMultiByte(CP_ACP, 0, StrPathName, StrPathName.GetLength(), NULL, 0, NULL, NULL);
			nDecLen = WideCharToMultiByte(CP_ACP, 0, StrPathName, nSrcLen, szFileName, MAX_BUFF_500, NULL, NULL);
			szFileName[nDecLen] = '\0';

			m_txtLocalPath.GetWindowText(strLocalPath);
			nSrcLen = WideCharToMultiByte(CP_ACP, 0, strLocalPath, strLocalPath.GetLength(), NULL, 0, NULL, NULL);
			nDecLen = WideCharToMultiByte(CP_ACP, 0, strLocalPath, nSrcLen, szLocalPath, MAX_BUFF_500, NULL, NULL);
			szLocalPath[nDecLen] = '\0';

			_DownloadFileInfo objDownloadFileInfo;

			//这里要注意，如果要设置更大的数据块，需要修改PSS的数据包最大大小的配置文件
			//默认PSS支持最大单数据包是20K，如果需要可以改的更大
			int nSize  = MAX_BUFF_10240; 

			sprintf_s(objDownloadFileInfo.szLocalPath, MAX_BUFF_500, "%s", szLocalPath);
			sprintf_s(objDownloadFileInfo.szFileName, MAX_BUFF_500, "%s", szFileName);
			sprintf_s(objDownloadFileInfo.szRemotePath, MAX_BUFF_500, "%s", szRemotePath);
			objDownloadFileInfo.nSize = nSize;

			int nIndex = 0;

			bool blState = Send_Download(szLocalPath, szFileName, szRemotePath, nIndex, nSize, nBufferCount);
			if(blState == false)
			{
				MessageBox(_T("远程文件下载失败"), _T("错误信息"), MB_OK);
				continue;
			}

			for(int i = 1; i < nBufferCount; i++)
			{
				blState = Send_Download(szLocalPath, szFileName, szRemotePath, i, nSize, nBufferCount);
				if(blState == false)
				{
					MessageBox(_T("远程文件下载失败"), _T("错误信息"), MB_OK);
					break;
				}
			}
		}
	}

	MessageBox(_T("下载文件完成"), _T("提示信息"), MB_OK);
	m_btnDownLoadFile.EnableWindow(TRUE);
}
void CPssFtpClientDlg::OnBnClickedButton5()
{
	//上传文件
	char szFileName[MAX_BUFF_500]   = {'\0'};
	char szRemotePath[MAX_BUFF_500] = {'\0'};
	char szLocalPath[MAX_BUFF_500]  = {'\0'};
	char szLocalFile[MAX_BUFF_500]  = {'\0'};
	char szRemoteFile[MAX_BUFF_500] = {'\0'};

	//得到文件名
	CString strData;
	CString strRomoteFilePath;
	CString strLocalPath;
	CString strLocalFile;

	m_txtRemotePath.GetWindowText(strRomoteFilePath);
	int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strRomoteFilePath, strRomoteFilePath.GetLength(), NULL, 0, NULL, NULL);
	int nDecLen = WideCharToMultiByte(CP_ACP, 0, strRomoteFilePath, nSrcLen, szRemotePath, MAX_BUFF_500, NULL, NULL);
	szRemotePath[nDecLen] = '\0';

	m_txtLocalPath.GetWindowText(strLocalPath);
	nSrcLen = WideCharToMultiByte(CP_ACP, 0, strLocalPath, strLocalPath.GetLength(), NULL, 0, NULL, NULL);
	nDecLen = WideCharToMultiByte(CP_ACP, 0, strLocalPath, nSrcLen, szLocalPath, MAX_BUFF_500, NULL, NULL);
	szLocalPath[nDecLen] = '\0';

	m_txtUpFileName.GetWindowText(strLocalFile);
	nSrcLen = WideCharToMultiByte(CP_ACP, 0, strLocalFile, strLocalFile.GetLength(), NULL, 0, NULL, NULL);
	nDecLen = WideCharToMultiByte(CP_ACP, 0, strLocalFile, nSrcLen, szLocalFile, MAX_BUFF_500, NULL, NULL);
	szLocalFile[nDecLen] = '\0';

	sprintf_s(szRemoteFile, MAX_BUFF_500, "%s/%s", strRomoteFilePath, szLocalFile);

	Send_Upload(szLocalPath, szLocalFile, szRemoteFile);

}
