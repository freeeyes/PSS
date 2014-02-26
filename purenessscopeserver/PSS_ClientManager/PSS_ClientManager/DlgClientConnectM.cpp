// DlgClientConnectM.cpp : implementation file
//

#include "stdafx.h"
#include "PSS_ClientManager.h"
#include "DlgClientConnectM.h"


// CDlgClientConnectM dialog

IMPLEMENT_DYNAMIC(CDlgClientConnectM, CDialog)

CDlgClientConnectM::CDlgClientConnectM(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgClientConnectM::IDD, pParent)
{

}

CDlgClientConnectM::~CDlgClientConnectM()
{
}

void CDlgClientConnectM::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_EDIT1, m_txtClientSocketID);
  DDX_Control(pDX, IDC_LIST1, m_lcClientTCPInfo);
}


BEGIN_MESSAGE_MAP(CDlgClientConnectM, CDialog)
  ON_BN_CLICKED(IDC_BUTTON1, &CDlgClientConnectM::OnBnClickedButton1)
  ON_BN_CLICKED(IDC_BUTTON2, &CDlgClientConnectM::OnBnClickedButton2)
END_MESSAGE_MAP()

CString CDlgClientConnectM::GetPageTitle()
{
  return _T("客户端链接管理");
}


// CDlgClientConnectM message handlers

void CDlgClientConnectM::OnBnClickedButton1()
{
  //断开链接
  char szScoketFD[50] = {'\0'};
  CString strSocketFD;
  m_txtClientSocketID.GetWindowText(strSocketFD);

  int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strSocketFD, strSocketFD.GetLength(), NULL, 0, NULL, NULL);
  int nDecLen = WideCharToMultiByte(CP_ACP, 0, strSocketFD, nSrcLen, szScoketFD, 50, NULL,NULL);
  szScoketFD[nDecLen] = '\0';

  if(strlen(szScoketFD) == 0)
  {
    MessageBox(_T(MESSAGE_INSERT_NULL) , _T(MESSAGE_TITLE_ERROR), MB_OK);
    return;
  }

  char szSendMessage[200] = {'\0'};
  char szCommand[100]     = {'\0'};
  sprintf_s(szCommand, 100, "%s CloseClient %d", m_pTcpClientConnect->GetKey(), (int)atoi(szScoketFD));
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
    int nStrLen       = 0;
    int nPos          = 4;
    int nOPState      = 0;
    memcpy_s(&nOPState, sizeof(int), &szRecvBuff[nPos], sizeof(int));
    nPos += sizeof(int);

    memcpy_s(&nOPState, sizeof(char), &szRecvBuff[4], sizeof(char));

    if(nOPState == 0)
    {
      MessageBox(_T(MESSAGE_RESULT_SUCCESS) , _T(MESSAGE_TITLE_ERROR), MB_OK);
    }
    else
    {
      MessageBox(_T(MESSAGE_RESULT_FAIL) , _T(MESSAGE_TITLE_SUCCESS), MB_OK);
    }
  }

}

void CDlgClientConnectM::OnBnClickedButton2()
{
  //查看TCP
  m_lcClientTCPInfo.DeleteAllItems();

  char szSendMessage[200] = {'\0'};
  char szCommand[100]     = {'\0'};
  sprintf_s(szCommand, 100, "%s ConnectClient -a", m_pTcpClientConnect->GetKey());
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
    memcpy_s(&nTCPCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
    nPos += sizeof(int);

    for(int i = 0; i < nTCPCount; i++)
    {
      //开始还原数据结构
      _ClientConnectInfo ClientConnectInfo;

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

      memcpy_s(&ClientConnectInfo.m_nRecvQueueTimeCost, sizeof(long long), &szRecvBuff[nPos], sizeof(long long));
      nPos += sizeof(long long);

      memcpy_s(&ClientConnectInfo.m_nSendQueueTimeCost, sizeof(long long), &szRecvBuff[nPos], sizeof(long long));
      nPos += sizeof(long long);

      //显示在界面上
      wchar_t szzTCPIP[50]      = {'\0'};
      wchar_t szzUpdateTime[30] = {'\0'};
      CString strConnectID;
      CString strSendPacketCount;
      CString strRecvPacketCount;
      CString strSendSize;
      CString strRecvSize;
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
      strAliveSecond.Format(_T("%d"), ClientConnectInfo.m_nAliveTime);
      strLogicCount.Format(_T("%d"), ClientConnectInfo.m_nRecvQueueCount);

      m_lcClientTCPInfo.InsertItem(i, szzTCPIP);
      m_lcClientTCPInfo.SetItemText(i, 1, strConnectID);
      m_lcClientTCPInfo.SetItemText(i, 2, strSendPacketCount);
      m_lcClientTCPInfo.SetItemText(i, 3, strRecvPacketCount);
      m_lcClientTCPInfo.SetItemText(i, 4, strSendSize);
      m_lcClientTCPInfo.SetItemText(i, 5, strRecvSize);
      m_lcClientTCPInfo.SetItemText(i, 6, szzUpdateTime);
      m_lcClientTCPInfo.SetItemText(i, 7, strAliveSecond);
      m_lcClientTCPInfo.SetItemText(i, 8, strLogicCount);

      objvecClientConnectInfo.push_back(ClientConnectInfo);
    }
  }
}

void CDlgClientConnectM::SetTcpClientConnect( CTcpClientConnect* pTcpClientConnect )
{
  m_pTcpClientConnect = pTcpClientConnect;
}

BOOL CDlgClientConnectM::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_lcClientTCPInfo.InsertColumn(0, _T("IP地址"), LVCFMT_CENTER, 100);
  m_lcClientTCPInfo.InsertColumn(1, _T("Socketfd"), LVCFMT_CENTER, 80);
  m_lcClientTCPInfo.InsertColumn(2, _T("发送包数"), LVCFMT_CENTER, 80);
  m_lcClientTCPInfo.InsertColumn(3, _T("接收包数"), LVCFMT_CENTER, 80);
  m_lcClientTCPInfo.InsertColumn(4, _T("发送字节"), LVCFMT_CENTER, 80);
  m_lcClientTCPInfo.InsertColumn(5, _T("接收字节"), LVCFMT_CENTER, 80);
  m_lcClientTCPInfo.InsertColumn(6, _T("创建时间"), LVCFMT_CENTER, 80);
  m_lcClientTCPInfo.InsertColumn(7, _T("存活秒数"), LVCFMT_CENTER, 80);
  m_lcClientTCPInfo.InsertColumn(8, _T("逻辑处理包数"), LVCFMT_CENTER, 120);

  return TRUE;
}
