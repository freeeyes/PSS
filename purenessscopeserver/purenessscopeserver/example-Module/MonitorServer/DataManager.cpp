#include "DataManager.h"

CDataManager* CDataManager::m_pInstance = NULL;

CDataManager::CDataManager()
{
}

CDataManager::~CDataManager()
{
    Close();
}

void CDataManager::Close()
{
    if(NULL != m_pTiXmlDocument)
    {
        delete m_pTiXmlDocument;
        m_pTiXmlDocument = NULL;
        m_pRootElement   = NULL;
    }

    for (mapGroupNodeData::iterator itGroupNodeData = m_mapGroupNodeData.begin();
         itGroupNodeData != m_mapGroupNodeData.end(); itGroupNodeData++)
    {
        mapIP2NodeData& objMapIP2NodeData = (mapIP2NodeData)itGroupNodeData->second;

        for (mapIP2NodeData::iterator itIP2Node = objMapIP2NodeData.begin();
             itIP2Node != objMapIP2NodeData.end(); )
        {
            PssNodeInfoList* pNodeInfo = (PssNodeInfoList*)itIP2Node->second;
            delete pNodeInfo;
            pNodeInfo = NULL;

            objMapIP2NodeData.erase(itIP2Node++);
        }
    }

    if (NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

//解析xml配置文件
bool CDataManager::ParseXmlFile(const char* pXmlFile)
{
    m_pTiXmlDocument = new TiXmlDocument(pXmlFile);

    if(NULL == m_pTiXmlDocument)
    {
        return false;
    }

    if(false == m_pTiXmlDocument->LoadFile())
    {
        OUR_DEBUG((LM_INFO, "[CDataManager::ParseXmlFile]LoadFile\n"));
        return false;
    }

    //获得根元素
    m_pRootElement = m_pTiXmlDocument->RootElement();

    char* pData = NULL;
    pData = this->GetData(m_pRootElement,"htmlIndex", "path");

    if(NULL != pData)
    {
        m_strHtmlIndexPath = pData;
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CDataManager::ParseXmlFile]htmlIndex path is Invalid!!, please check monitor.xml.\n"));
        return false;
    }

    pData = this->GetData(m_pRootElement,"htmlIndex", "name");

    if(NULL != pData)
    {
        m_strHtmlIndexName = pData;
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CDataManager::ParseXmlFile]htmlIndex name is Invalid!!, please check monitor.xml.\n"));
        return false;
    }

    pData = this->GetData(m_pRootElement,"htmldetail", "path");

    if(NULL != pData)
    {
        m_strHtmlDetailPath = pData;
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CDataManager::ParseXmlFile]htmldetail path is Invalid!!, please check monitor.xml.\n"));
        return false;
    }

    pData = this->GetData(m_pRootElement,"htmlJsondetail", "path");

    if(NULL != pData)
    {
        m_strHtmlJsonPath = pData;
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CDataManager::ParseXmlFile]htmlJsondetail path is Invalid!!, please check monitor.xml.\n"));
        return false;
    }

    pData = this->GetData(m_pRootElement,"defaultcfg", "historymaxcount");

    if(NULL != pData)
    {
        m_u4HistoryMaxCount = (uint16)ACE_OS::atoi(pData);
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CDataManager::ParseXmlFile]defaultcfg historymaxcount is Invalid!!, please check monitor.xml.\n"));
        return false;
    }

    pData = this->GetData(m_pRootElement,"defaultcfg", "timeinterval");

    if(NULL != pData)
    {
        m_u4TimeInterval = (uint16)ACE_OS::atoi(pData);
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CDataManager::ParseXmlFile]defaultcfg timeinterval is Invalid!!, please check monitor.xml.\n"));
        return false;
    }

    TiXmlElement* pNextGroupName = NULL;
    TiXmlElement* pSubElement = NULL;

    while(true)
    {
        if(NULL == pNextGroupName)
        {
            pSubElement = m_pRootElement->FirstChildElement("group");
        }
        else
        {
            pSubElement = pNextGroupName->NextSiblingElement();
        }

        string strGroupName;
        pData = this->GetData(m_pRootElement,"group", "name", pNextGroupName);

        if(pData != NULL)
        {
            strGroupName = pData;

            TiXmlElement* pNextTiXmlElementName = NULL;
            TiXmlElement* pNextTiXmlElementIP = NULL;
            TiXmlElement* pNextTiXmlElementCount = NULL;

            mapIP2NodeData objMapIP2NodeData;

            while(true)
            {
                string strName;
                pData = this->GetData(pSubElement,"ServerInfo", "name", pNextTiXmlElementName);

                if(pData != NULL)
                {
                    strName = pData;
                }
                else
                {
                    break;
                }

                string strIP;
                pData = this->GetData(pSubElement,"ServerInfo", "ip", pNextTiXmlElementIP);

                if(pData != NULL)
                {
                    strIP = pData;
                }
                else
                {
                    break;
                }

                uint32 u4Count;
                pData = this->GetData(pSubElement,"ServerInfo", "historymaxcount", pNextTiXmlElementCount);

                if(pData != NULL)
                {
                    u4Count = (uint16)ACE_OS::atoi(pData);
                }
                else
                {
                    break;
                }

                m_mapIP2GroupName.insert(std::make_pair(strName, strGroupName));
                PssNodeInfoList* pNodeInfo = new PssNodeInfoList;

                if(pNodeInfo != NULL)
                {
                    pNodeInfo->Init(u4Count);
                    objMapIP2NodeData.insert(std::make_pair(strIP, pNodeInfo));
                }
                else
                {
                    OUR_DEBUG((LM_INFO, "[CDataManager::ParseXmlFile]pNodeInfo is NULL.\n"));
                    return false;
                }
            }

            m_mapGroupNodeData.insert(std::make_pair(strGroupName, objMapIP2NodeData));
        }
        else
        {
            break;
        }
    }

    mapIP2NodeData objMapIP2NodeData;
    m_mapGroupNodeData.insert(std::make_pair("UnkownGroup", objMapIP2NodeData));

    /*
    OUR_DEBUG((LM_INFO, "[CDataManager::ParseXmlFile]m_strHtmlIndexPath:%s\n",m_strHtmlIndexPath.c_str()));
    OUR_DEBUG((LM_INFO, "[CDataManager::ParseXmlFile]m_strHtmlIndexName:%s\n",m_strHtmlIndexName.c_str()));
    OUR_DEBUG((LM_INFO, "[CDataManager::ParseXmlFile]m_strHtmlDetailPath:%s\n",m_strHtmlDetailPath.c_str()));
    OUR_DEBUG((LM_INFO, "[CDataManager::ParseXmlFile]m_strHtmlJsonPath:%s\n",m_strHtmlJsonPath.c_str()));
    OUR_DEBUG((LM_INFO, "[CDataManager::ParseXmlFile]m_u4HistoryMaxCount:%d\n",m_u4HistoryMaxCount));
    OUR_DEBUG((LM_INFO, "[CDataManager::ParseXmlFile]m_u4TimeInterval:%d\n",m_u4TimeInterval));
    */

    return true;
}

//增加节点数据
void CDataManager::AddNodeDate(const char* pIP, uint32 u4Cpu,uint32 u4MemorySize,uint32 u4ConnectCount,uint32 u4DataIn,uint32 u4DataOut)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);
    mapIP2GroupName::iterator itIP2Group = m_mapIP2GroupName.find(pIP);

    if (itIP2Group != m_mapIP2GroupName.end())
    {
        string strGroupName = itIP2Group->second;
        mapGroupNodeData::iterator itGroupNodeData = m_mapGroupNodeData.find(strGroupName);

        if (itGroupNodeData != m_mapGroupNodeData.end())
        {
            mapIP2NodeData& objMapIP2NodeData = (mapIP2NodeData)itGroupNodeData->second;
            mapIP2NodeData::iterator itIP2Node = objMapIP2NodeData.find(pIP);

            if (itIP2Node != objMapIP2NodeData.end())
            {
                PssNodeInfoList* pNodeInfo = (PssNodeInfoList*)itIP2Node->second;
                PssNodeInfoSt objPssNodeInfoSt;
                sprintf_safe(objPssNodeInfoSt.m_szClientIP, MAX_BUFF_50, "%s", pIP);
                objPssNodeInfoSt.m_u4Cpu = u4Cpu;
                objPssNodeInfoSt.m_u4MemorySize = u4MemorySize;
                objPssNodeInfoSt.m_u4ConnectCount = u4ConnectCount;
                objPssNodeInfoSt.m_u4DataIn = u4DataIn;
                objPssNodeInfoSt.m_u4DataOut = u4DataOut;
                objPssNodeInfoSt.m_tvRecvTime = ACE_OS::gettimeofday();
                pNodeInfo->AddObject(objPssNodeInfoSt);
            }
        }
    }
    else
    {
        //未知IP
        OUR_DEBUG((LM_INFO, "[CDataManager::AddNodeDate]pIP:%s is not found.\n",pIP));
        mapGroupNodeData::iterator itGroupNodeData = m_mapGroupNodeData.find("UnkownGroup");

        if (itGroupNodeData != m_mapGroupNodeData.end())
        {
            PssNodeInfoSt objPssNodeInfoSt;
            sprintf_safe(objPssNodeInfoSt.m_szClientIP, MAX_BUFF_50, "%s", pIP);
            objPssNodeInfoSt.m_u4Cpu = u4Cpu;
            objPssNodeInfoSt.m_u4MemorySize = u4MemorySize;
            objPssNodeInfoSt.m_u4ConnectCount = u4ConnectCount;
            objPssNodeInfoSt.m_u4DataIn = u4DataIn;
            objPssNodeInfoSt.m_u4DataOut = u4DataOut;
            objPssNodeInfoSt.m_tvRecvTime = ACE_OS::gettimeofday();

            mapIP2NodeData& objMapIP2NodeData = (mapIP2NodeData)itGroupNodeData->second;
            mapIP2NodeData::iterator itIP2Node = objMapIP2NodeData.find(pIP);

            if (itIP2Node != objMapIP2NodeData.end())
            {
                PssNodeInfoList* pNodeInfo = (PssNodeInfoList*)itIP2Node->second;
                pNodeInfo->AddObject(objPssNodeInfoSt);
            }
            else
            {
                PssNodeInfoList* pNodeInfo = new PssNodeInfoList;

                if(pNodeInfo != NULL)
                {
                    pNodeInfo->Init(m_u4HistoryMaxCount);
                    pNodeInfo->AddObject(objPssNodeInfoSt);
                    objMapIP2NodeData.insert(std::make_pair(pIP, pNodeInfo));
                }
            }
        }
    }
}

//生成index html文件
void CDataManager::make_index_html()
{

}

//生成detail html文件
void CDataManager::make_detail_html()
{

}

char* CDataManager::GetData(TiXmlElement* pRootElement,const char* pName, const char* pAttrName)
{
    if(pRootElement == NULL)
    {
        return NULL;
    }

    TiXmlElement* pTiXmlElement = pRootElement->FirstChildElement(pName);

    if(NULL != pTiXmlElement)
    {
        return (char* )pTiXmlElement->Attribute(pAttrName);
    }

    return NULL;
}

char* CDataManager::GetData(TiXmlElement* pRootElement, const char* pName, const char* pAttrName, TiXmlElement*& pNextTiXmlElement )
{
    if(pRootElement == NULL)
    {
        return NULL;
    }

    TiXmlElement* pTiXmlElement = NULL;

    if(NULL == pNextTiXmlElement)
    {
        pTiXmlElement = pRootElement->FirstChildElement(pName);
        pNextTiXmlElement = pTiXmlElement;
    }
    else
    {
        pTiXmlElement  = pNextTiXmlElement->NextSiblingElement();
        pNextTiXmlElement = pTiXmlElement;
    }

    if(NULL != pTiXmlElement)
    {
        return (char* )pTiXmlElement->Attribute(pAttrName);
    }

    return NULL;
}

CDataManager* CDataManager::GetInstance()
{
    if (NULL == m_pInstance)
    {
        m_pInstance = new CDataManager();
    }

    return m_pInstance;
}

//获取保存数据时间间隔
uint32 CDataManager::GetTimeInterval()
{
    return m_u4TimeInterval;
}

