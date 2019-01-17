#include "ProTTyHandler.h"

CProTTyHandler::CProTTyHandler() : m_blState(false), m_blPause(false), m_pTTyMessage(NULL)
{
    m_szName[0] = 0;
    m_pmbReadBuff = new ACE_Message_Block(MAX_BUFF_1024);
}

CProTTyHandler::~CProTTyHandler()
{
    Close();

    delete m_pmbReadBuff;
}

bool CProTTyHandler::ConnectTTy()
{
    if (true == m_blState)
    {
        //如果设备已经在连接状态则什么也不做
        return true;
    }

    //连接设备描述符
    if (m_Connector.connect(m_Ttyio, ACE_DEV_Addr(m_szName), 0, ACE_Addr::sap_any, 0, O_RDWR | FILE_FLAG_OVERLAPPED) == -1)
    {
        OUR_DEBUG((LM_INFO, "[CProTTyHandler::Init]m_Connector.connect(%s) fail.\n", m_szName));
        return false;
    }

    //关联设备本身
    if (m_Ttyio.control(ACE_TTY_IO::SETPARAMS, &m_ObjParams) == -1)
    {
        OUR_DEBUG((LM_INFO, "[CProTTyHandler::Init]m_Ttyio SETPARAMS(%s) fail.\n", m_szName));
        return false;
    }

    //将句柄绑定给反应器(读对象)
    if (-1 == m_ObjReadRequire.open(*this, m_Ttyio.get_handle(), 0, this->proactor()))
    {
        OUR_DEBUG((LM_INFO, "[CProTTyHandler::Init]m_Ttyio open(%s) read fail.\n", m_szName));
        return false;
    }

    //将句柄绑定给反应器（写对象）
    if (-1 == m_ObjWriteRequire.open(*this, m_Ttyio.get_handle(), 0, this->proactor()))
    {
        OUR_DEBUG((LM_INFO, "[CProTTyHandler::Init]m_Ttyio open(%s) write fail.\n", m_szName));
        return false;
    }

    m_blState = true;
    return true;
}

void CProTTyHandler::Close()
{
    if (true == m_blState)
    {
        m_ObjReadRequire.cancel();
        m_ObjWriteRequire.cancel();

        m_Ttyio.close();
        m_blState = false;
    }
}

bool CProTTyHandler::Init(uint32 u4ConnectID, const char* pName, ACE_TTY_IO::Serial_Params inParams, ITTyMessage* pTTyMessage)
{
    m_u4ConnectID = u4ConnectID;
    sprintf_safe(m_szName, MAX_BUFF_100, "%s", pName);
    m_pTTyMessage = pTTyMessage;
    m_ObjParams   = inParams;

    //初始化连接设备
    ConnectTTy();

    Ready_To_Read_Buff();
    return true;
}

bool CProTTyHandler::GetConnectState()
{
    return m_blState;
}

ACE_TTY_IO::Serial_Params CProTTyHandler::GetParams()
{
    return m_ObjParams;
}

void CProTTyHandler::SetPause(bool blPause)
{
    m_blPause = blPause;
}

bool CProTTyHandler::GetPause()
{
    return m_blPause;
}

void CProTTyHandler::handle_read_file(const ACE_Asynch_Read_File::Result& result)
{
    if (!result.success())
    {
        //接收设备数据异常
        OUR_DEBUG((LM_ERROR, "[CProTTyHandler::handle_read_file]Error:%d.\n", (int)result.error()));

        //通知上层应用
        m_pTTyMessage->ReportMessage(m_u4ConnectID, (uint32)result.error(), EM_TTY_EVENT_RW_ERROR);

        //断开当前设备
        Close();
        return;
    }

    if (0 == result.bytes_transferred())
    {
        Ready_To_Read_Buff();
        return;
    }

    ACE_Message_Block& mb = result.message_block();

    if (NULL != m_pTTyMessage && true == m_blPause)
    {
        //回调接收数据函数
        m_pTTyMessage->RecvData(m_u4ConnectID, mb.rd_ptr(), (uint32)result.bytes_transferred());
    }

    Ready_To_Read_Buff();
}

void CProTTyHandler::handle_write_file(const ACE_Asynch_Write_File::Result& result)
{
    if (!result.success())
    {
        OUR_DEBUG((LM_ERROR, "[CProTTyHandler::handle_write_file]Error:%s.\n", result.error()));
        return;
    }

    OUR_DEBUG((LM_ERROR, "[CProTTyHandler::handle_write_file]Send OK:(%d).\n", result.message_block().length()));
    result.message_block().release();
}

bool CProTTyHandler::Send_Data(const char* pData, ssize_t nLen)
{
    //如果连接已断开，这里尝试重连
    ConnectTTy();

    if (true == m_blState && true == m_blPause)
    {
        ACE_Message_Block* m = new ACE_Message_Block(nLen);
        m->copy(pData, nLen);

        int nCurrLen = (int)m->length();

        if (0 != m_ObjWriteRequire.write(*m, m->length()))
        {
            //发送数据失败
            m_pTTyMessage->ReportMessage(m_u4ConnectID, (uint32)errno, EM_TTY_EVENT_RW_ERROR);
        }

        //中断设备
        Close();

        return true;
    }
    else
    {
        //当前连接中断，无法发送数据
        return false;
    }

}

void CProTTyHandler::Ready_To_Read_Buff()
{
    m_pmbReadBuff->reset();
    m_ObjReadRequire.read(*m_pmbReadBuff, m_pmbReadBuff->space());
}
