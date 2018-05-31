#ifndef _CPPUNITMAIN_H
#define _CPPUNITMAIN_H

//CppUnit PSSÏîÄ¿×Ô¼ì
//add by freeeyes

#ifdef _CPPUNIT_TEST

#include "Unit_BuffPacket.h"

class CCppUnitMain
{
public:
    CCppUnitMain();
    virtual ~CCppUnitMain();
	
	int Run(const char* pReportPath);
};

#endif

#endif
