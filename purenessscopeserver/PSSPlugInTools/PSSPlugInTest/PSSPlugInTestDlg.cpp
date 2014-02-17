
// PSSPlugInTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PSSPlugInTest.h"
#include "PSSPlugInTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

//添加静态回调函数，用于回调列表排序
int CALLBACK CompareFunc(LPARAM lparam1, LPARAM lparam2, LPARAM lparamSort)
{

  CListCtrl* pListCtrl = (CListCtrl*) lparamSort;
  CString    strItem1 = pListCtrl->GetItemText(lparam1, g_nCommandColNumber);
  CString    strItem2 = pListCtrl->GetItemText(lparam2, g_nCommandColNumber);

  return wcscmp(strItem2.GetBuffer(), strItem1.GetBuffer());
}


//线程执行
DWORD WINAPI ThreadProc(LPVOID argv)
{
  CPlugInOperation* pPlugInOperation = (CPlugInOperation* )argv;
  pPlugInOperation->Run();

  return 0;
}

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


// CPSSPlugInTestDlg dialog




CPSSPlugInTestDlg::CPSSPlugInTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPSSPlugInTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPSSPlugInTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_txtPlugInName);
	DDX_Control(pDX, IDC_EDIT2, m_txtSendCount);
	DDX_Control(pDX, IDC_EDIT3, m_txtRecvCount);
	DDX_Control(pDX, IDC_EDIT4, m_txtThreadCount);
	DDX_Control(pDX, IDC_LIST3, m_lstCommand);
	DDX_Control(pDX, IDC_LIST2, m_lstPlugInInfo);
	DDX_Control(pDX, IDC_EDIT5, m_txtXMLFileName);
	DDX_Control(pDX, IDC_EDIT6, m_txtHelp);
}

BEGIN_MESSAGE_MAP(CPSSPlugInTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
  ON_BN_CLICKED(IDC_BUTTON1, &CPSSPlugInTestDlg::OnBnClickedButton1)
  ON_BN_CLICKED(IDC_BUTTON5, &CPSSPlugInTestDlg::OnBnClickedButton5)
  ON_BN_CLICKED(IDC_BUTTON4, &CPSSPlugInTestDlg::OnBnClickedButton4)
  ON_WM_CLOSE()
  ON_BN_CLICKED(IDC_BUTTON2, &CPSSPlugInTestDlg::OnBnClickedButton2)
  ON_BN_CLICKED(IDC_BUTTON3, &CPSSPlugInTestDlg::OnBnClickedButton3)
  ON_WM_TIMER()
  ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST3, &CPSSPlugInTestDlg::OnLvnColumnclickList3)
END_MESSAGE_MAP()


// CPSSPlugInTestDlg message handlers

BOOL CPSSPlugInTestDlg::OnInitDialog()
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

	// TODO: Add extra initialization here
    InitView();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPSSPlugInTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPSSPlugInTestDlg::OnPaint()
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
HCURSOR CPSSPlugInTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPSSPlugInTestDlg::OnBnClickedButton1()
{
  CString strData;

  char szPlugInFileName[MAX_BUFF_1024]    = {'\0'};
  m_txtPlugInName.GetWindowText(strData);
  int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
  int nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, szPlugInFileName, MAX_BUFF_1024, NULL,NULL);
  szPlugInFileName[nDecLen] = '\0';

  if(strlen(szPlugInFileName) == 0)
  {
    MessageBox(_T("请输入你的插件名称"), _T("提示信息"), MB_OK);
    return;
  }


  ClearPlugInOperation();
 
  //加载相应的初始化dll和数据
  int nCount = m_objCommandInfo.GetCommandGroupCount();
  for(int i = 0; i < nCount; i++)
  {
    vecCommaindInfo* pvecCommaindInfo = m_objCommandInfo.GetCommandGroup(i);
    if(NULL != pvecCommaindInfo)
    {
      CPlugInOperation* pPlugInOperation = new CPlugInOperation();
      if(pPlugInOperation->Init() == false)
      {
        delete pPlugInOperation;
        ClearPlugInOperation();
        MessageBox(_T("插件加载错误，DLL接口对接失败，请检查你的DLL。"), _T("提示信息"), MB_OK);
        return;
      }
      pPlugInOperation->Start(pvecCommaindInfo, szPlugInFileName);
      m_objvecPlugInOperationGroup.push_back(pPlugInOperation);
    }
  }

  MessageBox(_T("插件加载成功。"), _T("提示信息"), MB_OK);
  return;
}

void CPSSPlugInTestDlg::OnBnClickedButton5()
{
  char szFileName[MAX_BUFF_100] = {'\0'};

  //获得XML的文件名
  CString strData;
  m_txtXMLFileName.GetWindowText(strData);
  int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
  int nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, szFileName, MAX_BUFF_100, NULL,NULL);
  szFileName[nDecLen] = '\0';

  if(strlen(szFileName) == 0)
  {
    MessageBox(_T("请输入测试用例文件名。"), _T("提示信息"), MB_OK);
    return;
  }

  bool blFlag = m_objXmlOpeation.Init(szFileName);
  if(blFlag == false)
  {
    MessageBox(_T("测试用例文件非法，请确认文件合法性。"), _T("提示信息"), MB_OK);
    return;
  }

  char* pData = NULL;
  wchar_t szTemp[MAX_BUFF_2048] = {'\0'};
  pData = m_objXmlOpeation.GetData("Plugin", "PluginFileName");
  if(NULL != pData)
  {
    int nSrcLen = MultiByteToWideChar(CP_ACP, 0, pData, strlen(pData), NULL, 0);
    int nDecLen = MultiByteToWideChar(CP_ACP, 0, pData, nSrcLen, szTemp, MAX_BUFF_2048);
    szTemp[nDecLen] = '\0';
    m_txtPlugInName.SetWindowText((LPCTSTR)szTemp);
  }

  pData = m_objXmlOpeation.GetData("PluginTest", "SendCount");
  if(NULL != pData)
  {
    int nSrcLen = MultiByteToWideChar(CP_ACP, 0, pData, strlen(pData), NULL, 0);
    int nDecLen = MultiByteToWideChar(CP_ACP, 0, pData, nSrcLen, szTemp, MAX_BUFF_2048);
    szTemp[nDecLen] = '\0';
    m_txtSendCount.SetWindowText((LPCTSTR)szTemp);
  }

  pData = m_objXmlOpeation.GetData("PluginTest", "RecvCount");
  if(NULL != pData)
  {
    int nSrcLen = MultiByteToWideChar(CP_ACP, 0, pData, strlen(pData), NULL, 0);
    int nDecLen = MultiByteToWideChar(CP_ACP, 0, pData, nSrcLen, szTemp, MAX_BUFF_2048);
    szTemp[nDecLen] = '\0';
    m_txtRecvCount.SetWindowText((LPCTSTR)szTemp);
  }

  pData = m_objXmlOpeation.GetData("PluginTest", "ThreadCount");
  if(NULL != pData)
  {
    int nSrcLen = MultiByteToWideChar(CP_ACP, 0, pData, strlen(pData), NULL, 0);
    int nDecLen = MultiByteToWideChar(CP_ACP, 0, pData, nSrcLen, szTemp, MAX_BUFF_2048);
    szTemp[nDecLen] = '\0';
    m_txtThreadCount.SetWindowText((LPCTSTR)szTemp);
  }

  m_lstCommand.DeleteAllItems();
  
  TiXmlElement* pNextTiXmlElementID    = NULL;
  TiXmlElement* pNextTiXmlElementGroup = NULL;
  TiXmlElement* pNextTiXmlElementSort  = NULL;
  TiXmlElement* pNextTiXmlElementText  = NULL;

  int i = 0;
  while(true)
  {
    wchar_t szCommandID[MAX_BUFF_100]    = {'\0'};
    wchar_t szCommandGroup[MAX_BUFF_100] = {'\0'};
    wchar_t szCommandSort[MAX_BUFF_100]  = {'\0'};
    wchar_t szCommandText[MAX_BUFF_1024] = {'\0'};

    char szzCommandID[MAX_BUFF_100]    = {'\0'};
    char szzCommandGroup[MAX_BUFF_100] = {'\0'};
    char szzCommandSort[MAX_BUFF_100]  = {'\0'};
    char szzCommandText[MAX_BUFF_1024] = {'\0'};

    pData = m_objXmlOpeation.GetData("Command", "CommandID", pNextTiXmlElementID);
    if(pData != NULL)
    {
      memcpy_s(szzCommandID, MAX_BUFF_100, pData, strlen(pData));
      int nSrcLen = MultiByteToWideChar(CP_ACP, 0, pData, strlen(pData), NULL, 0);
      int nDecLen = MultiByteToWideChar(CP_ACP, 0, pData, nSrcLen, szCommandID, MAX_BUFF_100);
      szCommandID[nDecLen] = '\0';
    }
    else
    {
      break;
    }

    pData = m_objXmlOpeation.GetData("Command", "CommandGroup", pNextTiXmlElementGroup);
    if(pData != NULL)
    {
      memcpy_s(szzCommandGroup, MAX_BUFF_100, pData, strlen(pData));
      int nSrcLen = MultiByteToWideChar(CP_ACP, 0, pData, strlen(pData), NULL, 0);
      int nDecLen = MultiByteToWideChar(CP_ACP, 0, pData, nSrcLen, szCommandGroup, MAX_BUFF_100);
      szCommandGroup[nDecLen] = '\0';
    }
    else
    {
      break;
    }

    pData = m_objXmlOpeation.GetData("Command", "CommandSort", pNextTiXmlElementSort);
    if(pData != NULL)
    {
      memcpy_s(szzCommandSort, MAX_BUFF_100, pData, strlen(pData));
      int nSrcLen = MultiByteToWideChar(CP_ACP, 0, pData, strlen(pData), NULL, 0);
      int nDecLen = MultiByteToWideChar(CP_ACP, 0, pData, nSrcLen, szCommandSort, MAX_BUFF_100);
      szCommandSort[nDecLen] = '\0';
    }
    else
    {
      break;
    }

    pData = m_objXmlOpeation.GetData("Command", "CommandText", pNextTiXmlElementText);
    if(pData != NULL)
    {
      memcpy_s(szzCommandText, MAX_BUFF_100, pData, strlen(pData));
      int nSrcLen = MultiByteToWideChar(CP_ACP, 0, pData, strlen(pData), NULL, 0);
      int nDecLen = MultiByteToWideChar(CP_ACP, 0, pData, nSrcLen, szCommandText, MAX_BUFF_1024);
      szCommandText[nDecLen] = '\0';
    }
    else
    {
      break;
    }

    m_objCommandInfo.SetCommandData(szzCommandID, atoi(szzCommandGroup), atoi(szzCommandSort), szzCommandText);

    m_lstCommand.InsertItem(i, szCommandID);
    m_lstCommand.SetItemText(i, 1, szCommandGroup);
    m_lstCommand.SetItemText(i, 2, szCommandSort);
    m_lstCommand.SetItemText(i, 3, szCommandText);
    i++;
  }

  //最后排序所有的命令
  m_objCommandInfo.Sort();
}

bool CPSSPlugInTestDlg::InitView()
{
  m_txtSendCount.SetWindowText(_T("1"));
  m_txtRecvCount.SetWindowText(_T("1"));
  m_txtThreadCount.SetWindowText(_T("1"));
  m_txtXMLFileName.SetWindowText(_T("TestInstance.xml"));

  m_lstCommand.InsertColumn(0, _T("CommandID"), LVCFMT_CENTER, 100);
  m_lstCommand.InsertColumn(1, _T("组别"), LVCFMT_CENTER, 50);
  m_lstCommand.InsertColumn(2, _T("顺序"), LVCFMT_CENTER, 50);
  m_lstCommand.InsertColumn(3, _T("测试数据"), LVCFMT_CENTER, 300);

  m_lstPlugInInfo.InsertColumn(0, _T("CommandID"), LVCFMT_CENTER, 100);
  m_lstPlugInInfo.InsertColumn(1, _T("线程"), LVCFMT_CENTER, 50);
  m_lstPlugInInfo.InsertColumn(2, _T("当前发送"), LVCFMT_CENTER, 100);
  m_lstPlugInInfo.InsertColumn(3, _T("当前处理"), LVCFMT_CENTER, 100);
  m_lstPlugInInfo.InsertColumn(4, _T("每秒处理"), LVCFMT_CENTER, 100);

  m_txtHelp.SetWindowText(_T("注：测试用例文件请打开当前目录的TestInstance.xml文件。并详细阅读TestInstance.xml的说明文件。"));

  g_nCommandColNumber = 0;

  return true;
}

void CPSSPlugInTestDlg::OnBnClickedButton4()
{
  //写入XML
  char szFileName[MAX_BUFF_100] = {'\0'};

  //获得XML的文件名
  CString strData;
  m_txtXMLFileName.GetWindowText(strData);
  int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
  int nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, szFileName, MAX_BUFF_100, NULL,NULL);
  szFileName[nDecLen] = '\0';

  if(strlen(szFileName) == 0)
  {
    MessageBox(_T("请输入测试用例文件名。"), _T("提示信息"), MB_OK);
    return;
  }

  FILE* pFile = NULL;
  errno_t nErr = fopen_s(&pFile, szFileName, "wb+");
  if(0 != nErr)
  {
    MessageBox(_T("创建测试用例文件错误。"), _T("提示信息"), MB_OK);
    return;
  }

  char szTemp[MAX_BUFF_2048] = {'\0'};
  sprintf_s(szTemp, MAX_BUFF_2048, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<config>\r\n");

  size_t stSize = fwrite(szTemp, sizeof(char), strlen(szTemp), pFile);
  if(stSize !=  strlen(szTemp))
  {
    MessageBox(_T("存储测试用例文件错误。"), _T("提示信息"), MB_OK);
    fclose(pFile);
    return;
  }

  //存储插件名
  char szPlugInFileName[MAX_BUFF_1024]    = {'\0'};
  m_txtPlugInName.GetWindowText(strData);
  nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
  nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, szPlugInFileName, MAX_BUFF_1024, NULL,NULL);
  szPlugInFileName[nDecLen] = '\0';

  sprintf_s(szTemp, MAX_BUFF_2048, "<Plugin PluginFileName=\"%s\" />\r\n", szFileName);

  stSize = fwrite(szTemp, sizeof(char), strlen(szTemp), pFile);
  if(stSize !=  strlen(szTemp))
  {
    MessageBox(_T("存储测试用例文件错误。"), _T("提示信息"), MB_OK);
    fclose(pFile);
    return;
  }

  //存储插件信息
  char szSendCount[MAX_BUFF_100]    = {'\0'};
  char szRecvCount[MAX_BUFF_100]    = {'\0'};
  char szThreadCount[MAX_BUFF_100]  = {'\0'};

  m_txtSendCount.GetWindowText(strData);
  nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
  nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, szSendCount, MAX_BUFF_100, NULL,NULL);
  szSendCount[nDecLen] = '\0';

  m_txtRecvCount.GetWindowText(strData);
  nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
  nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, szRecvCount, MAX_BUFF_100, NULL,NULL);
  szRecvCount[nDecLen] = '\0';

  //获得线程数，根据用户命令分组来
  strData.Format(_T("%d"), m_objCommandInfo.GetCommandGroupCount());
  nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
  nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, szThreadCount, MAX_BUFF_100, NULL,NULL);
  szThreadCount[nDecLen] = '\0';

  sprintf_s(szTemp, MAX_BUFF_2048, "<PluginTest SendCount=\"%s\" RecvCount=\"%s\" ThreadCount=\"%s\" />\r\n", szSendCount, szRecvCount, szThreadCount);

  stSize = fwrite(szTemp, sizeof(char), strlen(szTemp), pFile);
  if(stSize !=  strlen(szTemp))
  {
    MessageBox(_T("存储测试用例文件错误。"), _T("提示信息"), MB_OK);
    fclose(pFile);
    return;
  }

  //存储命令信息
  for(int i = 0; i < m_lstCommand.GetItemCount(); i++)
  {
    char szCommandID[MAX_BUFF_100]    = {'\0'};
    char szCommandGroup[MAX_BUFF_100] = {'\0'};
    char szCommandSort[MAX_BUFF_100]  = {'\0'};
    char szCommandText[MAX_BUFF_1024] = {'\0'};
    

    strData = m_lstCommand.GetItemText(i, 0);
    nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
    nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, szCommandID, MAX_BUFF_100, NULL,NULL);
    szCommandID[nDecLen] = '\0';

    strData = m_lstCommand.GetItemText(i, 1);
    nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
    nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, szCommandGroup, MAX_BUFF_100, NULL,NULL);
    szCommandGroup[nDecLen] = '\0';

    strData = m_lstCommand.GetItemText(i, 2);
    nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
    nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, szCommandSort, MAX_BUFF_100, NULL,NULL);
    szCommandSort[nDecLen] = '\0';

    strData = m_lstCommand.GetItemText(i, 3);
    nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
    nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, szCommandText, MAX_BUFF_1024, NULL,NULL);
    szCommandText[nDecLen] = '\0';

    sprintf_s(szTemp, MAX_BUFF_2048, "<Command CommandID=\"%s\" CommandGroup==\"%s\" CommandSort=\"%s\" CommandText=\"%s\" />\r\n", szCommandID, szCommandGroup, szCommandSort, szCommandText);

    stSize = fwrite(szTemp, sizeof(char), strlen(szTemp), pFile);
    if(stSize !=  strlen(szTemp))
    {
      MessageBox(_T("存储测试用例文件错误。"), _T("提示信息"), MB_OK);
      fclose(pFile);
      return;
    }
  }

  //保存XML末尾
  sprintf_s(szTemp, MAX_BUFF_100, "</config>\r\n");

  stSize = fwrite(szTemp, sizeof(char), strlen(szTemp), pFile);
  if(stSize !=  strlen(szTemp))
  {
    MessageBox(_T("存储测试用例文件错误。"), _T("提示信息"), MB_OK);
    fclose(pFile);
    return;
  }

  fclose(pFile);

  MessageBox(_T("保存测试用例成功。"), _T("提示信息"), MB_OK);
}

void CPSSPlugInTestDlg::OnClose()
{
  // TODO: Add your message handler code here and/or call default
  ClearPlugInOperation();

  ACE::fini();

  CDialog::OnClose();
}

void CPSSPlugInTestDlg::ClearPlugInOperation()
{
  for(int i = 0; i < (int)m_objvecPlugInOperationGroup.size(); i++)
  {
    CPlugInOperation* pPlugInOperation = (CPlugInOperation* )m_objvecPlugInOperationGroup[i];
    if(NULL != pPlugInOperation)
    {
      delete pPlugInOperation;
      pPlugInOperation = NULL;
    }
  }
  m_objvecPlugInOperationGroup.clear();
}

void CPSSPlugInTestDlg::OnBnClickedButton2()
{
  SetTimer(1, 1000, NULL);
  //开始测试
  for(int i = 0; i < (int)m_objvecPlugInOperationGroup.size(); i++)
  {
    DWORD  ThreadID = 0;
    CreateThread(NULL, NULL, ThreadProc, (LPVOID)m_objvecPlugInOperationGroup[i], NULL, &ThreadID);
  }
}

void CPSSPlugInTestDlg::OnBnClickedButton3()
{
  KillTimer(1);
  // TODO: Add your control notification handler code here
  //开始测试
  for(int i = 0; i < (int)m_objvecPlugInOperationGroup.size(); i++)
  {
    m_objvecPlugInOperationGroup[i]->Stop();
  }
}

void CPSSPlugInTestDlg::OnTimer(UINT_PTR nIDEvent)
{
  wchar_t szName[MAX_BUFF_100] = {'\0'};
  wchar_t szData[MAX_BUFF_100] = {'\0'};

  if(nIDEvent == 1)
  {
    m_lstPlugInInfo.DeleteAllItems();
    int nRow = 0;
    for(int i = 0; i < (int)m_objvecPlugInOperationGroup.size(); i++)
    {
      vecCommaindInfo* pvecCommaindInfo = m_objvecPlugInOperationGroup[i]->GetvecCommaindInfo();
      if(NULL != pvecCommaindInfo)
      {
        for(int j = 0; j < (int)pvecCommaindInfo->size(); j++)
        {
          _CommaindInfo* pCommaindInfo = (*pvecCommaindInfo)[j];
          int nSrcLen = MultiByteToWideChar(CP_ACP, 0, pCommaindInfo->m_szCommandID, strlen(pCommaindInfo->m_szCommandID), NULL, 0);
          int nDecLen = MultiByteToWideChar(CP_ACP, 0, pCommaindInfo->m_szCommandID, nSrcLen, szName, MAX_BUFF_100);
          szName[nDecLen] = '\0';

          wchar_t szCurrName[MAX_BUFF_100] = {'\0'};
          wchar_t szCurrData[MAX_BUFF_100] = {'\0'};
          CString strUserData;
          bool blIsFind = false;
		  /*
          for(int h = 0; h < m_lstPlugInInfo.GetItemCount(); h++)
          {
            
            strUserData = m_lstPlugInInfo.GetItemText(h, 0);
            wsprintf(szCurrName, _T("%s"), strUserData.GetBuffer());
            if(wcscmp(szCurrName, szName) == 0)
            {
              //获得之前的数字
              strUserData = m_lstPlugInInfo.GetItemText(h, 3);
              wsprintf(szCurrData, _T("%s"), strUserData.GetBuffer());
              int nCurrRecvCount = _ttoi(szCurrData);
              int nSecondRecvCount = pCommaindInfo->m_nRecvDataCount - nCurrRecvCount;
              wsprintf(szData, _T("%d"), pCommaindInfo->m_nSendDataCount);
              m_lstPlugInInfo.SetItemText(nRow, 2, szData);
              wsprintf(szData, _T("%d"), pCommaindInfo->m_nRecvDataCount);
              m_lstPlugInInfo.SetItemText(nRow, 3, szData);
              wsprintf(szData, _T("%d"), nSecondRecvCount);
              m_lstPlugInInfo.SetItemText(h, 4, szData);
              blIsFind = true;
              break;
            }
          }
		  */

          //如果没有找到则添加
          if(blIsFind == false)
          {
            nRow = m_lstPlugInInfo.GetItemCount();
            m_lstPlugInInfo.InsertItem(nRow, szName);
            wsprintf(szData, _T("%d"), i);
            m_lstPlugInInfo.SetItemText(nRow, 1, szData);
            wsprintf(szData, _T("%d"), pCommaindInfo->m_nSendDataCount);
            m_lstPlugInInfo.SetItemText(nRow, 2, szData);
            wsprintf(szData, _T("%d"), pCommaindInfo->m_nRecvDataCount);
            m_lstPlugInInfo.SetItemText(nRow, 3, szData);
			int nSecondRecvCount = pCommaindInfo->m_nRecvDataCount - pCommaindInfo->GetBeforDataCount();
            wsprintf(szData, _T("%d"), nSecondRecvCount);
            m_lstPlugInInfo.SetItemText(nRow, 4, szData);
			pCommaindInfo->SetDataCount();
          }
        }
      }
	  nRow++;
    }
  }

  CDialog::OnTimer(nIDEvent);
}

void CPSSPlugInTestDlg::OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  
  g_nCommandColNumber = pNMLV->iSubItem;
  //列点击，排序
  m_lstCommand.SortItems(CompareFunc, (LPARAM)&m_lstCommand);

  *pResult = 0;
}
