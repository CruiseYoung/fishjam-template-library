#ifndef FTL_THREADPOOL_HPP
#define FTL_THREADPOOL_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlThreadPool.h"
#endif

namespace FTL
{
	///////////////////////////////////////////// CFJobBase ///////////////////////////////////////////////////
	template <typename T>
	CFJobBase<T>::CFJobBase()
	{
		m_nJobIndex = 0;
		m_pThreadPool = NULL;
		m_hEventJobStop = NULL;
		//m_bCancel = FALSE;
		//m_bInitialized = FALSE;
		//m_hEventStop = NULL;
		//m_hEventContinue = NULL;
	}
	template <typename T>
	CFJobBase<T>::~CFJobBase()
	{
		Finalize();
	}

	template <typename T>
	INT CFJobBase<T>::GetJobIndex() const
	{
		return m_nJobIndex;
	}

	template <typename T>
	BOOL CFJobBase<T>::NotifyStop()
	{
		BOOL bRet = FALSE;
		API_VERIFY(SetEvent(m_hEventJobStop));
		return bRet;
	}

	template <typename T>
	void CFJobBase<T>::OnCancelJob(T param)
	{
		UNREFERENCED_PARAMETER(param);
		//delete pParam; //ע����ת��
		//delete this; 
		//ע�⣺�˴��Ǹ���ʵ��
	}

	template <typename T>
	BOOL CFJobBase<T>::Initialize()
	{
		FTLASSERT(NULL == m_hEventJobStop);
		BOOL bRet = FALSE;
		API_VERIFY(NULL != (m_hEventJobStop = CreateEvent(NULL, TRUE, FALSE, NULL))); 
		return bRet;
	}

	template <typename T>
	BOOL CFJobBase<T>::Finalize()
	{
		BOOL bRet = TRUE;
		SAFE_CLOSE_HANDLE(m_hEventJobStop, NULL);
		return bRet;
	}

	template <typename T>
	FTLThreadWaitType CFJobBase<T>::GetJobWaitType(DWORD dwMilliseconds /* = INFINITE*/) const
	{
		FUNCTION_BLOCK_TRACE(0);
		FTLThreadWaitType waitType = ftwtError;
		HANDLE waitEvent[] = 
		{
			m_hEventJobStop,
			m_pThreadPool->m_hEventStop,
			m_pThreadPool->m_hEventContinue
		};
		DWORD dwResult = ::WaitForMultipleObjects(_countof(waitEvent),waitEvent,FALSE,dwMilliseconds);
		switch (dwResult)
		{
		case WAIT_OBJECT_0:
			waitType = ftwtStop;		//Job Stop Event
			break;
		case WAIT_OBJECT_0 + 1:
			waitType = ftwtStop;		//Job Thread Stop Event
			break;
		case WAIT_OBJECT_0 + 2:
			waitType = ftwtContinue;	//Job Thread Continue Event
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
		//FTLASSERT( nMaxNumThreads <= MAXIMUM_WAIT_OBJECTS);  

		m_nCurNumThreads = 0;
		m_nRunningThreadNum = 0;
		m_pThreadHandles = NULL;
		m_pThreadIds = NULL;
		m_nJobIndex = 0;

		//������ͬʱ���Ĺ���������Ϊ MAXLONG -- Ŀǰ��ʱ�����Ƕ����еĸ���
		m_hSemaphoreJobToDo = ::CreateSemaphore(NULL,0,MAXLONG,NULL);
		FTLASSERT(NULL != m_hSemaphoreJobToDo);

		m_hEventStop = CreateEvent(NULL,TRUE,FALSE,NULL);
		FTLASSERT(NULL != m_hEventStop);

		m_hEventContinue = ::CreateEvent(NULL,TRUE,TRUE,NULL);
		FTLASSERT(NULL != m_hEventContinue);

		m_hEventAllThreadComplete = ::CreateEvent(NULL, TRUE, FALSE, NULL);
		FTLASSERT(NULL != m_hEventAllThreadComplete);

		if (m_nMinNumThreads < m_nMaxNumThreads)
		{
			//�����С������̸߳�����ͬ��˵�����Զ�̬�����̳߳أ������� nMaxNumThreads - nMinNumThreads ���߳�
			m_hSemaphoreSubtractThread = CreateSemaphore(NULL,0,nMaxNumThreads - nMinNumThreads,NULL);
			FTLASSERT(NULL != m_hSemaphoreSubtractThread);
		}
		else
		{
			//���ܶ�̬���������̸߳��� -- ����һ�����ֵΪ1��Semaphore���߼��б�֤�������Release
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

		FTLASSERT(0 == m_nRunningThreadNum);  //����ʱ���е��̶߳�Ҫ����
	}

	template <typename T>  
	BOOL CFThreadPool<T>::Start()
	{
		FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
		BOOL bRet = FALSE;
		API_VERIFY(ResetEvent(m_hEventStop));   //����Stop�¼�
		API_VERIFY(SetEvent(m_hEventContinue)); //��������¼�
		API_VERIFY(ResetEvent(m_hEventAllThreadComplete));	
		{
			CFAutoLock<CFCriticalSection>   locker(&m_lockThreads); //m_lockJobs);
			if(NULL == m_pThreadHandles)    //��ֹ��ε���Start
			{
				unsigned int threadId = 0;
				m_pThreadHandles = new HANDLE[m_nMaxNumThreads];  //����m_nMaxNumThreads���̵߳Ŀռ�
				ZeroMemory(m_pThreadHandles,sizeof(HANDLE) * m_nMaxNumThreads);
				m_pThreadIds = new DWORD[m_nMaxNumThreads];
				ZeroMemory(m_pThreadIds,sizeof(DWORD) * m_nMaxNumThreads);
				for( LONG i = 0;i < m_nMinNumThreads; i++ )  //��ʼʱֻ���� m_nMinNumThreads ���߳�
				{
					m_pThreadHandles[m_nCurNumThreads] = (HANDLE) _beginthreadex( NULL, 0, JobThreadExecute, this, 0, &threadId);
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
		FTLTRACE(TEXT("CFThreadPool::Stop, this=%p\n"), this);
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
		DWORD dwResult = WaitForSingleObject(m_hEventAllThreadComplete, dwTimeOut);
		switch (dwResult)
		{
		case WAIT_OBJECT_0: //���е��̶߳�������
			bRet = TRUE;
			break;
		case WAIT_TIMEOUT:
			FTLTRACEEX(tlError,TEXT("!!!CFThreadPool::Wait, Not all thread over in %d millisec\n"),dwTimeOut);
			SetLastError(ERROR_TIMEOUT);
			bRet = FALSE;
			break;
		default:
			FTLASSERT(FALSE);
			bRet = FALSE;
			break;
		}

#if 0
		//���ʹ�����ֻ��ƵĻ�������64��������
		HANDLE* pTempHandles = NULL;
		LONG  nTempNumThreads = 0;
		{
			//�����߳̽�������Զ����� m_pThreadHandles ���飬��ˣ�����������ʱ����
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
			case WAIT_OBJECT_0: //���е��̶߳�������
				bRet = TRUE;
				break;
			case WAIT_TIMEOUT:
				FTLTRACEEX(tlError,TEXT("!!!CFThreadPool::Wait, Not all thread over in %d millisec\n"),dwTimeOut);
				SetLastError(ERROR_TIMEOUT);
				bRet = FALSE;
				break;
			default:
				FTLASSERT(FALSE);
				bRet = FALSE;
				break;
			}
			SAFE_DELETE_ARRAY(pTempHandles);
		}
#endif 
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
			FTLTRACE(TEXT("CFThreadPool::ClearUndoWork, waitingJob Number is %d\n"), m_WaitingJobs.size());
			while (!m_WaitingJobs.empty())
			{
				DWORD dwResult = WaitForSingleObject(m_hSemaphoreJobToDo,INFINITE); //�����ͷŶ�Ӧ���ű����
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
				//�����������������ټ���
				SetLastError(ERROR_INVALID_PARAMETER);
				bRet = FALSE;
			}
			else
			{
				unsigned int threadId = 0;
				for(LONG i = 0;i < nThreadNum; i++)
				{
					FTLASSERT(NULL == m_pThreadHandles[m_nCurNumThreads]);
					m_pThreadHandles[m_nCurNumThreads] = (HANDLE) _beginthreadex( NULL, 0, JobThreadExecute, this, 0, &threadId);
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
		//Ĭ��ʵ��ֱ�ӷ���FALSE����JobQueue���������
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
		FTLASSERT(NULL != m_hEventStop); //�������DestroyPool�󣬾Ͳ����ٴε��øú���
		FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
		BOOL bRet = FALSE;
		{
			//����Job���һ���һ���ȴ��߳�
			{
				CFAutoLock<CFLockObject> locker(&m_lockWaitingJobs);
				m_nJobIndex++;
				pJob->m_pThreadPool = this;         //����˽�б����������Լ���ֵ��ȥ
				pJob->m_nJobIndex = m_nJobIndex;	//����˽�б���������JobIndex

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
			}

			API_VERIFY(ReleaseSemaphore(m_hSemaphoreJobToDo,1L,NULL));
			//SwitchToThread();
			Sleep(1);	//���ѵȴ����̣߳�ʹ�������߳̿��Ի�ȡJob -- ע�� CFAutoLock �ķ�Χ

			{
				//����Ƿ���Ҫ�����߳�
				CFAutoLock<CFLockObject> locker(&m_lockWaitingJobs);
				BOOL bNeedMoreThread = (!m_WaitingJobs.empty() && (m_nCurNumThreads < m_nMaxNumThreads));
				{
					if (bNeedMoreThread)
					{
						API_VERIFY(AddThread(1L));      //ÿ������һ���߳�
					}
				}
			}
		}
		return bRet;	
	}

	template <typename T>  
	BOOL CFThreadPool<T>::CancelJob(INT nJobIndex)
	{
		BOOL bRet = TRUE;
		BOOL bFoundWaiting = FALSE;
		BOOL bFoundDoing = FALSE;
		{
			//���Ȳ���δ����������
			CFAutoLock<CFLockObject> locker(&m_lockWaitingJobs);
			JobInfoContainer::iterator iterWaiting = m_WaitingJobs.find(nJobIndex);
			if (iterWaiting != m_WaitingJobs.end())
			{
				DWORD dwResult = WaitForSingleObject(m_hSemaphoreJobToDo, INFINITE); //�ͷŶ�Ӧ���ű���󣬱����ٴ��ҵ�
				API_ASSERT(dwResult == WAIT_OBJECT_0);

				//�ҵ�,˵�����Job��û������
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
			//�����������е�����
			CFAutoLock<CFLockObject> locker(&m_lockDoingJobs);
			JobInfoContainer::iterator iterDoing = m_DoingJobs.find(nJobIndex);
			if (iterDoing != m_DoingJobs.end())
			{
				JobInfo* pInfo = iterDoing->second;
				FTLASSERT(pInfo);
				FTLASSERT(pInfo->pJob->GetJobIndex() == nJobIndex);
				pInfo->pJob->NotifyStop();
				//ע�⣺����ֻ������Cancel��ʵ���������Ƿ�������Cancel����Ҫ����Job��ʵ�֣�

				//��Ҫ delete pInfo �� m_DoingJobs.erase -- Job ��������ȡ��
				////delete pInfo;
				////m_DoingJobs.erase(iterDoing);

				bFoundDoing = TRUE;
			}
		}
		if (!bFoundWaiting && !bFoundDoing)
		{
			//Waiting �� Doing �ж�û���ҵ����Ѿ�ִ�����
			//do nothing
		}

		return bRet; //(bFoundDoing || bFoundWaiting);
	}

	//template <typename T>  
	//BOOL CFThreadPool<T>::PauseJob(INT nJobIndex)
	//{
	//	SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
	//	return FALSE;
	//}

	//template <typename T>  
	//BOOL CFThreadPool<T>::ResumeJob(INT nJobIndex)
	//{
	//	SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
	//	return FALSE;
	//}

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
		SAFE_CLOSE_HANDLE(m_hEventAllThreadComplete, NULL);
		SAFE_CLOSE_HANDLE(m_hEventContinue,NULL);
		SAFE_CLOSE_HANDLE(m_hEventStop,NULL);
	}

	template <typename T>  
	GetJobType CFThreadPool<T>::GetJob(CFJobBase<T>** ppJob, T* pParam)
	{
		FUNCTION_BLOCK_TRACE(0);
		HANDLE hWaitHandles[] = 
		{
			m_hEventStop,                 //�û�ѡ����ֹͣ
			m_hSemaphoreJobToDo,          //����δ����Job
			m_hSemaphoreSubtractThread,   //��Ҫ�����߳�
		};

		DWORD dwResult = WaitForMultipleObjects(_countof(hWaitHandles),hWaitHandles,FALSE,INFINITE);
		switch(dwResult)
		{
		case WAIT_OBJECT_0: //Stop Event
			return typeStop;
		case WAIT_OBJECT_0 + 1: //Get Job
			break;
		case WAIT_OBJECT_0 + 2://�����߳�
			return typeSubtractThread;
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
		BOOL bRet = FALSE;
		FUNCTION_BLOCK_TRACE(0);
		CFJobBase<T>* pJob = NULL;
		T param = T(); //��Ҫȱʡ����
		GetJobType getJobType = typeStop;
		while(typeGetJob == (getJobType = GetJob(&pJob, &param)))
		{
			INT nJobIndex = pJob->GetJobIndex();
			FTLTRACEEX(tlInfo, TEXT("CFThreadPool Begin Run Job %d\n"), nJobIndex);
			API_VERIFY(pJob->Initialize());
			pJob->Run(param); //ִ��Job������������غ�, pJob һ���Ѿ��� delete this ��, �����ٵ����䷽��

			FTLTRACEEX(tlInfo, TEXT("CFThreadPool End Run Job %d\n"), nJobIndex);

			//�˳�Run˵��һ��Job����
			{
				//���ȴ������б���ɾ��
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

			//���һ���Ƿ���Ҫ�����߳�
			BOOL bNeedSubtractThread = FALSE;
			{
				CFAutoLock<CFLockObject> locker(&m_lockWaitingJobs);
				//��������û��Job�����ҵ�ǰ�߳���������С�߳���ʱ
				bNeedSubtractThread = (m_WaitingJobs.empty() && (m_nCurNumThreads > m_nMinNumThreads) && !HadRequestStop());
				if (bNeedSubtractThread)
				{
					//֪ͨ����һ���߳�
					ReleaseSemaphore(m_hSemaphoreSubtractThread,1L,NULL);
				}
			}
		}
		if (typeSubtractThread == getJobType)  //��Ҫ�����߳�,Ӧ�ð��Լ��˳� -- ע�⣺֪ͨ�˳����̺߳�ʵ���˳����߳̿��ܲ���ͬһ��
		{
			FUNCTION_BLOCK_NAME_TRACE(TEXT("typeSubtractThread == getJobType,will remove self thread"),
				DEFAULT_BLOCK_TRACE_THRESHOLD);
			CFAutoLock<CFLockObject> locker(&m_lockThreads);
			LONG index = 0;
			DWORD dwCurrentThreadId = GetCurrentThreadId();
			for (; index < m_nCurNumThreads; index++)
			{
				if (m_pThreadIds[index] == dwCurrentThreadId)  //�ҵ��Լ�
				{
					break;
				}
			}
			FTLASSERT(index < m_nCurNumThreads);
			if (index < m_nCurNumThreads)
			{
				//�����һ���̵߳���Ϣ�Ƶ��˳����߳�λ�� -- ����˳����߳̾������һ��ʱҲ��ȷ
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
	unsigned int CFThreadPool<T>::JobThreadExecute(void *pThis)
	{
		FUNCTION_BLOCK_TRACE(0);
		CFThreadPool<T>* pThreadPool = (CFThreadPool<T>*)pThis;
		LONG nRunningNumber = InterlockedIncrement(&pThreadPool->m_nRunningThreadNum);
		FTLTRACE(TEXT("ThreadId[%d] Increment, %ld\n"), GetCurrentThreadId(), nRunningNumber);

		pThreadPool->DoJobs();

		nRunningNumber = InterlockedDecrement(&pThreadPool->m_nRunningThreadNum);
		FTLTRACE(TEXT("ThreadId[%d] Decrement, %ld\n"), GetCurrentThreadId(), nRunningNumber);
		if (0 == nRunningNumber)
		{
			//�߳̽������ж��Ƿ������һ���̣߳�����ǣ������¼�
			SetEvent(pThreadPool->m_hEventAllThreadComplete);
		}
		return(0);
	}
}

#endif //FTL_THREADPOOL_HPP