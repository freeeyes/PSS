#ifndef _DEFINE_H
#define _DEFINE_H

// Define.h
// ���ﶨ�����з������õ���ͨ�õĶ��������������һЩace��ͷ��ͨ�õ�һЩ���ݷ���
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
#include "ace/OS_NS_string.h"
#include "ace/High_Res_Timer.h"
#include "ace/INET_Addr.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Date_Time.h"
#include <math.h>
#include <type_traits>
#include <fstream>

#ifndef WIN32
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#endif

//����ʹ�õ������ĸ�ʽ������
#define FMT_HEADER_ONLY
#include "fmt/format.h"

#include <memory>
#include <vector>

#include "singleton.h"

using namespace std;

#define NAMESPACE PSS
#define BEGIN_NAMESPACE namespace NAMESPACE{
#define END_NAMESPACE }
#define USING_NAMESPACE using namespace NAMESPACE;

BEGIN_NAMESPACE

//�������Ͷ���
using uint8   = uint8_t;
using uint16  = uint16_t;
using uint32  = uint32_t;
using uint64  = uint64_t;
using int8    = int8_t;
using int16   = int16_t;
using int32   = int32_t;
using int64   = int64_t;
using float32 = float;
using float64 = double;

#define PSS_UNUSED_ARG(a) do {/* null */} while (&a == 0)

//�Զ��ж�����ϵͳ
#define PLATFORM_WIN     0
#define PLATFORM_UNIX    1
#define PLATFORM_APPLE   2

#if defined(__WIN32__) || defined(WIN32) || defined(_WIN32) || defined(__WIN64__) || defined(WIN64) || defined(_WIN64)
#  define PSS_PLATFORM PLATFORM_WIN
#elif defined(__APPLE_CC__)
#  define PSS_PLATFORM PLATFORM_APPLE
#else
#  define PSS_PLATFORM PLATFORM_UNIX
#endif

#define SET_PROFILE_OUTPUT 1;   //�Ƿ�����ܼ��ѡ��

static const char MAINCONFIG[]       = "main.xml";
static const char ALERTCONFIG[]      = "alert.xml";
static const char FORBIDDENIP_FILE[] = "forbiddenIP.xml";
static const char CONSOLECONFIG[]    = "ConsoleCommand.xml";

const uint16 MAX_BUFF_9     = 9;
const uint16 MAX_BUFF_20    = 20;
const uint16 MAX_BUFF_50    = 50;
const uint16 MAX_BUFF_100   = 100;
const uint16 MAX_BUFF_200   = 200;
const uint16 MAX_BUFF_300   = 300;
const uint16 MAX_BUFF_500   = 500;
const uint16 MAX_BUFF_1000  = 1000;
const uint16 MAX_BUFF_1024  = 1024;
const uint16 MAX_BUFF_10240 = 10240;

#define THREAD_PARAM THR_NEW_LWP | THR_BOUND | THR_DETACHED

/*
//���㵱ǰ�汾���Ƿ����ƶ��汾��һ��
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
const uint16 CONSOLE_PACKET_MAX_SIZE = 200; //����console�����ݰ���С

//���ݲ�ͬ�Ĳ���ϵͳ�����岻ͬ��recv���ղ�������
#if PSS_PLATFORM == PLATFORM_WIN
const uint8 MSG_NOSIGNAL = 0;            //�ź���������WINDOWS��
#endif

const uint8 SERVER_ACTOR_REACTOR  = 0;
const uint8 SERVER_ACTOR_PROACTOR = 1;

const uint8 SERVER_ACTOR_TYPE = SERVER_ACTOR_PROACTOR;         //��ǰ���������õļܹ����ͣ�������Proactor��Ҳ������Reactor

const uint16 MAX_DEV_POLL_COUNT = 5000;         //Devpoll�������ӵ�������

const uint8 CONSOLE_ENABLE  = 1;            //�򿪺�̨�˿�
const uint8 CONSOLE_DISABLE = 0;            //�رպ�̨�˿�

const uint8 CONNECT_CLOSE_TIME = 2;         //���ӻ���ر�ʱ���ӳ�

const uint16 MAX_MSG_THREADCOUNT = 1;            //Ĭ�ϵ��߼��̴߳�����
const uint32 MAX_MSG_THREADQUEUE = 60000;        //�����̵߳���Ϣ������
const uint16 MAX_MSG_TIMEDELAYTIME = 60;           //�߼��߳��Լ�ʱ����
const uint8  MAX_MSG_STARTTIME = 1;            //�߼��̴߳���ʼʱ��
const uint32 MAX_MSG_MASK = 64000;        //�߼�Mark���߳���
const uint16 MAX_MSG_PUTTIMEOUT = 100;          //�����߼����ӳ�
const uint16 MAX_MSG_SENDPACKET = 10;           //��໺�巢�Ͱ��ĸ���,����������ֱ�����������һ���������ݰ�
const uint16 MAX_MSG_SNEDTHRESHOLD = 10;           //���ͷ�ֵ(��Ϣ���ĸ���)
const uint16 MAX_MSG_SENDCHECKTIME = 100;          //ÿ�����ٺ������һ�η��͵ķ�ֵ
const uint16 MAX_MSG_THREADTIMEOUT = 30;           //�߳��ж���ʱ
const uint16 MAX_MSG_PACKETTIMEOUT = 5;            //�������ݰ���ʱʱ��
const uint32 MAX_MSG_SOCKETBUFF = 50 * 1024;      //�������ݻ����С
const uint16 MAX_MSG_SENDTIMEOUT = 5;            //���ͳ�ʱʱ��
const uint16 MAX_MSG_HANDLETIME = 120;          //ͳ�Ƶ�ǰ�������ļ��
const uint16 MAX_MSG_SENDALIVETIME = 60;           //���ʹ�����ʱ����
const uint16 MAX_MSG_SENDALCOUNT = 5;            //�ж������Ƿ���ļ���
const uint32 MAX_MSG_PACKETLENGTH = 20 * 1024;      //���һ���ݰ��������� 
const uint16 MAX_MP_POOL = 5000;         //_MakePacket����������Ĵ�С
const uint16 MAX_HANDLE_POOL = 1000;         //Ĭ��Handler����صĴ�С
const uint16 MAX_ASYNCH_BACKLOG = 100;          //Ĭ�����õ�BACKLOG����
const uint16 MAX_LOADMODEL_CLOSE = 5;            //Ĭ�ϵȴ�ģ��ر�ʱ��
const uint16 MAX_CONNECT_COUNT = 10000;        //Ĭ�ϵ�λʱ�����������������
const uint16 MAX_BLOCK_SIZE = 2048;         //Ĭ�������������ݿ�Ĵ�С
const uint16 MAX_BLOCK_COUNT = 3;            //Ĭ������Block����
const uint16 MAX_BLOCK_TIME = 1;            //Ĭ�ϵȴ��ط�ʱ�䣨��λ���룩
const uint16 MAX_QUEUE_TIMEOUT = 20;           //Ĭ�϶��г�ʱ����ʱ��
const uint16 MAX_RECV_UDP_TIMEOUT = 3;            //�����ճ�ʱʱ��(UDP) 

const uint8 PACKET_PARSE = 1;            //��Ϣ�������־
const uint8 PACKET_CONNECT = 2;            //���ӽ����¼���Ϣ��־
const uint8 PACKET_CDISCONNECT = 3;            //�ͻ��˶Ͽ��¼���Ϣ��־
const uint8 PACKET_SDISCONNECT = 4;            //�������Ͽ��¼���Ϣ��־
const uint8 PACKET_SEND_ERROR = 5;            //���ݷ���ʧ���¼� 
const uint8 PACKET_SEND_TIMEOUT = 6;            //����������ʱ�䳬����ֵ�ı�־
const uint8 PACKET_CHEK_TIMEOUT = 7;            //������������ⳬʱ�¼�
const uint8 PACKET_SEND_OK = 8;            //���ͳɹ���ִ
const uint8 PACKET_TTY_CONNECT = 9;            //TTy�豸����
const uint8 PACKET_TTY_DISCONNECT = 10;           //TTy�豸�Ͽ�����
const uint8 PACKET_SERVER_TCP_CONNECT = 11;       //��������ͨѶTcp���ӽ���
const uint8 PACKET_SERVER_TCP_DISCONNECT = 12;    //��������ͨѶTcp���ӶϿ�
const uint8 PACKET_SERVER_UDP_CONNECT = 13;       //��������ͨѶTcp���ӽ���
const uint8 PACKET_SERVER_UDP_DISCONNECT = 14;    //��������ͨѶTcp���ӶϿ�

const uint16 MAX_PACKET_PARSE = 5000;         //PACKETPARSE����ظ���
const uint16 MAX_MESSAGE_POOL = 5000;         //Message����ظ���

const uint16 BUFFPACKET_MAX_COUNT = 5000;         //��ʼ��BuffPacket������صĸ���
const uint16 SENDQUEUECOUNT       = 1;            //Ĭ�Ϸ����̶߳��е�����

const uint32 MAX_POSTMESSAGE_SIZE = 65536;        //����PostMessageѭ��

const uint16 MAX_TIMERMANAGER_THREADCOUNT = 1;   //Timer������������߳���

const uint16 PARM_CONNECTHANDLE_CHECK = 2;   //��ʱ�����Ͱ����
const uint16 PARM_HANDLE_CLOSE = 1;   //��ʱ������ر�

const uint16 HANDLE_ISCLOSE_NO = 0;      //�����Ѿ��ر�
const uint16 HANDLE_ISCLOSE_YES = 1;      //����Ŀǰ����

const uint8 TYPE_IPV4 = 1;      //IPv4��׼
const uint8 TYPE_IPV6 = 2;      //IPv6��׼

const uint16 MAX_UDP_PACKET_LEN = 1024;   //UDP���ݰ�������С
const uint16 UDP_HANDER_ID = 0;      //Ĭ��UDP��ConnectID

const uint16 COMMAND_TYPE_IN = 0;      //��������������״̬������CommandData��ͳ��������Ϣ�ࣩ
const uint16 COMMAND_TYPE_OUT = 1;      //���������������״̬������CommandData��ͳ��������Ϣ�ࣩ

const uint16 PACKET_WITHSTREAM = 0;      //������ͷ��������ģʽ
const uint16 PACKET_WITHHEAD = 1;      //����ͷ�����ݰ�ģʽ

const uint16 PACKET_GET_ENOUGH = 0;      //�õ����������ݰ�����Ҫ��������
const uint16 PACKET_GET_NO_ENOUGH = 1;      //�õ������ݰ�������
const uint16 PACKET_GET_ERROR = 2;      //���ݰ���ʽ����

const uint32 MAX_PACKET_SIZE = 1024 * 1024;            //�������İ��ߴ�

const bool PACKET_IS_FRAMEWORK_RECYC = true;   //��ܻ��գ����ݰ����ջ��ƣ�
const bool PACKET_IS_SELF_RECYC = false;  //���߼��Լ����գ����ݰ����ջ��ƣ�

const uint16 CONNECT_LIMIT_RETRY = 30;                   //��ʼ���м���������Ӻ��ڼ�飬��λ����

const uint8 TCP_NODELAY_ON = 0;                    //TCP Nagle�㷨���ؿ��ش�
const uint8 TCP_NODELAY_OFF = 1;                    //TCP Nagle�㷨���ؿ��عر�

//���json������
#define OUTPUT_CHART_JSON_X "{\"type\": \"category\",\"data\": [%s]}\n"
#define OUTPUT_CHART_JSON_Y "{\"name\":\"%d\",\"type\": \"line\",\"smooth\": \"true\",\"data\": [%s]}\n"
#define OUTPUT_CHART_JSON "{\"title\": {\"text\": \"%s\"},\"tooltip\" : {}, \
\"legend\" : {\"data\":[\"time\"]},\"xAxis\" : %s,\"yAxis\" : {\"type\":\"value\"}, \"series\": [%s]}"

//������Դ
enum class EM_PACKET_RESOURCE
{
    PACKET_RESOURCE_FROM_CLIENT = 0,      //�ͻ������ݰ�
    PACKET_RESOURCE_FROM_SERVER,          //�����������ݰ�
};

//���ӵ���������
enum class EM_CONNECT_IO_TYPE
{
    CONNECT_IO_TCP = 0,      //IO��TCP
    CONNECT_IO_UDP,          //IO��UDP
    CONNECT_IO_TTY,          //IO��TTY
    CONNECT_IO_SERVER_TCP,   //IO�Ƿ����ڼ�����TCP
    CONNECT_IO_SERVER_UDP,   //IO�Ƿ����ڼ�����UDP
    CONNECT_IO_FRAME,        //���Բ����Ļص�
    COMMAND_UPDATE,          //���Բ������  
    WORKTHREAD_CLOSE         //�رյ�ǰ�����߳�
};

//��������ͨѶ��TTyͨѶ�Ĵ���ģʽ������ѡ���ǿ�ܴ�����ҵ����
enum class EM_CONNECT_IO_DISPOSE
{
    CONNECT_IO_FRAME = 0,     //���PacketParse����
    CONNECT_IO_PLUGIN         //����Լ�����
};

//UDP�Ĳ���ģʽ
enum class EM_UDP_TYPE
{
    UDP_SINGLE = 0,        //UDP��������
    UDP_BROADCAST,         //UDP�㲥
};

//��Handler��������������͵Ķ���
enum class EM_IO_TYPE
{
    NET_INPUT = 0,      //�������
    FILE_INPUT,         //�ļ����
};

//��Ӧ��ǰ���֧�ֵ�����ģʽ
enum class NETWORKMODE
{
    NETWORKMODE_PRO_IOCP    = 1,    //IOCPģʽ
    NETWORKMODE_RE_SELECT   = 10,   //Selectģʽ
    NETWORKMODE_RE_TPSELECT = 11,   //TPSelectģʽ
    NETWORKMODE_RE_EPOLL    = 12,   //epollltģʽ(ˮƽ����)
    NETWORKMODE_RE_EPOLL_ET = 13,   //epolletģʽ(���ش���)
};

//��Ӧ���ӵ�״̬��������������ʱ���״̬
enum class CONNECTSTATE
{
    CONNECT_INIT         = 0,
    CONNECT_OPEN         = 1,
    CONNECT_RECVGEGIN    = 2,
    CONNECT_RECVGEND     = 3,
    CONNECT_SENDBEGIN    = 4,
    CONNECT_SENDEND      = 5,
    CONNECT_SERVER_CLOSE = 6,
    CONNECT_CLIENT_CLOSE = 7,
    CONNECT_RECVERROR    = 8,
    CONNECT_SENDERROR    = 9,
    CONNECT_SENDBUFF     = 10,
    CONNECT_SENDNON      = 11,
};

//��������ͨѶ���Ƿ���Ҫ�ص���ö��
enum class EM_s2s
{
    S2S_NEED_CALLBACK = 0,    //��Ҫ�ص�
    S2S_INNEED_CALLBACK,      //����Ҫ�ص�
};

//��Ӧ�����̵߳�״̬
enum class THREADSTATE
{
    THREAD_INIT      = 0,   //�̳߳�ʼ��
    THREAD_RUNBEGIN  = 1,   //��ʼ�����߳�
    THREAD_RUNEND    = 2,   //�������ݽ���
    THREAD_BLOCK     = 3,   //�߳�����
};

//��Ӧ��������ͨѶ�Ĵ���״̬
enum class EM_Server_Connect_State
{
    SERVER_CONNECT_READY = 0,
    SERVER_CONNECT_FIRST,
    SERVER_CONNECT_OK,
    SERVER_CONNECT_FAIL,
    SERVER_CONNECT_RECONNECT,
};

//��������ͨѶ���ݽ���״̬
const uint16 SERVER_RECV_TIMEOUT = 20;   //��������������ݳ�ʱʱ��
enum class EM_Server_Recv_State
{
    SERVER_RECV_INIT = 0,     //δ��������
    SERVER_RECV_BEGIN,        //�����������
    SERVER_RECV_END,          //�����������
};

//�����ļ����Խ���Ķ���
enum class FILE_TEST_RESULT
{
	RESULT_ERR_UNKOWN = -1,   //δ֪����
	RESULT_OK = 0,            //�����ɹ�
	RESULT_ERR_TESTING,       //���ڲ���
	RESULT_ERR_CFGFILE,       //�����ļ�����
	RESULT_ERR_PROFILE,       //Э���ļ�����
};

enum class EM_SEND_PACKET_PARSE
{
    EM_SENDMESSAGE_NOMAL = 0,      //Ĭ�Ϸ������ݰ�ģʽ(��PacketPrase�˿�)
    EM_SENDMESSAGE_JAMPNOMAL,      //�������ݰ�ģʽ(����PacketPrase�˿�)
};

//��־�������
const int LOG_SYSTEM                  = 1000;
const int LOG_SYSTEM_ERROR            = 1001;
const int LOG_SYSTEM_CONNECT          = 1002;
const int LOG_SYSTEM_WORKTHREAD       = 1003;
const int LOG_SYSTEM_POSTTHREAD       = 1004;
const int LOG_SYSTEM_UDPTHREAD        = 1005;
const int LOG_SYSTEM_POSTCONNECT      = 1006;
const int LOG_SYSTEM_PACKETTIME       = 1007;
const int LOG_SYSTEM_PACKETTHREAD     = 1008;
const int LOG_SYSTEM_CONNECTABNORMAL  = 1009;
const int LOG_SYSTEM_RECVQUEUEERROR   = 1010;
const int LOG_SYSTEM_SENDQUEUEERROR   = 1011;
const int LOG_SYSTEM_COMMANDDATA      = 1012;
const int LOG_SYSTEM_CONSOLEDATA      = 1013;
const int LOG_SYSTEM_DEBUG_CLIENTRECV = 1014;
const int LOG_SYSTEM_DEBUG_CLIENTSEND = 1015;
const int LOG_SYSTEM_DEBUG_SERVERRECV = 1016;
const int LOG_SYSTEM_DEBUG_SERVERSEND = 1017;
const int LOG_SYSTEM_MONITOR          = 1018;

const int DEBUG_ON  = 1;
const int DEBUG_OFF = 0;

/**
* Format a message to the thread-safe ACE logging mechanism.  Valid
* options (prefixed by '%', as in printf format strings) include:
*  - 'A': print an ACE_timer_t value (which could be either double
*         or ACE_UINT32.)
*  - 'a': abort the program at this point abruptly.
*  - 'b': print a ssize_t value
*  - 'B': print a size_t value
*  - 'c': print a character
*  - 'C': print a char* character string (also see s and W)
*  - 'i', 'd': print a decimal number
*  - 'I': indent according to nesting depth (obtained from
*         ACE_Trace::get_nesting_indent()).
*  - 'e', 'E', 'f', 'F', 'g', 'G': print a double
*  - 'l': print line number where an error occurred.
*  - 'M': print the name of the priority of the message.
*  - 'm': return the message corresponding to errno value, e.g., as
*         done by strerror()
*  - 'N': print file name where the error occurred.
*  - 'n': print the name of the program (or "<unknown>" if not set)
*  - 'o': print as an octal number
*  - 'P': print out the current process id
*  - 'p': print out the appropriate errno message from sys_errlist,
*         e.g., as done by perror()
*  - 'Q': print out the uint64 number
*  - 'q': print out the int64 number
*  - '@': print a void* pointer (in hexadecimal)
*  - 'r': call the function pointed to by the corresponding argument
*  - 'R': print return status
*  - 'S': print out the appropriate signal message corresponding
*         to var-argument, e.g., as done by strsignal()
*  - 's': prints a ACE_TCHAR* character string (also see C and W)
*  - 'T': print timestamp in hour:minute:sec:usec format (plain option,
*         i.e. without any flags, prints system supplied timestamp;
*         with '#' flag added expects ACE_Time_Value* in argument list)
*  - 'D': print timestamp as Weekday Month day year hour:minute:sec.usec
*         (plain option, i.e. without any flags, prints system supplied
*         timestamp; with '#' flag added expects ACE_Time_Value* in
*         argument list)
*  - 't': print thread id (1 if single-threaded)
*  - 'u': print as unsigned int
*  - 'w': prints a wide character
*  - 'W': prints a wchar_t* character string (also see C and s)
*  - 'x': print as a hex number
*  - 'X': print as a hex number
*  - 'z': print an ACE_OS::WChar character
*  - 'Z': print an ACE_OS::WChar character string
*  - ':': print a time_t value as an integral number
*  - '%': print out a single percent sign, '%'
*  - '?': print out stack trace (see Stack_Trace.h header comments)
*/
#define OUR_DEBUG(X) { ACE_DEBUG((LM_INFO, "[%D %P|%t][%N,%l]")); ACE_DEBUG(X); }

//*****************************************************************
//λ���������
#define BIT_SET(a,b) if((int)(sizeof(a)) * 8 > b && b >= 0) { ((a) |= ((long long)1<<(b))); }
#define BIT_CLEAR(a,b) if((int)(sizeof(a)) * 8 > b && b >= 0) { ((a) &= ~((long long)1<<(b))); }
#define BIT_FLIP(a,b) if((int)(sizeof(a)) * 8 > b && b >= 0) { ((a) ^= ((long long)1<<(b))); }
#define BIT_CHECK(a,b)  if((int)(sizeof(a)) * 8 > b && b >= 0) { ((a) & ((long long)1<<(b))); }

//*****************************************************************
//�����������������ͷ��һ�������ӽ�����һ���������˳�
const uint16 CLIENT_LINK_CONNECT        = 0x0001;      //�û�����
const uint16 CLIENT_LINK_CDISCONNET     = 0x0002;      //�ͻ����˳�
const uint16 CLIENT_LINK_SDISCONNET     = 0x0003;      //�������˳�
const uint16 CLINET_LINK_SENDTIMEOUT    = 0x0004;      //���������Ϳͻ���ʱ�䳬����ֵ
const uint16 CLINET_LINK_SENDERROR      = 0x0005;      //�ͻ��˷���ʧ����Ϣ 
const uint16 CLINET_LINK_CHECKTIMEOUT   = 0x0006;      //������������ⳬʱ��Ϣ  
const uint16 CLIENT_LINK_SENDOK         = 0x0007;      //���������ͳɹ���ִ
const uint16 CLINET_LINK_TTY_CONNECT    = 0x0008;      //tty��������
const uint16 CLINET_LINK_TTY_DISCONNECT = 0x0009;      //tty�Ͽ���������
const uint16 CLINET_LINK_ST_CONNECT     = 0x000a;      //��������TCP����
const uint16 CLINET_LINK_ST_DISCONNECT  = 0x000b;      //��������TCP���ӶϿ�
const uint16 CLINET_LINK_SU_CONNECT     = 0x000c;      //��������UDP����
const uint16 CLINET_LINK_SU_DISCONNECT  = 0x000d;      //��������UDP���ӶϿ�
const uint16 CLINET_LINK_HANDLER_CLOSE  = 0x000e;      //���ͷ������Ͽ�ָ�� 
const uint16 CLINET_LINK_IS_LOG         = 0x000f;      //������־�����͹ر�ָ�� 
const uint16 CLIENT_LINK_USER           = 0x0100;      //�û����ʼ����ͷ��
//*****************************************************************

#ifndef SAFE_DELETE
#define SAFE_DELETE(x) if( (x) != NULL ) {delete (x); (x) = NULL; }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x) if( (x) != NULL ) {delete[] (x); (x) = NULL; }
#endif

const uint32 PSS_INADDR_ANY = (uint32)INADDR_ANY;

//����һ������������֧���ڴ�Խ����
inline void sprintf_safe(char* szText, int nLen, const char* fmt ...)
{
    if(szText == NULL)
    {
        return;
    }

    va_list ap;
    va_start(ap, fmt);

    vsnprintf(szText, nLen, fmt, ap);
    szText[nLen - 1] = '\0';

    va_end(ap);
};

//֧��memcpy�ı߽���
inline bool memcpy_safe(const char* pSrc, uint32 u4SrcLen, char* pDes, uint32 u4DesLen, bool blIsString = false)
{
    if(u4SrcLen > u4DesLen)
    {
        return false;
    }
    else
    {
        memcpy((void* )pDes, (void* )pSrc, (size_t)u4SrcLen);

        if (u4DesLen > u4SrcLen && true == blIsString)
        {
            //������ı���������ĩβ��\0��β
            pDes[u4SrcLen] = '\0';
        }

        return true;
    }
}

//֧��strcpy�߽���
inline bool strcpy_safe(const char* pSrc, char* pDes, int nDesLen)
{
#if PSS_PLATFORM == PLATFORM_WIN
    strcpy_s(pDes, nDesLen, pSrc);
    return true;
#else
    auto nSrcLen = (int)strlen(pSrc);

    if(nSrcLen <= 0 || nDesLen <= 0 || nSrcLen > nDesLen)
    {
        return false;
    }
    else
    {
        strcpy(pDes, pSrc);
        return true;
    }
#endif
}

//֧��strcat�߽���
inline bool strcat_safe(const char* pSrc, char* pDes, int nDesLen)
{
#if PSS_PLATFORM == PLATFORM_WIN
    strcat_s(pDes, nDesLen, pSrc);
    return true;
#else
    auto nCurrSrcLen = (int)strlen(pSrc);
    auto nCurrDesLen = (int)strlen(pDes);

    if(nDesLen <= 0 || nDesLen <= nCurrSrcLen + nCurrDesLen)
    {
        return false;
    }
    else
    {
        strcat(pDes, (const char* )pSrc);
        return true;
    }
#endif
}

//֧��memmove�߽���
inline bool memmove_safe(char* pSrc, uint32 u4SrcLen, char* pDes, uint32 u4DesLen)
{
#if PSS_PLATFORM == PLATFORM_WIN
    memmove_s(pDes, u4DesLen, pSrc, u4SrcLen);
    return true;
#else
    if(u4SrcLen > u4DesLen)
    {
        return false;
    }
    else
    {
        memmove((void* )pDes, (void* )pSrc, (size_t)u4SrcLen);
        return true;
    }
#endif
}

//��ӡָ����Messahe_Block�е���Ϣ����Ļ
inline void Print_Binary(const char* pData, int nLen)
{
    if(NULL != pData)
    {
        OUR_DEBUG((LM_INFO, "[Print_Binary]"));

        for(int32 i = 0; i < nLen; i++)
        {
            OUR_DEBUG((LM_INFO, " %02x", (unsigned char)pData[i]));
        }

        OUR_DEBUG((LM_INFO, "\n"));
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[Print_Binary]pData is NULL.\n"));
    }
}

//�������ϻ�ȡ����2���ݵĺ���(�ڴ��Ӧ��)
static inline uint32 is_pow_of_2(uint32 x)
{
    return !(x & (x-1));
}

static inline uint32 next_pow_of_2(uint32 x)
{
    if ( is_pow_of_2(x) )
    {
        return x;
    }

    x |= x>>1;
    x |= x>>2;
    x |= x>>4;
    x |= x>>8;
    x |= x>>16;
    return x+1;
}

typedef struct FILETESTRESULTINFO
{
    FILE_TEST_RESULT n4Result = FILE_TEST_RESULT::RESULT_OK; //�������Խ����Ϣ
    int32 n4TimeInterval      = 0;                           //��������ʱ����
    int32 n4ProNum            = 0;                           //��������Э������
    int32 n4ConnectNum        = 0;                           //ģ��������
    vector<string> vecProFileDesc;                           //Э���ļ�����

    FILETESTRESULTINFO(const FILETESTRESULTINFO& ar)
    {
        vecProFileDesc.clear();
        this->n4Result = ar.n4Result;
        this->n4TimeInterval = ar.n4TimeInterval;
        this->n4ProNum = ar.n4ProNum;
        this->n4ConnectNum = ar.n4ConnectNum;
        this->vecProFileDesc.assign(ar.vecProFileDesc.begin(), ar.vecProFileDesc.end());
    }

    FILETESTRESULTINFO()
    {
    }

    ~FILETESTRESULTINFO()
    {
        n4Result = FILE_TEST_RESULT::RESULT_OK;
        n4TimeInterval = 0;
        n4ProNum = 0;
        n4ConnectNum = 0;
        vecProFileDesc.clear();
    }

    FILETESTRESULTINFO& operator= (const FILETESTRESULTINFO& ar)
    {
        this->n4Result = ar.n4Result;
        this->n4TimeInterval = ar.n4TimeInterval;
        this->n4ProNum = ar.n4ProNum;
        this->n4ConnectNum = ar.n4ConnectNum;
        this->vecProFileDesc.assign(ar.vecProFileDesc.begin(), ar.vecProFileDesc.end());
        return *this;
    }
} FileTestResultInfoSt;

//���󴴽���Ϣ
struct _Object_Create_Info
{
    string m_strCreateFileName;                         //�����ļ�
    uint32 m_u4Line                         = 0;        //��������λ��
    uint32 m_u4Count                        = 0;        //�������

    _Object_Create_Info() = default;
};

//�ͻ������ӻ�Ծ��Ϣ
struct _Connect_Chart_Info
{
    uint32 m_n4ConnectCount       = 0;        //��ǰ��Ծ������
    uint32 m_u4LastConnectCount   = 0;    //֮ǰһ���ӵ����ӽ�����
    ACE_Time_Value m_tvConnectTime; //�ռ����ݵ�ʱ��

    _Connect_Chart_Info()
    {
    }
};

//ָ��������Ϣ
struct _Command_Chart_Info
{
    uint16 m_u2CommandID    = 0;            //����ID
    uint32 m_u4CommandCount = 0;            //��λʱ����
    ACE_Time_Value m_tvCommandTime;         //�ֻ����ݵ�ʱ��

    _Command_Chart_Info()
    {
    }
};

//�ļ�����������Ϣ
typedef struct FILETESTDATAINFO
{
    char                    m_szData[MAX_BUFF_10240] = {'\0'};        //��ǰ���������ݵĳ���
    uint32                  m_u4DataLength           = 0;             //��ǰ������е����ݳ���

    FILETESTDATAINFO()
    {
    }

    FILETESTDATAINFO(const FILETESTDATAINFO& ar)
    {
        memset(m_szData, 0, MAX_BUFF_10240);

        if (false == memcpy_safe(const_cast<char*>(ar.m_szData), MAX_BUFF_10240, const_cast<char*>(this->m_szData), MAX_BUFF_10240))
        {
            OUR_DEBUG((LM_INFO, "[FILETESTDATAINFO::FILETESTDATAINFO]memcpy_safe error.\n"));
        }

        this->m_u4DataLength = ar.m_u4DataLength;
    }

    void Close()
    {
        memset(m_szData, 0, MAX_BUFF_10240);
        m_u4DataLength     = 0;
    }

    ~FILETESTDATAINFO()
    {
        Close();
    }

    FILETESTDATAINFO& operator= (const FILETESTDATAINFO& ar)
    {
        if (false == memcpy_safe(const_cast<char*>(ar.m_szData), MAX_BUFF_10240, const_cast<char*>(this->m_szData), MAX_BUFF_10240))
        {
            OUR_DEBUG((LM_INFO, "[FILETESTDATAINFO::FILETESTDATAINFO]operator= error.\n"));
        }

        this->m_u4DataLength = ar.m_u4DataLength;
        return *this;
    }
} FileTestDataInfoSt;

//���VCHARS_TYPE��ģʽ
enum class VCHARS_TYPE
{
    VCHARS_TYPE_TEXT = 0,      //�ı�ģʽ
    VCHARS_TYPE_BINARY,        //������ģʽ
};

#ifndef VCHARS_STR
typedef  struct _VCHARS_STR
{
    char*       text    = NULL;             //����ָ��
    bool        blCopy  = true;             //�Ƿ񿽱����ݿ飬True�ǿ�����Ĭ���ǿ���
    bool        blNew   = false;            //�Ƿ���new����������
    uint8       u1Len   = 0;                //���ݳ���
    VCHARS_TYPE type    = VCHARS_TYPE::VCHARS_TYPE_TEXT; //���ͣ����Ͷ����VCHARS_TYPE

    _VCHARS_STR(bool blIsCopy = true, VCHARS_TYPE ntype = VCHARS_TYPE::VCHARS_TYPE_TEXT)
    {
		blCopy = blIsCopy;
		type = ntype;
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
            //�������Ҫ�������ڴ棬������������
            if(blNew == true)
            {
                SAFE_DELETE(text);
            }

            if(type == VCHARS_TYPE::VCHARS_TYPE_TEXT)
            {
                //�ı�ģʽ
                uint8 u1targetLen = u1Length + 1;
                text = new char[u1targetLen];

                if (false == memcpy_safe(pData, u1Length, text, u1Length))
                {
                    OUR_DEBUG((LM_INFO, "[_VCHARS_STR::SetData]memcpy_safe error.\n"));
                    SAFE_DELETE(text);
                    return;
                }

                text[u1Length] = '\0';
                u1Len = u1Length + 1;
            }
            else
            {
                //������ģʽ
                text = new char[u1Length];

                if (false == memcpy_safe(pData, u1Length, text, u1Length))
                {
                    OUR_DEBUG((LM_INFO, "[_VCHARS_STR::SetData]binary memcpy_safe error.\n"));
                    SAFE_DELETE(text);
                    return;
                }

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

} VCHARS_STR;
#endif

#ifndef VCHARM_STR
typedef  struct _VCHARM_STR
{
    char*       text   = NULL;             //����ָ��
    uint16      u2Len  = 0;                //���ݳ���
    bool        blCopy = true;             //�Ƿ񿽱����ݿ飬True�ǿ�����Ĭ���ǿ���
    bool        blNew  = false;            //�Ƿ���new����������
    VCHARS_TYPE type   = VCHARS_TYPE::VCHARS_TYPE_TEXT; //���ͣ����Ͷ����VCHARS_TYPE

    _VCHARM_STR(bool blIsCopy = true, VCHARS_TYPE ntype = VCHARS_TYPE::VCHARS_TYPE_TEXT)
    {
        blCopy = blIsCopy;
        type   = ntype;
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
            //�������Ҫ�������ڴ棬������������
            if(blNew == true)
            {
                SAFE_DELETE(text);
            }

            if(type == VCHARS_TYPE::VCHARS_TYPE_TEXT)
            {
                //�ı�ģʽ
                uint16 u2targetLen = u2Length + 1;
                text = new char[u2targetLen];

                if (false == memcpy_safe((char*)pData, u2Length, text, u2Length))
                {
                    OUR_DEBUG((LM_INFO, "[_VCHARM_STR::SetData]memcpy_safe error.\n"));
                    SAFE_DELETE(text);
                    return;
                }

                text[u2Length] = '\0';
                u2Len = u2Length + 1;
            }
            else
            {
                //������ģʽ
                text = new char[u2Length];

                if (false == memcpy_safe((char*)pData, u2Length, text, u2Length))
                {
                    OUR_DEBUG((LM_INFO, "[_VCHARM_STR::SetData]binary memcpy_safe error.\n"));
                    SAFE_DELETE(text);
                    return;
                }

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

} VCHARM_STR;
#endif

#ifndef VCHARB_STR
typedef  struct _VCHARB_STR
{
    char*       text   = NULL;               //����ָ��
    uint32      u4Len  = 0;                  //���ݳ���
    bool        blCopy = true;               //�Ƿ񿽱����ݿ飬True�ǿ�����Ĭ���ǿ���
    bool        blNew  = false;              //�Ƿ���new����������
    VCHARS_TYPE type   = VCHARS_TYPE::VCHARS_TYPE_TEXT;   //���ͣ����Ͷ����VCHARS_TYPE

    _VCHARB_STR(bool blIsCopy = true, VCHARS_TYPE ntype = VCHARS_TYPE::VCHARS_TYPE_TEXT)
    {
        blCopy = blIsCopy;
        type   = ntype;
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
        //���������0�����治��������
        if (u4Length == 0)
        {
            return;
        }

        if(blCopy == true)
        {
            //�������Ҫ�������ڴ棬������������
            if(blNew == true)
            {
                SAFE_DELETE(text);
            }

            if(type == VCHARS_TYPE::VCHARS_TYPE_TEXT)
            {
                //�ı�ģʽ
                uint32 u4targetLen = u4Length + 1;
                text = new char[u4targetLen];

                if (false == memcpy_safe((char*)pData, u4Length, text, u4Length))
                {
                    OUR_DEBUG((LM_INFO, "[_VCHARB_STR::SetData]memcpy_safe error.\n"));
                    SAFE_DELETE(text);
                    return;
                }

                text[u4Length] = '\0';
                u4Len = u4Length + 1;
            }
            else
            {
                //������ģʽ
                text = new char[u4Length];

                if (false == memcpy_safe((char*)pData, u4Length, text, u4Length))
                {
                    OUR_DEBUG((LM_INFO, "[_VCHARB_STR::SetData]binary memcpy_safe error.\n"));
                    SAFE_DELETE(text);
                    return;
                }

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

} VCHARB_STR;
#endif

//����PacketParse�������Ϣ��
//���ݰ�ͷ�ṹ
struct _Head_Info
{
    uint32             m_u4HeadSrcLen      = 0;       //ԭʼ���ݰ�ͷ��������ǰ��
    uint32             m_u4HeadCurrLen     = 0;       //��ǰ���ݰ��� ��������
    uint32             m_u4BodySrcLen      = 0;       //��ǰ���峤�ȣ�����ǰ��
    uint16             m_u2PacketCommandID = 0;       //CommandID
    ACE_Message_Block* m_pmbHead           = NULL;    //��ͷ��Ϣ��

    _Head_Info() = default;
};

//���ݰ���ṹ
struct _Body_Info
{
    uint32             m_u4BodySrcLen      = 0;       //ԭʼ���ݰ��峤������ǰ��
    uint32             m_u4BodyCurrLen     = 0;       //��ǰ���ݰ��� ��������
    uint16             m_u2PacketCommandID = 0;       //CommandID(����У���ֱ�Ӹ�ֵ�����û�У��򱣳ֳ�ʼֵ����)
    ACE_Message_Block* m_pmbBody           = NULL;    //��ͷ��Ϣ��

    _Body_Info() = default;
};

//���ݰ������ṹ
struct _Packet_Info
{
    uint32             m_u4HeadSrcLen      = 0;       //ԭʼ���ݰ�ͷ��������ǰ��
    uint32             m_u4HeadCurrLen     = 0;       //��ǰ���ݰ��� ��������
    uint32             m_u4BodySrcLen      = 0;       //ԭʼ���ݰ�ͷ��������ǰ��
    uint32             m_u4BodyCurrLen     = 0;       //��ǰ���ݰ��� ��������
    uint16             m_u2PacketCommandID = 0;       //CommandID
    ACE_Message_Block* m_pmbHead           = NULL;    //��ͷ��Ϣ��
    ACE_Message_Block* m_pmbBody           = NULL;    //��ͷ��Ϣ��

    _Packet_Info() = default;
};

//��ʱ������ݰ���������������Ϣ������������Ч�Ե��߼��ж�
struct _TimeConnectInfo
{
    uint32 m_u4RecvPacketCount      = 0;         //��ǰ���հ�����
    uint32 m_u4RecvSize             = 0;         //��ǰ����������
    uint32 m_u4SendPacketCount      = 0;         //��ǰ���Ͱ�����
    uint32 m_u4SendSize             = 0;         //��ǰ����������
    uint32 m_u4ValidRecvPacketCount = 0;         //��λʱ�������������ݰ�������
    uint32 m_u4ValidRecvSize        = 0;         //��λʱ�����������ݽ�����
    uint32 m_u4ValidSendPacketCount = 0;         //��λʱ��������������ݰ�������
    uint32 m_u4ValidSendSize        = 0;         //��λʱ�����������ݷ�����
    uint8  m_u1Minutes              = 0;         //��ǰ�ķ�����

    _TimeConnectInfo() = default;

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
            //��Ҫ�Ƚ�
            if(m_u4RecvPacketCount > m_u4ValidRecvPacketCount)
            {
                return false;
            }
        }

        if(m_u4ValidRecvSize > 0)
        {
            //��Ҫ�Ƚ�
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
            //��Ҫ�Ƚ�
            if(m_u4SendPacketCount > m_u4ValidSendPacketCount)
            {
                return false;
            }
        }

        if(m_u4ValidSendSize > 0)
        {
            //��Ҫ�Ƚ�
            if(u4SendSize > m_u4ValidSendSize)
            {
                return false;
            }
        }

        return true;
    }
};

//��ʱ�����������ýṹ
class _TimerCheckID
{
public:
    uint16 m_u2TimerCheckID = 0;

    _TimerCheckID() = default;
};

//Message�������ݿ�ṹ��
class _PacketInfo
{
public:
    char*   m_pData    = NULL;       //�����������ͷָ��
    int32   m_nDataLen = 0;          //����������ݳ���

    _PacketInfo() = default;
};

//�ͻ���������Ϣ���ݽṹ
class _ClientConnectInfo
{
public:
    uint64        m_u8RecvQueueTimeCost = 0;     //�����߼��������ʱ������
    uint64        m_u8SendQueueTimeCost = 0;     //����������ʱ������
    uint32        m_u4ConnectID         = 0;     //����ID
    uint32        m_u4RecvCount         = 0;     //���հ�����
    uint32        m_u4SendCount         = 0;     //���Ͱ�����
    uint32        m_u4AllRecvSize       = 0;     //�����ֽ���
    uint32        m_u4AllSendSize       = 0;     //�����ֽ���
    uint32        m_u4BeginTime         = 0;     //���ӽ���ʱ��
    uint32        m_u4AliveTime         = 0;     //���ʱ������
    uint32        m_u4RecvQueueCount    = 0;     //�����߼�������ĸ���
    bool          m_blValid             = false; //��ǰ�����Ƿ���Ч
    ACE_INET_Addr m_addrRemote;                  //Զ�����ӵ�ַ

    _ClientConnectInfo() = default;
};
typedef vector<_ClientConnectInfo> vecClientConnectInfo;

//Ҫ���ӵķ�������Ϣ
class _ServerConnectInfo
{
public:
    uint32      m_u4ServerID   = 0;     //��������ID
    uint32      m_u4ServerPort = 0;     //�������Ķ˿�
    uint32      m_u4MaxConn    = 0;     //������������߳�������
    uint32      m_u4TimeOut    = 0;     //�����������ӳ�ʱʱ��
    ACE_TString m_strServerName;        //������������
    ACE_TString m_strServerIP;          //��������IP

    _ServerConnectInfo() = default;
};

//Ϊ�߼����ṩһ��Try catch�ı����꣬���ڵ��ԣ�����ʹ�÷�����ο�TestTcp����
//Ŀǰ���֧��һ��2K����־
//************************************************************************
const char ASSERT_LOG_PATH[] = "./Log/assert.log";   //���·�����Լ�Ҫ�����޸����

inline void __show__( const char* szTemp)
{
#if PSS_PLATFORM == PLATFORM_WIN
    printf_s("[__show__]%s.\n", szTemp);
    FILE* f = nullptr;
    fopen_s(&f, ASSERT_LOG_PATH, "a");
#else
    printf("[__show__]%s.\n", szTemp);
    FILE* f = fopen(ASSERT_LOG_PATH, "a");
#endif
    if (nullptr != f)
    {
        fwrite(szTemp, strlen(szTemp), sizeof(char), f);
        fwrite("\r\n", 1, 2 * sizeof(char), f);
        fclose(f);
    }
};

inline void __assertspecial__(const char* file, int32 line, const char* func, const char* expr, const char* msg)
{
    char szTemp[2*MAX_BUFF_1024] = {0};

    sprintf_safe( szTemp, 2*MAX_BUFF_1024, "Alert[%s][%d][%s][%s][%s]", file, line, func, expr,msg) ;
    __show__(szTemp) ;
};

#if PSS_PLATFORM == PLATFORM_WIN
#define AssertSpecial(expr,msg) ((void)((expr)?0:(__assertspecial__(__FILE__, __LINE__, __FUNCTION__, #expr, msg),0)))
#else
#define AssertSpecial(expr,msg) {if(!(expr)){__assertspecial__(__FILE__, __LINE__, __PRETTY_FUNCTION__, #expr, msg);}}
#endif

#if PSS_PLATFORM == PLATFORM_WIN
#define __ENTER_FUNCTION() {try{
#define __THROW_FUNCTION(msg) throw(msg)
#define __LEAVE_FUNCTION() }catch(char* msg){AssertSpecial(false,msg); }}
#define __LEAVE_FUNCTION_WITHRETURN(ret) }catch(char* msg){AssertSpecial(false,msg); return ret; }}
#else   //linux
#define __ENTER_FUNCTION() {try{
#define __THROW_FUNCTION(msg) throw(msg)
#define __LEAVE_FUNCTION() }catch(char* msg){AssertSpecial(false,msg);}}
#define __LEAVE_FUNCTION_WITHRETURN(ret) }catch(char* msg){AssertSpecial(false,msg); return ret; }}
#endif

//************************************************************************

//����һ��ͳ�ƺ�������ʱ��ĺ꣬��������ֱ��ͳ�Ƶ�ǰ����ִ��ʱ��
//ʹ������ __TIMECOST(100); 100Ϊ���룬����100����ͻ������־
//************************************************************************
const char ASSERT_TIME_PATH[] = "./Log/FuncTimeout.log";   //���·�����Լ�Ҫ�����޸����
class CTimeCost
{
public:
    CTimeCost(unsigned int nMillionSecond, const char* pFunctionName, const char* pFileName, int32 nLine)
    {
        m_nMillionSecond = nMillionSecond;
        sprintf_safe(m_szFunctionName, MAX_BUFF_100, "%s", pFunctionName);
        sprintf_safe(m_szFileName, MAX_BUFF_300, "%s", pFileName);
        m_nFileLine = nLine;
        TimeBegin();
    }

    ~CTimeCost()
    {
        TimeEnd();
    }

    void TimeBegin()
    {
        m_lBegin = GetSystemTickCount();
    }

    void TimeEnd()
    {
        m_lEnd = GetSystemTickCount();
        uint64 lTimeInterval = m_lEnd - m_lBegin;  //ת���ɺ���

        if(lTimeInterval >= (long)m_nMillionSecond)
        {
            //��¼��־
#if PSS_PLATFORM == PLATFORM_WIN
            FILE* pFile = nullptr;
            fopen_s(&pFile, ASSERT_TIME_PATH, "a");
#else
            FILE* pFile = fopen(ASSERT_TIME_PATH, "a+");
#endif

            if(pFile != nullptr)
            {
                char szLog[MAX_BUFF_1024] = { '\0' };

                char szTimeNow[MAX_BUFF_50];
                time_t tNow = time(NULL);
#if PSS_PLATFORM == PLATFORM_WIN
                struct tm tmNow_s;
                localtime_s(&tmNow_s, &tNow);
                struct tm* tmNow = &tmNow_s;
#else
                struct tm* tmNow = localtime(&tNow);
#endif
                sprintf_safe(szTimeNow, MAX_BUFF_50, "%04d-%02d-%02d %02d:%02d:%02d", tmNow->tm_year + 1900, tmNow->tm_mon + 1, tmNow->tm_mday, tmNow->tm_hour, tmNow->tm_min, tmNow->tm_sec);
                sprintf_safe(szLog, MAX_BUFF_1024, "[%s]dbTimeInterval more than (%d) < (%d), File(%s):FunName(%s):Line(%d).\n", szTimeNow, m_nMillionSecond, lTimeInterval, m_szFileName, m_szFunctionName, m_nFileLine);
                fwrite(szLog, strlen(szLog), sizeof(char), pFile);
                fclose(pFile);
            }
        }
    }

private:
    int64 GetSystemTickCount()
    {
#if PSS_PLATFORM == PLATFORM_WIN
        //return (unsigned long)GetTickCount();
        static LARGE_INTEGER TicksPerSecond = { 0 };
        LARGE_INTEGER Tick;

        if (!TicksPerSecond.QuadPart)
        {
            QueryPerformanceFrequency(&TicksPerSecond);
        }

        QueryPerformanceCounter(&Tick);

        int64 Seconds = Tick.QuadPart / TicksPerSecond.QuadPart;
        int64 LeftPart = Tick.QuadPart - (TicksPerSecond.QuadPart * Seconds);
        int64 MillSeconds = LeftPart * 1000 / TicksPerSecond.QuadPart;
        int64 Ret = Seconds * 1000 + MillSeconds;
        _ASSERT(Ret > 0);
        return Ret;
#else
        struct timespec ts;

        clock_gettime(CLOCK_MONOTONIC, &ts);

        return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
#endif
    }

private:
    int64        m_lBegin                       = 0;
    int64        m_lEnd                         = 0;
    int32        m_nFileLine                    = 0;
    uint32       m_nMillionSecond               = 0;
    char         m_szFunctionName[MAX_BUFF_100] = {'\0'};
    char         m_szFileName[MAX_BUFF_300]     = { '\0' };
};

#define __TIMECOST(cost) CTimeCost timecost(cost, __FUNCTION__, __FILE__, __LINE__);

//************************************************************************
//add by freeeyes
//��ö��Լ���С�˵������ж�
//�����mainConfig�У�����Ϊû��Ҫ��define.h���ж�
//�ڿ�����Լ��ж��Ƿ�ת����С��
enum class ENUM_CHAR_ORDER
{
    SYSTEM_LITTLE_ORDER = 0,   //С������
    SYSTEM_BIG_ORDER,          //�������
};

enum class ENUM_WHILE_STATE
{
    WHILE_STATE_CONTINUE = 0,
    WHILE_STATE_BREAK,
};

const unsigned int O32_LITTLE_ENDIAN = 0x03020100ul;
const unsigned int O32_BIG_ENDIAN    = 0x00010203ul;
const unsigned int O32_PDP_ENDIAN    = 0x01000302ul;

static const union
{
    unsigned char bytes[4];
    uint32 value;
} o32_host_order =
{ { 0, 1, 2, 3 } };

#define O32_HOST_ORDER (o32_host_order.value)

//����һ����64λ�����ε������ֽ����ת��
inline uint64 hl64ton(uint64 u8Data)
{
    if(O32_HOST_ORDER == O32_LITTLE_ENDIAN)
    {
        union
        {
            uint32 lv[2];
            uint64 llv;
        } u;
        u.lv[0] = htonl(u8Data >> 32);
        u.lv[1] = htonl(u8Data & 0xFFFFFFFFULL);
        return u.llv;
    }
    else
    {
        //��������Ǵ�˾Ͳ���
        return u8Data;
    }

}

//����һ����64λ�����ε������ֽ����ת��
inline uint64 ntohl64(uint64 u8Data)
{
    if(O32_HOST_ORDER == O32_LITTLE_ENDIAN)
    {
        union
        {
            uint32 lv[2];
            uint64 llv;
        } u;
        u.llv = u8Data;
        return ((uint64)ntohl(u.lv[0]) << 32) | (uint64)ntohl(u.lv[1]);
    }
    else
    {
        //��������Ǵ�˾Ͳ���
        return u8Data;
    }
}

//����һ����������֧���ַ����滻��Ŀǰ�Ȳ�����֧������
inline bool Replace_String(char* pText, uint32 u4Len, const char* pOld, const char* pNew)
{
    char* pTempSrc = new char[u4Len];

    if (false == memcpy_safe(pText, u4Len, pTempSrc, u4Len))
    {
        return false;
    }

    pTempSrc[u4Len - 1] = '\0';

    auto u2NewLen = (uint16)strlen(pNew);

    char* pPos = strstr(pTempSrc, pOld);

    while(pPos)
    {
        //�������Ҫ���ǵ��ַ�������
        uint32 u4PosLen = (uint32)(pPos - pTempSrc);

        //�����ǰ���
        if (false == memcpy_safe(pText, u4PosLen, pTempSrc, u4PosLen))
        {
            return false;
        }

        pText[u4PosLen] = '\0';

        if(u4PosLen + u2NewLen >= (uint32)u4Len)
        {
            //�����м����
            delete[] pTempSrc;
            return false;
        }
        else
        {
            //������ַ�
            if (false == memcpy_safe(pText, u2NewLen, &pText[u4PosLen], u2NewLen))
            {
                return false;
            }

            pText[u4PosLen + u2NewLen] = '\0';

            //ָ������ƶ�
            pTempSrc =  pTempSrc + u4PosLen;

            //Ѱ����һ����ͬ���ַ���
            pPos = strstr(pTempSrc, pOld);
        }

    }

    //�����м����
    delete[] pTempSrc;
    return true;
}

//д��ռ�ļ���
inline int32 AcquireWriteLock(int32 fd, int32 start, int32 len)
{
#if PSS_PLATFORM != PLATFORM_WIN
    struct flock arg;
    arg.l_type = F_WRLCK; // ��д��
    arg.l_whence = SEEK_SET;
    arg.l_start = start;
    arg.l_len = len;
    arg.l_pid = getpid();

    return fcntl(fd, F_SETLKW, &arg);
#else
    return -1;
#endif
}

//�ͷŶ�ռ�ļ���
inline int32 ReleaseLock(int32 fd, int32 start, int32 len)
{
#if PSS_PLATFORM != PLATFORM_WIN
    struct flock arg;
    arg.l_type = F_UNLCK; //  ����
    arg.l_whence = SEEK_SET;
    arg.l_start = start;
    arg.l_len = len;
    arg.l_pid = getpid();

    return fcntl(fd, F_SETLKW, &arg);
#else
    return -1;
#endif
}

//�鿴д��
inline int32 SeeLock(int32 fd, int32 start, int32 len)
{
#if PSS_PLATFORM != PLATFORM_WIN
    struct flock arg;
    arg.l_type = F_WRLCK;
    arg.l_whence = SEEK_SET;
    arg.l_start = start;
    arg.l_len = len;
    arg.l_pid = getpid();

    if (fcntl(fd, F_GETLK, &arg) != 0) // ��ȡ��
    {
        return -1; // ����ʧ��
    }

    if (arg.l_type == F_UNLCK)
    {
        return 0; // ����
    }
    else if (arg.l_type == F_RDLCK)
    {
        return 1; // ����
    }
    else if (arg.l_type == F_WRLCK)
    {
        return 2; // д��
    }

    return 0;
#else
    return -1;
#endif
}

//�ͻ���IP��Ϣ
class _ClientIPInfo
{
public:
    string  m_strClientIP;      //�ͻ��˵�IP��ַ
    uint16 m_u2Port  = 0;        //�ͻ��˵Ķ˿�
};

//���ӱ���ӳ����Ϣ(����PSS_ClientManager����)
class _ClientNameInfo
{
public:
    int16  m_u2Port                  = 0;           //�ͻ��˵Ķ˿�
    int32  m_nConnectID              = 0;           //����ID
    int32  m_nLog                    = 0;           //�Ƿ��¼��־
    string m_strName;                               //���ӱ���
    string m_strClientIP;                           //�ͻ��˵�IP��ַ

    _ClientNameInfo() = default;

};
using vecClientNameInfo = vector<_ClientNameInfo>;

//��ʽ��һ��ACE Hash��
template<class EXT_ID, class INT_ID>
class ACE_Hash_Map :
    public ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID,
    ACE_Hash<EXT_ID>, ACE_Equal_To<EXT_ID>, ACE_Null_Mutex>
{};


inline void Set_Output_To_File(int nTrunOn, ofstream*& pLogoStream, const char* pLogPath, const char* pLogName, int nMaxLogSize)
{
    //�������Ҫ�������־
    if (nTrunOn == 0)
    {
        return;
    }

    char szDebugFileName[MAX_BUFF_200] = { '\0' };
    sprintf_safe(szDebugFileName, MAX_BUFF_200, "%s/%s.log", pLogPath, pLogName);

    if (pLogoStream == NULL)
    {
        //������½����ļ�
        pLogoStream = new ofstream(szDebugFileName, std::ofstream::out | std::ofstream::trunc);
        ACE_LOG_MSG->msg_ostream(pLogoStream);
    }
    else
    {
        //��õ�ǰ�Ķ����Ƿ��Ѿ�������max����ֵ
        if ((size_t)nMaxLogSize <= (size_t)pLogoStream->tellp())
        {
            ACE_LOG_MSG->acquire();
            //��Ҫ����һ����־�ļ�
            ofstream* pOldLogoStream = (ofstream*)ACE_LOG_MSG->msg_ostream();
            if (NULL != pOldLogoStream)
            {
				ACE_Date_Time  dt;
				//ת����־�ļ�
				char szHistoryLogFile[MAX_BUFF_200] = { '\0' };
				sprintf_safe(szHistoryLogFile, MAX_BUFF_200, "%s/%s_%04d%02d%02d_%02d%02d%02d.log", pLogPath, pLogName,
					dt.year(), dt.month(), dt.day(), dt.hour(), dt.minute(), dt.second());
				rename(szDebugFileName, szHistoryLogFile);

				pOldLogoStream->close();
#if PSS_PLATFORM == PLATFORM_WIN
                _unlink(szDebugFileName);
#else
				unlink(szDebugFileName);
#endif

				pOldLogoStream->open(szDebugFileName, std::ofstream::out | std::ofstream::trunc);
            }

            ACE_LOG_MSG->release();
        }
    }

    ACE_LOG_MSG->set_flags(ACE_Log_Msg::STDERR | ACE_Log_Msg::OSTREAM);
}

//��ʽ������һ�ζ��������Ϊʮ������
inline string buffer_to_Hex_string(string strText)
{
    string strValue;

    for (auto f : strText)
    {
        strValue += fmt::format("{0:#x} ", (unsigned char)f);
    }

    return strValue;
}

//�з��ַ���
inline vector<string> split_string(const string& s, const char& c)
{
    string buff{ "" };
    vector<string> v;

    for (auto n : s)
    {
        if (n != c) buff += n; else
            if (n == c && buff != "") { v.push_back(buff); buff = ""; }
    }
    if (buff != "") v.push_back(buff);

    return v;
}

#if PSS_PLATFORM != PLATFORM_WIN

//��õ�ǰ�ļ�����
inline int Checkfilelimit(int nMaxOpenFile)
{
    //��õ�ǰ�ļ�����
    struct rlimit rfilelimit;

    if (getrlimit(RLIMIT_NOFILE, &rfilelimit) != 0)
    {
        OUR_DEBUG((LM_INFO, "[Checkfilelimit]failed to getrlimit number of files.\n"));
        return -1;
    }
    else
    {
        OUR_DEBUG((LM_INFO, "[Checkfilelimit]rfilelimit.rlim_cur=%d,nMaxOpenFile=%d.\n", rfilelimit.rlim_cur, nMaxOpenFile));

        //��ʾͬʱ�ļ��������㣬��Ҫ���á�
        if ((int)rfilelimit.rlim_cur < nMaxOpenFile)
        {
            OUR_DEBUG((LM_INFO, "[Checkfilelimit]** WARNING!WARNING!WARNING!WARNING! **.\n"));
            OUR_DEBUG((LM_INFO, "[Checkfilelimit]** PSS WILL AUTO UP FILE OPEN LIMIT **.\n"));
            OUR_DEBUG((LM_INFO, "[Checkfilelimit]** WARNING!WARNING!WARNING!WARNING! **.\n"));
            //����Զ������Ĺ�����ʱע�ͣ���ά��Ա����֪��������Ⲣ�Լ����ã�������ѡ��
            //������ʱ��߲����ļ���
            rfilelimit.rlim_cur = (rlim_t)nMaxOpenFile;
            rfilelimit.rlim_max = (rlim_t)nMaxOpenFile;

            if (setrlimit(RLIMIT_NOFILE, &rfilelimit) != 0)
            {
                OUR_DEBUG((LM_INFO, "[Checkfilelimit]failed to setrlimit number of files(error=%s).\n", strerror(errno)));
                return -1;
            }

            //����޸ĳɹ����ٴμ��һ��
            if (getrlimit(RLIMIT_NOFILE, &rfilelimit) != 0)
            {
                OUR_DEBUG((LM_INFO, "[Checkfilelimit]failed to getrlimit number of files.\n"));
                return -1;
            }

            //�ٴμ���޸ĺ���ļ������
            if ((int)rfilelimit.rlim_cur < nMaxOpenFile)
            {
                OUR_DEBUG((LM_INFO, "[Checkfilelimit]rlim.rlim_cur=%d, nMaxOpenFile=%d, openfile is not enougth�� please check [ulimit -a].\n", (int)rfilelimit.rlim_cur, nMaxOpenFile));
                return -1;
            }

            return 0;
        }
    }

    return 0;
}

inline void Gdaemon()
{
    pid_t pid;

    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);

    if (setpgrp() == -1)
    {
        perror("setpgrp failure");
    }

    signal(SIGHUP, SIG_IGN);

    if ((pid = fork()) < 0)
    {
        perror("fork failure");
        exit(1);
    }
    else if (pid > 0)
    {
        exit(0);
    }

    setsid();
    umask(0);

    signal(SIGCLD, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);
}
#endif
END_NAMESPACE
USING_NAMESPACE

#endif
