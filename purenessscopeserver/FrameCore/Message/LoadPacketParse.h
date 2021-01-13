#ifndef _LOADPACKETPARSE_H
#define _LOADPACKETPARSE_H

#include "ace/Date_Time.h"
#include "ace/Thread_Mutex.h"
#include "ace/Singleton.h"
#include "ace/OS_NS_dlfcn.h"

#include "define.h"
#include "IMessageBlockManager.h"
#include <unordered_map>
#include "TimeStamp.hpp"
#include "LoadLibrary.hpp"

class _Packet_Parse_Info
{
public:
    uint32              m_u4PacketParseID     = 0;       //当前packetParseID
    uint8               m_u1PacketParseType   = PACKET_WITHHEAD;     //包头包体或者流模式
    uint32              m_u4OrgLength         = 0;           //数据包头长度
    PSS_Time_Point      m_tvCreateTime        = CTimeStamp::Get_Time_Stamp();          //模块创建时间
    Pss_Library_Handler m_hModule             = nullptr;
    bool (*Parse_Packet_Head_Info)(uint32 u4ConnectID, ACE_Message_Block* pmbHead, IMessageBlockManager* pMessageBlockManager, _Head_Info* pHeadInfo, EM_CONNECT_IO_TYPE emIOType)        = nullptr;
    bool (*Parse_Packet_Body_Info)(uint32 u4ConnectID, ACE_Message_Block* pmbbody, IMessageBlockManager* pMessageBlockManager, _Body_Info* pBodyInfo, EM_CONNECT_IO_TYPE emIOType)        = nullptr;
    uint8 (*Parse_Packet_Stream)(uint32 u4ConnectID, ACE_Message_Block* pCurrMessage, IMessageBlockManager* pMessageBlockManager, _Packet_Info* pPacketInfo, EM_CONNECT_IO_TYPE emIOType) = nullptr;
    bool (*Make_Send_Packet)(uint32 u4ConnectID, const char* pData, uint32 u4Len, ACE_Message_Block* pMbData, uint16 u2CommandID) = nullptr;
    uint32 (*Make_Send_Packet_Length)(uint32 u4ConnectID, uint32 u4DataLen, uint16 u2CommandID) = nullptr;
    bool (*Connect)(uint32 u4ConnectID, _ClientIPInfo objClientIPInfo, _ClientIPInfo const& objLocalIPInfo) = nullptr;
    void (*DisConnect)(uint32 u4ConnectID) = nullptr;
    void(*Close)() = nullptr;
    void(*Set_output)(shared_ptr<spdlog::logger> logger) = nullptr;

    _Packet_Parse_Info() = default;
};

class CLoadPacketParse
{
public:
    CLoadPacketParse() = default;

    void Init(int nCount);

    bool LoadPacketInfo(uint32 u4PacketParseID, uint8 u1Type, uint32 u4HeadLen, const char* pPacketParsePath, const char* szPacketParseName);

    void Close();

    shared_ptr<_Packet_Parse_Info> GetPacketParseInfo(uint32 u4PacketParseID);

private:
    using hashmapPacketParseModuleList = unordered_map<uint32, shared_ptr<_Packet_Parse_Info>>;
    hashmapPacketParseModuleList        m_objPacketParseList;                  //Hash内存池
    int m_nModuleCount = 0;
};

using App_PacketParseLoader = PSS_singleton<CLoadPacketParse>;

#endif
