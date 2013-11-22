#ifndef FTL_MEM_H
#define FTL_MEM_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlMem.h requires ftlbase.h to be included first
#endif

#include "ftlBase.h"
#include "ftlThread.h"
#include "ftlFunctional.h"

/********************************************************************************************
* EM64T -- Intel �������ڴ�64����
*
* ��C++�У��ڴ�ֳ�5���������Ƿֱ��Ƕѡ�ջ�����ɴ洢����ȫ��/��̬�洢���ͳ����洢����
*   �� -- ��new������ڴ�飬��Ҫdelete����δ��Ӧ���ͷţ������ڴ�й¶��
*   ���ɴ洢�� -- ��malloc�ȷ�����ڴ�飬�Ͷ�ʮ�����ƣ�����free��������
*   ջ -- ����ִ��ʱ�������ھֲ������Ĵ洢��Ԫ������ִ�н���ʱ��Щ�洢��Ԫ�Զ����ͷš�Ч�ʸߣ��������ޣ�
*      �о�̬����(�ֲ����������������)�Ͷ�̬����(alloca����)���ַ�ʽ
*   ȫ��/��̬�洢�� -- ȫ�ֱ����;�̬���������䵽ͬһ���ڴ��У�C������ȫ�ֱ�����Ϊ��ʼ���ĺ�δ��ʼ���ģ���
*   �����洢�� -- ��ų������������޸�
*
* ����new��delete
*   Ĭ�ϵ�ȫ�ֲ�����
*     void * operator new(size_t size);
*     void operator delete(void *p);
*   �����еĲ���������(����Ĭ�ϻ�ʹ�ø����أ�������Ҳ����������)
*     public:
*       void * operator new(size_t size);
*       void operator delete(void *p);
*       //Ĭ������£������������ʱ�ᱻ����ȫ�ֵ�new[]��delete[]������Ҫ��Ҳ����������ء�
*       //ע�⣺���ڶ���C++��ʵ�֣�new[]�������еĸ�������������Ĵ�С���϶���Ĵ洢������Ŀ��һЩ�ֽڡ�
*       //      ���ڴ���������Ҫ���ǵ���һ�㡣Ӧ�þ����������������飬�Ӷ�ʹ�ڴ������Լ򵥡�
*       void * operator new[ ](size_t size);
*       void operator delete[](void *p);
* 
* 
* �ڴ�й¶��� -- CRT���ԶѺ���(ֻ�� _DEBUG �궨����������Ч)��������˳����������Ͱ��� ���ͷ�ļ�
*   #define _CRTDBG_MAP_ALLOC     //������ڴ����ط����ļ�·�����к�
*   #include <stdlib.h>
*   #include <crtdbg.h>
* ��غ���
*   _CrtDumpMemoryLeaks -- ���ĿǰΪֹ��δ�ͷŵ��ڴ���Ϣ
*   _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); -- ʹ�ó����˳�ʱ���Զ����� _CrtDumpMemoryLeaks
*      һ����Main��ʼ��#if defined(DEBUG) | defined(_DEBUG) ... #endif  �����Ժ����ûʲô�ã�ϵͳ������Զ����á�
*   _CrtSetReportMode -- �������ģʽ(��Ŀ�ĵ�)
*   _CrtSetBreakAlloc -- ��ָ�����ڴ������ֵ�����öϵ㣬Ҳ������Watch���������� {,,msvcrtd.dll}_crtBreakAlloc , �س���ָ�������
*   _CrtMemCheckpoint -- ���ڴ�״̬�����գ�ע�⣺�ж���̵߳�ʱ��
* CRT�����Ϣ
*   {18}            normal block at 0x00780E80, 64 bytes long.
*   {�ڴ������ֵ}  ģ�������       �ڴ��ַ       ��С         
*   ǰʮ���ֽڵ�����
* �ڴ�ģ������
*   Free Blocks     -- һ�ֱ��ͷŵ��ڴ�ģ��(���������)
*   Normal Blocks   -- ����ĳ���������ͨ�ڴ�(ע����ص�)
*   CRT Blocks      -- ��CRT���ṩ�Լ�ʹ�ö�������ڴ�ģ��,ͨ������Ҫע��(CRT����ʱ�Ż������)
*   Ignore Blocks   -- ������Ϊ���Ե��ڴ��
*   Client Blocks   -- MFC ʹ��
********************************************************************************************/

namespace FTL
{
    //Class
    
    //ģ���ڴ��
    //  ע�⣺Get�󷵻ص�ָ�뽫����һ�ε�ֵ����Ҫ���н��г�ʼ��
    //        Get��Release����listǰ�����У�����page�����Ĵ���
    template <typename T>  //, typename THREADLOCK = CFCriticalSection
    class CFMemoryPoolT
    {
    private:
        template <typename T>
        struct CFMemoryObjectT{
        public:
            LONG index;
            T    obj;
            FTLINLINE bool operator < (const CFMemoryObjectT<T> & other) const
            {
                COMPARE_MEM_LESS(index, other);
                return false;
            }
        };
        typedef CFMemoryObjectT<T>    CFMemoryObject;
    public:
        FTLINLINE CFMemoryPoolT( INT nInitSize = 0 , INT nMaxSize = INT_MAX);
        FTLINLINE ~CFMemoryPoolT();
        FTLINLINE T* Get();
        FTLINLINE void Retrun( T* t );

        //�������ͷţ���� nCount = -1�� ��ʾ�� m_freeBlocks �е�ȫ���ͷŵ�
        INT Free(INT nCount = -1);
    private:
        //typename THREADLOCK	m_lockObject;
        CFCriticalSection	m_lockObject;
        
        typedef std::set< CFMemoryObject*, UnreferenceLess<CFMemoryObject*> > ObjectContainer;
        ObjectContainer     m_allBlocks;
        ObjectContainer     m_freeBlocks;
        INT                 m_nMaxSize;
        INT                 m_nCurCount;
        LONG                m_nNextIndex;
        FTLINLINE LONG      _GetNextIndex(){ return InterlockedIncrement(&m_nNextIndex); }
    };

    //���ڴ���л�ȡ����ĸ����࣬�����л�ȡ���ͷ��зŻ��ڴ��
    template <typename T>
    class CFMemoryPoolObjectHelper
    {
    private:
        DISABLE_COPY_AND_ASSIGNMENT(CFMemoryPoolObjectHelper);
        typedef CFMemoryPoolT<T>        MemoryPoolType;
        MemoryPoolType* m_pMemPool;
    public:
        T*  pObject;
        CFMemoryPoolObjectHelper(MemoryPoolType* pMemPool);
        ~CFMemoryPoolObjectHelper();
    };

    //TODO: �ο� .NET �� System.Web.Caching.Cache ����ص�
    //  ����������Ѿ����ڣ����߻�������������ĳ���ļ���������˸ı䣬���ڴ����ʱ���Զ��Ƴ������� -- ʹ��ǰ��Ҫ��黺�����Ƿ����
    //  ������֧�ֻ���������(CacheDependency) -- ���ӵ����ļ���DB�еı�������κ����͵���Դ���緢���仯���Զ���Ϊ��Ч�����Ƴ�
    FTLEXPORT template<typename KEY, typename VALUE, typename CHILDCLASS>
    class CFItemCacheT
    {
    public:
        CFItemCacheT(DWORD nMaxCacheSize,DWORD clearPercent = 30);
        virtual ~CFItemCacheT();
    public:
        enum ClearMode
        {
            //clearByOptimal,     //����û��㷨(��Ҫ�����ơ���ʹ�û�ܳ�ʱ���ڲ�ʹ�õĵ������ʵ��)
            //clearByLFU,         //Least Frequently Used(�����ʹ��), TODO -- ʵ��
            clearByLRU,         //Least recently used(�������ʹ���㷨��ͨ��ֻ�û�һ��)
            clearByMRU,         //Most recently used(����ʹ���㷨�����ܷǳ���) 
            //clearByNUR,         //Not Used Recently(���δ���û��㷨,��LRU�Ľ����㷨���û����д��ϴμ����δʹ�ù���)
            clearByFIFO,        //First In First Out(�����ȳ��㷨����ֱ�ۣ����������)
            clearByRandom,      //��������������(�����ǰ��� map �� KEY �Ĵ�С˳������)
        };
        BOOL GetItemByKey(const KEY key,VALUE& value);
        //BOOL GetItmeByKeyAsync(const KEY key); //TODO:�첽��ȡ������������Ϸ��أ�����ͨ��ָ�����첽�������ú�ͨ���ص�����
        void SetClearMode(ClearMode mode);
        BOOL SetClearPercent(DWORD clearPercent);    //ʹ�ðٷֱȽ������
    protected:
        //virtual BOOL GetRealItem(KEY key,VALUE& value) = 0;
        //virtual void FreeItem(KEY key,VALUE& value) = 0;
    protected:
        struct CItemData
        {
            KEY key;
            VALUE value;
            LARGE_INTEGER lastAccess;   //��һ�η��ʵ�ʱ��
            DWORD index;
        };
        typedef std::map<KEY,CItemData*>  CACHE_MAP;
        CACHE_MAP m_ItemCacheMaps;
        static bool LastAccessGreater(CItemData* p, CItemData* q);
        static bool LastAccessSmaller(CItemData* p, CItemData* q);
        static bool IndexGreater(CItemData* p, CItemData* q);
        static bool IndexSmaller(CItemData* p, CItemData* q);
        void ClearCache(DWORD percent);     
    private:
        void Init(DWORD nMaxCacheSize,DWORD clearPercent);

        CFCriticalSection   m_LockObject;
        ClearMode           m_ClearMode;
        DWORD               m_MaxCacheSize;
        DWORD               m_ClearPercent;
        DWORD               m_CurrentIndex;
    };

    //ע�⣺����ֻ���ڵ��̵߳�����½��м�飬�����̷߳�����ڴ�Ҳ�ᱻ��⵽�����Ҫ��⣬������������߳�����ͣ

#ifdef _DEBUG
# define CHECK_MEM_LEAK(bPauseOtherThread)\
    FTL::CFMemLeakChecker   memLeakChecker ## __LINE__ (TEXT(__FILE__),__LINE__, TEXT(__FUNCTION__), bPauseOtherThread)
#  define CHECK_NAME_MEM_LEAK(blockName, bPauseOtherThread)\
    FTL::CFMemLeakChecker   memLeakChecker ## __LINE__ (TEXT(__FILE__),__LINE__, blockName, bPauseOtherThread)
#else
#  define CHECK_MEM_LEAK(bPauseOtherThread)                     __noop
#  define CHECK_NAME_MEM_LEAK(blockName, bPauseOtherThread)     __noop
#endif 
    
    class CFMemLeakChecker
    {
    public:
        FTLINLINE CFMemLeakChecker(LPCTSTR pszFileName,DWORD line, LPCTSTR pBlockName, BOOL bPauseOtherThread);
        FTLINLINE ~CFMemLeakChecker();
    private:
        const TCHAR*    m_pszFileName;
        const TCHAR*    m_pszBlkName;
        const DWORD     m_Line;
        const BOOL      m_bPauseOtherThread;
        _CrtMemState    m_sOld;
        _CrtMemState    m_sNew;
        _CrtMemState    m_sDiff;
    };

    //���ڼ��������˳�ʱ��ָ�������Ƿ����ڴ�й©��С������ -- ��������ΪӦ���ͷ����ڴ�ĵط�(���� main ���ص�λ��)���е��ü���
    FTLEXPORT class CFMemCheckBase
    {
    public:
        FTLINLINE CFMemCheckBase();
		FTLINLINE CFMemCheckBase(const CFMemCheckBase &other);
		FTLINLINE CFMemCheckBase& operator =( const CFMemCheckBase &ref );
		FTLINLINE virtual ~CFMemCheckBase();
    };

	class CFMemCheckManager
	{
	public:
		FTLINLINE static CFMemCheckManager& GetInstance();
		FTLINLINE static VOID ReleaseInstance();
	public:
		FTLINLINE BOOL AddObject(DWORD_PTR pObject, LPCTSTR pszPosition, LPCTSTR pszName = NULL);
		FTLINLINE BOOL RemoveObject(DWORD_PTR pObject);

		FTLINLINE VOID SetTrace(BOOL bTrace);
		FTLINLINE BOOL GetTrace();
		FTLINLINE VOID DumpLeakInfo();
	private:
		FTLINLINE CFMemCheckManager();
		FTLINLINE ~CFMemCheckManager();
		struct ObjectInfo
		{
			//UINT				m_nCount;		//o or 1
			//DWORD_PTR			pObjectAddr;
			CFStringFormater	m_strInfo;
		};
		CFCriticalSection		m_LockObj;
		typedef std::map<DWORD_PTR, ObjectInfo*>	ObjectPtrInfoContainer;
		ObjectPtrInfoContainer	m_allObjects;
		BOOL m_bTrace;

		static CFMemCheckManager*	s_pMemCheckMgr;
	};

	//#define CHECK_OBJ_MEM_LEAK()   \
	//if( 0 != CFDoubleFreeCheckMgr::Instance(). ) \
	//	{\
	//		FTLTRACEEX(FTL::tlWarning ,TEXT("%s(%d): Warning!!! Memory May Leak -- '%s' remain count is %d\n"), \
	//		TEXT(__FILE__), __LINE__, TEXT(#x), x::s_Count);\
	//	}

    //Function
}//namespace FTL

#ifndef USE_EXPORT
#  include "ftlMem.hpp"
#endif

#endif //FTL_MEM_H
