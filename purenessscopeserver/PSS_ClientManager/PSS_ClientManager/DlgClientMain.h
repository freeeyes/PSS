#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "TcpClientConnect.h"
#include "ClientDefine.h"

#include <vector>

using namespace std;

// CDlgClientMain dialog
struct _ServerInfo
{
  int  m_nSerevrID;
  char m_szServerName[20];
  char m_szServerVersion[20];
  int  m_nModuleCount;
  int  m_nWorkThreadCount;
  char m_szPacketVersion[20];

  _ServerInfo()
  {
    m_nSerevrID          = 0;
    m_szServerName[0]    = '\0';
    m_szServerVersion[0] = '\0';
    m_nModuleCount       = 0;
    m_nWorkThreadCount   = 0;
    m_szPacketVersion[0] = '\0';
  }
};

struct _ModuleInfo
{
  char szModuleName[200];
  char szModuleFile[200];
  char szModuleDesc[200];
  char szModuleCreateDate[200];

  _ModuleInfo()
  {
    szModuleName[0]       = '\0';
    szModuleFile[0]       = '\0';
    szModuleDesc[0]       = '\0';
    szModuleCreateDate[0] = '\0';
  }
};

typedef vector<_ModuleInfo> vecModuleInfo;

class CDlgClientMain : public CDialog
{
	DECLARE_DYNAMIC(CDlgClientMain)

public:
	CDlgClientMain(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgClientMain();

// Dialog Data
	enum { IDD = IDD_DIALOG_MAIN };

public:
  CString GetPageTitle();
  void SetTcpClientConnect(CTcpClientConnect* pTcpClientConnect);

private:
  CTcpClientConnect* m_pTcpClientConnect;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  CEdit m_txtServerIP;
  CEdit m_txtServerPort;
  CEdit m_txtServerInfo;
  CEdit m_txtModuleName;
  CListCtrl m_lcModuleList;
  CEdit m_txtKey;
  CEdit m_txtModuleFile;
  afx_msg void OnBnClickedButton1();
  afx_msg void OnBnClickedButton2();
  virtual BOOL OnInitDialog();
  afx_msg void OnBnClickedButton3();
  afx_msg void OnBnClickedButton4();
  afx_msg void OnBnClickedButton5();
  afx_msg void OnBnClickedButton7();
};
