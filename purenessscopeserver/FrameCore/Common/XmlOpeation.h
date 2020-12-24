#ifndef _XMLOPRATION_H
#define _XMLOPRATION_H

#include "tinyxml.h"
#include "tinystr.h"
#include "define.h"

class CXmlOpeation
{
public:
    CXmlOpeation(void);

    bool Init(const char* pFileName);

    const char* GetData(const char* pName, const char* pAttrName);
    const char* GetData(const char* pName, const char* pAttrName, TiXmlElement*& pNextTiXmlElement);

    //读取单个变量
    bool Read_XML_Data_Single_String(const char* pTag, const char* pName, string& strValue);
    bool Read_XML_Data_Single_String(const char* pTag, const char* pName, char* pValue, int nMaxSize);
    bool Read_XML_Data_Single_Uint32(const char* pTag, const char* pName, uint32& u4Value);
    bool Read_XML_Data_Single_Uint16(const char* pTag, const char* pName, uint16& u2Value);
    bool Read_XML_Data_Single_Uint8(const char* pTag, const char* pName, uint8& u1Value);
    bool Read_XML_Data_Single_Int(const char* pTag, const char* pName, int& nValue);

    //读取循环里的变量
    bool Read_XML_Data_Multiple_String(const char* pTag, const char* pName, string& strValue, TiXmlElement*& pTi);
    bool Read_XML_Data_Multiple_String(const char* pTag, const char* pName, char* pValue, int nMaxSize, TiXmlElement*& pTi);
    bool Read_XML_Data_Multiple_Uint32(const char* pTag, const char* pName, uint32& u4Value, TiXmlElement*& pTi);
    bool Read_XML_Data_Multiple_Uint16(const char* pTag, const char* pName, uint16& u2Value, TiXmlElement*& pTi);
    bool Read_XML_Data_Multiple_Uint8(const char* pTag, const char* pName, uint8& u1Value, TiXmlElement*& pTi);
    bool Read_XML_Data_Multiple_Int(const char* pTag, const char* pName, int& u4Value, TiXmlElement*& pTi);

    const string GetError();

private:
    std::shared_ptr<TiXmlDocument> m_pTiXmlDocument;
    TiXmlElement*                  m_pRootElement    = nullptr;
    string                         m_strError;
};
#endif
