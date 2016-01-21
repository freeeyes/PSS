#ifndef _PACKETBUFFER_H
#define _PACKETBUFFER_H

#include "define.h"
#include "BuffPacket.h"

#include <map>

using namespace std;

class CPacketBufferManager
{
public:
	CPacketBufferManager();
	~CPacketBufferManager();

	void Close();

	void AddBuffer(uint32 u4ConnectID);
	void DelBuffer(uint32 u4ConnectID);
	CBuffPacket* GetBuffPacket(uint32 u4ConnectID);

private:
	typedef map<uint32, CBuffPacket*> mapBuffter;
	mapBuffter m_mapBuffter;
};

typedef ACE_Singleton<CPacketBufferManager, ACE_Null_Mutex> App_PacketBufferManager;

#endif
