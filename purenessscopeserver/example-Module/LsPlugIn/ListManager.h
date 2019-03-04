#ifndef _LISTMANAGER_H
#define _LISTMANAGER_H

//管理服务器列表
//add by freeeyes

#include "define.h"
#include "IBuffPacket.h"
#include "MD5.h"
#include "iniparser.h"

#define LS_LIST_FILENAME "ListServer_LS.ini"  //本地化存储的文件名s

//LG服务器配置信息
struct _LG_Info
{
    uint32      m_u4ConnectID;       //LG连接ID
    uint32      m_u4LGID;            //LGID
    char        m_szIP[50];          //LG IP地址
    uint32      m_u4Port;            //LG 端口
    char        m_szMD5[33];         //LG服务器当前验证码
    char        m_szSession[33];     //LG服务器当前的Session
    uint16      m_u2Version;         //LG当前版本号

    _LG_Info()
    {
        m_u4ConnectID  = 0;
        m_u4LGID       = 0;
        m_szIP[0]      = '\0';
        m_u4Port       = 0;
        m_szMD5[0]     = '\0';
        m_szSession[0] = '\0';
        m_u2Version    = 0;
    }
};

struct _LG_Target_Info
{
    int         m_nConnectID;
    uint32      m_u4LGID;
    char        m_szIP[50];
    uint32      m_u4Port;

    _LG_Target_Info()
    {
        m_nConnectID   = -1;
        m_u4LGID       = 0;
        m_szIP[0]      = '\0';
        m_u4Port       = 0;
    }
};


class ClistManager
{
public:
    ClistManager();
    ~ClistManager();

    void Clear();

    void Add_LG_Info(uint32 u4ConnectID, uint32 u4LGID, const char* pIP, uint32 u4Port,
                     const char* pMD5, const char* pSession, uint16 u2Version);
    _LG_Info* Get_LG_Info_By_LGID(uint32 u4LGID);
    _LG_Info* Get_LG_Info_By_ConnectID(uint32 u4ConnectID);
    _LG_Info* Get_LG_Info_By_Index(uint32 u4Index);
    void Del_LG_Info_By_ConnectID(uint32 u4ConnectID);
    uint32 Get_LG_Count();

    //得到完整服务器列表信息
    void Get_All_LG_List(IBuffPacket* pbpList, uint32& u4Count);

    //得到指定的列表版本信息
    char* Get_MD5_Data();

    //读取监控列表
    void ReadList();

    //获得是否全部监控已经到位
    bool Get_All_Target_list_Finish();

private:
    void Check_LG_Target(uint32 u4ConnectID, uint32 u4LGID, const char* pIP, uint32 u4Port);
    void Compare_MD5();

private:
    typedef vector<_LG_Info>        vecLGInfo;
    typedef vector<_LG_Target_Info> vecLGTargetIndo;
    vecLGInfo                       m_vecLGInfo;
    vecLGTargetIndo                 m_vecLGTargetInfo;
    char                            m_szMD5[33];
    ACE_Recursive_Thread_Mutex      m_ThreadLock;
};

#endif
