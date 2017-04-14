#ifndef _PACKETCONSOLEPARSE_H
#define _PACKETCONSOLEPARSE_H

//这个类是完全交给开发者使用的
//在这里开发者只要去实现下面的5个接口，就可以完成一个数据包处理过程
//这里给出了一个例子，如果只用流模式的话，需要再构造函数里面指定m_u1PacketMode为PACKET_WITHSTREAM
//然后实现GetPacketStream()函数，里面有一个我写的小例子。
//其实，流模式并非只针对无包头的协议，有包头的协议，你不想用SetPacketHead和SetPacketBody
//你也可以自己去在GetPacketStream这个函数里实现
//看你自己的喜好，不过我还是推荐前者，因为这样做可能效率比较高。
//add by freeeyes

#include "PacketParseBase.h"

#define PACKET_CONSOLE_HEAD           4            //Console包头长度

class CConsolePacketParse : public CPacketParseBase
{
public:
    CConsolePacketParse(void);
    virtual ~CConsolePacketParse(void);

    //初始化PacketParsed
    void Init();

    //得到符合条件的数据包头数据块，u4ConnectID是连接ID，pmbHead是数据块，pMessageBlockManager是数据块池，如果不用解密这个参数对你无效
    bool SetPacketHead(uint32 u4ConnectID, ACE_Message_Block* pmbHead, IMessageBlockManager* pMessageBlockManager);
    //得到符合条件的数据包体数据块，u4ConnectID是连接ID，pmbHead是数据块，pMessageBlockManager是数据块池，如果不用解密这个参数对你无效
    bool SetPacketBody(uint32 u4ConnectID, ACE_Message_Block* pmbBody, IMessageBlockManager* pMessageBlockManager);

    //专门对应 Mode为0的不带包头的数据包,如果是带包头的模式，这里什么都不用做。
    //因为用到了内存池，所以pHead和pBody由框架提供，并且由框架回收，所以在这里，不可以用new出来的pHead和pBody，否则会造成内存泄露。
    //这里要注意一下啊。当然，如果你觉得前面的接口繁琐，你也可以用这个接口实现你的规则，前提是你的m_u1PacketMode必须是PACKET_WITHSTREAM
    uint8 GetPacketStream(uint32 u4ConnectID, ACE_Message_Block* pCurrMessage, IMessageBlockManager* pMessageBlockManager);

    //拼接数据返回包，所有的返回数据包都会调用这个
    bool MakePacket(uint32 u4ConnectID, const char* pData, uint32 u4Len, ACE_Message_Block* pMbData, uint16 u2CommandID = 1);
    //得到返回数据包的长度
    uint32 MakePacketLength(uint32 u4ConnectID, uint32 u4DataLen, uint16 u2CommandID = 1);
    //当连接第一次建立的时候，返回的接口用于你自己的处理。
    bool Connect(uint32 u4ConnectID, _ClientIPInfo objClientIPInfo, _ClientIPInfo objLocalIPInfo);
    //当连接断开的时候，返回你自己的处理
    void DisConnect(uint32 u4ConnectID);
};

#endif
