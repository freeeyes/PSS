
// PassTCPDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "ClientTcpSocket.h"
#include "ClientUdpSocket.h"
#include "afxwin.h"
#include <afxpriv.h>

#include "PacketDlg.h"

using namespace std;

typedef vector<CClientTcpSocket* > vecClientTcpSocket;
typedef vector<CClientUdpSocket* > vecClientUdpSocket;

#define COLOR_TEXT_BULE 1
#define COLOR_TEXT_RED  2

// CPassTCPDlg 对话框
class CPassTCPDlg : public CDialog
{
// 构造
public:
	CPassTCPDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PASSTCP_DIALOG };

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

private:
  void InitView();
  void ClearResult();
  void Close(bool blClose = false);

  void SetRichTextColor(int nColor);

private:
  CTime m_tmBegin;             //压测开始时间
  bool  m_blIsRun;             //压测是否在进行
  CTime m_tmEnd;               //压测停止时间
  int   m_nCurrTextStyle;      //当前文本风格
  CBaseDataLogic* m_pLogic;    //是否设置了最新的逻辑包

public:
  CEdit m_txtServerIP;
  CEdit m_txtPort;
  CEdit m_txtThreadCount;
  CEdit m_txtRecvTimeout;
  CEdit m_txtSocketInterval;
  CEdit m_txtRecvLength;
  CButton m_chkIsAlwayConnect;
  CButton m_chkRadomaDelay;
  CButton m_chkIsRecv;
  CButton m_ChkIsBroken;
  CButton m_chkRadomSendCount;
  CButton m_chkIsWriteLog;
  CButton m_chkSendOne;
  CButton m_chkLuaAdvance;
  CEdit m_txtSuccessConnect;
  CEdit m_txtSuccessSend;
  CEdit m_txtSuccessRecv;
  CEdit m_txtCurrConnect;
  CEdit m_txtFailConnect;
  CEdit m_txtFailSend;
  CEdit m_txtFailRecv;
  CEdit m_txtClientUdpPort;
  CEdit m_txtPacketTimewait;
  CEdit m_txtSendByteCount;
  CEdit m_txtRecvByteCount;
  CEdit m_txtLuaFilePath;
  CEdit m_txtSendCount;
  CEdit m_txtMinTime;
  CEdit m_txtMaxTime;
  CComboBox m_cbSendBuffStyle;
  CRichEditCtrl m_reSendText;
  int m_nRadio;
  
  vecClientTcpSocket m_vecClientTcpSocket;
  vecClientUdpSocket m_vecClientUdpSocket;
  afx_msg void OnClose();
  afx_msg void OnBnClickedButton2();
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg void OnBnClickedButton3();
  afx_msg void OnCbnSelchangeCombo1();
  afx_msg void OnBnClickedButton1();
  afx_msg void OnBnClickedButton4();

  afx_msg void OnBnClickedButton5();
};
