#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ClientDefine.h"
#include "TcpClientConnect.h"

#include <vector>

using namespace std;

typedef vector<_ClientConnectInfo> vecClientConnectInfo;

class CDlgClientConnectM : public CDialog
{
	DECLARE_DYNAMIC(CDlgClientConnectM)

public:
	CDlgClientConnectM(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgClientConnectM();

// Dialog Data
	enum { IDD = IDD_DIALOG_CLIENTCONNECTM };

public:
  CString GetPageTitle();
  void SetTcpClientConnect(CTcpClientConnect* pTcpClientConnect);

private:
  CTcpClientConnect* m_pTcpClientConnect;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  CEdit m_txtClientSocketID;
  CListCtrl m_lcClientTCPInfo;
  CEdit m_txtLogLevel;
  CListCtrl m_lcLogInfo;
  afx_msg void OnBnClickedButton1();
  afx_msg void OnBnClickedButton2();
  virtual BOOL OnInitDialog();
  afx_msg void OnBnClickedButton8();
  afx_msg void OnBnClickedButton9();
};
