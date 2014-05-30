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


//链接别名映射信息(用于PSS_ClientManager管理)
struct _ClientNameInfo
{
	char m_szName[100];      //连接别名 
	char m_szClientIP[20];   //客户端的IP地址
	int  m_nPort;            //客户端的端口
	int  m_nConnectID;       //连接ID 
	int  m_nLog;             //Log标记位

	_ClientNameInfo()
	{
		m_szName[0]     = '\0';
		m_szClientIP[0] = '\0';
		m_nPort         = 0;
		m_nConnectID    = 0;
		m_nLog          = 0;
	}
};
typedef vector<_ClientNameInfo> vecClientNameInfo;

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
	bool SendSetLog(int nConnectID, bool blFlag);

private:
	CTcpClientConnect* m_pTcpClientConnect;

public:
	DECLARE_MESSAGE_MAP()
	CEdit m_txtForbidenIP;
	CEdit m_txtForbidenSeconds;
	CButton m_btnTimeForbiden;
	CListCtrl m_lcForbidenList;
	CEdit m_txtNickName;
	CEdit m_txtConnectID;
	CListCtrl m_lcNickInfo;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton5();
};
