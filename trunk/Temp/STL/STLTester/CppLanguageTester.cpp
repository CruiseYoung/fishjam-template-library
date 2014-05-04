#include "StdAfx.h"
#include "CppLanguageTester.h"

//�ṹ���������ָ��,��ֱ�ӽ�ֵ������ȥ -- ͨ���Ǵ����
//  ��˱���д������ֵ�����Ϳ������캯��
struct BadStructPointerAssign
{
    //ʲô����д�����õ���ǳ������������ָ��ָ����ͬ�ĵ�ַ
    int iValue;
    CMyTestData*    pData;
};

struct GoodStructPointrAssign
{
    int iValue;
    CMyTestData*    pData;

    GoodStructPointrAssign()
    {
        iValue = 0;
        pData = NULL;
    }

    GoodStructPointrAssign(const GoodStructPointrAssign& other)
        :pData(NULL)
    {
        *this = other;
    }

    GoodStructPointrAssign& operator= (const GoodStructPointrAssign& other)
    {
        if(this != &other)
        {
            this->iValue = other.iValue;
            SAFE_DELETE(this->pData);
            if (other.pData)
            {
                this->pData = new CMyTestData(*other.pData);
            }
        }
        return *this;
    }
    ~GoodStructPointrAssign()
    {
        SAFE_DELETE(pData);
    }
};

//�ṹ�����������������ڸ�ֵʱ���Զ������俽������ -- ͨ������ȷ��
struct GoodStructObjectAssign
{
    int iValue;
    CMyTestData    data;
};

//ʹ�ó�ʼ���б�ķ�ʽ
class UseConstrcutInitList
{
public:
    UseConstrcutInitList(LONG i)
        :m_Data(i)
    {

    }
    ~UseConstrcutInitList()
    {
        
    }
private:
    CMyTestData m_Data;
};

#ifdef NEED_OPERATOR_FUNC

//ʹ�ù��캯���и�ֵ�ķ�ʽ
class UseConstrcutAssignment
{
public:
    UseConstrcutAssignment(BOOL bAssigneFromLONG, LONG i)
    {
        //������д����Ȼ��ͬ����ʵ��һ��
        //�˴�ֱ��ʹ�������͸�ֵ����ķ�ʽ������ 1��ȱʡ���� + 1�� LONG �Ĺ��� + 1�θ�ֵ���� + 2������
        if(bAssigneFromLONG)
        {
            m_Data = i;
        }
        else
        {
            //������캯������ excplit ���εĻ���ֻ�������ַ�ʽ������ 1��ȱʡ���� + 1�� LONG �Ĺ��� + 1�θ�ֵ���� + 2������
            m_Data = CMyTestData(1);
        }
    }
    ~UseConstrcutAssignment()
    {
        
    }
private:
    CMyTestData m_Data;
};
#endif

//////////////////////////////////////////////////////////////////////////

CCppLanguageTester::CCppLanguageTester()
{
    m_bCallGetMyTestDataInstance = FALSE;
}

CCppLanguageTester::~CCppLanguageTester()
{

}

void CCppLanguageTester::test_bad_struct_pointer_assign()
{
    DECLARE_MYTEST_DATA_COUNT_CHECKER(dataCountChecker, ctDataCount, 0, __FILE__, __LINE__);
    LONG curTestDataCount = CMyTestData::GetTestDataCount();
    
    BadStructPointerAssign badStructSrc = {0};
    badStructSrc.iValue = 100;
    badStructSrc.pData = new CMyTestData(badStructSrc.iValue);
    
    BadStructPointerAssign badStructDest = badStructSrc;
    //�ṹ�帳ֵʱû������ = ���ţ��ǰ���λ�����ķ�ʽ����,��������� CMyTestData ��ָ�룬��ָ�����ͬһ��ʵ��
    CPPUNIT_ASSERT(badStructSrc.iValue == badStructDest.iValue);
    CPPUNIT_ASSERT(badStructSrc.pData == badStructDest.pData);

#pragma TODO ( " ֻ���ͷ�һ�Σ�����ͻ���� double free �Ĵ���" )
    SAFE_DELETE(badStructSrc.pData);
    //SAFE_DELETE(badStructDest.pData);
    badStructDest.pData = NULL;

    CPPUNIT_ASSERT(curTestDataCount == CMyTestData::GetTestDataCount());
}

void CCppLanguageTester::test_construct_init_list()
{
    {
        //ʹ�ó�ʼ���б�ķ�ʽ
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataChecker, ctDataCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(copyConstructCountChecker, ctCopyConstructCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker, ctConstructCount, 1, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(destructCheck, ctDestructCount, 1, __FILE__, __LINE__);
        UseConstrcutInitList  useInitListObj(10);
    }

#ifdef NEED_OPERATOR_FUNC
    {
        //ʹ�ù��캯���и�ֵ�ķ�ʽһ������ֱ�Ӹ�ֵ����
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataChecker, ctDataCount, 0, __FILE__, __LINE__);

        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker,ctConstructCount, 2, __FILE__, __LINE__);         //һ��ȱʡ���졢һ�δ�LONG�����Ĺ���
        DECLARE_MYTEST_DATA_COUNT_CHECKER(assignmentCountChecker,ctAssignmentCount, 1, __FILE__, __LINE__);       //һ�θ�ֵ����
        DECLARE_MYTEST_DATA_COUNT_CHECKER(destructCheck,ctDestructCount, 2, __FILE__, __LINE__);                  //��������
        DECLARE_MYTEST_DATA_COUNT_CHECKER(copyConstructCountChecker,ctCopyConstructCount, 0, __FILE__, __LINE__); //ע�⣺û�п�������
        UseConstrcutAssignment  useAssignmentObj(TRUE, 11);
    }

    {
        //ʹ�ù��캯���и�ֵ�ķ�ʽ������������(���캯������ explict ����
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCountChecker,ctConstructCount, 2, __FILE__, __LINE__);         //һ��ȱʡ���졢һ�δ�LONG�����Ĺ���
        DECLARE_MYTEST_DATA_COUNT_CHECKER(assignmentCountChecker,ctAssignmentCount, 1, __FILE__, __LINE__);       //һ�θ�ֵ����
        DECLARE_MYTEST_DATA_COUNT_CHECKER(destructCheck,ctDestructCount, 2, __FILE__, __LINE__);                  //��������
        DECLARE_MYTEST_DATA_COUNT_CHECKER(copyConstructCountChecker,ctCopyConstructCount, 0, __FILE__, __LINE__); //ע�⣺û�п�������
        UseConstrcutAssignment  useAssignmentObj(FALSE, 11);
    }
#endif

}

void CCppLanguageTester::test_exception_lifecycle()
{
    {
        //�����׳� new ����쳣������ catch ָ�����
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataChecker,ctDataCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(destructCheck,ctDestructCount, 1, __FILE__, __LINE__);
        try
        {
            throw new CMyTestData(1);
            ASSERT(FALSE);      //���ᵽ������
        }
        catch (/*const*/ CMyTestData* e) //�Ƿ��const������
        {
            //new�������쳣�����ֹ�delete,��������ڴ�й©
            delete e;
        }
    }


    {
        //�ر�ע��: ʹ�� catch(...) �����쳣�Ļ���������ڴ�й© 
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataChecker,ctDataCount, 1, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructCheck,ctConstructCount, 1, __FILE__, __LINE__);

        //new�������쳣û�б��ͷţ�����ڴ�й© -- Ҳ˵�����׳��쳣ʱ����Ҫʹ�� new �ķ�ʽ
        DECLARE_MYTEST_DATA_COUNT_CHECKER(destructCheck,ctDestructCount, 0, __FILE__, __LINE__);  
        try
        {
            throw new CMyTestData(1);
            ASSERT(FALSE);      //���ᵽ������
        }
        catch (...)//�ɵ��㲶���쳣
        {
            TRACE(TEXT("in catch ...\n"));
        }
    }

    {
        //����ֲ��������쳣������ catch ���ò��� -- �����ֵ��
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataChecker,ctDataCount, 0, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructChecker,ctConstructCount, 1, __FILE__, __LINE__);
        DECLARE_MYTEST_DATA_COUNT_CHECKER(destructCheck,ctDestructCount, 1, __FILE__, __LINE__);
        try
        {
            throw CMyTestData(2);
            ASSERT(FALSE);      //���ᵽ������
        }
        catch (/*const*/ CMyTestData& e) //�Ƿ��const������
        {
            TRACE(TEXT("In catch (CMyTestData& e), e.GetCurrentIndex=%d\n"), e.GetCurrentIndex());
        }
    }

}

void CCppLanguageTester::test_good_struct_pointer_assign()
{
    DECLARE_MYTEST_DATA_COUNT_CHECKER(dataChecker,ctDataCount, 0, __FILE__, __LINE__);
    DECLARE_MYTEST_DATA_COUNT_CHECKER(destructCheck,ctDestructCount, 2, __FILE__, __LINE__);
    DECLARE_MYTEST_DATA_COUNT_CHECKER(constructChecker,ctConstructCount, 1, __FILE__, __LINE__);
    DECLARE_MYTEST_DATA_COUNT_CHECKER(copyConstructCheck,ctCopyConstructCount, 1, __FILE__, __LINE__);
    {
        GoodStructPointrAssign goodStructSrc;
        goodStructSrc.iValue = 100;
        goodStructSrc.pData = new CMyTestData(goodStructSrc.iValue);

        CPPUNIT_ASSERT(goodStructSrc.pData->GetCurrentIndex() == 100);

        //�����˸�ֵ��������ָ����������
        GoodStructPointrAssign goodStructDest = goodStructSrc ;
        CPPUNIT_ASSERT(goodStructSrc.pData->GetCurrentIndex() == goodStructDest.pData->GetCurrentIndex());

        //�����ĵ�ַ��һ��
        CPPUNIT_ASSERT(goodStructSrc.pData != goodStructDest.pData);
    }
}

void CCppLanguageTester::test_good_struct_object_assign()
{
    DECLARE_MYTEST_DATA_COUNT_CHECKER(dataChecker,ctDataCount, 0, __FILE__, __LINE__);
    DECLARE_MYTEST_DATA_COUNT_CHECKER(destructCheck,ctDestructCount, 2, __FILE__, __LINE__);
    DECLARE_MYTEST_DATA_COUNT_CHECKER(constructChecker,ctConstructCount, 1, __FILE__, __LINE__);
    DECLARE_MYTEST_DATA_COUNT_CHECKER(copyConstructCheck,ctCopyConstructCount, 1, __FILE__, __LINE__);
    {
        GoodStructObjectAssign goodStructSrc;
        goodStructSrc.iValue = 100;

        //���õ���ȱʡ���캯��
        CPPUNIT_ASSERT(goodStructSrc.data.GetCurrentIndex() == -1);
        goodStructSrc.data.SetCurrentIndex(10);

        //��Ȼ�ṹ��û�����ؿ��������ֵ��������C++������е����Զ����ÿ�������
        GoodStructObjectAssign goodStructDest = goodStructSrc;
        CPPUNIT_ASSERT(goodStructSrc.data.GetCurrentIndex() == goodStructDest.data.GetCurrentIndex());

        //�����ĵ�ַ��һ��
        CPPUNIT_ASSERT(&(goodStructSrc.data) != &(goodStructDest.data));
    }
}

class BigClass
{
public: 
   BigClass()   {} 
   ~BigClass(){} 
   char   BigArray[0x7FFFFFFF]; 
};

int my_new_handler(size_t) {
    throw std::bad_alloc();
    return 0;
}

void unfunction( ) 
{
    throw std::bad_alloc();
}


void CCppLanguageTester::test_cpp_alloc_fail()
{
    //����BigClass�Ŀռ�ʧ�ܣ��������� nothrow,��˻᷵�� NULL
    {
        BigClass* pFailObj = new (std::nothrow) BigClass();
        CPPUNIT_ASSERT(NULL == pFailObj);
    }

    {
        BOOL bThrowException = FALSE;
        _PNH _old_new_handler = NULL;
        //set_unexpected(unfunction);
        _old_new_handler = _set_new_handler(my_new_handler);
        BOOL bCatchInDot = FALSE;
        try
        {
            BigClass* pTemp = new BigClass();
            SAFE_DELETE(pTemp);
        }
        catch (std::bad_alloc& e)
        {
            bThrowException = TRUE;
        }
        catch(std::exception& e)
        {
            bThrowException = TRUE;
        }
        catch(...)
        {
            FTLTRACE(TEXT("In Catch new Faild\n"));
            bCatchInDot = TRUE;
        }
        _set_new_handler(_old_new_handler);

#pragma TODO(Ϊʲô bad_alloc ������)
        CPPUNIT_ASSERT(bCatchInDot);

        //CPPUNIT_ASSERT(bThrowException);
    }

    {
        //BigClass* pTemp = new( std::nothrow ) BigClass();
        //CPPUNIT_ASSERT(NULL == pTemp);
    }
}

void CCppLanguageTester::test_cpp_alloc_physical_memory()
{
    MEMORYSTATUS startMemoryStatus = {0}; 
    MEMORYSTATUS afterNewMemoryStatus = {0};
    MEMORYSTATUS afterAccessMemoryStatus = {0};

    GlobalMemoryStatus(&startMemoryStatus);
    
    //�������10M���ڴ�ռ�
    const int nAllocSize = 10 * 1024 * 1024;
    BYTE *pBuff = new BYTE[ nAllocSize ];
    GlobalMemoryStatus(&afterNewMemoryStatus);

    ZeroMemory(pBuff, nAllocSize);
    GlobalMemoryStatus(&afterAccessMemoryStatus);

    delete [] pBuff;

    FTLTRACE(TEXT("Memory Status: Start -> AfterNew -> AfterAccess\n")), 
    FTLTRACE(TEXT("  AvailPhys: %fM -> %fM -> %fM\n"), 
        (float)startMemoryStatus.dwAvailPhys/1024/1024,
        (float)afterNewMemoryStatus.dwAvailPhys/1024/1024,
        (float)afterAccessMemoryStatus.dwAvailPhys/1024/1024);
    FTLTRACE(TEXT("  AvailVirtual: %fM -> %fM -> %fM\n"), 
        (float)startMemoryStatus.dwAvailVirtual/1024/1024,
        (float)afterNewMemoryStatus.dwAvailVirtual/1024/1024,
        (float)afterAccessMemoryStatus.dwAvailVirtual/1024/1024);

    //����new�Ժ��ڴ漴�����
    CPPUNIT_ASSERT(startMemoryStatus.dwAvailPhys - afterNewMemoryStatus.dwAvailPhys >= nAllocSize);
    
}

void CCppLanguageTester::test_anonymous_temp_objcet_Life()
{

    {
        LONG oldCount = CMyTestData::GetTestDataCount();

        //����������δ���������ڣ����̾��ͷ���
        CMyTestData(1);

        LONG newCount = CMyTestData::GetTestDataCount();
        CPPUNIT_ASSERT(newCount == oldCount);
    }

    DECLARE_MYTEST_DATA_COUNT_CHECKER(dataChecker,ctDataCount, 0, __FILE__, __LINE__);
    {
        LONG oldCount = CMyTestData::GetTestDataCount();
        //����ľֲ��������������Ƶ�����������
        CMyTestData  tmpData(1);
        LONG newCount = CMyTestData::GetTestDataCount();

        //��ʱ��δ���������ڣ���˻���һ��
        CPPUNIT_ASSERT(newCount == oldCount + 1);
    }
}

CMyTestData& GetMyTestDataInstance()
{
    //�����еľ�̬������ռ�õ��ڴ��ڳ�������ʱ���䣬��ֱ����һ�ε���ʱ�ų�ʼ��(����)����ֱ�������˳�ʱ���ͷ�
    static CMyTestData s_MyTestData;
    return s_MyTestData;
}
void CCppLanguageTester::test_static_var_life()
{
    if(!m_bCallGetMyTestDataInstance)
    {
        m_bCallGetMyTestDataInstance = TRUE;
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataChecker,ctDataCount, 1, __FILE__, __LINE__);            //��һ�ε���ʱ��������ʵ������
        DECLARE_MYTEST_DATA_COUNT_CHECKER(constructChecker,ctConstructCount, 1, __FILE__, __LINE__);  //�˴�������һ��ȱʡ����
        CMyTestData& instanceData = GetMyTestDataInstance();

    }
    else
    {
        DECLARE_MYTEST_DATA_COUNT_CHECKER(dataChecker, ctDataCount, 0, __FILE__, __LINE__);  //���������µ�ʵ��
        CMyTestData& instanceData = GetMyTestDataInstance();
    }
}

//ö��ֵ��������Ϊö�ٵ���Χ������������ͬ��ö�ٲ��ܰ���������ͬ�ĳ�Ա��
enum Enum1
{
    someType,   //ȱʡ�Ǵ�0��ʼ
};
enum Enum2
{
#if 0
    someType /*= 1*/,  //��仰����Ϊ�� Enum1::someType ��ͻ������ɱ������ -- C2365
#endif 
    someTypeInEnum2,
};
void CCppLanguageTester::test_enum()
{
    Enum1 myEnum = someType;
    CPPUNIT_ASSERT(myEnum == 0);
    CPPUNIT_ASSERT(Enum2::someTypeInEnum2 == 0);
}