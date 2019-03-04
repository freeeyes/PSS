#ifndef _SMOPTION
#define _SMOPTION

#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_stdlib.h"
//
#ifdef WIN32
#include "ace/Shared_Memory_MM.h"      //windwos下使用
#else
#include "ace/Shared_Memory_SV.h"    //linux下使用
#endif
#include "ace/Log_Msg.h"
#include "ace/OS_NS_unistd.h"

#include "SMDefine.h"
#include <map>
#include <vector>

using namespace std;

typedef map<ID_t, _SMHeader*>         mapSMHeader;

class CSMOption
{
public:
    CSMOption(void);
    ~CSMOption(void);

    bool Init(key_t key, uint32 u4Size, uint32 u4ObjectCount, bool& blIsCreate);
    void Close();
    uint16 GetCount();

    _SMHeader* GetHeader(ID_t id);
    void* GetBuffer(ID_t nIndex);

    void   SetMemoryState(uint8 u1State);        //设置当前共享内存状态
    uint8  GetMemoryState();                     //得到当前共享内存状态
    void   SetMemoryVersion(uint32 u4Version);   //设置当前共享内存版本
    uint32 GetMemoryVersion();                   //得到当前共享内存版本

private:
    bool GetInitState();
    bool Init_Memory();

private:
    uint32                m_u4ObjectCount;
    uint32                m_u4BufferSize;
    char*                 m_pData;
#ifdef WIN32
    ACE_Shared_Memory_MM* m_pShareMemory;
#else
    ACE_Shared_Memory_SV* m_pShareMemory;
#endif
    mapSMHeader           m_mapSMHeader;
};
#endif

