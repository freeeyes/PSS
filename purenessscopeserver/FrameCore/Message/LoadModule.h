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
    int (*DoModuleMessage)(uint16 u2CommandID, IBuffPacket* pBuffPacket, IBuffPacket* pReturnBuffPacket) = nullptr;
    bool (*GetModuleState)(uint32& u4AErrorID)          = nullptr;

    _ModuleInfo()
    {
    }
};

class CWaitUnLoadModule
{
public:
    uint32           m_u4UpdateIndex              = 0;         //�����߳�����
    uint32           m_u4ThreadCurrEndCount       = 0;         //��ǰ�Ѿ������Ĺ����̸߳���
    uint8            m_u1UnloadState              = 0;         //����״̬��1Ϊж�أ�2Ϊ����
    char             m_szModuleName[MAX_BUFF_100] = {'\0'};    //�������
    ACE_SHLIB_HANDLE m_hModule                    = nullptr;      //�����ָ��
    int (*UnLoadModuleData)(void)                 = nullptr;      //ж�ز���ĺ���ָ��
    string           m_strModuleName;                          //ģ���ļ�����
    string           m_strModulePath;                          //ģ��·��
    string           m_strModuleParam;                         //ģ����������

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
                        string strModulePath, string strModuleName, string strModuleParam);                           //��Ҫж�صĲ�����뻺���б�
    int  UnloadListUpdate(uint32 u4UpdateIndex);                                                                      //�����̻߳ص��ӿڣ������й����̻߳ص��������ͷŲ���˿ڣ����ﷵ��0��ʲô������������1������Ҫ���¼��ظ���

    int  SendModuleMessage(const char* pModuleName, uint16 u2CommandID, IBuffPacket* pBuffPacket, IBuffPacket* pReturnBuffPacket);

    int  GetCurrModuleCount();
    int  GetModulePoolCount();
    _ModuleInfo* GetModuleInfo(const char* pModuleName);

    bool InitModule();                             //��Ӧ��������Ϻ󣬿�ʼ���ز����
    bool InitModule(const char* pModuleName);      //����ȼ��أ���������¼��غ�����Init������

    //����ӿ��ṩ��ع���
    bool GetModuleExist(const char* pModuleName);
    const char* GetModuleParam(const char* pModuleName);
    const char* GetModuleFileName(const char* pModuleName);
    const char* GetModuleFilePath(const char* pModuleName);
    const char* GetModuleFileDesc(const char* pModuleName);
    uint16 GetModuleCount();
    void GetAllModuleInfo(vector<_ModuleInfo*>& vecModeInfo);
    bool GetAllModuleName(uint32 u4Index, char* pName, uint16 nLen);

private:
    bool LoadModuleInfo(string strModuleName, _ModuleInfo* pModuleInfo, const char* pModulePath);    //��ʼ����ģ��Ľӿں�����

    CHashTable<_ModuleInfo>            m_objHashModuleList;
    vector<CWaitUnLoadModule>          m_veCWaitUnLoadModule;
    ACE_Recursive_Thread_Mutex         m_tmModule;
    vector<string>                     m_vecModuleNameList;               //��ǰ��������б�
};

typedef ACE_Singleton<CLoadModule, ACE_Null_Mutex> App_ModuleLoader;
#endif
