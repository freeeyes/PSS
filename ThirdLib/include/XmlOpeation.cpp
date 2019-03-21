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

bool CXmlOpeation::Parse_Class_File(const char* pFileName, vecClassInfo& objvecClassInfo)
{
	Close();
	m_pTiXmlDocument = new TiXmlDocument(pFileName);
	if(NULL == m_pTiXmlDocument)
	{
		return false;
	}

	if(false == m_pTiXmlDocument->LoadFile())
	{
		Close();
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
		_Class_Info objClassInfo;

		int nMainType = pMainNode->Type();

		if(nMainType != TiXmlText::TINYXML_ELEMENT)
		{
			continue;
		}

		TiXmlElement* pMainElement = pMainNode->ToElement();

		//获得元素的名称
		sprintf_safe(objClassInfo.m_szXMLName, 60, pMainElement->Value());
		sprintf_safe(objClassInfo.m_szDesc, 100, "%s", pMainElement->Attribute("desc"));

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

					//获得Stream的指定长度
					char* pStreamLength = (char* )pNode->ToElement()->Attribute("StreamLength");
					if(NULL != pStreamLength)
					{
						sprintf_safe(objProperty.m_szStreamLength, 10, "%s", pStreamLength);
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
				objClassInfo.m_vecProperty.push_back(objProperty);
			}
		}

		objvecClassInfo.push_back(objClassInfo);
	}

	Close();
	return true;
}

bool CXmlOpeation::Parse_Plug_In_Project(const char* pFileName, _Project_Info& objProjectInfo)
{
	Close();
	m_pTiXmlDocument = new TiXmlDocument(pFileName);
	if(NULL == m_pTiXmlDocument)
	{
		return false;
	}

	if(false == m_pTiXmlDocument->LoadFile())
	{
		Close();
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
	char* pName = (char* )m_pRootElement->Attribute("Name");
	if(NULL != pName)
	{
		sprintf_safe(objProjectInfo.m_szProjectName, 100, "%s", pName);
	}

	char* pDesc = (char* )m_pRootElement->Attribute("desc");
	if(NULL != pDesc)
	{
		sprintf_safe(objProjectInfo.m_szProjectDesc, 200, "%s", pDesc);
	}

	char* pKeyID = (char* )m_pRootElement->Attribute("keyID");
	if(NULL != pKeyID)
	{
		sprintf_safe(objProjectInfo.m_szProjectKey, 100, "%s", pKeyID);
	}

	//获得其中的数据体
	TiXmlNode* pNode = NULL;
	for(pNode = m_pRootElement->FirstChildElement();pNode;pNode=pNode->NextSiblingElement())
	{
		TiXmlElement* pSecondElement = pNode->ToElement();
		if(strcmp(pSecondElement->Value(), "Define") == 0)
		{
			_Define_Info obj_Define_Info;
			char* pTemp = (char* )pSecondElement->Attribute("DefName");
			if(NULL != pTemp)
			{
				sprintf_safe(obj_Define_Info.m_szDefineName, 100, "%s", pTemp);
			}
			pTemp = (char* )pSecondElement->Attribute("type");
			if(NULL != pTemp)
			{
				sprintf_safe(obj_Define_Info.m_szType, 20, "%s", pTemp);
			}
			pTemp = (char* )pSecondElement->Attribute("DefValue");
			if(NULL != pTemp)
			{
				sprintf_safe(obj_Define_Info.m_szDefineValue, 100, "%s", pTemp);
			}
			objProjectInfo.m_objDefineInfoList.push_back(obj_Define_Info);
		}
		else if(strcmp(pSecondElement->Value(), "Command") == 0)
		{
			_Command_Info_Func obj_Command_Info;
			char* pTemp = (char* )pSecondElement->Attribute("CommandIn");
			if(NULL != pTemp)
			{
				sprintf_safe(obj_Command_Info.m_szCommandInID, 50, "%s", pTemp);
			}
			pTemp = (char* )pSecondElement->Attribute("CommandOut");
			if(NULL != pTemp)
			{
				sprintf_safe(obj_Command_Info.m_szCommandOutID, 50, "%s", pTemp);
			}
			pTemp = (char* )pSecondElement->Attribute("FuncName");
			if(NULL != pTemp)
			{
				sprintf_safe(obj_Command_Info.m_szCommandFuncName, 100, "%s", pTemp);
			}

			TiXmlNode* pObjectNode = NULL;
			for(pObjectNode = pSecondElement->FirstChildElement();pObjectNode;pObjectNode=pObjectNode->NextSiblingElement())
			{
				_Object_Info obj_Object_Info;
				TiXmlElement* pThreeElement = pObjectNode->ToElement();

				pTemp = (char* )pThreeElement->Attribute("class");
				if(NULL != pTemp)
				{
					sprintf_safe(obj_Object_Info.m_szClassName, 50, "%s", pTemp);
				}
				pTemp = (char* )pThreeElement->Attribute("PacketType");
				if(NULL != pTemp)
				{
					if(strcmp(pTemp, "HEAD") == 0)
					{
						obj_Object_Info.m_emPacketType = PACKET_TYPE_HEAD;
					}
					else if(strcmp(pTemp, "BODY") == 0)
					{
						obj_Object_Info.m_emPacketType = PACKET_TYPE_BODY;
					}
					else if(strcmp(pTemp, "RETURN") == 0)
					{
						obj_Object_Info.m_emPacketType = PACKET_TYPE_RETURN;
					}
				}

				obj_Command_Info.m_vecObjectInfo.push_back(obj_Object_Info);
			}

			objProjectInfo.m_objCommandList.push_back(obj_Command_Info);
		}
	}

	Close();
	return true;
}
