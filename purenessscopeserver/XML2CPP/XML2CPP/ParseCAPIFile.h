#ifndef _PARSE_C_API_FILE_H
#define _PARSE_C_API_FILE_H

#include "Common.h"
#include <string>
using namespace std ;

#define CAPI_INCLUDE_BEGIN "//add your Include file under at here, don't delete this!\n"
#define CAPI_INCLUDE_END "//add your Include file end, don't delete this!\n\n"

//记录引用的函数体代码
struct _FunctionCode
{
	char   m_szFuncName[200];  //函数名
	string m_strFuncCode;      //函数声明代码
	string m_strCode;          //函数代码
	string m_strNotes;         //函数代码声明注释
	bool   m_blIsUsed;

	_FunctionCode()
	{
		m_szFuncName[0] = '\0';
		m_blIsUsed      = false;
	}
};
typedef vector<_FunctionCode> vecFunctionCode;

//文件拆解信息
struct _File_Info
{
	string m_strExtHead;                  //记录其他的头文件 
	vecFunctionCode m_vecFunctionCode;    //记录函数信息
};

//解析出函数名
bool Parse_Function_Name(char* pLine, char* pFunctionName);

//解析包含的头文件
void Parse_File_Include(char* pData, int nFileSize, _File_Info& obj_File_Info);

//解析包含的所有函数名和函数体
void Parse_File_Function_Info(char* pData, int nFileSize, _File_Info& obj_File_Info);

//解析文件中所包含的头文件
bool Parse_CAPI_File(const char* pFileName, _File_Info& obj_File_Info);

//查找指定的函数对应的信息
bool Search_CAPI_Code(const char* pFuncName, _File_Info& obj_File_Info, _FunctionCode*& pFunctionCode);

#endif