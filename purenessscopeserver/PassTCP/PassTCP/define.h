#ifndef _DEFINE_H
#define _DEFINE_H

#include <WinSock2.h>
#include <time.h>
#include <string>

using namespace std;

#define MAX_BUFF_20   20
#define MAX_BUFF_100  100
#define MAX_BUFF_1024 1024

//设置一个随机种子
inline void InitRandom()
{
	srand((int)time(NULL));
};

//从一个值域中获取一个随机值
inline int RandomValue(int nMin, int nMax)
{
	return  nMin + (int) ((nMax - nMin) * (rand() / (RAND_MAX + 1.0)));
};

class CConvertBuffer
{
public:
	CConvertBuffer() {};
	~CConvertBuffer() {};
	int GetBufferSize(const char* pData, int nSrcLen)
	{
		char szData[3] = {'\0'};
		int nPos         = 0;
		int nCurrSize    = 0;
		int nConvertSize = 0;
		bool blState     = false;   //转换后的字符串是否有效
		bool blSrcState  = true;    //元字符串是否有效
		unsigned char cData;

		while(nPos < nSrcLen)
		{
			if(pData[nPos] == '\r' || pData[nPos] == '\n' || pData[nPos] == ' ' || nPos == nSrcLen - 1)
			{
				if(nPos == nSrcLen - 1)
				{
					szData[nCurrSize++] = pData[nPos];
				}

				szData[nCurrSize] = '\0';
				if(blSrcState == true)
				{
					blState = ConvertStr2char(szData, cData);
					if(blState == true)
					{
						nConvertSize++;
					}
				}
				nCurrSize  = 0;
				blSrcState = true;
				nPos++;
			}
			else
			{
				if(nCurrSize < 2)
				{
					szData[nCurrSize++] = pData[nPos];
				}
				else
				{
					blSrcState = false;
				}
				nPos++;
			}
		}

		return nConvertSize;
	};

	bool Convertstr2charArray(const char* pData, int nSrcLen, unsigned char* pDes, int& nMaxLen)
	{
		char szData[3] = {'\0'};
		int nPos         = 0;
		int nCurrSize    = 0;
		int nConvertSize = 0;
		bool blState     = false;   //转换后的字符串是否有效
		bool blSrcState  = true;    //元字符串是否有效

		while(nPos < nSrcLen)
		{
			if(pData[nPos] == '\r' || pData[nPos] == '\n' || pData[nPos] == ' ' || nPos == nSrcLen - 1)
			{
				if(nPos == nSrcLen - 1)
				{
					szData[nCurrSize++] = pData[nPos];
				}

				szData[nCurrSize] = '\0';
				if(nConvertSize < nMaxLen && blSrcState == true)
				{
					blState = ConvertStr2char(szData, pDes[nConvertSize]);
					if(blState == true)
					{
						nConvertSize++;
					}
				}
				nCurrSize  = 0;
				blSrcState = true;
				nPos++;
			}
			else
			{
				if(nCurrSize < 2)
				{
					szData[nCurrSize++] = pData[nPos];
				}
				else
				{
					blSrcState = false;
				}
				nPos++;
			}
		}

		nMaxLen = nConvertSize;
		return true;
	};
private:
	bool Get_binary_Char(unsigned char cTag, unsigned char& cDes)
	{
		if(cTag >='A'&&  cTag <='F')
		{
			cDes = cTag - 'A' + 10;
			return true;
		}
		else if(cTag >='a'&&  cTag <='f')
		{
			cDes = cTag - 'a' + 10;
			return true; 
		}
		else if(cTag >= '0'&& cTag<= '9')
		{
			cDes = cTag-'0';
			return true;
		}
		else
		{
			return false;
		}
	}

	bool ConvertStr2char(const char* pData, unsigned char& cData)
	{
		if(pData == NULL || strlen(pData) != 2)
		{
			return false;
		}

		char cFirst = pData[1];
		unsigned char cTemp = 0;
		bool blStste = Get_binary_Char(cFirst, cTemp);
		if(false == blStste)
		{
			return false;
		}
		cData = cTemp;
		char cSecond = pData[0];
		blStste  = Get_binary_Char(cSecond, cTemp);
		if(false == blStste)
		{
			return false;
		}
		cTemp = cTemp << 4;
		cData = cData | cTemp;

		return true;
	}
};

enum EM_DATA_RETURN_STATE
{
	DATA_RETURN_STATE_SUCCESS = 0,
	DATA_RETURN_STATE_ERROR,
	DATA_RETURN_STATE_CONTINUE,
};

//声明一个基类，来负责格式化数据发送和接收的部分逻辑
class CBaseDataLogic
{
public:
	virtual bool InitSendSize(int nSendLen)                                 = 0;
	virtual char* GetSendData()                                             = 0;
	virtual char* GetSendData(int nThreadID, int nCurrIndex)                = 0;
	virtual int GetSendLength()                                             = 0;                   
	virtual int GetRecvLength()                                             = 0;
	virtual void SetRecvLength(int nRecvLen)                                = 0;
	virtual void SetMaxSendLength(int nMaxLength)                           = 0;
	virtual EM_DATA_RETURN_STATE GetRecvData(int nThreadID, int nCurrIndex, char* pData, int nLen) = 0;
};

//普通的TCP收发相关信息处理逻辑
//为了兼容一些特殊协议，比如websocket以及http
//收发数据不再是单独的一个指针，而是一个继承CBaseDataLogic的类
class CNomalLogic : public CBaseDataLogic
{
public:
	CNomalLogic() 
	{ 
		m_pSendData    = NULL;
		m_nSendLen     = 0;
		m_nRecvLen     = 0;
		m_nCurrRecvLen = 0;
	};

	~CNomalLogic() { Close(); };

	void Close()
	{
		if(NULL != m_pSendData)
		{
			delete[] m_pSendData;
			m_pSendData = NULL;
		}
	}

	bool InitSendSize(int nSendLen)
	{
		Close();

		m_pSendData = new char[nSendLen];
		m_nSendLen  = nSendLen;

		return true;
	}

	void SetRecvLength(int nRecvLen)
	{
		m_nRecvLen = nRecvLen;
	}

	void SetMaxSendLength(int nMaxLength)
	{
		m_nSendLen = nMaxLength;
	}

	//设置相关发送Buff
	void SetSendBuff(const char* pData, int nLen)
	{
		memcpy_s(m_pSendData, nLen, pData, nLen);
		m_nSendLen = nLen;
	}

	char* GetSendData()
	{
		return m_pSendData;
	}

	char* GetSendData(int nThreadID, int nCurrIndex)
	{
		return m_pSendData;
	}

	int GetSendLength()
	{
		return m_nSendLen;
	}

	int GetRecvLength()
	{
		return m_nRecvLen;
	}

	EM_DATA_RETURN_STATE GetRecvData(int nThreadID, int nCurrIndex, char* pData, int nLen)
	{
		m_nCurrRecvLen += nLen;
		if(m_nCurrRecvLen == m_nRecvLen)
		{
			m_nCurrRecvLen = 0;
			//全部接收完毕，返回正确
			return DATA_RETURN_STATE_SUCCESS;
		}
		else if(nLen < m_nRecvLen)
		{
			//没有接收完全，继续接收
			return DATA_RETURN_STATE_CONTINUE;
		}

		m_nCurrRecvLen = 0;
		return DATA_RETURN_STATE_ERROR;
	}

private:
	char* m_pSendData;
	int   m_nSendLen;
	int   m_nRecvLen;
	int   m_nCurrRecvLen;
};

//线程连接信息
class _Socket_Info
{
public:
	char  m_szSerevrIP[MAX_BUFF_20];      //远程服务器的IP
	int   m_nPort;                        //远程服务器的端口
	int   m_nThreadID;                    //线程ID
	int   m_nRecvTimeout;                 //接收数据超时时间（单位是毫秒）
	int   m_nDelaySecond;                 //短连接间延时（单位是毫秒）
	int   m_nPacketTimewait;              //数据包发送间隔(单位是毫秒)
	//int   m_nSendLength;                //发送字符串长度
	//int   m_nRecvLength;                //接收字符串长度限定
	//char* m_pSendBuff;                  //发送数据长度
	bool  m_blIsAlwayConnect;             //是否长连接
	bool  m_blIsRadomaDelay;              //是否随机延时
	bool  m_blIsRecv;                     //是否接收回应包
	bool  m_blIsBroken;                   //是否断线重连
	bool  m_blIsSendCount;                //是否随机数据包数
	bool  m_blIsWriteFile;                //是否写入文件
	bool  m_blIsSendOne;                  //是否只发一次
	bool  m_blLuaAdvance;                 //是否启动Lua高级模式 
	int   m_nConnectType;                 //链接类型，0是TCP，1是UDP
	int   m_nUdpClientPort;               //UDP客户端接收数据端口
	int   m_nSendCount;                   //发送总数据包数
	char  m_szLuaFileName[MAX_BUFF_1024]; //高级模式的Lua文件名
	CBaseDataLogic* m_pLogic;             //数据对象  

	_Socket_Info()
	{
		m_szSerevrIP[0]    = '\0';
		m_nPort            = 0;
		m_nThreadID        = 0;
		m_nRecvTimeout     = 0;
		m_nPacketTimewait  = 0;
		m_nDelaySecond     = 0;
		//m_nSendLength      = 0;
		//m_nRecvLength      = 0;
		m_nSendCount       = 0;
		//m_pSendBuff        = NULL;
		m_blIsAlwayConnect = false;
		m_blIsRadomaDelay  = false;
		m_blIsRecv         = true;
		m_blIsBroken       = true;
		m_blIsSendCount    = false;
		m_blIsWriteFile    = false;
		m_blIsSendOne      = false;
		m_blLuaAdvance     = false;
		m_nConnectType     = 0;
		m_nUdpClientPort   = 0;
		m_szLuaFileName[0] = '\0';
		m_pLogic           = NULL;
	}

	~_Socket_Info()
	{
		if(m_pLogic != NULL)
		{
			delete m_pLogic; 
		}
	}

};

//线程运行状态信息
struct _Socket_State_Info
{
	int m_nSuccessConnect;            //连接成功数
	int m_nSuccessSend;               //发送成功数
	int m_nSuccessRecv;               //接收成功数
	int m_nCurrectSocket;             //当前链接数
	int m_nFailConnect;               //连接失败数
	int m_nFailSend;                  //发送失败数
	int m_nFailRecv;                  //接收失败数
	int m_nSendByteCount;             //发送字节数
	int m_nRecvByteCount;             //接收字节数
	int m_nMinRecvTime;               //最小接收时间
	int m_nMaxRecvTime;               //最大接收时间 

	_Socket_State_Info()
	{
		m_nSuccessConnect = 0;
		m_nSuccessSend    = 0;
		m_nSuccessRecv    = 0;
		m_nCurrectSocket  = 0;
		m_nFailConnect    = 0;
		m_nFailSend       = 0;
		m_nFailRecv       = 0;
		m_nSendByteCount  = 0;
		m_nRecvByteCount  = 0;
		m_nMinRecvTime    = 0;
		m_nMaxRecvTime    = 0;
	}
};


#endif
