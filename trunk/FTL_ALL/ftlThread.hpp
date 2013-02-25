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
        HANDLE  m_hNotifyObject;  //����֪ͨ�Ķ���ԭ��û��(������Ϊ�ȴ��̵߳ľ��)��ʹ���̳߳ؼ���ʱ��Ҫ��Ϊ���ص� Event ��
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
        //ʹ�� InitializeCriticalSectionAndSpinCount �����������
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
        FTLASSERT(m_lockCount == 0); // ����ǰ������ȫ�ͷţ���������������
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
        FTLASSERT(m_lockCount == 0); // ����ǰ������ȫ�ͷţ��������������������̳߳���
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

    //�����ڵȴ�ʱ���� SEND ���͵���Ϣ(��DirectShow�п����Ĵ���) --
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
            { //���յ���Ϣ
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
                //���ڳ�ʱ���û�ֹͣ�������ٵȴ���ȡ������֮ǰ���õ�ֵ
                CFAutoLock<CFLockObject> lock(m_pLockObject);
                m_nWaitingReaders--;
            }
        }
        else
        {
            //����û��Ƿ�ֹͣ��
            DWORD dwRet = WaitForSingleObject(m_hStopEvent,0);
            if (WAIT_OBJECT_0 == dwRet) //�û�ֹͣ�ˣ��������ٶ�ȡ
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
                //���ڳ�ʱ���û�ֹͣ�������ٵȴ�д�룬����֮ǰ���õ�ֵ
                CFAutoLock<CFLockObject> lock(m_pLockObject);
                m_nWaitingWriters--;
            }
        }
        else
        {
            //����û��Ƿ�ֹͣ��
            DWORD dwRet = WaitForSingleObject(m_hStopEvent,0);
            if (WAIT_OBJECT_0 == dwRet) //�û�ֹͣ�ˣ���������д��
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

    //���Բ�����???�����߳����ֵ��ַ������߳������ڼ����һֱ����(�����ǳ�����ȫ�ֱ�������ڶ��� )
    BOOL CFThreadUtils::SetThreadName( DWORD dwThreadID, LPTSTR szThreadName)
    {
        BOOL fOkay = TRUE;
        //char** ppszThreadName = NULL;
        //__asm                            //��λ�����̵߳� TIB �ṹ ����TIB�ṹ�е�0x14ƫ�ƴ�  &pTIB->pvArbitrary ��
        //{
        //	mov eax,fs:[0x18]    ;
        //	add eax,0x14        ;
        //	mov [ppszThreadName],eax;        
        //}
        //if (*ppszThreadName == NULL)    //�������( pvArbitrary )û�����ù�,
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
            RaiseException(0x406d1388, 0, sizeof(tni) / sizeof(DWORD),(DWORD*)(&tni));      //���쳣֪ͨ���������������ַ������ַ����ָ�����쳣
        }__except(EXCEPTION_EXECUTE_HANDLER)
        {
            fOkay = FALSE;
        }
#ifdef _UNICODE
        HeapFree ( GetProcessHeap ( ) , 0 , szAnsiName ) ;
#endif

        return fOkay;
    }

    //��ȡ�߳����֣������ڴ�ӡ���ٴ���
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
        //��XpCommonLib��ѧ����������ʲô��
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
        //�ж��߳��Ƿ����
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

        if (m_hEventStop)  //��紫���EventStop
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
        //Stop�������RemoveAfterStop������������Stop��Ӧ�ò�����Append&Remove
        FTLASSERT(0 == m_AllElement.size()); //must remove all Element,for avoid resource/memory leak��
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
            bRet = TRUE; // FALSE;��������ʱӦ����Ϊ�Ѿ�ֹͣ
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
            m_hSemNumSlots,      //���Ƿ��пյ�Slot����
        };
        DWORD dwRet=WaitForMultipleObjects(_countof(hWaitObjects),hWaitObjects,FALSE,dwTimeOut);
        switch(dwRet)
        {
        case WAIT_OBJECT_0: //Stop
            FTLTRACEEX(tlWarning,_T("CFProducerResumerQueue::Append Get StopEvent,Must Quit\n"));
            waitType = ftwtStop;
            break;
        case WAIT_OBJECT_0 + 1: //�ȵ��˿յ�Slot
            {
                BOOL bRet = FALSE;
                CFAutoLock<CFLockObject>  locker(&m_lockElements);
                if (!OnAppendElement(element))
                {
                    //�������û�е������ȼ��������Queue�����
                    m_AllElement.push_back(element);
                }
                API_VERIFY(ReleaseSemaphore(m_hSemNumElements,1L,NULL));    //�ͷ�һ��Element�ź���
                waitType = ftwtContinue;
                break;
            }
        case WAIT_TIMEOUT:
            waitType = ftwtTimeOut;
            break;
        default:
            FTLASSERT(FALSE);   //ʲôʱ��ᷢ����
            waitType = ftwtError;
            break;
        }
        return(waitType);
    }

    //! �����߶˵���,�Ӷ�����ȡ��Ԫ��
    template<typename ELEMENT>
    FTLThreadWaitType CFProducerResumerQueue<ELEMENT>::Remove(ELEMENT& element, DWORD dwTimeOut)
    {
        FTLThreadWaitType waitType= ftwtError;
        HANDLE waitHandles[2] = 
        {
            m_hEventStop,
            m_hSemNumElements,  //���Ƿ���Ԫ�ؿ���
        };
        DWORD dwRet = WaitForMultipleObjects(_countof(waitHandles),waitHandles,FALSE,dwTimeOut);
        switch(dwRet)
        {
        case WAIT_OBJECT_0: //Stop
            FTLTRACEEX(tlWarning,_T("CFProducerResumerQueue::Remove Get StopEvent,Must Quit\n"));
            waitType = ftwtStop;
            break;
        case WAIT_OBJECT_0 + 1: //�ȵ�һ��Ԫ��
            {
                BOOL bRet = FALSE;
                CFAutoLock<CFLockObject> locker(&m_lockElements);
                element = m_AllElement.front();
                m_AllElement.pop_front();
                API_VERIFY(ReleaseSemaphore(m_hSemNumSlots,1L,NULL));       //�ͷ�һ��Slot
                waitType = ftwtContinue;
            }
            break;
        case WAIT_TIMEOUT:
            waitType = ftwtTimeOut;
            break;
        default:
            FTLASSERT(FALSE);
            waitType = ftwtError;   //ʲôʱ��ᷢ����
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
            API_VERIFY(ReleaseSemaphore(m_hSemNumSlots,1L,NULL));       //�ͷ�һ��Slot
        }
        else
        {
            FTLASSERT(WAIT_TIMEOUT == dwResult);
            //FTLASSERT(m_AllElement.empty()); ���ʹ���� RequestRemove�Ļ���������ж���
        }
        return bRet;
    }

    template<typename ELEMENT>
    FTLThreadWaitType CFProducerResumerQueue<ELEMENT>::ReserveSlot(LONG nCount,DWORD dwMilliseconds)
    {
        {
            CFAutoLock<CFLockObject> locker(&m_lockSlot);
            FTLASSERT(nCount <= (m_nCapability - m_nReserveSlot) || !TEXT("���ܱ����������Ƶ�Slot����"));
            if (nCount >  (m_nCapability - m_nReserveSlot))
            {
                SetLastError(ERROR_BAD_ARGUMENTS);
                return ftwtError;
            }
        }
        BOOL bRet = FALSE;
        DWORD dwStartTime = GetTickCount();
        DWORD dwOldMilliseconds = dwMilliseconds;   //������ǰ��ֵ�����ڱȽ�
        FTLThreadWaitType waitType = ftwtContinue;

        //��ʱ���߱��̵߳����ȼ� -- ��֤�ܱȱ���߳��Ȼ�ȡ�� m_hSemNumSlots �����Ƿ��ܳɹ�����
        //������Ҫ�ǵȴ���������˲���̫ռ��CPU��Դ
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
                dwMilliseconds -= (GetTickCount() - dwStartTime);  //�����������ȴ�����Ҫ��ȥ�Ѿ����ѵ�ʱ�䣬��֤������Ҫ���ʱ���ڷ���
                //FTLASSERT(FALSE); //��ϸ���ǣ��Ƿ�������������⣿���ֺ���μ�顢��δ���
                if (dwMilliseconds > dwOldMilliseconds)  //����ȴ����ʱ�仹��˵�����������
                {
                    dwMilliseconds = (MAXDWORD - dwMilliseconds);   //�Ƿ���У�
                }
            }
            DWORD dwResult = WaitForMultipleObjects(_countof(waitHandles),waitHandles,FALSE,dwMilliseconds);
            switch (dwResult)
            {
            case WAIT_OBJECT_0: //Stop
                waitType = ftwtStop;
                break;
            case WAIT_OBJECT_0 + 1:  //�ɹ��ȵ�
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
        if(0 < waitCount && waitCount < nCount) //û�гɹ���ȡ�㹻��Slot��Ӧ�ý��Ѿ���ȡ��Slot�ͷŵ�
        {
            API_VERIFY(ReleaseSemaphore(m_hSemNumSlots,waitCount,NULL));
        }
        else
        {
            CFAutoLock<CFLockObject> locker(&m_lockSlot);
            m_nReserveSlot += waitCount;    //����Ļ������±���Slot��ֵ
        }
        ::SetThreadPriority(::GetCurrentThread(),nOldPriority);   //�ָ��̵߳����ȼ�
        return waitType;
    }

    template<typename ELEMENT>
    BOOL CFProducerResumerQueue<ELEMENT>::ReleaseSlot(LONG nCount)
    {
        BOOL bRet = FALSE;
        CFAutoLock<CFLockObject> locker(&m_lockSlot);
        FTLASSERT(nCount <= m_nReserveSlot || !TEXT("�����ͷų���������Slot����"));
        if (nCount > m_nReserveSlot)    //���������ͷŵĸ���
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
            m_hSemNumSlots,  //���Ƿ���Slot����
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

    //100%��ȫ�����
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
        ClearCache(MAX_CLEAR_PERCENT); //// ����ʱ������� ClearCache ��������������ڴ�й©�����⡣
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
        //TODO: ʹ�� Effective STL �п����ķ����Ż�
        CFAutoLock<CFLockObject> locker(&m_LockObject);
        CHILDCLASS* pThis = static_cast<CHILDCLASS*>(this);
        CACHE_MAP::iterator iterMap;
        if (MAX_CLEAR_PERCENT == percent)  //�����ȫ������Ļ�(ͨ����������)
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
        else  //��������������
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

