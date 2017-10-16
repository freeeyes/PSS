#ifndef _COMMANDACCOUNT_K
#define _COMMANDACCOUNT_K

// 统计所有进出框架的命令执行情况
// 并统计所有的进出流量
// add by freeeyes
// 2012-03-19

#include "ace/Date_Time.h"
#include "define.h"
#include "LogManager.h"
#include "HashTable.h"

using namespace std;

//统计信息，里面有要统计的命令信息定义
struct _CommandData
{
    uint64 m_u8CommandCost;                //命令的执行耗费总时间
    uint32 m_u4CommandCount;               //命令的总调用次数
    uint32 m_u4PacketSize;                 //命令产生的总流量(未解密)
    uint32 m_u4CommandSize;                //命令产生的总流量(解密)
    uint16 m_u2CommandID;                  //命令的ID
    uint8  m_u1PacketType;                 //数据包来源类型
    uint8  m_u1CommandType;                //命令的类型，0是收到的命令，1是发出的命令
    ACE_Time_Value m_tvCommandTime;        //命令的最后处理时间

    _CommandData()
    {
        m_u2CommandID    = 0;
        m_u4CommandCount = 0;
        m_u8CommandCost  = 0;
        m_u4PacketSize   = 0;
        m_u4CommandSize  = 0;
        m_u1PacketType   = PACKET_TCP;
        m_u1CommandType  = COMMAND_TYPE_IN;
    }

    //拷贝构造函数
    _CommandData(const _CommandData& ar)
    {
        this->m_u2CommandID = ar.m_u2CommandID;
        this->m_u4CommandCount = ar.m_u4CommandCount;
        this->m_u8CommandCost = ar.m_u8CommandCost;
        this->m_u1CommandType = ar.m_u1CommandType;
        this->m_u4PacketSize = ar.m_u4PacketSize;
        this->m_u4CommandSize = ar.m_u4CommandSize;
        this->m_u1PacketType = ar.m_u1PacketType;
        this->m_tvCommandTime = ar.m_tvCommandTime;
    }

    _CommandData& operator = (const _CommandData& ar)
    {
        this->m_u2CommandID    = ar.m_u2CommandID;
        this->m_u4CommandCount = ar.m_u4CommandCount;
        this->m_u8CommandCost  = ar.m_u8CommandCost;
        this->m_u1CommandType  = ar.m_u1CommandType;
        this->m_u4PacketSize   = ar.m_u4PacketSize;
        this->m_u4CommandSize  = ar.m_u4CommandSize;
        this->m_u1PacketType   = ar.m_u1PacketType;
        this->m_tvCommandTime  = ar.m_tvCommandTime;
        return *this;
    }

    _CommandData& operator += (const _CommandData& ar)
    {
        if(this->m_u2CommandID != ar.m_u2CommandID)
        {
            this->m_u2CommandID = ar.m_u2CommandID;
        }

        this->m_u4CommandCount += ar.m_u4CommandCount;
        this->m_u8CommandCost  += ar.m_u8CommandCost;
        this->m_u1CommandType  += ar.m_u1CommandType;
        this->m_u4PacketSize   += ar.m_u4PacketSize;
        this->m_u4CommandSize  += ar.m_u4CommandSize;
        this->m_u1PacketType   += ar.m_u1PacketType;
        this->m_tvCommandTime  += ar.m_tvCommandTime;
        return *this;
    }
};

struct _CommandTimeOut
{
    uint32         m_u4TimeOutTime;                //超时时间
    uint16         m_u2CommandID;                  //命令的ID
    ACE_Time_Value m_tvTime;                       //发生时间

    _CommandTimeOut()
    {
        m_u2CommandID   = 0;
        m_tvTime        = ACE_OS::gettimeofday();
        m_u4TimeOutTime = 0;
    }

    //拷贝构造函数
    _CommandTimeOut(const _CommandTimeOut& ar)
    {
        this->m_u2CommandID   = ar.m_u2CommandID;
        this->m_tvTime        = ar.m_tvTime;
        this->m_u4TimeOutTime = ar.m_u4TimeOutTime;
    }

    _CommandTimeOut& operator = (const _CommandTimeOut& ar)
    {
        this->m_u2CommandID   = ar.m_u2CommandID;
        this->m_tvTime        = ar.m_tvTime;
        this->m_u4TimeOutTime = ar.m_u4TimeOutTime;
        return *this;
    }

    _CommandTimeOut& operator += (const _CommandTimeOut& ar)
    {
        this->m_u2CommandID   = ar.m_u2CommandID;
        this->m_tvTime        = ar.m_tvTime;
        this->m_u4TimeOutTime = ar.m_u4TimeOutTime;
        return *this;
    }
};

typedef vector<_CommandTimeOut> vecCommandTimeOut;   //记录所有超时命令的数组

struct _CommandAlertData
{
    uint32 m_u4CommandCount;
    uint32 m_u4MailID;
    uint32 m_u4CurrCount;
    uint16 m_u2CommandID;
    uint8  m_u1Minute;

    _CommandAlertData()
    {
        m_u2CommandID    = 0;
        m_u4CommandCount = 0;
        m_u4MailID       = 0;
        m_u4CurrCount    = 0;
        m_u1Minute       = 0;
    }
};
typedef vector<_CommandAlertData> vecCommandAlertData;   //记录所有的告警监控阀值

//流量流入和流出信息统计。
struct _CommandFlowAccount
{
    uint32 m_u4FlowIn;
    uint32 m_u4FlowOut;
    uint8  m_u1FLow;

    _CommandFlowAccount()
    {
        m_u1FLow    = 0;
        m_u4FlowIn  = 0;
        m_u4FlowOut = 0;
    }

    _CommandFlowAccount(const _CommandFlowAccount& ar)
    {
        this->m_u1FLow = ar.m_u1FLow;
        this->m_u4FlowIn = ar.m_u4FlowIn;
        this->m_u4FlowOut = ar.m_u4FlowOut;
    }

    _CommandFlowAccount& operator = (const _CommandFlowAccount& ar)
    {
        this->m_u1FLow    = ar.m_u1FLow;
        this->m_u4FlowIn  = ar.m_u4FlowIn;
        this->m_u4FlowOut = ar.m_u4FlowOut;
        return *this;
    }

    _CommandFlowAccount& operator += (const _CommandFlowAccount& ar)
    {
        this->m_u4FlowIn  += ar.m_u4FlowIn;
        this->m_u4FlowOut += ar.m_u4FlowOut;
        return *this;
    }
};

//统计所有进出框架的命令执行情况，目前不包括向其他服务器请求的统计，因为这部分协议无法统一。
class CCommandAccount
{
public:
    CCommandAccount();
    ~CCommandAccount();

    void InitName(const char* pName, uint32 u4CommandCount);
    void Init(uint8 u1CommandAccount, uint8 u1Flow, uint16 u2RecvTimeout);
    void AddCommandAlert(uint16 u2CommandID, uint32 u4Count, uint32 u4MailID);

    bool   SaveCommandData(uint16 u2CommandID, uint64 u8CommandCost, uint8 u1PacketType = PACKET_TCP, uint32 u4PacketSize = 0, uint32 u4CommandSize = 0, uint8 u1CommandType = COMMAND_TYPE_IN, ACE_Time_Value tvTime = ACE_OS::gettimeofday());   //记录命令执行信息
    bool   SaveCommandDataLog();                       //存储命令执行信息的日志
    void   ClearTimeOut();                             //清理超时时间的命令日志
    uint32 GetTimeoutCount();                          //得到超时命令日志个数
    _CommandTimeOut* GetTimeoutInfo(uint32 u4Index);   //得到超时数据库
    _CommandData* GetCommandData(uint16 u2CommandID);  //获得指定命令的相关数据

    uint32 GetFlowIn();                                //得到单位时间进入流量
    uint32 GetFlowOut();                               //得到党委时间流出流量
    uint8  GetFLow();                                  //得到当前流量开关状态

    _CommandFlowAccount GetCommandFlowAccount();                         //得到流量相关信息
    void GetCommandTimeOut(vecCommandTimeOut& CommandTimeOutList);       //得到所有的超时命令信息
    void GetCommandAlertData(vecCommandAlertData& CommandAlertDataList); //得到所有的告警命令信息

    void Close();

public:
    uint64                      m_u8PacketTimeout;               //包处理超时时间
    uint32                      m_u4FlowIn;                      //当前进入流量统计(单位，分钟)
    uint32                      m_u4FlowOut;                     //当前流出流量统计(单位，分钟)
    uint32                      m_u4PrvFlowIn;                   //上一分钟进入流量统计(单位，分钟)
    uint32                      m_u4PrvFlowOut;                  //上一分钟流出流量统计(单位，分钟)
    uint8                       m_u1CommandAccount;              //是否开启命令统计，1是打开，0是关闭
    uint8                       m_u1Flow;                        //是否打开流量统计，1是打开，0是关闭
    uint8                       m_u1Minute;                      //当前分钟数
    char                        m_szName[MAX_BUFF_50];           //当前统计的名字
    CHashTable<_CommandData>    m_objCommandDataList;            //命令Hash映射列表
    vecCommandTimeOut           m_vecCommandTimeOut;             //处理超时命令列表
    vecCommandAlertData         m_vecCommandAlertData;           //告警阀值数组
};

//typedef ACE_Singleton<CCommandAccount, ACE_Recursive_Thread_Mutex> App_CommandAccount;

#endif