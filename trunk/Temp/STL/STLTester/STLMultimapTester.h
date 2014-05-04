#pragma once

#include <cppunit/extensions/HelperMacros.h>

/*******************************************************************************************************
* multimap ����֧�ֶ����ͬ�� key, ��ÿһ��key����ӳ�䵽һ��value,���ڲ���Ȼʹ�ú��������
* ͨ�� find ����,������ҵ��Ļ�,�᷵�ص�һ���ĵ�����,���򷵻� end
* ͨ�� equal_range ���Է���һ��
*******************************************************************************************************/

class CSTLMultimapTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLMultimapTester );
    CPPUNIT_TEST( test_multimap );
    CPPUNIT_TEST_SUITE_END();

    void test_multimap();

    DECLARE_DEFAULT_TEST_CLASS(CSTLMultimapTester);
};
