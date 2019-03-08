#ifndef _ICLIENTMANAGER_H
#define _ICLIENTMANAGER_H

#include "ClientMessage.h"
#include "ClientUDPMassage.h"

//��������������ͨѶ�����ݹ���
class IClientManager
{
public:
    virtual ~IClientManager() {}

    //����TCP���Ӳ�����pClientMessageΪԶ�����ݵ��ﴦ���ࡣ
    virtual bool Connect(int nServerID, const char* pIP, int nPort, uint8 u1IPType, IClientMessage* pClientMessage)                  = 0;
    //����TCP���Ӳ�����pClientMessageΪԶ�����ݵ��ﴦ���࣬�������ñ���IP�Ͷ˿ڡ�
    virtual bool Connect(int nServerID, const char* pIP, int nPort, uint8 u1IPType, const char* pLocalIP, int nLocalPort, uint8 u1LocalIPType, IClientMessage* pClientMessage) = 0;
    //����UDP���Ӳ�����pClientUDPMessageΪԶ�����ݵ��ﴦ���ࡣ
    virtual bool ConnectUDP(int nServerID, const char* pIP, int nPort, uint8 u1IPType, EM_UDP_TYPE emType, IClientUDPMessage* pClientUDPMessage) = 0;
    //�ر�ĳһ��ServerID��Ӧ��TCP����
    virtual bool Close(int nServerID)                                                                                                 = 0;
    //�ر�ĳһ��ServerID��Ӧ��UDP����
    virtual bool CloseUDP(int nServerID)                                                                                              = 0;
    //����һ��TCP�����ݰ�������������blIsDelete�������Ƿ��ɿ�ܻ��գ������߼����գ�����ʹ��CBuffPacket����Ϊ���ڴ�أ��������ﲻ��ɾ��
    virtual bool SendData(int nServerID, char*& pData, int nSize, bool blIsDelete = true)                                             = 0;
    //����һ��UDP�����ݰ�������������blIsDelete�������Ƿ��ɿ�ܻ��գ������߼����գ�����ʹ��CBuffPacket����Ϊ���ڴ�أ��������ﲻ��ɾ��
    virtual bool SendDataUDP(int nServerID, const char* pIP, int nPort, char*& pMessage, uint32 u4Len, bool blIsDelete = true)  = 0;
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
    //���һ��ServerID��Ӧ��IP��Ϣ
    virtual bool GetServerIPInfo(int nServerID, _ClientIPInfo& objServerIPInfo)                                                       = 0;
    //ɾ��һ������ʹ�õ�IClientMessage
    virtual bool DeleteIClientMessage(IClientMessage* pClientMessage)                                                                 = 0;
};

#endif
