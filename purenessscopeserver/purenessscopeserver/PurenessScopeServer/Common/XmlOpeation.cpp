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

bool CXmlOpeation::Read_XML_Data_Single_String(const char* pTag, const char* pName, char* pValue, int nMaxSize)
{
    char* pData = GetData(pTag, pName);

    if (pData != NULL)
    {
        sprintf_safe(pValue, nMaxSize, "%s", pData);
        return true;
    }
    else
    {
        return false;
    }
}

bool CXmlOpeation::Read_XML_Data_Single_String(const char* pTag, const char* pName, string& strValue)
{
    char* pData = GetData(pTag, pName);

    if (pData != NULL)
    {
        strValue = (string)pData;
        return true;
    }
    else
    {
        return false;
    }
}

bool CXmlOpeation::Read_XML_Data_Multiple_String(const char* pTag, const char* pName, char* pValue, int nMaxSize, TiXmlElement*& pTi)
{
    char* pData = GetData(pTag, pName, pTi);

    if (pData != NULL)
    {
        sprintf_safe(pValue, nMaxSize, "%s", pData);
        return true;
    }
    else
    {
        return false;
    }
}

bool CXmlOpeation::Read_XML_Data_Multiple_String(const char* pTag, const char* pName, string& strValue, TiXmlElement*& pTi)
{
    char* pData = GetData(pTag, pName, pTi);

    if (pData != NULL)
    {
        strValue = (string)pData;
        return true;
    }
    else
    {
        return false;
    }
}

bool CXmlOpeation::Read_XML_Data_Single_Uint32(const char* pTag, const char* pName, uint32& u4Value)
{
    char* pData = GetData(pTag, pName);

    if (pData != NULL)
    {
        u4Value = (uint32)ACE_OS::atoi(pData);
        return true;
    }
    else
    {
        return false;
    }
}

bool CXmlOpeation::Read_XML_Data_Multiple_Uint32(const char* pTag, const char* pName, uint32& u4Value, TiXmlElement*& pTi)
{
    char* pData = GetData(pTag, pName, pTi);

    if (pData != NULL)
    {
        u4Value = (uint32)atoi(pData);
        return true;
    }
    else
    {
        return false;
    }
}

bool CXmlOpeation::Read_XML_Data_Single_Uint16(const char* pTag, const char* pName, uint16& u2Value)
{
    char* pData = GetData(pTag, pName);

    if (pData != NULL)
    {
        u2Value = (uint16)ACE_OS::atoi(pData);
        return true;
    }
    else
    {
        return false;
    }
}

bool CXmlOpeation::Read_XML_Data_Multiple_Uint16(const char* pTag, const char* pName, uint16& u2Value, TiXmlElement*& pTi)
{
    char* pData = GetData(pTag, pName, pTi);

    if (pData != NULL)
    {
        u2Value = (uint16)atoi(pData);
        return true;
    }
    else
    {
        return false;
    }
}

bool CXmlOpeation::Read_XML_Data_Single_Uint8(const char* pTag, const char* pName, uint8& u1Value)
{
    char* pData = GetData(pTag, pName);

    if (pData != NULL)
    {
        u1Value = (uint8)ACE_OS::atoi(pData);
        return true;
    }
    else
    {
        return false;
    }
}

bool CXmlOpeation::Read_XML_Data_Multiple_Uint8(const char* pTag, const char* pName, uint8& u1Value, TiXmlElement*& pTi)
{
    char* pData = GetData(pTag, pName, pTi);

    if (pData != NULL)
    {
        u1Value = (uint8)atoi(pData);
        return true;
    }
    else
    {
        return false;
    }
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


