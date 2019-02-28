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

bool CXmlOpeation::Parse_XML_Test_Assemble(const char* pFileName, _Test_Assemble& obj_Test_Assemble)
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

	TiXmlNode* pMainNode     = NULL;
	TiXmlNode* pSecondNode   = NULL;
	TiXmlNode* pThreeNode    = NULL;
	TiXmlNode* pColumnNode   = NULL;

	//获得根元素
	m_pRootElement = m_pTiXmlDocument->RootElement();

	if(NULL == m_pRootElement)
	{
		return false;
	}

	//获得测试集信息
	sprintf_safe(obj_Test_Assemble.m_szTestAssembleName, MAX_BUFF_50, "%s", (char* )m_pRootElement->Attribute("Name"));
	sprintf_safe(obj_Test_Assemble.m_szDesc, MAX_BUFF_100, "%s", (char* )m_pRootElement->Attribute("desc"));
	sprintf_safe(obj_Test_Assemble.m_szIP, MAX_BUFF_50, "%s", (char* )m_pRootElement->Attribute("IP"));
	sprintf_safe(obj_Test_Assemble.m_szOrder, MAX_BUFF_50, "%s", (char* )m_pRootElement->Attribute("ORDER"));
	obj_Test_Assemble.m_nPort = atoi((char* )m_pRootElement->Attribute("Port"));

	//循环获取预定义信息
	for(pMainNode = m_pRootElement->FirstChildElement();pMainNode;pMainNode = pMainNode->NextSiblingElement())
	{
		_Command_Info obj_Command_Info;

		int nMainType = pMainNode->Type();

		if(nMainType != TiXmlText::TINYXML_ELEMENT)
		{
			continue;
		}

		TiXmlElement* pMainElement = pMainNode->ToElement();
		sprintf_safe(obj_Command_Info.m_szCommandName, MAX_BUFF_50, "%s", (char* )pMainElement->Attribute("CommandName"));
		obj_Command_Info.m_nCount    = atoi((char* )pMainElement->Attribute("Count"));
		obj_Command_Info.m_nTimeCost = atoi((char* )pMainElement->Attribute("TimeCost"));
		if(pMainElement->Attribute("ThreadCount") != NULL)
		{
			obj_Command_Info.m_nThreadCount = atoi((char* )pMainElement->Attribute("ThreadCount"));
		}

		for(pSecondNode = pMainElement->FirstChildElement();pSecondNode;pSecondNode = pSecondNode->NextSiblingElement())
		{
			int nMainType = pMainNode->Type();

			if(nMainType != TiXmlText::TINYXML_ELEMENT)
			{
				continue;
			}

			TiXmlElement* pSecondElement = pSecondNode->ToElement();
			if(strcmp("Packet_Send", pSecondElement->Value()) == 0)
			{
				//获得所有元素信息
				for(pThreeNode = pSecondElement->FirstChildElement();pThreeNode;pThreeNode = pThreeNode->NextSiblingElement())
				{
					TiXmlElement* pThreeElement = pThreeNode->ToElement();
					_Data_Info obj_Data_Info;
					sprintf_safe(obj_Data_Info.m_szDataType, MAX_BUFF_50, "%s", (char* )pThreeElement->Value());
					sprintf_safe(obj_Data_Info.m_szDataName, MAX_BUFF_50, "%s", (char* )pThreeElement->Attribute("name"));
					if(NULL != pThreeElement->Attribute("length"))
					{
						obj_Data_Info.m_nLength =  atoi((char* )pThreeElement->Attribute("length"));
					}
					if(NULL != pThreeElement->Attribute("IsString"))
					{
						if(strcmp("1", pThreeElement->Attribute("IsString")) == 0)
						{
							obj_Data_Info.m_blIsString = true;
						}
						else
						{
							obj_Data_Info.m_blIsString = false;
						}
					}
					if(NULL != (char* )pThreeElement->GetText())
					{
						obj_Data_Info.m_strValue = (string)pThreeElement->GetText();
					}
					obj_Command_Info.m_obj_Packet_Send.m_obj_Data_Info_List.push_back(obj_Data_Info);
				}
			}
			else if(strcmp("Packet_Recv", pSecondElement->Value()) == 0)
			{
				//获得所有元素信息
				for(pThreeNode = pSecondElement->FirstChildElement();pThreeNode;pThreeNode = pThreeNode->NextSiblingElement())
				{
					TiXmlElement* pThreeElement = pThreeNode->ToElement();
					_Data_Info obj_Data_Info;
					sprintf_safe(obj_Data_Info.m_szDataType, MAX_BUFF_50, "%s", (char* )pThreeElement->Value());
					sprintf_safe(obj_Data_Info.m_szDataName, MAX_BUFF_50, "%s", (char* )pThreeElement->Attribute("name"));
					if(NULL != pThreeElement->Attribute("length"))
					{
						obj_Data_Info.m_nLength =  atoi((char* )pThreeElement->Attribute("length"));
					}
					if(NULL != pThreeElement->Attribute("IsString"))
					{
						if(strcmp("1", pThreeElement->Attribute("IsString")) == 0)
						{
							obj_Data_Info.m_blIsString = true;
						}
						else
						{
							obj_Data_Info.m_blIsString = false;
						}
					}
					if(NULL != (char* )pThreeElement->GetText())
					{
						obj_Data_Info.m_strValue = (string)((char* )pThreeElement->GetText());
					}
					obj_Command_Info.m_obj_Packet_Recv.m_obj_Data_Info_List.push_back(obj_Data_Info);
				}
			}
		}

		obj_Test_Assemble.m_obj_Command_Info_List.push_back(obj_Command_Info);
	}

	delete m_pTiXmlDocument;
	m_pTiXmlDocument = NULL;
	return true;
}

