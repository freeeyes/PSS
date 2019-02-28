// PSSProxyClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PSSProxyClient.h"
#include "PSSProxyClientDlg.h"

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


// CPSSProxyClientDlg 对话框




CPSSProxyClientDlg::CPSSProxyClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPSSProxyClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPSSProxyClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_txtServerIP);
	DDX_Control(pDX, IDC_EDIT3, m_txtServerPort);
	DDX_Control(pDX, IDC_EDIT1, m_txtSendData);
}

BEGIN_MESSAGE_MAP(CPSSProxyClientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CPSSProxyClientDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CPSSProxyClientDlg 消息处理程序

BOOL CPSSProxyClientDlg::OnInitDialog()
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
	InitView();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPSSProxyClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPSSProxyClientDlg::OnPaint()
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
HCURSOR CPSSProxyClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPSSProxyClientDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strData;
	m_txtServerIP.GetWindowText(strData);

	char szServerIP[MAX_BUFF_20] = {'\0'};
	int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
	int nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, szServerIP, MAX_BUFF_20, NULL,NULL);
	szServerIP[nDecLen] = '\0';

	m_txtServerPort.GetWindowText(strData);
	int nServerPort = _ttoi((LPCTSTR)strData);

	m_txtSendData.GetWindowText(strData);

	nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
	int nBufferSize = WideCharToMultiByte(CP_ACP, 0, strData, -1, NULL, 0, NULL, NULL);  
	char* pSendData = new char[nBufferSize];
	nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, pSendData, nBufferSize, NULL,NULL);
	pSendData[nDecLen] = '\0';

	CConvertBuffer objConvertBuffer;
	//获得要转换的数据块大小
	int nBuffSize = objConvertBuffer.GetBufferSize(pSendData, nDecLen);
	char* pSendBuff = new char[nBuffSize];
	//将数据串转换成二进制串
	objConvertBuffer.Convertstr2charArray(pSendData, strlen(pSendData), (unsigned char*)pSendBuff, nBuffSize);
	delete[] pSendData;

	//开始发送数据
	if(m_sckClient != INVALID_SOCKET)
	{
		Close();
	}

	if(Connect(szServerIP, nServerPort) == false)
	{
		return;
	}

	Send_Data(pSendBuff, nBuffSize);

	Close();

	delete[] pSendBuff;
}

void CPSSProxyClientDlg::InitView()
{
	m_txtServerIP.SetWindowText(_T("127.0.0.1"));
	m_txtServerPort.SetWindowText(_T("10002"));

	m_txtSendData.SetWindowText(_T("01 00 30 10 08 00 00 00 46 45 45 45 45 59 45 53 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 be cd aa 8f 3c 01 00 00"));

	//初始化TCP链接
	WSADATA wsaData;
	int nErr = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(nErr != 0)
	{
		MessageBox(_T("本机socket库加载失败，请检查本机socket库版本"), _T("错误信息"), MB_OK);
	}

	m_sckClient = INVALID_SOCKET;
}

bool CPSSProxyClientDlg::Connect(const char* pIP, int nPort)
{
	//socket创建的准备工作
	struct sockaddr_in sockaddr;

	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port   = htons(nPort);
	sockaddr.sin_addr.S_un.S_addr = inet_addr(pIP);

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

void CPSSProxyClientDlg::Close()
{
	closesocket(m_sckClient);
	m_sckClient = INVALID_SOCKET;
}

void CPSSProxyClientDlg::Send_Data( const char* pSendBuff, int nLen )
{
	int nTotalSendLen = nLen;
	int nBeginSend    = 0;
	int nCurrSendLen  = 0;
	bool blSendFlag   = false;
	int nBeginRecv    = 0;
	int nCurrRecvLen  = 0;
	bool blRecvFlag   = false;
	while(true)
	{
		nCurrSendLen = send(m_sckClient, pSendBuff + nBeginSend, nTotalSendLen, 0);
		if(nCurrSendLen <= 0)
		{
			DWORD dwError = GetLastError();
			MessageBox(_T("远程服务器发送数据失败"), _T("错误信息"), MB_OK);
			return;
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
	char szRecvLength[30] = {'\0'};
	nCurrRecvLen = recv(m_sckClient, (char* )szRecvLength, 30, 0);
	if(nCurrRecvLen != 30)
	{
		DWORD dwError = GetLastError();
		MessageBox(_T("远程服务器接收数据失败"), _T("错误信息"), MB_OK);
		return;
	}

	int nRecvLength = 0;
	memcpy_s(&nRecvLength, sizeof(int), &szRecvLength[4], sizeof(int));
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
			return;
		}
		else
		{
			nRecvBegin += nCurrRecvLen;
		}
	}

	MessageBox(_T("接收数据成功"), _T("提示信息"), MB_OK);
	return;
}
