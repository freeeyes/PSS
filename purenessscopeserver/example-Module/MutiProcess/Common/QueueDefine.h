#ifndef _QUEUEDEFINE_H
#define _QUEUEDEFINE_H

#include "define.h"

#ifdef __LINUX__
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#endif

#define PERMS_IPC 0600

//PSS接收数据后发送到多进程的消息队列ID
#define PSS_QUEUE_ID     11000
//多进程返回给PSS的数据消息队列ID
#define MI_QUEUE_ID      11001

//PSS传输队列数据结构
struct _Pss_QueueData
{
	uint32 m_u4ConnectID;
	uint16 m_u2CommandID;

	_Pss_QueueData()
	{
		m_u4ConnectID = 0;
		m_u2CommandID = 0;
	}
};

//Muit传输队列数据信息
struct _Muti_QueueData
{
	uint32 m_u4ConnectID;
	uint16 m_u2CommandID;

	_Muti_QueueData()
	{
		m_u4ConnectID = 0;
		m_u2CommandID = 0;
	}
};

//获得队列ID
inline int GetLinuxMsgQID(key_t key)
{
#ifdef __LINUX__
	int nMsgQID = -1;

	if(key <= 0)
	{
		return -1;
	}

	if((nMsgQID = msgget(key, PERMS_IPC|IPC_CREAT)) < 0) 
	{
		return -2;
	}

	return nMsgQID;
#else
	//不支持windows
	return 0;
#endif
}

//获得队列信息
inline int GetDataFromMsgQ(int nMsgQID, unsigned char *pData, int nLen)
{
#ifdef __LINUX__
	int nRetLen = 0;
	if(nMsgQID < 0)
	{
		return -1;
	}

	//IPC_NOWAIT为非阻塞，否则为阻塞
	while((nRetLen = msgrcv(nMsgQID, pData, nLen, 0, 0)) < 0)
	{
		if (errno == EIDRM)
		{
			exit(1);
		}

		if (errno != EINTR)
		{
			return -2;
		}
	}

	return nRetLen;
#else
	//不支持windows
	return -1;
#endif
}

//写入消息队列信息
inline int PutDataToMsgQ(int nMsgQID, const unsigned char *pData, int nLen)
{
#ifdef __LINUX__
	int ret;

	if (nMsgQID < 0)
	{
		return -1;
	}

	// 除非是操作被信号打断，否则不会重启操作
	while ((ret = msgsnd(nMsgQID, pData, nLen, IPC_NOWAIT)) < 0)
	{
		if (errno == EIDRM)
		{
			exit(1);
		}

		if (errno != EINTR)
		{
			break;
		}
	}

	return ret;
#else
	//不支持windows
	return -1;
#endif
}


#endif
