// DlgClientConnect.cpp : implementation file
//

#include "stdafx.h"
#include "PSS_ClientManager.h"
#include "DlgClientConnect.h"


// CDlgClientConnect dialog

IMPLEMENT_DYNAMIC(CDlgClientConnect, CDialog)

CDlgClientConnect::CDlgClientConnect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgClientConnect::IDD, pParent)
{
  m_pTcpClientConnect = NULL;
}

CDlgClientConnect::~CDlgClientConnect()
{
}

void CDlgClientConnect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_txtClientConnectCount);
	DDX_Control(pDX, IDC_EDIT2, m_txtCpu);
	DDX_Control(pDX, IDC_EDIT5, m_txtMemory);
	DDX_Control(pDX, IDC_LIST1, m_lcClientConnectHistory);
	DDX_Control(pDX, IDC_EDIT3, m_txtDebugState);
	DDX_Control(pDX, IDC_RADIO2, m_btnDebug);
	DDX_Control(pDX, IDC_EDIT7, m_txtFlowIn);
	DDX_Control(pDX, IDC_EDIT9, m_txtFlowOut);
}


BEGIN_MESSAGE_MAP(CDlgClientConnect, CDialog)
  ON_BN_CLICKED(IDC_BUTTON1, &CDlgClientConnect::OnBnClickedButton1)
  ON_BN_CLICKED(IDC_BUTTON6, &CDlgClientConnect::OnBnClickedButton6)
  ON_BN_CLICKED(IDC_BUTTON5, &CDlgClientConnect::OnBnClickedButton5)
  ON_BN_CLICKED(IDC_BUTTON4, &CDlgClientConnect::OnBnClickedButton4)
END_MESSAGE_MAP()

CString CDlgClientConnect::GetPageTitle()
{
  return _T("客户端链接管理模块");
}

void CDlgClientConnect::SetTcpClientConnect( CTcpClientConnect* pTcpClientConnect )
{
  m_pTcpClientConnect = pTcpClientConnect;
}


// CDlgClientConnect message handlers

void CDlgClientConnect::OnBnClickedButton1()
{
  // TODO: Add your control notification handler code here
  char szSendMessage[200] = {'\0'};
  char szCommand[100]     = {'\0'};
  sprintf_s(szCommand, 100, "%s ClientCount -cp", m_pTcpClientConnect->GetKey());
  int nSendLen = (int)strlen(szCommand); 

  memcpy_s(szSendMessage, 200, &nSendLen, sizeof(int));
  memcpy_s(&szSendMessage[4], 200, &szCommand, nSendLen);

  char szRecvBuff[10 * 1024] = {'\0'};
  int nRecvLen = 10 * 1024;
  bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen + sizeof(int), (char*)szRecvBuff, nRecvLen);
  if(blState == false)
  {
    MessageBox(_T(MESSAGE_SENDERROR) , _T(MESSAGE_TITLE_ERROR), MB_OK);
    return;
  }
  else
  {
    int nStrLen       = 0;
    int nPos          = 4;
    int nConnectCount = 0;
    memcpy_s(&nConnectCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
    nPos += sizeof(int);

    CString strConnectCount;
    strConnectCount.Format(_T("%d"), nConnectCount);
    m_txtClientConnectCount.SetWindowText(strConnectCount);
  }

  //获得CPU和内存占用量
  sprintf_s(szCommand, 100, "%s ShowCurrProcessInfo -a", m_pTcpClientConnect->GetKey());
  nSendLen = (int)strlen(szCommand); 

  memcpy_s(szSendMessage, 200, &nSendLen, sizeof(int));
  memcpy_s(&szSendMessage[4], 200, &szCommand, nSendLen);

  nRecvLen = 10 * 1024;
  blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen + sizeof(int), (char*)szRecvBuff, nRecvLen);
  if(blState == false)
  {
    MessageBox(_T(MESSAGE_SENDERROR) , _T(MESSAGE_TITLE_ERROR), MB_OK);
    return;
  }
  else
  {
    int  nStrLen       = 0;
    int  nPos          = 4;
    int  nCpuRote      = 0;
    int  nMemoryRote   = 0;
	char cFlow         = 0;
	int  nFlowIn       = 0;
	int  nFlowOut      = 0;

    memcpy_s(&nCpuRote, sizeof(int), &szRecvBuff[nPos], sizeof(int));
    nPos += sizeof(int);
    memcpy_s(&nMemoryRote, sizeof(int), &szRecvBuff[nPos], sizeof(int));
    nPos += sizeof(int);
	memcpy_s(&cFlow, sizeof(char), &szRecvBuff[nPos], sizeof(char));
	nPos += sizeof(char);
	memcpy_s(&nFlowIn, sizeof(int), &szRecvBuff[nPos], sizeof(int));
	nPos += sizeof(int);
	memcpy_s(&nFlowOut, sizeof(int), &szRecvBuff[nPos], sizeof(int));
	nPos += sizeof(int);

    CString strCpuRote;
    strCpuRote.Format(_T("%f"), (float)nCpuRote/100.0f);
    m_txtCpu.SetWindowText(strCpuRote);

    CString strMemoryRote;
    strMemoryRote.Format(_T("%f"), (float)nMemoryRote/(1000.0f*1000.0f));
    m_txtMemory.SetWindowText(strMemoryRote);

	if(cFlow == 0)
	{
		m_txtFlowIn.SetWindowText(_T("关闭"));
		m_txtFlowOut.SetWindowText(_T("关闭"));
	}
	else
	{
		CString strFlowIn;
		CString strFlowOut;
		strFlowIn.Format(_T("%d"), nFlowIn);
		m_txtFlowIn.SetWindowText(strFlowIn);
		strFlowOut.Format(_T("%d"), nFlowOut);
		m_txtFlowOut.SetWindowText(strFlowOut);
	}
  }

}

BOOL CDlgClientConnect::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_lcClientConnectHistory.InsertColumn(0, _T("IP地址"), LVCFMT_CENTER, 100);
  m_lcClientConnectHistory.InsertColumn(1, _T("单位时间链接次数"), LVCFMT_CENTER, 150);
  m_lcClientConnectHistory.InsertColumn(2, _T("总链接数"), LVCFMT_CENTER, 100);
  m_lcClientConnectHistory.InsertColumn(3, _T("最后链接时间"), LVCFMT_CENTER, 200);

  return TRUE;
}

void CDlgClientConnect::OnBnClickedButton6()
{
  m_lcClientConnectHistory.DeleteAllItems();

  char szSendMessage[200] = {'\0'};
  char szCommand[100]     = {'\0'};
  sprintf_s(szCommand, 100, "%s ShowConnectHistory -a", m_pTcpClientConnect->GetKey());
  int nSendLen = (int)strlen(szCommand); 

  memcpy_s(szSendMessage, 200, &nSendLen, sizeof(int));
  memcpy_s(&szSendMessage[4], 200, &szCommand, nSendLen);

  char szRecvBuff[100 * 1024] = {'\0'};
  int nRecvLen = 100 * 1024;
  bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen + sizeof(int), (char*)szRecvBuff, nRecvLen);
  if(blState == false)
  {
    MessageBox(_T(MESSAGE_SENDERROR) , _T(MESSAGE_TITLE_ERROR), MB_OK);
    return;
  }
  else
  {
    vecIPAccount objvecIPAccount;

    int nStrLen       = 0;
    int nPos          = 4;
    int nTCPCount     = 0;
    memcpy_s(&nTCPCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
    nPos += sizeof(int);

    for(int i = 0; i < nTCPCount; i++)
    {
      //开始还原数据结构
      _IPAccount IPAccount;

      memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
      nPos += sizeof(char);

      memcpy_s(IPAccount.m_szIP, nStrLen, &szRecvBuff[nPos], nStrLen);
      nPos += nStrLen;
      IPAccount.m_szIP[nStrLen] = '\0';

      memcpy_s(&IPAccount.m_nCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
      nPos += sizeof(int);

      memcpy_s(&IPAccount.m_nAllCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
      nPos += sizeof(int);

      memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
      nPos += sizeof(char);

      memcpy_s(IPAccount.m_szDate, nStrLen, &szRecvBuff[nPos], nStrLen);
      nPos += nStrLen;
      IPAccount.m_szDate[nStrLen] = '\0';

      //显示在界面上
      wchar_t szIP[200]    = {'\0'};
      CString strIPCount;
      CString strIPAllCount;
      wchar_t szIPTime[200]    = {'\0'};

      int nSrcLen = MultiByteToWideChar(CP_ACP, 0, IPAccount.m_szIP, -1, NULL, 0);
      int nDecLen = MultiByteToWideChar(CP_ACP, 0, IPAccount.m_szIP, -1, szIP, 200);

      strIPCount.Format(_T("%d"), IPAccount.m_nCount);
      strIPAllCount.Format(_T("%d"), IPAccount.m_nAllCount);

      nSrcLen = MultiByteToWideChar(CP_ACP, 0, IPAccount.m_szDate, -1, NULL, 0);
      nDecLen = MultiByteToWideChar(CP_ACP, 0, IPAccount.m_szDate, -1, szIPTime, 200);

      m_lcClientConnectHistory.InsertItem(i, szIP);
      m_lcClientConnectHistory.SetItemText(i, 1, strIPCount);
      m_lcClientConnectHistory.SetItemText(i, 2, strIPAllCount);
      m_lcClientConnectHistory.SetItemText(i, 3, szIPTime);

      objvecIPAccount.push_back(IPAccount);
    }

  }
}

void CDlgClientConnect::OnBnClickedButton5()
{
	//获得DEBUG状态
	char szSendMessage[200] = {'\0'};
	char szCommand[100]     = {'\0'};
	sprintf_s(szCommand, 100, "%s ShowDebug -a", m_pTcpClientConnect->GetKey());
	int nSendLen = (int)strlen(szCommand); 

	memcpy_s(szSendMessage, 200, &nSendLen, sizeof(int));
	memcpy_s(&szSendMessage[4], 200, &szCommand, nSendLen);

	char szRecvBuff[10 * 1024] = {'\0'};
	int nRecvLen = 10 * 1024;
	bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen + sizeof(int), (char*)szRecvBuff, nRecvLen);
	if(blState == false)
	{
		MessageBox(_T(MESSAGE_SENDERROR) , _T(MESSAGE_TITLE_ERROR), MB_OK);
	}
	else
	{
		int nStrLen      = 0;
		int nPos         = 4;
		int nResult      = 0;
		memcpy_s(&nResult, sizeof(int), &szRecvBuff[nPos], sizeof(char));
		nPos += sizeof(int);

		if(nResult == 0)
		{
			m_txtDebugState.SetWindowText(_T("DEBUG日志关闭"));
		}
		else
		{
			m_txtDebugState.SetWindowText(_T("DEBUG日志打开"));
		}
	}
}

void CDlgClientConnect::OnBnClickedButton4()
{
	//设置DEBUG状态
	int nType = 0;
	switch(GetCheckedRadioButton(IDC_RADIO1, IDC_RADIO2))
	{
	case IDC_RADIO1:
		nType = 0;
		break;
	case IDC_RADIO2:
		nType = 1;
		break;
	}

	char szSendMessage[200] = {'\0'};
	char szCommand[100]     = {'\0'};
	sprintf_s(szCommand, 100, "%s SetDebug -s %d ", m_pTcpClientConnect->GetKey(), nType);
	int nSendLen = (int)strlen(szCommand); 

	memcpy_s(szSendMessage, 200, &nSendLen, sizeof(int));
	memcpy_s(&szSendMessage[4], 200, &szCommand, nSendLen);

	char szRecvBuff[10 * 1024] = {'\0'};
	int nRecvLen = 10 * 1024;
	bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen + sizeof(int), (char*)szRecvBuff, nRecvLen);
	if(blState == false)
	{
		MessageBox(_T(MESSAGE_SENDERROR) , _T(MESSAGE_TITLE_ERROR), MB_OK);
	}
	else
	{
		int nStrLen      = 0;
		int nPos         = 4;
		int nResult      = 0;
		memcpy_s(&nResult, sizeof(int), &szRecvBuff[nPos], sizeof(char));
		nPos += sizeof(int);

		if(nResult == 0)
		{
			MessageBox(_T(MESSAGE_RESULT_SUCCESS) , _T(MESSAGE_TITLE_ERROR), MB_OK);
			OnBnClickedButton5();
		}
		else
		{
			MessageBox(_T(MESSAGE_RESULT_FAIL) , _T(MESSAGE_TITLE_SUCCESS), MB_OK);
		}
	}
}
