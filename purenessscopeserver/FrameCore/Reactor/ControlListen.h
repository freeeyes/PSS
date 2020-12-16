#ifndef _CONTROLLISTEN_H
#define _CONTROLLISTEN_H

#include "ConnectAccept.h"
#include "AceReactorManager.h"
#include "IControlListen.h"

class CControlListen : public IControlListen
{
public:
    CControlListen() = default;
    ~CControlListen() final = default ;

    bool   AddListen(const char* pListenIP, uint16 u2Port, uint8 u1IPType, int nPacketParseID) final;  //打开一个新的监听端口
    bool   DelListen(const char* pListenIP, uint16 u2Port) final;                                      //关闭一个已知的连接
    uint32 GetListenCount() final;                                                                     //得到已经打开的监听个数
    bool   ShowListen(uint32 u4Index, _ControlInfo& objControlInfo) final;                             //根据索引查看已打开的监听端口
    uint32 GetServerID() final;                                                                        //得到服务器ID
private:
    vecControlInfo m_vecListenList;
};

using App_ControlListen = PSS_singleton<CControlListen>;

#endif
