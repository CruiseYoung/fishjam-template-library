#ifndef FTL_IOCP_HPP
#define FTL_IOCP_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlIocp.h"
#endif

namespace FTL
{
    CFIocpBuffer::CFIocpBuffer()
    {
        m_dwSize = 0;
        m_pBuffer = NULL;
        m_operType = otInitialize;
        //m_nBufferIndex = -1;

        Reset(0);
    }

    CFIocpBuffer::~CFIocpBuffer()
    {
        SAFE_DELETE_ARRAY(m_pBuffer);
    }

    VOID CFIocpBuffer::Reset(DWORD dwSize)
    {
        ZeroMemory(&m_overLapped, sizeof(m_overLapped));
        m_nSequenceNumber = 0;
        m_hIoHandle = NULL;
        if (m_dwSize != dwSize)
        {
            SAFE_DELETE_ARRAY(m_pBuffer);
            m_dwSize = dwSize;
        }

        if (dwSize > 0)
        {
            if (!m_pBuffer)
            {
                m_pBuffer = new BYTE[dwSize];
            }
            if (m_pBuffer)
            {
                ZeroMemory(m_pBuffer, dwSize);
            }
        }

        m_wsaBuf.buf = (CHAR*)m_pBuffer;
        m_wsaBuf.len = dwSize;
    }

    CFIocpBaseTask::CFIocpBaseTask(CFIocpMgr* pIocpMgr)
    {
        m_pIocpMgr = pIocpMgr;
        //m_nCurReadSequence = 0;
        //m_nCurWriteSequence = 0;
    }
    CFIocpBaseTask::~CFIocpBaseTask()
    {
        for (IocpBufferContainer::iterator iter = m_IoBuffers.begin();
            iter != m_IoBuffers.end();
            ++iter)
        {
            CFIocpBuffer* pBuffer = *iter;
            m_pIocpMgr->m_IocpBufferPool.Retrun(pBuffer);
        }
        m_IoBuffers.clear();
    }

    VOID CFIocpBaseTask::AddBuffer(CFIocpBuffer* pIoBuffer)
    {
        IocpBufferContainer::iterator iter = m_IoBuffers.find(pIoBuffer);
        FTLASSERT(iter == m_IoBuffers.end());
        //FTLASSERT(pIoBuffer->m_nBufferIndex);
        m_IoBuffers.insert(pIoBuffer);
    }

    BOOL CFIocpBaseTask::RemoveBuffer(CFIocpBuffer* pIoBuffer)
    {
        BOOL bFound = FALSE;
        IocpBufferContainer::iterator iter = m_IoBuffers.find(pIoBuffer);
        if (iter != m_IoBuffers.end())
        {
            m_IoBuffers.erase(iter);
            bFound = TRUE;
        }
        return bFound;
    }

    //////////////////////////////////////////////////////////////////////////
    CFIocpMgr::CFIocpMgr(DWORD dwBufferSize)
        :m_dwBufferSize(dwBufferSize)
        ,m_IocpBufferPool(0, INT_MAX)
    {
        m_hIoCompletionPort = NULL;
        m_pWorkThreads = NULL;
        m_bStop = FALSE;

        m_nConcurrentThreadCount = 0;
    }
    CFIocpMgr::~CFIocpMgr()
    {
        Close();
    }

    BOOL CFIocpMgr::_InitializeIOCP(DWORD NumberOfConcurrentThreads)
    {
        BOOL bRet = FALSE;
        DWORD dwThreadCount = NumberOfConcurrentThreads;
        if (DEFAULT_THREAD_COUNT == dwThreadCount)
        {
            dwThreadCount = _GetDefaultConcurrentThreadCount();
        }

        //1.����һ����ɶ˿ڵ��ں˶���
        m_hIoCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, (ULONG_PTR)NULL, 
            0); //0 �����ж��ٸ���������������ͬʱ���ٸ��߳�����
        //dwThreadCount); //��������ͬʱִ�е�����߳���
        API_VERIFY(NULL != m_hIoCompletionPort);

        if (bRet)
        {
            UINT nThreadID = 0;
            m_pWorkThreads = new HANDLE[dwThreadCount];
            FTLASSERT(m_pWorkThreads);

            if (m_pWorkThreads)
            {
                m_nConcurrentThreadCount = dwThreadCount;

                for( DWORD i = 0; i < dwThreadCount; i ++ )
                {
                    //����ͣ��״̬�����߳�״̬
                    m_pWorkThreads[i] = (HANDLE)_beginthreadex(NULL, 0, 
                        (unsigned int (__stdcall *)(void *))
                        _WorkThreadProc, this , CREATE_SUSPENDED, (UINT*)&nThreadID);

                    FTLASSERT(m_pWorkThreads[i]);
                    if (m_pWorkThreads[i])
                    {
                        ::ResumeThread(m_pWorkThreads[i]);//�����߳�
                    }
                }
            }
        }
        return bRet;
    }
    BOOL CFIocpMgr::Start(DWORD NumberOfConcurrentThreads)
    {
        BOOL bRet = FALSE;
        m_bStop = FALSE;
        FTLASSERT(NULL == m_hIoCompletionPort);
        FTLASSERT(NULL == m_pWorkThreads);

        API_VERIFY(_InitializeIOCP(NumberOfConcurrentThreads));
        if (bRet)
        {
            bRet = OnIocpStart();
        }
        
        return bRet;
    }

    BOOL CFIocpMgr::Stop()
    {
        BOOL bRet = TRUE;
        if (!m_bStop)
        {
            m_bStop = TRUE;
            OnIocpStop();

            for (DWORD dwIndex = 0; dwIndex < m_nConcurrentThreadCount; ++dwIndex)
            {
                API_VERIFY(PostQueuedCompletionStatus(m_hIoCompletionPort, 0, NULL, NULL));
            }
        }
        return bRet;
    }
    VOID CFIocpMgr::Close(DWORD dwMilliseconds)
    {
        for (IocpTaskContainer::iterator iter = m_allIocpTasks.begin();
            iter != m_allIocpTasks.end(); ++iter)
        {
            CFIocpBaseTask* pTask = *iter;
            delete pTask;
        }
        m_allIocpTasks.clear();

        if (m_pWorkThreads)
        {
#if 0
            DWORD WaitTimes = 0;
            for (DWORD dwWaitIndex = 0; dwWaitIndex < m_nConcurrentThreadCount; dwWaitIndex+= MAXIMUM_WAIT_OBJECTS)
            {
                DWORD dwStartIndex = dwWaitIndex + dwWaitCount * MAXIMUM_WAIT_OBJECTS;
                DWORD dwWaitCount = (m_nConcurrentThreadCount - dwStartIndex) % MAXIMUM_WAIT_OBJECTS;
                WaitForMultipleObjects(dwWaitCount, &m_pWorkThreads[dwStartIndex], TRUE, INFINITE);
                for (DWORD dwCloseIndex = dwStartIndex; dwCloseIndex < dwCloseIndex + dwWaitCount; dwCloseIndex ++)
                {
                    CloseHandle(m_pWorkThreads[dwCloseIndex]);
                }
            }
#else
            FTLASSERT(m_nConcurrentThreadCount <= MAXIMUM_WAIT_OBJECTS);
            WaitForMultipleObjects(m_nConcurrentThreadCount, m_pWorkThreads, TRUE, dwMilliseconds);
            //for (DWORD dwWaitIndex = 0; dwWaitIndex < m_nConcurrentThreadCount; ++dwWaitIndex)
            //{
            //    WaitForSingleObject(m_pWorkThreads[dwWaitIndex], dwMilliseconds);
            //    CloseHandle(m_pWorkThreads[dwWaitIndex]);
            //}
#endif 
            SAFE_DELETE_ARRAY(m_pWorkThreads);
        }

        if (m_hIoCompletionPort)
        {
            CloseHandle(m_hIoCompletionPort);
            m_hIoCompletionPort = NULL;
        }
    }
    DWORD CFIocpMgr::_GetDefaultConcurrentThreadCount()
    {
        SYSTEM_INFO sysInfo = {0};
        GetSystemInfo(&sysInfo);

        //#pragma TODO(���̻߳���ʱ��ȡ���̸߳���)
        DWORD numberOfConcurrentThreads = sysInfo.dwNumberOfProcessors * 2 + 2;

        return numberOfConcurrentThreads;
    }

    BOOL CFIocpMgr::AssociateTask(CFIocpBaseTask* pTask, BOOL bPostFirstInit)
    {
        BOOL bRet = TRUE;
        //��һ���ص�IO��ʽ�Ķ������󶨵��Ѵ����õ���ɶ˿ڶ�����
        HANDLE hIoPort = NULL;
        INT nHandleCount = pTask->GetIocpHandleCount();
        for(INT iIndex = 0; iIndex < nHandleCount && bRet; iIndex++)
        {
            API_VERIFY((hIoPort = CreateIoCompletionPort(pTask->GetIocpHandle(iIndex), 
                m_hIoCompletionPort, (ULONG_PTR)pTask, 0)) != NULL );
            FTLASSERT(hIoPort == m_hIoCompletionPort);
        }
        if (bRet && bPostFirstInit)
        {
            CFIocpBuffer* pTaskBuffer = m_IocpBufferPool.Get();
            FTLASSERT(pTaskBuffer);
            if (pTaskBuffer)
            {
                pTaskBuffer->Reset(m_dwBufferSize);
                pTaskBuffer->m_operType = otInitialize;
                pTask->AddBuffer(pTaskBuffer);
                bRet = PostQueuedCompletionStatus(m_hIoCompletionPort, 0, (ULONG_PTR)pTask, &pTaskBuffer->m_overLapped);
            }
        }
        if (bRet)
        {
            m_allIocpTasks.insert(pTask);
        }
        return bRet;
    }

    BOOL CFIocpMgr::DissociateTask(CFIocpBaseTask* pTask)
    {
        BOOL bFound = FALSE;
        IocpTaskContainer::iterator iter = m_allIocpTasks.find(pTask);
        if (iter != m_allIocpTasks.end())
        {
            m_allIocpTasks.erase(iter);
            bFound = TRUE;
        }
        return bFound;
    }

    BOOL CFIocpMgr::OnWorkThreadInit()
    {
        //CoInitialize(NULL);
        return TRUE;
    }

    VOID CFIocpMgr::OnWorkThreadFina()
    {
        //CoUninitialize();
    }

    DWORD CFIocpMgr::_WorkThreadProc(LPVOID pParam)
    {
        DWORD dwResult = 0;
        CFIocpMgr* pThis = reinterpret_cast<CFIocpMgr*>(pParam);
        BOOL bRet = pThis->OnWorkThreadInit();
        if (bRet)
        {
            dwResult = pThis->_InnerWorkThreadProc();
            pThis->OnWorkThreadFina();
        }
        
        return dwResult;
    }

    DWORD CFIocpMgr::_InnerWorkThreadProc()
    {
        BOOL bRet = FALSE;
        DWORD dwResult = ERROR_SUCCESS;

        DWORD dwBytesTransferred = 0;
        //ULONG_PTR nCompletionKey = NULL;
        LPOVERLAPPED lpOverlapped = NULL;
        CFIocpBuffer* pFIocpBuffer = NULL;
        CFIocpBaseTask* pIocpTask = NULL;

        //���Ĺ�����ѭ������ GetQueuedCompletionStatus�������̻߳������ȴ�"���Ŷӵ����״̬"
        while(!m_bStop)
        {
            pIocpTask = NULL;
            pFIocpBuffer = NULL;

            API_VERIFY_EXCEPT1(GetQueuedCompletionStatus(
                m_hIoCompletionPort,        //��ɶ˿��ں˶���ľ��
                &dwBytesTransferred,        //����IO����ʵ�ʴ�����߽����˶��ٸ��ֽ�,������У�������շ�������
                (PULONG_PTR)&pIocpTask,            //���ذ󶨵����ʱ���Զ�������
                &lpOverlapped, 
                INFINITE), 
                ERROR_HANDLE_EOF);

            if (NULL == lpOverlapped)
            {
                FTLASSERT(m_bStop);
                //dwResult = ::GetLastError();
                break;
            }

            pFIocpBuffer = CONTAINING_RECORD(lpOverlapped,  CFIocpBuffer, m_overLapped);
            if (!bRet 
                || ((pFIocpBuffer->m_operType == otRead || pFIocpBuffer->m_operType == otWrite) && ( 0 == dwBytesTransferred))
                )
            {
                if(! _HandleError(pIocpTask, pFIocpBuffer, dwBytesTransferred, GetLastError()))
                {
                    break;
                }
                continue;
            }
            _ProcessPackage(pIocpTask, pFIocpBuffer, dwBytesTransferred);

            if( !HasOverlappedIoCompleted(lpOverlapped))
            {
                //��⵽����һ��������ɵ�״̬
            }
        }

        FTLTRACE(TEXT("Thread %d  will Quit\n"), GetCurrentThreadId());
        return dwResult;
    }

    BOOL CFIocpMgr::_HandleError(CFIocpBaseTask* pIocpTask, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred, DWORD dwErr)
    {
        BOOL bRet = TRUE;
        if (0 == dwBytesTransferred)
        {
            pIocpTask->RemoveBuffer(pIoBuffer);
            DissociateTask(pIocpTask);
            pIocpTask->OnUninitialize(this, pIoBuffer, dwBytesTransferred);
            m_IocpBufferPool.Retrun(pIoBuffer);
        }
        else
        {
            switch (dwErr)
            {
            case ERROR_HANDLE_EOF:
                if (pIocpTask && pIoBuffer)
                {
                    pIocpTask->RemoveBuffer(pIoBuffer);
                    DissociateTask(pIocpTask);
                    pIocpTask->OnUninitialize(this, pIoBuffer, dwBytesTransferred);
                    m_IocpBufferPool.Retrun(pIoBuffer);
                }
                break;
            default:
                FTLASSERT(FALSE);
                bRet = FALSE;
                break;
            }
        }
        return bRet;
    }

    BOOL CFIocpMgr::_ProcessPackage(CFIocpBaseTask* pTask, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred)
    {
        BOOL bRet = FALSE;

        FTLASSERT(pTask);
        FTLASSERT(pIoBuffer);

        if (pTask && pIoBuffer)
        {
            bRet = TRUE;
            switch (pIoBuffer->m_operType)
            {
            case otInitialize:
                pTask->OnInitialize(this, pIoBuffer, dwBytesTransferred);
                break;
            case otRead:
                pTask->AfterReadCompleted(this, pIoBuffer, dwBytesTransferred);
                break;
            case otWrite:
                pTask->AfterWriteCompleted(this, pIoBuffer, dwBytesTransferred);
                break;
            default:
                FTLASSERT(FALSE);
                break;
            }
        }

        return bRet;
    }
}
#endif //FTL_IOCP_HPP