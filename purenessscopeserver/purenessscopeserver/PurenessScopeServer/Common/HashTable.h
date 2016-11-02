#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdio.h>
#include <ctype.h>
#include <string.h>

enum EM_HASH_STATE
{
	EM_SELECT = 0,
	EM_INSERT,
};

//hash表结构
struct _Hash_Table_Cell 
{
	char  m_cExists;                       //当前块是否已经使用,1已经使用，0没有被使用
	char* m_szKey;                         //当前的key值，没有则为空
	int   m_nKeySize;                      //当前key数据长度
	int   m_nNextKeyIndex;                 //链表信息，如果主键有冲突,记录下一个冲突主键的位置
	int   m_nProvKeyIndex;                 //链表信息，如果主键有冲突,记录上一个冲突主键的位置
	unsigned long m_uHashA;                //第二次的hashkey值
	unsigned long m_uHashB;                //第三次的hashkey值 
	char* m_szValue;                       //当前数据体指针
	int   m_nValueSize;                    //当前数据体长度
	
	_Hash_Table_Cell()
	{
		Init();
	}
	
	void Init()
	{
		m_cExists       = 0;
		m_nKeySize      = 0;
		m_nValueSize    = 0;
		m_uHashA        = 0;
		m_uHashB        = 0;
		m_nNextKeyIndex = -1;
		m_nProvKeyIndex = -1;
		m_szKey         = NULL;
		m_szValue       = NULL;		
	}
	
	void Set_Key(char* pKey, int nKeySize)
	{
		m_szKey         = pKey;
		m_nKeySize      = nKeySize;
	}
	
	void Set_Value(char* pValue, int nValueSize)
	{
		m_szValue       = pValue;
		m_nValueSize    = nValueSize;
	}
	
	void Clear()
	{
		m_cExists       = 0;
		m_uHashA        = 0;
		m_uHashB        = 0;
		m_nNextKeyIndex = -1;
		m_nProvKeyIndex = -1;		
		if(NULL != m_szKey)	
		{
			memset(m_szKey, 0, m_nKeySize);		
		}
		if(NULL != m_szValue)
		{
			memset(m_szValue, 0, m_nValueSize);
		}
	}  
};

//hashTable类
class CHashTable
{
public:
	CHashTable();
	~CHashTable();
	
	//设置一个已知的内存数组块(必须初始化调用)
	void Set_Base_Addr(char* pData, int nCount);
	//设置一个Hash Key的数组块(必须初始化调用)
	void Set_Base_Key_Addr(char* pData, int nSize, int nKeySize);
	//设置一组Hash Value的地址块(必须初始化调用)
	void Set_Base_Value_Addr(char* pData, int nSize, int nValueSize);
	
	//得到当前缓冲块总个数
	int Get_Count();
	//得到整体数据内存大小
	size_t Get_Size();
	
	//添加一个Hash数据块
	int Add_Hash_Data(const char* pKey, const char* pValue);
	//获得一个已有映射对应数值
	char* Get_Hash_Box_Data(const char* pKey);
	//清理一个hash数据块
	int Del_Hash_Data(const char* pKey);
	
private:
	void prepareCryptTable();
	unsigned long HashString(const char* pKey, unsigned long dwHashType);
	int GetLastClashKey(int nStartIndex, const char *lpszString, unsigned long uHashA, unsigned long uHashB, EM_HASH_STATE emHashState);
	int GetHashTablePos(const char *lpszString, EM_HASH_STATE emHashState);	
	int DelHashTablePos(const char *lpszString);
	
private:
	_Hash_Table_Cell* m_lpTable;	
	char              m_cryptTable[1280];
	int               m_nCount;
	int               m_nKeySize;
	int               m_nValueSize;
};


#endif
