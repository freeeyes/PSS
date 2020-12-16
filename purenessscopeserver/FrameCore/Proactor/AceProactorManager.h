#ifndef _ACEPROACTORMANAGER_H
#define _ACEPROACTORMANAGER_H

#include "define.h"

#include "ace/Synch_Traits.h"
#include "ace/Task.h"
#include "ace/Singleton.h"
#include "ace/Proactor.h"
#include "ace/WIN32_Proactor.h"
#include "ace/POSIX_Proactor.h"
#include <stdexcept>

using namespace std;

enum
{
    Proactor_WIN32    = 0x10,
    Proactor_POSIX    = 0x11,
};

//��Ӧ������
class CAceProactor : public ACE_Task<ACE_MT_SYNCH>
{
private:
    CAceProactor();
    friend class CAceProactorManager;

public:
    ~CAceProactor();

    void Close();

    bool Init(int nProactorType, int nThreadCount);
    const char* GetError();
    int GetThreadCount();
    int GetProactorType();
    ACE_Proactor* GetProactor();

    bool Start();
    bool Stop();

    virtual int open(void* args = 0);
    virtual int svc(void);

    void   SetProactorID(uint32 u4ProactorID);
    uint32 GetProactorID();

private:
    ACE_Proactor* m_pProactor;
    int           m_nProactorType;
    int           m_nThreadCount;
    char          m_szError[MAX_BUFF_500];
    bool          m_blRun;          //��Ӧ���Ƿ�������
    uint32        m_u4ProactorID;    //��Ӧ���ı��
};

class CAceProactorManager
{
public:
    CAceProactorManager(void);
    ~CAceProactorManager(void);

    void Init(uint16 u2Count);

    bool AddNewProactor(int nProactorID, int nProactorType = Proactor_WIN32, int nThreadCount = 1);
    void Close();
    const char* GetError();

    bool StartOtherProactor();
    bool StartClientProactor();
    bool StopProactor();

    CAceProactor* GetAceProactor(int nProactorID);
    ACE_Proactor* GetAce_Proactor();
    ACE_Proactor* GetAce_Client_Proactor(int nProactorID);
    uint32 GetClientReactorCount();

private:
    CAceProactor** m_pAceProactorList;
    uint16         m_u2ProactorCount;
    char           m_szError[MAX_BUFF_500];
};

using App_ProactorManager = PSS_singleton<CAceProactorManager>;
#endif
