#ifndef _ACEPROACTORMANAGER_H
#define _ACEPROACTORMANAGER_H

#include "define.h"

#include "ace/Synch_Traits.h"
#include "ace/Task.h"
#include "ace/Singleton.h"
#include "ace/Proactor.h" 
#include "ace/WIN32_Proactor.h" 
#include "ace/POSIX_Proactor.h"

#include <map>

using namespace std;

enum
{
	Proactor_WIN32    = 0x10,
	Proactor_POSIX    = 0x11,
};

//反应器对象
class CAceProactor : public ACE_Task<ACE_MT_SYNCH>
{
public:
	CAceProactor();
	~CAceProactor();

	void Close();

	bool Init(int nProactorType, int nThreadCount);
	const char* GetError();
	int GetThreadCount();
	int GetProactorType();
	ACE_Proactor* GetProactor();

	bool Start();
	bool Stop();

	virtual int open(void *args = 0);
	virtual int svc(void);

	void   SetProactorID(uint32 u4ProactorID);
	uint32 GetProactorID();

private:
	ACE_Proactor* m_pProactor;
	int           m_nProactorType;
	int           m_nThreadCount;
	char          m_szError[MAX_BUFF_500];
	bool          m_blRun;          //反应器是否在运行
	uint32        m_u4ProactorID;    //反应器的编号
};

class CAceProactorManager
{
public:
	CAceProactorManager(void);
	~CAceProactorManager(void);

	bool AddNewProactor(int nProactorID, int nProactorType = Proactor_WIN32, int nThreadCount = 1);
	void Close();
	const char* GetError();

	bool StartProactor();
	bool StartProactorDefault();
	bool StopProactor();

	CAceProactor* GetAceProactor(int nProactorID);
	ACE_Proactor* GetAce_Proactor(int nProactorID);

private:
	typedef map<int, CAceProactor*> mapAceProactor;
	mapAceProactor m_mapAceProactor;
	char           m_szError[MAX_BUFF_500];
};

typedef ACE_Singleton<CAceProactorManager, ACE_Null_Mutex> App_ProactorManager;
#endif
