#ifndef _USERINFOMANAGER_H
#define _USERINFOMANAGER_H

#include "CacheManager.h"
#include <string>
#include <iostream>

//用户属性
//add by freeeyes
#ifdef WIN32
#define SHM_USERINFO_KEY (key_t) "67892"
#define PRINTF printf_s
#else
#define SHM_USERINFO_KEY (key_t) 67892
#define PRINTF printf
#endif

#define MAX_LOGIN_VALID_COUNT  10
#define SOURCE_FILE_INFO_PATH "./LoginBufferFile/UserLogin.txt"

//这里如果是想用缓冲功能，必须继承_CacheBlock对象
struct _UserInfo : public _CacheBlock
{
    //缓冲的数据结构
    uint32 m_u4UserID;      //用户ID
    uint32 m_u4Life;        //life
    uint32 m_u4Magic;       //magic

    _UserInfo()
    {
        m_u4UserID = 0;
        m_u4Life   = 0;
        m_u4Magic  = 0;
    }
};

typedef vector<_UserInfo*> vecUserInfo;          //空闲未使用的vecValid块
typedef map<uint32, _UserInfo*> mapUserInfo;     //已使用的vecValid

class CUserInfoManager : public CCacheManager
{
public:
    CUserInfoManager();
    ~CUserInfoManager();

    void Close();
    void Display();

    bool Init(uint32 u4CachedCount, key_t objMemorykey, uint32 u4CheckSize);

    _UserInfo* GetUserInfo(uint32 u4UserID);            //根据名称得到用户状态
    void Sync_Memory_To_DataReaource();                 //同步文件和共享内存
    void GetFreeUserInfo();                             //从空闲数据块中寻找已经新加载的数据

    bool Load_From_DataResouce(uint32 u4SeachUserID, uint32& u4CacheIndex);    //从文件里面寻找指定的用户名

    bool Reload_Map_CacheMemory(uint32 u4CacheIndex);     //更新指定的cached位置的数据到map映射表

private:
    bool Read_All_Init_DataResoure();         //从文件加载共享内存，并加载列表
    bool Read_All_From_CacheMemory();         //从共享内存加载列表
    void Begin_Sync_DataReaource_To_Memory(); //开始同步
    void End_Sync_DataReaource_To_Memory();   //结束同步
    bool GetFileInfo(const char* pLine, uint32& u4UserID, uint32& u4Life, uint32& u4Magic); //从文件同步
    bool WriteFileInfo(FILE* pFile, _UserInfo* pUserInfo);

private:
    mapUserInfo             m_mapUserInfo;
    vecUserInfo             m_vecFreeUserInfo;
    CCachedLRUList<uint32>  m_objLRU;
};


#endif
