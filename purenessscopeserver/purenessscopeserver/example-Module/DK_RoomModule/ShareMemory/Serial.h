#ifndef _SERIAL_H
#define _SERIAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string>

//构建序列化对象的类
class CSerial
{
public:
	CSerial(void);
	~CSerial(void);

	void Close();
	bool Init(char* pData, int nCellSize, int nCount);

	char* GetData(int nIndex);

	char* GetBase() const;

public:
	char* m_pData;               //共享内存头部第一个地址的头指针地址
	int   m_nSize;               //当前共享内存头区域的大小
	int   m_nCellSize;           //当前头部数据块的大小
	int   m_nCount;              //当前头部数据块的个数

};
#endif
