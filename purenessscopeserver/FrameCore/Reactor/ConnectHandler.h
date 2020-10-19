
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
#include "SendCacheManager.h"
#include "TimeWheelLink.h"
#include "FileTest.h"
#include "TcpRedirection.h"
#include "IDeviceHandler.h"

#if PSS_PLATFORM != PLATFORM_WIN
#include "netinet/tcp.h"
#endif

class CConnectHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>, public IDeviceHandler
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
    bool SendMessage(CSendMessageInfo objSendMessageInfo, uint32& u4PacketSize);  //���͵�ǰ����
    bool SendCloseMessage();                                                 //�������ӹر���Ϣ
    bool SendTimeoutMessage() const;                                         //�������ӳ�ʱ��Ϣ

    void SetRecvQueueTimeCost(uint32 u4TimeCost);                            //��¼��ǰ�������ݵ�ģ�鴦����ɵľ���ʱ������
    void SetSendQueueTimeCost(uint32 u4TimeCost) const;                      //��¼��ǰ�ӷ��Ͷ��е����ݷ�����ɵľ���ʱ������
    void SetLocalIPInfo(const char* pLocalIP, uint16 u2LocalPort);           //���ü���IP�Ͷ˿���Ϣ

    void Close();                                                            //�رյ�ǰ����
    void CloseFinally();                                                     //�����������������roz����

    uint32      GetHandlerID() const;                                        //�õ���ǰ��handlerID
    const char* GetError() const;                                            //�õ���ǰ������Ϣ
    void        SetConnectID(uint32 u4ConnectID);                            //���õ�ǰ����ID
    uint32      GetConnectID() const;                                        //�õ���ǰ����ID
    CONNECTSTATE       GetConnectState() const;                              //�õ�����״̬
    CONNECTSTATE       GetSendBuffState() const;                             //�õ�����״̬
    _ClientConnectInfo GetClientInfo() const;                                //�õ��ͻ�����Ϣ
    _ClientIPInfo      GetClientIPInfo() const;                              //�õ��ͻ���IP��Ϣ
    _ClientIPInfo      GetLocalIPInfo();                                     //�õ�����IP��Ϣ
    void SetConnectName(const char* pName);                                  //���õ�ǰ��������
    char* GetConnectName();                                                  //�õ�����
    void SetIsLog(bool blIsLog);                                             //���õ�ǰ���������Ƿ�д����־
    bool GetIsLog() const;                                                   //��õ�ǰ�����Ƿ����д����־
    void SetHashID(int nHashID);                                             //����Hash�����±�
    int  GetHashID() const;                                                  //�õ�Hash�����±�
    void SetSendCacheManager(CSendCacheManager* pSendCacheManager);          //���û���������
    bool Test_Paceket_Parse_Stream(ACE_Message_Block* pmb);                  //������ģʽ�����������
    void Output_Debug_Data(const ACE_Message_Block* pMbData, int nLogType);  //���DEBUG��Ϣ

    bool Write_SendData_To_File(bool blDelete, IBuffPacket* pBuffPacket);                             //����������д���ļ�
    bool Send_Input_To_Cache(CSendMessageInfo objSendMessageInfo, uint32& u4PacketSize);              //�����Ͷ�����뻺��
    bool Send_Input_To_TCP(CSendMessageInfo objSendMessageInfo, uint32& u4PacketSize, uint8 u1State); //�����ݷ��͸��Զ�

private:
    void ConnectOpen();                                                      //����������ش򿪴���
    void Get_Recv_length(int& nCurrCount) const;                             //�õ�Ҫ��������ݳ���
    int  Dispose_Paceket_Parse_Head();                                       //������Ϣͷ����
    int  Dispose_Paceket_Parse_Body();                                       //������Ϣͷ����
    int  Dispose_Paceket_Parse_Stream(ACE_Message_Block* pCurrMessage);      //��������Ϣ����
    bool CheckMessage();                                                     //������յ�����
    bool PutSendPacket(ACE_Message_Block* pMbData, uint32 u4Size, const ACE_Time_Value tvSend);//��������
    void ClearPacketParse();                                                 //��������ʹ�õ�PacketParse
    bool Send_Block_Queue(ACE_Message_Block* pMb);                           //���Ͷ���ֹͣ��Ϣ

    int  Dispose_Paceket_Parse_Stream_Single(ACE_Message_Block* pCurrMessage);//����һ���ݰ�
    int  RecvData();                                                          //�������ݣ�����ģʽ

    int  Dispose_Recv_Data();                                                 //�����������
    int  Init_Open_Connect();                                                 //����һ�ν������ӳ�ʼ����ʱ�����

    uint64                     m_u8RecvQueueTimeCost  = 0;                     //�ɹ��������ݵ����ݴ�����ɣ�δ���ͣ����ѵ�ʱ���ܺ�
    uint64                     m_u8SendQueueTimeCost  = 0;                     //�ɹ��������ݵ����ݴ�����ɣ�ֻ���ͣ����ѵ�ʱ���ܺ�
    uint64                     m_u8SendQueueTimeout   = MAX_QUEUE_TIMEOUT * 1000 * 1000;           //���ͳ�ʱʱ�䣬�������ʱ��Ķ��ᱻ��¼����־��
    uint64                     m_u8RecvQueueTimeout   = MAX_QUEUE_TIMEOUT * 1000 * 1000;           //���ܳ�ʱʱ�䣬�������ʱ��Ķ��ᱻ��¼����־��
    uint32                     m_u4HandlerID          = 0;                     //��Hander����ʱ��ID
    uint32                     m_u4ConnectID          = 0;                     //���ӵ�ID
    uint32                     m_u4AllRecvCount       = 0;                     //��ǰ���ӽ������ݰ��ĸ���
    uint32                     m_u4AllSendCount       = 0;                     //��ǰ���ӷ������ݰ��ĸ���
    uint32                     m_u4AllRecvSize        = 0;                     //��ǰ���ӽ����ֽ�����
    uint32                     m_u4AllSendSize        = 0;                     //��ǰ���ӷ����ֽ�����
    uint32                     m_u4MaxPacketSize      = MAX_MSG_PACKETLENGTH;  //�������ݰ�����󳤶�
    uint32                     m_u4RecvQueueCount     = 0;                     //��ǰ���ӱ���������ݰ���
    uint32                     m_u4SendMaxBuffSize    = 5 * MAX_BUFF_1024;     //����������󻺳峤��
    uint32                     m_u4SendThresHold      = MAX_MSG_SNEDTHRESHOLD; //���ͷ�ֵ(��Ϣ���ĸ���)
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
    char                       m_szError[MAX_BUFF_500]       = {'\0'};         //������Ϣ��������
    char                       m_szLocalIP[MAX_BUFF_50]      = { '\0' };       //���ص�IP�˿�
    char                       m_szConnectName[MAX_BUFF_100] = { '\0' };       //�������ƣ����Կ��Ÿ��߼����ȥ����
    ACE_INET_Addr              m_addrRemote;                                   //Զ�����ӿͻ��˵�ַ
    ACE_Time_Value             m_atvConnect;                                   //��ǰ���ӽ���ʱ��
    ACE_Time_Value             m_atvInput;                                     //���һ�ν�������ʱ��
    ACE_Time_Value             m_atvOutput;                                    //���һ�η�������ʱ��
    ACE_Time_Value             m_atvSendAlive;                                 //���Ӵ��ʱ��
    CPacketParse*              m_pPacketParse         = NULL;                  //���ݰ�������
    ACE_Message_Block*         m_pCurrMessage         = NULL;                  //��ǰ��MB����
    ACE_Message_Block*         m_pBlockMessage        = NULL;                  //��ǰ���ͻ���ȴ����ݿ�
    _TimeConnectInfo           m_TimeConnectInfo;                              //���ӽ��������
    char*                      m_pPacketDebugData     = NULL;                  //��¼���ݰ���Debug�����ַ���
    EM_IO_TYPE                 m_emIOType             = EM_IO_TYPE::NET_INPUT; //��ǰIO�������
    IFileTestManager*          m_pFileTest            = NULL;                  //�ļ����Խӿ����
    string                     m_strDeviceName;                                //ת���ӿ�����
};

//���������Ѿ�����������
class CConnectManager : public ACE_Task<ACE_MT_SYNCH>
{
public:
    CConnectManager(void);

    virtual int handle_timeout(const ACE_Time_Value& tv, const void* arg);   //��ʱ�����

    static void TimeWheel_Timeout_Callback(void* pArgsContext, vector<CConnectHandler*> vecConnectHandle);

    virtual int open();
    virtual int svc(void);
    virtual int close(u_long);

    void Init(uint16 u2Index);

    void CloseAll();
    bool AddConnect(uint32 u4ConnectID, CConnectHandler* pConnectHandler);
    bool SetConnectTimeWheel(CConnectHandler* pConnectHandler);                                            //������Ϣ����
    bool DelConnectTimeWheel(CConnectHandler* pConnectHandler);                                            //ɾ����Ϣ����
    bool SendMessage(CSendMessageInfo objSendMessageInfo);  //ͬ������                                                                     //���ͻ�������
    bool PostMessage(CSendMessageInfo objSendMessageInfo); //�첽����
    bool PostMessageAll(IBuffPacket* pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL, uint16 u2CommandID = 0, uint8 u1SendState = true, bool blDelete = true, int nMessageID = 0);                  //�첽Ⱥ��
    bool Close(uint32 u4ConnectID);                                                                          //�ͻ����ر�
    bool CloseUnLock(uint32 u4ConnectID);                                                                    //�ر����ӣ��������汾
    bool CloseConnect(uint32 u4ConnectID);                                                                   //�������ر�
    bool CloseConnect_By_Queue(uint32 u4ConnectID);                                                          //�������ر�(�����ر��͵���Ϣ�����йر�)
    void GetConnectInfo(vecClientConnectInfo& VecClientConnectInfo);                                         //���ص�ǰ������ӵ���Ϣ
    void SetRecvQueueTimeCost(uint32 u4ConnectID, uint32 u4TimeCost);                                        //��¼ָ���������ݴ���ʱ��
    void GetClientNameInfo(const char* pName, vecClientNameInfo& objClientNameInfo);                         //�õ�ָ������������������Ϣ

    _ClientIPInfo GetClientIPInfo(uint32 u4ConnectID);                                                       //�õ�ָ��������Ϣ
    _ClientIPInfo GetLocalIPInfo(uint32 u4ConnectID);                                                        //�õ�����������Ϣ

    bool StartTimer();                                                                                       //������ʱ��
    bool KillTimer();                                                                                        //�رն�ʱ��
    _CommandData* GetCommandData(uint16 u2CommandID);                                                        //�õ����������Ϣ
    void GetFlowInfo(uint32& u4UdpFlowIn, uint32& u4UdpFlowOut);                                             //�õ�������Ϣ

    int         GetCount();
    const char* GetError() const;

    bool SetConnectName(uint32 u4ConnectID, const char* pName);                                              //���õ�ǰ��������
    bool SetIsLog(uint32 u4ConnectID, bool blIsLog);                                                         //���õ�ǰ���������Ƿ�д����־
    EM_Client_Connect_status GetConnectState(uint32 u4ConnectID);                                            //�õ�ָ��������״̬

    int handle_write_file_stream(uint32 u4ConnectID, const char* pData, uint32 u4Size, uint8 u1ParseID);     //�ļ��ӿ�ģ�����ݰ����

private:
    virtual int CloseMsgQueue();
    bool Dispose_Queue();

    //�ر���Ϣ������������
    ACE_Thread_Mutex m_mutex;
    ACE_Condition<ACE_Thread_Mutex> m_cond;

    uint32                             m_u4TimeCheckID         = 0;                     //��ʱ������TimerID
    uint32                             m_u4SendQueuePutTime    = 0;                     //���Ͷ�����ӳ�ʱʱ��
    uint32                             m_u4TimeConnect         = 0;                     //��λʱ�����ӽ�����
    uint32                             m_u4TimeDisConnect      = 0;                     //��λʱ�����ӶϿ���
    uint16                             m_u2SendQueueMax        = MAX_MSG_SENDPACKET;    //���Ͷ�����󳤶�
    bool                               m_blRun                 = false;                 //�߳��Ƿ�������
    char                               m_szError[MAX_BUFF_500] = {'\0'};                //������Ϣ����
    CHashTable<CConnectHandler>        m_objHashConnectList;                            //��¼��ǰ�Ѿ����ӵĽڵ㣬ʹ�ù̶��ڴ�ṹ
    ACE_Recursive_Thread_Mutex         m_ThreadWriteLock;                               //����ѭ����غͶϿ�����ʱ���������
    _TimerCheckID*                     m_pTCTimeSendCheck      = NULL;                  //��ʱ���Ĳ����ṹ�壬����һ����ʱ��ִ�в�ͬ���¼�
    ACE_Time_Value                     m_tvCheckConnect;        //��ʱ����һ�μ������ʱ��
    CSendMessagePool                   m_SendMessagePool;       //������Ϣ��
    CCommandAccount                    m_CommandAccount;        //��ǰ�߳�����ͳ������
    CSendCacheManager                  m_SendCacheManager;      //���ͻ������
    CTimeWheelLink<CConnectHandler>    m_TimeWheelLink;         //����ʱ������
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
    uint32                            m_u4CurrMaxCount = 1;                  //��ǰ����Handler����
};

//����˼������ѷ��Ͷ�����ڼ����߳���ȥ����������ܡ������ﳢ��һ�¡�(���߳�ģʽ��һ���߳�һ�����У��������ֲ�������)
class CConnectManagerGroup : public IConnectManager
{
public:
    CConnectManagerGroup();

    void Init(uint16 u2SendQueueCount);
    void Close();

    bool AddConnect(CConnectHandler* pConnectHandler);
    bool SetConnectTimeWheel(CConnectHandler* pConnectHandler);                                            //������Ϣ����
    bool DelConnectTimeWheel(CConnectHandler* pConnectHandler);                                            //ɾ����Ϣ����
    virtual bool PostMessage(uint32 u4ConnectID, IBuffPacket*& pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL,
                             uint16 u2CommandID = 0, uint8 u1SendState = 0, bool blDelete = true, int nMessageID = 0);            //�첽����
    virtual bool PostMessage(uint32 u4ConnectID, char*& pData, uint32 nDataLen, uint8 u1SendType = SENDMESSAGE_NOMAL,
                             uint16 u2CommandID = 0, uint8 u1SendState = 0, bool blDelete = true, int nMessageID = 0);            //�첽����
    virtual bool PostMessage(vector<uint32> vecConnectID, IBuffPacket*& pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL,
                             uint16 u2CommandID = 0, uint8 u1SendState = 0, bool blDelete = true, int nMessageID = 0);            //�첽Ⱥ��ָ����ID
    virtual bool PostMessage(vector<uint32> vecConnectID, char*& pData, uint32 nDataLen, uint8 u1SendType = SENDMESSAGE_NOMAL,
                             uint16 u2CommandID = 0, uint8 u1SendState = 0, bool blDelete = true, int nMessageID = 0);   //�첽Ⱥ��ָ����ID
    virtual bool PostMessageAll(IBuffPacket*& pBuffPacket, uint8 u1SendType = SENDMESSAGE_NOMAL,
                                uint16 u2CommandID = 0, uint8 u1SendState = 0, bool blDelete = true, int nMessageID = 0);
    virtual bool PostMessageAll(char*& pData, uint32 nDataLen, uint8 u1SendType = SENDMESSAGE_NOMAL,
                                uint16 u2CommandID = 0, uint8 u1SendState = 0, bool blDelete = true, int nMessageID = 0);
    virtual bool CloseConnect(uint32 u4ConnectID);                                                                   //�������ر�
    bool CloseConnectByClient(uint32 u4ConnectID) const;                                                             //�ͻ��˹ر�
    virtual _ClientIPInfo GetClientIPInfo(uint32 u4ConnectID);                                                       //�õ�ָ��������Ϣ
    virtual _ClientIPInfo GetLocalIPInfo(uint32 u4ConnectID);                                                        //�õ�����������Ϣ
    virtual void GetClientNameInfo(const char* pName, vecClientNameInfo& objClientNameInfo);                         //�õ�ָ������������������Ϣ
    void GetConnectInfo(vecClientConnectInfo& VecClientConnectInfo) const;                                           //���ص�ǰ������ӵ���Ϣ
    void SetRecvQueueTimeCost(uint32 u4ConnectID, uint32 u4TimeCost) const;                                          //��¼ָ���������ݴ���ʱ��
    virtual uint16 GetConnectCheckTime();                                                                            //�õ�TCP����ʱ����

    virtual int  GetCount();
    void CloseAll();
    bool Close(uint32 u4ConnectID) const;                                                                    //�ͻ����ر�
    bool CloseUnLock(uint32 u4ConnectID) const;                                                              //�ر����ӣ��������汾
    bool SetConnectName(uint32 u4ConnectID, const char* pName);                                              //���õ�ǰ��������
    bool SetIsLog(uint32 u4ConnectID, bool blIsLog);                                                         //���õ�ǰ���������Ƿ�д����־
    void GetCommandData(uint16 u2CommandID, _CommandData& objCommandData) const;                             //���ָ������ͳ����Ϣ

    bool StartTimer() const;                                                                                 //������ʱ��
    const char* GetError() const;
    void GetFlowInfo(uint32& u4UdpFlowIn, uint32& u4UdpFlowOut) const;                                       //�õ�������Ϣ
    virtual EM_Client_Connect_status GetConnectState(uint32 u4ConnectID);

    int handle_write_file_stream(uint32 u4ConnectID, const char* pData, uint32 u4Size, uint8 u1ParseID) const; //�ļ��ӿ�ģ�����ݰ����

    CConnectManager* GetManagerFormList(int nIndex);                                                       //��õ�ǰManager��ָ��

private:
    uint32 GetGroupIndex();                                                                                  //�õ���ǰ���ӵ�ID������

    uint32            m_u4CurrMaxCount              = 0;                                                     //��ǰ����������
    uint16            m_u2ThreadQueueCount          = SENDQUEUECOUNT;                                        //��ǰ�����̶߳��и���
    ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;                                                           //���ƶ��߳���
    CConnectManager** m_objConnnectManagerList      = NULL;                                                  //�������ӹ�����
};

typedef ACE_Singleton<CConnectManagerGroup, ACE_Recursive_Thread_Mutex> App_ConnectManager;
typedef ACE_Singleton<CConnectHandlerPool, ACE_Null_Mutex> App_ConnectHandlerPool;

#endif
