#ifndef _TSCOMMON_H
#define _TSCOMMON_H

#include "CTimerEvent.h"
#include "XmlOpeation.h"
#include "HashTable.h"
#include "ThreadLogic.h"
#include "IMessageQueueManager.h"
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

class CEventsInfo
{
public:
    system_clock::time_point    m_ttNextTime;             //下一次执行时间
    void*                       m_pArg;
    int                         m_nMessageID;
    int                         m_nWorkThreadID;
    int                         m_nMessagePos;
    int                         m_nSec;                    //time of interval
    int                         m_nUsec;
    Enum_Message_Execute_State  m_emMessageState;
    Enum_Timer_Mode             m_emTimerMode;

    CMessageInfo::UserFunctor   fn;
    IMessageQueueManager*       m_pMessageQueueManager;

    CEventsInfo() : m_pArg(NULL), m_nMessageID(0), m_nWorkThreadID(0), m_nMessagePos(0), m_nSec(0), m_nUsec(0), m_emMessageState(Message_Run), m_emTimerMode(Timer_Mode_Run_Once), m_pMessageQueueManager(NULL)
    {
    }
};

using vecEventsList = vector<CEventsInfo>;

//定时器信息结构
class CTimerInfo
{
public:
    CTimerInfo() : m_nID(0), m_szName{ '\0' }, m_nInterval(0), m_nMaxQueueList(0), m_pMessageQueueManager(NULL)
    {
    }

    void AddEventsInfo(CEventsInfo objEventsInfo)
    {
        std::lock_guard <std::mutex> lock(m_objMutex);

        m_vecEventsList.push_back(objEventsInfo);
    }

    void DeleteEventInfo(int nMessagePos)
    {
        std::lock_guard <std::mutex> lock(m_objMutex);

        for (int i = 0; i < (int)m_vecEventsList.size(); i++)
        {
            if (m_vecEventsList[i].m_nMessagePos == nMessagePos)
            {
                m_vecEventsList[i].m_emMessageState = Message_Cancel;
                break;
            }
        }
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

                if ((*it).m_pMessageQueueManager != NULL && Message_Run == (*it).m_emMessageState)
                {
                    //输出到消息队列(消息)
                    (*it).m_pMessageQueueManager->SendLogicThreadMessage((*it).m_nMessageID, (*it).m_pArg);
                }
                else
                {
                    //打印日志
                    std::cout << "[CTaskTimeNode::Run](" << (*it).m_nMessageID << ") is disposed.\n" << endl;
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
