#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "TcpClientConnect.h"
#include "ClientDefine.h"

#include <vector>

using namespace std;

struct _CommandInfo
{
  char szModuleName[200];
  char szCommandID[200];
  int  m_nCount;                     //当前命令被调用的次数
  int  m_nTimeCost;                  //当前命令总时间消耗

  _CommandInfo()
  {
    szModuleName[0] = '\0';
    szCommandID[0]  = '\0';
    m_nCount        = 0;
    m_nTimeCost     = 0;
  }
};

struct _CommandTimeOut
{
	short          nCommandID;                  //命令的ID
	int            nTime;                       //发生时间
	int            nTimeOutTime;                //超时时间

	_CommandTimeOut()
	{
		nCommandID   = 0;
		nTime        = 0;
		nTimeOutTime = 0;
	}
};

typedef vector<_CommandInfo> vecCommandInfo;

class CDlgClientModule : public CDialog
{
	DECLARE_DYNAMIC(CDlgClientModule)

public:
	CDlgClientModule(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgClientModule();

// Dialog Data
	enum { IDD = IDD_DIALOG_MODULE };

public:
  CString GetPageTitle();
  void SetTcpClientConnect(CTcpClientConnect* pTcpClientConnect);

private:
  CTcpClientConnect* m_pTcpClientConnect;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  CEdit m_txtModuleID;
  CListCtrl m_lcModuleInfo;
  CListCtrl m_lcCommandTimeout;
  afx_msg void OnBnClickedButton2();
  afx_msg void OnBnClickedButton1();
  virtual BOOL OnInitDialog();
  afx_msg void OnBnClickedButton8();
  afx_msg void OnBnClickedButton9();
  afx_msg void OnBnClickedButton10();
};
