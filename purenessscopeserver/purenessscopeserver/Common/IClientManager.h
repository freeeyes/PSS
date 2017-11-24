#ifndef _ICLIENTMANAGER_H
#define _ICLIENTMANAGER_H

#include "ClientMessage.h"
#include "ClientUDPMassage.h"

//负责管理服务器间通讯的数据管理
class IClientManager
{
public:
    virtual ~IClientManager() {}

    //设置TCP链接参数，pClientMessage为远端数据到达处理类。
    virtual bool Connect(int nServerID, const char* pIP, int nPort, uint8 u1IPType, IClientMessage* pClientMessage)                  = 0;
    //设置TCP链接参数，pClientMessage为远端数据到达处理类，可以设置本地IP和端口。
    virtual bool Connect(int nServerID, const char* pIP, int nPort, uint8 u1IPType, const char* pLocalIP, int nLocalPort, uint8 u1LocalIPType, IClientMessage* pClientMessage) = 0;
    //设置UDP链接参数，pClientUDPMessage为远端数据到达处理类。
    virtual bool ConnectUDP(int nServerID, const char* pIP, int nPort, uint8 u1IPType, EM_UDP_TYPE emType, IClientUDPMessage* pClientUDPMessage) = 0;
    //关闭某一个ServerID对应的TCP链接
    virtual bool Close(int nServerID)                                                                                                 = 0;
    //关闭某一个ServerID对应的UDP链接
    virtual bool CloseUDP(int nServerID)                                                                                              = 0;
    //发送一个TCP的数据包，发送完数据blIsDelete来决定是否由框架回收，还是逻辑回收，不能使用CBuffPacket，因为是内存池，所以这里不能删除
    virtual bool SendData(int nServerID, char*& pData, int nSize, bool blIsDelete = true)                                             = 0;
    //发送一个UDP的数据包，发送完数据blIsDelete来决定是否由框架回收，还是逻辑回收，不能使用CBuffPacket，因为是内存池，所以这里不能删除
    virtual bool SendDataUDP(int nServerID, const char* pIP, int nPort, const char*& pMessage, uint32 u4Len, bool blIsDelete = true)  = 0;
    //链接存活检查，如果发现链接在不发送数据包的时候断开了，则会自动重建
    virtual bool StartConnectTask(int nIntervalTime)                                                                                  = 0;
    //关闭连接存活检查
    virtual void CancelConnectTask()                                                                                                  = 0;
    //关闭所有对外链接包括TCP和UDP
    virtual void Close()                                                                                                              = 0;
    //得到一个连接的当前状态
    virtual EM_Server_Connect_State GetConnectState(int nServerID)                                                                    = 0;
    //获得一个ServerID对应的pClientMessage对象信息
    virtual IClientMessage* GetClientMessage(int nServerID)                                                                           = 0;
    //获得一个ServerID对应的IP信息
    virtual bool GetServerIPInfo(int nServerID, _ClientIPInfo& objServerIPInfo)                                                       = 0;
    //删除一个正在使用的IClientMessage
    virtual bool DeleteIClientMessage(IClientMessage* pClientMessage)                                                                 = 0;
};

#endif
