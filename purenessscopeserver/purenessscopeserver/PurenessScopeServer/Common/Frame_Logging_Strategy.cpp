#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/Thread_Manager.h"
#include "ace/ARGV.h"
#include "ace/Reactor.h"

#include "ace/TP_Reactor.h"
#include "ace/Dev_Poll_Reactor.h"
#include "Frame_Logging_Strategy.h"



static void *run_reactor (void *pReactor)
{
    ACE_Reactor *pLogReactor = (ACE_Reactor *)pReactor;

    pLogReactor->owner(ACE_Thread_Manager::instance ()->thr_self());
    pLogReactor->run_reactor_event_loop ();

    ACE_DEBUG((LM_INFO, ACE_TEXT("(%P|%t) %M run_reactor exit[%N,%l]\n")));
    return 0;
}


Logging_Config_Param::Logging_Config_Param()
{
    //日志文件，全路径
    ACE_OS::strcpy(m_strLogFile, "./serverdebug.log");

    //文件大小检测时间(Secs)
    m_iChkInterval = 600;

    //每个日志文件最大大小(KB), 默认10M
    m_iLogFileMaxSize = 10240;

    //日志文件最大个数
    m_iLogFileMaxCnt = 3;

    //是否向终端发送
    m_bSendTerminal = 0;

    //日志级别
    memset(m_strLogLevel,0,sizeof(m_strLogLevel));
}

Logging_Config_Param::~Logging_Config_Param()
{
   ACE_DEBUG((LM_INFO, ACE_TEXT("(%P|%t) %M ~Logging_Config_Param[%N,%l]\n")));
}

//日志策略
Frame_Logging_Strategy::Frame_Logging_Strategy()
{
    pLogStraReactor = NULL;
    pLogStrategy = NULL;
}

Frame_Logging_Strategy::~Frame_Logging_Strategy()
{
    if(pLogStraReactor != NULL)
    {
		pLogStraReactor->close();
        delete pLogStraReactor;
    }

    if (pLogStrategy != NULL)
    {
        pLogStrategy->reactor(NULL);
        delete pLogStrategy;
    }
}

string Frame_Logging_Strategy::GetLogLevel(const string &strLogLevel)
{
    string strOutLogLevel="";
    if(strLogLevel == "DEBUG")
    {
        strOutLogLevel = "DEBUG|INFO|NOTICE|WARNING|ERROR|CRITICAL|ALERT|EMERGENCY";
    }
    else if(strLogLevel == "INFO")
    {
        strOutLogLevel = "~DEBUG|INFO|NOTICE|WARNING|ERROR|CRITICAL|ALERT|EMERGENCY";
    }
    else if(strLogLevel=="NOTICE")
    {
        strOutLogLevel = "~DEBUG|~INFO|NOTICE|WARNING|ERROR|CRITICAL|ALERT|EMERGENCY";
    }
    else if(strLogLevel=="WARNING")
    {
        strOutLogLevel = "~DEBUG|~INFO|~NOTICE|WARNING";
    }
    else if(strLogLevel=="ERROR")
    {
        strOutLogLevel = "~DEBUG|~INFO|~WARNING|~NOTICE|ERROR|CRITICAL|ALERT|EMERGENCY";
    }
    else if(strLogLevel=="CRITICAL")
    {
        strOutLogLevel = "~DEBUG|~INFO|~WARNING|~NOTICE|~ERROR|CRITICAL|ALERT|EMERGENCY";
    }
    else if(strLogLevel=="ALERT")
    {
        strOutLogLevel = "~DEBUG|~INFO|~WARNING|~NOTICE|~ERROR|~CRITICAL|ALERT|EMERGENCY";
    }
    else if(strLogLevel=="EMERGENCY")
    {
        strOutLogLevel = "~DEBUG|~INFO|~WARNING|~NOTICE|~ERROR|~CRITICAL|~ALERT|EMERGENCY";
    }
    else
    {
        strOutLogLevel = "DEBUG|INFO|WARNING|NOTICE|ERROR|CRITICAL|ALERT|EMERGENCY";
    }
    return strOutLogLevel;
}

//初始化日志策略
int Frame_Logging_Strategy::InitLogStrategy(Logging_Config_Param &ConfigParam)
{
    //Set Arg List
    char cmdline[1024] = {0};
    int iCfgParamCnt = 0;

    string strTemp = ConfigParam.m_strLogLevel;
    string strLogLevel = GetLogLevel(strTemp);

    if(ConfigParam.m_bSendTerminal)
    {
        iCfgParamCnt = 1;
        ACE_OS::sprintf(cmdline,"-s %s -f STDERR -p %s -i %d -m %d -N %d",
                        ConfigParam.m_strLogFile,
                        strLogLevel.c_str(),
                        ConfigParam.m_iChkInterval,
                        ConfigParam.m_iLogFileMaxSize,
                        ConfigParam.m_iLogFileMaxCnt);
    }
    else
    {
        iCfgParamCnt = LOG_CONFIG_ARGV_COUNT;
        ACE_OS::sprintf(cmdline,"-s %s -f OSTREAM -p %s -i %d -m %d -N %d",
                        ConfigParam.m_strLogFile,
                        strLogLevel.c_str(),
                        ConfigParam.m_iChkInterval,
                        ConfigParam.m_iLogFileMaxSize,
                        ConfigParam.m_iLogFileMaxCnt);
    }

    ACE_Reactor_Impl * pImpl = 0;

    ACE_NEW_RETURN (pImpl, ACE_TP_Reactor, -1);
    

    //ACE_NEW_RETURN(pLogStraReactor, ACE_Reactor(pImpl ,1), -1);
    ACE_NEW_RETURN(pLogStraReactor, ACE_Reactor, -1);
    ACE_NEW_RETURN(pLogStrategy, My_ACE_Logging_Strategy, -1);

    //Set Reactor
    pLogStrategy->reactor(pLogStraReactor);

    ACE_ARGV args;
    //args.add(__argv[0]);
    args.add(ACE_TEXT(cmdline));

    pLogStrategy->init(args.argc(),args.argv());

    if (ACE_Thread_Manager::instance ()->spawn(ACE_THR_FUNC (run_reactor), (void *)pLogStraReactor) == -1)
    {
        ACE_ERROR_RETURN ((LM_ERROR,"Spawning Reactor.\n"),-1);
    }

    ACE_DEBUG((LM_INFO, ACE_TEXT("(%P|%t) %M Init Log Strategy Success [%N,%l]\n")));
    return 0;
}

int Frame_Logging_Strategy::InitLogStrategy()
{
    Logging_Config_Param cfgParam;
    return InitLogStrategy(cfgParam);
}

//结束策略
int Frame_Logging_Strategy::EndLogStrategy()
{
    pLogStraReactor->end_event_loop();
    return 0;
}

