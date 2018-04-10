#ifndef _DATAMANAGER_H_
#define _DATAMANAGER_H_

#include "define.h"
#include "ObjectLru.h"

#include "tinyxml.h"
#include "tinystr.h"

#include <vector>
#include <map>
using namespace std;

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
        m_szClientIP[0]  = '\0';
        m_u4Cpu          = 0;
        m_u4MemorySize   = 0;
        m_u4ConnectCount = 0;
        m_u4DataIn       = 0;
        m_u4DataOut      = 0;
        m_tvRecvTime     = 0;
    }

    //拷贝构造函数
    PSSNODEINFO(const PSSNODEINFO& ar)
    {
        sprintf_safe(this->m_szClientIP, MAX_BUFF_50, "%s", ar.m_szClientIP);
        this->m_u4Cpu        = ar.m_u4Cpu;
        this->m_u4MemorySize = ar.m_u4MemorySize;
        this->m_u4ConnectCount = ar.m_u4ConnectCount;
        this->m_u4DataIn = ar.m_u4DataIn;
        this->m_u4DataOut = ar.m_u4DataOut;
        this->m_tvRecvTime = ar.m_tvRecvTime;
    }

    PSSNODEINFO& operator = (const PSSNODEINFO& ar)
    {
        sprintf_safe(this->m_szClientIP, MAX_BUFF_50, "%s", ar.m_szClientIP);
        this->m_u4Cpu        = ar.m_u4Cpu;
        this->m_u4MemorySize = ar.m_u4MemorySize;
        this->m_u4ConnectCount = ar.m_u4ConnectCount;
        this->m_u4DataIn = ar.m_u4DataIn;
        this->m_u4DataOut = ar.m_u4DataOut;
        this->m_tvRecvTime = ar.m_tvRecvTime;
        return *this;
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
    void AddNodeDate(const char* pIP, uint32 u4Cpu,uint32 u4MemorySize,uint32 u4ConnectCount,uint32 u4DataIn,uint32 u4DataOut);

    //生成index html文件
    void make_index_html();

    //生成detail html文件
    void make_detail_html();

private:
    CDataManager();

    char* GetData(const char* pName, const char* pAttrName);
    char* GetData(const char* pName, const char* pAttrName, TiXmlElement*& pNextTiXmlElement);
public:
    static CDataManager* GetInstance();

    //获取保存数据时间间隔
    uint32 GetTimeInterval();
private:
    static CDataManager* m_pInstance;

    //控制多线程锁
    ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;

    typedef CObjectLruList<PssNodeInfoSt, ACE_Null_Mutex> PssNodeInfoList;
    typedef map<uint32,string> mapConnectId2IP;
    typedef map<string,string> mapIP2GroupName;
    typedef map<string,map<string,PssNodeInfoList*> > mapGroupNodeData;

    //连接id到IP的映射关系
    mapConnectId2IP m_mapConnectId2IP;

    //ip到组名的映射
    mapIP2GroupName m_mapIP2GroupName;

    //组到对应的节点信息的映射
    mapGroupNodeData m_mapGroupNodeData;

private:
    TiXmlDocument* m_pTiXmlDocument;
    TiXmlElement*  m_pRootElement;
};

#endif //_DATAMANAGER_H_
