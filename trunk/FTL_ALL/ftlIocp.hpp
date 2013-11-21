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
        
        Reset(0);
    }

    CFIocpBuffer::~CFIocpBuffer()
    {
        SAFE_DELETE_ARRAY(m_pBuffer);
    }

    VOID CFIocpBuffer::Reset(DWORD dwSize)
    {
        ZeroMemory(&m_overLapped, sizeof(m_overLapped));
        m_operType = iotInitialize;
        m_nSequenceNumber = 0;
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
    }

    CFIocpBaseTask::CFIocpBaseTask()
    {
        m_nCurReadSequence = 0;
        m_nCurWriteSequence = 0;
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
        Stop();
        Close();
    }

    BOOL CFIocpMgr::_InitlizeIocp(DWORD NumberOfConcurrentThreads)
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

        API_VERIFY(_InitlizeIocp(NumberOfConcurrentThreads));
        if (bRet)
        {
            bRet = OnIocpStart();
        }
        

        return bRet;
    }

    BOOL CFIocpMgr::Stop()
    {
        BOOL bRet = TRUE;
        m_bStop = TRUE;
        OnIocpStop();

        for (DWORD dwIndex = 0; dwIndex < m_nConcurrentThreadCount; ++dwIndex)
        {
            API_VERIFY(PostQueuedCompletionStatus(m_hIoCompletionPort, 0, NULL, NULL));
        }
        return bRet;
    }
    VOID CFIocpMgr::Close(DWORD dwMilliseconds)
    {
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

    BOOL CFIocpMgr::AssociateTask(CFIocpBaseTask* pTask)
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
        if (bRet)
        {
            CFIocpBuffer* pTaskBuffer = m_IocpBufferPool.Get();
            FTLASSERT(pTaskBuffer);
            if (pTaskBuffer)
            {
                pTaskBuffer->Reset(m_dwBufferSize);
                bRet = PostQueuedCompletionStatus(m_hIoCompletionPort, 0, (ULONG_PTR)pTask, &pTaskBuffer->m_overLapped);
            }
        }
        return bRet;
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
        ULONG_PTR nCompletionKey = NULL;
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
            if (!bRet)
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
        switch (dwErr)
        {
        case ERROR_HANDLE_EOF:
            if (pIocpTask && pIoBuffer)
            {
                pIocpTask->OnUninitialize(this, pIoBuffer, dwBytesTransferred);
                m_IocpBufferPool.Retrun(pIoBuffer);
            }
            break;
        default:
            FTLASSERT(FALSE);
            bRet = FALSE;
            break;
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
            case iotInitialize:
                pTask->OnInitialize(this, pIoBuffer, dwBytesTransferred);
                break;
            case iotRead:
                pTask->AfterReadCompleted(this, pIoBuffer, dwBytesTransferred);
                break;
            case iotWrite:
                pTask->AfterWriteCompleted(this, pIoBuffer, dwBytesTransferred);
                break;
            default:
                FTLASSERT(FALSE);
                break;
            }
        }

        return bRet;
    }

    BOOL CFIocpNetServer::OnIocpStart()
    {
        BOOL bRet = FALSE;
        //��ʼ��Socket
        struct sockaddr_in ServerAddress;  
        // ������Ҫ�ر�ע�⣬���Ҫʹ���ص�I/O�Ļ����������Ҫʹ��WSASocket����ʼ��Socket   
        // ע�������и�WSA_FLAG_OVERLAPPED����   
        m_nSocketListen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);  

        CFIocpListenTask* pListenTask = new CFIocpListenTask(m_nSocketListen);
        AssociateTask(pListenTask);

        // ����ַ�ṹ��Ϣ   
        ZeroMemory((char *)&ServerAddress, sizeof(ServerAddress));  
        ServerAddress.sin_family = AF_INET;  
        // �������ѡ����κ�һ�����õĵ�ַ���������Լ�ָ����һ��IP��ַ    
        ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);                         
        //ServerAddress.sin_addr.s_addr = inet_addr("127.0.0.1");           
        ServerAddress.sin_port = htons(12345);                            
        // �󶨶˿�   

        bind(m_nSocketListen, (struct sockaddr *) &ServerAddress, sizeof(ServerAddress));
        // ��ʼ����
        listen(m_nSocketListen, SOMAXCONN);  

        return bRet;
    }
}
#endif //FTL_IOCP_HPP