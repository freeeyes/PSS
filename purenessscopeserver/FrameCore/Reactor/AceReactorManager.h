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
#include <unordered_map>

enum class EM_REACTOR_MODULE
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
    CAceReactor() = default;
    void Close();

    bool Init(EM_REACTOR_MODULE emReactorType, int nThreadCount, int nMaxHandleCount = 0);
    const char* GetError() const;
    int GetThreadCount() const;
    EM_REACTOR_MODULE GetReactorType() const;
    ACE_Reactor* GetReactor();

    bool Start();
    bool Stop();

    int open();
    int svc(void) final;

    void   SetReactorID(uint32 u4ReactorID);
    uint32 GetReactorID() const;

private:
#if PSS_PLATFORM == PLATFORM_WIN
    void Create_Reactor_WFMO();
#endif
    void Create_Reactor_Select();
    void Create_Reactor_TP();
    void Create_DEV_POLL(int nMaxHandleCount);

    ACE_Reactor*      m_pReactor              = nullptr;
    EM_REACTOR_MODULE m_emReactorType         = EM_REACTOR_MODULE::Reactor_DEV_POLL;
    int               m_nThreadCount          = 0;
    string            m_strError;
    bool              m_blRun                 = false;     //��Ӧ���Ƿ�������
    uint32            m_u4ReactorID           = 0;         //��Ӧ���ı��
};

class CAceReactorManager
{
public:
    CAceReactorManager(void) = default;

    void Init(uint16 u2Count);

    bool AddNewReactor(int nReactorID, EM_REACTOR_MODULE emReactorType = EM_REACTOR_MODULE::Reactor_Select, int nThreadCount = 1, int nMaxHandleCount = 0);
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
    using hashmapReactorList = unordered_map<uint16, CAceReactor*>;
    hashmapReactorList  m_pReactorList;
    uint16              m_u2RectorCount         = 0;
    string              m_strError;
};

using App_ReactorManager = ACE_Singleton<CAceReactorManager, ACE_Null_Mutex>;

#endif
