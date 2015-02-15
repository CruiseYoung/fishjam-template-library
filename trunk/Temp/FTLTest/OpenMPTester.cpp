#include "StdAfx.h"
#include "OpenMPTester.h"
#include <ftlOpenMP.h>
#include <ftlSystem.h>
#include <omp.h>

COpenMPTester::COpenMPTester()
{
    m_nOpenMPMaxThreadCount = omp_get_num_procs();

    m_nProcessorsCount = CFOSInfo::Instance()->GetNumberOfProcessors();
    FTLTRACE(TEXT("m_nOpenMPMaxThreadCount=%d, m_nProcessorsCount=%d\n"), m_nOpenMPMaxThreadCount, m_nProcessorsCount);

    //CPPUNIT_ASSERT(m_nOpenMPMaxThreadCount == m_nProcessorsCount);
}

COpenMPTester::~COpenMPTester()
{

}

void COpenMPTester::test_parallel()
{
    LONG nCount = 0;

    //�ֳ�N���̲߳���ִ�У�ÿ���߳�ִ����ͬ���߼�
    #pragma omp parallel
    {
        InterlockedIncrement(&nCount);
    }

    CPPUNIT_ASSERT(nCount == m_nOpenMPMaxThreadCount);
}

void COpenMPTester::test_parallel_for()
{

    const int COUNT = 100;
    LONG nCalcCount = 0;
    LONG nThreadCount = 0;

    int x[COUNT] = { 0 };
    int y[COUNT] = { 0 };
    int sum[COUNT] = { 0 };
    for (int i = 0; i < COUNT; i++)
    {
        x[i] = i;
        y[i] = COUNT - i;
    }

    //ע��: #pragma omp parallel + #pragma omp for ��д�����Լ�дΪ #pragma omp parallel for����������� for ѭ��
    #pragma omp parallel
    {
        #pragma omp for     //ͨ�� for ���������䵽N�����е��߳���ȥ
        for(int i = 0; i < COUNT; ++i)
        {
            //ע�⣺������ִ��ѭ����ʱ�����ȷ��û��ѭ���������� x[i] = x[i-1] + y[i]; �Ͳ���
            sum[i] = x[i] + y[i];
            InterlockedIncrement(&nCalcCount);
        }
        InterlockedIncrement(&nThreadCount);
    }

    //���ڲ�������Ľ�����ͬ�� -- ���е��߳̽������ڲ��������������ֱ�������̶߳����
    for (int i = 0; i < COUNT; i++)
    {
        CPPUNIT_ASSERT(COUNT == sum[i]);
    }
    CPPUNIT_ASSERT(COUNT == nCalcCount);    //���еĸ����� for ѭ���ĸ���
    CPPUNIT_ASSERT(m_nOpenMPMaxThreadCount == nThreadCount);
}

void COpenMPTester::test_parallel_cancel()
{
    const int COUNT = 10;
    LONG nCalcCount = 0;
    LONG nThreadCount = 0;
    int sum = 0;

    //��Ϊ for ѭ���в���ʹ�� break ��ǰ����������һ����ѡ�Ľ�����ʽ��ͨ�� continue ���п����㣬�����ٽ���
    //  TODO: ��û�����������ܿ��ٽ��� ? 
    #pragma omp parallel
    {
        #pragma omp for
        for(int i = 0; i < COUNT; ++i)
        {
            if (i > (COUNT/2))
            {
                continue;
                //break;  //error C3010: 'break' : jump out of OpenMP structured block not allowed

            }
            sum += i;
            InterlockedIncrement(&nCalcCount);
        }
        InterlockedIncrement(&nThreadCount);
    }

    CPPUNIT_ASSERT(15 == sum); //COUNT Ϊ10ʱ: 0 + 1 + 2 + 3 + 4 + 5
    CPPUNIT_ASSERT(nCalcCount == (COUNT /2 + 1)); 
    CPPUNIT_ASSERT(nThreadCount == m_nOpenMPMaxThreadCount);
}

void COpenMPTester::test_variable()
{
    //����������˽������

    const int COUNT = 5;
    LONG calcCount = 0;  //���� calcCount û���ƶ��Ӿ䣬Ĭ���ǹ����
    int sum = 10;       //���� sum ʹ��reduction ָ����˽�б����� ��#pragma omp for�������ɺ��߳�Ϊ���е�˽��sum��ԭֵ��+����(sum��ԭֵ�ڱ�������10),
                        //�ٰѽ����ֵ��ԭ���Ĺ����sum������
    int j = 5;          //����jʹ���� firstprivate ��ʾ��˽�еģ��Ҹ��߳��еı���Ҫ�̳����߳��еĳ�ֵ(�˴�Ϊ5)
    int i;              //����i��Ϊѭ����������˽�е�, ʹ���� lastprivate ��ʾ�ڲ��д���������ƻ����߳��еĶ�Ӧ����

    #pragma omp parallel
    {
        #pragma omp for firstprivate(j) lastprivate(i) reduction(+: sum)
        for(i = 0; i < COUNT; ++i)
        {
            int doubleI = 2 * i;
            for(; j < doubleI; ++j)  //j ͨ�� firstprivate �̳��˳�ʼֵ(5)����˲���Ҫ��ʼ��
            {
                sum += i * j;
                InterlockedIncrement(&calcCount);
            }
        }
    }

    CPPUNIT_ASSERT(sum == 97); //sum = 10(��ʼֵ) + 3*5(iΪ3ʱ) + (4*5 + 4*6 + 4*7)(iΪ4ʱ) = 10 + 15 + 20 + 24 + 28 = 97
    CPPUNIT_ASSERT(calcCount == 4);  // iΪ3��ʱ��һ�Σ�iΪ4ʱ3��
    CPPUNIT_ASSERT(COUNT == i);
}