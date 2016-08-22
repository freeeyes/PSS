#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdarg.h>
#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#endif

#include <string>
#include <vector>
using namespace std;

typedef vector<string> vec_Xml_File_Name;

#define MAX_BUFF_50  50
#define MAX_BUFF_100 100
#define MAX_BUFF_500 500

//发送数据格式
struct _Data_Info
{
	char   m_szDataName[MAX_BUFF_50];
	char   m_szDataType[MAX_BUFF_50];
	int    m_nLength;
	bool   m_blIsString;
	string m_strValue;

	_Data_Info()
	{
		m_szDataName[0] = '\0';
		m_szDataType[0] = '\0';
		m_nLength       = 0;
		m_blIsString    = true;
		m_strValue      = "";
	}
};
typedef vector<_Data_Info> vec_Data_Info;

struct _Packet_Send
{
	vec_Data_Info m_obj_Data_Info_List;
};

struct _Packet_Recv
{
	vec_Data_Info m_obj_Data_Info_List;
};

struct _Command_Info
{
	char m_szCommandName[MAX_BUFF_50];
	int  m_nCount;
	int  m_nTimeCost;   //单位是毫秒
	_Packet_Send m_obj_Packet_Send;
	_Packet_Recv m_obj_Packet_Recv;

	_Command_Info()
	{
		m_szCommandName[0] = '\0';
		m_nCount           = 1;
		m_nTimeCost        = 100;
	}
};
typedef vector<_Command_Info> vec_Command_Info;

struct _Test_Assemble
{
	char m_szTestAssembleName[MAX_BUFF_50];
	char m_szDesc[MAX_BUFF_100];
	char m_szIP[MAX_BUFF_50];
	int  m_nPort;
	vec_Command_Info m_obj_Command_Info_List;

	_Test_Assemble()
	{
		m_szTestAssembleName[0] = '\0';
		m_szDesc[0]             = '\0';
		m_szIP[0]               = '\0';
		m_nPort                 = 0;
	}
};
typedef vector<_Test_Assemble> vec_Test_Assemble;

//重载sprintf
static void sprintf_safe(char* szText, int nLen, const char* fmt ...)
{
	if(szText == NULL)
	{
		return;
	}

	va_list ap;
	va_start(ap, fmt);

	vsnprintf(szText, nLen, fmt, ap);
	szText[nLen - 1] = '\0';

	va_end(ap);
};


//遍历指定的目录，获得所有XML文件名
static bool Read_Xml_Folder( string folderPath, vec_Xml_File_Name& obj_vec_Xml_File_Name)
{
#ifdef WIN32
	_finddata_t FileInfo;
	string strfind = folderPath + "\\*";
	long Handle = _findfirst(strfind.c_str(), &FileInfo);

	if (Handle == -1L)
	{
		return false;
	}
	do
	{
		//判断是否有子目录
		if (FileInfo.attrib & _A_SUBDIR)    
		{
			//这个语句很重要
			if( (strcmp(FileInfo.name,".") != 0 ) &&(strcmp(FileInfo.name,"..") != 0))   
			{
				//不必支持子目录遍历
				//string newPath = folderPath + "\\" + FileInfo.name;
				//dfsFolder(newPath);
			}
		}
		else  
		{
			string filename = folderPath + "\\" + FileInfo.name;
			obj_vec_Xml_File_Name.push_back(filename);
		}
	} while (_findnext(Handle, &FileInfo) == 0);

	_findclose(Handle);
#else
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	if((dp = opendir(folderPath.c_str())) == NULL) 
	{
		printf("cannot open directory: %s\n", folderPath.c_str());
		return false;
	}
	chdir(folderPath.c_str());
	while((entry = readdir(dp)) != NULL) 
	{
		lstat(entry->d_name,&statbuf);
		if(S_ISDIR(statbuf.st_mode)) 
		{
			if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0)
				continue;
			//不需要支持子目录遍历
			//printf("%*s%s/\n",depth,"",entry->d_name);
			//dfsFolder(entry->d_name,depth+4);
		} 
		else 
		{
			string filename = folderPath + "/" + entry->d_name;
			obj_vec_Xml_File_Name.push_back(filename);
		}
	}
	chdir("..");
	closedir(dp);
#endif
	return true;
}

static void Create_HTML_Begin(FILE* pFile)
{
	char szTemp[MAX_BUFF_500]     = {'\0'};

	sprintf_safe(szTemp, MAX_BUFF_500, "<html xmlns='http://www.w3.org/1999/xhtml'>\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, MAX_BUFF_500, "<head><style type=‘text/css’> \
									   .title { color: red; font-size: 15px; background-color:#D6D6D6; }\
									   .content { color: blue; font-size: 15px; background-color:#C7EDCC; }\
									   .error { color: black; font-size: 15px; background-color:yellow; }\
									   </style>\
									   </head>\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, MAX_BUFF_500, "<body style='margin: 0; padding: 0;'>\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, MAX_BUFF_500, "<table border='1' cellpadding='0' cellspacing='0' width='600' align='center'>\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	fflush(pFile);
}

static void Create_TD_Title(FILE* pFile, const char* pAssemableName, const char* pDesc, const char* pIP, int nPort)
{
	char szTemp[MAX_BUFF_500]     = {'\0'};

	sprintf_safe(szTemp, MAX_BUFF_500, "<TR>\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, MAX_BUFF_500, "<td colspan='2' class='title' align='center'>%s:%s(%s:%d)</td>\n", pAssemableName, pDesc, pIP, nPort);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, MAX_BUFF_500, "</TR>\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	fflush(pFile);
}

static void Create_TD_Content(FILE* pFile, const char* pCssName, const char* pCommandName, const char* pContent)
{
	char szTemp[MAX_BUFF_500]     = {'\0'};

	sprintf_safe(szTemp, MAX_BUFF_500, "<TR>\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, MAX_BUFF_500, "<td  width='200' class='%s' align='center'>%s</td>\n", pCssName, pCommandName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, MAX_BUFF_500, "<td  width='400' class='%s' align='center'>%s</td>\n", pCssName, pContent);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, MAX_BUFF_500, "</TR>\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	fflush(pFile);
}

static void Create_HTML_End(FILE* pFile)
{
	char szTemp[MAX_BUFF_500]     = {'\0'};
	sprintf_safe(szTemp, MAX_BUFF_500, "</table>\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, MAX_BUFF_500, "</body>\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, MAX_BUFF_500, "</html>\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	fflush(pFile);
}

#endif
