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
    if (NULL != m_pTiXmlDocument)
    {
        delete m_pTiXmlDocument;
        m_pTiXmlDocument = NULL;
        m_pRootElement = NULL;
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

    if (NULL == m_pTiXmlDocument)
    {
        return false;
    }

    if (false == m_pTiXmlDocument->LoadFile())
    {
        OUR_DEBUG((LM_INFO, "[CDataManager::ParseXmlFile]LoadFile\n"));
        return false;
    }

    //获得根元素
    m_pRootElement = m_pTiXmlDocument->RootElement();

    char* pData = NULL;
    pData = this->GetData(m_pRootElement, "htmlIndex", "path");

    if (NULL != pData)
    {
        m_strHtmlIndexPath = pData;
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CDataManager::ParseXmlFile]htmlIndex path is Invalid!!, please check monitor.xml.\n"));
        return false;
    }

    pData = this->GetData(m_pRootElement, "htmlIndex", "name");

    if (NULL != pData)
    {
        m_strHtmlIndexName = pData;
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CDataManager::ParseXmlFile]htmlIndex name is Invalid!!, please check monitor.xml.\n"));
        return false;
    }

    pData = this->GetData(m_pRootElement, "htmldetail", "path");

    if (NULL != pData)
    {
        m_strHtmlDetailPath = pData;
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CDataManager::ParseXmlFile]htmldetail path is Invalid!!, please check monitor.xml.\n"));
        return false;
    }

    pData = this->GetData(m_pRootElement, "htmldetail", "relativepath");

    if (NULL != pData)
    {
        m_strRelativeDetailPath = pData;
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CDataManager::ParseXmlFile]htmldetail relativepath path is Invalid!!, please check monitor.xml.\n"));
        return false;
    }

    pData = this->GetData(m_pRootElement, "htmlJsondetail", "path");

    if (NULL != pData)
    {
        m_strHtmlJsonPath = pData;
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CDataManager::ParseXmlFile]htmlJsondetail path is Invalid!!, please check monitor.xml.\n"));
        return false;
    }

    pData = this->GetData(m_pRootElement, "htmlJsondetail", "relativepath");

    if (NULL != pData)
    {
        m_strRelativeJsonPath = pData;
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CDataManager::ParseXmlFile]htmlJsondetail relativepath path is Invalid!!, please check monitor.xml.\n"));
        return false;
    }

    pData = this->GetData(m_pRootElement, "defaultcfg", "historymaxcount");

    if (NULL != pData)
    {
        m_u4HistoryMaxCount = (uint16)ACE_OS::atoi(pData);
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CDataManager::ParseXmlFile]defaultcfg historymaxcount is Invalid!!, please check monitor.xml.\n"));
        return false;
    }

    pData = this->GetData(m_pRootElement, "defaultcfg", "timeinterval");

    if (NULL != pData)
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

    pData = this->GetData(m_pRootElement, "JsEchart", "src");

    if (NULL != pData)
    {
        m_strJSEchart = pData;
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CDataManager::ParseXmlFile]JsEchart is Invalid!!, please check monitor.xml.\n"));
        return false;
    }

    pData = this->GetData(m_pRootElement, "JsJquery", "src");

    if (NULL != pData)
    {
        m_strJSJQuery = pData;
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CDataManager::ParseXmlFile]JsJquery is Invalid!!, please check monitor.xml.\n"));
        return false;
    }

    TiXmlElement* pNextGroupName = NULL;
    TiXmlElement* pSubElement = NULL;

    while (true)
    {
        if (NULL == pNextGroupName)
        {
            pSubElement = m_pRootElement->FirstChildElement("group");
        }
        else
        {
            pSubElement = pNextGroupName->NextSiblingElement();
        }

        string strGroupName;
        pData = this->GetData(m_pRootElement, "group", "name", pNextGroupName);

        if (pData != NULL)
        {
            strGroupName = pData;

            TiXmlElement* pNextTiXmlElementName = NULL;
            TiXmlElement* pNextTiXmlElementIP = NULL;
            TiXmlElement* pNextTiXmlElementCount = NULL;

            mapIP2NodeData objMapIP2NodeData;

            while (true)
            {
                string strName;
                pData = this->GetData(pSubElement, "ServerInfo", "name", pNextTiXmlElementName);

                if (pData != NULL)
                {
                    strName = pData;
                }
                else
                {
                    break;
                }

                string strIP;
                pData = this->GetData(pSubElement, "ServerInfo", "ip", pNextTiXmlElementIP);

                if (pData != NULL)
                {
                    strIP = pData;
                }
                else
                {
                    break;
                }

                uint32 u4Count;
                pData = this->GetData(pSubElement, "ServerInfo", "historymaxcount", pNextTiXmlElementCount);

                if (pData != NULL)
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

                if (pNodeInfo != NULL)
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
void CDataManager::AddNodeDate(const char* pIP, uint32 u4Cpu, uint32 u4MemorySize, uint32 u4ConnectCount, uint32 u4DataIn, uint32 u4DataOut)
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
        OUR_DEBUG((LM_INFO, "[CDataManager::AddNodeDate]pIP:%s is not found.\n", pIP));
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

                if (pNodeInfo != NULL)
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
    div->AddAttribute("id", "Group");
    div->AddAttribute("style", "position:relative;overflow:hidden;width:800px;height:400px;padding:0px;margin:0px;border-width:0px;cursor:default;");
    HtmlDocument::Element* span1 = div->AddChild("span");
    HtmlDocument::Element* span2 = span1->AddChild("span");
    span2->AddAttribute("style", "font-weight:400;");
    HtmlDocument::Element* strong = span2->AddChild("strong");
    string strGroupName = ConvertGBKToUtf8("分组名称");
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
    string strServerName = ConvertGBKToUtf8("机组名");
    td1->AddTextChild(strServerName);

    HtmlDocument::Element* td2 = tr->AddChild("td");
    string strHostIp = ConvertGBKToUtf8("主机IP");
    td2->AddTextChild(strHostIp);

    HtmlDocument::Element* td3 = tr->AddChild("td");
    string strServerConnect = ConvertGBKToUtf8("连接数");
    td3->AddTextChild(strServerConnect);

    HtmlDocument::Element* td4 = tr->AddChild("td");
    td4->AddTextChild("Cpu");

    HtmlDocument::Element* td5 = tr->AddChild("td");
    string strServerMemory = ConvertGBKToUtf8("内存");
    td5->AddTextChild(strServerMemory);

    HtmlDocument::Element* td6 = tr->AddChild("td");
    string strServerFlowIn = ConvertGBKToUtf8("单位时间数据流入(字节)");
    td6->AddTextChild(strServerFlowIn);

    HtmlDocument::Element* td7 = tr->AddChild("td");
    string strServerFlowOut = ConvertGBKToUtf8("单位时间数据流出(字节)");
    td7->AddTextChild(strServerFlowOut);

    HtmlDocument::Element* td8 = tr->AddChild("td");
    string strServerDetail = ConvertGBKToUtf8("详细信息");
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
                    strUrl = m_strRelativeDetailPath + "/" + string(itIP2ServerName->second) + string(".html");
                }
                else
                {
                    strUrl = m_strRelativeDetailPath + "/" + string(vecObject[0].m_szClientIP) + string(".html");
                }

                td8a->AddAttribute("href", strUrl);
                td8a->AddAttribute("target", "_blank");
                string strServerDetail =  ConvertGBKToUtf8("detail");
                td8a->AddTextChild(strServerDetail);
            }
        }
    }

    // Convert the document to an HTML formatted string.
    string html_string;
    doc.GetHTML(html_string);
    string strIndexFile = m_strHtmlIndexPath + "/" + m_strHtmlIndexName;
    FILE* pFile = fopen(strIndexFile.c_str(), "w");

    fwrite(html_string.c_str(), sizeof(char), html_string.length(), pFile);

    fclose(pFile);
    return;
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
                        strDetailFileName = m_strHtmlDetailPath + "/" + string(itIP2ServerName->second) + string(".html");
                        strHostName = string(itIP2ServerName->second);
                    }
                    else
                    {
                        strDetailFileName = m_strHtmlDetailPath + "/" + string(vecPssNodeInfoList[0].m_szClientIP) + string(".html");
                        strHostName = string(vecPssNodeInfoList[0].m_szClientIP);
                    }

                    //生成细节html文件
                    Save_Detail_Html(strHostName, strDetailFileName);

                    //生成Json
                    Save_Json_Data(strHostName, vecPssNodeInfoList);
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
        mapIP2NodeData& objmapIP2NodeData = (mapIP2NodeData&)b->second;

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
    if (pRootElement == NULL)
    {
        return NULL;
    }

    TiXmlElement* pTiXmlElement = pRootElement->FirstChildElement(pName);

    if (NULL != pTiXmlElement)
    {
        return (char*)pTiXmlElement->Attribute(pAttrName);
    }

    return NULL;
}

char* CDataManager::GetData(TiXmlElement* pRootElement, const char* pName, const char* pAttrName, TiXmlElement*& pNextTiXmlElement)
{
    if (pRootElement == NULL)
    {
        return NULL;
    }

    TiXmlElement* pTiXmlElement = NULL;

    if (NULL == pNextTiXmlElement)
    {
        pTiXmlElement = pRootElement->FirstChildElement(pName);
        pNextTiXmlElement = pTiXmlElement;
    }
    else
    {
        pTiXmlElement = pNextTiXmlElement->NextSiblingElement();
        pNextTiXmlElement = pTiXmlElement;
    }

    if (NULL != pTiXmlElement)
    {
        return (char*)pTiXmlElement->Attribute(pAttrName);
    }

    return NULL;
}

bool CDataManager::Save_Detail_Html(string strServerName, string strFilePath)
{
    HtmlDocument doc;
    // Generate a document structure.
    HtmlDocument::Element* head = doc.root()->AddChild("head");
    HtmlDocument::Element* meta = head->AddChild("meta");
    meta->AddAttribute("charset", "utf-8");
    HtmlDocument::Element* title = head->AddChild("title");
    string strTitle = ConvertGBKToUtf8(strServerName);
    title->AddTextChild(strTitle);

    //添加JS代码
    HtmlDocument::Element* jsechart = head->AddChild("script");
    jsechart->AddAttribute("src", m_strJSEchart);
    HtmlDocument::Element* jsejquery = head->AddChild("script");
    jsejquery->AddAttribute("src", m_strJSJQuery);

    HtmlDocument::Element* jscode = doc.root()->AddChild("script");
    jscode->AddAttribute("type", "text/javascript");
    //添加JS函数
    jscode->AddTextChild(Make_detail_JS_Code(strTitle));

    HtmlDocument::Element* body = doc.root()->AddChild("body");
    HtmlDocument::Element* divCpu = body->AddChild("div");
    divCpu->AddAttribute("id", "Cpu");
    divCpu->AddAttribute("style", "position: relative; overflow: hidden; width: 800px; height: 400px; padding: 0px; margin: 0px; border-width: 0px; cursor: default;");

    HtmlDocument::Element* divMemory = body->AddChild("div");
    divMemory->AddAttribute("id", "Memory");
    divMemory->AddAttribute("style", "position: relative; overflow: hidden; width: 800px; height: 400px; padding: 0px; margin: 0px; border-width: 0px; cursor: default;");

    HtmlDocument::Element* divConnect = body->AddChild("div");
    divConnect->AddAttribute("id", "Connect");
    divConnect->AddAttribute("style", "position: relative; overflow: hidden; width: 800px; height: 400px; padding: 0px; margin: 0px; border-width: 0px; cursor: default;");

    HtmlDocument::Element* divFlow = body->AddChild("div");
    divFlow->AddAttribute("id", "Flow");
    divFlow->AddAttribute("style", "position: relative; overflow: hidden; width: 800px; height: 400px; padding: 0px; margin: 0px; border-width: 0px; cursor: default;");

    string html_string;
    doc.GetHTML(html_string);
    FILE* pFile = fopen(strFilePath.c_str(), "w");

    fwrite(html_string.c_str(), sizeof(char), html_string.length(), pFile);

    fclose(pFile);

    return true;
}

bool CDataManager::Save_Json_Data(string strServerName, vector<PssNodeInfoSt>& vecPssNodeInfoList)
{
    string strXTime;
    string strYCpu;
    string strYMemory;
    string strYConnect;
    string strYFlowIn;
    string strYFlowOut;

    string strJsonPathFile;

    //得到X轴的描述
    for (int i = 0; i < (int)vecPssNodeInfoList.size(); i++)
    {
        ACE_Date_Time DtTime(vecPssNodeInfoList[i].m_tvRecvTime);
        char szTime[MAX_BUFF_50] = { '\0' };
        sprintf_safe(szTime, MAX_BUFF_50, "%02d:%02d:%02d",
                     DtTime.hour(),
                     DtTime.minute(),
                     DtTime.second());

        if (i < (int)vecPssNodeInfoList.size() - 1)
        {
            //加逗号
            strXTime += "\"" + (string)szTime + "\",";
        }
        else
        {
            //不加逗号
            strXTime += "\"" + (string)szTime + "\"";
        }

        char szCpu[MAX_BUFF_20] = { '\0' };
        sprintf_safe(szCpu, MAX_BUFF_20, "%d", vecPssNodeInfoList[i].m_u4Cpu);

        if (i < (int)vecPssNodeInfoList.size() - 1)
        {
            //加逗号
            strYCpu += "\"" + (string)szCpu + "\",";
        }
        else
        {
            //不加逗号
            strYCpu += "\"" + (string)szCpu + "\"";
        }

        char szMemory[MAX_BUFF_20] = { '\0' };
        sprintf_safe(szMemory, MAX_BUFF_20, "%d", vecPssNodeInfoList[i].m_u4MemorySize);

        if (i < (int)vecPssNodeInfoList.size() - 1)
        {
            //加逗号
            strYMemory += "\"" + (string)szMemory + "\",";
        }
        else
        {
            //不加逗号
            strYMemory += "\"" + (string)szMemory + "\"";
        }

        char szConnect[MAX_BUFF_20] = { '\0' };
        sprintf_safe(szConnect, MAX_BUFF_20, "%d", vecPssNodeInfoList[i].m_u4ConnectCount);

        if (i < (int)vecPssNodeInfoList.size() - 1)
        {
            //加逗号
            strYConnect += "\"" + (string)szConnect + "\",";
        }
        else
        {
            //不加逗号
            strYConnect += "\"" + (string)szConnect + "\"";
        }

        char szFlowIn[MAX_BUFF_20] = { '\0' };
        sprintf_safe(szFlowIn, MAX_BUFF_20, "%d", vecPssNodeInfoList[i].m_u4DataIn);

        if (i < (int)vecPssNodeInfoList.size() - 1)
        {
            //加逗号
            strYFlowIn += "\"" + (string)szFlowIn + "\",";
        }
        else
        {
            //不加逗号
            strYFlowIn += "\"" + (string)szFlowIn + "\"";
        }

        char szFlowOut[MAX_BUFF_20] = { '\0' };
        sprintf_safe(szFlowOut, MAX_BUFF_20, "%d", vecPssNodeInfoList[i].m_u4DataOut);

        if (i < (int)vecPssNodeInfoList.size() - 1)
        {
            //加逗号
            strYFlowOut += "\"" + (string)szFlowOut + "\",";
        }
        else
        {
            //不加逗号
            strYFlowOut += "\"" + (string)szFlowOut + "\"";
        }
    }

    strXTime = (string)"{\"type\": \"category\",\"data\": [" + strXTime + (string)"]}";
    strYCpu = (string)"{\"name\":\"" + (string)"cpu" + "\",\"type\": \"line\",\"smooth\": \"true\",\"data\": [" + strYCpu + (string)"]}";
    strYMemory = (string)"{\"name\":\"" + (string)"memory" + "\",\"type\": \"line\",\"smooth\": \"true\",\"data\": [" + strYMemory + (string)"]}";
    strYConnect = (string)"{\"name\":\"" + (string)"connect" + "\",\"type\": \"line\",\"smooth\": \"true\",\"data\": [" + strYConnect + (string)"]}";
    strYFlowIn = (string)"{\"name\":\"" + (string)"flowin" + "\",\"type\": \"line\",\"smooth\": \"true\",\"data\": [" + strYFlowIn + (string)"]}";
    strYFlowOut = (string)"{\"name\":\"" + (string)"flowout" + "\",\"type\": \"line\",\"smooth\": \"true\",\"data\": [" + strYFlowOut + (string)"]}";

    //将图表数据写入文件
    FILE* pFile = NULL;
    string strJsonText;

    strJsonText = (string)"{\"title\": {\"text\": \"" + (string)"cpu" +
                  (string)"\"},\"tooltip\" : {},\"legend\" : {\"data\":[\"time\"]},\"xAxis\" :" +
                  strXTime + ",\"yAxis\" : {\"type\":\"value\"}, \"series\": [" +
                  strYCpu.c_str() + (string)"]}";

    strJsonPathFile = m_strHtmlJsonPath + "/" + strServerName + (string)"_cpu.json";
    pFile = ACE_OS::fopen(strJsonPathFile.c_str(), "w+");

    if (NULL != pFile)
    {
        ACE_OS::fwrite(strJsonText.c_str(), sizeof(char), strJsonText.length(), pFile);
    }

    ACE_OS::fclose(pFile);

    strJsonText = (string)"{\"title\": {\"text\": \"" + (string)"memory" +
                  (string)"\"},\"tooltip\" : {},\"legend\" : {\"data\":[\"time\"]},\"xAxis\" :" +
                  strXTime + ",\"yAxis\" : {\"type\":\"value\"}, \"series\": [" +
                  strYMemory.c_str() + (string)"]}";

    strJsonPathFile = m_strHtmlJsonPath + "/" + strServerName + (string)"_memory.json";
    pFile = ACE_OS::fopen(strJsonPathFile.c_str(), "w+");

    if (NULL != pFile)
    {
        ACE_OS::fwrite(strJsonText.c_str(), sizeof(char), strJsonText.length(), pFile);
    }

    ACE_OS::fclose(pFile);

    strJsonText = (string)"{\"title\": {\"text\": \"" + (string)"connect" +
                  (string)"\"},\"tooltip\" : {},\"legend\" : {\"data\":[\"time\"]},\"xAxis\" :" +
                  strXTime + ",\"yAxis\" : {\"type\":\"value\"}, \"series\": [" +
                  strYConnect.c_str() + (string)"]}";

    strJsonPathFile = m_strHtmlJsonPath + "/" + strServerName + (string)"_connect.json";
    pFile = ACE_OS::fopen(strJsonPathFile.c_str(), "w+");

    if (NULL != pFile)
    {
        ACE_OS::fwrite(strJsonText.c_str(), sizeof(char), strJsonText.length(), pFile);
    }

    ACE_OS::fclose(pFile);

    string strFlow = strYFlowIn + (string)"," + strYFlowOut;

    strJsonText = (string)"{\"title\": {\"text\": \"" + (string)"flow" +
                  (string)"\"},\"tooltip\" : {},\"legend\" : {\"data\":[\"time\"]},\"xAxis\" :" +
                  strFlow + ",\"yAxis\" : {\"type\":\"value\"}, \"series\": [" +
                  strYConnect.c_str() + (string)"]}";

    strJsonPathFile = m_strHtmlJsonPath + "/" + strServerName + (string)"_flow.json";
    pFile = ACE_OS::fopen(strJsonPathFile.c_str(), "w+");

    if (NULL != pFile)
    {
        ACE_OS::fwrite(strJsonText.c_str(), sizeof(char), strJsonText.length(), pFile);
    }

    ACE_OS::fclose(pFile);

    return true;
}

string CDataManager::Make_detail_JS_Code(string strServerName)
{
    string strJSCode;

    //CPU
    strJSCode += "$.getJSON(\"" + m_strRelativeJsonPath + "/" + strServerName + "_cpu.json\", function(strJson){\n";
    strJSCode += "\tvar myChartcpu = echarts.init(document.getElementById('Cpu'));\n";
    strJSCode += "\tvar option = strJson;\n";
    strJSCode += "\tmyChartcpu.setOption(option);\n";
    strJSCode += "\t});\n\n";

    //memory
    strJSCode += "$.getJSON(\"" + m_strRelativeJsonPath + "/" + strServerName + "_memory.json\", function(strJson){\n";
    strJSCode += "\tvar myChartmemory = echarts.init(document.getElementById('Memory'));\n";
    strJSCode += "\tvar option = strJson;\n";
    strJSCode += "\tmyChartmemory.setOption(option);\n";
    strJSCode += "\t});\n\n";

    //connect
    strJSCode += "$.getJSON(\"" + m_strRelativeJsonPath + "/" + strServerName + "_connect.json\", function(strJson){\n";
    strJSCode += "\tvar myChartconnect = echarts.init(document.getElementById('Connect'));\n";
    strJSCode += "\tvar option = strJson;\n";
    strJSCode += "\tmyChartconnect.setOption(option);\n";
    strJSCode += "\t});\n\n";

    //flow
    strJSCode += "$.getJSON(\"" + m_strRelativeJsonPath + "/" + strServerName + "_flow.json\", function(strJson){\n";
    strJSCode += "\tvar myChartflow = echarts.init(document.getElementById('Flow'));\n";
    strJSCode += "\tvar option = strJson;\n";
    strJSCode += "\tmyChartflow.setOption(option);\n";
    strJSCode += "\t});\n\n";

    return strJSCode;
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

