#ifndef _THREADLOGIC_H
#define _THREADLOGIC_H

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <unordered_map>
#include <functional>

using namespace std;

class CMessageInfo
{
public:
    using UserFunctor = std::function<void(int, void*)>;
    UserFunctor f;
    int message_id_ = 0;
    void* arg_      = NULL;
};

class CThreadInfo
{
public:
    CThreadInfo() : thread_logic_id_(0), is_run_(true)
    {

    };

    int thread_logic_id_;
    bool is_run_;
    mutable std::mutex thread_mutex_;
    std::condition_variable condition_;
    vector<CMessageInfo> thread_queue_data_;
};

static void thread_run(CThreadInfo* _thread_info)
{
    if (nullptr == _thread_info)
    {
        return;
    }

    printf("[thread_run]<%d> is begin.\n", _thread_info->thread_logic_id_);

    while (_thread_info->is_run_)
    {
        if (_thread_info->thread_queue_data_.size() == 0)
        {
            printf("[thread_run]<%d> is wait.\n", _thread_info->thread_logic_id_);
            std::unique_lock <std::mutex> lock(_thread_info->thread_mutex_);
            _thread_info->condition_.wait(lock);
            printf("[thread_run]<%d> is wakeup.\n", _thread_info->thread_logic_id_);
        }
        else
        {
            vector<CMessageInfo> thread_curr_queue_data_;
            _thread_info->thread_mutex_.lock();
            thread_curr_queue_data_.swap(_thread_info->thread_queue_data_);
            _thread_info->thread_mutex_.unlock();

            for (int i = 0; i < (int)thread_curr_queue_data_.size(); i++)
            {
                thread_curr_queue_data_[i].f(thread_curr_queue_data_[i].message_id_, thread_curr_queue_data_[i].arg_);
            }
        }
    }

    //printf("[thread_run]<%d> is close.\n", _thread_info->thread_logic_id_);
    delete _thread_info;
    _thread_info = NULL;
}

class CThreadQueueManager
{
public:
    CThreadQueueManager() {};

    void Close()
    {
        for (auto it = thread_list_.begin(); it != thread_list_.end(); ++it)
        {
            CThreadInfo* pThreadInfo = it->second;
            pThreadInfo->is_run_ = false;
            pThreadInfo->condition_.notify_one();
        }
    };

    bool Create(int _thread_logic_id)
    {
        unordered_map<int, CThreadInfo*>::iterator f = thread_list_.find(_thread_logic_id);

        if (thread_list_.end() != f)
        {
            printf("[CThreadQueueManager::Create]_thread_logic_id(%d) is exist.\n", _thread_logic_id);
            return false;
        }


        CThreadInfo* _thread_info = new CThreadInfo();
        _thread_info->thread_logic_id_ = _thread_logic_id;
        std::thread thread_ = std::thread(thread_run, _thread_info);
        thread_.detach();

        threads_.push_back(std::move(thread_));

        thread_list_.insert(std::make_pair(_thread_logic_id, _thread_info));

        return true;
    };

    bool AddMessage(int _thread_logic_id, CMessageInfo::UserFunctor&& fn, int _Message_id, void* _arg)
    {
        unordered_map<int, CThreadInfo*>::iterator f = thread_list_.find(_thread_logic_id);

        if (thread_list_.end() == f)
        {
            printf("[CThreadQueueManager::Create]_thread_logic_id(%d) is not exist.\n", _thread_logic_id);
            return false;
        }

        CThreadInfo* _thread_info = f->second;

        {
            std::lock_guard <std::mutex> lock(_thread_info->thread_mutex_);
            CMessageInfo _message_info;
            _message_info.f = std::move(fn);
            _message_info.message_id_ = _Message_id;
            _message_info.arg_ = _arg;

            _thread_info->thread_queue_data_.push_back(_message_info);
            _thread_info->condition_.notify_one();
        }

        return true;
    };

    template<typename TYPE>
    bool AddMessageClass(int _thread_logic_id, TYPE* my_obj_ptr, void(TYPE::*handler)(int, void*), int msg_id, void* arg)
    {
        auto functor = std::bind(handler, my_obj_ptr, std::placeholders::_1, std::placeholders::_2);
        return AddMessage(_thread_logic_id, std::move(functor), msg_id, arg);
    }

private:
    unordered_map<int, CThreadInfo*> thread_list_;
    vector<std::thread> threads_;
};

#endif
