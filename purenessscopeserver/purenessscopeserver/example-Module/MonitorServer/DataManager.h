#ifndef _DATAMANAGER_H_
#define _DATAMANAGER_H_

#include "define.h"
#include "tinyxml.h"
#include "tinystr.h"
#include <vector>
#include <map>
using namespace std;

class CDataManager
{
public:
    ~CDataManager();

    //解析xml配置文件
    bool ParseXmlFile(const char* pXmlFile);

    //增加节点数据
    void AddNodeDate(uint32 u4ConnectId,uint32 u4Cpu,uint32 u4MemorySize,uint32 u4ConnectCount,uint32 u4DataIn,uint32 u4DataOut);

    //生成index html文件
    void make_index_html();

    //生成detail html文件
    void make_detail_html();

private:
    CDataManager();
public:
    static CDataManager* GetInstance();

    //获取保存数据时间间隔
    uint32 GetTimeInterval();
private:
    static CDataManager* m_pInstance;

    //控制多线程锁
    ACE_Recursive_Thread_Mutex  m_ThreadWriteLock;

    typedef map<uint32,string> mapConnectId2IP;

    //保存连接id到IP的映射关系
    mapConnectId2IP m_mapConnectId2IP;

};

#endif //_DATAMANAGER_H_
