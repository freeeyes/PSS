// XML2CPP.cpp : 定义控制台应用程序的入口点。
//
#ifdef WIN32
#include <direct.h>
#else
#include <sys/stat.h> 
#endif

#include "Write_Packet_Parse_cpp.h"
#include "Write_Plugin_Main_cpp.h"
#include "Write_Plugin_Command.h"
#include "Write_Plugin_Make.h"
#include "Write_Plugin_Logic.h"

#define PROJECT_FILE_NAME     "PlugInInfo.xml"
#define DATASTRUCT_FILE_NAME  "DataStruct.xml"

int main(int argc, char* argv[])
{
	char szFilePath[255]   = {'\0'};
	char szConfigFile[255] = {'\0'};

	if(NULL == argv[1])
	{
		sprintf_safe(szFilePath, 255, "./Conf/");
	}
	else
	{
		sprintf_safe(szFilePath, 255, "%s", argv[1]);
	}

	//读取工程文件
	_Project_Info objProjectInfo;
	sprintf_safe(szConfigFile, 255, "%s%s", szFilePath, PROJECT_FILE_NAME);
	bool blState = Read_Project_File(szConfigFile, objProjectInfo);
	if(false == blState)
	{
		printf("[error]Project XML is error.\n");
		return 0;
	}

	//读取命令文件
	vecClassInfo objvecClassInfo;
	sprintf_safe(szConfigFile, 255, "%s%s", szFilePath, DATASTRUCT_FILE_NAME);
	blState = Read_Command_File(szConfigFile, objvecClassInfo);
	if(false == blState)
	{
		printf("[error]Command XML is error.\n");
		return 0;
	}

	//创建工程目录
	if(strlen(objProjectInfo.m_szProjectName) == 0)
	{
		printf("[error]m_szProjectName is null.\n");
		return 0;
	}

	//创建生成目录
#ifdef WIN32
	_mkdir(objProjectInfo.m_szProjectName);
#else
	mkdir(objProjectInfo.m_szProjectName);
#endif

	//写入主文件
	Gen_2_Cpp_Main(objProjectInfo, objvecClassInfo);

	//写入数据包文件
	Gen_2_Cpp_Packet(objProjectInfo, objvecClassInfo);

	//写入Command头文件
	Gen_2_Cpp_Command_H(objProjectInfo, objvecClassInfo);
	
	//写入Command体文件
	Gen_2_Cpp_Command_Cpp(objProjectInfo, objvecClassInfo);

	//生成make文件
	//Gen_2_Make_define(objProjectInfo.m_szProjectName);
	//Gen_2_Make(objProjectInfo.m_szProjectName, objProjectInfo);
	Gen_2_Mpc(objProjectInfo.m_szProjectName, objProjectInfo);
	Gen_2_RunLinuxMake(objProjectInfo.m_szProjectName, objProjectInfo);
	Gen_2_ZZZ(objProjectInfo.m_szProjectName);

	Gen_2_Cpp_Logic_H(objProjectInfo, objvecClassInfo);
	Gen_2_Cpp_Logic_Cpp(objProjectInfo, objvecClassInfo);

	printf("[success]OK.\n");
	getchar();
	return 0;
}

