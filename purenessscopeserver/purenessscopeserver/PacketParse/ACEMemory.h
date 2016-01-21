#ifndef _ACEMEMORY_H
#define _ACEMEMORY_H

#include "define.h"

typedef ACE_Malloc<ACE_LOCAL_MEMORY_POOL, ACE_SYNCH_MUTEX> PACKET_DATA_MALLOC;
typedef ACE_Allocator_Adapter<PACKET_DATA_MALLOC> PACKET_DATA_ALLOCTOR;

class CACEMemory
{
public:
	CACEMemory(void);
	~CACEMemory(void);

	void* malloc(size_t stSize);
	bool  free(void* p);

private:
	static PACKET_DATA_ALLOCTOR*  m_pPacketDataAlloctor;
};

typedef ACE_Singleton<CACEMemory, ACE_Null_Mutex> App_ACEMemory;
#endif
