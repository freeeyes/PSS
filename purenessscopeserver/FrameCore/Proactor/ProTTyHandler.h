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

    bool ConnectTTy();                          //连接指定的设备

    virtual void Close(uint32 u4ConnectID);
    virtual bool SendMessage(const CSendMessageInfo& objSendMessageInfo, uint32& u4PacketSize);
    virtual bool PutSendPacket(uint32 u4ConnectID, ACE_Message_Block* pMbData, uint32 u4Size, const PSS_Time_Point& tvSend);
    virtual void SetIsLog(bool blIsLog);

    bool Init(uint32 u4ConnectID,
              const char* pName,
              ACE_TTY_IO::Serial_Params inParams,
              ITTyMessage* pTTyMessage,
              EM_CONNECT_IO_DISPOSE emDispose = EM_CONNECT_IO_DISPOSE::CONNECT_IO_PLUGIN,
              uint32 u4PacketParseInfoID = 0);
    bool GetConnectState();
    ACE_TTY_IO::Serial_Params GetParams();
    void SetPause(bool blPause);                //设置暂停状态
    bool GetPause();                            //得到暂停状态

    //重写继承方法
    virtual  void handle_read_file(const ACE_Asynch_Read_File::Result& result);
    virtual  void handle_write_file(const ACE_Asynch_Write_File::Result& result);

    bool Send_Data(const char* pData, ssize_t nLen);                         //向设备发送数据

    virtual bool Device_Send_Data(const char* pData, ssize_t nLen);          //透传数据接口

    uint32 GetConnectID();                                                   //得到ConnectID

private:
    void Ready_To_Read_Buff();
    void Send_Hander_Event(uint8 u1Option);                                  //发送Handler的事件通知业务线程

private:

    char                                  m_szName[MAX_BUFF_100];
    ACE_TTY_IO                            m_Ttyio;
    ACE_DEV_Connector                     m_Connector;
    ACE_Asynch_Read_File                  m_ObjReadRequire;
    ACE_Asynch_Write_File                 m_ObjWriteRequire;
    ACE_Message_Block*                    m_pmbReadBuff   = nullptr;
    ACE_TTY_IO::Serial_Params             m_ObjParams;                 //设备接口参数
    bool                                  m_blState       = false;     //当前设备连接状态
    bool                                  m_blPause       = true;      //是否暂停
    uint32                                m_u4ConnectID   = 0;         //当前设备ID
    ITTyMessage*                          m_pTTyMessage   = nullptr;   //TTyMessage对象
    EM_CONNECT_IO_DISPOSE                 m_emDispose     = EM_CONNECT_IO_DISPOSE::CONNECT_IO_FRAME; //处理模式，框架处理 or 业务处理
    uint32                                m_u4PacketParseInfoID = 0;   //框架处理模块ID
    string                                m_strDeviceName;             //转发接口名称
    shared_ptr<_Packet_Parse_Info>        m_pPacketParse   = nullptr;  //数据包Packetparse函数接口
    CMakePacket                           m_MakePacket;
};
#endif

