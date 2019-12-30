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

class _ModuleInfo
{
public:
    string           strModuleName;         //模块文件名称
    string           strModulePath;         //模块路径
    string           strModuleParam;        //模块启动参数
    ACE_Date_Time    dtCreateTime;          //模块创建时间
    ACE_SHLIB_HANDLE hModule                            = NULL;
    int (*LoadModuleData)(CServerObject* pServerObject) = NULL;
    int (*InitModule)(CServerObject* pServerObject)     = NULL;
    int (*UnLoadModuleData)(void)                       = NULL;
    const char* (*GetDesc)(void)                        = NULL;
    const char* (*GetName)(void)                        = NULL;
    const char* (*GetModuleKey)(void)                   = NULL;
    int (*DoModuleMessage)(uint16 u2CommandID, IBuffPacket* pBuffPacket, IBuffPacket* pReturnBuffPacket) = NULL;
    bool (*GetModuleState)(uint32& u4AErrorID)          = NULL;

    _ModuleInfo()
    {
    }
};

class CWaitUnLoadModule
{
public:
    uint32           m_u4UpdateIndex              = 0;         //工作线程总数
    uint32           m_u4ThreadCurrEndCount       = 0;         //当前已经结束的工作线程个数
    uint8            m_u1UnloadState              = 0;         //重载状态，1为卸载，2为重载
    char             m_szModuleName[MAX_BUFF_100] = {'\0'};    //插件名称
    ACE_SHLIB_HANDLE m_hModule                    = NULL;      //插件的指针
    int (*UnLoadModuleData)(void)                 = NULL;      //卸载插件的函数指针
    string           m_strModuleName;                          //模块文件名称
    string           m_strModulePath;                          //模块路径
    string           m_strModuleParam;                         //模块启动参数

    CWaitUnLoadModule()
    {
    }
};

class CLoadModule : public IModuleInfo
{
public:
    CLoadModule(void);

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

    bool InitModule();                             //反应器启动完毕后，开始加载插件。
    bool InitModule(const char* pModuleName);      //插件热加载，当插件重新加载后，启动Init方法。

    //插件接口提供相关功能
    bool GetModuleExist(const char* pModuleName);
    const char* GetModuleParam(const char* pModuleName);
    const char* GetModuleFileName(const char* pModuleName);
    const char* GetModuleFilePath(const char* pModuleName);
    const char* GetModuleFileDesc(const char* pModuleName);
    uint16 GetModuleCount();
    void GetAllModuleInfo(vector<_ModuleInfo*>& vecModeInfo);
    bool GetAllModuleName(uint32 u4Index, char* pName, uint16 nLen);

private:
    bool LoadModuleInfo(string strModuleName, _ModuleInfo* pModuleInfo, const char* pModulePath);    //开始加载模块的接口和数据

    CHashTable<_ModuleInfo>            m_objHashModuleList;
    char                               m_szModulePath[MAX_BUFF_200] = {'\0'};
    vector<CWaitUnLoadModule>          m_veCWaitUnLoadModule;
    ACE_Recursive_Thread_Mutex         m_tmModule;
    vector<string>                     m_vecModuleNameList;               //当前插件名称列表
};

typedef ACE_Singleton<CLoadModule, ACE_Null_Mutex> App_ModuleLoader;
#endif
