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
    int message_id_;
    void* arg_;
};

class CThreadInfo
{
public:
    CThreadInfo();

    int thread_logic_id_;
    bool is_run_;
    std::thread::id thread_id_;
    std::thread thread_;
    mutable std::mutex thread_mutex_;
    std::condition_variable condition_;
    vector<CMessageInfo> thread_queue_data_;
};

class CThreadQueueManager
{
public:
    CThreadQueueManager();

    void Close();

    bool Create(int _thread_logic_id);

    bool AddMessage(int _thread_logic_id, CMessageInfo::UserFunctor&& f, int _Message_id, void* _arg);

    template<typename TYPE>
    bool AddMessageClass(int _thread_logic_id, TYPE* my_obj_ptr, void(TYPE::*handler)(int, void*), int msg_id, void* arg)
    {
        auto functor = std::bind(handler, my_obj_ptr, std::placeholders::_1, std::placeholders::_2);
        return AddMessage(_thread_logic_id, std::move(functor), msg_id, arg);
    }

private:
    unordered_map<int, CThreadInfo*> thread_list_;
};

#endif
