#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdarg.h>
#ifdef _WIN32
#include <io.h>
#include "winsock2.h"
#else
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <time.h>
#endif

#include <string>
#include <vector>
using namespace std;


//支持memcpy的边界检查
inline bool memcpy_safe(char* pSrc, int nSrcLen, char* pDes, int nDesLen)
{
	if(nSrcLen > nDesLen)
	{
		return false;
	}
	else
	{
#ifdef WIN32
		memcpy_s((void* )pDes, nDesLen, (void* )pSrc, (size_t)nSrcLen);
#else
		memcpy((void* )pDes, (void* )pSrc, (size_t)nSrcLen);
#endif
		return true;
	}
}

static unsigned long GetSystemTickCount()
{
#ifdef WIN32
	return GetTickCount();
#else
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);

	return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
#endif
}

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

//将字符串转换为数字
static int Char2Number(const char* pData)
{
	if(pData[0] == '0' && pData[1] == 'x')
	{
		//十六进制转换
		return (int)strtol(pData, NULL, 16);
	}
	else
	{
		//十进制转换
		return (int)atoi(pData);
	}
}

//字符串转二进制相关API
static bool Get_binary_Char(unsigned char cTag, unsigned char& cDes)
{
	if(cTag >='A'&&  cTag <='F')
	{
		cDes = cTag - 'A' + 10;
		return true;
	}
	else if(cTag >='a'&&  cTag <='f')
	{
		cDes = cTag - 'a' + 10;
		return true; 
	}
	else if(cTag >= '0'&& cTag<= '9')
	{
		cDes = cTag-'0';
		return true;
	}
	else
	{
		return false;
	}
}

static bool ConvertStr2char(const char* pData, unsigned char& cData)
{
	if(pData == NULL || strlen(pData) != 2)
	{
		return false;
	}

	char cFirst = pData[1];
	unsigned char cTemp = 0;
	bool blStste = Get_binary_Char(cFirst, cTemp);
	if(false == blStste)
	{
		return false;
	}
	cData = cTemp;
	char cSecond = pData[0];
	blStste  = Get_binary_Char(cSecond, cTemp);
	if(false == blStste)
	{
		return false;
	}
	cTemp = cTemp << 4;
	cData = cData | cTemp;

	return true;
}

//得到转换后的字符串长度
static int GetBufferSize(const char* pData, int nSrcLen)
{
	char szData[3] = {'\0'};
	int nPos         = 0;
	int nCurrSize    = 0;
	int nConvertSize = 0;
	bool blState     = false;   //转换后的字符串是否有效
	bool blSrcState  = true;    //元字符串是否有效
	unsigned char cData;

	while(nPos < nSrcLen)
	{
		if(pData[nPos] == '\r' || pData[nPos] == '\n' || pData[nPos] == ' ' || nPos == nSrcLen - 1)
		{
			if(nPos == nSrcLen - 1)
			{
				szData[nCurrSize++] = pData[nPos];
			}

			szData[nCurrSize] = '\0';
			if(blSrcState == true)
			{
				blState = ConvertStr2char(szData, cData);
				if(blState == true)
				{
					nConvertSize++;
				}
			}
			nCurrSize  = 0;
			blSrcState = true;
			nPos++;
		}
		else
		{
			if(nCurrSize < 2)
			{
				szData[nCurrSize++] = pData[nPos];
			}
			else
			{
				blSrcState = false;
			}
			nPos++;
		}
	}

	return nConvertSize;
};

//填充字符串转换为二进制串的代码
static bool Convertstr2charArray(const char* pData, int nSrcLen, unsigned char* pDes, int& nMaxLen)
{
	char szData[3] = {'\0'};
	int nPos         = 0;
	int nCurrSize    = 0;
	int nConvertSize = 0;
	bool blState     = false;   //转换后的字符串是否有效
	bool blSrcState  = true;    //元字符串是否有效

	while(nPos < nSrcLen)
	{
		if(pData[nPos] == '\r' || pData[nPos] == '\n' || pData[nPos] == ' ' || nPos == nSrcLen - 1)
		{
			if(nPos == nSrcLen - 1)
			{
				szData[nCurrSize++] = pData[nPos];
			}

			szData[nCurrSize] = '\0';
			if(nConvertSize < nMaxLen && blSrcState == true)
			{
				blState = ConvertStr2char(szData, pDes[nConvertSize]);
				if(blState == true)
				{
					nConvertSize++;
				}
			}
			nCurrSize  = 0;
			blSrcState = true;
			nPos++;
		}
		else
		{
			if(nCurrSize < 2)
			{
				szData[nCurrSize++] = pData[nPos];
			}
			else
			{
				blSrcState = false;
			}
			nPos++;
		}
	}

	nMaxLen = nConvertSize;
	return true;
};

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

	int Get_Length()
	{
		int nSize = 0;
		for(int i = 0; i < (int)m_obj_Data_Info_List.size(); i++)
		{
			if(strcmp(m_obj_Data_Info_List[i].m_szDataType, "short") == 0)
			{
				nSize += 2;
			}
			else if(strcmp(m_obj_Data_Info_List[i].m_szDataType, "int") == 0)
			{
				nSize += 4;
			}
			else if(strcmp(m_obj_Data_Info_List[i].m_szDataType, "char") == 0)
			{
				if(m_obj_Data_Info_List[i].m_nLength > 0)
				{
					nSize += m_obj_Data_Info_List[i].m_nLength;
				}
				else
				{
					nSize += (int)strlen(m_obj_Data_Info_List[i].m_strValue.c_str());
				}
			}
		}

		return nSize;
	}

	//输入成数据流
	void In_Stream(char* pData, int nLen, short sOrder)
	{
		int nPos = 0;
		for(int i = 0; i < (int)m_obj_Data_Info_List.size(); i++)
		{
			if(strcmp(m_obj_Data_Info_List[i].m_szDataType, "short") == 0)
			{
				short sData = (short)Char2Number(m_obj_Data_Info_List[i].m_strValue.c_str());
				if(sOrder == 1)
				{
					//网序
					sData = htons(sData);
				}
				memcpy_safe((char* )&sData, 2, &pData[nPos], nLen - nPos);
				nPos += 2;
			}
			else if(strcmp(m_obj_Data_Info_List[i].m_szDataType, "int") == 0)
			{
				int nData = (int)Char2Number(m_obj_Data_Info_List[i].m_strValue.c_str());
				if(sOrder == 1)
				{
					//网序
					nData = htonl(nData);
				}
				memcpy_safe((char* )&nData, 4, &pData[nPos], nLen - nPos);
				nPos += 4;
			}
			else if(strcmp(m_obj_Data_Info_List[i].m_szDataType, "char") == 0)
			{
				if(m_obj_Data_Info_List[i].m_nLength > 0)
				{
					if(m_obj_Data_Info_List[i].m_blIsString == true)
					{
						memcpy_safe((char* )m_obj_Data_Info_List[i].m_strValue.c_str(), m_obj_Data_Info_List[i].m_nLength, &pData[nPos], nLen - nPos);
						nPos += m_obj_Data_Info_List[i].m_nLength;
					}
					else
					{
						//二进制数据拼装
						int nValueLen = GetBufferSize((char*)m_obj_Data_Info_List[i].m_strValue.c_str(), m_obj_Data_Info_List[i].m_strValue.length());
						char* pValue  = new char[nValueLen];
						Convertstr2charArray(m_obj_Data_Info_List[i].m_strValue.c_str(),  m_obj_Data_Info_List[i].m_strValue.length(), (unsigned char* )pValue, nValueLen);
						memcpy_safe(pValue, nValueLen, &pData[nPos], nLen - nPos);
						delete pValue;
						nPos += m_obj_Data_Info_List[i].m_nLength;
					}
				}
				else if(m_obj_Data_Info_List[i].m_nLength == 1)
				{
					short sData = (short)Char2Number(m_obj_Data_Info_List[i].m_strValue.c_str());
					memcpy_safe((char* )&sData, 1, &pData[nPos], nLen - nPos);
					nPos += m_obj_Data_Info_List[i].m_nLength;
				}
				else
				{
					if(m_obj_Data_Info_List[i].m_blIsString == true)
					{
						memcpy_safe((char* )m_obj_Data_Info_List[i].m_strValue.c_str(), (int)strlen(m_obj_Data_Info_List[i].m_strValue.c_str()), &pData[nPos], nLen - nPos);
						nPos += (int)strlen(m_obj_Data_Info_List[i].m_strValue.c_str());
					}
					else
					{
						//二进制数据拼装
						int nValueLen = GetBufferSize((char*)m_obj_Data_Info_List[i].m_strValue.c_str(), m_obj_Data_Info_List[i].m_strValue.length());
						char* pValue  = new char[nValueLen];
						Convertstr2charArray(m_obj_Data_Info_List[i].m_strValue.c_str(),  m_obj_Data_Info_List[i].m_strValue.length(), (unsigned char* )pValue, nValueLen);
						memcpy_safe(pValue, nValueLen, &pData[nPos], nLen - nPos);
						delete pValue;
						nPos += nValueLen;
					}
				}
			}
		}
	}
};

struct _Packet_Recv
{
	vec_Data_Info m_obj_Data_Info_List;

	int Get_Length()
	{
		int nSize = 0;
		for(int i = 0; i < (int)m_obj_Data_Info_List.size(); i++)
		{
			if(strcmp(m_obj_Data_Info_List[i].m_szDataType, "short") == 0)
			{
				nSize += 2;
			}
			else if(strcmp(m_obj_Data_Info_List[i].m_szDataType, "int") == 0)
			{
				nSize += 4;
			}
			else if(strcmp(m_obj_Data_Info_List[i].m_szDataType, "char") == 0)
			{
				if(m_obj_Data_Info_List[i].m_nLength > 0)
				{
					nSize += m_obj_Data_Info_List[i].m_nLength;
				}
				else
				{
					nSize += (int)strlen(m_obj_Data_Info_List[i].m_strValue.c_str());
				}
			}

		}
		return nSize;
	}

	string Check_Stream(char* pData, int nLen, short sOrder, bool& blIsError)
	{
		string strRet = "接收数据包检测成功";
		int nPos = 0;
		for(int i = 0; i < (int)m_obj_Data_Info_List.size(); i++)
		{
			if(strcmp(m_obj_Data_Info_List[i].m_szDataType, "short") == 0)
			{
				short sData = 0;
				memcpy_safe(&pData[nPos], 2, (char* )&sData, 2);
				if(sOrder == 1)
				{
					//网序
					sData = ntohs(sData);
				}
				if(sData != (short)Char2Number(m_obj_Data_Info_List[i].m_strValue.c_str()))
				{
					char szError[MAX_BUFF_50] = {'\0'};
					sprintf_safe(szError, MAX_BUFF_50, "错误字段[%s],期待数值[%d],实际数值[%d]", 
						m_obj_Data_Info_List[i].m_szDataName, 
						(short)atoi(m_obj_Data_Info_List[i].m_strValue.c_str()), 
						sData);
					blIsError = true;
					return strRet; 
				}
				nPos += 2;
			}
			else if(strcmp(m_obj_Data_Info_List[i].m_szDataType, "int") == 0)
			{
				int nData = 0;
				memcpy_safe(&pData[nPos], 4, (char* )&nData, 4);
				if(sOrder == 1)
				{
					//网序
					nData = ntohl(nData);
				}
				if(nData != (int)Char2Number(m_obj_Data_Info_List[i].m_strValue.c_str()))
				{
					char szError[MAX_BUFF_50] = {'\0'};
					sprintf_safe(szError, MAX_BUFF_50, "错误字段[%s],期待数值[%d],实际数值[%d]", 
						m_obj_Data_Info_List[i].m_szDataName, 
						(int)atoi(m_obj_Data_Info_List[i].m_strValue.c_str()), 
						nData);
					blIsError = true;
					return strRet; 
				}
				nPos += 4;
			}
			else if(strcmp(m_obj_Data_Info_List[i].m_szDataType, "char") == 0)
			{
				if(m_obj_Data_Info_List[i].m_nLength > 1)
				{
					char* pObjectData = new char[m_obj_Data_Info_List[i].m_nLength + 1];
					memcpy_safe(&pData[nPos], m_obj_Data_Info_List[i].m_nLength, (char* )pObjectData, m_obj_Data_Info_List[i].m_nLength);
					pObjectData[m_obj_Data_Info_List[i].m_nLength] = '\0';
					if(strcmp(pObjectData, m_obj_Data_Info_List[i].m_strValue.c_str()) != 0)
					{
						char szError[MAX_BUFF_500] = {'\0'};
						sprintf_safe(szError, MAX_BUFF_50, "错误字段[%s],期待数值[%s],实际数值[%s]", 
							m_obj_Data_Info_List[i].m_szDataName, 
							m_obj_Data_Info_List[i].m_strValue.c_str(), 
							pObjectData);
						delete pObjectData;
						blIsError = true;
						return strRet; 
					}

					delete pObjectData;
					nPos += m_obj_Data_Info_List[i].m_nLength;
				}
				else if(m_obj_Data_Info_List[i].m_nLength == 1)
				{
					//如果是一个字节，则直接认为是数字
					char* pObjectData = new char[m_obj_Data_Info_List[i].m_nLength + 1];
					memcpy_safe(&pData[nPos], m_obj_Data_Info_List[i].m_nLength, (char* )pObjectData, m_obj_Data_Info_List[i].m_nLength);
					pObjectData[m_obj_Data_Info_List[i].m_nLength] = '\0';
					if(atoi(pObjectData) != atoi(m_obj_Data_Info_List[i].m_strValue.c_str()))
					{
						char szError[MAX_BUFF_500] = {'\0'};
						sprintf_safe(szError, MAX_BUFF_50, "错误字段[%s],期待数值[%s],实际数值[%s]", 
							m_obj_Data_Info_List[i].m_szDataName, 
							m_obj_Data_Info_List[i].m_strValue.c_str(), 
							pObjectData);
						delete pObjectData;
						blIsError = true;
						return strRet; 
					}
					delete pObjectData;
					nPos += m_obj_Data_Info_List[i].m_nLength;
				}
				else
				{
					char* pObjectData = new char[nLen - nPos + 1];
					memcpy_safe(&pData[nPos], nLen - nPos, (char* )pObjectData, nLen - nPos);
					pObjectData[nLen - nPos] = '\0';
					if(strcmp(pObjectData, m_obj_Data_Info_List[i].m_strValue.c_str()) != 0)
					{
						char szError[MAX_BUFF_500] = {'\0'};
						sprintf_safe(szError, MAX_BUFF_50, "错误字段[%s],期待数值[%s],实际数值[%s]", 
							m_obj_Data_Info_List[i].m_szDataName, 
							m_obj_Data_Info_List[i].m_strValue.c_str(), 
							pObjectData);
						delete pObjectData;
						blIsError = true;
						return strRet; 
					}

					delete pObjectData;
					nPos += (int)strlen(m_obj_Data_Info_List[i].m_strValue.c_str());
				}
			}

		}

		blIsError = false;
		return strRet;
	}
};

struct _Command_Info
{
	char m_szCommandName[MAX_BUFF_50];
	int  m_nCount;
	int  m_nTimeCost;   //单位是毫秒
	int  m_nThreadCount; //测试线程数
	_Packet_Send m_obj_Packet_Send;
	_Packet_Recv m_obj_Packet_Recv;

	_Command_Info()
	{
		m_szCommandName[0] = '\0';
		m_nCount           = 1;
		m_nTimeCost        = 100;
		m_nThreadCount     = 1;
	}
};
typedef vector<_Command_Info> vec_Command_Info;

struct _Test_Assemble
{
	char m_szTestAssembleName[MAX_BUFF_50];
	char m_szDesc[MAX_BUFF_100];
	char m_szIP[MAX_BUFF_50];
	char m_szOrder[MAX_BUFF_50];
	int  m_nPort;
	vec_Command_Info m_obj_Command_Info_List;

	_Test_Assemble()
	{
		m_szTestAssembleName[0] = '\0';
		m_szDesc[0]             = '\0';
		m_szIP[0]               = '\0';
		m_nPort                 = 0;
		sprintf_safe(m_szOrder, MAX_BUFF_50,"HOST");
	}
};
typedef vector<_Test_Assemble> vec_Test_Assemble;

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
	sprintf_safe(szTemp, MAX_BUFF_500, "<head><style>#ver-minimalist { \
										font-family: \"Lucida Sans Unicode\", \"Lucida Grande\", Sans- Serif;\
										font-size: 12px;margin: 45px;width: 1200px;text-align: left; \
										border-collapse: collapse;}#ver-minimalist th { padding: 8px 2px; font-weight: normal;");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, MAX_BUFF_500, "font-size: 14px;border-bottom: 2px solid #6678b1;border-right: 30px solid #fff;border-left: 30px solid #fff; \
									   color: #039;}#ver-minimalist td { padding: 12px 2px 0px 2px; border-right: 30px solid #fff; border-left: 30px solid #fff; \
									   color: #669;}</style></head>\n");
		fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, MAX_BUFF_500, "<body style='margin: 0; padding: 0;'>\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, MAX_BUFF_500, "<table border='1' cellpadding='0' cellspacing='0' width='800' align='center'  id='ver-minimalist'>\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	fflush(pFile);
}

static void Create_TD_Title(FILE* pFile, const char* pAssemableName, const char* pDesc, const char* pIP, int nPort)
{
	char szTemp[MAX_BUFF_500]     = {'\0'};

	sprintf_safe(szTemp, MAX_BUFF_500, "<TR>\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, MAX_BUFF_500, "<td colspan='3' class='title' align='center'>%s:%s(%s:%d)</td>\n", pAssemableName, pDesc, pIP, nPort);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, MAX_BUFF_500, "</TR>\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	fflush(pFile);
}

static void Create_TD_Content(FILE* pFile, const char* pCssName, const char* pCommandName, const char* pContent, const char* pTime)
{
	char szTemp[MAX_BUFF_500]     = {'\0'};

	sprintf_safe(szTemp, MAX_BUFF_500, "<TR>\n");
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, MAX_BUFF_500, "<td  width='200' class='%s' align='center'>%s</td>\n", pCssName, pCommandName);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, MAX_BUFF_500, "<td  width='400' class='%s' align='center'>%s</td>\n", pCssName, pContent);
	fwrite(szTemp, strlen(szTemp), sizeof(char), pFile);
	sprintf_safe(szTemp, MAX_BUFF_500, "<td  width='200' class='%s' align='center'>%s</td>\n", pCssName, pTime);
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
