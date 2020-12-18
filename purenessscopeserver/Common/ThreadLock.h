//在网上找了很多关于线程锁的东西，因为我不想让我的内存分配器依赖于ACE的接口。
//一开始觉得找一个这样的类拷贝一个过来，省事。但是发现要不写的太复杂，要不太简单。
//看来还是要自己动笔呀，不过好在我找到了一些比较好的例子。
//加油，不要随便说不可以，要努力去做。因为我坚信。

#ifndef _THREADLOCK_H
#define _THREADLOCK_H

#include "define.h"
#include "TimeStamp.hpp"
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
#if PSS_PLATFORM == PLATFORM_WIN
        EnterCriticalSection(&m_lock);
#else
        pthread_mutex_lock(&m_lock);
#endif
    }

    void UnLock()
    {
#if PSS_PLATFORM == PLATFORM_WIN
        LeaveCriticalSection(&m_lock);
#else
        pthread_mutex_unlock(&m_lock);
#endif
    }

private:
    LOCK_MUTEXT m_lock;
};

//自动加锁的类
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
