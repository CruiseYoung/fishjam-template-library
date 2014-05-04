#pragma once

#include <cppunit/extensions/HelperMacros.h>

/****************************************************************************************
* deque -- (��������)�洢��ָ��ͬԪ�ص�ָ������ɵ����飬������ٵ������λ����ͷβ�����ɾ��Ԫ�ء�
*   ������м�ɾ��Ԫ�أ������Ԫ���ڴ濽����
****************************************************************************************/
class CSTLDequeTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLDequeTester );
    CPPUNIT_TEST( test_deque_memory );
    CPPUNIT_TEST_SUITE_END();

    //����dequeʹ���ڴ��ģʽ -- ֤������Ԫ��ʱ��������ڴ濽�����������������м�ɾ��Ԫ��
    //  ������ڴ濽��.
    void test_deque_memory();
    
    DECLARE_DEFAULT_TEST_CLASS(CSTLDequeTester);
};