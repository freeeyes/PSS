#ifndef _TSCOMMON_H
#define _TSCOMMON_H

#include "TimerEvent.hpp"
#include "HashTable.h"
#include "ThreadLogic.hpp"
#include <vector>

using namespace std;

const char XML_CONF_FILE[] = "Timer.xml";

enum Enum_Timer_Mode
{
    Timer_Mode_Run_Once = 0,
    Timer_Mode_Interval,
};

enum Enum_Message_Execute_State
{
    Message_Run = 0,
    Message_Cancel,
};

//消息处理类模板
class IMessagePrecess
{
public:
    virtual ~IMessagePrecess() {};

    virtual void DoMessage(int _message_id, void* _arg) = 0;
};

class CEventsInfo
{
public:
    system_clock::time_point    m_ttNextTime;             //下一次执行时间
    void*                       m_pArg;
    int                         m_nMessageID;
    int                         m_nWorkThreadID;
    unsigned long long          m_nMessagePos;
    int                         m_nSec;                    //time of interval
    int                         m_nUsec;
    Enum_Message_Execute_State  m_emMessageState;
    Enum_Timer_Mode             m_emTimerMode;
    IMessagePrecess*            m_pIMessagePrecess;

    CEventsInfo() : m_pArg(nullptr), m_nMessageID(0), m_nWorkThreadID(0), m_nMessagePos(0), m_nSec(0), m_nUsec(0), m_emMessageState(Message_Run), m_emTimerMode(Timer_Mode_Run_Once), m_pIMessagePrecess(nullptr)
    {
    }
};

using vecEventsList = vector<CEventsInfo>;

//定时器信息结构
class CTimerInfo : public ITimerInfo
{
public:
    CTimerInfo() : m_nID(0), m_szName{ '\0' }, m_nInterval(0), m_nMaxQueueList(0), m_pMessageQueueManager(nullptr)
    {
    }

    virtual ~CTimerInfo() {}

    void AddEventsInfo(CEventsInfo objEventsInfo)
    {
        std::lock_guard <std::mutex> lock(m_objMutex);

        m_vecEventsList.push_back(objEventsInfo);
    }

    void* DeleteEventInfo(unsigned long long nMessagePos)
    {
        std::lock_guard <std::mutex> lock(m_objMutex);

        void* prg = nullptr;

        for (int i = 0; i < (int)m_vecEventsList.size(); i++)
        {
            if (m_vecEventsList[i].m_nMessagePos == nMessagePos)
            {
                m_vecEventsList[i].m_emMessageState = Message_Cancel;
                prg = m_vecEventsList[i].m_pArg;
                break;
            }
        }

        return prg;
    }

    void run()
    {
        std::lock_guard <std::mutex> lock(m_objMutex);
        auto tt_now = system_clock::now();

        //循环比较是否到期
        for (vecEventsList::iterator it = m_vecEventsList.begin(); it != m_vecEventsList.end();)
        {
            if ((*it).m_ttNextTime <= tt_now)
            {
                //到时的数据，拿出来处理
                std::cout << "[CTaskTimeNode::Run](" << m_szName.c_str() << ") is Arrived.\n" << endl;

                if (nullptr != m_pMessageQueueManager)
                {
                    m_pMessageQueueManager->AddMessageClass((*it).m_nWorkThreadID,
                                                            (*it).m_pIMessagePrecess,
                                                            &IMessagePrecess::DoMessage,
                                                            (*it).m_nMessageID,
                                                            (*it).m_pArg);
                }

                if ((*it).m_emTimerMode == Timer_Mode_Run_Once)
                {
                    it = m_vecEventsList.erase(it);
                }
                else
                {
                    //如果是定时执行需求，重新计算下一次的执行时间。
                    system_clock::time_point ttNextTime = tt_now + seconds((*it).m_nSec) + milliseconds((*it).m_nUsec);
                    (*it).m_ttNextTime = ttNextTime;
                }
            }
            else
            {
                ++it;
            }
        }
    }

public:
    int  m_nID;
    string m_szName;
    int  m_nInterval;
    int  m_nMaxQueueList;
    mutable std::mutex m_objMutex;
    vecEventsList m_vecEventsList;
    CThreadQueueManager* m_pMessageQueueManager;
};

#endif
