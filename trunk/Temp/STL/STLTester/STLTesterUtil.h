#pragma once


#include <cppunit/extensions/HelperMacros.h>

#include <string>
#include <set>
#include <algorithm>

extern LONG g_forEachCount;

//! ʹ�� for_each ��ӡÿһ��Ԫ��
void PrintStringItem (std::string& StringToPrint);
void PrintIntItem (int& intToPrint);
void PrintMyTestData(CMyTestData & myDataToPrint);

//! ͳ�Ʋ�����ĸ���
bool CountFailure(int value);



//�ж������д�����Ƿ���ѡ��
class IsAFlag { 
public: 
    bool operator () (std::string& PossibleFlag) { 
        return PossibleFlag.substr(0,1)=="-"; 
    } 
}; 

//���庯��������IsFailure���������жϳɼ��Ƿ񲻼���
//���������Ļ�����������operator()����һ��true�����򷵻�false
class IsFailure
{
public:  
    IsFailure()
    {
        TRACE(_T("IsFailure::Constructor\n"));
    }
    IsFailure(const IsFailure& copy)
    {
        TRACE(_T("IsFailure::Copy Constructor\n"));
    }
    ~IsFailure()
    {
        TRACE(_T("IsFailure::Destructor\n"));
    }
    bool operator() ( int value ) 
    {
        return (value < 60);
    }
private:
    IsFailure& operator = (const IsFailure& copy)
    {
        TRACE(_T("IsFailure::operator = \n"));
        if (this == &copy)
        {
            return *this;
        }
        return *this;
    }
};

//! ʹ�ýṹ����Ϊ�������󣬲�ͨ�����캯�����ݲ��� -- ͳ���ض��ɼ����е�����
struct IsClassX
{
public:
    IsClassX(TCHAR type, int lowScore, int highScore)
    {
        m_type = type;
        m_lowScore = lowScore;
        m_highScore = highScore;

        TRACE(_T("IsClassX::Constructor ,type='%c', score between [%d,%d)\n"),m_type,m_lowScore,m_highScore);
    }
    IsClassX(const IsClassX& copy)
    {
        m_type = copy.m_type;
        m_lowScore = copy.m_lowScore;
        m_highScore = copy.m_highScore;
        TRACE(_T("IsClassX::Copy Constructor,type='%c', score between [%d,%d)\n"), m_type,m_lowScore,m_highScore);
    }
    ~IsClassX()
    {
        TRACE(_T("IsClassX::Destructor, type = %c, score between [%d,%d)\n"),m_type,m_lowScore,m_highScore);
    }

    bool operator() ( int value )
    {
        //TODO:100 ��δ���
        if (value >= m_lowScore && value < m_highScore)
        {
            return true;
        }
        return false;
    }
    TCHAR GetClassType()
    {
        return m_type;
    }
private:
    TCHAR m_type;
    int m_lowScore;
    int m_highScore;
    IsClassX& operator = (const IsClassX& copy);
};

//��set<string*> �б�֤ set ������ĸ˳�򱣴�string -- �μ� Effective STL �е�����20
struct StringPtrLess: public std::binary_function<const std::string*, const std::string*,bool>
{
    bool operator()(const std::string *ps1, const std::string *ps2) const
    {
        return *ps1 < *ps2;
    }
};

typedef std::set<std::string*, StringPtrLess> StringPtrSet;

//��������������ָ��ʱʹ�õ� �º���ģ��
struct DereferenceLess 
{
    template <typename PtrType>
    bool operator()(PtrType pT1, PtrType pT2) const		// ������ֵ���ݵģ���Ϊ����ϣ�������ǣ�����Ϊ��ָ��
    {
        return *pT1 < *pT2;
    }
};


//���͵Ľ����÷º����� -- �������͵ķº���������һ��T*ʱ�����Ƿ���һ��const T&
struct Dereference 
{
    template <typename T>
    const T& operator()(const T *ptr) const
    {
        return *ptr;
    }
};


#if 0
    //�뽨�� ʹ�ô�ġ�֧���麯���ķº�������(ʵ�����ǲ��е�) -- �� Effective STL 38
    template<typename T>
    class CBPFC:	public unary_function<T, void>      // BPFC = ��Big Polymorphic Functor Class��
    {
    private:
        CMyTestData m_MyData;					    // ������кܶ����ݣ�������ֵ���ݻ�Ӱ��Ч��
        int m_otherInfo;
    public:
        virtual void operator()(const T& val) const;// ����һ���麯���������и�ʱ�������
    };
#endif 

//����һ������һ��ָ��ʵ�����ָ���С����̬���࣬Ȼ����������ݺ��麯���ŵ�ʵ���ࣺ
template<typename T>
class CBPFC;

template<typename T>            // �����޸ĵ�BPFC����ʵ����
class CBPFCImpl :public std::unary_function<T, void> 
{
private:
    CMyTestData m_MyData;					// ��ǰ�� CBPFC ���������������������
    int m_otherInfo;
public:
    virtual ~CBPFCImpl(){};				    // ��̬����Ҫ����������

    virtual void operator()(const T& val) const
    {
    
    }
    friend class CBPFC<T>;				    // �� CBPFC ���Է�����Щ����
};

//�º����ࣨCBPFC����С����̬�ģ������Է��ʴ���״̬������Ϊ��̬
//����֧�ֺ���ʽ�Ŀ��� -- ��֤�������캯����ָ���BPFCImpl�������˺�������顣Ҳ����򵥵ĺ���Ķ��������ü���
template<typename T>
class CBPFC : public std::unary_function<T, void>	// С�ģ���̬���BPFC
{
private:
    CBPFCImpl<T> *m_pImpl;				    // ����CBPFCΨһ������
public:
    CBPFC(CBPFCImpl<T>* pImpl)
    {
        m_pImpl = pImpl;
    }
    void operator()(const T& val) const		// ���ڷ��飻
    {						
        pImpl->operator() (val);            // ����BPFCImpl��
    }
private:
    CBPFC(const CBPFC<T>& other);
};


//�ж�ֵ�Ƿ���ָ��ֵ֮��ķº�����
//ʹ�ã�
//  vector<int>::iterator i = find_if(v.begin(), v.end(),BetweenValues<int>(x, y));
template<typename T>
class BetweenValues: public std::unary_function<T, bool> 
{
public: 
    BetweenValues(const T& lowValue, const T& highValue)			// ʹ���캯���������½�
        :lowVal(lowValue), highVal(highValue){} 
    bool operator()(const T& val) const	//����val�Ƿ��ڱ����ֵ֮�� 
    {
        return (val > lowVal) && (val < highVal);
    }
private: 
    T lowVal; 
    T highVal; 
}; 

//�ж����������е�Ԫ����ͬ�ĺ���
template<typename InIter1, typename InIter2>
void container_elem_equal_assert(InIter1 first1, InIter1 last1, InIter2 first2, InIter2 last2)
{
    std::ostringstream os;
    while (first1 != last1 && first2 != last2 )
    {
        os.str("");
        os << *first1 << " == " << *first2;

        if (*first1 != *first2)
        {
            //���� if �ڷ��� ASSERT ���Եķ��������Է������öϵ㣬���ֳ���ľ���λ��
            CPPUNIT_ASSERT_MESSAGE(os.str(), *first1 == *first2);
        }
        ++first1;
        ++first2;
    }

    //���������ĸ������ -- ���˳�ѭ����Ӧ�õ����
    CPPUNIT_ASSERT(std::distance(first1, last1) == std::distance(first2, last2));
    CPPUNIT_ASSERT(first1 == last1);
    CPPUNIT_ASSERT(first2 == last2);
}

#define STLTESTER_ASSERT_CONTAINER_ELEMENT_EQUAL    container_elem_equal_assert

