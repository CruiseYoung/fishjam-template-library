#pragma once
#include <cppunit/extensions/HelperMacros.h>

/************************************************************************
* �����������Ϊ�����ڴ�ģ�Ͷ������ģ�����⿪���ߺ�����ĳЩ16λ����ϵͳ��near��farָ������� -- ��Ŭ��ʧ����
* ��˱���Ƴɴٽ�ȫ�����ڴ�������ķ�չ��������䣨�ͻ��գ�ԭʼ�ڴ�
* ��׼��ȷ�������ʵ�ּ���ÿ���������� pointer typedef �� T* ��ͬ��ʣ�ÿ���������� reference typedef �� T& ��ͬ
* ������������״̬ -- ���������κηǾ�̬���ݳ�Ա
* ��ͬ���͵����з�������һ���ȼ�
* 
* �����Ҫд�Զ�������������밴������������(�󲿷����������ִ���������룬Ȼ���޲�һЩ��Ա����,�ر���allocate��deallocate)��
*   1.�ѷ���������һ��ģ�壬����ģ�����T������Ҫ�����ڴ�Ķ������͡� 
*   2.�ṩpointer��reference��typedef������������pointer��T*��reference��T&�� 
*   3.����Ҫ������������״̬��ͨ���������������зǾ�̬�����ݳ�Ա�� 
*   4.Ӧ�ô�����������allocate��Ա������Ҫ����Ķ�������������ֽ�����ҲӦ�üǵ���Щ��������T*ָ�루ͨ��pointer typedef����
*     ��ʹ��û��T���󱻹��졣 
*   5.һ��Ҫ�ṩ��׼������������Ƕrebindģ�塣
*
************************************************************************/

#if 0
    template<typename T>
    class SharedMemoryANocator 
    {
    public:
        typedef  T* pointer;
        typedef T& reference;
        pointer allocate(size_type numObiects, const void *localityHint = 0)
        {
            return static_cast<pointer>(mallocShared(numObiects * sizeof(T)));
        }

        void deallocate(pointer ptrToMemory, size_type numObjects)
        {
            freeShared(ptrToMiemory);
        } 
    };
    //ʹ���Զ���� Allocator
    typedef vector<double, SharedMemoryAllocator<double> >  SharedDoubleVec;

#endif 

class CSTLAllocatorTester : public CPPUNIT_NS::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( CSTLAllocatorTester );
    //CPPUNIT_TEST( testAppend );
    CPPUNIT_TEST_SUITE_END();

    //void testAppend();

    DECLARE_DEFAULT_TEST_CLASS(CSTLAllocatorTester);
};
