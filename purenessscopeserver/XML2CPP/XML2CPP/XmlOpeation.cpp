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
			if(strcmp(pNode->ToElement()->GetText(), "char") == 0)
			{
				objProperty.m_emType = PROPERTY_CHAR;

				//同时获得字符串最大长度
				char* pLength = (char* )pNode->ToElement()->Attribute("length");
				if(NULL != pLength)
				{
					objProperty.m_nLength = (int)atoi(pLength);
				}
			}
			else if(strcmp(pNode->ToElement()->GetText(), "string") == 0)
			{
				objProperty.m_emType = PROPERTY_STRING;
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
				sprintf_safe(objProperty.m_szClassName, 100, "%s", pNode->ToElement()->GetText());
			}

			//查看参数类别
			if(NULL != pNode->ToElement()->Attribute("class"))
			{
				if(strcmp(pNode->ToElement()->Attribute("class"), "vector") == 0)
				{
					objProperty.m_emClass = CLASS_VECTOR;
				}
				else if(strcmp(pNode->ToElement()->Attribute("class"), "map") == 0)
				{
					objProperty.m_emClass = CLASS_MAP;

					//如果是map 则查看keyName和keyType
					sprintf_safe(objProperty.m_szKeyName, 100, "%s", pNode->ToElement()->Attribute("keyName"));

					if(strcmp(pNode->ToElement()->Attribute("KeyClass"), "string") == 0)
					{
						objProperty.m_emKeyType = PROPERTY_STRING;
					}
					else if(strcmp(pNode->ToElement()->Attribute("KeyClass"), "uint16") == 0)
					{
						objProperty.m_emKeyType = PROPERTY_UINT16;
					}
					else if(strcmp(pNode->ToElement()->Attribute("KeyClass"), "uint32") == 0)
					{
						objProperty.m_emKeyType = PROPERTY_UINT32;
					}
				}
			}

			sprintf_safe(objProperty.m_szDesc, 100, "%s", pNode->ToElement()->Attribute("desc"));

			objxmlInfo.m_vecProperty.push_back(objProperty);
		}
	}

	Close();
	return true;
}

bool CXmlOpeation::Parse_XML_File(const char* pFileName, vecXmlInfo& objvecXmlInfo)
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

		//获得元素的名称
		sprintf_safe(objxmlInfo.m_szXMLName, 60, pMainElement->Value());
		sprintf_safe(objxmlInfo.m_szDesc, 100, "%s", pMainElement->Attribute("desc"));
		char* pCommandID = (char* )pMainElement->Attribute("CommandID");
		if(NULL != pCommandID)
		{
			objxmlInfo.m_nCommandID =atoi(pCommandID);
		}
		char* pCommandType = (char* )pMainElement->Attribute("CommandType");
		if(NULL != pCommandType)
		{
			if(strcmp(pCommandType, "in") == 0)
			{
				objxmlInfo.m_emCommandType = COMMAND_IN;
			}
			else if(strcmp(pCommandType, "out") == 0)
			{
				objxmlInfo.m_emCommandType = COMMAND_OUT;
			}
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
				if(strcmp(pNode->ToElement()->GetText(), "char") == 0)
				{
					objProperty.m_emType = PROPERTY_CHAR;

					//同时获得字符串最大长度
					char* pLength = (char* )pNode->ToElement()->Attribute("length");
					if(NULL != pLength)
					{
						objProperty.m_nLength = (int)atoi(pLength);
					}
				}
				else if(strcmp(pNode->ToElement()->GetText(), "string") == 0)
				{
					objProperty.m_emType = PROPERTY_STRING;
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
					//不是基本类型，是类
					objProperty.m_emType = PROPERTY_UNKNOW;
					sprintf_safe(objProperty.m_szClassName, 100, "%s", pNode->ToElement()->GetText());
				}

				//查看参数类别
				if(NULL != pNode->ToElement()->Attribute("class"))
				{
					if(strcmp(pNode->ToElement()->Attribute("class"), "vector") == 0)
					{
						objProperty.m_emClass = CLASS_VECTOR;
					}
					else if(strcmp(pNode->ToElement()->Attribute("class"), "map") == 0)
					{
						objProperty.m_emClass = CLASS_MAP;

						//如果是map 则查看keyName和keyType
						sprintf_safe(objProperty.m_szKeyName, 100, "%s", pNode->ToElement()->Attribute("keyName"));

						if(strcmp(pNode->ToElement()->Attribute("KeyClass"), "string") == 0)
						{
							objProperty.m_emKeyType = PROPERTY_STRING;
						}
						else if(strcmp(pNode->ToElement()->Attribute("KeyClass"), "uint16") == 0)
						{
							objProperty.m_emKeyType = PROPERTY_UINT16;
						}
						else if(strcmp(pNode->ToElement()->Attribute("KeyClass"), "uint32") == 0)
						{
							objProperty.m_emKeyType = PROPERTY_UINT32;
						}
					}
				}

				//得到参数描述信息
				sprintf_safe(objProperty.m_szDesc, 100, "%s", pNode->ToElement()->Attribute("desc"));

				objxmlInfo.m_vecProperty.push_back(objProperty);
			}
		}

		objvecXmlInfo.push_back(objxmlInfo);
	}

	Close();
	return true;
}

bool CXmlOpeation::Parse_XML_File_Project(const char* pFileName, _Project_Info& objProjectInfo)
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

	//获得头元素
	char* pName = GetData_Text("Name");
	if(NULL != pName)
	{
		sprintf_safe(objProjectInfo.m_szProjectName, 100, "%s", pName);
	}

	char* pDesc = GetData("Name", "desc");
	if(NULL != pDesc)
	{
		sprintf_safe(objProjectInfo.m_szProjectDesc, 200, "%s", pDesc);
	}

	char* pKeyID = GetData("Name", "keyID");
	if(NULL != pKeyID)
	{
		sprintf_safe(objProjectInfo.m_szProjectKey, 100, "%s", pDesc);
	}

	Close();
	return true;
}