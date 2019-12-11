// PurenessScopeServer.cpp : 定义控制台应用程序的入口点。
//
// 有时候的说，重新站起来绝对不是一件容易的事情，但是我现在必须站起来。要做好样的，freeeyes
// add by freeeyes, freebird92
// 2008-12-22(冬至)
// 从Twitter的主程序中吸收了一些有用的小技巧，不断融合。
// 没有目标的坚持是无用的，努力的让PSS更加适合开发，尽量减少开发者开发量，让写代码变的舒适。
// 并提供更完备的错误分析机制，和一些更好的程序编程技巧。
// 现在PSS不是一个人在努力了，它有了更多的伙伴，他们会帮助你不断成长。
// add by freeeyes
// 2013-09-24

#include "Frame_Logging_Strategy.h"

#ifndef WIN32
//如果是Linux
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "WaitQuitSignal.h"
#include "ServerManager.h"

#include "ace/Thread.h"
#include "ace/Synch.h"
#else
//如果是windows
#include "WindowsProcess.h"
#include "WindowsDump.h"
#include <windows.h>
#endif

//加载所有配置文件中的PacketParse模块
int Load_PacketParse_Module()
{
    int nPacketParseCount = (int)GetXmlConfigAttribute(xmlPacketParses)->vec.size();
    App_PacketParseLoader::instance()->Init(nPacketParseCount);

    for (uint8 i = 0; i < nPacketParseCount; i++)
    {
        bool blState = App_PacketParseLoader::instance()->LoadPacketInfo(GetXmlConfigAttribute(xmlPacketParses)->vec[i].ParseID,
                       GetXmlConfigAttribute(xmlPacketParses)->vec[i].Type,
                       GetXmlConfigAttribute(xmlPacketParses)->vec[i].OrgLength,
                       GetXmlConfigAttribute(xmlPacketParses)->vec[i].ModulePath.c_str(),
                       GetXmlConfigAttribute(xmlPacketParses)->vec[i].ModuleName.c_str());

        if (false == blState)
        {
            //回收隐式加载PacketParse
            App_PacketParseLoader::instance()->Close();
            return -1;
        }
    }

    return 0;
}

#ifndef WIN32
//关闭消息队列条件变量
ACE_Thread_Mutex g_mutex;
ACE_Condition<ACE_Thread_Mutex> g_cond(g_mutex);


//监控信号量线程
void* thread_Monitor(void* arg)
{
    ACE_UNUSED_ARG(arg);

    g_mutex.acquire();

    bool blFlag = true;

    while(WaitQuitSignal::wait(blFlag))
    {
        sleep(1);
    }

    g_cond.signal();
    g_mutex.release();

    OUR_DEBUG((LM_INFO, "[thread_Monitor]exit.\n"));
    pthread_exit(0);
}

int CheckCoreLimit(int nMaxCoreFile)
{
    //获得当前Core大小设置
    struct rlimit rCorelimit;

    if(getrlimit(RLIMIT_CORE, &rCorelimit) != 0)
    {
        OUR_DEBUG((LM_INFO, "[CheckCoreLimit]failed to getrlimit number of files.\n"));
        return -1;
    }

    if(nMaxCoreFile != 0)
    {
        OUR_DEBUG((LM_INFO, "[CheckCoreLimit]** WARNING!WARNING!WARNING!WARNING! **.\n"));
        OUR_DEBUG((LM_INFO, "[CheckCoreLimit]** PSS WILL AUTO UP CORE SIZE LIMIT **.\n"));
        OUR_DEBUG((LM_INFO, "[CheckCoreLimit]** WARNING!WARNING!WARNING!WARNING! **.\n"));
        rCorelimit.rlim_cur = RLIM_INFINITY;
        rCorelimit.rlim_max = RLIM_INFINITY;

        if (setrlimit(RLIMIT_CORE, &rCorelimit)!= 0)
        {
            OUR_DEBUG((LM_INFO, "[CheckCoreLimit]failed to setrlimit core size(error=%s).\n", strerror(errno)));
            return -1;
        }
    }
    else
    {
        if((int)rCorelimit.rlim_cur > 0)
        {
            //不需要Core文件尺寸，在这里把Core文件大小设置成0
            rCorelimit.rlim_cur = (rlim_t)nMaxCoreFile;
            rCorelimit.rlim_max = (rlim_t)nMaxCoreFile;

            if (setrlimit(RLIMIT_CORE, &rCorelimit)!= 0)
            {
                OUR_DEBUG((LM_INFO, "[Checkfilelimit]failed to setrlimit number of files.\n"));
                return -1;
            }
        }
    }

    return 0;
}

//设置当前代码路径
bool SetAppPath()
{
    int nSize = (int)pathconf(".",_PC_PATH_MAX);

    if (nSize <= 0)
    {
        OUR_DEBUG((LM_INFO, "[SetAppPath]pathconf is error(%d).\n", nSize));
        return false;
    }

    char* pFilePath = new char[nSize];

    char szPath[MAX_BUFF_300] = { '\0' };
    memset(pFilePath, 0, nSize);
    ACE_OS::snprintf(pFilePath, MAX_BUFF_300, "/proc/%d/exe", getpid());

    //从符号链接中获得当前文件全路径和文件名
    ssize_t stPathSize = readlink(pFilePath, szPath, MAX_BUFF_300 - 1);

    if (stPathSize <= 0)
    {
        OUR_DEBUG((LM_INFO, "[SetAppPath]no find work Path.\n", szPath));
        SAFE_DELETE_ARRAY(pFilePath);
        return false;
    }
    else
    {
        SAFE_DELETE_ARRAY(pFilePath);
    }

    while(szPath[stPathSize - 1]!='/')
    {
        stPathSize--;
    }

    szPath[stPathSize > 0 ? (stPathSize-1) : 0]= '\0';

    int nRet = chdir(szPath);

    if (-1 == nRet)
    {
        OUR_DEBUG((LM_INFO, "[SetAppPath]Set work Path (%s) fail.\n", szPath));
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[SetAppPath]Set work Path (%s) OK.\n", szPath));
    }

    return true;
}

//子进程程序
int Chlid_Run()
{
    //判断是否是需要以服务的状态启动
    if(GetXmlConfigAttribute(xmlServerType)->Type == 1)
    {
        OUR_DEBUG((LM_INFO, "[main]Procress is run background.\n"));
        Gdaemon();
    }

    //显式加载PacketParse
    if (0 != Load_PacketParse_Module())
    {
        pthread_exit(NULL);
    }

    //判断当前Core文件尺寸是否需要调整
    if(-1 == CheckCoreLimit(GetXmlConfigAttribute(xmlCoreSetting)->CoreNeed))
    {
        return 0;
    }

    //设置监控信号量的线程
    WaitQuitSignal::init();

    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_create(&tid, &attr, thread_Monitor, NULL);

    //等待线程锁生效
    ACE_Time_Value tvSleep(0, 1000);
    ACE_OS::sleep(tvSleep);

    //第二步，启动主服务器监控
    if(!App_ServerManager::instance()->Init())
    {
        OUR_DEBUG((LM_INFO, "[main]App_ServerManager::instance()->Init() error.\n"));
        App_ServerManager::instance()->Close();
        App_PacketParseLoader::instance()->Close();
        return 0;
    }

    OUR_DEBUG((LM_INFO, "[CServerManager::Start]Begin.\n"));

    if(!App_ServerManager::instance()->Start())
    {
        OUR_DEBUG((LM_INFO, "[main]App_ServerManager::instance()->Start() error.\n"));
        App_ServerManager::instance()->Close();
        App_PacketParseLoader::instance()->Close();
        return 0;
    }

    OUR_DEBUG((LM_INFO, "[main]Server Run is End.\n"));

    g_mutex.acquire();

    OUR_DEBUG((LM_INFO, "[main]Server Exit.\n"));

    //回收隐式加载PacketParse
    App_PacketParseLoader::instance()->Close();

    return 0;
}

int ACE_TMAIN(int argc, ACE_TCHAR* argv[])
{
    if(argc > 0)
    {
        OUR_DEBUG((LM_INFO, "[main]argc = %d.\n", argc));

        for(int i = 0; i < argc; i++)
        {
            OUR_DEBUG((LM_INFO, "[main]argc(%d) = %s.\n", argc, argv[i]));
        }
    }

    //首先设置当前工作路径
    if (false == SetAppPath())
    {
        OUR_DEBUG((LM_INFO, "[main]SetAppPath error.\n"));
    }

    //读取配置文件
    if(!App_XmlConfig::instance()->InitIsOk())
    {
        OUR_DEBUG((LM_INFO, "[main]App_XmlConfig::instance()->InitIsOk() is false.\n"));
        return 0;
    }

    if (0 != Chlid_Run())
    {
        OUR_DEBUG((LM_INFO, "[main]Chlid_Run error.\n"));
    }

    return 0;
}

#else

bool ctrlhandler(DWORD fdwctrltype)
{
    switch (fdwctrltype)
    {
    // handle the ctrl-c signal.
    case CTRL_C_EVENT:
        printf("ctrl-c event\n\n");
        return(true);

    // ctrl-close: confirm that the user wants to exit.
    case CTRL_CLOSE_EVENT:
        printf("ctrl-close event\n\n");
        App_ProServerManager::instance()->Close();
        return(true);

    // pass other signals to the next handler.
    case CTRL_BREAK_EVENT:
        printf("ctrl-break event\n\n");
        return false;

    case CTRL_LOGOFF_EVENT:
        printf("ctrl-logoff event\n\n");
        return false;

    case CTRL_SHUTDOWN_EVENT:
        printf("ctrl-shutdown event\n\n");
        App_ProServerManager::instance()->Close();
        return false;

    default:
        return false;
    }
}

int ACE_TMAIN(int argc, ACE_TCHAR* argv[])
{
    //指定当前目录，防止访问文件失败
    TCHAR szFileName[MAX_PATH] = {0};
    GetModuleFileName(0, szFileName, MAX_PATH);
    LPTSTR pszEnd = _tcsrchr(szFileName, TEXT('\\'));

    if (pszEnd != 0)
    {
        pszEnd++;
        *pszEnd = 0;
    }

    OUR_DEBUG((LM_INFO, "[main]PSS is Starting.\n"));

    SetCurrentDirectory(szFileName);

    //添加Dump文件
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)exception_handler);
    base_set_crash_handler(main_crash_handler, nullptr);

    //添加对Console  由于windows系统的机制导致ctrlhandler有时无法正常执行结束就被系统结束掉进程暂时关闭windows上的回调
#ifndef WIN32
    SetConsoleCtrlHandler((PHANDLER_ROUTINE)ctrlhandler, true);
#endif

    //第一步，读取配置文件
    if(!App_XmlConfig::instance()->InitIsOk())
    {
        OUR_DEBUG((LM_INFO, "[main]!App_XmlConfig->InitIsOk() is false.\n"));
        return 0;
    }

    //显式加载PacketParse
    if (0 != Load_PacketParse_Module())
    {
        return 0;
    }

    //判断是否是需要以服务的状态启动
    if(GetXmlConfigAttribute(xmlServerType)->Type == 1)
    {
        App_Process::instance()->startprocesslog();

        //以服务状态启动
        //首先看有没有配置启动windows服务
        App_Process::instance()->run(argc, argv);
    }
    else
    {
        //正常启动
        ServerMain();
    }

    //回收隐式加载PacketParse
    App_PacketParseLoader::instance()->Close();

    if(GetXmlConfigAttribute(xmlServerType)->Type == 1)
    {
        App_Process::instance()->stopprocesslog();
    }

    return 0;
}

#endif


