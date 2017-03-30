#include "LoadPacketParse.h"

CLoadPacketParse::CLoadPacketParse()
{
}

CLoadPacketParse::~CLoadPacketParse()
{
    OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::~CLoadPacketParse]\n"));
    //Close();
}

bool CLoadPacketParse::LoadPacketInfo(char* szPacketParseName)
{
    //隐式加载PacketParse接口
    m_Packet_Parse_Info.hModule = ACE_OS::dlopen((ACE_TCHAR*)szPacketParseName, RTLD_NOW);

    if(NULL == m_Packet_Parse_Info.hModule)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, pModuleInfo->hModule is NULL(%s)!\n", szPacketParseName, ACE_OS::dlerror()));
        return false;
    }

    m_Packet_Parse_Info.Parse_Packet_Head_Info = (bool(*)(uint32, ACE_Message_Block*, IMessageBlockManager*, _Head_Info*))ACE_OS::dlsym(m_Packet_Parse_Info.hModule, "Parse_Packet_Head_Info");

    if(NULL == m_Packet_Parse_Info.hModule || !m_Packet_Parse_Info.Parse_Packet_Head_Info)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function Parse_Packet_Head_Info is error!\n", szPacketParseName));
        return false;
    }

    m_Packet_Parse_Info.Parse_Packet_Body_Info = (bool(*)(uint32, ACE_Message_Block*, IMessageBlockManager*, _Body_Info*))ACE_OS::dlsym(m_Packet_Parse_Info.hModule, "Parse_Packet_Body_Info");

    if(NULL == m_Packet_Parse_Info.hModule || !m_Packet_Parse_Info.Parse_Packet_Body_Info)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function Parse_Packet_Body_Info is error!\n", szPacketParseName));
        return false;
    }

    m_Packet_Parse_Info.Parse_Packet_Stream = (uint8(*)(uint32, ACE_Message_Block*, IMessageBlockManager*, _Packet_Info*))ACE_OS::dlsym(m_Packet_Parse_Info.hModule, "Parse_Packet_Stream");

    if(NULL == m_Packet_Parse_Info.hModule || !m_Packet_Parse_Info.Parse_Packet_Stream)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function Parse_Packet_Stream is error!\n", szPacketParseName));
        return false;
    }

    m_Packet_Parse_Info.Make_Send_Packet = (bool(*)(uint32, const char*, uint32, ACE_Message_Block*, uint16))ACE_OS::dlsym(m_Packet_Parse_Info.hModule, "Make_Send_Packet");

    if(NULL == m_Packet_Parse_Info.hModule || !m_Packet_Parse_Info.Make_Send_Packet)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function Make_Send_Packet is error!\n", szPacketParseName));
        return false;
    }

    m_Packet_Parse_Info.Make_Send_Packet_Length = (uint32(*)(uint32, uint32, uint16))ACE_OS::dlsym(m_Packet_Parse_Info.hModule, "Make_Send_Packet_Length");

    if(NULL == m_Packet_Parse_Info.hModule || !m_Packet_Parse_Info.Make_Send_Packet_Length)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function Make_Send_Packet_Length is error!\n", szPacketParseName));
        return false;
    }

    m_Packet_Parse_Info.Connect = (bool(*)(uint32, _ClientIPInfo, _ClientIPInfo))ACE_OS::dlsym(m_Packet_Parse_Info.hModule, "Connect");

    if(NULL == m_Packet_Parse_Info.hModule || !m_Packet_Parse_Info.Connect)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function Connect is error!\n", szPacketParseName));
        return false;
    }

    m_Packet_Parse_Info.DisConnect = (void(*)(uint32))ACE_OS::dlsym(m_Packet_Parse_Info.hModule, "DisConnect");

    if(NULL == m_Packet_Parse_Info.hModule || !m_Packet_Parse_Info.DisConnect)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function DisConnect is error!\n", szPacketParseName));
        return false;
    }

    return true;
}

_Packet_Parse_Info* CLoadPacketParse::GetPacketParseInfo()
{
    return (_Packet_Parse_Info* )&m_Packet_Parse_Info;
}

void CLoadPacketParse::Close()
{
    if(m_Packet_Parse_Info.hModule != NULL)
    {
        int nRet = ACE_OS::dlclose(m_Packet_Parse_Info.hModule);
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::Close]%d.\n", nRet));
    }
}
