#include "StdAfx.h"
#include "STLMemoryTester.h"
#include <memory>
#include <iterator>
using std::tr1::shared_ptr;
using std::tr1::enable_shared_from_this;
using std::tr1::dynamic_pointer_cast;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CEnableSharedFromThis : public enable_shared_from_this<CEnableSharedFromThis>{
public:
    static LONG s_Count;
public:
    CEnableSharedFromThis(){
        InterlockedIncrement(&s_Count);
        FTLTRACE(TEXT("CEnableSharedFromThis  Construct\n"));
    }
    virtual ~CEnableSharedFromThis(){
        InterlockedDecrement(&s_Count);
        FTLTRACE(TEXT("CEnableSharedFromThis  Destruct\n"));
    }
    shared_ptr<CEnableSharedFromThis> getSharedThis(){
        return shared_from_this();
    }
    virtual int GetIdentify(){
        return 1;
    }
};
LONG CEnableSharedFromThis::s_Count = 0;

class CEnableSharedFromThisChild : public CEnableSharedFromThis{
public:
    CEnableSharedFromThisChild(){
        FTLTRACE(TEXT("CEnableSharedFromThisChild  Construct\n"));
    }
    virtual ~CEnableSharedFromThisChild(){
        FTLTRACE(TEXT("CEnableSharedFromThisChild  Destruct\n"));
    }
    virtual int GetIdentify(){
        return 2;
    }
    int CallChildMethod(){return 3;}
};


void CSTLMemoryTester::test_auto_ptr()
{
    DECLARE_MYTEST_DATA_COUNT_CHECKER(ConstructCountChecker,ctDataCount, 0, __FILE__, __LINE__);
    {
        //���Գ�����Χ���Զ��ͷ�
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountChecker,ctDataCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker,ctConstructCount, 1, __FILE__, __LINE__);
        {
            std::auto_ptr<CMyTestData> pMyTestData(new CMyTestData(1));
            CPPUNIT_ASSERT(pMyTestData.get()->GetCurrentIndex() == 1);
        }//�˴��Ժ��Զ��ͷ�
    }

    {
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountChecker,ctDataCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker,ctConstructCount, 1, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(copyConstructChecker,ctCopyConstructCount, 0, __FILE__, __LINE__); //ֻ��ָ��Ŀ����������п������캯��

        //����auto_ptr���и�ֵʱ������Ȩת�� -- ��Ϊ��ֵʱ����Ȩת�Ƶ����⣬���Բ������ڱ�׼����
        std::auto_ptr<CMyTestData> pMyData(new CMyTestData(1));
        CPPUNIT_ASSERT(pMyData->GetCurrentIndex() == 1);

        std::auto_ptr<CMyTestData> pMyDataAssign = pMyData;
        CPPUNIT_ASSERT(NULL == pMyData.get()); //����Ȩת�ƺ�ԭ��������ָ���ΪNULL
    }

    {
        //����reset����
        LONG lCount = CMyTestData::GetTestDataCount();
        {
            std::auto_ptr<CMyTestData> pMyTestData(new CMyTestData(1));
            CPPUNIT_ASSERT(CMyTestData::GetTestDataCount() == lCount + 1);
            CPPUNIT_ASSERT(pMyTestData.get()->GetCurrentIndex() == 1);

            //ʹ��reset��������ǰ�Ļ��ͷŵ��������µ�
            pMyTestData.reset(new CMyTestData(2));
            CPPUNIT_ASSERT(CMyTestData::GetTestDataCount() == lCount + 1);
            CPPUNIT_ASSERT(pMyTestData.get()->GetCurrentIndex() == 2);

            //ʹ��reset������ͬʱ���������Ļ������ͷ�
            pMyTestData.reset();

            CPPUNIT_ASSERT(CMyTestData::GetTestDataCount() == lCount);
            CPPUNIT_ASSERT(pMyTestData.get() == NULL);

        }
        CPPUNIT_ASSERT(CMyTestData::GetTestDataCount() == lCount);
    }

    {
        //�����ֹ�ʹ�� release �ͷ� -- ���õ�ʹ�÷�ʽ
        LONG lCount = CMyTestData::GetTestDataCount();

        std::auto_ptr<CMyTestData> pMyTestData(new CMyTestData(1));
        CPPUNIT_ASSERT(NULL != pMyTestData.get());
        CPPUNIT_ASSERT(CMyTestData::GetTestDataCount() == lCount + 1);
        CPPUNIT_ASSERT(pMyTestData.get()->GetCurrentIndex() == 1);

        //����release����������ǰ��ָ�룬��˱����ֹ��ͷ�
        CMyTestData* pTestData = pMyTestData.release();
        CPPUNIT_ASSERT(NULL == pMyTestData.get());
        CPPUNIT_ASSERT(NULL != pTestData);
        CPPUNIT_ASSERT(pTestData->GetCurrentIndex() == 1);
        delete pTestData;  //�����ֹ��ͷ�

        CPPUNIT_ASSERT(CMyTestData::GetTestDataCount() == lCount);
    }
}

void CSTLMemoryTester::test_contain_assign()
{
#ifdef NEED_OPERATOR_FUNC
    typedef std::vector<CMyTestData> DataContainer;
    DataContainer srcDataContainer;
    
    //����һ��ʼ��ʱ���ǹ��캯��������ʱ���ǵ��ÿ�������
    DECLARE_MYTEST_DATA_COUNT_CHECKER(ConstructCheck,ctConstructCount, 5, __FILE__, __LINE__);
    {
        for (long i = 0; i < 5; i++)
        {
            srcDataContainer.push_back(CMyTestData(i, 0));
        }

        {
            DECLARE_MYTEST_DATA_COUNT_CHECKER(ConstructCheck,ctConstructCount, 0, __FILE__, __LINE__);
            //��ֵʱ��������е�ÿһ��Ԫ�ص��ÿ������캯�� -- 
            DECLARE_MYTEST_DATA_COUNT_CHECKER(CopyConstructCheck,ctCopyConstructCount, 5, __FILE__, __LINE__);
            //����dstDataContainer���������ں��ͷ�
            DECLARE_MYTEST_DATA_COUNT_CHECKER(DestructCheck,ctDestructCount, 5, __FILE__, __LINE__);
            DataContainer dstDataContainer;
            dstDataContainer = srcDataContainer;
        }

        {
            DECLARE_MYTEST_DATA_COUNT_CHECKER(ConstructCheck,ctConstructCount, 0, __FILE__, __LINE__);
            //assignʱ��������е�ÿһ��Ԫ�ص��ÿ������캯�� -- 
            DECLARE_MYTEST_DATA_COUNT_CHECKER(CopyConstructCheck,ctCopyConstructCount, 5, __FILE__, __LINE__);
            //����dstDataContainer���������ں��ͷ�
            DECLARE_MYTEST_DATA_COUNT_CHECKER(DestructCheck,ctDestructCount, 5, __FILE__, __LINE__);
            DataContainer dstDataContainer;
            dstDataContainer.assign(srcDataContainer.begin(), srcDataContainer.end());
        }

        {
            DECLARE_MYTEST_DATA_COUNT_CHECKER(ConstructCheck,ctConstructCount, 0, __FILE__, __LINE__);
            DECLARE_MYTEST_DATA_COUNT_CHECKER(CopyConstructCheck,ctCopyConstructCount, 5, __FILE__, __LINE__);
            DECLARE_MYTEST_DATA_COUNT_CHECKER(DestructCheck,ctDestructCount, 5, __FILE__, __LINE__);

            DataContainer dstDataContainer;
            dstDataContainer.reserve(srcDataContainer.size());
            std::copy(srcDataContainer.begin(), srcDataContainer.end(), 
                std::back_insert_iterator<DataContainer>(dstDataContainer));
        }
    }
#endif
}

void CSTLMemoryTester::test_shared_ptr()
{
    typedef shared_ptr<CMyTestData>           CMyTestDataSharePtr;
    typedef shared_ptr<CMyTestDataChild>      CMyTestDataChildSharePtr;

    {
	    //���Խ� shared_ptr ��ָ���������������������������Χ�� clear ����Զ��ͷ�
	    DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountChecker,ctDataCount, 0, __FILE__, __LINE__);
	    DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker,ctConstructCount, 4, __FILE__, __LINE__);
	    {
		    typedef std::vector<CMyTestDataSharePtr>		CMyTestDataSharePtrContainer;
		    CMyTestDataSharePtrContainer myTestDataSharedPtrs;
		    for (int i = 0; i < 4; i++)
		    {
			    myTestDataSharedPtrs.push_back(CMyTestDataSharePtr(new CMyTestData(i)));
		    }
	    }//�˴��Ժ��Զ��ͷ�
    }

    {
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountChecker,ctDataCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker,ctConstructCount, 2, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataDestructCountChecker,ctDestructCount, 2, __FILE__, __LINE__);
        CMyTestDataSharePtr     ptr1(new CMyTestData(1));
        CMyTestDataSharePtr     ptr2(new CMyTestData(2));
        {
            DECLARE_MYTEST_DATA_COUNT_CHECKER(dataDestructCountChecker,ctDestructCount, 1, __FILE__, __LINE__);
            ptr1 = ptr2;        //һ��ֻ��ָ�븳ֵ�����е�����ָ�룬�Զ��ͷ�
        }
    }

    {
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountChecker,ctDataCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker,ctConstructCount, 2, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataDestructCountChecker,ctDestructCount, 2, __FILE__, __LINE__);

        //����ָ�뱣�� ���� ���� -- ���໥ת����
        CMyTestDataChildSharePtr    pChild1(new CMyTestDataChild(1));
        CMyTestDataSharePtr         pParent1(new CMyTestData(10));


        //pChild1 = pParent1;       //ֱ������д���޷�����ͨ��
        CMyTestDataChildSharePtr pParentToChildNULL =  dynamic_pointer_cast<CMyTestDataChild>(pParent1);
        CPPUNIT_ASSERT(pParentToChildNULL == NULL);  //���� pParent1 ʵ��ָ�����͵ı���������޷���̬ת����������

        pParent1 = pChild1;         //�������͵�ָ��ָ�������͵ı��� -- ����ֱ�Ӹ�ֵ
        CMyTestDataChildSharePtr pChildNoNull =  dynamic_pointer_cast<CMyTestDataChild>(pParent1);
        CPPUNIT_ASSERT(pChildNoNull != NULL);

#if 0
        //������÷� -- ��������ü����Ļ���
        CMyTestDataSharePtr         pParent2;
        pParent2.reset((CMyTestDataChild*)pChild1.get());
#endif
    }

    {
        //����ͨ�� shared_from_this() + dynamic_pointer_cast<> ������ָ��������Ϊ ����ָ�벢��������� �ض�����

        CPPUNIT_ASSERT(CEnableSharedFromThis::s_Count == 0);
        {
            shared_ptr<CEnableSharedFromThis>   pEnableSharedFromThis(new CEnableSharedFromThisChild());
            CPPUNIT_ASSERT(pEnableSharedFromThis->GetIdentify() == 2);

            //ת��������
            shared_ptr<CEnableSharedFromThisChild> pEnableShaedChild(
                dynamic_pointer_cast<CEnableSharedFromThisChild>(pEnableSharedFromThis->getSharedThis()));
            
            CPPUNIT_ASSERT(CEnableSharedFromThis::s_Count == 1);
            CPPUNIT_ASSERT(pEnableShaedChild->GetIdentify() == 2);
            CPPUNIT_ASSERT(pEnableShaedChild->CallChildMethod() == 3);  //�����������еķ���
        }
        CPPUNIT_ASSERT(CEnableSharedFromThis::s_Count == 0);
    }
}