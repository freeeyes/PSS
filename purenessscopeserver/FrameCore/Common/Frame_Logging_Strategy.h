
#pragma once

#include "MyACELoggingStrategy.h"
#include <string>

#define LOG_CONFIG_ARGV_COUNT   6

//修复ACE_Logging_Strategy的一个BUG
//解决在多线程下双写导致的tellp()函数线程安全的问题
//不想修改ACE源码，在这里打一个补丁

class Logging_Config_Param
{
public:
    Logging_Config_Param();
    ~Logging_Config_Param();

    Logging_Config_Param(const Logging_Config_Param& ar);

    Logging_Config_Param& operator = (const Logging_Config_Param& ar)
    {
        ACE_UNUSED_ARG(ar);

        return *this;
    }


    //文件大小检测时间(Secs)
    int m_iChkInterval;

    //每个日志文件最大大小(KB)
    int m_iLogFileMaxSize;

    //日志文件最大个数
    int m_iLogFileMaxCnt;

    //是否向终端发送
    int m_bSendTerminal;

    //日志等级描述
    char m_strLogLevel[128];

    //日志文件，全路径
    char m_strLogFile[256];
};

class Frame_Logging_Strategy
{
public:
    Frame_Logging_Strategy();
    ~Frame_Logging_Strategy();

    Frame_Logging_Strategy(const Frame_Logging_Strategy& ar);

    Frame_Logging_Strategy& operator = (const Frame_Logging_Strategy& ar)
    {
        ACE_UNUSED_ARG(ar);

        return *this;
    }

    //日志级别
    std::string GetLogLevel(const std::string& strLogLevel);

    int InitLogStrategy();

    //初始化日志策略
    int InitLogStrategy(Logging_Config_Param& ConfigParam);

    //结束策略
    int EndLogStrategy();

    //得到pLogStrategy指针
    My_ACE_Logging_Strategy* GetStrategy();

private:
    ACE_Reactor* pLogStraReactor;
    My_ACE_Logging_Strategy* pLogStrategy;
};

