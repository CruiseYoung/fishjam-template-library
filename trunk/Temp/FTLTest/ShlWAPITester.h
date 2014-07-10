#pragma once

#include <cppunit/extensions/HelperMacros.h>

/*******************************************************************************************************
* ע�⣺���²��Զ���������ϵͳ��
*   StrFromTimeInterval -- ��������ת��Ϊ��OS��ص�ʱ���ַ�����ע���ַ�ǰ�����һ���ո�
*     �о�����Ҫ������Чλ����ʵ����ת��Ч�����ã���Чλ��Ӧ�����óɶ��ٲź��� -- ��û������Ӧ�ģ���
*******************************************************************************************************/

class CShlWAPITester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CShlWAPITester );

    CPPUNIT_TEST( test_StrFormatByteSizeXX );
    CPPUNIT_TEST( test_StrFromTimeInterval );

    CPPUNIT_TEST_SUITE_END();

    void test_StrFormatByteSizeXX();
    void test_StrFromTimeInterval();

    DECLARE_DEFAULT_TEST_CLASS(CShlWAPITester);
};
