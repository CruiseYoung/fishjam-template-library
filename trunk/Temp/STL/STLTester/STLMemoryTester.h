#pragma once

#include <cppunit/extensions/HelperMacros.h>

/*********************************************************************************************************
* STL�����ṩ������ָ��auto_ptr���Խ�����׳��쳣ʱ�������ڴ�й©������
* auto_ptr �ڿ���ʱ������ָ����������Ȩ��ת�ƣ������������ָ��ᱻ��ΪNULL����ͨ�� const auto_ptr ����ֹ����
* auto_ptr ӵ������ָ��Ķ���ָ�롣��auto_ptr������ʱ������ָ��Ķ���Ҳ����������
* ���ƣ�
*   auto_ptr���ܹ�������Ȩ��
*   auto_ptr������Ϊ�����ĳ�Ա�� 
*   auto_ptr����ָ�����顣
*   ����ͨ����ֵ��������ʼ��auto_ptr ��
* 
* std::tr1::shared_ptr -- �°汾�м�����SharePtr
*   ����ָ�������ת��
*     std::tr1::static_pointer_cast
*     std::tr1::dynamic_pointer_cast
*
* �����thisʹ�� shared_ptr -- �� enable_share_from_this<> �̳� + shared_from_this();
*********************************************************************************************************/
class CSTLMemoryTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLMemoryTester );
    CPPUNIT_TEST( test_auto_ptr );
	CPPUNIT_TEST( test_contain_assign );
	CPPUNIT_TEST( test_shared_ptr );
    CPPUNIT_TEST_SUITE_END();

    void test_auto_ptr();

    //������������ֱ�Ӹ�ֵ(=)��assign��copy ���������ַ�ʽ�������ͬ
    void test_contain_assign();
	void test_shared_ptr();

    DECLARE_DEFAULT_TEST_CLASS(CSTLMemoryTester);
};
