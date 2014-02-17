#ifndef _SMACCESSOBJECT_H
#define _SMACCESSOBJECT_H

#include "ShareMemoryAPI.h"

class CSMAccessObject
{
public:
	CSMAccessObject(void);
	~CSMAccessObject(void);

	//创建对象，创建的内存为实际内存+头描述
	bool Create(SMKey key, int nSize, int nHeadSize);

	//检测对象是否存在，如果存在则不再创建，直接打开
	bool Open(SMKey key, int nSize, int nHeadSize);

	//删除对象
	void Destory();

	//得到指定长度偏移的指针位置
	char* GetData(int nSize);

	//得到头描述的位置
	char* GetHeadData();

	//得到此对象的总长度
	int GetDataSize();

	//得到此对象的头总长度
	int GetHeadDataSize();

private:
	SMHandle m_SMHandle;
	SMKey    m_Key;
	int      m_nSize;
	int      m_nHeadSize;
	char*    m_pHeadData;
	char*    m_pData;
};
#endif
