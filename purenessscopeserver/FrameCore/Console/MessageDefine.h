#ifndef _MESSAGEDEFINE_H
#define _MESSAGEDEFINE_H

//���ﴦ�����еĽ������������
//��Щ������Ծ�̬�����ķ�ʽ��¶����
//���������ڴ�����Ķ�����Ϣ��ӳ��
//��ô����ԭ���ǣ����������if else ��������
//�Ż�����ṹ������sonar��������
//add by freeeyes

#include <ace/OS_NS_sys_resource.h>

#include "define.h"
#include "ForbiddenIP.h"

//�������ֵ���Ͷ���
enum
{
    CONSOLE_MESSAGE_SUCCESS = 0,
    CONSOLE_MESSAGE_FAIL    = 1,
    CONSOLE_MESSAGE_CLOSE   = 2,
};

static const char COMMAND_SPLIT_STRING[] = " ";

//��������
class _CommandInfo
{
public:
    uint8 m_u1OutputType                = 0;        //������ͣ�0Ϊ�����ƣ�1Ϊ�ı�
    string m_strCommandTitle;                       //��������ͷ
    string m_strCommandExp;                         //����������չ����
    string m_strUser  = {'\0'};                     //�û���Ϣ

    _CommandInfo()
    {
    }
};

//�ļ����ṹ
class _FileInfo
{
public:
    char m_szFilePath[MAX_BUFF_100]  = {'\0'};
    char m_szFileName[MAX_BUFF_100]  = {'\0'};
    char m_szFileParam[MAX_BUFF_200] = {'\0'};

    _FileInfo()
    {
    }
};

//�����˿���Ϣ
class _ListenInfo
{
public:
    uint16 m_u2Port                  = 0;
    uint32 m_u4PacketParseID         = 0;
    uint8  m_u1IPType                = TYPE_IPV4;
    char   m_szListenIP[MAX_BUFF_20] = {'\0'};

    _ListenInfo()
    {
    }
};

//ȾɫIP��Ϣ
class _DyeIPInfo
{
public:
    char   m_szClientIP[MAX_BUFF_20] = {'\0'};   //Ⱦɫ�ͻ���IP
    uint16 m_u2MaxCount              = 0;        //�������
};

//Ⱦɫ��CommandID
class _DyeCommandInfo
{
public:
    uint16 m_u2CommandID = 0;               //Ⱦɫ�ͻ�������
    uint16 m_u2MaxCount  = 0;                //�������
};

class _PoolName
{
public:
    char   m_szPoolName[MAX_BUFF_50] = {'\0'};   //�ڴ������
    bool   m_blState                 = false;    //��ǰ�ڴ�ش�����Ϣ״̬
};

//Э�����, �������ݲ���
bool GetCommandParam(const char* pCommand, const char* pTag, char* pData, int nMaxSize);                  //������ָ����Command����
bool GetFileInfo(const char* pFile, _FileInfo& FileInfo);                                                 //��һ��ȫ·���зֳ��ļ���
bool GetForbiddenIP(const char* pCommand, _ForbiddenIP& ForbiddenIP);                                     //�õ���ֹ��IP�б�
bool GetConnectServerID(const char* pCommand, int& nServerID);                                            //�õ�һ��ָ���ķ�����ID
bool GetDebug(const char* pCommand, uint8& u1Debug);                                                      //�õ���ǰ���õ�BUDEG
bool GetTrackIP(const char* pCommand, _ForbiddenIP& ForbiddenIP);                                         //�õ����õ�׷��IP
bool GetLogLevel(const char* pCommand, int& nLogLevel);                                                   //�õ���־�ȼ�
bool GetAIInfo(const char* pCommand, int& nAI, int& nDispose, int& nCheck, int& nStop);                   //�õ�AI����
bool GetNickName(const char* pCommand, char* pName);                                                      //�õ����ӱ���
bool GetConnectID(const char* pCommand, uint32& u4ConnectID, bool& blFlag);                               //�õ�ConnectID
bool GetMaxConnectCount(const char* pCommand, uint16& u2MaxConnectCount);                                 //�õ�������������
bool GetListenInfo(const char* pCommand, _ListenInfo& objListenInfo);                                     //�õ������˿���Ϣ
bool GetTestFileName(const char* pCommand, char* pFileName);                                              //��ü��ز����ļ���
bool GetDyeingIP(const char* pCommand, _DyeIPInfo& objDyeIPInfo);                                         //���ȾɫIP�������Ϣ
bool GetDyeingCommand(const char* pCommand, _DyeCommandInfo& objDyeCommandInfo);                          //���ȾɫCommand�������Ϣ
bool GetPoolSet(const char* pCommand, _PoolName& objPoolName);                                            //����ڴ�ص�����

#endif
