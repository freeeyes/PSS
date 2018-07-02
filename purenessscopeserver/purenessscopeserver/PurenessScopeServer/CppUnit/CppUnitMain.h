#ifndef _CPPUNITMAIN_H
#define _CPPUNITMAIN_H

//CppUnit PSSÏîÄ¿×Ô¼ì
//add by freeeyes

#ifdef _CPPUNIT_TEST

#include "Unit_BuffPacket.h"
#include "Unit_FileTestManager.h"
#include "Unit_CommandAccount.h"
#include "Unit_AppConfig.h"
#include "Uint_ConsolePromissions.h"
#include "Uint_ConsoleMessage.h"
#include "Unit_Aes.h"
#include "Unit_ConnectTcp.h"
#include "Unit_ConnectUdp.h"
#include "Unit_BaseHandler.h"
#include "Unit_MessageDefine.h"
#include "Unit_Logging.h"
#include "Unit_IPAccount.h"
#include "Unit_FrameCommand.h"
#include "Unit_TimeQueue.h"
#include "Unit_ModuleMessageManager.h"

#define CPPUNIT_REPORT_PATH "./reports/cppunit/report.xml"

class CCppUnitMain
{
public:
    CCppUnitMain();
    virtual ~CCppUnitMain();

    int Run();
};

#endif

#endif
