
#pragma once

#include "MyACELoggingStrategy.h"
#include <string>
using namespace std;

#define LOG_CONFIG_ARGV_COUNT	6

//修复ACE_Logging_Strategy的一个BUG
//解决在多线程下双写导致的tellp()函数线程安全的问题
//不想修改ACE源码，在这里打一个补丁

class Logging_Config_Param
{
public:
	Logging_Config_Param();
	~Logging_Config_Param();

	//日志文件，全路径
	char m_strLogFile[256];
	
	//文件大小检测时间(Secs)
	int m_iChkInterval;

	//每个日志文件最大大小(KB)
	int m_iLogFileMaxSize;

	//日志文件最大个数
	int m_iLogFileMaxCnt;

	//是否向终端发送
	int m_bSendTerminal;

    char m_strLogLevel[128];
};

class Frame_Logging_Strategy
{
public:
	Frame_Logging_Strategy();
	~Frame_Logging_Strategy();

    //日志级别
    string GetLogLevel(const string &strLogLevel);
	
		int InitLogStrategy();

    //初始化日志策略
    int InitLogStrategy(Logging_Config_Param &ConfigParam);
	
    //结束策略
    int EndLogStrategy();

private:
	ACE_Reactor *pLogStraReactor;
	My_ACE_Logging_Strategy *pLogStrategy; 
};

