//���������˺ܶ�����߳����Ķ�������Ϊ�Ҳ������ҵ��ڴ������������ACE�Ľӿڡ�
//һ��ʼ������һ���������࿽��һ��������ʡ�¡����Ƿ���Ҫ��д��̫���ӣ�Ҫ��̫�򵥡�
//��������Ҫ�Լ�����ѽ�������������ҵ���һЩ�ȽϺõ����ӡ�
//���ͣ���Ҫ���˵�����ԣ�ҪŬ��ȥ������Ϊ�Ҽ��š�

#ifndef _THREADLOCK_H
#define _THREADLOCK_H

#include "define.h"
#if PSS_PLATFORM == PLATFORM_WIN
#include <Windows.h>
#else
#include <pthread.h>
#endif

#if PSS_PLATFORM == PLATFORM_WIN
#define LOCK_MUTEXT CRITICAL_SECTION
#else
#define LOCK_MUTEXT pthread_mutex_t
#endif

#include "ace/OS_main.h"

class CThreadLock
{
public:
    CThreadLock(void)
    {
        Init();
    }

    ~CThreadLock()
    {
        Close();
    }

    void Init()
    {
#if PSS_PLATFORM == PLATFORM_WIN
        InitializeCriticalSection(&m_lock);
#else
        pthread_mutex_init(&m_lock, nullptr);
#endif
    }

    void Close()
    {
#if PSS_PLATFORM == PLATFORM_WIN
        DeleteCriticalSection(&m_lock);
#else
        pthread_mutex_destroy(&m_lock);
#endif
    }

    void Lock()
    {
        m_Time = ACE_OS::gettimeofday();
#if PSS_PLATFORM == PLATFORM_WIN
        EnterCriticalSection(&m_lock);
#else
        pthread_mutex_lock(&m_lock);
#endif
    }

    void UnLock()
    {
        m_Time = ACE_OS::gettimeofday() - m_Time;

#if PSS_PLATFORM == PLATFORM_WIN
        LeaveCriticalSection(&m_lock);
#else
        pthread_mutex_unlock(&m_lock);
#endif
    }

private:
    LOCK_MUTEXT m_lock;
    ACE_Time_Value m_Time;
};

//�Զ���������
class CAutoLock
{
public:
    CAutoLock(CThreadLock* pThreadLock)
    {
        m_pThreadLock = pThreadLock;

        if(nullptr != m_pThreadLock)
        {
            m_pThreadLock->Lock();
        }
    }
    ~CAutoLock()
    {
        if(nullptr != m_pThreadLock)
        {
            m_pThreadLock->UnLock();
        }
    }

private:
    CThreadLock* m_pThreadLock;
};

#endif
