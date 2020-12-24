#include "Unit_BuffPacket.h"

#ifdef _CPPUNIT_TEST

void CUnit_BuffPacket::setUp(void)
{
    m_pBuffPacket = std::make_shared<CBuffPacket>();

    m_pBuffPacket->Init(DEFINE_PACKET_SIZE, TEST_BUFFPACKET_MAX_SIZE);
    m_pBuffPacket->SetNetSort(false);
}

void CUnit_BuffPacket::tearDown(void)
{
    PSS_LOGGER_DEBUG("[CUnit_BuffPacket::tearDown]Finish.");
}

void CUnit_BuffPacket::Read_Write_BuffPacket(void)
{
    bool blRet    = false;
    uint8   u1Data = 1;
    uint16  u2Data = 2;
    uint32  u4Data = 4;
    uint64  u8Data = 8;
    int8    n1Data = -1;
    int16   n2Data = -2;
    int32   n4Data = -4;
    float32 f4Data = 4.0F;
    float64 f8Data = 8.0F;

    uint8   u1OutData = 0;
    uint16  u2OutData = 0;
    uint32  u4OutData = 0;
    uint64  u8OutData = 0;
    int8    n1OutData = 0;
    int16   n2OutData = 0;
    int32   n4OutData = 0;
    float32 f4OutData = 0.0F;
    float64 f8OutData = 0.0F;

    //写入
    (*m_pBuffPacket) << u1Data;
    (*m_pBuffPacket) << u2Data;
    (*m_pBuffPacket) << u4Data;
    (*m_pBuffPacket) << u8Data;
    (*m_pBuffPacket) << n1Data;
    (*m_pBuffPacket) << n2Data;
    (*m_pBuffPacket) << n4Data;
    (*m_pBuffPacket) << f4Data;
    (*m_pBuffPacket) << f8Data;

    //读取
    (*m_pBuffPacket) >> u1OutData;
    (*m_pBuffPacket) >> u2OutData;
    (*m_pBuffPacket) >> u4OutData;
    (*m_pBuffPacket) >> u8OutData;
    (*m_pBuffPacket) >> n1OutData;
    (*m_pBuffPacket) >> n2OutData;
    (*m_pBuffPacket) >> n4OutData;
    (*m_pBuffPacket) >> f4OutData;
    (*m_pBuffPacket) >> f8OutData;

    if (m_pBuffPacket->GetWriteLen() - m_pBuffPacket->GetReadLen() != 0)
    {
        PSS_LOGGER_DEBUG("[Read_Write_String_BuffPacket]GetPacketLen=%d.", m_pBuffPacket->GetPacketLen());
        CPPUNIT_ASSERT_MESSAGE("[Read_Write_String_BuffPacket]GetPacketLen is not zero.", true == blRet);
        return;
    }

    if(u1OutData == u1Data &&
       u2OutData == u2Data &&
       u4OutData == u4Data &&
       u8OutData == u8Data &&
       n1OutData == n1Data &&
       n2OutData == n2Data &&
       n4OutData == n4Data &&
       f4OutData == f4Data &&
       f8OutData == f8Data)
    {
        blRet = true;
    }

    CPPUNIT_ASSERT_MESSAGE("[Read_Write_BuffPacket]fail.", true == blRet);
}

void CUnit_BuffPacket::Read_Write_String_BuffPacket(void)
{
    bool blRet = false;
    char szData[30]    = { '\0' };

    _VCHARS_STR obj_VCHARS_STR;
    _VCHARM_STR obj_VCHARM_STR;
    _VCHARB_STR obj_VCHARB_STR;

    _VCHARS_STR obj_Ret_VCHARS_STR;
    _VCHARM_STR obj_Ret_VCHARM_STR;
    _VCHARB_STR obj_Ret_VCHARB_STR;

    sprintf_safe(szData, 30, "freeeyes");

    obj_VCHARS_STR.text = szData;
    obj_VCHARS_STR.u1Len = (uint8)ACE_OS::strlen(szData);
    obj_VCHARM_STR.text = szData;
    obj_VCHARM_STR.u2Len = (uint16)ACE_OS::strlen(szData);
    obj_VCHARB_STR.text = szData;
    obj_VCHARB_STR.u4Len = (uint32)ACE_OS::strlen(szData);

    (*m_pBuffPacket) << obj_VCHARS_STR;
    (*m_pBuffPacket) << obj_VCHARM_STR;
    (*m_pBuffPacket) << obj_VCHARB_STR;

    (*m_pBuffPacket) >> obj_Ret_VCHARS_STR;
    (*m_pBuffPacket) >> obj_Ret_VCHARM_STR;
    (*m_pBuffPacket) >> obj_Ret_VCHARB_STR;

    if (m_pBuffPacket->GetWriteLen() - m_pBuffPacket->GetReadLen() != 0)
    {
        PSS_LOGGER_DEBUG("[Read_Write_String_BuffPacket]GetPacketLen=%d.", m_pBuffPacket->GetPacketLen());
        CPPUNIT_ASSERT_MESSAGE("[Read_Write_String_BuffPacket]GetPacketLen is not zero.", true == blRet);
        return;
    }

    if (ACE_OS::strcmp(obj_VCHARS_STR.text, obj_Ret_VCHARS_STR.text) == 0 &&
        ACE_OS::strcmp(obj_VCHARM_STR.text, obj_Ret_VCHARM_STR.text) == 0 &&
        ACE_OS::strcmp(obj_VCHARB_STR.text, obj_Ret_VCHARB_STR.text) == 0)
    {
        blRet = true;
    }

    CPPUNIT_ASSERT_MESSAGE("[Read_Write_String_BuffPacket]fail.", true == blRet);
}

void CUnit_BuffPacket::Read_Write_Binary_BuffPacket(void)
{
    bool blRet = false;
    char szData[30] = { '\0' };
    char szRetData[30] = { '\0' };

    sprintf_safe(szData, 30, "freeeyes");

    m_pBuffPacket->WriteStream(szData, (uint32)ACE_OS::strlen(szData));

    if (m_pBuffPacket->GetWriteLen() != ACE_OS::strlen(szData))
    {
        CPPUNIT_ASSERT_MESSAGE("[Read_Write_Binary_BuffPacket]write len fail.", true == blRet);
        return;
    }

    uint32 u4ReadLen = (uint32)ACE_OS::strlen(szData);
    m_pBuffPacket->ReadStream(szRetData, u4ReadLen);

    if (m_pBuffPacket->GetPacketLen() - m_pBuffPacket->GetReadLen() != 0)
    {
        PSS_LOGGER_DEBUG("[Read_Write_String_BuffPacket]GetPacketLen=%d.", m_pBuffPacket->GetPacketLen());
        CPPUNIT_ASSERT_MESSAGE("[Read_Write_Binary_BuffPacket]GetPacketLen is not zero.", true == blRet);
        return;
    }

    if (ACE_OS::strcmp(szData, szRetData) == 0)
    {
        blRet = true;
    }

    CPPUNIT_ASSERT_MESSAGE("[Read_Write_String_BuffPacket]fail.", true == blRet);
}

void CUnit_BuffPacket::Check_Size_BuffPacket(void)
{
    bool blRet = false;
    m_pBuffPacket->Clear();

    uint8   u1Data = 1;

    (*m_pBuffPacket) << u1Data;

    if (m_pBuffPacket->GetHeadLen() != 0)
    {
        PSS_LOGGER_DEBUG("[Check_Size_BuffPacket]m_pBuffPacket->GetHeadLen()={0}.", m_pBuffPacket->GetHeadLen());
        CPPUNIT_ASSERT_MESSAGE("[Check_Size_BuffPacket]GetHeadLen() error.", true == blRet);
        return;
    }

    if (m_pBuffPacket->GetPacketSize() % 1024 != 0)
    {
        CPPUNIT_ASSERT_MESSAGE("[Check_Size_BuffPacket]GetPacketSize() error.", true == blRet);
        return;
    }

    if (m_pBuffPacket->GetPacketLen() != 1)
    {
        CPPUNIT_ASSERT_MESSAGE("[Check_Size_BuffPacket]GetPacketLen() error.", true == blRet);
    }
}

void CUnit_BuffPacket::Check_AddBuff(void)
{
    bool blRet = false;

    if (false == m_pBuffPacket->AddBuff(1024))
    {
        CPPUNIT_ASSERT_MESSAGE("[Check_AddBuff]AddBuff(1024) error.", true == blRet);
    }
}

void CUnit_BuffPacket::Check_RollBack(void)
{
    bool blRet = false;

    uint8  u1Data = 0;
    uint32 u4Data = 1;
    (*m_pBuffPacket) << u1Data;
    (*m_pBuffPacket) << u4Data;

    if (false == m_pBuffPacket->RollBack(1))
    {
        CPPUNIT_ASSERT_MESSAGE("[Check_RollBack]RollBack(1) error.", true == blRet);
        return;
    }

    uint32 u4Out = 0;
    (*m_pBuffPacket) >> u4Out;

    if (u4Out != u4Data)
    {
        CPPUNIT_ASSERT_MESSAGE("[Check_RollBack]RollBack(1) u4Data != u4Out.", true == blRet);
    }
}

void CUnit_BuffPacket::Check_String(void)
{
    bool blRet = false;

    string strData = "freeeyes";
    string strCout = "";

    (*m_pBuffPacket) << strData;
    (*m_pBuffPacket) >> strCout;

    if (strCout != strData)
    {
        CPPUNIT_ASSERT_MESSAGE("[Check_String]RollBack(1) strData != strCout.", true == blRet);
    }
}

void CUnit_BuffPacket::Check_AddBuff_More_Uint8(void)
{
    bool blRet = false;
    //测试Buff自动增长
    uint8 u1Src     = 12;
    uint8 u1Des     = 0;
    uint32 u4DesLen  = MAX_BUFF_1024;
    char szBuff[MAX_BUFF_1024] = { '\0' };

    m_pBuffPacket->WriteStream(szBuff, MAX_BUFF_1024);

    (*m_pBuffPacket) << u1Src;

    m_pBuffPacket->ReadStream(szBuff, u4DesLen);

    (*m_pBuffPacket) >> u1Des;

    if (u1Src != u1Des)
    {
        CPPUNIT_ASSERT_MESSAGE("[Check_AddBuff_More_Uint8]Src != Des.", true == blRet);
    }
}

void CUnit_BuffPacket::Check_AddBuff_More_Uint16(void)
{
    bool blRet = false;
    //测试Buff自动增长
    uint16 u2Src    = 12;
    uint16 u2Des    = 0;
    uint32 u4DesLen  = MAX_BUFF_1024;
    char szBuff[MAX_BUFF_1024] = { '\0' };

    m_pBuffPacket->WriteStream(szBuff, MAX_BUFF_1024);

    (*m_pBuffPacket) << u2Src;

    m_pBuffPacket->ReadStream(szBuff, u4DesLen);

    (*m_pBuffPacket) >> u2Des;

    if (u2Src != u2Des)
    {
        CPPUNIT_ASSERT_MESSAGE("[Check_AddBuff_More_Uint16]Src != Des.", true == blRet);
    }
}

void CUnit_BuffPacket::Check_AddBuff_More_Uint32(void)
{
    bool blRet = false;
    //测试Buff自动增长
    uint32 u4Src    = 12;
    uint32 u4Des    = 0;
    uint32 u4DesLen  = MAX_BUFF_1024;
    char szBuff[MAX_BUFF_1024] = { '\0' };

    m_pBuffPacket->WriteStream(szBuff, MAX_BUFF_1024);

    (*m_pBuffPacket) << u4Src;

    m_pBuffPacket->ReadStream(szBuff, u4DesLen);

    (*m_pBuffPacket) >> u4Des;

    if (u4Src != u4Des)
    {
        CPPUNIT_ASSERT_MESSAGE("[Check_AddBuff_More_Uint32]Src != Des.", true == blRet);
    }
}

void CUnit_BuffPacket::Check_AddBuff_More_Uint64(void)
{
    bool blRet = false;
    //测试Buff自动增长
    uint64 u8Src    = 12;
    uint64 u8Des    = 0;
    uint32 u4DesLen  = MAX_BUFF_1024;
    char szBuff[MAX_BUFF_1024] = { '\0' };

    m_pBuffPacket->WriteStream(szBuff, MAX_BUFF_1024);

    (*m_pBuffPacket) << u8Src;

    m_pBuffPacket->ReadStream(szBuff, u4DesLen);

    (*m_pBuffPacket) >> u8Des;

    if (u8Src != u8Des)
    {
        CPPUNIT_ASSERT_MESSAGE("[Check_AddBuff_More_Uint64]Src != Des.", true == blRet);
    }
}

void CUnit_BuffPacket::Check_AddBuff_More_Int8(void)
{
    bool blRet = false;
    //测试Buff自动增长
    int8 n1Src      = 12;
    int8 n1Des      = 0;
    uint32 u4DesLen  = MAX_BUFF_1024;
    char szBuff[MAX_BUFF_1024] = { '\0' };

    m_pBuffPacket->WriteStream(szBuff, MAX_BUFF_1024);

    (*m_pBuffPacket) << n1Src;

    m_pBuffPacket->ReadStream(szBuff, u4DesLen);

    (*m_pBuffPacket) >> n1Des;

    if (n1Src != n1Des)
    {
        CPPUNIT_ASSERT_MESSAGE("[Check_AddBuff_More_Int8]Src != Des.", true == blRet);
    }
}

void CUnit_BuffPacket::Check_AddBuff_More_Int16(void)
{
    bool blRet = false;
    //测试Buff自动增长
    int16 n2Src     = 12;
    int16 n2Des     = 0;
    uint32 u4DesLen  = MAX_BUFF_1024;
    char szBuff[MAX_BUFF_1024] = { '\0' };

    m_pBuffPacket->WriteStream(szBuff, MAX_BUFF_1024);

    (*m_pBuffPacket) << n2Src;

    m_pBuffPacket->ReadStream(szBuff, u4DesLen);

    (*m_pBuffPacket) >> n2Des;

    if (n2Src != n2Des)
    {
        CPPUNIT_ASSERT_MESSAGE("[Check_AddBuff_More_Int16]Src != Des.", true == blRet);
    }
}

void CUnit_BuffPacket::Check_AddBuff_More_Int32(void)
{
    bool blRet = false;
    //测试Buff自动增长
    int32 n4Src     = 12;
    int32 n4Des     = 0;
    uint32 u4DesLen  = MAX_BUFF_1024;
    char szBuff[MAX_BUFF_1024] = { '\0' };

    m_pBuffPacket->WriteStream(szBuff, MAX_BUFF_1024);

    (*m_pBuffPacket) << n4Src;

    m_pBuffPacket->ReadStream(szBuff, u4DesLen);

    (*m_pBuffPacket) >> n4Des;

    if (n4Src != n4Des)
    {
        CPPUNIT_ASSERT_MESSAGE("[Check_AddBuff_More_Int32]Src != Des.", true == blRet);
    }
}

void CUnit_BuffPacket::Check_AddBuff_More_Float32(void)
{
    bool blRet = false;
    //测试Buff自动增长
    float32 f4Src   = 12;
    float32 f4Des   = 0;
    uint32 u4DesLen  = MAX_BUFF_1024;
    char szBuff[MAX_BUFF_1024] = { '\0' };

    m_pBuffPacket->WriteStream(szBuff, MAX_BUFF_1024);

    (*m_pBuffPacket) << f4Src;

    m_pBuffPacket->ReadStream(szBuff, u4DesLen);

    (*m_pBuffPacket) >> f4Des;

    if (f4Src != f4Des)
    {
        CPPUNIT_ASSERT_MESSAGE("[Check_AddBuff_More_Float32]Src != Des.", true == blRet);
    }
}

void CUnit_BuffPacket::Check_AddBuff_More_Float64(void)
{
    bool blRet = false;
    //测试Buff自动增长
    float64 f8Src   = 12;
    float64 f8Des   = 0;
    uint32 u4DesLen  = MAX_BUFF_1024;
    char szBuff[MAX_BUFF_1024] = { '\0' };

    m_pBuffPacket->WriteStream(szBuff, MAX_BUFF_1024);

    (*m_pBuffPacket) << f8Src;

    m_pBuffPacket->ReadStream(szBuff, u4DesLen);

    (*m_pBuffPacket) >> f8Des;

    if (f8Src != f8Des)
    {
        CPPUNIT_ASSERT_MESSAGE("[Check_AddBuff_More_Float64]Src != Des.", true == blRet);
    }
}

void CUnit_BuffPacket::Check_AddBuff_More_VCHARS_STR(void)
{
    bool blRet = false;
    char szSSrc[20] = { '\0' };
    uint32 u4DesLen  = MAX_BUFF_1024;
    VCHARS_STR strSrc;
    VCHARS_STR strDes;

    sprintf_safe(szSSrc, 20, "freeeyes");

    char szBuff[MAX_BUFF_1024] = { '\0' };

    m_pBuffPacket->WriteStream(szBuff, MAX_BUFF_1024);

    strSrc.text  = szSSrc;
    strSrc.u1Len = (uint8)ACE_OS::strlen(szSSrc);
    strSrc.type  = VCHARS_TYPE::VCHARS_TYPE_TEXT;

    (*m_pBuffPacket) << strSrc;

    m_pBuffPacket->ReadStream(szBuff, u4DesLen);

    (*m_pBuffPacket) >> strDes;

    if (ACE_OS::strcmp(szSSrc, strDes.text) != 0)
    {
        CPPUNIT_ASSERT_MESSAGE("[Check_AddBuff_More_VCHARS_STR]Src != Des.", true == blRet);
    }
}

void CUnit_BuffPacket::Check_AddBuff_More_VCHARM_STR(void)
{
    bool blRet = false;
    char szMSSrc[20] = { '\0' };
    uint32 u4DesLen  = MAX_BUFF_1024;
    VCHARM_STR strMSrc;
    VCHARM_STR strMDes;

    sprintf_safe(szMSSrc, 20, "freeeyes");

    char szBuff[MAX_BUFF_1024] = { '\0' };

    m_pBuffPacket->WriteStream(szBuff, MAX_BUFF_1024);

    strMSrc.text  = szMSSrc;
    strMSrc.u2Len = (uint16)ACE_OS::strlen(szMSSrc);
    strMSrc.type  = VCHARS_TYPE::VCHARS_TYPE_TEXT;

    (*m_pBuffPacket) << strMSrc;

    m_pBuffPacket->ReadStream(szBuff, u4DesLen);

    (*m_pBuffPacket) >> strMDes;

    if (ACE_OS::strcmp(szMSSrc, strMDes.text) != 0)
    {
        CPPUNIT_ASSERT_MESSAGE("[Check_AddBuff_More_VCHARM_STR]Src != Des.", true == blRet);
    }
}

void CUnit_BuffPacket::Check_AddBuff_More_VCHARB_STR(void)
{
    bool blRet = false;
    char szBSSrc[20] = { '\0' };
    uint32 u4DesLen  = MAX_BUFF_1024;
    VCHARB_STR strBSrc;
    VCHARB_STR strBDes;

    sprintf_safe(szBSSrc, 20, "freeeyes");

    char szBuff[MAX_BUFF_1024] = { '\0' };

    m_pBuffPacket->WriteStream(szBuff, MAX_BUFF_1024);

    strBSrc.text  = szBSSrc;
    strBSrc.u4Len = (uint32)ACE_OS::strlen(szBSSrc);
    strBSrc.type  = VCHARS_TYPE::VCHARS_TYPE_TEXT;

    (*m_pBuffPacket) << strBSrc;

    m_pBuffPacket->ReadStream(szBuff, u4DesLen);

    (*m_pBuffPacket) >> strBDes;

    if (ACE_OS::strcmp(szBSSrc, strBDes.text) != 0)
    {
        CPPUNIT_ASSERT_MESSAGE("[Check_AddBuff_More_VCHARB_STR]Src != Des.", true == blRet);
    }
}

void CUnit_BuffPacket::Test_PacketCount()
{
    bool blRet = false;
    m_pBuffPacket->SetPacketCount(2);

    if (m_pBuffPacket->GetPacketCount() != 2)
    {
        CPPUNIT_ASSERT_MESSAGE("[Test_PacketCount]GetPacketCount() != SetPacketCount.", true == blRet);
        return;
    }

    //测试移动读指针
    uint32 u4Data = 12;
    (*m_pBuffPacket) << u4Data;
    u4Data = 13;
    (*m_pBuffPacket) << u4Data;

    m_pBuffPacket->ReadBuffPtr(4);

    m_pBuffPacket->SetReadPtr(0);

    m_pBuffPacket->WriteBuffPtr(8);

    m_pBuffPacket->GetError();

    m_pBuffPacket->SetHashID(1);

    if (m_pBuffPacket->GetHashID() != 1)
    {
        CPPUNIT_ASSERT_MESSAGE("[Test_PacketCount]GetHashID() error.", true == blRet);
    }

}

#endif
