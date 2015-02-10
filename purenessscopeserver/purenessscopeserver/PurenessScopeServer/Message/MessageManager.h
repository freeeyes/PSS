#ifndef _MESSAGEMANAGER_H
#define _MESSAGEMANAGER_H

#include "IMessageManager.h"
#include "Message.h"
#include "LoadModule.h"

//这里修改一下，如果一个命令对应一个模块是有限制的。
//这里改为一个信令可以对应任意数量的处理模块，这样就比较好了。
//add by freeeyes

using namespace std;

//命令订阅者者的格式
struct _ClientCommandInfo
{
	CClientCommand* m_pClientCommand;              //当前命令指针
	uint16          m_u2CommandID;                 //当前命令对应的ID
	uint32          m_u4Count;                     //当前命令被调用的次数
	uint32          m_u4TimeCost;                  //当前命令总时间消耗
	char            m_szModuleName[MAX_BUFF_100];  //所属模块名称
	ACE_Date_Time   m_dtLoadTime;                  //当前命令加载时间
	uint32          m_u4CurrUsedCount;             //当前正在使用的引用次数
	uint8           m_u1State;                     //当前命令的状态，0为正常，1为正在关闭

	_ClientCommandInfo()
	{
		m_pClientCommand  = NULL;
		m_u4Count         = 0;
		m_u4TimeCost      = 0;
		m_szModuleName[0] = '\0';
		m_u4CurrUsedCount = 0;
		m_u1State         = 0;
	}
};

//模块和_ClientCommandInfo之间的对应关系
struct _ModuleClient
{
	vector<_ClientCommandInfo*> m_vecClientCommandInfo;    //一个模块所有对应命令列表
};

//管理工具需要此数据结构，用于回传计算模块信息
typedef map<string, _ModuleClient*> mapModuleClient;

//一个消息可以对应一个CClientCommand*的数组，当消息到达的时候分发给这些订阅者
class CClientCommandList
{
private:
	typedef vector<_ClientCommandInfo*> vecClientCommandList;
	vecClientCommandList m_vecClientCommandList;

public:
	CClientCommandList()
	{
	};

	~CClientCommandList()
	{
		Close();
	};

	void Close()
	{
		for(int i = 0; i < (int)m_vecClientCommandList.size(); i++)
		{
			_ClientCommandInfo* pClientCommandInfo = m_vecClientCommandList[i];
			SAFE_DELETE(pClientCommandInfo);
		}
		m_vecClientCommandList.clear();
	}

	_ClientCommandInfo* AddClientCommand(CClientCommand* pClientCommand, const char* pMuduleName)
	{
		_ClientCommandInfo* pClientCommandInfo = new _ClientCommandInfo();
		if(NULL != pClientCommandInfo)
		{
			pClientCommandInfo->m_pClientCommand = pClientCommand;
			m_vecClientCommandList.push_back(pClientCommandInfo);
			sprintf_safe(pClientCommandInfo->m_szModuleName, MAX_BUFF_100, "%s", pMuduleName);
			return pClientCommandInfo;
		}
		else
		{
			return NULL;
		}

	};

	//如果返回为true，证明这个消息已经没有对应项，需要外围map中除去
	bool DelClientCommand(CClientCommand* pClientCommand)
	{
		for(vecClientCommandList::iterator b = m_vecClientCommandList.begin(); b!= m_vecClientCommandList.end(); b++)
		{
			_ClientCommandInfo* pClientCommandInfo = (_ClientCommandInfo* )(*b);
			if(NULL != pClientCommandInfo && pClientCommand == pClientCommandInfo->m_pClientCommand)
			{
				SAFE_DELETE(pClientCommandInfo);
				m_vecClientCommandList.erase(b);
				break;
			}
		}

		if((int)m_vecClientCommandList.size() == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//得到个数
	int GetCount()
	{
		return (int)m_vecClientCommandList.size();
	}

	//得到指定位置的指针
	_ClientCommandInfo* GetClientCommandIndex(int nIndex)
	{
		if(nIndex >= (int)m_vecClientCommandList.size())
		{
			return NULL;
		}
		else
		{
			return (_ClientCommandInfo* )m_vecClientCommandList[nIndex];
		}
	}
};

class CMessageManager : public IMessageManager
{
public:
	CMessageManager(void);
	~CMessageManager(void);

	bool DoMessage(ACE_Time_Value& tvBegin, IMessage* pMessage, uint16& u2CommandID, uint32& u4TimeCost, uint16& u2Count, bool& bDeleteFlag);   //执行命令
	void Close();

	bool AddClientCommand(uint16 u2CommandID, CClientCommand* pClientCommand, const char* pModuleName);   //注册命令
	bool DelClientCommand(uint16 u2CommandID, CClientCommand* pClientCommand);                            //卸载命令
 
	bool UnloadModuleCommand(const char* pModuleName, uint8 u1State);  //卸载指定模块事件，u1State= 1 卸载，2 重载

	int  GetCommandCount();                                            //得到当前注册命令的个数
	CClientCommandList* GetClientCommandList(uint16 u2CommandID);      //得到当前命令的执行列表
	
	mapModuleClient* GetModuleClient();                                //返回所有模块绑定注册命令信息

	virtual uint32 GetWorkThreadCount();
	virtual uint32 GetWorkThreadByIndex(uint32 u4Index);

private:
	typedef map<uint16, CClientCommandList*> mapClientCommand;

	mapClientCommand            m_mapClientCommand;
	mapModuleClient             m_mapModuleClient;
	ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;                     //用于控制计数器的增减
};

typedef ACE_Singleton<CMessageManager, ACE_Null_Mutex> App_MessageManager; 
#endif
