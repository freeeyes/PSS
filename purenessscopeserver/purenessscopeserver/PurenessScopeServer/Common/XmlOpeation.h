#ifndef _XMLOPRATION_H
#define _XMLOPRATION_H

#include "tinyxml.h"
#include "tinystr.h"

class CXmlOpeation
{
public:
    CXmlOpeation(void);
    ~CXmlOpeation(void);

    bool Init(const char* pFileName);

    char* GetData(const char* pName, const char* pAttrName);
    char* GetData(const char* pName, const char* pAttrName, TiXmlElement*& pNextTiXmlElement);

    void Close();

private:
    TiXmlDocument* m_pTiXmlDocument;
    TiXmlElement*  m_pRootElement;
};
#endif
