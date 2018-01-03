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

#define HTML_REPORT "<html>\n \
<head>\n \
<style type=\"text/css\">\n \
.normal{background-color:#C1FFC1;color:Black;}\n \
.error{background-color:#CD5C5C;color:Black;}\n \
</style>\n \
<title>Test Result</title>\n \
</head>\n \
\n \
<body>\n \
<table border=\"1\" cellpadding=\"10\">\n \
  <tr>\n \
    <th>TestName</th>\n \
    <th>test result</th>\n \
	<th>test info</th>\n \
	<th>time cost(ms)</th>\n \
  </tr>\n \
  %s \
</table>\n \
</body>\n \
</html>"

#define TO_SUCCESS_TR "<tr class=\"normal\">\n<td>%s</td>\n<td>%d</td>\n<td>%s</td>\n<td>%fms</td>\n"
#define TO_ERROR_TR "<tr class=\"error\">\n<td>%s</td>\n<td>%d</td>\n<td>%s</td>\n<td>%fms</td>\n"

#define HTML_NAME "../../testresult/result.html"

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
	char  m_szTestName[MAX_BUFF_300];     //测试集名称
	char  m_szResult[MAX_BUFF_1024];      //测试结果描述 
	int   m_nRet;                         //测试结果 0 成功 1 失败
	float m_fMilliseconds;                //消耗时间      
	
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

//多线程测试参数
struct _ThreadParam
{
	_ClientInfo*       m_pClientInfo;
	_ResultInfo*       m_pResultInfo;
	pthread_barrier_t* m_Barrier;
	
	_ThreadParam()
	{
		m_pClientInfo = NULL;
		m_pResultInfo = NULL;
		m_Barrier     = NULL;
	}
};



#endif
