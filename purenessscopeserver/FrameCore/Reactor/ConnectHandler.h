
// ConnectHandle.h
// ����ͻ�������
// �ܶ�ʱ�䣬�����������������ˣ������������������������ˡ�û��ʲô��ںý���
// ������2009��Ĵ����һ�����ټ������������˼ά����������������ϵĶ����������������ļ̳�������
// �����ң��������Щ�ѣ������Ҳ����ں�����Ϊ��֪��������ĵ�·�Ӳ�ƽ̹���������¿�ʼ����Ϊ���ܸе�������ҵ�������
// ��Ӷ��������������ݰ����Ĺܿء�
// add by freeeyes
// 2008-12-22

#ifndef _CONNECTHANDLE_H
#define _CONNECTHANDLE_H

#include "ace/Reactor.h"
#include "ace/Svc_Handler.h"
#include "ace/Synch.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Reactor_Notification_Strategy.h"

#include "BaseTask.h"
#include "ObjectArrayList.h"
#include "HashTable.h"
#include "AceReactorManager.h"
#include "MessageService.h"
#include "IConnectManager.h"
#include "BaseHander.h"
#include "BuffPacketManager.h"
#include "ForbiddenIP.h"
#include "IPAccount.h"
#include "TimerManager.h"
#include "SendMessage.h"
#include "CommandAccount.h"
#include "TimeWheelLink.h"
#include "FileTest.h"
#include "TcpRedirection.h"
#include "IDeviceHandler.h"
#include "PerformanceCounter.h"
#include "ConnectCounter.h"

#if PSS_PLATFORM != PLATFORM_WIN
#include "netinet/tcp.h"
#endif

class CConnectHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>, public IDeviceHandler, public IHandler
{
public:
    CConnectHandler(void);

    //��д�̳з���
    virtual int open(void*);                                                 //�û�����һ������
    virtual int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE);            //���ܿͻ����յ������ݿ�
    virtual int handle_output(ACE_HANDLE fd = ACE_INVALID_HANDLE);           //���Ϳͻ�������
    virtual int handle_close(ACE_HANDLE h, ACE_Reactor_Mask mask);           //���ӹر��¼�
    virtual bool Device_Send_Data(const char* pData, ssize_t nLen);          //͸�����ݽӿ�

    uint32 file_open(IFileTestManager* pFileTest);                                           //�ļ���ڴ򿪽ӿ�
    int handle_write_file_stream(const char* pData, uint32 u4Size, uint8 u1ParseID);         //�ļ��ӿ�ģ�����ݰ����

    void Init(uint16 u2HandlerID);                                           //Connect Pool��ʼ������ʱ����õķ���
    void SetPacketParseInfoID(uint32 u4PacketParseInfoID);                   //���ö�Ӧ��m_u4PacketParseInfoID
    uint32 GetPacketParseInfoID() const;                                     //�����Ӧ��m_u4PacketParseInfoID


    bool CheckSendMask(uint32 u4PacketLen);                                  //���ָ�������ӷ��������Ƿ񳬹�������ֵ
    virtual bool SendMessage(CSendMessageInfo objSendMessageInfo, uint32& u4PacketSize);  //���͵�ǰ����

    bool SendTimeoutMessage() const;                                         //�������ӳ�ʱ��Ϣ

    void SetLocalIPInfo(const char* pLocalIP, uint16 u2LocalPort);           //���ü���IP�Ͷ˿���Ϣ

    virtual void Close(uint32 u4ConnectID);                                  //�رյ�ǰ����
    void CloseFinally();                                                     //�����������������roz����

    uint32      GetHandlerID() const;                                        //�õ���ǰ��handlerID
    void        SetConnectID(uint32 u4ConnectID);                            //���õ�ǰ����ID
    uint32      GetConnectID() const;                                        //�õ���ǰ����ID
    CONNECTSTATE       GetConnectState() const;                              //�õ�����״̬
    CONNECTSTATE       GetSendBuffState() const;                             //�õ�����״̬
    _ClientConnectInfo GetClientInfo() const;                                //�õ��ͻ�����Ϣ
    _ClientIPInfo      GetClientIPInfo() const;                              //�õ��ͻ���IP��Ϣ
    _ClientIPInfo      GetLocalIPInfo();                                     //�õ�����IP��Ϣ
    void SetConnectName(const char* pName);                                  //���õ�ǰ��������
    char* GetConnectName();                                                  //�õ�����
    virtual void SetIsLog(bool blIsLog);                                     //���õ�ǰ���������Ƿ�д����־
    bool GetIsLog() const;                                                   //��õ�ǰ�����Ƿ����д����־
    void SetHashID(int nHashID);                                             //����Hash�����±�
    int  GetHashID() const;                                                  //�õ�Hash�����±�
    void Output_Debug_Data(const ACE_Message_Block* pMbData, int nLogType);  //���DEBUG��Ϣ

    bool Write_SendData_To_File(bool blDelete, IBuffPacket* pBuffPacket);                              //����������д���ļ�
    bool Send_Input_To_Cache(CSendMessageInfo objSendMessageInfo, uint32& u4PacketSize);               //�����Ͷ�����뻺��
    bool Send_Input_To_TCP(CSendMessageInfo objSendMessageInfo, uint32& u4PacketSize);                 //�����ݷ��͸��Զ�
    virtual bool PutSendPacket(uint32 u4ConnectID, ACE_Message_Block* pMbData, uint32 u4Size, const ACE_Time_Value tvSend);//��������

private:
	bool Dispose_Recv_buffer();                                              //������յ����ݣ��а�
	void Move_Recv_buffer();                                                 //��������ڴ滺����
    void Send_Hander_Event(uint8 u1Option);                                  //����Handler���¼�֪ͨҵ���߳�
    void ConnectOpen();                                                      //����������ش򿪴���
    uint32 Get_Recv_length();                                                //�õ�Ҫ��������ݳ���
    int  Dispose_Paceket_Parse_Head(ACE_Message_Block* pmbHead);             //������Ϣͷ����
    int  Dispose_Paceket_Parse_Body(ACE_Message_Block* pmbBody);             //������Ϣͷ����
    int  Dispose_Paceket_Parse_Stream(ACE_Message_Block* pCurrMessage);      //��������Ϣ����
    bool CheckMessage();                                                     //������յ�����
    
    void ClearPacketParse();                                                 //��������ʹ�õ�PacketParse

    int  Dispose_Paceket_Parse_Stream_Single(ACE_Message_Block* pCurrMessage);//����һ���ݰ�

    int  Dispose_Recv_Data();                                                 //�����������
    int  Init_Open_Connect();                                                 //����һ�ν������ӳ�ʼ����ʱ�����

    uint32                     m_u4HandlerID          = 0;                     //��Hander����ʱ��ID
    uint32                     m_u4ConnectID          = 0;                     //���ӵ�ID
    uint32                     m_u4AllRecvSize        = 0;                     //��ǰ���ӽ����ֽ�����
    uint32                     m_u4AllSendSize        = 0;                     //��ǰ���ӷ����ֽ�����
    uint32                     m_u4MaxPacketSize      = MAX_MSG_PACKETLENGTH;  //�������ݰ�����󳤶�
    uint32                     m_u4RecvQueueCount     = 0;                     //��ǰ���ӱ���������ݰ���
    uint32                     m_u4SendMaxBuffSize    = 5 * MAX_BUFF_1024;     //����������󻺳峤��
    uint32                     m_u4ReadSendSize       = 0;                     //׼�����͵��ֽ�����ˮλ�꣩
    uint32                     m_u4SuccessSendSize    = 0;                     //ʵ�ʿͻ��˽��յ������ֽ�����ˮλ�꣩
    uint16                     m_u2LocalPort          = 0;                     //�����Ķ˿ں�
    uint32                     m_u4PacketParseInfoID  = 0;                     //��Ӧ����packetParse��ģ��ID
    uint32                     m_u4CurrSize           = 0;                     //��ǰMB�����ַ�����
    uint32                     m_u4PacketDebugSize    = 0;                     //��¼�ܴ���������ݰ�������ֽ�
    int                        m_nBlockCount          = 0;                     //���������Ĵ���
    int                        m_nHashID              = 0;                     //��Ӧ��Pool��Hash�����±�
    uint16                     m_u2MaxConnectTime     = 0;                     //���ʱ�������ж�
    uint16                     m_u2TcpNodelay         = TCP_NODELAY_ON;        //Nagle�㷨����
    CONNECTSTATE               m_u1ConnectState       = CONNECTSTATE::CONNECT_INIT;          //Ŀǰ���Ӵ���״̬
    CONNECTSTATE               m_u1SendBuffState      = CONNECTSTATE::CONNECT_SENDNON;       //Ŀǰ�������Ƿ��еȴ����͵�����
    uint8                      m_u1IsActive           = 0;                     //�����Ƿ�Ϊ����״̬��0Ϊ��1Ϊ��
    bool                       m_blBlockState         = false;                 //�Ƿ�������״̬ falseΪ��������״̬��trueΪ������״̬
    bool                       m_blIsLog              = false;                 //�Ƿ�д����־��falseΪ��д�룬trueΪд��
    char                       m_szLocalIP[MAX_BUFF_50]      = { '\0' };       //���ص�IP�˿�
    char                       m_szConnectName[MAX_BUFF_100] = { '\0' };       //�������ƣ����Կ��Ÿ��߼����ȥ����
    ACE_INET_Addr              m_addrRemote;                                   //Զ�����ӿͻ��˵�ַ
    ACE_Time_Value             m_atvConnect;                                   //��ǰ���ӽ���ʱ��
    ACE_Time_Value             m_atvInput;                                     //���һ�ν�������ʱ��
    ACE_Time_Value             m_atvOutput;                                    //���һ�η�������ʱ��
    CPacketParse               m_objPacketParse;                               //���ݰ�������
    ACE_Message_Block*         m_pBlockRecv           = nullptr;               //�������ݻ����
    ACE_Message_Block*         m_pBlockMessage        = nullptr;               //��ǰ���ͻ���ȴ����ݿ�
    char*                      m_pPacketDebugData     = nullptr;               //��¼���ݰ���Debug�����ַ���
    EM_IO_TYPE                 m_emIOType             = EM_IO_TYPE::NET_INPUT; //��ǰIO�������
    IFileTestManager*          m_pFileTest            = nullptr;               //�ļ����Խӿ����
    string                     m_strDeviceName;                                //ת���ӿ�����
    _Packet_Parse_Info*        m_pPacketParseInfo     = nullptr;               //PacketParse�Ľ�����
};

//����ConnectHandler�ڴ��
class CConnectHandlerPool
{
public:
    CConnectHandlerPool(void);

    void Init(int nObjcetCount);
    void Close();

    CConnectHandler* Create();
    bool Delete(CConnectHandler* pObject);

    int GetUsedCount();
    int GetFreeCount();

private:
    ACE_Recursive_Thread_Mutex        m_ThreadWriteLock;                     //���ƶ��߳���
    CHashTable<CConnectHandler>       m_objHashHandleList;                   //Hash�����
    CObjectArrayList<CConnectHandler> m_objHandlerList;                      //�����б����
    uint32                            m_u4CurrCount = 0;                     //��ǰ����Handler����
};


typedef ACE_Singleton<CConnectHandlerPool, ACE_Null_Mutex> App_ConnectHandlerPool;

#endif
