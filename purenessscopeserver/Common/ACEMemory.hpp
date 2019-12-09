#ifndef _ACEMEMORY_H
#define _ACEMEMORY_H

#include "define.h"

//暂时不用ACE_LOCAL_MEMORY_POOL, 实测使用效率存在问题。
//用new 和 delete 替换之

class CACEMemory
{
public:
    CACEMemory(void)
    {
        OUR_DEBUG((LM_ERROR, "[CACEMemory::CACEMemory].\n"));
    };

    ~CACEMemory(void)
    {
        OUR_DEBUG((LM_ERROR, "[CACEMemory::~CACEMemory]End.\n"));
    };

    void* malloc(size_t stSize)
    {
        return new char[stSize];
    };

    bool  free(void* p)
    {
        delete (char*)(p);
        return true;
    };
};

typedef ACE_Singleton<CACEMemory, ACE_Null_Mutex> App_ACEMemory;
#endif
