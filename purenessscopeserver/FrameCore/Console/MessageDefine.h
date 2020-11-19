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
#include <string>

//�������ֵ���Ͷ���
enum class EM_CONSOLE_MESSAGE
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

    _CommandInfo() = default;
};

//�ļ����ṹ
class _FileInfo
{
public:
    string m_strFilePath;
    string m_strFileName;
    string m_strFileParam;

    _FileInfo() = default;
};

//�����˿���Ϣ
class _ListenInfo
{
public:
    uint16 m_u2Port                  = 0;
    uint32 m_u4PacketParseID         = 0;
    uint8  m_u1IPType                = TYPE_IPV4;
    string m_strListenIP;

    _ListenInfo() = default;
};

//ȾɫIP��Ϣ
class _DyeIPInfo
{
public:
    string m_strClientIP             = {'\0'};   //Ⱦɫ�ͻ���IP
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
    string m_strPoolName             = {'\0'};   //�ڴ������
    bool   m_blState                 = false;    //��ǰ�ڴ�ش�����Ϣ״̬
};

//Э�����, �������ݲ���
bool GetCommandParam(const char* pCommand, const char* pTag, string& strValue);                           //������ָ����Command����
bool GetFileInfo(const char* pFile, _FileInfo& FileInfo);                                                 //��һ��ȫ·���зֳ��ļ���
bool GetForbiddenIP(const char* pCommand, _ForbiddenIP& ForbiddenIP);                                     //�õ���ֹ��IP�б�
bool GetConnectServerID(const char* pCommand, int& nServerID);                                            //�õ�һ��ָ���ķ�����ID
bool GetDebug(const char* pCommand, uint8& u1Debug);                                                      //�õ���ǰ���õ�BUDEG
bool GetTrackIP(const char* pCommand, _ForbiddenIP& ForbiddenIP);                                         //�õ����õ�׷��IP
bool GetLogLevel(const char* pCommand, int& nLogLevel);                                                   //�õ���־�ȼ�
bool GetAIInfo(const char* pCommand, int& nAI, int& nDispose, int& nCheck, int& nStop);                   //�õ�AI����
bool GetNickName(const char* pCommand, string& strName);                                                  //�õ����ӱ���
bool GetConnectID(const char* pCommand, uint32& u4ConnectID, bool& blFlag);                               //�õ�ConnectID
bool GetMaxConnectCount(const char* pCommand, uint16& u2MaxConnectCount);                                 //�õ�������������
bool GetListenInfo(const char* pCommand, _ListenInfo& objListenInfo);                                     //�õ������˿���Ϣ
bool GetTestFileName(const char* pCommand, string& strFileName);                                          //��ü��ز����ļ���
bool GetDyeingIP(const char* pCommand, _DyeIPInfo& objDyeIPInfo);                                         //���ȾɫIP�������Ϣ
bool GetDyeingCommand(const char* pCommand, _DyeCommandInfo& objDyeCommandInfo);                          //���ȾɫCommand�������Ϣ
bool GetPoolSet(const char* pCommand, _PoolName& objPoolName);                                            //����ڴ�ص�����

#endif
