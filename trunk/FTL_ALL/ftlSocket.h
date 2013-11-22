#ifndef FTL_SOCKET_H
#define FTL_SOCKET_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlSocket.h requires ftlbase.h to be included first
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>        //���� SO_CONNECT_TIME �Ⱥ궨�壬���ܻ������������?

#include "ftlNet.h"
#include "ftlIocp.h"

/*

��ô���֣�
             CFClientSocket/CFServerSocket
            / 
CFSocketBase
            \CFTCPSocket/CFUDPSocket
*/

/*************************************************************************************************************************
* 1.�Ƿ���ͨ�� select ���첽Accept?
* 2.��������Server��Ҫ����ͻ��˵�socket�б�����Ƕ���Ļ���ô����?
* 3.�ͻ��˵��� bind �ĺô���ʲô��UDP �ͻ���������ù� connect �Ļ�������ֱ��ͨ�� send/recv ��������ֻ�ܺ�һ���Զ˽����ˡ�
* 4.UDP ������ֻ��Ҫbind������Ҫlisten������ͨ�� recvfrom/sendto ֱ�ӽ��������Ƿ���Ҫaccept?
* 5.AF_INET6 ����linux�¶���Ϊ 10, �� Windows�¶���Ϊ 23? �����Ļ���ô���ݣ����Խ� ai_family ����Ϊ AF_UNSPEC(0)��Ȼ��ͨ���������������֣�
* 6.(ipv6.google.com Ϊʲô���ܷ��ʣ�) -- ��IPV6�����������Ļ�����ʹ�ͻ�����ʹ��IPV4��socket����Ҳ��������ͨѶ��IPV4�ĵ�ַ�ᱻת�������� ::ffff:192.168.1.1 �ĵ�ַ
*   IPV6�ķ������������IPV4�ĵ�ַ(������˫��Э��ջ)�����IPV4�ͻ����Ǽ��ݵ�
* 7.getaddrinfo �����ָ�� hints.family Ϊ AF_UNSPEC�������ʱ�᷵�ؿ��õ� IPV4/IPV6 ��ַ����������Զ����ݣ�
* 
*
* IN6_IS_ADDR_V4MAPPED -- �ж��Ƿ���IPV4�ĵ�ַӳ��ɵ�IPV6
*
* 
* ��ַ�ṹ
*   ����:sockaddr, family(2) + data(14) = 16
*   IPV4:sockaddr_in, family(2) + port(2) + sin_addr(4) + zero(8) = 16
*   IPV6:sockaddr_in6, family(2) + port(2) + flowinfo(4) + sin6_addr(16) [+ scope_id(4)] = 24[28]
*   Storage:sockaddr_storage -- family(2) + data(126?)�����Լ������еĵ�ַ�ṹ(IPV4/IPV6/Unix ��)
*     ���ṩ�ϸ�Ľṹ���룬������ϵͳ֧�ֵĸ���ĵ�ַ�ṹ��������
*     ʹ��ʱ��Ҫ����ǿ��ת���� sockaddr_in/sockaddr_in6 �����?
*     bind ����ʱ sizeof Ҳ֧�� storage
*   ??      sockaddr_storage_xp
* 
* addrinfo -- ͨ�õĵ�ַ��Ϣ?
* 
* ��Э�������
*     ��Ŀ                             IPV4                                   IPV6                             ����
*   ��ַ�ṹ                  sockaddr_in/SOCKADDR_IN                 sockaddr_in6/SOCKADDR_IN6         sockaddr_storage
*   ��ַ����                            4�ֽ�
*   ��ַ���Ⱥ�                   INET_ADDRSTRLEN(22)                   INET6_ADDRSTRLEN(65)
*   ͨ���ַ                      htonl(INADDR_ANY)                        in6addr_any 
*   ��ַת��                  inet_aton/inet_ntoa/inet_addr                                              WSAAddressToString/InetNtop(Vista!!)
*   �������͵�ַת��           gethostbyname/gethostbyaddr                                         getaddrinfo/freeaddrinfo/getnameinfo
*   ��ͷ����(�ֽ�)                      20                                     40 
*   DNS�еļ�¼(ʲô��˼?)             A��¼                                 AAAA��¼
*************************************************************************************************************************/


namespace FTL
{
	class CFWinsockEnvInit
	{
	public:
		FTLINLINE CFWinsockEnvInit(WORD wVersionRequested = MAKEWORD(2, 2));
		FTLINLINE ~CFWinsockEnvInit();
	public:
		int		m_initResult;
		WSADATA m_wsaData;
	};

	//���� IPV4/IPV6 �ĵ�ַ�����࣬�׽��ֲ���: Э��(AF_XXX) + IP��ַ + �˿ں�
	//template <typename SOCKADDR_BASE = sockaddr_in>
	//���Գ���ͨ�� union ���ݣ��ж� family �� ǿ��ת���ɶ�Ӧ�Ľṹ������

#if 0
	class CFSockAddrIn :  public SOCKADDR_IN //  : public addrinfo// 
	{
	public:
		FTLINLINE static USHORT GetPortNumber(LPCTSTR strServiceName);

		FTLINLINE CFSockAddrIn() { Clear(); }
		FTLINLINE CFSockAddrIn(const CFSockAddrIn& sin) { Copy( sin ); }
		FTLINLINE ~CFSockAddrIn() { }
		FTLINLINE CFSockAddrIn& Copy(const CFSockAddrIn& sin);
		FTLINLINE void    Clear() { memset(this, 0, sizeof(SOCKADDR_IN)); }
		FTLINLINE bool    IsEqual(const CFSockAddrIn& sin) const;
		FTLINLINE bool    IsGreater(const CFSockAddrIn& sin) const;
		FTLINLINE bool    IsLower(const CFSockAddrIn& pm) const;
		FTLINLINE bool    IsNull() const { return ((sin_addr.s_addr==0L)&&(sin_port==0)); }
		FTLINLINE ULONG   GetIPAddr() const { return sin_addr.s_addr; }
		FTLINLINE short   GetPort() const { return sin_port; }
		FTLINLINE bool    CreateFrom(LPCTSTR sAddr, LPCTSTR sService, int nFamily = AF_INET);
		FTLINLINE CFSockAddrIn& operator=(const CFSockAddrIn& sin) { return Copy( sin ); }
		FTLINLINE bool    operator==(const CFSockAddrIn& sin) { return IsEqual( sin ); }
		FTLINLINE bool    operator!=(const CFSockAddrIn& sin) { return !IsEqual( sin ); }
		FTLINLINE bool    operator<(const CFSockAddrIn& sin)  { return IsLower( sin ); }
		FTLINLINE bool    operator>(const CFSockAddrIn& sin)  { return IsGreater( sin ); }
		FTLINLINE bool    operator<=(const CFSockAddrIn& sin) { return !IsGreater( sin ); }
		FTLINLINE bool    operator>=(const CFSockAddrIn& sin) { return !IsLower( sin ); }
		FTLINLINE operator LPSOCKADDR() { return (LPSOCKADDR)(this); }
		FTLINLINE size_t  Size() const { return sizeof(SOCKADDR_IN); }
		FTLINLINE void    SetAddr(SOCKADDR_IN* psin) { CopyMemory(this, psin, Size()); }
	};
#endif 

    enum FSocketType
    {
        stTCP,
        stUDP,
    };

    enum FSocketMode
    {
        smClose,
        smAccept,
        smRead,
        smWrite,
        smPending
    };

    typedef struct tagFOVERLAPPED
    {
        OVERLAPPED				overLapped;
        WSABUF					dataBuf;
        FSocketMode             socketMode;
        volatile UINT	        nSession;
    }FOVERLAPPED, *LPFOVERLAPPED;

    class CFSocket
    {
    public:
        SOCKET              m_socket;
		FTLINLINE CFSocket* CreateSocketByAddr(CFSocketAddress& /*addr*/)
		{
			//�������ˣ�ͨ��hintsָ���������������������صĵ�ַ��Ȼ�����ֱ�����ڴ���socket����
			/*
			struct addrinfo hints = { 0 };
			struct addrinfo *res = NULL;
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_DGRAM;

			//AI_PASSIVE -- �����ģ�����bind��ͨ������ server socket
			//AI_CANONNAME -- ��������
			//AI_NUMERICHOST -- ��ַΪ���ִ�
			hints.ai_flags = AI_PASSIVE;

			rc = getaddrinfo(NULL, "8080", &hints, &res);  //res ���ص�ַ����һ����˵ʹ�õ�һ��bind�ɹ��Ϳ�����
			socket s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
			bind(s, res->ai_addr, res->ai_addrlen);
			freeaddrinfo(res);
			*/
			
			//�ͻ���
			struct addrinfo	hints = {0};
			struct addrinfo * res = NULL;
			hints.ai_flags = AF_UNSPEC;
			hints.ai_socktype = SOCK_DGRAM;
			hints.ai_flags = AI_CANONNAME;
			int rc = SOCKET_ERROR;
            NET_VERIFY(getaddrinfo("www.baidu.com", "http", &hints, &res));  //�õ�ָ�����������ӵ�ַ(���ܻ�ͬʱ���� IPV4/IPV6 �ĵ�ַ)
			SOCKET socketfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
			connect(socketfd, res->ai_addr, res->ai_addrlen);
		}

        FTLINLINE CFSocket();
        FTLINLINE virtual ~CFSocket();
		FTLINLINE BOOL IsOpen();
        FTLINLINE int Open(FSocketType st, BOOL bAsync);
        FTLINLINE int Close();
        FTLINLINE int Shutdown(INT how);

		//Connection
		FTLINLINE int Connect(const CFSocketAddress& addr);// LPCTSTR pszAddr, INT nSocketPort);

		//Data
		FTLINLINE int Send(const BYTE* pBuf, INT len, DWORD flags);
		FTLINLINE int Recv(BYTE* pBuf, INT len, DWORD flags);

        FTLINLINE int IoCtl(long cmd, u_long* argp);
		//FTLINLINE int SetOpt(xxxx); //setsocketopt �ķ�װ


		FTLINLINE CFSocket* Accept();
		FTLINLINE int Associate(SOCKET socket, SOCKADDR_IN* pForeignAddr);
		FTLINLINE SOCKADDR_IN& GetForeignAddr() { return m_foreignAddr; }
	protected:
		FTLINLINE virtual void OnOpen() {}
		FTLINLINE virtual void OnClose() {}
    protected:
        FSocketType         m_socketType;
        BOOL                m_bASync;
        CFCriticalSection   m_lockObject;
		SOCKADDR_IN		m_foreignAddr;
        volatile UINT       m_nSession;
    };

    class CFServerSocket : public CFSocket
    {
    public:
        FTLINLINE CFServerSocket();
        FTLINLINE ~CFServerSocket();
        //Server
		FTLINLINE int Bind(USHORT listenPort, LPCTSTR pszBindAddr);
        FTLINLINE int StartListen(INT backlog, INT nMaxClients);
		//FTLINLINE void ReleaseClient(CFClientSocketT<T>* pClient);
	protected:
		FTLINLINE virtual void OnClose();
    protected:
        INT     m_nMaxClients;
    };

    FTLEXPORT template < typename T>
    class CFNetClientT
    {
    public:
        CFNetClientT(FSocketType st = stTCP);
        virtual ~CFNetClientT();
        int Create();
        int Destroy();
        int Connect(LPCTSTR pszAddr);
    protected:
        CFSocket*		 m_pClientSocket;
        FSocketType      m_socketType;
    };

    FTLEXPORT template< typename T >
    class CFNetServerT
    {
    public:
        CFNetServerT(FSocketType st = stTCP);
        virtual ~CFNetServerT();
    public:

        /*
        * param [in] listenPort 
        * param [in] backlog ���ڵȴ����ӵ������г���
        */
        int Create(USHORT listenPort, LPCTSTR pszBindAddr = NULL );
        //int Close();
        int Destroy();
        int Start(INT backlog, INT nMaxClients);
        int Stop();
        //CFSocket<T>* Accept();
    protected:
        FSocketType                 m_socketType;
        CFServerSocket*			m_pServerSocket;

        //CFThreadPool<DWORD>*    m_pIoServerThreadPool;
        HANDLE                  m_hIoCompletionPort;
        HANDLE                  m_hServerThread;
        HANDLE                  *m_pWorkerThreads;

        DWORD                   getNumberOfConcurrentThreads();
        static unsigned int __stdcall serverThreadProc( LPVOID lpThreadParameter );
        static unsigned int __stdcall workerThreadProc( LPVOID lpThreadParameter );
        unsigned int     doServerLoop();
        unsigned int     doWorkerLoop();
    };
    class CFSocketUtils
    {
    public:
#if 0
        static FTLINLINE size_t readn(int fd, void* vptr, size_t n);
        static FTLINLINE size_t writen(int fd, const void* vptr, size_t n);
#endif 
        static FTLINLINE int DumpSocketOption(SOCKET s);
    };


    class CFIocpClientSocket : public CFIocpBaseTask
        , public CFSocket
    {
    public:
        FTLINLINE CFIocpClientSocket(CFIocpMgr* pIocpMgr);
        FTLINLINE virtual INT GetIocpHandleCount() const ;
        FTLINLINE virtual HANDLE GetIocpHandle( INT index ) const;
        FTLINLINE virtual BOOL OnInitialize( CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred );
        FTLINLINE virtual BOOL AfterReadCompleted( CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred );
        FTLINLINE virtual BOOL AfterWriteCompleted( CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred );
        FTLINLINE virtual BOOL OnUninitialize( CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred );
    };

    class CFIocpNetServer;

    class CFIocpListenTask : public CFIocpBaseTask
    {
    public:
        FTLINLINE CFIocpListenTask(CFIocpMgr* pIocpMgr, SOCKET socketListen);
        FTLINLINE ~CFIocpListenTask();

        FTLINLINE virtual INT GetIocpHandleCount() const;
        FTLINLINE virtual HANDLE GetIocpHandle( INT index ) const;

        FTLINLINE virtual BOOL OnInitialize(CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred );
        FTLINLINE virtual BOOL AfterReadCompleted( CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred );
        FTLINLINE virtual BOOL AfterWriteCompleted( CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred );
        FTLINLINE virtual BOOL OnUninitialize( CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD dwBytesTransferred );
    public:
        SOCKET m_socketListen;
    };

    // ͬʱͶ�ݵ�Accept���������(���Ҫ����ʵ�ʵ�����������)
#define MAX_POST_ACCEPT              1
    class CFIocpNetServer : public CFIocpMgr
    {
    public:
        FTLINLINE CFIocpNetServer();
        FTLINLINE ~CFIocpNetServer();
        FTLINLINE virtual BOOL OnIocpStart();
        FTLINLINE virtual BOOL OnIocpStop();
    protected:
        //SOCKET      m_nSocketListen;
        USHORT      m_nListenPort;
        CFIocpListenTask* m_pListenTask;
        // AcceptEx �� GetAcceptExSockaddrs �ĺ���ָ�룬���ڵ�����������չ����
    public:
        LPFN_ACCEPTEX                m_lpfnAcceptEx;
        LPFN_GETACCEPTEXSOCKADDRS    m_lpfnGetAcceptExSockAddrs;

    //protected:
    public:
        FTLINLINE BOOL _PostAccept(CFIocpBuffer* pioBuffer);
        FTLINLINE BOOL _PostRecv(CFIocpBuffer* pioBuffer);
        FTLINLINE BOOL _PostSend(CFIocpBuffer* pioBuffer);
    };
}
#endif //FTL_SOCKET_H

#ifndef USE_EXPORT
#  include "ftlSocket.hpp"
#endif 