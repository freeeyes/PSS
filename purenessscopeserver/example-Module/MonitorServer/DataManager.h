#ifndef _DATAMANAGER_H_
#define _DATAMANAGER_H_

#include "define.h"
#include "ObjectLru.h"
#include "HtmlPraseDoc.h"
#include "ace/Date_Time.h"
#include "GBKtoUTF8.h"

#include "tinyxml.h"
#include "tinystr.h"

#ifndef WIN32
#include <iconv.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <iostream>
using namespace std;

#define ECHART_JSON_SIZE MAX_BUFF_1024*2

//PSS节点监控信息
typedef struct PSSNODEINFO
{
    char           m_szClientIP[MAX_BUFF_50];   //客户端的IP地址
    uint32         m_u4Cpu;
    uint32         m_u4MemorySize;
    uint32         m_u4ConnectCount;
    uint32         m_u4DataIn;
    uint32         m_u4DataOut;
    ACE_Time_Value m_tvRecvTime;

    PSSNODEINFO()
    {
        m_szClientIP[0] = '\0';
        m_u4Cpu = 0;
        m_u4MemorySize = 0;
        m_u4ConnectCount = 0;
        m_u4DataIn = 0;
        m_u4DataOut = 0;
        m_tvRecvTime = 0;
    }

    //拷贝构造函数
    PSSNODEINFO(const PSSNODEINFO& ar)
    {
        sprintf_safe(this->m_szClientIP, MAX_BUFF_50, "%s", ar.m_szClientIP);
        this->m_u4Cpu = ar.m_u4Cpu;
        this->m_u4MemorySize = ar.m_u4MemorySize;
        this->m_u4ConnectCount = ar.m_u4ConnectCount;
        this->m_u4DataIn = ar.m_u4DataIn;
        this->m_u4DataOut = ar.m_u4DataOut;
        this->m_tvRecvTime = ar.m_tvRecvTime;
    }

    PSSNODEINFO& operator = (const PSSNODEINFO& ar)
    {
        sprintf_safe(this->m_szClientIP, MAX_BUFF_50, "%s", ar.m_szClientIP);
        this->m_u4Cpu = ar.m_u4Cpu;
        this->m_u4MemorySize = ar.m_u4MemorySize;
        this->m_u4ConnectCount = ar.m_u4ConnectCount;
        this->m_u4DataIn = ar.m_u4DataIn;
        this->m_u4DataOut = ar.m_u4DataOut;
        this->m_tvRecvTime = ar.m_tvRecvTime;
        return *this;
    }

    void Serialization(char* pData, int nSize)
    {
        sprintf_safe(pData, nSize, "%s,%d,%d,%d,%d,%d,%ld,\n",
                     m_szClientIP,
                     m_u4Cpu,
                     m_u4MemorySize,
                     m_u4ConnectCount,
                     m_u4DataIn,
                     m_u4DataOut,
                     m_tvRecvTime.sec());
    }

    void UnSerialization(char* pData, int nSize)
    {
        vector<string> vecData;
        SplitData(pData, vecData);

        if (vecData.size() == 7)
        {
            sprintf_safe(m_szClientIP, MAX_BUFF_50, "%s", vecData[0].c_str());
            m_u4Cpu = atoi(vecData[1].c_str());
            m_u4MemorySize = atoi(vecData[2].c_str());
            m_u4ConnectCount = atoi(vecData[3].c_str());
            m_u4DataIn = atoi(vecData[4].c_str());
            m_u4DataOut = atoi(vecData[5].c_str());

            //获得时间
            m_tvRecvTime = ACE_Time_Value(((time_t)atol(vecData[6].c_str())));
        }
    }

private:
    void SplitData(char* pData, vector<string>& vecData)
    {
        vecData.clear();
        const char* d = ",";
        char* p = NULL;
        p = strtok(pData, d);

        while (p)
        {
            string strData = p;
            vecData.push_back(strData);
            p = strtok(NULL, d);
        }
    }

} PssNodeInfoSt;

class CDataManager
{
public:
    ~CDataManager();

    void Close();

    //解析xml配置文件
    bool ParseXmlFile(const char* pXmlFile);

    //增加节点数据
    void AddNodeDate(const char* pIP, uint32 u4Cpu, uint32 u4MemorySize, uint32 u4ConnectCount, uint32 u4DataIn, uint32 u4DataOut);

    //生成index html文件
    void make_index_html();

    //生成detail html文件
    void make_detail_html();

    //生成序列化文件
    void Serialization();

    //反序化
    void UnSerialization();

private:
    static CDataManager* m_pInstance;

    //控制多线程锁
    ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;

    typedef CObjectLruList<PssNodeInfoSt, ACE_Null_Mutex> PssNodeInfoList;
    typedef map<string, string> mapIP2GroupName;
    typedef map<string, PssNodeInfoList*> mapIP2NodeData;
    typedef map<string, mapIP2NodeData> mapGroupNodeData;
    typedef map<string, string> mapIP2ServerName;

    //ip到组名的映射
    mapIP2GroupName m_mapIP2GroupName;

    //组到对应的节点信息的映射
    mapGroupNodeData m_mapGroupNodeData;

    //ip到servername的映射
    mapIP2ServerName m_mapIP2ServerName;

private:
    CDataManager();

    char* GetData(TiXmlElement* pRootElement, const char* pName, const char* pAttrName);
    char* GetData(TiXmlElement* pRootElement, const char* pName, const char* pAttrName, TiXmlElement*& pNextTiXmlElement);

    bool Save_Detail_Html(string strServerName, string strFilePath);
    bool Save_Json_Data(string strServerName, vector<PssNodeInfoSt>& vecPssNodeInfoList);
    string Make_detail_JS_Code(string strServerName);

public:
    static CDataManager* GetInstance();

    //获取保存数据时间间隔
    uint32 GetTimeInterval();
private:
    string m_strHtmlIndexPath;      //htmlindex文件的路径
    string m_strHtmlIndexName;      //htmlindex文件的名称
    string m_strHtmlDetailPath;     //htmldetail文件的路径
    string m_strRelativeDetailPath; //htmldetail的相对路径
    string m_strHtmlJsonPath;       //htmlJson文件的路径
    string m_strRelativeJsonPath;   //htmldetail的相对路径
    string m_strSerializationFile;  //序列化存储文件位置
    string m_strJSEchart;           //JS路径
    string m_strJSJQuery;           //JS路径
    uint32 m_u4HistoryMaxCount;
    uint32 m_u4TimeInterval;

    TiXmlDocument* m_pTiXmlDocument;
    TiXmlElement*  m_pRootElement;
};

#endif //_DATAMANAGER_H_
