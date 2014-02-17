#ifndef _COMMANDACCOUNT_K
#define _COMMANDACCOUNT_K

// 统计所有进出框架的命令执行情况
// add by freeeyes
// 2012-03-19

#include "ace/Date_Time.h"
#include "define.h"
#include <map>

using namespace std;

//统计信息，里面有要统计的命令信息定义
struct _CommandData
{
	uint16 m_u2CommandID;                  //命令的ID
	uint32 m_u4CommandCount;               //命令的总调用次数
	uint64 m_u8CommandCost;                //命令的执行耗费总时间
	uint8  m_u1CommandType;                //命令的类型，0是收到的命令，1是发出的命令
	uint32 m_u4PacketSize;                 //命令产生的总流量(未解密)
	uint32 m_u4CommandSize;                //命令产生的总流量(解密)
	uint8  m_u1PacketType;                 //数据包来源类型  
	ACE_Time_Value m_tvCommandTime;        //命令的最后处理时间

	_CommandData()
	{
		m_u2CommandID    = 0;
		m_u4CommandCount = 1;
		m_u8CommandCost  = 0;
		m_u4PacketSize   = 0;
		m_u4CommandSize  = 0;
		m_u1PacketType   = PACKET_TCP;
		m_u1CommandType  = COMMAND_TYPE_IN;
	}
};


struct _CommandTimeOut
{
	uint16         m_u2CommandID;                  //命令的ID
	ACE_Time_Value m_tvTime;                       //发生时间
	uint32         m_u4TimeOutTime;                //超时时间

	_CommandTimeOut()
	{
		m_u2CommandID   = 0;
		m_tvTime        = ACE_OS::gettimeofday();
		m_u4TimeOutTime = 0;
	}
};

typedef vector<_CommandTimeOut> vecCommandTimeOut;   //记录所有超时命令的数组

//统计所有进出框架的命令执行情况，目前不包括向其他服务器请求的统计，因为这部分协议无法统一。
class CCommandAccount
{
public:
	CCommandAccount();
	~CCommandAccount();

	void Init(uint8 u1CommandAccount, uint16 u2RecvTimeout);

	bool   SaveCommandData(uint16 u2CommandID, uint64 u8CommandCost, uint8 u1PacketType = PACKET_TCP, uint32 u4PacketSize = 0, uint32 u4CommandSize = 0, uint8 u1CommandType = COMMAND_TYPE_IN, ACE_Time_Value tvTime = ACE_OS::gettimeofday());   //记录命令执行信息
	bool   SaveCommandDataLog();                       //存储命令执行信息的日志
	void   ClearTimeOut();                             //清理超时时间的命令日志
	uint32 GetTimeoutCount();                          //得到超时命令日志个数
	_CommandTimeOut* GetTimeoutInfo(uint32 u4Index);   //得到超时数据库

	void   Close();

public:
	typedef map<uint16, _CommandData*> mapCommandDataList;
	vecCommandTimeOut  m_vecCommandTimeOut;
	mapCommandDataList m_mapCommandDataList;
	uint8              m_u1CommandAccount;
	uint64             m_u8PacketTimeout;
};

typedef ACE_Singleton<CCommandAccount, ACE_Recursive_Thread_Mutex> App_CommandAccount; 

#endif
