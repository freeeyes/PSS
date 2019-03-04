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

class CProTTyHandler : public ACE_Handler
{
public:
    CProTTyHandler();
    virtual ~CProTTyHandler();

    bool ConnectTTy();                          //连接指定的设备

    void Close();
    bool Init(uint32 u4ConnectID, const char* pName, ACE_TTY_IO::Serial_Params inParams, ITTyMessage* pTTyMessage);
    bool GetConnectState();
    ACE_TTY_IO::Serial_Params GetParams();
    void SetPause(bool blPause);                //设置暂停状态
    bool GetPause();                            //得到暂停状态

    //重写继承方法
    virtual  void handle_read_file(const ACE_Asynch_Read_File::Result& result);
    virtual  void handle_write_file(const ACE_Asynch_Write_File::Result& result);

    bool Send_Data(const char* pData, ssize_t nLen);                         //向设备发送数据

private:
    void Ready_To_Read_Buff();

private:

    char                                  m_szName[MAX_BUFF_100];
    ACE_TTY_IO                            m_Ttyio;
    ACE_DEV_Connector                     m_Connector;
    ACE_Asynch_Read_File                  m_ObjReadRequire;
    ACE_Asynch_Write_File                 m_ObjWriteRequire;
    ACE_Message_Block*                    m_pmbReadBuff;
    ACE_TTY_IO::Serial_Params             m_ObjParams;                 //设备接口参数
    bool                                  m_blState;                   //当前设备连接状态
    bool                                  m_blPause;                   //是否暂停
    uint32                                m_u4ConnectID;               //当前设备ID
    ITTyMessage*                          m_pTTyMessage;               //TTyMessage对象
};
#endif

