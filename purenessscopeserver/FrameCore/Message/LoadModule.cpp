// ����ʵ��ģ�����
// һ��������ɾ���Ϊ�࣬����ʵ��Ŀ�ꡣ
// add by freeeyes
// 2009-02-20

#include "LoadModule.h"

void CLoadModule::Init(uint16 u2MaxModuleCount)
{
    Close();

    m_u2MaxModuleCount = u2MaxModuleCount;
}

void CLoadModule::Close()
{
    //�رյ�ǰ��Ծģ��
    for_each(m_objHashModuleList.begin(), m_objHashModuleList.end(), [](const std::pair<string, shared_ptr<_ModuleInfo>>& iter) {
        //�ر�ģ��ӿ�
        iter.second->UnLoadModuleData();

        //���ģ���������������
        CLoadLibrary::PSS_dlClose(iter.second->hModule);
        });

    m_objHashModuleList.clear();
}

bool CLoadModule::LoadModule(const char* pModulePath, const char* pModuleName, const char* pModuleParam)
{
    string strModuleName = (string)pModuleName;

    auto pModuleInfo = std::make_shared<_ModuleInfo>();

    if (nullptr == pModuleInfo)
    {
        PSS_LOGGER_DEBUG("[CLoadModule::LoadMoudle] new _ModuleInfo is error!");
        return false;
    }

    //��¼ģ�����
    pModuleInfo->strModuleParam = (string)pModuleParam;

    //��ʼע��ģ�麯��
    if (false == LoadModuleInfo(strModuleName, pModuleInfo, pModulePath))
    {
        return false;
    }

    //���Ҵ�ģ���Ƿ��Ѿ���ע�ᣬ�������Ϣ����Ϣ����
    auto f = m_objHashModuleList.find(pModuleInfo->GetName());

    if (m_objHashModuleList.end() != f)
    {
        //ж�ؾɵĲ��
        CLoadLibrary::PSS_dlClose(f->second->hModule);
        m_objHashModuleList.erase(f);
    }

    //��ʼ����ģ���ʼ������
    int nRet = pModuleInfo->LoadModuleData(App_ServerObject::instance());

    if (nRet != 0)
    {
        PSS_LOGGER_DEBUG("[CLoadModule::LoadMoudle] strModuleName = {0}, Execute Function LoadModuleData is error!", strModuleName);
        return false;
    }

    //��ע��ɹ���ģ�飬���뵽Hash������
    m_objHashModuleList[pModuleInfo->GetName()] = pModuleInfo;

    PSS_LOGGER_DEBUG("[CLoadModule::LoadMoudle] Begin Load ModuleName[{0}] OK!", pModuleInfo->GetName());
    return true;
}

bool CLoadModule::UnLoadModule(const char* szModuleName, bool blIsDelete)
{
    string strModuleName = szModuleName;
    PSS_LOGGER_DEBUG("[CLoadModule::UnLoadModule]szResourceName={0}.", szModuleName);
    auto f = m_objHashModuleList.find(strModuleName);

    if (m_objHashModuleList.end() == f)
    {
        return false;
    }
    else
    {
        //����ʹ˹��������ж���
        auto pModuleInfo = f->second;
        pModuleInfo->UnLoadModuleData();

        //���ģ���������������
        CLoadLibrary::PSS_dlClose(pModuleInfo->hModule);

        if (true == blIsDelete)
        {
            m_objHashModuleList.erase(f);
        }

        PSS_LOGGER_DEBUG("[CLoadModule::UnLoadModule] Close Module={0}!", strModuleName);

        return true;
    }
}

bool CLoadModule::MoveUnloadList(const char* szModuleName, uint32 u4UpdateIndex, uint32 u4ThreadCount, uint8 u1UnLoadState, const string& strModulePath, const string& strModuleName, const string& strModuleParam)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_tmModule);
    PSS_LOGGER_DEBUG("[CLoadModule::MoveUnloadList]szResourceName={0}.", szModuleName);
    auto f = m_objHashModuleList.find(szModuleName);

    if (m_objHashModuleList.end() == f)
    {
        return false;
    }
    else
    {
        auto pModuleInfo = f->second;

        //����ȴ�������߳��б�
        CWaitUnLoadModule objWaitUnloadModule;
        objWaitUnloadModule.m_u4UpdateIndex = u4UpdateIndex;
        objWaitUnloadModule.m_hModule = pModuleInfo->hModule;
        objWaitUnloadModule.UnLoadModuleData = pModuleInfo->UnLoadModuleData;
        objWaitUnloadModule.m_u4ThreadCurrEndCount = u4ThreadCount;
        objWaitUnloadModule.m_u1UnloadState = u1UnLoadState;
        objWaitUnloadModule.m_strModuleName = strModuleName;
        objWaitUnloadModule.m_strModulePath = strModulePath;
        objWaitUnloadModule.m_strModuleParam = strModuleParam;
        m_veCWaitUnLoadModule.push_back(objWaitUnloadModule);

        //ɾ������m_objHashModuleList�Ĳ����Ϣ
        m_objHashModuleList.erase(f);
        PSS_LOGGER_DEBUG("[CLoadModule::MoveUnloadList]szResourceName={0} Move Finish.", szModuleName);
        return true;
    }
}

int CLoadModule::UnloadListUpdate(uint32 u4UpdateIndex)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_tmModule);
    int nRet = 0;
    vector<CWaitUnLoadModule>::iterator itr = m_veCWaitUnLoadModule.begin();

    while (itr != m_veCWaitUnLoadModule.end())
    {
        if ((*itr).m_u4UpdateIndex != u4UpdateIndex)
        {
            ++itr;
            continue;
        }

        if ((*itr).m_u4UpdateIndex > 0)
        {
            (*itr).m_u4UpdateIndex--;
        }

        if ((*itr).m_u4UpdateIndex == 0)
        {
            //�ص��߼������UnLoadModuleData����
            (*itr).UnLoadModuleData();

            //���ղ���˿���Դ
            PSS_LOGGER_DEBUG("[CLoadModule::UnloadListUpdate]szResourceName=%s UnLoad.", (*itr).m_strModuleName);
            CLoadLibrary::PSS_dlClose((*itr).m_hModule);

            //�ж��Ƿ���Ҫ���ز��
            if (2 == (*itr).m_u1UnloadState)
            {
                //���¼���
                LoadModule((*itr).m_strModulePath.c_str(), (*itr).m_strModuleName.c_str(), (*itr).m_strModuleParam.c_str());

                //���¼��������¼�
                InitModule((*itr).m_strModuleName.c_str());
                nRet = 1;
            }

            //����vector�е��������
            m_veCWaitUnLoadModule.erase(itr);
            return nRet;
        }
    }

    return nRet;
}

int CLoadModule::GetCurrModuleCount() const
{
    return (int)m_objHashModuleList.size();
}

int CLoadModule::GetModulePoolCount() const
{
    return m_u2MaxModuleCount;
}

shared_ptr<_ModuleInfo> CLoadModule::GetModuleInfo(const char* pModuleName)
{
    auto f = m_objHashModuleList.find(pModuleName);

    if (m_objHashModuleList.end() != f)
    {
        return f->second;
    }
    else
    {
        return nullptr;
    }
}

bool CLoadModule::InitModule()
{
    for_each(m_objHashModuleList.begin(), m_objHashModuleList.end(), [](const std::pair<string, shared_ptr<_ModuleInfo>>& iter) {
        //ִ�����еĲ�����ݽ���ǰ��׼��
        iter.second->InitModule(App_ServerObject::instance());
        });

    return true;
}

bool CLoadModule::InitModule(const char* pModuleName)
{
    auto f = m_objHashModuleList.find(pModuleName);

    if (m_objHashModuleList.end() != f)
    {
        f->second->InitModule(App_ServerObject::instance());
    }

    return true;
}

bool CLoadModule::LoadModuleInfo(string strModuleName, shared_ptr<_ModuleInfo> pModuleInfo, const char* pModulePath)
{
    string strModuleFile;

    if (nullptr == pModuleInfo)
    {
        PSS_LOGGER_DEBUG("[CLoadModule::LoadModuleInfo] strModuleName = {0}, pModuleInfo is nullptr!", strModuleName);
        return false;
    }

    pModuleInfo->strModulePath = (string)pModulePath;

    strModuleFile = fmt::format("{0}{1}", pModulePath, strModuleName);

    m_tmModule.acquire();

    pModuleInfo->hModule = CLoadLibrary::PSS_dlopen(strModuleFile.c_str(), RTLD_NOW);

    if (nullptr == pModuleInfo->hModule || !pModuleInfo->hModule)
    {
        PSS_LOGGER_DEBUG("[CLoadModule::LoadModuleInfo] strModuleName = {0}, pModuleInfo->hModule is nullptr({1})!", strModuleName, CLoadLibrary::PSS_dlerror());
        m_tmModule.release();
        return false;
    }

    pModuleInfo->LoadModuleData = (int(*)(CServerObject*))CLoadLibrary::PSS_dlsym(pModuleInfo->hModule, "LoadModuleData");

    if (nullptr == pModuleInfo->LoadModuleData)
    {
        PSS_LOGGER_DEBUG("[CLoadModule::LoadModuleInfo] strModuleName = {0}, Function LoadMoudle is error!", strModuleName);
        m_tmModule.release();
        return false;
    }

    pModuleInfo->UnLoadModuleData = (int(*)())CLoadLibrary::PSS_dlsym(pModuleInfo->hModule, "UnLoadModuleData");

    if (nullptr == pModuleInfo->UnLoadModuleData)
    {
        PSS_LOGGER_DEBUG("[CLoadModule::LoadModuleInfo] strModuleName = {0}, Function UnloadModule is error!", strModuleName);
        m_tmModule.release();
        return false;
    }

    pModuleInfo->GetDesc = (const char* (*)())CLoadLibrary::PSS_dlsym(pModuleInfo->hModule, "GetDesc");

    if (nullptr == pModuleInfo->GetDesc)
    {
        PSS_LOGGER_DEBUG("[CLoadModule::LoadModuleInfo] strModuleName = {0}, Function GetDesc is error!", strModuleName);
        m_tmModule.release();
        return false;
    }

    pModuleInfo->GetName = (const char* (*)())CLoadLibrary::PSS_dlsym(pModuleInfo->hModule, "GetName");

    if (nullptr == pModuleInfo->GetName)
    {
        PSS_LOGGER_DEBUG("[CLoadModule::LoadModuleInfo] strModuleName = {0}, Function GetName is error!", strModuleName);
        m_tmModule.release();
        return false;
    }

    pModuleInfo->GetModuleKey = (const char* (*)())CLoadLibrary::PSS_dlsym(pModuleInfo->hModule, "GetModuleKey");

    if (nullptr == pModuleInfo->GetModuleKey)
    {
        PSS_LOGGER_DEBUG("[CLoadModule::LoadModuleInfo] strModuleName = {0}, Function GetModuleKey is error!", strModuleName);
        m_tmModule.release();
        return false;
    }

    pModuleInfo->DoModuleMessage = (int(*)(uint16, shared_ptr<IBuffPacket>, shared_ptr<IBuffPacket>))CLoadLibrary::PSS_dlsym(pModuleInfo->hModule, "DoModuleMessage");

    if (nullptr == pModuleInfo->DoModuleMessage)
    {
        PSS_LOGGER_DEBUG("[CLoadModule::LoadModuleInfo] strModuleName = {0}, Function DoModuleMessage is error({1})!", strModuleName, errno);
        m_tmModule.release();
        return false;
    }

    pModuleInfo->GetModuleState = (bool(*)(uint32&))CLoadLibrary::PSS_dlsym(pModuleInfo->hModule, "GetModuleState");

    if (nullptr == pModuleInfo->GetModuleState)
    {
        PSS_LOGGER_DEBUG("[CLoadModule::LoadModuleInfo] strModuleName = {0}, Function GetModuleState is error({1})!", strModuleName, errno);
        m_tmModule.release();
        return false;
    }

    //�������Ϊ�գ���Ϊ�յ�ʱ���ʼ��������
    pModuleInfo->InitModule = (int(*)(CServerObject*))CLoadLibrary::PSS_dlsym(pModuleInfo->hModule, "InitModule");

    pModuleInfo->strModuleName = strModuleName;
    m_tmModule.release();
    return true;
}

int CLoadModule::SendModuleMessage(const char* pModuleName, uint16 u2CommandID, shared_ptr<IBuffPacket> pBuffPacket, shared_ptr<IBuffPacket> pReturnBuffPacket)
{
    auto f = m_objHashModuleList.find(pModuleName);

    if (m_objHashModuleList.end() != f)
    {
        f->second->DoModuleMessage(u2CommandID, pBuffPacket, pReturnBuffPacket);
    }

    return 0;
}

bool CLoadModule::GetModuleExist(const char* pModuleName)
{
    auto f = m_objHashModuleList.find(pModuleName);

    if (m_objHashModuleList.end() != f)
    {
        return true;
    }
    else
    {
        return false;
    }
}

const char* CLoadModule::GetModuleParam(const char* pModuleName)
{
    auto f = m_objHashModuleList.find(pModuleName);

    if (m_objHashModuleList.end() != f)
    {
        return f->second->strModuleParam.c_str();
    }
    else
    {
        return nullptr;
    }
}

const char* CLoadModule::GetModuleFileName(const char* pModuleName)
{
    auto f = m_objHashModuleList.find(pModuleName);

    if (m_objHashModuleList.end() != f)
    {
        return f->second->strModuleName.c_str();
    }
    else
    {
        return nullptr;
    }
}

const char* CLoadModule::GetModuleFilePath(const char* pModuleName)
{
    auto f = m_objHashModuleList.find(pModuleName);

    if (m_objHashModuleList.end() != f)
    {
        return f->second->strModulePath.c_str();
    }
    else
    {
        return nullptr;
    }
}

const char* CLoadModule::GetModuleFileDesc(const char* pModuleName)
{
    auto f = m_objHashModuleList.find(pModuleName);

    if (m_objHashModuleList.end() != f)
    {
        return f->second->GetDesc();
    }
    else
    {
        return nullptr;
    }
}

uint16 CLoadModule::GetModuleCount()
{
    return (uint16)m_objHashModuleList.size();
}

void CLoadModule::GetAllModuleInfo(vector<shared_ptr<_ModuleInfo>>& vecModeInfo)
{
    vecModeInfo.clear();
    m_vecModuleNameList.clear();

    for_each(m_objHashModuleList.begin(), m_objHashModuleList.end(), [&vecModeInfo, this](const std::pair<string, shared_ptr<_ModuleInfo>>& iter) {
        //���Է�ָ�����
        vecModeInfo.emplace_back(iter.second);
        m_vecModuleNameList.emplace_back(iter.second->GetName());
        });
}

bool CLoadModule::GetAllModuleName(uint32 u4Index, char* pName, uint16 nLen)
{
    if (u4Index >= m_vecModuleNameList.size())
    {
        return false;
    }

    if (nLen <= m_vecModuleNameList[u4Index].length())
    {
        PSS_LOGGER_DEBUG("[CLoadModule::GetAllModuleName]pName len({0}) is more than({1}).",
            m_vecModuleNameList[u4Index].length(),
            nLen);
        return false;
    }

    sprintf_safe(pName, nLen, "%s", m_vecModuleNameList[u4Index].c_str());
    return true;
}

