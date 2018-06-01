#ifndef _CPPUNITMAIN_H
#define _CPPUNITMAIN_H

//CppUnit PSSÏîÄ¿×Ô¼ì
//add by freeeyes

#ifdef _CPPUNIT_TEST

#include "Unit_BuffPacket.h"
#include "Unit_FileTestManager.h"

#define CPPUNIT_REPORT_PATH "./reports/cppunit/report.txt"

class CCppUnitMain
{
public:
    CCppUnitMain();
    virtual ~CCppUnitMain();

    int Run();
};

#endif

#endif
