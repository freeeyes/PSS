#pragma once
#include "afxcmn.h"
#include "ClientDefine.h"
#include "TcpClientConnect.h"

#include <vector>
#include <time.h>

using namespace std;

struct _WorkThreadInfo
{
  char                m_szThreadName[50];   //线程名称
  int                 m_nThreadID;         //线程ID
  int                 m_nUpdateTime;       //线程最后处理数据的时间
  int                 m_nState;            //0为没有数据正在处理，1为正在处理接受数据。
  int                 m_nRecvPacketCount;  //此线程下接收包的处理个数
  int                 m_nSendPacketCount;  //此线程下发送包的处理个数
  int                 m_nCommandID;        //当前处理的信令ID
  int                 m_nPacketTime;       //平均数据包处理时间
  int                 m_nCurrPacketCount;  //单位时间内的包处理数量

  _WorkThreadInfo()
  {
    m_nThreadID        = 0;
    m_nState           = 0;
    m_nRecvPacketCount = 0;
    m_nSendPacketCount = 0;
    m_nCommandID       = 0;
    m_nPacketTime      = 0;
    m_nCurrPacketCount = 0;
  }
};

typedef vector<_WorkThreadInfo> vecWorkThreadInfo;

class CDlgClientWorkThread : public CDialog
{
	DECLARE_DYNAMIC(CDlgClientWorkThread)

public:
	CDlgClientWorkThread(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgClientWorkThread();

// Dialog Data
	enum { IDD = IDD_DIALOG_WORKTHREAD };

public:
  CString GetPageTitle();
  void SetTcpClientConnect(CTcpClientConnect* pTcpClientConnect);

private:
  CTcpClientConnect* m_pTcpClientConnect;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  CListCtrl m_lcWorkThreadInfo;
  afx_msg void OnBnClickedButton1();
  virtual BOOL OnInitDialog();
};
