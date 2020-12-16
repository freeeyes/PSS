#ifndef MESSAGEBLOCKMANAGER_H
#define MESSAGEBLOCKMANAGER_H

//���︺��������д�����mb�����ݣ���ʹ���ڴ�ء�
//�������freebird���뷨����д�ⲿ�ִ��룬��Ȼ������æ�������ⲿ�ֱ�����ɣ�
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
    CMessageBlockManager(void) = default;

    void Init();
    void Close();

    ACE_Message_Block* Create(uint32 u4Size) final;
    bool Close(ACE_Message_Block* pMessageBlock, uint32 u4BlockLen = 0) final;

    uint32 GetUsedSize() const;
private:
    uint32 m_u4UsedSize = 0;
};

using App_MessageBlockManager = PSS_singleton<CMessageBlockManager>;
#endif
