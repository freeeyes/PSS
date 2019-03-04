#include <sys/types.h>
#include <stdarg.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "../Common/QueueDefine.h"

//锁文件路径
#define LOCKFILE    "%s/Purenessscopeserver/example-Module/MutiProcess/Watch/lockwatch.lk"

//子进程数量
#define CHILDCOUNT  1

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

int Chlid_Run()
{
	//初始化接收队列
	printf("[Chlid_Run]Begin.\n");
	
	//初始化发送消息队列
	int nPssQueueID = GetLinuxMsgQID((key_t)PSS_QUEUE_ID);
	if(-1 == nPssQueueID)
	{
		return 0;
	}

	//初始化接收消息队列
	int nMutiQueueID = GetLinuxMsgQID((key_t)MI_QUEUE_ID);
	if(-1 == nMutiQueueID)
	{
		return 0;
	}
	
	printf("[Chlid_Run]Init Success.\n");
	printf("[Chlid_Run]nPssQueueID=%d,nMutiQueueID=%d.\n", nPssQueueID, nMutiQueueID);
	
	_Pss_QueueData objPssQueueData;
	while(true)
	{
		//接收数据
		int nLen = GetDataFromMsgQ(nPssQueueID, (unsigned char* )&objPssQueueData, (int)sizeof(_Pss_QueueData));
		//printf("[Chlid_Run]nPssQueueID nLen=%d, DataLen=%d.\n", nPssQueueID, (int)sizeof(_Pss_QueueData));
		if(nLen == (int)sizeof(_Pss_QueueData))
		{
			//接收队列数据成功,在这里添加逻辑
			_Muti_QueueData objMutiQueueData;
			objMutiQueueData.m_u4ConnectID = objPssQueueData.m_u4ConnectID;
			objMutiQueueData.m_u2CommandID = (uint16)0xe200;
			
			//放入发送队列
			PutDataToMsgQ(nMutiQueueID, (unsigned char* )&objMutiQueueData, (int)sizeof(_Muti_QueueData));
		}
		else
		{
			printf("[Chlid_Run]GetDataFromMsgQ is error.\n");
			break;
		}
	}
	
	return 0;
}

int main(int argc, char *argv[])
{
	//当前监控子线程个数
	int nNumChlid = CHILDCOUNT;
	
	//检测时间间隔参数
	struct timespec tsRqt;
	
	//文件锁
	int fd_lock = 0;
	
	int nRet = 0;

	//主进程检测时间间隔（设置每隔5秒一次）
	tsRqt.tv_sec  = 5;
	tsRqt.tv_nsec = 0;
	
	// 打开（创建）锁文件
	char szFileName[200] = {'\0'};
	memset(szFileName, 0, sizeof(flock));
	sprintf(szFileName, LOCKFILE, getenv("HOME"));
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
				//ReleaseLock(fd_lock, nChlidIndex * sizeof(int), sizeof(int));	        
     		}
  	}
  	
  	printf("child count(%d) is ok.\n", nNumChlid);
  	//检查间隔
  	nanosleep(&tsRqt, NULL);
  }
	
	return 0;
}
