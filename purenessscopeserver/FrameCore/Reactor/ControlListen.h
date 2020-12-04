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

    bool   AddListen(const char* pListenIP, uint16 u2Port, uint8 u1IPType, int nPacketParseID) final;  //��һ���µļ����˿�
    bool   DelListen(const char* pListenIP, uint16 u2Port) final;                                      //�ر�һ����֪������
    uint32 GetListenCount() final;                                                                     //�õ��Ѿ��򿪵ļ�������
    bool   ShowListen(uint32 u4Index, _ControlInfo& objControlInfo) final;                             //���������鿴�Ѵ򿪵ļ����˿�
    uint32 GetServerID() final;                                                                        //�õ�������ID
private:
    vecControlInfo m_vecListenList;
};

using App_ControlListen = ACE_Singleton<CControlListen, ACE_Null_Mutex>;

#endif
