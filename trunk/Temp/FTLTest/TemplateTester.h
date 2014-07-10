#pragma once

#include <cppunit/extensions/HelperMacros.h>

/***************************************************************************************************
*  ׷����(Tracers) -- һ���û��Զ�����࣬����Ϊ�����Ե�ģ���ģ�������ǡ�÷��ϴ�����ģ���Ҫ�󣬿��Ա����
*    ���������Ͻ��еĲ����ľ��巽����˳��
***************************************************************************************************/

class CTemplateTester : public CPPUNIT_NS::TestFixture
{

    CPPUNIT_TEST_SUITE( CTemplateTester );
    CPPUNIT_TEST( test_fun_template_overload );
    CPPUNIT_TEST( test_tracer );
    CPPUNIT_TEST_SUITE_END();

    void test_fun_template_overload();

    //���������㷨��׷��������׷�� std::sort ��ô�Ƚ�
    void test_tracer();

    DECLARE_DEFAULT_TEST_CLASS(CTemplateTester);
};