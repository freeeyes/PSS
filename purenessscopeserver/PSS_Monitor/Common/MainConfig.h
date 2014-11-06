#include "define.h"
#include "XmlOpeation.h"

#include "ace/Singleton.h"
#include <vector>

using namespace std;

#define DEFAULT_TIME_INTERVAL 60

class CMainConfig
{
public:
	CMainConfig();
	~CMainConfig();

	bool Init(const char* szConfigPath);
	void Display();

	uint32 GetBuffSize();
	uint16 GetTimeInterval();
	uint32 GerServerInfoCount();
	_ServerInfo* GetServerInfo(uint32 u4Index);
	_MailAlert* GetMailAlert();

private:
	vecServerInfo m_vecServerInfo;
	CXmlOpeation  m_MainConfig;
	uint32        m_u4BuffSize;
	uint16        m_u2TimeInterval;
	_MailAlert    m_objMailAlert;
};

typedef ACE_Singleton<CMainConfig, ACE_Null_Mutex> App_MainConfig;

