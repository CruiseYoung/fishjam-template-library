#pragma once

#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* �����˹ᴩʹ����STL�еļ���ģ�������
* 
************************************************************************/

class CSTLUtilityTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CSTLUtilityTester );
    CPPUNIT_TEST( test_advance );
    CPPUNIT_TEST( test_distance );
    CPPUNIT_TEST( test_delete );
    CPPUNIT_TEST( test_boost_delete );
    CPPUNIT_TEST( test_StringPtrSet );
    CPPUNIT_TEST( test_swap );
    CPPUNIT_TEST( test_BPFC );
    CPPUNIT_TEST_SUITE_END();

    //��distance��advance��const_iteratorת����iterator -- �� Effective STL ����27
    //  advance -- ��ָ������Ŀ����������������ǰ�Ƶ���������ֵ����Ϊ����������˫���������������ʵ���������ֵ����Ϊ��
    void test_advance();

    //���ص���һ�����������裨��������������Ŀ
    void test_distance();


    //ʹ�� for_each ɾ��������new�����Ķ���ָ��--��ֹ�ڴ�й©��ͬʱ�ô��밲ȫ���׶� -- ����Ҫע��ɾ���Ķ����� virtual ��������
    void test_delete();
    void test_boost_delete();
    void test_StringPtrSet();

    //ʹ�á��������ɡ���������ʣ���� -- Effective STL ����17
    void test_swap();

    //����ʹ�ô����ݣ���̬�ķº�����
    void test_BPFC();
    DECLARE_DEFAULT_TEST_CLASS(CSTLUtilityTester);
};