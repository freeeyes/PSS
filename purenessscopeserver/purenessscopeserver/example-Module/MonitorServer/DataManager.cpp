#include "DataManager.h"

CDataManager* CDataManager::m_pInstance = NULL;

CDataManager::CDataManager()
{

}
CDataManager::~CDataManager()
{
    if (NULL != m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = NULL;
    }
}

//解析xml配置文件
bool CDataManager::ParseXmlFile(const char* pXmlFile)
{
    return true;
}

//增加节点数据
void CDataManager::AddNodeDate(const char* pIP,uint32 u4Cpu,uint32 u4MemorySize,uint32 u4ConnectCount,uint32 u4DataIn,uint32 u4DataOut)
{

}

//生成index html文件
void CDataManager::make_index_html()
{

}

//生成detail html文件
void CDataManager::make_detail_html()
{
    
}

CDataManager* CDataManager::GetInstance()
{
    if (NULL == m_pInstance)
    {
        m_pInstance = new CDataManager();
    }

    return m_pInstance;
}