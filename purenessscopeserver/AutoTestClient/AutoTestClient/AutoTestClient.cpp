// AutoTestClient.cpp : 定义控制台应用程序的入口点。
//

#include "XmlOpeation.h"
#include "TcpSocketClient.h"

#define XML_PATH "XML_Packet"
#define XML_FILE_RESULT "Auto_Test_Resault.html"

//运行测试子集
void Run_Test(FILE* pFile, _Command_Info obj_Command_Info, const char* pIP, int nPort, short sOrder)
{
	//连接远程测试
	ODSocket obj_ODSocket;
	string strContent;
	string strTime;

	printf("[Run_Test]obj_Command_Info.m_szCommandName=%s.\n", obj_Command_Info.m_szCommandName);

	obj_ODSocket.Init();
	obj_ODSocket.Create(AF_INET, SOCK_STREAM, 0);
	bool blState = obj_ODSocket.Connect(pIP, nPort, 5);
	if(false == blState)
	{
		Create_TD_Content(pFile, "error", obj_Command_Info.m_szCommandName, "连接建立失败", strTime.c_str());
		return;
	}

	bool blIsError = false;
	unsigned long llBegin = GetSystemTickCount();
	for(int i = 0; i < obj_Command_Info.m_nCount; i++)
	{
		//开始发送数据
		int nSendLen = obj_Command_Info.m_obj_Packet_Send.Get_Length();
		char* pSend = new char[nSendLen];
		obj_Command_Info.m_obj_Packet_Send.In_Stream(pSend, nSendLen, sOrder);

		bool blSendFlag = false;
		int nCurrSend = 0;
		while(true)
		{	
			int nDataLen = obj_ODSocket.Send(&pSend[nCurrSend], nSendLen - nCurrSend);
			if(nDataLen < 0)
			{
				strContent = "发送数据包失败";
				blIsError = true;
				break;
			}
			else if(nDataLen == nSendLen - nCurrSend)
			{
				blSendFlag = true;
				break;
			}
			else
			{
				nCurrSend += nDataLen; 
			}
		}
		delete pSend;

		//判断是否存在问题
		if(blIsError == true)
		{
			break;
		}

		int nRecvLen = obj_Command_Info.m_obj_Packet_Recv.Get_Length();
		if(nRecvLen > 0 && blSendFlag == true)
		{
			//需要接受返回验证数据包
			char* pRecv = new char[nRecvLen];

			int nCurrRecv = 0;
			while(true)
			{
				int nDataLen = obj_ODSocket.Recv(&pRecv[nCurrRecv], nRecvLen - nCurrRecv);
				if(nDataLen <= 0)
				{
					strContent = "接收返回数据包失败";
					blIsError = true;
					break;
				}
				else if(nDataLen == nRecvLen - nCurrRecv)
				{
					//接受完成数据包
					strContent = obj_Command_Info.m_obj_Packet_Recv.Check_Stream(pRecv, nRecvLen, sOrder, blIsError);
					break;
				}
				else
				{
					//继续收包 
					nCurrRecv += nDataLen;
				}
			}
			delete pRecv;
		}

		//判断是否存在问题
		if(blIsError == true)
		{
			break;
		}
	}

	//计算时间消耗
	unsigned long llEnd = GetSystemTickCount();
	int nCostTime = (int)(llEnd - llBegin);

	char szTimeContent[MAX_BUFF_500] = {'\0'};
	if(nCostTime <= obj_Command_Info.m_nTimeCost)
	{
		sprintf_safe(szTimeContent, MAX_BUFF_500, "要求时间[%d]毫秒 实际消耗时间[%d]毫秒，正常", obj_Command_Info.m_nTimeCost, nCostTime);
	}
	else
	{
		sprintf_safe(szTimeContent, MAX_BUFF_500, "要求时间[%d]毫秒 实际消耗时间[%d]毫秒，异常", obj_Command_Info.m_nTimeCost, nCostTime);
	}
	strTime = szTimeContent;

	if(blIsError == true)
	{
		Create_TD_Content(pFile, "error", obj_Command_Info.m_szCommandName, strContent.c_str(), strTime.c_str());
	}
	else
	{
		Create_TD_Content(pFile, "content", obj_Command_Info.m_szCommandName, strContent.c_str(), strTime.c_str());
	}
	obj_ODSocket.Close();
}

//运行测试集并获得结果
void Run_Assemble_List(vec_Test_Assemble obj_Test_Assemble_List)
{
	FILE* pFile = fopen(XML_FILE_RESULT, "w");
	if(NULL == pFile)
	{
		printf("[Run_Assemble_List](%s) File Create fail.\n");
		return;
	}

	//创建html文件头
	Create_HTML_Begin(pFile);

	for(int i = 0; i < (int)obj_Test_Assemble_List.size(); i++)
	{
		Create_TD_Title(pFile, obj_Test_Assemble_List[i].m_szTestAssembleName, obj_Test_Assemble_List[i].m_szDesc, 
						obj_Test_Assemble_List[i].m_szIP, obj_Test_Assemble_List[i].m_nPort);

		short sOrder = 0;
		if(strcmp(obj_Test_Assemble_List[i].m_szOrder, "NET") == 0)
		{
			sOrder = 1;
		}

		for(int j = 0; j < (int)obj_Test_Assemble_List[i].m_obj_Command_Info_List.size(); j++)
		{
			Run_Test(pFile, obj_Test_Assemble_List[i].m_obj_Command_Info_List[j], 
						obj_Test_Assemble_List[i].m_szIP,
						obj_Test_Assemble_List[i].m_nPort,
						sOrder);
		}
	}
	Create_HTML_End(pFile);
}


int main(int argc, char* argv[])
{
	vec_Xml_File_Name obj_vec_Xml_File_Name;
	CXmlOpeation      obj_XmlOpeation;
	vec_Test_Assemble obj_Test_Assemble_List;

	bool blRet = Read_Xml_Folder(XML_PATH, obj_vec_Xml_File_Name);
	if(false == blRet)
	{
		printf("[Main]Get XML path error.\n");
		printf("[Main]please any key to exit.\n");
		getchar();
	}

	for(int i = 0; i < (int)obj_vec_Xml_File_Name.size(); i++)
	{
		_Test_Assemble obj_Test_Assemble;
		obj_XmlOpeation.Parse_XML_Test_Assemble(obj_vec_Xml_File_Name[i].c_str(), obj_Test_Assemble);
		obj_Test_Assemble_List.push_back(obj_Test_Assemble);
	}

	//运行测试用例
	Run_Assemble_List(obj_Test_Assemble_List);

	return 0;
}

