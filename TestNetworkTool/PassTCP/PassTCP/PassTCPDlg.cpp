
// PassTCPDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PassTCP.h"
#include "PassTCPDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

//线程执行
DWORD WINAPI ThreadProc(LPVOID argv)
{

	CClientTcpSocket* pClientTcpSocket = (CClientTcpSocket* )argv;
	pClientTcpSocket->Run();

	return 0;
}

DWORD WINAPI ThreadUDPProc(LPVOID argv)
{

	CClientUdpSocket* pClientUdpSocket = (CClientUdpSocket* )argv;
	pClientUdpSocket->Run();

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


// CPassTCPDlg 对话框




CPassTCPDlg::CPassTCPDlg(CWnd* pParent /*=NULL*/)
: CDialog(CPassTCPDlg::IDD, pParent)
, m_nRadio(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPassTCPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_txtServerIP);
	DDX_Control(pDX, IDC_EDIT2, m_txtPort);
	DDX_Control(pDX, IDC_EDIT3, m_txtThreadCount);
	DDX_Control(pDX, IDC_EDIT13, m_txtRecvTimeout);
	DDX_Control(pDX, IDC_EDIT4, m_txtSocketInterval);
	DDX_Control(pDX, IDC_CHECK1, m_chkIsAlwayConnect);
	DDX_Control(pDX, IDC_CHECK3, m_chkRadomaDelay);
	DDX_Control(pDX, IDC_CHECK2, m_chkIsRecv);
	DDX_Control(pDX, IDC_CHECK4, m_ChkIsBroken);
	DDX_Control(pDX, IDC_EDIT6, m_txtSuccessConnect);
	DDX_Control(pDX, IDC_EDIT7, m_txtSuccessSend);
	DDX_Control(pDX, IDC_EDIT8, m_txtSuccessRecv);
	DDX_Control(pDX, IDC_EDIT9, m_txtCurrConnect);
	DDX_Control(pDX, IDC_EDIT10, m_txtFailConnect);
	DDX_Control(pDX, IDC_EDIT11, m_txtFailSend);
	DDX_Control(pDX, IDC_EDIT12, m_txtFailRecv);
	DDX_Control(pDX, IDC_EDIT14, m_txtRecvLength);
	DDX_Control(pDX, IDC_CHECK5, m_chkRadomSendCount);
	DDX_Control(pDX, IDC_CHECK6, m_chkIsWriteLog);
	DDX_Control(pDX, IDC_CHECK7, m_chkSendOne);
	DDX_Radio(pDX, IDC_RADIO1, m_nRadio);
	DDX_Control(pDX, IDC_EDIT15, m_txtClientUdpPort);
	DDX_Control(pDX, IDC_EDIT16, m_txtPacketTimewait);
	DDX_Control(pDX, IDC_EDIT17, m_txtSendByteCount);
	DDX_Control(pDX, IDC_EDIT18, m_txtRecvByteCount);
	DDX_Control(pDX, IDC_COMBO1, m_cbSendBuffStyle);
	DDX_Control(pDX, IDC_RICHEDIT21, m_reSendText);
	//DDX_Control(pDX, IDC_EDIT5, m_reSendText);
	DDX_Control(pDX, IDC_EDIT19, m_txtLuaFilePath);
	DDX_Control(pDX, IDC_CHECK8, m_chkLuaAdvance);
	DDX_Control(pDX, IDC_EDIT20, m_txtSendCount);
	DDX_Control(pDX, IDC_EDIT21, m_txtMinTime);
	DDX_Control(pDX, IDC_EDIT22, m_txtMaxTime);
}

BEGIN_MESSAGE_MAP(CPassTCPDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CPassTCPDlg::OnBnClickedButton1)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON2, &CPassTCPDlg::OnBnClickedButton2)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON3, &CPassTCPDlg::OnBnClickedButton3)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CPassTCPDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON4, &CPassTCPDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CPassTCPDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CPassTCPDlg 消息处理程序

BOOL CPassTCPDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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

	SetTimer(1, 1000, NULL);
	InitRandom();

	// TODO: 在此添加额外的初始化代码
	InitView();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPassTCPDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPassTCPDlg::OnPaint()
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
HCURSOR CPassTCPDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPassTCPDlg::OnBnClickedButton1()
{
	//开始压测代码在这里
	char* pSendData = NULL;
	ClearResult();
	Close();

	CString strData;

	m_txtThreadCount.GetWindowText(strData);
	int nThreadCount = _ttoi((LPCTSTR)strData);
	m_txtSendCount.GetWindowText(strData);
	int nAllSendCount = _ttoi((LPCTSTR)strData);

	//根据线程数计算发送量
	int nThreadSendCount = nAllSendCount / nThreadCount;
	 
	for(int i = 0; i < nThreadCount; i++)
	{
		//读取线程信息
		_Socket_Info* pSocket_Info             = new _Socket_Info();
		_Socket_State_Info* pSocket_State_Info = new _Socket_State_Info();

		ENUM_TYPE_PROTOCOL emType = ENUM_PROTOCOL_TCP;
		//默认TCP类型，0是TCP，1是UDP
		switch(GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO3))
		{
		case IDC_RADIO1:
			{
				pSocket_Info->m_nConnectType = 0;
				emType = ENUM_PROTOCOL_TCP;
				if(m_pLogic == NULL)
				{
					CNomalLogic* pNomalLogic = new CNomalLogic();
					m_pLogic = (CBaseDataLogic* )pNomalLogic;
					pSocket_Info->m_pLogic = m_pLogic;
				}
				else
				{
					pSocket_Info->m_pLogic = m_pLogic;
				}
				break;
			}
		case IDC_RADIO2:
			{
				pSocket_Info->m_nConnectType = 1;
				emType = ENUM_PROTOCOL_UDP;
				if(m_pLogic == NULL)
				{
					CNomalLogic* pNomalLogic = new CNomalLogic();
					m_pLogic =  (CBaseDataLogic* )pNomalLogic;
					pSocket_Info->m_pLogic = m_pLogic;
				}
				else
				{
					pSocket_Info->m_pLogic = m_pLogic;
				}
				break;
			}
		case IDC_RADIO3:
			{
				pSocket_Info->m_nConnectType = 0;
				emType = ENUM_PROTOCOL_WEBSOCKET;
				if(m_pLogic == NULL)
				{
					CWebSocketLogic* pWebSocketLogic = new CWebSocketLogic();
					pSocket_Info->m_pLogic = (CBaseDataLogic* )pWebSocketLogic;
				}
				else
				{
					pSocket_Info->m_pLogic = m_pLogic;
				}
				break;
			}
		}

		m_txtServerIP.GetWindowText(strData);
		int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
		int nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, pSocket_Info->m_szSerevrIP, MAX_BUFF_20, NULL,NULL);
		pSocket_Info->m_szSerevrIP[nDecLen] = '\0';

		m_txtPort.GetWindowText(strData);
		pSocket_Info->m_nPort = _ttoi((LPCTSTR)strData);
		m_txtRecvTimeout.GetWindowText(strData);
		pSocket_Info->m_nRecvTimeout = _ttoi((LPCTSTR)strData);
		m_txtSocketInterval.GetWindowText(strData);
		pSocket_Info->m_nDelaySecond = _ttoi((LPCTSTR)strData);
		m_txtRecvLength.GetWindowText(strData);
		pSocket_Info->m_pLogic->SetRecvLength(_ttoi((LPCTSTR)strData));
		m_txtClientUdpPort.GetWindowText(strData);
		pSocket_Info->m_nUdpClientPort = _ttoi((LPCTSTR)strData);
		m_txtPacketTimewait.GetWindowText(strData);
		pSocket_Info->m_nPacketTimewait = _ttoi((LPCTSTR)strData);

		if(nThreadSendCount > 0)
		{
			if(i == nThreadCount - 1 && nThreadCount > 1)
			{
				pSocket_Info->m_nSendCount = nThreadSendCount + ( nAllSendCount % nThreadSendCount);
			}
			else
			{
				pSocket_Info->m_nSendCount = nThreadSendCount;
			}
		}

		m_reSendText.GetWindowText(strData);


		int nBufferSize = 0;
		if(m_chkLuaAdvance.GetCheck() == BST_CHECKED)
		{
			//如果是Lua文件模式，直接初始化一个100k的数据块
			//然后根据脚本去组织发送数据
			pSocket_Info->m_pLogic->InitSendSize(100 * MAX_BUFF_1024);
		}
		else
		{
			nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
			nBufferSize = WideCharToMultiByte(CP_ACP, 0, strData, -1, NULL, 0, NULL, NULL);  
			pSendData = new char[nBufferSize];
			nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, pSendData, nBufferSize, NULL,NULL);
			pSendData[nDecLen] = '\0';

			if(m_cbSendBuffStyle.GetCurSel() == 0)
			{
				if(emType != ENUM_PROTOCOL_WEBSOCKET)
				{
					//如果是二进制模式
					CConvertBuffer objConvertBuffer;
					//获得要转换的数据块大小
					pSocket_Info->m_pLogic->InitSendSize(objConvertBuffer.GetBufferSize(pSendData, nDecLen));
					//将数据串转换成二进制串
					int nSendLen = nDecLen;
					objConvertBuffer.Convertstr2charArray(pSendData, strlen(pSendData), 
						(unsigned char*)pSocket_Info->m_pLogic->GetSendData(), nSendLen);
				}
				else
				{
					//如果是webSocket模式
					char szOriData[100 * MAX_BUFF_1024] = {'\0'};
					//如果是二进制模式
					CConvertBuffer objConvertBuffer;
					//获得要转换的数据块大小
					pSocket_Info->m_pLogic->InitSendSize(100 * MAX_BUFF_1024);
					//将数据串转换成二进制串
					int nSendLen = 100 * MAX_BUFF_1024;
					objConvertBuffer.Convertstr2charArray(pSendData, strlen(pSendData), 
						(unsigned char*)szOriData, nSendLen);
					
					pSocket_Info->m_pLogic->SetSendBuff(szOriData, nSendLen);
				}
			}
			else
			{
				if(emType != ENUM_PROTOCOL_WEBSOCKET)
				{
					//如果是文本模式
					pSocket_Info->m_pLogic->InitSendSize(nDecLen);
					memcpy_s(pSocket_Info->m_pLogic->GetSendData(), nDecLen, pSendData, nDecLen);
				}
				else
				{
					//如果是webSocket模式
					pSocket_Info->m_pLogic->InitSendSize(100 * MAX_BUFF_1024);
					pSocket_Info->m_pLogic->SetSendBuff(pSendData, nDecLen);
				}
			}

			delete[] pSendData;

		}

		if(m_chkIsAlwayConnect.GetCheck() == BST_CHECKED)
		{
			pSocket_Info->m_blIsAlwayConnect = true;
		}
		else
		{
			pSocket_Info->m_blIsAlwayConnect = false;
		}

		if(m_chkRadomaDelay.GetCheck() == BST_CHECKED)
		{
			pSocket_Info->m_blIsRadomaDelay = true;
		}
		else
		{
			pSocket_Info->m_blIsRadomaDelay = false;
		}

		if(m_chkIsRecv.GetCheck() == BST_CHECKED)
		{
			pSocket_Info->m_blIsRecv = true;
		}
		else
		{
			pSocket_Info->m_blIsRecv = false;
		}

		if(m_ChkIsBroken.GetCheck() == BST_CHECKED)
		{
			pSocket_Info->m_blIsBroken = true;
		}
		else
		{
			pSocket_Info->m_blIsBroken = false;
		}

		if(m_chkRadomSendCount.GetCheck() == BST_CHECKED)
		{
			pSocket_Info->m_blIsSendCount = true;
		}
		else
		{
			pSocket_Info->m_blIsSendCount = false;
		}

		if(m_chkIsWriteLog.GetCheck() == BST_CHECKED)
		{
			pSocket_Info->m_blIsWriteFile = true;
		}
		else
		{
			pSocket_Info->m_blIsWriteFile = false;
		}

		if(m_chkSendOne.GetCheck() == BST_CHECKED)
		{
			pSocket_Info->m_blIsSendOne = true;
		}
		else
		{
			pSocket_Info->m_blIsSendOne = false;
		}

		//是否开启高级模式
		if(m_chkLuaAdvance.GetCheck() == BST_CHECKED)
		{
			pSocket_Info->m_blLuaAdvance = true;

			//如果打开高级模式，则读取文件
			m_txtLuaFilePath.GetWindowText(strData);

			if(strData == "")
			{
				MessageBox(_T("您已经开启了高级模式，必须设置必要的Lua文件名。"));
				delete pSocket_Info;
				delete pSocket_State_Info;
				return;
			}

			nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
			int nBufferSize = WideCharToMultiByte(CP_ACP, 0, strData, -1, NULL, 0, NULL, NULL);  
			nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, pSocket_Info->m_szLuaFileName, nBufferSize, NULL,NULL);
			pSocket_Info->m_szLuaFileName[nDecLen] = '\0';
		}
		else
		{
			pSocket_Info->m_blLuaAdvance = false;
		}

		if(pSocket_Info->m_nConnectType == 0)
		{
			//TCP发送
			CClientTcpSocket* pClientTcpSocket = new CClientTcpSocket();
			pClientTcpSocket->SetSocketThread(pSocket_Info, pSocket_State_Info);
			pClientTcpSocket->SetThreadID(i);

			DWORD  ThreadID = 0;
			CreateThread(NULL, NULL, ThreadProc, (LPVOID)pClientTcpSocket, NULL, &ThreadID);

			m_vecClientTcpSocket.push_back(pClientTcpSocket);
		}
		else
		{
			//UDP发送
			CClientUdpSocket* pClientUdpSocket = new CClientUdpSocket();
			pClientUdpSocket->SetSocketThread(pSocket_Info, pSocket_State_Info);

			DWORD  ThreadID = 0;
			CreateThread(NULL, NULL, ThreadUDPProc, (LPVOID)pClientUdpSocket, NULL, &ThreadID);

			m_vecClientUdpSocket.push_back(pClientUdpSocket);
		}

		m_tmBegin = CTime::GetCurrentTime();
		m_blIsRun = true;
	}
}

void CPassTCPDlg::InitView()
{
	m_chkIsRecv.SetCheck(BST_CHECKED);
	m_ChkIsBroken.SetCheck(BST_CHECKED);

	m_reSendText.SetOptions(ECOOP_XOR, ECO_WANTRETURN);

	SetRichTextColor(COLOR_TEXT_BULE);

	m_pLogic = NULL;

	m_txtServerIP.SetWindowText(_T("127.0.0.1"));
	m_txtPort.SetWindowText(_T("10002"));
	m_txtThreadCount.SetWindowText(_T("1"));
	m_txtRecvTimeout.SetWindowText(_T("1000"));
	m_txtPacketTimewait.SetWindowText(_T("0"));
	m_txtSocketInterval.SetWindowText(_T("0"));
	m_txtRecvLength.SetWindowText(_T("12"));
	m_reSendText.SetWindowText(_T("01 00 00 10 08 00 00 00 46 45 45 45 45 59 45 53 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 be cd aa 8f 3c 01 00 00"));
	m_txtClientUdpPort.SetWindowText(_T("20002"));

	m_txtSendByteCount.SetWindowText(_T("0"));
	m_txtRecvByteCount.SetWindowText(_T("0"));

	m_txtSendCount.SetWindowText(_T("0"));
	m_txtMinTime.SetWindowText(_T("0"));
	m_txtMaxTime.SetWindowText(_T("0"));

	m_nRadio = 1;

	m_cbSendBuffStyle.InsertString(0, _T("二进制模式"));
	m_cbSendBuffStyle.InsertString(1, _T("文本模式"));
	m_cbSendBuffStyle.SetCurSel(0);
	m_nCurrTextStyle = 0;

	ClearResult();

	//初始化TCP链接
	WSADATA wsaData;
	int nErr = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(nErr != 0)
	{
		MessageBox(_T("本机socket库加载失败，请检查本机socket库版本"), _T("错误信息"), MB_OK);
	}
}

void CPassTCPDlg::ClearResult()
{
	m_txtSuccessConnect.SetWindowText(_T("0"));
	m_txtSuccessSend.SetWindowText(_T("0"));
	m_txtSuccessRecv.SetWindowText(_T("0"));
	m_txtCurrConnect.SetWindowText(_T("0"));
	m_txtFailConnect.SetWindowText(_T("0"));
	m_txtFailSend.SetWindowText(_T("0"));
	m_txtFailRecv.SetWindowText(_T("0"));

	m_txtSendByteCount.SetWindowText(_T("0"));
	m_txtRecvByteCount.SetWindowText(_T("0"));
}

void CPassTCPDlg::Close(bool blClose)
{
	int nCount = m_vecClientUdpSocket.size();
	for(int i = 0; i < nCount; i++)
	{
		CClientUdpSocket* pClientUdpSocket = (CClientUdpSocket* )m_vecClientUdpSocket[i];
		if(NULL != pClientUdpSocket)
		{
			pClientUdpSocket->Stop();

			DWORD dwSleep = 100;
			Sleep(dwSleep);

			delete pClientUdpSocket;
			pClientUdpSocket = NULL;
		}
	}
	m_vecClientUdpSocket.clear();

	nCount = m_vecClientTcpSocket.size();
	for(int i = 0; i < nCount; i++)
	{
		CClientTcpSocket* pClientTcpSocket = (CClientTcpSocket* )m_vecClientTcpSocket[i];
		if(NULL != pClientTcpSocket)
		{
			pClientTcpSocket->Stop();

			DWORD dwSleep = 100;
			Sleep(dwSleep);

			delete pClientTcpSocket;
			pClientTcpSocket = NULL;
		}
	}

	m_vecClientTcpSocket.clear();

	if(blClose == true && m_pLogic != NULL)
	{
		if(m_pLogic->m_nClassTye == 1)
		{
			delete (CNomalLogic* )m_pLogic;
		}
		else
		{
			delete (CWebSocketLogic* )m_pLogic;
		}
	}
}

void CPassTCPDlg::OnClose()
{
	Close(true);
	WSACleanup();
	CDialog::OnClose();
}

void CPassTCPDlg::OnBnClickedButton2()
{
	//停止压测
	int nCount = (int)m_vecClientUdpSocket.size();
	for(int i = 0; i < nCount; i++)
	{
		CClientUdpSocket* pClientUdpSocket = (CClientUdpSocket* )m_vecClientUdpSocket[i];
		if(NULL != pClientUdpSocket)
		{
			pClientUdpSocket->Stop();
		}
	}

	nCount = (int)m_vecClientTcpSocket.size();
	for(int i = 0; i < nCount; i++)
	{
		CClientTcpSocket* pClientTcpSocket = (CClientTcpSocket* )m_vecClientTcpSocket[i];
		if(NULL != pClientTcpSocket)
		{
			pClientTcpSocket->Stop();
		}
	}

	m_tmEnd = CTime::GetCurrentTime();
	m_blIsRun = false; 
}

void CPassTCPDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 1)
	{
		int nSuccessConnect = 0;
		int nSuccessSend    = 0;
		int nSuccessRecv    = 0;
		int ntCurrConnect   = 0;
		int nFailConnect    = 0;
		int nFailSend       = 0;
		int nFailRecv       = 0;
		int nCurrConnect    = 0;
		int nSendByteCount  = 0;
		int nRecvByteCount  = 0;
		int nMinTime        = 0;
		int nMaxTime        = 0;

		int nConnectType = 0;
		switch(GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO3))
		{
		case IDC_RADIO1:
			nConnectType = 0;
			break;
		case IDC_RADIO2:
			nConnectType = 1;
			break;
		case IDC_RADIO3:
			nConnectType = 0;
			break;
		}

		if(nConnectType == 0)
		{
			for(int i = 0; i < (int)m_vecClientTcpSocket.size(); i++)
			{
				CClientTcpSocket* pClientTcpSocket = (CClientTcpSocket* )m_vecClientTcpSocket[i];
				if(NULL != pClientTcpSocket)
				{
					_Socket_State_Info* pSocket_State_Info = pClientTcpSocket->GetStateInfo();
					if(NULL != pSocket_State_Info)
					{
						nSuccessConnect += pSocket_State_Info->m_nSuccessConnect;
						nSuccessSend    += pSocket_State_Info->m_nSuccessSend;
						nSuccessRecv    += pSocket_State_Info->m_nSuccessRecv;
						nFailConnect    += pSocket_State_Info->m_nFailConnect;
						nFailSend       += pSocket_State_Info->m_nFailSend;
						nFailRecv       += pSocket_State_Info->m_nFailRecv;
						nCurrConnect    += pSocket_State_Info->m_nCurrectSocket;
						nSendByteCount  += pSocket_State_Info->m_nSendByteCount;
						nRecvByteCount  += pSocket_State_Info->m_nRecvByteCount;

						if(i == 0)
						{
							nMinTime = pSocket_State_Info->m_nMinRecvTime;
							nMaxTime = pSocket_State_Info->m_nMaxRecvTime;
						}
						else
						{
							if(nMinTime > pSocket_State_Info->m_nMinRecvTime)
							{
								nMinTime = pSocket_State_Info->m_nMinRecvTime;
							}

							if(nMaxTime < pSocket_State_Info->m_nMaxRecvTime)
							{
								nMaxTime = pSocket_State_Info->m_nMaxRecvTime;
							}
						}
					}
				}
			}

			//显示相关数据
			CString strData;
			strData.Format(_T("%d"), nSuccessConnect);
			m_txtSuccessConnect.SetWindowText(strData);
			strData.Format(_T("%d"), nSuccessSend);
			m_txtSuccessSend.SetWindowText(strData);
			strData.Format(_T("%d"), nSuccessRecv);
			m_txtSuccessRecv.SetWindowText(strData);
			strData.Format(_T("%d"), nFailConnect);
			m_txtFailConnect.SetWindowText(strData);
			strData.Format(_T("%d"), nFailSend);
			m_txtFailSend.SetWindowText(strData);
			strData.Format(_T("%d"), nFailRecv);
			m_txtFailRecv.SetWindowText(strData);
			strData.Format(_T("%d"), nCurrConnect);
			m_txtCurrConnect.SetWindowText(strData);
			strData.Format(_T("%d"), nSendByteCount);
			m_txtSendByteCount.SetWindowText(strData);
			strData.Format(_T("%d"), nRecvByteCount);
			m_txtRecvByteCount.SetWindowText(strData);
			strData.Format(_T("%d"), nMinTime);
			m_txtMinTime.SetWindowText(strData);
			strData.Format(_T("%d"), nMaxTime);
			m_txtMaxTime.SetWindowText(strData);
		}
		else
		{
			for(int i = 0; i < (int)m_vecClientUdpSocket.size(); i++)
			{
				CClientUdpSocket* pClientUdpSocket = (CClientUdpSocket* )m_vecClientUdpSocket[i];
				if(NULL != pClientUdpSocket)
				{
					_Socket_State_Info* pSocket_State_Info = pClientUdpSocket->GetStateInfo();
					if(NULL != pSocket_State_Info)
					{
						nSuccessConnect += pSocket_State_Info->m_nSuccessConnect;
						nSuccessSend    += pSocket_State_Info->m_nSuccessSend;
						nSuccessRecv    += pSocket_State_Info->m_nSuccessRecv;
						nFailConnect    += pSocket_State_Info->m_nFailConnect;
						nFailSend       += pSocket_State_Info->m_nFailSend;
						nFailRecv       += pSocket_State_Info->m_nFailRecv;
						nCurrConnect    += pSocket_State_Info->m_nCurrectSocket;
					}

					if(i == 0)
					{
						nMinTime = pSocket_State_Info->m_nMinRecvTime;
						nMaxTime = pSocket_State_Info->m_nMaxRecvTime;
					}
					else
					{
						if(nMinTime > pSocket_State_Info->m_nMinRecvTime)
						{
							nMinTime = pSocket_State_Info->m_nMinRecvTime;
						}

						if(nMaxTime < pSocket_State_Info->m_nMaxRecvTime)
						{
							nMaxTime = pSocket_State_Info->m_nMaxRecvTime;
						}
					}
				}
			}

			//显示相关数据
			CString strData;
			strData.Format(_T("%d"), nSuccessConnect);
			m_txtSuccessConnect.SetWindowText(strData);
			strData.Format(_T("%d"), nSuccessSend);
			m_txtSuccessSend.SetWindowText(strData);
			strData.Format(_T("%d"), nSuccessRecv);
			m_txtSuccessRecv.SetWindowText(strData);
			strData.Format(_T("%d"), nFailConnect);
			m_txtFailConnect.SetWindowText(strData);
			strData.Format(_T("%d"), nFailSend);
			m_txtFailSend.SetWindowText(strData);
			strData.Format(_T("%d"), nFailRecv);
			m_txtFailRecv.SetWindowText(strData);
			strData.Format(_T("%d"), nCurrConnect);
			m_txtCurrConnect.SetWindowText(strData);
			strData.Format(_T("%d"), nMinTime);
			m_txtMinTime.SetWindowText(strData);
			strData.Format(_T("%d"), nMaxTime);
			m_txtMaxTime.SetWindowText(strData);
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CPassTCPDlg::OnBnClickedButton3()
{
	//写入压测报告
	FILE* pFile = NULL;
	char szFileName[20];
	sprintf_s(szFileName, "StressTest.log");
	fopen_s(&pFile, szFileName, "a+");
	if(pFile == NULL)
	{
		MessageBox(_T("导出压测报告失败，文件不存在"), _T("提示信息"), MB_OK);
		return;
	}

	char szLogText[1024] = {'\0'};

	sprintf_s(szLogText, 1024, "=============================================\n");
	fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

	//默认TCP类型，0是TCP，1是UDP
	switch(GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO3))
	{
	case IDC_RADIO1:
		sprintf_s(szLogText, 1024, "压测类型:TCP\n");
		break;
	case IDC_RADIO2:
		sprintf_s(szLogText, 1024, "压测类型:UDP\n");
		break;
	case IDC_RADIO3:
		sprintf_s(szLogText, 1024, "压测类型:WebSocket\n");
		break;
	}
	fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

	CString strData;
	CString strDataPort;
	m_txtServerIP.GetWindowText(strData);

	char szServerIP[MAX_BUFF_20] = {'\0'};
	int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
	int nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, szServerIP, MAX_BUFF_20, NULL,NULL);
	szServerIP[nDecLen] = '\0';

	m_txtPort.GetWindowText(strDataPort);
	sprintf_s(szLogText, 1024, "压测IP:%s, 压测端口:%d.\n", szServerIP, _ttoi((LPCTSTR)strDataPort));
	fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

	//CString strBeginTime = m_tmBegin.Format("%Y-%m-%d %H:%M:%S");
	sprintf_s(szLogText, 1024, "压测开始时间为: %04d-%02d-%02d %02d:%02d:%02d\n", m_tmBegin.GetYear(), m_tmBegin.GetMonth(), m_tmBegin.GetDay(), m_tmBegin.GetHour(), m_tmBegin.GetMinute(), m_tmBegin.GetSecond());
	fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

	if(m_blIsRun == true)
	{
		CTime tmNow = CTime::GetCurrentTime();

		//CString strNowTime = tmNow.Format("%Y-%m-%d %H:%M:%S");
		sprintf_s(szLogText, 1024, "压测结束时间为: %04d-%02d-%02d %02d:%02d:%02d\n", tmNow.GetYear(), tmNow.GetMonth(), tmNow.GetDay(), tmNow.GetHour(), tmNow.GetMinute(), tmNow.GetSecond());

		fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);
	}
	else
	{
		//CString strEndTime = m_tmEnd.Format("%Y-%m-%d %H:%M:%S");
		sprintf_s(szLogText, 1024, "压测结束时间为: %04d-%02d-%02d %02d:%02d:%02d\n", m_tmEnd.GetYear(), m_tmEnd.GetMonth(), m_tmEnd.GetDay(), m_tmEnd.GetHour(), m_tmEnd.GetMinute(), m_tmEnd.GetSecond());

		fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);
	}

	
	m_txtThreadCount.GetWindowText(strData);
	sprintf_s(szLogText, 1024, "压测线程数:%d\n", _ttoi((LPCTSTR)strData));
	fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

	m_txtSuccessConnect.GetWindowText(strData);
	sprintf_s(szLogText, 1024, "创建成功连接数:%d\n", _ttoi((LPCTSTR)strData));
	fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

	m_txtSuccessSend.GetWindowText(strData);
	sprintf_s(szLogText, 1024, "成功发送数据包数:%d\n", _ttoi((LPCTSTR)strData));
	fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

	m_txtSuccessRecv.GetWindowText(strData);
	sprintf_s(szLogText, 1024, "成功接收数据包数:%d\n", _ttoi((LPCTSTR)strData));
	fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

	m_txtSendByteCount.GetWindowText(strData);
	sprintf_s(szLogText, 1024, "发送字节数:%d\n", _ttoi((LPCTSTR)strData));
	fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

	m_txtRecvByteCount.GetWindowText(strData);
	sprintf_s(szLogText, 1024, "接收字节数:%d\n", _ttoi((LPCTSTR)strData));
	fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

	m_txtFailConnect.GetWindowText(strData);
	sprintf_s(szLogText, 1024, "连接失败数:%d\n", _ttoi((LPCTSTR)strData));
	fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

	m_txtFailSend.GetWindowText(strData);
	sprintf_s(szLogText, 1024, "发送失败数据包数:%d\n", _ttoi((LPCTSTR)strData));
	fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

	m_txtFailRecv.GetWindowText(strData);
	sprintf_s(szLogText, 1024, "接收失败数据包数:%d\n", _ttoi((LPCTSTR)strData));
	fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

	m_txtMinTime.GetWindowText(strData);
	sprintf_s(szLogText, 1024, "最小单包响应时间:%d毫秒\n", _ttoi((LPCTSTR)strData));
	fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

	m_txtMaxTime.GetWindowText(strData);
	sprintf_s(szLogText, 1024, "最大单包响应时间:%d毫秒\n", _ttoi((LPCTSTR)strData));
	fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

	//连接成功百分比
	float fRote = 0.0f;
	m_txtSuccessConnect.GetWindowText(strData);
	int nSucccess =  _ttoi((LPCTSTR)strData);
	m_txtFailConnect.GetWindowText(strData);
	int nFail     = _ttoi((LPCTSTR)strData);
	if(nSucccess + nFail == 0)
	{
		fRote = 0.0f;
	}
	else
	{
		fRote = (float)nSucccess/(nSucccess + nFail);
	}

	sprintf_s(szLogText, 1024, "连接成功百分比:%f%%\n", fRote*100.0);
	fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

	//发送成功百分比
	m_txtSuccessSend.GetWindowText(strData);
	nSucccess =  _ttoi((LPCTSTR)strData);
	m_txtFailSend.GetWindowText(strData);
	nFail     = _ttoi((LPCTSTR)strData);
	if(nSucccess + nFail == 0)
	{
		fRote = 0.0f;
	}
	else
	{
		fRote = (float)nSucccess/(nSucccess + nFail);
	}
	sprintf_s(szLogText, 1024, "发送数据包成功百分比:%f%%\n", fRote*100.0);
	fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

	//接收成功百分比
	m_txtSuccessRecv.GetWindowText(strData);
	nSucccess =  _ttoi((LPCTSTR)strData);
	m_txtFailRecv.GetWindowText(strData);
	nFail     = _ttoi((LPCTSTR)strData);
	if(nSucccess + nFail == 0)
	{
		fRote = 0.0f;
	}
	else
	{
		fRote = (float)nSucccess/(nSucccess + nFail);
	}
	sprintf_s(szLogText, 1024, "接收数据包成功百分比:%f%%\n", fRote*100.0);
	fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

	sprintf_s(szLogText, 1024, "=============================================\n");
	fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

	fclose(pFile);

	MessageBox(_T("导出压测报告成功"), _T("提示信息"), MB_OK);

}

void CPassTCPDlg::OnCbnSelchangeCombo1()
{
	//文本编制模式，根据用户的选项，变化文本的内容
	int nCurrTextStyle = m_cbSendBuffStyle.GetCurSel();
	if(nCurrTextStyle != m_nCurrTextStyle)
	{
		if(nCurrTextStyle == 0)  //讲文本转化为二进制
		{
			//当风格发生改变的时候变化
			CString strData;
			m_reSendText.GetWindowText(strData);

			int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
			int nBufferSize = WideCharToMultiByte(CP_ACP, 0, strData, -1, NULL, 0, NULL, NULL);  
			char* pSendData = new char[nBufferSize + 1];
			int nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, pSendData, nBufferSize, NULL,NULL);
			pSendData[nDecLen] = '\0';

			if(nDecLen <= 0)
			{
				//如果没有内容就不转换
				m_nCurrTextStyle = nCurrTextStyle;
				SetRichTextColor(COLOR_TEXT_BULE);
				return;
			}

			//转化后的字符串
			int nConvertSize = (nBufferSize + 1)*4;
			char* pErSendBuff = new char[nConvertSize];
			memset(pErSendBuff, 0, nConvertSize);

			for(int i = 0; i < nDecLen; i++)
			{
				char szLog[4] = {'\0'};
				if( i != nDecLen - 1)
				{
					sprintf_s(szLog, 4, "%02X ", (unsigned char)pSendData[i]);
				}
				else
				{
					sprintf_s(szLog, 4, "%02X", (unsigned char)pSendData[i]);
				}
				
				if(i == 0)
				{
					sprintf_s(pErSendBuff, nConvertSize, "%s%s", pErSendBuff, szLog);
				}
				else
				{
					sprintf_s(pErSendBuff, nConvertSize, "%s %s", pErSendBuff, szLog);
				}
			}

			//将转换后的二进制，显示在文本框内
			wchar_t *pwText = new wchar_t[nConvertSize];

			nSrcLen = MultiByteToWideChar (CP_ACP, 0, pErSendBuff, -1, NULL, 0);
			nBufferSize = MultiByteToWideChar (CP_ACP, 0, pErSendBuff, -1, pwText, nSrcLen);
			pwText[nBufferSize] = '\0';

			m_reSendText.SetWindowText(pwText);

			delete[] pwText;
			delete[] pSendData;
			delete[] pErSendBuff;

			SetRichTextColor(COLOR_TEXT_BULE);
		}
		else
		{
			//将二进制转化成文本
			CString strData;
			m_reSendText.GetWindowText(strData);

			int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
			int nBufferSize = WideCharToMultiByte(CP_ACP, 0, strData, -1, NULL, 0, NULL, NULL);  
			char* pSendData = new char[nBufferSize + 1];
			int nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, pSendData, nBufferSize, NULL,NULL);
			pSendData[nDecLen] = '\0';

			if(nDecLen <= 0)
			{
				//如果没有内容就不转换
				m_nCurrTextStyle = nCurrTextStyle;
				SetRichTextColor(COLOR_TEXT_RED);
				return;
			}

			int nTextSize = 0;
			if(nDecLen % 3 != 0)
			{
				nTextSize = nDecLen / 3 + 2;
			}
			else
			{
				nTextSize = nDecLen / 3 + 1;
			}

			char* pTextData = new char[nTextSize];
			memset(pTextData, 0, nTextSize);

			CConvertBuffer objConvertBuffer;
			//将数据串转换成二进制串
			objConvertBuffer.Convertstr2charArray(pSendData, strlen(pSendData), (unsigned char*)pTextData, nTextSize);

			//将转换后的二进制，显示在文本框内
			wchar_t *pwText = new wchar_t[nTextSize + 2];

			nSrcLen = MultiByteToWideChar (CP_ACP, 0, pTextData, -1, NULL, 0);
			nBufferSize = MultiByteToWideChar (CP_ACP, 0, pTextData, -1, pwText, nSrcLen);
			pwText[nBufferSize] = '\0';

			m_reSendText.SetWindowText(pwText);

			delete[] pwText;
			delete[] pSendData;
			delete[] pTextData;

			SetRichTextColor(COLOR_TEXT_RED);
		}

		m_nCurrTextStyle = nCurrTextStyle;

	}
}

void CPassTCPDlg::SetRichTextColor(int nColor)
{
	::CHARFORMAT2   cf; 
	memset(&cf, 0x00, sizeof(cf)); 
	cf.cbSize        =   sizeof(cf); 
	if(nColor == COLOR_TEXT_BULE)
	{
		cf.crTextColor   =   RGB(0, 0, 255); 
	}
	else if(nColor == COLOR_TEXT_RED)
	{
		cf.crTextColor   =   RGB(255, 0, 0); 
	}
	cf.dwMask        =   CFM_COLOR;

	m_reSendText.SetDefaultCharFormat(cf);//设置输入框内所有字符的字体
}

void CPassTCPDlg::OnBnClickedButton4()
{
	//打开Lua文件夹
	//首先判断高级选项是否已经打开
	if(m_chkLuaAdvance.GetCheck() != BST_CHECKED)
	{
		MessageBox(_T("请先选择启用高级模式开关。"), _T("提示信息"), MB_OK);
		return;
	}

	//打开当前路径
	TCHAR szBuffer[2048] = {'\0'};
	GetCurrentDirectory(2048, szBuffer);

	CFileDialog dlgFile(TRUE, _T( "*lua" ), NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("Lua文件(*.lua)|*.lua|"));
	dlgFile.m_ofn.lpstrInitialDir = szBuffer;

	if (dlgFile.DoModal() == IDOK) 
	{
		CString strLuaFile = dlgFile.GetPathName();
		m_txtLuaFilePath.SetWindowText(strLuaFile);
	}
}

void CPassTCPDlg::OnBnClickedButton5()
{
	//设置随机数据包序列
	CPacketDlg objPacketDlg;

	switch(GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO3))
	{
	case IDC_RADIO1:
		{
			if(m_pLogic != NULL)
			{
				delete m_pLogic;
			}

			CNomalLogic* pNomalLogic = new CNomalLogic();
			m_pLogic = (CBaseDataLogic* )pNomalLogic;
			break;
		}
	case IDC_RADIO2:
		{
			if(m_pLogic != NULL)
			{
				delete m_pLogic;
			}

			CNomalLogic* pNomalLogic = new CNomalLogic();
			m_pLogic = (CBaseDataLogic* )pNomalLogic;
			break;
		}
	case IDC_RADIO3:
		{
			if(m_pLogic != NULL)
			{
				delete m_pLogic;
			}

			CWebSocketLogic* pWebSocketLogic = new CWebSocketLogic();
			m_pLogic = (CBaseDataLogic* )pWebSocketLogic;
			break;
		}
	}

	objPacketDlg.SetBaseDataLogic(m_pLogic);

	objPacketDlg.DoModal();
}
