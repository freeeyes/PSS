#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "TcpClientConnect.h"
#include "ClientDefine.h"

#include "XmlOpeation.h"

#include <vector>

using namespace std;

#define CONFIG_NAME "./Config.xml"

//PSS远程服务器配置信息
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

//PSS模块加载信息
struct _ModuleInfo
{
	char szModuleName[200];
	char szModuleFile[200];
	char szModulePath[200];
	char szModuleParam[200];
	char szModuleDesc[200];
	char szModuleCreateDate[200];
	int  nModuleState;
	int  nModuleID;

	_ModuleInfo()
	{
		szModuleName[0]       = '\0';
		szModuleFile[0]       = '\0';
		szModulePath[0]       = '\0';
		szModuleParam[0]      = '\0';
		szModuleDesc[0]       = '\0';
		szModuleCreateDate[0] = '\0';
		nModuleState          = 0;
		nModuleID             = 0;
	}
};

//PSS配置文件信息
struct _ConfigInfo
{
	char m_szServerName[200];
	char m_szIP[50]; 
	int  m_nPort;
	char m_szMagicCode[300];

	_ConfigInfo()
	{
		m_szServerName[0] = '\0';
		m_szIP[0]         = '\0';
		m_nPort           = 0;
		m_szMagicCode[0]  = '\0';
	}
};

typedef vector<_ConfigInfo> vecConfigInfo;

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
	void ShowConfigList();

private:
	CTcpClientConnect* m_pTcpClientConnect;
	CXmlOpeation       m_objXmlOpeation;
	vecConfigInfo      m_vecConfigInfo;

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
	CEdit m_txtModuleParam;
	CEdit m_txtModuleFileName;
	CListBox m_lbConfig;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnLbnSelchangeList3();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnHdnItemclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
};
