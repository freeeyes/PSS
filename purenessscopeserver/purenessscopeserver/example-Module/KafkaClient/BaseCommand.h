#pragma once

#include "IBuffPacket.h"
#include "ClientCommand.h"
#include "IObject.h"
#include "ace/Date_Time.h"

#include "KafkaClient.h"

using namespace std;

//定义ZooKeeper服务器节点信息
#define ZOOKEEPER_ADDRESS "localhost:4181"

class CBaseCommand : public CClientCommand
{
public:
    CBaseCommand(void);
    ~CBaseCommand(void);

    void Init(const char* pPath);

    void Fini();

    int DoMessage(IMessage* pMessage, bool& bDeleteFlag);

    void SetServerObject(CServerObject* pServerObject);

    void Run();

private:
    int  Get_Log_File();

    void SetLogPath(const char* pPath);

private:
    CServerObject*             m_pServerObject;
    int                        m_nCount;
    CKafkaClient               m_KafkaClient;
    char                       m_szLogPath[256];
    int                        m_nComsumeIndex;
    ACE_Recursive_Thread_Mutex m_ThreadWriteLock;
};
