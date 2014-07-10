#include "StdAfx.h"
#include "FTLMemTester.h"
#include "ftlMem.h"

class CMyMemLeak : public FTL::CFMemCheckBase
{
public:
    CMyMemLeak()
    {
        m_SomeValue = 10;
    }
private:
    int m_SomeValue;
    
};

void CFTLMemTester::test_MemLeakBase()
{
    CMyMemLeak* pLeak = new CMyMemLeak();

    //�˴���û���ͷţ���˻��⵽�ڴ�й©
    //CHECK_OBJ_MEM_LEAK(CMyMemLeak);
    
    SAFE_DELETE(pLeak);
}