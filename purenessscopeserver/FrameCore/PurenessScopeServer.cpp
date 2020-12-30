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
#include "WindowsDump.h"
#include <windows.h>
#include <ProServerManager.h>
#include <tchar.h>
#endif

#include <iostream>
#include <mutex>
#include <chrono>
#include <thread>

//加载所有配置文件中的PacketParse模块
int Load_PacketParse_Module()
{
	auto nPacketParseCount = (int)GetXmlConfigAttribute(xmlPacketParses)->vec.size();
	App_PacketParseLoader::instance()->Init(nPacketParseCount);

	for (const xmlPacketParses::_PacketParse& packetparse : GetXmlConfigAttribute(xmlPacketParses)->vec)
	{
		bool blState = App_PacketParseLoader::instance()->LoadPacketInfo(packetparse.ParseID,
			packetparse.Type,
			packetparse.OrgLength,
			packetparse.ModulePath.c_str(),
			packetparse.ModuleName.c_str());

		if (false == blState)
		{
			//回收隐式加载PacketParse
			App_PacketParseLoader::instance()->Close();
			return -1;
		}
	}

	return 0;
}

#ifdef WIN32
static int32 ServerMain()
{
    //启动主服务器监控
    if (!App_ProServerManager::instance()->Init())
    {
        PSS_LOGGER_DEBUG("[main]App_ProServerManager::instance()->Init() error.");
        App_ProServerManager::instance()->Close();
        return 0;
    }

    if (!App_ProServerManager::instance()->Start())
    {
        PSS_LOGGER_DEBUG("[main]App_ProServerManager::instance()->Start() error.");
        App_ProServerManager::instance()->Close();
        return 0;
    }

    if (GetXmlConfigAttribute(xmlServerType)->Type == 0)
    {
        //等待服务结束
        ACE_Thread_Manager::instance()->wait();
    }

    PSS_LOGGER_DEBUG("[main]Server Run is End.");

    return 0;
}
#endif

#ifndef WIN32
int CheckCoreLimit(int nMaxCoreFile)
{
	//获得当前Core大小设置
	struct rlimit rCorelimit;

	if (getrlimit(RLIMIT_CORE, &rCorelimit) != 0)
	{
		PSS_LOGGER_DEBUG("[CheckCoreLimit]failed to getrlimit number of files.");
		return -1;
	}

	if (nMaxCoreFile != 0)
	{
		PSS_LOGGER_DEBUG("[CheckCoreLimit]** WARNING!WARNING!WARNING!WARNING! **.");
		PSS_LOGGER_DEBUG("[CheckCoreLimit]** PSS WILL AUTO UP CORE SIZE LIMIT **.");
		PSS_LOGGER_DEBUG("[CheckCoreLimit]** WARNING!WARNING!WARNING!WARNING! **.");
		rCorelimit.rlim_cur = RLIM_INFINITY;
		rCorelimit.rlim_max = RLIM_INFINITY;

		if (setrlimit(RLIMIT_CORE, &rCorelimit) != 0)
		{
			PSS_LOGGER_DEBUG("[CheckCoreLimit]failed to setrlimit core size(error={0}).", strerror(errno));
			return -1;
		}
	}
	else
	{
		if ((int)rCorelimit.rlim_cur > 0)
		{
			//不需要Core文件尺寸，在这里把Core文件大小设置成0
			rCorelimit.rlim_cur = (rlim_t)nMaxCoreFile;
			rCorelimit.rlim_max = (rlim_t)nMaxCoreFile;

			if (setrlimit(RLIMIT_CORE, &rCorelimit) != 0)
			{
				PSS_LOGGER_DEBUG("[Checkfilelimit]failed to setrlimit number of files.");
				return -1;
			}
		}
	}

	return 0;
}

//设置当前代码路径
bool SetAppPath()
{
	string strFilePath = fmt::format("/proc/{0}/exe", getpid());

	char szPath[MAX_BUFF_300] = { '\0' };

	//从符号链接中获得当前文件全路径和文件名
	ssize_t stPathSize = readlink(strFilePath.c_str(), szPath, MAX_BUFF_300 - 1);

	if (stPathSize <= 0)
	{
		PSS_LOGGER_DEBUG("[SetAppPath]no find work Path {0}.", szPath);
		return false;
	}

	while (szPath[stPathSize - 1] != '/')
	{
		stPathSize--;
	}

	szPath[stPathSize > 0 ? (stPathSize - 1) : 0] = '\0';

	int nRet = chdir(szPath);

	if (-1 == nRet)
	{
		PSS_LOGGER_DEBUG("[SetAppPath]Set work Path ({0}) fail.", szPath);
	}
	else
	{
		PSS_LOGGER_DEBUG("[SetAppPath]Set work Path ({0}) OK.", szPath);
	}

	return true;
}

//子进程程序
int Chlid_Run(std::thread& th_monitor)
{
    //初始化输出
    Init_Console_Output(GetXmlConfigAttribute(xmlAceDebug)->TrunOn,
        GetXmlConfigAttribute(xmlAceDebug)->FileCount,
        GetXmlConfigAttribute(xmlAceDebug)->LogFileMaxSize,
        GetXmlConfigAttribute(xmlAceDebug)->ConsoleName,
        GetXmlConfigAttribute(xmlAceDebug)->Level);

	//判断是否是需要以服务的状态启动
	if (GetXmlConfigAttribute(xmlServerType)->Type == 1)
	{
		PSS_LOGGER_DEBUG("[main]Procress is run background.");
		Gdaemon();
	}

	//显式加载PacketParse
	if (0 != Load_PacketParse_Module())
	{
		pthread_exit(nullptr);
	}

	//判断当前Core文件尺寸是否需要调整
	if (-1 == CheckCoreLimit(GetXmlConfigAttribute(xmlCoreSetting)->CoreNeed))
	{
		return 0;
	}

	//设置监控信号量的线程
	//关闭消息队列条件变量
	std::mutex g_mutex;
	WaitQuitSignal g_wait_sigal;

	g_wait_sigal.init();

	//监控异常中断信号量
	th_monitor = std::thread([g_wait_sigal, &g_mutex ]()
		{
			std::lock_guard<std::mutex> lk(g_mutex);

			bool blFlag = true;

			while (g_wait_sigal.wait(blFlag))
			{
				std::this_thread::sleep_for(std::chrono::microseconds(500));
			}

			PSS_LOGGER_DEBUG("[thread_Monitor]exit.");
		});

	//等待线程锁生效
	ACE_Time_Value tvSleep(0, 1000);
	ACE_OS::sleep(tvSleep);

	//第二步，启动主服务器监控
	if (!App_ServerManager::instance()->Init())
	{
		PSS_LOGGER_DEBUG("[main]App_ServerManager::instance()->Init() error.");
		App_ServerManager::instance()->Close();
		App_PacketParseLoader::instance()->Close();
		return 0;
	}

	PSS_LOGGER_DEBUG("[CServerManager::Start]Begin.");

	if (!App_ServerManager::instance()->Start())
	{
		PSS_LOGGER_DEBUG("[main]App_ServerManager::instance()->Start() error.");
		App_ServerManager::instance()->Close();
		App_PacketParseLoader::instance()->Close();
		return 0;
	}

	PSS_LOGGER_DEBUG("[main]Server Run is End.");

	std::lock_guard<std::mutex> lk(g_mutex);

	PSS_LOGGER_DEBUG("[main]Server Exit.");

	//回收隐式加载PacketParse
	App_PacketParseLoader::instance()->Close();

	th_monitor.join();

	return 0;
}

int ACE_TMAIN(int argc, ACE_TCHAR* argv[])
{
	if (argc > 0)
	{
		PSS_LOGGER_DEBUG("[main]argc = {0}.", argc);

		for (int i = 0; i < argc; i++)
		{
			PSS_LOGGER_DEBUG("[main]argc({0}) = {1}.", argc, argv[i]);
		}
	}

	//首先设置当前工作路径
	if (false == SetAppPath())
	{
		PSS_LOGGER_DEBUG("[main]SetAppPath error.");
	}

	//读取配置文件
	if (!App_XmlConfig::instance()->InitIsOk())
	{
		PSS_LOGGER_DEBUG("[main]App_XmlConfig::instance()->InitIsOk() is false.");
		return 0;
	}

	std::thread th_monitor;
	if (0 != Chlid_Run(th_monitor))
	{
		PSS_LOGGER_DEBUG("[main]Chlid_Run error.");
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
	TCHAR szFileName[MAX_PATH] = { 0 };
	GetModuleFileName(0, szFileName, MAX_PATH);
    LPTSTR pszEnd = _tcsrchr(szFileName, TEXT('\\'));

    if (pszEnd != 0)
    {
        pszEnd++;
        *pszEnd = 0;
    }

	PSS_LOGGER_DEBUG("[main]PSS is Starting.");

	SetCurrentDirectory(szFileName);

	//添加Dump文件
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)exception_handler);
	base_set_crash_handler(main_crash_handler, nullptr);

	//添加对Console  由于windows系统的机制导致ctrlhandler有时无法正常执行结束就被系统结束掉进程暂时关闭windows上的回调
#ifndef WIN32
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)ctrlhandler, true);
#endif

	//第一步，读取配置文件
	if (!App_XmlConfig::instance()->InitIsOk())
	{
		PSS_LOGGER_DEBUG("[main]!App_XmlConfig->InitIsOk() is false.");
		return 0;
	}

	//初始化输出
	Init_Console_Output(GetXmlConfigAttribute(xmlAceDebug)->TrunOn,
		GetXmlConfigAttribute(xmlAceDebug)->FileCount,
		GetXmlConfigAttribute(xmlAceDebug)->LogFileMaxSize,
		GetXmlConfigAttribute(xmlAceDebug)->ConsoleName,
		GetXmlConfigAttribute(xmlAceDebug)->Level);

	PSS_LOGGER_DEBUG("[main]PSS is load conf ok.");

	//显式加载PacketParse
	if (0 != Load_PacketParse_Module())
	{
		return 0;
	}

	//正常启动
	ServerMain();

	//回收隐式加载PacketParse
	App_PacketParseLoader::instance()->Close();

	//如果日志流不等于空，则回收

	return 0;
}

#endif


