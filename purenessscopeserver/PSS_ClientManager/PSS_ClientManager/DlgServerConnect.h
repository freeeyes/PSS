#pragma once
#include "afxcmn.h"
#include "ClientDefine.h"
#include "TcpClientConnect.h"

#include <vector>
#include "afxwin.h"

using namespace std;

typedef vector<_ClientConnectInfo> vecClientConnectInfo;

class CDlgServerConnect : public CDialog
{
    DECLARE_DYNAMIC(CDlgServerConnect)

public:
    CDlgServerConnect(CWnd* pParent = NULL);   // standard constructor
    virtual ~CDlgServerConnect();

    // Dialog Data
    enum { IDD = IDD_DIALOG_SERVERCONNECT };

public:
    CString GetPageTitle();
    void SetTcpClientConnect(CTcpClientConnect* pTcpClientConnect);

private:
    CTcpClientConnect* m_pTcpClientConnect;

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    DECLARE_MESSAGE_MAP()
public:
    CListCtrl m_lcServerConnect;
    CEdit m_txtServerID;
    CListCtrl m_lcServerListen;
    CIPAddressCtrl m_txtListenIP;
    CEdit m_txtListenPort;
    CComboBox m_cbListenType;

    virtual BOOL OnInitDialog();

    afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton8();
    afx_msg void OnBnClickedButton10();
    afx_msg void OnBnClickedButton13();
    CEdit m_txtPacketParseID;
};
