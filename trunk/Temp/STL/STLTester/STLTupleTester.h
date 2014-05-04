#pragma once

#include <cppunit/extensions/HelperMacros.h>
/************************************************************************
* tuple -- ���� pair,������2��10��Ԫ�ذ�װ��һ��
*   make_pair���ڳ�ʼ��pair,���Ƶ�VS2010�пɴ���2��10�����������غ���make_tuple
************************************************************************/
class CSTLTupleTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLTupleTester );
    CPPUNIT_TEST( test_tuple_init );
    CPPUNIT_TEST_SUITE_END();

	void test_tuple_init();

    DECLARE_DEFAULT_TEST_CLASS(CSTLTupleTester);
};