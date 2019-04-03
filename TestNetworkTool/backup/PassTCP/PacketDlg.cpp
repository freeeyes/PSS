// CPacketDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PassTCP.h"
#include "PacketDlg.h"
#include "tinyxml.h"

// CPacketDlg 对话框

IMPLEMENT_DYNAMIC(CPacketDlg, CDialog)

CPacketDlg::CPacketDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPacketDlg::IDD, pParent)
{

}

CPacketDlg::~CPacketDlg()
{
}

void CPacketDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_txtPacketData);
	DDX_Control(pDX, IDC_LIST1, m_lbRondomPacketList);
	DDX_Control(pDX, IDC_LIST2, m_lbPreviewList);
	DDX_Control(pDX, IDC_EDIT2, m_txtRecvLength);
	DDX_Control(pDX, IDC_RADIO1, m_RadioBuffer);
}


BEGIN_MESSAGE_MAP(CPacketDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CPacketDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CPacketDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CPacketDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CPacketDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON2, &CPacketDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON6, &CPacketDlg::OnBnClickedButton6)
END_MESSAGE_MAP()

void CPacketDlg::SetBaseDataLogic(CBaseDataLogic* pBaseDataLogic)
{
	m_pBaseDataLogic = pBaseDataLogic;
	m_pBaseDataLogic->ClearRandomPacket();
	m_pBaseDataLogic->InitSendSize(MAX_RANDOM_PACKET);
}

// CPacketDlg 消息处理程序

void CPacketDlg::OnBnClickedButton1()
{
	//添加数据包
	CString strData;
	char szPacketData[MAX_RANDOM_PACKET] = {'\0'};

	if(m_pBaseDataLogic == NULL)
	{
		MessageBox(_T("必须指定m_pBaseDataLogic指针，指针不能为空。"));
		return;
	}

	m_txtPacketData.GetWindowText(strData);
	int nSrcLen = WideCharToMultiByte(CP_ACP, 0, strData, strData.GetLength(), NULL, 0, NULL, NULL);
	int nDecLen = WideCharToMultiByte(CP_ACP, 0, strData, nSrcLen, szPacketData, MAX_RANDOM_PACKET, NULL,NULL);
	szPacketData[nDecLen] = '\0';

	//获得接收字节数
	m_txtRecvLength.GetWindowText(strData);
	int nRecvLength = _ttoi(strData);

	if(nRecvLength <= 0)
	{
		MessageBox(_T("必须指定接收数据包的长度，否则无法发送。"));
		return;
	}

	if(((CButton *)GetDlgItem(IDC_RADIO1))->GetCheck() == 1)
	{
		//二进制模式2
		m_pBaseDataLogic->InsertRandomPacket(szPacketData, nDecLen, nRecvLength, 2);
	}
	else
	{
		//文本模式1
		string strFormatData = (string)szPacketData;

		//替换\r\n
		string_replace(strFormatData, "\\r", "\r");
		string_replace(strFormatData, "\\n", "\n");

		m_pBaseDataLogic->InsertRandomPacket(strFormatData.c_str(), strFormatData.length(), nRecvLength, 1);
	}

	ShowPacketList();
}

void CPacketDlg::ShowPacketList()
{
	//显示所有的数据包
	m_lbRondomPacketList.ResetContent();

	if(m_pBaseDataLogic == NULL)
	{
		return;
	}

	for(int i = 0; i < m_pBaseDataLogic->GetRandomPacketCount(); i++)
	{
		_RandomPacketInfo* pRandomPacketInfo = m_pBaseDataLogic->GettRandomPacket(i);
		if(NULL != pRandomPacketInfo)
		{
			wchar_t szTemp[MAX_RANDOM_PACKET] = {'\0'};

			int nSrcLen = MultiByteToWideChar(CP_ACP, 0, pRandomPacketInfo->szPacket, -1, NULL, 0);
			int nDecLen = MultiByteToWideChar(CP_ACP, 0, pRandomPacketInfo->szPacket, -1, szTemp, MAX_RANDOM_PACKET);
			szTemp[nDecLen] = '\0';

			m_lbRondomPacketList.AddString(szTemp);
		}
	}
}

void CPacketDlg::OnBnClickedButton3()
{
	//预览数据包
	m_lbPreviewList.ResetContent();
	int nRecvLength = 0;

	char szTemp[MAX_RANDOM_PACKET] = {'\0'};
	for(int i = 0; i < m_pBaseDataLogic->GetRandomPacketCount(); i++)
	{
		int nLen = MAX_RANDOM_PACKET;
		m_pBaseDataLogic->GetReview(i, szTemp, nLen, nRecvLength);
		if(0 < nLen)
		{
			wchar_t sszTemp[MAX_RANDOM_PACKET] = {'\0'};

			int nSrcLen = MultiByteToWideChar(CP_ACP, 0, szTemp, -1, NULL, 0);
			int nDecLen = MultiByteToWideChar(CP_ACP, 0, szTemp, -1, sszTemp, MAX_RANDOM_PACKET);
			sszTemp[nDecLen] = '\0';

			m_lbPreviewList.AddString(sszTemp);
		}
	}
}

void CPacketDlg::OnBnClickedButton4()
{
	//删除选中的条目
	int nIndex = m_lbRondomPacketList.GetCurSel();
	if(nIndex >= 0)
	{
		m_pBaseDataLogic->DeleteRandomPacket(nIndex);
	}
}

BOOL CPacketDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_RadioBuffer.SetCheck(BST_CHECKED);
	m_txtRecvLength.SetWindowText(_T("0"));

	return TRUE;
}
void CPacketDlg::OnBnClickedButton5()
{
	//关闭当前主窗口
	OnOK();
}

void CPacketDlg::OnBnClickedButton2()
{
	//保存成XML文件
	FILE* pFile = NULL;

	char szFileName[20];
	sprintf_s(szFileName, PACKETDATA_FILENAME);
	fopen_s(&pFile, szFileName, "w");

	if(pFile == NULL)
	{
		MessageBox(_T("导出顺序数据包出错，文件不存在"), _T("提示信息"), MB_OK);
		return;
	}

	char szLogText[MAX_RANDOM_PACKET] = {'\0'};

	sprintf_s(szLogText, MAX_RANDOM_PACKET, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

	sprintf_s(szLogText, MAX_RANDOM_PACKET, "<config>\n");
	fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

	for(int i = 0; i < m_pBaseDataLogic->GetRandomPacketCount(); i++)
	{
		_RandomPacketInfo* pRandomPacketInfo = m_pBaseDataLogic->GettRandomPacket(i);
		sprintf_s(szLogText, MAX_RANDOM_PACKET, "<PacketData Len=\"%d\" Type=\"%d\" RecdvLength=\"%d\"><![CDATA[%s]]></PacketData>\n"
			,pRandomPacketInfo->nLen 
			,pRandomPacketInfo->nType
			,pRandomPacketInfo->nRecdvLength
			,pRandomPacketInfo->szPacket);

		fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);
	}

	sprintf_s(szLogText, MAX_RANDOM_PACKET, "</config>\n");
	fwrite(szLogText, strlen(szLogText), sizeof(char), pFile);

	fclose(pFile);

	MessageBox(_T("导出顺序数据包成功"), _T("提示信息"), MB_OK);
}

void CPacketDlg::OnBnClickedButton6()
{
	m_pBaseDataLogic->ClearRandomPacket();

	//读取XML文件
	TiXmlDocument * pDocument = new TiXmlDocument(PACKETDATA_FILENAME);//tstl可以为文件路径或文件名
	pDocument->LoadFile();

	if(NULL == pDocument)
	{
		MessageBox(_T("读取顺序数据包xml文件失败"), _T("提示信息"), MB_OK);
		return;
	}

	TiXmlElement *Root = pDocument->RootElement();//获取根节点<Particls>
	TiXmlElement *Particl = NULL;

	int nLen        = 0;
	int nType       = 0;
	int nRecvLength = 0;

	for(Particl = Root->FirstChildElement();Particl != NULL;Particl = Particl->NextSiblingElement())
	{
		//得到文档内容
		const char *sztext = Particl->GetText();

		//得到文档属性
		TiXmlAttribute* pAddrAttr = Particl->FirstAttribute();
		if(strcmp(pAddrAttr->Name(), "Len") == 0)
		{
			nLen = atoi(pAddrAttr->Value());
		}
		else if(strcmp(pAddrAttr->Name(), "Type") == 0)
		{
			nType = atoi(pAddrAttr->Value());
		}
		else
		{
			nRecvLength = atoi(pAddrAttr->Value());
		}

		TiXmlAttribute *pAttr1 = pAddrAttr->Next();
		if(strcmp(pAttr1->Name(), "Len") == 0)
		{
			nLen = atoi(pAttr1->Value());
		}
		else if(strcmp(pAttr1->Name(), "Type") == 0)
		{
			nType = atoi(pAttr1->Value());
		}
		else
		{
			nRecvLength = atoi(pAttr1->Value());
		}

		TiXmlAttribute * pAttr2 = pAttr1->Next();
		if(strcmp(pAttr2->Name(), "Len") == 0)
		{
			nLen = atoi(pAttr2->Value());
		}
		else if(strcmp(pAttr2->Name(), "Type") == 0)
		{
			nType = atoi(pAttr2->Value());
		}
		else
		{
			nRecvLength = atoi(pAttr2->Value());
		}

		//添加类型
		m_pBaseDataLogic->InsertRandomPacket(sztext, nLen, nRecvLength, nType);
	}

	ShowPacketList();

	delete pDocument;
}
