#ifndef FTL_SOCKET_HPP
#define FTL_SOCKET_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlSocket.h"
#endif
//#include <winsock.h>

namespace FTL
{
	CFWinsockEnvInit::CFWinsockEnvInit(WORD wVersionRequested /* = MAKEWORD(2, 2) */)
	{
		ZeroMemory(&m_wsaData, sizeof(m_wsaData));
		m_initResult = WSAStartup(wVersionRequested, &m_wsaData);

		//Success
		if (NO_ERROR != m_initResult)
		{
			FTLTRACEEX(tlError, TEXT("WSAStartup Error: %d(%s)"), 
				m_initResult, CFNetErrorInfo(m_initResult).GetConvertedInfo());
		}
		FTLASSERT(m_initResult == NO_ERROR);
		FTLASSERT(wVersionRequested == m_wsaData.wVersion);
	}
	
	CFWinsockEnvInit::~CFWinsockEnvInit()
	{
		if (0 == m_initResult)
		{
			WSACleanup();
		}
	}
	
	///////////////////////////////////////////////////////////////////////////////
#if 0
	CFSockAddrIn& CFSockAddrIn::Copy(const CFSockAddrIn& sin)
	{
		memcpy(this, &sin, Size());
		return *this;
	}

	// IsEqual
	bool CFSockAddrIn::IsEqual(const CFSockAddrIn& sin) const
	{
		// Is it Equal? - ignore 'sin_zero'
		return (memcmp(this, &sin, Size()-sizeof(sin_zero)) == 0);
	}

	///////////////////////////////////////////////////////////////////////////////
	// IsGreater
	bool CFSockAddrIn::IsGreater(const CFSockAddrIn& sin) const
	{
		// Is it Greater? - ignore 'sin_zero'
		return (memcmp(this, &sin, Size()-sizeof(sin_zero)) > 0);
	}

	///////////////////////////////////////////////////////////////////////////////
	// IsLower
	bool CFSockAddrIn::IsLower(const CFSockAddrIn& sin) const
	{
		// Is it Lower? - ignore 'sin_zero'
		return (memcmp(this, &sin, Size()-sizeof(sin_zero)) < 0);
	}

	///////////////////////////////////////////////////////////////////////////////
	// CreateFrom
	bool CFSockAddrIn::CreateFrom(LPCTSTR sAddr, LPCTSTR sService, int nFamily /*=AF_INET*/)
	{
		Clear();
		FTLASSERT(FALSE);
		CFConversion conv;
		sin_addr.s_addr = htonl(inet_addr(conv.TCHAR_TO_MBCS(sAddr)));

		sin_family = nFamily;
#if 0
		sin_addr.s_addr = htonl( CSocketComm::GetIPAddress(sAddr) );
		sin_port = htons( CSocketComm::GetPortNumber( sService ) );
		sin_family = nFamily;
#endif 
		return !IsNull();
	}
#endif

	CFSocket::CFSocket()
	{
		m_socket = INVALID_SOCKET;
		m_socketType = stTCP;
		m_bASync = TRUE;
		m_nSession = (UINT)(-1);
	}

	CFSocket::~CFSocket()
	{
		FTLASSERT(!IsOpen() && TEXT("Forget Close The Socket"));
		Close();
	}

	BOOL CFSocket::IsOpen()
	{
		BOOL isOpen = (INVALID_SOCKET != m_socket);
		return isOpen;
	}

	int CFSocket::Open(FSocketType st, BOOL bAsync)
	{
		FTLASSERT(INVALID_SOCKET == m_socket && TEXT("Can not Create Socket Twice"));

		int rc = NO_ERROR;
		m_socketType = st;
		m_bASync = bAsync;
		DWORD dwFlags = m_bASync ? WSA_FLAG_OVERLAPPED : 0;
		switch(m_socketType)
		{
		case stTCP:
			m_socket = WSASocket(AF_INET, SOCK_STREAM, 0/*IPPROTO_TCP*/, NULL, 0, dwFlags);
			break;
		case stUDP:
			m_socket = WSASocket(AF_INET, SOCK_DGRAM, 0/*IPPROTO_UDP*/, NULL, 0, dwFlags);
			break;
		default:
			FTLASSERT(FALSE);
			break;
		}
		if (INVALID_SOCKET == m_socket)
		{
			NET_VERIFY(SOCKET_ERROR);
			rc = SOCKET_ERROR;
		}
		return rc;
	}

	int CFSocket::Close()
	{
		int rc = NO_ERROR;
		SAFE_CLOSE_SOCKET(m_socket);
		//if (INVALID_SOCKET != m_socket)
		//{ 
		//    NET_VERIFY(closesocket(m_socket));
		//    m_socket = INVALID_SOCKET;
		//}
		m_nSession = (UINT)(-1);
		return rc;
	}

	int CFSocket::Shutdown(INT how)
	{
		FTLASSERT(INVALID_SOCKET != m_socket);
		int rc = NO_ERROR;
		rc = shutdown(m_socket, how);
		return rc;
	}

	int CFSocket::Connect(const CFSocketAddress& addrConnect)// (LPCTSTR pszAddr, INT nSocketPort)
	{
		//FTLASSERT(pszAddr);
		//FTLASSERT(nSocketPort);

		int rc = NO_ERROR;
		//int nLength = sizeof(addrConnect);
		//sockaddr_in addrConnect = {0};

		//addrConnect.sin_family = AF_INET;
		//addrConnect.sin_port = htons(nSocketPort);
		//addrConnect.sin_addr.S_un.S_addr = inet_addr(CT2A(pszAddr));

		NET_VERIFY(WSAConnect(m_socket, addrConnect.lpSockaddr, addrConnect.iSockaddrLength, NULL,NULL,NULL,NULL));

		return rc;
	}


	int CFSocket::Send(const BYTE* pBuf, INT len, DWORD flags)
	{
		int rc = NO_ERROR;
		WSABUF sendBuf = {0};
		sendBuf.len = (ULONG)len;
		sendBuf.buf = (char*)pBuf;
		DWORD dwNumberOfSend = 0;
		NET_VERIFY(WSASend(m_socket,&sendBuf, 1,&dwNumberOfSend,flags, NULL, NULL));
		return rc;
	}

	int CFSocket::Recv(BYTE* pBuf, INT len, DWORD flags)
	{
		int rc = NO_ERROR;

		if (m_socketType == stUDP)
		{
			//WSAEMSGSIZE
		}

		WSABUF recvBuf = {0};
		recvBuf.len = len;
		recvBuf.buf = (char*)pBuf;
		DWORD dwNumberOfRecv = 0;

		NET_VERIFY(WSARecv(m_socket,&recvBuf, 1, &dwNumberOfRecv, &flags ,NULL,NULL));
		return rc;
	}

	CFSocket*  CFSocket::Accept()
	{
		SOCKET soAccept = INVALID_SOCKET;
		//CFSockAddrIn addrClient;
		struct sockaddr_in addrClient = {0};
		int nLength = sizeof(struct sockaddr_in);
		CFSocket *pRetClient = NULL;

		// Check if there is something in the listen queue.
		soAccept = WSAAccept(m_socket, (struct sockaddr *) &addrClient, 
			&nLength, NULL, NULL);

		// Check for errors.
		if (soAccept != INVALID_SOCKET) 
		{
			// Get a pointer to the free ClientSocket 
			// instance from the pool.
			pRetClient = new CFSocket();
			if (pRetClient == NULL) 
			{
				// There are no free instances in the pool, maximum 
				// number of accepted client connections is reached.
				::shutdown(soAccept,2);
				::closesocket(soAccept);
			}
			else 
			{
				// Everything is fine, so associate the instance 
				// with the client socket.
				pRetClient->Associate(soAccept,&addrClient);
			}
		}

		return pRetClient;
	}

	int CFSocket::Associate(SOCKET socket, PSOCKADDR_IN pForeignAddr)
	{
		FTLASSERT(m_socket == INVALID_SOCKET);
		int rc = NO_ERROR;
		m_socket = socket;
		memcpy(&m_foreignAddr, pForeignAddr, sizeof(SOCKADDR_IN));
		return rc;
	}

	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	CFServerSocket::CFServerSocket()
	{
        m_nMaxClients = INT_MAX;
	}

	CFServerSocket::~CFServerSocket()
	{
	}

	void CFServerSocket::OnClose()
	{
	}

	int CFServerSocket::Bind(USHORT listenPort, LPCTSTR pszBindAddr  = NULL )
	{
		FTLASSERT(m_socket != INVALID_SOCKET);
		int rc = NO_ERROR;
		SOCKADDR_IN bindAddr_in = {0};
		bindAddr_in.sin_family = AF_INET;
		bindAddr_in.sin_port = htons(listenPort);

		if (pszBindAddr)
		{
			bindAddr_in.sin_addr.S_un.S_addr = inet_addr(CT2A(pszBindAddr));
		}
		else
		{
			bindAddr_in.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		}

		NET_VERIFY(bind(m_socket,(SOCKADDR*)&bindAddr_in, sizeof(bindAddr_in)));
		return rc;
	}

	int CFServerSocket::StartListen(INT backlog, INT nMaxClients)
	{
		int rc = NO_ERROR;
		//m_pClientSocketPool = new CFMemoryPoolT<CFClientSocketT <T> >(0,nMaxClients);

		FTLASSERT( m_socket != INVALID_SOCKET );
		NET_VERIFY(listen(m_socket, backlog));
        m_nMaxClients = nMaxClients;
		return rc;
	}

#if 0
	template<typename T>
	void CFServerSocketT<T>::ReleaseClient(CFClientSocketT<T>* pClient)
	{
		FTLASSERT(NULL != pClient);
		FTLASSERT(NULL != m_pClientSocketPool);
		if (pClient)
		{
			pClient->Close();
			if (m_pClientSocketPool)
			{
				m_pClientSocketPool->Release(pClient);
			}
		}
	}
#endif 
	//////////////////////////////////////////////////////////////////////////
	///                     CFNetClientT                                   ///
	//////////////////////////////////////////////////////////////////////////

	template< typename T >
	CFNetClientT<T>::CFNetClientT(FSocketType st)
	{
		m_pClientSocket = NULL;
		m_socketType = st;
	}

	template< typename T >
	CFNetClientT<T>::~CFNetClientT()
	{

	}

	template< typename T >
	int CFNetClientT<T>::Create()
	{
		int rc = NO_ERROR;
		m_pClientSocket = new CFSocket(m_socketType);
		m_pClientSocket->Open();
	}

	template< typename T >
	int CFNetClientT<T>::Destroy()
	{

	}

	template< typename T >
	int CFNetClientT<T>::Connect(LPCTSTR pszAddr)
	{
		int rc = NO_ERROR;
		SOCKADDR_IN conAddr = {0};
		conAddr.sin_family = AF_INET;
		conAddr.sin_port = 0;
		conAddr.sin_addr.S_un.S_addr = 0; 

		//WSAECONNREFUSED -- �����ӵļ����û�м���ָ���˿�
		//WSAETIMEDOUT -- 
		NET_VERIFY(WSAConnect(m_pClientSocket->m_socket,SOCKADDR*(&conAddr), sizeof(conAddr),NULL,NULL,NULL,NULL));
		return rc;
	}


	//////////////////////////////////////////////////////////////////////////
	///                     CFNetServerT                                   ///
	//////////////////////////////////////////////////////////////////////////

	template< typename T >
	DWORD CFNetServerT<T>::getNumberOfConcurrentThreads()
	{
		//���� CreateIoCompletionPort �Ĳ����ʹ����̡߳�
		//0��ʾϵͳ�ڰ�װ�˶��ٸ���������������ͬʱ���ж��ٸ��̲߳���IO��ɶ˿�,��ͨ������ CPU���� * 2 + 2;
		SYSTEM_INFO sysInfo = {0};
		GetSystemInfo(&sysInfo);

#pragma TODO(���̻߳���ʱ��ȡ���̸߳���)
		DWORD numberOfConcurrentThreads = sysInfo.dwNumberOfProcessors * 2 + 2;
		return numberOfConcurrentThreads;
	}

	template< typename T >
	unsigned int CFNetServerT<T>::serverThreadProc( LPVOID lpThreadParameter )
	{
		FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
		unsigned int ret = 0;
		CFNetServerT<T>* pThis = static_cast< CFNetServerT<T>* >(lpThreadParameter);
		ret = pThis->doServerLoop();
		return ret;
	}

	template< typename T >
	unsigned int CFNetServerT<T>::doServerLoop()
	{
		FUNCTION_BLOCK_TRACE(DEFAULT_BLOCK_TRACE_THRESHOLD);
		//int rc = 0;

		//��Accept�Ŀͻ���Socket�󶨵�IO��ɶ˿ڣ����صĽ���֮ǰ������IO�˿ڣ����������µ�
		//SOCKET acceptClient = 0;

		for (;;)
		{
			CFSocket* pClient = m_pServerSocket->Accept();
			if (pClient)
			{
				//HANDLE hbindIoPort = CreateIoCompletionPort((HANDLE)(pClient->m_socket),m_hIoCompletionPort,
				//	(ULONG_PTR)(pClient), 0);
				//FTLASSERT( hbindIoPort == m_hIoCompletionPort);
				//::PostQueuedCompletionStatus(m_hIoCompletionPort,1,pClient,)
			}
			//FOVERLAPPED*     pKey = new FOVERLAPPED();
			//pKey->socket = acceptClient;
			//pKey->overLapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

			//(m_pServerSocket->Accept())
			//break;
		}
		return 0;
	}

	template< typename T >
	unsigned int CFNetServerT<T>::workerThreadProc( LPVOID lpThreadParameter )
	{
		unsigned int ret = 0;
		CFNetServerT<T>* pThis = static_cast< CFNetServerT<T>* >(lpThreadParameter);
		ret = pThis->doWorkerLoop();
		return ret;
	}

	template< typename T >
	unsigned int CFNetServerT<T>::doWorkerLoop()
	{
		DWORD dwNumberOfBytesTransferred = 0;
		CFSocket* pClientSocket = NULL;//���ڻ�õڶ���CreateIoCompletionPortʱ����Ĳ���
		LPFOVERLAPPED    pOverLapped = NULL;
		BOOL bRet = FALSE;
		for (;;)
		{
			//���������(ע��� PostQueuedCompletionStatus �ĵ���Ҳ�ỽ�ѣ���Ҫ�жϲ�����)��
			//1.�ɹ�ȡ��һ��Completion Packet������TRUE������д��������;
			//2.����FALSE����ȡ��Completion Packet��pOverLappedָ��ʧ�ܵĲ���������GetLastError
			//3.����FALSE������û��ȡ�� Completion Packet, pOverLapped ΪNULL
			API_VERIFY(GetQueuedCompletionStatus(m_hIoCompletionPort,&dwNumberOfBytesTransferred,
				(PULONG_PTR)&pClientSocket, (LPOVERLAPPED*)(&pOverLapped), INFINITE));

			if (FALSE == bRet)
			{
				if (NULL == pOverLapped)
				{
					break;
				}
				else
				{
					//Happend instead of end-of-file
				}
			}
			else if(0 == dwNumberOfBytesTransferred)
			{
				//socket close
			}
			else
			{
				//FTLTRACEEX(FTL::tlInfo, TEXT("ClientSocket[%d,Session=%d],OverLapped[mode=%d,len=%d,dataBuf=%s]\n"),
				//    pClientSocket->m_socket,pClientSocket->m_nSession, 
				//    pOverLapped->socketMode,pOverLapped->dataBuf.len, (TCHAR*)(pOverLapped->dataBuf.buf));
				//Got a valid data block
			}
		}

		return 0;
	}

	template< typename T >
	CFNetServerT<T>::CFNetServerT(FSocketType st)
	{
		m_socketType = st;
		m_pServerSocket = NULL;
		m_hIoCompletionPort = NULL;
		//m_pIoServerThreadPool = NULL;
		//m_pServerThread = NULL;
	}

	template< typename T >
	CFNetServerT<T>::~CFNetServerT()
	{
		Destroy();
	}

	template< typename T >
	int CFNetServerT<T>::Create(USHORT listenPort, LPCTSTR pszBindAddr /* = NULL */)
	{
		FTLASSERT(NULL == m_pServerSocket);
		//if (NULL != m_pServerSocket)
		//{
		//    Destroy(); 
		//}
		int rc = NO_ERROR;
		//BOOL bRet = FALSE;
		m_pServerSocket = new CFServerSocket();
		NET_VERIFY(m_pServerSocket->Open(m_socketType, TRUE));

		do 
		{
			//int nZero = 0;
			//Winsock������ص�I/O������ֱ��ʹ��Ӧ�ó���Ļ��壬���⿽��
			//setsockopt(m_pServerSocket->m_socket,SOL_SOCKET, SO_SNDBUF,(char*)&nZero, sizeof(nZero));

			if (stTCP == m_socketType)
			{
				NET_VERIFY(m_pServerSocket->Bind(listenPort,pszBindAddr));
				if (NO_ERROR != rc)
				{
					break;
				}
			}

			//2.ʹ���̳߳ش���ΪIO��ɶ˿ڷ���Ĺ������߳�
			//m_pIoServerThreadPool = new CFThreadPool<DWORD>(0,0);

			//3.��Accept����׽��־��ͬ��ɶ˿ڹ�����һ��
			//CreateIoCompletionPort((HANLDE)acceptSocket,m_hIoCompletionPort,(ULONG_PTR)this,0);


			//4.ʹ��Overlapped��ʽ����IO��������������ɶ˿ڣ������첽
			//�������߳�������ɶ˿��ϵȴ� GetQueuedCompletionStatus(m_hIoCompletionPort,)

			//m_pServerThread = new CFThread<DefaultThreadTraits>();
			//FTLASSERT(m_pServerThread);

			//if (m_pServerThread)
			//{
			//    m_pServerThread->Start((LPTHREAD_START_ROUTINE)(&CFNetServerT<T>::ServerThreadProc), this, TRUE);
			//}
		} while (FALSE);

		if (NO_ERROR != rc)
		{
			Destroy();
		}
		return rc;
	}

	template< typename T >
	int CFNetServerT<T>::Destroy()
	{
		int rc = NO_ERROR;
		BOOL bRet = FALSE;
		//����ɶ˿��ϣ� ��ÿ���������̶߳�����һ�������������ݰ�,��ֹ���й������̵߳�����
		//PostQueuedCompletionStatus(m_hIoCompletionPort)
		SAFE_CLOSE_HANDLE(m_hIoCompletionPort, NULL);

		//if (m_pServerThread)
		//{
		//    m_pServerThread->StopAndWait(INFINITE,TRUE);
		//    SAFE_DELETE(m_pServerThread);
		//}
		if (m_pServerSocket)
		{
			NET_VERIFY(m_pServerSocket->Close());
			SAFE_DELETE(m_pServerSocket);
		}
		return rc;
	}


	template< typename T >
	int CFNetServerT<T>::Start(INT backlog, INT nMaxClients)
	{
		int rc = NO_ERROR;
		BOOL bRet = FALSE;

        DWORD dwNumberOfConcurrentThreads = getNumberOfConcurrentThreads();
		FTLASSERT(dwNumberOfConcurrentThreads > 0);

		//1.����IO��ɶ˿�
		m_hIoCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 
			NULL, 
			0, 
			dwNumberOfConcurrentThreads
			);
		API_VERIFY(NULL != m_hIoCompletionPort);

		m_pWorkerThreads = new HANDLE[dwNumberOfConcurrentThreads];
		ZeroMemory(m_pWorkerThreads, sizeof(HANDLE) * dwNumberOfConcurrentThreads );
		unsigned int threadId = 0;

		for (DWORD threadIndex = 0; threadIndex < dwNumberOfConcurrentThreads; threadIndex++)
		{
			m_pWorkerThreads[threadIndex] = (HANDLE)_beginthreadex(NULL, 0, workerThreadProc, this, 0, &threadId);
		}

		m_hServerThread = (HANDLE) _beginthreadex(NULL,0,serverThreadProc, this, 0, &threadId);

		NET_VERIFY(m_pServerSocket->StartListen(backlog,nMaxClients));
		return rc;
	}

	template< typename T >
	int CFNetServerT<T>::Stop()
	{
		int rc = NO_ERROR;
		BOOL bRet = FALSE;
		if (m_pWorkerThreads)
		{
			DWORD dwNumberOfConcurrentThreads = getNumberOfConcurrentThreads();
			for (DWORD threadIndex = 0; threadIndex < dwNumberOfConcurrentThreads; threadIndex++)
			{
				SAFE_CLOSE_HANDLE(m_pWorkerThreads[threadIndex], NULL);
			}
			SAFE_DELETE_ARRAY(m_pWorkerThreads);
		}
		SAFE_CLOSE_HANDLE(m_hServerThread, NULL);
		return rc;
	}

#if 0
	size_t CFSocketUtils::readn(int fd, void* vptr, size_t n)
	{
		size_t nleft = n;
		size_t nread = 0;
		char * ptr = (char*)vptr;
		while(nleft > 0)
		{
			if((nread = read(fd,ptr,nleft)) < 0)
			{
				if(EINTR == errno)
				{
					nread = 0; //call read again
				}
				else
				{
					return (size_t)(-1);
				}
			}
			else if(0 == nread)
			{
				break;   //EOF
			}
			nleft -= nread;
			ptr += nread;
		}
		return (n-nleft); 
	}

	size_t CFSocketUtils::writen(int fd, const void* vptr, size_t n)
	{
		size_t nleft = n;
		size_t nwritten = 0;
		const char* ptr = (const char*)vptr;
		while (nleft > 0)
		{
			if((nwritten = write(fd, ptr, nleft)) <= 0)
			{
				if(EINTR == errno)
				{
					nwritten = 0; //call write again-- reset�� ?
				}
				else
				{
					return (size_t)(-1);
				}
			}
			nleft -= nwritten;
			ptr += nwritten;
		} 
		return n;
	}
#endif 

    enum SockOptValueType
    {
        sovtInt,
        sovtBOOL,
        sovtDWORD,
        sovtSocketType,
        sovtLinger,
        sovtGroup,
        sovtWsaProtocolInfo,
    };
    struct SockOptInfo
    {
        int         level; 
        int         optName;
        TCHAR       pszOptName[40];
        int         nOptLen;
        SockOptValueType    valType;
    };

    int CFSocketUtils::DumpSocketOption(SOCKET s)
    {
        int rc = NO_ERROR;
        FTL::CFMemAllocator<CHAR> optValueBuffer(1024);
        //CHAR optValueBuffer[1024] = {0};

        static SockOptInfo allOptions[] = 
        {
            //SOL_SOCKET
            { SOL_SOCKET, SO_TYPE,          _T("SO_TYPE"),          sizeof(int), sovtSocketType }, //SOCK_STREAM �� 

            { SOL_SOCKET, SO_DEBUG,         _T("SO_DEBUG"),         sizeof(BOOL), sovtBOOL },   //FALSE
            { SOL_SOCKET, SO_ACCEPTCONN,    _T("SO_ACCEPTCONN"),    sizeof(BOOL), sovtBOOL },   //FALSE
            { SOL_SOCKET, SO_REUSEADDR,     _T("SO_REUSEADDR"),     sizeof(BOOL), sovtBOOL },   //FALSE
            { SOL_SOCKET, SO_KEEPALIVE,     _T("SO_KEEPALIVE"),     sizeof(BOOL), sovtBOOL },   //FALSE
            { SOL_SOCKET, SO_DONTROUTE,     _T("SO_DONTROUTE"),     sizeof(BOOL), sovtBOOL },   //FALSE

            { SOL_SOCKET, SO_BROADCAST,     _T("SO_BROADCAST"),     sizeof(BOOL), sovtBOOL },   //Fail -- WSAENOPROTOOPT
            { SOL_SOCKET, SO_USELOOPBACK,   _T("SO_USELOOPBACK"),   sizeof(BOOL), sovtBOOL },   //Fail -- WSAENOPROTOOPT

            { SOL_SOCKET, SO_DONTLINGER,    _T("SO_DONTLINGER"),    sizeof(BOOL), sovtBOOL },
            { SOL_SOCKET, SO_LINGER,        _T("SO_LINGER"),        sizeof(LINGER), sovtLinger}, //onoff=0, linger=0
            
            { SOL_SOCKET, SO_OOBINLINE,     _T("SO_OOBINLINE"),     sizeof(BOOL), sovtBOOL },   //FALSE

            { SOL_SOCKET, SO_SNDBUF,        _T("SO_SNDBUF"),        sizeof(int), sovtInt },     //8192
            { SOL_SOCKET, SO_RCVBUF ,       _T("SO_RCVBUF "),       sizeof(int), sovtInt },     //8192
            { SOL_SOCKET, SO_SNDLOWAT,      _T("SO_SNDLOWAT"),      sizeof(int), sovtInt },     //Fail -- WSAENOPROTOOPT
            { SOL_SOCKET, SO_RCVLOWAT,      _T("SO_RCVLOWAT"),      sizeof(int), sovtInt },     //Fail -- WSAENOPROTOOPT
            { SOL_SOCKET, SO_SNDTIMEO,      _T("SO_SNDTIMEO"),      sizeof(int), sovtInt },     //0
            { SOL_SOCKET, SO_RCVTIMEO,      _T("SO_RCVTIMEO"),      sizeof(int), sovtInt },     //0
            { SOL_SOCKET, SO_ERROR,         _T("SO_ERROR"),         sizeof(int), sovtInt },     //0

            { SOL_SOCKET, SO_GROUP_ID,      _T("SO_GROUP_ID"), sizeof(GROUP), sovtGroup },      //0
            { SOL_SOCKET, SO_GROUP_PRIORITY, _T("SO_GROUP_PRIORITY"), sizeof(int), sovtInt },   //0
            { SOL_SOCKET, SO_MAX_MSG_SIZE,  _T("SO_MAX_MSG_SIZE"), sizeof(unsigned int), sovtInt }, //1073741823 (ERROR_UNHANDLED_ERROR �� 2^30-1)

            //szProtocol=MSAFD Tcpip [TCP/IP], iVersion=2, iAddressFamily=2
            { SOL_SOCKET, SO_PROTOCOL_INFO, _T("SO_PROTOCOL_INFO"), sizeof(WSAPROTOCOL_INFO), sovtWsaProtocolInfo},
            
            { SOL_SOCKET, SO_CONDITIONAL_ACCEPT, _T("SO_CONDITIONAL_ACCEPT"), sizeof(BOOL), sovtBOOL }, //FALSE

            { SOL_SOCKET, SO_MAXDG,         _T("SO_MAXDG"), sizeof(DWORD), sovtDWORD },         //1073741823(ERROR_UNHANDLED_ERROR?)
            { SOL_SOCKET, SO_MAXPATHDG,     _T("SO_MAXPATHDG"), sizeof(DWORD), sovtDWORD },     //1073741823(ERROR_UNHANDLED_ERROR?)
            { SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, _T("SO_UPDATE_ACCEPT_CONTEXT"), sizeof(DWORD), sovtDWORD }, //Fail -- WSAENOPROTOOPT

            //-1(SOCKET_ERROR), ����Socket���ӵ�����, �����ڼ�����ӳɹ�ȴ���������ݵ����(ͨ����Ҫ�ر�)
            { SOL_SOCKET, SO_CONNECT_TIME,  _T("SO_CONNECT_TIME"), sizeof(DWORD), sovtDWORD },  
            
#if(_WIN32_WINNT >= 0x0501)
            { SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, _T("SO_UPDATE_CONNECT_CONTEXT"), sizeof(DWORD), sovtDWORD }, //Fail -- WSAENOPROTOOPT
#endif //(_WIN32_WINNT >= 0x0501)



            //IPPROTO_TCP
            {IPPROTO_TCP,  TCP_NODELAY, _T("TCP_NODELAY"), sizeof(BOOL), sovtBOOL },
            //{yyyy,  XXXX, _T("XXXXX"), sizeof(DWORD), sovtDWORD },
        };

        for (int i = 0; i < _countof(allOptions); i++)
        {
            FTLASSERT(allOptions[i].nOptLen <= (int)optValueBuffer.GetCount());
            if (allOptions[i].nOptLen <= (int)optValueBuffer.GetCount())
            {
                ZeroMemory(optValueBuffer, optValueBuffer.GetCount());

                NET_VERIFY_EXCEPT1(getsockopt(s, allOptions[i].level, allOptions[i].optName, 
                    (CHAR*)optValueBuffer, &allOptions[i].nOptLen),
                    WSAENOPROTOOPT);
                if (NO_ERROR == rc)
                {
                    switch (allOptions[i].valType)
                    {
                    case sovtInt:
                        FTLTRACE(TEXT("%s (int) is %d\n"), allOptions[i].pszOptName, 
                            *((int*)optValueBuffer.GetMemory()));
                        break;
                    case sovtBOOL:
                        FTLTRACE(TEXT("%s (BOOL) is %s\n"), allOptions[i].pszOptName, 
                            *((BOOL*)optValueBuffer.GetMemory()) ? TEXT("TRUE") : TEXT("FALSE") );
                        break;
                    case sovtDWORD:
                        FTLTRACE(TEXT("%s (DWORD) is %d\n"), allOptions[i].pszOptName, 
                            *((DWORD*)optValueBuffer.GetMemory()));
                        break;
                    case sovtSocketType:
                        {
                            int nSocketType = *((int*)optValueBuffer.GetMemory());
                            FTLTRACE(TEXT("%s (SocketType) is %d(%s)\n"), allOptions[i].pszOptName, 
                                nSocketType, FNetInfo::GetSocketType(nSocketType));
                        }
                        break;
                    case sovtLinger:
                        {
                            LINGER* pLinger = (LINGER*)optValueBuffer.GetMemory();
                            FTLTRACE(TEXT("%s (LINGER), onoff=%d, linger=%d\n"), allOptions[i].pszOptName, 
                                pLinger->l_onoff, pLinger->l_linger);
                        }
                        break;
                    case sovtGroup:
                        {
                            GROUP nGroup = *(GROUP*)optValueBuffer.GetMemory();
                            FTLTRACE(TEXT("%s (Group) group=%d\n"), allOptions[i].pszOptName, 
                                nGroup);
                        }
                        break;
                    case sovtWsaProtocolInfo:
                        {
                            WSAPROTOCOL_INFO* pProtInfo = (WSAPROTOCOL_INFO*)optValueBuffer.GetMemory();
                            FTLTRACE(TEXT("%s (ProtocolInfo), szProtocol=%s, iVersion=%d, iAddressFamily=%d\n"), 
                                allOptions[i].pszOptName, 
                                pProtInfo->szProtocol, pProtInfo->iVersion, pProtInfo->iAddressFamily);
                        }
                        break;
                    default:
                        FTLASSERT(FALSE);
                        break;
                    }
                }
                else
                {
                    FTLTRACE(TEXT("getsockopt for %s Fail, reason is %s\n"),
                        allOptions[i].pszOptName, CFNetErrorInfo(WSAGetLastError()).GetConvertedInfo());
                }
            }
        }

        return NO_ERROR;
    }

    //////////////////////////////////////////////////////////////////////////
    CFIocpClientSocket::CFIocpClientSocket(CFIocpMgr* pIocpMgr)
        :CFIocpBaseTask(pIocpMgr)
    {

    }
    INT CFIocpClientSocket::GetIocpHandleCount() const
    {
        return 1;
    }

    HANDLE CFIocpClientSocket::GetIocpHandle( INT /* index */) const
    {
        return (HANDLE)m_socket;
    }

    BOOL CFIocpClientSocket::OnInitialize( CFIocpMgr* /*pIocpMgr */, CFIocpBuffer* /*pIoBuffer*/, DWORD /* dwBytesTransferred */ )
    {
        throw std::exception("The method or operation is not implemented.");
    }

    BOOL CFIocpClientSocket::AfterReadCompleted( CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD /* dwBytesTransferred */ )
    {
        SOCKADDR_IN* ClientAddr = &m_foreignAddr;
        CFConversion convAddr, convInfo;
        LPCTSTR pszClientAddr = convAddr.MBCS_TO_TCHAR(inet_ntoa(ClientAddr->sin_addr));
        LPCTSTR pszClientInfo = convInfo.MBCS_TO_TCHAR(pIoBuffer->m_wsaBuf.buf);

        FTLTRACE(_T("�յ�  %s:%d ��Ϣ��%s\n"), pszClientAddr, ntohs(ClientAddr->sin_port), pszClientInfo);

        // Ȼ��ʼͶ����һ��WSARecv����
        CFIocpNetServer* pNetServer = dynamic_cast<CFIocpNetServer*>(pIocpMgr);
        if (pNetServer)
        {
            pNetServer->_PostRecv(pIoBuffer);
        }
        return TRUE;
    }

    BOOL CFIocpClientSocket::AfterWriteCompleted( CFIocpMgr* /*pIocpMgr */, CFIocpBuffer* /*pIoBuffer*/, DWORD /* dwBytesTransferred */ )
    {
        throw std::exception("The method or operation is not implemented.");
    }

    BOOL CFIocpClientSocket::OnUninitialize( CFIocpMgr* /*pIocpMgr */, CFIocpBuffer* /*pIoBuffer*/, DWORD /* dwBytesTransferred */ )
    {
        Close();
        delete this;
        return TRUE;
    }

    CFIocpListenTask::CFIocpListenTask(CFIocpMgr* pIocpMgr, SOCKET socketListen)
        :CFIocpBaseTask(pIocpMgr)
    {
        m_socketListen = socketListen;
    }
    CFIocpListenTask::~CFIocpListenTask()
    {
        int rc = SOCKET_ERROR;
        SAFE_CLOSE_SOCKET(m_socketListen);
    }
    INT CFIocpListenTask::GetIocpHandleCount() const
    {
        return 1;
    }

    HANDLE CFIocpListenTask::GetIocpHandle( INT index ) const
    {
        FTLASSERT(index < 1);
        return (HANDLE)m_socketListen;
    }

    BOOL CFIocpListenTask::OnInitialize(CFIocpMgr* pIocpMgr, CFIocpBuffer* pIoBuffer, DWORD /* dwBytesTransferred */ )
    {
        FTLASSERT(pIoBuffer->m_operType == otInitialize);

        SOCKADDR_IN* ClientAddr = NULL;
        SOCKADDR_IN* LocalAddr = NULL;  
        int remoteLen = sizeof(SOCKADDR_IN), localLen = sizeof(SOCKADDR_IN);  

        CFIocpNetServer* pNetServer = dynamic_cast<CFIocpNetServer*>(pIocpMgr);
        if (pNetServer)
        {
            // 1. ����ȡ������ͻ��˵ĵ�ַ��Ϣ
            // ��� m_lpfnGetAcceptExSockAddrs �����˰�~~~~~~
            // ��������ȡ�ÿͻ��˺ͱ��ض˵ĵ�ַ��Ϣ������˳��ȡ���ͻ��˷����ĵ�һ�����ݣ���ǿ����...
            pNetServer->m_lpfnGetAcceptExSockAddrs(pIoBuffer->m_wsaBuf.buf, pIoBuffer->m_wsaBuf.len - ((sizeof(SOCKADDR_IN)+16)*2),  
                sizeof(SOCKADDR_IN)+16, sizeof(SOCKADDR_IN)+16, (LPSOCKADDR*)&LocalAddr, &localLen, 
                (LPSOCKADDR*)&ClientAddr, &remoteLen);  

            CFConversion convAddr, convInfo;
            LPCTSTR pszClientAddr = convAddr.MBCS_TO_TCHAR(inet_ntoa(ClientAddr->sin_addr));
            LPCTSTR pszClientInfo = convInfo.MBCS_TO_TCHAR(pIoBuffer->m_wsaBuf.buf);

            FTLTRACE(_T("�ͻ��� %s:%d ����, ��Ϣ: %s\n"), pszClientAddr, ntohs(ClientAddr->sin_port), pszClientInfo);
        //    //pNetServer->_PostAccept(pIoBuffer);
        }
        //pIocpMgr->AssociateTask(pSocket);

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        // 2. ������Ҫע�⣬���ﴫ��������ListenSocket�ϵ�Context�����Context���ǻ���Ҫ���ڼ�����һ������
        // �����һ���Ҫ��ListenSocket�ϵ�Context���Ƴ���һ��Ϊ�������Socket�½�һ��SocketContext

        CFIocpClientSocket* pClientSocket = new CFIocpClientSocket(pIocpMgr);
        //PER_SOCKET_CONTEXT* pNewSocketContext = new PER_SOCKET_CONTEXT;
        pClientSocket->Associate((SOCKET)pIoBuffer->m_hIoHandle, ClientAddr);
        
        // ����������ϣ������Socket����ɶ˿ڰ�(��Ҳ��һ���ؼ�����)
        pNetServer->AssociateTask(pClientSocket, FALSE);

        // 3. �������������µ�IoContext�����������Socket��Ͷ�ݵ�һ��Recv��������
        CFIocpBuffer* pRecvBuffer = pNetServer->m_IocpBufferPool.Get();
        if (pRecvBuffer)
        {
            pRecvBuffer->Reset(pNetServer->m_dwBufferSize);
            pRecvBuffer->m_hIoHandle = (HANDLE)pClientSocket->m_socket;
            pClientSocket->AddBuffer(pRecvBuffer);

            // ���Buffer��Ҫ���������Լ�����һ�ݳ���
            //memcpy( pRecvBuffer->m_pBuffer, pIoBuffer->m_pBuffer, pIoBuffer->m_dwSize);


            // �����֮�󣬾Ϳ��Կ�ʼ�����Socket��Ͷ�����������
            pNetServer->_PostRecv(pRecvBuffer);        

            // 4. ���Ͷ�ݳɹ�����ô�Ͱ������Ч�Ŀͻ�����Ϣ�����뵽ContextList��ȥ(��Ҫͳһ���������ͷ���Դ)
            //this->_AddToContextList( pNewSocketContext );
        }

        // 5. ʹ�����֮�󣬰�Listen Socket���Ǹ�IoContext���ã�Ȼ��׼��Ͷ���µ�AcceptEx
        pIoBuffer->m_hIoHandle = INVALID_HANDLE_VALUE;
        pNetServer->_PostAccept(pIoBuffer);

        return TRUE;
    }

    BOOL CFIocpListenTask::AfterReadCompleted( CFIocpMgr* /*pIocpMgr */, CFIocpBuffer* /*pIoBuffer*/, DWORD /* dwBytesTransferred */ )
    {
        throw std::exception("The method or operation is not implemented.");
    }

    BOOL CFIocpListenTask::AfterWriteCompleted( CFIocpMgr* /*pIocpMgr */, CFIocpBuffer* /*pIoBuffer*/, DWORD /* dwBytesTransferred */ )
    {
        throw std::exception("The method or operation is not implemented.");
    }

    BOOL CFIocpListenTask::OnUninitialize( CFIocpMgr* /*pIocpMgr */, CFIocpBuffer* /*pIoBuffer*/, DWORD /* dwBytesTransferred */ )
    {
        throw std::exception("The method or operation is not implemented.");
    }

    //////////////////////////////////////////////////////////////////////////
    CFIocpNetServer::CFIocpNetServer()
    {
        m_nListenPort = 12345;
        m_pListenTask = NULL;
    }
    CFIocpNetServer::~CFIocpNetServer()
    {
        m_pListenTask = NULL;
        //SAFE_DELETE(m_pListenTask);
    }

    BOOL CFIocpNetServer::OnIocpStart()
    {
        BOOL bRet = FALSE;
        int rc = SOCKET_ERROR;

        // AcceptEx �� GetAcceptExSockaddrs ��GUID�����ڵ�������ָ��
        GUID GuidAcceptEx = WSAID_ACCEPTEX;  
        GUID GuidGetAcceptExSockAddrs = WSAID_GETACCEPTEXSOCKADDRS; 

        // ��������ַ��Ϣ�����ڰ�Socket
        struct sockaddr_in ServerAddress;  
        // ���Ҫʹ���ص�I/O�Ļ����������Ҫʹ��WSASocket(WSA_FLAG_OVERLAPPED)������Socket
        SOCKET socketListen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
        if (INVALID_SOCKET == socketListen)
        {
            CFNetErrorInfo err(WSAGetLastError());
            FTLTRACEEX(tlError, TEXT("Create Overlapped Listen Socket Fail, reason %d(%s)\n"),
                err, err.GetConvertedInfo());
            FTLASSERT(FALSE);
            SetLastError(err.GetInfo());
            return FALSE;
        }

        // ��ListenSocket������ɶ˿���
        FTLASSERT(NULL == m_pListenTask);
        m_pListenTask = new CFIocpListenTask(this, socketListen);
        AssociateTask(m_pListenTask, FALSE);

        // ����ַ�ṹ��Ϣ   
        ZeroMemory((char *)&ServerAddress, sizeof(ServerAddress));  
        ServerAddress.sin_family = AF_INET;  
        // �������ѡ����κ�һ�����õĵ�ַ���������Լ�ָ����һ��IP��ַ    
        ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);                         
        //ServerAddress.sin_addr.s_addr = inet_addr(m_strIP);           
        ServerAddress.sin_port = htons(m_nListenPort);                            

        // �󶨶˿�   
        NET_VERIFY(bind(socketListen, (struct sockaddr *) &ServerAddress, sizeof(ServerAddress)));

        // ��ʼ����
        NET_VERIFY(listen(socketListen, SOMAXCONN));

        // ʹ��AcceptEx��������Ϊ���������WinSock2�淶֮���΢�������ṩ����չ����
        // ������Ҫ�����ȡһ�º�����ָ��

        DWORD dwBytes = 0;  
        NET_VERIFY(WSAIoctl(
            socketListen, SIO_GET_EXTENSION_FUNCTION_POINTER, 
            &GuidAcceptEx, sizeof(GuidAcceptEx), 
            &m_lpfnAcceptEx, sizeof(m_lpfnAcceptEx), 
            &dwBytes, NULL, NULL));  
        NET_VERIFY(WSAIoctl(
            socketListen, SIO_GET_EXTENSION_FUNCTION_POINTER, 
            &GuidGetAcceptExSockAddrs, sizeof(GuidGetAcceptExSockAddrs), 
            &m_lpfnGetAcceptExSockAddrs, sizeof(m_lpfnGetAcceptExSockAddrs),   
            &dwBytes, NULL, NULL)); 

        //ΪAcceptEx ׼��������Ȼ��Ͷ��AcceptEx I/O����
        for( int i=0; i<MAX_POST_ACCEPT; i++ )
        {
            // �½�һ��IO_CONTEXT
            CFIocpBuffer* pAcceptBuffer = m_IocpBufferPool.Get();
            if (pAcceptBuffer)
            {
                pAcceptBuffer->Reset(m_dwBufferSize);
                m_pListenTask->AddBuffer(pAcceptBuffer);
                _PostAccept(pAcceptBuffer);
            }
        }

        return bRet;
    }

    BOOL CFIocpNetServer::_PostAccept(CFIocpBuffer* pioBuffer)
    {
        BOOL bRet = FALSE;
        FTLASSERT(INVALID_SOCKET != m_pListenTask->m_socketListen);

        // ׼������
        DWORD dwBytes = 0;  
        pioBuffer->m_operType = otInitialize;  
        //WSABUF *p_wbuf   = (WSABUF *)pAcceptBuffer->m_pBuffer;
        WSABUF* p_wbuf = &pioBuffer->m_wsaBuf;
        //p_wbuf->buf = (CHAR*)p_wbuf + sizeof(WSABUF);
        //p_wbuf->len = pAcceptBuffer->m_dwSize - sizeof(WSABUF);

        // Ϊ�Ժ�������Ŀͻ�����׼����Socket( ������봫ͳaccept�������� ) 
        pioBuffer->m_hIoHandle  = (HANDLE)WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 
            NULL, 0, WSA_FLAG_OVERLAPPED);  
        FTLASSERT(INVALID_SOCKET != (SOCKET)pioBuffer->m_hIoHandle);

        // Ͷ��AcceptEx
        bRet = m_lpfnAcceptEx( m_pListenTask->m_socketListen, (SOCKET)pioBuffer->m_hIoHandle, 
            p_wbuf->buf, p_wbuf->len - ((sizeof(SOCKADDR_IN)+16)*2),   
            sizeof(SOCKADDR_IN)+16, sizeof(SOCKADDR_IN)+16, &dwBytes, 
            &pioBuffer->m_overLapped);
        {
            if (!bRet)
            {
                API_VERIFY(WSA_IO_PENDING == WSAGetLastError());
            }
        }
        return bRet;
    }
    BOOL CFIocpNetServer::_PostRecv(CFIocpBuffer* pioBuffer)
    {
        BOOL bRet = TRUE;
        int rc = SOCKET_ERROR;
        pioBuffer->m_operType = otRead;

        SOCKET sock = (SOCKET)pioBuffer->m_hIoHandle;
        FTLASSERT(INVALID_SOCKET != sock);
        DWORD dwRecevd = 0;
        DWORD dwFlags = 0;
        NET_VERIFY_EXCEPT1(WSARecv(sock, &pioBuffer->m_wsaBuf, 1, &dwRecevd, &dwFlags, &pioBuffer->m_overLapped, NULL), 
            WSA_IO_PENDING);

        return bRet;
    }

    BOOL CFIocpNetServer::OnIocpStop()
    {
        //SAFE_DELETE(m_pListenTask);         
        return TRUE;
    }

}
#endif //FTL_SOCKET_HPP