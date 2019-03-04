#include "UserValidManager.h"

CUserValidManager::CUserValidManager()
{

}

CUserValidManager::~CUserValidManager()
{
    Close();
}

bool CUserValidManager::Read_All_Init_DataResoure()
{
    //从文件里面获取当前可以支持的用户名和密码，放入共享内存中去
    char szFileName[MAX_BUFF_200] = {'\0'};
    sprintf_safe(szFileName, MAX_BUFF_200, "%s", SOURCE_FILE_PATH);
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

    m_mapUserValid.clear();
    m_vecFreeValid.clear();

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
    char szUserName[MAX_BUFF_50] = {'\0'};
    char szUserPass[MAX_BUFF_50] = {'\0'};

    char szFind[2] = {'\0'};
    sprintf_safe(szFind, 2, ";");

    char* pLine = strtok((char* )pFileBuffer, szFind);

    while(pLine != NULL)
    {
        if(nIndex > (uint32)Get_Cache_Count())
        {
            break;
        }

        bool blState = GetFileInfo(pLine, szUserName, szUserPass);

        if(true == blState)
        {
            _UserValid* pUserValid = (_UserValid* )Get_CacheBlock_By_Index(nIndex);

            if(NULL != pUserValid)
            {
                //初始化块ID
                pUserValid->SetCacheIndex(nIndex);

                //初始化共享内存数据
                sprintf_safe(pUserValid->m_szUserName, MAX_BUFF_50, "%s", szUserName);
                sprintf_safe(pUserValid->m_szUserPass, MAX_BUFF_50, "%s", szUserPass);

                //插入Lru列表
                //这里不用检查Lru是否启动，因为是第一次加载
                //不会超过Get_Cache_Count()上限
                m_objLRU.Add_Cached_Lru((string)szUserName, pUserValid->GetCacheIndex());

                pUserValid->m_u4LoginCount  = 0;
                pUserValid->SetHit();

                string strUserName;
                strUserName = (string)pUserValid->m_szUserName;

                m_mapUserValid.insert(mapUserValid::value_type(strUserName, pUserValid));

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
            _UserValid* pUserValid = (_UserValid* )Get_CacheBlock_By_Index(i);

            if(NULL != pUserValid)
            {
                pUserValid->m_u4LoginCount                 = 0;
                pUserValid->SetUnHit();

                m_vecFreeValid.push_back(pUserValid);
            }
        }
    }

    SAFE_DELETE_ARRAY(pFileBuffer);
    fclose(pFile);

    Set_Memory_Init_Success();

    return true;
}

bool CUserValidManager::GetFileInfo( const char* pLine, char* pUserName, char* pUserPass )
{
    bool blRet = false;

    int nSplit = 0;
    int nLineSize = (int)strlen(pLine);

    for(int i = 0; i < nLineSize; i++)
    {
        if(pLine[i] == ',')
        {
            nSplit = i;
        }
    }

    if(pLine[0] != 13)
    {
        ACE_OS::memcpy(pUserName, pLine, nSplit);
        pUserName[nSplit] = '\0';
        ACE_OS::memcpy(pUserPass, &pLine[nSplit + 1], nLineSize - nSplit - 1);
        pUserPass[nLineSize - nSplit - 1] = '\0';
    }
    else
    {
        ACE_OS::memcpy(pUserName, pLine + 1, nSplit - 1);
        pUserName[nSplit - 1] = '\0';
        ACE_OS::memcpy(pUserPass, &pLine[nSplit + 1], nLineSize - nSplit - 1);
        pUserPass[nLineSize - nSplit - 1] = '\0';
    }


    if(nSplit > 0)
    {
        blRet = true;
    }

    return blRet;
}

void CUserValidManager::Close()
{
    m_mapUserValid.clear();
    m_vecFreeValid.clear();
}

bool CUserValidManager::Read_All_From_CacheMemory()
{
    m_mapUserValid.clear();
    m_vecFreeValid.clear();

    for(uint32 i = 1; i <= Get_Cache_Count(); i++)
    {
        _UserValid* pUserValid = (_UserValid* )Get_CacheBlock_By_Index(i);

        if(NULL != pUserValid)
        {
            //符合条件的数据才插入当前遍历列表
            if(ACE_OS::strlen(pUserValid->m_szUserName) != 0
               && ACE_OS::strlen(pUserValid->m_szUserPass) != 0
               && pUserValid->GetDelete() == false)
            {

                //插入Lru列表
                //这里不用检查Lru是否启动，因为是第一次从已有内存加载
                //不会超过Get_Cache_Count()上限
                m_objLRU.Add_Cached_Lru((string)pUserValid->m_szUserName, pUserValid->GetCacheIndex());

                string strUserName = (string)pUserValid->m_szUserName;
                m_mapUserValid.insert(mapUserValid::value_type(strUserName, pUserValid));
            }
            else
            {
                //属于空闲的数据块，存在一起
                m_vecFreeValid.push_back(pUserValid);
            }
        }
    }

#if 0
    //测试代码
    string strData = "rabbit";
    mapUserValid::iterator f = m_mapUserValid.find(strData);

    if(f == m_mapUserValid.end())
    {
        OUR_DEBUG((LM_INFO, "no find.\n"));
    }
    else
    {
        OUR_DEBUG((LM_INFO, "find.\n"));
    }

#endif
    return true;
}

_UserValid* CUserValidManager::GetUserValid( const char* pUserName )
{
    string strUserName = (string)pUserName;
    mapUserValid::iterator f = m_mapUserValid.find(strUserName);

    if(f == m_mapUserValid.end())
    {
        return NULL;
    }
    else
    {
        string strMapUserName = (string)f->first;
        _UserValid* pUserValid = (_UserValid* )f->second;

        if(NULL != pUserValid)
        {
            if(strUserName != (string)pUserValid->m_szUserName || pUserValid->GetDelete() == true)
            {
                //说明当前数据已经无效了，从当前map里面清除
                m_mapUserValid.erase(f);
                m_vecFreeValid.push_back(pUserValid);

                //删除Lru相关内容
                m_objLRU.Delete_Cached_Lru(strUserName);

                //如果新数据不是删除的数据，重新添加新加载的数据
                if(pUserValid->GetDelete() == false)
                {
                    m_mapUserValid.insert(mapUserValid::value_type((string)pUserValid->m_szUserName, pUserValid));
                }

                return NULL;
            }
            else
            {
                //添加命中
                m_objLRU.Add_Cached_Lru(strUserName, pUserValid->GetCacheIndex());

                return pUserValid;
            }
        }
        else
        {
            return NULL;
        }

    }
}

void CUserValidManager::Sync_DataReaource_To_Memory()
{
    char szFileName[MAX_BUFF_200] = {'\0'};
    sprintf_safe(szFileName, MAX_BUFF_200, "%s", SOURCE_FILE_PATH);
    FILE* pFile = fopen((char* )szFileName, "r");

    if(pFile == NULL)
    {
        //文件不存在
        return;
    }

    fseek(pFile, 0l, SEEK_END);

    uint32 u4FileSize = ftell(pFile);

    if(u4FileSize == 0)
    {
        //文件长度为0
        return;
    }

    fclose(pFile);

    //m_mapUserValid.clear();
    //m_vecFreeValid.clear();

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
        return;
    }

    char szUserName[MAX_BUFF_50] = {'\0'};
    char szUserPass[MAX_BUFF_50] = {'\0'};

    char szFind[2] = {'\0'};
    sprintf_safe(szFind, 2, ";");

    char* pLine = strtok((char* )pFileBuffer, szFind);

    Begin_Sync_DataReaource_To_Memory();

    while(pLine != NULL)
    {
        bool blState = GetFileInfo(pLine, szUserName, szUserPass);

        if(true == blState)
        {
            _UserValid* pUserValid = (_UserValid* )GetUserValid(szUserName);

            if(NULL != pUserValid)
            {
                //初始化共享内存数据
                sprintf_safe(pUserValid->m_szUserName, MAX_BUFF_50, "%s", szUserName);
                sprintf_safe(pUserValid->m_szUserPass, MAX_BUFF_50, "%s", szUserPass);
                pUserValid->SetHit();
            }
            else
            {
                //如果没有命中，则是新数据，从空闲池里取出数据放在里面
                if(m_vecFreeValid.size() <= 0)
                {
                    //如果空闲池已经没有了，则推出函数
                    pLine = strtok(NULL, szFind);
                    continue;
                }

                _UserValid* pUserValid = (_UserValid* )m_vecFreeValid[0];

                if(NULL != pUserValid)
                {
                    //初始化共享内存数据
                    sprintf_safe(pUserValid->m_szUserName, MAX_BUFF_50, "%s", szUserName);
                    sprintf_safe(pUserValid->m_szUserPass, MAX_BUFF_50, "%s", szUserPass);

                    pUserValid->m_u4LoginCount                 = 0;
                    pUserValid->SetHit();

                    string strUserName;
                    strUserName = (string)pUserValid->m_szUserName;

                    m_mapUserValid.insert(mapUserValid::value_type(strUserName, pUserValid));

                    vecValid::iterator b = m_vecFreeValid.begin();
                    m_vecFreeValid.erase(b);
                }
            }
        }

        pLine = strtok(NULL, szFind);
    }

    End_Sync_DataReaource_To_Memory();

    SAFE_DELETE_ARRAY(pFileBuffer);
    fclose(pFile);
}

void CUserValidManager::Begin_Sync_DataReaource_To_Memory()
{
    for(mapUserValid::iterator b = m_mapUserValid.begin(); b != m_mapUserValid.end(); b++)
    {
        _UserValid* pUserValid = (_UserValid* )b->second;

        if(NULL != pUserValid)
        {
            pUserValid->SetCheckState(CHECKS_UNHIT);
        }
    }
}

void CUserValidManager::End_Sync_DataReaource_To_Memory()
{
    for(mapUserValid::iterator b = m_mapUserValid.begin(); b != m_mapUserValid.end();)
    {
        _UserValid* pUserValid = (_UserValid* )b->second;

        if(NULL != pUserValid)
        {
            if(pUserValid->GetCheckState() != CHECKS_HIT)
            {
                //说明这个数据已经在遍历中不存在了，回收之
                pUserValid->SetDelete(true);

                //从Lru中删除
                m_objLRU.Delete_Cached_Lru((string)pUserValid->m_szUserName);

                m_mapUserValid.erase(b++);
                m_vecFreeValid.push_back(pUserValid);
            }
            else
            {
                b++;
            }
        }
    }

    OUR_DEBUG((LM_INFO, "[CUserValidManager::EndCheck]map is(%d), free is (%d).\n", m_mapUserValid.size(), m_vecFreeValid.size()));
}

void CUserValidManager::GetFreeValid()
{
    //如果当前map不存在，则去空闲内存块寻找，有可能从后台加载数据已经加载进来了。
    //这里的map和Watch的监控是不一致的，如果当前找不到才去当前以为空闲的去寻找。
    for(uint32 i = 0; i < (uint32)m_vecFreeValid.size(); i++)
    {
        if(m_vecFreeValid[i]->GetDelete() == false)
        {
            mapUserValid::iterator f = m_mapUserValid.find((string)m_vecFreeValid[i]->m_szUserName);

            if(f == m_mapUserValid.end())
            {
                m_mapUserValid.insert(mapUserValid::value_type((string)m_vecFreeValid[i]->m_szUserName, (_UserValid* )m_vecFreeValid[i]));
            }
        }
    }

    for(vecValid::iterator b = m_vecFreeValid.begin(); b != m_vecFreeValid.end();)
    {
        _UserValid* pUserValid = (_UserValid* )(*b);

        if(pUserValid->GetDelete() == false)
        {
            b = m_vecFreeValid.erase(b);
        }
        else
        {
            b++;
        }
    }
}

bool CUserValidManager::Load_From_DataResouce(const char* pUserName, uint32& u4CacheIndex)
{
    u4CacheIndex = 0;
    char szFileName[MAX_BUFF_200] = {'\0'};
    sprintf_safe(szFileName, MAX_BUFF_200, "%s", SOURCE_FILE_PATH);
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

    //m_mapUserValid.clear();
    //m_vecFreeValid.clear();

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

    char szUserName[MAX_BUFF_50] = {'\0'};
    char szUserPass[MAX_BUFF_50] = {'\0'};

    char szFind[2] = {'\0'};
    sprintf_safe(szFind, 2, ";");

    char* pLine = strtok((char* )pFileBuffer, szFind);

    while(pLine != NULL)
    {
        bool blState = GetFileInfo(pLine, szUserName, szUserPass);

        //从数据源中寻找指定的用户，并加载在共享内存中
        if(true == blState && ACE_OS::strcmp(szUserName, pUserName) == 0)
        {
            _UserValid* pUserValid = (_UserValid* )GetUserValid(szUserName);

            if(NULL != pUserValid)
            {
                //初始化共享内存数据
                sprintf_safe(pUserValid->m_szUserName, MAX_BUFF_50, "%s", szUserName);
                sprintf_safe(pUserValid->m_szUserPass, MAX_BUFF_50, "%s", szUserPass);
                pUserValid->SetHit();
                pUserValid->m_u4LoginCount                 = 0;
            }
            else
            {
                //如果没有命中，则是新数据，从空闲池里取出数据放在里面
                if(m_vecFreeValid.size() <= 0)
                {
                    //如果空闲池已经不存在了，淘汰最长时间未被访问的数据
                    string strUserName;
                    bool blIsLast = m_objLRU.Check_Cached_Lru(strUserName);

                    if(blIsLast == false)
                    {
                        //没有找到被淘汰的数据，这里肯定是不合法的。
                        SAFE_DELETE_ARRAY(pFileBuffer);
                        fclose(pFile);
                        return false;
                    }

                    mapUserValid::iterator f = m_mapUserValid.find(strUserName);

                    if(f == m_mapUserValid.end())
                    {
                        SAFE_DELETE_ARRAY(pFileBuffer);
                        fclose(pFile);
                        return false;
                    }
                    else
                    {
                        _UserValid* pUserValid = (_UserValid* )f->second;

                        //在这里进行LRU替换之
                        m_objLRU.Delete_Cached_Lru(strUserName);
                        m_objLRU.Add_Cached_Lru((string)pUserName, pUserValid->GetCacheIndex());

                        m_mapUserValid.erase(f);

                        //初始化共享内存数据
                        sprintf_safe(pUserValid->m_szUserName, MAX_BUFF_50, "%s", szUserName);
                        sprintf_safe(pUserValid->m_szUserPass, MAX_BUFF_50, "%s", szUserPass);
                        pUserValid->SetHit();
                        pUserValid->m_u4LoginCount                 = 0;

                        string strUserNewName;
                        strUserNewName = (string)pUserValid->m_szUserName;

                        m_mapUserValid.insert(mapUserValid::value_type(strUserNewName, pUserValid));
                        u4CacheIndex = pUserValid->GetCacheIndex();
                    }
                }
                else
                {
                    _UserValid* pUserValid = (_UserValid* )m_vecFreeValid[0];

                    if(NULL != pUserValid)
                    {
                        //初始化共享内存数据
                        sprintf_safe(pUserValid->m_szUserName, MAX_BUFF_50, "%s", szUserName);
                        sprintf_safe(pUserValid->m_szUserPass, MAX_BUFF_50, "%s", szUserPass);
                        pUserValid->SetHit();
                        pUserValid->m_u4LoginCount                 = 0;

                        string strUserName;
                        strUserName = (string)pUserValid->m_szUserName;

                        m_mapUserValid.insert(mapUserValid::value_type(strUserName, pUserValid));

                        vecValid::iterator b = m_vecFreeValid.begin();
                        m_vecFreeValid.erase(b);
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

bool CUserValidManager::Init(uint32 u4CachedCount, key_t objMemorykey, uint32 u4CheckSize)
{
    //设置Lru对象上限
    m_objLRU.Set_Lru_Max_Count(u4CachedCount);

    //调用父类方法
    return CCacheManager::Init(u4CachedCount, objMemorykey, u4CheckSize);
}

void CUserValidManager::Display()
{
    OUR_DEBUG((LM_INFO, "[CUserValidManager::Display]m_mapUserValid count=%d, m_vecFreeValid=%d.\n", m_mapUserValid.size(), m_vecFreeValid.size()));

    for(mapUserValid::iterator b = m_mapUserValid.begin(); b!= m_mapUserValid.end(); b++)
    {
        _UserValid* pUserValid = (_UserValid* )b->second;

        OUR_DEBUG((LM_INFO, "[CUserValidManager::Display]UserName=%s.\n", pUserValid->m_szUserName));
    }
}

bool CUserValidManager::Reload_Map_CacheMemory(uint32 u4CacheIndex)
{
    string strOldUserName;

    m_objLRU.Get_Cached_KeyByIndex(u4CacheIndex, strOldUserName);

    mapUserValid::iterator f = m_mapUserValid.find(strOldUserName);

    if(f != m_mapUserValid.end())
    {
        _UserValid* pUserValid = (_UserValid* )f->second;

        //重载Index列表对应关系
        m_objLRU.Reload_Cached_IndexList((string)pUserValid->m_szUserName, strOldUserName, u4CacheIndex);

        m_mapUserValid.erase(f);
        m_mapUserValid.insert(mapUserValid::value_type((string)pUserValid->m_szUserName, pUserValid));
        return true;
    }
    else
    {
        return false;
    }
}
