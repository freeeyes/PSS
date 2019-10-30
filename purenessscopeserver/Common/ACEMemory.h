#ifndef _ACEMEMORY_H
#define _ACEMEMORY_H

#include "define.h"

//暂时不用ACE_LOCAL_MEMORY_POOL, 实测使用效率存在问题。
//用new 和 delete 替换之

class CACEMemory
{
public:
    CACEMemory(void);
    ~CACEMemory(void);

    void* malloc(size_t stSize);
    bool  free(void* p);
};

typedef ACE_Singleton<CACEMemory, ACE_Null_Mutex> App_ACEMemory;
#endif
