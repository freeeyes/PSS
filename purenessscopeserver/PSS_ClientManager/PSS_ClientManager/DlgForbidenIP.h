#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ClientDefine.h"
#include "TcpClientConnect.h"

#include <vector>
#include <time.h>

using namespace std;

struct _ForbiddenIP
{
  char  m_szIP[50];
  int   m_nType;
  int   m_nBeginTime;
  int   m_nSecond;

  _ForbiddenIP()
  {
    m_szIP[0]    = '\0';
    m_nType      = 0;
    m_nBeginTime = 0;
    m_nSecond    = 0;
  }
};

typedef vector<_ForbiddenIP> vecForbiddenIP;

class CDlgForbidenIP : public CDialog
{
	DECLARE_DYNAMIC(CDlgForbidenIP)

public:
	CDlgForbidenIP(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgForbidenIP();

// Dialog Data
	enum { IDD = IDD_DIALOG_FORBIDENIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
  CString GetPageTitle();
  void SetTcpClientConnect(CTcpClientConnect* pTcpClientConnect);

private:
  CTcpClientConnect* m_pTcpClientConnect;

public:
	DECLARE_MESSAGE_MAP()
  CEdit m_txtForbidenIP;
  CEdit m_txtForbidenSeconds;
  CButton m_btnTimeForbiden;
  CListCtrl m_lcForbidenList;
  afx_msg void OnBnClickedButton1();
  afx_msg void OnBnClickedButton2();
  virtual BOOL OnInitDialog();
  afx_msg void OnBnClickedButton3();
};
