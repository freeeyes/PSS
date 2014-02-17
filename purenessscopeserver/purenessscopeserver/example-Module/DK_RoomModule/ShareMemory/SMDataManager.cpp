#include "SMDataManager.h"

CSMDataManager::CSMDataManager(void)
{
}

CSMDataManager::~CSMDataManager(void)
{
	
}

bool CSMDataManager::Init()
{
#ifdef _WIN32	
	m_objKey = 100020;
#else
	m_objKey = ftok("/home/freeeyes/SMData/", 0);
#endif

	bool blState = m_objUserPlayerPool.Init(m_objKey, PLAYER_SMPOOL_COUNT);
	return blState;
}

_UserPlayer* CSMDataManager::GetUserPlayer(uint32 u4Pos)
{
	return m_objUserPlayerPool.GetPosToData(u4Pos);
}


