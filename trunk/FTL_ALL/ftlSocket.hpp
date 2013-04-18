#ifndef FTL_SOCKET_HPP
#define FTL_SOCKET_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlSocket.h"
#endif

namespace FTL
{
	CFWinsockEnvInit::CFWinsockEnvInit(WORD wVersionRequested /* = MAKEWORD(2, 2) */)
	{
		ZeroMemory(&m_wsaData, sizeof(m_wsaData));
		m_initResult = WSAStartup(wVersionRequested, &m_wsaData);

		//Success
		if (0 != m_initResult)
		{
			FTLTRACEEX(tlError, TEXT("WSAStartup Error: %d(%s)"), 
				m_initResult, CFNetErrorInfo(m_initResult).GetConvertedInfo());
		}
		FTLASSERT(m_initResult == 0);
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

	template<typename T>
	CFSocketT<T>::CFSocketT()
	{
		m_socket = INVALID_SOCKET;
		m_socketType = stTCP;
		m_bASync = TRUE;
		m_nSession = (UINT)(-1);
	}

	template<typename T>
	CFSocketT<T>::~CFSocketT()
	{
		FTLASSERT(!IsOpen() && TEXT("Forget Close The Socket"));
		Close();
	}

	template<typename T>
	BOOL CFSocketT<T>::IsOpen()
	{
		BOOL isOpen = (INVALID_SOCKET != m_socket);
		return isOpen;
	}

	template<typename T>
	int CFSocketT<T>::Open(FSocketType st, BOOL bAsync)
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

	template<typename T>
	int CFSocketT<T>::Close()
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

	template<typename T>
	int CFSocketT<T>::Shutdown(INT how)
	{
		FTLASSERT(INVALID_SOCKET != m_socket);
		int rc = NO_ERROR;
		rc = shutdown(m_socket, how);
		return rc;
	}

	template<typename T>
	int CFSocketT<T>::Connect(const CFSockAddrIn& addrConnect)// (LPCTSTR pszAddr, INT nSocketPort)
	{
		//FTLASSERT(pszAddr);
		//FTLASSERT(nSocketPort);

		int rc = NO_ERROR;
		int nLength = sizeof(addrConnect);
		//sockaddr_in addrConnect = {0};

		//addrConnect.sin_family = AF_INET;
		//addrConnect.sin_port = htons(nSocketPort);
		//addrConnect.sin_addr.S_un.S_addr = inet_addr(CT2A(pszAddr));

		NET_VERIFY(WSAConnect(m_socket, &addrConnect, nLength, NULL,NULL,NULL,NULL));

		return rc;
	}


	template<typename T>
	int CFSocketT<T>::Send(const BYTE* pBuf, INT len, DWORD flags)
	{
		int rc = NO_ERROR;
		WSABUF sendBuf = {0};
		sendBuf.len = (ULONG)len;
		sendBuf.buf = (char*)pBuf;
		DWORD dwNumberOfSend = 0;
		NET_VERIFY(WSASend(m_socket,&sendBuf, 1,&dwNumberOfSend,flags, NULL, NULL));
		return rc;
	}

	template<typename T>
	int CFSocketT<T>::Recv(BYTE* pBuf, INT len, DWORD flags)
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

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	int CFClientSocketT<T>::Associate(SOCKET socket, PSOCKADDR_IN pForeignAddr)
	{
		FTLASSERT(m_socket == INVALID_SOCKET);
		int rc = NO_ERROR;
		this->m_socket = socket;
		memcpy(&m_foreignAddr, pForeignAddr, sizeof(SOCKADDR_IN));
		return rc;
	}

	//////////////////////////////////////////////////////////////////////////
	template<typename T>
	CFServerSocketT<T>::CFServerSocketT()
	{
		m_pClientSocketPool = NULL;
	}

	template<typename T>
	CFServerSocketT<T>::~CFServerSocketT()
	{
		SAFE_DELETE(m_pClientSocketPool);
	}

	template<typename T>
	void CFServerSocketT<T>::OnClose()
	{
		if (m_pClientSocketPool)
		{
			
		}
	}

	template<typename T>
	int CFServerSocketT<T>::Bind(USHORT listenPort, LPCTSTR pszBindAddr  = NULL )
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

	template<typename T>
	int CFServerSocketT<T>::StartListen(INT backlog, INT nMaxClients)
	{
		int rc = NO_ERROR;
		m_pClientSocketPool = new CFMemCacheT<CFClientSocketT <T> >(0,nMaxClients);

		FTLASSERT( m_socket != INVALID_SOCKET );
		NET_VERIFY(listen(m_socket, backlog));
		return rc;
	}

	template<typename T>
	CFClientSocketT<T>*  CFServerSocketT<T>::Accept()
	{
		SOCKET soAccept = INVALID_SOCKET;
		struct sockaddr_in addrClient = {0};
		int nLength = sizeof(struct sockaddr_in);
		CFClientSocketT<T> *pRetClient = NULL;

		// Check if there is something in the listen queue.
		soAccept = WSAAccept(m_socket, (struct sockaddr *) &addrClient, 
			&nLength, NULL, NULL);

		// Check for errors.
		if (soAccept != INVALID_SOCKET) 
		{
			// Get a pointer to the free ClientSocket 
			// instance from the pool.
			pRetClient = m_pClientSocketPool->Get();
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
			CFClientSocketT<T>* pClient = m_pServerSocket->Accept();
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
		CFClientSocketT<T>* pClientSocket;//���ڻ�õڶ���CreateIoCompletionPortʱ����Ĳ���
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
		m_pServerSocket = new CFServerSocketT<T>();
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
}
#endif //FTL_SOCKET_HPP