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

class _ModuleInfo
{
public:
    string           strModuleName;         //ģ���ļ�����
    string           strModulePath;         //ģ��·��
    string           strModuleParam;        //ģ����������
    ACE_Date_Time    dtCreateTime;          //ģ�鴴��ʱ��
    ACE_SHLIB_HANDLE hModule                            = nullptr;
    int (*LoadModuleData)(CServerObject* pServerObject) = nullptr;
    int (*InitModule)(CServerObject* pServerObject)     = nullptr;
    int (*UnLoadModuleData)(void)                       = nullptr;
    const char* (*GetDesc)(void)                        = nullptr;
    const char* (*GetName)(void)                        = nullptr;
    const char* (*GetModuleKey)(void)                   = nullptr;
    int (*DoModuleMessage)(uint16 u2CommandID, shared_ptr<IBuffPacket>, shared_ptr<IBuffPacket>) = nullptr;
    bool (*GetModuleState)(uint32& u4AErrorID)          = nullptr;

    _ModuleInfo() = default;
};

class CWaitUnLoadModule
{
public:
    uint32           m_u4UpdateIndex              = 0;         //�����߳�����
    uint32           m_u4ThreadCurrEndCount       = 0;         //��ǰ�Ѿ������Ĺ����̸߳���
    uint8            m_u1UnloadState              = 0;         //����״̬��1Ϊж�أ�2Ϊ����
    ACE_SHLIB_HANDLE m_hModule                    = nullptr;   //�����ָ��
    int (*UnLoadModuleData)(void)                 = nullptr;   //ж�ز���ĺ���ָ��
    string           m_strModuleName;                          //ģ���ļ�����
    string           m_strModulePath;                          //ģ��·��
    string           m_strModuleParam;                         //ģ����������

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
                        const string& strModulePath, const string& strModuleName, const string& strModuleParam);                           //��Ҫж�صĲ�����뻺���б�
    int  UnloadListUpdate(uint32 u4UpdateIndex);                                                                      //�����̻߳ص��ӿڣ������й����̻߳ص��������ͷŲ���˿ڣ����ﷵ��0��ʲô������������1������Ҫ���¼��ظ���

    int  SendModuleMessage(const char* pModuleName, uint16 u2CommandID, shared_ptr<IBuffPacket> pBuffPacket, shared_ptr<IBuffPacket> pReturnBuffPacket);

    int  GetCurrModuleCount() const;
    int  GetModulePoolCount() const;
    shared_ptr<_ModuleInfo> GetModuleInfo(const char* pModuleName);

    bool InitModule();                             //��Ӧ��������Ϻ󣬿�ʼ���ز����
    bool InitModule(const char* pModuleName);      //����ȼ��أ���������¼��غ�����Init������

    //����ӿ��ṩ��ع���
    bool GetModuleExist(const char* pModuleName) final;
    const char* GetModuleParam(const char* pModuleName) final;
    const char* GetModuleFileName(const char* pModuleName) final;
    const char* GetModuleFilePath(const char* pModuleName) final;
    const char* GetModuleFileDesc(const char* pModuleName) final;
    uint16 GetModuleCount() final;
    void GetAllModuleInfo(vector<shared_ptr<_ModuleInfo>>& vecModeInfo);
    bool GetAllModuleName(uint32 u4Index, char* pName, uint16 nLen) final;

private:
    bool LoadModuleInfo(string strModuleName, shared_ptr<_ModuleInfo> pModuleInfo, const char* pModulePath);    //��ʼ����ģ��Ľӿں�����

    using hashmapModuleList = unordered_map<string, shared_ptr<_ModuleInfo>>;

    hashmapModuleList                  m_objHashModuleList;
    vector<CWaitUnLoadModule>          m_veCWaitUnLoadModule;
    ACE_Recursive_Thread_Mutex         m_tmModule;
    vector<string>                     m_vecModuleNameList;               //��ǰ��������б�
    uint16                             m_u2MaxModuleCount = 0;            //���ģ������� 
};

using App_ModuleLoader = ACE_Singleton<CLoadModule, ACE_Null_Mutex>;
#endif
