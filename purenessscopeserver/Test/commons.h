#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <string.h>  
#include <errno.h>  
#include <assert.h>  
#include <fcntl.h>  
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/time.h> 

#define MAX_BUFF_200  200
#define MAX_BUFF_300  300
#define MAX_BUFF_1024 1024

#define COMMAND_AUTOTEST_HEAD 0x1000

//测试数据集
struct _ClientInfo
{
	char  m_szServerIP[30];
	int   m_nPort; 
	char* m_pSendBuffer;
	int   m_nSendLength;
	int   m_nRecvLength;

	_ClientInfo()
	{
		m_szServerIP[0] = '\0';
		m_nPort         = 0;
		m_pSendBuffer   = NULL;
		m_nSendLength   = 0;
		m_nRecvLength   = 0;
	}
};

//输出结果集
struct _ResultInfo
{
	char m_szTestName[MAX_BUFF_300];     //测试集名称
	char m_szResult[MAX_BUFF_1024];      //测试结果描述 
	int  m_nRet;                         //测试结果 0 成功 1 失败  
	
	_ResultInfo()
	{
		m_szTestName[0] = '\0';
		m_szResult[0]   = '\0';
		m_nRet          = 0;
	}
};
#endif
