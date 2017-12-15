# how to develop a parsing plug - in  
## documentation for development plug - ins  
> parsing plug-ins and logical plug - ins, parsing plug-ins do not provide packet processing.  
> it provides encryption and decryption services for packets and integrity verification services for packets.  
> you can implement it here, add your code.  
> data from this plug-in will eventually flow to the logical plug - in for processing.  
> the parsing plug-in currently implements three samples:    
> PacketParse_Interface parsing of normal packets。  
> PacketParse_Interface_Http http parsing of data packets  
> PacketParse_Interface_WebSocket websocket parsing of data packets  

```cpp
    DECLDIR bool Parse_Packet_Head_Info(uint32 u4ConnectID, ACE_Message_Block* pmbHead, IMessageBlockManager* pMessageBlockManager, _Head_Info* pHeadInfo);
    DECLDIR bool Parse_Packet_Body_Info(uint32 u4ConnectID, ACE_Message_Block* pmbbody, IMessageBlockManager* pMessageBlockManager, _Body_Info* pBodyInfo);
    DECLDIR uint8 Parse_Packet_Stream(uint32 u4ConnectID, ACE_Message_Block* pCurrMessage, IMessageBlockManager* pMessageBlockManager, _Packet_Info* pPacketInfo);
    DECLDIR bool Make_Send_Packet(uint32 u4ConnectID, const char* pData, uint32 u4Len, ACE_Message_Block* pMbData, uint16 u2CommandID);
    DECLDIR uint32 Make_Send_Packet_Length(uint32 u4ConnectID, uint32 u4DataLen, uint16 u2CommandID);
    DECLDIR bool Connect(uint32 u4ConnectID, _ClientIPInfo objClientIPInfo, _ClientIPInfo objLocalIPInfo);
    DECLDIR void DisConnect(uint32 u4ConnectID);
    DECLDIR void Close();
```
> Parse_Packet_Head_Info()Implement validation for processing packet headers, add your code here。(This function corresponds to main.xml configuration H&B mode)  
> Parse_Packet_Body_Info()Implement validation to process the packet body, add your code here.(This function corresponds to main.xml configuration H&B mode)  
> Parse_Packet_Stream() In this mode, you need to disassemble the packet header and the packet body according to your own rules.  
> Make_Send_Packet_Length() Gets the length of a send packet。  
> Make_Send_Packet() The grouping logic that handles sending data, add your code here.  
> Connect() When a client connection is established, you can handle some of the client connection initialization logic here.    
> DisConnect() When the client connection is disconnected, you can handle some client connection disconnection logic here.  
> Close() When the parsing plug-in is turned off, here you can add your code to do some memory cleanup.    

```cpp
//define PacketParse message
//packet head
struct _Head_Info
{
    uint32             m_u4HeadSrcLen;       //Original packet header length（Before parsing）
    uint32             m_u4HeadCurrLen;      //Current packet header length （After parsing）
    uint32             m_u4BodySrcLen;       //Current inclusion length（Before parsing）
    uint16             m_u2PacketCommandID;  //CommandID
    ACE_Message_Block* m_pmbHead;            //Header message block

    _Head_Info()
    {
        m_u4HeadSrcLen      = 0;
        m_u4HeadCurrLen     = 0;
        m_u4BodySrcLen      = 0;
        m_u2PacketCommandID = 0;
        m_pmbHead           = NULL;
    }
};

//Packet body structure
struct _Body_Info
{
    uint32             m_u4BodySrcLen;       //Original packet body length（Before parsing）
    uint32             m_u4BodyCurrLen;      //Current packet body length （After parsing）
    uint16             m_u2PacketCommandID;  //CommandID
    ACE_Message_Block* m_pmbBody;            //body message block

    _Body_Info()
    {
        m_u4BodySrcLen      = 0;
        m_u4BodyCurrLen     = 0;
        m_u2PacketCommandID = 0;
        m_pmbBody           = NULL;
    }
};

//stream Packet complete structure
struct _Packet_Info
{
    uint32             m_u4HeadSrcLen;       //Original packet header length（Before parsing）
    uint32             m_u4HeadCurrLen;      //Current packet header length （After parsing）
    uint32             m_u4BodySrcLen;       //Original packet body length（Before parsing）
    uint32             m_u4BodyCurrLen;      //Current packet body length （After parsing）
    uint16             m_u2PacketCommandID;  //CommandID
    ACE_Message_Block* m_pmbHead;            //Header message block
    ACE_Message_Block* m_pmbBody;            //body message block

    _Packet_Info()
    {
        m_u4HeadSrcLen      = 0;
        m_u4HeadCurrLen     = 0;
        m_u2PacketCommandID = 0;
        m_pmbHead           = NULL;

        m_u4BodySrcLen      = 0;
        m_u4BodyCurrLen     = 0;
        m_pmbBody           = NULL;
    }
};
```

