#ifndef _COMMONDATA_H
#define _COMMONDATA_H

//#include "MemoryPools.h"

////ÖØÔØNewºÍDelete²Ù×÷·û
//inline void* operator new(size_t szBuff)
//{
//	void* pBuff = CMemoryPools::Instance().GetBuff(szBuff);
//	//OUR_DEBUG((LM_ERROR, "[New] Size = %d Address = [0x%08x].!\n", (int)szBuff, pBuff));
//	return pBuff;
//}
//
//inline void operator delete(void* p)
//{
//	if(false == CMemoryPools::Instance().DelBuff(p))
//	{
//		OUR_DEBUG((LM_ERROR, "[Delete]*p = [0x%08x] false!\n", p));
//		//CMemoryPools::Instance().DisplayMemoryList(p);
//	}
//	else
//	{
//		OUR_DEBUG((LM_ERROR, "[Delete]*p = [0x%08x] OK!\n", p));
//	}
//}

#endif
