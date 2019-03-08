#ifndef _ICONNECTMANAGER_H
#define _ICONNECTMANAGER_H

#include "IBuffPacket.h"
#include <vector>

enum EM_Client_Connect_status
{
    CLIENT_CONNECT_EXIST = 0,
    CLIENT_CONNECT_NO_EXIST,
};

enum EM_Client_Close_status
{
    CLIENT_CLOSE_NOTHING = 0,  //ʲô������
    CLIENT_CLOSE_IMMEDIATLY,   //�����ر�����
    CLIENT_CLOSE_SENDOK,       //������ɹر�����
};

class IConnectManager
{
public:
    virtual ~IConnectManager() {}
    /*
    ����˵��
    u4ConnectID ��Ҫ�·���Ӧ��ConnectID
    pBuffPacket һ�����������ݣ�pDataͬ��
    u1SendType  �Ƿ������ݰ������ͣ�Ŀǰ����SENDMESSAGE_NOMAL(���ݰ�ͨ��packetParse����)��SENDMESSAGE_JAMPNOMAL(ֱ�ӷ��͸����ݰ�)
    u2CommandID ��Ҫ���Ͱ���Ӧ������ID�����ڿ�ܶ����ݰ���ͳ�ơ�
    u1SendState �������¼���״̬PACKET_SEND_IMMEDIATLY(���̷���),PACKET_SEND_CACHE(���ݰ����ͻ���)��PACKET_SEND_FIN_CLOSE(���̷��ͣ�������ɺ�ر�����)
    blDlete     �Ƿ��ɿ��ɾ�����ݰ��ڴ�飬true�ǿ��ɾ����false�ɲ���Լ������ڴ����ͷš�
    nMessageID  ��������������ݻ�ִ�����ID > 0����������ͳɹ�����ص�CLINET_LINK_SENDOK��������Թ�ע����¼���ý��������ʧ�ܻ����CLINET_LINK_SENDERROR
    */
    //�첽����ָ�������ݰ�(����PacketParse�����Ͱ�) IBuffPacket �����ڲ�������������
    virtual bool PostMessage(uint32 u4ConnectID, IBuffPacket*& pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL,
                             uint16 u2CommandID = 0, uint8 u1SendState = 0, bool blDlete = true, int nMessageID = 0)   = 0;
    //�첽����ָ�������ݰ�(������PacketParse�����Ͱ�) pData�����治�������٣���Ҫ���߼�ģ�����д���
    virtual bool PostMessage(uint32 u4ConnectID, char*& pData, uint32 nDataLen, uint8 u1SendType = SENDMESSAGE_NOMAL,
                             uint16 u2CommandID = 0, uint8 u1SendState = 0, bool blDlete = true, int nMessageID = 0)   = 0;
    //�첽Ⱥ��ָ�������ݰ�(����PacketParse�����Ͱ�) IBuffPacket �����ڲ�������������
    virtual bool PostMessage(vector<uint32> vecConnectID, IBuffPacket*& pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL,
                             uint16 u2CommandID = 0, uint8 u1SendState = 0, bool blDlete = true, int nMessageID = 0)   = 0;
    //�첽Ⱥ��ָ�������ݰ�(������PacketParse�����Ͱ�) pData�����治�������٣���Ҫ���߼�ģ�����д���
    virtual bool PostMessage(vector<uint32> vecConnectID, char*& pData, uint32 nDataLen, uint8 u1SendType = SENDMESSAGE_NOMAL,
                             uint16 u2CommandID = 0, uint8 u1SendState = 0, bool blDlete = true, int nMessageID = 0)   = 0;
    //�첽ȫ��ָ�������ݰ�(����PacketParse�����Ͱ�) IBuffPacket �����ڲ�������������
    virtual bool PostMessageAll(IBuffPacket*& pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL,
                                uint16 u2CommandID = 0,uint8 u1SendState = 0, bool blDlete = true, int nMessageID = 0)    = 0;
    //�첽ȫ��ָ�������ݰ�(������PacketParse�����Ͱ�) pData�����治�������٣���Ҫ���߼�ģ�����д���
    virtual bool PostMessageAll(char*& pData, uint32 nDataLen, uint8 u1SendType = SENDMESSAGE_NOMAL,
                                uint16 u2CommandID = 0, uint8 u1SendState = 0, bool blDlete = true, int nMessageID = 0)   = 0;
    //�������ر�����
    virtual bool CloseConnect(uint32 u4ConnectID)                                                                = 0;
    //�õ���ǰ������ϢIP�Ͷ˿�
    virtual _ClientIPInfo GetClientIPInfo(uint32 u4ConnectID)                                                    = 0;
    //�õ���ǰ���Ӽ���IP�Ͷ˿�
    virtual _ClientIPInfo GetLocalIPInfo(uint32 u4ConnectID)                                                     = 0;
    //�õ���ǰ��������
    virtual int  GetCount()                                                                                      = 0;
    //�������ConnectID��Name��ӳ���ϵ
    virtual bool SetConnectName(uint32 u4ConnectID, const char* pName)                                           = 0;
    //�������ConnectID�Ƿ����־
    virtual bool SetIsLog(uint32 u4ConnectID, bool blIsLog)                                                      = 0;
    //�õ�ָ��Name�����ConnectIDӳ����Ϣ
    virtual void GetClientNameInfo(const char* pName, vecClientNameInfo& objClientNameInfo)                      = 0;
    //�õ�ָ���Ŀͻ�������״̬,falseΪ���Ӳ����ڣ�trueΪ���Ӵ���
    virtual EM_Client_Connect_status GetConnectState(uint32 u4ConnectID)                                         = 0;
};

#endif
