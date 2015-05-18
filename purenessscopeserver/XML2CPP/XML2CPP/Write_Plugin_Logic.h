#ifndef _WRITE_PLUGIN_LOGIC_H
#define _WRITE_PLUGIN_LOGIC_H

#include "XmlOpeation.h"

_Xml_Info* Find_Logic_StructInfo(int nCommandID, vecXmlInfo& objvecXmlInfo)
{
	for(int i = 0; i < (int)objvecXmlInfo.size(); i++)
	{
		if(objvecXmlInfo[i].m_nCommandID == nCommandID)
		{
			return &objvecXmlInfo[i];
		}
	}

	return NULL;
}

void Gen_2_Cpp_Logic_H(_Project_Info& objProjectInfo, vecXmlInfo& objvecXmlInfo)
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
		_Xml_Info* pXmlIn  = Find_Logic_StructInfo(objProjectInfo.m_objCommandList[i].m_nCommandInID, objvecXmlInfo);
		_Xml_Info* pXmlOut = NULL;
		if(objProjectInfo.m_objCommandList[i].m_nCommandOutID > 0)
		{
			pXmlOut = Find_Logic_StructInfo(objProjectInfo.m_objCommandList[i].m_nCommandOutID, objvecXmlInfo);
		}

		if(NULL != pXmlIn)
		{
			if(NULL != pXmlOut)
			{
				//两边参数都有，写入函数
				sprintf_safe(szTemp, 200, "bool Logic_%s(%s& obj%s, %s& obj%s);\n\n", 
					pXmlIn->m_szXMLName,
					pXmlIn->m_szXMLName,
					pXmlIn->m_szXMLName,
					pXmlOut->m_szXMLName,
					pXmlOut->m_szXMLName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
		}
	}

	fclose(pFile);
}


void Gen_2_Cpp_Logic_Cpp(_Project_Info& objProjectInfo, vecXmlInfo& objvecXmlInfo)
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
		_Xml_Info* pXmlIn  = Find_Logic_StructInfo(objProjectInfo.m_objCommandList[i].m_nCommandInID, objvecXmlInfo);
		_Xml_Info* pXmlOut = NULL;
		if(objProjectInfo.m_objCommandList[i].m_nCommandOutID > 0)
		{
			pXmlOut = Find_Logic_StructInfo(objProjectInfo.m_objCommandList[i].m_nCommandOutID, objvecXmlInfo);
		}

		if(NULL != pXmlIn)
		{
			if(NULL != pXmlOut)
			{
				//两边参数都有，写入函数
				sprintf_safe(szTemp, 200, "bool Logic_%s(%s& obj%s, %s& obj%s)\n", 
					pXmlIn->m_szXMLName,
					pXmlIn->m_szXMLName,
					pXmlIn->m_szXMLName,
					pXmlOut->m_szXMLName,
					pXmlOut->m_szXMLName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t//add your dispose code at here.\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\treturn true;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "}\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else
			{
				//两边参数都有，写入函数
				sprintf_safe(szTemp, 200, "bool Logic_%s(%s& obj%s)\n", 
					pXmlIn->m_szXMLName,
					pXmlIn->m_szXMLName,
					pXmlIn->m_szXMLName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t//add your dispose code at here.\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\treturn true;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "}\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
		}
	}

	fclose(pFile);
}

#endif
