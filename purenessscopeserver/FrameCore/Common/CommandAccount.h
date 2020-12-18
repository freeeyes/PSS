#ifndef _COMMANDACCOUNT_K
#define _COMMANDACCOUNT_K

// 统计所有进出框架的命令执行情况
// 并统计所有的进出流量
// add by freeeyes
// 2012-03-19

#include "ace/Date_Time.h"
#include "define.h"
#include "TimeStamp.hpp"
#include "LogManager.h"
#include <string>
#include <sstream>
#include <unordered_map>

//统计信息，里面有要统计的命令信息定义
class _CommandData
{
public:
    uint64             m_u8CommandCost      = 0;                  //命令的执行耗费总时间
    uint32             m_u4CommandCount     = 0;                  //命令的总调用次数
    uint32             m_u4PacketSize       = 0;                  //命令产生的总流量
    uint16             m_u2CommandID        = 0;                  //命令的ID
    EM_CONNECT_IO_TYPE m_u1PacketType       = EM_CONNECT_IO_TYPE::CONNECT_IO_TCP;     //数据包来源类型
    uint8              m_u1CommandType      = COMMAND_TYPE_IN;    //命令的类型，0是收到的命令，1是发出的命令
    PSS_Time_Point     m_tvCommandTime;                           //命令的最后处理时间

    _CommandData() = default;

    _CommandData& operator += (const _CommandData& ar)
    {
        if(this->m_u2CommandID != ar.m_u2CommandID)
        {
            this->m_u2CommandID  = ar.m_u2CommandID;
            this->m_u1PacketType = ar.m_u1PacketType;
        }

        this->m_u4CommandCount += ar.m_u4CommandCount;
        this->m_u8CommandCost  += ar.m_u8CommandCost;
        this->m_u1CommandType  += ar.m_u1CommandType;
        this->m_u4PacketSize   += ar.m_u4PacketSize;
        this->m_tvCommandTime  = ar.m_tvCommandTime;
        return *this;
    }
};

class _CommandAlertData
{
public:
    uint32 m_u4CommandCount = 0;
    uint16 m_u2MailID       = 0;
    uint32 m_u4CurrCount    = 0;
    uint16 m_u2CommandID    = 0;
    uint8  m_u1Minute       = 0;

    _CommandAlertData() = default;
};
using vecCommandAlertData = vector<_CommandAlertData>;   //记录所有的告警监控阀值

//对应端口数据接收信息
class _Port_Data_Account
{
public:
    EM_CONNECT_IO_TYPE          m_u1Type    = EM_CONNECT_IO_TYPE::CONNECT_IO_TCP;  //当前连接类型
    uint32                      m_u4Port    = 0;                     //当前数据端口
    uint8                       m_u1Minute  = 0;                     //获得当前分钟数
    uint32                      m_u4FlowIn  = 0;                     //当前进入流量统计(单位，分钟)
    uint32                      m_u4FlowOut = 0;                     //当前流出流量统计(单位，分钟)

    _Port_Data_Account() = default;

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
    void Init(EM_CONNECT_IO_TYPE u1Type, uint32 u4Port)
    {
        m_u1Type = u1Type;
        m_u4Port = u4Port;
    }

    uint32 GetFlowIn()
    {
        //得到此刻的流量入数据
        auto u1Minute = CTimeStamp::Get_Time_of_Minute(CTimeStamp::Get_Time_Stamp());

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
        auto u1Minute = CTimeStamp::Get_Time_of_Minute(CTimeStamp::Get_Time_Stamp());

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

    void SetFlow(uint8 u1CommandType, uint32 u4Size, PSS_Time_Point const& tvNow)
    {
        //记录端口流量
        auto u1Minute = CTimeStamp::Get_Time_of_Minute(tvNow);

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

//统计所有进出框架的命令执行情况，目前不包括向其他服务器请求的统计，因为这部分协议无法统一。
class CCommandAccount
{
public:
    CCommandAccount() = default;

    void InitName(const char* pName, uint32 u4CommandCount);
    void Init(uint8 u1CommandAccount, uint8 u1Flow, uint16 u2RecvTimeout);
    void AddCommandAlert(uint16 u2CommandID, uint32 u4Count, uint16 u2MailID);

    void Save_Command_To_File(shared_ptr<_CommandData> pCommandData) const;

    bool SaveCommandData(uint16 u2CommandID, uint16 u2Port, EM_CONNECT_IO_TYPE u1PacketType = EM_CONNECT_IO_TYPE::CONNECT_IO_TCP,
        uint32 u4PacketSize = 0, uint8 u1CommandType = COMMAND_TYPE_IN,
        PSS_Time_Point const& tvTime = CTimeStamp::Get_Time_Stamp());   //记录命令执行信息
    bool SaveCommandDataLog();                         //存储命令执行信息的日志

    uint32 GetFlowIn();                                //得到单位时间进入流量
    uint32 GetFlowOut();                               //得到党委时间流出流量
    uint8  GetFLow() const;                            //得到当前流量开关状态

    void GetCommandAlertData(vecCommandAlertData& CommandAlertDataList); //得到所有的告警命令信息

    void GetFlowPortList(vector<_Port_Data_Account>& vec_Port_Data_Account);    //根据不同的监听端口，获得当前的端口对应的出入口数据信息

    void Close();

private:
    bool Save_Flow(uint16 u2CommandID, uint16 u2Port, EM_CONNECT_IO_TYPE u1PacketType = EM_CONNECT_IO_TYPE::CONNECT_IO_TCP,
        uint32 u4PacketSize = 0, uint8 u1CommandType = COMMAND_TYPE_IN,
        PSS_Time_Point const& tvTime = CTimeStamp::Get_Time_Stamp());                    //流量统计

    bool Save_Command(uint16 u2CommandID, uint16 u2Port, EM_CONNECT_IO_TYPE u1PacketType = EM_CONNECT_IO_TYPE::CONNECT_IO_TCP,
        uint32 u4PacketSize = 0, uint8 u1CommandType = COMMAND_TYPE_IN,
        PSS_Time_Point const& tvTime = CTimeStamp::Get_Time_Stamp());                 //命令统计

    bool Save_Alert(uint16 u2CommandID, uint16 u2Port, EM_CONNECT_IO_TYPE u1PacketType = EM_CONNECT_IO_TYPE::CONNECT_IO_TCP,
        uint32 u4PacketSize = 0, uint8 u1CommandType = COMMAND_TYPE_IN,
        PSS_Time_Point const& tvTime = CTimeStamp::Get_Time_Stamp()) const;                 //命令告警统计

public:                                                                                                                                  
    using hashmapPortAccount = unordered_map<uint16, shared_ptr<_Port_Data_Account>>;
    using hashmapCommandData = unordered_map<uint16, shared_ptr<_CommandData>>;
    uint64                                    m_u8PacketTimeout     = MAX_QUEUE_TIMEOUT * 1000; //包处理超时时间
    uint8                                     m_u1CommandAccount    = 0;                        //是否开启命令统计，1是打开，0是关闭
    uint8                                     m_u1Flow              = 0;                        //是否打开流量统计，1是打开，0是关闭
    uint8                                     m_u1Minute            = 0 ;                       //当前分钟数
    string                                    m_strName;                                        //当前统计的名字
    hashmapCommandData                        m_objCommandDataList;                             //命令Hash映射列表
    vecCommandAlertData                       m_vecCommandAlertData;                            //告警阀值数组
    hashmapPortAccount                        m_objectPortAccount;                              //根据端口统计每条数据的进出量
};

#endif