#pragma once

#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* deque -- ˫���queue?
* queue -- �Ƚ��ȳ���ִ������
* priority_queue -- Ԫ�صĴ����������������洢��ֵ���ϵ�ĳ��ν�ʾ����ĵ�һ�ֶ���
* 
* ���󲿷ֲ����ɾ�����������е�ͷ��βʱ����ѡ��deque�������ݽṹ
*
************************************************************************/

class CSTLQueueTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLQueueTester );
    //CPPUNIT_TEST( test_constructor);

    CPPUNIT_TEST_SUITE_END();

    //void test_constructor();

    DECLARE_DEFAULT_TEST_CLASS(CSTLQueueTester);
};
