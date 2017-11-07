#ifndef _TRACE_H
#define _TRACE_H

#include "stdio.h"
#include "stdlib.h"

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#else
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/syscall.h>
#define gettid() syscall(SYS_gettid)
#endif  // _WIND32
#include <string>

using namespace std;

// 定义64位整形
#if defined(_WIN32) && !defined(CYGWIN)
typedef __int64 int64_t;
#else
typedef long long int64t;
#endif  // _WIN32

//定义一个唯一的TraceID
struct _TRACE_ID
{
    int64_t m_nTime;     //当前时间，精确到微妙
    int m_nPID;          //当前进程ID
    int m_nTID;          //当前线程ID
    unsigned int m_nIP;  //当前用户IP
    short m_sPort;       //当前用户端口
    short m_sCommandID;  //当前命令ID

    _TRACE_ID()
    {
        m_nTime      = 0;
        m_nPID       = 0;
        m_nTID       = 0;
        m_nIP        = 0;
        m_sPort      = 0;
        m_sCommandID = 0;
    }
};

typedef _TRACE_ID TRACE_ID;

// 从1601年1月1日0:0:0:000到1970年1月1日0:0:0:000的时间(单位100ns)
#if defined(_WIN32)
#define EPOCHFILETIME   (116444736000000000UL)
#endif

//记录时间线功能
//add by freeeyes

// 获取系统的当前时间，单位微秒(us)
inline int64_t GetSysTimeMicros()
{
#if defined(_WIN32)
    FILETIME ft;
    LARGE_INTEGER li;
    int64_t tt = 0;
    GetSystemTimeAsFileTime(&ft);
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;
    // 从1970年1月1日0:0:0:000到现在的微秒数(UTC时间)
    tt = (li.QuadPart - EPOCHFILETIME) / 10;
    return tt;
#else
    timeval tv;
    gettimeofday(&tv, 0);
    return (int64_t)tv.tv_sec * 1000000 + (int64_t)tv.tv_usec;
#endif // _WIN32
}

//验证一个IP
static bool effective(const char* pIP)//检查ip地址的有效性
{
    int cnt = 0;
    char* ip = (char*)pIP;

    while (*ip != '\0')
    {
        if (*ip == '.')
        {
            cnt++;
        }

        if ((*ip >= '0' && *ip <= '9') || *ip == '.')
        {
            ip++;
        }
        else
        {
            return false;
        }
    }

    if (cnt != 3)
    {
        return false;
    }

    return true;
}

//将一个IP转换为整数
static unsigned int GetValueByIP(const char* pIP)
{
    if (!effective(pIP))
    {
        return 0;
    }

    unsigned int res = 0;

    unsigned char value[4] = { 0 };//存放IP地址的四个数字
    char word[10] = { 0 };
    int cnt = 0, cntNum = 0;
    char* ip = (char*)pIP;

    while (*ip != '\0')//拆分到value
    {
        while (*ip != '\0' && *ip != '.')
        {
            word[cnt++] = *ip;
            ip++;
        }

        word[cnt] = '\0';
        cnt = 0;

        if (atoi(word) > 255 || atoi(word) < 0)
        {
            return 0;
        }

        value[cntNum++] = atoi(word);

        if (cntNum == 4)
        {
            break;
        }

        ip++;
    }

    res = (value[0] << 24) | (value[1] << 16) | (value[2] << 8) | value[3];//将四个数字进行拼接成一个整数
    return res;
}

//定义一个函数，可以支持内存越界检查
inline void sprintf_trace_safe(char* szText, int nLen, const char* fmt ...)
{
    if (szText == NULL)
    {
        return;
    }

    va_list ap;
    va_start(ap, fmt);

    vsnprintf(szText, nLen, fmt, ap);
    szText[nLen - 1] = '\0';

    va_end(ap);
};

//创建一个TRACE_ID
inline TRACE_ID CreateTraceID(const char* pIP, short sPort, short sCommandID)
{
    TRACE_ID objTraceID;

    //组成规则 时间 + PID + TID + IP
    objTraceID.m_nTime = GetSysTimeMicros();
    printf("[CreateTraceID]%lld.\n", (long long int)objTraceID.m_nTime);

#if defined(_WIN32)
    objTraceID.m_nPID = (int)_getpid();
    objTraceID.m_nTID = GetCurrentThreadId();
#else
    objTraceID.m_nPID = (int)getpid();
    objTraceID.m_nTID = (int)gettid();
#endif

    objTraceID.m_nIP         = GetValueByIP(pIP);
    objTraceID.m_sPort       = sPort;
    objTraceID.m_sCommandID = sCommandID;
    return objTraceID ;
}

//创建一条traceID日志
inline void WriteTrace(const char* pFilePath, const char* pTraceID, const char* pFileName, int nCodeLine)
{
    //目前输出格式定义为XML
    char szFileLog[400] = { '\0' };
    char szTemp[1024] = { '\0' };

    if (NULL != pTraceID)
    {
        sprintf_trace_safe(szFileLog, 200, "%s/%s.xml", pFilePath, pTraceID);
#if defined(_WIN32)
        FILE* pFile = NULL;
        fopen_s(&pFile, szFileLog, "a+");
#else
        FILE* pFile = fopen(szFileLog, "w");
#endif

        if (NULL != pFile)
        {
            sprintf_trace_safe(szTemp, 1024, "<Info Time=\"%lld\" FileName=\"%s\" CodeLine=\"%d\"/>\n",
                               GetSysTimeMicros(),
                               pFileName,
                               nCodeLine);
            fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

            fclose(pFile);
        }
    }
}

//将TraceID对象转换成string
inline string Convert_TraceID_To_char(const char* pIP, short sPort, short sCommandID)
{
    TRACE_ID objTraceID = CreateTraceID(pIP, sPort, sCommandID);

    char pTraceID[100] = { '\0' };
    sprintf_trace_safe(pTraceID, 100, "%016llx%08x%08x%08x%04x%04x",
                       objTraceID.m_nTime,
                       objTraceID.m_nPID,
                       objTraceID.m_nTID,
                       objTraceID.m_nIP,
                       objTraceID.m_sPort,
                       objTraceID.m_sCommandID);
    return (string)pTraceID;
}

//分拆traceID的信息
inline void ShowKey(const char* pTraceID)
{
    char szTime[50]      = { '\0' };
    char szPID[50]       = { '\0' };
    char szTID[50]       = { '\0' };
    char szIP[50]        = { '\0' };
    char szPort[50]      = { '\0' };
    char szCommandID[50] = { '\0' };

    //将traceID的信息展现出来
    if (NULL == pTraceID || strlen(pTraceID) != 48)
    {
        return;
    }

    memcpy(szTime, (char*)pTraceID, 16);
    int64_t n8Time = strtoull(szTime, NULL, 16);
    printf("[ShowKey]Time=%lld.\n", (long long int)n8Time);

    memcpy(szPID, (char*)&pTraceID[16], 8);
    int nPID = (int)strtoul(szPID, NULL, 16);
    printf("[ShowKey]PTD=%d.\n", nPID);

    memcpy(szTID, (char*)&pTraceID[24], 8);
    int nTID = (int)strtoul(szTID, NULL, 16);
    printf("[ShowKey]PTD=%d.\n", nTID);

    memcpy(szIP, (char*)&pTraceID[32], 8);
    int nIP = (int)strtoul(szIP, NULL, 16);
    printf("[ShowKey]PTD=%d.\n", nIP);

    memcpy(szPort, (char*)&pTraceID[40], 4);
    int nPort = (int)strtoul(szPort, NULL, 16);
    printf("[ShowKey]PTD=%d.\n", nPort);

    memcpy(szCommandID, (char*)&pTraceID[44], 4);
    int nCommandID = (int)strtoul(szCommandID, NULL, 16);
    printf("[ShowKey]PTD=0x%04x.\n", nCommandID);
}

//设置宏
#define CREATE_TRACE(x,y,z) Convert_TraceID_To_char(x,y, z);
#define DO_TRACE(x, y) WriteTrace(x, y, __FILE__, __LINE__);
#define SHOW_TRACE_ID(x) ShowKey(x)

#endif

