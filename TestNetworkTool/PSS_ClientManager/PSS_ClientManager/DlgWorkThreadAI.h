#pragma once

#include "ClientDefine.h"
#include "TcpClientConnect.h"
#include "afxcmn.h"
#include "afxwin.h"

// CDlgWorkThreadAI 对话框

class CDlgWorkThreadAI : public CDialog
{
	DECLARE_DYNAMIC(CDlgWorkThreadAI)

public:
	CDlgWorkThreadAI(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgWorkThreadAI();

public:
	CString GetPageTitle();
	void SetTcpClientConnect(CTcpClientConnect* pTcpClientConnect);

// 对话框数据
	enum { IDD = IDD_DIALOG_WORKTHREADAI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CTcpClientConnect* m_pTcpClientConnect;

public:
	CListCtrl m_lcWorkThreadAI;
	CListCtrl m_lcTimeout;
	CListCtrl m_lcForbiden;
	CEdit m_txtDisposeTime;
	CEdit m_txtCheckTime;
	CEdit m_txtStopTime;
	CButton m_btnAI;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
