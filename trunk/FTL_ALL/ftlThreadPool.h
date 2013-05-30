///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @file   ftlthread.h
/// @brief  Functional Template Library Base Header File.
/// @author fujie
/// @version 0.6 
/// @date 03/30/2008
/// @defgroup ftlThreadPool ftl thread pool function and class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*************************************************************************************************************************
* �Ѷ�����
*   C++\ATL\Advanced\CThreadPool -- ʹ��ATL�е� CThreadPool ������
*************************************************************************************************************************/

#ifndef FTL_THREADPOOL_H
#define FTL_THREADPOOL_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlThreadPool.h requires ftlbase.h to be included first
#endif

#include "ftlThread.h"
//#include "ftlSharePtr.h"
#include "ftlFunctional.h"

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
	*       QueueUserWorkItem(�첽���ú���) -- ��һ��������Ŀ(�ص������Ķ���)�Ŷӷ����̳߳��е�һ���߳��в����������أ������Ͳ���ȡ��
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
    *     ���ܶ�̬�ı��С�����ܺܺõ���ͣ��ֹͣ ��
	* 
    * ʹ�÷�ʽ���μ� test_CFThreadPool
    *********************************************************************************************/
	//WaitForThreadpoolWorkCallbacks -- ����ȡ����δ���е����񣬵��޷�ȡ���Ѿ����е�����
	//http://www.cnblogs.com/wz19860913/articles/1274214.html --  Windows �̳߳�

	//Ŀǰ���̳߳� SubmitJob ��ʵ�ַ�ʽ���ܻ���� Job ������ʱ�����������û�з��ص�Bug ?
	//�����ڻص�����(��������÷��Ĺ�������) �� ���� SubmitJob �ĺ���(����÷��Ĺ��������м�����Ϣ) ͬ������Ԥ�����Bug

	//http://m.blog.csdn.net/blog/ithzhang/8373243

	//TODO:
	//  1.��������̸߳�����ʵ��(Ŀǰ���ô��������߳��������鷽ʽ���޷�֧�ֺܴ��ֵ)
	//  2.֧�����ú͵���Job������;
	//  3.֧�ֶ�̬�����̸߳���;
	//  4.֧�ֶ�̬����Job���ȼ�
	//  5.������Pool��ͣ��Ϊ֧�ֵ���Job��ͣ������װ����Pool��ͣ�ķ���;
	//  6.
	//ǰ������
	//! ����ģ��������̳߳��࣬���ܿ��Է���Ľ��в��������⣬��ӵ�������ص㣺
	//  1.���Զ�����������̵߳Ķ����� ��С/��� �̸߳���֮�����(Vista���ϵͳ�� SetThreadpoolThreadMaximum �Ⱥ��������ƹ���)
	//  2.�ܷ���Ķ���һ�������ȡ������������δ�������ɿ�ܴ��봦���������Ѿ����У�����Ҫ JobBase ��������� GetJobWaitType �ķ���ֵ���д���
	//    (��Ƚ϶��ԣ�WaitForThreadpoolWorkCallbacks ����ֻ��ȡ����δ���е�����)
	//  3.�ܶ������̳߳ؽ��� ��ͣ��������ֹͣ ���� -- ��Ҫ JobBase ��������� GetJobWaitType �ķ���ֵ���д���
	//  4.֧�ֻص���ʽ�ķ���֪ͨ( Progress/Error ��)
	//  5.ʹ��ģ�淽ʽʵ�֣��ܷ���Ľ��в�������
	//  6.�ڼ�������ʱ�����������ȼ�(Ŀǰ�в�֧�ֶ�̬����)
	//  7.ʹ�õ���΢��Ļ���API����֧��WinXP��Vista��Win7�ȸ��ֲ���ϵͳ(CreateThreadpoolWork ��ֻ����Vista�����ʹ��)
	//  8.�����ж��ʵ����������п���(QueueUserWorkItem ���ǵ�ʵ��)
	//  9.���õ��ǶԳ�ģʽ���̳߳�(û�й����߳�)�����гɱ���
	template <typename T> class CFThreadPool;  

	//enum FJobStatus
	//{
	//	jsWaiting,
	//	jsDoing,
	//	jsCancel,	//������������Ϊcancel, ���Job����֧��Cancel�Ļ��������� GetJobWaitType ���ú�������
	//	jsDone,
	//	jsError,	
	//};

	//! �����ʵ�ֶ�����в�ͬ�������͵�Job�����Խ�ģ����Ϊ DWORD_PTR �ȿ���ת��Ϊָ������ͼ���
	FTLEXPORT template <typename T>
	class CFJobBase
	{
		friend class CFThreadPool<T>;   //����Threadpool���� m_pThreadPool/m_nJobIndex ��ֵ
	public:
		FTLINLINE CFJobBase(BOOL bSuspendOnCreate = FALSE);
		FTLINLINE CFJobBase(T& rJobParam);
		FTLINLINE virtual ~CFJobBase();

		//bool operator < (const CFJobBase & other) const;

		//! ��ȡ������Job�����ȼ�, ����ԽС�����ȼ�Խ��(�ڵȴ�����������Խǰ��)��ȱʡֵ�� 0
		//  ע�⣺���ȼ������ڷ��� Pool ǰ���ã������Ͳ����ٵ�����
		FTLINLINE LONG GetPriority() const { return m_nJobPriority; }
		FTLINLINE LONG SetPriority(LONG nNewPriority);

		FTLINLINE LONG GetJobIndex() const;
		FTLINLINE DWORD GetErrorStatus() const;
		FTLINLINE LPCTSTR GetErrorInfo() const;

		//FTLINLINE FJobStatus GetJobStatus() const { return m_JobStatus; }

		T		m_JobParam;			//! Job��ʹ�õĲ������˴�Ϊ�˼򻯣�ֱ�Ӳ��ù��б����ķ�ʽ
	protected:
		// �����Run��ͨ����Ҫѭ�� ���� GetJobWaitType �������
		virtual BOOL Run() = 0;
		//! �����new�����ģ�ͨ����Ҫ�� OnFinalize �е��� delete this (������������������ڹ�������)
		//! isWaiting ��ʾ�Ƿ����ڵȴ�״̬�½�����
		//!   TRUE -- δ���е�Job(ֱ��ȡ�����̳߳�ֹͣ) 
		//!   FALSE -- ���й���Job,���������н�����Ҳ���������й����б�ȡ��
		virtual VOID OnFinalize(BOOL isWaiting) = 0;
	protected:
		FTLINLINE void _SetErrorStatus(DWORD dwErrorStatus, LPCTSTR pszErrorInfo);
		FTLINLINE void _NotifyProgress(LONGLONG nCurPos, LONGLONG nTotalSize);
		FTLINLINE void _NotifyError();
		FTLINLINE void _NotifyError(DWORD dwError, LPCTSTR pszDescription);
		FTLINLINE void _NotifyCancel();

		//! ͨ���ú�������ȡ�̳߳ص�״̬(Stop/Pause)���Լ�Job�Լ���Stop, �÷�ͬ CFThread:GetThreadWaitType:
		//! �����֧����ͣ�������� INFINITE���粻��֧����ͣ(�����紫��)��������� 0
		FTLINLINE FTLThreadWaitType GetJobWaitType(DWORD dwMilliseconds = INFINITE) const;
	private:
		//����Ϊ˽�еı����ͷ�������ʹ������Ҳ��Ҫֱ�Ӹ��ģ���Pool���ý��п���
		BOOL		m_bSuspendOnCreate;
		LONG		m_nJobPriority;
		LONG		m_nJobIndex;
		DWORD				m_dwErrorStatus;	//GetLastError
		CFStringFormater	m_strFormatErrorInfo;		
		HANDLE		m_hEventJobContinue;
		HANDLE		m_hEventJobStop;					//ֹͣJob���¼����ñ�������Pool�������ͷ�(TODO:Pool�л���?)
		//FJobStatus	m_JobStatus;
		CFThreadPool<T>* m_pThreadPool;

		FTLINLINE BOOL _Pause();
		FTLINLINE BOOL _Resume();
		//���Job�������й����б�ȡ����������������
		FTLINLINE BOOL _Cancel();
	};

	typedef enum tagGetJobType
	{
		typeStop,
		typeSubtractThread,
		typeGetJob,
		typeError,		//����δ֪����(Ŀǰ�в����ʲô����»ᷢ��)
	}GetJobType;

	//! �Ƚ�Job�Ĵ�С������ȷ���� Waiting �����е�˳�� ��������Ϊ Priority -> Index
	//! ΢����Bug?(invalid operator< for m_WaitingJobs.find) -- http://support.microsoft.com/kb/949171
	struct WaitingJobSorter
	{
	public:
		LONG nJobPriority;
		LONG nJobIndex;

		FTLINLINE WaitingJobSorter()
		{
			nJobPriority = 0;
			nJobIndex = 0;
		}
		FTLINLINE WaitingJobSorter(LONG nJobPriority, LONG nJobIndex)
		{
			this->nJobPriority = nJobPriority;
			this->nJobIndex = nJobIndex;
		}
		WaitingJobSorter(const WaitingJobSorter&  rOther)
		{
			nJobPriority = rOther.nJobPriority;
			nJobIndex = rOther.nJobIndex;
		}
		FTLINLINE bool operator < (const WaitingJobSorter & other) const
		{
			COMPARE_MEM_LESS(nJobPriority, other);
			COMPARE_MEM_LESS(nJobIndex, other);
			return false;
		}
	};

	//�ص����� -- ͨ�� pJob->m_JobParam ���Է�������Ϊ T �� ����
	FTLEXPORT template <typename T>
	class IFThreadPoolCallBack
	{
	public:
		//��Job���������Ժ󣬻��� Pool ���� Begin �� End ��������
		FTLINLINE virtual void OnJobBegin(LONG nJobIndex, CFJobBase<T>* pJob )
		{
			UNREFERENCED_PARAMETER(nJobIndex);
			UNREFERENCED_PARAMETER(pJob);
			FTLTRACEEX(FTL::tlInfo, TEXT("IFThreadPoolCallBack[0x%x]::OnJobBegin[%d]\n"), this, nJobIndex);
		} 
		FTLINLINE virtual void OnJobEnd(LONG nJobIndex, CFJobBase<T>* pJob)
		{
			UNREFERENCED_PARAMETER(nJobIndex);
			UNREFERENCED_PARAMETER(pJob);
			FTLTRACEEX(FTL::tlInfo, TEXT("IFThreadPoolCallBack[0x%x]::OnJobEnd[%d]\n"), this, nJobIndex);
		}

		//�����δ��������״̬�ͱ�ȡ����Job������Pool�����������
		FTLINLINE virtual void OnJobCancel(LONG nJobIndex, CFJobBase<T>* pJob)
		{
			UNREFERENCED_PARAMETER(nJobIndex);
			UNREFERENCED_PARAMETER(pJob);
			FTLTRACEEX(FTL::tlInfo, TEXT("IFThreadPoolCallBack[0x%x]::OnJobCancel[%d]\n"), this, nJobIndex);
		}

		//Progress �� Error �� JobBase �����༤��
		FTLINLINE virtual void OnJobProgress(LONG nJobIndex , CFJobBase<T>* pJob, LONGLONG nCurPos, LONGLONG nTotalSize)
		{
			UNREFERENCED_PARAMETER(nJobIndex);
			UNREFERENCED_PARAMETER(pJob);
			UNREFERENCED_PARAMETER(nCurPos);
			UNREFERENCED_PARAMETER(nTotalSize);
		}
		FTLINLINE virtual void OnJobError(LONG nJobIndex , CFJobBase<T>* pJob, DWORD dwError, LPCTSTR pszDescription)
		{
			UNREFERENCED_PARAMETER(nJobIndex);
			UNREFERENCED_PARAMETER(pJob);
			UNREFERENCED_PARAMETER(dwError);
			UNREFERENCED_PARAMETER(pszDescription);
			FTLTRACEEX(FTL::tlError, TEXT("IFThreadPoolCallBack[0x%x]::OnJobError[%d], dwError=%d, pszDesc=%s\n"), 
				this, nJobIndex, dwError, pszDescription);
		}
	};

	FTLEXPORT template <typename T>  
	class CFThreadPool
	{
		//typedef CFSharePtr<CFJobBase< T> > CFJobBasePtr;
		//friend class CFJobBasePtr;
		friend class CFJobBase<T>;  //����Job�� GetJobWaitType �л�ȡ m_hEventStop/m_hEventContinue
		DISABLE_COPY_AND_ASSIGNMENT(CFThreadPool);
	public:
		FTLINLINE CFThreadPool(IFThreadPoolCallBack<T>* pCallBack = NULL, LONG nMaxWaitingJobs = LONG_MAX);
		FTLINLINE virtual ~CFThreadPool(void);

		//! ��ʼ�̳߳�,��ʱ�ᴴ�� nMinNumThreads ���̣߳�Ȼ�������������� nMinNumThreads -- nMaxNumThreads ֮�����е����̵߳ĸ���
		FTLINLINE BOOL Start(LONG nMinNumThreads, LONG nMaxNumThreads);

		FTLINLINE BOOL GetThreadsCount(LONG* pMinNumThreads, LONG* pMaxNumThreads) const;

		//! ��̬�����̸߳������������Ϊ -1����ʾ���ı��Ӧ���߳���
		//! ע�⣺����Ǽ����̸߳������ҵ�ǰ�������е�Jobռ�����̣߳�����ǿ��ȡ���̵߳�ִ��
		FTLINLINE BOOL SetThreadsCount(LONG nMinNumThreads, LONG nMaxNumThreads);

		//! ����ֹͣ�̳߳�
		//! ע�⣺
		//!   1.ֻ������StopEvent����ҪJob����GetJobWaitType���� 
		//!   2.���������ǰע��ĵ���δ���еĹ����������Ҫɾ������Ҫ����ClearUndoWork
		FTLINLINE BOOL Stop();

		FTLINLINE BOOL StopAndWait(DWORD dwTimeOut = FTL_MAX_THREAD_DEADLINE_CHECK);

		//! �ȴ������̶߳��������ͷ�Start�з�����߳���Դ
		FTLINLINE BOOL Wait(DWORD dwTimeOut = FTL_MAX_THREAD_DEADLINE_CHECK);

		//! �����ǰδ��ɵĹ�����
		FTLINLINE BOOL ClearUndoWork();
		
		//! ��ȡ��ǰ�̳߳ص�����״̬�����ص�ǰ�������е�Job�͵ȴ����е�Job����
		FTLINLINE BOOL GetRunningStatus(INT* pDoingJobCount, INT* pWaitingJobCount);

		//! ���̳߳���ע�Ṥ�� -- �����ǰû�п��е��̣߳����ҵ�ǰ�߳���С������߳���������Զ������µ��̣߳�
		//! �ɹ����ͨ�� outJobIndex ����Job�������ţ���ͨ����������λ��ȡ���ض���Job
        //! bCheckDuplicate -- �Ƿ�����ظ��Լ��(JobIndex)
        //! ���صĴ����룺
        //    ERROR_ALREADY_EXISTS -- bCheckDuplicate ΪTRUEʱ��⵽��ͬ��Job
        //    ERROR_CANCELLED -- ����Jobʱ�̳߳�ֹͣ
        //    ERROR_TIMEOUT -- ��ʱ
		FTLINLINE BOOL SubmitJob(CFJobBase<T>* pJob, LONG* pOutJobIndex, BOOL bCheckDuplicate = FALSE, DWORD dwMilliseconds = INFINITE);

		//! TODO:���ȡ��Job���ͻ������ܵ����ߵõ�ָ��ʱ��Jobִ����� delete this�����ճ�Ұָ���쳣
		FTLINLINE BOOL PauseJob(LONG nJobIndex);
		FTLINLINE BOOL ResumeJob(LONG nJobIndex);
		//! ȡ��ָ����Job,
		FTLINLINE BOOL CancelJob(LONG nJobIndex);

		FTLINLINE BOOL GetJobPriority(LONG nJobIndex, LONG& nJobPriority);
		//! ��̬����Job�����ȼ�
		FTLINLINE BOOL SetJobPriority(LONG nJobIndex, LONG nNewPriority);

		//! ������ͣ�̳߳صĲ���
		FTLINLINE BOOL PauseAll();

		//! ��������̳߳صĲ���
		FTLINLINE BOOL ResumeAll();

		//! �Ƿ��Ѿ�������ֹͣ�̳߳�
		FTLINLINE BOOL HadRequestStop() const;
	protected:
		//! �������е��߳�,��� ��ǰ�߳��� + nThreadNum <= m_nMaxNumThreads ʱ ��ɹ�ִ��
		FTLINLINE BOOL _AddJobThread(LONG nThreadNum);
		FTLINLINE void _DestroyPool();
		FTLINLINE void _DoJobs();

		FTLINLINE GetJobType _GetJob(CFJobBase<T>** ppJob);

		FTLINLINE void _NotifyJobBegin(CFJobBase<T>* pJob);
		FTLINLINE void _NotifyJobEnd(CFJobBase<T>* pJob);
		FTLINLINE void _NotifyJobCancel(CFJobBase<T>* pJob);

		FTLINLINE void _NotifyJobProgress(CFJobBase<T>* pJob, LONGLONG nCurPos, LONGLONG nTotalSize);
		FTLINLINE void _NotifyJobError(CFJobBase<T>* pJob, DWORD dwError, LPCTSTR pszDescription); 

		typedef BOOL (CALLBACK CFThreadPool::*HandleJobProc)(CFJobBase<T>* pJob, BOOL bFoundInWaiting, LONG_PTR param);
		FTLINLINE BOOL _FindAndHandleSpecialJob(LONG nJobIndex, HandleJobProc pProc, LONG_PTR param);
		FTLINLINE BOOL CALLBACK _InnerPauseJob(CFJobBase<T>* pJob, BOOL bFoundInWaiting, LONG_PTR param);
		FTLINLINE BOOL CALLBACK _InnerResumeJob(CFJobBase<T>* pJob, BOOL bFoundInWaiting, LONG_PTR param);
		FTLINLINE BOOL CALLBACK _InnerCancelJob(CFJobBase<T>* pJob, BOOL bFoundInWaiting, LONG_PTR param);
		FTLINLINE BOOL CALLBACK _InnerGetJobPriority(CFJobBase<T>* pJob, BOOL bFoundInWaiting, LONG_PTR param);
		FTLINLINE BOOL CALLBACK _InnerSetJobPriority(CFJobBase<T>* pJob, BOOL bFoundInWaiting, LONG_PTR param);
	protected:
		LONG m_nMinNumThreads;					//! �̳߳������ٵ��̸߳���
		LONG m_nMaxNumThreads;					//! �̳߳��������̸߳���
        LONG m_nMaxWaitingJobs;                 //! �ȴ������е�����̸߳���
		IFThreadPoolCallBack<T>* m_pCallBack;	//! �ص��ӿ�
		LONG m_nJobIndex;						//! Job��������ÿ SubmitJob һ�Σ������1

		LONG m_nRunningJobNumber;				//! ��ǰ�������е�Job����
		LONG m_nRunningThreadNum;				//! ��ǰ�����ŵ��̸߳���(���������е��߳̽���ʱ���� Complete �¼�)

		typedef std::map<DWORD, HANDLE>   TaskThreadContrainer;
		TaskThreadContrainer m_TaskThreads;
		//TODO: ������ list���������Է��㶯̬���������С�̵߳ĸ��� ?
		//HANDLE* m_pJobThreadHandles;            //! �����߳̾��������
		//DWORD*  m_pJobThreadIds;                //! �����߳� Id ������(Ϊ�����߳̽�������������е�λ��)

		//HANDLE	m_hMgrThread;					//! Pool�����̵߳ľ��

		//! ����ȴ�Job����Ϣ�����������ȼ������⣬����һ���Ǵ���ǰ�濪ʼȡ����˱���ɰ��� WaitingJobSorter �����map
		typedef std::map<WaitingJobSorter, CFJobBase<T> * > WaitingJobContainer;
		typedef std::map<LONG, CFJobBase<T>* >	IndexToJobContainer;

		WaitingJobContainer		m_WaitingJobs;	//! �ȴ����е�Job
		IndexToJobContainer		m_WaitingIndexJobs;

		//! ��������Job����Ϣ�� ���ڻ�Ƶ�����롢ɾ��������Ҫ����JobIndex���ң���˱���� map
		IndexToJobContainer		m_DoingJobs;	//! �������е�Job

		HANDLE m_hEventStop;                    //! ֹͣPool���¼�
		HANDLE m_hEventAllThreadComplete;		//! ���е��̶߳�����ʱ��������¼�
		//HANDLE m_hEventContinue;				//! ����Pool�������е��¼�
        HANDLE m_hSemaphoreWaitingPos;          //! ����ȴ������л����Էŵ�Job������ÿȡ��һ��Job������1��ÿSubmitһ����ȥ�ͼ�1
		HANDLE m_hSemaphoreJobToDo;             //! ���滹�ж��ٸ�Job���ź���,ÿSubmitһ��Job,������һ��
		HANDLE m_hSemaphoreSubtractThread;      //! ���ڼ����̸߳���ʱ���ź���,��ʼʱ����Ϊ0,ÿҪ�ͷ�һ����������һ����

		CFCriticalSection m_lockDoingJobs;		//���� m_DoingJobs ʱ����
		CFCriticalSection m_lockWaitingJobs;    //���� m_WaitingJobs ʱ����
		CFCriticalSection m_lockThreads;        //���� m_pJobThreadHandles/m_pJobThreadIds ʱ����

		static unsigned int CALLBACK JobThreadProc(void *pParam);    //! �����̵߳�ִ�к���
		//static unsigned int CALLBACK MgrThreadProc(void* pParam);	 //! �����̵߳�ִ�к���
	};
}

#endif //FTL_THREADPOOL_H

#ifndef USE_EXPORT
#  include "ftlThreadPool.hpp"
#endif