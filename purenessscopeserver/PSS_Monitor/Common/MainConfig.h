#include "define.h"
#include "XmlOpeation.h"

#include "ace/Singleton.h"
#include <vector>

using namespace std;

struct _ServerInfo
{
	char  m_szServerIP[MAX_BUFF_50];
	int   m_nPort;
	uint8 m_u1IPType;
	char  m_szKey[MAX_BUFF_50];

	_ServerInfo()
	{
		m_szServerIP[0] = '\0';
		m_nPort         = 0;
		m_u1IPType      = TYPE_IPV4;
		m_szKey[0]      = '\0';
	}
};

typedef vector<_ServerInfo> vecServerInfo;

class CMainConfig
{
public:
	CMainConfig();
	~CMainConfig();

	bool Init(const char* szConfigPath);
	void Display();

	uint32 GetBuffSize();
	uint32 GerServerInfoCount();
	_ServerInfo* GetServerInfo(uint32 u4Index);

private:
	vecServerInfo m_vecServerInfo;
	CXmlOpeation  m_MainConfig;
	uint32        m_u4BuffSize;
};

typedef ACE_Singleton<CMainConfig, ACE_Null_Mutex> App_MainConfig;

