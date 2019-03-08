#pragma once

//���������ѽ��������Ӷ���
//#include <winsock.h>
//#include <Windows.h>
#include <vector>
#include "sock_wrap.h"
#include "macro.h"

using namespace std;

#define MAX_BUFF_50    50
#define MAX_BUFF_1024  1024

#define FUNC_SUCCESS          0      //����ִ�гɹ�
#define FUNC_FAIL_EXIST       -1     //�����Ѿ�����
#define FUNC_FAIL_NO_EXIST    -2     //���Ӳ�����
#define FUNC_SEND_ERROR       -3     //��������ʧ��

#define INTVALID_SERVERID     -1     //��Ч��ID

#define TIMER_EVENT_ID        1      //��ʱ����ϢID
#define TIMER_EVENT_INTERVAL  2000   //��ʱ�����ʱ�䣨��λ���룩
#define TCP_RECV_TIMEOUT      5000   //�������ݳ�ʱʱ��
#define TCP_RECV_SLEEP        1000   //�������ݼ��ʱ��

//�������ݵ����ݸ�ʽ
class CRecvData
{
public:
	virtual void RecvData(char* pRecvData, int nLen) = 0;
};

//TCP���Ӷ���
struct _TpClientInfo
{
	int        m_nServerID;             //������ID
	char       m_szTpIP[MAX_BUFF_50];   //TCP��IP
	int        m_nPort;                 //TCP�Ķ˿�
	HSocket    m_sckClient;             //Socket
	int        m_nState;                //0Ϊδ���ӣ�1Ϊ������
	CRecvData* m_pRecvData;             //����������ݵĻص���

	_TpClientInfo()
	{
		m_nServerID = INTVALID_SERVERID;
		m_szTpIP[0] = '\0';
		m_nPort     = 0;
		m_sckClient = INVALID_SOCKET;
		m_nState    = 0;
		m_pRecvData = NULL;
	}
};

class CTpClientManager
{
public:
	~CTpClientManager(void);

	static CTpClientManager& Instance()
	{
		//��ʼ��TCP����
		//WSADATA wsaData;
		//int nErr = WSAStartup(MAKEWORD(2, 2), &wsaData);
		int nErr = InitializeSocketEnvironment();
		if(nErr != 0)
		{
			//MessageBox(_T("����socket�����ʧ�ܣ����鱾��socket��汾"), _T("������Ϣ"), MB_OK);
			printf_s("����socket�����ʧ�ܣ����鱾��socket��汾");
		}

		if(m_pTpClientManager == NULL)
		{
			m_pTpClientManager = new CTpClientManager();
		}

		return *m_pTpClientManager;
	}

	//����Զ�̷�����
	int TPClient_Connect(int nServerID, const char* pIP, int nPort, CRecvData* pRecvData = NULL);

	//�ر�Զ�̷���������
	int TPClient_DisConnect(int nServerID);

	//�ر���������
	int TPClient_DisConnect_All();

	//������ָ�����ӵ���������
	int TPClient_Send(int nServerID, const char* pBuff, int nLen);

	//���һ�����ӵ�ָ��״̬
	int TPClient_ConnectState(int nServerID);

	//�Զ�����ĳһ��ָ����ServerID
	int TPClient_ReConnect(int nServerID);

	//��ʱ���¼������ڴ���ʱ����
	int TPClient_HandleEvents(int nEventsID);

	//������ʱ��
	int TPClient_Run(bool blIsDebug = false);

	//�������н�������
	void Recv_All_Data();

	//�Զ���������δ���ӵĿͻ���
	void Connect_All_Server();

	//�Ƿ���ʾdebug���
	void SetDebug(bool blDebug);

private:
	CTpClientManager(void);
	typedef vector<_TpClientInfo> vecTpClientInfo;

	bool           IsFindTpClient(int nServerID);           //��ѯָ����ServerID�Ƿ����
	_TpClientInfo* IsGetTpClient(int nServerID);            //�ҵ�ָ����ServerID
	bool           DelTcpClient(int nServerID);             //ɾ��ָ����ServerID
	bool           DelTcpClient_All();                      //ɾ�����е�ServerID

	int            ConnectToServer(_TpClientInfo& objTpClientInfo);    //����Զ�̷�����

private:
	static CTpClientManager* m_pTpClientManager;
	vecTpClientInfo          m_vecTpClientInfo;
	bool                     m_blDebug;
};
