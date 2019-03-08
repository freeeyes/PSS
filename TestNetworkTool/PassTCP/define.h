#ifndef _DEFINE_H
#define _DEFINE_H

//#include <WinSock2.h>
#include <arpa/inet.h>
#include <time.h>
#include <string>
#include <vector>
#include <cstring>



using namespace std;

#define MAX_BUFF_20   20
#define MAX_BUFF_100  100
#define MAX_BUFF_1024 1024

#define MAX_RANDOM_PACKET 5*1024

//�ַ����滻����
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

//������ݰ���Ϣ
struct _RandomPacketInfo
{
	char szPacket[MAX_RANDOM_PACKET];   //����������ݰ�
	int nLen;                           //������ݰ�����
	int nType;                          //�������ͣ�Ĭ�����ı����ı�1��������2
	int nRecdvLength;                   //�����ַ����ĳ���

	_RandomPacketInfo()
	{
		szPacket[0]  = '\0';
		nLen         = 0;
		nType        = 1;
		nRecdvLength = 0;
	}
};

//����������ݰ�����
typedef vector<_RandomPacketInfo> vecRandomPacketInfo;

//����һ���������
inline void InitRandom()
{
	srand((int)time(NULL));
};

//��һ��ֵ���л�ȡһ�����ֵ
inline int RandomValue(int nMin, int nMax)
{
	return  nMin + (int) ((nMax - nMin) * (rand() / (RAND_MAX + 1.0)));
};

//WebSocket�����㷨
inline void WebSocketSendData(const char* pData, int nLen, char* pSendData, int& nSendLen)
{
	//Ŀǰ�������ֻ֧��С��64K�����ݰ�
	char szMark[4]  = {'\0'};
	int nPos = 0;

	if(nSendLen < nLen + 8)
	{
		//���峤�Ȳ���
		return;
	}

	if(nLen <= 125)
	{
		pSendData[nPos] = -127;
		nPos++;

		//�������ֳ���
		pSendData[nPos] =  (char)(nLen & 0x7F);
		nPos++;

		//4�ֽڵ�mark
		memcpy((char* )szMark,  &nLen, 4);
		memcpy((char* )&pSendData[nPos],  szMark, 4);
		nPos += 4;

		//��λ����
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

		//�������ֳ���
		pSendData[nPos] =  126;
		nPos++;

		//����ʵ�ʳ��ȡ����65535
		//ת������������
		short sDataLen = htons((short)nLen);
		memcpy((char* )&pSendData[nPos],  (char* )&sDataLen, 2);
		nPos += 2;

		//4�ֽڵ�mark
		memcpy((char* )szMark,  &nLen, 4);
		memcpy((char* )&pSendData[nPos],  szMark, 4);
		nPos += 4;

		//��λ����
		for(int i = 0; i < nLen; i++)
		{
			char szTemp = pData[i] ^ (szMark[i % 4]);
			pSendData[nPos] = szTemp;
			nPos++;
		}
	}


	nSendLen = nPos;
}

//����
enum ENUM_TYPE_PROTOCOL
{
	ENUM_PROTOCOL_TCP = 0,
	ENUM_PROTOCOL_UDP,
	ENUM_PROTOCOL_WEBSOCKET,
};

//�����ƺ��ַ����໥ת����
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
		bool blState     = false;   //ת������ַ����Ƿ���Ч
		bool blSrcState  = true;    //Ԫ�ַ����Ƿ���Ч
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
		bool blState     = false;   //ת������ַ����Ƿ���Ч
		bool blSrcState  = true;    //Ԫ�ַ����Ƿ���Ч

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

//����һ�����࣬�������ʽ�����ݷ��ͺͽ��յĲ����߼�
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
			//ɾ��ָ������
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

	//�����ָ�
	bool InsertRandomPacket(const char* pData, int nLen, int nRecvLength, int nType)
	{
		_RandomPacketInfo objRandomPacketInfo;

		if(nLen >= MAX_RANDOM_PACKET || nLen <= 0)
		{
			return false;
		}

		memcpy(objRandomPacketInfo.szPacket,  pData, nLen);
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

	//�õ����������ĸ���
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

	//�õ��������ݵľ�����Ϣ
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
			//ֻ���ı�ģʽ����ʱ����������滻
			string strData = (string)pRandomPacketInfo->szPacket;

			ReplaceNumber(strData, (string)"%01d", 1);
			ReplaceNumber(strData, (string)"%02d", 2);
			ReplaceNumber(strData, (string)"%03d", 3);
			ReplaceNumber(strData, (string)"%04d", 4);

			memcpy(pData,  strData.c_str(), strData.length());
			pData[strData.length()] = '\0';
			nLen = strData.length();
			nRecvLength = pRandomPacketInfo->nRecdvLength;
		}
		else
		{
			//���ж�����ת��
			CConvertBuffer objConvertBuffer;

			//��������
			objConvertBuffer.Convertstr2charArray(pRandomPacketInfo->szPacket, pRandomPacketInfo->nLen,
				(unsigned char*)pData, nLen);
			nRecvLength = pRandomPacketInfo->nRecdvLength;
		}

		return true;
	}

	//��ʾԤ��
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
			//ֻ���ı�ģʽ����ʱ����������滻
			string strData = (string)pRandomPacketInfo->szPacket;

			ReplaceNumber(strData, (string)"%01d", 1);
			ReplaceNumber(strData, (string)"%02d", 2);
			ReplaceNumber(strData, (string)"%03d", 3);
			ReplaceNumber(strData, (string)"%04d", 4);

			memcpy(pData, strData.c_str(), strData.length());
			pData[strData.length()] = '\0';
			nLen = strData.length();
			nRecvLength = pRandomPacketInfo->nRecdvLength;
		}
		else
		{
			//��������
			memcpy(pData, pRandomPacketInfo->szPacket, pRandomPacketInfo->nLen);
			nLen = pRandomPacketInfo->nLen;
			nRecvLength = pRandomPacketInfo->nRecdvLength;
		}

		return true;
	}

	//�õ��Ƿ�Ϊ������ݰ�
	bool GetRandomType()
	{
		return m_blRandomPacket;
	}

	//����ָ�
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
			sprintf(szNumber,  "%d", nNumber);
			string_replace(strData, strTag, (string)szNumber);
		}
		else if(nStep == 2)
		{
			char szNumber[20] = {'\0'};
			int nNumber = RandomValue(0, 99);
			sprintf(szNumber,  "%d", nNumber);
			string_replace(strData, strTag, (string)szNumber);
		}
		else if(nStep == 3)
		{
			char szNumber[20] = {'\0'};
			int nNumber = RandomValue(0, 999);
			sprintf(szNumber,  "%d", nNumber);
			string_replace(strData, strTag, (string)szNumber);
		}
		else if(nStep == 4)
		{
			char szNumber[20] = {'\0'};
			int nNumber = RandomValue(0, 9999);
			sprintf(szNumber,  "%d", nNumber);
			string_replace(strData, strTag, (string)szNumber);
		}
	}

private:
	bool  m_blRandomPacket;     //�Ƿ����������У�falaseΪ��������trueΪ����
	vecRandomPacketInfo m_vecRandomPacketInfo;

public:
	int m_nClassTye;    //1ΪCNomalLogic��2ΪWebSocketLogic
};

//��ͨ��TCP�շ������Ϣ�����߼�
//Ϊ�˼���һЩ����Э�飬����websocket�Լ�http
//�շ����ݲ����ǵ�����һ��ָ�룬����һ���̳�CBaseDataLogic����
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

	//������ط���Buff
	void SetSendBuff(const char* pData, int nLen)
	{
		memcpy(m_pSendData,  pData, nLen);
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
			//��һ���ݰ�
			nSendDataLen = m_nSendLen;
			return m_pSendData;
		}
		else
		{
			//˳�����ݰ�
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
			//ȫ��������ϣ�������ȷ
			return DATA_RETURN_STATE_SUCCESS;
		}
		else if(nLen < m_nRecvLen)
		{
			//û�н�����ȫ����������
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

//websocketЭ���߼���
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
		sprintf(m_pHandInData,  "GET / HTTP/1.1\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nHost: 127.0.0.1:10002\r\nSec-WebSocket-Key: cfMpieGIwzS7+4+nIqv5fA==\r\nSec-WebSocket-Version: 13\r\n\r\n");

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

	//������ط���Buff
	void SetSendBuff(const char* pData, int nLen)
	{
		//WebSocket��������
		WebSocketSendData(pData, nLen, m_pSendData, m_nSendLen);
	}

	char* GetSendData()
	{
		return m_pSendData;
	}

	char* GetSendData(int nThreadID, int nCurrIndex, int& nSendDataLen)
	{
		//�������websocket�����Ʒ�
		if(nCurrIndex == 0)
		{
			//����ǵ�һ�η������ݣ��������ְ�
			nSendDataLen = strlen(m_pHandInData);
			return m_pHandInData;
		}
		else
		{
			if(GetRandomType() == false)
			{
				//��һ���ݰ�
				nSendDataLen = m_nSendLen;
				return m_pSendData;
			}
			else
			{
				//˳�����ݰ�
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

//�߳�������Ϣ
class _Socket_Info
{
public:
	char  m_szSerevrIP[MAX_BUFF_20];      //Զ�̷�������IP
	int   m_nPort;                        //Զ�̷������Ķ˿�
	int   m_nThreadID;                    //�߳�ID
	int   m_nRecvTimeout;                 //�������ݳ�ʱʱ�䣨��λ�Ǻ��룩
	int   m_nDelaySecond;                 //�����Ӽ���ʱ����λ�Ǻ��룩
	int   m_nPacketTimewait;              //���ݰ����ͼ��(��λ�Ǻ���)
	//int   m_nSendLength;                //�����ַ�������
	//int   m_nRecvLength;                //�����ַ��������޶�
	//char* m_pSendBuff;                  //�������ݳ���
	bool  m_blIsAlwayConnect;             //�Ƿ�����
	bool  m_blIsRadomaDelay;              //�Ƿ������ʱ
	bool  m_blIsRecv;                     //�Ƿ���ջ�Ӧ��
	bool  m_blIsBroken;                   //�Ƿ��������
	bool  m_blIsSendCount;                //�Ƿ�������ݰ���
	bool  m_blIsWriteFile;                //�Ƿ�д���ļ�
	bool  m_blIsSendOne;                  //�Ƿ�ֻ��һ��
	bool  m_blLuaAdvance;                 //�Ƿ�����Lua�߼�ģʽ
	int   m_nConnectType;                 //�������ͣ�0��TCP��1��UDP
	int   m_nUdpClientPort;               //UDP�ͻ��˽������ݶ˿�
	int   m_nSendCount;                   //���������ݰ���
	char  m_szLuaFileName[MAX_BUFF_1024]; //�߼�ģʽ��Lua�ļ���
	CBaseDataLogic* m_pLogic;             //���ݶ���

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
		//���ﲻ�ٹ��ͷţ������ϲ���
		//if(m_pLogic != NULL)
		//{
		//	delete m_pLogic;
		//}
	}

};

//�߳�����״̬��Ϣ
struct _Socket_State_Info
{
	int m_nSuccessConnect;            //���ӳɹ���
	int m_nSuccessSend;               //���ͳɹ���
	int m_nSuccessRecv;               //���ճɹ���
	int m_nCurrectSocket;             //��ǰ������
	int m_nFailConnect;               //����ʧ����
	int m_nFailSend;                  //����ʧ����
	int m_nFailRecv;                  //����ʧ����
	int m_nSendByteCount;             //�����ֽ���
	int m_nRecvByteCount;             //�����ֽ���
	int m_nMinRecvTime;               //��С����ʱ��
	int m_nMaxRecvTime;               //������ʱ��

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
