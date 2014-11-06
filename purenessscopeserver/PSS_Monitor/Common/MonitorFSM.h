#include "define.h"
#include "ClientMessage.h"
#include "LogFile.h"
#include "MailManager.h"

//这个类用于实现监控各种指定的PSS数据的状态机
//根据收到的数据，进行分析并整合，并计划下一步

enum EM_EVENT
{
	EM_EVENT_COMMAND_MONITORINFO = 0,
	EM_EVENT_COMMAND_FINISH,
};

#define LOG_ROOT "./"

class CClientMessage : public IClientMessage
{
public:
	CClientMessage() 
	{ 
		m_nServerID       = 0;
		m_szServerName[0] = '\0';
	};

	virtual ~CClientMessage() {};

	void Init(_MailAlert* pMailAlert)
	{
		m_objMailManager.SetMailAlert(pMailAlert);
	}

	void SetServerName(const char* pServerName)
	{
		sprintf_safe(m_szServerName, MAX_BUFF_50, "%s", pServerName);

		m_objLogFile.SetRoot(LOG_ROOT);
		m_objLogFile.SetLogName(m_szServerName);
	}

	void ReConnect(int nServerID)
	{
		OUR_DEBUG((LM_INFO, "[ReConnect]nServerID=%d.\n", nServerID));
		m_nServerID = nServerID;
	}

	bool RecvData(ACE_Message_Block* mbRecv, _ClientIPInfo objServerIPInfo)
	{
		OUR_DEBUG((LM_INFO, "[RecvData]nServerID=%d, size=%d.\n", m_nServerID, mbRecv->length()));

		//把收到的数据变换成日志
		uint32 u4ActiveClient;
		uint32 u4PoolClient;
		uint16 u2MaxHandlerCount;
		uint16 u2CurrConnect;
		uint32 u4FlowIn;
		uint32 u4FlowOut;

		char* pData = mbRecv->rd_ptr();
		int nPos = 0;
		ACE_OS::memcpy(&u4ActiveClient, &pData[nPos], sizeof(uint32));
		nPos += sizeof(uint32);
		ACE_OS::memcpy(&u4PoolClient, &pData[nPos], sizeof(uint32));
		nPos += sizeof(uint32);
		ACE_OS::memcpy(&u2MaxHandlerCount, &pData[nPos], sizeof(uint16));
		nPos += sizeof(uint16);
		ACE_OS::memcpy(&u2CurrConnect, &pData[nPos], sizeof(uint16));
		nPos += sizeof(uint16);
		ACE_OS::memcpy(&u4FlowIn, &pData[nPos], sizeof(uint32));
		nPos += sizeof(uint32);
		ACE_OS::memcpy(&u4FlowOut, &pData[nPos], sizeof(uint32));
		nPos += sizeof(uint32);

		//记录当前时间
		char szDate[MAX_BUFF_50] = {'\0'};
		ACE_Date_Time dt;
		dt.update(ACE_OS::gettimeofday());

		sprintf_safe(szDate, MAX_BUFF_50, "%04d-%02d-%02d %02d:%02d:%02d", 
			dt.year(), 
			dt.month(),
			dt.day(), 
			dt.hour(),
			dt.minute(),
			dt.second());

		m_objLogFile.DoLog("<Monitor Time=\"%s\" State=\"Connected\" ActiveClient=\"%d\" PoolClient=\"%d\" MaxHandlerCount=\"%d\" FlowIn=\"%d\" FlowOut=\"%d\" />\n", 
			szDate,
			u4ActiveClient, 
			u4PoolClient, 
			u2MaxHandlerCount, 
			u4FlowIn, 
			u4FlowOut);

		return true;
	}

	bool ConnectError(int nError, _ClientIPInfo objServerIPInfo)
	{
		OUR_DEBUG((LM_INFO, "[ConnectError]nServerID=%d, errno=%d.\n", m_nServerID, nError));

		//记录当前时间
		char szDate[MAX_BUFF_50] = {'\0'};
		ACE_Date_Time dt;
		dt.update(ACE_OS::gettimeofday());

		sprintf_safe(szDate, MAX_BUFF_50, "%04d-%02d-%02d %02d:%02d:%02d", 
			dt.year(), 
			dt.month(),
			dt.day(), 
			dt.hour(),
			dt.minute(),
			dt.second());

		m_objLogFile.DoLog("<Monitor Time=\"%s\" State=\"UnConnected\" ActiveClient=\"0\" PoolClient=\"0\" MaxHandlerCount=\"0\" FlowIn=\"0\" FlowOut=\"0\" />\n", 
			szDate);

		char szTitle[MAX_BUFF_50] = {'\0'};
		char szBody[MAX_BUFF_100] = {'\0'};
		sprintf_safe(szTitle, MAX_BUFF_50, "ServerMonitor disconnect");
		sprintf_safe(szBody, MAX_BUFF_100, "[ConnectError]nServerName=%s, errno=%d.", m_szServerName, nError);

		m_objMailManager.SendMail(szTitle, szBody);

		return true;
	}

	int GetServerID()
	{
		return m_nServerID;
	}

private:
	ACE_Message_Block* m_mbRecv;
	int                m_nServerID;
	char               m_szServerName[MAX_BUFF_50];
	CLogFile           m_objLogFile;
	CMailManager       m_objMailManager; 
};

class MonitorFSM
{
public:
	MonitorFSM();
	~MonitorFSM();

	IClientMessage* GetClientMessage(); 

	int GetServerID();
	char* GetKey();

	void Init(_ServerInfo* pServerInfo, _MailAlert* pMailAlert);

private:
	CClientMessage* m_pClientMessage;
	_ServerInfo*    m_pServerInfo;
};