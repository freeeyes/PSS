
// PSS_ClientManagerDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "DlgClientMain.h"
#include "DlgClientConnect.h"
#include "DlgClientModule.h"
#include "DlgClientConnectM.h"
#include "DlgClientWorkThread.h"
#include "DlgServerConnect.h"
#include "DlgForbidenIP.h"
#include "DlgTrackIP.h"
#include "DlgWorkThreadAI.h"

// CPSS_ClientManagerDlg dialog
class CPSS_ClientManagerDlg : public CDialog
{
// Construction
public:
	CPSS_ClientManagerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PSS_CLIENTMANAGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
  CTabCtrl m_tabClientMain;

private:
  void InitView();

private:
  CDlgClientMain       m_DlgClientMain;
  CDlgClientConnect    m_DlgClientConnect;
  CDlgClientModule     m_DlgClientModule;
  CDlgClientConnectM   m_DlgClientConnectM;
  CDlgClientWorkThread m_DlgClientWorkThread;
  CDlgServerConnect    m_DlgServerConnect;
  CDlgForbidenIP       m_DlgForbidenIP;
  CDlgTrackIP          m_DlgTrackIP;
  CDlgWorkThreadAI     m_DlgWorkThreadAI;
  CTcpClientConnect    m_TcpClientConnect;

public:
  afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
