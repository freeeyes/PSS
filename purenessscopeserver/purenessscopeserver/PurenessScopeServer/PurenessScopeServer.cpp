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

//监控信号量线程
void *thread_Monitor(void *arg)
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
	char szPath[MAX_BUFF_300] = {'\0'};
	char* pFilePath = NULL;

	int nSize = pathconf(".",_PC_PATH_MAX);
	if((pFilePath = (char *)new char[nSize]) != NULL)
	{
		memset(pFilePath, 0, nSize);
		sprintf(pFilePath,"/proc/%d/exe",getpid());

		//从符号链接中获得当前文件全路径和文件名
		readlink(pFilePath, szPath, MAX_BUFF_300 - 1);
		delete[] pFilePath;
		pFilePath = NULL;
		//从szPath里面拆出当前路径
		int nLen = strlen(szPath);
		while(szPath[nLen - 1]!='/') 
		{
			nLen--;
		}

		szPath[nLen > 0 ? (nLen-1) : 0]= '\0';

		chdir(szPath);
		OUR_DEBUG((LM_INFO, "[SetAppPath]Set work Path (%s) OK.\n", szPath));

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

//写独占文件锁
int AcquireWriteLock(int fd, int start, int len)
{
	struct flock arg;
	arg.l_type = F_WRLCK; // 加写锁
	arg.l_whence = SEEK_SET;
	arg.l_start = start;
	arg.l_len = len;
	arg.l_pid = getpid();

	return fcntl(fd, F_SETLKW, &arg);
}

//释放独占文件锁
int ReleaseLock(int fd, int start, int len)
{
	struct flock arg;
	arg.l_type = F_UNLCK; //  解锁
	arg.l_whence = SEEK_SET;
	arg.l_start = start;
	arg.l_len = len;
	arg.l_pid = getpid();

	return fcntl(fd, F_SETLKW, &arg);
}

//查看写锁
int SeeLock(int fd, int start, int len)
{
	struct flock arg;
	arg.l_type = F_WRLCK;
	arg.l_whence = SEEK_SET;
	arg.l_start = start;
	arg.l_len = len;
	arg.l_pid = getpid();

	if (fcntl(fd, F_GETLK, &arg) != 0) // 获取锁
	{
		return -1; // 测试失败
	}

	if (arg.l_type == F_UNLCK)
	{
		return 0; // 无锁
	}
	else if (arg.l_type == F_RDLCK)
	{
		return 1; // 读锁
	}
	else if (arg.l_type == F_WRLCK)
	{
		return 2; // 写所
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
	bool blState = App_PacketParseLoader::instance()->LoadPacketInfo(App_MainConfig::instance()->GetPacketParseInfo()->m_szPacketParseName);
	if(true == blState)
	{
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
		//	return 0;
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

		ACE_Time_Value tvSleep(2, 0);
		ACE_OS::sleep(tvSleep);

		OUR_DEBUG((LM_INFO, "[main]Server Exit.\n"));
		
		//回收隐式加载PacketParse
		App_PacketParseLoader::instance()->Close();			

		pthread_exit(NULL);
	}	
	
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
	SetAppPath();

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
	
	//当前监控子线程个数
	int nNumChlid = 1;	

	//检测时间间隔参数
	struct timespec tsRqt;
	
	//文件锁
	int fd_lock = 0;
	
	int nRet = 0;

	//主进程检测时间间隔（设置每隔5秒一次）
	tsRqt.tv_sec  = 5;
	tsRqt.tv_nsec = 0;
	
	//获得当前路径
	char szWorkDir[255] = {0};
  if(!getcwd(szWorkDir, 260))  
  {  
		exit(1);
  }
  
	// 打开（创建）锁文件
	char szFileName[200] = {'\0'};
	memset(szFileName, 0, sizeof(flock));
	sprintf(szFileName, "%s/lockwatch.lk", szWorkDir);
	fd_lock = open(szFileName, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
	if (fd_lock < 0)
	{
		printf("open the flock and exit, errno = %d.", errno);
		exit(1);
	}
	
	//查看当前文件锁是否已锁
	nRet = SeeLock(fd_lock, 0, sizeof(int));
	if (nRet == -1 || nRet == 2) 
	{
		printf("file is already exist!");
		exit(1);
	}
	
	//如果文件锁没锁，则锁住当前文件锁
	if (AcquireWriteLock(fd_lock, 0, sizeof(int)) != 0)
	{
		printf("lock the file failure and exit, idx = 0!.");
		exit(1);
	}
	
	//写入子进程锁信息
	lseek(fd_lock, 0, SEEK_SET);
	for (int nIndex = 0; nIndex <= nNumChlid; nIndex++)
	{
		write(fd_lock, &nIndex, sizeof(nIndex));
	}
	
	if(App_MainConfig::instance()->GetServerType() == 1)
	{
		//添加监控守护进程
		while (1)
		{
			for (int nChlidIndex = 1; nChlidIndex <= nNumChlid; nChlidIndex++)
			{
				//测试每个子进程的锁是否还存在
				nRet = SeeLock(fd_lock, nChlidIndex * sizeof(int), sizeof(int));
				if (nRet == -1 || nRet == 2)
				{
					continue;
				}
				//如果文件锁没有被锁，则设置文件锁，并启动子进程
				int npid = fork();
				if (npid == 0)
				{
					//上文件锁
					if(AcquireWriteLock(fd_lock, nChlidIndex * sizeof(int), sizeof(int)) != 0)
					{
						printf("child %d AcquireWriteLock failure.\n", nChlidIndex);
						exit(1);
					}
					
					//启动子进程
					Chlid_Run();
					
					//子进程在执行完任务后必须退出循环和释放锁 
					ReleaseLock(fd_lock, nChlidIndex * sizeof(int), sizeof(int));	        
					}
				}
			
			//printf("child count(%d) is ok.\n", nNumChlid);
			//检查间隔
			nanosleep(&tsRqt, NULL);
		}
	}
	else
	{
		//直接运行
		Chlid_Run();
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
	bool blState = App_PacketParseLoader::instance()->LoadPacketInfo(App_MainConfig::instance()->GetPacketParseInfo()->m_szPacketParseName);
	if(true == blState)
	{
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


