#pragma once

//线程处理队列，处理消息队列和定时消息
//add by freeeyes
#include "threadqueue.h"
#include "timermanager.h"
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
		std::cout << "Thread(" << m_u4ThreadID << ") is Finish." << std::endl;
	}

	void svc()
	{
		m_tvBegin = CTimeStamp::Get_Time_Stamp();
		while (m_run)
		{
			shared_ptr<CLogicMessage> msg;
			if (true == m_thread_queue.Pop(msg))
			{
				if(EM_LOGIC_TYPE::LOGIC_TYPE_RUN == msg->m_emType)
				//获得了数据，进行处理
				msg->m_func();
			}
			else
			{
				//关闭线程
				continue;
			}
		}

		std::cout << "Thread is over." << std::endl;
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
	
	bool CreateLogic(uint32 u4LogicID) //创建一个逻辑线程
	{
		auto f = m_mapLogicList.find(u4LogicID);
		if(f != m_mapLogicList.end())
		{
			return false;		
		}
		else
		{
			//创建线程
			auto pLogicTask = std::make_shared<CLogicTasK>();
			pLogicTask->start(u4LogicID);
			
			m_mapLogicList[u4LogicID] = pLogicTask;
			
			cout << "CreateLogic(" << u4LogicID << ")." << endl;
		}
	};
	
	//添加消息
	bool AddMessage(uint32 u4LogicID, task_function func)
	{
		auto f = m_mapLogicList.find(u4LogicID);
		if(f != m_mapLogicList.end())
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
	
	//关闭系统
	void Close()
	{
    for_each(m_mapLogicList.begin(), m_mapLogicList.end(), [](const std::pair<uint32, shared_ptr<CLogicTasK>>& iter) {
        iter.second->Close();
        });
        
		cout << "TMS is close." << endl;
	}
	
private:
		using mapthreads = map<uint32, std::shared_ptr<CLogicTasK>>;
		mapthreads m_mapLogicList;
};

