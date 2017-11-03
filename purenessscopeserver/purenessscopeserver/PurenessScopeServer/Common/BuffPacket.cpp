// BuffPacket.h
// 这里定义接受数据包的内容缓冲
// 实际的坚持，在于你对它是否是重要的，同理，它对你是否是重要的。
// add by freeeyes
// 2009-01-02

#include "BuffPacket.h"

CBuffPacket::CBuffPacket(int32 nSize, int32 nMaxBuffSize)
    :m_szData(NULL), m_u4ReadPtr(0), m_u4WritePtr(0), m_u4PacketLen(0), m_u4PacketCount(0), m_u4MaxPacketSize(0), m_u4BuffID(0), m_nHashID(0), m_blNetSort(false)
{
    if (false == Init(nSize, nMaxBuffSize))
    {
        OUR_DEBUG((LM_INFO, "[CBuffPacket::CBuffPacket]Error(%s).\n", m_szError));
    }
}

CBuffPacket::~CBuffPacket(void)
{
    //OUR_DEBUG((LM_ERROR, "[CBuffPacket::~CBuffPacket].\n"));
    if (false == Close())
    {
        OUR_DEBUG((LM_INFO, "[CBuffPacket::~CBuffPacket]Close error(%s).\n", m_szError));
    }
}

bool CBuffPacket::Init(int32 nSize, int32 nMaxBuffSize)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    try
    {
        m_szError[0]      = '\0';
        m_u4MaxPacketSize = (uint32)nMaxBuffSize;

        if(nSize >= (int32)m_u4MaxPacketSize)
        {
            OUR_DEBUG((LM_ERROR, "[CBuffPacket::Init] nSize [%d] is more than m_u4MaxPacketSize.\n", nSize));
            char szError[MAX_BUFF_500] = {'\0'};
            sprintf_safe(szError, MAX_BUFF_500, "[CBuffPacket::Init] nSize [%d] is more than m_u4MaxPacketSize.", nSize);
            throw szError;
        }

        //初始化包数据结构
        m_u4ReadPtr       = 0;
        m_u4WritePtr      = 0;
        m_u4PacketCount   = 0;
        m_nHashID         = 0;
        m_u4PacketLen     = (uint32)((int32)ceil((double)nSize/(double)DEFINE_PACKET_ADD))*DEFINE_PACKET_ADD;

        m_szData          = (char*)App_ACEMemory::instance()->malloc(m_u4PacketLen);
        m_u4BuffID        = 0;

        //OUR_DEBUG((LM_ERROR, "[CBuffPacket::Init] nSize [%d], m_szData=[0x%08x].\n", m_u4PacketLen, m_szData));

        if(NULL == m_szData)
        {
            OUR_DEBUG((LM_ERROR, "[CBuffPacket::Init] nSize [%d] is new error.\n", m_u4PacketLen));
            char szError[MAX_BUFF_500] = {'\0'};
            sprintf_safe(szError, MAX_BUFF_500, "[CBuffPacket::Init] nSize [%d] is new error..", m_u4PacketLen);
            throw szError;
        }

        m_blNetSort = false;

        return true;
    }
    catch(const char* szError)
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "%s", szError);
        return false;
    }
}

bool CBuffPacket::Close()
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    try
    {
        if(m_u4PacketLen >= m_u4MaxPacketSize)
        {
            OUR_DEBUG((LM_ERROR, "[CBuffPacket::Close] nSize [%d] is more than m_u4MaxPacketSize.\n", m_u4PacketLen));
            char szError[MAX_BUFF_500] = {'\0'};
            sprintf_safe(szError, MAX_BUFF_500, "[CBuffPacket::Close] nSize [%d] is more than m_u4MaxPacketSize.", m_u4PacketLen);
            throw szError;
        }

        //OUR_DEBUG((LM_ERROR, "[CBuffPacket::Close] nSize [%d], m_szData=[0x%08x].\n", m_u4PacketLen, m_szData));

        if(NULL != m_szData)
        {
            App_ACEMemory::instance()->free((void* )m_szData);
            //delete m_szData;
            m_szData          = NULL;
            m_u4ReadPtr       = 0;
            m_u4WritePtr      = 0;
            m_u4PacketLen     = 0;
        }

        return true;
    }
    catch(const char* szError)
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "%s", szError);
        return false;
    }
}

bool CBuffPacket::Clear()
{
    m_u4ReadPtr       = 0;
    m_u4WritePtr      = 0;
    m_u4PacketCount   = 0;

    m_blNetSort       = false;

    return true;
}

bool CBuffPacket::AddBuffPacket(uint32 u4Size)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    try
    {
        char* szNewData = NULL;

        if(u4Size + m_u4PacketLen >= m_u4MaxPacketSize)
        {
            OUR_DEBUG((LM_ERROR, "[CBuffPacket::AddBuff] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", u4Size, m_u4PacketLen));
            char szError[MAX_BUFF_500] = {'\0'};
            sprintf_safe(szError, MAX_BUFF_500, "[CBuffPacket::AddBuff] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", u4Size, m_u4PacketLen);
            throw szError;
        }

        //保存当前长度
        uint32 u4OldPacketLen = m_u4PacketLen;

        //格式化新创建的内存大小
        u4Size = (uint32)(((int32)ceil((double)u4Size/(double)DEFINE_PACKET_ADD))*DEFINE_PACKET_ADD);
        m_u4PacketLen += u4Size;

        szNewData = (char*)App_ACEMemory::instance()->malloc(m_u4PacketLen);

        if(NULL == szNewData)
        {
            OUR_DEBUG((LM_ERROR, "[CBuffPacket::AddBuff] nSize [%d] is new error.\n", m_u4PacketLen));
            char szError[MAX_BUFF_500] = {'\0'};
            sprintf_safe(szError, MAX_BUFF_500, "[CBuffPacket::AddBuff] nSize [%d] is new error..", m_u4PacketLen);
            throw szError;
        }

        if(NULL == m_szData)
        {
            m_szData = szNewData;
        }
        else
        {
            //移动内存
            memmove_safe(m_szData, u4OldPacketLen, szNewData, m_u4PacketLen);

            //删除已经不用的内存
            App_ACEMemory::instance()->free(m_szData);
            //delete m_szData;
            m_szData = szNewData;
        }


        return true;
    }
    catch(const char* szError)
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "%s", szError);
        return false;
    }
}


bool CBuffPacket::AddBuff(uint32 u4Size)
{
    try
    {
        char* szNewData = NULL;

        if(u4Size + m_u4PacketLen >= m_u4MaxPacketSize)
        {
            OUR_DEBUG((LM_ERROR, "[CBuffPacket::AddBuff] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", u4Size, m_u4PacketLen));
            char szError[MAX_BUFF_500] = {'\0'};
            sprintf_safe(szError, MAX_BUFF_500, "[CBuffPacket::AddBuff] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", u4Size, m_u4PacketLen);
            throw szError;
        }

        //保存当前长度
        uint32 u4OldPacketLen = m_u4PacketLen;

        //格式化新创建的内存大小
        u4Size = (uint32)(((int32)ceil((double)u4Size/(double)DEFINE_PACKET_ADD))*DEFINE_PACKET_ADD);
        m_u4PacketLen += u4Size;

        szNewData = (char*)App_ACEMemory::instance()->malloc(m_u4PacketLen);

        if(NULL == szNewData)
        {
            OUR_DEBUG((LM_ERROR, "[CBuffPacket::AddBuff] nSize [%d] is new error.\n", m_u4PacketLen));
            char szError[MAX_BUFF_500] = {'\0'};
            sprintf_safe(szError, MAX_BUFF_500, "[CBuffPacket::AddBuff] nSize [%d] is new error..", m_u4PacketLen);
            throw szError;
        }

        if(NULL == m_szData)
        {
            m_szData = szNewData;
        }
        else
        {
            //移动内存
            memmove_safe(m_szData, u4OldPacketLen, szNewData, m_u4PacketLen);

            //删除已经不用的内存
            App_ACEMemory::instance()->free(m_szData);
            //delete m_szData;
            m_szData = szNewData;
        }


        return true;
    }
    catch(const char* szError)
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "%s", szError);
        return false;
    }
}

const char* CBuffPacket::GetData()
{
    return m_szData;
}

uint32 CBuffPacket::GetPacketSize()
{
    return m_u4PacketLen;
}

uint32 CBuffPacket::GetPacketLen()
{
    return m_u4WritePtr;
}

uint32 CBuffPacket::GetReadLen()
{
    return m_u4ReadPtr;
}

uint32 CBuffPacket::GetWriteLen()
{
    return m_u4WritePtr;
}

uint32 CBuffPacket::GetHeadLen()
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    if(m_u4PacketLen - m_u4ReadPtr > sizeof(uint32))
    {
        return *(uint32*)ReadPtr();
    }
    else
    {
        return (uint32)0;
    }
}

void CBuffPacket::ReadBuffPtr(uint32 u4Size)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    ReadPtr(u4Size);
}

void CBuffPacket::ReadPtr(uint32 u4Size)
{
    if(m_u4ReadPtr + u4Size < m_u4PacketLen)
    {
        m_u4ReadPtr += u4Size;
    }
}

void CBuffPacket::WriteBuffPtr(uint32 u4Size)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    WritePtr(u4Size);
}

void CBuffPacket::WritePtr(uint32 u4Size)
{
    if(m_u4WritePtr + u4Size <= m_u4PacketLen)
    {
        m_u4WritePtr += u4Size;
    }
}

char* CBuffPacket::ReadPtr()
{
    return m_szData + m_u4ReadPtr;
}

void CBuffPacket::SetReadPtr(uint32 u4Pos)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    if(u4Pos < m_u4PacketLen)
    {
        m_u4ReadPtr = u4Pos;
    }
}

char* CBuffPacket::WritePtr()
{
    return m_szData + m_u4WritePtr;
}

bool CBuffPacket::RollBack(uint32 u4Len)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    uint32 u4NewLen = 0;

    if(u4Len >= m_u4WritePtr)
    {
        if(u4Len > m_u4WritePtr)
        {
            OUR_DEBUG((LM_ERROR, "[CBuffPacket::RollBack] Error u4Len(%d) m_u4WritePtr(%d).\n", u4Len, m_u4WritePtr));
            return false;
        }

        m_u4WritePtr = 0;
        m_u4ReadPtr  = 0;
        return true;
    }
    else
    {
        u4NewLen = m_u4WritePtr - u4Len;
        memmove_safe(m_szData + u4Len, u4NewLen, m_szData, u4NewLen);

        m_u4ReadPtr  = 0;
        m_u4WritePtr = u4NewLen;
        return true;
    }
}

bool CBuffPacket::WriteStream(const char* pData, uint32 u4Len)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    if(u4Len + m_u4PacketLen >= m_u4MaxPacketSize)
    {
        OUR_DEBUG((LM_ERROR, "[CBuffPacket::WriteStream] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", u4Len, m_u4PacketLen));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CBuffPacket::WriteStream] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", u4Len, m_u4PacketLen);
        return false;
    }

    if(m_u4WritePtr + u4Len > m_u4PacketLen)
    {
        if(false == AddBuff(u4Len))
        {
            OUR_DEBUG((LM_ERROR, "[CBuffPacket::WriteStream] nSize = [%d] m_u4PacketLen = [%d] AddBuff false.\n", u4Len, m_u4PacketLen));
            return false;
        }

        //写入文本内容
        memcpy_safe((char* )pData, u4Len, WritePtr(), GetPacketSize() - GetWriteLen());
        WritePtr(u4Len);
        return true;
    }
    else
    {
        //写入文本内容
        memcpy_safe((char* )pData, u4Len, WritePtr(), GetPacketSize() - GetWriteLen());
        WritePtr(u4Len);
        return true;
    }
}

bool CBuffPacket::ReadStream(char* pData, uint32& u4Len)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    if(pData == NULL || u4Len > m_u4PacketLen - m_u4ReadPtr)
    {
        return false;
    }

    memcpy_safe(ReadPtr(), u4Len, pData, u4Len);
    ReadPtr(u4Len);
    return true;
}

void CBuffPacket::SetPacketCount(uint32 u4PacketCount)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    m_u4PacketCount = u4PacketCount;
}

uint32 CBuffPacket::GetPacketCount()
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    return m_u4PacketCount;
}

CBuffPacket& CBuffPacket::operator >> (uint8& u1Data)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    u1Data = 0;

    if(m_u4ReadPtr <= m_u4WritePtr - sizeof(u1Data))
    {
        u1Data = *(uint8*)ReadPtr();
        ReadPtr((uint32)sizeof(u1Data));
    }

    return *this;
}

CBuffPacket& CBuffPacket::operator >> (uint16& u2Data)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    u2Data = 0;

    if(m_u4ReadPtr <= m_u4WritePtr - (uint32)sizeof(u2Data))
    {
        //把网络字节序，转换为主机字节序
        uint16 n2Net = 0;
        memcpy_safe(ReadPtr(), (uint32)sizeof(uint16), (char* )&n2Net, (uint32)sizeof(uint16));
        ReadPtr((uint32)sizeof(n2Net));

        if(true == m_blNetSort)
        {
            u2Data = ACE_NTOHS(n2Net);
        }
        else
        {
            u2Data = n2Net;
        }

    }

    return *this;
}

CBuffPacket& CBuffPacket::operator >> (uint32& u4Data)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    u4Data = 0;

    if(m_u4ReadPtr <= m_u4WritePtr - (uint32)sizeof(u4Data))
    {
        //把网络字节序，转换为主机字节序
        //uint32 n4Net = *(uint32 *)ReadPtr();
        uint32 n4Net = 0;
        memcpy_safe(ReadPtr(), (uint32)sizeof(uint32), (char* )&n4Net, (uint32)sizeof(uint32));
        ReadPtr((uint32)sizeof(n4Net));

        if(true == m_blNetSort)
        {
            u4Data = ACE_NTOHL(n4Net);
        }
        else
        {
            u4Data = n4Net;
        }

    }

    return *this;
}

CBuffPacket& CBuffPacket::operator >> (uint64& u8Data)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    u8Data = 0;

    if(m_u4ReadPtr <= m_u4WritePtr - (uint32)sizeof(u8Data))
    {
        //uint64 u8Net = *(uint64 *)ReadPtr();
        uint64 u8Net = 0;
        memcpy_safe(ReadPtr(), (uint32)sizeof(uint64), (char* )&u8Net, (uint32)sizeof(uint64));
        ReadPtr((uint32)sizeof(u8Net));

        if(true == m_blNetSort)
        {
            u8Data = ntohl64(u8Net);
        }
        else
        {
            u8Data = u8Net;
        }
    }

    return *this;
}

CBuffPacket& CBuffPacket::operator >> (int8& n1Data)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    n1Data = 0;

    if(m_u4ReadPtr <= m_u4WritePtr - sizeof(n1Data))
    {
        n1Data = *(int8*)ReadPtr();
        ReadPtr((uint32)sizeof(n1Data));
    }

    return *this;
}

CBuffPacket& CBuffPacket::operator >> (int16& n2Data)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    n2Data = 0;

    if(m_u4ReadPtr <= m_u4WritePtr - sizeof(n2Data))
    {
        int16 n2Net = 0;
        memcpy_safe(ReadPtr(), (uint32)sizeof(int16), (char* )&n2Net, (uint32)sizeof(int16));
        ReadPtr((uint32)sizeof(n2Net));

        if(true == m_blNetSort)
        {
            n2Data = ACE_NTOHS(n2Net);
        }
        else
        {
            n2Data = n2Net;
        }
    }

    return *this;
}

CBuffPacket& CBuffPacket::operator >> (int32& n4Data)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    n4Data = 0;

    if(m_u4ReadPtr <= m_u4WritePtr - sizeof(n4Data))
    {
        //n4Data = *(int32 *)ReadPtr();
        //ReadPtr((uint32)sizeof(n4Data));

        int32 n4Net = 0;
        memcpy_safe(ReadPtr(), (uint32)sizeof(int32), (char* )&n4Net, (uint32)sizeof(int32));
        ReadPtr((uint32)sizeof(n4Net));

        if(true == m_blNetSort)
        {
            n4Data = ACE_NTOHL(n4Net);
        }
        else
        {
            n4Data = n4Net;
        }
    }

    return *this;
}

CBuffPacket& CBuffPacket::operator >> (float32& f4Data)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    f4Data = 0;

    if(m_u4ReadPtr <= m_u4WritePtr - (uint32)sizeof(f4Data))
    {
        //浮点型不需要字序转换
        memcpy_safe(ReadPtr(), (uint32)sizeof(float32), (char* )&f4Data, (uint32)sizeof(float32));
        ReadPtr((uint32)sizeof(f4Data));
    }

    return *this;
}

CBuffPacket& CBuffPacket::operator >> (float64& f8Data)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    f8Data = 0;

    if(m_u4ReadPtr <= m_u4WritePtr - (uint32)sizeof(f8Data))
    {
        //浮点型不需要字序转换
        memcpy_safe(ReadPtr(), (uint32)sizeof(float64), (char* )&f8Data, (uint32)sizeof(float64));
        ReadPtr((uint32)sizeof(f8Data));
    }

    return *this;
}

CBuffPacket& CBuffPacket::operator >> (VCHARS_STR& str)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    uint8 u1Len = 0;
    (*this) >> (u1Len);

    if(u1Len && m_u4ReadPtr <= m_u4WritePtr - u1Len)
    {
        str.SetData((char* )ReadPtr(), u1Len);
        ReadPtr((uint32)u1Len);
    }
    else
    {
        str.text  = NULL;
        str.u1Len = 0;
    }

    return *this;
}

CBuffPacket& CBuffPacket::operator >> (VCHARM_STR& str)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    uint16 u2Len = 0;
    (*this) >> (u2Len);

    if(u2Len && m_u4ReadPtr <= m_u4WritePtr - u2Len)
    {
        str.SetData((char* )ReadPtr(), u2Len);
        ReadPtr((uint32)u2Len);
    }
    else
    {
        str.text  = NULL;
        str.u2Len = 0;
    }

    return *this;
}

CBuffPacket& CBuffPacket::operator >> (VCHARB_STR& str)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    uint32 u4Len = 0;
    (*this) >> (u4Len);

    if(u4Len && m_u4ReadPtr <= m_u4WritePtr - u4Len)
    {
        str.SetData((char* )ReadPtr(), u4Len);
        ReadPtr((uint32)u4Len);
    }
    else
    {
        str.text  = NULL;
        str.u4Len = 0;
    }

    return *this;
}

CBuffPacket& CBuffPacket::operator >> (string& str)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    uint32 u4Len = 0;
    (*this) >> (u4Len);

    if(u4Len && m_u4ReadPtr <= m_u4WritePtr - u4Len)
    {
        str = string((char* )ReadPtr(), u4Len);
        ReadPtr((uint32)u4Len);
    }

    return *this;
}

CBuffPacket& CBuffPacket::operator << (uint8 u1Data)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    if( (uint32)sizeof(u1Data) + m_u4PacketLen >= m_u4MaxPacketSize)
    {
        OUR_DEBUG((LM_ERROR, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", (uint32)sizeof(u1Data), m_u4PacketLen));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", (uint32)sizeof(u1Data), m_u4PacketLen);
        return *this;
    }

    if(m_u4WritePtr + (uint32)sizeof(u1Data) >= GetPacketSize())
    {
        if(false == AddBuff(DEFINE_PACKET_ADD))
        {
            return *this;
        }
        else
        {
            *(uint8*)WritePtr() = u1Data;
            WritePtr((uint32)sizeof(u1Data));
            return *this;
        }
    }
    else
    {
        *(uint8*)WritePtr() = u1Data;
        WritePtr((uint32)sizeof(u1Data));
        return *this;
    }

    return *this;
}

CBuffPacket& CBuffPacket::operator << (uint16 u2Data)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    if( (uint32)sizeof(u2Data) + m_u4PacketLen >= m_u4MaxPacketSize)
    {
        OUR_DEBUG((LM_ERROR, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", (uint32)sizeof(u2Data), m_u4PacketLen));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", (uint32)sizeof(u2Data), m_u4PacketLen);
        return *this;
    }

    if(m_u4WritePtr + (uint32)sizeof(u2Data) >= GetPacketSize())
    {
        if(false == AddBuff(DEFINE_PACKET_ADD))
        {
            return *this;
        }
        else
        {
            //为了存取方便，在存储数字的时候，自动转换为网络字节序，取出的时候自动转回主机字节序
            uint16 u2Net = 0;

            if(true == m_blNetSort)
            {
                u2Net = ACE_HTONS(u2Data);
            }
            else
            {
                u2Net = u2Data;
            }

            *(uint16*)WritePtr() = u2Net;
            WritePtr((uint32)sizeof(u2Net));
            return *this;
        }
    }
    else
    {
        //为了存取方便，在存储数字的时候，自动转换为网络字节序，取出的时候自动转回主机字节序
        uint16 u2Net = 0;

        if(true == m_blNetSort)
        {
            u2Net = ACE_HTONS(u2Data);
        }
        else
        {
            u2Net = u2Data;
        }

        *(uint16*)WritePtr() = u2Net;
        WritePtr((uint32)sizeof(u2Net));
        return *this;
    }

    return *this;
}

CBuffPacket& CBuffPacket::operator << (uint32 u4Data)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    if( (uint32)sizeof(u4Data) + m_u4PacketLen >= m_u4MaxPacketSize)
    {
        OUR_DEBUG((LM_ERROR, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", (uint32)sizeof(u4Data), m_u4PacketLen));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", (uint32)sizeof(u4Data), m_u4PacketLen);
        return *this;
    }

    if(m_u4WritePtr + (uint32)sizeof(u4Data) >= GetPacketSize())
    {
        if(false == AddBuff(DEFINE_PACKET_ADD))
        {
            return *this;
        }
        else
        {
            //为了存取方便，在存储数字的时候，自动转换为网络字节序，取出的时候自动转回主机字节序
            uint32 u4Net = 0;

            if(true == m_blNetSort)
            {
                u4Net = ACE_HTONL(u4Data);
            }
            else
            {
                u4Net = u4Data;
            }

            *(uint32*)WritePtr() = u4Net;
            WritePtr((uint32)sizeof(u4Net));
            return *this;
        }
    }
    else
    {
        //为了存取方便，在存储数字的时候，自动转换为网络字节序，取出的时候自动转回主机字节序
        uint32 u4Net = 0;

        if(true == m_blNetSort)
        {
            u4Net = ACE_HTONL(u4Data);
        }
        else
        {
            u4Net = u4Data;
        }

        *(uint32*)WritePtr() = u4Net;
        WritePtr((uint32)sizeof(u4Net));
        return *this;
    }

    return *this;
}

CBuffPacket& CBuffPacket::operator << (uint64 u8Data)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    if( (uint32)sizeof(u8Data) + m_u4PacketLen >= m_u4MaxPacketSize)
    {
        OUR_DEBUG((LM_ERROR, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", (uint32)sizeof(u8Data), m_u4PacketLen));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", (uint32)sizeof(u8Data), m_u4PacketLen);
        return *this;
    }

    if(m_u4WritePtr + (uint32)sizeof(u8Data) >= GetPacketSize())
    {
        if(false == AddBuff(DEFINE_PACKET_ADD))
        {
            return *this;
        }
        else
        {
            //为了存取方便，在存储数字的时候，自动转换为网络字节序，取出的时候自动转回主机字节序
            uint64 u8Net = 0;

            if(true == m_blNetSort)
            {
                u8Net = hl64ton(u8Data);
            }
            else
            {
                u8Net = u8Data;
            }

            *(uint64*)WritePtr() = u8Net;
            WritePtr((uint32)sizeof(u8Net));
            return *this;
        }
    }
    else
    {
        //为了存取方便，在存储数字的时候，自动转换为网络字节序，取出的时候自动转回主机字节序
        uint64 u8Net = 0;

        if(true == m_blNetSort)
        {
            u8Net = hl64ton(u8Data);
        }
        else
        {
            u8Net = u8Data;
        }

        *(uint64*)WritePtr() = u8Net;
        WritePtr((uint32)sizeof(u8Net));
        return *this;
    }

    return *this;
}

CBuffPacket& CBuffPacket::operator << (int8 n1Data)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    if( (uint32)sizeof(n1Data) + m_u4PacketLen >= m_u4MaxPacketSize)
    {
        OUR_DEBUG((LM_ERROR, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", (uint32)sizeof(n1Data), m_u4PacketLen));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", (uint32)sizeof(n1Data), m_u4PacketLen);
        return *this;
    }

    if(m_u4WritePtr + (uint32)sizeof(n1Data) >= GetPacketSize())
    {
        if(false == AddBuff(DEFINE_PACKET_ADD))
        {
            return *this;
        }
        else
        {
            *(uint8*)WritePtr() = n1Data;
            WritePtr((uint32)sizeof(n1Data));
            return *this;
        }
    }
    else
    {
        *(uint8*)WritePtr() = n1Data;
        WritePtr((uint32)sizeof(n1Data));
        return *this;
    }

    return *this;
}

CBuffPacket& CBuffPacket::operator << (int16 n2Data)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    if( (uint32)sizeof(n2Data) + m_u4PacketLen >= m_u4MaxPacketSize)
    {
        OUR_DEBUG((LM_ERROR, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", (uint32)sizeof(n2Data), m_u4PacketLen));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", (uint32)sizeof(n2Data), m_u4PacketLen);
        return *this;
    }

    if(m_u4WritePtr + (uint32)sizeof(n2Data) >= GetPacketSize())
    {
        if(false == AddBuff(DEFINE_PACKET_ADD))
        {
            return *this;
        }
        else
        {
            *(int16*)WritePtr() = n2Data;
            WritePtr((uint32)sizeof(n2Data));
            return *this;
        }
    }
    else
    {
        *(int16*)WritePtr() = n2Data;
        WritePtr((uint32)sizeof(n2Data));
        return *this;
    }

    return *this;
}

CBuffPacket& CBuffPacket::operator << (int32 n4Data)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    if( (uint32)sizeof(n4Data) + m_u4PacketLen >= m_u4MaxPacketSize)
    {
        OUR_DEBUG((LM_ERROR, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", (uint32)sizeof(n4Data), m_u4PacketLen));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", (uint32)sizeof(n4Data), m_u4PacketLen);
        return *this;
    }

    if(m_u4WritePtr + (uint32)sizeof(n4Data) >= GetPacketSize())
    {
        if(false == AddBuff(DEFINE_PACKET_ADD))
        {
            return *this;
        }
        else
        {
            *(int32*)WritePtr() = n4Data;
            WritePtr((uint32)sizeof(n4Data));
            return *this;
        }
    }
    else
    {
        *(int32*)WritePtr() = n4Data;
        WritePtr((uint32)sizeof(n4Data));
        return *this;
    }

    return *this;
}

CBuffPacket& CBuffPacket::operator << (float32 f4Data)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    if( (uint32)sizeof(f4Data) + m_u4PacketLen >= m_u4MaxPacketSize)
    {
        OUR_DEBUG((LM_ERROR, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", (uint32)sizeof(f4Data), m_u4PacketLen));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", (uint32)sizeof(f4Data), m_u4PacketLen);
        return *this;
    }

    if(m_u4WritePtr + (uint32)sizeof(f4Data) >= GetPacketSize())
    {
        if(false == AddBuff(DEFINE_PACKET_ADD))
        {
            return *this;
        }
        else
        {
            *(float32*)WritePtr() = f4Data;
            WritePtr((uint32)sizeof(f4Data));
            return *this;
        }
    }
    else
    {
        *(float32*)WritePtr() = f4Data;
        WritePtr((uint32)sizeof(f4Data));
        return *this;
    }

    return *this;
}

CBuffPacket& CBuffPacket::operator << (float64 f8Data)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    if( (uint32)sizeof(f8Data) + m_u4PacketLen >= m_u4MaxPacketSize)
    {
        OUR_DEBUG((LM_ERROR, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", (uint32)sizeof(f8Data), m_u4PacketLen));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", (uint32)sizeof(f8Data), m_u4PacketLen);
        return *this;
    }

    if(m_u4WritePtr + (uint32)sizeof(f8Data) >= GetPacketSize())
    {
        if(false == AddBuff(DEFINE_PACKET_ADD))
        {
            return *this;
        }
        else
        {
            *(float64*)WritePtr() = f8Data;
            WritePtr((uint32)sizeof(f8Data));
            return *this;
        }
    }
    else
    {
        *(float64*)WritePtr() = f8Data;
        WritePtr((uint32)sizeof(f8Data));
        return *this;
    }

    return *this;
}

CBuffPacket& CBuffPacket::operator << (VCHARS_STR& str)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    if( (uint32)sizeof(str.u1Len) + str.u1Len + m_u4PacketLen >= m_u4MaxPacketSize)
    {
        OUR_DEBUG((LM_ERROR, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", (uint32)sizeof(str.u1Len) + str.u1Len, m_u4PacketLen));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", (uint32)sizeof(str.u1Len) + str.u1Len, m_u4PacketLen);
        return *this;
    }

    if(m_u4WritePtr + (uint32)sizeof(str.u1Len) + str.u1Len >= GetPacketSize())
    {
        if(false == AddBuff((uint32)sizeof(str.u1Len) + str.u1Len))
        {
            return *this;
        }
        else
        {
            //写入字符串长度
            *this << str.u1Len;

            //写入文本内容
            memcpy_safe(str.text, (uint32)str.u1Len, WritePtr(), (uint32)str.u1Len);
            WritePtr(str.u1Len);
            return *this;
        }
    }
    else
    {
        //写入字符串长度
        *this << str.u1Len;

        //写入文本内容
        memcpy_safe(str.text, (uint32)str.u1Len, WritePtr(), (uint32)str.u1Len);
        WritePtr(str.u1Len);
        return *this;
    }

    return *this;
}

CBuffPacket& CBuffPacket::operator << (VCHARM_STR& str)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    if( (uint32)sizeof(str.u2Len) + str.u2Len + m_u4PacketLen >= m_u4MaxPacketSize)
    {
        OUR_DEBUG((LM_ERROR, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", (uint32)sizeof(str.u2Len) + str.u2Len, m_u4PacketLen));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", (uint32)sizeof(str.u2Len) + str.u2Len, m_u4PacketLen);
        return *this;
    }

    if(m_u4WritePtr + (uint32)sizeof(str.u2Len) + str.u2Len >= GetPacketSize())
    {
        if(false == AddBuff((uint32)sizeof(str.u2Len) + str.u2Len))
        {
            return *this;
        }
        else
        {
            //写入字符串长度
            *this << str.u2Len;

            //写入文本内容
            memcpy_safe(str.text, (uint32)str.u2Len, WritePtr(), (uint32)str.u2Len);
            WritePtr(str.u2Len);
            return *this;
        }
    }
    else
    {
        //写入字符串长度
        *this << str.u2Len;

        //写入文本内容
        memcpy_safe(str.text, (uint32)str.u2Len, WritePtr(), (uint32)str.u2Len);
        WritePtr(str.u2Len);
        return *this;
    }

    return *this;
}

CBuffPacket& CBuffPacket::operator << (VCHARB_STR& str)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    if((uint32)sizeof(str.u4Len) + str.u4Len + m_u4PacketLen >= m_u4MaxPacketSize)
    {
        OUR_DEBUG((LM_ERROR, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", (uint32)sizeof(str.u4Len) + str.u4Len, m_u4PacketLen));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", (uint32)sizeof(str.u4Len) + str.u4Len, m_u4PacketLen);
        return *this;
    }

    if(m_u4WritePtr + (uint32)sizeof(str.u4Len) + str.u4Len >= GetPacketSize())
    {
        if(false == AddBuff((uint32)sizeof(str.u4Len) + str.u4Len))
        {
            return *this;
        }
        else
        {
            //写入字符串长度
            *this << str.u4Len;

            //写入文本内容
            memcpy_safe(str.text, (uint32)str.u4Len, WritePtr(), (uint32)str.u4Len);
            WritePtr(str.u4Len);
            return *this;
        }
    }
    else
    {
        //写入字符串长度
        *this << str.u4Len;

        //写入文本内容
        memcpy_safe(str.text, (uint32)str.u4Len, WritePtr(), (uint32)str.u4Len);
        WritePtr(str.u4Len);
        return *this;
    }

    return *this;
}

CBuffPacket& CBuffPacket::operator << (string& str)
{
    //ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);
    uint32 u4Len = (uint32)str.length();

    if((uint32)sizeof(u4Len) + u4Len + m_u4PacketLen >= m_u4MaxPacketSize)
    {
        OUR_DEBUG((LM_ERROR, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", (uint32)sizeof(u4Len) + u4Len, m_u4PacketLen));
        sprintf_safe(m_szError, MAX_BUFF_500, "[CBuffPacket::operator << ] nSize = [%d] m_u4PacketLen = [%d] is more than m_u4MaxPacketSize.\n", (uint32)sizeof(u4Len) + u4Len, m_u4PacketLen);
        return *this;
    }

    if(m_u4WritePtr + (uint32)sizeof(u4Len) + u4Len >= GetPacketSize())
    {
        if(false == AddBuff((uint32)sizeof(u4Len) + u4Len))
        {
            return *this;
        }
        else
        {
            //写入字符串长度
            *this << u4Len;

            //写入文本内容
            memcpy_safe((char* )str.c_str(), u4Len, WritePtr(), u4Len);
            WritePtr(u4Len);
            return *this;
        }
    }
    else
    {
        //写入字符串长度
        *this << u4Len;

        //写入文本内容
        memcpy_safe((char* )str.c_str(), u4Len, WritePtr(), u4Len);
        WritePtr(u4Len);
        return *this;
    }

    return *this;
}

void CBuffPacket::SetNetSort(bool blState)
{
    m_blNetSort = blState;
}

void CBuffPacket::SetBuffID(uint32 u4BuffID)
{
    m_u4BuffID = u4BuffID;
}

uint32 CBuffPacket::GetBuffID()
{
    return m_u4BuffID;
}

void CBuffPacket::SetHashID(int32 nHashID)
{
    m_nHashID = nHashID;
}

int32 CBuffPacket::GetHashID()
{
    return m_nHashID;
}


