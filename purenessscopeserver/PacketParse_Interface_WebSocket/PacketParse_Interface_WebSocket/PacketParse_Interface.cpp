//ʵ������PSS��PacketParse����
//�򻯽ӿڣ��Ժ����ķ�ʽʵ��
//add by freeeyes

#include "define.h"
#include "WebSocketInfo.h"
#include "IMessageBlockManager.h"

#include "ace/svc_export.h"

#ifdef WIN32
#ifdef PACKETPARSE_INTERFACE_EXPORTS
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif
#else
#define DECLDIR ACE_Svc_Export
#endif

//HTTP�õ����жϱ�ʶ
#define HTTP_HEAD_END    "\r\n\r\n"
#define HTTP_BODY_LENGTH "Content-Length: "

using namespace PSS;

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

    //����WebSocket�������ڲ�����
    uint8 WebSocketDisposeHandIn(_WebSocketInfo* pWebSocketInfo, ACE_Message_Block* pCurrMessage, IMessageBlockManager* pMessageBlockManager, _Packet_Info* pPacketInfo);
    uint8 WebSocketDisposeDataIn(_WebSocketInfo* pWebSocketInfo, ACE_Message_Block* pCurrMessage, IMessageBlockManager* pMessageBlockManager, _Packet_Info* pPacketInfo);
    uint8 Decrypt(char* pOriData, uint32& u4Len, char* pEncryData, uint32& u4EncryLen);
    uint8 ReadDataPacketInfo(const char* pData, uint32& u4DataLen, uint16& u2CommandID, uint32& u4PacketLen, _Packet_Info* pPacketInfo);

    //������ͷ����Ҫ���pHeadInfo���ݽṹ����ɺ����_Head_Info�����ݽṹ
    bool Parse_Packet_Head_Info(uint32 u4ConnectID, ACE_Message_Block* pmbHead, IMessageBlockManager* pMessageBlockManager, _Head_Info* pHeadInfo)
    {
        if(NULL == pHeadInfo || NULL == pMessageBlockManager)
        {
            return false;
        }

        //HTTP��ʹ�ô˲���

        return true;
    }

    //�������壬��Ҫ���pBodyInfo���ݽṹ����ɺ����_Body_Info�����ݽṹ
    bool Parse_Packet_Body_Info(uint32 u4ConnectID, ACE_Message_Block* pmbbody, IMessageBlockManager* pMessageBlockManager, _Body_Info* pBodyInfo)
    {
        if(NULL == pBodyInfo || NULL == pMessageBlockManager)
        {
            return false;
        }

        //HTTP��ʹ�ô˲���

        return true;
    }

    //��ģʽ�ݽ����������ɹ���Ҫ���_Packet_Info�ṹ
    uint8 Parse_Packet_Stream(uint32 u4ConnectID, ACE_Message_Block* pCurrMessage, IMessageBlockManager* pMessageBlockManager, _Packet_Info* pPacketInfo)
    {
        //������������Ĵ���
        if(NULL == pCurrMessage || NULL == pMessageBlockManager)
        {
            return PACKET_GET_ERROR;
        }

        //�����ж���Ӧ�������ְ��߼��������ݰ��߼�
        _WebSocketInfo* pWebSocketInfo = App_WebSocketInfoManager::instance()->GetWebSocketInfo(u4ConnectID);

        if(NULL == pWebSocketInfo)
        {
            //û���ҵ����������Ϣ��˵���д��󣬶Ͽ�����
            return PACKET_GET_ERROR;
        }

        if(pWebSocketInfo->m_emState == WEBSOCKET_STATE_HANDIN)
        {
            return WebSocketDisposeHandIn(pWebSocketInfo, pCurrMessage, pMessageBlockManager, pPacketInfo);
        }
        else
        {
            return WebSocketDisposeDataIn(pWebSocketInfo, pCurrMessage, pMessageBlockManager, pPacketInfo);
        }
    }

    //ƴ�����ݷ��ذ������еķ������ݰ�����������
    bool Make_Send_Packet(uint32 u4ConnectID, const char* pData, uint32 u4Len, ACE_Message_Block* pMbData, uint16 u2CommandID)
    {
        if(pMbData == NULL && u2CommandID == 0)
        {
            return false;
        }

        //ƴװ���ݰ�
        memcpy_safe((char* )&u4Len, (uint32)sizeof(uint32), pMbData->wr_ptr(), (uint32)sizeof(uint32));
        pMbData->wr_ptr(sizeof(uint32));
        memcpy_safe((char* )pData, u4Len, pMbData->wr_ptr(), u4Len);
        pMbData->wr_ptr(u4Len);

        return true;
    }

    //�õ��������ݰ��ĳ���
    uint32 Make_Send_Packet_Length(uint32 u4ConnectID, uint32 u4DataLen, uint16 u2CommandID)
    {
        return u4DataLen + sizeof(uint32);
    }

    //�����ӵ�һ�ν�����ʱ�򣬷��صĽӿ��������Լ��Ĵ���
    bool Connect(uint32 u4ConnectID, _ClientIPInfo objClientIPInfo, _ClientIPInfo objLocalIPInfo)
    {
        //���ҵ�ǰ���Ӽ�¼�������޵�ǰ��¼
        return App_WebSocketInfoManager::instance()->Insert(u4ConnectID);
    }

    //�����ӶϿ���ʱ�򣬷������Լ��Ĵ���
    void DisConnect(uint32 u4ConnectID)
    {
        //�������������ӶϿ����߼�����
        App_WebSocketInfoManager::instance()->Delete(u4ConnectID);
    }

    uint8 Decrypt(char* pOriData, uint32& u4Len, char* pEncryData, uint32& u4EncryLen)
    {
        //֡ͷ���ȣ�Ĭ����6,��չ����8
        int nFrameLen = 6;

        if(u4Len < 6)
        {
            return PACKET_GET_NO_ENOUGTH;
        }

        unsigned char* mp_mask_byte = NULL;
        int nMinExpectedSize = 0;


        unsigned char payloadFlags = pOriData[0];

        if (payloadFlags != 129)
        {
            return PACKET_GET_ERROR;
        }

        unsigned char basicSize = pOriData[1] & 0x7F;
        unsigned int payloadSize;
        int masksOffset;

        if (basicSize <= 125)
        {
            payloadSize = basicSize;
            masksOffset = 2;
        }
        else if (basicSize == 126)
        {
            nMinExpectedSize += 2;

            if (u4Len < (uint32)nMinExpectedSize)
            {
                return PACKET_GET_NO_ENOUGTH;
            }

            payloadSize = ntohs( *(u_short*) (pOriData + 2) );
            masksOffset = 4;
            nFrameLen   = 8;
        }
        else if (basicSize == 127)
        {
            nMinExpectedSize += 8;

            if (u4Len < (uint32)nMinExpectedSize)
            {
                return PACKET_GET_NO_ENOUGTH;
            }

            payloadSize = ntohl( *(u_long*) (pOriData + 2) );
            masksOffset = 6;
            nFrameLen   = 10;
        }
        else
        {
            return PACKET_GET_ERROR;
        }

        nMinExpectedSize += payloadSize;

        if (u4Len < (uint32)nMinExpectedSize)
        {
            return PACKET_GET_NO_ENOUGTH;
        }

        char masks[4];
        memcpy_safe(pOriData + masksOffset, 4, (char* )masks, 4);

        memcpy_safe((char* )(pOriData + masksOffset + 4), payloadSize, pEncryData, payloadSize);

        for (unsigned int i = 0; i < payloadSize; i++)
        {
            pEncryData[i] = (pEncryData[i] ^ masks[i%4]);
        }

        pEncryData[payloadSize] = '\0';

        if(u4EncryLen < payloadSize)
        {
            //����ַ������������壬�򷵻ذ�����
            return PACKET_GET_ERROR;
        }

        u4EncryLen = payloadSize;

        //����6���ֽ�ͷ�ǹ̶��ģ���һ���ֽڹ̶���-127,����������ݵĳ��Ȼ���4����6�ֽ�
        if(payloadSize + nFrameLen > u4Len)
        {
            //˵������������������������ݳ��Ⱥ͵�ǰ���ݳ��Ȳ����
            return PACKET_GET_ERROR;
        }

        u4Len      = payloadSize + nFrameLen;

        return PACKET_GET_ENOUGTH;
    }

    uint8 ReadDataPacketInfo(const char* pData, uint32& u4DataLen, uint16& u2CommandID, uint32& u4PacketLen, _Packet_Info* pPacketInfo)
    {
        uint32 u4AllDataCount = u4DataLen;

        char* pInfo = NULL;
        char szTemp[MAX_BUFF_100] = {'\0'};

        //Ѱ�ҵ�һ��{�����ڽ綨���ݰ���Ϣ�ĳ���
        pInfo = (char* )ACE_OS::strstr(pData, "{");

        if(NULL == pInfo)
        {
            //����Ҳ��������ţ�˵�����ݰ��쳣���Ͽ��������
            return (uint8)PACKET_GET_ERROR;
        }

        if((int)(pInfo - pData) == 0)
        {
            //�Ҳ���ǰ�������ͷ������ʧ��
            return (uint8)PACKET_GET_ERROR;
        }

        //��������ַ���
        memcpy_safe((char* )pData, (uint32)(pInfo - pData), szTemp, (uint32)MAX_BUFF_100);

        //����ҵ��ˣ���ʼ�з����ݣ�������ݳ��Ⱥ�����ID
        char* pCommand = NULL;
        pCommand = (char* )ACE_OS::strstr(szTemp, ",");

        if(NULL == pInfo)
        {
            //����Ҳ������ţ�˵�����������Ǵ���ģ��Ͽ�����
            return (uint8)PACKET_GET_ERROR;
        }
        else
        {
            char szTemp2[MAX_BUFF_50] = {'\0'};

            if(pCommand - szTemp == 0)
            {
                //˵��û���ҵ�����ͷ������ʧ��
                return (uint8)PACKET_GET_ERROR;
            }

            memcpy_safe((char* )pData, (uint32)(pCommand - szTemp), szTemp2, (uint32)MAX_BUFF_50);
            //�õ������ֺ����ݰ�����
            //u2CommandID = (uint16)ACE_OS::atoi(szTemp2);
            //ʮ�������ַ���ת������
            int nCommandID = 0;
            sscanf(szTemp2, "%x", &nCommandID);
            pPacketInfo->m_u2PacketCommandID = (uint16)nCommandID;
            memcpy_safe((char* )(pCommand + 1), (uint32)(ACE_OS::strlen(szTemp) - (int)(pCommand - szTemp) - 1),
                        szTemp2,
                        (uint32)MAX_BUFF_50);
            szTemp2[ACE_OS::strlen(szTemp) - (int)(pCommand - szTemp) - 1] = '\0';
            u4PacketLen = (uint32)ACE_OS::atoi(szTemp2);

            if(u4PacketLen == 0 || u2CommandID == 0)
            {
                //�������������ֺͳ�����һ�����ԾͶϿ�����
                return (uint8)PACKET_GET_ERROR;
            }
            else
            {
                //�жϰ��Ƿ����������
                if(u4PacketLen > u4AllDataCount)
                {
                    return (uint8)PACKET_GET_NO_ENOUGTH;
                }
                else
                {
                    u4DataLen = u4PacketLen;
                    return (uint8)PACKET_GET_ENOUGTH;
                }
            }
        }
    }

    uint8 WebSocketDisposeHandIn(_WebSocketInfo* pWebSocketInfo, ACE_Message_Block* pCurrMessage, IMessageBlockManager* pMessageBlockManager, _Packet_Info* pPacketInfo)
    {
        //�������ְ�
        char* pData   = (char* )pCurrMessage->rd_ptr();     //�õ�������ݿ�����ֽ�
        uint32 u4Data = (uint32)pCurrMessage->length();     //�õ�������ݿ�ĳ���

        //���յ�������ճ�뻺��ȴ����а�����
        if(pWebSocketInfo->m_u4DataLength + u4Data > MAX_DECRYPTLENGTH)
        {
            OUR_DEBUG((LM_ERROR, "[CPacketParse::WebSocketDisposeHandIn]pWebSocketInfo is full.\n"));
            return PACKET_GET_ERROR;
        }

        memcpy_safe(pData, u4Data, (char* )&pWebSocketInfo->m_szData[pWebSocketInfo->m_u4DataLength], u4Data);
        pWebSocketInfo->m_u4DataLength += u4Data;

        //�ж��ǲ������ְ��Ľ������ҵ�ĩβ4���ַ��ǲ���\r\n\r\n
        if(pData[u4Data - 1] == '\n' && pData[u4Data - 2] == '\r'
           && pData[u4Data - 3] == '\n' && pData[u4Data - 4] == '\r')
        {
            //���յ����������������ݰ�����ʼ��������

            //����һ����ͷ����¼��ǰ�������г���
            pPacketInfo->m_pmbHead = pMessageBlockManager->Create(sizeof(uint32));

            if(NULL == pPacketInfo->m_pmbHead)
            {
                OUR_DEBUG((LM_ERROR, "[CPacketParse::WebSocketDisposeHandIn]m_pmbHead is NULL.\n"));
                return PACKET_GET_ERROR;
            }

            uint32 u4NetPacketLen = pWebSocketInfo->m_u4DataLength;
            memcpy_safe((char*)&u4NetPacketLen, (uint32)sizeof(uint32), (char* )pPacketInfo->m_pmbHead->wr_ptr(), (uint32)sizeof(uint32));
            pPacketInfo->m_pmbHead->wr_ptr(sizeof(uint32));
            pPacketInfo->m_u4HeadSrcLen  = (uint32)sizeof(uint32);
            pPacketInfo->m_u4HeadCurrLen = (uint32)sizeof(uint32);

            //��ð���
            pPacketInfo->m_pmbBody = pMessageBlockManager->Create(u4NetPacketLen);

            if(NULL == pPacketInfo->m_pmbBody)
            {
                OUR_DEBUG((LM_ERROR, "[CPacketParse::WebSocketDisposeHandIn]m_pmbBody is NULL.\n"));
                return PACKET_GET_ERROR;
            }

            memcpy_safe((char*)pWebSocketInfo->m_szData, pWebSocketInfo->m_u4DataLength, (char* )pPacketInfo->m_pmbBody->wr_ptr(), pWebSocketInfo->m_u4DataLength);
            pPacketInfo->m_pmbBody->wr_ptr(u4NetPacketLen);
            pPacketInfo->m_u4BodySrcLen  = u4NetPacketLen;
            pPacketInfo->m_u4BodyCurrLen = u4NetPacketLen;

            //����������(0xe001ָ��������ID)
            pPacketInfo->m_u2PacketCommandID = 0xee01;

            //����Ϊ���ݰ���ʽ
            pWebSocketInfo->m_emState = WEBSOCKET_STATE_DATAIN;

            //����������ݴӳ����Ƴ�
            pCurrMessage->rd_ptr(u4Data);

            //���û��崦�����
            pWebSocketInfo->m_u4DataLength = 0;

            return (uint8)PACKET_GET_ENOUGTH;
        }
        else
        {
            //û�н��յ��������������ݰ�����������
            return (uint8)PACKET_GET_NO_ENOUGTH;
        }
    }

    uint8 WebSocketDisposeDataIn(_WebSocketInfo* pWebSocketInfo, ACE_Message_Block* pCurrMessage, IMessageBlockManager* pMessageBlockManager, _Packet_Info* pPacketInfo)
    {
        //�������ݰ�
        char* pData   = (char* )pCurrMessage->rd_ptr();     //�õ�������ݿ�����ֽ�
        uint32 u4Data = (uint32)pCurrMessage->length();     //�õ�������ݿ�ĳ���
        uint32 u4PacketLen = 0;
        uint8 u1Ret        = (uint8)PACKET_GET_ERROR;

        //���յ�������ճ�뻺��ȴ����а�����
        if(pWebSocketInfo->m_u4DataLength + u4Data > MAX_ENCRYPTLENGTH)
        {
            OUR_DEBUG((LM_ERROR, "[CPacketParse::WebSocketDisposeHandIn]pWebSocketInfo is full.\n"));
            return (uint8)PACKET_GET_ERROR;
        }

        memcpy_safe(pData, u4Data, (char* )&pWebSocketInfo->m_szData[pWebSocketInfo->m_u4DataLength], u4Data);
        pWebSocketInfo->m_u4DataLength += u4Data;

        //��������Լ��Ϊ
        //<������>,<���ݰ�����><���ݰ���>

        //���Ƚ������ݰ�,���������ݰ���5K
        //����ٴ�����չ�����С
        if(pWebSocketInfo->m_u4DecryptDataLen + pWebSocketInfo->m_u4DataLength > MAX_DECRYPTLENGTH)
        {
            OUR_DEBUG((LM_ERROR, "[CPacketParse::WebSocketDisposeHandIn]m_u4DecryptDataLen is full.\n"));
            return (uint8)PACKET_GET_ERROR;
        }

        char* pDecryptData = (char* )&pWebSocketInfo->m_szDecryptData[pWebSocketInfo->m_u4DecryptDataLen];
        uint32 u4DecryptLen = MAX_DECRYPTLENGTH;
        uint32 u4OriPacketLen = pWebSocketInfo->m_u4DataLength;
        u1Ret = Decrypt(pWebSocketInfo->m_szData, u4OriPacketLen, pDecryptData, u4DecryptLen);

        if(u1Ret != (uint8)PACKET_GET_ENOUGTH)
        {
            return u1Ret;
        }

        pWebSocketInfo->m_u4DecryptDataLen += u4DecryptLen;

        //��������ֽ�����9��������Ϊ��ͷ������
        if(u4Data < 9)
        {
            return (uint8)PACKET_GET_NO_ENOUGTH;
        }

        //�Ѿ��������������ݰ����ж��Ƿ��к������ݰ��Ĳ�������
        if(pWebSocketInfo->m_u4DataLength > u4OriPacketLen)
        {
            //�к��������ݰ�����������Ҫ����һ��
            pWebSocketInfo->m_u4DataLength -= u4OriPacketLen;

            memcpy_safe((char* )&pWebSocketInfo->m_szData[u4OriPacketLen], pWebSocketInfo->m_u4DataLength, (char* )&pWebSocketInfo->m_szData, pWebSocketInfo->m_u4DataLength);
        }
        else
        {
            pWebSocketInfo->m_u4DataLength = 0;
        }

        //���յ������������ݰ�����ʼ��������
        //��õ�ǰ��ID�Ͱ���
        uint32 u4CurrDecryptDataLen = pWebSocketInfo->m_u4DecryptDataLen;
        u1Ret = ReadDataPacketInfo(pWebSocketInfo->m_szDecryptData, u4CurrDecryptDataLen, pPacketInfo->m_u2PacketCommandID, u4PacketLen, pPacketInfo);

        if(u1Ret == PACKET_GET_ENOUGTH)
        {
            //����һ����ͷ����¼��ǰ�������г���
            pPacketInfo->m_pmbHead = pMessageBlockManager->Create(sizeof(uint32));

            if(NULL == pPacketInfo->m_pmbHead)
            {
                return (uint8)PACKET_GET_ERROR;
            }

            //ȥ����ͷ��ֻ���������ڰ�����
            char* pInfo = (char* )ACE_OS::strstr(pWebSocketInfo->m_szDecryptData, "{");

            if(NULL == pInfo)
            {
                //����Ҳ��������ţ�˵�����ݰ��쳣���Ͽ��������
                return (uint8)PACKET_GET_ERROR;
            }

            uint32 u4NetPacketLen = u4CurrDecryptDataLen - (uint32)(pInfo - pWebSocketInfo->m_szDecryptData);
            memcpy_safe((char*)&u4NetPacketLen, (uint32)sizeof(uint32), (char* )pPacketInfo->m_pmbHead->wr_ptr(), (uint32)sizeof(uint32));
            pPacketInfo->m_pmbHead->wr_ptr(sizeof(uint32));
            pPacketInfo->m_u4HeadSrcLen  = (uint32)sizeof(uint32);
            pPacketInfo->m_u4HeadCurrLen = (uint32)sizeof(uint32);

            //��ð���
            pPacketInfo->m_pmbBody = pMessageBlockManager->Create(u4NetPacketLen);

            if(NULL == pPacketInfo->m_pmbBody)
            {
                return PACKET_GET_ERROR;
            }

            memcpy_safe((char*)pInfo, u4NetPacketLen, (char* )pPacketInfo->m_pmbBody->wr_ptr(), u4NetPacketLen);
            pPacketInfo->m_pmbBody->wr_ptr(u4NetPacketLen);
            pPacketInfo->m_u4BodySrcLen  = u4NetPacketLen;
            pPacketInfo->m_u4BodyCurrLen = u4NetPacketLen;

            //����������ݴӿ����Ƴ�
            pCurrMessage->reset();

            //�����ʣ�����ݣ�������ϵͳ�ж�һ��
            if(pWebSocketInfo->m_u4DataLength > 0)
            {
                pCurrMessage->size((size_t)pWebSocketInfo->m_u4DataLength);
                memcpy_safe(pWebSocketInfo->m_szData, pWebSocketInfo->m_u4DataLength, (char* )pCurrMessage->wr_ptr(), pWebSocketInfo->m_u4DataLength);
                pCurrMessage->wr_ptr(pWebSocketInfo->m_u4DataLength);
                pWebSocketInfo->m_u4DataLength = 0;
            }

            //����������������ݣ����ٷŻ�ȥ
            pWebSocketInfo->m_u4DecryptDataLen -= u4CurrDecryptDataLen;

            if(pWebSocketInfo->m_u4DecryptDataLen > 0)
            {
                memcpy_safe((char* )&pWebSocketInfo->m_szDecryptData[u4CurrDecryptDataLen],
                            pWebSocketInfo->m_u4DecryptDataLen,
                            pWebSocketInfo->m_szDecryptData,
                            pWebSocketInfo->m_u4DecryptDataLen);
            }

            return (uint8)PACKET_GET_ENOUGTH;
        }
        else
        {
            return u1Ret;
        }
    }

    //����˳���ʱ�����
    void Close()
    {
        OUR_DEBUG((LM_INFO, "[CPacketParse::Close]exit.\n"));
    }
}
