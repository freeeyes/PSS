#include "LoadPacketParse.h"

void CLoadPacketParse::Init(int nCount)
{
    Close();
    //初始化HashTable
    m_nModuleCount = nCount;
}

bool CLoadPacketParse::LoadPacketInfo(uint32 u4PacketParseID, uint8 u1Type, uint32 u4HeadLen, const char* pPacketParsePath, const char* szPacketParseName)
{
    int nRet = 0;
    //隐式加载PacketParse接口
    auto pPacketParseInfo = std::make_shared<_Packet_Parse_Info>();

    if (nullptr == pPacketParseInfo)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] pPacketParseInfo is nullptr!\n"));
        return false;
    }

    pPacketParseInfo->m_u4PacketParseID   = u4PacketParseID;
    pPacketParseInfo->m_u1PacketParseType = u1Type;
    pPacketParseInfo->m_u4OrgLength       = u4HeadLen;
    char szFilePath[MAX_BUFF_500]         = { '\0' };
    sprintf_safe(szFilePath, MAX_BUFF_500, "%s%s", pPacketParsePath, szPacketParseName);
    pPacketParseInfo->m_hModule           = ACE_OS::dlopen((ACE_TCHAR*)szFilePath, RTLD_NOW);

    if(nullptr == pPacketParseInfo->m_hModule)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, pModuleInfo->hModule is nullptr(%s)!\n", szPacketParseName, ACE_OS::dlerror()));
        return false;
    }

    pPacketParseInfo->Parse_Packet_Head_Info = (bool(*)(uint32, ACE_Message_Block*, IMessageBlockManager*, _Head_Info*, EM_CONNECT_IO_TYPE))ACE_OS::dlsym(pPacketParseInfo->m_hModule, "Parse_Packet_Head_Info");

    if(nullptr == pPacketParseInfo->m_hModule || !pPacketParseInfo->Parse_Packet_Head_Info)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function Parse_Packet_Head_Info is error!\n", szPacketParseName));
        nRet = ACE_OS::dlclose(pPacketParseInfo->m_hModule);
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo]PacketID=%d, ret=%d.\n", pPacketParseInfo->m_u4PacketParseID, nRet));
        return false;
    }

    pPacketParseInfo->Parse_Packet_Body_Info = (bool(*)(uint32, ACE_Message_Block*, IMessageBlockManager*, _Body_Info*, EM_CONNECT_IO_TYPE))ACE_OS::dlsym(pPacketParseInfo->m_hModule, "Parse_Packet_Body_Info");

    if(nullptr == pPacketParseInfo->m_hModule || !pPacketParseInfo->Parse_Packet_Body_Info)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function Parse_Packet_Body_Info is error!\n", szPacketParseName));
        nRet = ACE_OS::dlclose(pPacketParseInfo->m_hModule);
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo]PacketID=%d, ret=%d.\n", pPacketParseInfo->m_u4PacketParseID, nRet));
        return false;
    }

    pPacketParseInfo->Parse_Packet_Stream = (uint8(*)(uint32, ACE_Message_Block*, IMessageBlockManager*, _Packet_Info*, EM_CONNECT_IO_TYPE))ACE_OS::dlsym(pPacketParseInfo->m_hModule, "Parse_Packet_Stream");

    if(nullptr == pPacketParseInfo->m_hModule || !pPacketParseInfo->Parse_Packet_Stream)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function Parse_Packet_Stream is error!\n", szPacketParseName));
        nRet = ACE_OS::dlclose(pPacketParseInfo->m_hModule);
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo]PacketID=%d, ret=%d.\n", pPacketParseInfo->m_u4PacketParseID, nRet));
        return false;
    }

    pPacketParseInfo->Make_Send_Packet = (bool(*)(uint32, const char*, uint32, ACE_Message_Block*, uint16))ACE_OS::dlsym(pPacketParseInfo->m_hModule, "Make_Send_Packet");

    if(nullptr == pPacketParseInfo->m_hModule || !pPacketParseInfo->Make_Send_Packet)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function Make_Send_Packet is error!\n", szPacketParseName));
        nRet = ACE_OS::dlclose(pPacketParseInfo->m_hModule);
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo]PacketID=%d, ret=%d.\n", pPacketParseInfo->m_u4PacketParseID, nRet));
        return false;
    }

    pPacketParseInfo->Make_Send_Packet_Length = (uint32(*)(uint32, uint32, uint16))ACE_OS::dlsym(pPacketParseInfo->m_hModule, "Make_Send_Packet_Length");

    if(nullptr == pPacketParseInfo->m_hModule || !pPacketParseInfo->Make_Send_Packet_Length)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function Make_Send_Packet_Length is error!\n", szPacketParseName));
        nRet = ACE_OS::dlclose(pPacketParseInfo->m_hModule);
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo]PacketID=%d, ret=%d.\n", pPacketParseInfo->m_u4PacketParseID, nRet));
        return false;
    }

    pPacketParseInfo->Connect = (bool(*)(uint32, _ClientIPInfo, _ClientIPInfo const&))ACE_OS::dlsym(pPacketParseInfo->m_hModule, "Connect");

    if(nullptr == pPacketParseInfo->m_hModule || !pPacketParseInfo->Connect)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function Connect is error!\n", szPacketParseName));
        nRet = ACE_OS::dlclose(pPacketParseInfo->m_hModule);
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo]PacketID=%d, ret=%d.\n", pPacketParseInfo->m_u4PacketParseID, nRet));
        return false;
    }

    pPacketParseInfo->DisConnect = (void(*)(uint32))ACE_OS::dlsym(pPacketParseInfo->m_hModule, "DisConnect");

    if(nullptr == pPacketParseInfo->m_hModule || !pPacketParseInfo->DisConnect)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function DisConnect is error!\n", szPacketParseName));
        nRet = ACE_OS::dlclose(pPacketParseInfo->m_hModule);
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo]PacketID=%d, ret=%d.\n", pPacketParseInfo->m_u4PacketParseID, nRet));
        return false;
    }

    pPacketParseInfo->Close = (void(*)())ACE_OS::dlsym(pPacketParseInfo->m_hModule, "Close");

    if (nullptr == pPacketParseInfo->m_hModule || !pPacketParseInfo->Close)
    {
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function Close is error!\n", szPacketParseName));
        nRet = ACE_OS::dlclose(pPacketParseInfo->m_hModule);
        OUR_DEBUG((LM_ERROR, "[CLoadPacketParse::LoadPacketInfo]PacketID=%d, ret=%d.\n", pPacketParseInfo->m_u4PacketParseID, nRet));
        return false;
    }

    //添加到HashPool里面
    m_objPacketParseList[pPacketParseInfo->m_u4PacketParseID] = pPacketParseInfo;
    return true;
}

shared_ptr<_Packet_Parse_Info> CLoadPacketParse::GetPacketParseInfo(uint32 u4PacketParseID)
{
    auto f = m_objPacketParseList.find(u4PacketParseID);

    if(m_objPacketParseList.end() == f)
    {
        //这里打印输出错误
        OUR_DEBUG((LM_INFO, "[CLoadPacketParse::GetPacketParseInfo]can't find u4PacketParseID(%d).\n", u4PacketParseID));
        return nullptr;
    }
    else
    {
        return f->second;
    }
}

void CLoadPacketParse::Close()
{
    OUR_DEBUG((LM_INFO, "[CLoadPacketParse::Close]Begin.\n"));
    //清理所有已存在的指针
    for_each(m_objPacketParseList.begin(), m_objPacketParseList.end(), [](const std::pair<uint32, shared_ptr<_Packet_Parse_Info>>& iter) {
        //关闭模块接口
        iter.second->Close();
        ACE_OS::dlclose(iter.second->m_hModule);
        });

    m_objPacketParseList.clear();
    OUR_DEBUG((LM_INFO, "[CLoadPacketParse::Close]End.\n"));
}
