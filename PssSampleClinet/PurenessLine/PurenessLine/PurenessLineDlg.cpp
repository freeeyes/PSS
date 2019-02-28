
// PurenessLineDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PurenessLine.h"
#include "PurenessLineDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPurenessLineDlg 对话框




CPurenessLineDlg::CPurenessLineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPurenessLineDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPurenessLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CPUVIEW, m_cvCPU);
	DDX_Control(pDX, IDC_MEMORYVIEW, m_cvMemory);
	DDX_Control(pDX, IDC_EDIT1, m_txtServerIP);
	DDX_Control(pDX, IDC_EDIT2, m_txtServerPort);
}

BEGIN_MESSAGE_MAP(CPurenessLineDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CONTROL(CVN_ViewPortChanged, IDC_CPUVIEW, OnViewPortChanged)
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &CPurenessLineDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CPurenessLineDlg 消息处理程序

BOOL CPurenessLineDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	InitView();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPurenessLineDlg::InitView()
{
	m_blState = FALSE;
	AfxSocketInit();

	//初始化列表
	for(int i = 0; i < MAX_DATA_COUNT; i++)
	{
		m_dbCPU[i]    = 0.0;
		m_dbMemory[i] = 0.0;
	}

	//设置随机种子
	InitRandom();

	//开启定时器
	SetTimer(TIMER_ID, TIMER_INTERVAL, 0);

	//绘制相应的数据图表
	drawChart();
}

BOOL CPurenessLineDlg::GetServerStateInfo(_ServerRunInfo& objServerRunInfo)
{
	if(m_blState == FALSE)
	{
		return FALSE;
	}

	//拼接发送协议
	char szSendDate[100] = {'\0'};
	char szCommand[100]  = {'\0'}; 
	size_t stSendEnd = 0;

	sprintf_s(szCommand, 100, "ShowCurrProcessInfo -a");
	int nAllLen = (int)strlen(szCommand);
	memcpy(&szSendDate[0], (const void*)&nAllLen, sizeof(int));
	memcpy(&szSendDate[4], (const void*)&szCommand, nAllLen);

	//发送数据
	int nSend = m_sckServer.Send(szSendDate, nAllLen + 4);
	if(nSend != nAllLen + 4)
	{
		return FALSE;
	}

	//接受数据
	char szBuffRecv[200] = {'\0'};
	m_sckServer.Receive(szBuffRecv, 200);

	int nStrLen              = 0;
	int nPos                 = 4;
	int nClientCount         = 0;
	//包处理线程
	memcpy(&objServerRunInfo.m_nCPU, &szBuffRecv[nPos], sizeof(int));
	nPos += sizeof(int);
	memcpy(&objServerRunInfo.m_nMemorySize, &szBuffRecv[nPos], sizeof(int));
	nPos += sizeof(int);

	return TRUE;
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPurenessLineDlg::OnViewPortChanged()
{
	_ServerRunInfo objServerRunInfo;
	if(GetServerStateInfo(objServerRunInfo) == TRUE)
	{
		//设置数据
		double dbData = (double)RandomValue(1, 100);
		shiftData(m_dbCPU, MAX_DATA_COUNT, (double)objServerRunInfo.m_nCPU/100.0);

		dbData = (double)RandomValue(1, 100);
		shiftData(m_dbMemory, MAX_DATA_COUNT, (double)objServerRunInfo.m_nMemorySize/(1000*1000));

		//绘制相应的数据图表
		drawChart();
	}
	else
	{
		//设置数据
		double dbData = 0.0;
		shiftData(m_dbCPU, MAX_DATA_COUNT, (double)objServerRunInfo.m_nCPU/100.0);

		dbData = 0.0;
		shiftData(m_dbMemory, MAX_DATA_COUNT, (double)objServerRunInfo.m_nMemorySize/(1000*1000));

		//绘制相应的数据图表
		drawChart();
	}
}

void CPurenessLineDlg::OnPaint()
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
HCURSOR CPurenessLineDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPurenessLineDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnClose();
}

void CPurenessLineDlg::drawChart()
{
	const char* szLabels[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
		"11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23",
		"24"};

	XYChart* pXYCPU    = NULL;
	XYChart* pXYMemory = NULL;

	//设置默认窗体大小(CPU)
	pXYCPU = new XYChart(410, 220, 0xeeeeff, 0x000000, 1);
	pXYCPU->setRoundedFrame();

	//设置X轴下标
	pXYCPU->xAxis()->setLabels(StringArray(szLabels, (int)(sizeof(szLabels) / sizeof(szLabels[0]))));

	//设置位置
	pXYCPU->setPlotArea(30, 20, 360, 160, 0x000200, -1, -1, 0x337f59);

	//获得一个新的线层
	LineLayer* pLayer = pXYCPU->addLineLayer();

	if(NULL != pLayer)
	{
		pLayer->setLineWidth(2);
		pLayer->addDataSet(DoubleArray(m_dbCPU, (int)(sizeof(m_dbCPU) / sizeof(m_dbCPU[0]))), 0xff0000, "Server #1");
	}

	//将制定的Chart对象绑定给指定控件
	m_cvCPU.setChart(pXYCPU);
	delete pXYCPU;

	//设置默认窗体大小(Memory)
	pXYMemory = new XYChart(410, 220, 0xeeeeff, 0x000000, 1);
	pXYMemory->setRoundedFrame();

	//设置X轴下标
	pXYMemory->xAxis()->setLabels(StringArray(szLabels, (int)(sizeof(szLabels) / sizeof(szLabels[0]))));

	//设置位置
	pXYMemory->setPlotArea(30, 20, 360, 160, 0x000200, -1, -1, 0x337f59);

	//获得一个新的线层
	pLayer = pXYMemory->addLineLayer();

	if(NULL != pLayer)
	{
		pLayer->setLineWidth(2);
		pLayer->addDataSet(DoubleArray(m_dbMemory, (int)(sizeof(m_dbMemory) / sizeof(m_dbMemory[0]))), 0xff0000, "Server #1");
	}

	//将制定的Chart对象绑定给指定控件
	m_cvMemory.setChart(pXYMemory);
	delete pXYMemory;
}

void CPurenessLineDlg::shiftData(double *data, int len, double newValue)
{
	memmove(data, data + 1, sizeof(*data) * (len - 1));
	data[len - 1] = newValue;
}

void CPurenessLineDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == TIMER_ID);
	{
		m_cvCPU.updateViewPort(true, true);
	}

	CDialog::OnTimer(nIDEvent);
}

void CPurenessLineDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CString strServerIP;
	CString strServerPort;

	int nPort = 0;

	m_txtServerIP.GetWindowText(strServerIP);
	m_txtServerPort.GetWindowText(strServerPort);

	if(strServerIP.GetLength() == 0 || strServerPort.GetLength() == 0)
	{
		MessageBox(_T("请输入完整的服务器IP和Port。"), _T("链接信息"), MB_OK);
		return;
	}

	//解析端口地址
	nPort = _wtoi((LPCTSTR)strServerPort);

	m_sckServer.Close();

	BOOL blFlag = m_sckServer.Create();
	if(FALSE == blFlag)
	{
		MessageBox(_T("初始化Socket失败。"), _T("链接信息"), MB_OK);
		return;
	}

	blFlag = m_sckServer.Connect((LPCTSTR)strServerIP, nPort);
	if(FALSE == blFlag)
	{
		wchar_t szError[100] = {'\0'};
		wsprintf(szError, _T("链接[%s]，端口[%d]失败，error[%d]。"), (LPCTSTR)strServerIP, nPort, GetLastError());
		MessageBox((LPCTSTR)szError, _T("链接信息"), MB_OK);
		return;
	}

	m_blState = TRUE;
}
