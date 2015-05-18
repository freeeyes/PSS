#ifndef _WRITE_PLUGIN_COMMAND_H
#define _WRITE_PLUGIN_COMMAND_H

#include "XmlOpeation.h"

void Gen_2_Cpp_Command_H(_Project_Info& objProjectInfo)
{
	char szTemp[200]     = {'\0'};
	char szPathFile[200] = {'\0'};

	sprintf_safe(szPathFile, 200, "%s/BaseCommand.h", 
		objProjectInfo.m_szProjectName);

	//首先生成声明文件。
	FILE* pFile = fopen(szPathFile, "w");
	if(NULL == pFile)
	{
		return;
	}

	sprintf_safe(szTemp, 200, "#pragma once\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "#include \"%s_Logic.h\"\n",
		objProjectInfo.m_szProjectName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "#include \"ClientCommand.h\"\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "#include \"IObject.h\"\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "#include <string>\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "#define MESSAGE_FUNCTION_BEGIN(x) switch(x) {\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "#define MESSAGE_FUNCTION(x,y,z) case x: { y(z); break; }\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "#define MESSAGE_FUNCTION_END }\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "using namespace std;\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	sprintf_safe(szTemp, 200, "class CBaseCommand : public CClientCommand\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "{\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "public:\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\tCBaseCommand(void);\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t~CBaseCommand(void);\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\tint DoMessage(IMessage* pMessage, bool& bDeleteFlag);\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\tvoid SetServerObject(CServerObject* pServerObject);\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "private:\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	for(int i = 0; i < (int)objProjectInfo.m_objCommandList.size(); i++)
	{
		sprintf_safe(szTemp, 200, "\tint %s(IMessage* pMessage);\n", 
			objProjectInfo.m_objCommandList[i].m_szCommandFuncName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}
	sprintf_safe(szTemp, 200, "\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "private:\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\tCServerObject* m_pServerObject;\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\tint            m_nCount;\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "};\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	fclose(pFile);
}

_Xml_Info* Find_Xml_StructInfo(int nCommandID, vecXmlInfo& objvecXmlInfo)
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

void Gen_2_Cpp_Command_Cpp(_Project_Info& objProjectInfo, vecXmlInfo& objvecXmlInfo)
{
	char szTemp[200]     = {'\0'};
	char szPathFile[200] = {'\0'};

	sprintf_safe(szPathFile, 200, "%s/BaseCommand.cpp", 
		objProjectInfo.m_szProjectName);

	//首先生成声明文件。
	FILE* pFile = fopen(szPathFile, "w");
	if(NULL == pFile)
	{
		return;
	}

	sprintf_safe(szTemp, 200, "#include \"BaseCommand.h\"\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "CBaseCommand::CBaseCommand(void)\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "{\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\tm_pServerObject = NULL;\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\tm_nCount        = 0;\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "}\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "CBaseCommand::~CBaseCommand(void)\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "{\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "}\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "void CBaseCommand::SetServerObject(CServerObject* pServerObject)\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "{\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\tm_pServerObject = pServerObject;\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "}\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "int CBaseCommand::DoMessage(IMessage* pMessage, bool& bDeleteFlag)\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "{\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t__ENTER_FUNCTION();\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\tif(m_pServerObject == NULL)\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t{\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t\tOUR_DEBUG((LM_ERROR, \"[CBaseCommand::DoMessage] m_pServerObject is NULL.\\n\"));\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t\treturn -1;\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t}\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\tif(pMessage == NULL)\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t{\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t\tOUR_DEBUG((LM_ERROR, \"[CBaseCommand::DoMessage] pMessage is NULL.\\n\"));\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t}\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\tMESSAGE_FUNCTION_BEGIN(pMessage->GetMessageBase()->m_u2Cmd);\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	for(int i = 0; i < (int)objProjectInfo.m_objCommandList.size(); i++)
	{
		_Xml_Info* pXmlInfo = Find_Xml_StructInfo(objProjectInfo.m_objCommandList[i].m_nCommandInID, objvecXmlInfo);
		if(NULL != pXmlInfo)
		{
			if(strlen(pXmlInfo->m_szMacroName) > 0)
			{
				sprintf_safe(szTemp, 200, "\tMESSAGE_FUNCTION((uint16)%s,  %s, pMessage);\n",
					pXmlInfo->m_szMacroName,
					objProjectInfo.m_objCommandList[i].m_szCommandFuncName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
			else
			{
				sprintf_safe(szTemp, 200, "\tMESSAGE_FUNCTION((uint16)%d,  %s, pMessage);\n",
					objProjectInfo.m_objCommandList[i].m_nCommandInID,
					objProjectInfo.m_objCommandList[i].m_szCommandFuncName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
		}
	}
	sprintf_safe(szTemp, 200, "\tMESSAGE_FUNCTION_END;\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\treturn 0;\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "\t__LEAVE_FUNCTION_WITHRETURN(0);\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, 200, "}\n\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

	for(int i = 0; i < (int)objProjectInfo.m_objCommandList.size(); i++)
	{
		sprintf_safe(szTemp, 200, "int CBaseCommand::%s(IMessage* pMessage)\n",
			objProjectInfo.m_objCommandList[i].m_szCommandFuncName);
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "{\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);

		if(objProjectInfo.m_objCommandList[i].m_nCommandInID > 0)
		{
			_Xml_Info* pXmlInfo = Find_Xml_StructInfo(objProjectInfo.m_objCommandList[i].m_nCommandInID, objvecXmlInfo);
			if(NULL != pXmlInfo)
			{
				sprintf_safe(szTemp, 200, "\tIBuffPacket* pRecvPacket = m_pServerObject->GetPacketManager()->Create();\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\tif(NULL == pRecvPacket)\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\tOUR_DEBUG((LM_ERROR, \"[CBaseCommand::%s] pRecvPacket is NULL.\\n\"));\n",
					objProjectInfo.m_objCommandList[i].m_szCommandFuncName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\treturn -1;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t}\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t_PacketInfo objRecvPacket;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\tpMessage->GetPacketBody(objRecvPacket);\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\tpRecvPacket->WriteStream(objRecvPacket.m_pData, objRecvPacket.m_nDataLen);\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t%s obj%s;\n",
					pXmlInfo->m_szXMLName,
					pXmlInfo->m_szXMLName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t%s_Out_Stream(obj%s, pRecvPacket);\n",
					pXmlInfo->m_szXMLName,
					pXmlInfo->m_szXMLName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\tm_pServerObject->GetPacketManager()->Delete(pRecvPacket);\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\tbool blState = false;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				if(objProjectInfo.m_objCommandList[i].m_nCommandOutID > 0)
				{
					_Xml_Info* pXmlOut = Find_Xml_StructInfo(objProjectInfo.m_objCommandList[i].m_nCommandOutID, objvecXmlInfo);
					if(NULL != pXmlOut)
					{
						sprintf_safe(szTemp, 200, "\t%s obj%s;\n",
							pXmlOut->m_szXMLName,
							pXmlOut->m_szXMLName);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
						sprintf_safe(szTemp, 200, "\tblState = Logic_%s(obj%s, obj%s);\n",
							pXmlInfo->m_szXMLName,
							pXmlInfo->m_szXMLName,
							pXmlOut->m_szXMLName);
						fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
					}
				}
				else
				{
					sprintf_safe(szTemp, 200, "\tblState = Logic_%s(obj%s);\n", 
						pXmlInfo->m_szXMLName,
						pXmlInfo->m_szXMLName);
					fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				}
				sprintf_safe(szTemp, 200, "\tif(false == blState)\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\treturn -1;\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t}\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
		}

		if(objProjectInfo.m_objCommandList[i].m_nCommandOutID > 0)
		{
			_Xml_Info* pXmlInfo = Find_Xml_StructInfo(objProjectInfo.m_objCommandList[i].m_nCommandOutID, objvecXmlInfo);
			if(NULL != pXmlInfo)
			{
				sprintf_safe(szTemp, 200, "\tIBuffPacket* pSendPacket = m_pServerObject->GetPacketManager()->Create();\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\tif(NULL == pSendPacket)\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\tOUR_DEBUG((LM_ERROR, \"[CBaseCommand::%s] pSendPacket is NULL.\\n\"));\n",
					objProjectInfo.m_objCommandList[i].m_szCommandFuncName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\treturn -1;\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t}\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t%s_In_Stream(obj%s, pSendPacket);\n\n",
					pXmlInfo->m_szXMLName,
					pXmlInfo->m_szXMLName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				//发送数据
				sprintf_safe(szTemp, 200, "\tif(NULL != m_pServerObject->GetConnectManager())\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);;
				sprintf_safe(szTemp, 200, "\t\tm_pServerObject->GetConnectManager()->PostMessage((uint16)%d,\n",
					objProjectInfo.m_objCommandList[i].m_nCommandOutID);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\t\tpSendPacket,\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\t\tSENDMESSAGE_NOMAL,\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\t\t(uint16)%d,\n", 
					objProjectInfo.m_objCommandList[i].m_nCommandOutID);
				sprintf_safe(szTemp, 200, "\t\t\tPACKET_SEND_IMMEDIATLY,\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\t\tPACKET_IS_FRAMEWORK_RECYC);\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t}\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\telse\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t{\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\tOUR_DEBUG((LM_INFO, \"[CBaseCommand::%s] m_pConnectManager = NULL.\\n\"));\n",
					objProjectInfo.m_objCommandList[i].m_szCommandFuncName);
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t\tm_pServerObject->GetPacketManager()->Delete(pSendPacket);\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
				sprintf_safe(szTemp, 200, "\t}\n\n");
				fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
			}
		}
		sprintf_safe(szTemp, 200, "\treturn 0;\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
		sprintf_safe(szTemp, 200, "}\n\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	}

	fclose(pFile);
}

#endif
