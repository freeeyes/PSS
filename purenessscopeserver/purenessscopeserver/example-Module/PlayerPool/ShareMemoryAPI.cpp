#include "ShareMemoryAPI.h"

namespace ShareMemoryAPI
{

SMHandle CreateShareMemory(SMKey Key, int nSize)
{
#ifdef _WIN32
	char szKey[MAX_SHAREMEMORY_64] = {'\0'};
	sprintf(szKey, "%d", Key);
	return CreateFileMapping((HANDLE )INIT_HANDLE, NULL, PAGE_READWRITE, 0, nSize, szKey);
#endif

#ifdef _LINUX
	return shmget(Key, nSize, IPC_CREAT|IPC_EXCL|0666); 
#endif

	return SMHandle(-1);
}

SMHandle OpenShareMemory(SMKey Key, int nSize)
{
#ifdef _WIN32
	char szKey[MAX_SHAREMEMORY_64] = {'\0'};
	sprintf(szKey, "%d", Key);
    return OpenFileMapping(FILE_MAP_ALL_ACCESS, TRUE, szKey);
#endif

#ifdef _LINUX
	return shmget(Key, nSize, 0);
#endif

	return SMHandle(-1);
}

char* MapShareMemory(SMHandle handle)
{
	if(handle == SM_INVALID_HANDLE)
	{
		return NULL;
	}

#ifdef _WIN32
	return (char* )MapViewOfFile(handle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
#endif

#ifdef _LINUX
	return (char* )shmat(handle,0,0);
#endif

	return NULL;
}

void UnMapShareMemory(char* pData)
{
	if(pData == NULL)
	{
		return;
	}

#ifdef _WIN32
	UnmapViewOfFile(pData);
#endif

#ifdef _LINUX
	shmdt(pData);
#endif
};

void CloseSharememory(SMHandle handle)
{
#ifdef _WIN32
	CloseHandle(handle);
#endif

#ifdef _LINUX
	shmctl(handle, IPC_RMID, 0);
#endif
}

};

