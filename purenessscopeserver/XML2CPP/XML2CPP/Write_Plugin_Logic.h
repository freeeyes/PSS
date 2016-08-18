#ifndef _WRITE_PLUGIN_LOGIC_H
#define _WRITE_PLUGIN_LOGIC_H

#include "XmlOpeation.h"

_Class_Info* Find_Logic_StructInfo(int nCommandID, vecClassInfo& objvecClassInfo)
{
	/*
	for(int i = 0; i < (int)objvecClassInfo.size(); i++)
	{
		if(objvecXmlInfo[i].m_nCommandID == nCommandID)
		{
			return &objvecXmlInfo[i];
		}
	}
	*/

	return NULL;
}

void Gen_2_Cpp_Logic_H(_Project_Info& objProjectInfo, vecClassInfo& objvecClassInfo)
{
	char szTemp[200]     = {'\0'};
	char szPathFile[200] = {'\0'};

	sprintf_safe(szPathFile, 200, "%s/%s_Logic.h", 
		objProjectInfo.m_szProjectName, 
		objProjectInfo.m_szProjectName);

	//首先生成声明文件。
	FILE* pFile = fopen(szPathFile, "w");
	if(NULL == pFile)
	{
		return;
	}

	sprintf_safe(szTemp, 200, "#include \"Protocol.h\"\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	for(int i = 0; i < (int)objProjectInfo.m_objCommandList.size(); i++)
	{
		if(strlen(objProjectInfo.m_objCommandList[i].m_szCommandInID) > 0)
		{
			char* pHeadObjectName   = NULL;
			char* pBodyObjectName   = NULL;
			char* pReturnObjectName = NULL;

			for(int j = 0; j < (int)objProjectInfo.m_objCommandList[i].m_vecObjectInfo.size(); j++)
			{
				if(objProjectInfo.m_objCommandList[i].m_vecObjectInfo[j].m_emPacketType == PACKET_TYPE_HEAD)
				{
					pHeadObjectName = objProjectInfo.m_objCommandList[i].m_vecObjectInfo[j].m_szClassName;
				}
				else if(objProjectInfo.m_objCommandList[i].m_vecObjectInfo[j].m_emPacketType == PACKET_TYPE_BODY)
				{
					pBodyObjectName = objProjectInfo.m_objCommandList[i].m_vecObjectInfo[j].m_szClassName;
				}
				else if(objProjectInfo.m_objCommandList[i].m_vecObjectInfo[j].m_emPacketType == PACKET_TYPE_RETURN)
				{
					pReturnObjectName = objProjectInfo.m_objCommandList[i].m_vecObjectInfo[j].m_szClassName;
				}
			}

			bool bl_Is_Data = false;
			//两边参数都有，写入函数
			sprintf_safe(szTemp, 200, "bool Logic_%s(", 
				objProjectInfo.m_objCommandList[i].m_szCommandFuncName);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			if(NULL != pHeadObjectName)
			{
				sprintf_safe(szTemp, 200, "%s& obj%s", 
					pHeadObjectName, pHeadObjectName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				bl_Is_Data = true;
			}
			if(NULL != pBodyObjectName)
			{
				if(true == bl_Is_Data)
				{
					sprintf_safe(szTemp, 200, ", %s& obj%s", 
						pBodyObjectName, pBodyObjectName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
				else
				{
					sprintf_safe(szTemp, 200, "%s& obj%s", 
						pBodyObjectName, pBodyObjectName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					bl_Is_Data = true;
				}
			}
			if(NULL != pReturnObjectName)
			{
				if(true == bl_Is_Data)
				{
					sprintf_safe(szTemp, 200, ", %s& obj%s", 
						pReturnObjectName, pReturnObjectName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
				else
				{
					sprintf_safe(szTemp, 200, "%s& obj%s", 
						pReturnObjectName, pReturnObjectName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					bl_Is_Data = true;
				}
			}
			sprintf_safe(szTemp, 200, ");\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
	}
	

	fclose(pFile);
}


void Gen_2_Cpp_Logic_Cpp(_Project_Info& objProjectInfo, vecClassInfo& objvecClassInfo)
{
	char szTemp[200]     = {'\0'};
	char szPathFile[200] = {'\0'};

	sprintf_safe(szPathFile, 200, "%s/%s_Logic.cpp", 
		objProjectInfo.m_szProjectName, 
		objProjectInfo.m_szProjectName);

	//首先生成声明文件。
	FILE* pFile = fopen(szPathFile, "w");
	if(NULL == pFile)
	{
		return;
	}

	sprintf_safe(szTemp, 200, "#include \"%s_Logic.h\"\n\n",
		objProjectInfo.m_szProjectName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	for(int i = 0; i < (int)objProjectInfo.m_objCommandList.size(); i++)
	{
		if(strlen(objProjectInfo.m_objCommandList[i].m_szCommandInID) > 0)
		{
			char* pHeadObjectName   = NULL;
			char* pBodyObjectName   = NULL;
			char* pReturnObjectName = NULL;

			for(int j = 0; j < (int)objProjectInfo.m_objCommandList[i].m_vecObjectInfo.size(); j++)
			{
				if(objProjectInfo.m_objCommandList[i].m_vecObjectInfo[j].m_emPacketType == PACKET_TYPE_HEAD)
				{
					pHeadObjectName = objProjectInfo.m_objCommandList[i].m_vecObjectInfo[j].m_szClassName;
				}
				else if(objProjectInfo.m_objCommandList[i].m_vecObjectInfo[j].m_emPacketType == PACKET_TYPE_BODY)
				{
					pBodyObjectName = objProjectInfo.m_objCommandList[i].m_vecObjectInfo[j].m_szClassName;
				}
				else if(objProjectInfo.m_objCommandList[i].m_vecObjectInfo[j].m_emPacketType == PACKET_TYPE_RETURN)
				{
					pReturnObjectName = objProjectInfo.m_objCommandList[i].m_vecObjectInfo[j].m_szClassName;
				}
			}

			//两边参数都有，写入函数
			bool bl_Is_Data = false;
			sprintf_safe(szTemp, 200, "bool Logic_%s(", 
				objProjectInfo.m_objCommandList[i].m_szCommandFuncName);
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			if(NULL != pHeadObjectName)
			{
				sprintf_safe(szTemp, 200, "%s& obj%s", 
					pHeadObjectName, pHeadObjectName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				bl_Is_Data = true;
			}
			if(NULL != pBodyObjectName)
			{
				if(true == bl_Is_Data)
				{
					sprintf_safe(szTemp, 200, ", %s& obj%s", 
						pBodyObjectName, pBodyObjectName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
				else
				{
					sprintf_safe(szTemp, 200, "%s& obj%s", 
						pBodyObjectName, pBodyObjectName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					bl_Is_Data = true;
				}
			}
			if(NULL != pReturnObjectName)
			{
				if(true == bl_Is_Data)
				{
					sprintf_safe(szTemp, 200, ", %s& obj%s", 
						pReturnObjectName, pReturnObjectName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
				else
				{
					sprintf_safe(szTemp, 200, "%s& obj%s", 
						pReturnObjectName, pReturnObjectName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					bl_Is_Data = true;
				}
			}
			sprintf_safe(szTemp, 200, ")\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "{\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\t//add your code at here.\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "\treturn true;\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			sprintf_safe(szTemp, 200, "}\n\n");
			fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		}
	}

	fclose(pFile);
}

#endif
