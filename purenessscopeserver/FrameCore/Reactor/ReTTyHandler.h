#ifndef _RETTYHANDLER_H
#define _RETTYHANDLER_H

//TTy设备接入的reactor实现
//add by freeeyes

#include "ace/Event_Handler.h"
#include "ace/Message_Queue.h"
#include "ace/Synch.h"
#include "ace/Reactor.h"
#include "ace/TTY_IO.h"
#include "ace/DEV_Connector.h"
#include "ITTyMessage.h"
#include "define.h"
#include "LoadPacketParse.h"
#include "MessageBlockManager.h"
#include "BaseHander.h"
#include "TcpRedirection.h"
#include "IHandler.h"

class CReTTyHandler : public ACE_Event_Handler, public IHandler
{
public:
    CReTTyHandler();
    ~CReTTyHandler() final;

    bool ConnectTTy();                          //连接指定的设备

    void Close(uint32 u4ConnectID) final;
    bool SendMessage(const CSendMessageInfo& objSendMessageInfo, uint32& u4PacketSize) final;
    bool PutSendPacket(uint32 u4ConnectID, ACE_Message_Block* pMbData, uint32 u4Size, const PSS_Time_Point& tvSend) final;
    void SetIsLog(bool blIsLog) final;

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

    int handle_input(ACE_HANDLE handle);
    int handle_signal(int signum, siginfo_t* psinfo, ucontext_t* context);
    int handle_close(ACE_HANDLE handle, ACE_Reactor_Mask close_mask);
    int handle_timeout(const ACE_Time_Value& tvNow, const void*  p);

    bool Send_Data(const char* pData, ssize_t nLen);                         //向设备发送数据
    bool Device_Send_Data(const char* pData, ssize_t nLen) final;            //透传数据接口
    uint32 GetConnectID();

private:
    void Send_Hander_Event(uint8 u1Option);                                  //发送Handler的事件通知业务线程

private:
    char                                  m_szName[MAX_BUFF_100] = {'\0'};
    ACE_TTY_IO                            m_ReTtyio;
    ACE_DEV_Connector                     m_ReConnector;
    ACE_Message_Block*                    m_pmbReadBuff          = nullptr;
    ACE_TTY_IO::Serial_Params             m_ObjParams;                                //设备接口参数
    bool                                  m_blState              = false;             //当前设备连接状态
    bool                                  m_blPause              = false;             //是否暂停
    uint32                                m_u4ConnectID          = 0;                 //当前设备ID
    ITTyMessage*                          m_pTTyMessage          = nullptr;              //TTyMessage对象
    EM_CONNECT_IO_DISPOSE                 m_emDispose            = EM_CONNECT_IO_DISPOSE::CONNECT_IO_PLUGIN; //处理模式，框架处理 or 业务处理
    uint32                                m_u4PacketParseInfoID  = 0;                 //框架处理模块ID
    string                                m_strDeviceName;                            //转发接口名称
    CMakePacket                           m_MakePacket;                               //工作线程消息生成器 
    shared_ptr<_Packet_Parse_Info>        m_pPacketParse         = nullptr;           //数据包Packetparse函数接口
};

#endif
