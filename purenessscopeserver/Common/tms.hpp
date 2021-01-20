#pragma once

//�̴߳�����У�������Ϣ���кͶ�ʱ��Ϣ
//add by freeeyes
#include "ThreadQueue.h"
#include "TimerManager.hpp"
#include "TimeStamp.hpp"
#include <map>

using namespace std;

using task_function = std::function<void()>;

enum class EM_LOGIC_TYPE
{
    LOGIC_TYPE_RUN = 0,
    LOGIC_TYPE_CLOSE
};

class CLogicMessage
{
public:
    task_function m_func;
    EM_LOGIC_TYPE m_emType = EM_LOGIC_TYPE::LOGIC_TYPE_RUN;
};

class CLogicTasK
{
public:
    CLogicTasK() = default;

    void Put(std::shared_ptr<CLogicMessage> msg)
    {
        m_thread_queue.Push(msg);
    }

    void start(uint32 u4ThreadID)
    {
        m_run = true;
        m_ttlogic = std::thread([this]()
            {
                svc();
            });
        m_u4ThreadID = u4ThreadID;
    }

    void Close()
    {
        m_run = false;

        auto pLogicMessage = std::make_shared<CLogicMessage>();
        pLogicMessage->m_emType = EM_LOGIC_TYPE::LOGIC_TYPE_CLOSE;
        Put(pLogicMessage);

        m_ttlogic.join();
        PSS_LOGGER_DEBUG("Thread({0}) is Finish.", m_u4ThreadID);
    }

    void svc()
    {
        m_tvBegin = CTimeStamp::Get_Time_Stamp();
        while (m_run)
        {
            shared_ptr<CLogicMessage> msg;
            if (true == m_thread_queue.Pop(msg))
            {
                if (EM_LOGIC_TYPE::LOGIC_TYPE_RUN == msg->m_emType)
                {
                    //��������ݣ����д���
                    msg->m_func();
                }
                else
                {
                    break;
                }
            }
            else
            {
                //�ر��߳�
                continue;
            }
        }

        PSS_LOGGER_DEBUG("Thread({0}) is over.", m_u4ThreadID);
    }

private:
    CMessageQueue<shared_ptr<CLogicMessage>> m_thread_queue;
    bool m_run = false;
    std::thread m_ttlogic;
    PSS_Time_Point m_tvBegin;
    uint32 m_u4ThreadID = 0;
};

class TMS
{
public:
    TMS() = default;

    //��ʼ��
    void Init()
    {
        //������ʱ���߳�
        m_ttTimer = std::thread([this]()
            {
                m_timerManager.schedule();
            });
    }

    bool CreateLogic(uint32 u4LogicID) //����һ���߼��߳�
    {
        auto f = m_mapLogicList.find(u4LogicID);
        if (f != m_mapLogicList.end())
        {
            return false;
        }
        else
        {
            //�����߳�
            auto pLogicTask = std::make_shared<CLogicTasK>();
            pLogicTask->start(u4LogicID);

            m_mapLogicList[u4LogicID] = pLogicTask;

            PSS_LOGGER_DEBUG("CreateLogic({0}) is created.", u4LogicID);
            return true;
        }
    };

    bool CloseLogic(uint32 u4LogicID)  //�ر�һ���߼��߳�
    {
        auto f = m_mapLogicList.find(u4LogicID);
        if (f != m_mapLogicList.end())
        {
            auto pLogicTask = f->second;
            pLogicTask->Close();
            m_mapLogicList.erase(f);
            return true;
        }
        else
        {
            return false;
        }
    }

    //�����Ϣ(��ʱ)
    bool AddMessage(uint32 u4LogicID, task_function func)
    {
        auto f = m_mapLogicList.find(u4LogicID);
        if (f != m_mapLogicList.end())
        {
            auto pLogicMessage = std::make_shared<CLogicMessage>();
            pLogicMessage->m_func = func;
            m_mapLogicList[u4LogicID]->Put(pLogicMessage);
            return true;
        }
        else
        {
            return false;
        }
    };

    //�����Ϣ(��ʱ)
    brynet::Timer::WeakPtr AddMessage(uint32 u4LogicID, std::chrono::milliseconds millisecond, task_function func)
    {
        brynet::Timer::WeakPtr timer;
        auto f = m_mapLogicList.find(u4LogicID);
        if (f != m_mapLogicList.end())
        {
            auto pLogicMessage = std::make_shared<CLogicMessage>();
            pLogicMessage->m_func = func;

            timer = m_timerManager.addTimer(millisecond, [this, u4LogicID, pLogicMessage]() {
                m_mapLogicList[u4LogicID]->Put(pLogicMessage);
                });
        }

        return timer;
    }

    //�ر�ϵͳ
    void Close()
    {
        for_each(m_mapLogicList.begin(), m_mapLogicList.end(), [](const std::pair<uint32, shared_ptr<CLogicTasK>>& iter) {
            iter.second->Close();
            });

        //�رն�ʱ��
        m_timerManager.Close();
        m_ttTimer.join();

        PSS_LOGGER_DEBUG("TMS is close.");

    }

private:
    using mapthreads = map<uint32, std::shared_ptr<CLogicTasK>>;
    brynet::TimerMgr m_timerManager;
    mapthreads m_mapLogicList;
    std::thread m_ttTimer;
};

