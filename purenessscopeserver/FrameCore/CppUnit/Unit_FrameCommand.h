#ifndef _UNIT_FRAMECOMMAND_H
#define _UNIT_FRAMECOMMAND_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "FrameCommand.h"

class CUnit_FrameCommand: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_FrameCommand);
    CPPUNIT_TEST(Test_Frame_Command);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_FrameCommand() = default;

    void setUp(void) final;

    void tearDown(void) final;

    void Test_Frame_Command(void);

private:
    shared_ptr<CFrameCommand> m_pFrameCommand = nullptr;
};

#endif

#endif
