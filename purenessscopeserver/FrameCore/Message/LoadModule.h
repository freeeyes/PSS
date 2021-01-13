#ifndef _LOADMODULE_H
#define _LOADMODULE_H

#include "ace/Date_Time.h"
#include "ace/Thread_Mutex.h"
#include "ace/Singleton.h"
#include "ace/OS_NS_dlfcn.h"
#include "IObject.h"

#include <string>
#include <vector>
#include <unordered_map>
#include "LoadLibrary.hpp"

class _ModuleInfo
{
public:
    string           strModuleName;         //模块文件名称
    string           strModulePath;         //模块路径
    string           strModuleParam;        //模块启动参数
    PSS_Time_Point   tvCreateTime = CTimeStamp::Get_Time_Stamp(); //模块创建时间
    Pss_Library_Handler hModule                         = nullptr;
    int (*LoadModuleData)(CServerObject* pServerObject) = nullptr;
    int (*InitModule)(CServerObject* pServerObject)     = nullptr;
    int (*UnLoadModuleData)(void)                       = nullptr;
    const char* (*GetDesc)(void)                        = nullptr;
    const char* (*GetName)(void)                        = nullptr;
    const char* (*GetModuleKey)(void)                   = nullptr;
    int (*DoModuleMessage)(uint16 u2CommandID, shared_ptr<IBuffPacket>, shared_ptr<IBuffPacket>) = nullptr;
    bool (*GetModuleState)(uint32& u4AErrorID)          = nullptr;
    void (*Set_output)(shared_ptr<spdlog::logger> logger) = nullptr;

    _ModuleInfo() = default;
};

class CWaitUnLoadModule
{
public:
    uint32              m_u4UpdateIndex              = 0;         //工作线程总数
    uint32              m_u4ThreadCurrEndCount       = 0;         //当前已经结束的工作线程个数
    uint8               m_u1UnloadState              = 0;         //重载状态，1为卸载，2为重载
    Pss_Library_Handler m_hModule                    = nullptr;   //插件的指针
    int (*UnLoadModuleData)(void)                 = nullptr;   //卸载插件的函数指针
    string              m_strModuleName;                          //模块文件名称
    string              m_strModulePath;                          //模块路径
    string              m_strModuleParam;                         //模块启动参数

    CWaitUnLoadModule() = default;
};

class CLoadModule : public IModuleInfo
{
public:
    CLoadModule(void) = default;

    void Init(uint16 u2MaxModuleCount);

    void Close();

    bool LoadModule(const char* pModulePath, const char* pModuleName, const char* pModuleParam);
    bool UnLoadModule(const char* szModuleName, bool blIsDelete = true);
    bool MoveUnloadList(const char* szModuleName, uint32 u4UpdateIndex, uint32 u4ThreadCount, uint8 u1UnLoadState,
                        const string& strModulePath, const string& strModuleName, const string& strModuleParam);                           //将要卸载的插件放入缓冲列表
    int  UnloadListUpdate(uint32 u4UpdateIndex);                                                                      //工程线程回调接口，当所有工作线程回调结束，释放插件端口，这里返回0则什么都不做，返回1则是需要重新加载副本

    int  SendModuleMessage(const char* pModuleName, uint16 u2CommandID, shared_ptr<IBuffPacket> pBuffPacket, shared_ptr<IBuffPacket> pReturnBuffPacket);

    int  GetCurrModuleCount() const;
    int  GetModulePoolCount() const;
    shared_ptr<_ModuleInfo> GetModuleInfo(const char* pModuleName);

    bool InitModule();                             //反应器启动完毕后，开始加载插件。
    bool InitModule(const char* pModuleName);      //插件热加载，当插件重新加载后，启动Init方法。

    //插件接口提供相关功能
    bool GetModuleExist(const char* pModuleName) final;
    const char* GetModuleParam(const char* pModuleName) final;
    const char* GetModuleFileName(const char* pModuleName) final;
    const char* GetModuleFilePath(const char* pModuleName) final;
    const char* GetModuleFileDesc(const char* pModuleName) final;
    uint16 GetModuleCount() final;
    void GetAllModuleInfo(vector<shared_ptr<_ModuleInfo>>& vecModeInfo);
    bool GetAllModuleName(uint32 u4Index, char* pName, uint16 nLen) final;

private:
    bool LoadModuleInfo(string strModuleName, shared_ptr<_ModuleInfo> pModuleInfo, const char* pModulePath);    //开始加载模块的接口和数据

    using hashmapModuleList = unordered_map<string, shared_ptr<_ModuleInfo>>;

    hashmapModuleList                  m_objHashModuleList;
    vector<CWaitUnLoadModule>          m_veCWaitUnLoadModule;
    ACE_Recursive_Thread_Mutex         m_tmModule;
    vector<string>                     m_vecModuleNameList;               //当前插件名称列表
    uint16                             m_u2MaxModuleCount = 0;            //最大模块加载数 
};

using App_ModuleLoader = PSS_singleton<CLoadModule>;
#endif
