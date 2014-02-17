#ifndef _CLIENTUDPMESSAGE_H
#define _CLIENTUDPMESSAGE_H

//处理UDP客户端数据接收，服务器间通讯
//UDP不是面向链接的，也不必处理数据包的具体切割，所以这里完全可以把收到的数据交给用户自己处理。
//add by freeeyes


class IClientUDPMessage
{
public:
	virtual bool RecvData(const char* pData, int nLen) = 0;    //接收数据的函数
};

#endif
