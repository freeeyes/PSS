// DlgTrackIP.cpp : 实现文件
//

#include "stdafx.h"
#include "PSS_ClientManager.h"
#include "DlgTrackIP.h"


// CDlgTrackIP 对话框

IMPLEMENT_DYNAMIC(CDlgTrackIP, CDialog)

CDlgTrackIP::CDlgTrackIP(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTrackIP::IDD, pParent)
{

}

CDlgTrackIP::~CDlgTrackIP()
{
}

void CDlgTrackIP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_txtTrackIP);
	DDX_Control(pDX, IDC_LIST1, m_lcTrackIPHistory);
	DDX_Control(pDX, IDC_EDIT2, m_txtConnectID);
	DDX_Control(pDX, IDC_EDIT7, m_txtIPInfo);
}


BEGIN_MESSAGE_MAP(CDlgTrackIP, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlgTrackIP::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlgTrackIP::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON8, &CDlgTrackIP::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON6, &CDlgTrackIP::OnBnClickedButton6)
END_MESSAGE_MAP()

CString CDlgTrackIP::GetPageTitle()
{
	return _T("IP追踪器");
}

void CDlgTrackIP::SetTcpClientConnect( CTcpClientConnect* pTcpClientConnect )
{
	m_pTcpClientConnect = pTcpClientConnect;
}

// CDlgTrackIP 消息处理程序

BOOL CDlgTrackIP::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_lcTrackIPHistory.InsertColumn(0, _T("IP地址"), LVCFMT_CENTER, 70);
	m_lcTrackIPHistory.InsertColumn(1, _T("端口"), LVCFMT_CENTER, 50);
	m_lcTrackIPHistory.InsertColumn(2, _T("类型"), LVCFMT_CENTER, 50);
	m_lcTrackIPHistory.InsertColumn(3, _T("建立时间"), LVCFMT_CENTER, 150);
	m_lcTrackIPHistory.InsertColumn(4, _T("断开时间"), LVCFMT_CENTER, 150);
	m_lcTrackIPHistory.InsertColumn(5, _T("接收字节"), LVCFMT_CENTER, 70);
	m_lcTrackIPHistory.InsertColumn(6, _T("发送字节"), LVCFMT_CENTER, 70);

	DWORD dwStyle = m_lcTrackIPHistory.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	m_lcTrackIPHistory.SetExtendedStyle(dwStyle); //设置扩展风格

	return TRUE;
}

void CDlgTrackIP::OnBnClickedButton1()
{
	//设置IP
	char szIP[20]      = {'\0'};
	CString strIP;

	m_txtTrackIP.GetWindowText(strIP);

	int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strIP, strIP.GetLength(), NULL, 0, NULL, NULL);
	int nDecLen = WideCharToMultiByte(CP_ACP, 0, strIP, nSrcLen, szIP, 20, NULL,NULL);
	szIP[nDecLen] = '\0';

	if(strlen(szIP) == 0)
	{
		MessageBox(_T(MESSAGE_INSERT_NULL) , _T(MESSAGE_TITLE_ERROR), MB_OK);
		return;
	}

	char szSendMessage[200] = {'\0'};
	char szCommand[100]     = {'\0'};
	sprintf_s(szCommand, 100, "%s SetTrackIP -c %s ", m_pTcpClientConnect->GetKey(), szIP);
	int nSendLen = (int)strlen(szCommand); 

	memcpy_s(szSendMessage, 200, &nSendLen, sizeof(int));
	memcpy_s(&szSendMessage[4], 200, &szCommand, nSendLen);

	char szRecvBuff[10 * 1024] = {'\0'};
	int nRecvLen = 10 * 1024;
	bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen + sizeof(int), (char*)szRecvBuff, nRecvLen);
	if(blState == false)
	{
		MessageBox(_T(MESSAGE_SENDERROR) , _T(MESSAGE_TITLE_ERROR), MB_OK);
	}
	else
	{
		int nStrLen      = 0;
		int nPos         = 4;
		int nResult      = 0;
		memcpy_s(&nResult, sizeof(char), &szRecvBuff[nPos], sizeof(char));
		nPos += sizeof(char);

		if(nResult == 0)
		{
			MessageBox(_T(MESSAGE_RESULT_SUCCESS) , _T(MESSAGE_TITLE_ERROR), MB_OK);
		}
		else
		{
			MessageBox(_T(MESSAGE_RESULT_FAIL) , _T(MESSAGE_TITLE_SUCCESS), MB_OK);
		}
	}

}

void CDlgTrackIP::OnBnClickedButton2()
{
	//取消跟踪IP
	char szSendMessage[200] = {'\0'};
	char szCommand[100]     = {'\0'};
	sprintf_s(szCommand, 100, "%s DelTrackIP -a", m_pTcpClientConnect->GetKey());
	int nSendLen = (int)strlen(szCommand); 

	memcpy_s(szSendMessage, 200, &nSendLen, sizeof(int));
	memcpy_s(&szSendMessage[4], 200, &szCommand, nSendLen);

	char szRecvBuff[10 * 1024] = {'\0'};
	int nRecvLen = 10 * 1024;
	bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen + sizeof(int), (char*)szRecvBuff, nRecvLen);
	if(blState == false)
	{
		MessageBox(_T(MESSAGE_SENDERROR) , _T(MESSAGE_TITLE_ERROR), MB_OK);
	}
	else
	{
		int nStrLen      = 0;
		int nPos         = 4;
		int nResult      = 0;
		memcpy_s(&nResult, sizeof(char), &szRecvBuff[nPos], sizeof(char));
		nPos += sizeof(char);

		if(nResult == 0)
		{
			MessageBox(_T(MESSAGE_RESULT_SUCCESS) , _T(MESSAGE_TITLE_ERROR), MB_OK);
		}
		else
		{
			MessageBox(_T(MESSAGE_RESULT_FAIL) , _T(MESSAGE_TITLE_SUCCESS), MB_OK);
		}
	}
}

void CDlgTrackIP::OnBnClickedButton8()
{
	//跟踪历史记录
	m_lcTrackIPHistory.DeleteAllItems();

	char szSendMessage[200] = {'\0'};
	char szCommand[100]     = {'\0'};
	sprintf_s(szCommand, 100, "%s GetTrackIPInfo -a", m_pTcpClientConnect->GetKey());
	int nSendLen = (int)strlen(szCommand); 

	memcpy_s(szSendMessage, 200, &nSendLen, sizeof(int));
	memcpy_s(&szSendMessage[4], 200, &szCommand, nSendLen);

	char szRecvBuff[10 * 1024] = {'\0'};
	int nRecvLen = 10 * 1024;
	bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen + sizeof(int), (char*)szRecvBuff, nRecvLen);
	if(blState == false)
	{
		MessageBox(_T(MESSAGE_SENDERROR) , _T(MESSAGE_TITLE_ERROR), MB_OK);
	}
	else
	{
		int nStrLen      = 0;
		int nPos         = 4;
		int nCount       = 0;
		memcpy_s(&nCount, sizeof(short), &szRecvBuff[nPos], sizeof(short));
		nPos += sizeof(short);

		for(int i = 0; i < nCount; i++)
		{
			//开始还原数据结构
			char szIP[20]         = {'\0'};
			int  nPort            = 0;
			char szBeginTime[100] = {'\0'};
			char szEndTime[100]   = {'\0'};
			int  nRecvByteSize    = 0;
			int  nSendByteSize    = 0;
			int  nState           = 0;

			memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
			nPos += sizeof(char);

			//获得IP地址
			memcpy_s(szIP, nStrLen, &szRecvBuff[nPos], nStrLen);
			nPos += nStrLen;
			szIP[nStrLen] = '\0';

			//得到端口
			memcpy_s(&nPort, sizeof(int), &szRecvBuff[nPos], sizeof(int));
			nPos += sizeof(int);

			//得到开始时间
			memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
			nPos += sizeof(char);

			memcpy_s(szBeginTime, nStrLen, &szRecvBuff[nPos], nStrLen);
			nPos += nStrLen;
			szBeginTime[nStrLen] = '\0';

			//得到结束时间
			memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
			nPos += sizeof(char);

			memcpy_s(szEndTime, nStrLen, &szRecvBuff[nPos], nStrLen);
			nPos += nStrLen;
			szEndTime[nStrLen] = '\0';

			//得到接收字节数
			memcpy_s(&nRecvByteSize, sizeof(int), &szRecvBuff[nPos], sizeof(int));
			nPos += sizeof(int);

			//得到发送字节数
			memcpy_s(&nSendByteSize, sizeof(int), &szRecvBuff[nPos], sizeof(int));
			nPos += sizeof(int);

			//得到状态位
			memcpy_s(&nState, sizeof(char), &szRecvBuff[nPos], sizeof(char));
			nPos += sizeof(char);

			//显示在桌面上
			wchar_t swzIP[200]         = {'\0'};
			wchar_t swzzBeginTime[100] = {'\0'};
			wchar_t swzzEndTime[100]   = {'\0'};

			int nSrcLen = MultiByteToWideChar(CP_ACP, 0, szIP, -1, NULL, 0);
			int nDecLen = MultiByteToWideChar(CP_ACP, 0, szIP, -1, swzIP, 200);

			nSrcLen = MultiByteToWideChar(CP_ACP, 0, szBeginTime, -1, NULL, 0);
			nDecLen = MultiByteToWideChar(CP_ACP, 0, szBeginTime, -1, swzzBeginTime, 100);

			nSrcLen = MultiByteToWideChar(CP_ACP, 0, szEndTime, -1, NULL, 0);
			nDecLen = MultiByteToWideChar(CP_ACP, 0, szEndTime, -1, swzzBeginTime, 100);

			CString strData;

			m_lcTrackIPHistory.InsertItem(i, swzIP);
			strData.Format(_T("%d"), nPort);
			m_lcTrackIPHistory.SetItemText(i, 1, strData);
			if(nState == 1)
			{
				m_lcTrackIPHistory.SetItemText(i, 2, _T("关闭"));
			}
			else
			{

				m_lcTrackIPHistory.SetItemText(i, 2, _T("尚未关闭"));
			}

			m_lcTrackIPHistory.SetItemText(i, 3, swzzBeginTime);
			if(nState == 1)
			{
				m_lcTrackIPHistory.SetItemText(i, 4, swzzBeginTime);
			}
			else
			{

				m_lcTrackIPHistory.SetItemText(i, 4, _T("尚未关闭"));
			}
			strData.Format(_T("%d"), nRecvByteSize);
			m_lcTrackIPHistory.SetItemText(i, 5, strData);
			strData.Format(_T("%d"), nSendByteSize);
			m_lcTrackIPHistory.SetItemText(i, 6, strData);
		}
	}
}

void CDlgTrackIP::OnBnClickedButton6()
{
	//根据ConnectID获得相关的数据

	//得到ConnectID
	int nConnectID = 0;
	CString strData;

	m_txtConnectID.GetWindowText(strData);

	nConnectID = _ttoi(strData);

	char szSendMessage[200] = {'\0'};
	char szCommand[100]     = {'\0'};
	sprintf_s(szCommand, 100, "%s GetConnectIPInfo -s %d", m_pTcpClientConnect->GetKey(), nConnectID);
	int nSendLen = (int)strlen(szCommand); 

	memcpy_s(szSendMessage, 200, &nSendLen, sizeof(int));
	memcpy_s(&szSendMessage[4], 200, &szCommand, nSendLen);

	char szRecvBuff[10 * 1024] = {'\0'};
	int nRecvLen = 10 * 1024;
	bool blState = m_pTcpClientConnect->SendConsoleMessage(szSendMessage, nSendLen + sizeof(int), (char*)szRecvBuff, nRecvLen);
	if(blState == false)
	{
		MessageBox(_T(MESSAGE_SENDERROR) , _T(MESSAGE_TITLE_ERROR), MB_OK);
	}
	else
	{
		int nStrLen      = 0;
		int nPos         = 4;
		int nResult      = 0;
		memcpy_s(&nResult, sizeof(short), &szRecvBuff[nPos], sizeof(short));
		nPos += sizeof(short);

		if(nResult == 1)
		{
			m_txtIPInfo.SetWindowText(_T(""));
			MessageBox(_T("当前ConnectID无连接信息。") , _T(MESSAGE_TITLE_ERROR), MB_OK);
		}
		else
		{
			char szIP[20]         = {'\0'};
			int  nPort            = 0;

			memcpy_s(&nStrLen, sizeof(char), &szRecvBuff[nPos], sizeof(char));
			nPos += sizeof(char);

			//获得IP地址
			memcpy_s(szIP, nStrLen, &szRecvBuff[nPos], nStrLen);
			nPos += nStrLen;
			szIP[nStrLen] = '\0';

			//得到端口
			memcpy_s(&nPort, sizeof(int), &szRecvBuff[nPos], sizeof(int));
			nPos += sizeof(int);

			char szData[100] ={'\0'};
			sprintf_s(szData, 100, "%s:%d", szIP, nPort);

			wchar_t swzIP[200]         = {'\0'};

			int nSrcLen = MultiByteToWideChar(CP_ACP, 0, szData, -1, NULL, 0);
			int nDecLen = MultiByteToWideChar(CP_ACP, 0, szData, -1, swzIP, 200);

			m_txtIPInfo.SetWindowText(swzIP);
		}

	}
}
