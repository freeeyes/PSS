#ifndef _ILOGOBJECT
#define _ILOGOBJECT

#include "define.h"
#include "ace/SString.h"
#include "ace/Message_Block.h"

//��־�����ݽṹ
struct _LogBlockInfo
{
public:
    uint32 m_u4LogID;                    //LogID���
    uint32 m_u4MailID;                   //�ʼ������ID���
    uint32 m_u4Length;                   //�鳤��
    char*  m_pBlock;                     //��ָ��
    bool   m_blIsUsed;                   //�Ƿ�����ʹ��
    char   m_szMailTitle[MAX_BUFF_200];  //�ʼ�����ı���

    ACE_Message_Block*  m_pmbQueuePtr;   //��Ϣ����ָ���

    _LogBlockInfo()
    {
        m_pBlock         = NULL;
        m_u4Length       = 0;
        m_u4LogID        = 0;
        m_blIsUsed       = false;
        m_u4MailID       = 0;
        m_szMailTitle[0] = '\0';

        //����������Ϣ����ģ��ָ�����ݣ������Ͳ��ط�����new��delete����������
        //ָ���ϵҲ����������ֱ��ָ��������ʹ�õ�ʹ����ָ��
        m_pmbQueuePtr  = new ACE_Message_Block(sizeof(_LogBlockInfo*));

        _LogBlockInfo** ppMessage = (_LogBlockInfo**)m_pmbQueuePtr->base();
        *ppMessage = this;
    }

    _LogBlockInfo(const _LogBlockInfo& ar)
    {
        this->m_pBlock      = ar.m_pBlock;
        this->m_u4Length    = ar.m_u4Length;
        this->m_blIsUsed    = ar.m_blIsUsed;
        this->m_u4LogID     = ar.m_u4LogID;
        this->m_u4MailID    = ar.m_u4MailID;
        this->m_pmbQueuePtr = new ACE_Message_Block(sizeof(_LogBlockInfo*));
        sprintf_safe(m_szMailTitle, MAX_BUFF_200, "%s", ar.m_szMailTitle);

        _LogBlockInfo** ppMessage = (_LogBlockInfo**)m_pmbQueuePtr->base();
        *ppMessage = this;
    }

    _LogBlockInfo& operator = (const _LogBlockInfo& ar)
    {
        if(NULL != this->m_pBlock)
        {
            SAFE_DELETE(this->m_pBlock);
        }

        this->m_pBlock      = ar.m_pBlock;
        this->m_u4Length    = ar.m_u4Length;
        this->m_blIsUsed    = ar.m_blIsUsed;
        this->m_u4LogID     = ar.m_u4LogID;
        this->m_u4MailID    = ar.m_u4MailID;
        memcpy_safe((char* )ar.m_pmbQueuePtr->base(), (uint32)ar.m_pmbQueuePtr->length(), m_pmbQueuePtr->base(), (uint32)m_pmbQueuePtr->length());
        sprintf_safe(m_szMailTitle, MAX_BUFF_200, "%s", ar.m_szMailTitle);

        _LogBlockInfo** ppMessage = (_LogBlockInfo**)m_pmbQueuePtr->base();
        *ppMessage = this;
        return *this;
    }

    ~_LogBlockInfo()
    {
        if(NULL != m_pmbQueuePtr)
        {
            m_pmbQueuePtr->release();
            m_pmbQueuePtr = NULL;
        }
    }

    ACE_Message_Block* GetQueueMessage()
    {
        return m_pmbQueuePtr;
    }

    void clear()
    {
        m_u4Length       = 0;
        m_u4LogID        = 0;
        m_blIsUsed       = false;
        m_u4MailID       = 0;
        m_szMailTitle[0] = '\0';
    }
};

//��־�����(��һ�����࣬�ɱ����̳�ʵ���ڲ�)
class IServerLogger
{
public:
    virtual ~IServerLogger() {}

    virtual int DoLog(int nLogType, _LogBlockInfo* pLogBlockInfo) = 0;
    virtual int GetLogTypeCount()                                 = 0;

    virtual uint32 GetBlockSize()                                 = 0;
    virtual uint32 GetPoolCount()                                 = 0;

    virtual uint32 GetCurrLevel()                                 = 0;
    virtual uint16 GetLogID(uint16 u2Index)                       = 0;
    virtual char*  GetLogInfoByServerName(uint16 u2LogID)         = 0;
    virtual char*  GetLogInfoByLogName(uint16 u2LogID)            = 0;
    virtual int    GetLogInfoByLogDisplay(uint16 u2LogID)         = 0;
    virtual uint16 GetLogInfoByLogLevel(uint16 u2LogID)           = 0;
    virtual bool   ReSet(uint32 u4CurrLogLevel)                   = 0;
};
#endif
