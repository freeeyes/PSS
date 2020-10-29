#pragma once

#include "IBuffPacket.h"
#include "ClientCommand.h"
#include "IObject.h"
#include "Trace.h"

#include <string>

//����ͻ�������(TCP)
//Ĭ������ID������CLIENT_LINK_USER�Ժ�����֣�֮ǰ������ID�Ǳ�����
#define COMMAND_BASE            0x1000
#define COMMAND_RETURN_ALIVE    0xf000
#define NET_VERSION				0x6800			//�汾
#define COMMAND_HEARTBEAT		0x2100			//����
#define COMMAND_TESTREPLY		0x2101			//���Իظ�

#define SIGNALING_KEY									"R3CadminLUBOcomPuterWybNUsdkIPS"
#define SIGNALING_IV									"ADMINLUBOCOMr3c"

#define MESSAGE_FUNCTION_BEGIN(x) switch(x) {
#define MESSAGE_FUNCTION(x,y,z,h) case x: { y(z,h); break; }
#define MESSAGE_FUNCTION_END }

using namespace std;

class CBaseCommand : public CClientCommand
{
public:
    CBaseCommand(void);
    ~CBaseCommand(void);

    virtual int DoMessage(IMessage* pMessage, bool& bDeleteFlag, IBuffPacket* pSendBuffPacket);
    void SetServerObject(CServerObject* pServerObject);

    void ReadIniFile(const char* pIniFileName);

private:
    int Do_Connect(IMessage* pMessage, IBuffPacket* pSendBuffPacket);
    int Do_DisConnect(IMessage* pMessage, IBuffPacket* pSendBuffPacket);
    int Do_ClientSendTimeout(IMessage* pMessage, IBuffPacket* pSendBuffPacket);
    int Do_Base(IMessage* pMessage, IBuffPacket* pSendBuffPacket);
    int Do_ClientSendOk(IMessage* pMessage, IBuffPacket* pSendBuffPacket);
    int Do_ReplyTest(IMessage* pMessage, IBuffPacket* pSendBuffPacket);
    int SendClient(_PacketInfo BodyPacket, short nCommand, uint32 nConnectId, char* pKey, char* pIv, bool nEncrypt, IBuffPacket* pSendBuffPacket);

private:
    CServerObject* m_pServerObject;
    int            m_nCount;
};
