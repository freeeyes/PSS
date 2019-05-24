#ifndef _ITTYCLIENTMANAGER_H
#define _ITTYCLIENTMANAGER_H

#include "ITTyMessage.h"

//提供连接串口，红外和蓝牙设备的对应手段和方法。
//add by Tangkai & freeyes

struct _TTyDevParam // 尽量与ACE_TTY_IO::Serial_Params保持一致方便赋值
{
    //常用参数
    char          m_szDevName[32];         //设备名（串口号,蓝牙,红外）
    int           m_nBaudRate;              //波特率
    unsigned char m_uDatabits;              //数据位：5，6，7，8
    unsigned char m_uStopbits;              //停止位：1，2
    const char*   m_pParitymode;            //校验位：none,even,odd,mark,space
    bool          m_blCTSenb;               //开启CTS
    unsigned char m_uRTSenb;                //RTS参数:0,1,2,3
    bool          m_blDSRenb;               //开启DSR
    bool          m_blDTRdisable;           //关闭DTR
    int           m_nReadtimeoutmsec;       //读取超时

    // 非常用参数
    bool          m_blXinenb;
    bool          m_blXoutenb;
    bool          m_blModem;
    bool          m_blRcvenb;
    int           m_nXOnlim;                // XON
    int           m_nXOfflim;               // XOFF
    unsigned int  m_u4Readmincharacters;

    _TTyDevParam()
    {
        m_szDevName[0]        = '\0';
        m_nBaudRate           = 19200;
        m_uDatabits           = 8;
        m_uStopbits           = 1;
        m_pParitymode         = "NONE";
        m_blCTSenb            = 0;
        m_uRTSenb             = 0;
        m_blDSRenb            = 0;
        m_blDTRdisable        = 0;
        m_nReadtimeoutmsec    = 0;
        m_blXinenb            = 0;
        m_blXoutenb           = 0;
        m_blModem             = 0;
        m_blRcvenb            = 1;
        m_nXOnlim             = 0;
        m_nXOfflim            = 0;
        m_u4Readmincharacters = 0;
    }
};

//文件描述符管理类
class ITTyClientManager
{
public:
    virtual ~ITTyClientManager() {}
    virtual int Connect(uint16 u2ConnectID, const char* pName, _TTyDevParam& inParam, ITTyMessage* pMessageRecv) = 0;           // 连接（打开）端口
    virtual int ConnectFrame(uint16 u2ConnectID, const char* pName, _TTyDevParam& inParam, uint32 u4PacketParseID) = 0;         //连接（打开）端口，投递到框架

    virtual bool GetClientDevInfo(uint16 u2ConnectID, _TTyDevParam& outParam) = 0;                           // 获取连接配置信息
    virtual bool IsConnect(uint16 u2ConnectID) = 0;                                                          // 是否连接（打开）状态

    virtual bool Close(uint16 u2ConnectID) = 0;                                                              // 关闭端口
    virtual bool Pause(uint16 u2ConnectID) = 0;                                                              // 暂停端口
    virtual bool Resume(uint16 u2ConnectID) = 0;                                                             // 恢复暂停端口

    virtual bool SendMessage(uint16 u2ConnectID, char*& pMessage, uint32 u4Len) = 0;                         // 发送数据
};
#endif