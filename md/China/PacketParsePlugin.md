# 如何开发一个解析插件  
## 开发插件的文件说明  
> 解析插件和逻辑插件不同，解析插件不提供数据包的处理。  
> 它提供的是数据包的加解密服务，以及数据包的完整性验证服务。  
> 你可以在这里实现它，添加你的代码。  
> 通过此插件的数据，将会最终流到逻辑插件中去处理。  
> 解析插件目前实现了三个样例：  
> PacketParse_Interface 普通数据包的解析。  
> PacketParse_Interface_Http http数据包的解析  
> PacketParse_Interface_WebSocket websocket数据包的解析

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
> Parse_Packet_Head_Info()实现处理数据包头的验证，在这里添加你的代码。(此函数对应main.xml配置H&B模式)  
> Parse_Packet_Body_Info()实现处理数据包体的验证，在这里添加你的代码。(此函数对应main.xml配置H&B模式)  
> Parse_Packet_Stream()实现对流数据的处理，在这个模式下，需要你自己按照自己的规则拆解数据包头和包体。
> Make_Send_Packet_Length()获得一个发送数据包的长度。  
> Make_Send_Packet()处理发送数据的组包逻辑，在这里添加你的代码。
> Connect() 当客户端连接建立的时候，你可以在这里处理一些客户端连接的初始化逻辑。  
> DisConnect() 当客户端连接断开的时候，你可以在这里处理一些客户端连接断开逻辑。  
> Close() 当解析插件被关闭的时候，这里可以添加你的代码做一些内存清理动作。  

```cpp
//定义PacketParse的相关消息体
//数据包头结构
struct _Head_Info
{
    uint32             m_u4HeadSrcLen;       //原始数据包头长（解析前）
    uint32             m_u4HeadCurrLen;      //当前数据包长 （解析后）
    uint32             m_u4BodySrcLen;       //当前包体长度（解析前）
    uint16             m_u2PacketCommandID;  //CommandID
    ACE_Message_Block* m_pmbHead;            //包头消息体

    _Head_Info()
    {
        m_u4HeadSrcLen      = 0;
        m_u4HeadCurrLen     = 0;
        m_u4BodySrcLen      = 0;
        m_u2PacketCommandID = 0;
        m_pmbHead           = NULL;
    }
};

//数据包体结构
struct _Body_Info
{
    uint32             m_u4BodySrcLen;       //原始数据包体长（解析前）
    uint32             m_u4BodyCurrLen;      //当前数据包长 （解析后）
    uint16             m_u2PacketCommandID;  //CommandID(如果有，则直接赋值，如果没有，则保持初始值不变)
    ACE_Message_Block* m_pmbBody;            //包头消息体

    _Body_Info()
    {
        m_u4BodySrcLen      = 0;
        m_u4BodyCurrLen     = 0;
        m_u2PacketCommandID = 0;
        m_pmbBody           = NULL;
    }
};

//数据包完整结构
struct _Packet_Info
{
    uint32             m_u4HeadSrcLen;       //原始数据包头长（解析前）
    uint32             m_u4HeadCurrLen;      //当前数据包长 （解析后）
    uint32             m_u4BodySrcLen;       //原始数据包头长（解析前）
    uint32             m_u4BodyCurrLen;      //当前数据包长 （解析后）
    uint16             m_u2PacketCommandID;  //CommandID
    ACE_Message_Block* m_pmbHead;            //包头消息体
    ACE_Message_Block* m_pmbBody;            //包头消息体

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

