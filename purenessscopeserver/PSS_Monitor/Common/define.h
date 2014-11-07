#ifndef _DEFINE_H
#define _DEFINE_H

// Define.h
// 这里定义所有服务器用到的通用的东西，在这里加载一些ace的头和通用的一些数据方法
// add by freeeyes
// 2008-12-22

#include "ace/SString.h"
#include "ace/Malloc.h"
#include "ace/Malloc_T.h"
#include "ace/Task_T.h"
#include "ace/Local_Memory_Pool.h"
#include "ace/Time_Value.h"
#include "ace/OS_main.h"
#include "ace/OS_NS_sys_stat.h"
#include "ace/OS_NS_sys_socket.h"
#include "ace/OS_NS_unistd.h"
#include "ace/High_Res_Timer.h"
#include "ace/INET_Addr.h" 
#include <math.h>

#include <vector>

using namespace std;

#ifndef NULL
#define NULL 0
#endif

#define MAINCONFIG            "main.xml"
#define ALERTCONFIG           "alert.xml"
#define FORBIDDENIP_FILE      "forbiddenIP.xml"

#define MAX_BUFF_9    9
#define MAX_BUFF_20   20
#define MAX_BUFF_50   50
#define MAX_BUFF_100  100
#define MAX_BUFF_200  200
#define MAX_BUFF_300  300
#define MAX_BUFF_500  500
#define MAX_BUFF_1000 1000
#define MAX_BUFF_1024 1024

/*
//计算当前版本号是否与制定版本好一致
static bool Convert_Version(int nTagVserion)
{
	int nCurrVserion = 0;
	nCurrVserion += (int)ACE::major_version() * 1000;
	nCurrVserion += (int)ACE::minor_version() * 100;
	nCurrVserion += (int)ACE::beta_version();

	if(nTagVserion >= nCurrVserion)
	{
		return true;
	}
	else
	{
		return false;
	}
}

#define CONVERT_ACE_VERSION Convert_Version(6200)
*/

#define CONSOLE_HEAD_SIZE     6

enum em_State
{
	MESSAGE_HEAD = 0,
	MESSAGE_BODY,
};

//根据不同的操作系统，定义不同的recv接收参数类型
#ifdef WIN32
#define MSG_NOSIGNAL          0            //信号量参数（WINDOWS）
#endif

#define SERVER_ACTOR_REACTOR  0
#define SERVER_ACTOR_PROACTOR 1

#define SERVER_ACTOR_TYPE     SERVER_ACTOR_PROACTOR         //当前服务器所用的架构类型，可以是Proactor，也可以是Reactor

#define MAX_DEV_POLL_COUNT    5000         //Devpoll处理链接的最大个数

#define CONSOLE_ENABLE        1            //打开后台端口
#define CONSOLE_DISABLE       0            //关闭后台端口

#define CONNECT_CLOSE_TIME    2            //链接缓冲关闭时间延迟

#define MAX_MSG_THREADCOUNT   1            //默认的逻辑线程处理数
#define MAX_MSG_THREADQUEUE   60000        //处理线程的消息队列数
#define MAX_MSG_TIMEDELAYTIME 60           //逻辑线程自检时间间隔
#define MAX_MSG_STARTTIME     1            //逻辑线程处理开始时间
#define MAX_MSG_MASK          64000        //逻辑Mark的线程数
#define MAX_MSG_PUTTIMEOUT    100          //放入逻辑的延迟
#define MAX_MSG_SENDPACKET    10           //最多缓冲发送包的个数,多于这个数字报警并丢弃下一个发送数据包
#define MAX_MSG_SNEDTHRESHOLD 10           //发送阀值(消息包的个数)
#define MAX_MSG_SENDCHECKTIME 100          //每隔多少毫秒进行一次发送的阀值
#define MAX_MSG_THREADTIMEOUT 30           //线程判定超时
#define MAX_MSG_PACKETTIMEOUT 5            //处理数据包超时时间
#define MAX_MSG_SOCKETBUFF    50*1024      //接收数据缓存大小
#define MAX_MSG_SENDTIMEOUT   5            //发送超时时间
#define MAX_MSG_HANDLETIME    120          //统计当前连接数的间隔
#define MAX_MSG_SENDALIVETIME 60           //发送存活包的时间间隔
#define MAX_MSG_SENDALCOUNT   5            //判定链接是否存活的极限
#define MAX_MSG_PACKETLENGTH  20*1024      //最大单一数据包长度限制 
#define MAX_MP_POOL           5000         //_MakePacket交换池里面的大小
#define MAX_HANDLE_POOL       1000         //默认Handler对象池的大小
#define MAX_ASYNCH_BACKLOG    100          //默认设置的BACKLOG数量
#define MAX_LOADMODEL_CLOSE   5            //默认等待模块关闭时间
#define MAX_CONNECT_COUNT     10000        //默认单位时间最大链接重连次数
#define MAX_BLOCK_SIZE        2048         //默认阻塞缓冲数据块的大小
#define MAX_BLOCK_COUNT       3            //默认最大的Block次数
#define MAX_BLOCK_TIME        1            //默认等待重发时间（单位是秒）
#define MAX_QUEUE_TIMEOUT     20           //默认队列超时处理时间
#define MAX_RECV_UDP_TIMEOUT  3            //最大接收超时时间(UDP) 

#define PACKET_PARSE          1            //消息处理包标志
#define PACKET_CONNECT        2            //链接建立事件消息标志
#define PACKET_CDISCONNECT    3            //客户端断开事件消息标志
#define PACKET_SDISCONNECT    4            //服务器断开事件消息标志
#define PACKET_SEND_TIMEOUT   6            //服务器发送时间超过阀值的标志

#define MAX_PACKET_PARSE      5000         //PACKETPARSE对象池个数
#define MAX_MESSAGE_POOL      5000         //Message对象池个数

#define PACKET_HEAD           4            //包头长度
#define BUFFPACKET_MAX_COUNT  5000         //初始化BuffPacket包缓冲池的个数
#define SENDQUEUECOUNT        1            //默认发送线程队列的数量

#define MAX_POSTMESSAGE_SIZE  65536        //最大的PostMessage循环

#define MAX_TIMERMANAGER_THREADCOUNT      1   //Timer管理器的最大线程数

#define PARM_CONNECTHANDLE_CHECK          2   //定时器发送包检测
#define PARM_HANDLE_CLOSE                 1   //定时器句柄关闭

#define HANDLE_ISCLOSE_NO                 0      //连接已经关闭
#define HANDLE_ISCLOSE_YES                1      //连接目前正常

#define TYPE_IPV4                         1      //IPv4标准
#define TYPE_IPV6                         2      //IPv6标准

#define MAX_UDP_PACKET_LEN                1024   //UDP数据包的最大大小
#define UDP_HANDER_ID                     0      //默认UDP的ConnectID

#define PACKET_TCP                        0      //数据包来源类型TCP
#define PACKET_UDP                        1      //数据包来源类型UDP

#define CONNECT_TCP                       0      //链接类型为TCP
#define CONNECT_UDP                       1      //链接类型为UDP

#define SENDMESSAGE_NOMAL                 0      //默认发送数据包模式(走PacketPrase端口)
#define SENDMESSAGE_JAMPNOMAL             1      //发送数据包模式(不走PacketPrase端口)

#define COMMAND_TYPE_IN                   0      //进入服务器命令包状态（用于CommandData，统计命令信息类）
#define COMMAND_TYPE_OUT                  1      //出服务器的命令包状态（用于CommandData，统计命令信息类）

#define PACKET_WITHSTREAM                 0      //不带包头的数据流模式
#define PACKET_WITHHEAD                   1      //带包头的数据包模式

#define PACKET_GET_ENOUGTH                0      //得到完整的数据包，需要继续接收
#define PACKET_GET_NO_ENOUGTH             1      //得到的数据包不完整
#define PACKET_GET_ERROR                  2      //数据包格式错误

#define MAX_PACKET_SIZE     1024*1024            //最大允许的包尺寸

#define PACKET_IS_FRAMEWORK_RECYC         true   //框架回收（数据包回收机制）
#define PACKET_IS_SELF_RECYC              false  //由逻辑自己回收（数据包回收机制）

#define PACKET_SEND_IMMEDIATLY            true   //立刻发送（数据包发送机制）
#define PACKET_SEND_CACHE                 false  //缓存发送（数据包发送机制）

#define CONNECT_LIMIT_RETRY 30                   //初始化中间服务器链接后定期检查，单位是秒

#define TCP_NODELAY_ON      0                    //TCP Nagle算法开关开关打开
#define TCP_NODELAY_OFF     1                    //TCP Nagle算法开关开关关闭

//对应当前框架支持的网络模式
enum
{
	NETWORKMODE_PRO_IOCP    = 1,    //IOCP模式
	NETWORKMODE_RE_SELECT   = 10,   //Select模式
	NETWORKMODE_RE_TPSELECT = 11,   //TPSelect模式 
	NETWORKMODE_RE_EPOLL    = 12,   //epolllt模式(水平触发)
	NETWORKMODE_RE_EPOLL_ET = 13,   //epollet模式(边沿触发)
};

//对应链接的状态，用于设置链接时候的状态
enum
{
	CONNECT_INIT         = 0,
	CONNECT_OPEN         = 1,
	CONNECT_RECVGEGIN    = 2,
	CONNECT_RECVGEND     = 3,
	CONNECT_SENDBEGIN    = 4,
	CONNECT_SENDEND      = 5,
	CONNECT_CLOSEBEGIN   = 6,
	CONNECT_CLOSEEND     = 7,
	CONNECT_RECVERROR    = 8,
	CONNECT_SENDERROR    = 9,
	CONNECT_SENDBUFF     = 10,
	CONNECT_SENDNON      = 11,
	CONNECT_SERVER_CLOSE = 12,
};

//对应处理线程的状态
enum
{
	THREAD_INIT      = 0,   //线程初始化
	THREAD_RUNBEGIN  = 1,   //开始处理线程
	THREAD_RUNEND    = 2,   //处理数据结束
	THREAD_BLOCK     = 3,   //线程阻塞
};

//对应服务器间通讯的传输状态
enum EM_Server_Connect_State
{
	SERVER_CONNECT_READY = 0,
	SERVER_CONNECT_FIRST,
	SERVER_CONNECT_OK,
	SERVER_CONNECT_FAIL,
	SERVER_CONNECT_RECONNECT,
};

//日志编号声明
#define LOG_SYSTEM                      1000
#define LOG_SYSTEM_ERROR                1001
#define LOG_SYSTEM_CONNECT              1002
#define LOG_SYSTEM_WORKTHREAD           1003
#define LOG_SYSTEM_POSTTHREAD           1004
#define LOG_SYSTEM_UDPTHREAD            1005
#define LOG_SYSTEM_POSTCONNECT          1006
#define LOG_SYSTEM_PACKETTIME           1007
#define LOG_SYSTEM_PACKETTHREAD         1008
#define LOG_SYSTEM_CONNECTABNORMAL      1009
#define LOG_SYSTEM_RECVQUEUEERROR       1010
#define LOG_SYSTEM_SENDQUEUEERROR       1011
#define LOG_SYSTEM_COMMANDDATA          1012
#define LOG_SYSTEM_CONSOLEDATA          1013
#define LOG_SYSTEM_DEBUG_CLIENTRECV     1014
#define LOG_SYSTEM_DEBUG_CLIENTSEND     1015
#define LOG_SYSTEM_DEBUG_SERVERRECV     1016
#define LOG_SYSTEM_DEBUG_SERVERSEND     1017
#define LOG_SYSTEM_MONITOR              1018 

#define DEBUG_ON  1
#define DEBUG_OFF 0

#define OUR_DEBUG(X)  ACE_DEBUG((LM_INFO, "[%t]")); ACE_DEBUG(X)

enum
{
	REACTOR_CLIENTDEFINE  = 0,
	REACTOR_POSTDEFINE    = 1,
	REACTOR_UDPDEFINE     = 2,
};

//中间服务器的ID
enum
{
	POSTSERVER_TEST    = 1,
};

//*****************************************************************
//增加两个特殊的命令头，一个是链接建立，一个是链接退出
#define CLIENT_LINK_CONNECT     0x0001      //用户链接
#define CLIENT_LINK_CDISCONNET  0x0002      //客户端退出
#define CLIENT_LINK_SDISCONNET  0x0003      //服务器退出
#define CLINET_LINK_SENDTIMEOUT 0x0004      //服务器发送客户端时间超过阀值
//*****************************************************************

//*****************************************************************

//定义一个内存管理分配器
typedef  ACE_Malloc<ACE_LOCAL_MEMORY_POOL, ACE_SYNCH_MUTEX> MUTEX_MALLOC;
typedef ACE_Allocator_Adapter<MUTEX_MALLOC> Mutex_Allocator;

#ifndef uint8
typedef ACE_UINT8 uint8;
#endif

#ifndef uint16
typedef ACE_UINT16 uint16;
#endif

#ifndef uint32
typedef ACE_UINT32 uint32;
#endif

#ifndef uint64
typedef ACE_UINT64 uint64;
#endif

#ifndef int8
typedef char int8;
#endif

#ifndef int16
typedef short int16;
#endif

#ifndef int32
typedef int int32;
#endif

#ifndef float32
typedef float float32;
#endif

#ifndef float64
typedef double float64;
#endif

#ifdef UNICODE
typedef wofstream _tofstream;
typedef wifstream _tifstream;
typedef std::wstring _tstring;
#else
typedef ofstream _tofstream;
typedef ifstream _tifstream;
typedef std::string _tstring;
#endif // UNICODE

//标记VCHARS_TYPE的模式
enum VCHARS_TYPE
{
	VCHARS_TYPE_TEXT = 0,      //文本模式
	VCHARS_TYPE_BINARY,        //二进制模式
};

#ifndef VCHARS_STR
typedef  struct _VCHARS_STR 
{
	char*       text;            //数据指针
	uint8       u1Len;           //数据长度
	bool        blCopy;          //是否拷贝数据块，True是拷贝，默认是拷贝
	bool        blNew;           //是否是new出来的数据
	VCHARS_TYPE type;            //类型，类型定义见VCHARS_TYPE

	_VCHARS_STR(bool blIsCopy = true, VCHARS_TYPE ntype = VCHARS_TYPE_TEXT)
	{
		text   = NULL;
		u1Len  = 0;
		blCopy = blIsCopy;
		type   = ntype;
		blNew  = false;
	}

	~_VCHARS_STR()
	{
		if(blNew == true)
		{
			delete text;
		}
	}

	void SetData(const char* pData, uint8& u1Length)
	{
		if(blCopy == true)
		{
			//如果是需要构建新内存，则在这里申请
			if(blNew == true)
			{
				delete text;
			}

			if(type == VCHARS_TYPE_TEXT)
			{
				//文本模式
				text = new char[u1Length + 1];
				ACE_OS::memcpy(text, pData, u1Length);
				text[u1Length] = '\0';
				u1Len = u1Length + 1;
			}
			else
			{
				//二进制模式
				text = new char[u1Length];
				ACE_OS::memcpy(text, pData, u1Length);
				u1Len = u1Length;
			}
			blNew = true;
		}
		else
		{
			text  = (char* )pData;
			u1Len = u1Length;
		}
	}

}VCHARS_STR;
#endif

#ifndef VCHARM_STR
typedef  struct _VCHARM_STR 
{
	char*       text;            //数据指针 
	uint16      u2Len;           //数据长度
	bool        blCopy;          //是否拷贝数据块，True是拷贝，默认是拷贝
	bool        blNew;           //是否是new出来的数据
	VCHARS_TYPE type;            //类型，类型定义见VCHARS_TYPE

	_VCHARM_STR(bool blIsCopy = true, VCHARS_TYPE ntype = VCHARS_TYPE_TEXT)
	{
		text   = NULL;
		u2Len  = 0;
		blCopy = blIsCopy;
		type   = ntype;
		blNew  = false;
	}

	~_VCHARM_STR()
	{
		if(blNew == true)
		{
			delete text;
		}
	}

	void SetData(const char* pData, uint16& u2Length)
	{
		if(blCopy == true)
		{
			//如果是需要构建新内存，则在这里申请
			if(blNew == true)
			{
				delete text;
			}

			if(type == VCHARS_TYPE_TEXT)
			{
				//文本模式
				text = new char[u2Length + 1];
				ACE_OS::memcpy(text, pData, u2Length);
				text[u2Length] = '\0';
				u2Len = u2Length + 1;
			}
			else
			{
				//二进制模式
				text = new char[u2Length];
				ACE_OS::memcpy(text, pData, u2Length);
				u2Len = u2Length;
			}
			blNew = true;
		}
		else
		{
			text  = (char* )pData;
			u2Len = u2Length;
		}
	}

}VCHARM_STR;
#endif

#ifndef VCHARB_STR
typedef  struct _VCHARB_STR 
{
	char*       text;            //数据指针 
	uint32      u4Len;           //数据长度
	bool        blCopy;          //是否拷贝数据块，True是拷贝，默认是拷贝
	bool        blNew;           //是否是new出来的数据
	VCHARS_TYPE type;            //类型，类型定义见VCHARS_TYPE

	_VCHARB_STR(bool blIsCopy = true, VCHARS_TYPE ntype = VCHARS_TYPE_TEXT)
	{
		text   = NULL;
		u4Len  = 0;
		blCopy = blIsCopy;
		type   = ntype;
		blNew  = false;
	}

	~_VCHARB_STR()
	{
		if(blNew == true)
		{
			delete text;
		}
	}

	void SetData(const char* pData, uint32& u4Length)
	{
		if(blCopy == true)
		{
			//如果是需要构建新内存，则在这里申请
			if(blNew == true)
			{
				delete text;
			}

			if(type == VCHARS_TYPE_TEXT)
			{
				//文本模式
				text = new char[u4Length + 1];
				ACE_OS::memcpy(text, pData, u4Length);
				text[u4Length] = '\0';
				u4Len = u4Length + 1;
			}
			else
			{
				//二进制模式
				text = new char[u4Length];
				ACE_OS::memcpy(text, pData, u4Length);
				u4Len = u4Length;
			}
			blNew = true;
		}
		else
		{
			text  = (char* )pData;
			u4Len = u4Length;
		}
	}

}VCHARB_STR;
#endif

//用于记录数据包头信息
#define SESSION_LEN           32     //Session的大小
struct _PacketHeadInfo
{
	uint16 m_u2Version;              //协议版本号
	uint16 m_u2CmdID;                //协议命令字   
	uint32 m_u4BodyLen;              //协议数据包体长度  
	char   m_szSession[SESSION_LEN]; //数据包Session 

	void Clear()
	{
		m_u2Version    = 0;
		m_u2CmdID      = 0;
		m_u4BodyLen    = 0;
		m_szSession[0] = '\0';
	}
};

//定时监控数据包和流量的数据信息，用于链接有效性的逻辑判定
struct _TimeConnectInfo
{
	uint8  m_u1Minutes;               //当前的分钟数
	uint32 m_u4RecvPacketCount;       //当前接收包数量
	uint32 m_u4RecvSize;              //当前接收数据量
	uint32 m_u4SendPacketCount;       //当前发送包数量
	uint32 m_u4SendSize;              //当前发送数据量

	uint32 m_u4ValidRecvPacketCount;  //单位时间可允许接收数据包的上限
	uint32 m_u4ValidRecvSize;         //单位时间可允许的数据接收量
	uint32 m_u4ValidSendPacketCount;  //单位时间可允许数据数据包的上限
	uint32 m_u4ValidSendSize;         //单位时间可允许的数据发送量

	_TimeConnectInfo()
	{ 
		m_u1Minutes              = 0;
		m_u4RecvPacketCount      = 0;
		m_u4RecvSize             = 0;
		m_u4SendPacketCount      = 0;
		m_u4SendSize             = 0;

		m_u4ValidRecvPacketCount = 0;
		m_u4ValidRecvSize        = 0;
		m_u4ValidSendPacketCount = 0;
		m_u4ValidSendSize        = 0;
	}

	void Init(uint32 u4RecvPacketCount, uint32 u4RecvSize, uint32 u4SendPacketCount, uint32 u4ValidSendSize)
	{
		m_u1Minutes              = 0;
		m_u4RecvPacketCount      = 0;
		m_u4RecvSize             = 0;
		m_u4SendPacketCount      = 0;
		m_u4SendSize             = 0;

		m_u4ValidRecvPacketCount = u4RecvPacketCount;
		m_u4ValidRecvSize        = u4RecvSize;
		m_u4ValidSendPacketCount = u4SendPacketCount;
		m_u4ValidSendSize        = u4ValidSendSize;
	}

	bool RecvCheck(uint8 u1Minutes, uint16 u2PacketCount, uint32 u4RecvSize)
	{
		if(m_u1Minutes != u1Minutes)
		{
			m_u1Minutes         = u1Minutes;
			m_u4RecvPacketCount = u2PacketCount;
			m_u4RecvSize        = u4RecvSize;
		}
		else
		{
			m_u4RecvPacketCount += u2PacketCount;
			m_u4RecvSize        += u4RecvSize;
		}

		if(m_u4ValidRecvPacketCount > 0)
		{
			//需要比较
			if(m_u4RecvPacketCount > m_u4ValidRecvPacketCount)
			{
				return false;
			}
		}

		if(m_u4ValidRecvSize > 0)
		{
			//需要比较
			if(u4RecvSize > m_u4ValidRecvSize)
			{
				return false;
			}
		}

		return true;
	}

	bool SendCheck(uint8 u1Minutes, uint16 u2PacketCount, uint32 u4SendSize)
	{
		if(m_u1Minutes != u1Minutes)
		{
			m_u1Minutes         = u1Minutes;
			m_u4SendPacketCount = u2PacketCount;
			m_u4RecvSize        = u4SendSize;
		}
		else
		{
			m_u4SendPacketCount += u2PacketCount;
			m_u4RecvSize        += u4SendSize;
		}

		if(m_u4ValidSendPacketCount > 0)
		{
			//需要比较
			if(m_u4SendPacketCount > m_u4ValidSendPacketCount)
			{
				return false;
			}
		}

		if(m_u4ValidSendSize > 0)
		{
			//需要比较
			if(u4SendSize > m_u4ValidSendSize)
			{
				return false;
			}
		}

		return true;
	}
};

//定时器参数的设置结构
struct _TimerCheckID 
{
	uint16 m_u2TimerCheckID;

	_TimerCheckID()
	{
		m_u2TimerCheckID = 0;
	}
};

//Message里面数据块结构体
struct _PacketInfo
{
	char*   m_pData;            //解析后的数据头指针
	int     m_nDataLen;         //解析后的数据长度

	_PacketInfo()
	{
		m_pData       = NULL;
		m_nDataLen    = 0;
	}
};

//客户端链接信息数据结构
struct _ClientConnectInfo
{
	bool          m_blValid;              //当前链接是否有效
	uint32        m_u4ConnectID;          //链接ID
	ACE_INET_Addr m_addrRemote;           //远程链接地址
	uint32        m_u4RecvCount;          //接收包数量
	uint32        m_u4SendCount;          //发送包数量
	uint32        m_u4AllRecvSize;        //接收字节数
	uint32        m_u4AllSendSize;        //发送字节数
	uint32        m_u4BeginTime;          //链接建立时间
	uint32        m_u4AliveTime;          //存活时间秒数
	uint32        m_u4RecvQueueCount;     //接受逻辑处理包的个数
	uint64        m_u8RecvQueueTimeCost;  //接受逻辑处理包总时间消耗
	uint64        m_u8SendQueueTimeCost;  //发送数据总时间消耗

	_ClientConnectInfo()
	{
		m_blValid             = false;
		m_u4ConnectID         = 0;
		m_u4RecvCount         = 0;
		m_u4SendCount         = 0;
		m_u4AllRecvSize       = 0;
		m_u4AllSendSize       = 0;
		m_u4BeginTime         = 0;
		m_u4AliveTime         = 0;
		m_u4RecvQueueCount    = 0;
		m_u8RecvQueueTimeCost = 0;
		m_u8SendQueueTimeCost = 0;
	}

	_ClientConnectInfo& operator = (const _ClientConnectInfo& ar)
	{
		this->m_blValid             = ar.m_blValid;
		this->m_u4ConnectID         = ar.m_u4ConnectID;
		this->m_addrRemote          = ar.m_addrRemote;
		this->m_u4RecvCount         = ar.m_u4RecvCount;
		this->m_u4SendCount         = ar.m_u4SendCount;
		this->m_u4AllRecvSize       = ar.m_u4AllRecvSize;
		this->m_u4AllSendSize       = ar.m_u4AllSendSize;
		this->m_u4BeginTime         = ar.m_u4BeginTime;
		this->m_u4AliveTime         = ar.m_u4AliveTime;
		this->m_u4RecvQueueCount    = ar.m_u4RecvQueueCount;
		this->m_u8RecvQueueTimeCost = ar.m_u8RecvQueueTimeCost;
		this->m_u8SendQueueTimeCost = ar.m_u8SendQueueTimeCost;
		return *this;
	}
};
typedef vector<_ClientConnectInfo> vecClientConnectInfo;

//要连接的服务器信息
struct _ServerConnectInfo
{
	uint32      m_u4ServerID;     //服务器的ID
	ACE_TString m_strServerName;  //服务器的名称
	ACE_TString m_strServerIP;    //服务器的IP
	uint32      m_u4ServerPort;   //服务器的端口
	uint32      m_u4MaxConn;      //服务器的最大线程连接数
	uint32      m_u4TimeOut;      //服务器的链接超时时间

	_ServerConnectInfo()
	{
		m_u4ServerID   = 0;
		m_u4ServerPort = 0;
		m_u4MaxConn    = 0;
		m_u4TimeOut    = 0;
	}
};

#ifndef SAFE_DELETE
#define SAFE_DELETE(x) if( (x) != NULL ) { delete (x); (x) = NULL; }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) if( (x) != NULL ) { delete[] (x); (x) = NULL; }
#endif

//定义一个函数，可以支持内存越界检查
inline void sprintf_safe(char* szText, int nLen, const char* fmt ...)
{
	if(szText == NULL)
	{
		return;
	}

	va_list ap;
	va_start(ap, fmt);

	ACE_OS::vsnprintf(szText, nLen, fmt, ap);
	szText[nLen - 1] = '\0';

	va_end(ap);
};

//为逻辑块提供一个Try catch的保护宏，用于调试，具体使用方法请参看TestTcp用例
//目前最多支持一条2K的日志
//************************************************************************
#define ASSERT_LOG_PATH  "./Log/assert.log"   //如果路径想自己要，请修改这里。

inline void __show__( const char* szTemp)
{
#ifdef WIN32
	printf_s("[__show__]%s.\n", szTemp);
#else
	printf("[__show__]%s.\n", szTemp);
#endif

	FILE* f = ACE_OS::fopen(ASSERT_LOG_PATH, "a") ;
	ACE_OS::fwrite( szTemp, strlen(szTemp), sizeof(char), f) ;
	ACE_OS::fwrite( "\r\n", 1, 2*sizeof(char), f);
	fclose(f);
};

inline void __assertspecial__(const char* file, int line, const char* func, const char* expr, const char* msg)
{
	char szTemp[2*MAX_BUFF_1024] = {0};

	sprintf_safe( szTemp, 2*MAX_BUFF_1024, "Alert[%s][%d][%s][%s][%s]", file, line, func, expr ,msg) ;
	__show__(szTemp) ;
};

#if defined(WIN32)
#define AssertSpecial(expr,msg) ((void)((expr)?0:(__assertspecial__(__FILE__, __LINE__, __FUNCTION__, #expr, msg),0)))
#else
#define AssertSpecial(expr,msg) {if(!(expr)){__assertspecial__(__FILE__, __LINE__, __PRETTY_FUNCTION__, #expr, msg);}}
#endif

#if defined(WIN32)
#define __ENTER_FUNCTION() {try{
#define __THROW_FUNCTION(msg) throw(msg)
#define __LEAVE_FUNCTION() }catch(char* msg){AssertSpecial(false,msg); }}
#define __LEAVE_FUNCTION_WITHRETURN(ret) }catch(char* msg){AssertSpecial(false,msg); return ret; }}
#else	//linux
#define __ENTER_FUNCTION() {try{
#define __THROW_FUNCTION(msg) throw(msg)
#define __LEAVE_FUNCTION() }catch(char* msg){AssertSpecial(false,msg);}}
#define __LEAVE_FUNCTION_WITHRETURN(ret) }catch(char* msg){AssertSpecial(false,msg); return ret; }}
#endif 

//************************************************************************

//增加一个统计函数处理时间的宏，这个宏可以直接统计当前函数执行时间
//使用例子 __TIMECOST(100); 100为毫秒，超过100毫秒就会计入日志
//************************************************************************
#define ASSERT_TIME_PATH  "./Log/FuncTimeout.log"   //如果路径想自己要，请修改这里。
class CTimeCost
{
public:
	CTimeCost(int nMillionSecond, const char* pFunctionName, const char* pFileName, int nLine) 
	{
		m_nMillionSecond = nMillionSecond;
		sprintf_safe(m_szFunctionName, MAX_BUFF_100, "%s", pFunctionName);
		sprintf_safe(m_szFileName, MAX_BUFF_300, "%s", pFileName);
		m_nFileLine = nLine;
		TimeBegin();
	};

	~CTimeCost()
	{
		TimeEnd();
	};

	void TimeBegin()
	{
		m_lBegin = GetSystemTickCount();
	};

	void TimeEnd()
	{
		m_lEnd = GetSystemTickCount();
		long lTimeInterval = m_lEnd - m_lBegin;  //转换成毫秒
		if(lTimeInterval >= (long)m_nMillionSecond)
		{
			char szLog[MAX_BUFF_1024];
			//记录日志
			FILE* pFile = ACE_OS::fopen(ASSERT_TIME_PATH, "a+");
			if(pFile != NULL)
			{
				char szTimeNow[MAX_BUFF_50];
				time_t tNow = time(NULL);
				struct tm* tmNow = ACE_OS::localtime(&tNow);
				sprintf_safe(szTimeNow, MAX_BUFF_50, "%04d-%02d-%02d %02d:%02d:%02d", tmNow->tm_year + 1900, tmNow->tm_mon + 1, tmNow->tm_mday, tmNow->tm_hour, tmNow->tm_min, tmNow->tm_sec);
				sprintf_safe(szLog, MAX_BUFF_1024, "[%s]dbTimeInterval more than (%d) < (%d), File(%s):FunName(%s):Line(%d).\n", szTimeNow, m_nMillionSecond, lTimeInterval, m_szFileName, m_szFunctionName, m_nFileLine);
				ACE_OS::fwrite(szLog, strlen(szLog), sizeof(char), pFile);
				ACE_OS::fclose(pFile);
			}
		}
	};

private:
	unsigned long GetSystemTickCount()
	{
#ifdef WIN32
		return GetTickCount();
#else
		struct timespec ts;

		clock_gettime(CLOCK_MONOTONIC, &ts);

		return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
#endif
	}

private:
	long         m_lBegin;
	long         m_lEnd;
	unsigned int m_nMillionSecond;
	char         m_szFunctionName[MAX_BUFF_100];
	char         m_szFileName[MAX_BUFF_300];
	int          m_nFileLine;
};

#define __TIMECOST(cost) CTimeCost timecost(cost, __FUNCTION__, __FILE__, __LINE__);

//************************************************************************
//add by freeeyes
//获得对自己大小端的自我判定
//添加在mainConfig中，是因为没必要在define.h中判定
//在框架里自己判定是否转换大小端
enum
{
	O32_LITTLE_ENDIAN = 0x03020100ul,
	O32_BIG_ENDIAN = 0x00010203ul,
	O32_PDP_ENDIAN = 0x01000302ul
};

static const union { unsigned char bytes[4]; uint32 value; } o32_host_order =
{ { 0, 1, 2, 3 } };

#define O32_HOST_ORDER (o32_host_order.value)

//定义一个对64位长整形的网络字节序的转换
inline uint64 hl64ton(uint64 u8Data)   
{   
	if(O32_HOST_ORDER == O32_LITTLE_ENDIAN)
	{
		union { uint32 lv[2]; uint64 llv; } u;  
		u.lv[0] = htonl(u8Data >> 32);  
		u.lv[1] = htonl(u8Data & 0xFFFFFFFFULL);  
		return u.llv;   
	}
	else
	{
		//如果本身是大端就不理
		return u8Data;
	}
  
}

//定义一个队64位长整形的主机字节序的转换
inline uint64 ntohl64(uint64 u8Data)   
{   
	if(O32_HOST_ORDER == O32_LITTLE_ENDIAN)
	{
		union { uint32 lv[2]; uint64 llv; } u;  
		u.llv = u8Data;  
		return ((uint64)ntohl(u.lv[0]) << 32) | (uint64)ntohl(u.lv[1]);
	}
	else
	{
		//如果本身是大端就不理
		return u8Data;
	}
}

//定义一个函数，可支持字符串替换，目前先不考虑支持中文
inline bool Replace_String(char* pText, uint32 u4Len, const char* pOld, const char* pNew)
{
	char* pTempSrc = new char(u4Len);

	ACE_OS::memcpy(pTempSrc, pText, u4Len);
	pTempSrc[u4Len - 1] = '\0';

	uint16 u2NewLen = (uint16)ACE_OS::strlen(pNew);

	char* pPos = ACE_OS::strstr(pTempSrc, pOld); 

	while(pPos)
	{
		//计算出需要覆盖的字符串长度
		uint32 u4PosLen = (uint32)(pPos - pTempSrc);

		//黏贴最前面的
		ACE_OS::memcpy(pText, pTempSrc, u4PosLen);
		pText[u4PosLen] = '\0';

		if(u4PosLen + u2NewLen >= (uint32)u4Len)
		{
			//清理中间变量
			delete[] pTempSrc;
			return false;		
		}
		else
		{
			//黏贴新字符
			ACE_OS::memcpy(&pText[u4PosLen], pNew, u2NewLen);
			pText[u4PosLen + u2NewLen] = '\0';

			//指针向后移动	
			pTempSrc = 	pTempSrc + u4PosLen;

			//寻找下一个透汗的字符串
			pPos = ACE_OS::strstr(pTempSrc, pOld); 
		}

	}

	//清理中间变量
	delete[] pTempSrc;
	return true;
}

//客户端IP信息
struct _ClientIPInfo
{
	char m_szClientIP[MAX_BUFF_50];   //客户端的IP地址
	int  m_nPort;                     //客户端的端口

	_ClientIPInfo()
	{
		m_szClientIP[0] = '\0';
		m_nPort         = 0;
	}

	_ClientIPInfo& operator = (const _ClientIPInfo& ar)
	{
		sprintf_safe(this->m_szClientIP, MAX_BUFF_50, "%s", ar.m_szClientIP);
		this->m_nPort = ar.m_nPort;
		return *this;
	}
};

//链接别名映射信息(用于PSS_ClientManager管理)
struct _ClientNameInfo
{
	char m_szName[MAX_BUFF_100];      //连接别名 
	char m_szClientIP[MAX_BUFF_50];   //客户端的IP地址
	int  m_nPort;                     //客户端的端口
	int  m_nConnectID;                //连接ID  
	int  m_nLog;                      //是否记录日志

	_ClientNameInfo()
	{
		m_szName[0]     = '\0';
		m_szClientIP[0] = '\0';
		m_nPort         = 0;
		m_nConnectID    = 0;
		m_nLog          = 0;
	}

	_ClientNameInfo& operator = (const _ClientNameInfo& ar)
	{
		sprintf_safe(this->m_szName, MAX_BUFF_100, "%s", ar.m_szName);
		sprintf_safe(this->m_szClientIP, MAX_BUFF_50, "%s", ar.m_szClientIP);
		this->m_nPort      = ar.m_nPort;
		this->m_nConnectID = ar.m_nConnectID;
		this->m_nLog       = ar.m_nLog;
		return *this;
	}
};
typedef vector<_ClientNameInfo> vecClientNameInfo;

struct _ServerInfo
{
	char  m_szServerName[MAX_BUFF_50];
	char  m_szServerIP[MAX_BUFF_50];
	int   m_nPort;
	uint8 m_u1IPType;
	char  m_szKey[MAX_BUFF_50];

	_ServerInfo()
	{
		m_szServerName[0] = '\0';
		m_szServerIP[0]   = '\0';
		m_nPort           = 0;
		m_u1IPType        = TYPE_IPV4;
		m_szKey[0]        = '\0';
	}
};

//邮件配置相关信息
struct _MailAlert
{
	uint32 m_u4MailID;
	char   m_szFromMailAddr[MAX_BUFF_200];
	char   m_szToMailAddr[MAX_BUFF_200];
	char   m_szMailPass[MAX_BUFF_200];
	char   m_szMailUrl[MAX_BUFF_200];
	uint32 m_u4MailPort;

	_MailAlert()
	{
		m_u4MailID          = 0;
		m_szFromMailAddr[0] = '\0';
		m_szToMailAddr[0]   = '\0';
		m_szMailPass[0]     = '\0';
		m_szMailUrl[0]      = '\0';
		m_u4MailPort        = 0;
	}
};

typedef vector<_ServerInfo> vecServerInfo;

#endif
