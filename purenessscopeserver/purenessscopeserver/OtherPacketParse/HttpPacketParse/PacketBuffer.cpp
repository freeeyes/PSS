#include "PacketBuffer.h"

CPacketBufferManager::CPacketBufferManager()
{

}

CPacketBufferManager::~CPacketBufferManager()
{
	Close();
}

void CPacketBufferManager::Close()
{
	/*
	for(mapBuffter::iterator b = m_mapBuffter.begin(); b != m_mapBuffter.end(); b++)
	{
		CBuffPacket* pBufferPacket = (CBuffPacket* )b->second;
		SAFE_DELETE(pBufferPacket);
	}
	m_mapBuffter.clear();
	*/
}


void CPacketBufferManager::AddBuffer(uint32 u4ConnectID)
{
	/*
	mapBuffter::iterator f = m_mapBuffter.find(u4ConnectID);
	if(f != m_mapBuffter.end())
	{
		return;
	}
	else
	{
		CBuffPacket* pBufferPacket = new CBuffPacket();
		m_mapBuffter.insert(mapBuffter::value_type(u4ConnectID, pBufferPacket));
	}
	*/
}

void CPacketBufferManager::DelBuffer(uint32 u4ConnectID)
{
	/*
	mapBuffter::iterator f = m_mapBuffter.find(u4ConnectID);
	if(f != m_mapBuffter.end())
	{
		CBuffPacket* pBufferPacket = (CBuffPacket* )f->second;
		SAFE_DELETE(pBufferPacket);
		m_mapBuffter.erase(f);
	}
	*/
}

CBuffPacket* CPacketBufferManager::GetBuffPacket(uint32 u4ConnectID)
{
	/*
	mapBuffter::iterator f = m_mapBuffter.find(u4ConnectID);
	if(f != m_mapBuffter.end())
	{
		return (CBuffPacket* )f->second;
	}
	else
	{
		return NULL;
	}
	*/
	return NULL;
}
