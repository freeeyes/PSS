#pragma once

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

template<class Type>
class CMessageQueue
{
public:
    CMessageQueue& operator = (const CMessageQueue&) = delete;
    CMessageQueue(const CMessageQueue& mq) = delete;


    CMessageQueue() :_queue(), _mutex(), _condition() {}
    virtual ~CMessageQueue() {}

    void Push(Type msg) {
        std::lock_guard <std::mutex> lock(_mutex);
        _queue.push(msg);
        //当使用阻塞模式从消息队列中获取消息时，由condition在新消息到达时提醒等待线程
        _condition.notify_one();
    }

    //blocked定义访问方式是同步阻塞或者非阻塞模式
    bool Pop(Type& msg, bool isBlocked = true) {
        if (isBlocked)
        {
            std::unique_lock <std::mutex> lock(_mutex);
            while (_queue.empty())
            {
                _condition.wait(lock);

            }
            //注意这一段必须放在if语句中，因为lock的生命域仅仅在if大括号内
            msg = std::move(_queue.front());
            _queue.pop();
            return true;

        }
        else
        {
            std::lock_guard<std::mutex> lock(_mutex);
            if (_queue.empty())
                return false;


            msg = std::move(_queue.front());
            _queue.pop();
            return true;
        }

    }

    int32_t Size() {
        std::lock_guard<std::mutex> lock(_mutex);
        return _queue.size();
    }

    bool Empty() {
        std::lock_guard<std::mutex> lock(_mutex);
        return _queue.empty();
    }
private:
    std::queue<Type> _queue;//存储消息的队列
    mutable std::mutex _mutex;//同步锁
    std::condition_variable _condition;//实现同步式获取消息
};
