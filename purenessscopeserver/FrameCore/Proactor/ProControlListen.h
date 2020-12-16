#ifndef _PROCONTROLLISTEN_H
#define _PROCONTROLLISTEN_H

#include "ProConnectAccept.h"
#include "AceProactorManager.h"
#include "IControlListen.h"

//连接管理类，用于创建删除和查看监听
//add by freeeyes
class CProControlListen : public IControlListen
{
public:
    CProControlListen();
    virtual ~CProControlListen();

    bool   AddListen(const char* pListenIP, uint16 u2Port, uint8 u1IPType, int nPacketParseID);  //打开一个新的监听端口
    bool   DelListen(const char* pListenIP, uint16 u2Port);                                      //关闭一个已知的连接
    uint32 GetListenCount();                                                                     //得到已经打开的监听个数
    bool   ShowListen(uint32 u4Index, _ControlInfo& objControlInfo);                             //根据索引查看已打开的监听端口
    uint32 GetServerID();                                                                        //得到服务器ID
private:
    vecControlInfo m_vecListenList;
};

using App_ProControlListen = PSS_singleton<CProControlListen> ;

#endif
