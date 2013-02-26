///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   ftlthread.h
/// @brief  Functional Template Library Base Header File.
/// @author fujie
/// @version 0.6 
/// @date 03/30/2008
/// @defgroup ftlthread ftl thread function and class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef FTL_THREADPOOL_H
#define FTL_THREADPOOL_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlThreadPool.h requires ftlbase.h to be included first
#endif

#include "ftlThread.h"

namespace FTL
{
	/*********************************************************************************************
    * �̳߳� -- ����Ԥ�������������̴߳��������ٱ����������Ŀ�����̯���˸�������������ϣ�
    *           ִ�д���Խ�࣬ÿ���������ֵ������̱߳�������ԽС
    *   ���÷�Χ��
    *     1.��λʱ���ڴ�������Ƶ������������ʱ��� -- �粻�ʺ� FTP ��������
    *     2.��ʵʱ��Ҫ��ϸߡ�������ܵ�������ڴ����̣߳��������㲻��ʵʱҪ����˱�������̳߳ؽ���Ԥ������ 
    *     ע�⣺ʹ���̳߳��������Ŀ����ϴ�,�൱����̺߳��ڲ����ݽṹ�������Ľ��̵�һ����ɲ���,��ҪäĿʹ��
	*
	* ΢���ṩ���̳߳�(ms-help://MS.MSDNQTR.v90.en/dllproc/base/thread_pools.htm) -- Vista֮�����?
	*	�̳߳������ĸ������ÿ�������һ���߳����
	*     1. �� I/O ��� ( WT_EXECUTEDEFAULT )�������߳�����ɶ˿����������˿���ͬʱ���е��߳���������Ϊ CPU ������������
	*        �� I/O ����е��߳��Ǹ��ݹ����������������ġ�
	*     2. I/O ���( WT_EXECUTEINIOTHREAD ) ���� ���з����첽I/O����Ĵ���
	*        CreateThreadpoolIo -> StartThreadpoolIo -> 
	*     3. ��ʱ����� ( WT_EXECUTEINTIMERTHREAD ) -- ʹ��ʱ��������߳��ܹ�ִ�иù�����Ŀ,ע��:����Ӧ��Ѹ��ִ�У���Ӧ���ж�
	*     4. �ȴ���� (WT_EXECUTEINWAITTHREAD) -- ����ʹ�ȴ�������߳�֮һ���й�����Ŀ�����������ʸ���(������Ŀ�����Ŷӷ��� I/O �����)��
	*      ����һ��Σ��(����ִ�й�����Ŀ�ĵȴ�����������߳��޷��ȴ���������õ�֪ͨ)
	* 
	*   �ܹ�ִ�����в���
	*     a.�첽���ú���(�������������̷߳���ÿһ���ͻ�)
	*       QueueUserWorkItem(�첽���ú���) -- ��һ��������Ŀ(�ص������Ķ���)�Ŷӷ����̳߳��е�һ���߳��в����������أ�
	*         �̳߳��е�ĳ���߳̽�����ù�����Ŀ�����º��������á��߳�ִ����󲢲����������������Ƿ����̳߳ء�
	*         ����Ϊÿ���ͻ������󴴽��ͳ����̡߳��ڲ�ʹ�� I/O ��ɶ˿�
	*     b.���չ涨��ʱ�������ú��� -- �ȴ���ʱ���ں˶���,����ػ�û���ʱ���֪ͨ.�̳߳صĶ�ʱ����������ȴ���ʱ��,��APC��Ŀ�Ŷ�
	*       CreateTimerQueue -- ������ʱ������,��һ�鶨ʱ��������֯����
	*       CreateTimerQueueTimer -- �ڶ�ʱ�������д�����ʱ��,����ָ������ϵͳĬ�ϵĶ�ʱ������(NULL)
	*       DeleteTimerQueueTimer/DeleteTimerQueue -- ��ʹ��ʱ��Ҫɾ�� ��ʱ��/��ʱ������ ,ע��:��Ҫ�ڹ�����Ŀ�߳���ɾ��,��������������
	*       ChangeTimerQueueTimer -- �ı䶨ʱ������ʱ��͵�������,�������ڵ�����ʱ����Ŀ����ʱ,����ʱ��
	*         �����ڵ���ʱ�ر��� MessageBox �����ĶԻ��� --WM_TIMER Ҳ����,�ؼ�����ʾ��Ϣ�� ID Ϊ0x0000FFFF(Spy++)
	*     c.�������ں˶����Ϊ��֪ͨ״̬ʱ���ú��� -- ���͸��ȴ����(��������Ŀ׼��ִ��ʱ������Ĭ���Ŷӷ���� I/O ������߳���)
	*       RegisterWaitForSingleObject -- �ڲ�ʹ�� WaitForMultipleObjects ���ȴ��Ѿ�ע��Ķ���
	*         ���ƣ�1.���ܶ�εȴ��������(DuplicateHandle)
	*               2.ÿ�� 63 ������󣬾�Ҫ����һ���߳���Ӹ������
	*       UnregisterWait -- ȡ��ע��״̬.��ȡ���ȴ������ע��״̬֮ǰ����Ҫ�ر��ں˶���ľ�����ᵼ�������ȴ�������޷���������
	*     d.���첽 I/O �������ʱ���ú���
	*       ������Ӧ�ó��򷢳�ĳЩ�첽 I/O ���󣬵���Щ�������ʱ����Ҫ��һ���̳߳�׼��������������ɵ�I/O����(���豸���������̳߳صķ�I/O�����������)��
	*       BindIoCompletionCallback -- �����첽IO������̳߳��еĺ���ָ�룬�ں����� ReadFileEx/WriteFileEx �Ⱥ������ʱ������ָ���ĺ�����
	*         �ú������ڲ�����CreateIoCompletionPort�����Ա�֤������һ���߳�ʼ���ڷ�I/O�����.
	*         ���Ҫ���������������Ϣ����ص�������Ӧ��ʹ�ý���������Ϣ���� OVERLAPPED �ṹ�Ľ�β���Ĵ�ͳ����(���� OVERLAPPED Ϊ��һ����Ա���������µĽṹ��)
	*         ע�⣺�ر��豸�ᵼ���������д������I/O����������ɣ�������һ��������롣�ڻص�������ע�⴦�����������
	*   ����˵��
	*     WT_EXECUTEINPERSISTENTTHREAD  ���� Ҫ������̲߳�����ֹ����,ʹ�������̳߳ص��̣߳�Ӧ�ñ�֤������ĿѸ��ִ��(�ŵ���ʱ������߳���ִ��)
	*     WT_EXECUTELONGFUNCTION ���� ������Ŀ��Ҫ���Ѻܳ���ʱ�������У������̳߳ؾ����Ƿ�Ҫ�����߳���Ӹ��̳߳ء�
	*     WT_EXECUTEONLYONCE ����������Ŀִֻ��һ�ξ�ֹͣ
	*     WT_TRANSFER_IMPERSONATION
	* 
	*
    * ATL7��ʼ�ṩ������ɶ˿ڵ��̳߳�(CThreadPool)ģ����
    * Sample��http://msdn.microsoft.com/en-us/library/39f8fee2(VS.80).aspx
	*   ���ܵ�ʵ�֣�
	*     ����һ�� I/O ��ɶ˿ڣ�������һ���ȴ��ö˿ڵ��̳߳أ��򿪶�� I/O �豸�������ǵľ������ɶ˿ڹ���������
	*     ���첽 I/O �������ʱ���豸��������ͽ���������Ŀ���Ŷ��������ɶ˿ڡ�
    *   ȱ�㣺
    *     Ҫ����õ���������������ͬ��--��������������ֻ�ܰ���һģһ������������(���� QueueUserWorkItem ���)
    *     
	* 
    * ʹ�÷�ʽ���μ� test_CFThreadPool
    *********************************************************************************************/
#pragma TODO(CreateThreadpoolIo)
	//Ŀǰ���̳߳� SubmitJob ��ʵ�ַ�ʽ���ܻ��ճ� Job ������ʱ�����������û�з��ص�Bug ?
	//http://m.blog.csdn.net/blog/ithzhang/8373243

	template <typename T> class CFThreadPool;  //ǰ������
	//! �̳߳��� -- ����ģ�������ͨ�����Է���������ݣ�
	//! �����ʵ�ֶ�����в�ͬ�������͵�Job�����Խ�ģ����Ϊ DWORD_PTR �ȿ���ת��Ϊָ������ͼ���
	FTLEXPORT template <typename T>
	class CFJobBase
	{
		friend class CFThreadPool<T>;   //����Threadpool���� m_pThreadPool ��ֵ
	public:
		CFJobBase();
		virtual ~CFJobBase();
		INT	 GetJobIndex() const;
		//���Job�������й����б�ȡ����������������
		BOOL NotifyStop();

		//! �����new�����ģ�һ��Ҫ�ڽ���ʱ���� delete this���������� pParam�� -- �Ƿ�������� m_bAutoDelete��
		// �����Run��ͨ����Ҫѭ�� ���� GetJobWaitType �������
		virtual void Run(T param) = 0;

		//����̳߳�ֹͣ�ˣ�ʣ�µ�Job������ִ�У�����øú�������������ڴ����Դ, �� delete param/this ��
		virtual void OnCancelJob(T param);
	protected:
		//BOOL m_bCancel;
		//! ͨ���ú�������ȡ�̳߳ص�״̬(Stop/Pause)���Լ�Job�Լ���Stop, �÷�ͬ CFThread:GetThreadWaitType:
		FTLINLINE FTLThreadWaitType GetJobWaitType(DWORD dwMilliseconds = INFINITE) const;
	private:
		//����Ϊ˽�еı����ͷ�������ʹ������Ҳ��Ҫֱ�Ӹ��ģ���Pool���ý��п���
		INT m_nJobIndex;
		HANDLE m_hEventJobStop;
		CFThreadPool<T>* m_pThreadPool;
		BOOL Initialize();
		BOOL Finalize();
	};


	typedef enum tagGetJobType
	{
		typeStop,
		typeSubtractThread,
		typeGetJob,
		typeError,		//����δ֪����(Ŀǰ�в����ʲô����»ᷢ��)
	}GetJobType;

	//����ʹ��ָ���ƺ����ã�ʹ��ģ��Ļ�������ʱӦ��ʹ�����û���ָ�룿��ֱ����ֵ�� -- ��ֵ����Ҫ�����Ż�
	FTLEXPORT template <typename T>  
	class CFThreadPool
	{
		//typedef CFSharePtr<CFJobBase< T> > CFJobBasePtr;
		//friend class CFJobBasePtr;
		friend class CFJobBase<T>;  //����Job��ȡStopEventHandle -- ���߻�Ϊfriend���Ƿ���ʣ�
	public:
		struct JobInfo
		{
			JobInfo()
			{
				nJobPriority = 0;
				nJobIndex = 0;
				pJob = NULL;
				param = T();
			}
			bool operator < (const JobInfo & other) const
			{
				COMPARE_MEM_LESS(nJobPriority, other);
				COMPARE_MEM_LESS(nJobIndex, other);
				return true;
			}

			//�����ĳ�map�����ȼ��Ѿ�����������
			INT nJobPriority;		//���ȼ���ֵԽС�����ȼ�Խ�ߣ�Խ�类���ã�Ĭ����0������ OnSubmitJob �е���(�в�֧�ֶ�̬����)
			INT nJobIndex;
			CFJobBase<T> *pJob;
			//CFJobBasePtr *pJob;
			T param;
		};
		//! ���� nMinNumThreads -- nMaxNumThreads ֮�����е����̵߳ĸ���
		//! ע�⣺nMaxNumThreads ����С�� MAXIMUM_WAIT_OBJECTS
		FTLINLINE CFThreadPool(LONG nMinNumThreads,LONG nMaxNumThreads);
		FTLINLINE virtual ~CFThreadPool(void);

		//! ��ʼ�̳߳�,��ʱ�ᴴ�� nMinNumThreads ���߳�
		FTLINLINE BOOL Start();

		//! ֹͣ�̳߳� -- ÿһ���߳̽���ʱ����� OnSubtractThread
		//! ע�⣺����ɾ����ǰע��ĵ���δ���еĹ����������Ҫɾ������Ҫ����ClearUndoWork
		FTLINLINE BOOL Stop();

		FTLINLINE BOOL StopAndWait(DWORD dwTimeOut = FTL_MAX_THREAD_DEADLINE_CHECK ,
			BOOL bCloseHandle = TRUE );

		//! �ȴ������̶߳�����
		FTLINLINE BOOL Wait(DWORD dwTimeOut,BOOL bCloseHandle);

		//! �����ǰδ��ɵĹ�����
		FTLINLINE BOOL ClearUndoWork();

		//! ���̳߳���ע�Ṥ�� -- �����ǰû�п��е��̣߳����ҵ�ǰ�߳���С������߳���������Զ������µ��̣߳�
		//! ÿһ���̴߳����ǻ���� OnAddThread
		//! �ɹ����ͨ�� outJobIndex ����Job�������ţ���ͨ����������λ��ȡ���ض���Job
		FTLINLINE BOOL SubmitJob(CFJobBase<T>* pJob,const T& param, INT* pOutJobIndex);
		//FTLINLINE BOOL SubmitJob(void (*pJobProc)(T& param), T& param, INT* pOutJobIndex);

		//ȡ��ָ����Job, TODO:���ȡ��Job���ͻ������ܵ����ߵõ�ָ��ʱ��Jobִ����� delete this�����ճ�Ұָ���쳣
		FTLINLINE BOOL CancelJob(INT nJobIndex);
		//FTLINLINE BOOL PauseJob(INT nJobIndex);
		//FTLINLINE BOOL ResumeJob(INT nJobIndex);

		//! ��ȡ��ǰ�����ŵ��̸߳��� -- �Ƿ���Ҫ���ӻ�ȡ ��С������̵߳ĺ���������Ҫ
		FTLINLINE LONG GetActiveThreadCount() const;

		//! ������ͣ�̳߳صĲ���
		FTLINLINE BOOL Pause();
		//! ��������̳߳صĲ���
		FTLINLINE BOOL Resume();
		//! �Ƿ��Ѿ���������ͣ
		FTLINLINE BOOL HadRequestPause() const;
		FTLINLINE BOOL HadRequestStop() const;
	protected:
		virtual BOOL OnAddThread();
		virtual BOOL OnSubtractThread();
		//! ��ע��Jobʱ���ã����Ҫʵ�ִ����ȼ���Job���������ظú�������Job�����ʵ���λ�ã�Ȼ�󷵻�TRUE
		virtual BOOL OnSubmitJob(const JobInfo* pJobInfo);
	protected:
		//! �������е��߳�,��� ��ǰ�߳��� + nThreadNum <= m_nMaxNumThreads ʱ ��ɹ�ִ��
		FTLINLINE BOOL AddThread(LONG nThreadNum);
		FTLINLINE void DestroyPool();
		FTLINLINE GetJobType GetJob(CFJobBase<T>** ppJob,T* pParam);
		FTLINLINE void DoJobs();
		//FTLINLINE BOOL InternalStop(DWORD dwMilliseconds);
		//FTLINLINE BOOL InternalSubtractThread(LONG nThreadNum,DWORD dwMilliseconds);
	protected:
		const LONG m_nMinNumThreads;            //! �̳߳������ٵ��̸߳���
		const LONG m_nMaxNumThreads;            //! �̳߳��������̸߳���

		//TODO: �������ͳһ��
		LONG m_nCurNumThreads;                  //! ��ǰ���̸߳���(��Ҫ����ά�� m_pThreadHandles ����)
		LONG m_nRunningThreadNum;				//! ��ǰ�����ŵ��̸߳���(���������е��߳̽���ʱ���� Complete �¼�)

		//LONG m_nCanSubtractThreadNum;
		HANDLE* m_pThreadHandles;               //! �����߳̾��������(Ϊ�˷���Wait)
		DWORD*  m_pThreadIds;                   //! �����߳� Id ������(Ϊ�����߳̽�������������е�λ��)
		//! ����Job����Ϣ,���ڻ�Ƶ�����롢ɾ��������Ҫ����JobIndex���ң���˱���� map/set/queue ?
		typedef std::map<INT, JobInfo* >	JobInfoContainer;
		JobInfoContainer		  m_WaitingJobs;//! �ȴ����е�Job
		JobInfoContainer		  m_DoingJobs;  //! �������е�Job
		//std::queue<JobInfo*>    m_JobsQueue;    
		HANDLE m_hEventStop;                    
		HANDLE m_hEventContinue;
		HANDLE m_hEventAllThreadComplete;		//! ���е��̶߳�����ʱ��������¼�
		HANDLE m_hSemaphoreJobToDo;             //! ���滹�ж��ٸ�Job���ź�����ÿSubmitһ��Job��������һ��
		HANDLE m_hSemaphoreSubtractThread;      //! ���ڼ����̸߳���ʱ���ű�,��ʼʱ����Ϊ0��ÿҪ�ͷ�һ����������һ����
		//! ������Ϊ m_nMaxNumThreads - m_nMinNumThreads
		INT               m_nJobIndex;
		CFCriticalSection m_lockDoingJobs;		//���� m_DoingJobs ʱ����
		CFCriticalSection m_lockWaitingJobs;    //���� m_WaitingJobs ʱ����
		CFCriticalSection m_lockThreads;        //���� m_pThreadHandles/m_pThreadIds ʱ����

		static unsigned int CALLBACK JobThreadExecute(void *pThis);    //�̳߳����̵߳�ִ�к���
		//static unsigned int CALLBACK StopAsyncProc(void *pAsyncProxyParam);
		//static unsigned int CALLBACK SubtractThreadAsyncProc(void * pAsyncProxyParam);
	};

}

#endif //FTL_THREADPOOL_H

#ifndef USE_EXPORT
#  include "ftlThreadPool.hpp"
#endif