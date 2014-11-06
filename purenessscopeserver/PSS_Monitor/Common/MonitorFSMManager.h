#include "ace/Task_T.h"
#include "MonitorFSM.h"

typedef vector<MonitorFSM*> vecMonitorFSM;

class CMonitorFSMManager : public ACE_Task<ACE_MT_SYNCH>
{
public:
	CMonitorFSMManager();
	~CMonitorFSMManager();

	virtual int handle_timeout(const ACE_Time_Value& tv, const void* arg);   //¶¨Ê±Æ÷¼ì²é

	MonitorFSM* Create(_ServerInfo* pServerInfo, _MailAlert* pMailAlert);

	void Close();

	uint32 GetCount();
	IClientMessage* GetClientMessage(uint32 u4Index);

private:
	vecMonitorFSM m_vecMonitorFSM;
};
