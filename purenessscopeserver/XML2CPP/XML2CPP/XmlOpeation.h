#ifndef _XMLOPRATION_H
#define _XMLOPRATION_H

#include "tinyxml.h"
#include "tinystr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <vector>

using namespace std;

//将XML转换为MAP结构
//add by freeeyes

//支持类型
enum PROPERTY_TYPE
{
	PROPERTY_UNKNOW,
	PROPERTY_CHAR,
	PROPERTY_STRING,
	PROPERTY_UINT8,
	PROPERTY_UINT16,
	PROPERTY_UINT32,
	PROPERTY_UINT64,
	PROPERTY_INT8,
	PROPERTY_INT16,
	PROPERTY_INT32,
	PROPERTY_FLOAT32,
	PROPERTY_FLOAT64,
};

enum PROPERTY_CLASS
{
	CLASS_SINGLE,
	CLASS_VECTOR,
	CLASS_MAP,
};

enum COMMAND_TYPE
{
	COMMAND_NONE,
	COMMAND_IN,
	COMMAND_OUT,
};

//属性信息
struct _Property
{
	char           m_szPropertyName[50];   //参数名称
	char           m_szClassName[50];      //对象类名
	char           m_szDesc[100];          //描述信息  
	PROPERTY_TYPE  m_emType;               //参数类型
	PROPERTY_CLASS m_emClass;              //参数类别
	int            m_nLength;              //长度
	int            m_nNeedHeadLength;      //是否需要自动追加数据长度 0追加，1不追加
	char           m_szKeyName[50];        //KeyName
	PROPERTY_TYPE  m_emKeyType;            //key参数类型

	_Property()
	{
		m_szPropertyName[0] = '\0';
		m_szClassName[0]    = '\0';
		m_szDesc[0]         = '\0';
		m_szKeyName[0]      = '\0';
		m_emType            = PROPERTY_UNKNOW;
		m_emKeyType         = PROPERTY_UNKNOW;
		m_emClass           = CLASS_SINGLE;
		m_nLength           = 0;
		m_nNeedHeadLength   = 0;
	}
};

typedef vector<_Property> vecProperty;

//类信息
struct _Xml_Info
{
	char           m_szXMLName[60];
	char           m_szDesc[100];
	char           m_szMacroName[50];
	int            m_nCommandID;
	COMMAND_TYPE   m_emCommandType;        //命令类型 
	vecProperty    m_vecProperty;

	_Xml_Info()
	{
		m_nCommandID     = 0;
		m_szXMLName[0]   = '\0';
		m_szDesc[0]      = '\0';
		m_szMacroName[0] = '\0';
		m_emCommandType  = COMMAND_NONE;
	}
};
typedef vector<_Xml_Info> vecXmlInfo;

struct _Command_Relation_info
{
	char m_szCommandFuncName[100];
	int  m_nCommandInID;
	int  m_nCommandOutID;
	int  m_nOutPcket;        //0为走PacketParse 1为直接发送  

	_Command_Relation_info()
	{
		m_szCommandFuncName[0] = '\0';
		m_nCommandInID         = 0;
		m_nCommandOutID        = 0;
		m_nOutPcket            = 0;
	}
};
typedef vector<_Command_Relation_info> vecCommandRelationinfo;

struct _Project_Info
{
	char m_szProjectName[100];   //工程名称
	char m_szProjectDesc[200];   //工程描述
	char m_szProjectKey[100];    //工程key

	vecCommandRelationinfo m_objCommandList;    //命令关系

	_Project_Info()
	{
		m_szProjectName[0] = '\0';
		m_szProjectDesc[0] = '\0';
		m_szProjectKey[0]  = '\0';
	}
};

inline void sprintf_safe(char* szText, int nLen, const char* fmt ...)
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

class CXmlOpeation
{
public:
  CXmlOpeation(void);
  ~CXmlOpeation(void);

  bool Init(const char* pFileName);
  bool Init_String(const char* pXMLText);

  char* GetData(const char* pName, const char* pAttrName);
  char* GetData(const char* pName, const char* pAttrName, TiXmlElement*& pNextTiXmlElement);

  char* GetData_Text(const char* pName);
  char* GetData_Text(const char* pName, TiXmlElement*& pNextTiXmlElement);

  bool Parse_XML(char* pText, _Xml_Info& objxmlInfo);
  bool Parse_XML_File(const char* pFileName, vecXmlInfo& objvecXmlInfo);
  bool Parse_XML_File_Project(const char* pFileName, _Project_Info& objProjectInfo);

  void Close();

private:
  TiXmlDocument* m_pTiXmlDocument;
  TiXmlElement*  m_pRootElement;
};
#endif
