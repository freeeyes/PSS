#ifndef _ITTYCLIENTMANAGER_H
#define _ITTYCLIENTMANAGER_H

#include "ITTyMessage.h"

//�ṩ���Ӵ��ڣ�����������豸�Ķ�Ӧ�ֶκͷ�����
//add by Tangkai & freeyes

struct _TTyDevParam // ������ACE_TTY_IO::Serial_Params����һ�·��㸳ֵ
{
    //���ò���
    char          m_szDevName[32];         //�豸�������ں�,����,���⣩
    int           m_nBaudRate;              //������
    unsigned char m_uDatabits;              //����λ��5��6��7��8
    unsigned char m_uStopbits;              //ֹͣλ��1��2
    const char*   m_pParitymode;            //У��λ��none,even,odd,mark,space
    bool          m_blCTSenb;               //����CTS
    unsigned char m_uRTSenb;                //RTS����:0,1,2,3
    bool          m_blDSRenb;               //����DSR
    bool          m_blDTRdisable;           //�ر�DTR
    int           m_nReadtimeoutmsec;       //��ȡ��ʱ

    // �ǳ��ò���
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

//�ļ�������������
class ITTyClientManager
{
public:
    virtual ~ITTyClientManager() {}
    virtual int Connect(uint16 u2ConnectID, const char* pName, _TTyDevParam& inParam, ITTyMessage* pMessageRecv) = 0;           // ���ӣ��򿪣��˿�

    virtual bool GetClientDevInfo(uint16 u2ConnectID, _TTyDevParam& outParam) = 0;                           // ��ȡ����������Ϣ
    virtual bool IsConnect(uint16 u2ConnectID) = 0;                                                          // �Ƿ����ӣ��򿪣�״̬

    virtual bool Close(uint16 u2ConnectID) = 0;                                                              // �رն˿�
    virtual bool Pause(uint16 u2ConnectID) = 0;                                                              // ��ͣ�˿�
    virtual bool Resume(uint16 u2ConnectID) = 0;                                                             // �ָ���ͣ�˿�

    virtual bool SendMessage(uint16 u2ConnectID, char*& pMessage, uint32 u4Len) = 0;                         // ��������
};
#endif