#include "Unit_MessageDyeingManager.h"

#ifdef _CPPUNIT_TEST

CUnit_MessageDyeingManager::CUnit_MessageDyeingManager()
{
}

void CUnit_MessageDyeingManager::setUp(void)
{
    m_pMessageDyeingManager = new CMessageDyeingManager();
}

void CUnit_MessageDyeingManager::tearDown(void)
{
    delete m_pMessageDyeingManager;
    m_pMessageDyeingManager = nullptr;
}

void CUnit_MessageDyeingManager::Test_MessageDyeingManager(void)
{
    bool blRet                 = false;
    uint16 u2CommandID         = 0x1000;

    //��������Ⱦɫ�ӿ�
    if (false == m_pMessageDyeingManager->AddDyeingCommand(u2CommandID, 5))
    {
        OUR_DEBUG((LM_INFO, "[Test_MessageDyeingManager]AddDyeingCommand is false.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_MessageDyeingManager]AddDyeingCommand is false.\n", true == blRet);
        return;
    }

    m_pMessageDyeingManager->AddDyringIP("127.0.0.1", 5);

    vec_Dyeing_Command_list objList;
    m_pMessageDyeingManager->GetDyeingCommand(objList);

    if (1 != objList.size())
    {
        OUR_DEBUG((LM_INFO, "[GetDyeingCommand]GetDyeingCommand is not 1.\n"));
        CPPUNIT_ASSERT_MESSAGE("[GetDyeingCommand]GetDyeingCommand is not 1.\n", true == blRet);
        return;
    }

    string strTraceID = m_pMessageDyeingManager->GetTraceID("120.0.0.1", 10002, u2CommandID);

    if (0 == strTraceID.length())
    {
        OUR_DEBUG((LM_INFO, "[GetDyeingCommand]GetTraceID is error.\n"));
        CPPUNIT_ASSERT_MESSAGE("[GetDyeingCommand]GetTraceID is error.\n", true == blRet);
    }
}

#endif


