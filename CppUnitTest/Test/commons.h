#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>  
#include <stdlib.h>  

#include <string.h>  
#include <errno.h>  
#include <assert.h>  
#include <time.h>

#if defined(_WIN32_PLATFORM_)
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib") 


static int gettimeofday(struct timeval *tp, void *tzp)
{
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;
	GetLocalTime(&wtm);
	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;
	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;
	clock = mktime(&tm);
	tp->tv_sec = clock;
	tp->tv_usec = wtm.wMilliseconds * 1000;
	return (0);
}




#endif

#if defined(_LINUX_PLATFORM_)
#include <unistd.h>  
#include <sys/time.h>
#include <linux/errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#define timelong_t struct timeval

#include <fcntl.h>  
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/time.h> 


#endif








#define MAX_BUFF_200  200
#define MAX_BUFF_300  300
#define MAX_BUFF_1024 1024

#define COMMAND_AUTOTEST_HEAD     0x1000
#define COMMAND_AUTOTEST_UDP_HEAD 0x1001

#define HTML_REPORT "<table border=\"1\" cellpadding=\"10\">\n \
  <tr>\n \
    <th>TestName</th>\n \
    <th>test result</th>\n \
	<th>test info</th>\n \
	<th>time cost(ms)</th>\n \
  </tr>\n \
   <tr>\n \
     <td colspan=\"4\">%s</td>\n \
   </tr>\n \
  %s \
</table>\n"

#define TO_SUCCESS_TR "<tr style=\"background-color:#C1FFC1;color:Black;\">\n<td>%s</td>\n<td>%d</td>\n<td>%s</td>\n<td>%fms</td>\n"
#define TO_ERROR_TR "<tr style=\"background-color:#CD5C5C;color:Black;\">\n<td>%s</td>\n<td>%d</td>\n<td>%s</td>\n<td>%fms</td>\n"

#define HTML_NAME "../../testresult/result.md"

//²âÊÔÊý¾Ý¼¯
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

//Êä³ö½á¹û¼¯
struct _ResultInfo
{
	char  m_szTestName[MAX_BUFF_300];     //²âÊÔ¼¯Ãû³Æ
	char  m_szResult[MAX_BUFF_1024];      //²âÊÔ½á¹ûÃèÊö 
	int   m_nRet;                         //²âÊÔ½á¹û 0 ³É¹¦ 1 Ê§°Ü
	float m_fMilliseconds;                //ÏûºÄÊ±¼ä      
	
	_ResultInfo()
	{
		m_szTestName[0] = '\0';
		m_szResult[0]   = '\0';
		m_nRet          = 0;
		m_fMilliseconds = 0.0f;
	}
	
	void Display()
	{
		printf("[_ResultInfo::Display]m_szTestName=%s.\n", m_szTestName);
		printf("[_ResultInfo::Display]m_nRet=%d.\n", m_nRet);
		printf("[_ResultInfo::Display]m_szResult=%s.\n", m_szResult);
		printf("[_ResultInfo::Display]m_fMilliseconds=%fms.\n", m_fMilliseconds);
	}
	
	void To_Html(char* pText)
	{
		if(0 == m_nRet)
		{
			sprintf(pText, TO_SUCCESS_TR, m_szTestName, m_nRet, m_szResult, m_fMilliseconds);
		}
		else
		{
			sprintf(pText, TO_ERROR_TR, m_szTestName, m_nRet, m_szResult, m_fMilliseconds);
		}
	}
};

//¶àÏß³Ì²âÊÔ²ÎÊý
struct _ThreadParam
{
	_ClientInfo*       m_pClientInfo;
	_ResultInfo*       m_pResultInfo;

	_ThreadParam()
	{
		m_pClientInfo = NULL;
		m_pResultInfo = NULL;

	}
};



#endif
