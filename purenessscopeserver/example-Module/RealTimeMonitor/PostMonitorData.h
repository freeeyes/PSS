#ifndef _POSTMONITORDATA_H
#define _POSTMONITORDATA_H

#include "IObject.h"
#include "IMonitorCommand.h"

class CPostServerData : public IClientMessage
{
public:
    CPostServerData();
    ~CPostServerData();

    void Init(IMonitorCommand* pMonitorCommand);

    bool Need_Send_Format();
    bool Send_Format_data(char* pData, uint32 u4Len, IMessageBlockManager* pMessageBlockManager, ACE_Message_Block*& mbSend);
    bool Recv_Format_data(ACE_Message_Block* mbRecv, IMessageBlockManager* pMessageBlockManager, uint16& u2CommandID, ACE_Message_Block*& mbFinishRecv, EM_PACKET_ROUTE& emPacketRoute);

    void ReConnect(int nServerID);
    bool RecvData(uint16 u2CommandID, ACE_Message_Block* mbRecv, _ClientIPInfo objServerIPInfo);
    bool ConnectError(int nError, _ClientIPInfo objServerIPInfo);
private:
    IMonitorCommand* m_pMonitorCommand;
};

#endif

