#include "XmlOpeation.h"

CXmlOpeation::CXmlOpeation(void)
{
    m_pTiXmlDocument = NULL;
    m_pRootElement   = NULL;
}

CXmlOpeation::~CXmlOpeation(void)
{
    Close();
}

bool CXmlOpeation::Init(const char* pFileName)
{
    Close();
    m_pTiXmlDocument = new TiXmlDocument(pFileName);

    if(NULL == m_pTiXmlDocument)
    {
        return false;
    }

    if(false == m_pTiXmlDocument->LoadFile())
    {
        return false;
    }

    //获得根元素
    m_pRootElement = m_pTiXmlDocument->RootElement();

    return true;
}

void CXmlOpeation::Close()
{
    if(NULL != m_pTiXmlDocument)
    {
        delete m_pTiXmlDocument;
        m_pTiXmlDocument = NULL;
        m_pRootElement   = NULL;
    }
}

char* CXmlOpeation::GetData(const char* pName, const char* pAttrName)
{
    if(m_pRootElement == NULL)
    {
        return NULL;
    }

    TiXmlElement* pTiXmlElement = m_pRootElement->FirstChildElement(pName);

    if(NULL != pTiXmlElement)
    {
        return (char* )pTiXmlElement->Attribute(pAttrName);
    }

    return NULL;
}

char* CXmlOpeation::GetData( const char* pName, const char* pAttrName, TiXmlElement*& pNextTiXmlElement )
{
    if(m_pRootElement == NULL)
    {
        return NULL;
    }

    TiXmlElement* pTiXmlElement = NULL;

    if(NULL == pNextTiXmlElement)
    {
        pTiXmlElement = m_pRootElement->FirstChildElement(pName);
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


