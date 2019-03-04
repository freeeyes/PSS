#ifndef _USERVALIDMANAGER_H
#define _USERVALIDMANAGER_H

#include "CacheManager.h"
#include <string>
#include <iostream>

//设置共享内存key值，此key对应Valid数据
#ifdef WIN32
#define SHM_USERVALID_KEY (key_t) "67891"
#define PRINTF printf_s
#else
#define SHM_USERVALID_KEY (key_t) 67891
#define PRINTF printf
#endif

#define MAX_LOGIN_INFO_COUNT  10
#define SOURCE_FILE_PATH "./LoginBufferFile/UserValid.txt"

//这里如果是想用缓冲功能，必须继承_CacheBlock对象
struct _UserValid : public _CacheBlock
{
    //缓冲的数据结构
    char   m_szUserName[MAX_BUFF_50];   //用户名
    char   m_szUserPass[MAX_BUFF_50];   //密码
    uint32 m_u4LoginCount;              //登陆次数
};

typedef vector<_UserValid*> vecValid;              //空闲未使用的vecValid块
typedef map<string, _UserValid*> mapUserValid;     //已使用的vecValid

class CUserValidManager : public CCacheManager
{
public:
    CUserValidManager();
    ~CUserValidManager();

    void Close();
    void Display();

    bool Init(uint32 u4CachedCount, key_t objMemorykey, uint32 u4CheckSize);

    _UserValid* GetUserValid(const char* pUserName);      //根据名称得到用户状态
    void Sync_DataReaource_To_Memory();                   //同步文件和共享内存
    void GetFreeValid();                                  //从空闲数据块中寻找已经新加载的数据

    bool Load_From_DataResouce(const char* pUserName, uint32& u4CacheIndex);    //从文件里面寻找指定的用户名

    bool Reload_Map_CacheMemory(uint32 u4CacheIndex);     //更新指定的cached位置的数据到map映射表

private:
    bool Read_All_Init_DataResoure();         //从文件加载共享内存，并加载列表
    bool Read_All_From_CacheMemory();         //从共享内存加载列表
    void Begin_Sync_DataReaource_To_Memory(); //开始同步
    void End_Sync_DataReaource_To_Memory();   //结束同步
    bool GetFileInfo(const char* pLine, char* pUserName, char* pUserPass); //从文件同步

private:
    mapUserValid            m_mapUserValid;
    vecValid                m_vecFreeValid;
    CCachedLRUList<string>  m_objLRU;
};

#endif
