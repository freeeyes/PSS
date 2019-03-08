#ifndef _TRACE_H
#define _TRACE_H

#include "stdio.h"
#include "stdlib.h"
#include "define.h"

#if PSS_PLATFORM == PLATFORM_WIN
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

// ����64λ����
#if defined(_WIN32) && !defined(CYGWIN)
typedef __int64 int64_t;
#else
typedef long long int64t;
#endif  // _WIN32

//����һ��Ψһ��TraceID
struct _TRACE_ID
{
    int64_t m_nTime;     //��ǰʱ�䣬��ȷ��΢��
    int m_nPID;          //��ǰ����ID
    int m_nTID;          //��ǰ�߳�ID
    unsigned int m_nIP;  //��ǰ�û�IP
    short m_sPort;       //��ǰ�û��˿�
    short m_sCommandID;  //��ǰ����ID

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

// ��1601��1��1��0:0:0:000��1970��1��1��0:0:0:000��ʱ��(��λ100ns)
#if defined(_WIN32)
#define EPOCHFILETIME   (116444736000000000UL)
#endif

//��¼ʱ���߹���
//add by freeeyes

// ��ȡϵͳ�ĵ�ǰʱ�䣬��λ΢��(us)
inline int64_t GetSysTimeMicros()
{
#if defined(_WIN32)
    FILETIME ft;
    LARGE_INTEGER li;
    int64_t tt = 0;
    GetSystemTimeAsFileTime(&ft);
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;
    // ��1970��1��1��0:0:0:000�����ڵ�΢����(UTCʱ��)
    tt = (li.QuadPart - EPOCHFILETIME) / 10;
    return tt;
#else
    timeval tv;
    gettimeofday(&tv, 0);
    return (int64_t)tv.tv_sec * 1000000 + (int64_t)tv.tv_usec;
#endif // _WIN32
}

//��֤һ��IP
static bool effective(const char* pIP)//���ip��ַ����Ч��
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

//��һ��IPת��Ϊ����
static unsigned int GetValueByIP(const char* pIP)
{
    if (!effective(pIP))
    {
        return 0;
    }

    unsigned int res = 0;

    unsigned char value[4] = { 0 };//���IP��ַ���ĸ�����
    char word[10] = { 0 };
    int cnt = 0, cntNum = 0;
    char* ip = (char*)pIP;

    while (*ip != '\0')//��ֵ�value
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

    res = (value[0] << 24) | (value[1] << 16) | (value[2] << 8) | value[3];//���ĸ����ֽ���ƴ�ӳ�һ������
    return res;
}

//����һ������������֧���ڴ�Խ����
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

//����һ��TRACE_ID
inline TRACE_ID CreateTraceID(const char* pIP, short sPort, short sCommandID)
{
    TRACE_ID objTraceID;

    //��ɹ��� ʱ�� + PID + TID + IP
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

//����һ��traceID��־
inline void WriteTrace(const char* pFilePath, const char* pTraceID, const char* pFileName, int nCodeLine)
{
    //Ŀǰ�����ʽ����ΪXML
    char szFileLog[400] = { '\0' };
    char szTemp[1024] = { '\0' };

    if (NULL != pTraceID && 0 < strlen(pTraceID))
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

//��TraceID����ת����string
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

//�ֲ�traceID����Ϣ
inline void ShowKey(const char* pTraceID)
{
    char szTime[50]      = { '\0' };
    char szPID[50]       = { '\0' };
    char szTID[50]       = { '\0' };
    char szIP[50]        = { '\0' };
    char szPort[50]      = { '\0' };
    char szCommandID[50] = { '\0' };

    //��traceID����Ϣչ�ֳ���
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

//���ú�
#define CREATE_TRACE(x,y,z) Convert_TraceID_To_char(x,y, z);
#define DO_TRACE(x, y) WriteTrace(x, y, __FILE__, __LINE__);
#define SHOW_TRACE_ID(x) ShowKey(x)

#endif

