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
    CUnit_FrameCommand();

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_Frame_Command(void);

private:
    CFrameCommand* m_pFrameCommand;
};

#endif

#endif
