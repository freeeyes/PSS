#ifndef _BUFFPACKETMAGER_H
#define _BUFFPACKETMAGER_H

//����������CreateBuffPacket��ǳ�����������Ҫ����������
//ʹ��BuffPacket�أ��ڷ�����������ʱ��һ���Դ����ö��Buffpacket�������ڴ�ص��뷨��
//�����лfreebird�Ĳ��Կͻ��ˣ�������Ϊ�����Ҳ��ܷ�����������⡣��Ҫ�ù�ע�����ʧ�������Ǳ���ġ�
//2010-06-03 freeeyes

#include "IPacketManager.h"
#include "BuffPacket.h"
#include "ObjectPoolManager.h"
#include "XmlConfig.h"

class CBuffPacketManager : public CObjectPoolManager<CBuffPacket, ACE_Recursive_Thread_Mutex>, public IPacketManager
{
public:
    CBuffPacketManager() = default;
    virtual ~CBuffPacketManager() = default;

    static void Init_Callback(int nIndex, CBuffPacket* pBuffPacket);
    static void Close_Callback(int nIndex, CBuffPacket* pBuffPacket);

    uint32 GetBuffPacketUsedCount() final;
    uint32 GetBuffPacketFreeCount() final;
    void OutputCreateInfo() final;
    void SetCreateFlag(bool blTagCreateInfo) final;

    IBuffPacket* Create(const char* pFileName, uint32 u4Line) final;
    bool Delete(IBuffPacket* pBuffPacket) final;
    uint32 GetCreateInfoCount() final;
    bool GetCreateInfoList(uint32 u4Index, _Object_Create_Info& objCreateInfo) final;
private:
    vector<_Object_Create_Info> m_objCreateList;
};

typedef ACE_Singleton<CBuffPacketManager, ACE_Null_Mutex> App_BuffPacketManager;
#endif
