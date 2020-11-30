#ifndef MESSAGE_DYEING_MANAGER_H
#define MESSAGE_DYEING_MANAGER_H

#include "IMessage.h"
#include "Trace.h"
#include <array>

//处理数据包染色相关功能
//考虑到遍历的代价，目前还是觉得这种用小数组遍历应该是可以接受的。
//add by freeeyes

const uint16 MAX_DYEING_COMMAND_COUNT = 10;  //最大染色命令数

//IP监控，目前考虑同一时刻只能监控一个
class _Dyeing_IP
{
public:
    uint16  m_u2MaxCount     = 0;        //当前需要监控的最大个数
    uint16  m_u2CurrCount    = 0;        //当前的已处理的个数
    bool m_blIsValid         = false;    //监控是否有效
    string m_strIP;                      //用户IP

    _Dyeing_IP() = default;

    void Clear()
    {
		m_u2MaxCount = 0;
		m_u2CurrCount = 0;
		m_blIsValid = false;
        m_strIP = "";
    }
};

//监控命令参数
class _Dyeing_Command
{
public:
    uint16  m_u2CommandID = 0;       //当前CommandID
    uint16  m_u2MaxCount  = 0;       //当前需要监控的最大个数
    uint16  m_u2CurrCount = 0;       //当前的已处理的个数
    bool    m_blIsValid   = false;   //监控是否有效

    _Dyeing_Command()
    {
        Clear();
    }

    void Clear()
    {
        m_u2CommandID = 0;
        m_u2MaxCount = 0;
        m_u2CurrCount = 0;
        m_blIsValid = false;
    }
};

using vec_Dyeing_Command_list = vector<_Dyeing_Command>;  //CommandID染色列表

class CMessageDyeingManager
{
public:
    CMessageDyeingManager() = default;

    void AddDyringIP(const char* pClientIP, uint16 u2MaxCount);                      //添加一个IP染色规则
    bool AddDyeingCommand(uint16 u2CommandID, uint16 u2MaxCount);                    //添加一个命令染色规则

    void GetDyeingCommand(vec_Dyeing_Command_list& objList) const;                   //获得当前命令染色状态

    string GetTraceID(const char* pClientIP, short sClintPort, uint16 u2CommandID);  //看看是否输出新的traceID

private:
    std::array<_Dyeing_Command, MAX_DYEING_COMMAND_COUNT> m_objCommandList;   //当前允许数据染色的CommandID列表
    _Dyeing_IP      m_objDyeingIP;                                //当前允许染色的IP
    uint16          m_u2CurrCommandCount = 0;                     //当前正在执行的染色CommandID个数
};

#endif
