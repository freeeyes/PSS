#ifndef _LOADPACKETPARSE_H
#define _LOADPACKETPARSE_H

#include "ace/Date_Time.h"
#include "ace/Thread_Mutex.h"
#include "ace/Singleton.h"
#include "ace/OS_NS_dlfcn.h"

#include "define.h"
#include "HashTable.h"
#include "IMessageBlockManager.h"

class _Packet_Parse_Info
{
public:
    uint32           m_u4PacketParseID;       //当前packetParseID
    uint8            m_u1PacketParseType;     //包头包体或者流模式
    uint32           m_u4OrgLength;           //数据包头长度
    ACE_Date_Time    m_dtCreateTime;          //模块创建时间
    ACE_SHLIB_HANDLE m_hModule;
    bool (*Parse_Packet_Head_Info)(uint32 u4ConnectID, ACE_Message_Block* pmbHead, IMessageBlockManager* pMessageBlockManager, _Head_Info* pHeadInfo, EM_CONNECT_IO_TYPE emIOType);
    bool (*Parse_Packet_Body_Info)(uint32 u4ConnectID, ACE_Message_Block* pmbbody, IMessageBlockManager* pMessageBlockManager, _Body_Info* pBodyInfo, EM_CONNECT_IO_TYPE emIOType);
    uint8 (*Parse_Packet_Stream)(uint32 u4ConnectID, ACE_Message_Block* pCurrMessage, IMessageBlockManager* pMessageBlockManager, _Packet_Info* pPacketInfo, EM_CONNECT_IO_TYPE emIOType);
    bool (*Make_Send_Packet)(uint32 u4ConnectID, const char* pData, uint32 u4Len, ACE_Message_Block* pMbData, uint16 u2CommandID);
    uint32 (*Make_Send_Packet_Length)(uint32 u4ConnectID, uint32 u4DataLen, uint16 u2CommandID);
    bool (*Connect)(uint32 u4ConnectID, _ClientIPInfo objClientIPInfo, _ClientIPInfo const& objLocalIPInfo);
    void (*DisConnect)(uint32 u4ConnectID);
    void(*Close)();

    _Packet_Parse_Info()
    {
        m_u4PacketParseID       = 0;
        m_u1PacketParseType     = PACKET_WITHHEAD;
        m_u4OrgLength           = 0;
        m_hModule               = NULL;
        Parse_Packet_Head_Info  = NULL;
        Parse_Packet_Body_Info  = NULL;
        Parse_Packet_Stream     = NULL;
        Make_Send_Packet        = NULL;
        Make_Send_Packet_Length = NULL;
        Connect                 = NULL;
        DisConnect              = NULL;
        Close                   = NULL;
    }
};

class CLoadPacketParse
{
public:
    CLoadPacketParse();
    ~CLoadPacketParse();

    CLoadPacketParse(const CLoadPacketParse& ar);

    CLoadPacketParse& operator = (const CLoadPacketParse& ar)
    {
        if (this != &ar)
        {
            ACE_UNUSED_ARG(ar);
        }

        return *this;
    }

    void Init(int nCount);

    bool LoadPacketInfo(uint32 u4PacketParseID, uint8 u1Type, uint32 u4HeadLen, const char* pPacketParsePath, const char* szPacketParseName);

    void Close();

    _Packet_Parse_Info* GetPacketParseInfo(uint32 u4PacketParseID);

private:
    CHashTable<_Packet_Parse_Info>        m_objPacketParseList;                  //Hash内存池
};

typedef ACE_Singleton<CLoadPacketParse, ACE_Null_Mutex> App_PacketParseLoader;

#endif
