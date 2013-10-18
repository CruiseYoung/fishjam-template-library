///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   ftlFunctional.h
/// @brief  Fishjam Template Library Functional Header File.
/// @author fujie
/// @version 0.6 
/// @date 03/30/2008
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FTL_FUNCTIONAL_H
#define FTL_FUNCTIONAL_H
#pragma once

#include <iosfwd>
#include <list>
#include <sstream>
#include "ftlTypes.h"
//#include <WinSock2.h>
namespace FTL
{
/*****************************************************************************************************
* �������еĳ�Ա����ָ��
*   ����ʱ��typedef ����ֵ (T::* MyFunProc)(�����б�);
*   ʹ��ʱ��(pObj->*pMyFunProc)(����);
*   ������ʹ�ó�����
*     a.��Ա����ָ������(һ������ӳ�����)��
*     b.C++ ��ʵ�� Delegate(�μ� DuiLib �е� UIDelegate.h) ��
*     c.���� MFC �е���Ϣӳ��ʱ��ͨ��һ���и������ͺ���ָ��� union(MessageMapFunctions) ���к������÷�ʽת��
*
* �﷨
*   operator bool();   -- bool ���������ã����� if( (bool) *pMyObj) { ... }
*   bool operator()(); -- ����ֵΪ bool �ĺ������ã���������Ϊ0�� �� bRet = ��*pMyObj)(); 
*****************************************************************************************************/

    //STL��û��ʵ�� copy_if -- Ϊʲô��
    //���������д�ģ�һ����ȷʵ�ֵ�copy_if
    template<typename InputIterator,typename OutputIterator,typename Predicate>
    OutputIterator copy_if(InputIterator begin,InputIterator end,OutputIterator destBegin, Predicate p)
    {
        while (begin != end) 
        {
            if (p(*begin))*destBegin++ = *begin;
            ++begin;
        }
        return destBegin;
    }

    //ͨ������ set<T*,UnreferenceLess<T*> > �У�����Ĭ�ϻ��Ϊ�Ƚ�ָ��ĵ�ַ
    template <typename T>
    struct UnreferenceLess : public std::binary_function<T, T, bool>
    {
        bool operator()(const T& _Left, const T& _Right) const
        {
            return (*_Left < *_Right);
        }
    };

    //ʹ�� for_each ɾ�������б���Ķ���ָ�����(�ṹ) -- �μ� Effective STL �е�����7
    template <typename T>
    struct ObjecteDeleter
    {
        //ע�⣺��Ҫ�� T* ptr, ���� PairDeleter �޷�����Ϊ������һ�������ͽ���ɾ��
        void operator()(const T& ptr) const  //ͨ��ָ��������Զ�ʵ����һ��operator
        {
            delete ptr;
        }
    };

    //��ɾ����ʶ��һ������ pair �� KEY
    template <typename T>
    struct NullDeleter
    {
        void operator()(const T& /*ptr*/) const
        {
            //Do Nothing
        }
    };

    //usage: for_each(map.begin(), map.end(), PairDeleter...) 
    template <
        typename KEY,
        typename VALUE,
        typename KeyDeleter = NullDeleter<KEY>,
        typename ValueDeleter = ObjecteDeleter<VALUE>
    >
    struct PairDeleter
    {
        void operator()(const std::pair<KEY,VALUE>& pa) const
        {
            KeyDeleter()(pa.first);
            ValueDeleter()(pa.second);
        }
    };

    //Longest Common Sequence -- ������󹫹�������(�����ַ��������������)
    //http://blog.csdn.net/hackbuteer1/article/details/6686931
    //http://www.codeproject.com/Articles/3697/V-Diff-A-File-Comparer-with-visual-output
    //INT CalculateLongestCommonSequence(LPCTSTR pszX, LPCTSTR pszY, LPTSTR* pStartPos);    

#if 0
#pragma TODO(���Ա�дһ��������������������)
	template <typename T, typename C>
	class CFContainerIter
	{
	public:
		CFContainerIter(C& c)
			:m_rContainer(c)
		{

		}
		//BOOL	SetContainer(C& c);
		BOOL	MoveToFirst()
		{
			m_iterator = m_rContainer.begin();
		}
		BOOL	MoveToNext()
		{
			if (m_iterator != m_rContainer.end())
			{
				++m_iterator;
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
	private:
		C&				m_rContainer;
		C::const_iter	m_iterator;
	};
#endif

    //֮ǰʹ����VS�����е� static _Kfn ����������linux�ϲ�ͨ�ã�����Լ�дһ��
    template <class K>
    const K& ftl_Kfn(const K& Val)
    {
        return (Val);
    }

    template <class K, class V>
    const K& ftl_Kfn(const std::pair<const K,V> & Val)
    {
        return (Val.first);
    }

    template <class K>
    const K& ftl_Vfn(const K& Val)
    {
        return (Val);
    }

    template <class K, class V>
    const V& ftl_Vfn(const std::pair<const K,V> & Val)
    {
        return (Val.second);
    }

    //�����Ч�ġ���ӻ���¡����ܵ�ģ�溯�� -- �μ� Effective STL ���� 24
    //ʹ�ã�
    // iterator affectedPair =	efficientAddOrUpdate(m, k, v);
    //   �����k����map m�У���Ч�ذ�pair(k, v)��ӵ�m�У�
    //   �����Ч�ذѺ�k������ֵ����Ϊv������һ��ָ����ӻ��޸ĵ�pair�ĵ�����
    template<typename MapType,typename KeyArgType,typename ValueArgtype>
    typename MapType::iterator efficientAddOrUpdate(MapType& m,const KeyArgType& k, const ValueArgtype& v)
    {
        //��Ҫ���ҳ�k��ֵ�Ƿ���map�У���������������������������ǣ����ñ��������� -- ʹ��low_bound��ԭ��
        typename MapType::iterator Ib = m.lower_bound(k);  // �ҵ�k�ڻ�Ӧ�������

        if(Ib != m.end() && !(m.key_comp()(k, Ib->first))) // ���Ibָ��һ��pair���ļ��ȼ���k
        {
            Ib->second = v;	// �������pair��ֵ
            return Ib;      // ����ָ��pair�ĵ�����
        }
        else
        {   //����
            typedef typename MapType::value_type MVT;
            //ʹ����insert�ġ���ʾ����ʽ -- ָ����Ҫ�����λ��Ib,���Խ�����ʱ���̯Ϊ����ʱ�� -- ���Ƕ���ʱ��
            return m.insert(Ib, MVT(k, v));	// ��pair(k, v)��ӵ�m������ָ����mapԪ�صĵ�����
        }
    }

    //��set��map�и���key������ӽ���Ԫ�أ�ͨ������ķº���(F func)���ж��ĸ����ӽ���Ҫ�Ķ���
    //����ҵ�������������ӽ��ģ�����true,�� retIter Ϊ��ӽ�Ԫ�صĵ�����������Ҳ���������false
    // F ���������½ṹ�ĺ���ָ���º���
    //   int operator()(const key_type* pPre, const key_type* pWant, const key_type* pNext)
    //   ע�⣺
    //     1.����ֵ������ : С��0 ��ʾ pPre ���ӽ��� ���� 0 ��ʾ���������ӽ��� ����0 ��ʾ��һ���ӽ�
    //     2.pPre �� pNext ������ΪNULL���� pWant ������ΪNULL

    template<typename C, typename F>
    bool find_nearest(const C& container, 
        const typename C::key_type& want, 
        F func,
        typename C::const_iterator& retIter)
    {
        if(container.empty())
        {
            return false;
        }

        retIter = container.end();
        bool bResult = false;

        //�鿴�Ƿ�����Ҫ�����ݣ����߸ò����λ��(��ȵĻ��һ��)
        typename C::const_iterator nextIter = container.lower_bound(want);
        typename C::const_iterator preIter = nextIter;

        int retCmp = 0; 
        const typename C::key_type* pPreValue = NULL;
        const typename C::key_type* pNextValue = NULL;

        //�ҵ��м�
        if (nextIter != container.end())
        {
            //ֱ���ҵ��ȼ۵�
            if(!container.key_comp()(want, ftl_Kfn(*nextIter)))
            {
                bResult = true;
                retIter = nextIter;
                return bResult;
            }

            preIter--;
            if(preIter != container.end())
            {
                pPreValue = &ftl_Kfn(*preIter);
            }
            pNextValue = &ftl_Kfn(*nextIter);
        }
        else
        {
            //�����˵��û�����ݣ�������Ҫ��������ֵ
            nextIter = (--container.end());
            pNextValue = &ftl_Kfn(*nextIter);
        }

        retCmp = func(pPreValue,&want,pNextValue);
        if(retCmp < 0)
        {
            bResult = true;
            retIter = preIter;
        }
        else if(retCmp > 0)
        {
            bResult = true;
            retIter = nextIter;
        }
        //else -- Ϊ0,����û���ҵ���retIter �� bResult ֮ǰ�Ѿ�����ֵ��

        return bResult;
    }

    ////STL�����ı��� -- ����ģ����������Կ����ȸ����ȹ�Ȳ�ͬ���㷨
    //template <typename Predicate, template Functor>
    //int ForSubTree(const HATIterater& iRoot, Predicate pred, Functor f){}

    template<typename T>
    FTLINLINE T& ToRef(T * p)
    {
        FTLASSERT(NULL != p);
        return *p;
    }

    template<typename T>
    T& ToRef(const std::auto_ptr<T>& sp)
    {
        FTLASSERT(NULL != sp.get());
        return *(sp.get());
    }

    template<typename T>
    T* ToPtr(T &obj)
    {
        return &obj;
    }

    FTLINLINE size_t Split(const tstring& text, 
        const tstring& delimiter,
        bool bWithDelimeter,
        std::list<tstring>& tokens)
    {
        size_t len = text.length();
        size_t start = text.find_first_not_of(delimiter); //�ҵ���һ�����Ƿָ�����
        size_t stop = 0;
        while ( (start >=0) && (start < len))
        {
            stop = text.find_first_of(delimiter, start); //�ҵ���֮��ĵ�һ���ָ���
            if( (stop < 0) || stop > len)
            {
                stop = len;
            }
            if (bWithDelimeter && start > 0)
            {
                tokens.push_back(text.substr(start - 1, stop - start + 1));
            }
            else
            {
                tokens.push_back(text.substr(start, stop - start));
            }
            start = text.find_first_not_of(delimiter, stop + 1);
        }
        return tokens.size();
    }

    FTLINLINE int GetRandomArray(int from, int to, std::vector<int>& result)
    {
        FTLASSERT(from <= to);
        int size = to - from + 1;
        if (size <= 0)
        {
            return 0;
        }

        std::vector<int> tmpVector;
        tmpVector.resize(size);

        //��ʼ��
        for (int i = from; i <= to; i++)
        {
            tmpVector[i - from] = i;
        }
        result.resize(size);

        //time_t now = 0;
        srand(GetTickCount());

        for (int i = 0; i < size; i++)
        {
            int index = rand() % (size-i);
            result[i] = tmpVector[index];
            tmpVector[index] = tmpVector[size - i - 1];
        }
        return size;
    }

    //һ�������������������� std::generate, ʹ�÷����� test_generate
    //  std::generate(intVect.begin(), intVect.end(), sequence_generator<int>(1,1));
    //  
    template<typename T>
    struct sequence_generator
    {
    public:
        sequence_generator(const T& _start, const T& _step)
            :m_start(_start)
            ,m_step(_step)
        {

        }
        T operator()()
        {
            T e = m_start;
            m_start += m_step;
            return e;
        }
    private:
        T m_start;
        T m_step;
    };
    template <typename InType, typename OutType, typename Fun>
    struct sequence_generator_ex
    {
    public:
        sequence_generator_ex(const InType& _start, const InType& _step)
            :m_start(_start)
            ,m_step(_step)
        {

        }
        OutType operator()()
        {
            OutType e = Fun(m_start);
            m_start+= m_step;
            return e;
        }
    private:
        InType m_start;
        InType m_step;
    };

    /************************************************************************
    * ���ÿ���Luhn�㷨
    *   ���ִ�������ÿż��λ��2,δ��2�����ֺ͸ղŵĽ����ӡ�
    *   һλ���ֵ�ֱ����ӣ���λ������ֱ����(�� 7x2��Ϊ14�������Ϊ 1+4=5��
    *   �����ۺϿɱ� 10 ����������Ч
    ************************************************************************/
    FTLINLINE int LuhnCalc(const std::string& strInput)
    {
        UNREFERENCED_PARAMETER(strInput);
        FTLASSERT(FALSE);
        return 0;
    }

    //��������������� -- �� google �Ĵ���������
    class binarystream
    {
    public:
        explicit binarystream(std::ios_base::openmode mode = std::ios_base::out |std::ios_base::in)
            : m_stream(mode) {}
        explicit binarystream(const std::string &str,  
            std::ios_base::openmode mode = std::ios_base::out|std::ios_base::in)  
            : m_stream(str, mode) {}  
        explicit binarystream(const char *str, size_t size,
            std::ios_base::openmode mode = std::ios_base::out|std::ios_base::in)
            : m_stream(std::string(str, size), mode) {}

        binarystream &operator>>(std::string &str)
        {
            u_int32_t length;
            *this >> length;
            if (eof())
            {
                return *this;
            }
            if (length == 0) {
                str.clear();
                return *this;
            }
            std::vector<char> buffer(length);
            m_stream.read(&buffer[0], length);
            if (!eof())
            {
                str.assign(&buffer[0], length);
            }
            return *this;
        }
        binarystream &operator>>(u_int8_t &u8)
        {
            m_stream.read((char *)&u8, 1);
            return *this;
        }
        binarystream &operator>>(u_int16_t &u16)
        {
            u_int16_t temp;
            m_stream.read((char *)&temp, 2);
            if (!eof())
            {
                u16 = temp;
            }
            return *this;
        }
        binarystream &operator>>(u_int32_t &u32)
        {
            u_int32_t temp;
            m_stream.read((char *)&temp, 4);
            if (!eof())
            {
                u32 = temp;
            }
            return *this;
        }
        binarystream &operator>>(u_int64_t &u64)
        {
            u_int32_t lower, upper;
            *this >> lower >> upper;
            if (!eof())
            {
                u64 = static_cast<u_int64_t>(lower) | (static_cast<u_int64_t>(upper) << 32);
            }
            return *this;
        }

        binarystream &operator<<(const std::string &str)
        {
            u_int32_t length = (u_int32_t)(str.length());
            *this <<  length;
            m_stream.write(str.c_str(), length);
            return *this;
        }
        binarystream &operator<<(u_int8_t u8)
        {
            m_stream.write((const char*)&u8, 1);
            return *this;
        }
        binarystream &operator<<(u_int16_t u16)
        {
            m_stream.write((const char*)&u16, 2);
            return *this;
        }
        binarystream &operator<<(u_int32_t u32)
        {
            m_stream.write((const char*)&u32, 4);
            return *this;
        }
        binarystream &operator<<(u_int64_t u64)
        {
            // write 64-bit ints as two 32-bit ints, so we can byte-swap them easily
            u_int32_t lower = static_cast<u_int32_t>(u64 & 0xFFFFFFFF);
            u_int32_t upper = static_cast<u_int32_t>(u64 >> 32);
            *this << lower << upper;
            return *this;
        }

        bool eof() const { return m_stream.eof(); }
        void clear() { m_stream.clear(); }
        std::string str() const { return m_stream.str(); }
        void str(const std::string &s){ m_stream.str(s); }
    
        // Seek both read and write pointers to the beginning of the stream.
        void rewind(){
            m_stream.seekg (0, std::ios::beg);
            m_stream.seekp (0, std::ios::beg);
        }
    private:
        std::stringstream m_stream;
    };

    //��¡������ -- http://googlechinablog.com/2007/07/bloom-filter.html
    //  �ھ�����Hash�����У�ͨ�������ͬ�� hash ����������Ϣָ�ƣ������ж�һ��Ԫ���Ƿ���ڣ����Խ�ԼΪ 1/8~1/4 �Ŀռ��С
    //  �ܳ��Ķ�����������һϵ�����ӳ�亯��
    //�ô������١�ʡ�ռ�
    //���⣺������©�������м�С�Ŀ������У������Ĳ��Ȱ취Ϊ ����һ��������
    template <typename T>
    class BloomFilter
    {
    public:
    };
}//namespace FTL

#endif //FTL_FUNCTIONAL_H