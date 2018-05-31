#include "Unit_BuffPacket.h"

#ifdef _CPPUNIT_TEST

void CUnit_BuffPacket::setUp(void)
{
	m_pBuffPacket = new CBuffPacket();
	
  m_pBuffPacket->Init(DEFINE_PACKET_SIZE, 20480);
  m_pBuffPacket->SetNetSort(false);	
}

void CUnit_BuffPacket::tearDown(void)
{
	delete m_pBuffPacket;
	m_pBuffPacket = NULL;
}

void CUnit_BuffPacket::Read_Write_BuffPacket(void)
{
	bool blRet    = false;
	uint8  u1Data = 1;
	uint16 u2Data = 2;
	uint32 u4Data = 4;
	uint64 u8Data = 8;
	
	uint8  u1OutData = 0;
	uint16 u2OutData = 0;
	uint32 u4OutData = 0;
	uint64 u8OutData = 0;	
	
	//–¥»Î
	(*m_pBuffPacket) << u1Data;
	(*m_pBuffPacket) << u2Data;
	(*m_pBuffPacket) << u4Data;
	(*m_pBuffPacket) << u8Data;
	
	//∂¡»°
	(*m_pBuffPacket) >> u1OutData;
	(*m_pBuffPacket) >> u2OutData;
	(*m_pBuffPacket) >> u4OutData;
	(*m_pBuffPacket) >> u8OutData;
	
	if(u1OutData == u1Data &&
		u2OutData == u2Data &&
		u4OutData == u4OutData &&
		u8OutData == u8OutData)
	{
		blRet = true;
	}
	
	CPPUNIT_ASSERT_MESSAGE("[Read_Write_BuffPacket]fail.", true == blRet);
}
#endif
