#include "CppUnitMain.h"

#ifdef _CPPUNIT_TEST

CCppUnitMain::CCppUnitMain()
{
    CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CUnit_BuffPacket, "pss");
    CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CUnit_FileTestManager, "pss");
    CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CUnit_CommandAccount, "pss");
    CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CUnit_AppConfig, "pss");
    CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CUnit_ConsolePromissions, "pss");
    CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CUnit_ConsoleMessage, "pss");
    CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CUnit_ConnectTcp, "pss");
}

CCppUnitMain::~CCppUnitMain()
{

}

int CCppUnitMain::Run()
{
    ofstream out(CPPUNIT_REPORT_PATH);

    // informs test-listener about testresults
    CPPUNIT_NS::TestResult testresult;

    // register listener for collecting the test-results
    CPPUNIT_NS::TestResultCollector collectedresults;
    testresult.addListener (&collectedresults);

    // register listener for per-test progress output
    CPPUNIT_NS::BriefTestProgressListener progress;
    testresult.addListener (&progress);

    // insert test-suite at test-runner by registry
    CPPUNIT_NS::TestRunner testrunner;
    testrunner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry("pss").makeTest());
    testrunner.run(testresult);

    // Output XML for Jenkins CPPunit plugin
    CppUnit::XmlOutputter xmlOut(&collectedresults, out);
    xmlOut.write();
    out.close();

    // return 0 if tests were successful
    return collectedresults.wasSuccessful() ? 0 : 1;
}

#endif

