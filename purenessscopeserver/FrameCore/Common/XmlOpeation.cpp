#include "XmlOpeation.h"

CXmlOpeation::CXmlOpeation(void)
{
    m_pTiXmlDocument = std::make_shared<TiXmlDocument>();
}

bool CXmlOpeation::Init(const char* pFileName)
{
    if(nullptr == m_pTiXmlDocument)
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

    if (pData != nullptr)
    {
        sprintf_safe(pValue, nMaxSize, "%s", pData);
        return true;
    }
    else
    {
        m_strError = fmt::format("[CXmlOpeation::Read_XML_Data_Single_String]tag={0},name={1} no find.", pTag, pName);
        return false;
    }
}

bool CXmlOpeation::Read_XML_Data_Single_String(const char* pTag, const char* pName, string& strValue)
{
    const char* pData = GetData(pTag, pName);

    if (pData != nullptr)
    {
        strValue = (string)pData;
        return true;
    }
    else
    {
        m_strError = fmt::format("[CXmlOpeation::Read_XML_Data_Single_String]tag=%s,name=%s no find.", pTag, pName);
        return false;
    }
}

bool CXmlOpeation::Read_XML_Data_Multiple_String(const char* pTag, const char* pName, char* pValue, int nMaxSize, TiXmlElement*& pTi)
{
    const char* pData = GetData(pTag, pName, pTi);

    if (pData != nullptr)
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

    if (pData != nullptr)
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

    if (pData != nullptr)
    {
        u4Value = (uint32)ACE_OS::atoi(pData);
        return true;
    }
    else
    {
        m_strError = fmt::format("[CXmlOpeation::Read_XML_Data_Single_Uint32]tag=%s,name=%s no find.", pTag, pName);
        return false;
    }
}

bool CXmlOpeation::Read_XML_Data_Multiple_Uint32(const char* pTag, const char* pName, uint32& u4Value, TiXmlElement*& pTi)
{
    const char* pData = GetData(pTag, pName, pTi);

    if (pData != nullptr)
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

    if (pData != nullptr)
    {
        u2Value = (uint16)ACE_OS::atoi(pData);
        return true;
    }
    else
    {
        m_strError = fmt::format("[CXmlOpeation::Read_XML_Data_Single_Uint16]tag=%s,name=%s no find.", pTag, pName);
        return false;
    }
}

bool CXmlOpeation::Read_XML_Data_Multiple_Uint16(const char* pTag, const char* pName, uint16& u2Value, TiXmlElement*& pTi)
{
    const char* pData = GetData(pTag, pName, pTi);

    if (pData != nullptr)
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

    if (pData != nullptr)
    {
        u1Value = (uint8)ACE_OS::atoi(pData);
        return true;
    }
    else
    {
        m_strError = fmt::format("[CXmlOpeation::Read_XML_Data_Single_Uint8]tag=%s,name=%s no find.", pTag, pName);
        return false;
    }
}

bool CXmlOpeation::Read_XML_Data_Single_Int(const char* pTag, const char* pName, int& nValue)
{
    const char* pData = GetData(pTag, pName);

    if (pData != nullptr)
    {
        nValue = ACE_OS::atoi(pData);
        return true;
    }
    else
    {
        m_strError = fmt::format("[CXmlOpeation::Read_XML_Data_Single_Int]tag=%s,name=%s no find.", pTag, pName);
        return false;
    }
}

bool CXmlOpeation::Read_XML_Data_Multiple_Uint8(const char* pTag, const char* pName, uint8& u1Value, TiXmlElement*& pTi)
{
    const char* pData = GetData(pTag, pName, pTi);

    if (pData != nullptr)
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

    if (pData != nullptr)
    {
        u4Value = atoi(pData);
        return true;
    }
    else
    {
        return false;
    }
}

const string CXmlOpeation::GetError()
{
    return m_strError;
}

const char* CXmlOpeation::GetData(const char* pName, const char* pAttrName)
{
    if(m_pRootElement == nullptr)
    {
        return nullptr;
    }

    const TiXmlElement* pTiXmlElement = m_pRootElement->FirstChildElement(pName);

    if(nullptr != pTiXmlElement)
    {
        return pTiXmlElement->Attribute(pAttrName);
    }

    return nullptr;
}

const char* CXmlOpeation::GetData( const char* pName, const char* pAttrName, TiXmlElement*& pNextTiXmlElement )
{
    if(m_pRootElement == nullptr)
    {
        return nullptr;
    }

    TiXmlElement* pTiXmlElement = nullptr;

    if(nullptr == pNextTiXmlElement)
    {
        pTiXmlElement = m_pRootElement->FirstChildElement(pName);
        pNextTiXmlElement = pTiXmlElement;
    }
    else
    {
        pTiXmlElement  = pNextTiXmlElement->NextSiblingElement();
        pNextTiXmlElement = pTiXmlElement;
    }

    if(nullptr != pTiXmlElement)
    {
        return pTiXmlElement->Attribute(pAttrName);
    }

    return nullptr;
}


