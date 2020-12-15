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
        //��ʹ������ģʽ����Ϣ�����л�ȡ��Ϣʱ����condition������Ϣ����ʱ���ѵȴ��߳�
        _condition.notify_one();
    }

    //blocked������ʷ�ʽ��ͬ���������߷�����ģʽ
    bool Pop(Type& msg, bool isBlocked = true) {
        if (isBlocked)
        {
            std::unique_lock <std::mutex> lock(_mutex);
            while (_queue.empty())
            {
                _condition.wait(lock);

            }
            //ע����һ�α������if����У���Ϊlock�������������if��������
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
    std::queue<Type> _queue;//�洢��Ϣ�Ķ���
    mutable std::mutex _mutex;//ͬ����
    std::condition_variable _condition;//ʵ��ͬ��ʽ��ȡ��Ϣ
};
