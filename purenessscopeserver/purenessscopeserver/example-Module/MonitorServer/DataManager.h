#ifndef _DATAMANAGER_H_
#define _DATAMANAGER_H_

#include "define.h"
#include "tinyxml.h"
#include "tinystr.h"

class CDataManager 
{
public:
    CDataManager();
    ~CDataManager();

    //解析xml配置文件
    bool ParseXmlFile(const char* pXmlFile);

    //增加节点数据
    void AddNodeDate(const char* pIP,uint32 u4Cpu,uint32 u4MemorySize,uint32 u4ConnectCount,uint32 u4DataIn,uint32 u4DataOut);

    //生成index html文件
    void make_index_html();

    //生成detail html文件
    void make_detail_html();

private:

};

#endif //_DATAMANAGER_H_
