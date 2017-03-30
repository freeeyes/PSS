#include "UserInfoManager.h"

CUserInfoManager::CUserInfoManager()
{

}

CUserInfoManager::~CUserInfoManager()
{

}

void CUserInfoManager::Close()
{
    m_mapUserInfo.clear();
    m_vecFreeUserInfo.clear();
}

void CUserInfoManager::Display()
{
    OUR_DEBUG((LM_INFO, "[CUserInfoManager::Display]m_mapUserInfo count=%d, m_vecFreeUserInfo=%d.\n", m_mapUserInfo.size(), m_vecFreeUserInfo.size()));

    for(mapUserInfo::iterator b = m_mapUserInfo.begin(); b!= m_mapUserInfo.end(); b++)
    {
        _UserInfo* pUserInfo = (_UserInfo* )b->second;

        OUR_DEBUG((LM_INFO, "[CUserValidManager::Display]UserID=%d, life=%d, magic=%d.\n", pUserInfo->m_u4UserID, pUserInfo->m_u4Life, pUserInfo->m_u4Magic));
    }
}

bool CUserInfoManager::Init( uint32 u4CachedCount, key_t objMemorykey, uint32 u4CheckSize )
{
    //设置Lru对象上限
    m_objLRU.Set_Lru_Max_Count(u4CachedCount);

    //调用父类方法
    return CCacheManager::Init(u4CachedCount, objMemorykey, u4CheckSize);
}

_UserInfo* CUserInfoManager::GetUserInfo( uint32 u4UserID )
{
    mapUserInfo::iterator f = m_mapUserInfo.find(u4UserID);

    if(f == m_mapUserInfo.end())
    {
        return NULL;
    }
    else
    {
        uint32 u4UserID = (uint32)f->first;
        _UserInfo* pUserInfo = (_UserInfo* )f->second;

        if(NULL != pUserInfo)
        {
            if(u4UserID != (uint32)pUserInfo->m_u4UserID || pUserInfo->GetDelete() == true)
            {
                //说明当前数据已经无效了，从当前map里面清除
                m_mapUserInfo.erase(f);
                m_vecFreeUserInfo.push_back(pUserInfo);

                //删除Lru相关内容
                m_objLRU.Delete_Cached_Lru(u4UserID);

                //如果新数据不是删除的数据，重新添加新加载的数据
                if(pUserInfo->GetDelete() == false)
                {
                    m_mapUserInfo.insert(mapUserInfo::value_type((uint32)pUserInfo->m_u4UserID, pUserInfo));
                }

                return NULL;
            }
            else
            {
                //添加命中
                m_objLRU.Add_Cached_Lru(u4UserID, pUserInfo->GetCacheIndex());

                return pUserInfo;
            }
        }
        else
        {
            return NULL;
        }
    }
}

void CUserInfoManager::Sync_Memory_To_DataReaource()
{
    //删除数据源
    remove(SOURCE_FILE_INFO_PATH);

    //数据同步到文件
    FILE* pFile = fopen((char* )SOURCE_FILE_INFO_PATH, "ab+");

    if(pFile == NULL)
    {
        //文件不存在
        return;
    }

    for(mapUserInfo::iterator b = m_mapUserInfo.begin(); b != m_mapUserInfo.end(); b++)
    {
        WriteFileInfo(pFile, (_UserInfo* )b->second);
    }

    for(uint32 i = 0; i < m_vecFreeUserInfo.size(); i++)
    {
        if(m_vecFreeUserInfo[i]->m_u4UserID > 0)
        {
            WriteFileInfo(pFile, (_UserInfo* )m_vecFreeUserInfo[i]);
        }
    }

    fclose(pFile);
}

bool CUserInfoManager::WriteFileInfo( FILE* pFile, _UserInfo* pUserInfo )
{
    char szUserInfoText[MAX_BUFF_200] = {'\0'};
    sprintf_safe(szUserInfoText, MAX_BUFF_200, "%d,%d,%d;\n", pUserInfo->m_u4UserID, pUserInfo->m_u4Life, pUserInfo->m_u4Magic);
    fwrite(szUserInfoText, sizeof(char), strlen(szUserInfoText), pFile);
    return true;
}

void CUserInfoManager::GetFreeUserInfo()
{
    //如果当前map不存在，则去空闲内存块寻找，有可能从后台加载数据已经加载进来了。
    //这里的map和Watch的监控是不一致的，如果当前找不到才去当前以为空闲的去寻找。
    for(uint32 i = 0; i < (uint32)m_vecFreeUserInfo.size(); i++)
    {
        if(m_vecFreeUserInfo[i]->GetDelete() == false)
        {
            mapUserInfo::iterator f = m_mapUserInfo.find(m_vecFreeUserInfo[i]->m_u4UserID);

            if(f == m_mapUserInfo.end())
            {
                m_mapUserInfo.insert(mapUserInfo::value_type(m_vecFreeUserInfo[i]->m_u4UserID, (_UserInfo* )m_vecFreeUserInfo[i]));
            }
        }
    }

    for(vecUserInfo::iterator b = m_vecFreeUserInfo.begin(); b != m_vecFreeUserInfo.end();)
    {
        _UserInfo* pUserInfo = (_UserInfo* )(*b);

        if(pUserInfo->GetDelete() == false)
        {
            b = m_vecFreeUserInfo.erase(b);
        }
        else
        {
            b++;
        }
    }
}

bool CUserInfoManager::Load_From_DataResouce( uint32 u4SeachUserID, uint32& u4CacheIndex )
{
    u4CacheIndex = 0;
    char szFileName[MAX_BUFF_200] = {'\0'};
    sprintf_safe(szFileName, MAX_BUFF_200, "%s", SOURCE_FILE_INFO_PATH);
    FILE* pFile = fopen((char* )szFileName, "r");

    if(pFile == NULL)
    {
        //文件不存在
        return false;
    }

    fseek(pFile, 0l, SEEK_END);

    uint32 u4FileSize = ftell(pFile);

    if(u4FileSize == 0)
    {
        //文件长度为0
        return false;
    }

    fclose(pFile);

    pFile = fopen((char* )szFileName, "r");
    fseek(pFile, 0l, SEEK_SET);

    char* pFileBuffer = new char[u4FileSize + 1];
    memset(pFileBuffer, 0, u4FileSize);

    //读取文件,一次读不完循环读，直到全部读完为止
    uint32 u4ReadSize = (uint32)fread((char* )pFileBuffer, sizeof(char), u4FileSize, pFile);

    if(u4ReadSize >= u4FileSize)
    {
        //读完了
    }
    else if(u4ReadSize == 0)
    {
        //读取文件失败了
        SAFE_DELETE_ARRAY(pFileBuffer);
        fclose(pFile);
        return false;
    }

    uint32 u4UserID    = 0;
    uint32 u4UserLife  = 0;
    uint32 u4UserMagic = 0;

    char szFind[2] = {'\0'};
    sprintf_safe(szFind, 2, ";");

    char* pLine = strtok((char* )pFileBuffer, szFind);

    while(pLine != NULL)
    {
        bool blState = GetFileInfo(pLine, u4UserID, u4UserLife, u4UserMagic);

        //从数据源中寻找指定的用户，并加载在共享内存中
        if(true == blState && u4SeachUserID == u4UserID)
        {
            _UserInfo* pUserInfo = (_UserInfo* )GetUserInfo(u4UserID);

            if(NULL != pUserInfo)
            {
                //初始化共享内存数据
                pUserInfo->m_u4UserID = u4UserID;
                pUserInfo->m_u4Life   = u4UserLife;
                pUserInfo->m_u4Magic  = u4UserLife;
                pUserInfo->SetHit();
            }
            else
            {
                //如果没有命中，则是新数据，从空闲池里取出数据放在里面
                if(m_vecFreeUserInfo.size() <= 0)
                {
                    //如果空闲池已经不存在了，淘汰最长时间未被访问的数据
                    uint32 u4DeleteUserID = 0;
                    bool blIsLast = m_objLRU.Check_Cached_Lru(u4DeleteUserID);

                    if(blIsLast == false)
                    {
                        //没有找到被淘汰的数据，这里肯定是不合法的。
                        SAFE_DELETE_ARRAY(pFileBuffer);
                        fclose(pFile);
                        return false;
                    }

                    mapUserInfo::iterator f = m_mapUserInfo.find(u4DeleteUserID);

                    if(f == m_mapUserInfo.end())
                    {
                        SAFE_DELETE_ARRAY(pFileBuffer);
                        fclose(pFile);
                        return false;
                    }
                    else
                    {
                        _UserInfo* pUserInfo = (_UserInfo* )f->second;

                        //在这里进行LRU替换之
                        m_objLRU.Delete_Cached_Lru(u4DeleteUserID);
                        m_objLRU.Add_Cached_Lru(u4UserID, pUserInfo->GetCacheIndex());

                        m_mapUserInfo.erase(f);

                        //初始化共享内存数据
                        pUserInfo->m_u4UserID = u4UserID;
                        pUserInfo->m_u4Life   = u4UserLife;
                        pUserInfo->m_u4Magic  = u4UserLife;
                        pUserInfo->SetHit();

                        m_mapUserInfo.insert(mapUserInfo::value_type(pUserInfo->m_u4UserID, pUserInfo));
                        u4CacheIndex = pUserInfo->GetCacheIndex();
                    }
                }
                else
                {
                    _UserInfo* pUserInfo = (_UserInfo* )m_vecFreeUserInfo[0];

                    if(NULL != pUserInfo)
                    {
                        //初始化共享内存数据
                        pUserInfo->m_u4UserID = u4UserID;
                        pUserInfo->m_u4Life   = u4UserLife;
                        pUserInfo->m_u4Magic  = u4UserLife;
                        pUserInfo->SetHit();

                        m_mapUserInfo.insert(mapUserInfo::value_type(pUserInfo->m_u4UserID, pUserInfo));

                        vecUserInfo::iterator b = m_vecFreeUserInfo.begin();
                        m_vecFreeUserInfo.erase(b);
                    }
                }
            }

            break;
        }

        pLine = strtok(NULL, szFind);
    }

    SAFE_DELETE_ARRAY(pFileBuffer);
    fclose(pFile);

    Display();

    return true;
}

bool CUserInfoManager::GetFileInfo( const char* pLine, uint32& u4UserID, uint32& u4Life, uint32& u4Magic )
{
    bool blRet = false;

    char szTemp[MAX_BUFF_200] = {'\0'};

    if(pLine[0] != 13)
    {
        int nIndex    = 0;
        int nBeginPos = 0;
        int nEndPos   = 0;

        for(int i = 0; i < (int)strlen(pLine); i++)
        {
            if(pLine[i]  == ',')
            {
                if(nIndex == 0)
                {
                    //用户ID
                    ACE_OS::memcpy(szTemp, &pLine[nBeginPos], nEndPos - nBeginPos);
                    szTemp[nEndPos - nBeginPos] = '\0';
                    u4UserID = ACE_OS::atoi(szTemp);
                    nBeginPos = i + 1;
                    nIndex++;
                }
                else if(nIndex == 1)
                {
                    //Life
                    ACE_OS::memcpy(szTemp, &pLine[nBeginPos], nEndPos - nBeginPos);
                    szTemp[nEndPos - nBeginPos] = '\0';
                    u4Life = ACE_OS::atoi(szTemp);
                    nBeginPos = i + 1;
                    nIndex++;
                }
            }

            nEndPos++;
        }

        if(nIndex == 2)
        {
            //Magic
            ACE_OS::memcpy(szTemp, &pLine[nBeginPos], (int)strlen(pLine) - nBeginPos);
            szTemp[(int)strlen(pLine) - nBeginPos] = '\0';
            u4Magic = ACE_OS::atoi(szTemp);
            blRet = true;
        }
    }
    else
    {
        int nIndex    = 0;
        int nBeginPos = 1;
        int nEndPos   = 0;

        for(int i = 1; i < (int)strlen(pLine); i++)
        {
            if(pLine[i]  == ',')
            {
                if(nIndex == 0)
                {
                    //用户ID
                    ACE_OS::memcpy(szTemp, &pLine[nBeginPos], nEndPos - nBeginPos);
                    szTemp[nEndPos - nBeginPos] = '\0';
                    u4UserID = ACE_OS::atoi(szTemp);
                    nBeginPos = i + 1;
                    nIndex++;
                }
                else if(nIndex == 1)
                {
                    //Life
                    ACE_OS::memcpy(szTemp, &pLine[nBeginPos], nEndPos - nBeginPos);
                    szTemp[nEndPos - nBeginPos] = '\0';
                    u4Life = ACE_OS::atoi(szTemp);
                    nBeginPos = i + 1;
                    nIndex++;
                }
            }

            nEndPos++;
        }

        if(nIndex == 2)
        {
            //Magic
            ACE_OS::memcpy(szTemp, &pLine[nBeginPos], (int)strlen(pLine) - nBeginPos - 1);
            szTemp[(int)strlen(pLine) - nBeginPos - 1] = '\0';
            u4Magic = ACE_OS::atoi(szTemp);
            blRet = true;
        }
    }

    return blRet;
}

bool CUserInfoManager::Reload_Map_CacheMemory( uint32 u4CacheIndex )
{
    uint32 u4OldUserID = 0;

    m_objLRU.Get_Cached_KeyByIndex(u4CacheIndex, u4OldUserID);

    mapUserInfo::iterator f = m_mapUserInfo.find(u4OldUserID);

    if(f != m_mapUserInfo.end())
    {
        _UserInfo* pUserInfo = (_UserInfo* )f->second;

        //重载Index列表对应关系
        m_objLRU.Reload_Cached_IndexList((uint32)pUserInfo->m_u4UserID, u4OldUserID, u4CacheIndex);

        m_mapUserInfo.erase(f);
        m_mapUserInfo.insert(mapUserInfo::value_type((uint32)pUserInfo->m_u4UserID, pUserInfo));
        return true;
    }
    else
    {
        return false;
    }
}

bool CUserInfoManager::Read_All_Init_DataResoure()
{
    //从文件里面获取当前可以支持的用户名和密码，放入共享内存中去
    char szFileName[MAX_BUFF_200] = {'\0'};
    sprintf_safe(szFileName, MAX_BUFF_200, "%s", SOURCE_FILE_INFO_PATH);
    FILE* pFile = fopen((char* )szFileName, "r");

    if(pFile == NULL)
    {
        //文件不存在
        return false;
    }

    fseek(pFile, 0l, SEEK_END);

    uint32 u4FileSize = ftell(pFile);

    if(u4FileSize == 0)
    {
        //文件长度为0
        return false;
    }

    fclose(pFile);

    m_mapUserInfo.clear();
    m_vecFreeUserInfo.clear();

    pFile = fopen((char* )szFileName, "r");
    fseek(pFile, 0l, SEEK_SET);

    char* pFileBuffer = new char[u4FileSize + 1];
    memset(pFileBuffer, 0, u4FileSize);

    //读取文件,一次读不完循环读，直到全部读完为止
    uint32 u4ReadSize = (uint32)fread((char* )pFileBuffer, sizeof(char), u4FileSize, pFile);

    if(u4ReadSize >= u4FileSize)
    {
        //读完了
    }
    else if(u4ReadSize == 0)
    {
        //读取文件失败了
        SAFE_DELETE_ARRAY(pFileBuffer);
        fclose(pFile);
        return false;
    }

    ID_t nIndex = 1;

    char szFind[2] = {'\0'};
    sprintf_safe(szFind, 2, ";");

    char* pLine = strtok((char* )pFileBuffer, szFind);

    while(pLine != NULL)
    {
        if(nIndex > (uint32)Get_Cache_Count())
        {
            break;
        }

        uint32 u4UserID = 0;
        uint32 u4Life   = 0;
        uint32 u4Magic  = 0;

        bool blState = GetFileInfo(pLine, u4UserID, u4Life, u4Magic);

        if(true == blState)
        {
            _UserInfo* pUserInfo = (_UserInfo* )Get_CacheBlock_By_Index(nIndex);

            if(NULL != pUserInfo)
            {
                //初始化块ID
                pUserInfo->SetCacheIndex(nIndex);

                //初始化共享内存数据
                pUserInfo->m_u4UserID = u4UserID;
                pUserInfo->m_u4Life   = u4Life;
                pUserInfo->m_u4Magic  = u4Magic;

                //插入Lru列表
                //这里不用检查Lru是否启动，因为是第一次加载
                //不会超过Get_Cache_Count()上限
                m_objLRU.Add_Cached_Lru(u4UserID, pUserInfo->GetCacheIndex());

                pUserInfo->SetHit();

                m_mapUserInfo.insert(mapUserInfo::value_type(u4UserID, pUserInfo));

                nIndex++;
            }
        }

        pLine = strtok(NULL, szFind);
    }

    //剩下的没有使用的共享内存块放在一起
    if(nIndex <= (uint32)Get_Cache_Count())
    {
        for(ID_t i = nIndex; i <= (ID_t)Get_Cache_Count(); i++)
        {
            _UserInfo* pUserInfo = (_UserInfo* )Get_CacheBlock_By_Index(i);

            if(NULL != pUserInfo)
            {
                pUserInfo->SetUnHit();

                m_vecFreeUserInfo.push_back(pUserInfo);
            }
        }
    }

    SAFE_DELETE_ARRAY(pFileBuffer);
    fclose(pFile);

    Set_Memory_Init_Success();

    return true;
}

bool CUserInfoManager::Read_All_From_CacheMemory()
{
    m_mapUserInfo.clear();
    m_vecFreeUserInfo.clear();

    for(uint32 i = 1; i <= Get_Cache_Count(); i++)
    {
        _UserInfo* pUserInfo = (_UserInfo* )Get_CacheBlock_By_Index(i);

        if(NULL != pUserInfo)
        {
            //符合条件的数据才插入当前遍历列表
            if(pUserInfo->m_u4UserID > 0
               && pUserInfo->GetDelete() == false)
            {
                //插入Lru列表
                //这里不用检查Lru是否启动，因为是第一次从已有内存加载
                //不会超过Get_Cache_Count()上限
                m_objLRU.Add_Cached_Lru(pUserInfo->m_u4UserID, pUserInfo->GetCacheIndex());

                m_mapUserInfo.insert(mapUserInfo::value_type(pUserInfo->m_u4UserID, pUserInfo));
            }
            else
            {
                //属于空闲的数据块，存在一起
                m_vecFreeUserInfo.push_back(pUserInfo);
            }
        }
    }

    return true;
}

void CUserInfoManager::Begin_Sync_DataReaource_To_Memory()
{
    for(mapUserInfo::iterator b = m_mapUserInfo.begin(); b != m_mapUserInfo.end(); b++)
    {
        _UserInfo* pUserInfo = (_UserInfo* )b->second;

        if(NULL != pUserInfo)
        {
            pUserInfo->SetCheckState(CHECKS_UNHIT);
        }
    }
}

void CUserInfoManager::End_Sync_DataReaource_To_Memory()
{
    for(mapUserInfo::iterator b = m_mapUserInfo.begin(); b != m_mapUserInfo.end();)
    {
        _UserInfo* pUserInfo = (_UserInfo* )b->second;

        if(NULL != pUserInfo)
        {
            if(pUserInfo->GetCheckState() != CHECKS_HIT)
            {
                //说明这个数据已经在遍历中不存在了，回收之
                pUserInfo->SetDelete(true);

                //从Lru中删除
                m_objLRU.Delete_Cached_Lru((uint32)pUserInfo->m_u4UserID);

                m_mapUserInfo.erase(b++);
                m_vecFreeUserInfo.push_back(pUserInfo);
            }
            else
            {
                b++;
            }
        }
    }

    OUR_DEBUG((LM_INFO, "[CUserValidManager::EndCheck]map is(%d), free is (%d).\n", m_mapUserInfo.size(), m_vecFreeUserInfo.size()));
}