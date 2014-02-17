#ifndef _IODATA_H
#define _IODATA_H

//这个类负责处理共享内存数据和IO介质之间的交换

#include "PlayerObject.h"

//用户数据存放路径，这里是一个总目录，在这里，需要一个存储策略的方法，提供文件存储。
#define DB_FILE_PATH  "D:\\IOData"

#define MAX_FILENAME_BUFF  200
#define MAX_FILEBUFF_500   500

class CIOData
{
public:
	CIOData(void);
	~CIOData(void);

	bool DeletePlayer(const char* pPlayerNick);          //删除一个用户数据文件
	bool SavePlayer(CPlayerData* pPlayerData);           //保存创建用户的数据
	CPlayerData* GetPlayer(const char* pPlayerNick);     //这里在IO里面查找，找到了就new一个CPlayerData对象出来，返回给上层，由上层用完负责删除

private:
	bool GetFileName(const char* pPlayerNick, char* pFileName);   //文件名保存策略
	CPlayerData* ParseFile(char* pBuff);                          //分析文件中的数据，并填充CPlayerData结构
	bool GetData(char*& pBin, const char* pTagBegin, const char* pTagEnd, char* pValue);
};
#endif
