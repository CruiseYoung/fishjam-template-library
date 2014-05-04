#pragma once

#include <cppunit/extensions/HelperMacros.h>
#include <string>

/**********************************************************************************************************
* �㷨ͷ�ļ�����һ���ģ�溯����ɣ����г��õ��Ĺ��ܷ�Χ�漰���Ƚϡ����������ҡ��������������ơ��޸ġ�
*   �Ƴ�����ת�����򡢺ϲ��ȵȡ�
* 
* �ȶ��������һ�������е�����Ԫ���еȼ۵�ֵ�����ǵ����λ��������󲻸ı䡣
* �����ж��ַ��� -- ��Ҫ������ʵ��������Ա�׼������������Ԫ��û������
*   ����(�ɸߵ���)��partition > stable_partition > nth_element > partial_sort > sort > stable_sort
*   nth_element -- �ҳ�������Ҫ��[��]N��Ԫ�أ�����˳�����У�-- �����ҵ�ָ���ٷֵ��Ԫ��
*      //�ҵ�����ֵΪ75%��Widget,begin + goalOffset����ָ�������ȼ�Ϊ75%��Widget
*      vector<Widget>::size_type goalOffset = 0.25 * widgets.size();
*      nth_element(begin, begin + goalOffset,end, qualityCompare); 
*   sort -- ��Ԫ�ؽ�����ȫ����
*   stable_sort -- sort ���ȶ�����汾
*   partition -- ���������е�Ԫ����ʹ��������ĳ����׼��Ԫ�ض�������Ŀ�ͷ
*   stable_partition -- partition ���ȶ�����汾
*   partial_sort -- ���ȶ��Ĳ����������ҳ�����10��Ԫ�ز���˳�����У�
*   
* �Բ�֧�ֵ��������ʹ�������㷨�ķ�����
*   1.��Ԫ�ؿ�����һ��֧��������ʵ���������������ʹ���㷨��
*   2.����һ�������������������Ǹ�����ʹ���㷨��Ȼ��ͨ������������Ԫ�أ�
*   3.ʹ������ĵ�������������Ϣ�������ذ�Ԫ�ؽӺϵ���Ҫ��λ�á�
*   
* ֻ�ܲ����������ݣ���Ҫ���� sort ���򣩵��㷨��
*   binary_search��lower_bound��upper_bound��equal_range -- ��Ҫʹ�ö��ַ�����������ֵ
*   set_union��set_intersection��set_difference��set_symmetric_difference -- ����������������ʱ������ָ������
*   merge��inplace_merge -- ִ������Ч�ĵ���ϲ������㷨(��ȡ�����������䣬Ȼ�����һ������������Դ��������Ԫ�ص�����������)
*   includes -- ���һ����������ж����Ƿ�Ҳ����һ��������
*  ͨ����Ҫ�������ݣ�����죬����ǿ�󣩵��㷨��
*   unique��unique_copy -- ��ÿ�����Ԫ�ص���������ȥ����һ���������е�Ԫ��
*  ע�⣺�����ݽ��� ������㷨�ıȽϺ�����Ϊ Ӧ�ú� Ҫʹ�õ��㷨�ıȽϺ�����Ϊ һ�£�����붼������ -- ������δ����
* ��δ���������������ʱ����㷨��count��count_if��find��find_if
*   ע�⣺count/find �����������㷨ʹ�á���ȡ����жϣ��� binary_search/lower_bound �����������㷨�á��ȼۡ����ж� -- ������Ҫ���ĵ�
* 
* copy �㷨 -- ��׼STL�д�copy���ֵ��㷨��11������û�� copy_if -- ftl ��ʵ����һ��
*   copy��copy_backward��replace_copy reverse_copy��replace_copy_if unique_copy��remove_copy rotate_copy��
*   remove_copy_if��partial_sort_copy��uninitialized_copy 
*                                      
* ע�⣺<algorithm> �еĺܶ��㷨����������Ҫstd���ֿռ䣿
*
* ʹ�� replace_if �����ַ��滻
*   replace_if(str.begin(), str.end(), bind2nd(equal_to<char>(),'.',',');  <== �� '.'(��)�滻�� ','(����)
*
* ���Ͷ��㷨(C++ ��׼���ṩ���� max-heap)
*   make_heap -- �ѷ�Χ�ڵ�Ԫ������һ����
*   pop_heap -- �����������İ����Ԫ�شӶ��е�����������������ѡ����� first �� last-1 ������Ȼ����������һ���ѣ�
*     ����ʹ�������� back �����ʱ�����������Ԫ�ػ���ʹ�� pop_back ��������ɾ����
*   push_heap -- ���� first �� last-1 ��һ����Ч�Ķѣ�Ҫ������ѵ�Ԫ����λ�� last-1 ���������ɶѡ�
*     ��ָ��ú���ǰ�������Ȱ�Ԫ�ز���������
*   sort_heap -- �Է�Χ�ڵ�������������������������Ǹ�����Ķ�
*   
**********************************************************************************************************/

class CSTLAlgorithmTester : public CPPUNIT_NS::TestFixture
{
public:
    CSTLAlgorithmTester();
    ~CSTLAlgorithmTester();

    virtual void setUp();
    virtual void tearDown();


    CPPUNIT_TEST_SUITE( CSTLAlgorithmTester );
    CPPUNIT_TEST( test_adjacent_find );
    CPPUNIT_TEST( test_adjacent_difference );
	CPPUNIT_TEST( test_binary_search );

    CPPUNIT_TEST( test_copy_if );
    CPPUNIT_TEST( test_count );
    CPPUNIT_TEST( test_equal );
    CPPUNIT_TEST( test_equal_range );
    CPPUNIT_TEST( test_fill );
    CPPUNIT_TEST( test_find );
    CPPUNIT_TEST( test_find_if );
    CPPUNIT_TEST( test_find_end );
    CPPUNIT_TEST( test_find_first_of );
    CPPUNIT_TEST( test_for_each );
    CPPUNIT_TEST( test_generate );
    CPPUNIT_TEST( test_includes );
    CPPUNIT_TEST( test_inner_product );
    CPPUNIT_TEST( test_inner_merge );
    CPPUNIT_TEST( test_iter_swap );
    CPPUNIT_TEST( test_lexicographical_compare );
    CPPUNIT_TEST( test_nth_element );
    CPPUNIT_TEST( test_permutation );
    CPPUNIT_TEST( test_lower_bound );
    CPPUNIT_TEST( test_min_max_element );
    CPPUNIT_TEST( test_mismatch );
    CPPUNIT_TEST( test_search );
    CPPUNIT_TEST( test_set_difference );
    CPPUNIT_TEST( test_set_intersection );
    CPPUNIT_TEST( test_random_shuffle );
    CPPUNIT_TEST( test_remove );
    CPPUNIT_TEST( test_remove_copy );
    CPPUNIT_TEST( test_remove_copy_if );
    CPPUNIT_TEST( test_replace_if );
    CPPUNIT_TEST( test_reverse );
    CPPUNIT_TEST( test_rotate );
    CPPUNIT_TEST( test_partition );
    CPPUNIT_TEST( test_partial_sort );
    CPPUNIT_TEST( test_partial_sum );
    CPPUNIT_TEST( test_set_union );
    CPPUNIT_TEST( test_stable_sort );
    CPPUNIT_TEST( test_stable_partition );
    CPPUNIT_TEST( test_swap );
    CPPUNIT_TEST( test_swap_range );
    CPPUNIT_TEST( test_transform );
    CPPUNIT_TEST( test_unique );
    CPPUNIT_TEST( test_unique_copy );
    CPPUNIT_TEST( test_upper_bound );
    CPPUNIT_TEST( test_merge );

    CPPUNIT_TEST( test_heap );
    CPPUNIT_TEST_SUITE_END();

private:
    //!������Ȼ������ض��������ٽ�Ԫ��
    void test_adjacent_find();  

    //�����еĺ�һ����ȥ�������ڵ�ǰһ���õ��µ�����
    //����һ�������У������е�ÿ����ֵ�������˵�ǰԪ������һ��Ԫ�صĲ�
    void test_adjacent_difference();
              
    //ʹ�ö��ַ���һ���Ѿ����ź�����Range�в�������ָ��������ֵ
    void test_binary_search();  //if (binary_search(vw.begin(), vw.end(), w))

    //��������, ��Ӧ���� copy, copy_backward(���򿽱�), copy_if 
    //ʹ�� copy_XXX ʱ������ȷ��Ŀ���������㹻��Ŀռ䣬���������������Զ���չ�ģ�ͨ���� back_inserter �Ƚ����Զ���չ
    void test_copy_if();

    //@ count()��count_it ����ͳ�� ����ĳ��ֵ �� �����ض����� �Ķ���ĸ��������� count_if ��Ҫʹ�� ����ָ���������
    void test_count();

    //������������ڷ�Χ�ڵ�Ԫ�ض���ȣ��� equal ���� true 
    void test_equal();

    //����һ�Ե���������һ������lower_bound���صĵ��������ڶ�������upper_bound����ֵ -- �����ֳ��˺�Ҫ������ֵ�ȼ۵�����
    //�����������������ͬ������ζ�Ŷ���������ǿյģ����ֵû���ҵ�
    //pair<vector<Widget>::iterator,vector<Widget>::iterator> range = equal_range(vw.begin(),vw.end(), w);
    //if (range.first != range.second)  //�ҵ���,range.firstָ���һ����range.secondָ�����һ������һ��
    //��first �� second �� distance �͵��������ж������
    void test_equal_range();  

    //�������ֵ�Ŀ���������Χ�ڵ�ÿ��Ԫ��, ��Ӧ���� fill_n 
    void test_fill();

    //find,find_if -- �������в���ָ��Ԫ��,���û���ҵ�ָ���Ķ��󣬾ͻ᷵�� end() ��ֵ��Ҫ���ҵ��˾ͷ���һ��ָ���ҵ��Ķ���� iterator
    //ע�⣺ÿ�����ֻ�ܲ���һ�������Ҫ����ȫ���ģ����������ʼλ�ã�ѭ������
    void test_find();
    void test_find_if();

    //�ڷ�Χ�ڲ��ҡ������������һ�� iterator �Ա�־�ĵڶ������С������һ�γ���
    void test_find_end();

    //�ڷ�Χ�ڲ��ҡ������������һ�� iterator �Ա�־�ĵڶ������С��е�����һ��Ԫ�صĵ�һ�γ��֡�
    void test_find_first_of();

    /**
    *   for_each ����һ��iterator�ķ�Χ���ú��������޸������е��κ�Ԫ�� -- ����Ҫ��ʼ�����ȽϺ͸�iterator����
    *   ��ʼiteratorָ�����������￪ʼ��ĩβiteratorָ�����Ľ������������������������Χ��
    *   ע�⣺���for_eachҪ�����Ա����������ʹ�� bind1st��mem_fun(�μ�test_bind1st_this)
    *     ��Ӧ�ķº�������Ϊ void operator ( ) ( Type& elem ) const
    **/
    void test_for_each();

    //ʹ���ض����㷨��������,��Ӧ�Ļ��� generate_n
    void test_generate();

    //�ж� [first1, last1) ��һ��Ԫ���Ƿ񱻰���������һ�������С�ʹ�õײ�Ԫ�ص� <= ������
    void test_includes();

    //�������������ڻ� ( ��Ӧ��Ԫ����ˣ������ ) �������ڻ��ӵ�һ������ĵĳ�ʼֵ��
    void test_inner_product();

    //�ϲ������Ź�����������У�������и��������˷�Χ
    void test_inner_merge();

    //�������� ForwardIterator ��ֵ
    void test_iter_swap();

    //�����洢�����������е�ֵ
    void test_swap();

    //���ڷ�Χ�ڵ�Ԫ��������һ�����е�Ԫ��ֵ���н���
    void test_swap_range();

    //�Ƚ���������
    void test_lexicographical_compare();

    //��顰����������ǣ���һ�����������������ǣ������������ -- ͬʱ����Ƿ����end �� �Ƿ�����Ҫ��ֵ(����ʹ�á��ȼۡ�)
    //����һ�� iterator ����ָ���ڷ�Χ�ڵ����������п��Բ���ָ��ֵ�����ƻ�����˳��ĵ�һ��λ��
    void test_lower_bound();  //vector<Widget>::iterator i = lower_bound(vw.begin(), vw.end(), w);if (i != vw.end() && !(w < *i))

    //����һ�� iterator ����ָ���ڷ�Χ�ڵ����������в��� value �����ƻ�����˳������һ��λ�ã���λ�ñ�־��һ������ value ��ֵ��
    void test_upper_bound();

    //�ϲ������������У�����ŵ�����һ��������
    void test_merge();

    //��ȡ�����е���С�����ֵ,
    //��Ӧ�Ļ��� max_element(����һ�� iterator ��ָ������������Ԫ��), min_element(������С��Ԫ��)
    void test_min_max_element();

    //���еıȽ��������У�ָ����һ����ƥ���λ�ã�������һ�� iterator ����־��һ����ƥ���Ԫ��λ�á������ƥ�䣬����ÿ�������� last 
    void test_mismatch();

    // search -- ��һ������������һ�����еĵ�һ�γ��ֵ�λ��(���ַ�������)��ʹ��operator == 
    //   ���ڶ��������Ƿ��ڵ�һ�������г��֣���˳����ͬ
    //   ע�⣺������������Ƕ���(CMyTestData)���������operator==��������Ƕ���ָ�루CMyTestData*������ʹ�õ�ַ���бȽϡ�
    //   ���ƵĻ��� search_n -- ���ҳ��� n �ε�������
    void test_search();

    
    //����һ���Ź�������У����е�Ԫ�س����ڵ�һ�������У����ǲ������ڵڶ��������С�
    void test_set_difference();

    //����һ���Ź�������У����е�Ԫ���ڵ�һ�������г��֣����ǲ������ڵڶ���������
    //  ���⣺����������Ļ����Ǻ� set_difference ��ʲô����
    void test_set_symmetric_difference();

    //����һ���Ź�������У����е�Ԫ�������������ж�����
    void test_set_intersection();

    //����һ���Ź�������У����������������е����еĲ��ظ�Ԫ��
    void test_set_union();

    //next_permuttion -- ȡ����ǰ��Χ�ڵ����У���������������Ϊ��һ������
    //prev_permutation -- ȡ����Χ�ڵ����в�������������Ϊ��һ�����С������������һ�������򷵻� false
    void test_permutation();

    //����Χ�ڵ�������������ʹ����С�ڵ� n ��Ԫ�ص�Ԫ�ض���������ǰ�棬���������Ķ������ں���
    void test_nth_element();

    //�������ԭ�ȵ�˳�� -- ��Ҫrandomiter�����ֻ������ list/set �ȣ�ֻ������ vector ?
    void test_random_shuffle();

    //   remove -- ͨ���㷨remove()ʹ�ú������ĳ�Ա����ʹ�ò�ͬ�ķ�ʽ���� -- ���ı������Ĵ�С��ʣ�µ�Ԫ�ر��ֲ��䡣
    //     ����һ��ָ���µ�list�Ľ�β��iterator(����ġ����߼��յ㡱)��
    //     �ӿ�ʼ������µĽ�β�������½�βԪ�أ��ķ�Χ ������remove��ʣ������Ԫ�� -- ע�⣺û�а�ɾ�����Ԫ���Ƶ����
    //     ������list��Ա����erase������ɾ�����½�β���Ͻ�β�Ĳ��֡�
    //   ��Ҫǰ��������Ϳ���ͨ����Щ��������ֵ������
    //   �� Effective STL ���� 23, ע�������� erase() ������ɾ��
    //   ���Ƶ��� remove_if
    void test_remove();

    //�����в�ƥ���Ԫ�ض����Ƶ�һ��ָ�����������ص� OutputIterator ָ�򱻿�����ĩԪ�ص���һ��λ��
    void test_remove_copy();

    //remove_copy_if ��ν�η���false��Ԫ�ؿ����� Ŀ�ĵ��У���������Ҫɾ���ģ�
    //���� remove_copy_if �������ڴ�����(����map)��ɾ��ָ�������ݣ��Ӷ�����ʹ�� erase ʱ��ע��
    //  ��������copy������ִ�п������죬������ܿ�������ʧ��
    void test_remove_copy_if();

    //���һ����Χ�ڵ�ÿ��ֵ���������ָ���������������ض���ֵ�����滻 -- ԭ����ֵ��ô�죿ֱ�ӱ��滻��ָ��ʱ��
    //��Ӧ���� replace_copy, replace_copy_if   
    void test_replace_if();

    //���������е���, ��Ӧ���� reverse_copy
    void test_reverse();

    //����Χ�ڵ�Ԫ���Ƶ�����ĩβ���� middle ָ���Ԫ�س�Ϊ������һ��Ԫ��
    //���Ƶ��� rotate_copy 
    void test_rotate();

    //��ָ���Ĺ�������н����ȶ������� -- �������Ԫ��֮���˳���ϵ
    void test_stable_sort();

    //stable_partition ��������Ԫ�أ�ʹ������ָ��������Ԫ������ ������������Ԫ��ǰ�档��ά��������Ԫ�ص�˳���ϵ -- ��������
    //����֤���������е����˳��
    void test_stable_partition();

    //�Է�Χ��Ԫ����������ʹ������ĺ������Ѽ�����Ϊ true ��Ԫ�ض����ڽ��Ϊ false ��Ԫ��֮ǰ
    void test_partition();

    //�������������������򣬱�����Ԫ�صĸ������ÿ��Ա��ŵ���Χ��,
    //��Ӧ���� partial_sort_copy��������������и��Ƶ�����һ������
    void test_partial_sort();

    //����һ���µ�Ԫ�����У�����ÿ��Ԫ�ص�ֵ�����˷�Χ�ڸ�λ��֮ǰ����Ԫ��֮��
    void test_partial_sum();

    //�������������з�Χ�ڵ�Ԫ�� -- ��Ҫ�����ȡ��iterator(vector/deque��)
    void test_sort();
    
    //�������ڵ�ÿ��Ԫ�ض�ʩ��ĳ�����������������д������һ���ط�
    //������Ĳ��������ڷ�Χ�ڵ�ÿ��Ԫ���ϣ�������һ���µ����С����ذ汾������������һ��Ԫ���ϣ�
    //����һ��Ԫ���������������һ�����У���������ָ��������
    void test_transform();

    //����������ظ���Ԫ�أ���һ������ɾ���������ڽ����ظ�ֵ�������÷��ʳ�������Ԫ�ص���������֮�������ó�Ա���� erase -- ͬremove
    //��ÿ�����Ԫ�ص���������ȥ����һ���������е�Ԫ�� 
    //ɾ�������ظ�Ԫ�أ����������
    void test_unique();
    void test_unique_copy();

    //���Է��Ͷ��㷨��صĺ�����make_heap��pop_heap��push_heap��sort_heap
    void test_heap();

    //DECLARE_DEFAULT_TEST_CLASS(CSTLAlgorithmTester);
private:
    typedef std::map<std::string, CMyTestData*>  String2MyTestDataMap;
    typedef std::pair<std::string, CMyTestData*> String2MyTestDataPair;

    String2MyTestDataMap    m_string2MyTestDataMap;

    typedef std::list<CMyTestData* >    MyTestDataList;
    MyTestDataList                      m_MyTestDataList;
};
