#ifndef _PROCONTROLLISTEN_H
#define _PROCONTROLLISTEN_H

#include "ProConnectAccept.h"
#include "AceProactorManager.h"
#include "IControlListen.h"

//���ӹ����࣬���ڴ���ɾ���Ͳ鿴����
//add by freeeyes
class CProControlListen : public IControlListen
{
public:
    CProControlListen();
    virtual ~CProControlListen();

    bool   AddListen(const char* pListenIP, uint16 u2Port, uint8 u1IPType, int nPacketParseID);  //��һ���µļ����˿�
    bool   DelListen(const char* pListenIP, uint16 u2Port);                                      //�ر�һ����֪������
    uint32 GetListenCount();                                                                     //�õ��Ѿ��򿪵ļ�������
    bool   ShowListen(uint32 u4Index, _ControlInfo& objControlInfo);                             //���������鿴�Ѵ򿪵ļ����˿�
    uint32 GetServerID();                                                                        //�õ�������ID
private:
    vecControlInfo m_vecListenList;
};

using App_ProControlListen = PSS_singleton<CProControlListen> ;

#endif
