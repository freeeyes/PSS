// DlgServerConnect.cpp : implementation file
//

#include "stdafx.h"
#include "PSS_ClientManager.h"
#include "DlgServerConnect.h"


// CDlgServerConnect dialog

IMPLEMENT_DYNAMIC(CDlgServerConnect, CDialog)

CDlgServerConnect::CDlgServerConnect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgServerConnect::IDD, pParent)
{

}

CDlgServerConnect::~CDlgServerConnect()
{
}

void CDlgServerConnect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lcServerConnect);
	DDX_Control(pDX, IDC_EDIT1, m_txtServerID);
}


BEGIN_MESSAGE_MAP(CDlgServerConnect, CDialog)
  ON_BN_CLICKED(IDC_BUTTON2, &CDlgServerConnect::OnBnClickedButton2)
  ON_BN_CLICKED(IDC_BUTTON1, &CDlgServerConnect::OnBnClickedButton1)
END_MESSAGE_MAP()

CString CDlgServerConnect::GetPageTitle()
{
  return _T("服务器间链接信息");
}


// CDlgServerConnect message handlers

void CDlgServerConnect::OnBnClickedButton2()
{
  //查看TCP
  m_lcServerConnect.DeleteAllItems();

  char szSendMessage[200] = {'\0'};
  char szCommand[100]     = {'\0'};
  sprintf_s(szCommand, 100, "%s ServerConnectTCP -a", m_pTcpClientConnect->GetKey());
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
    vecClientConnectInfo objvecClientConnectInfo;

    int nStrLen       = 0;
    int nPos          = 4;
    int nTCPCount     = 0;
	int nState        = 0;
    memcpy_s(&nTCPCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
    nPos += sizeof(int);

    for(int i = 0; i < nTCPCount; i++)
    {
      //开始还原数据结构
      _ClientConnectInfo ClientConnectInfo;

	  memcpy_s(&nState, sizeof(int), &szRecvBuff[nPos], sizeof(char));
	  nPos += sizeof(char);

      memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
      nPos += sizeof(char);

      memcpy_s(ClientConnectInfo.m_szIP, nStrLen, &szRecvBuff[nPos], nStrLen);
      nPos += nStrLen;
      ClientConnectInfo.m_szIP[nStrLen] = '\0';

      memcpy_s(&ClientConnectInfo.m_nConnectID, sizeof(int), &szRecvBuff[nPos], sizeof(int));
      nPos += sizeof(int);

      memcpy_s(&ClientConnectInfo.m_nRecvCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
      nPos += sizeof(int);

      memcpy_s(&ClientConnectInfo.m_nSendCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
      nPos += sizeof(int);

      memcpy_s(&ClientConnectInfo.m_nAllRecvSize, sizeof(int), &szRecvBuff[nPos], sizeof(int));
      nPos += sizeof(int);

      memcpy_s(&ClientConnectInfo.m_nAllSendSize, sizeof(int), &szRecvBuff[nPos], sizeof(int));
      nPos += sizeof(int);

	  char szUpdateTime[30] = {'\0'};
      memcpy_s(&ClientConnectInfo.m_nBeginTime, sizeof(int), &szRecvBuff[nPos], sizeof(int));
      nPos += sizeof(int);
	  struct tm tmDate;
	  time_t newRawTime = ClientConnectInfo.m_nBeginTime;
	  localtime_s(&tmDate, &newRawTime);
	  sprintf_s(szUpdateTime, 30, "%04d-%02d-%02d %02d:%02d:%02d", tmDate.tm_year + 1900, 
		  tmDate.tm_mon + 1,
		  tmDate.tm_mday,
		  tmDate.tm_hour,
		  tmDate.tm_min,
		  tmDate.tm_sec);

      memcpy_s(&ClientConnectInfo.m_nAliveTime, sizeof(int), &szRecvBuff[nPos], sizeof(int));
      nPos += sizeof(int);

      memcpy_s(&ClientConnectInfo.m_nRecvQueueCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
      nPos += sizeof(int);

      //memcpy_s(&ClientConnectInfo.m_nRecvQueueTimeCost, sizeof(long long), &szRecvBuff[nPos], sizeof(long long));
      nPos += sizeof(long long);

      //memcpy_s(&ClientConnectInfo.m_nSendQueueTimeCost, sizeof(long long), &szRecvBuff[nPos], sizeof(long long));
      nPos += sizeof(long long);

      //显示在界面上
      wchar_t szzTCPIP[50]      = {'\0'};
	  wchar_t szzUpdateTime[30] = {'\0'};
      CString strConnectID;
      CString strSendPacketCount;
      CString strRecvPacketCount;
      CString strSendSize;
      CString strRecvSize;
      CString strCreateTime;
      CString strAliveSecond;
      CString strLogicCount;

      int nSrcLen = MultiByteToWideChar(CP_ACP, 0, ClientConnectInfo.m_szIP, -1, NULL, 0);
      int nDecLen = MultiByteToWideChar(CP_ACP, 0, ClientConnectInfo.m_szIP, -1, szzTCPIP, 50);

	  nSrcLen = MultiByteToWideChar(CP_ACP, 0, szUpdateTime, -1, NULL, 0);
	  nDecLen = MultiByteToWideChar(CP_ACP, 0, szUpdateTime, -1, szzUpdateTime, 30);

      strConnectID.Format(_T("%d"), ClientConnectInfo.m_nConnectID);
      strSendPacketCount.Format(_T("%d"), ClientConnectInfo.m_nSendCount);
      strRecvPacketCount.Format(_T("%d"), ClientConnectInfo.m_nRecvCount);
      strSendSize.Format(_T("%d"), ClientConnectInfo.m_nAllSendSize);
      strRecvSize.Format(_T("%d"), ClientConnectInfo.m_nAllRecvSize);
      strCreateTime.Format(_T("%d"), ClientConnectInfo.m_nBeginTime);
      strAliveSecond.Format(_T("%d"), ClientConnectInfo.m_nAliveTime);
      strLogicCount.Format(_T("%d"), ClientConnectInfo.m_nRecvQueueCount);

      m_lcServerConnect.InsertItem(i, strConnectID);
      m_lcServerConnect.SetItemText(i, 1, szzTCPIP);
	  if(nState == 0)
	  {
		  m_lcServerConnect.SetItemText(i, 2, _T("链接已存在"));
	  }
	  else
	  {
		  m_lcServerConnect.SetItemText(i, 2, _T("链接不存在"));
	  }
      m_lcServerConnect.SetItemText(i, 3, strSendPacketCount);
      m_lcServerConnect.SetItemText(i, 4, strRecvPacketCount);
      m_lcServerConnect.SetItemText(i, 5, strSendSize);
      m_lcServerConnect.SetItemText(i, 6, strRecvSize);
      m_lcServerConnect.SetItemText(i, 7, szzUpdateTime);
      m_lcServerConnect.SetItemText(i, 8, strAliveSecond);

      objvecClientConnectInfo.push_back(ClientConnectInfo);
    }
  }
}

void CDlgServerConnect::SetTcpClientConnect( CTcpClientConnect* pTcpClientConnect )
{
  m_pTcpClientConnect = pTcpClientConnect;
}

BOOL CDlgServerConnect::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_lcServerConnect.InsertColumn(0, _T("ID"), LVCFMT_CENTER, 100);
  m_lcServerConnect.InsertColumn(1, _T("IP地址"), LVCFMT_CENTER, 100);
  m_lcServerConnect.InsertColumn(2, _T("连接状态"), LVCFMT_CENTER, 80);
  m_lcServerConnect.InsertColumn(3, _T("发送包数"), LVCFMT_CENTER, 80);
  m_lcServerConnect.InsertColumn(4, _T("接收包数"), LVCFMT_CENTER, 80);
  m_lcServerConnect.InsertColumn(5, _T("发送字节"), LVCFMT_CENTER, 80);
  m_lcServerConnect.InsertColumn(6, _T("接收字节"), LVCFMT_CENTER, 80);
  m_lcServerConnect.InsertColumn(7, _T("创建时间"), LVCFMT_CENTER, 80);
  m_lcServerConnect.InsertColumn(8, _T("存活秒数"), LVCFMT_CENTER, 80);

  return TRUE;
}

void CDlgServerConnect::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strServerID;
	char szServerIP[30] = {'\0'};
	m_txtServerID.GetWindowText(strServerID);

	int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strServerID, strServerID.GetLength(), NULL, 0, NULL, NULL);
	int nDecLen = WideCharToMultiByte(CP_ACP, 0, strServerID, nSrcLen, szServerIP, 30, NULL,NULL);

	m_lcServerConnect.DeleteAllItems();

	char szSendMessage[200] = {'\0'};
	char szCommand[100]     = {'\0'};
	sprintf_s(szCommand, 100, "%s ReConnectServer -s %s", m_pTcpClientConnect->GetKey(), szServerIP);
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
		vecClientConnectInfo objvecClientConnectInfo;

		int nStrLen       = 0;
		int nPos          = 4;
		int nTCPCount     = 0;
		int nState        = 0;
		memcpy_s(&nTCPCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
		nPos += sizeof(int);

		for(int i = 0; i < nTCPCount; i++)
		{
			//开始还原数据结构
			_ClientConnectInfo ClientConnectInfo;

			memcpy_s(&nState, sizeof(int), &szRecvBuff[nPos], sizeof(char));
			nPos += sizeof(char);

			memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
			nPos += sizeof(char);

			memcpy_s(ClientConnectInfo.m_szIP, nStrLen, &szRecvBuff[nPos], nStrLen);
			nPos += nStrLen;
			ClientConnectInfo.m_szIP[nStrLen] = '\0';

			memcpy_s(&ClientConnectInfo.m_nConnectID, sizeof(int), &szRecvBuff[nPos], sizeof(int));
			nPos += sizeof(int);

			memcpy_s(&ClientConnectInfo.m_nRecvCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
			nPos += sizeof(int);

			memcpy_s(&ClientConnectInfo.m_nSendCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
			nPos += sizeof(int);

			memcpy_s(&ClientConnectInfo.m_nAllRecvSize, sizeof(int), &szRecvBuff[nPos], sizeof(int));
			nPos += sizeof(int);

			memcpy_s(&ClientConnectInfo.m_nAllSendSize, sizeof(int), &szRecvBuff[nPos], sizeof(int));
			nPos += sizeof(int);

			char szUpdateTime[30] = {'\0'};
			memcpy_s(&ClientConnectInfo.m_nBeginTime, sizeof(int), &szRecvBuff[nPos], sizeof(int));
			nPos += sizeof(int);
			struct tm tmDate;
			time_t newRawTime = ClientConnectInfo.m_nBeginTime;
			localtime_s(&tmDate, &newRawTime);
			sprintf_s(szUpdateTime, 30, "%04d-%02d-%02d %02d:%02d:%02d", tmDate.tm_year + 1900, 
				tmDate.tm_mon + 1,
				tmDate.tm_mday,
				tmDate.tm_hour,
				tmDate.tm_min,
				tmDate.tm_sec);

			memcpy_s(&ClientConnectInfo.m_nAliveTime, sizeof(int), &szRecvBuff[nPos], sizeof(int));
			nPos += sizeof(int);

			memcpy_s(&ClientConnectInfo.m_nRecvQueueCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
			nPos += sizeof(int);

			//memcpy_s(&ClientConnectInfo.m_nRecvQueueTimeCost, sizeof(long long), &szRecvBuff[nPos], sizeof(long long));
			nPos += sizeof(long long);

			//memcpy_s(&ClientConnectInfo.m_nSendQueueTimeCost, sizeof(long long), &szRecvBuff[nPos], sizeof(long long));
			nPos += sizeof(long long);

			//显示在界面上
			wchar_t szzTCPIP[50]      = {'\0'};
			wchar_t szzUpdateTime[30] = {'\0'};
			CString strConnectID;
			CString strSendPacketCount;
			CString strRecvPacketCount;
			CString strSendSize;
			CString strRecvSize;
			CString strCreateTime;
			CString strAliveSecond;
			CString strLogicCount;

			int nSrcLen = MultiByteToWideChar(CP_ACP, 0, ClientConnectInfo.m_szIP, -1, NULL, 0);
			int nDecLen = MultiByteToWideChar(CP_ACP, 0, ClientConnectInfo.m_szIP, -1, szzTCPIP, 50);

			nSrcLen = MultiByteToWideChar(CP_ACP, 0, szUpdateTime, -1, NULL, 0);
			nDecLen = MultiByteToWideChar(CP_ACP, 0, szUpdateTime, -1, szzUpdateTime, 30);

			strConnectID.Format(_T("%d"), ClientConnectInfo.m_nConnectID);
			strSendPacketCount.Format(_T("%d"), ClientConnectInfo.m_nSendCount);
			strRecvPacketCount.Format(_T("%d"), ClientConnectInfo.m_nRecvCount);
			strSendSize.Format(_T("%d"), ClientConnectInfo.m_nAllSendSize);
			strRecvSize.Format(_T("%d"), ClientConnectInfo.m_nAllRecvSize);
			strCreateTime.Format(_T("%d"), ClientConnectInfo.m_nBeginTime);
			strAliveSecond.Format(_T("%d"), ClientConnectInfo.m_nAliveTime);
			strLogicCount.Format(_T("%d"), ClientConnectInfo.m_nRecvQueueCount);

			m_lcServerConnect.InsertItem(i, strConnectID);
			m_lcServerConnect.SetItemText(i, 1, szzTCPIP);
			if(nState == 0)
			{
				m_lcServerConnect.SetItemText(i, 2, _T("链接已存在"));
			}
			else
			{
				m_lcServerConnect.SetItemText(i, 2, _T("链接不存在"));
			}
			m_lcServerConnect.SetItemText(i, 3, strSendPacketCount);
			m_lcServerConnect.SetItemText(i, 4, strRecvPacketCount);
			m_lcServerConnect.SetItemText(i, 5, strSendSize);
			m_lcServerConnect.SetItemText(i, 6, strRecvSize);
			m_lcServerConnect.SetItemText(i, 7, szzUpdateTime);
			m_lcServerConnect.SetItemText(i, 8, strAliveSecond);

			objvecClientConnectInfo.push_back(ClientConnectInfo);
		}
	}

	MessageBox(_T(MESSAGE_RESULT_SUCCESS) , _T(MESSAGE_TITLE_ERROR), MB_OK);
}
