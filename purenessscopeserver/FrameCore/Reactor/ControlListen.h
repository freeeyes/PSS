#ifndef _CONTROLLISTEN_H
#define _CONTROLLISTEN_H

#include "ConnectAccept.h"
#include "AceReactorManager.h"
#include "IControlListen.h"

class CControlListen : public IControlListen
{
public:
    CControlListen();
    virtual ~CControlListen();

    bool   AddListen(const char* pListenIP, uint16 u2Port, uint8 u1IPType, int nPacketParseID);  //打开一个新的监听端口
    bool   DelListen(const char* pListenIP, uint16 u2Port);                                      //关闭一个已知的连接
    uint32 GetListenCount();                                                                     //得到已经打开的监听个数
    bool   ShowListen(uint32 u4Index, _ControlInfo& objControlInfo);                             //根据索引查看已打开的监听端口
    uint32 GetServerID();                                                                        //得到服务器ID
private:
    vecControlInfo m_vecListenList;
};

typedef ACE_Singleton<CControlListen, ACE_Null_Mutex> App_ControlListen;

#endif
