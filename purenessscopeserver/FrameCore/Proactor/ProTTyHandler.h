#ifndef TTYHANDLER_H
#define TTYHANDLER_H

#include "define.h"
#include  <ace/Asynch_IO.h>
#include "ace/Message_Queue.h"
#include "ace/Synch.h"
#include "ace/Proactor.h"
#include "ace/TTY_IO.h"
#include "ace/DEV_Connector.h"
#include "ITTyMessage.h"
#include "LoadPacketParse.h"
#include "MessageBlockManager.h"
#include "BaseHander.h"
#include "TcpRedirection.h"
#include "IHandler.h"

class CProTTyHandler : public ACE_Handler, public IHandler
{
public:
    CProTTyHandler();
    virtual ~CProTTyHandler();

    bool ConnectTTy();                          //����ָ�����豸

    virtual void Close(uint32 u4ConnectID);
    virtual bool SendMessage(const CSendMessageInfo& objSendMessageInfo, uint32& u4PacketSize);
    virtual bool PutSendPacket(uint32 u4ConnectID, ACE_Message_Block* pMbData, uint32 u4Size, const ACE_Time_Value& tvSend);
    virtual void SetIsLog(bool blIsLog);

    bool Init(uint32 u4ConnectID,
              const char* pName,
              ACE_TTY_IO::Serial_Params inParams,
              ITTyMessage* pTTyMessage,
              EM_CONNECT_IO_DISPOSE emDispose = EM_CONNECT_IO_DISPOSE::CONNECT_IO_PLUGIN,
              uint32 u4PacketParseInfoID = 0);
    bool GetConnectState();
    ACE_TTY_IO::Serial_Params GetParams();
    void SetPause(bool blPause);                //������ͣ״̬
    bool GetPause();                            //�õ���ͣ״̬

    //��д�̳з���
    virtual  void handle_read_file(const ACE_Asynch_Read_File::Result& result);
    virtual  void handle_write_file(const ACE_Asynch_Write_File::Result& result);

    bool Send_Data(const char* pData, ssize_t nLen);                         //���豸��������

    virtual bool Device_Send_Data(const char* pData, ssize_t nLen);          //͸�����ݽӿ�

    uint32 GetConnectID();                                                   //�õ�ConnectID

private:
    void Ready_To_Read_Buff();
    void Send_Hander_Event(uint8 u1Option);                                  //����Handler���¼�֪ͨҵ���߳�

private:

    char                                  m_szName[MAX_BUFF_100];
    ACE_TTY_IO                            m_Ttyio;
    ACE_DEV_Connector                     m_Connector;
    ACE_Asynch_Read_File                  m_ObjReadRequire;
    ACE_Asynch_Write_File                 m_ObjWriteRequire;
    ACE_Message_Block*                    m_pmbReadBuff   = nullptr;
    ACE_TTY_IO::Serial_Params             m_ObjParams;                 //�豸�ӿڲ���
    bool                                  m_blState       = false;     //��ǰ�豸����״̬
    bool                                  m_blPause       = true;      //�Ƿ���ͣ
    uint32                                m_u4ConnectID   = 0;         //��ǰ�豸ID
    ITTyMessage*                          m_pTTyMessage   = nullptr;   //TTyMessage����
    EM_CONNECT_IO_DISPOSE                 m_emDispose     = EM_CONNECT_IO_DISPOSE::CONNECT_IO_FRAME; //����ģʽ����ܴ��� or ҵ����
    uint32                                m_u4PacketParseInfoID = 0;   //��ܴ���ģ��ID
    string                                m_strDeviceName;             //ת���ӿ�����
    shared_ptr<_Packet_Parse_Info>        m_pPacketParse   = nullptr;  //���ݰ�Packetparse�����ӿ�
    CMakePacket                           m_MakePacket;
};
#endif

