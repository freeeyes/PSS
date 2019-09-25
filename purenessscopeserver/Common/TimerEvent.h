#pragma once

#include "define.h"
#include "ITSTimer.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <time.h>
#include "date.h"

enum em_timer_events_state
{
    em_execute_timer = 0,     //执行定时器事件
    em_insert_timer,          //添加定时器
    em_delete_timer           //删除定时器
};


inline std::string get_curr_time()
{
    string strDate;

    auto  tt_now = system_clock::now();

    //得到毫秒值
    system_clock::duration tp = tt_now.time_since_epoch();
    auto dp = date::floor<date::days>(tp);

    auto time = date::make_time(duration_cast<milliseconds>(tp - dp));

    auto tt = std::chrono::system_clock::to_time_t
              (tt_now);

    struct tm tm_now;
    char date[60] = { 0 };

#if PSS_PLATFORM == PLATFORM_WIN
    localtime_s(&tm_now, &tt);
    sprintf_s(date, 60, "%04d-%02d-%02d-%02d.%02d.%02d:%d",
              (int)tm_now.tm_year + 1900, (int)tm_now.tm_mon + 1, (int)tm_now.tm_mday,
              (int)time.hours().count(), (int)time.minutes().count(), (int)time.seconds().count(), (int)time.subseconds().count());

#else
    localtime_r(&tt, &tm_now);
    sprintf(date, "%04d-%02d-%02d-%02d.%02d.%02d:%d",
            (int)tm_now.tm_year + 1900, (int)tm_now.tm_mon + 1, (int)tm_now.tm_mday,
            (int)time.hours().count(), (int)time.minutes().count(), (int)time.seconds().count(), (int)time.subseconds().count());

#endif


    strDate = date;
    return strDate;
}

//timer执行事件
class CTimerEvents
{
public:
    CTimerEvents() : em_timer_events_state_(em_execute_timer), timer_id_(0), timer_interval_(0), function_arg_(nullptr)
    {

    }

    em_timer_events_state em_timer_events_state_;
    int timer_id_;
    milliseconds timer_interval_;
    TimerFunctor timer_function_;
    void*        function_arg_;
};

class CTimerThreadInfo
{
public:
    CTimerThreadInfo() : is_run_(false) {};

    ~CTimerThreadInfo()
    {
        std::cout << "[CTimerThreadInfo::~CTimerThreadInfo]" << endl;
    }

    void get_timer_events_list(vector<CTimerEvents>& timer_events_temp_list_)
    {
        std::lock_guard <std::mutex> lock(thread_mutex_);

        if (timer_events_list_.size() > 0)
        {
            timer_events_temp_list_.swap(timer_events_list_);
        }
    }

    CTimerNodeList timer_node_list_;
    bool is_run_;
    std::thread thread_;
    std::mutex thread_mutex_;
    mutable std::mutex thread_wait_mutex_;
    std::condition_variable condition_;
    vector<CTimerEvents> timer_events_list_;
};

class CTimerManager : public ITSTimerManager
{
public:
    CTimerManager();
    virtual ~CTimerManager();

    virtual bool Add_Timer(int timer_id, milliseconds timer_interval, TimerFunctor&& f, void* arg);

    virtual bool Del_Timer(int timer_id);

    virtual bool Pause();

    virtual bool Restore();

    void Close();

private:
    void run();

private:
    CTimerThreadInfo timer_thread_info_;
};

