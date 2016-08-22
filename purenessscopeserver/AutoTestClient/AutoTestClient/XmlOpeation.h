#ifndef _XMLOPRATION_H
#define _XMLOPRATION_H

#include "tinyxml.h"
#include "tinystr.h"

#include "Common.h"

//将XML转换为MAP结构
//add by freeeyes


class CXmlOpeation
{
public:
  CXmlOpeation(void);
  ~CXmlOpeation(void);

  bool Init(const char* pFileName);
  bool Init_String(const char* pXMLText);

  char* GetData(const char* pName, const char* pAttrName);
  char* GetData(const char* pName, const char* pAttrName, TiXmlElement*& pNextTiXmlElement);

  char* GetData_Text(const char* pName);
  char* GetData_Text(const char* pName, TiXmlElement*& pNextTiXmlElement);

  bool Parse_XML_Test_Assemble(const char* pFileName, _Test_Assemble& obj_Test_Assemble);

  void Close();

private:
  TiXmlDocument* m_pTiXmlDocument;
  TiXmlElement*  m_pRootElement;
};
#endif
