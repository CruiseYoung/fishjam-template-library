#include "StdAfx.h"
#include "TemplateTester.h"
#include <algorithm>

enum callType
{
    ctUnknownFun,
    ctNoneTemplateFunInt,
    ctTemplateFun2Pointer,
    ctTemplateFun2ConstChar,
    ctTemplateFun2,
    ctTemplateFun3
};

callType    g_CallType = ctUnknownFun;

//��������int�еĽϴ���
int const& myMax(int const& a, int const& b)
{
    g_CallType = ctNoneTemplateFunInt;
    return a < b ? b : a;
}

//��������ָ����ָ����Ľϴ�ֵ
template <typename T>
T* const& myMax(T* const& a, T* const& b)
{
    g_CallType = ctTemplateFun2Pointer;
    return *a < *b ? b : a;
}

const char* myMax(const char*& a, const char*& b)
{
    g_CallType = ctTemplateFun2ConstChar;
    return std::strcmp(a,b) < 0 ? b : a;
}
//���������������͵���ֵ�еĽϴ���
template <typename T>
T const& myMax(T const& a, T const& b)
{
    g_CallType = ctTemplateFun2;
    return a < b ? b : a;
}

//���������������͵���ֵ�е������
template <typename T>
const T& myMax(const T &a, const T &b, const T &c) 
{
    g_CallType = ctTemplateFun3;  
    //return ::myMax(::myMax(a,b), c);//ע�⣺����Ϊ������ g_CallType����˲���ʹ�õ���2�������� myMax ����
    return (a < b) ? ( b < c ? c : b) : ( a < c ? c : a);
}

void CTemplateTester::test_fun_template_overload()
{
    myMax(7,42,68);
    CPPUNIT_ASSERT(g_CallType == ctTemplateFun3);

    myMax(7.0,42.0);
    CPPUNIT_ASSERT(g_CallType == ctTemplateFun2);  //myMax<double>

    myMax('a','b');
    CPPUNIT_ASSERT(g_CallType == ctTemplateFun2);  //myMax<char>

    myMax(7,42);
    CPPUNIT_ASSERT(g_CallType == ctNoneTemplateFunInt);  //myMax(int)��������������ͬʱ������ѡ���ģ�庯��

    myMax<>(7,42);          
    CPPUNIT_ASSERT(g_CallType == ctTemplateFun2);    //max<int>   

    myMax<double>(7,42);
    CPPUNIT_ASSERT(g_CallType == ctTemplateFun2);   //max<douoble> -- ����Ҫ�����Ƶ�

#pragma warning(disable : 4244)
    myMax('a',42.7); //����һ�����棺C4244: ��������: �ӡ�double��ת������const int�������ܶ�ʧ����
#pragma warning(default : 4244)
    CPPUNIT_ASSERT(g_CallType == ctNoneTemplateFunInt); //ע�⣺ģ���в������Զ�����ת������˻���á���������int�������ķ�ģ�庯��

    int a = 7;
    int b = 42;
    myMax(a,b);
    CPPUNIT_ASSERT(g_CallType == ctNoneTemplateFunInt);

    int *pa = &a;
    int *pb = &b;
    myMax(pa,pb);
    CPPUNIT_ASSERT(g_CallType == ctTemplateFun2Pointer);

    std::string s1 = "hello";
    std::string s2 = " world!";
    myMax(s1,s2);
    CPPUNIT_ASSERT(g_CallType == ctTemplateFun2);  //myMax<std::string>

    const char* cc1 = "hello";
    const char* cc2 = " world!";
    myMax(cc1,cc2);
    CPPUNIT_ASSERT(g_CallType == ctTemplateFun2ConstChar); //myMax(const char*,const char*)

    char* c1 = "hello";
    char* c2 = " world!";
    myMax(c1,c2);
    CPPUNIT_ASSERT(g_CallType == ctTemplateFun2Pointer); //myMax<char*>
}

class SortTracer
{
private:
    int m_value;            //�������������
    int m_generation;       //��׷���������ɸ���
    static long s_created;  //���캯�������õĴ�����
    static long s_destroyed;//�������������õĴδ���
    static long s_assigned; //��ֵ������
    static long s_compared; //�Ƚϴ�����
    static long s_max_live; //ͬһʱ�������ڼ���Objects
    
    //���¼��㡰ͬһʱ�������ڼ���Objects��
    static void Update_Max_live()
    {
        if (s_created - s_destroyed > s_max_live)
        {
            s_max_live = s_created - s_destroyed;
        }
    }
public:
    static long creations(){ return s_created; }
    static long destructions(){ return s_destroyed; }
    static long assignments(){ return s_assigned; }
    static long comparisions(){ return s_compared; }
    static long max_live(){ return s_max_live; }
public:
    //���캯��
    SortTracer(int v = 0) : m_value(v),m_generation(1)
    {
        ++s_created;
        Update_Max_live();
        TRACE(TEXT("SortTracer Constructor: value=%d, generation = %d, total = %d\n"),m_value,m_generation, s_created - s_destroyed);
    }
    //��������
    SortTracer(const SortTracer& oth)
        :m_value(oth.m_value),m_generation(oth.m_generation)
    {
        ++s_created;
        Update_Max_live();
        TRACE(TEXT("SortTracer copy constructor: value=%d, generation = %d, total = %d\n"),m_value,m_generation, s_created - s_destroyed);
    }
    //��������
    ~SortTracer()
    {
        ++s_destroyed;
        Update_Max_live();
        TRACE(TEXT("SortTracer destroyed: value=%d, generation = %d, total = %d\n"),m_value,m_generation, s_created - s_destroyed);
    }
    //��ֵ������
    SortTracer& operator = (const SortTracer& oth)
    {
        ASSERT(this != &oth); //����ʱ���ܶ��Լ���ֵ
        ++s_assigned;
        TRACE(TEXT("SortTracer assigned: value=%d, generation = %d, total = %d\n"),m_value,m_generation, s_created - s_destroyed);
        this->m_value = oth.m_value;
        return *this;
    }
    //�Ƚ�(comparision)
    friend bool operator < (const SortTracer& lft, const SortTracer& rht )
    {
        ++s_compared;
        TRACE(TEXT("SortTracer comparision: left value=%d,right value = %d\n"),lft.m_value,rht.m_value);
        return lft.m_value < rht.m_value;
    }
    int val() const
    {
        return m_value;
    }
};

long SortTracer::s_created = 0;
long SortTracer::s_destroyed = 0;
long SortTracer::s_max_live = 0;
long SortTracer::s_assigned = 0;
long SortTracer::s_compared = 0;

void CTemplateTester::test_tracer()
{
    SortTracer input[] = {7,3,5,6,4,2,0,1,9,8};
    std::sort<>(&input[0], &input[9] + 1);

    //CPPUNIT_ASSERT(SortTracer::creations() == 15);
    //CPPUNIT_ASSERT(SortTracer::max_live() == 12);
    //CPPUNIT_ASSERT(SortTracer::assignments() == 32);
    //CPPUNIT_ASSERT(SortTracer::comparisions() == 27);
    
}