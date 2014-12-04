#pragma once

#include <functional>
#include <iostream>

class reference_counter
{
    int m_ref_count;
public:
    reference_counter() : m_ref_count(0) {}
    virtual ~reference_counter() {}

    friend void intrusive_ptr_add_ref(reference_counter* p)
    {
        ++p->m_ref_count; 
    }
    friend void intrusive_ptr_release(reference_counter* p)
    {
        if (--p->m_ref_count==0) delete p;
    }
protected:    
    reference_counter& operator=(const reference_counter&) 
    {
        // �޲���      
        return *this;    
    }

#ifdef NEED_OPERATOR_FUNC
    public:
#else
    private:
#endif
    // ��ֹ���ƹ��캯��    
    reference_counter(const reference_counter& copy)
    {
        m_ref_count = copy.m_ref_count;
    }
};


class CMyTestData;
//TODO�� == ��Ҫ��Ԫ�����������Ա����������
bool operator == (const CMyTestData& lhs, const CMyTestData& rhs);
//bool operator <  (const CMyTestData& lhs, const CMyTestData& rhs){return false;}
class CMyTestData : public reference_counter
{
    friend bool operator==(const CMyTestData& lhs, const CMyTestData& rhs);
public:
    CMyTestData();
    CMyTestData(LONG index,BOOL bDumpInfo = TRUE);
    ~CMyTestData();

    LONG GetCurrentIndex()const { return m_Index;}
    LONG SetCurrentIndex(LONG lIndex)
    {
        LONG oldIndex = m_Index;
        m_Index = lIndex;
        return oldIndex;
    }

    //���ڲ����̰߳�ȫ�ԵĴ��� -- �Ȼ�ȡ m_Index ��ֵ��Sleepһ��ʱ����� ++/--������Ƿ��̰߳�ȫ�ķ�ʽ���ã�ֵ��ȷ��
    LONG LongTimeIncreatment(DWORD dwMilliseconds);
    LONG LongTimeDecreatment(DWORD dwMilliseconds);

    DWORD GetOtherInfo() const {return m_OtherInfo;}

    void DumpDataInfo();

    // �������캯��
    CMyTestData( const CMyTestData &copy );
private:
    LONG m_Index;  //���洫���Index����Ϊ KEY
    DWORD m_OtherInfo; //����һЩ������ֵ(Ŀǰʹ��TickCount)
    BOOL m_bDumpInfo; //�Ƿ������Ϣ

#ifdef NEED_OPERATOR_FUNC
    public:
#else
    private:
#endif

    bool operator < (const CMyTestData & other) const;
    //bool operator > (const CMyTestData & other) const;
    
    bool operator != (const CMyTestData & other);
    bool operator == (const CMyTestData & other);
    //friend std::ostream operator<< (std::ostream& o, const CMyTestData& le);  
    //friend std::istream operator>> (std::istream& i, CMyTestData& le);

    // ������ֵ������
    CMyTestData& operator =( const CMyTestData &copy );

public:
    static LONG GetTestDataCount();
    static LONG GetConstructCount();
    static LONG GetCopyConstrcutCount();
    static LONG GetAssignmentCount();
    static LONG GetCompareEqualCount();
    static LONG GetCompareLessCount();
    static LONG GetDestructCount();
private:
    static LONG s_TestDataCount;
    static LONG s_ConstructCount;
    static LONG s_CopyConstructCount;
    static LONG s_AssignmentCount;
    static LONG s_CompareEqualCount;
    static LONG s_CompareLessCount;
    static LONG s_DestructCount;
};

//std::ostream operator<< (std::ostream& o,  const CMyTestData& le) 
//{
//    o << le.m_bDumpInfo << le.m_Index << le.m_OtherInfo;
//    return o;
//}
//std::istream operator>>(std::istream& i, CMyTestData& le) 
//{
//    i >> le.m_bDumpInfo >> le.m_Index >> le.m_OtherInfo;
//    return i;
//}

class equal_CMyTestData : public std::binary_function<CMyTestData,CMyTestData,bool>
{
public:
    bool operator()(const CMyTestData & lhs, const CMyTestData & rhs) const 
    {
        return  lhs.GetCurrentIndex() == rhs.GetCurrentIndex();
    }
};

class less_Index_CMyTestData : public std::unary_function<CMyTestData, bool>
{
public:
    less_Index_CMyTestData(LONG cmpIndex)
    {
        m_CmpIndex = cmpIndex;
    }
    bool operator()(const CMyTestData & lhs) const
    {
        return lhs.GetCurrentIndex() < m_CmpIndex;
    }
private:
    LONG m_CmpIndex;
};

class less_CMyTestData_Pointer  : public std::binary_function<CMyTestData*,CMyTestData*,bool>
{
public:
    bool operator()(const CMyTestData* lhs, const CMyTestData* rhs) const 
    {
        return  lhs->GetCurrentIndex() < rhs->GetCurrentIndex();
    }
};

//����ԭ�е�ָ�������������һ��
class duplicate_abs_CMytestData : public std::unary_function<CMyTestData*, CMyTestData*>
{
public:
    //CMyTestData& operator()(const CMyTestData & lhs) const
    //{
    //    LONG curIndex = lhs.GetCurrentIndex();
    //    if ( curIndex < 0)
    //    {
    //        lhs.SetCurrentIndex(-curIndex);
    //    }
    //    return lhs;
    //}
    CMyTestData* operator()(const CMyTestData* lhs) const
    {
        CMyTestData* pNewMyTestData = new CMyTestData(*lhs);

        LONG curIndex = pNewMyTestData->GetCurrentIndex();
        if ( curIndex < 0)
        {
            pNewMyTestData->SetCurrentIndex(-curIndex);                  
        }
        return pNewMyTestData;
    }
};

enum CheckType
{
    ctDataCount,                    //TestData�ĸ�����ͨ������ȷ��û���ڴ�й©
    ctConstructCount,
    ctCopyConstructCount,
    ctAssignmentCount,
    ctCompareEqualCount,
    ctCompareLessCount,
    ctDestructCount
};

inline std::ostream& operator << (std::ostream& os, const CheckType& type)
{
    switch(type)
    {
    case ctDataCount:
        os << "ctDataCount";
        break;
    case ctConstructCount:
        os << "ctConstructCount";
        break;
    case ctCopyConstructCount:
        os << "ctCopyConstructCount";
        break;
    case ctAssignmentCount:
        os << "ctAssignmentCount";
        break;
    case ctCompareEqualCount:
        os << "ctCompareEqualCount";
        break;
    case ctCompareLessCount:
        os << "ctCompareLessCount";
        break;
    case ctDestructCount:
        os << "ctDestructCount";
        break;
    default:
        os << "uknown";
    }
    return os;
}

#define DECLARE_MYTEST_DATA_COUNT_CHECKER(name, ckType, diff , file, line) \
    CMyTestDataCountChecker name(ckType, diff, file, line)

class CMyTestDataCountChecker
{
public:
    CMyTestDataCountChecker(CheckType checkType, LONG diff, LPCSTR pszFile, DWORD dwLine);
    ~CMyTestDataCountChecker();
private:
    CheckType m_checkType;
    LONG m_diff;
    LONG m_OrignalCount;
    LPCSTR     m_pszFile;
    DWORD       m_dwLine;
};

class CMyTestDataChild : public CMyTestData
{
public:
    CMyTestDataChild(LONG index,BOOL bDumpInfo = TRUE) : CMyTestData(index, bDumpInfo)
    {
    }

};