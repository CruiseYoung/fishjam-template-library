#pragma once

#include <cppunit/extensions/HelperMacros.h>
/************************************************************************
* array -- STL���������࣬���ڴ洢һ���̶���С�����顣�����С������Ԫ������һͬ��ģ�����ָ����
*   ��ָ���Ĵ�С��ֵ������һ����������ʱ�� -- �ﲻ�ܡ����ӻ����������С
*   �����Ŀ���� Ϊ�˺�����STL���������޷����� -- �� vector/list ���滻����ͨ����ʱ�����Է��� begin/end �ȷ���
************************************************************************/
class CSTLArrayTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLArrayTester );
    CPPUNIT_TEST( test_array );
    CPPUNIT_TEST_SUITE_END();

	void test_array();

    DECLARE_DEFAULT_TEST_CLASS(CSTLArrayTester);
};