#include "XmlOpeation.h"

CXmlOpeation::CXmlOpeation(void)
{
    m_pTiXmlDocument = std::make_shared<TiXmlDocument>();
    m_pRootElement   = NULL;
}

bool CXmlOpeation::Init(const char* pFileName)
{
    if(NULL == m_pTiXmlDocument)
    {
        return false;
    }

    if(false == m_pTiXmlDocument->LoadFile(pFileName))
    {
        return false;
    }

    //获得根元素
    m_pRootElement = m_pTiXmlDocument->RootElement();

    return true;
}

bool CXmlOpeation::Read_XML_Data_Single_String(const char* pTag, const char* pName, char* pValue, int nMaxSize)
{
    const char* pData = GetData(pTag, pName);

    if (pData != NULL)
    {
        sprintf_safe(pValue, nMaxSize, "%s", pData);
        return true;
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CXmlOpeation::Read_XML_Data_Single_String]tag=%s,name=%s no find.\n", pTag, pName));
        return false;
    }
}

bool CXmlOpeation::Read_XML_Data_Single_String(const char* pTag, const char* pName, string& strValue)
{
    const char* pData = GetData(pTag, pName);

    if (pData != NULL)
    {
        strValue = (string)pData;
        return true;
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CXmlOpeation::Read_XML_Data_Single_String]tag=%s,name=%s no find.\n", pTag, pName));
        return false;
    }
}

bool CXmlOpeation::Read_XML_Data_Multiple_String(const char* pTag, const char* pName, char* pValue, int nMaxSize, TiXmlElement*& pTi)
{
    const char* pData = GetData(pTag, pName, pTi);

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
    const char* pData = GetData(pTag, pName, pTi);

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
    const char* pData = GetData(pTag, pName);

    if (pData != NULL)
    {
        u4Value = (uint32)ACE_OS::atoi(pData);
        return true;
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CXmlOpeation::Read_XML_Data_Single_Uint32]tag=%s,name=%s no find.\n", pTag, pName));
        return false;
    }
}

bool CXmlOpeation::Read_XML_Data_Multiple_Uint32(const char* pTag, const char* pName, uint32& u4Value, TiXmlElement*& pTi)
{
    const char* pData = GetData(pTag, pName, pTi);

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
    const char* pData = GetData(pTag, pName);

    if (pData != NULL)
    {
        u2Value = (uint16)ACE_OS::atoi(pData);
        return true;
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CXmlOpeation::Read_XML_Data_Single_Uint16]tag=%s,name=%s no find.\n", pTag, pName));
        return false;
    }
}

bool CXmlOpeation::Read_XML_Data_Multiple_Uint16(const char* pTag, const char* pName, uint16& u2Value, TiXmlElement*& pTi)
{
    const char* pData = GetData(pTag, pName, pTi);

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
    const char* pData = GetData(pTag, pName);

    if (pData != NULL)
    {
        u1Value = (uint8)ACE_OS::atoi(pData);
        return true;
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CXmlOpeation::Read_XML_Data_Single_Uint8]tag=%s,name=%s no find.\n", pTag, pName));
        return false;
    }
}

bool CXmlOpeation::Read_XML_Data_Single_Int(const char* pTag, const char* pName, int& nValue)
{
    const char* pData = GetData(pTag, pName);

    if (pData != NULL)
    {
        nValue = ACE_OS::atoi(pData);
        return true;
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[CXmlOpeation::Read_XML_Data_Single_Int]tag=%s,name=%s no find.\n", pTag, pName));
        return false;
    }
}

bool CXmlOpeation::Read_XML_Data_Multiple_Uint8(const char* pTag, const char* pName, uint8& u1Value, TiXmlElement*& pTi)
{
    const char* pData = GetData(pTag, pName, pTi);

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

bool CXmlOpeation::Read_XML_Data_Multiple_Int(const char* pTag, const char* pName, int& u4Value, TiXmlElement*& pTi)
{
    const char* pData = GetData(pTag, pName, pTi);

    if (pData != NULL)
    {
        u4Value = atoi(pData);
        return true;
    }
    else
    {
        return false;
    }
}

const char* CXmlOpeation::GetData(const char* pName, const char* pAttrName)
{
    if(m_pRootElement == NULL)
    {
        return NULL;
    }

    TiXmlElement* pTiXmlElement = m_pRootElement->FirstChildElement(pName);

    if(NULL != pTiXmlElement)
    {
        return pTiXmlElement->Attribute(pAttrName);
    }

    return NULL;
}

const char* CXmlOpeation::GetData( const char* pName, const char* pAttrName, TiXmlElement*& pNextTiXmlElement )
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
        return pTiXmlElement->Attribute(pAttrName);
    }

    return NULL;
}


