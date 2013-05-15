// ThreadPoolPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FTLDemo.h"
#include "ThreadPoolPage.h"

// CThreadPoolPage �Ի���
IMPLEMENT_DYNAMIC(CThreadPoolPage, CPropertyPage)

class CFTLPoolJob : public FTL::CFJobBase<CThreadPoolPage*>
{
public:
	virtual BOOL Run()
	{
		FTLTRACE(TEXT("CFTLPoolJob Run, Thread id = %d\n"),GetCurrentThreadId());
		LONG lCount = 5;
		while(ftwtContinue == GetJobWaitType(INFINITE) && lCount > 0)
		{
			FUNCTION_BLOCK_NAME_TRACE(TEXT("In CFTLPoolJob::Run"),0);
			Sleep(200);
			lCount--;
		}
		return TRUE;
	}
	virtual VOID OnFinalize()
	{
		delete this;
	}
	virtual void OnCancelJob()
	{
		FUNCTION_BLOCK_TRACE(0);
		delete this;
	}
};

CThreadPoolPage::CThreadPoolPage()
	: CPropertyPage(CThreadPoolPage::IDD)
	, m_FtlThreadPool(NULL, 2)
	, m_nFtlCurJobIndex(0)
{
	m_CurWorkItemCount = 0;
    m_nHighJobPriority = 0;
    m_nLowJobPriority = 0;
}

CThreadPoolPage::~CThreadPoolPage()
{
}

void CThreadPoolPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CThreadPoolPage, CPropertyPage)
    ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_FTL_THREAD_POOL_START, &CThreadPoolPage::OnBnClickedBtnFtlThreadPoolStart)
	ON_BN_CLICKED(IDC_BTN_FTL_THREAD_POOL_ADD_JOB_LOW, &CThreadPoolPage::OnBnClickedBtnFtlThreadPoolAddJobLow)
    ON_BN_CLICKED(IDC_BTN_FTL_THREAD_POOL_ADD_JOB_HIGH, &CThreadPoolPage::OnBnClickedBtnFtlThreadPoolAddJobHigh)
	ON_BN_CLICKED(IDC_BTN_FTL_THREAD_POOL_CANCEL_JOB, &CThreadPoolPage::OnBnClickedBtnFtlThreadPoolCancelJob)
	ON_BN_CLICKED(IDC_BTN_FTL_THREAD_POOL_STOP, &CThreadPoolPage::OnBnClickedBtnFtlThreadPoolStop)
	ON_BN_CLICKED(IDC_BTN_FTL_THREAD_POOL_PAUSE_RESUME, &CThreadPoolPage::OnBnClickedBtnFtlThreadPoolPauseResume)

	ON_BN_CLICKED(IDC_BTN_ATL_THREAD_POOL_START, &CThreadPoolPage::OnBnClickedBtnAtlThreadPoolStart)
	ON_BN_CLICKED(IDC_BTN_ATL_THREAD_POOL_ADD_JOB, &CThreadPoolPage::OnBnClickedBtnAtlThreadPoolAddJob)
	ON_BN_CLICKED(IDC_BTN_ATL_THREAD_POOL_STOP, &CThreadPoolPage::OnBnClickedBtnAtlThreadPoolStop)
	ON_BN_CLICKED(IDC_BTN_THREADPOOL_QUEUE_USER_WORK_ITEM, &CThreadPoolPage::OnBnClickedBtnThreadpoolQueueUserWorkItem)

#if CHECK_VISTA_THREAD_POOL
    ON_BN_CLICKED(IDC_BTN_VISTA_CREATE_THREAD_POOL, &CThreadPoolPage::OnBnClickedBtnVistaCreateThreadPool)
    ON_BN_CLICKED(IDC_BTN_VISTA_CLOSE_THREAD_POOL, &CThreadPoolPage::OnBnClickedBtnVistaCloseThreadPool)
    ON_BN_CLICKED(IDC_BTN_VISTA_CREATE_THREADPOOL_WORK, &CThreadPoolPage::OnBnClickedBtnVistaCreateThreadpoolWork)
    ON_BN_CLICKED(IDC_BTN_VISTA_CREATE_THREADPOOL_TIMER, &CThreadPoolPage::OnBnClickedBtnVistaCreateThreadpoolTimer)
#endif 
END_MESSAGE_MAP()

// CThreadPoolPage ��Ϣ�������

void CThreadPoolPage::OnDestroy()
{
	m_FtlThreadPool.StopAndWait();

    CPropertyPage::OnDestroy();
}

void CThreadPoolPage::SetFtlThreadPoolButtonStatus(BOOL bStarted, BOOL bPaused)
{
	GetDlgItem(IDC_BTN_FTL_THREAD_POOL_START)->EnableWindow(!bStarted);
	GetDlgItem(IDC_BTN_FTL_THREAD_POOL_ADD_JOB_LOW)->EnableWindow(bStarted);
    GetDlgItem(IDC_BTN_FTL_THREAD_POOL_ADD_JOB_HIGH)->EnableWindow(bStarted);
	GetDlgItem(IDC_BTN_FTL_THREAD_POOL_CANCEL_JOB)->EnableWindow(bStarted);
	GetDlgItem(IDC_BTN_FTL_THREAD_POOL_PAUSE_RESUME)->EnableWindow(bStarted);
	GetDlgItem(IDC_BTN_FTL_THREAD_POOL_STOP)->EnableWindow(bStarted);

	GetDlgItem(IDC_BTN_FTL_THREAD_POOL_PAUSE_RESUME)->SetWindowText( bPaused ? TEXT("Resume") : TEXT("Pause"));
}

void CThreadPoolPage::OnBnClickedBtnFtlThreadPoolStart()
{
	m_FtlThreadPool.Start(0, 6);
	SetFtlThreadPoolButtonStatus(TRUE,FALSE);
}

void CThreadPoolPage::OnBnClickedBtnFtlThreadPoolAddJobLow()
{
    m_nLowJobPriority++;
	CFTLPoolJob* pNewJob = new CFTLPoolJob();
	pNewJob->m_JobParam = this;
    pNewJob->SetJobPriority(m_nLowJobPriority);
	m_FtlThreadPool.SubmitJob(pNewJob, &m_nFtlCurJobIndex);
}

void CThreadPoolPage::OnBnClickedBtnFtlThreadPoolAddJobHigh()
{
    //add a higher priority job
    m_nHighJobPriority--;
    CFTLPoolJob* pNewJob = new CFTLPoolJob();
    pNewJob->m_JobParam = this;
    pNewJob->SetJobPriority(m_nHighJobPriority);
    m_FtlThreadPool.SubmitJob(pNewJob, &m_nFtlCurJobIndex);
}

void CThreadPoolPage::OnBnClickedBtnFtlThreadPoolCancelJob()
{
	m_FtlThreadPool.CancelJob(--m_nFtlCurJobIndex);
}

void CThreadPoolPage::OnBnClickedBtnFtlThreadPoolStop()
{
	FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
	m_FtlThreadPool.StopAndWait();
	SetFtlThreadPoolButtonStatus(FALSE, m_FtlThreadPool.HadRequestPause());
}

void CThreadPoolPage::OnBnClickedBtnFtlThreadPoolPauseResume()
{
	if(m_FtlThreadPool.HadRequestPause())
	{
		m_FtlThreadPool.Resume();
	}
	else
	{
		m_FtlThreadPool.Pause();
	}
	SetFtlThreadPoolButtonStatus(TRUE, m_FtlThreadPool.HadRequestPause());
}

//ATL Thread Pool
void CThreadPoolPage::SetAtlThreadPoolButtonStatus(BOOL bStarted, BOOL bPaused)
{
    UNREFERENCED_PARAMETER(bPaused);

	GetDlgItem(IDC_BTN_ATL_THREAD_POOL_START)->EnableWindow(!bStarted);
	GetDlgItem(IDC_BTN_ATL_THREAD_POOL_ADD_JOB)->EnableWindow(bStarted);
	GetDlgItem(IDC_BTN_ATL_THREAD_POOL_STOP)->EnableWindow(bStarted);
}

void CThreadPoolPage::OnBnClickedBtnAtlThreadPoolStart()
{
	m_AtlThreadPool.Initialize((void*)321, 3);
	SetAtlThreadPoolButtonStatus(TRUE, FALSE);
}

void CThreadPoolPage::OnBnClickedBtnAtlThreadPoolAddJob()
{
	m_AtlThreadPool.QueueRequest( (CATLPoolJob::RequestType) new CATLPoolJob());
}

void CThreadPoolPage::OnBnClickedBtnAtlThreadPoolStop()
{
	FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
	m_AtlThreadPool.Shutdown(0);
	SetAtlThreadPoolButtonStatus(FALSE, FALSE);
}


DWORD CThreadPoolPage::QueueUserWorkItemDemoProc( LPVOID lpThreadParameter )
{
	CThreadPoolPage* pThis = reinterpret_cast<CThreadPoolPage*>(lpThreadParameter);
	DWORD dwCurrentThreadId = GetCurrentThreadId();

	FTLTRACE(TEXT("QueueUserWorkItemDemoProc [%d] Enter, CurWorkItem=%ld\n"), dwCurrentThreadId, pThis->m_CurWorkItemCount);

	{
		CFAutoLock<CFLockObject>	lockObj(&pThis->m_csLock);
		ThreadIdSet::iterator iter = pThis->m_ThreadIdSetOfQueueUserWorkItem.find(dwCurrentThreadId);
		if (iter == pThis->m_ThreadIdSetOfQueueUserWorkItem.end())
		{
			pThis->m_ThreadIdSetOfQueueUserWorkItem.insert(dwCurrentThreadId);
		}

		FTLTRACE(TEXT(">>>QueueUserWorkItemDemoProc, different Thread Id count is %d\n"), pThis->m_ThreadIdSetOfQueueUserWorkItem.size());
	}

	int nCount = 10;
	while (nCount > 0)
	{
		nCount--;
		Sleep(500);
	}
	LONG nCurWorkItemCount = InterlockedDecrement(&pThis->m_CurWorkItemCount);
	FTLTRACE(TEXT("QueueUserWorkItemDemoProc [%d] Leave, CurWorkItem=%ld\n"), dwCurrentThreadId, nCurWorkItemCount);

	return 0;
}

void CThreadPoolPage::OnBnClickedBtnThreadpoolQueueUserWorkItem()
{
	//��ȷʹ���̳߳صķ����ǣ�������Ŀ�����Լ���������õ������к������������̳߳ذ�ȫ�ġ���ȫ�ĺ�����Ӧ�ü����߳���һ�����̵߳Ļ����������̡߳�
	//һ����˵��Ӧ�ñ���ʹ���̱߳��ش洢�ͷ�����Ҫ�����̵߳��첽IO���ã�����˵RegNotifyChangeKeyValue������
	//�����Ҫ�������߳���ִ�������ĺ����Ļ������Ը�QueueUserWorkItem����һ��ѡ��WT_EXECUTEINPERSISTENTTHREAD��

	//QueueUserWorkItem ����������ȴ����У�Ȼ�����ε���ִ�У���һ�ε���ʱ�ᴴ���̳߳�( 1 ���� + 1 ����)
	//���Ų�����������Ӷ������̸߳�������ǰͬʱ���е��̸߳�����Լ�� (CPUCOUNT * 4 - 1) ����, �ҵĻ�����˫��CPU�����ͬʱ����7�������̡߳�
	//��û������һ��ʱ��󣬻��ͷ��߳�(�����̲߳����˳�)
	//�������������ȡ��
	BOOL bRet = FALSE;
	LONG nCurWorkItemCount = InterlockedIncrement(&m_CurWorkItemCount);
    UNREFERENCED_PARAMETER(nCurWorkItemCount);
	API_VERIFY(QueueUserWorkItem(QueueUserWorkItemDemoProc, this, WT_EXECUTEDEFAULT));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Vista ֮����̳߳�
//  Callback Environment -- �ص�����
//  Cleanup Group -- ���ڸ���һ�������̳߳ػص�
//    CreateThreadpoolCleanupGroup ->   SetThreadpoolCallbackCleanupGroup(�����ص������������) ->  CloseThreadpoolCleanupGroupMembers -> CloseThreadpoolCleanupGroup

//�ŵ㣺
//  1.
//ȱ�㣺
//  n.��Ϊ work, I/O, timer, wait objects �����࣬��úܸ��ӵ�����
//  1.�ƺ�Work�����Ժ��̲߳����Զ�����(��������10����)
//  n.CloseThreadPool ʱ����������񣬲����������������ǲ����첽�ķ�ʽ�������񶼽�������ͷ��߳�
//  

#if CHECK_VISTA_THREAD_POOL
VOID CALLBACK MyWaitCallback(PTP_CALLBACK_INSTANCE Instance,PVOID Parameter, PTP_WAIT Wait,TP_WAIT_RESULT WaitResult)
{
    //
    // Do something when the wait is over
    //
}


VOID CALLBACK MyTimerCallback(PTP_CALLBACK_INSTANCE Instance, PVOID Parameter, PTP_TIMER Timer)
{
    FTLTRACE(TEXT("[%d] MyTimerCallback Enter\n"), GetCurrentThreadId());
    for (int i = 0; i < 10; i++)
    {
        //FTLTRACE(TEXT("In MyWorkCallback %d\n"), i);
        Sleep(500);
    }
    FTLTRACE(TEXT("[%d] MyTimerCallback Leave\n"), GetCurrentThreadId());
}

VOID CALLBACK MyWorkCallback(PTP_CALLBACK_INSTANCE Instance, PVOID Parameter, PTP_WORK Work)
{
    FTLTRACE(TEXT("[%d] MyWorkCallback Enter\n"), GetCurrentThreadId());
    for (int i = 0; i < 10; i++)
    {
        //FTLTRACE(TEXT("In MyWorkCallback %d\n"), i);
        Sleep(500);
    }
    FTLTRACE(TEXT("[%d] MyWorkCallback Leave\n"), GetCurrentThreadId());
}

void CThreadPoolPage::SetVistaThreadPoolButtonStatus(BOOL bStarted, BOOL bPaused)
{
    GetDlgItem(IDC_BTN_VISTA_CREATE_THREAD_POOL)->EnableWindow(!bStarted);
    GetDlgItem(IDC_BTN_VISTA_CREATE_THREADPOOL_WORK)->EnableWindow(bStarted);
    GetDlgItem(IDC_BTN_VISTA_CREATE_THREADPOOL_TIMER)->EnableWindow(bStarted);
    GetDlgItem(IDC_BTN_VISTA_CLOSE_THREAD_POOL)->EnableWindow(bStarted);

    //GetDlgItem(IDC_BTN_FTL_THREAD_POOL_PAUSE_RESUME)->SetWindowText( bPaused ? TEXT("Resume") : TEXT("Pause"));
}

void CThreadPoolPage::OnBnClickedBtnVistaCreateThreadPool()
{
    BOOL bRet = FALSE;
    PTP_WORK work = NULL;
    PTP_TIMER timer = NULL;
    //PTP_POOL pool = NULL;
    UINT rollback = 0;

    InitializeThreadpoolEnvironment(&m_CallBackEnviron);


    API_VERIFY((m_pVistaThreadPool = CreateThreadpool(NULL)) != NULL);
    if (bRet)
    {
        API_VERIFY(SetThreadpoolThreadMinimum(m_pVistaThreadPool, 0));
        SetThreadpoolThreadMaximum(m_pVistaThreadPool, 4);

        //Create a cleanup group for this thread pool
        API_VERIFY((m_pVistaCleanupGroup = CreateThreadpoolCleanupGroup()) != NULL);
        if (bRet)
        {
            //Associate the callback environment with our thread pool
            SetThreadpoolCallbackPool(&m_CallBackEnviron, m_pVistaThreadPool);
            SetThreadpoolCallbackCleanupGroup(&m_CallBackEnviron, m_pVistaCleanupGroup, NULL);
            
            SetVistaThreadPoolButtonStatus(TRUE, FALSE);
        }
    }
}

void CThreadPoolPage::OnBnClickedBtnVistaCreateThreadpoolWork()
{
    BOOL bRet = FALSE;
    PTP_WORK pWork = NULL;
    API_VERIFY((pWork = CreateThreadpoolWork((PTP_WORK_CALLBACK) MyWorkCallback,  NULL, &m_CallBackEnviron)) != NULL);
    if (pWork)
    {
        //Submit the work to the pool. Because this was a pre-allocated work item (using CreateThreadpoolWork), it is guaranteed to execute
        SubmitThreadpoolWork(pWork);

        //���ڴ�����Work�����Զ��Submit
        //SubmitThreadpoolWork(pWork);
    }
}
void CThreadPoolPage::OnBnClickedBtnVistaCreateThreadpoolTimer()
{
    BOOL bRet = FALSE;
    PTP_TIMER pTimer = NULL;
    API_VERIFY((pTimer = CreateThreadpoolTimer((PTP_TIMER_CALLBACK) MyTimerCallback, NULL, &m_CallBackEnviron)) != NULL);
    if (pTimer)
    {
        ULARGE_INTEGER ulDueTime;
        FILETIME FileDueTime;

        ulDueTime.QuadPart = (LONGLONG) -(1 * 10 * 1000 * 1000);
        FileDueTime.dwHighDateTime = ulDueTime.HighPart;
        FileDueTime.dwLowDateTime  = ulDueTime.LowPart;

        SetThreadpoolTimer(pTimer, &FileDueTime, 0,0);
    }
}

void CThreadPoolPage::OnBnClickedBtnVistaCloseThreadPool()
{
	if (m_pVistaCleanupGroup)
	{
		CloseThreadpoolCleanupGroupMembers(m_pVistaCleanupGroup, TRUE, NULL);
		CloseThreadpoolCleanupGroup(m_pVistaCleanupGroup);
		m_pVistaCleanupGroup = NULL;
	}
	if (m_pVistaThreadPool)
	{
		CloseThreadpool(m_pVistaThreadPool);
		m_pVistaThreadPool = NULL;
	}
	SetVistaThreadPoolButtonStatus(FALSE, FALSE);
}

#endif //CHECK_VISTA_THREAD_POOL

