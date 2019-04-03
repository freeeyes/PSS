#ifndef ZK_CLIENT_H
#define ZK_CLIENT_H

#include <errno.h>
#include <string.h>
#include <zookeeper.h>

class CZKClient
{
public:
	CZKClient();
	~CZKClient();
	
	bool Init(const char* pConnectInfo);
	
	bool Create_Node(const char* pPath, const char* pData, int nLen);
	
	bool Get_Node(const char* pPath, char* pData, int& nLen);
	
	bool Set_Node(const char* pPath, char* pData, int& nLen);
	
	int Get_Node_Count(const char* pPath);
	
	void Close();
	
private:
	zhandle_t* m_pZh;
	//clientid_t m_myid;
};

#endif
