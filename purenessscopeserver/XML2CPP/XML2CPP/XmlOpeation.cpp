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

bool CXmlOpeation::Init_String(const char* pXMLText)
{
	//解析字符串
	Close();
	m_pTiXmlDocument = new TiXmlDocument();
	m_pTiXmlDocument->Parse(pXMLText, 0, TIXML_DEFAULT_ENCODING); 

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

char* CXmlOpeation::GetData(const char* pName, const char* pAttrName, TiXmlElement*& pNextTiXmlElement)
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

char* CXmlOpeation::GetData_Text(const char* pName)
{
	if(m_pRootElement == NULL)
	{
		return NULL;
	}

	TiXmlElement* pTiXmlElement = m_pRootElement->FirstChildElement(pName);
	if(NULL != pTiXmlElement)
	{
		return (char* )pTiXmlElement->GetText();
	}

	return NULL;
}

char* CXmlOpeation::GetData_Text(const char* pName, TiXmlElement*& pNextTiXmlElement)
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
		return (char* )pTiXmlElement->GetText();
	}

	return NULL;
}

bool CXmlOpeation::Parse_XML(char* pText, _Xml_Info& objxmlInfo)
{
	//解析字符串
	Close();
	m_pTiXmlDocument = new TiXmlDocument();
	m_pTiXmlDocument->Parse(pText, 0, TIXML_DEFAULT_ENCODING); 

	//获得根元素
	m_pRootElement = m_pTiXmlDocument->RootElement();

	//获得根元素的名称
	sprintf_safe(objxmlInfo.m_szXMLName, 60, m_pRootElement->Value());
	sprintf_safe(objxmlInfo.m_szDesc, 100, "%s", m_pRootElement->Attribute("desc"));
	char* pCommandID = (char* )m_pRootElement->Attribute("CommandID");
	if(NULL != pCommandID)
	{
		objxmlInfo.m_nCommandID =atoi(pCommandID);
	}

	//循环打印出每一个变量
	if(NULL == m_pRootElement)
	{
		return false;
	}

	//printf("Root=%s.\n", m_pRootElement->Value());

	TiXmlNode* pNode = NULL;

	for(pNode = m_pRootElement->FirstChildElement();pNode;pNode=pNode->NextSiblingElement())
	{
		int nType = pNode->Type();
		if(nType == TiXmlText::TINYXML_ELEMENT)
		{
			//printf("Name=%s,Values=%s.\n", pNode->Value(), pNode->ToElement()->GetText());
			_Property objProperty;
			sprintf_safe(objProperty.m_szPropertyName, 50, "%s", pNode->Value());
			if(strcmp(pNode->ToElement()->GetText(), "string") == 0)
			{
				objProperty.m_emType = PROPERTY_STRING;

				//同时获得字符串最大长度
				char* pLength = (char* )pNode->ToElement()->Attribute("length");
				if(NULL != pLength)
				{
					objProperty.m_nLength = (int)atoi(pLength);
				}
			}
			else if(strcmp(pNode->ToElement()->GetText(), "uint8") == 0)
			{
				objProperty.m_emType = PROPERTY_UINT8;
			}
			else if(strcmp(pNode->ToElement()->GetText(), "uint16") == 0)
			{
				objProperty.m_emType = PROPERTY_UINT16;
			}
			else if(strcmp(pNode->ToElement()->GetText(), "uint32") == 0)
			{
				objProperty.m_emType = PROPERTY_UINT32;
			}
			else if(strcmp(pNode->ToElement()->GetText(), "uint64") == 0)
			{
				objProperty.m_emType = PROPERTY_UINT64;
			}
			else if(strcmp(pNode->ToElement()->GetText(), "int8") == 0)
			{
				objProperty.m_emType = PROPERTY_INT8;
			}
			else if(strcmp(pNode->ToElement()->GetText(), "int16") == 0)
			{
				objProperty.m_emType = PROPERTY_INT16;
			}
			else if(strcmp(pNode->ToElement()->GetText(), "int32") == 0)
			{
				objProperty.m_emType = PROPERTY_INT32;
			}
			else if(strcmp(pNode->ToElement()->GetText(), "float32") == 0)
			{
				objProperty.m_emType = PROPERTY_FLOAT32;
			}
			else if(strcmp(pNode->ToElement()->GetText(), "float64") == 0)
			{
				objProperty.m_emType = PROPERTY_FLOAT64;
			}
			else
			{
				objProperty.m_emType = PROPERTY_UNKNOW;
			}

			sprintf_safe(objProperty.m_szDesc, 100, "%s", pNode->ToElement()->Attribute("desc"));

			objxmlInfo.m_vecProperty.push_back(objProperty);
		}
	}

	Close();
	return true;
}

bool CXmlOpeation::Parse_XML_File(char* pFileName, vecXmlInfo& objvecXmlInfo)
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

	TiXmlNode* pMainNode = NULL;
	//获得根元素
	m_pRootElement = m_pTiXmlDocument->RootElement();

	//循环打印出每一个变量
	if(NULL == m_pRootElement)
	{
		return false;
	}

	//获得根元素的子元素
	for(pMainNode = m_pRootElement->FirstChildElement();pMainNode;pMainNode=pMainNode->NextSiblingElement())
	{
		_Xml_Info objxmlInfo;

		int nMainType = pMainNode->Type();

		if(nMainType != TiXmlText::TINYXML_ELEMENT)
		{
			continue;
		}

		TiXmlElement* pMainElement = pMainNode->ToElement();

		//获得根元素的名称
		sprintf_safe(objxmlInfo.m_szXMLName, 60, pMainElement->Value());
		sprintf_safe(objxmlInfo.m_szDesc, 100, "%s", pMainElement->Attribute("desc"));
		char* pCommandID = (char* )pMainElement->Attribute("CommandID");
		if(NULL != pCommandID)
		{
			objxmlInfo.m_nCommandID =atoi(pCommandID);
		}

		//printf("Root=%s.\n", m_pRootElement->Value());

		TiXmlNode* pNode = NULL;

		for(pNode = pMainElement->FirstChildElement();pNode;pNode=pNode->NextSiblingElement())
		{
			int nType = pNode->Type();
			if(nType == TiXmlText::TINYXML_ELEMENT)
			{
				//printf("Name=%s,Values=%s.\n", pNode->Value(), pNode->ToElement()->GetText());
				_Property objProperty;
				sprintf_safe(objProperty.m_szPropertyName, 50, "%s", pNode->Value());
				if(strcmp(pNode->ToElement()->GetText(), "string") == 0)
				{
					objProperty.m_emType = PROPERTY_STRING;

					//同时获得字符串最大长度
					char* pLength = (char* )pNode->ToElement()->Attribute("length");
					if(NULL != pLength)
					{
						objProperty.m_nLength = (int)atoi(pLength);
					}
				}
				else if(strcmp(pNode->ToElement()->GetText(), "uint8") == 0)
				{
					objProperty.m_emType = PROPERTY_UINT8;
				}
				else if(strcmp(pNode->ToElement()->GetText(), "uint16") == 0)
				{
					objProperty.m_emType = PROPERTY_UINT16;
				}
				else if(strcmp(pNode->ToElement()->GetText(), "uint32") == 0)
				{
					objProperty.m_emType = PROPERTY_UINT32;
				}
				else if(strcmp(pNode->ToElement()->GetText(), "uint64") == 0)
				{
					objProperty.m_emType = PROPERTY_UINT64;
				}
				else if(strcmp(pNode->ToElement()->GetText(), "int8") == 0)
				{
					objProperty.m_emType = PROPERTY_INT8;
				}
				else if(strcmp(pNode->ToElement()->GetText(), "int16") == 0)
				{
					objProperty.m_emType = PROPERTY_INT16;
				}
				else if(strcmp(pNode->ToElement()->GetText(), "int32") == 0)
				{
					objProperty.m_emType = PROPERTY_INT32;
				}
				else if(strcmp(pNode->ToElement()->GetText(), "float32") == 0)
				{
					objProperty.m_emType = PROPERTY_FLOAT32;
				}
				else if(strcmp(pNode->ToElement()->GetText(), "float64") == 0)
				{
					objProperty.m_emType = PROPERTY_FLOAT64;
				}
				else
				{
					objProperty.m_emType = PROPERTY_UNKNOW;
				}
				sprintf_safe(objProperty.m_szDesc, 100, "%s", pNode->ToElement()->Attribute("desc"));

				objxmlInfo.m_vecProperty.push_back(objProperty);
			}
		}

		objvecXmlInfo.push_back(objxmlInfo);
	}

	Close();
	return true;
}
