#include "SMOption.h"

CSMOption::CSMOption(void)
{
    m_pShareMemory  = NULL;
    m_pData         = NULL;
    m_u4ObjectCount = 0;
    m_u4BufferSize  = 0;
}

CSMOption::~CSMOption(void)
{
    Close();
}

bool CSMOption::Init(key_t key, uint32 u4Size, uint32 u4ObjectCount, bool& blIsCreate)
{
    bool blIsLinuxFirst = false;
    //得到共享内存大小
    //1字节当前共享内存状态，0是未初始化，1是已初始化
    //4字节的共享内存更新信息，由watch维护更新
    uint32 u4MemorySize = sizeof(uint8) + sizeof(uint32) + sizeof(_SMHeader)*u4ObjectCount + u4Size*u4ObjectCount;
#ifdef WIN32

    if(m_pShareMemory == NULL)
    {
        m_pShareMemory = new ACE_Shared_Memory_MM(key, u4MemorySize);
    }

#else

    if(m_pShareMemory == NULL)
    {
        m_pShareMemory = new ACE_Shared_Memory_SV();
        //先尝试打开共享内存
        int nError = m_pShareMemory->open(key, u4MemorySize, ACE_Shared_Memory_SV::ACE_OPEN);

        if(nError != 0)
        {
            //创建共享内存
            nError = m_pShareMemory->open(key, u4MemorySize, ACE_Shared_Memory_SV::ACE_CREATE);

            if(nError != 0)
            {
                OUR_DEBUG((LM_ERROR, "[CSMOption::Init]create share memory fail(%d).\n", errno));
            }

            blIsLinuxFirst = true;
        }

        blIsCreate = true;
    }

#endif
    m_pData = (char*)m_pShareMemory->malloc();

    if(NULL == m_pData)
    {
        //PRINTF("[CSMOption::Init]open share memory fail.\n");
        return false;
    }

    if(true == blIsLinuxFirst)
    {
        ACE_OS::memset(m_pData, 0, u4MemorySize);
    }

    m_u4ObjectCount = u4ObjectCount;
    m_u4BufferSize  = u4Size;

    //是否初始化共享内存
    bool blState = GetInitState();

    if(true == blState)
    {
        Init_Memory();
    }
    else
    {
        Init_Memory();
        blIsCreate = false;
    }

    return true;
}

void CSMOption::Close()
{
    if(NULL != m_pShareMemory)
    {
        m_pShareMemory->remove();

        delete m_pShareMemory;
        m_pShareMemory = NULL;
    }

    m_u4ObjectCount = 0;
}

bool CSMOption::GetInitState()
{
    if(m_pData == NULL)
    {
        return false;
    }
    else
    {
        uint8 u1State = 0;
        ACE_OS::memcpy(&u1State, m_pData, sizeof(uint8));

        //printf("[CSMOption::GetInitState]u1State=%d, READERINITSTATE=%d.\n", u1State, READERINITSTATE);
        if(u1State == READERINITSTATE)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool CSMOption::Init_Memory()
{
    if(m_pData == NULL)
    {
        //PRINTF("[CSMOption::Init_Memory]m_pData is NULL.\n");
        return false;
    }

    ACE_Time_Value tvNow = ACE_OS::gettimeofday();
    char* pBegin = m_pData + sizeof(uint8) + sizeof(uint32);

    int nPos = 0;

    for(uint16 i = 1; i <= m_u4ObjectCount; i++)
    {
        _SMHeader* pSMHeader = (_SMHeader* )(pBegin + nPos*(sizeof(_SMHeader) + m_u4BufferSize));
        pSMHeader->m_HeaderID     = (ID_t)i;
        pSMHeader->m_u4DataOffset = nPos*(sizeof(_SMHeader) + m_u4BufferSize) + sizeof(_SMHeader);
        pSMHeader->m_tvUpdate     = tvNow;

        m_mapSMHeader.insert(mapSMHeader::value_type(pSMHeader->m_HeaderID, pSMHeader));

        nPos++;
    }

    //初始化共享内存相关参数
    uint8 u1State = (uint8)READERINITSTATED;
    ACE_OS::memcpy(m_pData, &u1State, sizeof(uint8));
    uint32 u4Version = 0;
    ACE_OS::memcpy((char* )&m_pData[sizeof(uint8)], &u4Version, sizeof(uint32));
    return true;
}

uint16 CSMOption::GetCount()
{
    return m_u4ObjectCount;
}

_SMHeader* CSMOption::GetHeader( ID_t id )
{
    mapSMHeader::iterator f = m_mapSMHeader.find(id);

    if(f == m_mapSMHeader.end())
    {
        return NULL;
    }
    else
    {
        return (_SMHeader* )f->second;
    }
}

void* CSMOption::GetBuffer( ID_t nIndex )
{
    _SMHeader* pSMHeader = GetHeader((ID_t)nIndex);

    if(NULL == pSMHeader)
    {
        return NULL;
    }
    else
    {
        return (void* )(m_pData + pSMHeader->m_u4DataOffset);
    }

}

void CSMOption::SetMemoryState(uint8 u1State)
{
    if(NULL != m_pData)
    {
        ACE_OS::memcpy(m_pData, &u1State, sizeof(uint8));
    }
}

uint8 CSMOption::GetMemoryState()
{
    if(NULL != m_pData)
    {
        uint8 u1Ret = 0;
        ACE_OS::memcpy((char* )&u1Ret, m_pData, sizeof(uint8));
        return u1Ret;
    }
    else
    {
        return (uint8)0;
    }
}

void CSMOption::SetMemoryVersion(uint32 u4Version)
{
    if(NULL != m_pData)
    {
        ACE_OS::memcpy(&m_pData[sizeof(uint8)], &u4Version, sizeof(uint32));
    }
}

uint32 CSMOption::GetMemoryVersion()
{
    uint32 u4Version = 0;

    if(NULL != m_pData)
    {
        ACE_OS::memcpy((char* )&u4Version, (char* )&m_pData[sizeof(uint8)], sizeof(uint32));
        return (uint32)u4Version;
    }
    else
    {
        return (uint32)u4Version;
    }
}
