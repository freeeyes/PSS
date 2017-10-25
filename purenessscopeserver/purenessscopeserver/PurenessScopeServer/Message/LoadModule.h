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
    uint32           m_u4UpdateIndex;
    uint32           m_u4ThreadCurrEndCount;
    char             m_szModuleName[MAX_BUFF_100];
    ACE_SHLIB_HANDLE m_hModule;

    _WaitUnloadModule()
    {
        m_u4ThreadCurrEndCount = 0;
        m_u4UpdateIndex        = 0;
        m_szModuleName[0]      = '\0';
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
    bool MoveUnloadList(const char* szModuleName, uint32 u4UpdateIndex, uint32 u4ThreadCount);   //将要卸载的插件放入缓冲列表
    void UnloadListUpdate(uint32 u4UpdateIndex);                                                 //工程线程回调接口，当所有工作线程回调结束，释放插件端口

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
