#ifndef _CPPUNITMAIN_H
#define _CPPUNITMAIN_H

//CppUnit PSSÏîÄ¿×Ô¼ì
//add by freeeyes

#ifdef _CPPUNIT_TEST

#include "Unit_BuffPacket.h"
#include "Unit_FileTestManager.h"
#include "Unit_CommandAccount.h"
#include "Uint_ConsolePromissions.h"
#include "Uint_ConsoleMessage.h"
#include "Unit_Aes.h"
#include "Unit_ConnectTcp.h"
#include "Unit_ConnectUdp.h"
#include "Unit_BaseHandler.h"
#include "Unit_MessageDefine.h"
#include "Unit_IPAccount.h"
#include "Unit_FrameCommand.h"
#include "Unit_TimeQueue.h"
#include "Unit_ModuleMessageManager.h"
#include "Unit_FileLog.h"
#include "Unit_BaseConnectClient.h"
#include "Uint_ServerMessageInfoPool.h"
#include "Uint_ServerMessageManager.h"
#include "Unit_MessageDyeingManager.h"
#include "Unit_ProfileTime.h"
#include "Unit_WorkThreadAI.h"
#include "Unit_ControlListen.h"
#include "Unit_MakePacket.h"
#include "Unit_ForbiddenIP.h"
#include "Uint_AceReactorManager.h"
#include "Uint_ThreadInfo.h"
#include "Uint_ConnectHandler.h"
#include "Uint_PacketParse.h"
#include "Uint_MessageService.h"
#include "Uint_ConnectClient.h"
#include "Uint_LogManager.h"
#include "Unit_LoadModule.h"
#include "Uint_TcpRedirection.h"
#include "Unit_EChartlog.h"
#include "Uint_Proformancecount.h"

static const char CPPUNIT_REPORT_PATH[] = "./reports/cppunit/report.xml";

class CCppUnitMain
{
public:
    CCppUnitMain();
    ~CCppUnitMain() = default;

    int Run() const;
};

#endif

#endif
