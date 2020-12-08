// ConnectHandle.h
// ����ͻ�������
// ���ò�˵��������һ�����Ŀ��飬���������������Ż���reactor�ܹ�����Ȼ������Ӧ����1024������������
// ����ACE�ܹ�����⣬ʹ�����µ���ʶ����Ȼ�����ڵ��ǣ��滻�ܹ��Ĳ��֣�ֻ�з��ͺͽ��ܲ��֡��������ֶ����Ա�����
// �Ѿ��Ļ��������Ķ�������Ȼ���Լ��񶨣���һ����ʹ��Ĺ��̣���Ȼ����������ĸ��ã����Ǳ���Ĵ��ۡ�
// �����Լ��ܹ����ĸ��ã��������⣬������⼴�ɡ�
// ���Ͱɣ����������ġ�
// add by freeeyes
// 2009-08-23
//Bobo�����һ�������⣬���IOCP�£����ͺͽ��ղ��Եȣ�������ڴ����������������
//�������ܺã���������ǣ���������ˮλ�꣬�������ˮλ���೬����һ������ֵ����ô�ͶϿ����ӡ�
//linux�²�����������⣬ֻ��IOCP�³��֡�
//add by freeyes
//2013-09-18

#ifndef _PROCONNECTHANDLE_H
#define _PROCONNECTHANDLE_H

#include "ace/Svc_Handler.h"
#include "ace/Synch.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Asynch_IO.h"
#include "ace/Asynch_Acceptor.h"
#include "ace/Proactor.h"

#include "IHandler.h"
#include "BaseHander.h"
#include "BaseTask.h"
#include "ObjectArrayList.h"
#include "HashTable.h"
#include "AceProactorManager.h"
#include "IConnectManager.h"
#include "TimerManager.h"
#include "BuffPacketManager.h"
#include "Fast_Asynch_Read_Stream.h"
#include "ForbiddenIP.h"
#include "IPAccount.h"
#include "SendMessage.h"
#include "CommandAccount.h"
#include "TimeWheelLink.h"
#include "FileTest.h"
#include "XmlConfig.h"
#include "TcpRedirection.h"
#include "PerformanceCounter.h"
#include "ConnectCounter.h"

class CProConnectHandler : public ACE_Service_Handler, public IDeviceHandler, public IHandler
{
public:
    CProConnectHandler(void);
    virtual ~CProConnectHandler(void);

    //��д�̳з���
    virtual void open(ACE_HANDLE h, ACE_Message_Block&);                                             //�û�����һ������
    virtual void handle_read_stream(const ACE_Asynch_Read_Stream::Result& result);                   //������ܵ��û����ݰ���Ϣ�¼�
    virtual void handle_write_stream(const ACE_Asynch_Write_Stream::Result& result);                 //�����͵��û�������ɵ��¼�
    virtual void addresses(const ACE_INET_Addr& remote_address, const ACE_INET_Addr& local_address); //��õ�ǰԶ�̿ͻ��˵�IP��ַ��Ϣ
    virtual bool Device_Send_Data(const char* pData, ssize_t nLen);                                  //͸�����ݽӿ�

    uint32 file_open(IFileTestManager* pFileTest);                                           //�ļ���ڴ򿪽ӿ�
    int handle_write_file_stream(const char* pData, uint32 u4Size, uint8 u1ParseID);         //�ļ��ӿ�ģ�����ݰ����

    void Init(uint16 u2HandlerID);                                                           //Connect Pool��ʼ�����õĺ���

    virtual bool SendMessage(const CSendMessageInfo& objSendMessageInfo, uint32& u4PacketSize);     //���͸��ͻ������ݵĺ���
    virtual void Close(uint32 u4ConnectID);                                                  //��ǰ���Ӷ���ر�
    void SetLocalIPInfo(const char* pLocalIP, uint16 u2LocalPort);                           //���ü���IP�Ͷ˿���Ϣ

    uint32             GetHandlerID();                                        //�õ���ǰ��ʼ����HanddlerID
    void               SetConnectID(uint32 u4ConnectID);                      //���õ�ǰ���ӵ�ID
    uint32             GetConnectID();                                        //��õ�ǰ���ӵ�ID
    CONNECTSTATE       GetConnectState();                                     //�õ�����״̬
    CONNECTSTATE       GetSendBuffState();                                    //�õ�����״̬
    _ClientConnectInfo GetClientInfo();                                       //�õ��ͻ�����Ϣ
    _ClientIPInfo      GetClientIPInfo();                                     //�õ��ͻ���IP��Ϣ
    _ClientIPInfo      GetLocalIPInfo();                                      //�õ�����IP��Ϣ
    void               SetConnectName(const char* pName);                     //���õ�ǰ��������
    char*              GetConnectName();                                      //�õ�����
    virtual void       SetIsLog(bool blIsLog);                                //���õ�ǰ���������Ƿ�д����־
    bool               GetIsLog();                                            //�õ��Ƿ����д��־
    void               SetHashID(int nHashID);                                //����HashID
    int                GetHashID();                                           //�õ���ǰHashID

    void SetPacketParseInfoID(uint32 u4PacketParseInfoID);                    //���ö�Ӧ��m_u4PacketParseInfoID
    uint32 GetPacketParseInfoID();                                            //�����Ӧ��m_u4PacketParseInfoID
    bool SendTimeoutMessage();                                                //�������ӳ�ʱ��Ϣ
    virtual bool PutSendPacket(uint32 u4ConnectID, ACE_Message_Block* pMbData, uint32 u4Size, const ACE_Time_Value& tvSend);       //���������ݷ��ͳ�ȥ

private:
    bool Dispose_Recv_buffer();                                              //������յ����ݣ��а�
    void Move_Recv_buffer();                                                 //��������ڴ滺����
    void Send_Hander_Event(uint8 u1Option);                                  //����Handler���¼�֪ͨҵ���߳�
    void Get_Recv_length();                                                  //�õ�Ҫ��������ݳ���
    int  Dispose_Paceket_Parse_Head(ACE_Message_Block* pmb);                 //������Ϣͷ����
    int  Dispose_Paceket_Parse_Body(ACE_Message_Block* pmb, uint32 u4SrcBodyLength);                 //������Ϣͷ����
    int  Dispose_Paceket_Parse_Stream(ACE_Message_Block* pCurrMessage);      //��������Ϣ����

    bool Write_SendData_To_File(bool blDelete, shared_ptr<IBuffPacket> pBuffPacket);                  //����������д���ļ�
    bool Send_Input_To_Cache(CSendMessageInfo objSendMessageInfo, uint32& u4PacketSize);              //�����Ͷ�����뻺��
    bool Send_Input_To_TCP(CSendMessageInfo objSendMessageInfo, uint32& u4PacketSize);                //�����ݷ��͸��Զ�

    bool RecvClinetPacket();                                                                          //�������ݰ�
    bool CheckMessage();                                                                              //������յ�����

    void ClearPacketParse();                                                                          //��������ʹ�õ�PacketParse
    void PutSendPacketError(ACE_Message_Block* pMbData);                                              //����ʧ�ܻص�

    uint32             m_u4MaxPacketSize;              //�������ݰ�����󳤶�
    uint32             m_u4RecvQueueCount;             //��ǰ���ӱ���������ݰ���
    uint32             m_u4HandlerID;                  //��Hander����ʱ��ID
    uint32             m_u4ConnectID;                  //��ǰConnect����ˮ��
    uint32             m_u4AllRecvSize;                //��ǰ���ӽ����ֽ�����
    uint32             m_u4AllSendSize;                //��ǰ���ӷ����ֽ�����
    uint32             m_u4SendMaxBuffSize;            //����������󻺳峤��
    uint16             m_u2LocalPort;                  //���ؼ����˿�
    uint32             m_u4SendCheckTime;              //���ͼ��ʱ��ķ�ֵ
    uint32             m_u4PacketParseInfoID;          //��Ӧ����packetParse��ģ��ID
    uint32             m_u4PacketDebugSize;            //��¼�ܴ���������ݰ�������ֽ�
    int                m_nHashID;                      //��ӦHash�б��е�ID
    uint16             m_u2MaxConnectTime;             //�������ʱ���ж�
    uint16             m_u2RecvQueueTimeout;           //���ܳ�ʱʱ�䣬�������ʱ��Ķ��ᱻ��¼����־��
    uint16             m_u2TcpNodelay;                 //Nagle�㷨����
    CONNECTSTATE       m_u1ConnectState;               //Ŀǰ���Ӵ���״̬
    CONNECTSTATE       m_u1SendBuffState;              //Ŀǰ�������Ƿ��еȴ����͵�����
    bool               m_blIsLog;                      //�Ƿ�д����־��falseΪ��д�룬trueΪд��

    ACE_INET_Addr      m_addrRemote;                   //Զ�����ӿͻ��˵�ַ
    ACE_INET_Addr      m_addrListen;                   //���ؼ�����ַ
    ACE_Time_Value     m_atvConnect;                   //��ǰ���ӽ���ʱ��
    ACE_Time_Value     m_atvInput;                     //���һ�ν�������ʱ��
    ACE_Time_Value     m_atvOutput;                    //���һ�η�������ʱ��
    ACE_Time_Value     m_atvSendAlive;                 //���Ӵ��ʱ��
    shared_ptr<CPacketParse> m_pPacketParse;           //���ݰ�������
    char               m_szConnectName[MAX_BUFF_100];  //�������ƣ����Կ��Ÿ��߼����ȥ����
    char               m_szLocalIP[MAX_BUFF_50];       //���ؼ���IP
    string             m_strDeviceName;                //ת���ӿ�����

    ACE_Message_Block*  m_pBlockMessage    = nullptr;  //��ǰ���ͻ���ȴ����ݿ�
    ACE_Message_Block*  m_pBlockRecv       = nullptr;  //�������ݻ����
    shared_ptr<_Packet_Parse_Info> m_pPacketParseInfo = nullptr;  //PacketParse�Ľ�����

    shared_ptr<CPacketParse> m_objSendPacketParse;          //�������ݰ���֯�ṹ

    EM_IO_TYPE          m_emIOType;                    //��ǰIO�������
    IFileTestManager*   m_pFileTest;                   //�ļ����Խӿ����

    Fast_Asynch_Read_Stream  m_Reader;
    Fast_Asynch_Write_Stream m_Writer;

    CPerformanceCounter m_SendCounter;
    CPerformanceCounter m_RecvCounter;
};

//����ConnectHandler�ڴ��
class CProConnectHandlerPool
{
public:
	CProConnectHandlerPool(void);
	~CProConnectHandlerPool(void);

	void Init(int nObjcetCount);
	void Close();

	CProConnectHandler* Create();
	bool Delete(CProConnectHandler* pObject);

	int GetUsedCount();
	int GetFreeCount();

private:
	ACE_Recursive_Thread_Mutex           m_ThreadWriteLock;                     //���ƶ��߳���
	CHashTable<CProConnectHandler>       m_objHashHandleList;                   //Hash�����
	CObjectArrayList<CProConnectHandler> m_objHandlerList;                      //�����б����
};

typedef ACE_Singleton<CProConnectHandlerPool, ACE_Null_Mutex> App_ProConnectHandlerPool;

#endif
