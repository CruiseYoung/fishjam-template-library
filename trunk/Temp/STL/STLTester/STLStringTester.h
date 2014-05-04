#pragma once

#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* string������һ��������������ֻ��basic_string ģ�����һ��typedef ����,���Ӧ�Ļ���wstring -- û��tstring��
* basic_string �ǻ����ַ���������(Sequence)��ģ����, �������������������ĳ��ò�����ͬʱҲ�������ַ����ı�׼������
*   ͨ���ַ����ͺ����͵�����( Character Traits )��ʵ����
* �ʵ��ַ����ȽϽ������ַ��Ƚ�֮�ϡ�һ��������һ�����Դ�Сд���ַ��ȽϺ�����������ͽ���ˡ�
*  ��׼δ�涨string����ʹ�������ڴ�ʹ����ַ��Ľ�����־
*  c_str() -- ����Կ��ַ�������C����ַ���
*  data() -- ����֤�Կ��ַ�����
* �ж��ַ����Ƿ�Ϊ�գ�Ӧ�� empty ����������Ҫ�ó����Ƚ�
************************************************************************/

#if 0
//���Դ�Сд�ַ����Ƚϵĺ���
int ciStringCompare(const string& s1, const string& s2);

//���ں��Դ�Сд�ַ����Ƚϵķº����� -- �μ� Effective STL �е�����19
struct CIStringCompare : public binary_function<string, string, bool>
{
    bool operator()(const string& lhs, const string& rhs) const
    {
        return ciStringCompare(lhs, rhs);
    }
};
//�Ժ��Դ�Сд�ķ�ʽ�����ַ�����vector.
// std::sort(v.begin(),v.end(),CIStringCompare());

//����һ�ֺ��Ӵ�Сд���ַ����Ƚ��� -- �μ� Effective STL �еĸ�¼ A
struct lt_str_1 : public std::binary_function<std::string, std::string, bool> {
        struct lt_char {
            const std::ctype<char>& ct;
            lt_char(const std::ctype<char>& c) : ct(c) {}
            bool operator()(char x, char y) const {
                return ct.toupper(x) < ct.toupper(y);
            }
        };
        std::locale loc;
        const std::ctype<char>& ct;
        lt_str_1(const std::locale& L = std::locale::classic())
            : loc(L), ct(std::use facet<std::ctype<char> >(loc)) {}

        bool operator()(const std::string& x, const std::string& y) const{
            return std::lexicographical_compare(x.begin(), x.end(),
                y.begin(), y.end(),
                lt_char(ct));
        }
};
#endif 

class CSTLStringTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLStringTester );
    CPPUNIT_TEST( test_reserve );
    CPPUNIT_TEST( test_string );
    CPPUNIT_TEST( test_swap );
    CPPUNIT_TEST_SUITE_END();

    void test_reserve();
    void test_string();
    void test_swap();

    DECLARE_DEFAULT_TEST_CLASS(CSTLStringTester);
};
