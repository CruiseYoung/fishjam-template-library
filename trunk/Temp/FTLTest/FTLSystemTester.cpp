#include "StdAfx.h"
#include "FTLSystemTester.h"
#include <ftlSystem.h>
#include <vector>

void CFTLSystemTester::test_IntelIsLittleSystem()
{
    //һ�㶼��Intelƽ̨
    CPPUNIT_ASSERT(CFSystemUtil::IsLittleSystem());
}