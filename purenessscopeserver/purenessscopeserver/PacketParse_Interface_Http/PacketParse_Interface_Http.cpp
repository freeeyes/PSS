//实现所有PSS的PacketParse解析
//简化接口，以函数的方式实现
//add by freeeyes

#include "define.h"
#include "HttpInfo.h"
#include "IMessageBlockManager.h"

#include "ace/svc_export.h"

#ifdef WIN32
#ifdef PACKETPARSE_INTERFACE_HTTP_EXPORTS
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif
#else
#define DECLDIR ACE_Svc_Export
#endif

//HTTP用到的判断标识
#define HTTP_HEAD_END    "\r\n\r\n"
#define HTTP_BODY_LENGTH "Content-Length: "

extern "C"
{
    DECLDIR bool Parse_Packet_Head_Info(uint32 u4ConnectID, ACE_Message_Block* pmbHead, IMessageBlockManager* pMessageBlockManager, _Head_Info* pHeadInfo);
    DECLDIR bool Parse_Packet_Body_Info(uint32 u4ConnectID, ACE_Message_Block* pmbbody, IMessageBlockManager* pMessageBlockManager, _Body_Info* pBodyInfo);
    DECLDIR uint8 Parse_Packet_Stream(uint32 u4ConnectID, ACE_Message_Block* pCurrMessage, IMessageBlockManager* pMessageBlockManager, _Packet_Info* pPacketInfo);
    DECLDIR bool Make_Send_Packet(uint32 u4ConnectID, const char* pData, uint32 u4Len, ACE_Message_Block* pMbData, uint16 u2CommandID);
    DECLDIR uint32 Make_Send_Packet_Length(uint32 u4ConnectID, uint32 u4DataLen, uint16 u2CommandID);
    DECLDIR bool Connect(uint32 u4ConnectID, _ClientIPInfo objClientIPInfo, _ClientIPInfo objLocalIPInfo);
    DECLDIR void DisConnect(uint32 u4ConnectID);
    DECLDIR void Close();

    uint8 HttpDispose(_HttpInfo* pHttpInfo, ACE_Message_Block* pCurrMessage, IMessageBlockManager* pMessageBlockManager, _Packet_Info* pPacketInfo);
    uint8 GetHttpBodyLen(char* pData, uint32 u4Len, uint32 u4HeadLen, uint32& u4BodyLen, _Packet_Info* pPacketInfo);

    //解析包头，需要填充pHeadInfo数据结构，完成后填充_Head_Info的数据结构
    bool Parse_Packet_Head_Info(uint32 u4ConnectID, ACE_Message_Block* pmbHead, IMessageBlockManager* pMessageBlockManager, _Head_Info* pHeadInfo)
    {
        ACE_UNUSED_ARG(u4ConnectID);
        ACE_UNUSED_ARG(pmbHead);

        if(NULL == pHeadInfo || NULL == pMessageBlockManager)
        {
            return false;
        }

        //HTTP不使用此部分

        return true;
    }

    //解析包体，需要填充pBodyInfo数据结构，完成后填充_Body_Info的数据结构
    bool Parse_Packet_Body_Info(uint32 u4ConnectID, ACE_Message_Block* pmbbody, IMessageBlockManager* pMessageBlockManager, _Body_Info* pBodyInfo)
    {
        ACE_UNUSED_ARG(u4ConnectID);
        ACE_UNUSED_ARG(pmbbody);

        if(NULL == pBodyInfo || NULL == pMessageBlockManager)
        {
            return false;
        }

        //HTTP不使用此部分

        return true;
    }

    //流模式据解析，解析成功需要填充_Packet_Info结构
    uint8 Parse_Packet_Stream(uint32 u4ConnectID, ACE_Message_Block* pCurrMessage, IMessageBlockManager* pMessageBlockManager, _Packet_Info* pPacketInfo)
    {
        //这里可以添加你的代码
        _HttpInfo* pHttpInfo = App_HttpInfoManager::instance()->GetHttpInfo(u4ConnectID);

        if(NULL == pHttpInfo)
        {
            //没有找到连接相关信息，说明有错误，断开连接
            return PACKET_GET_ERROR;
        }

        //判断http数据头是否完整
        return HttpDispose(pHttpInfo, pCurrMessage, pMessageBlockManager, pPacketInfo);
    }

    //拼接数据返回包，所有的返回数据包都会调用这个
    bool Make_Send_Packet(uint32 u4ConnectID, const char* pData, uint32 u4Len, ACE_Message_Block* pMbData, uint16 u2CommandID)
    {
        ACE_UNUSED_ARG(u4ConnectID);

        if(pMbData == NULL && u2CommandID == 0)
        {
            return false;
        }

        //拼装数据包
        memcpy_safe((char* )&u4Len, (uint32)sizeof(uint32), pMbData->wr_ptr(), (uint32)sizeof(uint32));
        pMbData->wr_ptr(sizeof(uint32));
        memcpy_safe((char* )pData, u4Len, pMbData->wr_ptr(), u4Len);
        pMbData->wr_ptr(u4Len);

        return true;
    }

    //得到返回数据包的长度
    uint32 Make_Send_Packet_Length(uint32 u4ConnectID, uint32 u4DataLen, uint16 u2CommandID)
    {
        ACE_UNUSED_ARG(u4ConnectID);
        ACE_UNUSED_ARG(u2CommandID);
        return u4DataLen + sizeof(uint32);
    }

    //当连接第一次建立的时候，返回的接口用于你自己的处理。
    bool Connect(uint32 u4ConnectID, _ClientIPInfo objClientIPInfo, _ClientIPInfo objLocalIPInfo)
    {
        ACE_UNUSED_ARG(objClientIPInfo);
        ACE_UNUSED_ARG(objLocalIPInfo);
        //查找当前连接记录里面有无当前记录
        return App_HttpInfoManager::instance()->Insert(u4ConnectID);
    }

    //当连接断开的时候，返回你自己的处理
    void DisConnect(uint32 u4ConnectID)
    {
        //这里添加你对连接断开的逻辑处理
        App_HttpInfoManager::instance()->Delete(u4ConnectID);
    }

    //Http解析协议部分
    uint8 HttpDispose(_HttpInfo* pHttpInfo, ACE_Message_Block* pCurrMessage, IMessageBlockManager* pMessageBlockManager, _Packet_Info* pPacketInfo)
    {
        if((NULL == pHttpInfo)||(NULL == pCurrMessage)||(NULL == pMessageBlockManager)||(NULL == pPacketInfo))
        {
            OUR_DEBUG((LM_ERROR, "[CPacketParse::HttpDispose]ptrpoint is full.\n"));
            return PACKET_GET_ERROR;
        }

        char* pData   = pCurrMessage->rd_ptr();     //得到这个数据块的首字节
        uint32 u4Data = (uint32)pCurrMessage->length();     //得到这个数据块的长度

        //将收到的数据粘入缓冲等待做切包处理
        if(pHttpInfo->m_u4DataLength + u4Data > MAX_DECRYPTLENGTH)
        {
            OUR_DEBUG((LM_ERROR, "[CPacketParse::HttpDispose]pWebSocketInfo is full.\n"));
            return PACKET_GET_ERROR;
        }

        //OUR_DEBUG((LM_ERROR, "[CPacketParse::HttpDispose]sizeof()=%d.\n", sizeof(pHttpInfo->m_szData)));
        //OUR_DEBUG((LM_ERROR, "[CPacketParse::HttpDispose]m_u4DataLength=%d.\n", pHttpInfo->m_u4DataLength));
        //OUR_DEBUG((LM_ERROR, "[CPacketParse::HttpDispose]u4Data=%d.\n", u4Data));

        ACE_OS::memcpy(&pHttpInfo->m_szData[pHttpInfo->m_u4DataLength], pData, u4Data);
        pHttpInfo->m_u4DataLength += u4Data;
        //pHttpInfo->m_szData[pHttpInfo->m_u4DataLength] = '\0';

        //寻找Http包头
        char* pHttpHead = ACE_OS::strstr(pHttpInfo->m_szData, HTTP_HEAD_END);

        if(NULL == pHttpHead)
        {
            return PACKET_GET_NO_ENOUGTH;
        }

        //OUR_DEBUG((LM_ERROR, "[CPacketParse::HttpDispose]http:\n%s", pHttpInfo->m_szData));
        uint32 u4HttpHeadLen = (uint32)(pHttpHead - pHttpInfo->m_szData + 4);
        //OUR_DEBUG((LM_ERROR, "[CPacketParse::HttpDispose]u4HttpHeadLen=%d.\n", u4HttpHeadLen));

        //找到了完整的包头
        pPacketInfo->m_pmbHead = pMessageBlockManager->Create(u4HttpHeadLen + 1);

        if(NULL == pPacketInfo->m_pmbHead)
        {
            OUR_DEBUG((LM_ERROR, "[CPacketParse::HttpDispose]m_pmbHead is NULL.\n"));
            return PACKET_GET_ERROR;
        }

        memcpy(pPacketInfo->m_pmbHead->wr_ptr(), (char*)pHttpInfo->m_szData, u4HttpHeadLen);
        pPacketInfo->m_pmbHead->wr_ptr(u4HttpHeadLen);

        //设置命令字(HTTP固定命令字)
        pPacketInfo->m_u2PacketCommandID = 0xea01;
        pPacketInfo->m_u4HeadSrcLen      = u4HttpHeadLen + 1;
        pPacketInfo->m_u4HeadCurrLen     = u4HttpHeadLen + 1;

        //查看有没有包体
        uint32 u4HttpBodyLength = 0;
        uint8 u1Ret = GetHttpBodyLen(pHttpInfo->m_szData, pHttpInfo->m_u4DataLength, u4HttpHeadLen, u4HttpBodyLength, pPacketInfo);

        if(u1Ret != PACKET_GET_ENOUGTH)
        {
            return u1Ret;
        }

        //OUR_DEBUG((LM_ERROR, "[CPacketParse::HttpDispose]u4HttpBodyLength=%d.\n", u4HttpBodyLength));
        if(u4HttpBodyLength == 0)
        {
            //获得包体
            pPacketInfo->m_pmbBody = NULL;
            pPacketInfo->m_u4BodySrcLen  = 0;
            pPacketInfo->m_u4BodyCurrLen = 0;
        }
        else
        {
            //有包体，创建包体
            pPacketInfo->m_pmbBody = pMessageBlockManager->Create(u4HttpBodyLength + 1);

            if(NULL == pPacketInfo->m_pmbBody)
            {
                OUR_DEBUG((LM_ERROR, "[CPacketParse::HttpDispose]m_pmbBody is NULL.\n"));
                return PACKET_GET_ERROR;
            }

            memcpy(pPacketInfo->m_pmbBody->wr_ptr(), (char*)pHttpHead+4, u4HttpBodyLength);
            pPacketInfo->m_pmbBody->wr_ptr(u4HttpBodyLength);

            pPacketInfo->m_u4BodySrcLen  = (uint32)u4HttpBodyLength + 1;
            pPacketInfo->m_u4BodyCurrLen = (uint32)u4HttpBodyLength + 1;
        }

        //处理完的数据从池中移除
        pCurrMessage->rd_ptr(u4Data);

        pHttpInfo->Init();
        return (uint8)PACKET_GET_ENOUGTH;
    }

    uint8 GetHttpBodyLen(char* pData, uint32 u4Len, uint32 u4HeadLen, uint32& u4BodyLen, _Packet_Info* pPacketInfo)
    {
        ACE_UNUSED_ARG(pPacketInfo);
        char szBodyLen[10] = {'\0'};
        int nNameLen = (int)ACE_OS::strlen(HTTP_BODY_LENGTH);

        //OUR_DEBUG((LM_ERROR, "[CPacketParse::GetHttpBodyLen]nNameLen=%d.\n", nNameLen));
        //OUR_DEBUG((LM_ERROR, "[CPacketParse::GetHttpBodyLen]pData=%s.\n", pData));

        //解析出整个Http包长
        char* pLength = ACE_OS::strstr(pData, HTTP_BODY_LENGTH);

        if(NULL != pLength)
        {
            uint8 u1LengthLen = 0;

            //包含了包长字段,解析出包长字段
            for(int i = nNameLen; i < nNameLen + 9; i++)
            {
                if(pLength[i] == '\r')
                {
                    break;
                }

                u1LengthLen++;
            }

            //OUR_DEBUG((LM_ERROR, "[CPacketParse::GetHttpBodyLen]u1LengthLen=%d.\n", u1LengthLen));
            ACE_OS::memcpy(szBodyLen, &pLength[nNameLen], u1LengthLen);

            u4BodyLen = ACE_OS::atoi(szBodyLen);

            //OUR_DEBUG((LM_ERROR, "[CPacketParse::GetHttpBodyLen]u4BodyLen=%d.\n", u4BodyLen));
            if(u4BodyLen == 0)
            {
                return PACKET_GET_ENOUGTH;
            }

            //如果接受的字节比http里面的数据长度短，说明没接受完
            if(u4BodyLen > u4Len - u4HeadLen)
            {
                return PACKET_GET_NO_ENOUGTH;
            }
        }
        else
        {
            //找不到包长，就把所有数据算作包长
            u4BodyLen = u4Len - u4HeadLen;
        }

        return PACKET_GET_ENOUGTH;
    }

    //插件退出的时候调用
    void Close()
    {
        OUR_DEBUG((LM_INFO, "[CPacketParse::Close]begin.\n"));
        App_HttpInfoManager::close();
        OUR_DEBUG((LM_INFO, "[CPacketParse::Close]end.\n"));
    }
}

