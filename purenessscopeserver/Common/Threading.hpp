#ifndef THREADING_H
#define THREADING_H

#include <ace/Thread.h>
#include <ace/TSS_T.h>
#include <ace/Atomic_Op.h>
#include <assert.h>
#include <ace/OS_NS_unistd.h>
#include <ace/Sched_Params.h>
#include <vector>

class Runnable
{
public:
    virtual ~Runnable() {}
    virtual void run() = 0;

    void incReference()
    {
        ++m_refs;
    }
    void decReference()
    {
        if (!--m_refs)
        {
            delete this;
        }
    }
private:
    ACE_Atomic_Op<ACE_Thread_Mutex, long> m_refs;
};

enum Priority
{
    Idle,
    Lowest,
    Low,
    Normal,
    High,
    Highest,
    Realtime
};

#define MAXPRIORITYNUM (Realtime + 1)

class ThreadPriority
{
public:
    ThreadPriority()
    {
        for (int i = Idle; i < MAXPRIORITYNUM; ++i)
        {
            m_priority[i] = ACE_THR_PRI_OTHER_DEF;
        }

        m_priority[Idle] = ACE_Sched_Params::priority_min(ACE_SCHED_OTHER);
        m_priority[Realtime] = ACE_Sched_Params::priority_max(ACE_SCHED_OTHER);

        std::vector<int> _tmp;

        ACE_Sched_Params::Policy _policy = ACE_SCHED_OTHER;
        ACE_Sched_Priority_Iterator pr_iter(_policy);

        while (pr_iter.more())
        {
            _tmp.push_back(pr_iter.priority());
            pr_iter.next();
        }

        if (_tmp.size() >= MAXPRIORITYNUM)
        {
            const size_t max_pos = _tmp.size();
            size_t min_pos = 1;
            size_t norm_pos = 0;

            for (size_t i = 0; i < max_pos; ++i)
            {
                if (_tmp[i] == ACE_THR_PRI_OTHER_DEF)
                {
                    norm_pos = i + 1;
                    break;
                }
            }

            //since we have only 7(seven) values in enum Priority
            //and 3 we know already (Idle, Normal, Realtime) so
            //we need to split each list [Idle...Normal] and [Normal...Realtime]
            //into ?piesces
            const size_t _divider = 4;
            size_t _div = (norm_pos - min_pos) / _divider;

            if (_div == 0)
            {
                _div = 1;
            }

            min_pos = (norm_pos - 1);

            m_priority[Low] = _tmp[min_pos -= _div];
            m_priority[Lowest] = _tmp[min_pos -= _div];

            _div = (max_pos - norm_pos) / _divider;

            if (_div == 0)
            {
                _div = 1;
            }

            min_pos = norm_pos - 1;

            m_priority[High] = _tmp[min_pos += _div];
            m_priority[Highest] = _tmp[min_pos += _div];
        }
    };

    int getPriority(Priority p) const
    {
        if (p < Idle)
        {
            p = Idle;
        }

        if (p > Realtime)
        {
            p = Realtime;
        }

        return m_priority[p];
    };

private:
    int m_priority[MAXPRIORITYNUM];
};

#define THREADFLAG (THR_NEW_LWP | THR_SCHED_DEFAULT| THR_JOINABLE)
Thread::ThreadStorage Thread::m_ThreadStorage;
ThreadPriority Thread::m_TpEnum;

class Thread
{
public:
    Thread() : m_iThreadId(0), m_hThreadHandle(0), m_task(0)
    {
    }
    ;
    explicit Thread(Runnable* instance) : m_iThreadId(0), m_hThreadHandle(0), m_task(instance)
    {
        if (m_task)
        {
            m_task->incReference();
        }

        start();
    };

    ~Thread()
    {
        if (m_task)
        {
            m_task->decReference();
        }
    };

    bool start()
    {
        if (m_task == 0 || m_iThreadId != 0)
        {
            return false;
        }

        bool res = (ACE_Thread::spawn(&Thread::ThreadTask, (void*)m_task, THREADFLAG, &m_iThreadId, &m_hThreadHandle) == 0);

        if (res)
        {
            m_task->incReference();
        }

        return res;
    };

    bool wait()
    {
        if (!m_hThreadHandle || !m_task)
        {
            return false;
        }

        ACE_THR_FUNC_RETURN _value = ACE_THR_FUNC_RETURN(-1);
        int _res = ACE_Thread::join(m_hThreadHandle, &_value);

        m_iThreadId = 0;
        m_hThreadHandle = 0;

        return (_res == 0);
    };

    void destroy()
    {
        if (!m_iThreadId || !m_task)
        {
            return;
        }

        if (ACE_Thread::kill(m_iThreadId, SIGABRT) != 0)
        {
            return;
        }

        m_iThreadId = 0;
        m_hThreadHandle = 0;

        // reference set at ACE_Thread::spawn
        m_task->decReference();
    };

    void suspend()
    {
        ACE_Thread::suspend(m_hThreadHandle);
    };

    void resume()
    {
        ACE_Thread::resume(m_hThreadHandle);
    };

    void setPriority(Priority type)
    {
        int _priority = m_TpEnum.getPriority(type);
        ACE_Thread::setprio(m_hThreadHandle, _priority);
    };

    static void Sleep(unsigned long msecs)
    {
        ACE_OS::sleep(ACE_Time_Value(0, 1000 * msecs));
    };

    static ACE_thread_t currentId()
    {
        return ACE_Thread::self();
    };

    static ACE_hthread_t currentHandle()
    {
        ACE_hthread_t _handle;
        ACE_Thread::self(_handle);

        return _handle;
    };

    static Thread* current()
    {
        Thread* _thread = m_ThreadStorage.ts_object();

        if (!_thread)
        {
            _thread = new Thread();
            _thread->m_iThreadId = Thread::currentId();
            _thread->m_hThreadHandle = Thread::currentHandle();

            Thread* _oldValue = m_ThreadStorage.ts_object(_thread);

            if (_oldValue)
            {
                delete _oldValue;
            }
        }

        return _thread;
    };

private:
    Thread(const Thread&);

    static ACE_THR_FUNC_RETURN ThreadTask(void* param)
    {
        Runnable* _task = (Runnable*)param;
        _task->run();

        // task execution complete, free referecne added at
        _task->decReference();

        return (ACE_THR_FUNC_RETURN)0;
    };

    ACE_thread_t m_iThreadId;
    ACE_hthread_t m_hThreadHandle;
    Runnable* m_task;

    typedef ACE_TSS<Thread> ThreadStorage;
    //global object - container for Thread class representation of every thread
    static ThreadStorage m_ThreadStorage;
    //use this object to determine current OS thread priority values mapped to enum Priority{}
    static ThreadPriority m_TpEnum;
};

#endif
