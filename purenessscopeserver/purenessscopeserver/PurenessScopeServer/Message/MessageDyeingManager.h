#ifndef MESSAGE_DYEING_MANAGER_H
#define MESSAGE_DYEING_MANAGER_H

#include "IMessage.h"
#include "Trace.h"

//处理数据包染色相关功能
//考虑到遍历的代价，目前还是觉得这种用小数组遍历应该是可以接受的。
//add by freeeyes

#define  MAX_DYEING_COMMAND_COUNT 10  //最大染色命令数

//IP监控，目前考虑同一时刻只能监控一个
struct _Dyeing_IP
{
    uint16  m_u2MaxCount;        //当前需要监控的最大个数
    uint16  m_u2CurrCount;       //当前的已处理的个数
    bool m_blIsValid;            //监控是否有效
    char m_szIP[MAX_BUFF_20];    //用户IP

    _Dyeing_IP()
    {
        m_u2MaxCount  = 0;
        m_u2CurrCount = 0;
        m_blIsValid   = false;
        m_szIP[0]     = '\0';
    }

    void Clear()
    {
        m_u2MaxCount = 0;
        m_u2CurrCount = 0;
        m_blIsValid = false;
        m_szIP[0] = '\0';
    }
};

//监控命令参数
struct _Dyeing_Command
{
    uint16  m_u2CommandID;       //当前CommandID
    uint16  m_u2MaxCount;        //当前需要监控的最大个数
    uint16  m_u2CurrCount;       //当前的已处理的个数
    bool    m_blIsValid;         //监控是否有效

    _Dyeing_Command()
    {
        m_u2CommandID   = 0;
        m_u2MaxCount    = 0;
        m_u2CurrCount   = 0;
        m_blIsValid     = false;
    }

    void Clear()
    {
        m_u2CommandID = 0;
        m_u2MaxCount = 0;
        m_u2CurrCount = 0;
        m_blIsValid = false;
    }
};

typedef vector<_Dyeing_Command> vec_Dyeing_Command_list;  //CommandID染色列表

class CMessageDyeingManager
{
public:
    CMessageDyeingManager();
    ~CMessageDyeingManager();

    void AddDyringIP(const char* pClientIP, uint16 u2MaxCount);                    //添加一个IP染色规则
    bool AddDyeingCommand(uint16 u2CommandID, uint16 u2MaxCount);                  //添加一个命令染色规则

    void GetDyeingCommand(vec_Dyeing_Command_list& objList);                       //获得当前命令染色状态

    string GetTraceID(const char* pClientIP, short sClintPort, uint16 u2CommandID);  //看看是否输出新的traceID

private:
    _Dyeing_Command m_objCommandList[MAX_DYEING_COMMAND_COUNT];   //当前允许数据染色的CommandID列表
    _Dyeing_IP      m_objDyeingIP;                                //当前允许染色的IP
    uint16          m_u2CurrCommandCount;                         //当前正在执行的染色CommandID个数
};

#endif
