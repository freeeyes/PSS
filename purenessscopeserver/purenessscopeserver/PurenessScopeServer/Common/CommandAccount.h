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
#include "ace/Hash_Map_Manager.h"

using namespace std;

//统计信息，里面有要统计的命令信息定义
struct _CommandData
{
    uint64 m_u8CommandCost;                //命令的执行耗费总时间
    uint32 m_u4CommandCount;               //命令的总调用次数
    uint32 m_u4PacketSize;                 //命令产生的总流量
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
        m_u1PacketType   = PACKET_TCP;
        m_u1CommandType  = COMMAND_TYPE_IN;
    }

    //拷贝构造函数
    _CommandData(const _CommandData& ar)
    {
        this->m_u2CommandID    = ar.m_u2CommandID;
        this->m_u4CommandCount = ar.m_u4CommandCount;
        this->m_u8CommandCost  = ar.m_u8CommandCost;
        this->m_u1CommandType  = ar.m_u1CommandType;
        this->m_u4PacketSize   = ar.m_u4PacketSize;
        this->m_u1PacketType   = ar.m_u1PacketType;
        this->m_tvCommandTime  = ar.m_tvCommandTime;
    }

    _CommandData& operator = (const _CommandData& ar)
    {
        this->m_u2CommandID    = ar.m_u2CommandID;
        this->m_u4CommandCount = ar.m_u4CommandCount;
        this->m_u8CommandCost  = ar.m_u8CommandCost;
        this->m_u1CommandType  = ar.m_u1CommandType;
        this->m_u4PacketSize   = ar.m_u4PacketSize;
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
        this->m_u1PacketType   += ar.m_u1PacketType;
        this->m_tvCommandTime  += ar.m_tvCommandTime;
        return *this;
    }
};

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

//对应端口数据接收信息
struct _Port_Data_Account
{
    uint8                       m_u1Type;                        //当前连接类型
    uint32                      m_u4Port;                        //当前数据端口
    uint8                       m_u1Minute;                      //获得当前分钟数
    uint32                      m_u4FlowIn;                      //当前进入流量统计(单位，分钟)
    uint32                      m_u4FlowOut;                     //当前流出流量统计(单位，分钟)

    _Port_Data_Account()
    {
        m_u1Type       = PACKET_TCP;
        m_u4Port       = 0;
        m_u4FlowIn     = 0;
        m_u4FlowOut    = 0;
        m_u1Minute     = 0;
    }

    _Port_Data_Account& operator = (const _Port_Data_Account& ar)
    {
        this->m_u1Type    = ar.m_u1Type;
        this->m_u4Port    = ar.m_u4Port;
        this->m_u4FlowIn  = ar.m_u4FlowIn;
        this->m_u4FlowOut = ar.m_u4FlowOut;
        this->m_u1Minute  = ar.m_u1Minute;
        return *this;
    }

    _Port_Data_Account& operator += (const _Port_Data_Account& ar)
    {
        if ((this->m_u4Port == ar.m_u4Port) && (this->m_u1Minute == ar.m_u1Minute))
        {
            this->m_u4FlowIn += ar.m_u4FlowIn;
            this->m_u4FlowOut += ar.m_u4FlowOut;
        }

        return *this;
    }

    //初始化数据
    void Init(uint8 u1Type, uint32 u4Port)
    {
        m_u1Type = u1Type;
        m_u4Port = u4Port;
    }

    uint32 GetFlowIn()
    {
        //得到此刻的流量入数据
        ACE_Date_Time dtNowTime(ACE_OS::gettimeofday());
        uint8 u1Minute = (uint8)dtNowTime.minute();

        if (u1Minute != m_u1Minute)
        {
            m_u4FlowIn  = 0;
            m_u4FlowOut = 0;
            m_u1Minute  = u1Minute;
            return m_u4FlowIn;
        }
        else
        {
            return m_u4FlowIn;
        }
    }

    uint32 GetFlowOut()
    {
        //得到此刻的流量出数据
        ACE_Date_Time dtNowTime(ACE_OS::gettimeofday());
        uint8 u1Minute = (uint8)dtNowTime.minute();

        if (u1Minute != m_u1Minute)
        {
            m_u4FlowIn  = 0;
            m_u4FlowOut = 0;
            m_u1Minute  = u1Minute;
            return m_u4FlowOut;
        }
        else
        {
            return m_u4FlowOut;
        }
    }

    void SetFlow(uint8 u1CommandType, uint32 u4Size, ACE_Time_Value tvNow)
    {
        //记录端口流量
        ACE_Date_Time dtNowTime(tvNow);
        uint8 u1Minute = (uint8)dtNowTime.minute();

        if (u1Minute != m_u1Minute)
        {
            if (COMMAND_TYPE_IN == u1CommandType)
            {
                m_u4FlowIn  = u4Size;
                m_u4FlowOut = 0;
            }
            else
            {
                m_u4FlowIn  = 0;
                m_u4FlowOut = u4Size;
            }

            m_u1Minute = u1Minute;
        }
        else
        {
            if (COMMAND_TYPE_IN == u1CommandType)
            {
                m_u4FlowIn += u4Size;
            }
            else
            {
                m_u4FlowOut += u4Size;
            }
        }
    }
};
typedef vector<_Port_Data_Account> vecPortDataAccount;

static void Combo_Port_List(vecPortDataAccount& vec_Port_Data_Account, vecPortDataAccount& vec_Port_Data_All_Account)
{
    int n4PartSize = (int)vec_Port_Data_Account.size();

    for (int iLoop = 0; iLoop < n4PartSize; iLoop++)
    {
        int n4AllSize = (int)vec_Port_Data_All_Account.size();
        bool bFound = false;

        for (int jLoop = 0; jLoop < n4AllSize; jLoop++)
        {
            if (vec_Port_Data_Account[iLoop].m_u4Port == vec_Port_Data_All_Account[jLoop].m_u4Port)
            {
                vec_Port_Data_All_Account[jLoop] += vec_Port_Data_Account[iLoop];
                bFound = true;
                break;
            }
        }

        if (false == bFound)
        {
            vec_Port_Data_All_Account.push_back(vec_Port_Data_Account[iLoop]);
        }
    }

    return;
}

//格式化一个ACE Hash类
template<class EXT_ID, class INT_ID>
class ACE_Hash_Map :
    public ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID,
    ACE_Hash<EXT_ID>, ACE_Equal_To<EXT_ID>, ACE_Null_Mutex>
{};

//统计所有进出框架的命令执行情况，目前不包括向其他服务器请求的统计，因为这部分协议无法统一。
class CCommandAccount
{
public:
    CCommandAccount();
    ~CCommandAccount();

    void InitName(const char* pName, uint32 u4CommandCount);
    void Init(uint8 u1CommandAccount, uint8 u1Flow, uint16 u2RecvTimeout);
    void AddCommandAlert(uint16 u2CommandID, uint32 u4Count, uint32 u4MailID);

    bool SaveCommandData(uint16 u2CommandID, uint32 u4Port, uint8 u1PacketType = PACKET_TCP,
                         uint32 u4PacketSize = 0, uint8 u1CommandType = COMMAND_TYPE_IN,
                         ACE_Time_Value tvTime = ACE_OS::gettimeofday());   //记录命令执行信息
    bool SaveCommandDataLog();                         //存储命令执行信息的日志
    _CommandData* GetCommandData(uint16 u2CommandID);  //获得指定命令的相关数据

    uint32 GetFlowIn();                                //得到单位时间进入流量
    uint32 GetFlowOut();                               //得到党委时间流出流量
    uint8  GetFLow();                                  //得到当前流量开关状态

    _CommandFlowAccount GetCommandFlowAccount();                         //得到流量相关信息
    void GetCommandAlertData(vecCommandAlertData& CommandAlertDataList); //得到所有的告警命令信息

    void GetFlowPortList(vector<_Port_Data_Account>& vec_Port_Data_Account);    //根据不同的监听端口，获得当前的端口对应的出入口数据信息

    void Close();

private:
    bool Save_Flow(uint16 u2CommandID, uint32 u4Port, uint8 u1PacketType = PACKET_TCP,
                   uint32 u4PacketSize = 0, uint8 u1CommandType = COMMAND_TYPE_IN,
                   ACE_Time_Value tvTime = ACE_OS::gettimeofday());                    //流量统计

    bool Save_Command(uint16 u2CommandID, uint32 u4Port, uint8 u1PacketType = PACKET_TCP,
                      uint32 u4PacketSize = 0, uint8 u1CommandType = COMMAND_TYPE_IN,
                      ACE_Time_Value tvTime = ACE_OS::gettimeofday());                 //命令统计

    bool Save_Alert(uint16 u2CommandID, uint32 u4Port, uint8 u1PacketType = PACKET_TCP,
                    uint32 u4PacketSize = 0, uint8 u1CommandType = COMMAND_TYPE_IN,
                    ACE_Time_Value tvTime = ACE_OS::gettimeofday());                 //命令告警统计

public:
    uint64                                    m_u8PacketTimeout;               //包处理超时时间
    uint8                                     m_u1CommandAccount;              //是否开启命令统计，1是打开，0是关闭
    uint8                                     m_u1Flow;                        //是否打开流量统计，1是打开，0是关闭
    uint8                                     m_u1Minute;                      //当前分钟数
    char                                      m_szName[MAX_BUFF_50];           //当前统计的名字
    CHashTable<_CommandData>                  m_objCommandDataList;            //命令Hash映射列表
    vecCommandAlertData                       m_vecCommandAlertData;           //告警阀值数组
    ACE_Hash_Map<uint32, _Port_Data_Account*> m_objectPortAccount;             //根据端口统计每条数据的进出量
};

//typedef ACE_Singleton<CCommandAccount, ACE_Recursive_Thread_Mutex> App_CommandAccount;

#endif