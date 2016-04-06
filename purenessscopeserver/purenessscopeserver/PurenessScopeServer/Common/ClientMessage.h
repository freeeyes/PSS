#ifndef _ICLIENTMESSAGE_H
#define _ICLIENTMESSAGE_H

//这是一个接口类，负责将数据发送出去，并获得数据返回，提供链接错误的类
//方法包括RecvMessage()接收数据和ConnectError()发送错误三种方法。
//add by freeeyes 2010-12-31

#include "IMessageBlockManager.h"

class IClientMessage
{
public:
	virtual ~IClientMessage() {};

	virtual bool Send_Format_data(char* pData, uint32 u4Len, IMessageBlockManager* pMessageBlockManager, ACE_Message_Block*& mbSend)                                 = 0;    //处理数据包发送接口
	virtual bool Recv_Format_data(ACE_Message_Block* mbRecv, IMessageBlockManager* pMessageBlockManager, uint16& u2CommandID, ACE_Message_Block*& mbFinishRecv)  = 0;    //处理数据包接收接口

	virtual void ReConnect(int nServerID)                                                               = 0;    //连接重连成功接口
	virtual bool RecvData(uint16 u2CommandID, ACE_Message_Block* mbRecv, _ClientIPInfo objServerIPInfo) = 0;    //接收数据的函数
	virtual bool ConnectError(int nError, _ClientIPInfo objServerIPInfo)                                = 0;    //当出错的时候，调用此接口返回错误信息  
};

#endif
