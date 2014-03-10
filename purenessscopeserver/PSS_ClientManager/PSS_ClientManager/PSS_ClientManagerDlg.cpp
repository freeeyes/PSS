
// PSS_ClientManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PSS_ClientManager.h"
#include "PSS_ClientManagerDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CPSS_ClientManagerDlg dialog




CPSS_ClientManagerDlg::CPSS_ClientManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPSS_ClientManagerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPSS_ClientManagerDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_TAB1, m_tabClientMain);
}

BEGIN_MESSAGE_MAP(CPSS_ClientManagerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
  ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CPSS_ClientManagerDlg::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CPSS_ClientManagerDlg message handlers

BOOL CPSS_ClientManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

  InitView();
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPSS_ClientManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPSS_ClientManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPSS_ClientManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPSS_ClientManagerDlg::InitView()
{
  m_tabClientMain.InsertItem(m_tabClientMain.GetItemCount(), m_DlgClientMain.GetPageTitle());
  m_tabClientMain.InsertItem(m_tabClientMain.GetItemCount(), m_DlgClientConnect.GetPageTitle());
  m_tabClientMain.InsertItem(m_tabClientMain.GetItemCount(), m_DlgClientModule.GetPageTitle());
  m_tabClientMain.InsertItem(m_tabClientMain.GetItemCount(), m_DlgClientConnectM.GetPageTitle());
  m_tabClientMain.InsertItem(m_tabClientMain.GetItemCount(), m_DlgClientWorkThread.GetPageTitle());
  m_tabClientMain.InsertItem(m_tabClientMain.GetItemCount(), m_DlgServerConnect.GetPageTitle());
  m_tabClientMain.InsertItem(m_tabClientMain.GetItemCount(), m_DlgForbidenIP.GetPageTitle());
  m_tabClientMain.InsertItem(m_tabClientMain.GetItemCount(), m_DlgTrackIP.GetPageTitle());
  m_tabClientMain.InsertItem(m_tabClientMain.GetItemCount(), m_DlgWorkThreadAI.GetPageTitle());

  m_DlgClientMain.SetTcpClientConnect(&m_TcpClientConnect);
  m_DlgClientConnect.SetTcpClientConnect(&m_TcpClientConnect);
  m_DlgClientModule.SetTcpClientConnect(&m_TcpClientConnect);
  m_DlgClientConnectM.SetTcpClientConnect(&m_TcpClientConnect);
  m_DlgServerConnect.SetTcpClientConnect(&m_TcpClientConnect);
  m_DlgClientWorkThread.SetTcpClientConnect(&m_TcpClientConnect);
  m_DlgForbidenIP.SetTcpClientConnect(&m_TcpClientConnect);
  m_DlgTrackIP.SetTcpClientConnect(&m_TcpClientConnect);
  m_DlgWorkThreadAI.SetTcpClientConnect(&m_TcpClientConnect);

  m_DlgClientMain.Create(IDD_DIALOG_MAIN, this);
  m_DlgClientConnect.Create(IDD_DIALOG_CLIENTCONNECT, this);
  m_DlgClientModule.Create(IDD_DIALOG_MODULE, this);
  m_DlgClientConnectM.Create(IDD_DIALOG_CLIENTCONNECTM, this);
  m_DlgClientWorkThread.Create(IDD_DIALOG_WORKTHREAD, this);
  m_DlgServerConnect.Create(IDD_DIALOG_SERVERCONNECT, this);
  m_DlgForbidenIP.Create(IDD_DIALOG_FORBIDENIP, this);
  m_DlgTrackIP.Create(IDD_DIALOG_TRACKIP, this);
  m_DlgWorkThreadAI.Create(IDD_DIALOG_WORKTHREADAI, this);

  CRect rtClient;
  GetClientRect(&rtClient);

  CRect rtTab;
  m_tabClientMain.GetClientRect(&rtTab);
  m_tabClientMain.MapWindowPoints(this, &rtTab);
  m_tabClientMain.AdjustRect(FALSE, &rtTab);

  m_DlgClientMain.MoveWindow(&rtTab);
  m_DlgClientConnect.MoveWindow(&rtTab);
  m_DlgClientModule.MoveWindow(&rtTab);
  m_DlgClientConnectM.MoveWindow(&rtTab);
  m_DlgClientWorkThread.MoveWindow(&rtTab);
  m_DlgServerConnect.MoveWindow(&rtTab);
  m_DlgForbidenIP.MoveWindow(&rtTab);
  m_DlgTrackIP.MoveWindow(&rtTab);
  m_DlgWorkThreadAI.MoveWindow(&rtTab);

  m_tabClientMain.SetCurSel(0);

  m_DlgClientMain.ShowWindow(SW_SHOW);

  //初始化TCP链接
  WSADATA wsaData;
  int nErr = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if(nErr != 0)
  {
    MessageBox(_T("本机socket库加载失败，请检查本机socket库版本"), _T("错误信息"), MB_OK);
  }
}


void CPSS_ClientManagerDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
  // TODO: Add your control notification handler code here
  int nIndex = m_tabClientMain.GetCurSel();
  switch (nIndex)
  {
  case 0:
    m_DlgClientMain.ShowWindow(SW_SHOW);
    m_DlgClientConnect.ShowWindow(SW_HIDE);
    m_DlgClientModule.ShowWindow(SW_HIDE);
    m_DlgClientConnectM.ShowWindow(SW_HIDE);
    m_DlgClientWorkThread.ShowWindow(SW_HIDE);
    m_DlgServerConnect.ShowWindow(SW_HIDE);
    m_DlgForbidenIP.ShowWindow(SW_HIDE);
	m_DlgTrackIP.ShowWindow(SW_HIDE);
	m_DlgWorkThreadAI.ShowWindow(SW_HIDE);
    break;
  case 1:
    m_DlgClientMain.ShowWindow(SW_HIDE);
    m_DlgClientConnect.ShowWindow(SW_SHOW);
    m_DlgClientModule.ShowWindow(SW_HIDE);
    m_DlgClientConnectM.ShowWindow(SW_HIDE);
    m_DlgClientWorkThread.ShowWindow(SW_HIDE);
    m_DlgServerConnect.ShowWindow(SW_HIDE);
    m_DlgForbidenIP.ShowWindow(SW_HIDE);
	m_DlgTrackIP.ShowWindow(SW_HIDE);
	m_DlgWorkThreadAI.ShowWindow(SW_HIDE);
    break;
  case 2:
    m_DlgClientMain.ShowWindow(SW_HIDE);
    m_DlgClientConnect.ShowWindow(SW_HIDE);
    m_DlgClientModule.ShowWindow(SW_SHOW);
    m_DlgClientConnectM.ShowWindow(SW_HIDE);
    m_DlgClientWorkThread.ShowWindow(SW_HIDE);
    m_DlgServerConnect.ShowWindow(SW_HIDE);
    m_DlgForbidenIP.ShowWindow(SW_HIDE);
	m_DlgTrackIP.ShowWindow(SW_HIDE);
	m_DlgWorkThreadAI.ShowWindow(SW_HIDE);
    break;
  case 3:
    m_DlgClientMain.ShowWindow(SW_HIDE);
    m_DlgClientConnect.ShowWindow(SW_HIDE);
    m_DlgClientModule.ShowWindow(SW_HIDE);
    m_DlgClientConnectM.ShowWindow(SW_SHOW);
    m_DlgClientWorkThread.ShowWindow(SW_HIDE);
    m_DlgServerConnect.ShowWindow(SW_HIDE);
    m_DlgForbidenIP.ShowWindow(SW_HIDE);
	m_DlgTrackIP.ShowWindow(SW_HIDE);
	m_DlgWorkThreadAI.ShowWindow(SW_HIDE);
    break;
  case 4:
    m_DlgClientMain.ShowWindow(SW_HIDE);
    m_DlgClientConnect.ShowWindow(SW_HIDE);
    m_DlgClientModule.ShowWindow(SW_HIDE);
    m_DlgClientConnectM.ShowWindow(SW_HIDE);
    m_DlgClientWorkThread.ShowWindow(SW_SHOW);
    m_DlgServerConnect.ShowWindow(SW_HIDE);
    m_DlgForbidenIP.ShowWindow(SW_HIDE);
	m_DlgTrackIP.ShowWindow(SW_HIDE);
	m_DlgWorkThreadAI.ShowWindow(SW_HIDE);
    break;
  case 5:
    m_DlgClientMain.ShowWindow(SW_HIDE);
    m_DlgClientConnect.ShowWindow(SW_HIDE);
    m_DlgClientModule.ShowWindow(SW_HIDE);
    m_DlgClientConnectM.ShowWindow(SW_HIDE);
    m_DlgClientWorkThread.ShowWindow(SW_HIDE);
    m_DlgServerConnect.ShowWindow(SW_SHOW);
    m_DlgForbidenIP.ShowWindow(SW_HIDE);
	m_DlgTrackIP.ShowWindow(SW_HIDE);
	m_DlgWorkThreadAI.ShowWindow(SW_HIDE);
    break;
  case 6:
    m_DlgClientMain.ShowWindow(SW_HIDE);
    m_DlgClientConnect.ShowWindow(SW_HIDE);
    m_DlgClientModule.ShowWindow(SW_HIDE);
    m_DlgClientConnectM.ShowWindow(SW_HIDE);
    m_DlgClientWorkThread.ShowWindow(SW_HIDE);
    m_DlgServerConnect.ShowWindow(SW_HIDE);
    m_DlgForbidenIP.ShowWindow(SW_SHOW);
	m_DlgTrackIP.ShowWindow(SW_HIDE);
	m_DlgWorkThreadAI.ShowWindow(SW_HIDE);
    break;
  case 7:
	  m_DlgClientMain.ShowWindow(SW_HIDE);
	  m_DlgClientConnect.ShowWindow(SW_HIDE);
	  m_DlgClientModule.ShowWindow(SW_HIDE);
	  m_DlgClientConnectM.ShowWindow(SW_HIDE);
	  m_DlgClientWorkThread.ShowWindow(SW_HIDE);
	  m_DlgServerConnect.ShowWindow(SW_HIDE);
	  m_DlgForbidenIP.ShowWindow(SW_HIDE);
	  m_DlgTrackIP.ShowWindow(SW_SHOW);
	  m_DlgWorkThreadAI.ShowWindow(SW_HIDE);
	  break;
  case 8:
	  m_DlgClientMain.ShowWindow(SW_HIDE);
	  m_DlgClientConnect.ShowWindow(SW_HIDE);
	  m_DlgClientModule.ShowWindow(SW_HIDE);
	  m_DlgClientConnectM.ShowWindow(SW_HIDE);
	  m_DlgClientWorkThread.ShowWindow(SW_HIDE);
	  m_DlgServerConnect.ShowWindow(SW_HIDE);
	  m_DlgForbidenIP.ShowWindow(SW_HIDE);
	  m_DlgTrackIP.ShowWindow(SW_HIDE);
	  m_DlgWorkThreadAI.ShowWindow(SW_SHOW);
	  break;
  default:
    break;
  }

  *pResult = 0;
}
