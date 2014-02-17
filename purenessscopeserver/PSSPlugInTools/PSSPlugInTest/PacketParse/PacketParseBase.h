#ifndef _PACKETPARSEBASE_H
#define _PACKETPARSEBASE_H

#include "BuffPacket.h"
#include "IMessageBlockManager.h"

#ifdef WIN32
#if defined PACKETPARSE_BUILD_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif
#else
#define DLL_EXPORT
#endif 

#ifdef WIN32
class DLL_EXPORT CPacketParseBase
#else
class CPacketParseBase
#endif 
{
public:
	CPacketParseBase(void);

	virtual ~CPacketParseBase(void);

	void Init();                                 //初始化CPacketParseBase的所有数据对象
	void Clear();                                //清理当前数据缓冲 
	void Close();                                //删除所有当前数据对象

	const char* GetPacketVersion();              //得到解析包逻辑版本 
	uint8 GetPacketMode();                       //得到当前包的模式，1是带包头的，0是不带包头的（需要判定头尾标志的），默认是1
	uint32 GetPacketHeadLen();                   //得到数据包头长度（解密后）
	uint32 GetPacketBodyLen();                   //得到数据包体长度（解密后）

	uint16 GetPacketCommandID();
	bool GetIsHead();                            //得到包头标记位，如果当前是包头，则返回True，否则返回False
	uint32 GetPacketHeadSrcLen();                //得到数据包头长度（解密前）
	uint32 GetPacketBodySrcLen();                //得到数据包体长度（解密前）

	ACE_Message_Block* GetMessageHead();         //得到数据包头内容（解密后） 
	ACE_Message_Block* GetMessageBody();         //得到数据包体内容（解密后）

	virtual bool SetPacketHead(uint32 u4ConnectID, ACE_Message_Block* pmbHead, IMessageBlockManager* pMessageBlockManager)           = 0;  //设置得到的数据包头，并在这里解密
	virtual bool SetPacketBody(uint32 u4ConnectID, ACE_Message_Block* pmbBody, IMessageBlockManager* pMessageBlockManager)           = 0;  //设置得到的数据包体，并在这里解密 
	virtual uint8 GetPacketStream(uint32 u4ConnectID, ACE_Message_Block* pCurrMessage, IMessageBlockManager* pMessageBlockManager)   = 0;  //专门对应 Mode为0的不带包头的数据包,如果是带包头的模式，这里什么都不用做。
	virtual bool MakePacket(uint32 u4ConnectID, const char* pData, uint32 u4Len, ACE_Message_Block* pMbData, uint16 u2CommandID = 0) = 0;                        //创建返回包体结构
	virtual uint32 MakePacketLength(uint32 u4ConnectID, uint32 u4DataLen, uint16 u2CommandID = 0)                                    = 0;                        //预先得到返回包体的长度

	virtual bool Connect(uint32 u4ConnectID, _ClientIPInfo& objClientIPInfo)                                                         = 0;  //处理连接建立的代码
	virtual void DisConnect(uint32 u4ConnectID)                                                                                      = 0;  //处理连接断开的代码

protected:
	uint32 m_u4PacketHead;               //包头的长度
	uint32 m_u4PacketData;               //包体的长度
	uint32 m_u4HeadSrcSize;              //包头的原始长度 
	uint32 m_u4BodySrcSize;              //包体的原始长度
	uint16 m_u2PacketCommandID;          //包命令
	bool   m_blIsHead;
	char   m_szPacketVersion[MAX_BUFF_20];   //包解析器版本
	uint8  m_u1PacketMode;                   //包解析模式    

	ACE_Message_Block* m_pmbHead;   //包头部分
	ACE_Message_Block* m_pmbBody;   //包体部分

	CBuffPacket m_objCurrBody;      //记录尚未完整的包体
};

#endif
