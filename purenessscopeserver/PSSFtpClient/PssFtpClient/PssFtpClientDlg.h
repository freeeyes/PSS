// PssFtpClientDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include <WinSock2.h>

#define MAX_BUFF_50    50
#define MAX_BUFF_500   500 
#define MAX_BUFF_10240 10240

#define COMMAND_LOGIN                  0x2000     //登陆
#define COMMAND_LOGOUT                 0x2001     //退出 
#define COMMAND_FILELIST               0x2002     //文件，文件夹列表
#define COMMAND_FILE_DOWNLOAD          0x2003     //文件下载
#define COMMAND_FILE_UPLOAD            0x2004     //文件上传
#define COMMAND_RETURN_LOGIN           0xe000     //登陆应答 
#define COMMAND_RETURN_LOGOUT          0xe001     //登出应答
#define COMMAND_RETURN_FILELIST        0xe002     //文件，文件夹列表应答
#define COMMAND_RETURN_FILE_DOWNLOAD   0xe003     //文件下载
#define COMMAND_RETURN_FILE_UPLOAD     0xe004     //文件上传

#define LOGIN_SUCCESS            0
#define LOGIN_FAIL_NOEXIST       1
#define LOGIN_FAIL_ONLINE        2
#define LOGIN_FAIL_PASSWORD      3

#define OP_OK                    0
#define OP_FAIL                  1
 
#define IS_FILE                  0
#define IS_DIRRNT                1

struct _ClientFTPInfo
{
	char szServerIP[MAX_BUFF_50];
	int  nServerPort;
	char szUserName[MAX_BUFF_50];
	char szUserPass[MAX_BUFF_50];

	_ClientFTPInfo()
	{
		szServerIP[0] = '\0';
		nServerPort   = 0;
		szUserName[0] = '\0';
		szUserPass[0] = '\0';
	}
};

struct _DownloadFileInfo
{
	char szLocalPath[MAX_BUFF_500];
	char szFileName[MAX_BUFF_500];
	char szRemotePath[MAX_BUFF_500];
	int  nSize;
};

// CPssFtpClientDlg 对话框
class CPssFtpClientDlg : public CDialog
{
// 构造
public:
	CPssFtpClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PSSFTPCLIENT_DIALOG };

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
	afx_msg void OnBnClickedButton1();
	CEdit m_txtServerIP;
	CEdit m_txtServerPort;
	CEdit m_txtUserName;
	CEdit m_txtUserPass;
	CListCtrl m_lcPath;
	CButton m_btnLogin;
	CButton m_btnLogout;
	CEdit m_txtLocalPath;
	CEdit m_txtRemotePath;
	CButton m_btnDownLoadFile;
	CEdit m_txtUpFileName;

private:
	void Init();
	void Close();
	bool Connect();

	bool Send_Login();
	bool Send_Logout();
	bool Send_FileList(const char* pRemotePath);
	bool Send_Download(const char* pLocalPath, const char* pFileName, const char* pRemotePath, int nIndex, int nSize, int& nBockCount);
	bool Send_Upload(const char* pLocalPath, const char* pFileName, const char* pRemotePath);

private:
	SOCKET         m_sckClient;
	_ClientFTPInfo m_ClientFTPInfo;

public:
	void DownLoadListFile();

public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
};
