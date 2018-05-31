#include "CppUnitMain.h"

#ifdef _CPPUNIT_TEST

CCppUnitMain::CCppUnitMain()
{
	CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CUnit_BuffPacket, "Pss");
}

CCppUnitMain::~CCppUnitMain()
{
	
}

int CCppUnitMain::Run(const char* pReportPath)
{
	ofstream out(pReportPath); 
	
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
  testrunner.addTest (CPPUNIT_NS::TestFactoryRegistry::getRegistry("Pss").makeTest ());
  testrunner.run(testresult);
  
  // output results in compiler-format
  CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, out);   
  compileroutputter.write ();
  out.close();

  // Output XML for Jenkins CPPunit plugin
  //ofstream xmlFileOut("cppTestBasicMathResults.xml");
  //XmlOutputter xmlOut(&collectedresults, xmlFileOut);
  //xmlOut.write();

  // return 0 if tests were successful
  return collectedresults.wasSuccessful() ? 0 : 1;
}

#endif

