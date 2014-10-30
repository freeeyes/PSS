#ifndef _ICLIENTMESSAGE_H
#define _ICLIENTMESSAGE_H

//这是一个接口类，负责将数据发送出去，并获得数据返回，提供链接错误的类
//方法包括RecvMessage()接收数据和ConnectError()发送错误三种方法。
//add by freeeyes 2010-12-31

#include "define.h"
#include "ace/Message_Block.h"

class IClientMessage
{
public:
	virtual ~IClientMessage() {};
	
	virtual void ReConnect(int nServerID)                                            = 0;    //连接重连成功接口
	virtual bool RecvData(ACE_Message_Block* mbRecv, _ClientIPInfo objServerIPInfo)  = 0;    //接收数据的函数
	virtual bool ConnectError(int nError, _ClientIPInfo objServerIPInfo)             = 0;    //当出错的时候，调用此接口返回错误信息  
};

#endif
