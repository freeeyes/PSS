// ConnectHandle.h
// 觉得依赖反应器的定时器有很大的限制性，比如Reactor和Proactor下使用定时器的时候会非常麻烦。
// 既然让系统支持两种模式，索性改的好一些，将定时器管理器抽象出来。负责所有服务器中用到的定时器操作。
// add by freeeyes
// 2009-08-25

#ifndef _TIMEMANAGER_H
#define _TIMEMANAGER_H

#include "define.h"

#include <functional>
#include <queue>
#include <memory>
#include <vector>
#include <chrono>
#include <mutex>
#include <iostream>
#include <condition_variable>
#include <thread>

//定时器组件
namespace brynet {

    enum class EM_TIMER_STATE
    {
        TIMER_STATE_ADD_TIMER = 0,
        TIMER_STATE_EXECUTE_TIMER,
    };

    class TimerMgr;

    class Timer final
    {
    public:
        using Ptr = std::shared_ptr<Timer>;
        using WeakPtr = std::weak_ptr<Timer>;
        using Callback = std::function<void(void)>;

        Timer(std::chrono::steady_clock::time_point startTime,
            std::chrono::nanoseconds lastTime,
            Callback&& callback)
            :
            mCallback(std::move(callback)),
            mStartTime(startTime),
            mLastTime(lastTime)
        {
        }

        const std::chrono::steady_clock::time_point& getStartTime() const
        {
            return mStartTime;
        }

        const std::chrono::nanoseconds& getLastTime() const
        {
            return mLastTime;
        }

        std::chrono::nanoseconds    getLeftTime() const
        {
            const auto now = std::chrono::steady_clock::now();
            return getLastTime() - (now - getStartTime());
        }

        void    cancel()
        {
            std::call_once(mExecuteOnceFlag, [this]() {
                mCallback = nullptr;
                });
        }

    private:
        void            operator() ()
        {
            Callback callback;
            std::call_once(mExecuteOnceFlag, [&callback, this]() {
                callback = std::move(mCallback);
                mCallback = nullptr;
                });

            if (callback != nullptr)
            {
                callback();
            }
        }

    private:
        std::once_flag                                  mExecuteOnceFlag;
        Callback                                        mCallback;
        const std::chrono::steady_clock::time_point     mStartTime;
        const std::chrono::nanoseconds                  mLastTime;

        friend class TimerMgr;
    };

    class TimerMgr final
    {
    public:
        using Ptr = std::shared_ptr<TimerMgr>;

        template<typename F, typename ...TArgs>
        Timer::WeakPtr  addTimer(
            std::chrono::nanoseconds timeout,
            F&& callback,
            TArgs&& ...args)
        {
            auto timer = std::make_shared<Timer>(
                std::chrono::steady_clock::now(),
                std::chrono::nanoseconds(timeout),
                std::bind(std::forward<F>(callback), std::forward<TArgs>(args)...));
            mTimers.push(timer);

            //唤醒线程
            timer_wakeup_state = EM_TIMER_STATE::TIMER_STATE_ADD_TIMER;
            cv.notify_one();
            return timer;
        }

        void addTimer(const Timer::Ptr& timer)
        {
            mTimers.push(timer);
            //唤醒线程
            timer_wakeup_state = EM_TIMER_STATE::TIMER_STATE_ADD_TIMER;
            cv.notify_one();
        }

        void Close()
        {
            timer_run_ = false;
            //唤醒线程
            timer_wakeup_state = EM_TIMER_STATE::TIMER_STATE_ADD_TIMER;
            cv.notify_one();
        }

        void schedule()
        {
            std::unique_lock <std::mutex> lck(mtx);
            timer_run_ = true;

            while (timer_run_)
            {
                if (mTimers.empty())
                {
                    //当前没有定时器，等待唤醒
                    cv.wait(lck);
                }

                if (!timer_run_)
                {
                    break;
                }

                auto tmp = mTimers.top();
                auto timer_wait = tmp->getLeftTime();
                if (timer_wait > std::chrono::nanoseconds::zero())
                {
                    //还需要等待下一个到期时间
                    cv.wait_for(lck, timer_wait);

                    if (timer_wakeup_state == EM_TIMER_STATE::TIMER_STATE_ADD_TIMER)
                    {
                        continue;
                    }
                }

                if (!timer_run_)
                {
                    break;
                }

                mTimers.pop();
                (*tmp)();
                timer_wakeup_state = EM_TIMER_STATE::TIMER_STATE_EXECUTE_TIMER;
            }

            std::cout << "Time manager is end" << std::endl;
        }

        bool isEmpty() const
        {
            return mTimers.empty();
        }

        // if timer empty, return zero
        std::chrono::nanoseconds nearLeftTime() const
        {
            if (mTimers.empty())
            {
                return std::chrono::nanoseconds::zero();
            }

            auto result = mTimers.top()->getLeftTime();
            if (result < std::chrono::nanoseconds::zero())
            {
                return std::chrono::nanoseconds::zero();
            }

            return result;
        }

        void    clear()
        {
            while (!mTimers.empty())
            {
                mTimers.pop();
            }
        }

    private:
        class CompareTimer
        {
        public:
            bool operator() (const Timer::Ptr& left,
                const Timer::Ptr& right) const
            {
                const auto startDiff = left->getStartTime() - right->getStartTime();
                const auto lastDiff = left->getLastTime() - right->getLastTime();
                const auto diff = startDiff.count() + lastDiff.count();
                return diff > 0;
            }
        };

        std::priority_queue<Timer::Ptr, std::vector<Timer::Ptr>, CompareTimer>  mTimers;
        std::mutex mtx;
        std::condition_variable cv;
        bool timer_run_ = false;
        EM_TIMER_STATE timer_wakeup_state = EM_TIMER_STATE::TIMER_STATE_EXECUTE_TIMER;
    };

}

class PSS_Timer_Manager
{
public:
    void Start();

    void Close();

    brynet::TimerMgr::Ptr GetTimerPtr();

private:
    brynet::TimerMgr::Ptr m_timerMgr;
    std::thread           m_ttTimerThread;
};

using App_TimerManager = ACE_Singleton<PSS_Timer_Manager, ACE_Null_Mutex>;
#endif
