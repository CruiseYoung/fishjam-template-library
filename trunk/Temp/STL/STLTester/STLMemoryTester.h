#pragma once

#include <cppunit/extensions/HelperMacros.h>

/*********************************************************************************************************
* C++���ü����������Է�Ϊ����
*   ����ʽ(�� CComPtr) -- Ҫ����Դ������ά�����ü�����ͬʱ�ṩ�������ü����Ĺ���ӿ�
*   ������ʽ(�� shared_ptr) -- ����Դ������û���κ�Ҫ��,��ȫ����������ʽ���ü�������ָ������Դ�����ⲿά�����������ü���
*    
* STL�����ṩ������ָ��auto_ptr���Խ�����׳��쳣ʱ�������ڴ�й©������
* auto_ptr �ڿ���ʱ������ָ����������Ȩ��ת�ƣ������������ָ��ᱻ��ΪNULL����ͨ�� const auto_ptr ����ֹ����
* auto_ptr ӵ������ָ��Ķ���ָ�롣��auto_ptr������ʱ������ָ��Ķ���Ҳ����������
* ���ƣ�
*   auto_ptr���ܹ�������Ȩ��
*   auto_ptr������Ϊ�����ĳ�Ա�� 
*   auto_ptr����ָ�����顣
*   ����ͨ����ֵ��������ʼ��auto_ptr ��
* 
* std::tr1::shared_ptr -- �°汾�м�����SharePtr�����ü�������ӿ���˽�еģ��޷���shared_ptr֮��ٿ�
*   �ŵ�:
*     1.�����Զ��� deleter/allocator ��
*   ���ƣ�
*     1.һ����ʼ����Դ����ʹ��shared_ptr���ͱ���һֱʹ��
*     2.����ʹ�������shared_ptrʵ��������άϵ��Դ����������
*     3.�����Դ����ĳ�Ա������Ҫ��ȡһ��ָ���Լ���shared_ptr��
*       ��Ҫ�� enable_shared_from_this<CMyClass> �̳�(���Ϊ����ʽ���ˡ�) + shared_from_this() ��������ָ������� shared_ptr
*   ����ָ�������ת��
*     std::tr1::static_pointer_cast
*     std::tr1::dynamic_pointer_cast
*
* �����thisʹ�� shared_ptr -- 
* TODO: weak_ptr
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
