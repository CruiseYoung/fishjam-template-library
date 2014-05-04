#pragma once
#include <cppunit/extensions/HelperMacros.h>

/*****************************************************************************************************
* C++11(Ҳ��C++0x,2011������±�׼)-- �������Ե����򽫱�������ƣ��������߳�֧�֡� ���ͱ�̡�ͳһ�ĳ�ʼ�����Լ����ֵļ�ǿ
*   �����ڱ���ǿ�� -- 
*     ��ֵ����(typename &&) -- ���Ա�����Ϊ��constֵ���Ӷ�����ı���ֵ���﷨Ϊ typename &&��
*       ����ģ��������ʱ�����Դ���һЩ���Զ����þ�����Ӧ�����������Ĺ�������(����֧�� tuple )
*       std::move(xxx) -- ����ʹ�ñ�����������ֵ������?
*     ת������(ת�ƹ�����?) --
*     ��ʼ���б�����(std::initializer_list) -- 
*   �����ڱ���ǿ�� -- 
*     �����캯��������������(����Java)��ע�⣺��һ���캯�����������Ѿ���ɹ���Ķ��������У���ҪС���ظ���ʼ����
*     �ⲿģ��(extern, �����Ż������ٶ�) -- ��: extern template class std::vector<MyClass>; ���߱�������Ҫ��������뵥Ԫʵ������ģ��
*     ͳһ�ĳ�ʼ�� -- һ�����������κζ������ȫͳһ�����ͳ�ʼ����ʽ, CMyClass myObj{ ÿһ�����б�����ֵ�б� }�� //����myObj����������ʼ���乲�б���
*       ע�⣺ͳһ�ĳ�ʼ����ʽ����ȡ���������﷨
*   ������ǿ��
*     std::initializer_list -- ��ʼ���б����������������������ܳ�ʼ���б���Ϊ�����
*     �Է�ΧΪ������ for ѭ��(for each) -- 
*   
* Lambda -- LambdaΪC++�ṩlambda���ʽ��������������ʹ�ñ�׼���㷨ʱ�ر����(�� sort �� find ��)��
*   Lambda�������ں��е㴴���������˴������С�ĺ�������Lambda������һЩ����ȡ���ڱ������ĺ�������������ֻ�Ա��������š�
*   ʹ��lambdas��ζ�Ÿ��ٵĴ��룬������Ҫ������д����ȷ�ɢ�ڴ�������ĺ������������������ά����
*   �ص㣺���㷨�������õĴ����㸽������ν�ʺ�����
*   �﷨��[��ѡ�ıհ������б�](�����б�)->����ֵ����{ ʵ���� } -- ��ʵ���岻���أ��򷵻�ֵ���Ϳ�ʡ��
*     
* �µĹؼ���
*   auto -- ����ʱͨ�������ƶ����������ʼ����������ͬ�ı�������Ҫ��Ϊ�˽�� ģ����ʱ�����Ƶ����������� �����⣬���ɼ��ٴ�������
*   constexpr -- �������ʽ��ʹ�û��ܱ�֤ĳ�������������ڱ����ڲ�������, 
*   decltype -- �ڱ����ڸ��������ı�����ȷ��ĳ�����ʽ������
*   enum class -- ǿ����ö�٣�ʹ��ʱ������ʽָ��������(��׼C++��ö�پ�������)
*   nullptr -- ��ָ�룬������0��ָ��ֿ��������������� foo(char*) �� foo(int) �ĵ���
* �µĺ�������
*   std::reference_closure
* �µ�����
*   Լ��(concept/requires) -- �ṩ��һ�ֽ�ģ���������������Ľӿڴ��뻯�Ļ���(�����Ŀ���ǸĽ�ģ����������Ϣ������)
*   
*****************************************************************************************************/

//VS2010 �Ժ�� VisualStudio ֧���µ� 2011 C++ ��׼(c++0x)
#if _MSC_VER >= 1600  //VS2010
#	define SUPPORT_CPP2011	1
#else
#  define SUPPORT_CPP2011	0
#endif //_MSC_VER

class CCpp2011Tester : public CPPUNIT_NS::TestFixture
{

public:
	CPPUNIT_TEST_SUITE( CCpp2011Tester );
#if SUPPORT_CPP2011
	CPPUNIT_TEST( test_auto );
	CPPUNIT_TEST( test_constexpr );
	CPPUNIT_TEST( test_decltype );
	CPPUNIT_TEST( test_initializer_list );
    CPPUNIT_TEST( test_lambda );
    CPPUNIT_TEST( test_requires );
	CPPUNIT_TEST( test_right_reference );
	CPPUNIT_TEST( test_scope_for );
#endif  //SUPPORT_CPP2011
	CPPUNIT_TEST_SUITE_END();

	DECLARE_DEFAULT_TEST_CLASS(CCpp2011Tester);
private:
#if SUPPORT_CPP2011
	void test_auto();
	void test_constexpr();
	void test_decltype();

	void test_initializer_list();

    void test_lambda();

    //����Լ��
    void test_requires();

	//������ֵ���� -- Ӣ���Ƿ���ȷ?
	void test_right_reference();

	//�Է�ΧΪ������ for ѭ��
	void test_scope_for();
#endif  //SUPPORT_CPP2011
};
