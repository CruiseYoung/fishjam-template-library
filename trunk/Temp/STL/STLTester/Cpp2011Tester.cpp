#include "StdAfx.h"
#include "Cpp2011Tester.h"
#include <vector>
//#include <regex>

#if SUPPORT_CPP2011  //VS2012

void CCpp2011Tester::test_auto()
{
	std::vector<int> myIntVector;
	
	
	for (auto iter = myIntVector.begin(); iter != myIntVector.end(); ++iter)
	{
		//���Ա���д�����µ���ʽ:
		//for(std::vector<int>::const_iterator iter = myIntVector.begin(); iter != myIntVector.end(); ++itr)
	}
	
}

//constexpr int GetFive() 
//{ 
//	//constexpr ʹ�� ��������Ⲣȷ�� GetFive �Ǹ������ڳ���
//	return 5;
//}

void CCpp2011Tester::test_constexpr()
{
//	int num[GetFive()] = {1, 2, 3, 4, 5};
}

void CCpp2011Tester::test_decltype()
{
	LONGLONG someTypeValue = 1;
	decltype(someTypeValue) otherIntegerVariable = 5;

	//TODO:C++ ������ж�������ȣ�
	CPPUNIT_ASSERT(sizeof(otherIntegerVariable) == sizeof(LONGLONG));  //otherIntegerVariable ��������someTypeValue������
}

//class SequenceClass
//{
//public:
//	//��ʼ���б�����
//	SequenceClass(std::initializer_list< int >list)
//	{
//
//	}
//	//ʹ�ó�ʼ���б����ͨ����
//	void SomeFunctionName(std::initializer_list<float> list)
//	{
//
//	}
//};

void CCpp2011Tester::test_initializer_list()
{
	////��ʼ���б��ǳ�����һ������������б�ĳ�Ա�Լ����Ա�����������ݱ��޷��ı�
	//SequenceClass myObject = {1, 2, 3, 4};
	//myObject.SomeFunctionName({3, 4, 5, 6});
}

void CCpp2011Tester::test_lambda()
{
    std::vector<int>  intVect(10);
    //ʹ�� generate ����֪��С�� vector ������ [1,10]
    std::generate(intVect.begin(), intVect.end(), FTL::sequence_generator<int>(1,1));

    int total = 0;
    std::for_each(intVect.begin(), intVect.end(), [&total](int x) {
        //�հ�����total��ջ����total�����ã�ʹ��ǰ�߿��Ըı���ߵ�ֵ -- TODO:�հ��������Ƿ���Ҫ��
        total += x
    });
    CPPUNIT_ASSERT(total == 55);

    //���һ��lambda������һ�����ĳ����Ա�������壬��ô��lambda�����㱻�϶�Ϊ�������Ԫ��
    //ֻ�е�lambda������SomeType��ĳ����Ա�����д���ʱ��δ�����ܹ�����
    //[](SomeType *typePtr) { typePtr->SomePrivateMemberFunction() };


    //����û�ϣ����lambda����������������ôҪô������Ӧ����Ϊģ�壬Ҫô����һ��std::function���ڱ���lambda����
    //ʹ��auto�ؼ�������Խ�lambda���������ھֲ�������
    auto myLambdaFunc = [this]() {
        this->SomePrivateMemberFunction()
    };
}

//������Ϊ LessThanComparable ��Լ�� -- ������Ҫ��С�ڲ�����
auto concept LessThanComparable < typename T >
{
    bool operator<(T, T);
}

//ʹ�� LessThanComparable Լ��������ģ�庯��
template<typename T> requires LessThanComparable<T>
const T& MyMin(const T &x, const T &y)
{
    return y < x ? y : x;
}

void CCpp2011Tester::test_requires()
{

}

//std::vector<CMyTestData>&& GetTempVector()
//{
//	std::vector<CMyTestData>	tempVector;
//	tempVector.push_back(CMyTestData(1));
//	return tempVector;
//}

void CCpp2011Tester::test_right_reference()
{
	//������ֵ���ã�һ������Ϊָ��ĳ��vector����ֵ���õ�std::vector��ת�ƹ��������ܹ��򵥵ؽ�����ֵ��Cʽ�����ָ�븴�Ƶ��µ�vector��Ȼ�󽫸���ֵ��ա�
	//����û�����鿽�����������ٱ���յ���ֵҲ�������ٱ������ݵ��ڴ� -- ��Ҫ ת�ƹ����� ��֧��
	//std::vector<CMyTestData>	myDataVector = GetTempVector();
}

void CCpp2011Tester::test_scope_for()
{
	int my_array[5] = { 1, 2, 3, 4, 5 };	
	for(int &x:			//������������������ѭ���ı���,�ñ�����������Ҳ������ѭ��֮��
		my_array)		//��ʾ������ѭ��������, ����ѭ�������仹������std::vector�����κη����������Ķ���
	{
		x *= 2;
	}
	CPPUNIT_ASSERT(x == 120);
}
#endif //SUPPORT_CPP2011
