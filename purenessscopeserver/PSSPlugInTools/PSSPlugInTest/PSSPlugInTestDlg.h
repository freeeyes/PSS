
// PSSPlugInTestDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "PlugInOperation.h"
#include "XmlOpeation.h"

typedef vector<CPlugInOperation* > vecPlugInOperationGroup;

static int g_nCommandColNumber = 0;

// CPSSPlugInTestDlg dialog
class CPSSPlugInTestDlg : public CDialog
{
// Construction
public:
	CPSSPlugInTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PSSPLUGINTEST_DIALOG };

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

private:
  bool InitView();
  void ClearPlugInOperation();

public:
  CEdit m_txtPlugInName;
  CEdit m_txtSendCount;
  CEdit m_txtRecvCount;
  CEdit m_txtThreadCount;
  CListCtrl m_lstCommand;
  CListCtrl m_lstPlugInInfo;
  CEdit m_txtXMLFileName;
  CEdit m_txtHelp;
  afx_msg void OnBnClickedButton1();
  afx_msg void OnBnClickedButton5();  //读取XML文件
  afx_msg void OnBnClickedButton4();  //写入XML文件 
  afx_msg void OnClose();

private:
  CXmlOpeation            m_objXmlOpeation;
  CCommandInfo            m_objCommandInfo;
  vecPlugInOperationGroup m_objvecPlugInOperationGroup; 

public:
  afx_msg void OnBnClickedButton2();
  afx_msg void OnBnClickedButton3();
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg void OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult);
};
