#ifndef _TIMER_NODE_LIST_H
#define _TIMER_NODE_LIST_H

#include <iostream>
#include <chrono>
#include <algorithm>
#include <functional>
#include <vector>
#include <memory>

using namespace std;
using namespace std::chrono;

class ITimerInfo
{
public:
    virtual ~ITimerInfo() {};
    virtual void run() = 0;
};

using TimerFunctor = std::function<void(ITimerInfo*)>;

//节点信息
class CTimerNodeInfo
{
public:
    CTimerNodeInfo() : timer_id_(0), timer_interval_(0), function_arg_(nullptr)
    {
        steady_clock::time_point timer_now = steady_clock::now();
        begin_time_ = timer_now;
        next_time_  = timer_now;
    }

    void Step(steady_clock::time_point& timer_now)
    {
        if (next_time_ < timer_now)
        {
            //需要跳转计算到当前时间最后一个执行时间点
            milliseconds interval = duration_cast<milliseconds>(timer_now - begin_time_);

            milliseconds begin = interval % timer_interval_;

            next_time_ = timer_now + (timer_interval_ - begin);
        }
        else
        {
            next_time_ += timer_interval_;
        }
    }

    bool operator < (const CTimerNodeInfo& ar) const
    {
        return (this->next_time_ < ar.next_time_);
    }

public:
    int timer_id_;                            //定时器ID
    milliseconds timer_interval_;             //具体时间间隔
    steady_clock::time_point next_time_;      //下一次执行时间
    steady_clock::time_point begin_time_;     //定时器开始时间
    TimerFunctor timer_function_;             //定时器执行函数
    ITimerInfo*  function_arg_;               //执行函数参数
};

//镜像信息
class CRunNodeInfo
{
public:
    CRunNodeInfo() : timer_id_(0), curr_time_(steady_clock::now()), timer_interval_(0), function_arg_(nullptr)
    {
    };

    int                      timer_id_;
    steady_clock::time_point curr_time_;
    microseconds             timer_interval_;
    TimerFunctor             timer_function_;             //定时器执行函数
    ITimerInfo*              function_arg_;               //执行函数参数
};

class CTimerNodeList
{
public:
    CTimerNodeList() : lcm_data_(0), timer_run_list_index_(0)
    {

    };
    ~CTimerNodeList() {};

    void add_timer_node_info(int timer_id, milliseconds interval, TimerFunctor f, ITimerInfo* arg)
    {
        std::shared_ptr<CTimerNodeInfo> timer_node_info = std::make_shared<CTimerNodeInfo>();
        steady_clock::time_point timer_now = steady_clock::now();

        timer_node_info->timer_id_ = timer_id;
        timer_node_info->timer_interval_ = interval;
        timer_node_info->timer_function_ = std::move(f);
        timer_node_info->function_arg_ = arg;
        timer_node_info->Step(timer_now);

        vec_timer_node_list_.push_back(timer_node_info);

        get_run_list(timer_now);

        timer_run_list_index_ = 0;
    };
    void del_timer_node_info(int timer_id)
    {
        steady_clock::time_point timer_now = steady_clock::now();

        for (auto it = vec_timer_node_list_.begin(); it != vec_timer_node_list_.end(); it++)
        {
            if ((*it)->timer_id_ == timer_id)
            {
                vec_timer_node_list_.erase(it);

                get_run_list(timer_now);

                return;
            }
        }

        timer_run_list_index_ = 0;
    };

    void display()
    {
        int nSize = (int)vec_timer_run_list_.size();
        std::cout << "timer count " << nSize << endl;

        //显示最小公倍数
        std::cout << "lcm is " << lcm_data_ << endl;

        std::cout << "############" << endl;

        for (auto f : vec_timer_run_list_)
        {
            std::cout << "timer id (" << f.timer_id_ << ") interval=" << f.timer_interval_.count() << endl;
        }

        std::cout << "############" << endl;
    };

    void get_run_list(steady_clock::time_point timer_now = steady_clock::now())
    {
        int nSize = (int)vec_timer_node_list_.size();

        //设置判定循环
        vector<CTimerNodeInfo> vec_timer_temp_node_list_;

        for (auto f : vec_timer_node_list_)
        {
            CTimerNodeInfo temp_node;
            temp_node = *(f.get());
            vec_timer_temp_node_list_.push_back(temp_node);
        }

        //计算所有数据下次执行时间
        lcm_data_ = 0;

        for (int i = 0; i < nSize; i++)
        {
            if (i == 0)
            {
                lcm_data_ = (int)vec_timer_node_list_[i]->timer_interval_.count();
            }
            else
            {
                lcm_data_ = lcm((int)vec_timer_node_list_[i]->timer_interval_.count(), lcm_data_);
            }
        }

        //开始生成数据循环集合
        if (nSize > 1)
        {
            vec_timer_run_list_.clear();

            std::sort(vec_timer_temp_node_list_.begin(), vec_timer_temp_node_list_.end());

            timer_now = vec_timer_temp_node_list_[0].next_time_;

            while (vec_timer_temp_node_list_[0].next_time_ < timer_now + milliseconds(lcm_data_))
            {
                //std::cout << "Timer ID " << vec_timer_node_list_[0]->timer_id_ << endl;
                CRunNodeInfo run_node_info;
                run_node_info.timer_id_ = vec_timer_temp_node_list_[0].timer_id_;
                run_node_info.curr_time_ = vec_timer_temp_node_list_[0].next_time_;
                run_node_info.timer_function_ = vec_timer_temp_node_list_[0].timer_function_;
                run_node_info.function_arg_ = vec_timer_temp_node_list_[0].function_arg_;

                vec_timer_temp_node_list_[0].Step(timer_now);
                std::sort(vec_timer_temp_node_list_.begin(), vec_timer_temp_node_list_.end());

                run_node_info.timer_interval_ = duration_cast<microseconds>(vec_timer_temp_node_list_[0].next_time_ - run_node_info.curr_time_);

                vec_timer_run_list_.push_back(run_node_info);
            }

            //std::cout << "Timer Count " << vec_timer_run_list_.size() << endl;
        }
        else if (nSize == 1)
        {
            CRunNodeInfo run_node_info;
            run_node_info.timer_id_ = vec_timer_node_list_[0]->timer_id_;
            run_node_info.timer_interval_ = vec_timer_node_list_[0]->timer_interval_;
            run_node_info.timer_function_ = vec_timer_temp_node_list_[0].timer_function_;
            run_node_info.function_arg_ = vec_timer_temp_node_list_[0].function_arg_;

            vec_timer_run_list_.push_back(run_node_info);
        }
    };

    int get_next_run_timer_interval(CRunNodeInfo& run_node_info, microseconds& timer_interval)
    {
        int curr_size = (int)vec_timer_run_list_.size();

        //std::cout << "[get_next_run_timer_interval]curr_size = " << curr_size << endl;

        if (curr_size == 0)
        {
            return -1;
        }
        else if (curr_size == 1)
        {
            timer_interval = vec_timer_run_list_[0].timer_interval_;
            run_node_info = vec_timer_run_list_[0];
            return 0;
        }
        else
        {
            if (timer_run_list_index_ == 0)
            {
                timer_interval = vec_timer_run_list_[curr_size - 1].timer_interval_;
                run_node_info = vec_timer_run_list_[0];
            }
            else
            {
                timer_interval = vec_timer_run_list_[timer_run_list_index_ - 1].timer_interval_;
                run_node_info = vec_timer_run_list_[timer_run_list_index_];
            }

            if (++timer_run_list_index_ >= curr_size)
            {
                timer_run_list_index_ = 0;
            }

            //std::cout << "[get_next_run_timer_interval]timer_run_list_index_ = " << timer_run_list_index_ << endl;
            return 0;
        }
    };

private:
    int lcm(int num1, int num2)
    {
        return num1 / gcd(num1, num2) * num2;
    };
    int gcd(int num1, int num2)
    {
        while (num1 != num2)
        {
            if (num1 > num2)
            {
                num1 = num1 - num2;
            }
            else
            {
                num2 = num2 - num1;
            }
        }

        return num1;
    };

public:
    vector<std::shared_ptr<CTimerNodeInfo>> vec_timer_node_list_;
    vector<CRunNodeInfo> vec_timer_run_list_;
    int lcm_data_;
    int timer_run_list_index_;
};
#endif
