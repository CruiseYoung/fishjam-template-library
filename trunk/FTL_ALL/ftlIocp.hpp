#ifndef FTL_IOCP_HPP
#define FTL_IOCP_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlIocp.h"
#endif

namespace FTL
{
    //HANDLE CFSocketIocpTask::GetIocpHandle()
    //{
    //    //SOCKET skSocket = ::WSASocket(AF_INET,SOCK_STREAM,IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
    //    return (HANDLE)m_nSocket;
    //}

    //HANDLE CFFileIocpTask::GetIocpHandle()
    //{
    //    return m_hHandle;
    //}

    //////////////////////////////////////////////////////////////////////////
    CFIocpMgr::CFIocpMgr()
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
    BOOL CFIocpMgr::Start(DWORD NumberOfConcurrentThreads)
    {
        BOOL bRet = FALSE;
        m_bStop = FALSE;

        DWORD dwThreadCount = NumberOfConcurrentThreads;
        if (DEFAULT_THREAD_COUNT == dwThreadCount)
        {
            dwThreadCount = _GetDefaultConcurrentThreadCount();
        }
        
        //1.����һ����ɶ˿ڵ��ں˶���
        m_hIoCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, (ULONG_PTR)NULL, 
            dwThreadCount); //��������ͬʱִ�е�����߳���
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

    BOOL CFIocpMgr::Stop()
    {
        BOOL bRet = TRUE;
        m_bStop = TRUE;
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
#endif 
            for (DWORD dwWaitIndex = 0; dwWaitIndex < m_nConcurrentThreadCount; ++dwWaitIndex)
            {
                WaitForSingleObject(m_pWorkThreads[dwWaitIndex], dwMilliseconds);
                CloseHandle(m_pWorkThreads[dwWaitIndex]);
            }
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

    BOOL CFIocpMgr::AddTask(CFIocpBaseTask* pTask)
    {
        BOOL bRet = FALSE;
        //��һ���ص�IO��ʽ�Ķ������󶨵��Ѵ����õ���ɶ˿ڶ�����
        HANDLE hIoPort = NULL;
        API_VERIFY((hIoPort = CreateIoCompletionPort(pTask->GetIocpHandle(), 
            m_hIoCompletionPort, (ULONG_PTR)pTask, 0)) != NULL );
        if (bRet)
        {
            FTLASSERT(hIoPort == m_hIoCompletionPort);
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
        F_IOCP_OVERLAPPED* pFIocpOverLapped = NULL;

        //���Ĺ�����ѭ������ GetQueuedCompletionStatus�������̻߳������ȴ�"���Ŷӵ����״̬"
        while(TRUE)
        {
            API_VERIFY(GetQueuedCompletionStatus(
                m_hIoCompletionPort,        //��ɶ˿��ں˶���ľ��
                &dwBytesTransferred,        //����IO����ʵ�ʴ�����߽����˶��ٸ��ֽ�,������У�������շ�������
                &nCompletionKey,            //���ذ󶨵����ʱ���Զ�������
                &lpOverlapped, 
                INFINITE));
            if (!bRet)
            {
                //continue;
                break;
            }
            if (NULL == lpOverlapped)
            {
                FTLASSERT(m_bStop);
                //dwResult = ::GetLastError();
                break;
            }
            CFIocpBaseTask* pIocpTask = reinterpret_cast<CFIocpBaseTask*>(nCompletionKey);
            pFIocpOverLapped = CONTAINING_RECORD(lpOverlapped,  F_IOCP_OVERLAPPED, overLapped);
            if (pIocpTask && pFIocpOverLapped)
            {
                pIocpTask->OnIoComplete(this, pFIocpOverLapped, dwBytesTransferred);
            }

            //CONTAINING_RECORD(lpOverlapped, MYOVERLAPPED, m_ol);
            if( !HasOverlappedIoCompleted(lpOverlapped))
            {
                //��⵽����һ��������ɵ�״̬
            }
        }

        return dwResult;
    }
}
#endif //FTL_IOCP_HPP