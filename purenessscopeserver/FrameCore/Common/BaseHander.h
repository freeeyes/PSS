#ifndef _BASEREACTORHANDLER_H
#define _BASEREACTORHANDLER_H

#include "ace/Task_T.h"

#include "define.h"
#include "MakePacket.h"
#include "SendMessage.h"
#include "MessageBlockManager.h"
#include "LoadPacketParse.h"
#include "PacketParse.h"
#include "FileTest.h"
#include "ace/SOCK_Dgram.h"
#include "XmlConfig.h"
#include "TcpRedirection.h"
#include "MakePacket_Common.h"
#include "IPAccount.h"
#include <string>
#include <sstream>
#include <iomanip>

//���е�handler�õ��Ĺ�������
//add by freeeyes


//�����ڴ�
void Recovery_Message(bool blDelete, char*& pMessage);

//Udp_Common_Send_Message�Ĵ���
class _Send_Message_Param
{
public:
    uint32 m_u4PacketParseInfoID      = 0;
    uint32 m_u4SendLength             = 0;
    string m_strClientIP;
    uint16 m_u2Port                   = 0;
    uint16 m_u2CommandID              = 0;
    bool   m_blDlete                  = false;
    EM_SEND_PACKET_PARSE m_emSendType = EM_SEND_PACKET_PARSE::EM_SENDMESSAGE_JAMPNOMAL;
};

//�����ݷ����빤���߳���Ϣ����
void Send_MakePacket_Queue(_MakePacket const& objMakePacket);

//udp�����������ݰ��ϳɺ���
bool Udp_Common_Send_Message(_Send_Message_Param const& obj_Send_Message_Param, shared_ptr<IBuffPacket> pBuffPacket, const ACE_SOCK_Dgram& skRemote, shared_ptr<_Packet_Parse_Info> pPacketParseInfo, ACE_Message_Block* pBlockMessage);

//udp������Ϣͷ����
bool Udp_Common_Recv_Head(uint32 u4ConnectID, ACE_Message_Block* pMBHead, shared_ptr<CPacketParse> pPacketParse, shared_ptr<_Packet_Parse_Info> pPacketParseInfo, uint32 u4Len);

//udp������Ϣ�庯��
bool Udp_Common_Recv_Body(uint32 u4ConnectID, ACE_Message_Block* pMBBody, shared_ptr<CPacketParse> pPacketParse, shared_ptr<_Packet_Parse_Info> pPacketParseInfo);

//udp����Ϣ����
bool Udp_Common_Recv_Stream(uint32 u4ConnectID, ACE_Message_Block* pMbData, shared_ptr<CPacketParse> pPacketParse, shared_ptr<_Packet_Parse_Info> pPacketParseInfo);

//�ύudp���ݵ������߳�
bool Udp_Common_Send_WorkThread(uint32 u4ConnectID, shared_ptr<CPacketParse> pPacketParse, const ACE_INET_Addr& addrRemote, const ACE_INET_Addr& addrLocal, const ACE_Time_Value& tvCheck);

//�������ݻ���
void Recovery_Common_BuffPacket(bool blDelete, shared_ptr<IBuffPacket> pBuffPacket);

//������Ϣ���ع����߳�
void Tcp_Common_Send_Message_Error(uint32 u4ConnectID, uint16 u2CommandID, bool blDelete, shared_ptr<IBuffPacket> pBuffPacket);

//TCP����Ϣ����
uint8 Tcp_Common_Recv_Stream(uint32 u4ConnectID, ACE_Message_Block* pMbData, shared_ptr<CPacketParse> pPacketParse, uint32 u4PacketParseInfoID);

//��¼��־����
void Output_Debug_Data(const ACE_Message_Block* pMbData, uint16 u2LogType, const ACE_INET_Addr& addrRemote);

//File_Message�Ĳ���
class _File_Message_Param
{
public:
    uint32 m_u4ConnectID          = 0;
    IFileTestManager* m_pFileTest = nullptr;
    bool m_blDelete               = false;
    uint32 m_u4PacketDebugSize    = 0;
    ACE_INET_Addr m_addrRemote;

    _File_Message_Param() = default;
};

//�ļ�ѹ�������빤���߳�
bool Tcp_Common_File_Message(_File_Message_Param const& obj_File_Message_Param, shared_ptr<IBuffPacket> pBuffPacket, const char* pConnectName);

//Tcp_Common_ClientInfo�Ĳ���
class _ClientConnectInfo_Param
{
public:
    uint32 m_u4ConnectID         = 0;
    ACE_INET_Addr m_addrRemote;
    uint32 m_u4AllRecvCount      = 0;
    uint32 m_u4AllSendCount      = 0;
    uint32 m_u4AllRecvSize       = 0;
    uint32 m_u4AllSendSize       = 0;
    ACE_Time_Value m_atvConnect; 
    uint32 m_u4RecvQueueCount    = 0;

    _ClientConnectInfo_Param() = default;
};

//���ص�ǰHandler����Ϣ
_ClientConnectInfo Tcp_Common_ClientInfo(_ClientConnectInfo_Param const& obj_ClientConnectInfo_Param);

//Tcp_Common_Send_Input_To_Cache�Ĳ���
class _Input_To_Cache_Param
{
public:
    uint32 m_u4ConnectID              = 0;
    uint32 m_u4PacketParseInfoID      = 0;
    uint32 m_u4SendMaxBuffSize        = 0;
    EM_SEND_PACKET_PARSE m_emSendType = EM_SEND_PACKET_PARSE::EM_SENDMESSAGE_JAMPNOMAL;
    uint16 m_u2CommandID              = 0;
    bool m_blDelete                   = false;

    _Input_To_Cache_Param() = default;
};

//����������뷢�ͻ�����
bool Tcp_Common_Send_Input_To_Cache(_Input_To_Cache_Param obj_Input_To_Cache_Param,
    ACE_Message_Block* pBlockMessage, uint32& u4PacketSize,
    shared_ptr<IBuffPacket> pBuffPacket);

//Tcp_Common_Make_Send_Packet�Ĳ���
class _Send_Packet_Param
{
public:
    uint32 m_u4ConnectID               = 0;
    EM_SEND_PACKET_PARSE m_emSendType  = EM_SEND_PACKET_PARSE::EM_SENDMESSAGE_JAMPNOMAL;
    uint32 m_u4PacketParseInfoID       = 0;
    uint32 m_u4SendMaxBuffSize         = 0;
    uint16 m_u2CommandID               = 0;
    bool m_blDelete                    = false;

    _Send_Packet_Param() = default;
};

//��װ��������
bool Tcp_Common_Make_Send_Packet(_Send_Packet_Param obj_Send_Packet_Param,
    shared_ptr<IBuffPacket> pBuffPacket,
    ACE_Message_Block* pBlockMessage,
    uint32& u4PacketSize);

//Tcp_Common_Manager_Post_Message�Ĳ���
class _Post_Message_Param
{
public:
    uint32 m_u4ConnectID         = 0;
    uint8 m_u1SendType           = 0;
    uint16 m_u2CommandID         = 0;
    uint8 m_u1SendState          = 0;
    bool m_blDelete              = false;
    int m_nMessageID             = 0;
    uint16 m_u2SendQueueMax      = 0;
    uint32 m_u4SendQueuePutTime  = 0;

    _Post_Message_Param() = default;
};

//��ʱ�����ͳ����־
void Tcp_Common_Manager_Timeout_CheckInfo(int nActiveConnectCount);

//�����������
_ClientNameInfo Tcp_Common_ClientNameInfo(uint32 u4ConnectID, const char* pConnectName, const char* pClientIP, uint16 u2ClientPort, bool IsLog);

#endif
