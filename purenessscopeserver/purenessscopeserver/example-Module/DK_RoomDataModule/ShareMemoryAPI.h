#ifndef _SHAREMEMORYAPI_H
#define _SHAREMEMORYAPI_H

//创建共享内存区API，提供跨平台支持
#ifdef  _WIN32
	#include <stdio.h>
	#include <Windows.h>
#endif

#ifdef  _LINUX
  #include <stdio.h>
	#include <sys/types.h> 
	#include <sys/ipc.h> 
	#include <sys/shm.h>
#endif

#ifdef _WIN32
	#define SM_INVALID_HANDLE NULL 
#endif
	
#ifdef  _LINUX
  #define SM_INVALID_HANDLE -1 
#endif

#define MAX_SHAREMEMORY_64  64

//如果是windows，定义对象
#ifdef _WIN32
	#define INIT_HANDLE 0xFFFFFFFFFFFFFFFF
    typedef HANDLE SMHandle;
	typedef int    SMKey;
#endif

//如果是linux，定义对象
#ifdef  _LINUX
	typedef int   SMHandle;
	typedef key_t SMKey;	
#endif

namespace ShareMemoryAPI
{
	//创建一个ShareMemory对象指针
	SMHandle CreateShareMemory(SMKey Key, int nSize);

	//打开一个已经存在的ShareMemory对象指针
	SMHandle OpenShareMemory(SMKey pKey, int nSize);

	//得到指定的ShareMemory对象指针映射
	char* MapShareMemory(SMHandle handle);

	//关闭指定的ShareMemory对象指针映射
	void UnMapShareMemory(char* pData);

	void CloseSharememory(SMHandle handle);
};

#endif
