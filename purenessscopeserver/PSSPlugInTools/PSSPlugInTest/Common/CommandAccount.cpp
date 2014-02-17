#include "CommandAccount.h"

CCommandAccount::CCommandAccount()
{
	m_u1CommandAccount = 0;
	m_u8PacketTimeout    =  MAX_QUEUE_TIMEOUT * 1000;
}

CCommandAccount::~CCommandAccount()
{
	OUR_DEBUG((LM_ERROR, "CCommandAccount::~CCommandAccount].\n"));
	Close();
}

void CCommandAccount::Init(uint8 u1CommandAccount, uint16 u2PacketTimeout)
{
	m_u1CommandAccount = u1CommandAccount;
	m_u8PacketTimeout    = (uint64)(u2PacketTimeout * 1000000);
}

void CCommandAccount::Close()
{
	for(mapCommandDataList::iterator itorFreeB = m_mapCommandDataList.begin(); itorFreeB != m_mapCommandDataList.end(); itorFreeB++)
	{
		_CommandData* pCommandData = (_CommandData* )itorFreeB->second;
		SAFE_DELETE(pCommandData);
	}

	m_mapCommandDataList.clear();
	m_u1CommandAccount = 0;
}

bool CCommandAccount::SaveCommandData(uint16 u2CommandID, uint64 u8CommandCost, uint8 u1PacketType, uint32 u4PacketSize, uint32 u4CommandSize, uint8 u1CommandType, ACE_Time_Value tvTime)
{
	//统计数据到达时间，是否已经超越了限定的阀值，如果超越了，写入日志。
	if(m_u8PacketTimeout < u8CommandCost)
	{
		//记录超时的命令
		_CommandTimeOut objCommandTimeOut;
		objCommandTimeOut.m_u2CommandID   = u2CommandID;
		objCommandTimeOut.m_u4TimeOutTime = (uint32)(u8CommandCost / 1000000);  //转换为毫秒
		m_vecCommandTimeOut.push_back(objCommandTimeOut);
		//AppLogManager::instance()->WriteLog(LOG_SYSTEM_PACKETTIME, "u2CommandID=%d, Timeout=[%d].", u2CommandID, (uint32)u8CommandCost);
	}

	//如果统计开关打开，才开始记录统计信息
	if(m_u1CommandAccount == 0)
	{
		return true;
	}

	//查找并添加
	mapCommandDataList::iterator f = m_mapCommandDataList.find(u2CommandID);
	if(f != m_mapCommandDataList.end())
	{
		//如果已经存在，则直接添加
		_CommandData* pCommandData = (_CommandData* )f->second;
		if(pCommandData != NULL)
		{
			pCommandData->m_u4CommandCount++;
			pCommandData->m_u8CommandCost += u8CommandCost;
			pCommandData->m_u1PacketType  = u1PacketType;
			pCommandData->m_u4PacketSize  += u4PacketSize;
			pCommandData->m_u4CommandSize += u4CommandSize;
			pCommandData->m_tvCommandTime = tvTime;
		}
		else
		{
			return false;
		}
	}
	else
	{
		//添加新的命令统计信息
		_CommandData* pCommandData =  new _CommandData();
		if(pCommandData != NULL)
		{
			pCommandData->m_u2CommandID   = u2CommandID;
			pCommandData->m_u1CommandType = u1CommandType;
			pCommandData->m_u8CommandCost = u8CommandCost;
			pCommandData->m_u1PacketType  = u1PacketType;
			pCommandData->m_u4PacketSize  += u4PacketSize;
			pCommandData->m_u4CommandSize += u4CommandSize;
			pCommandData->m_tvCommandTime = tvTime;

			m_mapCommandDataList.insert(mapCommandDataList::value_type(u2CommandID, pCommandData));
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool CCommandAccount::SaveCommandDataLog()
{
	//如果统计开关打开，才开始记录统计信息
	if(m_u1CommandAccount == 0)
	{
		return true;
	}

	//AppLogManager::instance()->WriteLog(LOG_SYSTEM_COMMANDDATA, "<Command Data Account>");
	for(mapCommandDataList::iterator itorFreeB = m_mapCommandDataList.begin(); itorFreeB != m_mapCommandDataList.end(); itorFreeB++)
	{
		_CommandData* pCommandData = (_CommandData* )itorFreeB->second;
		if(pCommandData != NULL)
		{
			ACE_Date_Time dtLastTime(pCommandData->m_tvCommandTime);
			ACE_TString   strCommandType;
			ACE_TString   strPacketType;

			if(pCommandData->m_u1CommandType == COMMAND_TYPE_IN)
			{
				strCommandType = "Server In Data";
			}
			else
			{
				strCommandType = "Server Out Data";
			}

			if(pCommandData->m_u1PacketType == PACKET_TCP)
			{
				strPacketType = "TCP";
			}
			else
			{
				strPacketType = "UDP";
			}

		}
	}

	return true;
}

void CCommandAccount::ClearTimeOut()
{
	m_vecCommandTimeOut.clear();
}

uint32 CCommandAccount::GetTimeoutCount()
{
	return (uint32)m_vecCommandTimeOut.size();
}

_CommandTimeOut* CCommandAccount::GetTimeoutInfo(uint32 u4Index)
{
	if(u4Index >= m_vecCommandTimeOut.size())
	{
		return NULL;
	}

	return (_CommandTimeOut* )&m_vecCommandTimeOut[u4Index];
}

