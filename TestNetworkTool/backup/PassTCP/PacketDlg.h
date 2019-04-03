#pragma once
#include "afxwin.h"
#include "define.h"

// PacketDlg 对话框
#define PACKETDATA_FILENAME "PacketData.xml"

class CPacketDlg : public CDialog
{
	DECLARE_DYNAMIC(CPacketDlg)

public:
	CPacketDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPacketDlg();

// 对话框数据
	enum { IDD = IDD_PACKET_DIALOG };
	
public:
	void SetBaseDataLogic(CBaseDataLogic* pBaseDataLogic);

private:
	void ShowPacketList();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_txtPacketData;
	CListBox m_lbRondomPacketList;
	CListBox m_lbPreviewList;
	CEdit m_txtRecvLength;

private:
	CBaseDataLogic* m_pBaseDataLogic;

public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	CButton m_RadioBuffer;
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton6();
};
