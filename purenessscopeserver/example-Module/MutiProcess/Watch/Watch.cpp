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

//���ļ�·��
#define LOCKFILE    "%s/Purenessscopeserver/example-Module/MutiProcess/Watch/lockwatch.lk"

//�ӽ�������
#define CHILDCOUNT  1
/*
//д��ռ�ļ���
int AcquireWriteLock(int fd, int start, int len)
{
	struct flock arg;
	arg.l_type = F_WRLCK; // ��д��
	arg.l_whence = SEEK_SET;
	arg.l_start = start;
	arg.l_len = len;
	arg.l_pid = getpid();

	return fcntl(fd, F_SETLKW, &arg);
}
*/
//�ͷŶ�ռ�ļ���
int ReleaseLock(int fd, int start, int len)
{
	struct flock arg;
	arg.l_type = F_UNLCK; //  ����
	arg.l_whence = SEEK_SET;
	arg.l_start = start;
	arg.l_len = len;
	arg.l_pid = getpid();

	return fcntl(fd, F_SETLKW, &arg);
}
/*
//�鿴д��
int SeeLock(int fd, int start, int len)
{
	struct flock arg;
	arg.l_type = F_WRLCK;
	arg.l_whence = SEEK_SET;
	arg.l_start = start;
	arg.l_len = len;
	arg.l_pid = getpid();

	if (fcntl(fd, F_GETLK, &arg) != 0) // ��ȡ��
	{
		return -1; // ����ʧ��
	}

	if (arg.l_type == F_UNLCK)
	{
		return 0; // ����
	}
	else if (arg.l_type == F_RDLCK)
	{
		return 1; // ����
	}
	else if (arg.l_type == F_WRLCK)
	{
		return 2; // д��
	}

	return 0;
}
*/

int Chlid_Run()
{
	//��ʼ�����ն���
	printf("[Chlid_Run]Begin.\n");
	
	//��ʼ��������Ϣ����
	int nPssQueueID = GetLinuxMsgQID((key_t)PSS_QUEUE_ID);
	if(-1 == nPssQueueID)
	{
		return 0;
	}

	//��ʼ��������Ϣ����
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
		//��������
		int nLen = GetDataFromMsgQ(nPssQueueID, (unsigned char* )&objPssQueueData, (int)sizeof(_Pss_QueueData));
		//printf("[Chlid_Run]nPssQueueID nLen=%d, DataLen=%d.\n", nPssQueueID, (int)sizeof(_Pss_QueueData));
		if(nLen == (int)sizeof(_Pss_QueueData))
		{
			//���ն������ݳɹ�,����������߼�
			_Muti_QueueData objMutiQueueData;
			objMutiQueueData.m_u4ConnectID = objPssQueueData.m_u4ConnectID;
			objMutiQueueData.m_u2CommandID = (uint16)0xe200;
			
			//���뷢�Ͷ���
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
	//��ǰ������̸߳���
	int nNumChlid = CHILDCOUNT;
	
	//���ʱ��������
	struct timespec tsRqt;
	
	//�ļ���
	int fd_lock = 0;
	
	int nRet = 0;

	//�����̼��ʱ����������ÿ��5��һ�Σ�
	tsRqt.tv_sec  = 5;
	tsRqt.tv_nsec = 0;
	
	// �򿪣����������ļ�
	char szFileName[200] = {'\0'};
	memset(szFileName, 0, sizeof(flock));
	sprintf(szFileName, LOCKFILE, getenv("HOME"));
	fd_lock = open(szFileName, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
	if (fd_lock < 0)
	{
		printf("open the flock and exit, errno = %d.", errno);
		exit(1);
	}
	
	//�鿴��ǰ�ļ����Ƿ�����
	nRet = SeeLock(fd_lock, 0, sizeof(int));
	if (nRet == -1 || nRet == 2) 
	{
		printf("file is already exist!");
		exit(1);
	}
	
	//����ļ���û��������ס��ǰ�ļ���
	if (AcquireWriteLock(fd_lock, 0, sizeof(int)) != 0)
	{
		printf("lock the file failure and exit, idx = 0!.");
		exit(1);
	}
	
	//д���ӽ�������Ϣ
	lseek(fd_lock, 0, SEEK_SET);
	for (int nIndex = 0; nIndex <= nNumChlid; nIndex++)
	{
		write(fd_lock, &nIndex, sizeof(nIndex));
	}
  
  while (1)
  {
  	for (int nChlidIndex = 1; nChlidIndex <= nNumChlid; nChlidIndex++)
  	{
     		//����ÿ���ӽ��̵����Ƿ񻹴���
     		nRet = SeeLock(fd_lock, nChlidIndex * sizeof(int), sizeof(int));
     		if (nRet == -1 || nRet == 2)
     		{
     			continue;
     		}
     		//����ļ���û�б������������ļ������������ӽ���
     		int npid = fork();
     		if (npid == 0)
     		{
     			//���ļ���
     			if(AcquireWriteLock(fd_lock, nChlidIndex * sizeof(int), sizeof(int)) != 0)
				{
					printf("child %d AcquireWriteLock failure.\n", nChlidIndex);
					exit(1);
				}
	        
				//�����ӽ���
				Chlid_Run();
	        
				//�ӽ�����ִ�������������˳�ѭ�����ͷ��� 
				//ReleaseLock(fd_lock, nChlidIndex * sizeof(int), sizeof(int));	        
     		}
  	}
  	
  	printf("child count(%d) is ok.\n", nNumChlid);
  	//�����
  	nanosleep(&tsRqt, NULL);
  }
	
	return 0;
}
