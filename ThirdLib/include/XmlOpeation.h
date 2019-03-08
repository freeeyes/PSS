#ifndef _XMLOPRATION_H
#define _XMLOPRATION_H

#include "tinyxml.h"
#include "tinystr.h"

#include "Common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <vector>
#include <ctype.h>

using namespace std;

//��XMLת��ΪMAP�ṹ
//add by freeeyes

//֧������
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

//�õ�Stream����
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

//Ԥ����ṹ��
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


//������Ϣ
struct _Property
{
	char           m_szPropertyName[50];   //��������
	char           m_szClassName[50];      //��������
	char           m_szDesc[100];          //������Ϣ
	char           m_szStreamLength[10];   //�����ݳ���
	PROPERTY_TYPE  m_emType;               //��������
	PROPERTY_CLASS m_emClass;              //�������
	int            m_nLength;              //����
	int            m_nNeedHeadLength;      //�Ƿ���Ҫ�Զ�׷�����ݳ��� 0׷�ӣ�1��׷��
	char           m_szKeyName[50];        //KeyName
	PROPERTY_TYPE  m_emKeyType;            //key��������

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

		::sprintf_safe(m_szStreamLength, 10, "0");
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

//����Ϣ
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

struct _Command_Info_Func
{
	char m_szCommandFuncName[100];
	char m_szCommandInID[50];
	char m_szCommandOutID[50];

	vecObjectInfo m_vecObjectInfo;

	_Command_Info_Func()
	{
		m_szCommandFuncName[0] = '\0';
		m_szCommandInID[0]     = '\0';
		m_szCommandOutID[0]    = '\0';
	}
};
typedef vector<_Command_Info_Func> vecCommandRelationinfo;

struct _Project_Info
{
	char m_szProjectName[100];   //��������
	char m_szProjectDesc[200];   //��������
	char m_szProjectKey[100];    //����key

	vecCommandRelationinfo m_objCommandList;     //�����ϵ
	vecDefineInfo          m_objDefineInfoList;  //Ԥ������Ϣ

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

  bool Parse_XML_Test_Assemble(const char* pFileName, _Test_Assemble& obj_Test_Assemble);

  bool Parse_Class_File(const char* pFileName, vecClassInfo& objvecClassInfo);
  bool Parse_Plug_In_Project(const char* pFileName, _Project_Info& objProjectInfo);

  void Close();

private:
  TiXmlDocument* m_pTiXmlDocument;
  TiXmlElement*  m_pRootElement;
};
#endif
