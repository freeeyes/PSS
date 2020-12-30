// PurenessScopeServer.cpp : �������̨Ӧ�ó������ڵ㡣
//
// ��ʱ���˵������վ�������Բ���һ�����׵����飬���������ڱ���վ������Ҫ�������ģ�freeeyes
// add by freeeyes, freebird92
// 2008-12-22(����)
// ��Twitter����������������һЩ���õ�С���ɣ������ںϡ�
// û��Ŀ��ļ�������õģ�Ŭ������PSS�����ʺϿ������������ٿ����߿���������д���������ʡ�
// ���ṩ���걸�Ĵ���������ƣ���һЩ���õĳ����̼��ɡ�
// ����PSS����һ������Ŭ���ˣ������˸���Ļ�飬���ǻ�����㲻�ϳɳ���
// add by freeeyes
// 2013-09-24

#ifndef WIN32
//�����Linux
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "WaitQuitSignal.h"
#include "ServerManager.h"

#include "ace/Thread.h"
#include "ace/Synch.h"
#else
//�����windows
#include "WindowsDump.h"
#include <windows.h>
#include <ProServerManager.h>
#include <tchar.h>
#endif

#include <iostream>
#include <mutex>
#include <chrono>
#include <thread>

//�������������ļ��е�PacketParseģ��
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
			//������ʽ����PacketParse
			App_PacketParseLoader::instance()->Close();
			return -1;
		}
	}

	return 0;
}

#ifdef WIN32
static int32 ServerMain()
{
    //���������������
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
        //�ȴ��������
        ACE_Thread_Manager::instance()->wait();
    }

    PSS_LOGGER_DEBUG("[main]Server Run is End.");

    return 0;
}
#endif

#ifndef WIN32
int CheckCoreLimit(int nMaxCoreFile)
{
	//��õ�ǰCore��С����
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
			//����ҪCore�ļ��ߴ磬�������Core�ļ���С���ó�0
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

//���õ�ǰ����·��
bool SetAppPath()
{
	string strFilePath = fmt::format("/proc/{0}/exe", getpid());

	char szPath[MAX_BUFF_300] = { '\0' };

	//�ӷ��������л�õ�ǰ�ļ�ȫ·�����ļ���
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

//�ӽ��̳���
int Chlid_Run(std::thread& th_monitor)
{
    //��ʼ�����
    Init_Console_Output(GetXmlConfigAttribute(xmlAceDebug)->TrunOn,
        GetXmlConfigAttribute(xmlAceDebug)->FileCount,
        GetXmlConfigAttribute(xmlAceDebug)->LogFileMaxSize,
        GetXmlConfigAttribute(xmlAceDebug)->ConsoleName,
        GetXmlConfigAttribute(xmlAceDebug)->Level);

	//�ж��Ƿ�����Ҫ�Է����״̬����
	if (GetXmlConfigAttribute(xmlServerType)->Type == 1)
	{
		PSS_LOGGER_DEBUG("[main]Procress is run background.");
		Gdaemon();
	}

	//��ʽ����PacketParse
	if (0 != Load_PacketParse_Module())
	{
		pthread_exit(nullptr);
	}

	//�жϵ�ǰCore�ļ��ߴ��Ƿ���Ҫ����
	if (-1 == CheckCoreLimit(GetXmlConfigAttribute(xmlCoreSetting)->CoreNeed))
	{
		return 0;
	}

	//���ü���ź������߳�
	//�ر���Ϣ������������
	std::mutex g_mutex;
	WaitQuitSignal g_wait_sigal;

	g_wait_sigal.init();

	//����쳣�ж��ź���
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

	//�ȴ��߳�����Ч
	ACE_Time_Value tvSleep(0, 1000);
	ACE_OS::sleep(tvSleep);

	//�ڶ��������������������
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

	//������ʽ����PacketParse
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

	//�������õ�ǰ����·��
	if (false == SetAppPath())
	{
		PSS_LOGGER_DEBUG("[main]SetAppPath error.");
	}

	//��ȡ�����ļ�
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
	//ָ����ǰĿ¼����ֹ�����ļ�ʧ��
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

	//���Dump�ļ�
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)exception_handler);
	base_set_crash_handler(main_crash_handler, nullptr);

	//��Ӷ�Console  ����windowsϵͳ�Ļ��Ƶ���ctrlhandler��ʱ�޷�����ִ�н����ͱ�ϵͳ������������ʱ�ر�windows�ϵĻص�
#ifndef WIN32
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)ctrlhandler, true);
#endif

	//��һ������ȡ�����ļ�
	if (!App_XmlConfig::instance()->InitIsOk())
	{
		PSS_LOGGER_DEBUG("[main]!App_XmlConfig->InitIsOk() is false.");
		return 0;
	}

	//��ʼ�����
	Init_Console_Output(GetXmlConfigAttribute(xmlAceDebug)->TrunOn,
		GetXmlConfigAttribute(xmlAceDebug)->FileCount,
		GetXmlConfigAttribute(xmlAceDebug)->LogFileMaxSize,
		GetXmlConfigAttribute(xmlAceDebug)->ConsoleName,
		GetXmlConfigAttribute(xmlAceDebug)->Level);

	PSS_LOGGER_DEBUG("[main]PSS is load conf ok.");

	//��ʽ����PacketParse
	if (0 != Load_PacketParse_Module())
	{
		return 0;
	}

	//��������
	ServerMain();

	//������ʽ����PacketParse
	App_PacketParseLoader::instance()->Close();

	//�����־�������ڿգ������

	return 0;
}

#endif


