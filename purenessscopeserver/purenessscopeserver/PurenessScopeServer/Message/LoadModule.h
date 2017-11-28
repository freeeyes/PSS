#ifndef _LOADMODULE_H
#define _LOADMODULE_H

#include "ace/Date_Time.h"
#include "ace/Thread_Mutex.h"
#include "ace/Singleton.h"
#include "ace/OS_NS_dlfcn.h"
#include "IObject.h"
#include "HashTable.h"

#include <string>
#include <vector>

using namespace std;

struct _ModuleInfo
{
    string           strModuleName;         //模块文件名称
    string           strModulePath;         //模块路径
    string           strModuleParam;        //模块启动参数
    ACE_Date_Time    dtCreateTime;          //模块创建时间
    ACE_SHLIB_HANDLE hModule;
    int (*LoadModuleData)(CServerObject* pServerObject);
    int (*UnLoadModuleData)(void);
    const char* (*GetDesc)(void);
    const char* (*GetName)(void);
    const char* (*GetModuleKey)(void);
    int (*DoModuleMessage)(uint16 u2CommandID, IBuffPacket* pBuffPacket, IBuffPacket* pReturnBuffPacket);
    bool (*GetModuleState)(uint32& u4AErrorID);

    _ModuleInfo()
    {
        hModule           = NULL;
        LoadModuleData    = NULL;
        UnLoadModuleData  = NULL;
        GetDesc           = NULL;
        GetName           = NULL;
        GetModuleKey      = NULL;
        DoModuleMessage   = NULL;
        GetModuleState    = NULL;
    }
};

struct _WaitUnloadModule
{
    uint32           m_u4UpdateIndex;                //工作线程总数
    uint32           m_u4ThreadCurrEndCount;         //当前已经结束的工作线程个数
    uint8            m_u1UnloadState;                //重载状态，1为卸载，2为重载
    char             m_szModuleName[MAX_BUFF_100];   //插件名称
    ACE_SHLIB_HANDLE m_hModule;                      //插件的指针
    int (*UnLoadModuleData)(void);                   //卸载插件的函数指针
    string           m_strModuleName;                //模块文件名称
    string           m_strModulePath;                //模块路径
    string           m_strModuleParam;               //模块启动参数

    _WaitUnloadModule()
    {
        m_hModule              = 0;
        m_u1UnloadState        = 0;
        m_u4ThreadCurrEndCount = 0;
        m_u4UpdateIndex        = 0;
        m_szModuleName[0]      = '\0';
        UnLoadModuleData       = NULL;
    }
};

class CLoadModule : public IModuleInfo
{
public:
    CLoadModule(void);
    virtual ~CLoadModule(void);

    void Init(uint16 u2MaxModuleCount);

    void Close();

    bool LoadModule(const char* pModulePath, const char* pModuleName, const char* pModuleParam);
    bool UnLoadModule(const char* szModuleName, bool blIsDelete = true);
    bool MoveUnloadList(const char* szModuleName, uint32 u4UpdateIndex, uint32 u4ThreadCount, uint8 u1UnLoadState,
                        string strModulePath, string strModuleName, string strModuleParam);                           //将要卸载的插件放入缓冲列表
    int  UnloadListUpdate(uint32 u4UpdateIndex);                                                                      //工程线程回调接口，当所有工作线程回调结束，释放插件端口，这里返回0则什么都不做，返回1则是需要重新加载副本

    int  SendModuleMessage(const char* pModuleName, uint16 u2CommandID, IBuffPacket* pBuffPacket, IBuffPacket* pReturnBuffPacket);

    int  GetCurrModuleCount();
    int  GetModulePoolCount();
    _ModuleInfo* GetModuleInfo(const char* pModuleName);

    //插件接口提供相关功能
    bool GetModuleExist(const char* pModuleName);
    const char* GetModuleParam(const char* pModuleName);
    const char* GetModuleFileName(const char* pModuleName);
    const char* GetModuleFilePath(const char* pModuleName);
    const char* GetModuleFileDesc(const char* pModuleName);
    uint16 GetModuleCount();
    void GetAllModuleInfo(vector<_ModuleInfo*>& vecModeInfo);
    void GetAllModuleName(vector<string> vecModuleName);

private:
    bool LoadModuleInfo(string strModuleName, _ModuleInfo* pModuleInfo, const char* pModulePath);    //开始加载模块的接口和数据

private:
    CHashTable<_ModuleInfo>            m_objHashModuleList;
    char                               m_szModulePath[MAX_BUFF_200];
    vector<_WaitUnloadModule>          m_vecWaitUnloadModule;
    ACE_Recursive_Thread_Mutex         m_tmModule;
};

typedef ACE_Singleton<CLoadModule, ACE_Null_Mutex> App_ModuleLoader;
#endif
