// DlgClientWorkThread.cpp : implementation file
//

#include "stdafx.h"
#include "PSS_ClientManager.h"
#include "DlgClientWorkThread.h"


// CDlgClientWorkThread dialog

IMPLEMENT_DYNAMIC(CDlgClientWorkThread, CDialog)

CDlgClientWorkThread::CDlgClientWorkThread(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgClientWorkThread::IDD, pParent)
{

}

CDlgClientWorkThread::~CDlgClientWorkThread()
{
}

void CDlgClientWorkThread::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_LIST1, m_lcWorkThreadInfo);
}


BEGIN_MESSAGE_MAP(CDlgClientWorkThread, CDialog)
  ON_BN_CLICKED(IDC_BUTTON1, &CDlgClientWorkThread::OnBnClickedButton1)
END_MESSAGE_MAP()

CString CDlgClientWorkThread::GetPageTitle()
{
  return _T("工作线程状态");
}


// CDlgClientWorkThread message handlers

void CDlgClientWorkThread::OnBnClickedButton1()
{
  //查看工作线程信息
  m_lcWorkThreadInfo.DeleteAllItems();

  char szSendMessage[200] = {'\0'};
  char szCommand[100]     = {'\0'};
  sprintf_s(szCommand, 100, "%s WorkThreadState -s", m_pTcpClientConnect->GetKey());
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
    vecWorkThreadInfo objvecWorkThreadInfo;

    int nStrLen       = 0;
    int nPos          = 4;
    int nThreadCount  = 0;
    memcpy_s(&nThreadCount, sizeof(char), &szRecvBuff[nPos], sizeof(char));
    nPos += sizeof(char);

    for(int i = 0; i < nThreadCount; i++)
    {
      //开始还原数据结构
      _WorkThreadInfo WorkThreadInfo;
      sprintf_s(WorkThreadInfo.m_szThreadName, 50, "工作线程");

      memcpy_s(&WorkThreadInfo.m_nThreadID, sizeof(char), &szRecvBuff[nPos], sizeof(char));
      nPos += sizeof(char);

      char szUpdateTime[30] = {'\0'};
      memcpy_s(&WorkThreadInfo.m_nUpdateTime, sizeof(int), &szRecvBuff[nPos], sizeof(int));
      nPos += sizeof(int);
      struct tm tmDate;
      time_t newRawTime = WorkThreadInfo.m_nUpdateTime;
      localtime_s(&tmDate, &newRawTime);
      sprintf_s(szUpdateTime, 30, "%04d-%02d-%02d %02d:%02d:%02d", tmDate.tm_year + 1900, 
        tmDate.tm_mon + 1,
        tmDate.tm_mday,
        tmDate.tm_hour,
        tmDate.tm_min,
        tmDate.tm_sec);

      memcpy_s(&WorkThreadInfo.m_nState, sizeof(char), &szRecvBuff[nPos], sizeof(char));
      nPos += sizeof(char);

      memcpy_s(&WorkThreadInfo.m_nRecvPacketCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
      nPos += sizeof(int);

      memcpy_s(&WorkThreadInfo.m_nSendPacketCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
      nPos += sizeof(int);

      memcpy_s(&WorkThreadInfo.m_nCommandID, sizeof(short), &szRecvBuff[nPos], sizeof(short));
      nPos += sizeof(short);

      memcpy_s(&WorkThreadInfo.m_nPacketTime, sizeof(short), &szRecvBuff[nPos], sizeof(short));
      nPos += sizeof(short);

      memcpy_s(&WorkThreadInfo.m_nCurrPacketCount, sizeof(int), &szRecvBuff[nPos], sizeof(int));
      nPos += sizeof(int);

      //显示在界面上
      wchar_t szzTheadName[50]  = {'\0'};
      wchar_t szzUpdateTime[30] = {'\0'};
      CString strThreadID;
      CString strSendPacketCount;
      CString strRecvPacketCount;
      CString strCurrCommandID;
      CString strPacketTime;
      CString strAvgDisposeCount;

      int nSrcLen = MultiByteToWideChar(CP_ACP, 0, WorkThreadInfo.m_szThreadName, -1, NULL, 0);
      int nDecLen = MultiByteToWideChar(CP_ACP, 0, WorkThreadInfo.m_szThreadName, -1, szzTheadName, 50);

      nSrcLen = MultiByteToWideChar(CP_ACP, 0, szUpdateTime, -1, NULL, 0);
      nDecLen = MultiByteToWideChar(CP_ACP, 0, szUpdateTime, -1, szzUpdateTime, 50);

      strThreadID.Format(_T("%d"), WorkThreadInfo.m_nThreadID);
      strRecvPacketCount.Format(_T("%d"), WorkThreadInfo.m_nRecvPacketCount);
      strSendPacketCount.Format(_T("%d"), WorkThreadInfo.m_nSendPacketCount);
      strCurrCommandID.Format(_T("%d"), WorkThreadInfo.m_nCommandID);
      strPacketTime.Format(_T("%d"), WorkThreadInfo.m_nPacketTime);
      strAvgDisposeCount.Format(_T("%d"), WorkThreadInfo.m_nCurrPacketCount);

      m_lcWorkThreadInfo.InsertItem(i, _T("工作线程"));
      m_lcWorkThreadInfo.SetItemText(i, 1, strThreadID);
      m_lcWorkThreadInfo.SetItemText(i, 2, szzUpdateTime);
      m_lcWorkThreadInfo.SetItemText(i, 3, strRecvPacketCount);
      m_lcWorkThreadInfo.SetItemText(i, 4, strSendPacketCount);
      m_lcWorkThreadInfo.SetItemText(i, 5, strCurrCommandID);
      m_lcWorkThreadInfo.SetItemText(i, 6, strPacketTime);
      m_lcWorkThreadInfo.SetItemText(i, 7, strAvgDisposeCount);

      objvecWorkThreadInfo.push_back(WorkThreadInfo);
    }
  }
}

void CDlgClientWorkThread::SetTcpClientConnect( CTcpClientConnect* pTcpClientConnect )
{
  m_pTcpClientConnect = pTcpClientConnect;
}

BOOL CDlgClientWorkThread::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_lcWorkThreadInfo.InsertColumn(0, _T("线程名称"), LVCFMT_CENTER, 100);
  m_lcWorkThreadInfo.InsertColumn(1, _T("线程ID"), LVCFMT_CENTER, 100);
  m_lcWorkThreadInfo.InsertColumn(2, _T("最后执行时间"), LVCFMT_CENTER, 100);
  m_lcWorkThreadInfo.InsertColumn(3, _T("接收包数量"), LVCFMT_CENTER, 80);
  m_lcWorkThreadInfo.InsertColumn(4, _T("发送包数量"), LVCFMT_CENTER, 80);
  m_lcWorkThreadInfo.InsertColumn(5, _T("当前处理ID"), LVCFMT_CENTER, 80);
  m_lcWorkThreadInfo.InsertColumn(6, _T("平均耗时"), LVCFMT_CENTER, 80);
  m_lcWorkThreadInfo.InsertColumn(7, _T("单位处理量"), LVCFMT_CENTER, 80);

  return TRUE;
}
