#ifndef FTL_IOCP_H
#define FTL_IOCP_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlIocp.h requires ftlbase.h to be included first
#endif

/*************************************************************************************************************************
* IOCP(IO Completion Port -- ��ɶ˿�) -- һ��֪ͨ���У���Overlapped I/OЭͬ������������һ���������ڶ�ľ����������I/O�ĳ���(��Web������)���л��������ܡ�
*   û��64��HANDLE�����ƣ�ʹ��һ���߳�(ͨ��������Ϊ CPU����*2+2 )����һ��Events�����ʣ��Զ�֧�� scalable��
*   ����ϵͳ���Ѿ���ɵ��ص�I/O�����֪ͨ������ɶ˿ڵ�֪ͨ����(һ���߳̽�һ��������ʱ��������)���ȴ���һ���߳�Ϊ����ʵ�ʷ���
*   �̳߳ص������߳� = Ŀǰ����ִ�е� + �������� + ����ɶ˿��ϵȴ��ġ�
*   ������һ�������첽IO�������ʱ�Ļص��������̳߳�,ĿǰWindowsƽ̨��������õ��������ģ�͡�
*   ΢�����ӣ�web\Wininet\Async
*   ʹ�����̣�
*     1.����һ��I/O completion port -- hPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL...)������һ��û�к��κ��ļ�Handle�й�ϵ��port
*     2.�������ļ�handle�������� -- CreateIoCompletionPort(hFile,hPort...)��Ϊÿһ�������ӵ��ļ�Handle���й���
*     3.����һ���߳� -- for(CPU*2+2){ _beginthreadex }
*     4.��ÿһ���߳���Completion Port�ϵȴ������غ����������д���
*       GetQueuedCompletionStatus(,INFINITE)����ͨ�� CONTAINING_RECORD ��ȡ������ָ����չ�ṹ��ָ��
*     5.��ʼ�����Ǹ��ļ�handle����һЩoverlapped I/O���� -- �� ReadFile��WriteFile��DeviceIoControl�ȣ�
*     6.�����߳������Ҫ����(��֪ͨ�������)����Ҫͨ�� PostQueuedCompletionStatus ����ɶ˿ڷ����¼�֪ͨ���Ӷ����Ѹ����߳�,
*       Ȼ�� WaitForSingleObject(hPort, ��ʱʱ��)
*   ע�⣺
*     1.����ص���������ʧ��ʱ(����ֵ�����Ҵ���ԭ����ERROR_IO_PENDING)����ô��ɶ˿ڽ������յ��κ����֪ͨ��
*       ����ص��������óɹ������ߴ���ԭ����ERROR_IO_PENDING�Ĵ���ʱ����ɶ˿ڽ������ܹ��յ����֪ͨ��
*
*   BindIoCompletionCallback -- �ڲ���װ��IOCP�Ĺ��ܣ�ͨ�����õĻص���������IO��
*     ͨ������ص�����Ҫ���� RtlNtStatusToDosError ���ڲ���NTStatus��ת����Windows�Ĵ�����(Ring0 -> Ring3)
*     ?���ʵ��(��������IOCP)?--�ڻص������и��ݲ�������(�������ɻ������)���� QueueUserWorkItem 
*        ����Ӧ������WorkItem�����̳߳�
*************************************************************************************************************************/

namespace FTL
{
    //ǰ������
    class CFIocpBaseTask;
    class CFIocpMgr;


    typedef struct tagF_IOCP_OVERLAPPED
    {
        tagF_IOCP_OVERLAPPED()
        {
            ZeroMemory(&overLapped, sizeof(overLapped));
        }
        OVERLAPPED  overLapped;
    }F_IOCP_OVERLAPPED, *LPF_IOCP_OVERLAPPED;

    class CFIocpBaseTask
    {
    public:
        F_IOCP_OVERLAPPED   m_OverLapped;
    public:
        virtual HANDLE GetIocpHandle() const = 0;
        virtual BOOL   OnIoComplete(CFIocpMgr* pIocpMgr, F_IOCP_OVERLAPPED* pIocpOverLapped, DWORD dwBytesTransferred) = 0;
    };

    //class CFSocketIocpTask : public CFIocpBaseTask
    //{
    //public:
    //    FTLINLINE virtual HANDLE GetIocpHandle() const;
    //protected:
    //    SOCKET  m_nSocket;
    //};

    //class CFFileIocpTask : public CFIocpBaseTask
    //{
    //public:
    //    FTLINLINE virtual HANDLE GetIocpHandle() const;
    //protected:
    //    HANDLE m_hHandle;
    //};

    class CFIocpMgr
    {
    public:
        enum 
        {
            DEFAULT_THREAD_COUNT = 0,
        };

        FTLINLINE CFIocpMgr();
        FTLINLINE ~CFIocpMgr();

        //0 is default, will create as 2 * CPU + 2
        FTLINLINE BOOL Start(DWORD NumberOfConcurrentThreads = DEFAULT_THREAD_COUNT);
        FTLINLINE BOOL Stop();
        FTLINLINE VOID Close(DWORD dwMilliseconds = INFINITE);

        //������Ҫ�ȴ���ɵ��ص�IO�����ľ��(�� File��SOCKET��Pipe ��)
        FTLINLINE BOOL AddTask(CFIocpBaseTask* pTask);

    protected:
        //�����̳߳�ʼ������ֹ�������Խ���COM��ʼ����
        FTLINLINE virtual BOOL OnWorkThreadInit();
        FTLINLINE virtual VOID OnWorkThreadFina();
    private:
        HANDLE  m_hIoCompletionPort;
        HANDLE* m_pWorkThreads;
        volatile BOOL m_bStop;

        DWORD   m_nConcurrentThreadCount;

        FTLINLINE DWORD _GetDefaultConcurrentThreadCount();
        FTLINLINE DWORD _InnerWorkThreadProc();

        FTLINLINE static DWORD __stdcall _WorkThreadProc(LPVOID pParam);
    };
}
#endif //FTL_IOCP_H

#ifndef USE_EXPORT
#  include "ftlIocp.hpp"
#endif 