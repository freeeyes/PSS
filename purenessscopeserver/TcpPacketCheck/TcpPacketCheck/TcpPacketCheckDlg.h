// TcpPacketCheckDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#define MAX_BUFF_200   200

#define COMMAND_AUTOTEST_HEAD         0x5000   //带头返回数据包
#define COMMAND_AUTOTEST_NOHEAD       0x5001   //不带头数据包
#define COMMAND_AUTOTEST_HEADBUFF     0x5002   //带头缓冲数据包
#define COMMAND_AUTOTEST_NOHEADBUFF   0x5003   //不带头缓冲数据包

#define COMMAND_AUTOTEST_RETUEN_HEAD         0xf000   
#define COMMAND_AUTOTEST_RETUEN_NOHEAD       0xf001
#define COMMAND_AUTOTEST_RETUEN_HEADBUFF     0xf002
#define COMMAND_AUTOTEST_RETUEN_NOHEADBUFF   0xf003

class CConvertBuffer
{
public:
	CConvertBuffer() {};
	~CConvertBuffer() {};
	int GetBufferSize(const char* pData, int nSrcLen)
	{
		char szData[3] = {'\0'};
		int nPos         = 0;
		int nCurrSize    = 0;
		int nConvertSize = 0;
		bool blState     = false;   //转换后的字符串是否有效
		bool blSrcState  = true;    //元字符串是否有效
		unsigned char cData;

		while(nPos < nSrcLen)
		{
			if(pData[nPos] == '\r' || pData[nPos] == '\n' || pData[nPos] == ' ' || nPos == nSrcLen - 1)
			{
				if(nPos == nSrcLen - 1)
				{
					szData[nCurrSize++] = pData[nPos];
				}

				szData[nCurrSize] = '\0';
				if(blSrcState == true)
				{
					blState = ConvertStr2char(szData, cData);
					if(blState == true)
					{
						nConvertSize++;
					}
				}
				nCurrSize  = 0;
				blSrcState = true;
				nPos++;
			}
			else
			{
				if(nCurrSize < 2)
				{
					szData[nCurrSize++] = pData[nPos];
				}
				else
				{
					blSrcState = false;
				}
				nPos++;
			}
		}

		return nConvertSize;
	};

	bool Convertstr2charArray(const char* pData, int nSrcLen, unsigned char* pDes, int& nMaxLen)
	{
		char szData[3] = {'\0'};
		int nPos         = 0;
		int nCurrSize    = 0;
		int nConvertSize = 0;
		bool blState     = false;   //转换后的字符串是否有效
		bool blSrcState  = true;    //元字符串是否有效

		while(nPos < nSrcLen)
		{
			if(pData[nPos] == '\r' || pData[nPos] == '\n' || pData[nPos] == ' ' || nPos == nSrcLen - 1)
			{
				if(nPos == nSrcLen - 1)
				{
					szData[nCurrSize++] = pData[nPos];
				}

				szData[nCurrSize] = '\0';
				if(nConvertSize < nMaxLen && blSrcState == true)
				{
					blState = ConvertStr2char(szData, pDes[nConvertSize]);
					if(blState == true)
					{
						nConvertSize++;
					}
				}
				nCurrSize  = 0;
				blSrcState = true;
				nPos++;
			}
			else
			{
				if(nCurrSize < 2)
				{
					szData[nCurrSize++] = pData[nPos];
				}
				else
				{
					blSrcState = false;
				}
				nPos++;
			}
		}

		nMaxLen = nConvertSize;
		return true;
	};
private:
	bool Get_binary_Char(unsigned char cTag, unsigned char& cDes)
	{
		if(cTag >='A'&&  cTag <='F')
		{
			cDes = cTag - 'A' + 10;
			return true;
		}
		else if(cTag >='a'&&  cTag <='f')
		{
			cDes = cTag - 'a' + 10;
			return true; 
		}
		else if(cTag >= '0'&& cTag<= '9')
		{
			cDes = cTag-'0';
			return true;
		}
		else
		{
			return false;
		}
	}

	bool ConvertStr2char(const char* pData, unsigned char& cData)
	{
		if(pData == NULL || strlen(pData) != 2)
		{
			return false;
		}

		char cFirst = pData[1];
		unsigned char cTemp = 0;
		bool blStste = Get_binary_Char(cFirst, cTemp);
		if(false == blStste)
		{
			return false;
		}
		cData = cTemp;
		char cSecond = pData[0];
		blStste  = Get_binary_Char(cSecond, cTemp);
		if(false == blStste)
		{
			return false;
		}
		cTemp = cTemp << 4;
		cData = cData | cTemp;

		return true;
	}
};

struct _ClientInfo
{
	char  m_szServerIP[30];
	int   m_nPort; 
	char* m_pSendBuffer;
	int   m_nSendLength;
	int   m_nRecvLength;

	_ClientInfo()
	{
		m_szServerIP[0] = '\0';
		m_nPort         = 0;
		m_pSendBuffer   = NULL;
		m_nSendLength   = 0;
		m_nRecvLength   = 0;
	}
};

// CTcpPacketCheckDlg 对话框
class CTcpPacketCheckDlg : public CDialog
{
// 构造
public:
	CTcpPacketCheckDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TCPPACKETCHECK_DIALOG };

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
	bool CheckTcpPacket(_ClientInfo& objClientInfo, int nIndex);
	bool CheckMultipleTcpPacket(_ClientInfo& objClientInfo, int nIndex);
	bool CheckValidPacket(_ClientInfo& objClientInfo, int nIndex);
	bool CheckHalfPacket(_ClientInfo& objClientInfo, int nIndex);
	bool CheckIsHead(_ClientInfo& objClientInfo, int nIndex);
	bool CheckIsNoHead(_ClientInfo& objClientInfo, int nIndex);
	bool CheckIsHeadBuffer(_ClientInfo& objClientInfo, int nIndex);
	bool CheckIsNoHeadBuffer(_ClientInfo& objClientInfo, int nIndex);


private:
	CEdit m_txtPacketBuffer;
	CEdit m_txtServerIP;
	CEdit m_txtPort;
	CEdit m_txtRecvLength;
	CListCtrl m_lstResult;
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
};
