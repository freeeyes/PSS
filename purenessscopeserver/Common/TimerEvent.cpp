#include "TimerEvent.h"

void timer_thread_run(CTimerThreadInfo* timer_thread_info)
{
    int ret = -1;
    system_clock::time_point execute_start;
    system_clock::time_point execute_end;
    microseconds next_run_timer_execute_interval;
    CRunNodeInfo run_node_info;

    if (nullptr == timer_thread_info)
    {
        return;
    }

    timer_thread_info->is_run_ = true;

    execute_start = system_clock::now();

    //执行定时器信息
    while (timer_thread_info->is_run_)
    {
        microseconds timer_interval(0);
        std::unique_lock <std::mutex> lock(timer_thread_info->thread_wait_mutex_);

        if (-1 == ret)
        {
            //等待下一次唤醒
            timer_thread_info->condition_.wait(lock);
            execute_start = system_clock::now();
            //std::cout << "[timer_thread_run]wait wake up" << " : " << timer_thread_info->thread_mutex_.try_lock() << endl;
        }
        else
        {
            //指定当前定时器
            bool timer_run = true;

            while (timer_run)
            {
                //执行定时器
                //std::cout << "[" << get_curr_time().c_str() << "]execute timer_id(" << timer_id << ")" << endl;
                run_node_info.timer_function_(run_node_info.function_arg_);

                //计算下次执行时间
                ret = timer_thread_info->timer_node_list_.get_next_run_timer_interval(run_node_info, timer_interval);

                //如果在指定误差下，可以同时执行
                if (timer_interval.count() > 2000)
                {
                    break;
                }
            }

            //下一次执行时间唤醒时间计算
            execute_end = system_clock::now();
            microseconds execute_microsec = duration_cast<microseconds>(execute_end - execute_start);
            //std::cout << "[timer_thread_run]execute_microsec(" << execute_microsec.count() << ")" << endl;

            next_run_timer_execute_interval = timer_interval - execute_microsec;

            while(next_run_timer_execute_interval.count() < 0)
            {
                timer_thread_info->timer_node_list_.get_next_run_timer_interval(run_node_info, timer_interval);
                next_run_timer_execute_interval += timer_interval;
            }

            timer_thread_info->condition_.wait_for(lock, next_run_timer_execute_interval);
            execute_start = system_clock::now();
            //std::cout << "[timer_thread_run]wait_for wake up" << endl;
        }

        //唤醒后判断应该做什么（增删改定时器）
        vector<CTimerEvents> timer_events_temp_list_;

        timer_thread_info->get_timer_events_list(timer_events_temp_list_);

        for (auto f : timer_events_temp_list_)
        {
            if (em_insert_timer == f.em_timer_events_state_)
            {
                std::cout << "[timer_thread_run]add_timer_id = " << f.timer_id_ << endl;
                timer_thread_info->timer_node_list_.add_timer_node_info(f.timer_id_,
                        f.timer_interval_,
                        f.timer_function_,
                        f.function_arg_);
            }
            else
            {
                timer_thread_info->timer_node_list_.del_timer_node_info(f.timer_id_);
            }
        }

        if (timer_events_temp_list_.size() > 0)
        {
            ret = timer_thread_info->timer_node_list_.get_next_run_timer_interval(run_node_info, timer_interval);
        }
    }

    std::cout << "[timer_thread_run]is end." << endl;
    timer_thread_info->is_run_ = false;
}

CTimerManager::CTimerManager()
{
}


CTimerManager::~CTimerManager()
{
}

bool CTimerManager::Add_Timer(int timer_id, milliseconds timer_interval, TimerFunctor&& f, void* arg)
{
    //如果线程没有启动，则启动定时器线程
    run();

    std::lock_guard <std::mutex> lock(timer_thread_info_.thread_mutex_);

    CTimerEvents timer_event;
    timer_event.em_timer_events_state_ = em_insert_timer;
    timer_event.timer_id_              = timer_id;
    timer_event.timer_interval_        = timer_interval;
    timer_event.timer_function_        = std::move(f);
    timer_event.function_arg_          = arg;
    timer_thread_info_.timer_events_list_.push_back(timer_event);

    std::cout << "[add_timer]add_timer_id = " << timer_id << endl;
    timer_thread_info_.condition_.notify_one();

    return true;
}

bool CTimerManager::Del_Timer(int timer_id)
{
    std::lock_guard <std::mutex> lock(timer_thread_info_.thread_mutex_);

    CTimerEvents timer_event;
    timer_event.em_timer_events_state_ = em_delete_timer;
    timer_event.timer_id_ = timer_id;
    timer_event.timer_interval_ = milliseconds(0);
    timer_thread_info_.timer_events_list_.push_back(timer_event);

    timer_thread_info_.condition_.notify_one();
    return true;
}

bool CTimerManager::Pause()
{
    return true;
}

bool CTimerManager::Restore()
{
    return true;
}

void CTimerManager::Close()
{
    timer_thread_info_.is_run_ = false;
    timer_thread_info_.condition_.notify_one();
    this_thread::sleep_for(milliseconds(100));
}

void CTimerManager::run()
{
    if(timer_thread_info_.is_run_ == false)
    {
        timer_thread_info_.thread_ = std::thread(timer_thread_run, &timer_thread_info_);
        timer_thread_info_.thread_.detach();
        this_thread::sleep_for(milliseconds(10));
    }
}

