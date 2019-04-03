#pragma once

#include "ClientDefine.h"
#include "TcpClientConnect.h"
#include "afxwin.h"
#include "afxcmn.h"

#include <vector>

using namespace std;

struct _IPAccount
{
    char           m_szIP[50];   //当前链接地址
    int            m_nCount;              //当前链接次数
    int            m_nAllCount;           //指定IP链接次数总和
    int            m_nMinute;             //当前分钟数
    char           m_szDate[50]; //最后更新时间

    _IPAccount()
    {
        m_szIP[0]    = '\0';
        m_nCount     = 0;
        m_nAllCount  = 0;
        m_szDate[0]  = '\0';
    }
};

typedef vector<_IPAccount> vecIPAccount;

class CDlgClientConnect : public CDialog
{
    DECLARE_DYNAMIC(CDlgClientConnect)

public:
    CDlgClientConnect(CWnd* pParent = NULL);   // standard constructor
    virtual ~CDlgClientConnect();

    // Dialog Data
    enum { IDD = IDD_DIALOG_CLIENTCONNECT };

public:
    CString GetPageTitle();
    void SetTcpClientConnect(CTcpClientConnect* pTcpClientConnect);

private:
    CTcpClientConnect* m_pTcpClientConnect;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    CEdit m_txtClientConnectCount;
    CEdit m_txtCpu;
    CEdit m_txtMemory;
    CEdit m_txtDebugState;
    CButton m_btnDebug;
    CEdit m_txtFlowIn;
    CEdit m_txtFlowOut;
    CEdit m_txtMaxConnectCount;
    CListCtrl m_lcClientConnectHistory;
    CEdit m_txtPacketFileName;
    afx_msg void OnBnClickedButton1();
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedButton6();
    afx_msg void OnBnClickedButton5();
    afx_msg void OnBnClickedButton4();
    afx_msg void OnBnClickedButton12();
    afx_msg void OnBnClickedButton14();
    afx_msg void OnBnClickedButton15();
};
