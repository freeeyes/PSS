// BuffPacket.h
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
        ACE_OS::dlclose(iter.second->hModule);
        });

    m_objHashModuleList.clear();
}

bool CLoadModule::LoadModule(const char* pModulePath, const char* pModuleName, const char* pModuleParam)
{
    string strModuleName = (string)pModuleName;

    auto pModuleInfo = std::make_shared<_ModuleInfo>();

    if(nullptr == pModuleInfo)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadModule::LoadMoudle] new _ModuleInfo is error!\n"));
        return false;
    }

    //��¼ģ�����
    pModuleInfo->strModuleParam = (string)pModuleParam;

    //��ʼע��ģ�麯��
    if(false == LoadModuleInfo(strModuleName, pModuleInfo, pModulePath))
    {
        return false;
    }

    //���Ҵ�ģ���Ƿ��Ѿ���ע�ᣬ�������Ϣ����Ϣ����
    auto f = m_objHashModuleList.find(pModuleInfo->GetName());

    if(m_objHashModuleList.end() != f)
    {
        //ж�ؾɵĲ��
        ACE_OS::dlclose(f->second->hModule);
        m_objHashModuleList.erase(f);
    }

    //��ʼ����ģ���ʼ������
    int nRet = pModuleInfo->LoadModuleData(App_ServerObject::instance());

    if(nRet != 0)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadModule::LoadMoudle] strModuleName = %s, Execute Function LoadModuleData is error!\n", strModuleName.c_str()));
        return false;
    }

    //��ע��ɹ���ģ�飬���뵽Hash������
    m_objHashModuleList[pModuleInfo->GetName()] = pModuleInfo;

    OUR_DEBUG((LM_ERROR, "[CLoadModule::LoadMoudle] Begin Load ModuleName[%s] OK!\n", pModuleInfo->GetName()));
    return true;
}

bool CLoadModule::UnLoadModule(const char* szModuleName, bool blIsDelete)
{
    string strModuleName = szModuleName;
    OUR_DEBUG((LM_ERROR, "[CLoadModule::UnLoadModule]szResourceName=%s.\n", szModuleName));
    auto f = m_objHashModuleList.find(strModuleName);

    if(m_objHashModuleList.end() == f)
    {
        return false;
    }
    else
    {
        //����ʹ˹��������ж���
        auto pModuleInfo = f->second;
        pModuleInfo->UnLoadModuleData();

        //���ģ���������������
        int nRet = ACE_OS::dlclose(pModuleInfo->hModule);

        if(true == blIsDelete)
        {
            m_objHashModuleList.erase(f);
        }

        OUR_DEBUG((LM_ERROR, "[CLoadModule::UnLoadModule] Close Module=%s, nRet=%d!\n", strModuleName.c_str(), nRet));

        return true;
    }
}

bool CLoadModule::MoveUnloadList(const char* szModuleName, uint32 u4UpdateIndex, uint32 u4ThreadCount, uint8 u1UnLoadState, const string& strModulePath, const string& strModuleName, const string& strModuleParam)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_tmModule);
    OUR_DEBUG((LM_ERROR, "[CLoadModule::MoveUnloadList]szResourceName=%s.\n", szModuleName));
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
        objWaitUnloadModule.m_u4UpdateIndex        = u4UpdateIndex;
        objWaitUnloadModule.m_hModule              = pModuleInfo->hModule;
        objWaitUnloadModule.UnLoadModuleData       = pModuleInfo->UnLoadModuleData;
        objWaitUnloadModule.m_u4ThreadCurrEndCount = u4ThreadCount;
        objWaitUnloadModule.m_u1UnloadState        = u1UnLoadState;
        objWaitUnloadModule.m_strModuleName        = strModuleName;
        objWaitUnloadModule.m_strModulePath        = strModulePath;
        objWaitUnloadModule.m_strModuleParam       = strModuleParam;
        m_veCWaitUnLoadModule.push_back(objWaitUnloadModule);

        //ɾ������m_objHashModuleList�Ĳ����Ϣ
        m_objHashModuleList.erase(f);
        OUR_DEBUG((LM_ERROR, "[CLoadModule::MoveUnloadList]szResourceName=%s Move Finish.\n", szModuleName));
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
            OUR_DEBUG((LM_ERROR, "[CLoadModule::UnloadListUpdate]szResourceName=%s UnLoad.\n", (*itr).m_strModuleName.c_str()));
            ACE_OS::dlclose((*itr).m_hModule);

            //�ж��Ƿ���Ҫ���ز��
            if(2 == (*itr).m_u1UnloadState)
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

    if(nullptr == pModuleInfo)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadModule::LoadModuleInfo] strModuleName = %s, pModuleInfo is nullptr!\n", strModuleName.c_str()));
        return false;
    }

    pModuleInfo->strModulePath = (string)pModulePath;

    strModuleFile = fmt::format("{0}{1}", pModulePath, strModuleName);

    m_tmModule.acquire();

    pModuleInfo->hModule = ACE_OS::dlopen(strModuleFile.c_str(), RTLD_NOW);

    if(nullptr == pModuleInfo->hModule || !pModuleInfo->hModule)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadModule::LoadModuleInfo] strModuleName = %s, pModuleInfo->hModule is nullptr(%s)!\n", strModuleName.c_str(), ACE_OS::dlerror()));
        m_tmModule.release();
        return false;
    }

    pModuleInfo->LoadModuleData = (int(*)(CServerObject*))ACE_OS::dlsym(pModuleInfo->hModule, "LoadModuleData");

    if(nullptr == pModuleInfo->LoadModuleData)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadModule::LoadModuleInfo] strModuleName = %s, Function LoadMoudle is error!\n", strModuleName.c_str()));
        m_tmModule.release();
        return false;
    }

    pModuleInfo->UnLoadModuleData = (int(*)())ACE_OS::dlsym(pModuleInfo->hModule, "UnLoadModuleData");

    if(nullptr == pModuleInfo->UnLoadModuleData)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadModule::LoadModuleInfo] strModuleName = %s, Function UnloadModule is error!\n", strModuleName.c_str()));
        m_tmModule.release();
        return false;
    }

    pModuleInfo->GetDesc = (const char* (*)())ACE_OS::dlsym(pModuleInfo->hModule, "GetDesc");

    if(nullptr == pModuleInfo->GetDesc)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadModule::LoadModuleInfo] strModuleName = %s, Function GetDesc is error!\n", strModuleName.c_str()));
        m_tmModule.release();
        return false;
    }

    pModuleInfo->GetName = (const char* (*)())ACE_OS::dlsym(pModuleInfo->hModule, "GetName");

    if(nullptr == pModuleInfo->GetName)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadModule::LoadModuleInfo] strModuleName = %s, Function GetName is error!\n", strModuleName.c_str()));
        m_tmModule.release();
        return false;
    }

    pModuleInfo->GetModuleKey = (const char* (*)())ACE_OS::dlsym(pModuleInfo->hModule, "GetModuleKey");

    if(nullptr == pModuleInfo->GetModuleKey)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadModule::LoadModuleInfo] strModuleName = %s, Function GetModuleKey is error!\n", strModuleName.c_str()));
        m_tmModule.release();
        return false;
    }

    pModuleInfo->DoModuleMessage = (int(*)(uint16, shared_ptr<IBuffPacket>, shared_ptr<IBuffPacket>))ACE_OS::dlsym(pModuleInfo->hModule, "DoModuleMessage");

    if(nullptr == pModuleInfo->DoModuleMessage)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadModule::LoadModuleInfo] strModuleName = %s, Function DoModuleMessage is error(%d)!\n", strModuleName.c_str(), errno));
        m_tmModule.release();
        return false;
    }

    pModuleInfo->GetModuleState = (bool(*)(uint32&))ACE_OS::dlsym(pModuleInfo->hModule, "GetModuleState");

    if(nullptr == pModuleInfo->GetModuleState)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadModule::LoadModuleInfo] strModuleName = %s, Function GetModuleState is error(%d)!\n", strModuleName.c_str(), errno));
        m_tmModule.release();
        return false;
    }

    //�������Ϊ�գ���Ϊ�յ�ʱ���ʼ��������
    pModuleInfo->InitModule = (int(*)(CServerObject*))ACE_OS::dlsym(pModuleInfo->hModule, "InitModule");

    pModuleInfo->strModuleName = strModuleName;
    m_tmModule.release();
    return true;
}

int CLoadModule::SendModuleMessage(const char* pModuleName, uint16 u2CommandID, shared_ptr<IBuffPacket> pBuffPacket, shared_ptr<IBuffPacket> pReturnBuffPacket)
{
    auto f = m_objHashModuleList.find(pModuleName);

    if(m_objHashModuleList.end() != f)
    {
        f->second->DoModuleMessage(u2CommandID, pBuffPacket, pReturnBuffPacket);
    }

    return 0;
}

bool CLoadModule::GetModuleExist(const char* pModuleName)
{
    auto f = m_objHashModuleList.find(pModuleName);

    if(m_objHashModuleList.end() != f)
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

    if(m_objHashModuleList.end() != f)
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
        OUR_DEBUG((LM_INFO, "[CLoadModule::GetAllModuleName]pName len(%d) is more than(%d).\n",
                   m_vecModuleNameList[u4Index].length(),
                   nLen));
        return false;
    }

    sprintf_safe(pName, nLen, "%s", m_vecModuleNameList[u4Index].c_str());
    return true;
}

