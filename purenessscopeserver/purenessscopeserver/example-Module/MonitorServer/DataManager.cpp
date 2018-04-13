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
        mapIP2NodeData* pMapIP2NodeData = (mapIP2NodeData*)(&itGroupNodeData->second);

        for (mapIP2NodeData::iterator itIP2Node = pMapIP2NodeData->begin();
             itIP2Node != pMapIP2NodeData->end(); )
        {
            PssNodeInfoList* pNodeInfo = (PssNodeInfoList*)itIP2Node->second;
            delete pNodeInfo;
            pNodeInfo = NULL;

            pMapIP2NodeData->erase(itIP2Node++);
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

    pData = this->GetData(m_pRootElement, "serializationfile", "filepath");

    if (NULL != pData)
    {
        m_strSerializationFile = pData;
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CDataManager::ParseXmlFile]serializationfile filepath is Invalid!!, please check monitor.xml.\n"));
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

                m_mapIP2ServerName.insert(make_pair(strIP, strName));
                m_mapIP2GroupName.insert(make_pair(strIP, strGroupName));
                PssNodeInfoList* pNodeInfo = new PssNodeInfoList;

                if(pNodeInfo != NULL)
                {
                    pNodeInfo->Init(u4Count);
                    objMapIP2NodeData.insert(make_pair(strIP, pNodeInfo));
                }
                else
                {
                    OUR_DEBUG((LM_INFO, "[CDataManager::ParseXmlFile]pNodeInfo is NULL.\n"));
                    return false;
                }
            }

            m_mapGroupNodeData.insert(make_pair(strGroupName, objMapIP2NodeData));
        }
        else
        {
            break;
        }
    }

    mapIP2NodeData objMapIP2NodeData;
    m_mapGroupNodeData.insert(make_pair("UnkownGroup", objMapIP2NodeData));

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
    mapIP2GroupName::iterator itIP2Group = m_mapIP2GroupName.find(string(pIP));

    if (itIP2Group != m_mapIP2GroupName.end())
    {
        string strGroupName = itIP2Group->second;
        mapGroupNodeData::iterator itGroupNodeData = m_mapGroupNodeData.find(strGroupName);

        if (itGroupNodeData != m_mapGroupNodeData.end())
        {
            mapIP2NodeData* pMapIP2NodeData = (mapIP2NodeData*)(&itGroupNodeData->second);
            mapIP2NodeData::iterator itIP2Node = pMapIP2NodeData->find(string(pIP));

            if (itIP2Node != pMapIP2NodeData->end())
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

            mapIP2NodeData* pMapIP2NodeData = (mapIP2NodeData*)(&itGroupNodeData->second);
            mapIP2NodeData::iterator itIP2Node = pMapIP2NodeData->find(string(pIP));

            if (itIP2Node != pMapIP2NodeData->end())
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
                    pMapIP2NodeData->insert(make_pair(pIP, pNodeInfo));
                }
            }
        }
    }
}

//生成index html文件
void CDataManager::make_index_html()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadWriteLock);
    HtmlDocument doc;
    // Generate a document structure.
    HtmlDocument::Element* head = doc.root()->AddChild("head");
    HtmlDocument::Element* meta = head->AddChild("meta");
    meta->AddAttribute("charset", "utf-8");
    HtmlDocument::Element* title = head->AddChild("title");
    string strTitle = ConvertGBKToUtf8("Pss集群监控中心");
    title->AddTextChild(strTitle);

    // Generate a document structure.
    HtmlDocument::Element* body = doc.root()->AddChild("body");
    HtmlDocument::Element* div = body->AddChild("div");
    div->AddAttribute("id","Group");
    div->AddAttribute("style", "position:relative;overflow:hidden;width:800px;height:400px;padding:0px;margin:0px;border-width:0px;cursor:default;");
    HtmlDocument::Element* span1 = div->AddChild("span");
    HtmlDocument::Element* span2 = span1->AddChild("span");
    span2->AddAttribute("style", "font-weight:400;");
    HtmlDocument::Element* strong = span2->AddChild("strong");
    string strGroupName =  ConvertGBKToUtf8("分组名称");
    strong->AddTextChild(strGroupName);
    HtmlDocument::Element* br = div->AddChild("br");
    HtmlDocument::Element* table = div->AddChild("table");
    table->AddAttribute("style", "width:100%;");
    table->AddAttribute("cellpadding", "0");
    table->AddAttribute("cellspacing", "0");
    table->AddAttribute("border", "1");
    table->AddAttribute("bordercolor", "#000000");

    //标题栏
    HtmlDocument::Element* tr = table->AddChild("tr");
    HtmlDocument::Element* td1 = tr->AddChild("td");
    string strServerName =  ConvertGBKToUtf8("机组名");
    td1->AddTextChild(strServerName);

    HtmlDocument::Element* td2 = tr->AddChild("td");
    string strHostIp =  ConvertGBKToUtf8("主机IP");
    td2->AddTextChild(strHostIp);

    HtmlDocument::Element* td3 = tr->AddChild("td");
    string strServerConnect =  ConvertGBKToUtf8("连接数");
    td3->AddTextChild(strServerConnect);

    HtmlDocument::Element* td4 = tr->AddChild("td");
    td4->AddTextChild("Cpu");

    HtmlDocument::Element* td5 = tr->AddChild("td");
    string strServerMemory =  ConvertGBKToUtf8("内存");
    td5->AddTextChild(strServerMemory);

    HtmlDocument::Element* td6 = tr->AddChild("td");
    string strServerFlowIn =  ConvertGBKToUtf8("单位时间数据流入(字节)");
    td6->AddTextChild(strServerFlowIn);

    HtmlDocument::Element* td7 = tr->AddChild("td");
    string strServerFlowOut =  ConvertGBKToUtf8("单位时间数据流出(字节)");
    td7->AddTextChild(strServerFlowOut);

    HtmlDocument::Element* td8 = tr->AddChild("td");
    string strServerDetail =  ConvertGBKToUtf8("详细信息");
    td8->AddTextChild(strServerDetail);

    //具体数据
    for (mapGroupNodeData::iterator itGroupNodeData = m_mapGroupNodeData.begin();
         itGroupNodeData != m_mapGroupNodeData.end(); itGroupNodeData++)
    {
        mapIP2NodeData* pMapIP2NodeData = (mapIP2NodeData*)(&itGroupNodeData->second);

        for (mapIP2NodeData::iterator itIP2Node = pMapIP2NodeData->begin();
             itIP2Node != pMapIP2NodeData->end(); itIP2Node++)
        {
            char szTmp[32] = {0};
            PssNodeInfoList* pNodeInfo = (PssNodeInfoList*)itIP2Node->second;
            vector<PssNodeInfoSt> vecObject;
            pNodeInfo->GetAllSavingObject(vecObject);

            if(vecObject.size() >0 )
            {
                HtmlDocument::Element* tr = table->AddChild("tr");
                HtmlDocument::Element* td1 = tr->AddChild("td");
                string strServerName =  ConvertGBKToUtf8(itGroupNodeData->first);
                td1->AddTextChild(strServerName);

                HtmlDocument::Element* td2 = tr->AddChild("td");
                td2->AddTextChild(vecObject[0].m_szClientIP);

                HtmlDocument::Element* td3 = tr->AddChild("td");
                sprintf(szTmp, "%d", vecObject[0].m_u4ConnectCount);
                td3->AddTextChild(szTmp);

                HtmlDocument::Element* td4 = tr->AddChild("td");
                sprintf(szTmp, "%d", vecObject[0].m_u4Cpu);
                td4->AddTextChild(szTmp);

                HtmlDocument::Element* td5 = tr->AddChild("td");
                sprintf(szTmp, "%d", vecObject[0].m_u4MemorySize);
                td5->AddTextChild(szTmp);

                HtmlDocument::Element* td6 = tr->AddChild("td");
                sprintf(szTmp, "%d", vecObject[0].m_u4DataIn);
                td6->AddTextChild(szTmp);

                HtmlDocument::Element* td7 = tr->AddChild("td");
                sprintf(szTmp, "%d", vecObject[0].m_u4DataOut);
                td7->AddTextChild(szTmp);

                HtmlDocument::Element* td8 = tr->AddChild("td");
                HtmlDocument::Element* td8a = td8->AddChild("a");
                string strUrl;
                mapIP2ServerName::iterator itIP2ServerName = m_mapIP2ServerName.find(vecObject[0].m_szClientIP);

                if (itIP2ServerName != m_mapIP2ServerName.end())
                {
                    strUrl =  m_strHtmlDetailPath + string(itIP2ServerName->second) + string(".html");
                }
                else
                {
                    strUrl =  m_strHtmlDetailPath + string(vecObject[0].m_szClientIP) + string(".html");
                }

                td8a->AddAttribute("href", strUrl);
                td8a->AddAttribute("target", "_blank");
                string strServerDetail =  ConvertGBKToUtf8("详细信息");
                td8->AddTextChild(strServerDetail);
            }
        }
    }

    // Convert the document to an HTML formatted string.
    string html_string;
    doc.GetHTML(html_string);
    string strIndexFile = m_strHtmlIndexPath + m_strHtmlIndexName;
    FILE* pFile = fopen(strIndexFile.c_str(), "w");

    fwrite(html_string.c_str(), sizeof(char), html_string.length(), pFile);

    fclose(pFile);
    return ;
}

//生成detail html文件
void CDataManager::make_detail_html()
{
    //生成细节html文件

    //获得所有的群组
    for (mapGroupNodeData::iterator b = m_mapGroupNodeData.begin(); b != m_mapGroupNodeData.end(); ++b)
    {
        mapIP2NodeData& objmapIP2NodeData = (mapIP2NodeData&)b->second;

        //遍历群组列表
        for (mapIP2NodeData::iterator nb = objmapIP2NodeData.begin(); nb != objmapIP2NodeData.end(); ++nb)
        {
            PssNodeInfoList* pPssNodeInfoList = (PssNodeInfoList*)nb->second;

            if (NULL != pPssNodeInfoList)
            {
                vector<PssNodeInfoSt> vecPssNodeInfoList;
                pPssNodeInfoList->GetAllSavingObject(vecPssNodeInfoList);

                if (vecPssNodeInfoList.size() > 0)
                {
                    //生成细节页面
                    string strDetailFileName;
                    string strHostName;
                    mapIP2ServerName::iterator itIP2ServerName = m_mapIP2ServerName.find(vecPssNodeInfoList[0].m_szClientIP);

                    if (itIP2ServerName != m_mapIP2ServerName.end())
                    {
                        strDetailFileName = m_strHtmlDetailPath + string(itIP2ServerName->second) + string(".html");
                        strHostName       = string(itIP2ServerName->second);
                    }
                    else
                    {
                        strDetailFileName = m_strHtmlDetailPath + string(vecPssNodeInfoList[0].m_szClientIP) + string(".html");
                        strHostName       = string(vecPssNodeInfoList[0].m_szClientIP);
                    }

                    HtmlDocument doc;
                    // Generate a document structure.
                    HtmlDocument::Element* head = doc.root()->AddChild("head");
                    HtmlDocument::Element* meta = head->AddChild("meta");
                    meta->AddAttribute("charset", "utf-8");
                    HtmlDocument::Element* title = head->AddChild("title");
                    string strTitle = ConvertGBKToUtf8(strHostName);
                    title->AddTextChild(strTitle);

                    //添加JS代码

                    string html_string;
                    doc.GetHTML(html_string);
                    FILE* pFile = fopen(strDetailFileName.c_str(), "w");

                    fwrite(html_string.c_str(), sizeof(char), html_string.length(), pFile);

                    fclose(pFile);
                }
            }
        }
    }
}

void CDataManager::Serialization()
{
    if (m_strSerializationFile.length() == 0)
    {
        OUR_DEBUG((LM_INFO, "[CDataManager::Serialization]no find Serialization.\n"));
        return;
    }

    FILE* pFile = ACE_OS::fopen(m_strSerializationFile.c_str(), "w");

    if (NULL == pFile)
    {
        OUR_DEBUG((LM_INFO, "[CDataManager::Serialization]can't create Serialization(%s).\n", m_strSerializationFile.c_str()));
        return;
    }

    //序列化当前的文件

    //获得所有的群组
    for (mapGroupNodeData::iterator b = m_mapGroupNodeData.begin(); b != m_mapGroupNodeData.end(); ++b)
    {
        mapIP2NodeData& objmapIP2NodeData = (mapIP2NodeData& )b->second;

        //遍历群组列表
        for (mapIP2NodeData::iterator nb = objmapIP2NodeData.begin(); nb != objmapIP2NodeData.end(); ++nb)
        {
            PssNodeInfoList* pPssNodeInfoList = (PssNodeInfoList*)nb->second;

            if (NULL != pPssNodeInfoList)
            {
                vector<PssNodeInfoSt> vecPssNodeInfoList;
                pPssNodeInfoList->GetAllSavingObject(vecPssNodeInfoList);

                for (int i = 0; i < (int)vecPssNodeInfoList.size(); i++)
                {
                    char szSData[MAX_BUFF_1024] = { '\0' };
                    int nSize = MAX_BUFF_1024;

                    vecPssNodeInfoList[i].Serialization(szSData, nSize);
                    ACE_OS::fwrite(szSData, sizeof(char), ACE_OS::strlen(szSData), pFile);
                }
            }
        }
    }

    ACE_OS::fclose(pFile);
}

void CDataManager::UnSerialization()
{
    if (m_strSerializationFile.length() == 0)
    {
        OUR_DEBUG((LM_INFO, "[CDataManager::UnSerialization]no find Serialization.\n"));
        return;
    }

    std::ifstream fin(m_strSerializationFile.c_str(), std::ios::in);

    char szLine[1024] = { 0 };

    //按行读取配置文件
    while (fin.getline(szLine, sizeof(szLine)))
    {
        PssNodeInfoSt objPssNodeInfoSt;

        objPssNodeInfoSt.UnSerialization(szLine, (int)ACE_OS::strlen(szLine));

        //插入到当前内存中
    }

    fin.clear();
    fin.close();
}

char* CDataManager::GetData(TiXmlElement* pRootElement, const char* pName, const char* pAttrName)
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

