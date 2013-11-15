///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   ftlthread.h
/// @brief  Fishjam Template Library Thread Header File.
/// @author fujie
/// @version 0.6 
/// @date 03/30/2008
/// @defgroup ftlthread ftl thread function and class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FTL_THREAD_H
#define FTL_THREAD_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlthread.h requires ftlbase.h to be included first
#endif

//#ifdef _DEBUG  //__AFX_H__ , _MFC_VER
//#  define new DEBUG_NEW
//#endif

#if defined(_MT)
#  include <process.h>    //for _beginthreadex
#endif

//#include <ftlFunctional.h>
//#include <ftlSharePtr.h>

#include <queue>
#include <vector>
#include <set>
#include <map>
#include <list>
#include <algorithm>

/******************************************************************************************************
* Intel Hyper-Threading Technology -- Intel�ĳ��̼߳���
* 
* APC(Asynchronous Procedure Call) -- �첽���̵��á�APCֻ�е������� "Aalertable̬��ʱ���Żᱻ���á�
*   Ŀǰ�����������SleepEx��WaitForXXXEx(,,TRUE)��SignalObjectAndWait�ȣ������᷵�� WAIT_IO_COMPLETION��
*   ��ʱOVERLAPPED�е�hEvent���ط���EventHandle����������ʹ�á�
*
* �ٽ���(Critial Section)--���ٽ���Դʵʩ�����ĳ���Ρ��Ի����ٽ��������ԭ���ǣ�
*   �п�������޿���ȣ����޵ȴ�����Ȩ�ȴ�(�����ܽ����ٽ���ʱ��Ӧ�����ͷ�CPU����������æ״̬)
*   ����ϸ�������ƣ�����д����(RCU)����дƽ����(RW)������д����(SEQ)��
*     ��ʱ������ɿ�����spin_lock������æ�ȡ�
*     ע�⣺1.spin_lock��ʹ��Ҫ��ܸߣ���������������κ�������ȵĺ�����
*           2.���� pthread_spin_lock - �÷���ֻ��ģ��
* 
*   ����������Ľ��˼·ֻ��һ�� -- ���ٻ�����
*   һ���Ѽ����Ļ��Ʒ�Ϊ���֣�
*     ҵ���߼���
*     ������
* 
* �ź�����S>=0��ʾĳ��Դ�Ŀ�������S<0ʱ�����ֵ��ʾ���������еȴ�����Դ�Ľ���(�߳�)����
*   ԭ�Ӳ���PV:P������ʾ����һ����Դ��S=S-1; if(S<0){Wait(S);������������}
*              V������ʾ�ͷ�һ����Դ��S=S+1; if(S<=0){Resume(S);�����������ܻ���һ��}
*
* ����������4����Ҫ������
*   1.�����������̶߳�����Ҫ�����Դ���������Կ��ƣ���һ��ֻ����һ���߳�ʹ�ã�
*   2.���󱣳����������ǵ�����֧Ԯ�����ѻ�ò�����Դ��������Դ��������
*   3.���ɰ����������߳��ѻ�õ���Դ��δʹ����֮ǰ�����ܱ����ᣬֻ����ʹ����ʱ���Լ��ͷţ�
*   4.��·����������������ʱ�����߳���Դ����ͼ�бع��ɻ�·��ÿ���߳�ռ������һ���߳������һ��������Դ��
*
* ���Ķ��������(�Ƿ���ȷ��)��
*   \\Global\
*   \\Local\
*
* ���Ķ���İ�ȫ����(LPSECURITY_ATTRIBUTES)
*   NULL -- Ĭ�����ԣ������Admin���������û��޷���
*   
* ��Ҫ���߳�֮�乲��GDI���� -- Ϊʲô��ͨ��Ӧ��ֻ�����̲߳Ų���GDI����
* MFC�������Win32 handle֮���ӳ���ϵ��¼���ֲ߳̾��洢�У����û�а취���̼߳䴫��MFC��������ָ�룬
*   ��������Ǵ��ݶ����Handle(ͨ��GetSafeXXX��ȡ)��Ŀ���߳̿�������FromHandle����һ����ʱ����
*   ʹ��Attach���ŵ�һ�����ö���(�˳�֮ǰӦDetach)��

* TerminateThread ��Σ��
*   1.�������߳�û�л����ڽ���ǰ����������ջ���ᱻ�ͷ�(�ڴ�й¶)��
*   2.���DLL������ Thread Detach ��֪ͨ��
*   3.����������߳�����CS�У����CS����Զ��������״̬��Mutex���ᱻAbandoned
* 
* �����һ��worker�߳��е���GetMessage�����߳̾ͻ������Ϣ���У���Ȼ����û�д��ڣ�
*   ��ʱ�Ϳ��Ե���PostThreadMessage����worker�̷߳�����Ϣ��
*
* ��GUI�߳�(non-GUI)--�̴߳���ʱ���Ƿ�GUI�߳�
* GUI�߳� -- ӵ�� ����Ķ�ջ��Win32k.sys���ڴ����ͽ���ʱ�յ�֪ͨ�����̱�ת����GUI����
* 
* TLS -- �ֲ߳̾��洢
*   C runtime library��errno��strtokָ�����TLS��
*   MFCͨ��TLS��׷��ÿһ���߳���ʹ�õ�GDI�����USER����
*   _declspec(thread) DWORD ������ -- �ñ�����ÿһ���߳��Ƕ�һ�޶��ģ��ڿ�ִ���ļ�������һ������Ľ������ں����е��ֲ߳̾�������
*   ��һ��DLL���ʹ����_declspec(thread)����û�а취��LoadLibrary����(KB118816)
*
* �̵߳��ԣ�
*   1.���̻߳����£����û������ͬ������һ���߳������������󣬵�����һ���̵߳����˸ö�������غ��������ܲ������麯�����õĴ���
*   2.�߳�����ʱ�� ProcessExplorer �ܺ����׵ض�λ������λ�ú�ԭ��
*   3.ʹ��Window Object Explorer(WinObjEx)�鿴���󣬿��˽�����ʹ��״̬
*   
* ���ܵ��ţ�
*   1.�޹�ϵ�����ݲ�ҪΪ�˷���ʹ��ͬһ����
*   2.������˵�߳�Խ���л���Խ�� -- Ӧ�ý���ϵͳ�л�Ƶ�ʣ����̡߳�û�������Լ���ʱ��Ƭ���ͷ����л�ʱ���ĺܴ���ȴ�IO������ȴ�����ʱ����ȡ�
*   3.����ĸ���������(����ˮ�ߣ�)�Ļ�������ԭ���ǣ����뷴������һ����������������ϵͳ�Ķ���
*   4.��������ɢ�У��ɸ�����ڵ�����ֱ��ɢ�е���ͬ�����ݽṹ�ϣ�ʹͬʱ����������ݵĻ����С(glibc���ڴ��Ϊ�������ÿ�����õ�������)
*     linux ���и� PER_CPU ���ƣ�����Ϊÿ��CPU����һ�ݵ���������
*   5.�����㷨 -- ������ָ�븳ֵ��ԭ���ԣ��䱾��������ɢ��
*   6.
*
*
* Э��/�˳�Fiber? -- �������̣߳�
*   1.�ܹ��ڵ�һ��ϵͳ�߳���ģ��������Ĳ���ִ��;
*   2.��һ���ض���ʱ�䣬ֻ��һ�����������У������������ز���
*   3.������������ȷ�ʽ(����û��������ռʱ��Ƭ��˵��)����һ����������ִ��ʱ���ⲿû�а취��ֹ����
*     Ҫ���������л���ֻ��ͨ���ɸ������������yield()����������CPUʹ��Ȩ��������IO����ִ������(ͨ��poll����)
*   4.ÿ��Э�����Լ��Ķ�ջ�;ֲ�����
*   libtask -- һ��������Э�̿�(http://swtch.com/libtask/), linuxϵͳ�ϵ�(��ʹ�� makecontext/swapcontext ����)
*     ����������� -- ʹ������Ҫʵ��������taskmain
*     ��������� -- taskcreate, 
*     �첽IO
*     channel -- chancreate
******************************************************************************************************/

namespace FTL
{
    //�Զ�ͬ�����Ļ���
    class CFLockObject
    {
    public:
        virtual BOOL Lock(DWORD dwTimeout = INFINITE) = 0;
        virtual BOOL UnLock() = 0;
    };

    FTLEXPORT class CFNullLockObject : public CFLockObject
    {
    public:
        FTLINLINE virtual BOOL Lock(DWORD dwTimeout = INFINITE);
        FTLINLINE virtual BOOL UnLock();
    };

#ifdef FTL_DEBUG
#  define CRITICAL_SECTION_LOCK_TRACE(pCS, bTrace)  { pCS->SetTrace(bTrace);}
#else
#  define CRITICAL_SECTION_LOCK_TRACE(pCS, bTrace)
#endif

    //! class CFCriticalSection
    //! �����߳�ͬ����ͬ������(�ٽ���)
    FTLEXPORT class CFCriticalSection : public CFLockObject
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFCriticalSection);
    public:
        FTLINLINE CFCriticalSection();
        FTLINLINE ~CFCriticalSection();

        //һ��һ���߳̽���һ��CS�������ܹ�һ�ٵ��ظ������CS,����Ҫ��֤ ��Ӧ��UnLock
        FTLINLINE BOOL Lock(DWORD dwTimeout = INFINITE);
        FTLINLINE BOOL UnLock();
        FTLINLINE BOOL TryLock();
#ifdef FTL_DEBUG
        FTLINLINE BOOL IsLocked() const;
        FTLINLINE BOOL SetTrace(BOOL bTrace);
#endif
    private:
        CRITICAL_SECTION m_CritSec;
#ifdef FTL_DEBUG
        DWORD   m_currentOwner;     //ָ�����ĸ��߳������˸��ٽ���
        BOOL    m_fTrace;           //ָ���Ƿ�Ҫ��ӡ��־��Ϣ
        DWORD   m_lockCount;        //���ڸ����߳̽���ؼ�����εĴ���,�������ڵ���
#endif //FTL_DEBUG
    };

    //Mutex -- ��סһ��δ��ӵ�е�Mutex������סһ��δ��ӵ�е�CS����Ҫ���Ѽ���100����ʱ��(ʵ��Xpʱ��30������)
    //   CS����Ҫ����ϵͳ�ںˣ�ֱ����ring3���Ϳ��Խ��в���
    
    FTLEXPORT class CFMutex : public CFLockObject
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFMutex);
    public:
        FTLINLINE CFMutex(BOOL bInitialOwner = FALSE, LPCTSTR lpName = NULL,
            LPSECURITY_ATTRIBUTES lpMutexAttributes = NULL);
        FTLINLINE ~CFMutex();
        FTLINLINE BOOL UnLock();

        //ӵ��mutex���̲߳����ٵ��ö��ٴ�wait������Ҳ���ᱻ����ס
        FTLINLINE BOOL Lock(DWORD dwTimeout = INFINITE);
        FTLINLINE operator const HANDLE() const { return m_hMutex;}
    protected:
        HANDLE  m_hMutex;
#ifdef FTL_DEBUG
        DWORD   m_currentOwner;     //ָ�����ĸ��߳������˸û�����
        DWORD   m_lockCount;        //���ڸ����߳̽���ؼ�����εĴ���,�������ڵ���
#endif //FTL_DEBUG
    };
    
    //! �¼�����
    FTLEXPORT class CFEvent : public CFLockObject
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFEvent);
    public:
        FTLINLINE CFEvent(BOOL bInitialState = TRUE, BOOL bManualReset = TRUE,
            LPCTSTR pszName = NULL, LPSECURITY_ATTRIBUTES lpsaAttribute = NULL);
        FTLINLINE ~CFEvent();
        FTLINLINE operator const HANDLE () const { return m_hEvent; }

        FTLINLINE BOOL Wait(DWORD dwTimeout = INFINITE);
        //�����ڵȴ�ʱ���� SEND ���͵���Ϣ(��DirectShow�п����Ĵ���) --
        //http://support.microsoft.com/kb/136885/en-us
        FTLINLINE BOOL WaitMsg(DWORD dwTimeout = INFINITE);

        FTLINLINE BOOL Reset();
        FTLINLINE BOOL UnLock();
        FTLINLINE BOOL Lock(DWORD dwTimeout = INFINITE);
    private:
        HANDLE m_hEvent;
    };

    //! �Զ�����ͬ������, MFC �� CSingleLock(&m_lockObj, TRUE) ��ʾ���캯�����Զ����������Զ�����
    FTLEXPORT template<typename T = CFLockObject>
    class CFAutoLock
    {
    public:
        explicit FTLINLINE CFAutoLock<T>(T* pLockObj);
        FTLINLINE ~CFAutoLock();
    private:
        T*   m_pLockObj;
    };

    //���ȼ����װ�࣬���������ص�ʱ����ƽ���ٶ�
    class CFProgressCalculator
    {

    };

    //һ�������̰߳�ȫ�����װ���̰߳�ȫ���� -- ����ʹ�÷����� test_threadSafeWrapper
    //  ������ operator-> �����ԣ�������صĲ���ָ�����ͣ���������÷���ֵ��operator->()������ֱ�����ս�����һ��ָ������
    //  ��һ�η��ط�ָ�����͵� proxy�����ǻ�����һ����ʱ�� proxy ���󣬶� m_ptr ������Ȼ��ͨ�� proxy::-> ���е��ã�
    //  ����ͷ���ʱ�� proxy ������������http://cunsh.ycool.com/post.1785495.html
    template <typename T, typename Lock>
    class CFThreadSafeWrapper
    {
    public:
        //typedef CFThreadSafeWrapper self_type;
        //typedef Pointer pointer_type;
        //typedef Lock lock_type;
    private:
        struct proxy{
            //Ƕ����proxy�Ĺ��캯������ʽת��(��û�м�explicit)�Ǳ�Ҫ�ģ����������������copy ctor������
            proxy(CFThreadSafeWrapper* host)   
                : m_host(host)
                , m_locker(&host->m_lockObj)
            {
                //FTLTRACE(_T("proxy construct\n"));
            };
            ~proxy()
            {
                //FTLTRACE(_T("proxy destructor\n"));
            }
            T* operator->(){
                return m_host->m_ptr;
            }
        private:
            CFThreadSafeWrapper* m_host;  //self_type* m_host;
            CFAutoLock<Lock> m_locker;
        private:
            //��ֹ��������͸�ֵ������
            proxy(const proxy&);
            proxy& operator=(const proxy&);
        };
        friend struct proxy;
    public:
        CFThreadSafeWrapper(T* ptr, BOOL bManage)
            :m_ptr(ptr)
            ,m_bManage(bManage)
        {
        }
        ~CFThreadSafeWrapper()
        {
            if(m_bManage)
            {
                SAFE_DELETE(m_ptr);
            }
        }
        proxy operator->() {
            return this;
        }
        void reset(T* ptr ,BOOL bManage)
        {
            if (m_bManage)
            {
                SAFE_DELETE(m_ptr);
            }
            m_ptr = ptr;
            m_bManage = bManage;
        }
        T* release()
        {
            T* pRet = m_ptr;
            m_ptr = NULL;
            m_bManage = FALSE;
            return pRet;
        }

        //ԭ��������ʱ��ȡ�ڲ������ṩһ����Χ�����Ļ������������ܣ���ʵ����������
#if 0
        friend T* getImpObject(const CFThreadSafeWrapper<T, Lock> & self);
        friend Lock& getLockObject(const CFThreadSafeWrapper<T, Lock> & self);
#endif
    private:
        T*          m_ptr;
        BOOL        m_bManage;
        Lock        m_lockObj;
    };

#if 0
    template <typename T, typename Lock>
    T* getImpObject(const CFThreadSafeWrapper<T, Lock> & self){ return self.m_ptr;}

    template <typename T, typename Lock>
    Lock& getLockObject(const CFThreadSafeWrapper<T, Lock> & self){ return self.m_lockObj; }
#endif

    //�̴߳������� -- TODO: MFC ?
    class CFCRTThreadTraits
    {
    public:
        static HANDLE CreateThread(LPSECURITY_ATTRIBUTES lpsa, DWORD dwStackSize, LPTHREAD_START_ROUTINE pfnThreadProc, void *pvParam, DWORD dwCreationFlags, DWORD *pdwThreadId) throw()
        {
            FTLASSERT(sizeof(DWORD) == sizeof(unsigned int)); // sanity check for pdwThreadId

            // _beginthreadex calls CreateThread which will set the last error value before it returns.
            return (HANDLE) _beginthreadex(lpsa, dwStackSize, (unsigned int (__stdcall *)(void *)) pfnThreadProc, pvParam, dwCreationFlags, (unsigned int *) pdwThreadId);
        }
    };

    class CFWin32ThreadTraits
    {
    public:
        static HANDLE CreateThread(LPSECURITY_ATTRIBUTES lpsa, DWORD dwStackSize, LPTHREAD_START_ROUTINE pfnThreadProc, void *pvParam, DWORD dwCreationFlags, DWORD *pdwThreadId) throw()
        {
            //CreateThreadʱC����ʱ��û�г�ʼ��
            return ::CreateThread(lpsa, dwStackSize, pfnThreadProc, pvParam, dwCreationFlags, pdwThreadId);
        }
    };
    //TODO:FTL����ô�ж�Ĭ�ϴ�����Ӧ����˭��
#if defined(_MT)  //!defined(_ATL_MIN_CRT) && 
    typedef CFCRTThreadTraits DefaultThreadTraits;
#else
    typedef CFWin32ThreadTraits DefaultThreadTraits;
#endif

    #define FTL_MAX_THREAD_DEADLINE_CHECK   5000

    //! ģ���߳���
    typedef enum tagFTLThreadWaitType
    {
        ftwtStop, 
        ftwtContinue,
        ftwtTimeOut,
        ftwtError,

        ftwtUserHandle,
    }FTLThreadWaitType;
    
    /************************************************************************
    * ����д�����/����Ķ�����ı���--�漰����ͼ���ʹ�����Դ�������������߳�
    * ��Щ�̣߳�д�������Ҫ�޸����ݵ����ݣ�����Щ�̣߳��Ķ���������Ҫ��ȡ����
    * 4��ԭ��:
    *   1) ��һ���߳�����д������ʱ�������κ��̲߳���д�����ݡ�
    *   2) ��һ���߳�����д������ʱ�������κ��̲߳��ܶ�ȡ���ݡ�
    *   3) ��һ���߳����ڶ�ȡ����ʱ�������κ��̲߳���д�����ݡ�
    *   4) ��һ���߳����ڶ�ȡ����ʱ�������߳�Ҳ�ܹ���ȡ���ݡ�            
    ************************************************************************/
    FTLEXPORT
    class CFRWLocker
    {
    public:
        FTLINLINE CFRWLocker();        // Constructor
        FTLINLINE virtual ~CFRWLocker();                // Destructor

        FTLINLINE BOOL Start();
        FTLINLINE BOOL Stop();

        FTLINLINE FTLThreadWaitType WaitToRead(DWORD dwTimeOut);        // Call this to gain shared read access
        FTLINLINE FTLThreadWaitType WaitToWrite(DWORD dwTimeOut);       // Call this to gain exclusive write access
        FTLINLINE void Done();              // Call this when done accessing the resource
    private:
        //���ڱ������е�������Ա�����������������ǵĲ������ܹ���ԭ�Ӳ�����ʽ�����
        CFLockObject*   m_pLockObject;

        //������̵߳���WaitToRead����������m_nActive��- 1�����ܾ�����ʱ�������Ķ��߳̾��ȴ����űꡣ
        //�����һ�����ڵȴ����Ķ��̵߳���Doneʱ�����ű걻�ͷţ���������m_nWaitingReaders���Ӷ������������ڵȴ����Ķ��߳�
        HANDLE m_hSemReaders;     // Readers wait on this if a writer has access

        //���̵߳���WaitToWrite����������m_nActive����0�����ܾ�����ʱ������д���߳̾��ȴ����űꡣ
        //��һ���߳����ڵȴ�ʱ�����Ķ��߳̽����ܾ����ʸ���Դ������Է�ֹ�Ķ��߳�¢�ϸ���Դ��
        //�����һ��ӵ����Դ����Ȩ���Ķ��̵߳���Doneʱ�����ű�ͱ��ͷţ���������1���Ӷ�����һ�����ڵȴ���д���߳�
        HANDLE m_hSemWriters;

        HANDLE m_hStopEvent;        //Event to stop

        int    m_nWaitingReaders; // ��ʾ��Ҫ������Դ���Ķ��̵߳�������
        int    m_nWaitingWriters; // ��ʾ��Ҫ������Դ��д���̵߳�������

        //���ڷ�ӳ������Դ�ĵ�ǰ״̬(0=no threads, >0=# of readers, -1=1 writer)
        //1.�����ֵ��0����ôû���߳��ڷ�����Դ��
        //2.�����ֵ����0�����ֵ���ڱ�ʾ��ǰ��ȡ����Դ���̵߳�������
        //3.�����������Ǹ�ֵ����ôд��������ڽ�����д�����Դ��Ψһ��Ч�ĸ�ֵ��- 1
        int    m_nActive;         
    };


    FTLEXPORT class CFEventChecker
    {
		DISABLE_COPY_AND_ASSIGNMENT(CFEventChecker);
    public:
        FTLINLINE CFEventChecker(HANDLE hEventStop,HANDLE hEventContinue);
        FTLINLINE ~CFEventChecker();
        FTLINLINE FTLThreadWaitType GetWaitType(DWORD dwTimeOut = INFINITE);
        FTLINLINE FTLThreadWaitType GetWaitTypeEx(HANDLE* pUserHandles, DWORD nUserHandlCount, 
            DWORD* pResultHandleIndex, BOOL  bCheckContinue = FALSE, DWORD dwTimeOut = INFINITE);
        FTLINLINE FTLThreadWaitType SleepAndCheckStop(DWORD dwTimeOut);
    private:
        HANDLE		    m_hEventStop;
        HANDLE          m_hEventContinue;
    };

    FTLEXPORT template <typename T>
    class CFSyncEventUtility
    {
    public:
        FTLINLINE CFSyncEventUtility(void);
        FTLINLINE ~CFSyncEventUtility(void);

        FTLINLINE void ClearAllEvent();
        FTLINLINE void AddEvent(T t, HANDLE hEvent = NULL);
        FTLINLINE void SetEvent(T t);
        FTLINLINE void ResetEvent(T t);
        FTLINLINE void SetAllEvent();
        FTLINLINE void ResetAllEvent();
        FTLINLINE BOOL WaitAllEvent(DWORD dwMilliseconds = INFINITE);
        FTLINLINE size_t GetSyncEventCount();
        T WaitOneEvent(DWORD dwMilliseconds /* = INFINITE */);
    private:
        mutable CFCriticalSection   m_LockObject;
        struct SyncEventInfo
        {
            BOOL            bCreateEvent;
            HANDLE          hEvent;
        };
        typedef std::map<T, SyncEventInfo> SYNC_EVENT_MAP;
        SYNC_EVENT_MAP  m_AllSyncEventMap;
    };

    /*********************************************************************************************************
    * TIB -- Thread Information Block
    *********************************************************************************************************/
    FTLEXPORT
    class CFThreadUtils
    {
    public:
        // if dwThreadId = -1, then current thread
        //VC6��,��ʾ�߳����ֲ����� Watch ���������� (char*)(dw(@TIB+0x14)),s
        FTLINLINE static BOOL SetThreadName( DWORD dwThreadID, LPTSTR szThreadName);
        FTLINLINE static LPCSTR GetThreadName(DWORD dwThreadID);
    };

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// һ��֧�ְ�ȫ�� ��ͣ��������ֹͣ ���ܵ��߳̿��
    FTLEXPORT template <typename ThreadTraits = DefaultThreadTraits>
    class CFThread
    {
		DISABLE_COPY_AND_ASSIGNMENT(CFThread);
    public:
        /// ���캯���������߳���ʵ��
        /// @param [in] hEventStop �ⲿ��������߳�ֹͣʱ�������¼������ΪNULL���߳��ڲ����Լ�����һ����
        /// @param [in] hEventContinue �ⲿ����ı�֤�߳��������е��¼�(������¼�δ�����������̻߳�ȴ�--��ͣ)��
        ///                            ���ΪNULL���߳��ڲ����Լ�����һ��
        /// @note  hEventStop �� hEventContinue ���������ֶ������¼�
        FTLINLINE CFThread(HANDLE hEventStop = NULL,HANDLE hEventContinue = NULL);

        FTLINLINE virtual ~CFThread();
    public:
        FTLINLINE FTLThreadWaitType GetThreadWaitType(DWORD dwTimeOut = INFINITE);

        //���Եȴ��û��ṩ���¼�(����������Stopһ��Ľ����¼� -- ��ȴ����̽�����)
        FTLINLINE FTLThreadWaitType GetThreadWaitTypeEx(HANDLE* pUserHandles, 
            DWORD nUserHandlCount, 
            DWORD* pResultHandleIndex, 
            BOOL  bCheckContinue = FALSE,
            DWORD dwTimeOut  = INFINITE);
        FTLINLINE virtual FTLThreadWaitType SleepAndCheckStop(DWORD dwTimeOut);
        FTLINLINE BOOL Start(LPTHREAD_START_ROUTINE pfnThreadProc, void *pvParam, BOOL resetEvent = TRUE);
        FTLINLINE BOOL Wait(DWORD dwTimeOut = INFINITE, BOOL bCloseHandle = TRUE, BOOL bTerminateIfTimeOut = TRUE);
        FTLINLINE BOOL Stop();
        FTLINLINE BOOL StopAndWait(DWORD dwTimeOut = FTL_MAX_THREAD_DEADLINE_CHECK, BOOL bCloseHandle = TRUE,
			BOOL bTerminateIfTimeOut = TRUE );
        FTLINLINE BOOL Pause();
        FTLINLINE BOOL Resume();
        FTLINLINE BOOL LowerPriority();
        FTLINLINE BOOL RaisePriority();
        FTLINLINE int  GetPriority() const;
        FTLINLINE BOOL SetPriority(int priority);
        FTLINLINE BOOL IsThreadRunning() const;
        FTLINLINE BOOL HadRequestStop() const;
        FTLINLINE BOOL HadRequestPause() const;
    protected:
        //unsigned int	m_Id;			//!< Thread ID
        HANDLE		    m_hEventStop;
        HANDLE          m_hEventContinue;
        HANDLE		    m_hThread;		//!< Thread Handle
        CFEventChecker* m_pEventChecker;
    private:
        BOOL            m_bCreateEventStop;
        BOOL            m_bCreateEventContinue;
    };

    //ǰ������
    template<typename ELEMENT> class CFAutoQueueOperator;
    /*****************************************************************************************************************************
    * @brief
    *  ʹ�� �ű��ں˶���(Semaphore)���Ƶ� ������/������ ���̶߳���
    *     *********************************************************
    *     |                      Capability                       |	//���������� -- ���캯���д��룬���ɸ���)
    *     |    Queue Max Size         |   nReserveSlot            | //Queue����ܷ�����ٸ�Element����ͨ�� ReserveSlot/ReleaseSlot ����
    *     |  m_hSemNumElements  |      m_hSemNumSlots             | //��ǰQueue���ж��ٸ�Element��ͨ�� Append/Remove ����/ɾ��
    *     *********************************************************
    *     | <= Remove() , Append() => |                           | //Ĭ��ʱ��Append�ڶ���β��׷���µ�Element��Remove��ͷȡ
    *     |    ReleaseSlot() =>       |     <= ReserveSlot()      |
    *     *********************************************************
    * 
    * @note
    *   1.��ǰ�Element�ĸ���Ϊ���������߳�(���ڴ���) + Queue�еĸ��� + �������߳�(���ڴ���)��
    *     ��ˣ��������ܻ�ȹ����д���Ķ࣬���Ҫ��ȫ��ͬ����Ҫ�� CFAutoQueueOperator ��������������--���ĺ�δ��ϸ���ԣ�������Bug
    *   2.ReserveSlot/ReleaseSlot ������δ��ϸ����
    *****************************************************************************************************************************/
    FTLEXPORT template<typename ELEMENT>
    class CFProducerResumerQueue
    {
        friend class CFAutoQueueOperator<ELEMENT>;
    public:
        //����nReserveSlot��ʵ�ֶ�̬�ı���д�С������δ��ϸ���ǺͲ��ԣ����ܳ���
        //���nReserveSlotΪ0��������ǰһ������Ҫ���� RelaseSlot �� ReserveSlot��
        FTLINLINE CFProducerResumerQueue(LONG nCapability, LONG nReserveSlot = 0, HANDLE hEventStop = NULL);
        FTLINLINE virtual ~CFProducerResumerQueue();
        //! �������׷������ -- �չ����ʱ������������ݵ�
        FTLINLINE BOOL Start();
        //! ֹͣ���У��ỽ�����еȴ����̣߳�֮�󽫲����� Append & Remove
        FTLINLINE BOOL Stop();
        
        //! �ж�Queue�Ƿ��Ѿ�Stop
        FTLINLINE BOOL IsStopped();

        //! �����ߵ��ã��ڶ���������Ԫ�أ�
        //! @return 
        //!   ftwtStop ������Element��ʱ��ֹͣ
        //!   ftwtContinue �ɹ�����
        //!   ftwtTimeOut ��ʱ
        //!   ftwtError   ���ִ�������GetLastError(ʲôʱ���������������)
        FTLINLINE FTLThreadWaitType Append(const ELEMENT& element, DWORD dwTimeOut);
        //! �����߶˵���,�Ӷ�����ȡ��Ԫ��
        //! @return -- ͬ Append
        FTLINLINE FTLThreadWaitType Remove(ELEMENT& element, DWORD dwTimeOut);

        //! ��ֹͣ����ø÷���ȡ��Ԫ�أ����ڱ�֤�ͷ�Queue�б�����Element
        //! ��ʱ��ʩ���Ƿ��и��õķ��� ��
        //! ��Queue��û��ʣ�µ�Element�󣬷���False
        FTLINLINE BOOL RemoveAfterStop(ELEMENT& element);

        //! ����Queue�Ŀ��ô�С(��ȡһЩSlot���ڱ���)������ָ����ʱֵ(��Ҫ�ȴ�ռ��Slot��element��ȡ��) -- �����첽֪ͨ���ƣ�
        FTLINLINE FTLThreadWaitType ReserveSlot(LONG nCount,DWORD dwMilliseconds);
        //! ����Queue�Ŀ��ô�С(�ͷ�һЩ������Slot)
        FTLINLINE BOOL ReleaseSlot(LONG nCount);

        //! ��ȡQueue���������ӵ���� -- �ɹ��캯������
        FTLINLINE LONG GetCapability() const;
        //! ���ص�ǰQueue�е�Element����
        FTLINLINE LONG GetElementCount() const;
        //! ���ر���������Slot��
        FTLINLINE LONG GetReserveSlotCount() const;

    protected:
        //! �����ʵ�ִ����ȼ��Ķ��У��������ظú������� element ������ʵ�λ��(Խǰ������ȼ�Խ��)�����ҷ��� TRUE
        FTLINLINE virtual BOOL OnAppendElement(const ELEMENT &element);

        //!��Ϊ������Ա������Ԫ�� CFAutoQueueOperator ���ʣ�������ȫ�Ŀ���Element�ĸ��� 
        FTLINLINE BOOL RequestAppend(DWORD dwTimeOut, const ELEMENT& element);
        FTLINLINE BOOL CommitAppend();
        FTLINLINE BOOL RequestRemove(DWORD dwTimeOut, ELEMENT& element);
        FTLINLINE BOOL CommitRemove();
    protected:
        std::deque<ELEMENT> m_AllElement;
        const LONG m_nCapability;                   //! �����Է����Ԫ�ظ���
        LONG m_nReserveSlot;                        //! Slot�ĸ��� 
        BOOL m_bCreateEventStop;                    //! �ж��Ƿ��ڱ����д����� m_hEventStop
        mutable CFMutex             m_lockElements; //! ���� m_AllElement ���ٽ���
        mutable CFCriticalSection   m_lockSlot;     //! ���� m_nReserveSlot ���ٽ���
        HANDLE              m_hSemNumElements;      //! ���ڿ��Ƶ�ǰ�Ѿ��е�Ԫ��
        HANDLE              m_hSemNumSlots;         //! ���ڿ��ƻ����������з������Ԫ��
        HANDLE              m_hEventStop;           //! ����ֹͣ���¼�,ֹͣ�󽫲��������Element
    };

    //! ���ڰ������������ߡ������߶��еĸ����࣬������������
    FTLEXPORT template<typename ELEMENT>
    class CFAutoQueueOperator
    {
    public:
        FTLINLINE CFAutoQueueOperator(
            CFProducerResumerQueue<ELEMENT>* pQueue,
            ELEMENT &refOperationElement, 
            BOOL bIsQuestAppend = TRUE,
            DWORD dwTimeOut = INFINITE);
          FTLINLINE ~CFAutoQueueOperator();
          FTLINLINE BOOL HaveGotQueue() const;
    private:
        CFProducerResumerQueue<ELEMENT>* m_pQueue;
        BOOL m_bIsQuestAppend;
        BOOL m_bHaveGotQueue;
        ELEMENT& m_refOperationElement;
    };

    //���ڶ��������������������ݽ���ͬ��������˳���ṩ��������(һ��ֻ��һ��)�Ķ��С�
    //���֮ǰ��ŵ�����û�����������߱���ȴ�
    //CFProducerResumerQueue �е� OnAppendElement û��̫������� -- ����߳�Ҳ������ȡ������
    //���ں� CFProducerResumerQueue �Ľӿں�ʵ�ֲ��Ƚϴ�ʹ�ü̳л�������д��
    //ͨ�����������̳߳ش������ݣ���������������Ҫͬ������� -- ��ˮ�߸��ã�
    FTLEXPORT template<typename ELEMENT>
    class CFSyncProducerResumerQueue // : public CFProducerResumerQueue<ELEMENT>
    {
    private:
        //< ÿ���߳�һ��PRQueue���������ԺܺõĿ���ÿ���̵߳�ִ�����
        std::map<DWORD, CFProducerResumerQueue<ELEMENT> >   m_threadsPRQueue; 
    };


    //��̨�̻߳���
    struct BackDrawInfo
    {
        //RECT    rect;
        HWND      hWnd;
        HDC       hDC;
    };

	//��̨�̻߳��ƣ�Ȼ��Render��HDC�У�ע����� -- ���ӣ�codeproject �ϵ�gdiplusspeed
    template <typename T>
    FTLEXPORT class CFBackDrawer
    {
    public:
        CFBackDrawer(HWND hWnd, FTL::CFCriticalSection& refLockObj, DWORD dwInterval = 0)
            :m_hWnd(hWnd)
            ,m_refLockObj(refLockObj)
            ,m_ThreadDraw(NULL,NULL)
            ,m_dwLastUpdateTime(0)
        {
        }
        BOOL Start()
        {
            BOOL bRet = FALSE;

            T* pThis = static_cast<T*>(this);
            pThis->OnInit();
            bRet = m_ThreadDraw.Start(DrawProc, this, TRUE);
            return bRet;
        }
        BOOL Stop()
        {
            BOOL bRet = FALSE;
            API_VERIFY(m_ThreadDraw.StopAndWait());
            T* pThis = static_cast<T*>(this);
            pThis->OnFina();
            return bRet;
        }
        BOOL Pause()
        {
            BOOL bRet = FALSE;
            API_VERIFY(m_ThreadDraw.Pause());
            return bRet;
        }
        BOOL Resume()
        {
            BOOL bRet = FALSE;
            API_VERIFY(m_ThreadDraw.Resume());
            return bRet;
        }
        //BOOL AddDrawJob(HDC hSrcDC, const BackDrawInfo* pDrawInfo);
    public:
        BOOL OnInit()
        {
            return TRUE;
        }
        BOOL OnSingleStep()
        {
            //FTLASSERT(FALSE);
            return FALSE;
        }
        BOOL OnFina()
        {
            return TRUE;
        }
    protected:
        static DWORD WINAPI DrawProc(LPVOID pParam)
        {
            T* pThis = static_cast<T*>(pParam);
            DWORD dwResult = pThis->InnerDrawProc();
            return dwResult;
        }
        DWORD InnerDrawProc()
        {
            T* pThis = static_cast<T*>(this);
            while (ftwtContinue == m_ThreadDraw.GetThreadWaitType(INFINITE))
            {
                pThis->OnSingleStep();
            }
            return 0;
        }
    protected:
        HWND                        m_hWnd;
        FTL::CFCriticalSection&     m_refLockObj;
        FTL::CFThread<>             m_ThreadDraw;
        DWORD                       m_dwLastUpdateTime;
    };

//Function
    /*******************************************************************************************************
    * ������ӵ��߳�ͬ�����⣨����ÿ�ε��ö���Ҫ�����̣߳���Դ���Ľϴ󣭣�ע��ʹ�õĵط���Ƶ�ʣ�
    * ������ͣ�����̵߳����У�ֱ���������е����ж������ͬʱ�õ�֪ͨΪֹ��
    *
    * ʹ�÷�ʽ:
    *   HANDLE hWaitObjes[] = {hEventCopy,hSemFile,NULL,hEventContinue,NULL,hEventStop};
    *   DWORD dwWait = WaitForMultipleExpressions(_countof(hWaitObjes),hWaitObjes,INFINITE);
    *
    * ����ֵ��
    *	WAIT_OBJECT_0��(WAIT_OBJECT_0 +���ʽ��1�ĺ���) ---  ����ָ���ĸ����ʽ��ѡ����
    *   WAIT_TIMEOUT  ��ָ����ʱ����û��ѡ�����ʽ
    *	WAIT_FAILED   ����һ��������Ҫ�˽���ϸ��Ϣ������GetLastError��
    *                 ERROR_TOO_MANY_SECRETS -- �趨�ı��ʽ������64��
    *                 ERROR_SECRET_TOO_LONG  --������һ�����ʽ�趨�Ķ��󳬹���63��
    *
    *	@param[in] nExpObjects		����ָ������phExpObjectsָ��������е���Ŀ����
    *	@param[in] phExpObjects	    ���������ں˶�������ÿ����֮����һ��NULL�����ֿ�,
    *                               ���������еĶ�����Ϊ��AND��������Ķ����飬
    *                               �����������������OR��������ľ����
    *	@param[in] dwMilliseconds	
    *
    *   @attention ����������Ŀ���Դ�󳬹�64��Ȼ������ӵ��64�����ϵı��ʽ������ÿ�����ʽ���԰���63�����ϵľ����
    *              ��֧�ֻ������(����֢״��)����ԭ���ǡ����������Ա��߳���ӵ��
    *              ���AND�߳�֮һ��öԻ�����������Ȩ����ô���߳���ֹ����ʱ�����ͻ�����û������
    * 
    * ʵ��˵����
    *   WaitForMultipleObjectsExʹ���߳̿��Եȴ����� AND ���ʽ��
    *   Ϊ����չ�ú�����ʹ֮����ʹ��OR�ı��ʽ���������ɶ���̣߳�ÿ��OR���ʽ��Ҫһ���̡߳�
    *   ��Щ�߳��е�ÿһ����ʹ�� WaitForMultipleObjectsEx ���ȴ�AND���ʽ��
    *   �����е�һ�����ʽ��ѡ��ʱ�����ɵ��߳��о���һ�������Ѳ���ֹ���С�
    *   ���� WaitForMultipleExpressions �������߳�(�����������OR�̵߳��߳���ͬ)����ȴ���
    *   ֱ�����е�һ��OR���ʽ����ʵ�֡����ɵ��߳�(OR���ʽ)�����������ݸ�dwObjects������phObjects������ָ��һ�����飬������
    *   �������ɵ��߳̾�����б�����fWaitAll�������򴫵�FALSE ��������һ���κ�һ�����ʽ����ʵ�֣����߳̾�����������
    *   ʵ��ע�⣺
    *     1.��ϣ�����OR�߳���ͬʱ��������Ϊ�ɹ��صȴ�ĳ���ں˶���ᵼ�¸ö���ı���״̬�����ű������� 
    *       -- ����һ���Լ����ű���󣬳�ʼ������1��ÿ��OR�̶߳����еȴ�(ÿ���������趨�ľ�����ó���64��1��(һ���Զ�����ű�))
    *     2.���ǿ�����ڵȴ����߳�ֹͣ�ȴ����Ա���ȷ�س�������ʹ�� QueueUserAPC ��ǿ�Ƶȴ��߳���������һ����Ŀ����
    *       �첽���̵��ã�APC������ʱ������ȴ��̴߳��ڴ���״̬����ô���Ǿͻᱻǿ�ƻ��ѣ�
    *     3.�����ȷ����ʱ --����߳��ڵȴ�ʱû���κα��ʽ�ȴ�ʵ�֣���ô���̵߳��õ�WaitForMultipleObjects�ͷ���һ��
    *       WAIT_TIMEOUTֵ�������������������Ҿ���Ҫ��ֹ�κα��ʽ����ʵ�֣�������ܵ��¶���ı����ǵ�״̬
    *       ����ʹ�õȴ��ű�İ취����ֹ�������ʽ��ʵ�֡��⽫ʹ�ű�������ݼ�Ϊ0��ͬʱ���е�OR�̶߳�����������������м�ĳ��
    *       ������϶��ĳ�����ʽʵ�֣����ٴεȴ��߳̽�����
    ********************************************************************************************************/

    FTLINLINE DWORD WINAPI WaitForMultipleExpressions(DWORD nExpObjects,CONST HANDLE * phExpObjects,DWORD dwMilliseconds);

    //ʹ���̳߳ؼ�������Ƶ�������������̵߳�����
    //FTLINLINE DWORD WINAPI WaitForMultipleExpressions(CFThreadPool<EXPRESSION>* pWaitThreadPool,DWORD nExpObjects,
    //    CONST HANDLE * phExpObjects,DWORD dwMilliseconds);

}//namespace FTL




namespace FTL
{

    namespace ThreadUtil
    {
#if 0
        class CFWaitForMultipleExpressionsJob : public CFJobBase<EXPRESSION>
        {
        public:
            virtual unsigned int Run(EXPRESSION express)
            {
                DWORD dwResult = WaitForMultipleObjectsEx(
                    express.m_nExpObjects, express.m_phExpObjects, 
                    TRUE, INFINITE, TRUE);
                delete this;
                return dwResult;
            }
            virtual void OnCancelJob(EXPRESSION express)
            {
                delete this;
            }
        };

        inline DWORD WINAPI WaitForMultipleExpressions(CFThreadPool<EXPRESSION>* pWaitThreadPool,DWORD nExpObjects,
            CONST HANDLE * phExpObjects,DWORD dwMilliseconds)
        {
            FTLASSERT(FALSE);//��û�и������

            FTLASSERT(pWaitThreadPool->IsThreadPoolRunning());
            

            BOOL bRet = FALSE;
            //Allocate a temporary array because we modify the passed array and 
            //we need to add a handle at the end for the hsemOnlyOne semaphore.
            PHANDLE phExpObjectsTemp = (PHANDLE) _alloca(sizeof(HANDLE) * (nExpObjects+1));
            CopyMemory(phExpObjectsTemp,phExpObjects,sizeof(HANDLE) * nExpObjects);
            phExpObjectsTemp[nExpObjects] = NULL; //put sentinel at end

            //Semaphore to guarantee that only one expression gets satisfied
            HANDLE hsemOnlyOne = CreateSemaphore(NULL,1,1,NULL);

            //Expression information: 1 per possible thread
            EXPRESSION Expression[MAXIMUM_WAIT_OBJECTS] = {0};

            DWORD dwExpNum      = 0; //Current expression number
            DWORD dwNumExps     = 0; //Total number of expressions

            DWORD dwObjBegin    = 0; //First index of a set
            DWORD dwObjCur      = 0; //Current index of object in a set

            DWORD dwWaitRet = 0;
            unsigned int ThreadId = 0;
            //Array of thread handles for threads : 1 per expression
            HANDLE ahThreads[MAXIMUM_WAIT_OBJECTS] = {0};

            //Parse the callers handle list by initializing a structure for each expression
            //and adding hsemOnlyOne to each expression.

            while ((dwWaitRet != WAIT_FAILED) && (dwObjCur <= nExpObjects))
            {
                //while no errors,and object handles are in the caller's list...
                //Find next expression (OR-expressions are separated by NULL handles)
                while (phExpObjectsTemp[dwObjCur] != NULL)
                {
                    dwObjCur++;
                }

                //Initialize Expression structure which an OR-thread waits on
                phExpObjectsTemp[dwObjCur] = hsemOnlyOne;
                Expression[dwNumExps].m_phExpObjects = &phExpObjectsTemp[dwObjBegin];
                Expression[dwNumExps].m_nExpObjects =  (dwObjCur + 1 - dwObjBegin);

                if (Expression[dwNumExps].m_nExpObjects > MAXIMUM_WAIT_OBJECTS)
                {
                    //Error: Too many handles in single expression
                    dwWaitRet = WAIT_FAILED;
                    SetLastError(ERROR_SECRET_TOO_LONG);
                }

                //Advance to the next expression
                dwObjBegin = ++dwObjCur;
                if (++dwNumExps == MAXIMUM_WAIT_OBJECTS)
                {
                    //Error: Too many expressions
                    dwWaitRet = WAIT_FAILED;
                    SetLastError(ERROR_TOO_MANY_SECRETS);
                }
            }

            if (dwWaitRet != WAIT_FAILED)
            {
                //No errors occurred while parsing the handle list

                //Spawn thread to wait on each expression
                for (dwExpNum = 0; dwExpNum < dwNumExps; dwExpNum++)
                {
                    ahThreads[dwExpNum] = (HANDLE)_beginthreadex(NULL,
                        1, //we only require a small stack
                        WFME_ThreadExpression,&Expression[dwExpNum],
                        0,&ThreadId);
                }

                //wait for an expression to come TRUE or for a timeout
                dwWaitRet = WaitForMultipleObjects(dwExpNum,ahThreads,FALSE,dwMilliseconds);
                if (WAIT_TIMEOUT == dwWaitRet)
                {
                    //we timed-out, check if any expressions were satisfied by checking the state of the hsemOnlyOne semaphore
                    dwWaitRet = WaitForSingleObject(hsemOnlyOne,0);
                    if (WAIT_TIMEOUT == dwWaitRet)
                    {
                        //if the semaphore was not signaled, some thread expressions
                        //was satisfied; we need to determine which expression.
                        dwWaitRet = WaitForMultipleObjects(dwExpNum,ahThreads,FALSE,INFINITE);
                    }
                    else
                    {
                        //No expression was satisfied and WaitForSingleObject just gave us the semaphore
                        //so we know that no expression can ever be satisfied now -- waiting for an expression has tiemd-out.
                        dwWaitRet = WAIT_TIMEOUT;
                    }
                }
                //Break all the waiting expression threads out of their wait state so that they can terminate cleanly
                for (dwExpNum = 0; dwExpNum < dwNumExps ; dwExpNum++)
                {
                    if ((WAIT_TIMEOUT == dwWaitRet) || (dwExpNum != (dwWaitRet - WAIT_OBJECT_0)))
                    {
                        QueueUserAPC(WFME_ExpressionAPC,ahThreads[dwExpNum],0);
                    }
                }
#ifdef _DEBUG
                //In debug builds,wait for all of expression threads to terminate to make sure that we are forcing
                //the threads to wake up.
                //In non-debug builds,we'll assume that this works and not keep this thread waiting any longer.
                WaitForMultipleObjects(dwExpNum,ahThreads,TRUE,INFINITE);
#endif
                //Close our handles to all the expression threads
                for (dwExpNum = 0; dwExpNum < dwNumExps; dwExpNum++)
                {
                    SAFE_CLOSE_HANDLE(ahThreads[dwExpNum],NULL);
                }
            }//error occurred while parsing

            SAFE_CLOSE_HANDLE(hsemOnlyOne,NULL);
            return dwWaitRet;
        }
#endif
    }
}

#endif //FTL_THREAD_H

#ifndef USE_EXPORT
#  include "ftlthread.hpp"
#endif
