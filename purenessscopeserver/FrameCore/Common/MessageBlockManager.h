#ifndef MESSAGEBLOCKMANAGER_H
#define MESSAGEBLOCKMANAGER_H

//这里负责管理所有创建的mb的数据，并使用内存池。
//最近深受freebird的想法，重写这部分代码，虽然工作很忙，但是这部分必须完成！
//add by freeeyes
//2010-06-12

#include "ace/Thread_Mutex.h"
#include "ace/Malloc.h"
#include "ace/Malloc_T.h"
#include "ace/Local_Memory_Pool.h"
#include "ace/Synch_Traits.h"
#include "ace/Singleton.h"
#include "ace/Thread_Mutex.h"
#include "ace/Task.h"
#include "ace/Synch.h"
#include <math.h>
#include <unordered_map>

#include "IMessageBlockManager.h"
#include "XmlConfig.h"

class CMessageBlockManager : public IMessageBlockManager
{
public:
    CMessageBlockManager(void);

    void Init() const;
    void Close() const;

    virtual ACE_Message_Block* Create(uint32 u4Size);
    virtual bool Close(ACE_Message_Block* pMessageBlock, uint32 u4BlockLen = 0);

    uint32 GetUsedSize() const;

};

typedef ACE_Singleton<CMessageBlockManager,ACE_Null_Mutex> App_MessageBlockManager;
#endif
