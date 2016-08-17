#ifndef _XMLOPRATION_H
#define _XMLOPRATION_H

#include "tinyxml.h"
#include "tinystr.h"

#include "Common.h"

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

enum STREAM_TYPE
{
	STREAM_TYPE_STRING = 0,
	STREAM_TYPE_CHAR,
	STREAM_TYPE_ALL,
};

enum PACKET_TYPE
{
	PACKET_TYPE_HEAD = 0,
	PACKET_TYPE_BODY,
	PACKET_TYPE_RETURN,
};

//得到Stream类型
struct _Stream_Type_Info
{
	STREAM_TYPE m_emType;
	int m_nLength;

	_Stream_Type_Info()
	{
		m_emType  = STREAM_TYPE_STRING;
		m_nLength = 0;
	}
};

//预定义结构体
struct _Define_Info
{
	char m_szDefineName[100];
	char m_szType[20];
	char m_szDefineValue[100];

	_Define_Info()
	{
		m_szDefineName[0]  = '\0';
		m_szType[0]        = '\0';
		m_szDefineValue[0] = '\0';
	}
};
typedef vector<_Define_Info> vecDefineInfo;


//属性信息
struct _Property
{
	char           m_szPropertyName[50];   //参数名称
	char           m_szClassName[50];      //对象类名
	char           m_szDesc[100];          //描述信息
	char           m_szStreamLength[10];   //流数据长度 
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
		
		sprintf_safe(m_szStreamLength, 10, "0");
	}

	_Stream_Type_Info Get_Stream_Info()
	{
		_Stream_Type_Info obj_Stream_Type_Info;
		if(strcmp(m_szStreamLength, "all") == 0)
		{
			obj_Stream_Type_Info.m_emType  = STREAM_TYPE_ALL;
			obj_Stream_Type_Info.m_nLength = 0;
			return obj_Stream_Type_Info;
		}
		else
		{
			int n_Stream_Length = atoi(m_szStreamLength);
			if(0 == n_Stream_Length)
			{
				obj_Stream_Type_Info.m_emType  = STREAM_TYPE_STRING;
				obj_Stream_Type_Info.m_nLength = 0;
				return obj_Stream_Type_Info;
			}
			else
			{
				obj_Stream_Type_Info.m_emType  = STREAM_TYPE_CHAR;
				obj_Stream_Type_Info.m_nLength = n_Stream_Length;
				return obj_Stream_Type_Info;
			}
		}
	}
};
typedef vector<_Property> vecProperty;

//类信息
struct _Class_Info
{
	char           m_szXMLName[60];
	char           m_szDesc[100];
	vecProperty    m_vecProperty;

	_Class_Info()
	{
		m_szXMLName[0]   = '\0';
		m_szDesc[0]      = '\0';
	}
};
typedef vector<_Class_Info> vecClassInfo;

struct _Object_Info
{
	char m_szClassName[50];
	PACKET_TYPE m_emPacketType;

	_Object_Info()
	{
		m_szClassName[0] = '\0';
		m_emPacketType   = PACKET_TYPE_HEAD;
	}
};
typedef vector<_Object_Info> vecObjectInfo;

struct _Command_Info
{
	char m_szCommandFuncName[100];
	char m_szCommandInID[50];
	char m_szCommandOutID[50];

	vecObjectInfo m_vecObjectInfo;

	_Command_Info()
	{
		m_szCommandFuncName[0] = '\0';
		m_szCommandInID[0]     = '\0';
		m_szCommandOutID[0]    = '\0';
	}
};
typedef vector<_Command_Info> vecCommandRelationinfo;

struct _Project_Info
{
	char m_szProjectName[100];   //工程名称
	char m_szProjectDesc[200];   //工程描述
	char m_szProjectKey[100];    //工程key

	vecCommandRelationinfo m_objCommandList;     //命令关系
	vecDefineInfo          m_objDefineInfoList;  //预定义信息 

	_Project_Info()
	{
		m_szProjectName[0] = '\0';
		m_szProjectDesc[0] = '\0';
		m_szProjectKey[0]  = '\0';
	}
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

  bool Parse_Class_File(const char* pFileName, vecClassInfo& objvecClassInfo);
  bool Parse_Plug_In_Project(const char* pFileName, _Project_Info& objProjectInfo);

  void Close();

private:
  TiXmlDocument* m_pTiXmlDocument;
  TiXmlElement*  m_pRootElement;
};
#endif
