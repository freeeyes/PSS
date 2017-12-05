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

#include "MainConfig.h"
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

//关闭消息队列条件变量
ACE_Thread_Mutex g_mutex;
ACE_Condition<ACE_Thread_Mutex> g_cond(g_mutex);


//监控信号量线程
void* thread_Monitor(void* arg)
{
    if(NULL != arg)
    {
        OUR_DEBUG((LM_INFO, "[thread_Monitor]arg is not NULL.\n"));
        pthread_exit(0);
    }

    bool blFlag = true;

    while(WaitQuitSignal::wait(blFlag))
    {
        //OUR_DEBUG((LM_INFO, "[thread_Monitor]blFlag=false.\n"));
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
        //OUR_DEBUG((LM_INFO, "[CheckCoreLimit]rlim.rlim_cur=%d, nMaxOpenFile=%d, openfile is not enougth， please check [ulimit -a].\n", (int)rCorelimit.rlim_cur, nMaxCoreFile));
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

    //OUR_DEBUG((LM_INFO, "[CheckCoreLimit]rlim.rlim_cur=%d, nMaxOpenFile=%d, openfile is not enougth， please check [ulimit -a].\n", (int)rCorelimit.rlim_cur, nMaxCoreFile));
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

    if(NULL != pFilePath)
    {
        char szPath[MAX_BUFF_300] = { '\0' };
        memset(pFilePath, 0, nSize);
        sprintf(pFilePath,"/proc/%d/exe",getpid());

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
    else
    {
        OUR_DEBUG((LM_INFO, "[SetAppPath]Set work Path[null].\n"));
        return false;
    }
}

//获得当前文件打开数
int Checkfilelimit(int nMaxOpenFile)
{
    //获得当前文件打开数
    struct rlimit rfilelimit;

    if (getrlimit(RLIMIT_NOFILE, &rfilelimit) != 0)
    {
        OUR_DEBUG((LM_INFO, "[Checkfilelimit]failed to getrlimit number of files.\n"));
        return -1;
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[Checkfilelimit]rfilelimit.rlim_cur=%d,nMaxOpenFile=%d.\n", rfilelimit.rlim_cur, nMaxOpenFile));

        //提示同时文件打开数不足，需要设置。
        if((int)rfilelimit.rlim_cur < nMaxOpenFile)
        {
            OUR_DEBUG((LM_INFO, "[Checkfilelimit]** WARNING!WARNING!WARNING!WARNING! **.\n"));
            OUR_DEBUG((LM_INFO, "[Checkfilelimit]** PSS WILL AUTO UP FILE OPEN LIMIT **.\n"));
            OUR_DEBUG((LM_INFO, "[Checkfilelimit]** WARNING!WARNING!WARNING!WARNING! **.\n"));
            //这段自动提升的功能暂时注释，运维人员必须知道这个问题并自己设置，这是上选。
            //尝试临时提高并行文件数
            rfilelimit.rlim_cur = (rlim_t)nMaxOpenFile;
            rfilelimit.rlim_max = (rlim_t)nMaxOpenFile;

            if (setrlimit(RLIMIT_NOFILE, &rfilelimit)!= 0)
            {
                OUR_DEBUG((LM_INFO, "[Checkfilelimit]failed to setrlimit number of files(error=%s).\n", strerror(errno)));
                return -1;
            }

            //如果修改成功，再次检查一下
            if (getrlimit(RLIMIT_NOFILE, &rfilelimit) != 0)
            {
                OUR_DEBUG((LM_INFO, "[Checkfilelimit]failed to getrlimit number of files.\n"));
                return -1;
            }

            //再次检查修改后的文件句柄数
            if((int)rfilelimit.rlim_cur < nMaxOpenFile)
            {
                OUR_DEBUG((LM_INFO, "[Checkfilelimit]rlim.rlim_cur=%d, nMaxOpenFile=%d, openfile is not enougth， please check [ulimit -a].\n", (int)rfilelimit.rlim_cur, nMaxOpenFile));
                return -1;
            }

            //OUR_DEBUG((LM_INFO, "[Checkfilelimit]rlim.rlim_cur=%d, nMaxOpenFile=%d, openfile is not enougth， please check [ulimit -a].\n", (int)rfilelimit.rlim_cur, nMaxOpenFile));
            return 0;
        }
    }

    return 0;
}

void Gdaemon()
{
    pid_t pid;

    signal(SIGTTOU,SIG_IGN);
    signal(SIGTTIN,SIG_IGN);
    signal(SIGTSTP,SIG_IGN);

    if(setpgrp() == -1)
    {
        perror("setpgrp failure");
    }

    signal(SIGHUP,SIG_IGN);

    if((pid = fork()) < 0)
    {
        perror("fork failure");
        exit(1);
    }
    else if(pid > 0)
    {
        exit(0);
    }

    setsid();
    umask(0);

    signal(SIGCLD,SIG_IGN);
    signal(SIGCHLD,SIG_IGN);
    signal(SIGPIPE,SIG_IGN);
}

//子进程程序
int Chlid_Run()
{
    //显式加载PacketParse
    App_PacketParseLoader::instance()->Init(App_MainConfig::instance()->GetPacketParseCount());

    for (uint8 i = 0; i < App_MainConfig::instance()->GetPacketParseCount(); i++)
    {
        _PacketParseInfo* pPacketParseInfo = App_MainConfig::instance()->GetPacketParseInfo(i);
        bool blState = App_PacketParseLoader::instance()->LoadPacketInfo(pPacketParseInfo->m_u4PacketID,
                       pPacketParseInfo->m_u1Type,
                       pPacketParseInfo->m_u4OrgLength,
                       pPacketParseInfo->m_szPacketParsePath,
                       pPacketParseInfo->m_szPacketParseName);

        if (false == blState)
        {
            //回收隐式加载PacketParse
            App_PacketParseLoader::instance()->Close();

            pthread_exit(NULL);

            return 0;
        }
    }

    //判断是否是需要以服务的状态启动
    if(App_MainConfig::instance()->GetServerType() == 1)
    {
        OUR_DEBUG((LM_INFO, "[main]Procress is run background.\n"));
        //daemon(1,1);
        Gdaemon();
    }

    //判断当前并行连接数是否支持框架
    //if(-1 == Checkfilelimit(App_MainConfig::instance()->GetMaxHandlerCount()))
    //{
    //  return 0;
    //}

    //判断当前Core文件尺寸是否需要调整
    if(-1 == CheckCoreLimit(App_MainConfig::instance()->GetCoreFileSize()))
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

    pthread_exit(NULL);

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
    if(!App_MainConfig::instance()->Init())
    {
        OUR_DEBUG((LM_INFO, "[main]%s\n", App_MainConfig::instance()->GetError()));
        return 0;
    }
    else
    {
        App_MainConfig::instance()->Display();
    }

    if (0 != Chlid_Run())
    {
        OUR_DEBUG((LM_INFO, "[main]Chlid_Run error.\n"));
    }

    return 0;
}

#else
//如果是windows
#include "WindowsProcess.h"
#include "WindowsDump.h"

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

    SetCurrentDirectory(szFileName);

    //添加Dump文件
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);

    //第一步，读取配置文件
    if(!App_MainConfig::instance()->Init())
    {
        OUR_DEBUG((LM_INFO, "[main]%s\n", App_MainConfig::instance()->GetError()));
    }
    else
    {
        App_MainConfig::instance()->Display();
    }

    //隐式加载PacketParse
    App_PacketParseLoader::instance()->Init(App_MainConfig::instance()->GetPacketParseCount());

    for (uint8 i = 0; i < App_MainConfig::instance()->GetPacketParseCount(); i++)
    {
        _PacketParseInfo* pPacketParseInfo = App_MainConfig::instance()->GetPacketParseInfo(i);
        bool blState = App_PacketParseLoader::instance()->LoadPacketInfo(pPacketParseInfo->m_u4PacketID,
                       pPacketParseInfo->m_u1Type,
                       pPacketParseInfo->m_u4OrgLength,
                       pPacketParseInfo->m_szPacketParsePath,
                       pPacketParseInfo->m_szPacketParseName);

        if (false == blState)
        {
            //回收隐式加载PacketParse
            App_PacketParseLoader::instance()->Close();

            if (App_MainConfig::instance()->GetServerType() == 1)
            {
                App_Process::instance()->stopprocesslog();
            }

            return 0;
        }
    }

    //判断是否是需要以服务的状态启动
    if(App_MainConfig::instance()->GetServerType() == 1)
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

    if(App_MainConfig::instance()->GetServerType() == 1)
    {
        App_Process::instance()->stopprocesslog();
    }

    return 0;
}

#endif


