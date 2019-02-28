// LoginClientDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include <WinSock2.h>

#include <vector>

using namespace std;

#define MAX_BUFF_50      50
#define MAX_BUFF_500     500

#define COMMAND_LOGIN         0x2100     //登陆
#define COMMAND_LOGOUT        0x2101     //退出 
#define COMMAND_USERINFO      0x2102     //获得玩家信息  
#define COMMAND_SET_USERINFO  0x2103     //设置用户信息 

#define LOGIN_SUCCESS            0
#define LOGIN_FAIL_NOEXIST       1
#define LOGIN_FAIL_ONLINE        2
#define LOGIN_FAIL_PASSWORD      3

struct _LoginInfo
{
	char m_szUserName[MAX_BUFF_50];
	char m_szUserPass[MAX_BUFF_50];
	int  m_nSendCount;
	int  m_nServerSuccess;
	int  m_nServerFail;

	_LoginInfo()
	{
		m_szUserName[0]  = '\0';
		m_szUserPass[0]  = '\0';
		m_nSendCount     = 0;
		m_nServerSuccess = 0;
		m_nServerFail    = 0;
	}
};

struct _ServerInfo
{
	char m_szServerIP[MAX_BUFF_50];
	int  m_nServerPort;

	_ServerInfo()
	{
		m_szServerIP[0] = '\0';
		m_nServerPort   = 0;
	}
};

struct _LoginClient
{
	char m_szUserName[MAX_BUFF_50];
	char m_szUserPass[MAX_BUFF_50];
	int  m_nUserIDFrom;
	int  m_nUserIDTo;

	_LoginClient()
	{
		m_szUserName[0] = '\0';
		m_szUserPass[0] = '\0';
		m_nUserIDFrom   = 0;
		m_nUserIDTo     = 0;
	}
};

typedef vector<_LoginInfo> vecLoginInfo;

// CLoginClientDlg 对话框
class CLoginClientDlg : public CDialog
{
// 构造
public:
	CLoginClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_LOGINCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	bool Send_Multiple_Login();

private:
	void Init();
	void Close();
	bool Connect();
	int  Random(int nStart, int nEnd);

	bool Send_Login(_LoginInfo& objLoginInfo);
	bool Send_Single_Login();
	
	bool Send_UserInfo(int nUserID);
	bool Send_SetUserInfo(int nUserID, int nLife, int nMagic);

	// 更新指定项测试数据
	void updateItemData(_LoginInfo& objLoginInfo, const int nItemIndex); 

private:
	vecLoginInfo m_vecLoginInfo;
	_ServerInfo  m_objServerInfo;
	_LoginClient m_objLoginClient; 
	SOCKET       m_sckClient;
	bool         m_blMultiple;

	int	m_nLastSecondSendCount;
	int          m_nSendCount;

public:
	CEdit m_txtServerIP;
	CEdit m_txtServerPort;
	CEdit m_txtUserName;
	CEdit m_txtUserPass;
	CEdit m_txtIDFrom;
	CEdit m_txtIDTo;
	CListCtrl m_lcServer;
	CEdit m_txtClientCost;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CEdit m_txtSeachUserID;
	CEdit m_txtUserID;
	CEdit m_txtUserLife;
	CEdit m_txtUserMagic;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
};
