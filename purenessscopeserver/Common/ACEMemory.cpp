// ACEMemory.h
// 做了而一个内存缓冲池，后来发现ACE本身支持内存缓冲池。
// 我的缓冲池因为在ACE DLL的外面，在ACE内部重载我的内存池不会调用我外面的缓冲池。
// 用了两周时间，才找到我的内存池冲突的本质原因。
// 要想让ACE支持我的内存池，就得改ACE的源码，将我的内存池添入DLL编译。还是先用它的吧。
// add by freeeyes
// 2009-08-03

#include "ACEMemory.h"

CACEMemory::CACEMemory(void)
{
    try
    {
        OUR_DEBUG((LM_ERROR, "[CACEMemory::~CACEMemory].\n"));
    }
    catch (...)
    {
        OUR_DEBUG((LM_ERROR, "[CACEMemory::~CACEMemory]. New Object Error\n"));
    }
}

CACEMemory::~CACEMemory(void)
{
    OUR_DEBUG((LM_ERROR, "[CACEMemory::~CACEMemory]End.\n"));
}

void* CACEMemory::malloc(size_t stSize)
{
    return new char[stSize];
}

bool CACEMemory::free(void* p)
{
    delete (char*)(p);
    return true;
}
