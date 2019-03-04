#ifndef _PACKETPARSE_H
#define _PACKETPARSE_H

//这个类是完全交给开发者使用的
//在这里开发者只要去实现下面的5个接口，就可以完成一个数据包处理过程
//这里给出了一个例子，如果只用流模式的话，需要再构造函数里面指定m_u1PacketMode为PACKET_WITHSTREAM
//然后实现GetPacketStream()函数，里面有一个我写的小例子。
//其实，流模式并非只针对无包头的协议，有包头的协议，你不想用SetPacketHead和SetPacketBody
//你也可以自己去在GetPacketStream这个函数里实现
//看你自己的喜好，不过我还是推荐前者，因为这样做可能效率比较高。
//add by freeeyes

#include "PacketParseBase.h"

#define PACKET_HEAD_LENGTH 40

class CPacketParse : public CPacketParseBase
{
public:
    CPacketParse(void);
    virtual ~CPacketParse(void);

    //初始化PacketParsed
    void Init();

    void SetHashID(int nHashID);
    int  GetHashID();

private:
    int m_nHashID;
};

#endif
