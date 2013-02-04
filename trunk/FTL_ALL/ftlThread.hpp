#ifndef FTL_THREAD_HPP
#define FTL_THREAD_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlthread.h"
#endif

namespace FTL
{
    // Internal data structure representing a single expression.
    // Used to tell OR-threads what objects to wait on.
    typedef struct EXPRESSION{
        PHANDLE m_phExpObjects;   // Points to set of handles
        DWORD   m_nExpObjects;    // Number of handles
        HANDLE  m_hNotifyObject;  //用于通知的对象，原来没有(可以设为等待线程的句柄)，使用线程池技术时需要设为返回的 Event ？
    } EXPRESSION, *PEXPRESSION;

    // The OR-thread function
    FTLINLINE unsigned int WINAPI WFME_ThreadExpression(PVOID pvParam) 
    {
        // This thread function just waits for an expression to come true. 
        // The thread waits in an alert able state so that it can be forced
        // to stop waiting by queuing an entry to its APC queue.
        PEXPRESSION pExpression = (PEXPRESSION) pvParam;
        return(WaitForMultipleObjectsEx(
            pExpression->m_nExpObjects, pExpression->m_phExpObjects, 
            TRUE, INFINITE, TRUE));
    }

    //This is the APC callback routine function
    FTLINLINE VOID WINAPI WFME_ExpressionAPC(ULONG_PTR dwData)
    {
        UNREFERENCED_PARAMETER(dwData);
        //FTLTRACEEX(tlInfo,_T("WFME thread(ID=%04d) Left By APC\n"),GetCurrentThreadId());        
        //This function intentionally left blank
    }

    FTLINLINE DWORD WINAPI WaitForMultipleExpressions(DWORD nExpObjects,CONST HANDLE * phExpObjects,DWORD dwMilliseconds)
    {
        //BOOL bRet = FALSE;
        //Allocate a temporary array because we modify the passed array and 
        //we need to add a handle at the end for the hsemOnlyOne semaphore.
        PHANDLE phExpObjectsTemp = (PHANDLE) new HANDLE[nExpObjects+1];
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

        BOOL bRet = FALSE;
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
        SAFE_DELETE_ARRAY(phExpObjectsTemp);
        return dwWaitRet;
    }

    BOOL CFNullLockObject::Lock(DWORD dwTimeout/* = INFINITE */)
    {
        UNREFERENCED_PARAMETER(dwTimeout);
        return TRUE;
    }

    BOOL CFNullLockObject::UnLock()
    {
        return TRUE;
    }

    CFCriticalSection::CFCriticalSection()
    {
#if (_WIN32_WINNT >= 0x0403)
        //使用 InitializeCriticalSectionAndSpinCount 可以提高性能
        ::InitializeCriticalSectionAndSpinCount(&m_CritSec,4000);
#else
        ::InitializeCriticalSection(&m_CritSec);
#endif

#ifdef FTL_DEBUG
        m_lockCount = 0;
        m_currentOwner = 0;
        m_fTrace = FALSE;
#endif
    }

    CFCriticalSection::~CFCriticalSection()
    {
#ifdef FTL_DEBUG
        FTLASSERT(m_lockCount == 0); // 析构前必须完全释放，否则可能造成死锁
#endif
        ::DeleteCriticalSection(&m_CritSec);
    }

    BOOL CFCriticalSection::Lock(DWORD dwTimeout/* = INFINITE*/)
    {
        UNREFERENCED_PARAMETER( dwTimeout );
        FTLASSERT(INFINITE == dwTimeout && ("CFCriticalSection NotSupport dwTimeOut"));
#ifdef FTL_DEBUG
        DWORD us = GetCurrentThreadId();
        DWORD currentOwner = m_currentOwner;
        if (currentOwner && (currentOwner != us)) // already owned, but not by us
        {
            if (m_fTrace) 
            {
                FTLTRACEEX(tlDetail,TEXT("Thread %d begin to wait for CriticalSection %x Owned by %d\n"),
                    us, &m_CritSec, currentOwner);
            }
        }
#endif
        ::EnterCriticalSection(&m_CritSec);

#ifdef FTL_DEBUG
        if (0 == m_lockCount++) // we now own it for the first time.  Set owner information
        {
            m_currentOwner = us;
            if (m_fTrace) 
            {
                FTLTRACEEX(tlDetail,TEXT("Thread %d now owns CriticalSection %x\n"), m_currentOwner, &m_CritSec);
            }
        }
#endif
        return TRUE;
    }

    BOOL CFCriticalSection::UnLock()
    {
#ifdef FTL_DEBUG
        DWORD us = GetCurrentThreadId();
        FTLASSERT( us == m_currentOwner ); //just the owner can unlock it
        FTLASSERT( m_lockCount > 0 );
        if ( 0 == --m_lockCount ) 
        {
            // begin to unlock
            if (m_fTrace) 
            {
                FTLTRACEEX(tlDetail,TEXT("Thread %d releasing CriticalSection %x\n"), m_currentOwner, &m_CritSec);
            }
            m_currentOwner = 0;
        }
#endif
        ::LeaveCriticalSection(&m_CritSec);
        return TRUE;
    }

    BOOL CFCriticalSection::TryLock()
    {
        BOOL bRet = TryEnterCriticalSection(&m_CritSec);
        return bRet;
    }

#ifdef FTL_DEBUG
    BOOL CFCriticalSection::IsLocked() const
    {
        return (m_lockCount > 0);
    }
    BOOL CFCriticalSection::SetTrace(BOOL bTrace)
    {
        m_fTrace = bTrace;
        return TRUE;
    }
#endif

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    CFMutex::CFMutex(BOOL bInitialOwner /*= FALSE*/, LPCTSTR lpName/* = NULL*/,
        LPSECURITY_ATTRIBUTES lpMutexAttributes/* = NULL*/)
    {
        m_hMutex = ::CreateMutex(lpMutexAttributes,bInitialOwner,lpName);
        FTLASSERT(NULL != m_hMutex);
#ifdef FTL_DEBUG
        if (bInitialOwner)
        {
            m_currentOwner = GetCurrentThreadId();
            m_lockCount = 1;
        }
        else
        {
            m_currentOwner = 0;
            m_lockCount = 0;
        }
#endif
    }
    CFMutex::~CFMutex()
    {
#ifdef FTL_DEBUG
        FTLASSERT(m_lockCount == 0); // 析构前必须完全释放，否则可能造成死锁或别的线程出错
#endif
        BOOL bRet = FALSE;
        SAFE_CLOSE_HANDLE(m_hMutex,NULL);
    }
    BOOL CFMutex::Lock(DWORD dwTimeout /* = INFINITE */)
    {
        BOOL bRet = FALSE;
        DWORD dwResult = WaitForSingleObject(m_hMutex,dwTimeout);
#ifdef FTL_DEBUG
        switch (dwResult)
        {
        case WAIT_OBJECT_0:
            m_currentOwner = GetCurrentThreadId();
            m_lockCount++;
            break;
        case WAIT_TIMEOUT:
            break;
        default:
            FTLASSERT(FALSE);
            break;
        }
#endif
        bRet = (dwResult == WAIT_OBJECT_0);
        return bRet;
    }

    BOOL CFMutex::UnLock()
    {
        BOOL bRet = FALSE;
#ifdef FTL_DEBUG
    FTLASSERT(m_lockCount > 0);
        FTLASSERT(GetCurrentThreadId() == m_currentOwner);
#endif
        API_VERIFY(ReleaseMutex(m_hMutex));
#ifdef FTL_DEBUG
        m_lockCount--;
#endif
        return bRet;
    }

    CFEvent::CFEvent(BOOL bInitialState/* = TRUE*/, BOOL bManualReset/* = TRUE*/,
        LPCTSTR pszName/* = NULL*/, LPSECURITY_ATTRIBUTES lpsaAttribute/* = NULL*/)
    {
        m_hEvent = ::CreateEvent(lpsaAttribute, bManualReset,
            bInitialState, pszName);
        FTLASSERT(m_hEvent);
    }
    CFEvent::~CFEvent()
    {
        BOOL bRet = FALSE;
        SAFE_CLOSE_HANDLE(m_hEvent,NULL);
    }

    BOOL CFEvent::Wait(DWORD dwTimeout/* = INFINITE*/) 
    {
        BOOL bRet = FALSE;
        DWORD dwType = WaitForSingleObject(m_hEvent, dwTimeout);
        switch(dwType)
        {
        case WAIT_OBJECT_0:
            bRet = TRUE;
            break;
        case WAIT_TIMEOUT:
            break;
        default:
            API_VERIFY(FALSE);
            break;
        }
        return bRet;
    };

    //允许在等待时处理 SEND 发送的消息(从DirectShow中拷贝的代码) --
    //http://support.microsoft.com/kb/136885/en-us
    BOOL CFEvent::WaitMsg(DWORD dwTimeout/* = INFINITE*/) 
    {
        BOOL  bRet = FALSE;
        DWORD dwWait = 0;
        DWORD dwStartTime = 0;
        DWORD dwWaitTime = dwTimeout;// set the waiting period.

        // the timeout will eventually run down as we iterate
        // processing messages.  grab the start time so that
        // we can calculate elapsed times.
        if (dwWaitTime != INFINITE) 
        {
            dwStartTime = GetTickCount();
        }

        do 
        {
            dwWait = MsgWaitForMultipleObjects(1,&m_hEvent,FALSE, dwWaitTime, QS_SENDMESSAGE);
            if (dwWait == WAIT_OBJECT_0 + 1) 
            { //接收到消息
                MSG Message;
                PeekMessage(&Message,NULL,0,0,PM_NOREMOVE);

                // If we have an explicit length of time to wait calculate
                // the next wake up point - which might be now.
                // If dwTimeout is INFINITE, it stays INFINITE
                if (dwWaitTime != INFINITE) 
                {
                    DWORD dwElapsed = GetTickCount() - dwStartTime;
                    //DWORD dwElapsed = GetTickCount() - dwStartTime;
                    dwWaitTime = (dwElapsed >= dwTimeout)
                        ? 0  // wake up with WAIT_TIMEOUT
                        : dwTimeout-dwElapsed;
                }
            }
        } while (dwWait == WAIT_OBJECT_0 + 1);
        // return TRUE if we woke on the event handle,
        //        FALSE if we timed out.
        bRet = (dwWait == WAIT_OBJECT_0);
        return bRet;
    }

    BOOL CFEvent::Reset() 
    { 
        BOOL bRet = FALSE;
        API_VERIFY(ResetEvent(m_hEvent));
        return bRet;
    };

    BOOL CFEvent::UnLock() 
    {
        BOOL bRet = FALSE;
        API_VERIFY(SetEvent(m_hEvent));
        return bRet;
    }
    BOOL CFEvent::Lock(DWORD dwTimeout/* = INFINITE*/)
    {
        BOOL bRet = FALSE;
        API_VERIFY(Wait(dwTimeout));
        return bRet;
    }

    template<typename T>
    CFAutoLock<T>::CFAutoLock(T* pLockObj)
    {
        FTLASSERT(pLockObj);
        m_pLockObj = pLockObj;
        m_pLockObj->Lock(INFINITE);
    }
    template<typename T>
    CFAutoLock<T>::~CFAutoLock()
    {
        m_pLockObj->UnLock();
    }

    //template<typename T, typename LOCK>
    //CFThreadSafeWrapper::CFThreadSafeWrapper(T* ptr, BOOL bManage)
    //    :m_ptr(ptr)
    //    ,m_bManage(bManage)
    //{
    //}
    //
    //template<typename T, typename LOCK>
    //CFThreadSafeWrapper::~CFThreadSafeWrapper()
    //{
    //    CFAutoLock<LOCK> locker(&m_lockObj);
    //    if (m_bManage)
    //    {
    //        SAFE_DELETE(m_ptr);
    //    }
    //}

    //template<typename T, typename LOCK>
    //T* CFThreadSafeWrapper::operator->() const
    //{
    //    
    //}


    //////////////////////////////////////////////////////////////////////////////////////////
    ////////                  CFRWLocker                         /////////
    //////////////////////////////////////////////////////////////////////////////////////////
    CFRWLocker::CFRWLocker()
    {
        // Initially no readers want access, no writers want access, and 
        // no threads are accessing the resource
        m_pLockObject = new CFCriticalSection;
        m_nWaitingReaders = 0;
        m_nWaitingWriters = 0;
        m_nActive = 0;
        m_hStopEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
        m_hSemReaders = CreateSemaphore(NULL, 0, MAXLONG, NULL);
        m_hSemWriters = CreateSemaphore(NULL, 0, MAXLONG, NULL);
    }

    CFRWLocker::~CFRWLocker()
    {
        // A SWMRG shouldn't be destroyed if any threads are using the resource
        {
            CFAutoLock<CFLockObject> lock(m_pLockObject);
            _ASSERT(0 == m_nActive);
            m_nWaitingReaders = 0;
            m_nWaitingWriters = 0;
            m_nActive = 0;
        }
        BOOL bRet = FALSE;
        SAFE_CLOSE_HANDLE(m_hStopEvent,NULL);
        SAFE_CLOSE_HANDLE(m_hSemReaders,NULL);
        SAFE_CLOSE_HANDLE(m_hSemWriters,NULL);
        SAFE_DELETE(m_pLockObject);
    }

    BOOL CFRWLocker::Start()
    {
        BOOL bRet = FALSE;
        API_VERIFY(ResetEvent(m_hStopEvent));
        return bRet;
    }

    BOOL CFRWLocker::Stop()
    {
        BOOL bRet = FALSE;
        API_VERIFY(SetEvent(m_hStopEvent));
        return bRet;
    }

    FTLThreadWaitType CFRWLocker::WaitToRead(DWORD dwTimeOut)
    {
        FTLThreadWaitType retWaitType = ftwtContinue;
        BOOL fResourceWritePending = FALSE;
        {
            // Ensure exclusive access to the member variables
            CFAutoLock<CFLockObject> lock(m_pLockObject);
            // Are there writers waiting or is a writer writing?
            fResourceWritePending = (m_nWaitingWriters || (m_nActive < 0));
            if (fResourceWritePending) 
            {
                // This reader must wait, increment the count of waiting readers
                m_nWaitingReaders++;
            }
            else 
            {
                // This reader can read, increment the count of active readers
                m_nActive++;
            }
        }
        if (fResourceWritePending) 
        {
            // This thread must wait
            HANDLE hWaitObjects[2] = 
            {
                m_hStopEvent,
                m_hSemReaders
            };
            DWORD dwRet = WaitForMultipleObjects(_countof(hWaitObjects),hWaitObjects,FALSE,dwTimeOut);
            switch(dwRet)
            {
            case WAIT_OBJECT_0:
                retWaitType = ftwtStop;
                break;
            case WAIT_OBJECT_0 + 1:
                retWaitType = ftwtContinue;
                break;
            case WAIT_TIMEOUT:
                retWaitType = ftwtTimeOut;
                break;
            default:
                retWaitType = ftwtError;
                _ASSERT(FALSE);
                break;
            }
            if (ftwtContinue != retWaitType)
            {
                //由于超时或用户停止，不能再等待读取，减少之前设置的值
                CFAutoLock<CFLockObject> lock(m_pLockObject);
                m_nWaitingReaders--;
            }
        }
        else
        {
            //检查用户是否停止了
            DWORD dwRet = WaitForSingleObject(m_hStopEvent,0);
            if (WAIT_OBJECT_0 == dwRet) //用户停止了，将不能再读取
            {
                CFAutoLock<CFLockObject> lock(m_pLockObject);
                m_nActive--;
                retWaitType = ftwtStop;
            }
            else
            {
                FTLASSERT(WAIT_TIMEOUT == dwRet);
                FTLASSERT(ftwtContinue == retWaitType);
            }
        }
        return retWaitType;
    }

    FTLThreadWaitType CFRWLocker::WaitToWrite(DWORD dwTimeOut)
    {
        FTLThreadWaitType retWaitType = ftwtContinue;
        BOOL fResourceOwned = FALSE;
        {
            // Ensure exclusive access to the member variables
            CFAutoLock<CFLockObject> lock(m_pLockObject);
            // Are there any threads accessing the resource?
            fResourceOwned = (m_nActive != 0);
            if (fResourceOwned) 
            {
                // This writer must wait, increment the count of waiting writers
                m_nWaitingWriters++;
            } 
            else 
            {
                // This writer can write, decrement the count of active writers
                m_nActive = -1;
            }
        }

        if (fResourceOwned)
        {
            // This thread must wait
            HANDLE hWaitObjects[2] = 
            {
                m_hStopEvent,
                m_hSemWriters
            };
            DWORD dwRet = WaitForMultipleObjects(_countof(hWaitObjects),hWaitObjects,FALSE,dwTimeOut);
            switch(dwRet)
            {
            case WAIT_OBJECT_0:
                retWaitType = ftwtStop;
                break;
            case WAIT_OBJECT_0 + 1:
                retWaitType = ftwtContinue;
                break;
            case WAIT_TIMEOUT:
                retWaitType = ftwtTimeOut;
                break;
            default:
                retWaitType = ftwtError;
                _ASSERT(FALSE);
                break;
            }
            if (ftwtContinue != retWaitType)
            {
                //由于超时或用户停止，不能再等待写入，更改之前设置的值
                CFAutoLock<CFLockObject> lock(m_pLockObject);
                m_nWaitingWriters--;
            }
        }
        else
        {
            //检查用户是否停止了
            DWORD dwRet = WaitForSingleObject(m_hStopEvent,0);
            if (WAIT_OBJECT_0 == dwRet) //用户停止了，将不能再写入
            {
                CFAutoLock<CFLockObject> lock(m_pLockObject);
                m_nActive = 0;
                retWaitType = ftwtStop;
            }
            else
            {
                FTLASSERT(WAIT_TIMEOUT == dwRet);
            }
        }
        return retWaitType;
    }

    void CFRWLocker::Done()
    {
        HANDLE hsem = NULL;  // Assume no threads are waiting
        LONG lCount = 1;     // Assume only 1 waiter wakes; always true for writers
        {
            // Ensure exclusive access to the member variables
            CFAutoLock<CFLockObject> lock(m_pLockObject);
            FTLASSERT(m_nActive != 0);
            if (m_nActive > 0) 
            {
                // Readers have control so a reader must be done
                m_nActive--;
            } 
            else 
            {
                // Writers have control so a writer must be done
                m_nActive++;
            }

            if (m_nActive == 0) 
            {
                // No thread has access, who should wake up?
                // NOTE: It is possible that readers could never get access
                //       if there are always writers wanting to write
                if (m_nWaitingWriters > 0) 
                {
                    // Writers are waiting and they take priority over readers
                    m_nActive = -1;         // A writer will get access
                    m_nWaitingWriters--;    // One less writer will be waiting
                    hsem = m_hSemWriters;   // Writers wait on this semaphore
                    // NOTE: The semaphore will release only 1 writer thread
                } 
                else if (m_nWaitingReaders > 0) 
                {
                    // Readers are waiting and no writers are waiting
                    m_nActive = m_nWaitingReaders;   // All readers will get access
                    m_nWaitingReaders = 0;           // No readers will be waiting
                    hsem = m_hSemReaders;            // Readers wait on this semaphore
                    lCount = m_nActive;              // Semaphore releases all readers
                } 
                else 
                {
                    // There are no threads waiting at all; no semaphore gets released
                }
            }
            // Allow other threads to attempt reading/writing
            //LeaveCriticalSection(&m_cs);
        }
        if (hsem != NULL) 
        {
            // Some threads are to be released
            ReleaseSemaphore(hsem, lCount, NULL);
        }
    }


    CFEventChecker::CFEventChecker(HANDLE hEventStop,HANDLE hEventContinue)
    {
        m_hEventStop = hEventStop;
        m_hEventContinue = hEventContinue;
        FTLASSERT(m_hEventStop);
        FTLASSERT(m_hEventContinue);
    }

    CFEventChecker::~CFEventChecker()
    {
        m_hEventStop = NULL;
        m_hEventContinue = NULL;
    }

    FTLThreadWaitType CFEventChecker::GetWaitType(DWORD dwTimeOut /* = INFINITE */)
    {
        HANDLE waitEvent[] = 
        {
            {m_hEventStop},
            {m_hEventContinue}
        };

        FTLThreadWaitType waitType = ftwtError;
        DWORD dwResult = WaitForMultipleObjects(sizeof(waitEvent)/sizeof(waitEvent[0]),waitEvent,FALSE,dwTimeOut);
        switch(dwResult)
        {
        case WAIT_OBJECT_0: // stop
            waitType = ftwtStop;
            break;
        case WAIT_OBJECT_0 + 1: //continue
            waitType = ftwtContinue;
            break;
        case WAIT_TIMEOUT:
            waitType = ftwtTimeOut;
            break;
        default:
            FTLTRACEEX(tlError,_T("CFEventChecker::GetWaitType Error!!!\r\n"));
            waitType = ftwtError;
            FTLASSERT(FALSE);
            break;
        }
        return waitType;
    }

    FTLThreadWaitType CFEventChecker::SleepAndCheckStop(DWORD dwTimeOut)
    {
        FTLThreadWaitType waitType = ftwtError;
        DWORD dwResult = WaitForSingleObject(m_hEventStop,dwTimeOut);
        switch (dwResult)
        {
        case WAIT_OBJECT_0: //Stop
            waitType = ftwtStop;
            break;
        case WAIT_TIMEOUT:
            waitType = ftwtTimeOut;
            break;
        default:  //how can come here ?
            waitType = ftwtError;
            break;
        }
        return waitType;
    }

    template <typename T>
    CFSyncEventUtility<T>::CFSyncEventUtility()
    {
        FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
    }

    template <typename T>
    CFSyncEventUtility<T>::~CFSyncEventUtility()
    {
        CFAutoLock<CFLockObject> locker(&m_LockObject);
        ClearAllEvent();
    }

    template <typename T>
    size_t CFSyncEventUtility<T>::GetSyncEventCount()
    {
        size_t count = 0;
        {
            CFAutoLock<CFLockObject> locker(&m_LockObject);
            count = m_AllSyncEventMap.size();
        }
        return count;
    }

    template <typename T>
    void CFSyncEventUtility<T>::ClearAllEvent()
    {
        FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
        CFAutoLock<CFLockObject> locker(&m_LockObject);
        for (SYNC_EVENT_MAP::iterator iter = m_AllSyncEventMap.begin(); 
            iter != m_AllSyncEventMap.end();
            ++iter)
        {
            if ((*iter).second.bCreateEvent)
            {
                HANDLE hEvent = (*iter).second.hEvent;
                FTLASSERT(hEvent);
                CloseHandle(hEvent);
            }
        }
        m_AllSyncEventMap.clear();
    }

    template <typename T>
    void CFSyncEventUtility<T>::AddEvent(T t, HANDLE hEvent /* = NULL */)
    {
        FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
        SyncEventInfo eventInfo = {0};
        if (NULL == hEvent)
        {
            eventInfo.hEvent = ::CreateEvent(NULL,TRUE, FALSE, NULL);
            eventInfo.bCreateEvent = true;
        }
        else
        {
            eventInfo.hEvent = hEvent;
            eventInfo.bCreateEvent = false;
        }

        {
            CFAutoLock<CFLockObject> locker(&m_LockObject);
            FTLASSERT(m_AllSyncEventMap.find(t) == m_AllSyncEventMap.end());
            m_AllSyncEventMap[t] = eventInfo;
        }
    }

    template <typename T>
    void CFSyncEventUtility<T>::SetEvent(T t)
    {
        FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
        CFAutoLock<CFLockObject> locker(&m_LockObject);

        SYNC_EVENT_MAP::iterator iter = m_AllSyncEventMap.find(t);
        FTLASSERT(iter != m_AllSyncEventMap.end());
        if (iter != m_AllSyncEventMap.end())
        {
            ::SetEvent(iter->second.hEvent);
        }
    }

    template <typename T>
    void CFSyncEventUtility<T>::ResetEvent(T t)
    {
        FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
        CFAutoLock<CFLockObject> locker(&m_LockObject);

        SYNC_EVENT_MAP::iterator iter = m_AllSyncEventMap.find(t);
        FTLASSERT(iter != m_AllSyncEventMap.end());
        if (iter != m_AllSyncEventMap.end())
        {
            ::ResetEvent(iter->second.hEvent);
        }
    }

    template <typename T>
    void CFSyncEventUtility<T>::SetAllEvent()
    {
        FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
        CFAutoLock<CFLockObject> locker(&m_LockObject);
        SYNC_EVENT_MAP::iterator iter = m_AllSyncEventMap.begin();
        for (; iter != m_AllSyncEventMap.end(); ++iter)
        {
            ::SetEvent(iter->second.hEvent);
        }
    }

    template <typename T>
    void CFSyncEventUtility<T>::ResetAllEvent()
    {
        FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
        CFAutoLock<CFLockObject> locker(&m_LockObject);
        SYNC_EVENT_MAP::iterator iter = m_AllSyncEventMap.begin();
        for (; iter != m_AllSyncEventMap.end(); ++iter)
        {
            ::ResetEvent(iter->second.hEvent);
        }
    }

    template <typename T>
    BOOL CFSyncEventUtility<T>::WaitAllEvent(DWORD dwMilliseconds /* = INFINITE */)
    {
        FUNCTION_BLOCK_TRACE(0);
        BOOL bRet = FALSE;

        std::vector<HANDLE> hAllEvents;
        SYNC_EVENT_MAP::iterator iter = m_AllSyncEventMap.begin();
        for (; iter != m_AllSyncEventMap.end(); ++iter)
        {
            hAllEvents.push_back(iter->second.hEvent);
        }

        DWORD dwResult = ::WaitForMultipleObjects((DWORD)hAllEvents.size(), &hAllEvents[0], TRUE, dwMilliseconds);
        if (WAIT_OBJECT_0 == dwResult)
        {
            bRet = TRUE;
        }
        return bRet;
    }

    template <typename T>
    T CFSyncEventUtility<T>::WaitOneEvent(DWORD dwMilliseconds /* = INFINITE */)
    {
        std::vector<HANDLE> hAllEvents;
        SYNC_EVENT_MAP::iterator iter = m_AllSyncEventMap.begin();
        for (; iter != m_AllSyncEventMap.end(); ++iter)
        {
            hAllEvents.push_back(iter->second.hEvent);
        }
        DWORD dwResult = ::WaitForMultipleObjects(hAllEvents.size(), &hAllEvents[0], FALSE, dwMilliseconds);

        T tRet;
        int index = 0;
        for (SYNC_EVENT_MAP::iterator iter = m_AllSyncEventMap.begin(); iter != m_AllSyncEventMap.end(); ++iter)
        {
            if (index == waitIndex)
            {
                tRet = iter->first;
                break;
            }
            index++
        }
        return tRet; 
    }

    typedef struct tagTHREADNAME_INFO
    {
        DWORD dwType; // must be 0x1000
        LPSTR pszName; // pointer to name (in user addr space)
        DWORD dwThreadID; // thread ID (-1=caller thread)
        DWORD dwFlags; // reserved for future use, must be zero
    } THREADNAME_INFO;

    //可以不遵守???设置线程名字的字符串在线程生存期间必须一直存在(可以是常数、全局变量或放在堆中 )
    BOOL CFThreadUtils::SetThreadName( DWORD dwThreadID, LPTSTR szThreadName)
    {
        BOOL fOkay = TRUE;
        //char** ppszThreadName = NULL;
        //__asm                            //定位呼叫线程的 TIB 结构 （在TIB结构中的0x14偏移处  &pTIB->pvArbitrary ）
        //{
        //	mov eax,fs:[0x18]    ;
        //	add eax,0x14        ;
        //	mov [ppszThreadName],eax;        
        //}
        //if (*ppszThreadName == NULL)    //如果该域( pvArbitrary )没有设置过,
        //{
        //	*ppszThreadName = pszName;
        //	fOkay = TRUE;
        //}
        THREADNAME_INFO tni = {	0 };
        LPSTR szAnsiName = NULL;

#ifdef _UNICODE
        USES_CONVERSION;
        size_t iLen = wcslen ( szThreadName ) + 1 ;
        szAnsiName = (LPSTR)HeapAlloc ( GetProcessHeap ( )        ,
            HEAP_GENERATE_EXCEPTIONS |
            HEAP_ZERO_MEMORY      ,
            iLen * sizeof ( char ));
        strncpy_s(szAnsiName,iLen, T2A(szThreadName),_TRUNCATE);
#else
        szAnsiName = szThreadName;
#endif

        tni.dwType = 0x1000;
        tni.pszName = szAnsiName;
        tni.dwThreadID = dwThreadID;  //if -1, then current Thread
        __try
        {
            RaiseException(0x406d1388, 0, sizeof(tni) / sizeof(DWORD),(DWORD*)(&tni));      //该异常通知调试器将给定的字符串名字分配给指定的异常
        }__except(EXCEPTION_EXECUTE_HANDLER)
        {
            fOkay = FALSE;
        }
#ifdef _UNICODE
        HeapFree ( GetProcessHeap ( ) , 0 , szAnsiName ) ;
#endif

        return fOkay;
    }

    //获取线程名字，常用于打印跟踪代码
    LPCSTR CFThreadUtils::GetThreadName(DWORD dwThreadID)
    {
        UNREFERENCED_PARAMETER(dwThreadID);
        char *pszName = NULL;
        __asm{
            mov eax,fs:[0x18];
            mov eax,[eax+0x14];
            mov [pszName],eax;
        }
        return(pszName?pszName:"unKnown");
    }


    struct StartThreadProxyStruct
    {
        LPTHREAD_START_ROUTINE	pfn;
        LPVOID			        param;
    };
    static DWORD WINAPI StartThreadProxy(void* param)
    {
        //	_control87(_EM_DENORMAL | _EM_ZERODIVIDE | _EM_OVERFLOW | _EM_UNDERFLOW | _EM_INEXACT,  _MCW_EM);
        //	_control87(_PC_53, _MCW_PC);
#if 0
        //从XpCommonLib中学来，作用是什么？
        _control87(_RC_CHOP, _MCW_RC);
        setlocale(LC_ALL, "");
#endif 
        StartThreadProxyStruct* pProxyStruct = (StartThreadProxyStruct*)param;
        DWORD retval = (*pProxyStruct->pfn)(pProxyStruct->param);
        delete pProxyStruct;
        return retval;
    }


    template <typename ThreadTraits>
    CFThread<ThreadTraits>::CFThread(HANDLE hEventStop/* = NULL*/,HANDLE hEventContinue/* = NULL*/)
    {
        m_hThread	= NULL;
        if (NULL == hEventStop)
        {
            m_hEventStop	= ::CreateEvent(NULL,TRUE,FALSE,NULL);
            m_bCreateEventStop = TRUE;
        }
        else
        {
            m_hEventStop = hEventStop;
            m_bCreateEventStop = FALSE;
        }

        if (NULL == hEventContinue)
        {
            m_hEventContinue = ::CreateEvent(NULL,TRUE,TRUE,NULL);
            m_bCreateEventContinue = TRUE;
        }
        else
        {
            m_hEventContinue = hEventContinue;
            m_bCreateEventContinue = FALSE;
        }
        m_pEventChecker = new CFEventChecker(m_hEventStop, m_hEventContinue);
        //m_ThreadState = ftsStopped;
    }
    template <typename ThreadTraits>
    CFThread<ThreadTraits>::~CFThread()
    {
        BOOL bRet = FALSE;
        if( IsThreadRunning())
        {
            FTLASSERT(!_T("Please Stop Thread Before App Exit"));
            API_VERIFY(StopAndWait(FTL_MAX_THREAD_DEADLINE_CHECK,TRUE));
        }
        SAFE_DELETE(m_pEventChecker);

        if (m_bCreateEventContinue)
        {
            SAFE_CLOSE_HANDLE(m_hEventContinue,NULL);
        }
        else
        {
            m_hEventContinue = NULL;
        }
        if (m_bCreateEventStop)
        {
            SAFE_CLOSE_HANDLE(m_hEventStop,NULL);
        }
        else
        {
            m_hEventStop = NULL;
        }
    }
    template <typename ThreadTraits>
    FTLThreadWaitType CFThread<ThreadTraits>::GetThreadWaitType(DWORD dwTimeOut = INFINITE)
    {
        FTLThreadWaitType waitType = ftwtError;
        waitType = m_pEventChecker->GetWaitType(dwTimeOut);
        return waitType;
    }

    template <typename ThreadTraits>
    FTLThreadWaitType CFThread<ThreadTraits>::SleepAndCheckStop(DWORD dwTimeOut)
    {
        FTLThreadWaitType waitType = ftwtError;
        waitType = m_pEventChecker->SleepAndCheckStop(dwTimeOut);
        return waitType;
    }

    template <typename ThreadTraits>
    BOOL CFThread<ThreadTraits>::Start(LPTHREAD_START_ROUTINE pfnThreadProc,void *pvParam ,BOOL resetEvent/* = TRUE*/)
    {
        BOOL bRet = FALSE;
        FTLASSERT(pfnThreadProc);

        if (NULL == pfnThreadProc)
        {
            SetLastError(ERROR_INVALID_PARAMETER);
            return FALSE;
        }

        if( IsThreadRunning())
        {
            FTLASSERT(!TEXT("must stop thread before restart"));
            //SetLastError(ERROR_ALREADY_INITIALIZED);
            return TRUE;
        }

        if( m_hThread )
        {
            FTLASSERT(FALSE && TEXT("stop thread before call start again"));
            ::CloseHandle( m_hThread );
            m_hThread = NULL;
        }

        if (resetEvent)
        {
            ResetEvent(m_hEventStop);
            SetEvent(m_hEventContinue);
        }

        DWORD threadid = 0;
        StartThreadProxyStruct* pProxyStruct = new StartThreadProxyStruct;
        pProxyStruct->param = pvParam;
        pProxyStruct->pfn = pfnThreadProc;

        API_VERIFY(NULL != (m_hThread = ThreadTraits::CreateThread(NULL,0,
            StartThreadProxy,pProxyStruct,0,&threadid)));
        if( !m_hThread )
        {
            delete pProxyStruct;
            return FALSE;
        }
        //m_ThreadState = ftsRunning;
        return bRet;
    }

    template <typename ThreadTraits>
    BOOL CFThread<ThreadTraits>::Stop()
    {
        BOOL bRet = TRUE;
		if (IsThreadRunning())
		{
			API_VERIFY(::SetEvent( m_hEventStop));
			// allow thread to run at higher priority during kill process
			::SetThreadPriority(m_hThread, THREAD_PRIORITY_ABOVE_NORMAL);
		}
        return (bRet);
    }

    template <typename ThreadTraits>
    BOOL CFThread<ThreadTraits>::StopAndWait(DWORD dwTimeOut /* = FTL_MAX_THREAD_DEADLINE_CHECK */, 
        BOOL bCloseHandle /* = TRUE */, BOOL bTerminateIfTimeOut /* = TRUE */)
    {
        BOOL bRet = TRUE;
		API_VERIFY(Stop());
		API_VERIFY_EXCEPT1(Wait(dwTimeOut,bCloseHandle, bTerminateIfTimeOut), ERROR_TIMEOUT);
        return bRet;
    }

    template <typename ThreadTraits>
    BOOL CFThread<ThreadTraits>::Pause()
    {
        BOOL bRet = FALSE;
        API_VERIFY(::ResetEvent(m_hEventContinue));
        return bRet;
    }
    template <typename ThreadTraits>
    BOOL CFThread<ThreadTraits>::Resume()
    {
        BOOL bRet = FALSE;
        API_VERIFY(::SetEvent(m_hEventContinue));
        return bRet;
    }

    template <typename ThreadTraits>
    BOOL CFThread<ThreadTraits>::LowerPriority()
    {
        BOOL bRet = FALSE;
        int priority = ::GetThreadPriority(m_hThread); 
        API_VERIFY(::SetThreadPriority(m_hThread, priority - 1));
        return bRet;
    }

    template <typename ThreadTraits>
    BOOL CFThread<ThreadTraits>::RaisePriority()
    {
        BOOL bRet = FALSE;
        int priority = ::GetThreadPriority(m_hThread); 
        API_VERIFY(::SetThreadPriority(m_hThread, priority + 1));
        return bRet;
    }

    template <typename ThreadTraits>
    int CFThread<ThreadTraits>::GetPriority() const
    {
        int priority = ::GetThreadPriority(m_hThread); 
        return priority;
    }


    template <typename ThreadTraits>
    BOOL CFThread<ThreadTraits>::SetPriority(int priority)
    {
        BOOL bRet = FALSE;
        API_VERIFY(::SetThreadPriority(m_hThread, priority));
        return bRet;
    }

    template <typename ThreadTraits>
    BOOL CFThread<ThreadTraits>::IsThreadRunning() const
    {
        if( !m_hThread )
            return FALSE;

        DWORD	dwExitCode = 0;
        BOOL bRet = FALSE;
        //判断线程是否结束
        API_VERIFY(::GetExitCodeThread( m_hThread, &dwExitCode ));
        if (FALSE == bRet)
        {
            return FALSE;
        }
        bRet = (dwExitCode == STILL_ACTIVE ? TRUE : FALSE);
        return bRet;
    }

    template <typename ThreadTraits>
    BOOL CFThread<ThreadTraits>::HadRequestPause() const
    {
        DWORD dwResult = WaitForSingleObject(m_hEventContinue,0);
        BOOL bRet = (dwResult == WAIT_TIMEOUT);
        return bRet;
    }

    template <typename ThreadTraits>
    BOOL CFThread<ThreadTraits>::Wait(DWORD dwTimeOut/* = INFINITE*/,BOOL bCloseHandle/* = TRUE*/, 
		BOOL bTerminateIfTimeOut /* = TRUE */)
    {
        BOOL bRet = TRUE;
        if (NULL != m_hThread)
        {
            DWORD dwRet = WaitForSingleObject(m_hThread,dwTimeOut);
            switch(dwRet)
            {
            case WAIT_OBJECT_0:  //Stopped
                bRet = TRUE;
                break;
            case WAIT_TIMEOUT:
				FTLTRACEEX(tlWarning, TEXT("WARNING!!!: Wait For Thread %d(ms) TimeOut, Handle=0x%x\n"), 
					dwTimeOut, m_hThread);
				if (bTerminateIfTimeOut)
				{
					FTLASSERT(FALSE && TEXT("WaitFor Time Out"));
					TerminateThread(m_hThread, DWORD(-1));
				}
                bRet = FALSE;
				SetLastError(ERROR_TIMEOUT);
                break;
            default:
                FTLASSERT(FALSE);
                bRet = FALSE;
                break;
            }
            if (bCloseHandle)
            {
                SAFE_CLOSE_HANDLE(m_hThread,NULL);
            }
        }
        return bRet;
    }

    template <typename T>
    CFJobBase<T>::CFJobBase()
    {
		m_nJobIndex = 0;
        m_pThreadPool = NULL;
		m_bCancel = FALSE;
    }
    template <typename T>
    CFJobBase<T>::~CFJobBase()
    {

    }
	template <typename T>
	INT CFJobBase<T>::GetJobIndex() const
	{
		return m_nJobIndex;
	}
    template <typename T>
    FTLThreadWaitType CFJobBase<T>::GetThreadPoolWaitType(DWORD dwMilliseconds /* = INFINITE*/) const
    {
        FUNCTION_BLOCK_TRACE(0);
        FTLThreadWaitType waitType = ftwtError;
        HANDLE waitEvent[] = 
        {
            {m_pThreadPool->m_hEventStop},
            {m_pThreadPool->m_hEventContinue}
        };
        DWORD dwResult = ::WaitForMultipleObjects(_countof(waitEvent),waitEvent,FALSE,dwMilliseconds);
        switch (dwResult)
        {
        case WAIT_OBJECT_0:
            waitType = ftwtStop;
            break;
        case WAIT_OBJECT_0 + 1:
            waitType = ftwtContinue;
            break;
        case WAIT_TIMEOUT:
            waitType = ftwtTimeOut;
            break;
        default:
            FTLASSERT(FALSE);
            waitType = ftwtError;
            break;
        }
        return waitType;
    }

    //////////////////////////////////////    CFThreadPool    ///////////////////////////////////////////////////
    template <typename T>  
    CFThreadPool<T>::CFThreadPool(LONG nMinNumThreads,LONG nMaxNumThreads)
        :m_nMinNumThreads(nMinNumThreads)
        ,m_nMaxNumThreads(nMaxNumThreads)
    {
        FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
        FTLASSERT( 0 <= nMinNumThreads );
        FTLASSERT( nMinNumThreads <= nMaxNumThreads );       
        FTLASSERT( nMaxNumThreads <= MAXIMUM_WAIT_OBJECTS);  

        m_nCurNumThreads = 0;
        m_pThreadHandles = NULL;
        m_pThreadIds = NULL;
		m_nJobIndex = 0;

        //将可以同时做的工作数设置为 MAXLONG -- 目前暂时不考虑队列中的个数
        m_hSemaphoreJobToDo = ::CreateSemaphore(NULL,0,MAXLONG,NULL);
        FTLASSERT(NULL != m_hSemaphoreJobToDo);

        m_hEventStop = CreateEvent(NULL,TRUE,FALSE,NULL);
        FTLASSERT(NULL != m_hEventStop);

        m_hEventContinue = ::CreateEvent(NULL,TRUE,TRUE,NULL);
        FTLASSERT(NULL != m_hEventContinue);

        if (m_nMinNumThreads < m_nMaxNumThreads)
        {
            //如果最小和最大线程个数不同，说明可以动态调整线程池，最多减少 nMaxNumThreads - nMinNumThreads 个线程
            m_hSemaphoreSubtractThread = CreateSemaphore(NULL,0,nMaxNumThreads - nMinNumThreads,NULL);
            FTLASSERT(NULL != m_hSemaphoreSubtractThread);
        }
        else
        {
            //不能动态调整调整线程个数 -- 创建一个最大值为1的Semaphore，逻辑中保证不会调用Release
            m_hSemaphoreSubtractThread = CreateSemaphore(NULL,0,1,NULL);
        }
    }

    template <typename T>  
    CFThreadPool<T>::~CFThreadPool(void)
    {
        FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
        BOOL bRet = FALSE;
        API_VERIFY(StopAndWait(FTL_MAX_THREAD_DEADLINE_CHECK, TRUE));
        DestroyPool();
    }

    template <typename T>  
    BOOL CFThreadPool<T>::Start()
    {
        FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
        BOOL bRet = FALSE;
        API_VERIFY(ResetEvent(m_hEventStop));   //重设Stop事件
        API_VERIFY(SetEvent(m_hEventContinue)); //重设继续事件
        {
            CFAutoLock<CFCriticalSection>   locker(&m_lockThreads); //m_lockJobs);
            if(NULL == m_pThreadHandles)    //防止多次调用Start
            {
                unsigned int threadId = 0;
                m_pThreadHandles = new HANDLE[m_nMaxNumThreads];  //分配m_nMaxNumThreads个线程的空间
                ZeroMemory(m_pThreadHandles,sizeof(HANDLE) * m_nMaxNumThreads);
                m_pThreadIds = new DWORD[m_nMaxNumThreads];
                ZeroMemory(m_pThreadIds,sizeof(DWORD) * m_nMaxNumThreads);
                for( LONG i = 0;i < m_nMinNumThreads; i++ )  //开始时只创建 m_nMinNumThreads 个线程
                {
                    m_pThreadHandles[m_nCurNumThreads] = (HANDLE) _beginthreadex( NULL, 0, ThreadExecute, this, 0, &threadId);
                    m_pThreadIds[m_nCurNumThreads] = threadId;
                    FTLASSERT(m_pThreadHandles[m_nCurNumThreads]);
                    m_nCurNumThreads++;
                }
                FTLASSERT(m_nCurNumThreads == m_nMinNumThreads);
            }
        }
        bRet = TRUE;
        return bRet;
    }

    template <typename T>  
    BOOL CFThreadPool<T>::Stop()
    {
        FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
        BOOL bRet = TRUE;
        API_VERIFY(SetEvent(m_hEventStop));
        return bRet;
    }

    template <typename T>  
    BOOL CFThreadPool<T>::StopAndWait(DWORD dwTimeOut /* = FTL_MAX_THREAD_DEADLINE_CHECK */,
            BOOL bCloseHandle /* = TRUE */)
    {
        FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
        BOOL bRet = TRUE;
        API_VERIFY(Stop());
        API_VERIFY(Wait(dwTimeOut,bCloseHandle));
        return bRet;
    }

    template <typename T>
    BOOL CFThreadPool<T>::Wait(DWORD dwTimeOut,BOOL bCloseHandle)
    {
        FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
        BOOL bRet = TRUE;
        HANDLE* pTempHandles = NULL;
        LONG  nTempNumThreads = 0;
        {
            //由于线程结束后会自动调整 m_pThreadHandles 数组，因此，这里生成临时变量
            CFAutoLock<CFLockObject> locker(&m_lockThreads); //m_lockJobs);
            if(m_pThreadHandles) //The threads have started;
            {
                nTempNumThreads = m_nCurNumThreads;
                pTempHandles = new HANDLE[m_nCurNumThreads];
                CopyMemory(pTempHandles,m_pThreadHandles,sizeof(HANDLE) * m_nCurNumThreads);
            }
        }
        if (pTempHandles)
        {
            DWORD dwResult = WaitForMultipleObjects(nTempNumThreads,pTempHandles,TRUE,dwTimeOut);
            switch (dwResult)
            {
            case WAIT_OBJECT_0: //所有的线程都结束了
                bRet = TRUE;
                break;
            case WAIT_TIMEOUT:
                FTLTRACEEX(tlError,TEXT("!!!CFThreadPool::Wait, Not all thread over in %d millisec\n"),dwTimeOut);
                bRet = FALSE;
                break;
            default:
                FTLASSERT(FALSE);
                bRet = FALSE;
                break;
            }
            SAFE_DELETE_ARRAY(pTempHandles);
        }
        if (bCloseHandle)
        {
            CFAutoLock<CFLockObject> locker(&m_lockThreads);
            for (LONG i = 0; i < m_nCurNumThreads; i++)
            {
                SAFE_CLOSE_HANDLE(m_pThreadHandles[i],NULL);
            }
            SAFE_DELETE_ARRAY(m_pThreadHandles);
            SAFE_DELETE_ARRAY(m_pThreadIds);
            m_nCurNumThreads = 0;
        }
        return bRet;
    }

    template <typename T>
    BOOL CFThreadPool<T>::Pause()
    {
        BOOL bRet = FALSE;
        API_VERIFY(::ResetEvent(m_hEventContinue));
        return bRet;
    }

    template <typename T>
    BOOL CFThreadPool<T>::Resume()
    {
        BOOL bRet = FALSE;
        API_VERIFY(::SetEvent(m_hEventContinue));
        return bRet;
    }

    template <typename T>  
    BOOL CFThreadPool<T>::ClearUndoWork()
    {
        FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
        BOOL bRet = TRUE;
        {
            CFAutoLock<CFLockObject> locker(&m_lockWaitingJobs);
            while (!m_WaitingJobs.empty())
            {
                DWORD dwResult = WaitForSingleObject(m_hSemaphoreJobToDo,INFINITE); //用于释放对应的信标对象
                API_VERIFY(dwResult == WAIT_OBJECT_0);

				JobInfoContainer::iterator iterBegin = m_WaitingJobs.begin();
                JobInfo* pInfo = iterBegin->second;
                FTLASSERT(pInfo);
                pInfo->pJob->OnCancelJob(pInfo->param);
                delete pInfo;
                m_WaitingJobs.erase(iterBegin);
            }
        }
        return bRet;
    }

    template <typename T>  
    BOOL CFThreadPool<T>::AddThread(LONG nThreadNum)
    {
        FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
        BOOL bRet = FALSE;
        {
            CFAutoLock<CFLockObject> locker(&m_lockThreads);
            if (m_nCurNumThreads + nThreadNum > m_nMaxNumThreads)
            {
                //超过最大个数，不能再加了
                SetLastError(ERROR_INVALID_PARAMETER);
                bRet = FALSE;
            }
            else
            {
                unsigned int threadId = 0;
                for(LONG i = 0;i < nThreadNum; i++)
                {
                    FTLASSERT(NULL == m_pThreadHandles[m_nCurNumThreads]);
                    m_pThreadHandles[m_nCurNumThreads] = (HANDLE) _beginthreadex( NULL, 0, ThreadExecute, this, 0, &threadId);
                    m_pThreadIds[m_nCurNumThreads] = threadId;
                    FTLASSERT(NULL != m_pThreadHandles[m_nCurNumThreads]);
                    m_nCurNumThreads++;
                    FTLTRACEEX(tlTrace,TEXT("CFThreadPool<T>::AddThread,Thread id = %d,CurNumThreads = %d\n"),
                        threadId,m_nCurNumThreads);
                }
                bRet = TRUE;
            }
        }
        return bRet;
    }

    template <typename T>
    BOOL CFThreadPool<T>::OnSubmitJob(const JobInfo* pJobInfo)
    {
        UNREFERENCED_PARAMETER(pJobInfo);
        //默认实现直接返回FALSE，在JobQueue的最后增加
        return FALSE;
    }

    template <typename T>
    BOOL CFThreadPool<T>::OnAddThread()
    {
        return FALSE;
    }

    template <typename T>
    BOOL CFThreadPool<T>::OnSubtractThread()
    {
        return FALSE;
    }

    template <typename T>  
    BOOL CFThreadPool<T>::SubmitJob(CFJobBase<T>* pJob,const T& param, INT* pOutJobIndex)
    {
        FTLASSERT(NULL != m_hEventStop); //如果调用DestroyPool后，就不能再次调用该函数
        FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
        BOOL bRet = FALSE;
        {
            //加入Job并且唤醒一个等待线程
            {
                CFAutoLock<CFLockObject> locker(&m_lockWaitingJobs);
				m_nJobIndex++;
                pJob->m_pThreadPool = this;         //访问私有变量，并将自己赋值过去
				pJob->m_nJobIndex = m_nJobIndex;	//访问私有变量，设置JobIndex

				JobInfo *pInfo = new JobInfo;
                pInfo->pJob = pJob;
                pInfo->param = param;
				if (pOutJobIndex)
				{
					*pOutJobIndex = m_nJobIndex;
				}
                if (!OnSubmitJob(pInfo))
                {
					m_WaitingJobs.insert(JobInfoContainer::value_type(m_nJobIndex, pInfo));
                    //m_WaitingJobs[m_nJobIndex] = pInfo;
                }
                API_VERIFY(ReleaseSemaphore(m_hSemaphoreJobToDo,1L,NULL));
            }

            SwitchToThread();//唤醒等待的线程，使得其他线程可以获取Job -- 注意 CFAutoLock 的范围

            {
				//检查是否需要增加线程
                CFAutoLock<CFLockObject> locker(&m_lockWaitingJobs);
                BOOL bNeedMoreThread = (!m_WaitingJobs.empty() && (m_nCurNumThreads < m_nMaxNumThreads));
                {
                    if (bNeedMoreThread)
                    {
                        API_VERIFY(AddThread(1L));      //每次增加一个线程
                    }
                }
            }
        }
        return bRet;	
    }

	template <typename T>  
	BOOL CFThreadPool<T>::CancelJob(INT nJobIndex)
	{
		//本函数尚未测试
		BOOL bRet = FALSE;
		BOOL bFoundWaiting = FALSE;
		BOOL bFoundDoing = FALSE;
		{
			//首先查找未启动的任务
			CFAutoLock<CFLockObject> locker(&m_lockWaitingJobs);
			JobInfoContainer::iterator iterWaiting = m_WaitingJobs.find(nJobIndex);
			if (iterWaiting != m_WaitingJobs.end())
			{
				DWORD dwResult = WaitForSingleObject(m_hSemaphoreJobToDo, INFINITE); //释放对应的信标对象，避免再次找到
				API_ASSERT(dwResult == WAIT_OBJECT_0);

				//找到,说明这个Job还没有启动
				JobInfo* pInfo = iterWaiting->second;
				FTLASSERT(pInfo);
				FTLASSERT(pInfo->pJob->GetJobIndex() == nJobIndex);
				pInfo->pJob->OnCancelJob(pInfo->param);
				delete pInfo;
				m_WaitingJobs.erase(iterWaiting);

				bFoundWaiting = TRUE;
			}
		}
		if (!bFoundWaiting)
		{
			//查找正在运行的任务
			CFAutoLock<CFLockObject> locker(&m_lockDoingJobs);
			JobInfoContainer::iterator iterDoing = m_DoingJobs.find(nJobIndex);
			if (iterDoing != m_DoingJobs.end())
			{
				JobInfo* pInfo = iterDoing->second;
				FTLASSERT(pInfo);
				FTLASSERT(pInfo->pJob->GetJobIndex() == nJobIndex);
				pInfo->pJob->NotifyCancel(pInfo->param);
				//注意：这里只是请求Cancel，实际上任务是否能真正Cancel，需要依赖Job的实现，

				//不要 delete pInfo 和 m_DoingJobs.erase -- Job 结束后再取消
				////delete pInfo;
				////m_DoingJobs.erase(iterDoing);
				
				bFoundDoing = TRUE;
			}
		}
		if (!bFoundWaiting && !bFoundDoing)
		{
			//Waiting 和 Doing 中都没有找到，已经执行完毕
			//do nothing
		}

		return TRUE; //(bFoundDoing || bFoundWaiting);
	}

    template <typename T>  
    BOOL CFThreadPool<T>::HadRequestStop() const
    {
        _ASSERT(NULL != m_hEventStop);
        BOOL bRet = (WaitForSingleObject(m_hEventStop,0) == WAIT_OBJECT_0);
        return bRet;
    }

    template <typename T>
    BOOL CFThreadPool<T>::HadRequestPause() const
    {
        BOOL bRet = (WaitForSingleObject(m_hEventContinue,0) == WAIT_TIMEOUT);
        return bRet;
    }

    template <typename T>  
    void CFThreadPool<T>::DestroyPool()
    {
        FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
        BOOL bRet = FALSE;
        API_VERIFY(ClearUndoWork());
        SAFE_CLOSE_HANDLE(m_hSemaphoreJobToDo,NULL);
        SAFE_CLOSE_HANDLE(m_hSemaphoreSubtractThread,NULL);
        SAFE_CLOSE_HANDLE(m_hEventContinue,NULL);
        SAFE_CLOSE_HANDLE(m_hEventStop,NULL);
    }

    template <typename T>  
    GetJobType CFThreadPool<T>::GetJob(CFJobBase<T>** ppJob, T* pParam)
    {
        FUNCTION_BLOCK_TRACE(0);
        HANDLE hWaitHandles[] = 
        {
            m_hEventStop,                 //用户选择了停止
            m_hSemaphoreSubtractThread,   //需要减少线程
            m_hSemaphoreJobToDo,          //还有未作的Job
        };

        DWORD dwResult = WaitForMultipleObjects(_countof(hWaitHandles),hWaitHandles,FALSE,INFINITE);
        switch(dwResult)
        {
        case WAIT_OBJECT_0: //Stop Event
            return typeStop;
        case WAIT_OBJECT_0 + 1://减少线程
            return typeSubtractThread;
        case WAIT_OBJECT_0 + 2: //Get Job
            break;
        default:
            FTLASSERT(FALSE);
            break;
        }
        {
            CFAutoLock<CFLockObject> lockerWating(&m_lockWaitingJobs);
			FTLASSERT(!m_WaitingJobs.empty());
			JobInfoContainer::iterator iterBegin = m_WaitingJobs.begin();
			INT nJobIndex = iterBegin->first;
            JobInfo* pInfo = iterBegin->second;
            FTLASSERT(pInfo);

            *ppJob = pInfo->pJob;
			*pParam = pInfo->param;
            m_WaitingJobs.erase(iterBegin);
			{
				CFAutoLock<CFLockObject> lockerDoing(&m_lockDoingJobs);
				m_DoingJobs.insert(JobInfoContainer::value_type(nJobIndex, pInfo));			
			}
        }
        return typeGetJob;	
    }

    template <typename T>  
    void CFThreadPool<T>::DoJobs()
    {
        FUNCTION_BLOCK_TRACE(0);
        CFJobBase<T>* pJob = NULL;
        T param = T(); //需要缺省构造
        GetJobType getJobType = typeStop;
        while(typeGetJob == (getJobType = GetJob(&pJob, &param)))
        {
            try
            {
				INT nJobIndex = pJob->GetJobIndex();
				FTLTRACEEX(tlTrace, TEXT("CFThreadPool Begin Run Job %d\n"), nJobIndex);
                pJob->Run(param); //执行Job，这个函数返回后, pJob 一般已经被 delete this 了, 不能再调用其方法

				FTLTRACEEX(tlTrace, TEXT("CFThreadPool End Run Job %d\n"), nJobIndex);

                //退出Run说明一个Job结束
				{
					//首先从运行列表中删除
					CFAutoLock<CFLockObject> lockerDoing(&m_lockDoingJobs);
					JobInfoContainer::iterator iter = m_DoingJobs.find(nJobIndex);
					FTLASSERT(m_DoingJobs.end() != iter);
					if (m_DoingJobs.end() != iter)
					{
						JobInfo* pInfo = iter->second;
						m_DoingJobs.erase(iter);
						delete pInfo;
					}
				}

				//检查一下是否需要减少线程
                BOOL bNeedSubtractThread = FALSE;
                {
                    CFAutoLock<CFLockObject> locker(&m_lockWaitingJobs);
                    //当队列中没有Job，并且当前线程数大于最小线程数时
                    bNeedSubtractThread = (m_WaitingJobs.empty() && (m_nCurNumThreads > m_nMinNumThreads) && !HadRequestStop());
                    if (bNeedSubtractThread)
                    {
                        //通知减少一个线程
                        ReleaseSemaphore(m_hSemaphoreSubtractThread,1L,NULL);
                    }
                }
            }
            catch (...)
            {
                FTLASSERT(FALSE);
#if 1           //是否需要调用该函数？或者新的 pJob->OnException
                //防止线程中出现未处理的异常造成线程池的错误
                //正常情况下，异常应该由工作函数自己处理
                pJob->OnCancelJob(param);  
#endif
            }
        }
        if (typeSubtractThread == getJobType)  //需要减少线程,应该把自己退出 -- 注意：通知退出的线程和实际退出的线程可能不是同一个
        {
            FUNCTION_BLOCK_NAME_TRACE(TEXT("typeSubtractThread == getJobType,will remove self thread"),
                DEFAULT_BLOCK_TRACE_THRESHOLD);
            CFAutoLock<CFLockObject> locker(&m_lockThreads);
            LONG index = 0;
            DWORD dwCurrentThreadId = GetCurrentThreadId();
            for (; index < m_nCurNumThreads; index++)
            {
                if (m_pThreadIds[index] == dwCurrentThreadId)  //找到自己
                {
                    break;
                }
            }
            FTLASSERT(index < m_nCurNumThreads);
            if (index < m_nCurNumThreads)
            {
                //把最后一个线程的信息移到退出的线程位置 -- 如果退出的线程就是最后一个时也正确
                HANDLE hOldTemp = m_pThreadHandles[index];
                m_pThreadHandles[index] = m_pThreadHandles[m_nCurNumThreads - 1];
                m_pThreadHandles[m_nCurNumThreads-1] = NULL;
                m_pThreadIds[index] = m_pThreadIds[m_nCurNumThreads - 1];
                m_pThreadIds[m_nCurNumThreads - 1] = 0;
                m_nCurNumThreads--;
                CloseHandle(hOldTemp);

                FTLTRACEEX(tlTrace,TEXT("CFThreadPool Subtract a thread, thread id = %d, curThreadNum = %d\n"),
                    dwCurrentThreadId,m_nCurNumThreads);
            }
        }
        else //typeStop
        {
            //Do Nothing
        }
    }

    template <typename T>  
    unsigned int CFThreadPool<T>::ThreadExecute(void *pThis)
    {
        FUNCTION_BLOCK_TRACE(0);
        ((CFThreadPool<T>*)pThis)->DoJobs();
        return(0);
    }

    //////////////////////////////     CFProducerResumerQueue     /////////////////////////////////////////////
    template<typename ELEMENT>
    CFProducerResumerQueue<ELEMENT>::CFProducerResumerQueue(LONG nCapability, 
        LONG nReserveSlot /* = 0 */, 
        HANDLE hEventStop /* = NULL */)
        : m_nCapability(nCapability)
        , m_nReserveSlot(nReserveSlot)
        , m_hEventStop(hEventStop)
    {
        FUNCTION_BLOCK_TRACE(0);
        FTLASSERT(0 <= nReserveSlot);
        FTLASSERT(nReserveSlot <= nCapability);
        
        m_hSemNumElements = ::CreateSemaphore(NULL,0,nCapability ,NULL);
        m_hSemNumSlots = ::CreateSemaphore(NULL,m_nCapability - m_nReserveSlot,nCapability,NULL);

        if (m_hEventStop)  //外界传入的EventStop
        {
            m_bCreateEventStop = FALSE;
        }
        else
        {
            m_bCreateEventStop = TRUE;
            m_hEventStop = ::CreateEvent(NULL,TRUE,FALSE,NULL);
        }
        
        FTLASSERT(m_hSemNumElements);
        FTLASSERT(m_hSemNumSlots);
        FTLASSERT(m_hEventStop);
    }

    template<typename ELEMENT>
    CFProducerResumerQueue<ELEMENT>::~CFProducerResumerQueue()
    {
        FUNCTION_BLOCK_TRACE(0);
#ifdef FTL_DEBUG
        //Stop后可以用RemoveAfterStop进行清理工作？Stop后应该不允许Append&Remove
        FTLASSERT(0 == m_AllElement.size()); //must remove all Element,for avoid resource/memory leak，
        FTLASSERT(WAIT_TIMEOUT == WaitForSingleObject(m_hSemNumElements,0));
#endif
        BOOL bRet = FALSE;
        if (m_bCreateEventStop)
        {
            SAFE_CLOSE_HANDLE(m_hEventStop,NULL);
        }
        SAFE_CLOSE_HANDLE(m_hSemNumElements,NULL);
        SAFE_CLOSE_HANDLE(m_hSemNumSlots,NULL);
    }

    template<typename ELEMENT>
    BOOL CFProducerResumerQueue<ELEMENT>::OnAppendElement(const ELEMENT &element)
    {
        UNREFERENCED_PARAMETER(element);
        return FALSE;
    }

    template<typename ELEMENT>
    BOOL CFProducerResumerQueue<ELEMENT>::Start()
    {
        BOOL bRet = FALSE;
        API_VERIFY(ResetEvent(m_hEventStop));
        return bRet;
    }

    template<typename ELEMENT>
    BOOL CFProducerResumerQueue<ELEMENT>::Stop()
    {
        BOOL bRet = FALSE;
        API_VERIFY(::SetEvent(m_hEventStop));
        return bRet;
    }

    template<typename ELEMENT>
    BOOL CFProducerResumerQueue<ELEMENT>::IsStopped()
    {
        BOOL bRet = FALSE;
        DWORD dwResult = WaitForSingleObject(m_hEventStop,0);
        switch(dwResult)
        {
        case WAIT_OBJECT_0:
            bRet = TRUE;
            break;
        case WAIT_TIMEOUT:
            bRet = FALSE;
            break;
        default:
            FTLASSERT(FALSE);
            bRet = TRUE; // FALSE;发生错误时应该认为已经停止
            break;
        }
        return bRet;
    }


    template<typename ELEMENT>
    FTLThreadWaitType CFProducerResumerQueue<ELEMENT>::Append(const ELEMENT& element, DWORD dwTimeOut)    
    {
        FTLThreadWaitType waitType= ftwtError;
        HANDLE hWaitObjects[2] = 
        {
            m_hEventStop,       
            m_hSemNumSlots,      //看是否有空的Slot可用
        };
        DWORD dwRet=WaitForMultipleObjects(_countof(hWaitObjects),hWaitObjects,FALSE,dwTimeOut);
        switch(dwRet)
        {
        case WAIT_OBJECT_0: //Stop
            FTLTRACEEX(tlWarning,_T("CFProducerResumerQueue::Append Get StopEvent,Must Quit\n"));
            waitType = ftwtStop;
            break;
        case WAIT_OBJECT_0 + 1: //等到了空的Slot
            {
                BOOL bRet = FALSE;
                CFAutoLock<CFLockObject>  locker(&m_lockElements);
                if (!OnAppendElement(element))
                {
                    //如果子类没有调整优先级，则放在Queue的最后
                    m_AllElement.push_back(element);
                }
                API_VERIFY(ReleaseSemaphore(m_hSemNumElements,1L,NULL));    //释放一个Element信号量
                waitType = ftwtContinue;
                break;
            }
        case WAIT_TIMEOUT:
            waitType = ftwtTimeOut;
            break;
        default:
            FTLASSERT(FALSE);   //什么时候会发生？
            waitType = ftwtError;
            break;
        }
        return(waitType);
    }

    //! 消费者端调用,从队列中取出元素
    template<typename ELEMENT>
    FTLThreadWaitType CFProducerResumerQueue<ELEMENT>::Remove(ELEMENT& element, DWORD dwTimeOut)
    {
        FTLThreadWaitType waitType= ftwtError;
        HANDLE waitHandles[2] = 
        {
            m_hEventStop,
            m_hSemNumElements,  //看是否有元素可用
        };
        DWORD dwRet = WaitForMultipleObjects(_countof(waitHandles),waitHandles,FALSE,dwTimeOut);
        switch(dwRet)
        {
        case WAIT_OBJECT_0: //Stop
            FTLTRACEEX(tlWarning,_T("CFProducerResumerQueue::Remove Get StopEvent,Must Quit\n"));
            waitType = ftwtStop;
            break;
        case WAIT_OBJECT_0 + 1: //等到一个元素
            {
                BOOL bRet = FALSE;
                CFAutoLock<CFLockObject> locker(&m_lockElements);
                element = m_AllElement.front();
                m_AllElement.pop_front();
                API_VERIFY(ReleaseSemaphore(m_hSemNumSlots,1L,NULL));       //释放一个Slot
                waitType = ftwtContinue;
            }
            break;
        case WAIT_TIMEOUT:
            waitType = ftwtTimeOut;
            break;
        default:
            FTLASSERT(FALSE);
            waitType = ftwtError;   //什么时候会发生？
            break;
        }
        return(waitType);
    }

    template<typename ELEMENT>
    BOOL CFProducerResumerQueue<ELEMENT>::RemoveAfterStop(ELEMENT& element)
    {
        BOOL bRet = FALSE;
        DWORD dwResult = WaitForSingleObject(m_hSemNumElements,0);
        if (WAIT_OBJECT_0 == dwResult)
        {
            CFAutoLock<CFLockObject> locker(&m_lockElements);
            FTLASSERT(!m_AllElement.empty());
            element = m_AllElement.front();
            m_AllElement.pop_front();
            API_VERIFY(ReleaseSemaphore(m_hSemNumSlots,1L,NULL));       //释放一个Slot
        }
        else
        {
            FTLASSERT(WAIT_TIMEOUT == dwResult);
            //FTLASSERT(m_AllElement.empty()); 如果使用了 RequestRemove的话，这里会有断言
        }
        return bRet;
    }

    template<typename ELEMENT>
    FTLThreadWaitType CFProducerResumerQueue<ELEMENT>::ReserveSlot(LONG nCount,DWORD dwMilliseconds)
    {
        {
            CFAutoLock<CFLockObject> locker(&m_lockSlot);
            FTLASSERT(nCount <= (m_nCapability - m_nReserveSlot) || !TEXT("不能保留超过限制的Slot个数"));
            if (nCount >  (m_nCapability - m_nReserveSlot))
            {
                SetLastError(ERROR_BAD_ARGUMENTS);
                return ftwtError;
            }
        }
        BOOL bRet = FALSE;
        DWORD dwStartTime = GetTickCount();
        DWORD dwOldMilliseconds = dwMilliseconds;   //保留以前的值，用于比较
        FTLThreadWaitType waitType = ftwtContinue;

        //临时调高本线程的优先级 -- 保证能比别的线程先获取到 m_hSemNumSlots 对象（是否能成功？）
        //由于主要是等待函数，因此不会太占用CPU资源
        int nOldPriority = GetThreadPriority(::GetCurrentThread());
        API_VERIFY(::SetThreadPriority(::GetCurrentThread(),THREAD_PRIORITY_HIGHEST));  //THREAD_PRIORITY_ABOVE_NORMAL

        LONG waitCount = 0;
        HANDLE waitHandles[2] = 
        {
            m_hEventStop,
            m_hSemNumSlots,
        };
        for ( ; (waitCount < nCount) && (ftwtContinue == waitType); waitCount++)
        {
            if(INFINITE != dwMilliseconds)
            {
                dwMilliseconds -= (GetTickCount() - dwStartTime);  //如果不是无穷等待，需要减去已经花费的时间，保证尽量在要求的时间内返回
                //FTLASSERT(FALSE); //仔细考虑，是否会出现溢出的问题？出现后如何检查、如何处理？
                if (dwMilliseconds > dwOldMilliseconds)  //如果比传入的时间还大，说明产生了溢出
                {
                    dwMilliseconds = (MAXDWORD - dwMilliseconds);   //是否可行？
                }
            }
            DWORD dwResult = WaitForMultipleObjects(_countof(waitHandles),waitHandles,FALSE,dwMilliseconds);
            switch (dwResult)
            {
            case WAIT_OBJECT_0: //Stop
                waitType = ftwtStop;
                break;
            case WAIT_OBJECT_0 + 1:  //成功等到
                waitType = ftwtContinue;
                break;
            case WAIT_TIMEOUT:
                waitType = ftwtTimeOut;
                break;
            default:
                FTLASSERT(FALSE);
                waitType = ftwtError;
                break;
            }
        }
        if(0 < waitCount && waitCount < nCount) //没有成功获取足够的Slot，应该将已经获取的Slot释放掉
        {
            API_VERIFY(ReleaseSemaphore(m_hSemNumSlots,waitCount,NULL));
        }
        else
        {
            CFAutoLock<CFLockObject> locker(&m_lockSlot);
            m_nReserveSlot += waitCount;    //否则的话，更新保留Slot的值
        }
        ::SetThreadPriority(::GetCurrentThread(),nOldPriority);   //恢复线程的优先级
        return waitType;
    }

    template<typename ELEMENT>
    BOOL CFProducerResumerQueue<ELEMENT>::ReleaseSlot(LONG nCount)
    {
        BOOL bRet = FALSE;
        CFAutoLock<CFLockObject> locker(&m_lockSlot);
        FTLASSERT(nCount <= m_nReserveSlot || !TEXT("不能释放超过保留的Slot个数"));
        if (nCount > m_nReserveSlot)    //超过可以释放的个数
        {
            SetLastError(ERROR_BAD_ARGUMENTS);
            return bRet;
        }
        API_VERIFY(ReleaseSemaphore(m_hSemNumSlots,nCount,NULL));
        if (bRet)
        {
            m_nReserveSlot -= nCount;
        }
        return bRet;
    }

    template<typename ELEMENT>
    LONG CFProducerResumerQueue<ELEMENT>::GetCapability() const
    {
        return m_nCapability;
    }

    template<typename ELEMENT>
    LONG CFProducerResumerQueue<ELEMENT>::GetElementCount() const
    {
        LONG nElementCount = 0;
        {
            CFAutoLock<CFLockObject> locker(&m_lockElements);
            nElementCount = m_AllElement.size();
        }
        return nElementCount;
    }
    template<typename ELEMENT>
    LONG CFProducerResumerQueue<ELEMENT>::GetReserveSlotCount() const
    {
        LONG nReserveSlotCount = 0;
        {
            CFAutoLock<CFLockObject> locker(&m_lockSlot);
            nReserveSlotCount = m_nReserveSlot;
        }
        return nReserveSlotCount;
    }

    template<typename ELEMENT>
    BOOL CFProducerResumerQueue<ELEMENT>::RequestAppend(DWORD dwTimeOut,const ELEMENT& element)
    {
        BOOL bRet = FALSE;
        HANDLE hWaitObjects[2] = 
        {
            m_hEventStop,
            m_hSemNumSlots,  //看是否有Slot可用
        };
        DWORD dwResult = WaitForMultipleObjects(_countof(hWaitObjects),hWaitObjects,FALSE,dwTimeOut);
        switch(dwResult)
        {
        case WAIT_OBJECT_0://Stop
            FTLTRACEEX(tlWarning,_T("CFProducerResumerQueue::RequestAppend Get StopEvent,Must Quit\n"));
            bRet = FALSE;
            break;
        case WAIT_OBJECT_0 + 1: //Get Slot
            {
                CFAutoLock<CFLockObject> locker(&m_lockElements);
                if(!OnAppendElement(element))
                {
                    m_AllElement.push_back(element);
                }
            }
            bRet = TRUE;
            break;
        case WAIT_TIMEOUT:
            bRet = FALSE;
            break;
        default:
            FTLASSERT(FALSE);
            bRet = FALSE;
            break;
        }
        return (bRet);
    }

    template<typename ELEMENT>
    BOOL CFProducerResumerQueue<ELEMENT>::CommitAppend()
    {
        BOOL bRet = FALSE;
        CFAutoLock<CFLockObject> locker(&m_lockElements);
        API_VERIFY(ReleaseSemaphore(m_hSemNumElements,1L,NULL));
        return bRet;
    }

    template<typename ELEMENT>
    BOOL CFProducerResumerQueue<ELEMENT>::RequestRemove(DWORD dwTimeOut,ELEMENT& element)
    {
        BOOL bRet = FALSE;
        HANDLE waitHandles[2] = 
        {
            m_hEventStop,
            m_hSemNumElements
        };
        DWORD dwResult = WaitForMultipleObjects(_countof(waitHandles),waitHandles,FALSE,dwTimeOut);
        switch(dwResult)
        {
        case WAIT_OBJECT_0:  //Stop
            FTLTRACEEX(tlWarning,_T("CFProducerResumerQueue::RequestRemove Get StopEvent,Must Quit\n"));
            bRet = FALSE;
            break;
        case WAIT_OBJECT_0 + 1: //Get Element
            {
                bRet = TRUE;
                CFAutoLock<CFLockObject> locker(&m_lockElements);
                FTLASSERT(m_AllElement.size() > 0);
                element = m_AllElement.front();
            }
            break;
        case WAIT_TIMEOUT:
            bRet = FALSE;
            break;
        default:
            FTLASSERT(FALSE);
            bRet = FALSE;
            break;
        }
        return (bRet);
    }

    template<typename ELEMENT>
    BOOL CFProducerResumerQueue<ELEMENT>::CommitRemove()
    {
        BOOL bRet = TRUE;
        CFAutoLock<CFLockObject> locker(&m_lockElements);
        m_AllElement.pop_front();
        API_VERIFY(ReleaseSemaphore(m_hSemNumSlots,1L,NULL));
        return bRet;
    }

    template<typename ELEMENT>
    CFAutoQueueOperator<ELEMENT>::CFAutoQueueOperator(
        CFProducerResumerQueue<ELEMENT>* pQueue,
        ELEMENT &refOperationElement, 
        BOOL bIsQuestAppend/* = TRUE*/,
        DWORD dwTimeOut/* = INFINITE*/)
        :m_refOperationElement(refOperationElement)
    {
        FTLASSERT(pQueue);
        m_pQueue = pQueue;
        m_bIsQuestAppend = bIsQuestAppend;
        if (bIsQuestAppend)
        {
            m_bHaveGotQueue = m_pQueue->RequestAppend(dwTimeOut,m_refOperationElement);
        }
        else
        {
            m_bHaveGotQueue = m_pQueue->RequestRemove(dwTimeOut,m_refOperationElement);
        }
    }
    template<typename ELEMENT>
    CFAutoQueueOperator<ELEMENT>::~CFAutoQueueOperator()
    {
        if (m_bHaveGotQueue)
        {
            if (m_bIsQuestAppend)
            {
                m_pQueue->CommitAppend();
            }
            else
            {
                m_pQueue->CommitRemove();
            }
        }
    }
    template<typename ELEMENT>
    BOOL CFAutoQueueOperator<ELEMENT>::HaveGotQueue() const
    {
        return m_bHaveGotQueue;
    }

    //////////////////////////////////////////////////////////////////////////

    //100%，全部清除
    #define MAX_CLEAR_PERCENT       (100)

    template<typename KEY, typename VALUE,typename CHILDCLASS>
    CFItemCacheT<KEY,VALUE,CHILDCLASS>::CFItemCacheT(DWORD nMaxCacheSize,DWORD clearPercent /* = 30 */)
    {
        Init(nMaxCacheSize,clearPercent);
    }

    template<typename KEY, typename VALUE,typename CHILDCLASS>
    void CFItemCacheT<KEY,VALUE,CHILDCLASS>::Init(DWORD nMaxCacheSize,DWORD clearPercent)
    {
        CFAutoLock<CFLockObject> locker(&m_LockObject);
        m_MaxCacheSize = nMaxCacheSize;
        m_ClearPercent = clearPercent;
        m_ClearMode = clearByLRU;			
        m_CurrentIndex = 0;
    }

    //template<typename KEY, typename VALUE,typename CHILDCLASS>
    //void CFItemCacheT<KEY,VALUE,CHILDCLASS>::SetMaxCacheSize(DWORD nMaxCacheSize)
    //{
    //    CFAutoLock<CFLockObject> locker(&m_LockObject);
    //    if (m_MaxCacheSize != nMaxCacheSize)
    //    {
    //        if (m_MaxCacheSize > nMaxCacheSize)  //smaller(will clear)
    //        {
    //            m_MaxCacheSize = nMaxCacheSize;
    //            //clear some out of range data
    //        }
    //        else
    //        {
    //            m_MaxCacheSize = nMaxCacheSize;
    //        }

    //    }
    //    m_MaxCacheSize = nMaxCacheSize;
    //    m_CurrentIndex = 0;
    //    m_ClearMode = clearByLRU;			
    //}


    template<typename KEY, typename VALUE,typename CHILDCLASS>
    CFItemCacheT<KEY,VALUE,CHILDCLASS>::~CFItemCacheT()
    {
        ClearCache(MAX_CLEAR_PERCENT); //// 析构时必须调用 ClearCache 函数，否则可能内存泄漏等问题。
    }

    template<typename KEY, typename VALUE,typename CHILDCLASS>
    BOOL CFItemCacheT<KEY,VALUE,CHILDCLASS>::GetItemByKey(const KEY key,VALUE& value)
    {
        BOOL bRet = FALSE;
        CFAutoLock<CFLockObject> locker(&m_LockObject);
        CACHE_MAP::iterator iter = m_ItemCacheMaps.find(key);
        if (iter != m_ItemCacheMaps.end()) // find
        {
            value =  iter->second->value;
            API_VERIFY(::QueryPerformanceCounter(&iter->second->lastAccess));
            return TRUE;
        }

        // not find ,will get RealItem and insert to Item cache
        if (m_ItemCacheMaps.size() >= m_MaxCacheSize)
        {
            ClearCache(m_ClearPercent);
        }

        BOOL bGot = FALSE;
        {
            CHILDCLASS* pThis = static_cast<CHILDCLASS*>(this);
            KEY lookupKey = key;
            iter = m_ItemCacheMaps.find(lookupKey);
            if (m_ItemCacheMaps.end() == iter) //if not found, then GetRealItem
            {
                bGot = pThis->GetRealItem(lookupKey,value);
                if (bGot)
                {
                    CItemData *pNewItem = new CItemData;
                    pNewItem->key = lookupKey;
                    pNewItem->value = value;
                    API_VERIFY(::QueryPerformanceCounter(&pNewItem->lastAccess));
                    pNewItem->index = m_CurrentIndex++;
                    m_ItemCacheMaps.insert(CACHE_MAP::value_type(lookupKey,pNewItem));
                }
            }
        }
        return bGot;
    }

    template<typename KEY, typename VALUE,typename CHILDCLASS>
    void CFItemCacheT<KEY,VALUE,CHILDCLASS>::SetClearMode(ClearMode mode)
    {
        CFAutoLock<CFLockObject> locker(&m_LockObject);
        m_ClearMode = mode;
    }

    template<typename KEY, typename VALUE,typename CHILDCLASS>
    bool CFItemCacheT<KEY,VALUE,CHILDCLASS>::LastAccessGreater(CItemData* p, CItemData* q)
    {
        return p->lastAccess.QuadPart > q->lastAccess.QuadPart;
        //return p->lastAccess > q->lastAccess;
    }

    template<typename KEY, typename VALUE,typename CHILDCLASS>
    bool CFItemCacheT<KEY,VALUE,CHILDCLASS>::LastAccessSmaller(CItemData* p, CItemData* q)
    {
        return p->lastAccess.QuadPart < q->lastAccess.QuadPart;
        //return p->lastAccess < q->lastAccess;
    }

    template<typename KEY, typename VALUE,typename CHILDCLASS>
    bool CFItemCacheT<KEY,VALUE,CHILDCLASS>::IndexGreater(CItemData* p, CItemData* q)
    {
        return p->index > q->index;
    }

    template<typename KEY, typename VALUE,typename CHILDCLASS>
    bool CFItemCacheT<KEY,VALUE,CHILDCLASS>::IndexSmaller(CItemData* p, CItemData* q)
    {
        return p->index < q->index;
    }


    template<typename KEY, typename VALUE,typename CHILDCLASS>
    void CFItemCacheT<KEY,VALUE,CHILDCLASS>::ClearCache(DWORD percent)
    {
        //TODO: 使用 Effective STL 中看到的方法优化
        CFAutoLock<CFLockObject> locker(&m_LockObject);
        CHILDCLASS* pThis = static_cast<CHILDCLASS*>(this);
        CACHE_MAP::iterator iterMap;
        if (MAX_CLEAR_PERCENT == percent)  //如果是全部清除的话(通常是析构中)
        {
            for (iterMap = m_ItemCacheMaps.begin();iterMap != m_ItemCacheMaps.end();iterMap++)
            {
                CItemData* pTempItemData = iterMap->second;
                _ASSERT(pTempItemData!= NULL);
                pThis->FreeItem(pTempItemData->key,pTempItemData->value);
                delete pTempItemData;
                pTempItemData = NULL;
            }
            m_ItemCacheMaps.clear();
            return;
        }
        DWORD dwClearCount = m_MaxCacheSize * percent / MAX_CLEAR_PERCENT;

        if(m_ClearMode != clearByRandom)
        {
            std::vector<CItemData*> valVector;
            for (iterMap = m_ItemCacheMaps.begin();iterMap != m_ItemCacheMaps.end(); iterMap++)
            {
                valVector.push_back(iterMap->second);
            }

            switch(m_ClearMode)
            {
            case clearByLRU:
                sort(valVector.begin(),valVector.end(),LastAccessSmaller); // time up
                break;
            case clearByFIFO:
                sort(valVector.begin(),valVector.end(),IndexSmaller); //index up
                break;
                //case clearByMRU:
                //	break;
            default:
                _ASSERT(FALSE);
                break;
            }

            m_ItemCacheMaps.clear();

            for (DWORD dwIndex = 0; dwIndex < dwClearCount; dwIndex++)
            {
                CItemData* pTempItemData = valVector.at(dwIndex);
                _ASSERT(pTempItemData!= NULL);
                pThis->FreeItem(pTempItemData->key,pTempItemData->value);
                delete pTempItemData;
                pTempItemData = NULL;
            }
            for (DWORD dwIndex = dwClearCount; dwIndex < valVector.size(); dwIndex++)
            {
                CItemData *pTempItemData = valVector.at(dwIndex);
                KEY key = pTempItemData->key;
                m_ItemCacheMaps.insert(CACHE_MAP::value_type(key,pTempItemData));
            }
            valVector.clear();
            //   partial_sort_copy(m_ItemCacheMaps.begin(),m_ItemCacheMaps.end(),testVector.begin(),testVector.end());
        }
        else  //随机清除，不排序
        {
            DWORD dwCount = 0;
            iterMap = m_ItemCacheMaps.begin();
            while (dwCount < dwClearCount)
            {
                CItemData* pTempItemData = iterMap->second;
                _ASSERT(pTempItemData!= NULL);
                pThis->FreeItem(pTempItemData->key,pTempItemData->value);
                delete pTempItemData;
                pTempItemData = NULL;
                iterMap = m_ItemCacheMaps.erase(iterMap);
                dwCount++;
            }
        }
    }

    template < typename T >
    CFMemCacheT<T>::CFMemCacheT( INT nInitSize /* = 0  */, INT nMaxSize /* = MAXINT */ )
    {
        FTLASSERT( 0 <= nInitSize );
        FTLASSERT( nInitSize <= nMaxSize );

        m_nCurCount = 0;
        m_nMaxSize = nMaxSize;
        for (int i = 0; i < nInitSize; i++) 
        {
            T *t = new T;
            if (t != NULL) 
            {
                m_nCurCount++;
                m_freeBlocks.push_back( t );
                m_allBlocks.push_back( t );
            }
        }
    }

    template < typename T >
    CFMemCacheT<T>::~CFMemCacheT()
    {
        CFAutoLock<CFLockObject> lock(&m_lockObject);
        for ( std::list<T*>::iterator iter = m_allBlocks.begin();
            iter != m_allBlocks.end(); ++iter) 
        {
            T* t = *iter;
            delete t;
        }
        m_nCurCount = 0;
        m_allBlocks.clear();
        m_freeBlocks.clear();
    }

    template < typename T >
    T* CFMemCacheT<T>::Get()
    {
        T* t = NULL;
        CFAutoLock<CFLockObject> lock(&m_lockObject);
        if (!m_freeBlocks.empty())
        {   
            t = m_freeBlocks.front();
            m_freeBlocks.pop_front();
        }

        if (NULL == t && (m_nCurCount < m_nMaxSize))
        {
            t = new T();
            if (NULL != t)
            {
                m_nCurCount++;
                m_allBlocks.push_back(t);
            }
        }
        return t;
    }

    template < typename T >
    void CFMemCacheT<T>::Release( T* t )
    {
        FTLASSERT(NULL != t);
        if (NULL != t)
        {
            CFAutoLock<CFLockObject> lock(&m_lockObject);
            m_freeBlocks.push_front(t);
        }
    }


}//namespace FTL

#endif //FTL_THREAD_HPP

