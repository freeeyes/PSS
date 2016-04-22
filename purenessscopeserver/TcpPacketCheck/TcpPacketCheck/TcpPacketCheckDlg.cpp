// TcpPacketCheckDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TcpPacketCheck.h"
#include "TcpPacketCheckDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
//线程执行
DWORD WINAPI ThreadProc(LPVOID argv)
{
	CTcpPacketCheckDlg* pTcpPacketCheckDlg = (CTcpPacketCheckDlg* )argv;

	pTcpPacketCheckDlg->m_btnRun.EnableWindow(FALSE);
	pTcpPacketCheckDlg->Run();
	pTcpPacketCheckDlg->m_btnRun.EnableWindow(TRUE);

	return 0;
}

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


// CTcpPacketCheckDlg 对话框




CTcpPacketCheckDlg::CTcpPacketCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTcpPacketCheckDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTcpPacketCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_txtPacketBuffer);
	DDX_Control(pDX, IDC_EDIT2, m_txtServerIP);
	DDX_Control(pDX, IDC_EDIT3, m_txtPort);
	DDX_Control(pDX, IDC_EDIT4, m_txtRecvLength);
	DDX_Control(pDX, IDC_LIST1, m_lstResult);
	DDX_Control(pDX, IDC_BUTTON1, m_btnRun);
}

BEGIN_MESSAGE_MAP(CTcpPacketCheckDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CTcpPacketCheckDlg::OnBnClickedButton1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, OnCustomDraw)
END_MESSAGE_MAP()


// CTcpPacketCheckDlg 消息处理程序

BOOL CTcpPacketCheckDlg::OnInitDialog()
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
	m_txtServerIP.SetWindowText(_T("127.0.0.1"));
	m_txtPort.SetWindowText(_T("10002"));
	m_txtRecvLength.SetWindowText(_T("14"));
	m_txtPacketBuffer.SetWindowText(_T("be cd aa 8f 3c 01 00 00"));

	//初始化TCP链接
	WSADATA wsaData;
	int nErr = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(nErr != 0)
	{
		MessageBox(_T("本机socket库加载失败，请检查本机socket库版本"), _T("错误信息"), MB_OK);
	}

	m_lstResult.InsertColumn(0, _T("测试项目"), LVCFMT_LEFT, 220);
	m_lstResult.InsertColumn(2, _T("测试结果"), LVCFMT_LEFT, 430);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTcpPacketCheckDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTcpPacketCheckDlg::OnPaint()
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
HCURSOR CTcpPacketCheckDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTcpPacketCheckDlg::OnBnClickedButton1()
{
	DWORD  ThreadID = 0;
	CreateThread(NULL, NULL, ThreadProc, (LPVOID)this, NULL, &ThreadID);
}

bool CTcpPacketCheckDlg::CheckTcpPacket( _ClientInfo& objClientInfo, int nIndex)
{
	SOCKET sckClient;
	char szResult[1024]     = {'\0'};
	wchar_t sszResult[1024] = {'\0'};
	char szSession[32]      = {'\0'}; 
	int nSrcLen = 0;
	int nDecLen = 0;

	sprintf_s(szSession, 32, "FREEEYES");

	//socket创建的准备工作
	struct sockaddr_in sockaddr;

	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port   = htons(objClientInfo.m_nPort);
	sockaddr.sin_addr.S_un.S_addr = inet_addr(objClientInfo.m_szServerIP);

	sckClient = socket(AF_INET, SOCK_STREAM, 0);

	DWORD TimeOut = (DWORD)2000;
	::setsockopt(sckClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&TimeOut, sizeof(TimeOut));

	//连接远程服务器
	int nErr = connect(sckClient, (SOCKADDR*)&sockaddr, sizeof(SOCKADDR));
	if(0 != nErr)
	{
		DWORD dwError = GetLastError();
		sprintf_s(szResult, 1024, "[e]与[%s:%d]服务器连接失败，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

		nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

		m_lstResult.InsertItem(nIndex, _T("单个数据包检测"));
		m_lstResult.SetItemText(nIndex, 1, sszResult);

		return false;
	}

	//拼装发送包体
	char szSendBuffer[MAX_BUFF_200] ={'\0'};

	short sVersion = 1;
	short sCommand = (short)COMMAND_AUTOTEST_HEAD;
	int nPacketLen = objClientInfo.m_nSendLength;

	memcpy_s(szSendBuffer, sizeof(short), (char* )&sVersion, sizeof(short));
	memcpy_s((char* )&szSendBuffer[2], sizeof(short), (char* )&sCommand, sizeof(short));
	memcpy_s((char* )&szSendBuffer[4], sizeof(int), (char* )&nPacketLen, sizeof(int));
	memcpy_s((char* )&szSendBuffer[8], sizeof(char)*32, (char* )&szSession, sizeof(char)*32);
	memcpy_s((char* )&szSendBuffer[40], sizeof(char) * objClientInfo.m_nSendLength, (char* )objClientInfo.m_pSendBuffer, sizeof(char) * objClientInfo.m_nSendLength);
	int nSendLen = nPacketLen + 40;

	//发送数据
	int nTotalSendLen = nSendLen;
	int nBeginSend    = 0;
	int nCurrSendLen  = 0;
	bool blSendFlag   = false;
	int nBeginRecv    = 0;
	int nCurrRecvLen  = 0;
	bool blRecvFlag   = false;
	while(true)
	{
		nCurrSendLen = send(sckClient, szSendBuffer + nBeginSend, nTotalSendLen, 0);
		if(nCurrSendLen <= 0)
		{
			closesocket(sckClient);

			DWORD dwError = GetLastError();
			sprintf_s(szResult, 1024, "[e]与[%s:%d]服务器发送数据失败，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

			nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

			m_lstResult.InsertItem(nIndex, _T("单个数据包检测"));
			m_lstResult.SetItemText(nIndex, 1, sszResult);

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

	if(blSendFlag == false)
	{
		closesocket(sckClient);

		sprintf_s(szResult, 1024, "[e]与[%s:%d]发送字节数不匹配，缓冲字节数[%d]，实际发送字节数[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, objClientInfo.m_nSendLength, nTotalSendLen);

		nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

		m_lstResult.InsertItem(nIndex, _T("单个数据包检测"));
		m_lstResult.SetItemText(nIndex, 1, sszResult);

		return false;
	}
	else
	{
		int nTotalRecvLen               = nPacketLen + sizeof(int);
		char szRecvBuffData[1024 * 100] = {'\0'};

		while(true)
		{
			
			//如果发送成功了，则处理接收数据
			nCurrRecvLen = recv(sckClient, (char* )szRecvBuffData + nBeginRecv, nTotalRecvLen, 0);
			if(nCurrRecvLen <= 0)
			{
				closesocket(sckClient);

				DWORD dwError = GetLastError();
				sprintf_s(szResult, 1024, "[e]与[%s:%d]客户端无法接收数据，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

				nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

				m_lstResult.InsertItem(nIndex, _T("单个数据包检测"));
				m_lstResult.SetItemText(nIndex, 1, sszResult);

				return false;
			}
			else
			{
				nTotalRecvLen -= nCurrRecvLen;
				if(nTotalRecvLen == 0)
				{
					//接收完成
					break;
				}
				else
				{
					nBeginRecv += nCurrRecvLen;
				}
			}
		}
	}

	sprintf_s(szResult, 1024, "[s]与[%s:%d]单数据包检测成功。", objClientInfo.m_szServerIP, objClientInfo.m_nPort);

	nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

	m_lstResult.InsertItem(nIndex, _T("单个数据包检测"));
	m_lstResult.SetItemText(nIndex, 1, sszResult);

	closesocket(sckClient);

	return true;
}

bool CTcpPacketCheckDlg::CheckMultipleTcpPacket( _ClientInfo& objClientInfo, int nIndex )
{
	SOCKET sckClient;
	char szResult[1024]     = {'\0'};
	wchar_t sszResult[1024] = {'\0'};
	char szSession[32]      = {'\0'}; 
	int nSrcLen = 0;
	int nDecLen = 0;

	sprintf_s(szSession, 32, "FREEEYES");

	//socket创建的准备工作
	struct sockaddr_in sockaddr;

	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port   = htons(objClientInfo.m_nPort);
	sockaddr.sin_addr.S_un.S_addr = inet_addr(objClientInfo.m_szServerIP);

	sckClient = socket(AF_INET, SOCK_STREAM, 0);

	DWORD TimeOut = (DWORD)2000;
	::setsockopt(sckClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&TimeOut, sizeof(TimeOut));

	//连接远程服务器
	int nErr = connect(sckClient, (SOCKADDR*)&sockaddr, sizeof(SOCKADDR));
	if(0 != nErr)
	{
		DWORD dwError = GetLastError();
		sprintf_s(szResult, 1024, "[e]与[%s:%d]服务器连接失败，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

		nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

		m_lstResult.InsertItem(nIndex, _T("多个数据包检测"));
		m_lstResult.SetItemText(nIndex, 1, sszResult);

		return false;
	}

	//拼装发送包体
	char szSendBuffer[MAX_BUFF_200] ={'\0'};

	short sVersion = 1;
	short sCommand = (short)COMMAND_AUTOTEST_HEAD;
	int nPacketLen = objClientInfo.m_nSendLength;

	memcpy_s(szSendBuffer, sizeof(short), (char* )&sVersion, sizeof(short));
	memcpy_s((char* )&szSendBuffer[2], sizeof(short), (char* )&sCommand, sizeof(short));
	memcpy_s((char* )&szSendBuffer[4], sizeof(int), (char* )&nPacketLen, sizeof(int));
	memcpy_s((char* )&szSendBuffer[8], sizeof(char)*32, (char* )&szSession, sizeof(char)*32);
	memcpy_s((char* )&szSendBuffer[40], sizeof(char) * objClientInfo.m_nSendLength, (char* )objClientInfo.m_pSendBuffer, sizeof(char) * objClientInfo.m_nSendLength);	
	int nSendLen = nPacketLen + 40;

	//固定5个数据包同时发送
	char* pData = new char[5 * nSendLen];
	memset(pData, 0, 5 * nSendLen);
	for(int i = 0; i < 5; i++)
	{
		memcpy_s((char* )&pData[i * nSendLen], nSendLen, szSendBuffer, nSendLen);
	}

	//发送数据
	int nTotalSendLen = 5 * nSendLen;
	int nBeginSend    = 0;
	int nCurrSendLen  = 0;
	bool blSendFlag   = false;
	int nBeginRecv    = 0;
	int nCurrRecvLen  = 0;
	bool blRecvFlag   = false;
	while(true)
	{
		nCurrSendLen = send(sckClient, pData + nBeginSend, nTotalSendLen, 0);
		if(nCurrSendLen <= 0)
		{
			closesocket(sckClient);

			DWORD dwError = GetLastError();
			sprintf_s(szResult, 1024, "[e]与[%s:%d]服务器发送数据失败，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

			nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

			m_lstResult.InsertItem(nIndex, _T("多个数据包检测"));
			m_lstResult.SetItemText(nIndex, 1, sszResult);

			delete[] pData;
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

	if(blSendFlag == false)
	{
		closesocket(sckClient);

		sprintf_s(szResult, 1024, "[e]与[%s:%d]发送字节数不匹配，缓冲字节数[%d]，实际发送字节数[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, objClientInfo.m_nSendLength, nTotalSendLen);

		nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

		m_lstResult.InsertItem(nIndex, _T("多个数据包检测"));
		m_lstResult.SetItemText(nIndex, 1, sszResult);

		return false;
	}
	else
	{
		int nTotalRecvLen               = 5 * (nPacketLen + sizeof(int));
		char szRecvBuffData[1024 * 100] = {'\0'};

		while(true)
		{

			//如果发送成功了，则处理接收数据
			nCurrRecvLen = recv(sckClient, (char* )szRecvBuffData + nBeginRecv, nTotalRecvLen, 0);
			if(nCurrRecvLen <= 0)
			{
				closesocket(sckClient);

				DWORD dwError = GetLastError();
				sprintf_s(szResult, 1024, "[e]与[%s:%d]客户端无法接收数据，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

				nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

				m_lstResult.InsertItem(nIndex, _T("多个数据包检测"));
				m_lstResult.SetItemText(nIndex, 1, sszResult);

				delete[] pData;
				return false;
			}
			else
			{
				nTotalRecvLen -= nCurrRecvLen;
				if(nTotalRecvLen == 0)
				{
					//接收完成
					break;
				}
				else
				{
					nBeginRecv += nCurrRecvLen;
				}
			}
		}
	}

	sprintf_s(szResult, 1024, "[s]与[%s:%d]多数据包检测成功(5个同时发送)。", objClientInfo.m_szServerIP, objClientInfo.m_nPort);

	nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

	m_lstResult.InsertItem(nIndex, _T("多个数据包检测"));
	m_lstResult.SetItemText(nIndex, 1, sszResult);

	closesocket(sckClient);
	delete[] pData;
	return true;
}

bool CTcpPacketCheckDlg::CheckValidPacket( _ClientInfo& objClientInfo, int nIndex)
{
	SOCKET sckClient;
	char szResult[1024]     = {'\0'};
	wchar_t sszResult[1024] = {'\0'};
	char szSession[32]      = {'\0'}; 
	int nSrcLen = 0;
	int nDecLen = 0;

	sprintf_s(szSession, 32, "FREEEYES");

	//socket创建的准备工作
	struct sockaddr_in sockaddr;

	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port   = htons(objClientInfo.m_nPort);
	sockaddr.sin_addr.S_un.S_addr = inet_addr(objClientInfo.m_szServerIP);

	sckClient = socket(AF_INET, SOCK_STREAM, 0);

	DWORD TimeOut = (DWORD)2000;
	::setsockopt(sckClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&TimeOut, sizeof(TimeOut));

	//连接远程服务器
	int nErr = connect(sckClient, (SOCKADDR*)&sockaddr, sizeof(SOCKADDR));
	if(0 != nErr)
	{
		DWORD dwError = GetLastError();
		sprintf_s(szResult, 1024, "[e]与[%s:%d]服务器发送数据失败，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

		nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

		m_lstResult.InsertItem(nIndex, _T("无效数据包检测"));
		m_lstResult.SetItemText(nIndex, 1, sszResult);

		return false;
	}

	//随便一个全0的数据包
	char* pData = new char[objClientInfo.m_nSendLength];
	memset(pData, 0, objClientInfo.m_nSendLength);

	//发送数据
	int nTotalSendLen = 5 * objClientInfo.m_nSendLength;
	int nBeginSend    = 0;
	int nCurrSendLen  = 0;
	bool blSendFlag   = false;
	int nBeginRecv    = 0;
	int nCurrRecvLen  = 0;
	bool blRecvFlag   = false;
	while(true)
	{
		nCurrSendLen = send(sckClient, pData + nBeginSend, nTotalSendLen, 0);
		if(nCurrSendLen <= 0)
		{
			closesocket(sckClient);

			DWORD dwError = GetLastError();
			sprintf_s(szResult, 1024, "[e]与[%s:%d]无效包被服务器断开，测试失败。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

			nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

			m_lstResult.InsertItem(nIndex, _T("无效数据包检测"));
			m_lstResult.SetItemText(nIndex, 1, sszResult);

			delete[] pData;
			return true;
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

	if(blSendFlag == false)
	{
		closesocket(sckClient);

		sprintf_s(szResult, 1024, "[e]与[%s:%d]发送字节数不匹配，缓冲字节数[%d]，实际发送字节数[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, objClientInfo.m_nSendLength, nTotalSendLen);

		nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

		m_lstResult.InsertItem(nIndex, _T("无效数据包检测"));
		m_lstResult.SetItemText(nIndex, 1, sszResult);

		return false;
	}
	else
	{
		int nTotalRecvLen               = 5 * objClientInfo.m_nRecvLength;
		char szRecvBuffData[1024 * 100] = {'\0'};

		while(true)
		{

			//如果发送成功了，则处理接收数据
			nCurrRecvLen = recv(sckClient, (char* )szRecvBuffData + nBeginRecv, nTotalRecvLen, 0);
			if(nCurrRecvLen <= 0)
			{
				closesocket(sckClient);

				DWORD dwError = GetLastError();
				sprintf_s(szResult, 1024, "[s]与[%s:%d]无效数据包服务器断开连接，测试成功。", objClientInfo.m_szServerIP, objClientInfo.m_nPort);

				nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

				m_lstResult.InsertItem(nIndex, _T("无效数据包检测"));
				m_lstResult.SetItemText(nIndex, 1, sszResult);

				delete[] pData;
				return true;
			}
			else
			{
				nTotalRecvLen -= nCurrRecvLen;
				if(nTotalRecvLen == 0)
				{
					//接收完成
					break;
				}
				else
				{
					nBeginRecv += nCurrRecvLen;
				}
			}
		}
	}

	closesocket(sckClient);

	DWORD dwError = GetLastError();
	sprintf_s(szResult, 1024, "[e]与[%s:%d]无效包被服务器没有断开，测试失败。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

	nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

	m_lstResult.InsertItem(nIndex, _T("无效数据包检测"));
	m_lstResult.SetItemText(nIndex, 1, sszResult);

	delete[] pData;
	return false;
}

bool CTcpPacketCheckDlg::CheckHalfPacket( _ClientInfo& objClientInfo, int nIndex )
{
	SOCKET sckClient;
	char szResult[1024]     = {'\0'};
	wchar_t sszResult[1024] = {'\0'};
	char szSession[32]      = {'\0'}; 
	int nSrcLen = 0;
	int nDecLen = 0;

	sprintf_s(szSession, 32, "FREEEYES");

	//socket创建的准备工作
	struct sockaddr_in sockaddr;

	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port   = htons(objClientInfo.m_nPort);
	sockaddr.sin_addr.S_un.S_addr = inet_addr(objClientInfo.m_szServerIP);

	sckClient = socket(AF_INET, SOCK_STREAM, 0);

	DWORD TimeOut = (DWORD)2000;
	::setsockopt(sckClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&TimeOut, sizeof(TimeOut));

	//连接远程服务器
	int nErr = connect(sckClient, (SOCKADDR*)&sockaddr, sizeof(SOCKADDR));
	if(0 != nErr)
	{
		DWORD dwError = GetLastError();
		sprintf_s(szResult, 1024, "[e]与[%s:%d]服务器发送数据失败，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

		nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

		m_lstResult.InsertItem(nIndex, _T("半包数据包检测"));
		m_lstResult.SetItemText(nIndex, 1, sszResult);

		return false;
	}

	//拼装发送包体
	char szSendBuffer[MAX_BUFF_200] ={'\0'};

	short sVersion = 1;
	short sCommand = (short)COMMAND_AUTOTEST_HEAD;
	int nPacketLen = objClientInfo.m_nSendLength;

	memcpy_s(szSendBuffer, sizeof(short), (char* )&sVersion, sizeof(short));
	memcpy_s((char* )&szSendBuffer[2], sizeof(short), (char* )&sCommand, sizeof(short));
	memcpy_s((char* )&szSendBuffer[4], sizeof(int), (char* )&nPacketLen, sizeof(int));
	memcpy_s((char* )&szSendBuffer[8], sizeof(char)*32, (char* )&szSession, sizeof(char)*32);
	memcpy_s((char* )&szSendBuffer[40], sizeof(char) * objClientInfo.m_nSendLength, (char* )objClientInfo.m_pSendBuffer, sizeof(char) * objClientInfo.m_nSendLength);	
	int nSendLen = nPacketLen + 40;

	//数据包随意切分,2个数据包
	char* pData = new char[2 * nSendLen];
	memset(pData, 0, 2 * nSendLen);
	for(int i = 0; i < 2; i++)
	{
		memcpy_s((char* )&pData[i * nSendLen], nSendLen, szSendBuffer, nSendLen);
	}

	//发送数据（前半部分）
	int nTotalSendLen = 2;
	int nBeginSend    = 0;
	int nCurrSendLen  = 0;
	bool blSendFlag   = false;
	int nBeginRecv    = 0;
	int nCurrRecvLen  = 0;
	bool blRecvFlag   = false;
	while(true)
	{
		nCurrSendLen = send(sckClient, pData + nBeginSend, nTotalSendLen, 0);
		if(nCurrSendLen <= 0)
		{
			closesocket(sckClient);

			DWORD dwError = GetLastError();
			sprintf_s(szResult, 1024, "[e]与[%s:%d]无效包被服务器断开，测试失败。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

			nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

			m_lstResult.InsertItem(nIndex, _T("半包数据包检测"));
			m_lstResult.SetItemText(nIndex, 1, sszResult);

			delete[] pData;
			return true;
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

	//发送数据（后半部分）
	nTotalSendLen = 2 * nSendLen - 2;
	nBeginSend    = 0;
	nCurrSendLen  = 0;
	blSendFlag   = false;
	nBeginRecv    = 0;
	nCurrRecvLen  = 0;
	blRecvFlag   = false;
	while(true)
	{
		nCurrSendLen = send(sckClient, pData + 2 + nBeginSend, nTotalSendLen, 0);
		if(nCurrSendLen <= 0)
		{
			closesocket(sckClient);

			DWORD dwError = GetLastError();
			sprintf_s(szResult, 1024, "[e]与[%s:%d]无效包被服务器断开，测试成功。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

			nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

			m_lstResult.InsertItem(nIndex, _T("半包数据包检测"));
			m_lstResult.SetItemText(nIndex, 1, sszResult);

			delete[] pData;
			return true;
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

	if(blSendFlag == false)
	{
		closesocket(sckClient);

		sprintf_s(szResult, 1024, "[e]与[%s:%d]发送字节数不匹配，缓冲字节数[%d]，实际发送字节数[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, objClientInfo.m_nSendLength, nTotalSendLen);

		nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

		m_lstResult.InsertItem(nIndex, _T("半包数据包检测"));
		m_lstResult.SetItemText(nIndex, 1, sszResult);

		return false;
	}
	else
	{
		int nTotalRecvLen               = 2 * (nPacketLen + sizeof(int));
		char szRecvBuffData[1024 * 100] = {'\0'};

		while(true)
		{
			//如果发送成功了，则处理接收数据
			nCurrRecvLen = recv(sckClient, (char* )szRecvBuffData + nBeginRecv, nTotalRecvLen, 0);
			if(nCurrRecvLen <= 0)
			{
				closesocket(sckClient);

				DWORD dwError = GetLastError();
				sprintf_s(szResult, 1024, "[e]与[%s:%d]客户端无法接收数据，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

				nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

				m_lstResult.InsertItem(nIndex, _T("半包数据包检测"));
				m_lstResult.SetItemText(nIndex, 1, sszResult);

				delete[] pData;
				return false;
			}
			else
			{
				nTotalRecvLen -= nCurrRecvLen;
				if(nTotalRecvLen == 0)
				{
					//接收完成
					break;
				}
				else
				{
					nBeginRecv += nCurrRecvLen;
				}
			}
		}
	}

	sprintf_s(szResult, 1024, "[s]与[%s:%d]数据包任意切割发送检测成功(2个同时发送)。", objClientInfo.m_szServerIP, objClientInfo.m_nPort);

	nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

	m_lstResult.InsertItem(nIndex, _T("半包数据包检测"));
	m_lstResult.SetItemText(nIndex, 1, sszResult);

	closesocket(sckClient);
	delete[] pData;
	return true;

}

bool CTcpPacketCheckDlg::CheckIsHead( _ClientInfo& objClientInfo, int nIndex )
{
	SOCKET sckClient;
	char szResult[1024]     = {'\0'};
	wchar_t sszResult[1024] = {'\0'};
	char szSession[32]      = {'\0'}; 
	int nSrcLen = 0;
	int nDecLen = 0;

	sprintf_s(szSession, 32, "FREEEYES");

	//socket创建的准备工作
	struct sockaddr_in sockaddr;

	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port   = htons(objClientInfo.m_nPort);
	sockaddr.sin_addr.S_un.S_addr = inet_addr(objClientInfo.m_szServerIP);

	sckClient = socket(AF_INET, SOCK_STREAM, 0);

	DWORD TimeOut = (DWORD)2000;
	::setsockopt(sckClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&TimeOut, sizeof(TimeOut));

	//连接远程服务器
	int nErr = connect(sckClient, (SOCKADDR*)&sockaddr, sizeof(SOCKADDR));
	if(0 != nErr)
	{
		DWORD dwError = GetLastError();
		sprintf_s(szResult, 1024, "[e]与[%s:%d]服务器连接失败，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

		nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

		m_lstResult.InsertItem(nIndex, _T("数据包含包头(PacketParse)检测"));
		m_lstResult.SetItemText(nIndex, 1, sszResult);

		return false;
	}

	//拼装发送包体
	char szSendBuffer[MAX_BUFF_200] ={'\0'};

	short sVersion = 1;
	short sCommand = (short)COMMAND_AUTOTEST_HEAD;
	int nPacketLen = objClientInfo.m_nSendLength;

	memcpy_s(szSendBuffer, sizeof(short), (char* )&sVersion, sizeof(short));
	memcpy_s((char* )&szSendBuffer[2], sizeof(short), (char* )&sCommand, sizeof(short));
	memcpy_s((char* )&szSendBuffer[4], sizeof(int), (char* )&nPacketLen, sizeof(int));
	memcpy_s((char* )&szSendBuffer[8], sizeof(char)*32, (char* )&szSession, sizeof(char)*32);
	memcpy_s((char* )&szSendBuffer[40], sizeof(char) * objClientInfo.m_nSendLength, (char* )objClientInfo.m_pSendBuffer, sizeof(char) * objClientInfo.m_nSendLength);
	int nSendLen = nPacketLen + 40;

	//发送数据
	int nTotalSendLen = nSendLen;
	int nBeginSend    = 0;
	int nCurrSendLen  = 0;
	bool blSendFlag   = false;
	int nBeginRecv    = 0;
	int nCurrRecvLen  = 0;
	bool blRecvFlag   = false;
	while(true)
	{
		nCurrSendLen = send(sckClient, szSendBuffer + nBeginSend, nTotalSendLen, 0);
		if(nCurrSendLen <= 0)
		{
			closesocket(sckClient);

			DWORD dwError = GetLastError();
			sprintf_s(szResult, 1024, "[e]与[%s:%d]服务器发送数据失败，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

			nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

			m_lstResult.InsertItem(nIndex, _T("数据包含包头(PacketParse)检测"));
			m_lstResult.SetItemText(nIndex, 1, sszResult);

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

	if(blSendFlag == false)
	{
		closesocket(sckClient);

		sprintf_s(szResult, 1024, "[e]与[%s:%d]发送字节数不匹配，缓冲字节数[%d]，实际发送字节数[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, objClientInfo.m_nSendLength, nTotalSendLen);

		nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

		m_lstResult.InsertItem(nIndex, _T("数据包含包头(PacketParse)检测"));
		m_lstResult.SetItemText(nIndex, 1, sszResult);

		return false;
	}
	else
	{
		int nTotalRecvLen               = objClientInfo.m_nRecvLength;
		char szRecvBuffData[1024 * 100] = {'\0'};

		while(true)
		{

			//如果发送成功了，则处理接收数据
			nCurrRecvLen = recv(sckClient, (char* )szRecvBuffData + nBeginRecv, nTotalRecvLen, 0);
			if(nCurrRecvLen <= 0)
			{
				closesocket(sckClient);

				DWORD dwError = GetLastError();
				sprintf_s(szResult, 1024, "[e]与[%s:%d]客户端无法接收数据，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

				nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

				m_lstResult.InsertItem(nIndex, _T("数据包含包头(PacketParse)检测"));
				m_lstResult.SetItemText(nIndex, 1, sszResult);

				return false;
			}
			else
			{
				nTotalRecvLen -= nCurrRecvLen;
				if(nTotalRecvLen == 0)
				{
					//接收完成
					break;
				}
				else
				{
					nBeginRecv += nCurrRecvLen;
				}
			}
		}

		//检测接收数据的正确性
		int nRecvPacketSize = 0;
		int nReturnCommand  = 0;
		char szTempData[8]  = {'\0'};

		memcpy_s((char* )&nRecvPacketSize, sizeof(int), (char* )&szRecvBuffData[0], sizeof(int));
		memcpy_s((char* )&nReturnCommand, sizeof(short), (char* )&szRecvBuffData[4], sizeof(short));
		memcpy_s((char* )szTempData, 8 * sizeof(char), (char* )&szRecvBuffData[6], 8 * sizeof(char));

		//检测返回包头大小
		if(nRecvPacketSize != 10)
		{
			closesocket(sckClient);

			sprintf_s(szResult, 1024, "[e]与[%s:%d]接收返回包大小不对。", objClientInfo.m_szServerIP, objClientInfo.m_nPort);

			nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

			m_lstResult.InsertItem(nIndex, _T("数据包含包头(PacketParse)检测"));
			m_lstResult.SetItemText(nIndex, 1, sszResult);

			return false;
		}

		//判断返回命令字
		if(nReturnCommand != (int)COMMAND_AUTOTEST_RETUEN_HEAD)
		{
			closesocket(sckClient);

			sprintf_s(szResult, 1024, "[e]与[%s:%d]接受包返回命令字不对。", objClientInfo.m_szServerIP, objClientInfo.m_nPort);

			nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

			m_lstResult.InsertItem(nIndex, _T("数据包含包头(PacketParse)检测"));
			m_lstResult.SetItemText(nIndex, 1, sszResult);

			return false;
		}

		//判断返回数据
		int nRet = memcmp(szTempData, objClientInfo.m_pSendBuffer, 8);
		if(nRet != 0)
		{
			closesocket(sckClient);

			sprintf_s(szResult, 1024, "[e]与[%s:%d]接受包数据和发送数据不符。", objClientInfo.m_szServerIP, objClientInfo.m_nPort);

			nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

			m_lstResult.InsertItem(nIndex, _T("数据包含包头(PacketParse)检测"));
			m_lstResult.SetItemText(nIndex, 1, sszResult);

			return false;
		}
	}

	sprintf_s(szResult, 1024, "[s]与[%s:%d]数据包检测成功。", objClientInfo.m_szServerIP, objClientInfo.m_nPort);

	nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

	m_lstResult.InsertItem(nIndex, _T("数据包含包头(PacketParse)检测"));
	m_lstResult.SetItemText(nIndex, 1, sszResult);

	closesocket(sckClient);

	return true;
}

bool CTcpPacketCheckDlg::CheckIsNoHead( _ClientInfo& objClientInfo, int nIndex )
{
	SOCKET sckClient;
	char szResult[1024]     = {'\0'};
	wchar_t sszResult[1024] = {'\0'};
	char szSession[32]      = {'\0'}; 
	int nSrcLen = 0;
	int nDecLen = 0;

	sprintf_s(szSession, 32, "FREEEYES");

	//socket创建的准备工作
	struct sockaddr_in sockaddr;

	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port   = htons(objClientInfo.m_nPort);
	sockaddr.sin_addr.S_un.S_addr = inet_addr(objClientInfo.m_szServerIP);

	sckClient = socket(AF_INET, SOCK_STREAM, 0);

	DWORD TimeOut = (DWORD)2000;
	::setsockopt(sckClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&TimeOut, sizeof(TimeOut));

	//连接远程服务器
	int nErr = connect(sckClient, (SOCKADDR*)&sockaddr, sizeof(SOCKADDR));
	if(0 != nErr)
	{
		DWORD dwError = GetLastError();
		sprintf_s(szResult, 1024, "[e]与[%s:%d]服务器连接失败，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

		nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

		m_lstResult.InsertItem(nIndex, _T("数据包透传(PacketParse)检测"));
		m_lstResult.SetItemText(nIndex, 1, sszResult);

		return false;
	}

	//拼装发送包体
	char szSendBuffer[MAX_BUFF_200] ={'\0'};

	short sVersion = 1;
	short sCommand = (short)COMMAND_AUTOTEST_NOHEAD;
	int nPacketLen = objClientInfo.m_nSendLength + 2;

	memcpy_s(szSendBuffer, sizeof(short), (char* )&sVersion, sizeof(short));
	memcpy_s((char* )&szSendBuffer[2], sizeof(short), (char* )&sCommand, sizeof(short));
	memcpy_s((char* )&szSendBuffer[4], sizeof(int), (char* )&nPacketLen, sizeof(int));
	memcpy_s((char* )&szSendBuffer[8], sizeof(char)*32, (char* )&szSession, sizeof(char)*32);
	memcpy_s((char* )&szSendBuffer[40], sizeof(char) * objClientInfo.m_nSendLength, (char* )objClientInfo.m_pSendBuffer, sizeof(char) * objClientInfo.m_nSendLength);
	int nSendLen = nPacketLen + 40;

	//发送数据
	int nTotalSendLen = nSendLen;
	int nBeginSend    = 0;
	int nCurrSendLen  = 0;
	bool blSendFlag   = false;
	int nBeginRecv    = 0;
	int nCurrRecvLen  = 0;
	bool blRecvFlag   = false;
	while(true)
	{
		nCurrSendLen = send(sckClient, szSendBuffer + nBeginSend, nTotalSendLen, 0);
		if(nCurrSendLen <= 0)
		{
			closesocket(sckClient);

			DWORD dwError = GetLastError();
			sprintf_s(szResult, 1024, "[e]与[%s:%d]服务器发送数据失败，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

			nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

			m_lstResult.InsertItem(nIndex, _T("数据包透传(PacketParse)检测"));
			m_lstResult.SetItemText(nIndex, 1, sszResult);

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

	if(blSendFlag == false)
	{
		closesocket(sckClient);

		sprintf_s(szResult, 1024, "[e]与[%s:%d]发送字节数不匹配，缓冲字节数[%d]，实际发送字节数[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, objClientInfo.m_nSendLength, nTotalSendLen);

		nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

		m_lstResult.InsertItem(nIndex, _T("数据包透传(PacketParse)检测"));
		m_lstResult.SetItemText(nIndex, 1, sszResult);

		return false;
	}
	else
	{
		int nTotalRecvLen               = objClientInfo.m_nRecvLength;
		char szRecvBuffData[1024 * 100] = {'\0'};

		while(true)
		{

			//如果发送成功了，则处理接收数据
			nCurrRecvLen = recv(sckClient, (char* )szRecvBuffData + nBeginRecv, nTotalRecvLen, 0);
			if(nCurrRecvLen <= 0)
			{
				closesocket(sckClient);

				DWORD dwError = GetLastError();
				sprintf_s(szResult, 1024, "[e]与[%s:%d]客户端无法接收数据，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

				nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

				m_lstResult.InsertItem(nIndex, _T("数据包透传(PacketParse)检测"));
				m_lstResult.SetItemText(nIndex, 1, sszResult);

				return false;
			}
			else
			{
				nTotalRecvLen -= nCurrRecvLen;
				if(nTotalRecvLen == 0)
				{
					//接收完成
					break;
				}
				else
				{
					nBeginRecv += nCurrRecvLen;
				}
			}
		}

		//检测接收数据的正确性
		int nRecvPacketSize = 0;
		int nReturnCommand  = 0;
		char szTempData[8]  = {'\0'};

		memcpy_s((char* )&nRecvPacketSize, sizeof(int), (char* )&szRecvBuffData[0], sizeof(int));
		memcpy_s((char* )&nReturnCommand, sizeof(short), (char* )&szRecvBuffData[4], sizeof(short));
		memcpy_s((char* )szTempData, 8 * sizeof(char), (char* )&szRecvBuffData[6], 8 * sizeof(char));

		//检测返回包头大小
		if(nRecvPacketSize != 10)
		{
			closesocket(sckClient);

			sprintf_s(szResult, 1024, "[e]与[%s:%d]接收返回包大小不对。", objClientInfo.m_szServerIP, objClientInfo.m_nPort);

			nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

			m_lstResult.InsertItem(nIndex, _T("数据包透传(PacketParse)检测"));
			m_lstResult.SetItemText(nIndex, 1, sszResult);

			return false;
		}

		//判断返回命令字
		if(nReturnCommand != (int)COMMAND_AUTOTEST_RETUEN_NOHEAD)
		{
			closesocket(sckClient);

			sprintf_s(szResult, 1024, "[e]与[%s:%d]接受包返回命令字不对。", objClientInfo.m_szServerIP, objClientInfo.m_nPort);

			nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

			m_lstResult.InsertItem(nIndex, _T("数据包透传(PacketParse)检测"));
			m_lstResult.SetItemText(nIndex, 1, sszResult);

			return false;
		}

		//判断返回数据
		int nRet = memcmp(szTempData, objClientInfo.m_pSendBuffer, 8);
		if(nRet != 0)
		{
			closesocket(sckClient);

			sprintf_s(szResult, 1024, "[e]与[%s:%d]接受包数据和发送数据不符。", objClientInfo.m_szServerIP, objClientInfo.m_nPort);

			nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

			m_lstResult.InsertItem(nIndex, _T("数据包透传(PacketParse)检测"));
			m_lstResult.SetItemText(nIndex, 1, sszResult);

			return false;
		}
	}

	sprintf_s(szResult, 1024, "[s]与[%s:%d]数据包检测成功。", objClientInfo.m_szServerIP, objClientInfo.m_nPort);

	nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

	m_lstResult.InsertItem(nIndex, _T("数据包透传(PacketParse)检测"));
	m_lstResult.SetItemText(nIndex, 1, sszResult);

	closesocket(sckClient);

	return true;
}


bool CTcpPacketCheckDlg::CheckIsHeadBuffer( _ClientInfo& objClientInfo, int nIndex )
{
	SOCKET sckClient;
	char szResult[1024]     = {'\0'};
	wchar_t sszResult[1024] = {'\0'};
	char szSession[32]      = {'\0'}; 
	int nSrcLen = 0;
	int nDecLen = 0;

	sprintf_s(szSession, 32, "FREEEYES");

	//socket创建的准备工作
	struct sockaddr_in sockaddr;

	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port   = htons(objClientInfo.m_nPort);
	sockaddr.sin_addr.S_un.S_addr = inet_addr(objClientInfo.m_szServerIP);

	sckClient = socket(AF_INET, SOCK_STREAM, 0);

	DWORD TimeOut = (DWORD)2000;
	::setsockopt(sckClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&TimeOut, sizeof(TimeOut));

	//连接远程服务器
	int nErr = connect(sckClient, (SOCKADDR*)&sockaddr, sizeof(SOCKADDR));
	if(0 != nErr)
	{
		DWORD dwError = GetLastError();
		sprintf_s(szResult, 1024, "[e]与[%s:%d]服务器连接失败，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

		nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

		m_lstResult.InsertItem(nIndex, _T("带数据头的缓冲(PacketParse)检测"));
		m_lstResult.SetItemText(nIndex, 1, sszResult);

		return false;
	}

	//拼装发送包体
	char szSendBuffer[MAX_BUFF_200] ={'\0'};

	short sVersion = 1;
	short sCommand = (short)COMMAND_AUTOTEST_HEADBUFF;
	int nPacketLen = objClientInfo.m_nSendLength;

	memcpy_s(szSendBuffer, sizeof(short), (char* )&sVersion, sizeof(short));
	memcpy_s((char* )&szSendBuffer[2], sizeof(short), (char* )&sCommand, sizeof(short));
	memcpy_s((char* )&szSendBuffer[4], sizeof(int), (char* )&nPacketLen, sizeof(int));
	memcpy_s((char* )&szSendBuffer[8], sizeof(char)*32, (char* )&szSession, sizeof(char)*32);
	memcpy_s((char* )&szSendBuffer[40], sizeof(char) * objClientInfo.m_nSendLength, (char* )objClientInfo.m_pSendBuffer, sizeof(char) * objClientInfo.m_nSendLength);
	int nSendLen = nPacketLen + 40;

	//发送数据
	int nTotalSendLen = nSendLen;
	int nBeginSend    = 0;
	int nCurrSendLen  = 0;
	bool blSendFlag   = false;
	int nBeginRecv    = 0;
	int nCurrRecvLen  = 0;
	bool blRecvFlag   = false;
	while(true)
	{
		nCurrSendLen = send(sckClient, szSendBuffer + nBeginSend, nTotalSendLen, 0);
		if(nCurrSendLen <= 0)
		{
			closesocket(sckClient);

			DWORD dwError = GetLastError();
			sprintf_s(szResult, 1024, "[e]与[%s:%d]服务器发送数据失败，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

			nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

			m_lstResult.InsertItem(nIndex, _T("带数据头的缓冲(PacketParse)检测"));
			m_lstResult.SetItemText(nIndex, 1, sszResult);

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

	if(blSendFlag == false)
	{
		closesocket(sckClient);

		sprintf_s(szResult, 1024, "[e]与[%s:%d]发送字节数不匹配，缓冲字节数[%d]，实际发送字节数[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, objClientInfo.m_nSendLength, nTotalSendLen);

		nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

		m_lstResult.InsertItem(nIndex, _T("带数据头的缓冲(PacketParse)检测"));
		m_lstResult.SetItemText(nIndex, 1, sszResult);

		return false;
	}
	else
	{
		int nTotalRecvLen               = (objClientInfo.m_nRecvLength) * 2;
		char szRecvBuffData[1024 * 100] = {'\0'};

		while(true)
		{

			//如果发送成功了，则处理接收数据
			nCurrRecvLen = recv(sckClient, (char* )szRecvBuffData + nBeginRecv, nTotalRecvLen, 0);
			if(nCurrRecvLen <= 0)
			{
				closesocket(sckClient);

				DWORD dwError = GetLastError();
				sprintf_s(szResult, 1024, "[e]与[%s:%d]客户端无法接收数据，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

				nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

				m_lstResult.InsertItem(nIndex, _T("带数据头的缓冲(PacketParse)检测"));
				m_lstResult.SetItemText(nIndex, 1, sszResult);

				return false;
			}
			else
			{
				nTotalRecvLen -= nCurrRecvLen;
				if(nTotalRecvLen == 0)
				{
					//接收完成
					break;
				}
				else
				{
					nBeginRecv += nCurrRecvLen;
				}
			}
		}

		//检测接收数据的正确性
		int nRecvPacketSize = 0;
		int nReturnCommand  = 0;
		char szTempData[8]  = {'\0'};

		memcpy_s((char* )&nRecvPacketSize, sizeof(int), (char* )&szRecvBuffData[0], sizeof(int));
		memcpy_s((char* )&nReturnCommand, sizeof(short), (char* )&szRecvBuffData[4], sizeof(short));
		memcpy_s((char* )szTempData, 8 * sizeof(char), (char* )&szRecvBuffData[6], 8 * sizeof(char));

		int nRecvPacketSize1 = 0;
		int nReturnCommand1  = 0;
		char szTempData1[8]  = {'\0'};

		memcpy_s((char* )&nRecvPacketSize1, sizeof(int), (char* )&szRecvBuffData[14], sizeof(int));
		memcpy_s((char* )&nReturnCommand1, sizeof(short), (char* )&szRecvBuffData[18], sizeof(short));
		memcpy_s((char* )szTempData1, 8 * sizeof(char), (char* )&szRecvBuffData[20], 8 * sizeof(char));

		//检测返回包头大小
		if(nRecvPacketSize != 10 || nRecvPacketSize1 != 10)
		{
			closesocket(sckClient);

			sprintf_s(szResult, 1024, "[e]与[%s:%d]接收返回包大小不对。", objClientInfo.m_szServerIP, objClientInfo.m_nPort);

			nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

			m_lstResult.InsertItem(nIndex, _T("带数据头的缓冲(PacketParse)检测"));
			m_lstResult.SetItemText(nIndex, 1, sszResult);

			return false;
		}

		//判断返回命令字
		if(nReturnCommand != (int)COMMAND_AUTOTEST_RETUEN_HEADBUFF || nReturnCommand1 != COMMAND_AUTOTEST_RETUEN_HEADBUFF)
		{
			closesocket(sckClient);

			sprintf_s(szResult, 1024, "[e]与[%s:%d]接受包返回命令字不对。", objClientInfo.m_szServerIP, objClientInfo.m_nPort);

			nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

			m_lstResult.InsertItem(nIndex, _T("带数据头的缓冲(PacketParse)检测"));
			m_lstResult.SetItemText(nIndex, 1, sszResult);

			return false;
		}

		//判断返回数据
		int nRet  = memcmp(szTempData, objClientInfo.m_pSendBuffer, 8);
		int nRet1 = memcmp(szTempData1, objClientInfo.m_pSendBuffer, 8);
		if(nRet != 0 && nRet1 != 0)
		{
			closesocket(sckClient);

			sprintf_s(szResult, 1024, "[e]与[%s:%d]接受包数据和发送数据不符。", objClientInfo.m_szServerIP, objClientInfo.m_nPort);

			nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

			m_lstResult.InsertItem(nIndex, _T("带数据头的缓冲(PacketParse)检测"));
			m_lstResult.SetItemText(nIndex, 1, sszResult);

			return false;
		}
	}

	sprintf_s(szResult, 1024, "[s]与[%s:%d]数据包检测成功。", objClientInfo.m_szServerIP, objClientInfo.m_nPort);

	nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

	m_lstResult.InsertItem(nIndex, _T("带数据头的缓冲(PacketParse)检测"));
	m_lstResult.SetItemText(nIndex, 1, sszResult);

	closesocket(sckClient);

	return true;
}

bool CTcpPacketCheckDlg::CheckIsNoHeadBuffer( _ClientInfo& objClientInfo, int nIndex )
{
	SOCKET sckClient;
	char szResult[1024]     = {'\0'};
	wchar_t sszResult[1024] = {'\0'};
	char szSession[32]      = {'\0'}; 
	int nSrcLen = 0;
	int nDecLen = 0;

	sprintf_s(szSession, 32, "FREEEYES");

	//socket创建的准备工作
	struct sockaddr_in sockaddr;

	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port   = htons(objClientInfo.m_nPort);
	sockaddr.sin_addr.S_un.S_addr = inet_addr(objClientInfo.m_szServerIP);

	sckClient = socket(AF_INET, SOCK_STREAM, 0);

	DWORD TimeOut = (DWORD)2000;
	::setsockopt(sckClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&TimeOut, sizeof(TimeOut));

	//连接远程服务器
	int nErr = connect(sckClient, (SOCKADDR*)&sockaddr, sizeof(SOCKADDR));
	if(0 != nErr)
	{
		DWORD dwError = GetLastError();
		sprintf_s(szResult, 1024, "[e]与[%s:%d]服务器连接失败，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

		nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

		m_lstResult.InsertItem(nIndex, _T("不带数据头的缓冲(PacketParse)检测"));
		m_lstResult.SetItemText(nIndex, 1, sszResult);

		return false;
	}

	//拼装发送包体
	char szSendBuffer[MAX_BUFF_200] ={'\0'};

	short sVersion = 1;
	short sCommand = (short)COMMAND_AUTOTEST_NOHEADBUFF;
	int nPacketLen = objClientInfo.m_nSendLength;

	memcpy_s(szSendBuffer, sizeof(short), (char* )&sVersion, sizeof(short));
	memcpy_s((char* )&szSendBuffer[2], sizeof(short), (char* )&sCommand, sizeof(short));
	memcpy_s((char* )&szSendBuffer[4], sizeof(int), (char* )&nPacketLen, sizeof(int));
	memcpy_s((char* )&szSendBuffer[8], sizeof(char)*32, (char* )&szSession, sizeof(char)*32);
	memcpy_s((char* )&szSendBuffer[40], sizeof(char) * objClientInfo.m_nSendLength, (char* )objClientInfo.m_pSendBuffer, sizeof(char) * objClientInfo.m_nSendLength);
	int nSendLen = nPacketLen + 40;

	//发送数据
	int nTotalSendLen = nSendLen;
	int nBeginSend    = 0;
	int nCurrSendLen  = 0;
	bool blSendFlag   = false;
	int nBeginRecv    = 0;
	int nCurrRecvLen  = 0;
	bool blRecvFlag   = false;
	while(true)
	{
		nCurrSendLen = send(sckClient, szSendBuffer + nBeginSend, nTotalSendLen, 0);
		if(nCurrSendLen <= 0)
		{
			closesocket(sckClient);

			DWORD dwError = GetLastError();
			sprintf_s(szResult, 1024, "[e]与[%s:%d]服务器发送数据失败，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

			nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

			m_lstResult.InsertItem(nIndex, _T("不带数据头的缓冲(PacketParse)检测"));
			m_lstResult.SetItemText(nIndex, 1, sszResult);

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

	if(blSendFlag == false)
	{
		closesocket(sckClient);

		sprintf_s(szResult, 1024, "[e]与[%s:%d]发送字节数不匹配，缓冲字节数[%d]，实际发送字节数[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, objClientInfo.m_nSendLength, nTotalSendLen);

		nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

		m_lstResult.InsertItem(nIndex, _T("不带数据头的缓冲(PacketParse)检测"));
		m_lstResult.SetItemText(nIndex, 1, sszResult);

		return false;
	}
	else
	{
		int nTotalRecvLen               = objClientInfo.m_nRecvLength;
		char szRecvBuffData[1024 * 100] = {'\0'};

		while(true)
		{

			//如果发送成功了，则处理接收数据
			nCurrRecvLen = recv(sckClient, (char* )szRecvBuffData + nBeginRecv, nTotalRecvLen, 0);
			if(nCurrRecvLen <= 0)
			{
				closesocket(sckClient);

				DWORD dwError = GetLastError();
				sprintf_s(szResult, 1024, "[e]与[%s:%d]客户端无法接收数据，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

				nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

				m_lstResult.InsertItem(nIndex, _T("不带数据头的缓冲(PacketParse)检测"));
				m_lstResult.SetItemText(nIndex, 1, sszResult);

				return false;
			}
			else
			{
				nTotalRecvLen -= nCurrRecvLen;
				if(nTotalRecvLen == 0)
				{
					//接收完成
					break;
				}
				else
				{
					nBeginRecv += nCurrRecvLen;
				}
			}
		}

		//检测接收数据的正确性
		int nRecvPacketSize = 0;
		int nReturnCommand  = 0;
		int nData1          = 0;
		int nData2          = 0; 

		memcpy_s((char* )&nRecvPacketSize, sizeof(int), (char* )&szRecvBuffData[0], sizeof(int));
		memcpy_s((char* )&nReturnCommand, sizeof(short), (char* )&szRecvBuffData[4], sizeof(short));
		memcpy_s((char* )&nData1, sizeof(int), (char* )&szRecvBuffData[6], sizeof(int));
		memcpy_s((char* )&nData2, sizeof(int), (char* )&szRecvBuffData[10], sizeof(int));

		//检测返回包头大小
		if(nRecvPacketSize != 10)
		{
			closesocket(sckClient);

			sprintf_s(szResult, 1024, "[e]与[%s:%d]接收返回包大小不对。", objClientInfo.m_szServerIP, objClientInfo.m_nPort);

			nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

			m_lstResult.InsertItem(nIndex, _T("不带数据头的缓冲(PacketParse)检测"));
			m_lstResult.SetItemText(nIndex, 1, sszResult);

			return false;
		}

		//判断返回命令字
		if(nReturnCommand != (int)COMMAND_AUTOTEST_RETUEN_NOHEADBUFF)
		{
			closesocket(sckClient);

			sprintf_s(szResult, 1024, "[e]与[%s:%d]接受包返回命令字不对。", objClientInfo.m_szServerIP, objClientInfo.m_nPort);

			nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

			m_lstResult.InsertItem(nIndex, _T("不带数据头的缓冲(PacketParse)检测"));
			m_lstResult.SetItemText(nIndex, 1, sszResult);

			return false;
		}

		//判断返回数据
		if(nData1 != 32 || nData2 != 64)
		{
			closesocket(sckClient);

			sprintf_s(szResult, 1024, "[e]与[%s:%d]接受包数据和发送数据不符。", objClientInfo.m_szServerIP, objClientInfo.m_nPort);

			nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

			m_lstResult.InsertItem(nIndex, _T("不带数据头的缓冲(PacketParse)检测"));
			m_lstResult.SetItemText(nIndex, 1, sszResult);

			return false;
		}
	}

	sprintf_s(szResult, 1024, "[s]与[%s:%d]数据包检测成功。", objClientInfo.m_szServerIP, objClientInfo.m_nPort);

	nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

	m_lstResult.InsertItem(nIndex, _T("不带数据头的缓冲(PacketParse)检测"));
	m_lstResult.SetItemText(nIndex, 1, sszResult);

	closesocket(sckClient);

	return true;
}


bool CTcpPacketCheckDlg::CheckLogFile( _ClientInfo& objClientInfo, int nIndex )
{
	SOCKET sckClient;
	char szResult[1024]     = {'\0'};
	wchar_t sszResult[1024] = {'\0'};
	char szSession[32]      = {'\0'}; 
	int nSrcLen = 0;
	int nDecLen = 0;

	sprintf_s(szSession, 32, "FREEEYES");

	int nLogCount = 2;

	for(int i = 0; i < nLogCount; i++)
	{
		//socket创建的准备工作
		struct sockaddr_in sockaddr;

		memset(&sockaddr, 0, sizeof(sockaddr));
		sockaddr.sin_family = AF_INET;
		sockaddr.sin_port   = htons(objClientInfo.m_nPort);
		sockaddr.sin_addr.S_un.S_addr = inet_addr(objClientInfo.m_szServerIP);

		sckClient = socket(AF_INET, SOCK_STREAM, 0);

		DWORD TimeOut = (DWORD)2000;
		::setsockopt(sckClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&TimeOut, sizeof(TimeOut));

		//连接远程服务器
		int nErr = connect(sckClient, (SOCKADDR*)&sockaddr, sizeof(SOCKADDR));
		if(0 != nErr)
		{
			DWORD dwError = GetLastError();
			sprintf_s(szResult, 1024, "[e]与[%s:%d]服务器连接失败，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

			nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

			m_lstResult.InsertItem(nIndex, _T("日志信息接口检测"));
			m_lstResult.SetItemText(nIndex, 1, sszResult);

			return false;
		}

		//拼装发送包体
		char szSendBuffer[MAX_BUFF_200] ={'\0'};

		short sVersion = 1;
		short sCommand = (short)COMMAND_AUTOTEST_LOGDATA;
		int nPacketLen = 4;

		memcpy_s(szSendBuffer, sizeof(short), (char* )&sVersion, sizeof(short));
		memcpy_s((char* )&szSendBuffer[2], sizeof(short), (char* )&sCommand, sizeof(short));
		memcpy_s((char* )&szSendBuffer[4], sizeof(int), (char* )&nPacketLen, sizeof(int));
		memcpy_s((char* )&szSendBuffer[8], sizeof(char)*32, (char* )&szSession, sizeof(char)*32);
		memcpy_s((char* )&szSendBuffer[40], sizeof(int), (char* )&i, sizeof(int));
		int nSendLen = 40 + 4;

		//发送数据
		int nTotalSendLen = nSendLen;
		int nBeginSend    = 0;
		int nCurrSendLen  = 0;
		bool blSendFlag   = false;
		int nBeginRecv    = 0;
		int nCurrRecvLen  = 0;
		bool blRecvFlag   = false;
		while(true)
		{
			nCurrSendLen = send(sckClient, szSendBuffer + nBeginSend, nTotalSendLen, 0);
			if(nCurrSendLen <= 0)
			{
				closesocket(sckClient);

				DWORD dwError = GetLastError();
				sprintf_s(szResult, 1024, "[e]与[%s:%d]服务器发送数据失败，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

				nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

				m_lstResult.InsertItem(nIndex, _T("日志信息接口检测"));
				m_lstResult.SetItemText(nIndex, 1, sszResult);

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

		if(blSendFlag == false)
		{
			closesocket(sckClient);

			sprintf_s(szResult, 1024, "[e]与[%s:%d]发送字节数不匹配，缓冲字节数[%d]，实际发送字节数[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, objClientInfo.m_nSendLength, nTotalSendLen);

			nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

			m_lstResult.InsertItem(nIndex, _T("日志信息接口检测"));
			m_lstResult.SetItemText(nIndex, 1, sszResult);

			return false;
		}
		else
		{
			int nTotalRecvLen               = 10;
			char szRecvBuffData[1024 * 100] = {'\0'};

			while(true)
			{

				//如果发送成功了，则处理接收数据
				nCurrRecvLen = recv(sckClient, (char* )szRecvBuffData + nBeginRecv, nTotalRecvLen, 0);
				if(nCurrRecvLen <= 0)
				{
					closesocket(sckClient);

					DWORD dwError = GetLastError();
					sprintf_s(szResult, 1024, "[e]与[%s:%d]客户端无法接收数据，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

					nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

					m_lstResult.InsertItem(nIndex, _T("日志信息接口检测"));
					m_lstResult.SetItemText(nIndex, 1, sszResult);

					return false;
				}
				else
				{
					nTotalRecvLen -= nCurrRecvLen;
					if(nTotalRecvLen == 0)
					{
						//接收完成
						break;
					}
					else
					{
						nBeginRecv += nCurrRecvLen;
					}
				}
			}

			//检测接收数据的正确性
			int nRecvPacketSize = 0;
			int nReturnCommand  = 0;
			int nResult         = 0;

			memcpy_s((char* )&nRecvPacketSize, sizeof(int), (char* )&szRecvBuffData[0], sizeof(int));
			memcpy_s((char* )&nReturnCommand, sizeof(short), (char* )&szRecvBuffData[4], sizeof(short));
			memcpy_s((char* )&nResult, sizeof(int), (char* )&szRecvBuffData[6], sizeof(int));

			//检测返回包头大小
			if(nRecvPacketSize != 6)
			{
				closesocket(sckClient);

				sprintf_s(szResult, 1024, "[e]与[%s:%d]接收返回包大小不对。", objClientInfo.m_szServerIP, objClientInfo.m_nPort);

				nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

				m_lstResult.InsertItem(nIndex, _T("日志信息接口检测"));
				m_lstResult.SetItemText(nIndex, 1, sszResult);

				return false;
			}

			//判断返回命令字
			if(nReturnCommand != (int)COMMAND_AUTOTEST_RETUEN_LOGDATA)
			{
				closesocket(sckClient);

				sprintf_s(szResult, 1024, "[e]与[%s:%d]接受包返回命令字不对。", objClientInfo.m_szServerIP, objClientInfo.m_nPort);

				nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

				m_lstResult.InsertItem(nIndex, _T("日志信息接口检测"));
				m_lstResult.SetItemText(nIndex, 1, sszResult);

				return false;
			}

			//判断返回数据
			if(nResult != 0)
			{
				closesocket(sckClient);

				sprintf_s(szResult, 1024, "[e]与[%s:%d]接受包数据和发送数据不符。", objClientInfo.m_szServerIP, objClientInfo.m_nPort);

				nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

				m_lstResult.InsertItem(nIndex, _T("日志信息接口检测"));
				m_lstResult.SetItemText(nIndex, 1, sszResult);

				return false;
			}
		}

		closesocket(sckClient);

		//等待100毫秒
		//SleepEx(100, TRUE);
	}

	sprintf_s(szResult, 1024, "[s]与[%s:%d](%d)个日志数据包检测成功。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, nLogCount);

	nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

	m_lstResult.InsertItem(nIndex, _T("日志信息接口检测"));
	m_lstResult.SetItemText(nIndex, 1, sszResult);

	return true;
}

bool CTcpPacketCheckDlg::CheckWorkTimeout( _ClientInfo& objClientInfo, int nIndex )
{
	SOCKET sckClient;
	char szResult[1024]     = {'\0'};
	wchar_t sszResult[1024] = {'\0'};
	char szSession[32]      = {'\0'}; 
	int nSrcLen = 0;
	int nDecLen = 0;

	sprintf_s(szSession, 32, "FREEEYES");

	int nLogCount = 2;

	for(int i = 0; i < nLogCount; i++)
	{
		//socket创建的准备工作
		struct sockaddr_in sockaddr;

		memset(&sockaddr, 0, sizeof(sockaddr));
		sockaddr.sin_family = AF_INET;
		sockaddr.sin_port   = htons(objClientInfo.m_nPort);
		sockaddr.sin_addr.S_un.S_addr = inet_addr(objClientInfo.m_szServerIP);

		sckClient = socket(AF_INET, SOCK_STREAM, 0);

		DWORD TimeOut = (DWORD)2000;
		::setsockopt(sckClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&TimeOut, sizeof(TimeOut));

		//连接远程服务器
		int nErr = connect(sckClient, (SOCKADDR*)&sockaddr, sizeof(SOCKADDR));
		if(0 != nErr)
		{
			DWORD dwError = GetLastError();
			sprintf_s(szResult, 1024, "[e]与[%s:%d]服务器连接失败，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

			nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

			m_lstResult.InsertItem(nIndex, _T("工作线程自我修复检测"));
			m_lstResult.SetItemText(nIndex, 1, sszResult);

			return false;
		}

		//拼装发送包体
		char szSendBuffer[MAX_BUFF_200] ={'\0'};

		short sVersion = 1;
		short sCommand = (short)COMMAND_AUTOTEST_WORKTIMEOUT;
		int nPacketLen = 4;

		memcpy_s(szSendBuffer, sizeof(short), (char* )&sVersion, sizeof(short));
		memcpy_s((char* )&szSendBuffer[2], sizeof(short), (char* )&sCommand, sizeof(short));
		memcpy_s((char* )&szSendBuffer[4], sizeof(int), (char* )&nPacketLen, sizeof(int));
		memcpy_s((char* )&szSendBuffer[8], sizeof(char)*32, (char* )&szSession, sizeof(char)*32);
		memcpy_s((char* )&szSendBuffer[40], sizeof(int), (char* )&i, sizeof(int));
		int nSendLen = 4 + 40;

		//发送数据
		int nTotalSendLen = nSendLen;
		int nBeginSend    = 0;
		int nCurrSendLen  = 0;
		bool blSendFlag   = false;
		int nBeginRecv    = 0;
		int nCurrRecvLen  = 0;
		bool blRecvFlag   = false;
		while(true)
		{
			nCurrSendLen = send(sckClient, szSendBuffer + nBeginSend, nTotalSendLen, 0);
			if(nCurrSendLen <= 0)
			{
				closesocket(sckClient);

				DWORD dwError = GetLastError();
				sprintf_s(szResult, 1024, "[e]与[%s:%d]服务器发送数据失败，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

				nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

				m_lstResult.InsertItem(nIndex, _T("工作线程自我修复检测"));
				m_lstResult.SetItemText(nIndex, 1, sszResult);

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

		if(blSendFlag == false)
		{
			closesocket(sckClient);

			sprintf_s(szResult, 1024, "[e]与[%s:%d]发送字节数不匹配，缓冲字节数[%d]，实际发送字节数[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, objClientInfo.m_nSendLength, nTotalSendLen);

			nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

			m_lstResult.InsertItem(nIndex, _T("工作线程自我修复检测"));
			m_lstResult.SetItemText(nIndex, 1, sszResult);

			return false;
		}
		else
		{
			if(i > 0)
			{
				int nTotalRecvLen               = 10;
				char szRecvBuffData[1024 * 100] = {'\0'};

				//首先接收4字节
				int nPacketSize     = 0;
				char szPacketLen[4] = {'\0'};  
				nCurrRecvLen = recv(sckClient, (char* )szPacketLen, sizeof(int), 0);
				if(nCurrRecvLen != sizeof(int))
				{
					closesocket(sckClient);

					DWORD dwError = GetLastError();
					sprintf_s(szResult, 1024, "[e]与[%s:%d]客户端接收包长失败，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

					nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

					m_lstResult.InsertItem(nIndex, _T("工作线程自我修复检测"));
					m_lstResult.SetItemText(nIndex, 1, sszResult);

					return false;
				}

				memcpy_s((char* )&nPacketSize, sizeof(int), szPacketLen, sizeof(int));
				nTotalRecvLen               = nPacketSize;

				while(true)
				{
					//如果发送成功了，则处理接收数据
					nCurrRecvLen = recv(sckClient, (char* )szRecvBuffData + nBeginRecv, nTotalRecvLen, 0);
					if(nCurrRecvLen <= 0)
					{
						closesocket(sckClient);

						DWORD dwError = GetLastError();
						sprintf_s(szResult, 1024, "[e]与[%s:%d]客户端无法接收数据，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

						nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

						m_lstResult.InsertItem(nIndex, _T("工作线程自我修复检测"));
						m_lstResult.SetItemText(nIndex, 1, sszResult);

						return false;
					}
					else
					{
						nTotalRecvLen -= nCurrRecvLen;
						if(nTotalRecvLen == 0)
						{
							//接收完成
							break;
						}
						else
						{
							nBeginRecv += nCurrRecvLen;
						}
					}
				}

				//检测接收数据的正确性
				int nRecvPacketSize = 0;
				int nReturnCommand  = 0;
				int nResult         = 0;

				memcpy_s((char* )&nReturnCommand, sizeof(short), (char* )&szRecvBuffData[0], sizeof(short));
				memcpy_s((char* )&nResult, sizeof(int), (char* )&szRecvBuffData[2], sizeof(int));

				//判断返回命令字
				if(nReturnCommand != (int)COMMAND_AUTOTEST_RETURN_WORKTIMEOUT && nReturnCommand != 0xffff)
				{
					closesocket(sckClient);

					sprintf_s(szResult, 1024, "[e]与[%s:%d]接受包返回命令字不对。", objClientInfo.m_szServerIP, objClientInfo.m_nPort);

					nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

					m_lstResult.InsertItem(nIndex, _T("工作线程自我修复检测"));
					m_lstResult.SetItemText(nIndex, 1, sszResult);

					return false;
				}

				//判断返回数据
				if(nResult != 0)
				{
					closesocket(sckClient);

					sprintf_s(szResult, 1024, "[e]与[%s:%d]接受包数据和发送数据不符。", objClientInfo.m_szServerIP, objClientInfo.m_nPort);

					nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

					m_lstResult.InsertItem(nIndex, _T("工作线程自我修复检测"));
					m_lstResult.SetItemText(nIndex, 1, sszResult);

					return false;
				}
			}
		}

		closesocket(sckClient);

		//只等一次
		if(i == 0)
		{
			//等待15秒
			SleepEx(20000, TRUE);
		}
	}

	sprintf_s(szResult, 1024, "[s]与[%s:%d]框架工作线程自我修复功能检测成功。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, nLogCount);

	nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

	m_lstResult.InsertItem(nIndex, _T("工作线程自我修复检测"));
	m_lstResult.SetItemText(nIndex, 1, sszResult);

	return true;
}

bool CTcpPacketCheckDlg::CheckWorkAI( _ClientInfo& objClientInfo, int nIndex )
{
	SOCKET sckClient;
	char szResult[1024]     = {'\0'};
	wchar_t sszResult[1024] = {'\0'};
	char szSession[32]      = {'\0'}; 
	int nSrcLen = 0;
	int nDecLen = 0;

	sprintf_s(szSession, 32, "FREEEYES");

	bool blIsAlert = false;

	int nLogCount = 3;

	for(int i = 0; i < nLogCount; i++)
	{
		//socket创建的准备工作
		struct sockaddr_in sockaddr;

		memset(&sockaddr, 0, sizeof(sockaddr));
		sockaddr.sin_family = AF_INET;
		sockaddr.sin_port   = htons(objClientInfo.m_nPort);
		sockaddr.sin_addr.S_un.S_addr = inet_addr(objClientInfo.m_szServerIP);

		sckClient = socket(AF_INET, SOCK_STREAM, 0);

		DWORD TimeOut = (DWORD)2000;
		::setsockopt(sckClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&TimeOut, sizeof(TimeOut));

		//连接远程服务器
		int nErr = connect(sckClient, (SOCKADDR*)&sockaddr, sizeof(SOCKADDR));
		if(0 != nErr)
		{
			DWORD dwError = GetLastError();
			sprintf_s(szResult, 1024, "[e]与[%s:%d]服务器连接失败，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

			nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

			m_lstResult.InsertItem(nIndex, _T("工作线程AI检测"));
			m_lstResult.SetItemText(nIndex, 1, sszResult);

			return false;
		}

		//拼装发送包体
		char szSendBuffer[MAX_BUFF_200] ={'\0'};

		short sVersion = 1;
		short sCommand = (short)COMMAND_AUTOTEST_WORKAI;
		int nPacketLen = 4;

		memcpy_s(szSendBuffer, sizeof(short), (char* )&sVersion, sizeof(short));
		memcpy_s((char* )&szSendBuffer[2], sizeof(short), (char* )&sCommand, sizeof(short));
		memcpy_s((char* )&szSendBuffer[4], sizeof(int), (char* )&nPacketLen, sizeof(int));
		memcpy_s((char* )&szSendBuffer[8], sizeof(char)*32, (char* )&szSession, sizeof(char)*32);
		memcpy_s((char* )&szSendBuffer[40], sizeof(int), (char* )&i, sizeof(int));
		int nSendLen = 40 + 4;

		//发送数据
		int nTotalSendLen = nSendLen;
		int nBeginSend    = 0;
		int nCurrSendLen  = 0;
		bool blSendFlag   = false;
		int nBeginRecv    = 0;
		int nCurrRecvLen  = 0;
		bool blRecvFlag   = false;
		while(true)
		{
			nCurrSendLen = send(sckClient, szSendBuffer + nBeginSend, nTotalSendLen, 0);
			if(nCurrSendLen <= 0)
			{
				closesocket(sckClient);

				DWORD dwError = GetLastError();
				sprintf_s(szResult, 1024, "[e]与[%s:%d]服务器发送数据失败，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

				nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

				m_lstResult.InsertItem(nIndex, _T("工作线程AI检测"));
				m_lstResult.SetItemText(nIndex, 1, sszResult);

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

		if(blSendFlag == false)
		{
			closesocket(sckClient);

			sprintf_s(szResult, 1024, "[e]与[%s:%d]发送字节数不匹配，缓冲字节数[%d]，实际发送字节数[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, objClientInfo.m_nSendLength, nTotalSendLen);

			nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

			m_lstResult.InsertItem(nIndex, _T("工作线程AI检测"));
			m_lstResult.SetItemText(nIndex, 1, sszResult);

			return false;
		}
		else
		{
			char szRecvBuffData[1024 * 100] = {'\0'};

			//首先接收4字节
			int nPacketSize     = 0;
			char szPacketLen[4] = {'\0'};  
			nCurrRecvLen = recv(sckClient, (char* )szPacketLen, sizeof(int), 0);
			if(nCurrRecvLen != sizeof(int))
			{
				closesocket(sckClient);

				DWORD dwError = GetLastError();
				sprintf_s(szResult, 1024, "[e]与[%s:%d]客户端接收包长失败，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

				nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

				m_lstResult.InsertItem(nIndex, _T("工作线程AI检测"));
				m_lstResult.SetItemText(nIndex, 1, sszResult);

				return false;
			}

			memcpy_s((char* )&nPacketSize, sizeof(int), szPacketLen, sizeof(int));
			int nTotalRecvLen               = nPacketSize;

			while(true)
			{
				//如果发送成功了，则处理接收数据
				nCurrRecvLen = recv(sckClient, (char* )szRecvBuffData + nBeginRecv, nTotalRecvLen, 0);
				if(nCurrRecvLen <= 0)
				{
					closesocket(sckClient);

					DWORD dwError = GetLastError();
					sprintf_s(szResult, 1024, "[e]与[%s:%d]客户端无法接收数据，错误号[%d]。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, dwError);

					nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

					m_lstResult.InsertItem(nIndex, _T("工作线程AI检测"));
					m_lstResult.SetItemText(nIndex, 1, sszResult);

					return false;
				}
				else
				{
					nTotalRecvLen -= nCurrRecvLen;
					if(nTotalRecvLen == 0)
					{
						//接收完成
						break;
					}
					else
					{
						nBeginRecv += nCurrRecvLen;
					}
				}
			}

			//检测接收数据的正确性
			int nRecvPacketSize = 0;
			int nReturnCommand  = 0;
			int nResult         = 0;

			memcpy_s((char* )&nReturnCommand, sizeof(short), (char* )&szRecvBuffData[0], sizeof(short));

			//判断返回命令字
			if(nReturnCommand != (int)COMMAND_AUTOTEST_RETURN_WORKAI && nReturnCommand != 0xffff)
			{
				closesocket(sckClient);

				sprintf_s(szResult, 1024, "[e]与[%s:%d]接受包返回命令字不对(当前返回0x%04x)。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, nReturnCommand);

				nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

				m_lstResult.InsertItem(nIndex, _T("工作线程AI检测"));
				m_lstResult.SetItemText(nIndex, 1, sszResult);

				return false;
			}
			else
			{
				CString strTrace;
				strTrace.Format(_T("CommandID=0x%04x.\n"), nReturnCommand);
				TRACE(strTrace);
				if(nReturnCommand == 0xffff)
				{
					blIsAlert = true;
				}
			}
		}

		closesocket(sckClient);

		//等待100毫秒
		SleepEx(100, TRUE);
	}

	if(blIsAlert == false)
	{
		sprintf_s(szResult, 1024, "[e]与[%s:%d]没有检测到应急包。", objClientInfo.m_szServerIP, objClientInfo.m_nPort);

		nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

		m_lstResult.InsertItem(nIndex, _T("工作线程AI检测"));
		m_lstResult.SetItemText(nIndex, 1, sszResult);

		return false;
	}

	sprintf_s(szResult, 1024, "[s]与[%s:%d](%d)个日志数据包检测成功。", objClientInfo.m_szServerIP, objClientInfo.m_nPort, nLogCount);

	nDecLen = MultiByteToWideChar(CP_ACP, 0, szResult, -1, sszResult, 1024);

	m_lstResult.InsertItem(nIndex, _T("工作线程AI检测"));
	m_lstResult.SetItemText(nIndex, 1, sszResult);

	return true;
}

//处理行颜色
void CTcpPacketCheckDlg::OnCustomDraw( NMHDR* pNMHDR, LRESULT* pResult )
{
	LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR; 
	*pResult  =  CDRF_DODEFAULT;

	switch (lplvcd->nmcd.dwDrawStage) 
	{ 
	case CDDS_PREPAINT : 
		{ 
			*pResult = CDRF_NOTIFYITEMDRAW; 
			return; 
		} 
	case CDDS_ITEMPREPAINT: 
		{ 
			*pResult = CDRF_NOTIFYSUBITEMDRAW; 
			return; 
		} 
	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
		{ 

			int nItem = static_cast<int>(lplvcd->nmcd.dwItemSpec);
			COLORREF clrNewTextColor, clrNewBkColor;

			//判断文字中的关键字
			CString strText;
			strText = m_lstResult.GetItemText(nItem, 1);

			//寻找关键字
			int nColor = 0;

			if(strText.Find(_T("[s]")) >= 0)
			{
				nColor = 1;
			}
			else if(strText.Find(_T("[e]")) >= 0)
			{
				nColor = 2;
			}

			clrNewTextColor=RGB(0,0,0);
			if(nColor == 1)
			{
				clrNewBkColor = RGB(0, 255, 0); //成功，绿色 
			}
			else if(nColor == 2)
			{
				clrNewBkColor = RGB(255, 0, 0); //失败，红色
			}
			else
			{
				clrNewBkColor = RGB(255, 251, 240);   //未知，白色
			}

			lplvcd->clrText = clrNewTextColor;
			lplvcd->clrTextBk = clrNewBkColor;

			*pResult = CDRF_DODEFAULT;  
			return; 
		} 
	} 
}

void CTcpPacketCheckDlg::Run()
{
	_ClientInfo objClientInfo;

	//获得发送的IP和端口
	CString strServerIP;
	CString strPort;
	char szServerIP[30]   = {'\0'};
	char szServerPort[20] = {'\0'};

	m_txtServerIP.GetWindowText(strServerIP);
	m_txtPort.GetWindowText(strPort);

	int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strServerIP, strServerIP.GetLength(), NULL, 0, NULL, NULL);
	int nDecLen = WideCharToMultiByte(CP_ACP, 0, strServerIP, nSrcLen, szServerIP, 30, NULL,NULL);
	szServerIP[nDecLen] = '\0';

	nSrcLen = WideCharToMultiByte(CP_ACP, 0, strPort, strPort.GetLength(), NULL, 0, NULL, NULL);
	nDecLen = WideCharToMultiByte(CP_ACP, 0, strPort, nSrcLen, szServerPort, 30, NULL,NULL);
	szServerPort[nDecLen] = '\0';

	if(strlen(szServerPort) == 0 || strlen(szServerIP) == 0)
	{
		MessageBox(_T("必须输入要发送的IP和端口") , _T("错误信息"), MB_OK);
		return;
	}

	int nServerPort = atoi(szServerPort);

	sprintf_s(objClientInfo.m_szServerIP, 30, "%s", szServerIP);
	objClientInfo.m_nPort = nServerPort;

	//获得验证包长度
	CString strRecvLength;
	char szRecvLength[10] = {'\0'};

	m_txtRecvLength.GetWindowText(strRecvLength);

	nSrcLen = WideCharToMultiByte(CP_ACP, 0, strRecvLength, strRecvLength.GetLength(), NULL, 0, NULL, NULL);
	nDecLen = WideCharToMultiByte(CP_ACP, 0, strRecvLength, nSrcLen, szRecvLength, 30, NULL,NULL);
	szRecvLength[nDecLen] = '\0';

	if(strlen(szRecvLength) == 0)
	{
		MessageBox(_T("必须输入验证接收字节长度") , _T("错误信息"), MB_OK);
		return;
	}

	objClientInfo.m_nRecvLength = atoi(szRecvLength);

	//获取发送Buffer
	CString strPacketBuffer;
	char szPacketBuffer[500] = {'\0'};

	m_txtPacketBuffer.GetWindowText(strPacketBuffer);

	nSrcLen = WideCharToMultiByte(CP_ACP, 0, strPacketBuffer, strPacketBuffer.GetLength(), NULL, 0, NULL, NULL);
	nDecLen = WideCharToMultiByte(CP_ACP, 0, strPacketBuffer, nSrcLen, szPacketBuffer, 500, NULL,NULL);
	szPacketBuffer[nDecLen] = '\0';

	if(strlen(szPacketBuffer) == 0)
	{
		MessageBox(_T("必须输入发送数据包") , _T("错误信息"), MB_OK);
		return;
	}

	CConvertBuffer objConvertBuffer;
	int nSendLength = objConvertBuffer.GetBufferSize(szPacketBuffer, nDecLen);
	char* pSendBuffer = new char[nSendLength];
	objConvertBuffer.Convertstr2charArray(szPacketBuffer, strlen(szPacketBuffer), (unsigned char*)pSendBuffer, nSendLength);

	objClientInfo.m_pSendBuffer = pSendBuffer;
	objClientInfo.m_nSendLength = nSendLength;

	//开始测试
	int nIndex = 0;
	m_lstResult.DeleteAllItems();

	//单个包检测
	CheckTcpPacket(objClientInfo, nIndex++);

	//多个包检测
	CheckMultipleTcpPacket(objClientInfo, nIndex++);

	//无效包检测
	CheckValidPacket(objClientInfo, nIndex++);

	//任意切割数据包检测
	CheckHalfPacket(objClientInfo, nIndex++);

	//测试数据包包含包头
	CheckIsHead(objClientInfo, nIndex++);

	//检测直接返回数据包(透传指令)
	CheckIsNoHead(objClientInfo, nIndex++);

	//检测带包头的缓冲
	CheckIsHeadBuffer(objClientInfo, nIndex++);

	//检测不带包头缓冲
	CheckIsNoHeadBuffer(objClientInfo, nIndex++);

	//测试日志记录
	CheckLogFile(objClientInfo, nIndex++);

	//测试工作线程自我修复
	CheckWorkTimeout(objClientInfo, nIndex++);

	//检测工作线程AI
	CheckWorkAI(objClientInfo, nIndex++);

	delete[] pSendBuffer;
}

