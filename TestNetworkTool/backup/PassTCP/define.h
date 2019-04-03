#ifndef _DEFINE_H
#define _DEFINE_H

#include <WinSock2.h>
#include <time.h>
#include <string>
#include <vector>

using namespace std;

#define MAX_BUFF_20   20
#define MAX_BUFF_100  100
#define MAX_BUFF_1024 1024

#define MAX_RANDOM_PACKET 5*1024

//字符串替换函数
inline void string_replace( string&s1,const string&s2,const string&s3 )
{	
	string::size_type pos = 0;
	string::size_type a = s2.size();
	string::size_type b = s3.size();
	while((pos=s1.find(s2,pos))!=string::npos)
	{
		s1.replace(pos,a,s3);
		pos += b;
	}
}

//随机数据包信息
struct _RandomPacketInfo
{
	char szPacket[MAX_RANDOM_PACKET];   //保存随机数据包
	int nLen;                           //随机数据包长度
	int nType;                          //数据类型，默认是文本，文本1，二进制2
	int nRecdvLength;                   //接收字符串的长度  

	_RandomPacketInfo()
	{
		szPacket[0]  = '\0';
		nLen         = 0;
		nType        = 1;
		nRecdvLength = 0;
	}
};

//保存随机数据包序列
typedef vector<_RandomPacketInfo> vecRandomPacketInfo;

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

//WebSocket加密算法
inline void WebSocketSendData(const char* pData, int nLen, char* pSendData, int& nSendLen)
{
	//目前这个函数只支持小于64K的数据包
	char szMark[4]  = {'\0'};
	int nPos = 0;

	if(nSendLen < nLen + 8)
	{
		//缓冲长度不够
		return;
	}

	if(nLen <= 125) 
	{
		pSendData[nPos] = -127;
		nPos++;

		//发送数字长度
		pSendData[nPos] =  (char)(nLen & 0x7F);
		nPos++;

		//4字节的mark
		memcpy_s((char* )szMark, 4, &nLen, 4);
		memcpy_s((char* )&pSendData[nPos], 4, szMark, 4);
		nPos += 4;

		//按位加密
		for(int i = 0; i < nLen; i++)
		{
			char szTemp = pData[i] ^ (szMark[i % 4]);
			pSendData[nPos] = szTemp;
			nPos++;
		}
	}
	else
	{
		pSendData[nPos] = -127;
		nPos++;

		//发送数字长度
		pSendData[nPos] =  126;
		nPos++;

		//数据实际长度。最大65535
		//转换成网络字序
		short sDataLen = htons((short)nLen);
		memcpy_s((char* )&pSendData[nPos], 2, (char* )&sDataLen, 2);
		nPos += 2;

		//4字节的mark
		memcpy_s((char* )szMark, 4, &nLen, 4);
		memcpy_s((char* )&pSendData[nPos], 4, szMark, 4);
		nPos += 4;

		//按位加密
		for(int i = 0; i < nLen; i++)
		{
			char szTemp = pData[i] ^ (szMark[i % 4]);
			pSendData[nPos] = szTemp;
			nPos++;
		}
	}


	nSendLen = nPos;
}

//类型
enum ENUM_TYPE_PROTOCOL
{
	ENUM_PROTOCOL_TCP = 0,
	ENUM_PROTOCOL_UDP,
	ENUM_PROTOCOL_WEBSOCKET,
};

//二进制和字符串相互转换类
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
	CBaseDataLogic()
	{
		m_blRandomPacket = false;
		m_nClassTye      = 1;
	}

	void ClearRandomPacket()
	{
		m_vecRandomPacketInfo.clear();
	}

	void DeleteRandomPacket(int nIndex)
	{
		if(nIndex >= 0 && nIndex < (int)m_vecRandomPacketInfo.size())
		{
			int nPos = 0;
			//删除指定数据
			for(vecRandomPacketInfo::iterator b = m_vecRandomPacketInfo.begin();b != m_vecRandomPacketInfo.end(); b++)
			{
				if(nIndex == nPos)
				{
					m_vecRandomPacketInfo.erase(b);
					break;
				}
			}
		}
	}

	//随机包指令集
	bool InsertRandomPacket(const char* pData, int nLen, int nRecvLength, int nType)
	{
		_RandomPacketInfo objRandomPacketInfo;

		if(nLen >= MAX_RANDOM_PACKET || nLen <= 0)
		{
			return false;
		}

		memcpy_s(objRandomPacketInfo.szPacket, nLen, pData, nLen);
		objRandomPacketInfo.szPacket[nLen] = '\0';
		objRandomPacketInfo.nLen           = nLen;
		objRandomPacketInfo.nRecdvLength   = nRecvLength;
		objRandomPacketInfo.nType          = nType;

		m_vecRandomPacketInfo.push_back(objRandomPacketInfo);

		if(m_blRandomPacket == false)
		{
			m_blRandomPacket = true;
		}

		return true;
	};

	//得到随机命令包的个数
	int  GetRandomPacketCount()
	{
		return (int)m_vecRandomPacketInfo.size();
	};

	_RandomPacketInfo* GettRandomPacket(int nIndex)
	{
		if(nIndex >= (int)m_vecRandomPacketInfo.size() || nIndex < 0)
		{
			return NULL;
		}
		else
		{
			return (_RandomPacketInfo* )&m_vecRandomPacketInfo[nIndex];
		}
	};

	//得到发送数据的具体信息
	bool GetRandomSend(int nIndex, char* pData, int& nLen, int& nRecvLength)
	{
		int nPos = nIndex % (int)m_vecRandomPacketInfo.size();

		_RandomPacketInfo* pRandomPacketInfo = GettRandomPacket(nPos);
		if(NULL == pRandomPacketInfo)
		{
			return false;
		}

		if(nLen < pRandomPacketInfo->nLen)
		{
			return false;
		}

		if(pRandomPacketInfo->nType == 1)
		{
			//只有文本模式的是时候进行数字替换
			string strData = (string)pRandomPacketInfo->szPacket;

			ReplaceNumber(strData, (string)"%01d", 1);
			ReplaceNumber(strData, (string)"%02d", 2);
			ReplaceNumber(strData, (string)"%03d", 3);
			ReplaceNumber(strData, (string)"%04d", 4);

			memcpy_s(pData, strData.length(), strData.c_str(), strData.length());
			pData[strData.length()] = '\0';
			nLen = strData.length();
			nRecvLength = pRandomPacketInfo->nRecdvLength;
		}
		else
		{
			//进行二进制转换
			CConvertBuffer objConvertBuffer;

			//存入数据
			objConvertBuffer.Convertstr2charArray(pRandomPacketInfo->szPacket, pRandomPacketInfo->nLen, 
				(unsigned char*)pData, nLen);
			nRecvLength = pRandomPacketInfo->nRecdvLength;
		}

		return true;
	}

	//显示预览
	bool GetReview(int nIndex, char* pData, int& nLen, int& nRecvLength)
	{
		int nPos = nIndex % (int)m_vecRandomPacketInfo.size();

		_RandomPacketInfo* pRandomPacketInfo = GettRandomPacket(nPos);
		if(NULL == pRandomPacketInfo)
		{
			return false;
		}

		if(nLen < pRandomPacketInfo->nLen)
		{
			return false;
		}

		if(pRandomPacketInfo->nType == 1)
		{
			//只有文本模式的是时候进行数字替换
			string strData = (string)pRandomPacketInfo->szPacket;

			ReplaceNumber(strData, (string)"%01d", 1);
			ReplaceNumber(strData, (string)"%02d", 2);
			ReplaceNumber(strData, (string)"%03d", 3);
			ReplaceNumber(strData, (string)"%04d", 4);

			memcpy_s(pData, strData.length(), strData.c_str(), strData.length());
			pData[strData.length()] = '\0';
			nLen = strData.length();
			nRecvLength = pRandomPacketInfo->nRecdvLength;
		}
		else
		{
			//存入数据
			memcpy_s(pData, pRandomPacketInfo->nLen, pRandomPacketInfo->szPacket, pRandomPacketInfo->nLen);
			nLen = pRandomPacketInfo->nLen;
			nRecvLength = pRandomPacketInfo->nRecdvLength;
		}

		return true;
	}

	//得到是否为随机数据包
	bool GetRandomType()
	{
		return m_blRandomPacket;
	}

	//正常指令集
	virtual bool InitSendSize(int nSendLen)                                     = 0;
	virtual char* GetSendData()                                                 = 0;
	virtual char* GetSendData(int nThreadID, int nCurrIndex, int& nSendDataLen) = 0;
	virtual int GetSendLength()                                                 = 0;                   
	virtual int GetRecvLength()                                                 = 0;
	virtual void SetRecvLength(int nRecvLen)                                    = 0;
	virtual void SetMaxSendLength(int nMaxLength)                               = 0;
	virtual void SetSendBuff(const char* pData, int nLen)                       = 0;
	virtual EM_DATA_RETURN_STATE GetRecvData(int nThreadID, int nCurrIndex, char* pData, int nLen) = 0;

private:
	void ReplaceNumber(string& strData, string strTag, int nStep)
	{
		if(nStep == 1)
		{
			char szNumber[20] = {'\0'}; 
			int nNumber = RandomValue(0, 9);
			sprintf_s(szNumber, 20, "%d", nNumber);
			string_replace(strData, strTag, (string)szNumber);
		}
		else if(nStep == 2)
		{
			char szNumber[20] = {'\0'}; 
			int nNumber = RandomValue(0, 99);
			sprintf_s(szNumber, 20, "%d", nNumber);
			string_replace(strData, strTag, (string)szNumber);
		}
		else if(nStep == 3)
		{
			char szNumber[20] = {'\0'}; 
			int nNumber = RandomValue(0, 999);
			sprintf_s(szNumber, 20, "%d", nNumber);
			string_replace(strData, strTag, (string)szNumber);
		}
		else if(nStep == 4)
		{
			char szNumber[20] = {'\0'}; 
			int nNumber = RandomValue(0, 9999);
			sprintf_s(szNumber, 20, "%d", nNumber);
			string_replace(strData, strTag, (string)szNumber);
		}
	}

private:
	bool  m_blRandomPacket;     //是否包含随机包列，falase为不包含，true为包含
	vecRandomPacketInfo m_vecRandomPacketInfo;

public:
	int m_nClassTye;    //1为CNomalLogic，2为WebSocketLogic
};

//普通的TCP收发相关信息处理逻辑
//为了兼容一些特殊协议，比如websocket以及http
//收发数据不再是单独的一个指针，而是一个继承CBaseDataLogic的类
class CNomalLogic : public CBaseDataLogic
{
public:
	CNomalLogic() 
	{ 
		m_pSendData      = NULL;
		m_nSendLen       = 0;
		m_nRecvLen       = 0;
		m_nCurrRecvLen   = 0;
		m_nClassTye      = 1;
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

	bool InitSendSize(int nSendLen = MAX_RANDOM_PACKET)
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

	char* GetSendData(int nThreadID, int nCurrIndex, int& nSendDataLen)
	{
		if(GetRandomType() == false)
		{
			//单一数据包
			nSendDataLen = m_nSendLen;
			return m_pSendData;
		}
		else
		{
			//顺序数据包
			nSendDataLen = MAX_RANDOM_PACKET;
			GetRandomSend(nCurrIndex, m_pSendData, nSendDataLen, m_nRecvLen);
			return m_pSendData;
		}
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

//websocket协议逻辑包
class CWebSocketLogic : public CBaseDataLogic
{
public:
	CWebSocketLogic() 
	{ 
		m_pHandInData    = NULL;
		m_pSendData      = NULL;
		m_nSendLen       = 0;
		m_nRecvLen       = 0;
		m_nCurrRecvLen   = 0;
		m_nClassTye      = 2;
	};

	~CWebSocketLogic() { Close(); };

	void Close()
	{
		if(NULL != m_pHandInData)
		{
			delete[] m_pHandInData;
			m_pHandInData = NULL;
		}

		if(NULL != m_pSendData)
		{
			delete[] m_pSendData;
			m_pSendData = NULL;
		}
	}

	bool InitSendSize(int nSendLen = MAX_RANDOM_PACKET)
	{
		Close();

		m_pHandInData = new char[300];
		sprintf_s(m_pHandInData, 300, "GET / HTTP/1.1\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nHost: 127.0.0.1:10002\r\nSec-WebSocket-Key: cfMpieGIwzS7+4+nIqv5fA==\r\nSec-WebSocket-Version: 13\r\n\r\n");

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
		//WebSocket加密数据
		WebSocketSendData(pData, nLen, m_pSendData, m_nSendLen);
	}

	char* GetSendData()
	{
		return m_pSendData;
	}

	char* GetSendData(int nThreadID, int nCurrIndex, int& nSendDataLen)
	{
		//这里添加websocket加密善法
		if(nCurrIndex == 0)
		{
			//如果是第一次发送数据，则发送握手包
			nSendDataLen = strlen(m_pHandInData);
			return m_pHandInData;
		}
		else
		{
			if(GetRandomType() == false)
			{
				//单一数据包
				nSendDataLen = m_nSendLen;
				return m_pSendData;
			}
			else
			{
				//顺序数据包
				nSendDataLen = MAX_RANDOM_PACKET;
				GetRandomSend(nCurrIndex - 1, m_pSendData, nSendDataLen, m_nRecvLen);
				return m_pSendData;
			}
		}
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
		return DATA_RETURN_STATE_SUCCESS;
	}

private:
	char* m_pHandInData;
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
		//这里不再管释放，交由上层解决
		//if(m_pLogic != NULL)
		//{
		//	delete m_pLogic; 
		//}
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
