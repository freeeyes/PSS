#ifndef UNIT_TCPREDIRECTION_H
#define UNIT_TCPREDIRECTION_H

#ifdef _CPPUNIT_TEST

#include "Unit_Common.h"
#include "TcpRedirection.h"
#include "ConnectHandler.h"
#include "ClientReConnectManager.h"
#include "XmlConfig.h"

class CUnit_Redirection : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CUnit_Redirection);
    CPPUNIT_TEST(Test_Redirection);
    CPPUNIT_TEST_SUITE_END();

public:
    CUnit_Redirection();

    virtual ~CUnit_Redirection();

    CUnit_Redirection(const CUnit_Redirection& ar);

    CUnit_Redirection& operator = (const CUnit_Redirection& ar)
    {
        if (this != &ar)
        {
            ACE_UNUSED_ARG(ar);
        }

        return *this;
    }

    virtual void setUp(void);

    virtual void tearDown(void);

    void Test_Redirection(void);

private:
    CTcpRedirection* m_pTcpRedirection;
};

#endif

#endif
