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
        PSS_LOGGER_DEBUG("[CLoadPacketParse::LoadPacketInfo] pPacketParseInfo is nullptr!");
        return false;
    }

    pPacketParseInfo->m_u4PacketParseID   = u4PacketParseID;
    pPacketParseInfo->m_u1PacketParseType = u1Type;
    pPacketParseInfo->m_u4OrgLength       = u4HeadLen;
    string strFilePath;

    strFilePath = fmt::format("{0}{1}", pPacketParsePath, szPacketParseName);
    pPacketParseInfo->m_hModule = CLoadLibrary::PSS_dlopen(strFilePath.c_str(), RTLD_NOW);

    if(nullptr == pPacketParseInfo->m_hModule)
    {
        PSS_LOGGER_DEBUG("[CLoadPacketParse::LoadPacketInfo] strModuleName = {0}, pModuleInfo->hModule is nullptr({1})!", szPacketParseName, CLoadLibrary::PSS_dlerror());
        return false;
    }

    pPacketParseInfo->Parse_Packet_Head_Info = (bool(*)(uint32, ACE_Message_Block*, IMessageBlockManager*, _Head_Info*, EM_CONNECT_IO_TYPE))CLoadLibrary::PSS_dlsym(pPacketParseInfo->m_hModule, "Parse_Packet_Head_Info");

    if(nullptr == pPacketParseInfo->m_hModule || !pPacketParseInfo->Parse_Packet_Head_Info)
    {
        PSS_LOGGER_DEBUG("[CLoadPacketParse::LoadPacketInfo] strModuleName = {0}, Function Parse_Packet_Head_Info is error!", szPacketParseName);
        CLoadLibrary::PSS_dlClose(pPacketParseInfo->m_hModule);
        PSS_LOGGER_DEBUG("[CLoadPacketParse::LoadPacketInfo]PacketID={0}, ret={1}.", pPacketParseInfo->m_u4PacketParseID, nRet);
        return false;
    }

    pPacketParseInfo->Parse_Packet_Body_Info = (bool(*)(uint32, ACE_Message_Block*, IMessageBlockManager*, _Body_Info*, EM_CONNECT_IO_TYPE))CLoadLibrary::PSS_dlsym(pPacketParseInfo->m_hModule, "Parse_Packet_Body_Info");

    if(nullptr == pPacketParseInfo->m_hModule || !pPacketParseInfo->Parse_Packet_Body_Info)
    {
        PSS_LOGGER_DEBUG("[CLoadPacketParse::LoadPacketInfo] strModuleName = {0}, Function Parse_Packet_Body_Info is error!", szPacketParseName);
        CLoadLibrary::PSS_dlClose(pPacketParseInfo->m_hModule);
        PSS_LOGGER_DEBUG("[CLoadPacketParse::LoadPacketInfo]PacketID={0}, ret={1}.", pPacketParseInfo->m_u4PacketParseID, nRet);
        return false;
    }

    pPacketParseInfo->Parse_Packet_Stream = (uint8(*)(uint32, ACE_Message_Block*, IMessageBlockManager*, _Packet_Info*, EM_CONNECT_IO_TYPE))CLoadLibrary::PSS_dlsym(pPacketParseInfo->m_hModule, "Parse_Packet_Stream");

    if(nullptr == pPacketParseInfo->m_hModule || !pPacketParseInfo->Parse_Packet_Stream)
    {
        PSS_LOGGER_DEBUG("[CLoadPacketParse::LoadPacketInfo] strModuleName = {0}, Function Parse_Packet_Stream is error!", szPacketParseName);
        CLoadLibrary::PSS_dlClose(pPacketParseInfo->m_hModule);
        PSS_LOGGER_DEBUG("[CLoadPacketParse::LoadPacketInfo]PacketID={0}, ret={1}.", pPacketParseInfo->m_u4PacketParseID, nRet);
        return false;
    }

    pPacketParseInfo->Make_Send_Packet = (bool(*)(uint32, const char*, uint32, ACE_Message_Block*, uint16))CLoadLibrary::PSS_dlsym(pPacketParseInfo->m_hModule, "Make_Send_Packet");

    if(nullptr == pPacketParseInfo->m_hModule || !pPacketParseInfo->Make_Send_Packet)
    {
        PSS_LOGGER_DEBUG("[CLoadPacketParse::LoadPacketInfo] strModuleName = %s, Function Make_Send_Packet is error!", szPacketParseName);
        CLoadLibrary::PSS_dlClose(pPacketParseInfo->m_hModule);
        PSS_LOGGER_DEBUG("[CLoadPacketParse::LoadPacketInfo]PacketID={0}, ret={1}.", pPacketParseInfo->m_u4PacketParseID, nRet);
        return false;
    }

    pPacketParseInfo->Make_Send_Packet_Length = (uint32(*)(uint32, uint32, uint16))CLoadLibrary::PSS_dlsym(pPacketParseInfo->m_hModule, "Make_Send_Packet_Length");

    if(nullptr == pPacketParseInfo->m_hModule || !pPacketParseInfo->Make_Send_Packet_Length)
    {
        PSS_LOGGER_DEBUG("[CLoadPacketParse::LoadPacketInfo] strModuleName = {0}, Function Make_Send_Packet_Length is error!", szPacketParseName);
        CLoadLibrary::PSS_dlClose(pPacketParseInfo->m_hModule);
        PSS_LOGGER_DEBUG("[CLoadPacketParse::LoadPacketInfo]PacketID={0}, ret={1}.", pPacketParseInfo->m_u4PacketParseID, nRet);
        return false;
    }

    pPacketParseInfo->Connect = (bool(*)(uint32, _ClientIPInfo, _ClientIPInfo const&))CLoadLibrary::PSS_dlsym(pPacketParseInfo->m_hModule, "Connect");

    if(nullptr == pPacketParseInfo->m_hModule || !pPacketParseInfo->Connect)
    {
        PSS_LOGGER_DEBUG("[CLoadPacketParse::LoadPacketInfo] strModuleName = {0}, Function Connect is error!", szPacketParseName);
        CLoadLibrary::PSS_dlClose(pPacketParseInfo->m_hModule);
        PSS_LOGGER_DEBUG("[CLoadPacketParse::LoadPacketInfo]PacketID={0}, ret={1}.", pPacketParseInfo->m_u4PacketParseID, nRet);
        return false;
    }

    pPacketParseInfo->DisConnect = (void(*)(uint32))CLoadLibrary::PSS_dlsym(pPacketParseInfo->m_hModule, "DisConnect");

    if(nullptr == pPacketParseInfo->m_hModule || !pPacketParseInfo->DisConnect)
    {
        PSS_LOGGER_DEBUG("[CLoadPacketParse::LoadPacketInfo] strModuleName = {0}, Function DisConnect is error!", szPacketParseName);
        CLoadLibrary::PSS_dlClose(pPacketParseInfo->m_hModule);
        PSS_LOGGER_DEBUG("[CLoadPacketParse::LoadPacketInfo]PacketID={0}, ret={1}.", pPacketParseInfo->m_u4PacketParseID, nRet);
        return false;
    }

    pPacketParseInfo->Close = (void(*)())CLoadLibrary::PSS_dlsym(pPacketParseInfo->m_hModule, "Close");

    if (nullptr == pPacketParseInfo->m_hModule || !pPacketParseInfo->Close)
    {
        PSS_LOGGER_DEBUG("[CLoadPacketParse::LoadPacketInfo] strModuleName = {0}, Function Close is error!", szPacketParseName);
        CLoadLibrary::PSS_dlClose(pPacketParseInfo->m_hModule);
        PSS_LOGGER_DEBUG("[CLoadPacketParse::LoadPacketInfo]PacketID={0}, ret={1}.", pPacketParseInfo->m_u4PacketParseID, nRet);
        return false;
    }

    pPacketParseInfo->Set_output = (void(*)(shared_ptr<spdlog::logger>))CLoadLibrary::PSS_dlsym(pPacketParseInfo->m_hModule, "Set_output");

    if (nullptr == pPacketParseInfo->m_hModule || !pPacketParseInfo->Set_output)
    {
        PSS_LOGGER_DEBUG("[CLoadPacketParse::LoadPacketInfo] strModuleName = {0}, Function Set_output is error!", szPacketParseName);
        CLoadLibrary::PSS_dlClose(pPacketParseInfo->m_hModule);
        PSS_LOGGER_DEBUG("[CLoadPacketParse::LoadPacketInfo]PacketID={0}, ret={1}.", pPacketParseInfo->m_u4PacketParseID, nRet);
        return false;
    }

    //设置日志生效
    pPacketParseInfo->Set_output(spdlog::default_logger());

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
        PSS_LOGGER_DEBUG("[CLoadPacketParse::GetPacketParseInfo]can't find u4PacketParseID({0}).", u4PacketParseID);
        return nullptr;
    }
    else
    {
        return f->second;
    }
}

void CLoadPacketParse::Close()
{
    PSS_LOGGER_DEBUG("[CLoadPacketParse::Close]Begin.");
    //清理所有已存在的指针
    for_each(m_objPacketParseList.begin(), m_objPacketParseList.end(), [](const std::pair<uint32, shared_ptr<_Packet_Parse_Info>>& iter) {
        //关闭模块接口
        iter.second->Close();
        CLoadLibrary::PSS_dlClose(iter.second->m_hModule);
        });

    m_objPacketParseList.clear();
    PSS_LOGGER_DEBUG("[CLoadPacketParse::Close]End.");
}
