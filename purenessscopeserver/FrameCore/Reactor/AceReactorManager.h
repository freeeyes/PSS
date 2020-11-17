// Define.h
// ���ﶨ����ʹ�õ��ķ�Ӧ��
// ������Щ���鲢����ô���ף������������ǲ���սʤ�ģ��ؼ����������Ƿ�Ը��սʤ����
// add by freeeyes
// 2008-12-23

#ifndef _ACEREACTORMANAGER_H
#define _ACEREACTORMANAGER_H

#include "define.h"

#include "ace/Synch_Traits.h"
#include "ace/Reactor.h"
#include "ace/Select_Reactor.h"
#include "ace/TP_Reactor.h"
#include "ace/WFMO_Reactor.h"
#include "ace/Dev_Poll_Reactor.h"
#include "ace/Task.h"
#include "ace/Singleton.h"
#include <stdexcept>

enum
{
    Reactor_Select      = 0x01,
    Reactor_TP          = 0x02,
    Reactor_WFMO        = 0x03,
    Reactor_DEV_POLL    = 0x04,
    Reactor_DEV_POLL_ET = 0x05,
};

//���������Ӧ����Ĭ������
const int REACTOR_CLIENTDEFINE = 0;
const int REACTOR_POSTDEFINE   = 1;
const int REACTOR_UDPDEFINE    = 2;

//��Ӧ������
class CAceReactor : public ACE_Task<ACE_MT_SYNCH>
{
public:
    CAceReactor();
    void Close();

    bool Init(int nReactorType, int nThreadCount, int nMaxHandleCount = 0);
    const char* GetError() const;
    int GetThreadCount() const;
    int GetReactorType() const;
    ACE_Reactor* GetReactor();

    bool Start();
    bool Stop();

    int open();
    virtual int svc(void);

    void   SetReactorID(uint32 u4ReactorID);
    uint32 GetReactorID() const;

private:
#if PSS_PLATFORM == PLATFORM_WIN
    void Create_Reactor_WFMO();
#endif
    void Create_Reactor_Select();
    void Create_Reactor_TP();
    void Create_DEV_POLL(int nMaxHandleCount);

    ACE_Reactor* m_pReactor              = nullptr;
    int          m_nReactorType          = 0;
    int          m_nThreadCount          = 0;
    char         m_szError[MAX_BUFF_500] = {'\0'};
    bool         m_blRun                 = false;     //��Ӧ���Ƿ�������
    uint32       m_u4ReactorID           = 0;         //��Ӧ���ı��
};

class CAceReactorManager
{
public:
    CAceReactorManager(void);

    void Init(uint16 u2Count);

    bool AddNewReactor(int nReactorID, int nReactorType = Reactor_Select, int nThreadCount = 1, int nMaxHandleCount = 0);
    void Close();
    const char* GetError() const;

    bool StartOtherReactor() const;
    bool StartClientReactor() const;
    bool StopReactor() const;

    CAceReactor* GetAceReactor(int nReactorID);
    ACE_Reactor* GetAce_Reactor(int nReactorID);
    ACE_Reactor* GetAce_Client_Reactor(int nReactorID);
    uint32 GetClientReactorCount() const;

private:
    CAceReactor**  m_pReactorList          = nullptr;
    uint16         m_u2RectorCount         = 0;
    char           m_szError[MAX_BUFF_500] = {'\0'};
};

typedef ACE_Singleton<CAceReactorManager, ACE_Null_Mutex> App_ReactorManager;

#endif
