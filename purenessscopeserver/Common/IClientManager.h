#ifndef _ICLIENTMANAGER_H
#define _ICLIENTMANAGER_H

#include "ClientMessage.h"
#include "ClientUDPMassage.h"

//Server 2 server
enum class EM_S2S_Run_State
{
    S2S_Run_State_Init = 0,
    S2S_Run_State_Run,
};

//��¼׼�����ӵķ�������������Ϣ
class CS2SConnectGetRandyInfo
{
public:
    CS2SConnectGetRandyInfo() 
    {
    };

    uint32 m_nServerID               = 0;
    string m_strServerIP;
    uint16 m_u2ServerPort            = 0;
    uint8 m_u1Type                   = 0;
    string m_strLocalIP;
    uint16 m_u2LocalPort             = 0;
    uint8 m_u1LocalIPType            = 0;
    IClientMessage* m_pClientMessage = NULL;
    uint32 m_u4PacketParseID         = 0;
};

//��������������ͨѶ�����ݹ���
class IClientManager
{
public:
    virtual ~IClientManager() {}

    //����TCP���Ӳ�����pClientMessageΪԶ�����ݵ��ﴦ���ࡣ
    virtual bool Connect(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, IClientMessage* pClientMessage, uint32 u4PacketParseID) = 0;
    //����TCP���Ӳ�����pClientMessageΪԶ�����ݵ��ﴦ���࣬�������ñ���IP�Ͷ˿ڡ�
    virtual bool Connect(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, const char* pLocalIP, uint16 u2LocalPort, uint8 u1LocalIPType, IClientMessage* pClientMessage, uint32 u4PacketParseID) = 0;

    //����TCP���Ӳ�����pClientMessageΪԶ�����ݵ��ﴦ���ࡣ
    virtual bool ConnectFrame(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, uint32 u4PacketParse) = 0;
    //����TCP���Ӳ�����pClientMessageΪԶ�����ݵ��ﴦ���࣬�������ñ���IP�Ͷ˿ڡ�
    virtual bool ConnectFrame(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, const char* pLocalIP, uint16 u2LocalPort, uint8 u1LocalIPType, uint32 u4PacketParse) = 0;

    //����UDP���Ӳ�����pClientUDPMessageΪԶ�����ݵ��ﴦ���ࡣ
    virtual bool ConnectUDP(int nServerID, const char* pIP, uint16 u2Port, uint8 u1IPType, EM_UDP_TYPE emType, IClientUDPMessage* pClientUDPMessage) = 0;
    //�ر�ĳһ��ServerID��Ӧ��TCP����
    virtual bool Close(int nServerID)                                                                                                 = 0;
    //�ر�ĳһ��ServerID��Ӧ��UDP����
    virtual bool CloseUDP(int nServerID)                                                                                              = 0;
    //����һ��TCP�����ݰ�������������blIsDelete�������Ƿ��ɿ�ܻ��գ������߼����գ�����ʹ��CBuffPacket����Ϊ���ڴ�أ��������ﲻ��ɾ��
    virtual bool SendData(int nServerID, char*& pData, int nSize, bool blIsDelete = true)                                             = 0;
    //����һ��UDP�����ݰ�������������blIsDelete�������Ƿ��ɿ�ܻ��գ������߼����գ�����ʹ��CBuffPacket����Ϊ���ڴ�أ��������ﲻ��ɾ��
    virtual bool SendDataUDP(int nServerID, const char* pIP, uint16 u2Port, char*& pMessage, uint32 u4Len, bool blIsDelete = true)  = 0;
    //���Ӵ���飬������������ڲ��������ݰ���ʱ��Ͽ��ˣ�����Զ��ؽ�
    virtual bool StartConnectTask(int nIntervalTime)                                                                                  = 0;
    //�ر����Ӵ����
    virtual void CancelConnectTask()                                                                                                  = 0;
    //�ر����ж������Ӱ���TCP��UDP
    virtual void Close()                                                                                                              = 0;
    //�õ�һ�����ӵĵ�ǰ״̬
    virtual EM_Server_Connect_State GetConnectState(int nServerID)                                                                    = 0;
    //���һ��ServerID��Ӧ��pClientMessage������Ϣ
    virtual IClientMessage* GetClientMessage(int nServerID)                                                                           = 0;
    //���һ��ServerID��Ӧ��PacketParseID
    virtual uint32 GetPacketParseID(int nServerID)                                                                                    = 0;
    //���һ��ServerID��Ӧ��IP��Ϣ
    virtual bool GetServerIPInfo(int nServerID, _ClientIPInfo& objServerIPInfo)                                                       = 0;
    //ɾ��һ������ʹ�õ�IClientMessage
    virtual bool DeleteIClientMessage(IClientMessage* pClientMessage)                                                                 = 0;
};

#endif
