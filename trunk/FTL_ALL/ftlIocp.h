#ifndef FTL_IOCP_H
#define FTL_IOCP_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlIocp.h requires ftlbase.h to be included first
#endif

#include "ftlThread.h"
#include "ftlMem.h"
#include "ftlSharePtr.h"
#include "ftlSocket.h"

/*************************************************************************************************************************
// http://www.cnblogs.com/lancidie/archive/2011/12/19/2293773.html
* IOCP(IO Completion Port -- ��ɶ˿�) -- һ��֪ͨ���У���Overlapped I/OЭͬ������������һ���������ڶ�ľ����������I/O�ĳ���(��Web������)���л��������ܡ�
*   û��64��HANDLE�����ƣ�ʹ��һ���߳�(ͨ��������Ϊ CPU����*2+2 )����һ��Events�����ʣ��Զ�֧�� scalable��
*   ����ϵͳ���Ѿ���ɵ��ص�I/O�����֪ͨ������ɶ˿ڵ�֪ͨ����(һ���߳̽�һ��������ʱ��������)���ȴ���һ���߳�Ϊ����ʵ�ʷ���
*   �̳߳ص������߳� = Ŀǰ����ִ�е� + �������� + ����ɶ˿��ϵȴ��ġ�
*   ������һ�������첽IO�������ʱ�Ļص��������̳߳�,ĿǰWindowsƽ̨��������õ��������ģ�͡�
*   ΢�����ӣ�web\Wininet\Async
*   ʹ�����̣�
*     1.����һ��I/O completion port -- hPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL...)������һ��û�к��κ��ļ�Handle�й�ϵ��port
*     2.����һ���߳� -- һ���� for(CPU*2+2){ _beginthreadex }
*     3.����ɶ˿ں��ļ�handle��Socket����Ȳ������� -- CreateIoCompletionPort(hFile,hPort...)��Ϊÿһ�������ӵ��ļ�Handle���й���
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

    FTLINLINE LONGLONG AddOverLappedOffset(LPOVERLAPPED pOverlapped, DWORD dwTransfered)
    {
        LARGE_INTEGER* pLargeInteger = reinterpret_cast<LARGE_INTEGER*>(&pOverlapped->Offset);
        pLargeInteger->QuadPart += dwTransfered;
        return pLargeInteger->QuadPart;
    }

    enum IocpOperationType 
    {
        //iotUnknown = -1,

	    iotInitialize,   // The client just connected
	    iotRead,         // Read from the client. 
	    //IOReadCompleted,// Read completed
	    iotWrite,        // Write to the Client
	    //IOWriteCompleted, // Write Completed.
	    //IOZeroByteRead, // Read zero Byte from client (dummy for avoiding The System Blocking error) 
	    //IOZeroReadCompleted, // Read Zero Byte  completed. (se IOZeroByteRead)
	    //IOTransmitFileCompleted,
    };

    class CFIocpBuffer
    {
    public:
        OVERLAPPED          m_overLapped;
        //HANDLE              m_hIoHandle;
        IocpOperationType   m_operType;
        LONG            m_nSequenceNumber;
        PBYTE	        m_pBuffer;
        DWORD           m_dwSize;
        //DWORD           m_dwUsed;
    public:
        FTLINLINE CFIocpBuffer();
        FTLINLINE ~CFIocpBuffer();
        FTLINLINE VOID Reset(DWORD dwSize);
    };
    typedef CFMemoryPoolObjectHelper<CFIocpBuffer> CFIoBufferPoolObject;
    typedef CFSharePtr<CFIoBufferPoolObject>    CFIoBufferPoolObjectPtr;

    class CFIocpBaseTask
    {
        friend class CFIocpMgr;
    public:
        typedef std::map<LONG, CFIoBufferPoolObjectPtr> IocpBufferMap;  //����������ͬʱͶ�ݶ��IO����
        FTLINLINE CFIocpBaseTask();

        virtual INT    GetIocpHandleCount() const { return 1; }
        virtual HANDLE GetIocpHandle(INT index) const = 0;
        //virtual BOOL   OnIoComplete(CFIocpMgr* pIocpMgr, CFIoBufferPoolObjectPtr& pIoBuffer, DWORD dwBytesTransferred) = 0;
        virtual BOOL OnInitialize(CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred) { return TRUE; }
        virtual BOOL AfterReadCompleted(CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred) { return TRUE; }
        virtual BOOL AfterWriteCompleted(CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred) { return TRUE; }
        virtual BOOL OnUninitialize(CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred) { return TRUE; }

    private:
        LONG            m_nCurReadSequence;
        LONG            m_nCurWriteSequence;

        IocpBufferMap   m_ReadBufferMap;
        IocpBufferMap   m_WriteBufferMap;
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

    //template <typename T>
    class CFIocpMgr
    {
    public:
        enum 
        {
            DEFAULT_THREAD_COUNT = (DWORD)(-1),
        };

        FTLINLINE CFIocpMgr(DWORD dwBufferSize = 4096);
        FTLINLINE ~CFIocpMgr();

        //0 is default, will create as 2 * CPU + 2
        FTLINLINE BOOL Start(DWORD NumberOfConcurrentThreads = DEFAULT_THREAD_COUNT);
        FTLINLINE BOOL Stop();
        FTLINLINE VOID Close(DWORD dwMilliseconds = INFINITE);

        //������Ҫ�ȴ���ɵ��ص�IO�����ľ��(�� File��SOCKET��Pipe ��)
        FTLINLINE BOOL AssociateTask(CFIocpBaseTask* pTask);

    protected:
        //�����̳߳�ʼ������ֹ�������Խ���COM��ʼ����
        FTLINLINE virtual BOOL OnIocpStart() { return TRUE; }
        FTLINLINE virtual BOOL OnIocpStop() { return TRUE; }

        FTLINLINE virtual BOOL OnWorkThreadInit();
        FTLINLINE virtual VOID OnWorkThreadFina();
    private:
        HANDLE  m_hIoCompletionPort;
        HANDLE* m_pWorkThreads;
        DWORD    m_dwBufferSize;
        volatile BOOL m_bStop;

        CFMemoryPoolT<CFIocpBuffer> m_IocpBufferPool;

        DWORD   m_nConcurrentThreadCount;

        FTLINLINE BOOL  _InitlizeIocp(DWORD NumberOfConcurrentThreads);
 
        FTLINLINE DWORD _GetDefaultConcurrentThreadCount();
        FTLINLINE DWORD _InnerWorkThreadProc();
        FTLINLINE BOOL  _HandleError(CFIocpBaseTask* pIocpTask, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred, DWORD dwErr);

        FTLINLINE static DWORD __stdcall _WorkThreadProc(LPVOID pParam);
        FTLINLINE BOOL _ProcessPackage(CFIocpBaseTask* pTask, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred);
    };

    class CFIospServerSocket : public CFIocpBaseTask
        , public CFSocket
    {
        virtual INT GetIocpHandleCount() const
        {
            return 1;
        }

        virtual HANDLE GetIocpHandle( INT index ) const
        {
            return (HANDLE)m_socket;
        }

    };

    class CFIocpNetServer;

    class CFIocpListenTask : public CFIocpBaseTask
    {
    public:
        CFIocpListenTask(SOCKET socketListen)
        {
            m_socketListen = socketListen;
        }
        FTLINLINE virtual INT GetIocpHandleCount() const
        {
            return 1;
        }

        FTLINLINE virtual HANDLE GetIocpHandle( INT index ) const
        {
            FTLASSERT(index < 1);
            return (HANDLE)m_socketListen;
        }

        FTLINLINE virtual BOOL OnInitialize(CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred )
        {
            FTLASSERT(pIoBuffer->m_operType == iotInitialize);
            
            CFIospServerSocket* pSocket = new CFIospServerSocket();
            pIocpMgr->AssociateTask(pSocket);

            return TRUE;
        }
    private:
        SOCKET m_socketListen;
    };
    class CFIocpNetServer : public CFIocpMgr
    {
    public:
        FTLINLINE virtual BOOL OnIocpStart();
        FTLINLINE virtual BOOL OnIocpStop();
    protected:
        SOCKET      m_nSocketListen;
    };
}
#endif //FTL_IOCP_H

#ifndef USE_EXPORT
#  include "ftlIocp.hpp"
#endif 