#ifndef FTL_NET_H
#define FTL_NET_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlNet.h requires ftlbase.h to be included first
#endif

#include "ftlBase.h"
#include "ftlthread.h"
#include <WinInet.h>

/*************************************************************************************************************************
* ����ץ������
*   Wireshark -- ���ˣ�Capture->Options->Capture Filter->HTTP TCP port(80) 
*************************************************************************************************************************/


/*************************************************************************************************************************
* ͨ�����Ҫ���ǵģ�����ĳ�ʼ���ͷֲ�,��������,������,������,�¼�ѭ���̳�,�ݴ��
* ����Ͽ��������Ӻ���������Э���ʱ����Ҫ�漰���ӳ�(latency)����������(scalability)���ɿ���(reliability)
*   ������Э�� -- ������Ϣ��ÿ����Ϣ����Ѱַ�ͷ��ͣ����õ���UDP��IP�������ڶ�ý��ֱ��������һ���̶����ݶ�ʧ����Ҫ��ʵʱ�Եĳ���
*   ��������Э�� -- �ṩ�ɿ������򡢲��ظ��ķ��ͷ���Ϊ��֤�ɿ��Բ����˶Դ��͵�ÿ�������ֽڱ�š�У��ͼ��㡢����ȷ��(ACK)��
*     ��ʱ�ش�����������(ͨ�����������ɽ��ܷ����Ʒ��ͷ����͵�������) ��һϵ�д�ʩ��
*     �����ڵ����ʼ��Ȳ��������ݶ�ʧ�ĳ��ϣ���Ҫ�����������ѡ��
*     ���ݳ�֡����--������Ϣ(�� TP4��XTP)�������ֽ���(�� TCP)����������Ϣ�߽�
*     ���Ӷ�·���ò���--��·����(����̹߳���TCP���ӣ����ڱ��)���Ƕ�·����(ÿ���߳�ʹ�ò�ͬ�����ӣ�ͬ������С���������Բ���)
*   
*   ͬ������/Ӧ��Э�� -- ÿһ���������ͬ�����յ�һ��Ӧ�𣬲��ܷ�����һ������ʵ�ּ򵥣�������������������������������
*   �첽����/Ӧ��Э�� -- ���Խ���������������������������ͬ���ȴ�Ӧ������Ч�������磬���������ܣ�
*  
* IP��ַ(A~E ?)
*  
* �˿ں�
*   ��Ϊ���������ࣺ����֪���˿ڡ���ע��˿ڡ���̬�ͣ���˽�ö˿ڡ�
*     0 ~ 1023 �� IANA(��������ŷ�����֤)���ƣ���Ϊ�̶��������ġ�
*     1024 ~ 49151 �� IANA �г����ġ���ע��Ķ˿ڣ�����ͨ�û�����ͨ�û����̻����ʹ�á�
*     49152 ~ 65535 �Ƕ�̬�ͣ���˽�ö˿ڡ�
*************************************************************************************************************************/

/*************************************************************************************************************************
* TCPճ��(���ͷ����͵����ɰ����ݵ����շ�����ʱճ��һ�������ջ������к�һ�����ݵ�ͷ������ǰһ�����ݵ�β)���������ճ��һ������ݰ���������
* �����������Ϊ���ṹ�����ݣ���Ҫ���ְ�����(������ļ�������������������Ͳ��طְ�)
*   ���ͷ�ԭ��TCPΪ��ߴ���Ч�ʣ����ռ����㹻�����ݺ�ŷ���(Nagle�㷨)
*   ���շ�ԭ�򣺽��շ��û����̲���ʱ��ϵͳ���ջ������������ݣ���һ�����ݵ���ʱ�ͽӵ�ǰһ������֮��
* 
* �ϺõĶԲߣ�
*   ���շ�����һԤ�����̣߳��Խ��յ������ݰ�����Ԥ������ճ���İ��ֿ�
*************************************************************************************************************************/

/*************************************************************************************************************************
* ARP(Address Resolution Protocol)--��ַת��Э�飬���ڶ�̬�����IP��ַ�������ַ��ת����
* BOOTP(BooTstrap Protocol)--��ѡ��ȫ����Э�飬ʹ��UDP��Ϣ���ṩһ���и�����Ϣ�����̹���վ��ͨ���ļ��������ϵ��ڴ�Ӱ����������
* Cookie(Cookies) -- ��վΪ�˱���û���ݡ�����session���ٶ��������û������ն��ϵļ�ֵ�����ݣ�ͨ���������ܣ���
*   ����Ϊ RFC2109���ѷ������� RFC2965������͵�Ӧ�����Զ���¼�����ﳵ��
*   ������������ -> ���͸�UserAgent(�����) -> �����������ı� -> �´η���ʱ���� -> ��������֤
*   �������� -- Cookie����ʱָ����Expire����������Cookie�ͻᱻ�������λΪ��������������Ϊ"0"��ֵ��ʾ�ر�ҳ��ʱ���������
*   ��ȫ���⣺1.ʶ�𲻾�ȷ����ͬһ̨�������ʹ��ͬһ������Ķ��û�Ⱥ��cookie�����������ǵ���ݣ�����ʹ�ò��õ��û�����¼OS��
*             2.��ҳ�����ͼƬ(͸����ֻ��һ������-�Ա�����)�������з��ʹ���ҳ��ļ����д��cookie��������վ���������ʼ�
* ICMP(Internet Control Message Protocol)--����������ϢЭ�飬��Ҫ������������·��������IP���ݱ��ش����п��ܳ��ֵĲ����������
* IP(Internet Protocol)--����Э�飬�涨�����ݴ���Ļ�����Ԫ(���ķ���)�Լ��������������ʴ���ʱ��ȷ�и�ʽ�淶��
* LANA(LAN Adapter)Number -- ��Ӧ������������Э���Ψһ���
* MTU(Maximum Transmission Unit) -- ����䵥Ԫ 
* NetBios Name -- ΢�������еĻ��������õı���NetBIOS���֣���������ʱ���Ὣ����ע�ᵽ���ص�WINS��
*   ͨ�� nbtstat ��������г���Ϣ��
* OSI -- ����ϵͳ������Ӧ�ò㡢��ʾ�㡢�Ự�㡢����㡢����㡢������·�㡢����㣻
* OOB(Out Of Band) -- ��������(��������)��ͨ���������߼��ŵ������պʹ���������Ҫʹ�á�
* TCP(Transmission Control Protocol) -- �������Э�飬��������
* RARP(Reverse Address Resolution Protocol)--���ڶ�̬��������ַ��IP��ַ��ת����
* TTL -- ����ʱ�䣬��ʾһ�����ݰ��ڶ���֮ǰ�����������ϴ��ڶ೤ʱ�䣬ֵΪ0ʱ����������
* UDP(User Datagram Protocol) -- �û����ݱ�Э�飬������
* WINS -- Windows����������������,ά������ע�������NetBIOS���ֵ�һ���б�
* WinSock -- �������̽ӿڣ�������Э�飬��Э���޹ء��������һ�־���Э�飨��IP��TCP��IPX��lrDA�ȣ������׽��֡�
*  ��������Ϣ����������Ϣ�߽�--ÿ�ζ�ȡ����һ����Ϣ����������Ϸ�Ŀ��ư����� ���������������������ݴ��䣬�ᾡ���ض�ȡ��Ч���ݣ�
*  ���������ӡ��͡������ӡ�
*   �������ӵ�Э��ͬʱҲ����ʽЭ�飬������Э�鼸�����ǻ�����Ϣ�ġ�
*************************************************************************************************************************/

/*************************************************************************************************************************
* NetBios -- �����ڽ��ϵĲ���ϵͳ(��Dos��)���������ڶ������Э��(��TCP/IP��NETBEUI��IPX��)����Э���޹أ�
*   ʹ��LANA����������Э�顣
* �ض�����
* �ʲ� -- ����Windows����֮��ʵ�ֹ㲥�͵�������ͨ�š�
* �����ܵ� -- ˫���ŵ�
*
*************************************************************************************************************************/

/*************************************************************************************************************************
* �鲥�õ���D��IP��ַ224.0.0.0 -- ��Ҫ�Է���������鲥��
* �����㲥 -- �㲥�ÿ����������Ǽ�λ�ģ����ڶ���CIDR���·�ɣ�������ȫ1���ǹ㲥��ַ
*************************************************************************************************************************/


/*************************************************************************************************************************
* Socketģʽ -- ��������һ���׽��ֵ���ʱ����ЩWinSock��������Ϊ
*   ����ģʽ(ȱʡ)����I/O�������ǰ��ִ�в�����Winsock����(��send��recv)��һֱ����ȥ�������������س���
*   ������ģʽ��Winsock�������۳ɹ���(����0)����ʧ�ܶ����������أ�ʧ��ʱ����SOCKET_ERROR��������Ϊ WSAEWOULDBLOCK��
      ͨ����ҪƵ���ĵ����ж�(Ч�ʵͣ�Ӧ��ʹ��Socketģ�ͽ����첽)��
*     ioctlsocket(,FIONBIO,&1) -- ע�⣺ipmsg�Ķ˿ھ�ʹ���˸�ģʽ
*   IO���ã��� select/poll/WaitForMultiXXX ��ϵͳ����������������һ�εȴ�����ļ����˿ڵĶ�����(��/д/�쳣)   
*
* Socketģ�� -- ������һ��Ӧ�ó�����ζ��׽����Ͻ��е�I/O���й�������
*   select(ѡ��) -- ����select�����ж��׽������Ƿ�������ݣ������ܷ���һ���׽���д�����ݡ�
*     �����֣��ɶ��ԡ���д�ԡ����⡣�������غ�ͨ���ٴ��ж�socket�Ƿ��Ǽ��ϵ�һ������ȷ���Ƿ�ɶ�д��
*     ȱ�㣺���ܶ�̬�ĵ���(�����ӡ�ɾ��)Socket������̲���һ���źŲ����źŴ�����򷵻أ��ȴ�һ��ᱻ�жϣ������źŴ������ָ�� SA_RESTART ��ϵͳ֧�֣�
*   WSAAsyncSelect(�첽ѡ��) -- ������Windows��ϢΪ�����������¼�֪ͨ(�������¼����˺�����Ϣ���д���)��MFC��CAsyncSocket�ķ�ʽ��ģʽ�Զ���Ϊ��������
*     ��IPMsg��tcp���ڽ������ӣ�UDP���ڶ�ȡ���ݣ�
*       ::WSAAsyncSelect(udp_sd, hWnd, WM_UDPEVENT, FD_READ)
*       ::WSAAsyncSelect(tcp_sd, hWnd, WM_TCPEVENT, FD_ACCEPT|FD_CLOSE)
*   WSAEventSelect(�¼�ѡ��) -- ���ÿһ���׽��֣�ʹ��WSACreateEvent����һ���¼�(Ĭ�����ֶ�����)�������й�����
*     ���� WSAWaitForMultipleEvents ���еȴ������֧��64�������� WSAEnumNetworkEvents ��ȡ�������¼�
*   Overlapped I/O(�ص�ʽI/O) -- ���нϺõ����ܣ�ʹ�� WSA_FLAG_OVERLAPPED ����Socket((Ĭ������)��
*     �������Socket�����м��� WSAOVERLAPPED �ṹ��ʹ��"�������"��
*     ���� WSAResetEvent �����¼�(���¼����ֶ����õ�)��ͨ�� WSAGetOverlappedResult ��ȡ
*     ִ�н��(�����Ͳ�ʹ��Overlapped����ReadFile�Ⱥ���ʱ���صĽ��һ��)��
*     ע�⣺����Overlapped�󣬺���ͨ������ʧ�ܣ�������ΪWSA_IO_PENDING�����������������Ѿ���Cache�У���ֱ�ӷ��سɹ�
*   IO Completion Port(��ɶ˿�) -- �� Overlapped I/O Эͬ������������һ����������I/O�ĳ����л��������ܡ�
*     û��64��HANDLE�����ƣ�ʹ��һ���߳�(ͨ��������Ϊ CPU����*2+2 )����һ��Events�����ʣ��Զ�֧�� scalable��
*     ����ϵͳ���Ѿ���ɵ��ص�I/O�����֪ͨ������ɶ˿ڵ�֪ͨ����(һ���߳̽�һ��������ʱ��������)���ȴ���һ���߳�Ϊ����ʵ�ʷ���
*     �̳߳ص������߳� = Ŀǰ����ִ�е� + �������� + ����ɶ˿��ϵȴ��ġ�
*     ʹ�����̣�
*       1.����һ��I/O completion port -- hPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL...)������һ��û�к��κ��ļ�Handle�й�ϵ��port
*       2.�������ļ�handle�������� -- CreateIoCompletionPort(hFile,hPort...)��Ϊÿһ�������ӵ��ļ�Handle���й���
*       3.����һ���߳� -- for(CPU*2+2){ _beginthreadex }
*       4.��ÿһ���߳���Completion Port�ϵȴ� -- GetQueuedCompletionStatus����ͨ�� CONTAINING_RECORD ��ȡ������ָ����չ�ṹ��ָ��
*       5.��ʼ�����Ǹ��ļ�handle����һЩoverlapped I/O���� -- �� ReadFile��WriteFile��DeviceIoControl�ȣ�
*   epool -- ͨ��ϵͳ���ϵ����豸�������ݣ�ͨ��Ԥע��Ļص����������ں˲�������  
*       
*     ��ʱ�ر����֪ͨ(����Խ��ʱ)���� OVERLAPPED ��hEvent �����λ��Ϊ1
*       overlap.hEvent = (HANDLE)((DWORD)overlap.hEvent | 0x1));
*     ΢���� NET ���ṩ�� IHostIoCompletionManager
*   
*   Unix�»���
*     �ź�����I/O(SIGIO�����ں�֪ͨ��ʱ���ԡ�������һ��IO����)
*     �첽I/O(Posix �е�aio_ϵ�к���,���ں�֪ͨIO��ʱ����ɡ���aio_read->�źŴ���)
* 
* ����IPЭ���µ�ԭʼ�׽��֣�������ICMP(������������ϢЭ��)��
* Winsock API��װ�ڡ��Ự�㡱�͡����Ͳ㡱֮�䣬�ṩ��һ�ֿ�Ϊָ������Э��򿪡�����͹رջỰ��������
*
*************************************************************************************************************************/

/*************************************************************************************************************************
* ͷ�ļ�
*   <netinet/in.h>  -- ����IPv4/IPv6�ĵ�ַ�ṹ
*   <arpa/inet.h> <== ������ inet_pton/inet_ntop
* 
* �ṹ
*   sockaddr_in/sockaddr_in6 -- ��������׽ӿڵ�ַ�ṹ��ÿ��Э���嶼���������Լ����׽ӿڵ�ַ�ṹ�����е�ַ�Ͷ˿ںű����������ֽ��򱣴�
*     AF_INET/AF_INET6
*
* socket API���¿��Է�Ϊ���ࣺ
*   A.���ػ�����������Ϣͨ��������OS�ں˻�ϵͳ����
*     1.socket -- ����һ��socket����������ظ�������
*     2.bind -- ��һ��socket�����һ�����ػ�Զ�̵�ַ��������
*     3.getsockname -- ����socket�󶨵ı��ص�ַ
*     4.getpeername -- ����socket�󶨵�Զ�̵�ַ
*     5.close/closesocket -- �ͷ�socket�����ʹ��ɸ���
*   B.���ӵĽ�������ֹ
*     1.connect -- ������һ��socket����Ͻ������ӣ���������ó�ʱ������ioctlsocket��socket����Ϊ�Ƕ����ģ���connectʱ���������أ�
*         Ȼ������select�����ȴ� readfd �����ó�ʱֵ
*     2.listen -- ��ʾԸ�ⱻ���������Կͻ�����������
*     3.accept -- ������������Ӧ�ͻ����󣬴���һ���µ����ӣ����û�����󣬽�����
*     4.shutdown -- ��ѡ�����ֹ˫�������ж�ȡ����д�뷽��������
*   C.���ݴ��ͻ���
*     1.send/recv/ReadFile/WriteFile -- ͨ��ĳһ�ض���I/O��������ͺͽ������ݻ�����
*     2.sendto/recvfrom -- �������������ݱ���ÿһ�ε��ö���������շ�/���ͷ��������ַ
*     Unix/Linuxƽ̨ר�ú���
*     3.read/write -- ͨ��ĳһ��������պʹ������ݻ�����
*     4.readv/writev -- �ֱ�֧�֡���ɢ��ȡ���͡�����д�롱���壬���Ż���ģʽ�л��������ڴ����
*     5.sendmsg/recvmsg -- ͨ�ú����������������ݴ��亯������Ϊ
*     recv -- ����ʹ�ñ�־ MSG_WAITALL ����ȡ��Ҫ����������
*   D.ѡ�����
*     1.setsockopt/getsockopt -- ��Э��ջ�Ĳ�ͬ�� ���Ļ�õ� ѡ��
*       ������ SocketOption(SOL_SOCKET)
*         SO_DEBUG        turn on debugging info recording 
*         SO_ACCEPTCONN   socket has had listen()
*         SO_REUSEADDR    �������ô��� TIME_WAIT ״̬���׽��ֽ������ӡ�
*         SO_KEEPALIVE    keep connections alive -- ����(�Զ���)�������ӣ�����Զ˱�����������֪������ô֪������
*         SO_DONTROUTE    just use interface addresses 
*         SO_BROADCAST    �����͹㲥��Ϣ��֮�����ͨ�������й㲥
*         SO_USELOOPBACK  bypass hardware when possible 
*         SO_LINGER       linger on close if data present 
*         SO_OOBINLINE    leave received OOB data in line 
*         SO_SNDBUF       ���ͻ������Ĵ�С
*         SO_RCVBUF       ���ջ������Ĵ�С
*         SO_RCVTIMEO     ���ճ�ʱ��ʱ��(Windows ��֧�֣�)
*     2.ioctlsocket -- ����socket��IOģʽ������ FIONBIO(�����Ƿ��������ģʽ)
*   E.�����ַ -- �ڿɶ��Ե�����(������)�͵ͼ������ַ(��IP)֮�����ת��
*     1.gethostbyname/gethostbyaddr -- ������������ IPV4 ��ַ֮��������ַӳ��
*     2.getipnodebyname/getipnodebyaddr -- ������������ IPV6 ��ַ֮��������ַӳ��
*     3.getservbyname -- ͨ�����пɶ��Ե����Ʊ�ʶ����
*       getpeername -- 
*     4.inet_aton/inet_ntoa -- �ڵ��ʮ��������(��192.168.0.1)���Ӧ��32λ�����ֽ��������ֵ��ת����ֻ������IPv4
*       ע�� inet_ntoa ���ص��ַ����洢�ھ�̬�ڴ��У����̰߳�ȫ��
*     5.inet_pton/inet_ntop <== presentaion(�ַ������)<->numeric(�׽ӿڵ�ַ�ṹ�еĶ�����ֵ)��������IPv4��IPv6��
*         inet_ntop(AF_INET,&cliaddr.sin_addr, buff, sizeof(buff));
*         inet_pton(AF_INET,"192.168.0.1", &addr.sin_addr, ...); 
*
* ע�⣺
*   1.Socket APIʹ��һ�ֺܻ����ĵ�ַ�ṹ(sockaddr),���ݵ�ַ�ز�ͬ���ṹ��Ա��ռλҲ��ͬ��
*     ��IPV4�� sockaddr_in,ʹ��ǰһ��Ҫȫ�����㣬�������׳��ִ���
*   2.ע�������ֽ���������ֽ��������
*     �����ֽ��򣺶˿�(addr.sin_port)��IP��ַ
*   3.Ĭ������£������TCP/IPʵ�ֻ�ʹ��Nagle�㷨�����ڷ��Ͷ��л����������η��͵����ݱ���ͨ���������
*     ��������ӵ���������������ӳٲ���������������ͨ�� TCP_NODELAY ѡ��رգ���ֹ���ݺϲ���
*     �����ھ���������С���ݱ�(���� IPMsg )
*       :setsockopt(info->sd, SOL_SOCKET, TCP_NODELAY, (char *)&flg, sizeof(flg));

*************************************************************************************************************************/

/*************************************************************************************************************************
* TransmitFile -- ���ں��и����ܵĴ����ļ�����
* WSAStartup/WSACleanup --��ʼ�������
* WSAEnumProtocols -- ���ϵͳ�а�װ������Э��������Ϣ
* WSASocket(��ַ����,�׽�������,Э��),���ǰ�������������� FROM_PROTOCOL_INFO ������ָ�� WSAPROTOCOL_INFO�ṹ����ʹ�ýṹ�еġ�
*   �������� WSAEnumProtocols ö�٣�Ȼ�󴴽�ָ����Socket
*************************************************************************************************************************/

/*************************************************************************************************************************
* MFC�е�������������
*  CAsyncSocket -- �첽������Socket��װ��,ͨ������CSocketWnd���岢ͨ��WSAAsyncSelect�����첽����.
*    ����(�� Connect )���ú�,���� WSAEWOULDBLOCK,��Ҫ�� OnXXX����(��OnConnect)�еȴ����жϴ�����.
*  CSocket -- �� CAsyncSocket�̳е�ͬ��������װ��.��CAsyncSocket��������WSAEWOULDBLOCK��,
*    ͨ�� PumpMessage �����ӵ�ǰ�̵߳���Ϣ������ȡ���ĵ���Ϣ,
*************************************************************************************************************************/

/*************************************************************************************************************************
* FTP
*    ��Ϊ���ƶ˿�(21)�����ݶ˿�(����ʱ��20���ȿ��ƶ˿ڵ�һλ)�������ӣ�vsftp��ÿһ�����ӻ�������vsftpd���̡�
*    ��Ϊ��������ģʽ��
*         ����(PORT) -- ������ʱ���ͻ�����PORT������߷�������21�˿�(�ͻ��˵�IP)��Ȼ�����������20�˿���������ȥ�����з�������֧�֡�
*         ����(PASV) -- ������ʱ����������PASV������߿ͻ���(����XXX�˿�)���ɿͻ�������ȥ���󲿷ַ�������֧�֣���Ҫָ���˿ڷ�Χ��
*             ��������ǽ��
*    ע�⣺ѡ���� PASV ��ʽ ���� PORT ��ʽ��¼FTP����ѡ��Ȩ�ڿͻ��ˣ������Ƿ���ˡ�
*************************************************************************************************************************/

/*************************************************************************************************************************
* WebBrowser
*   ʹ�÷�ʽ
*     CLSID_WebBrowser--��Ƕ�Ŀؼ�����Ӧ��ActiveXΪ"Microsoft Web Browser"
*     CLSID_InternetExplorer--�ⲿ����Exe����IE
*   �ܹ�
*     WebBrowser Host -- ����,��������WebBrowser Control��Ӧ�ó����� CLSID_InternetExplorer
*     Shdocvw.dll(contains WebBrowser control) -- ��װ������Webbrowser control�����ϲ������ṩ�������
*     mshtml.dll -- ��ʾHTML�ĵ��������Ҳ��һ��active �ĵ��������������ؼ�(��ű����桢Plugin)������
*     ActiveXControl/Plugin/HTML
*  �ӿ�
*    IWebBrowser -- WebBrowser Control���ṩ���غ���ʾWeb��Word��ҳ��Ļ�������
*      Navigate--������ָ��ҳ����ļ���
*        PostData -- ��ָ��������(�� ?)ͨ��POST�����͵����������粻ָ���κ����ݣ���ʹ��Get����
*        Headers -- ����HTTPͷ��Ϣ(��?)��������
*    IWebBrowserApp -- Internet Explorer,�̳��� IWebBrowser�����Կ���״̬��,���������û��ӿ�
*      GetProperty/PutProperty -- ��ȡ������IE���԰�(property bag),��������԰�����Щ? 
*    IWebBrowser2 -- WebBrowser Control and Internet Explorer,�̳��� IWebBrowserApp(����IEʱ��ѡ�ýӿ�)
*      ExecWB -- IOleCommandTarget::Exec�İ�װʵ��,����WebBrowser�����������µĹ��ܶ����ô����µĽӿڣ�
*                ��ͨ�� ExecWB(OLECMDID_ZOOM, OLECMDEXECOPT_DONTPROMPTUSER, 4, NULL) ʵ��"�������Ϊ���"�Ĺ���
*                ������ OLECMDID_PAGESETUP(��ӡ����)��OLECMDID_PRINT(��ӡ) ��
*    DWebBrowserEvents2 -- �¼�
*************************************************************************************************************************/

/*************************************************************************************************************************
* HTTP(Hypertext Transport Protocol) -- ���ı�����Э��, RFC1945����1.0, RFC2616�����Ϊʹ�õ� 1.1������״̬��Э�顣
*   ͨ��������TCP��TLS��SSL(��HTTPS��Ĭ�϶˿�443)Э���֮�ϣ���Ϊ POST(RFC1867) �� GET(RFC???)
*
* HTML VERBS(�ַ���)
*   POST
*   GET
*   HEAD
*   PUT
*   LINK
*   DELETE
*   UNLINK
*
*
* ʹ��HTTP��ȡ���ݵĲ���: 
*   1.�ͻ��˽�������
*   2.�ͻ��˷�������
*     ���ݰ�����ͳһ��Դ��ʶ����URL����Э��汾�ţ������MIME��Ϣ�����������η����ͻ�����Ϣ�Ϳ��ܵ�����
*   3.������������Ӧ��Ϣ����ʽΪһ��״̬�У�������Ϣ��Э��汾�š�һ���ɹ������Ĵ��룬�����MIME��Ϣ������������Ϣ��ʵ����Ϣ�Ϳ��ܵ�����
* 
* ͷ��(����:��ֵ)��ÿ������������"\r\n"�ֿ���������������"\r\n"
*   [M]Accept: text/plain, x/x(ע�⣺�˴�������*��)
*   [O]Accept-Encoding: gzip, deflate
*   [O]Accept-Language: zh-cn
*   [O]Cache-Control: ָ���������Ӧ��ѭ�Ļ�����ƣ��� no-cache, max-age ��
*   [M]Content-Type: multipart/form-data; boundary=--MULTI-PARTS-FORM-DATA-BOUNDARY
*     �����б� -- rfc1341( http://www.ietf.org/rfc/rfc1341.txt )
*     //AVI video/avi
*     //JPG image/jpeg
*     //PNG image/x-png
*     //BMP image/bmp
*     //TIF image/tiff
*     //GIF image/gif
*     //TXT text/plain
*     //XML text/xml
*   []Content-Disposition: -- �ɴ���������ļ�(ǰ������� ----MULTI-PARTS-FORM-DATA-BOUNDARY )
      Э��ͷʱ��form-data; name=\"attach_file\"; filename=\"xxxx\"    -- ���е� "attach_file" �Ǹ�����վ��ͬ��?
      Э��βʱ��form-data; name=\"icount\" + CRLF + CRLF + _T("1") + CRLF + 
      \"submitted\"
*   [M]Content-Length: ���ݳ��ȣ����� ͷ + ���� + β��
*   [O]Content-Transfer-Encoding: binary
*   []Connection: Keep-Alive
*   []Cookie : ?? �Ƿֺ�";"�ֿ��Ķ�� "������=ֵ" �ļ�ֵ��
*   []Date: ��ʾ��Ϣ���͵�ʱ�䣬��RFC822���壬�������׼ʱ���� Sat, 26 May 2012 00:42:19 GMT
*   [O]Host: 192.168.0.8�����������б����������?
*   []Referer: http://Դ��Դ��ַ�� �����������������ɻ���������������½���Ż���cache��
*   []User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0) ���Զ�������֣��Ƿ���������û���Ϣ
                
* GET /�ļ���ַ HTTP/1.1
*   []Range: bytes=388136960-   (��ʽ����ô����?)
*   []Content-Range: bytes ��ʼλ��-����λ��/��λ�� (��ȷ���Ƿ��������ʽ)

* ������Ϣ
*   
*   []Server: Apache/2.2.11 (Unix)
*   []Last-Modified: Wed, 23 May 2012 21:38:23 GMT
*   []ETag: "1d2d6-5c896800-4c0baf45c91c0"
*   []Accept-Ranges: bytes
*   []Content-Length: 1164371968
*   []Content-Range: bytes 388136960-1552508927/1552508928
*   []Keep-Alive: timeout=5, max=100
*
*     InternetGetConnectedState()
*     InternetOpen
*     InternetConnect
*     InternetAttemptConnect
*     HttpOpenRequest
*     HttpSendRequest
*     HttpQueryInfo
*     InternetCloseHandle(hRequest,hConnect, hOpen)
*************************************************************************************************************************/

namespace FTL
{
    #ifdef FTL_DEBUG
        //send �Ⱥ����ɹ�ʱ���ط��͵��ֽ���
        #define NET_VERIFY(x)	\
        {\
            rc = (x);\
            if(SOCKET_ERROR == rc)\
            {\
                int lastSocketError = WSAGetLastError();\
                REPORT_ERROR_INFO(FTL::CFNetErrorInfo, lastSocketError, x);\
                WSASetLastError(lastSocketError);\
            }\
        }
    #define FTL_DEBUG_EXCEPT1(x, e1) \
        {\
            rc = (x);\
            if(SOCKET_ERROR == rc)\
            {\
                int lastSocketError = WSAGetLastError();\
                if((e1) != lastSocketError)\
                {\
                    REPORT_ERROR_INFO(FTL::CFNetErrorInfo, lastSocketError, x);\
                }\
                WSASetLastError(lastSocketError);\
            }\
        }
    #else
        #define NET_VERIFY(x)	\
            rc = (x);
        #define FTL_DEBUG_EXCEPT1(x, e1) \
            rc = (x);
    #endif

    #define SAFE_CLASE_WSAEVENT(h)\
        if(WSA_INVALID_EVENT != (h))\
        {\
            WSACloseEvent((h));\
            (h) = WSA_INVALID_EVENT;\
        }

    #define SAFE_CLOSE_SOCKET(h)\
        if(INVALID_SOCKET != (h))\
        {\
            NET_VERIFY(closesocket((h)));\
            (h) = INVALID_SOCKET;\
        }\

    FTLEXPORT class CFNetErrorInfo : public CFConvertInfoT<CFNetErrorInfo,int>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFNetErrorInfo);
    public:
        FTLINLINE explicit CFNetErrorInfo(int err);
        FTLINLINE virtual LPCTSTR ConvertInfo();
    };

    namespace NetInfo
    {
        //��ȡЭ��ĵ�ַ���壺�� AF_INET / AF_INET6
        FTLINLINE LPCTSTR GetAddressFamily(int iAddressFamily);

        //��ȡ�׽�������
        FTLINLINE LPCTSTR GetSocketType(int iSocketType);

        //��ȡָ����ַ�����Э�飺 �� AF_INETx �е� IPROTO_IP/IPROTO_TCP/IPROTO_UDP ��
        FTLINLINE LPCTSTR GetProtocolType(int iAddressFamily,int iProtocol);

        FTLINLINE LPCTSTR GetServiceFlagsType(DWORD dwServiceFlags);
        FTLINLINE LPCTSTR GetProviderFlagsType(DWORD dwProviderFlags);

        //��ȡ���ص�IP��ַ
        FTLINLINE LONG GetLocalIPAddress();
    }

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

    FTLEXPORT template <typename T>
    class CFSocketT
    {
    public:
        FTLINLINE CFSocketT();
        FTLINLINE virtual ~CFSocketT();
        FTLINLINE int Open(FSocketType st, BOOL bAsync);
        FTLINLINE int Close();
        FTLINLINE int Shutdown(INT how);

        FTLINLINE int IoCtl(long cmd, u_long* argp);

    //protected:
        SOCKET              m_socket;
        FSocketType         m_socketType;
        BOOL                m_bASync;
        CFCriticalSection   m_lockObject;
        volatile UINT       m_nSession;
    };

    FTLEXPORT template <typename T>
    class CFClientSocketT : public CFSocketT<T>
    {
    public:
        //Client
        FTLINLINE int Connect(LPCTSTR pszAddr, INT nSocketPort);

        //Data
        FTLINLINE int Send(const BYTE* pBuf, INT len, DWORD flags);
        FTLINLINE int Recv(BYTE* pBuf, INT len, DWORD flags);
        FTLINLINE int Associate(SOCKET socket, struct sockaddr_in * pForeignAddr);
    protected:
        struct sockaddr_in		m_foreignAddr;
    };

    FTLEXPORT template <typename T>
    class CFServerSocketT : public CFSocketT< T >
    {
    public:
        FTLINLINE CFServerSocketT();
        FTLINLINE ~CFServerSocketT();
        //Server
        FTLINLINE int StartListen(INT backlog, INT nMaxClients);
        FTLINLINE int Bind(USHORT listenPort, LPCTSTR pszBindAddr);
        FTLINLINE CFClientSocketT<T>* Accept();
    protected:
        CFMemCacheT<CFClientSocketT <T> >*    m_pClientSocketPool;
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
        CFSocketT<T>*    m_pClientSocket;
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
        //CFSocketT<T>* Accept();
    protected:
        FSocketType                 m_socketType;
        CFServerSocketT<T>*         m_pServerSocket;

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
        static FTLINLINE size_t readn(int fd, void* vptr, size_t n);
        static FTLINLINE size_t writen(int fd, const void* vptr, size_t n);
    };

	typedef std::map<tstring, tstring> CookiKeyValueMap;
	class CFNetUtil
	{
	public:
		FTLINLINE static LPCTSTR GetCookieInfo(CFStringFormater& formater, LPCTSTR lpszUrl, LPCTSTR lpszCookieName);
		FTLINLINE static DWORD GetCookieInfoMap(LPCTSTR pszCookies, CookiKeyValueMap& cookieMap);
	};

	class CUrlComponents : public URL_COMPONENTS
	{
	public:
		FTLINLINE CUrlComponents();
		FTLINLINE BOOL ParseUrl( LPCTSTR pstrURL, DWORD& dwServiceType,
			WORD& nPort, DWORD dwFlags );
	private:
		TCHAR m_szScheme[INTERNET_MAX_SCHEME_LENGTH];
		TCHAR m_szHostName[INTERNET_MAX_HOST_NAME_LENGTH];
		TCHAR m_szUserName[INTERNET_MAX_USER_NAME_LENGTH];
		TCHAR m_szPassword[INTERNET_MAX_PASSWORD_LENGTH];
		TCHAR m_szUrlPath[INTERNET_MAX_URL_LENGTH];
		TCHAR m_szExtraInfo[INTERNET_MAX_PATH_LENGTH];
	};

	//////////////////////////////////////////////////////////////////////////
	#define __HTTP_VERB_GET			TEXT("GET")
	#define __HTTP_VERB_POST		TEXT("POST")
	#define __HTTP_ACCEPT_TYPE		TEXT("*/*")
	#define __HTTP_ACCEPT			TEXT("Accept: */*\r\n")
	#define __SIZE_BUFFER			1024

#if 0
	class CFGenericHTTPClient
	{
	public:
		struct GenericHTTPArgument
		{							// ARGUMENTS STRUCTURE
			ATL::CAtlString	szName;
			ATL::CAtlString  szValue;
			DWORD	dwType;

			GenericHTTPArgument()
			{
				szName = TEXT("");
				szValue = TEXT("");
				dwType = 0;
			}
			bool operator==(const GenericHTTPArgument &argV)
			{
				if (argV.dwType == dwType
					&& argV.szName == szName
					&& argV.szValue == szValue)
				{
					return true;
				}
				return false;
				//return !lstrcmp(szName, argV.szName) && !_tcscmp(szValue, argV.szValue);
			}
		};

		enum RequestMethod
		{
			// REQUEST METHOD
			RequestUnknown = -1,
			RequestGetMethod = 0,
			RequestPostMethod,
			RequestPostMethodMultiPartsFormData
		};

		enum TypePostArgument
		{
			// POST TYPE
			TypeUnknown = -1,
			TypeNormal = 0,
			TypeBinary
		};

		FTLINLINE CFGenericHTTPClient();
		virtual ~CFGenericHTTPClient();

		// HTTP Method handler 
		BOOL Request(LPCTSTR szURL, LPCTSTR szAgent,
			RequestMethod nMethod = RequestGetMethod);
	protected:
		BOOL Connect(LPCTSTR szAddress, 
			WORD nPort = INTERNET_DEFAULT_HTTP_PORT,
			LPCTSTR szAgent = NULL,
			LPCTSTR pszCookie = NULL,
			LPCTSTR szUserAccount = NULL, 
			LPCTSTR szPassword = NULL);
		BOOL Close();

		BOOL RequestGet(LPCTSTR szURI);

	private:
		std::vector<GenericHTTPArgument>	m_vArguments;				// POST ARGUMENTS VECTOR
		std::vector<CAtlString>				m_vRequestHeaders;

		CFStringFormater	m_formaterHTTPResponseHeader;
		CFStringFormater	m_formaterHTTPResponseHTML;
		//TCHAR		m_szHTTPResponseHeader[__SIZE_HTTP_BUFFER];	// RECEIVE HTTP HEADR
		//TCHAR		m_szHTTPResponseHTML[__SIZE_HTTP_BUFFER];		// RECEIVE HTTP BODY

		HINTERNET	m_hOpen;				// internet open handle
		HINTERNET	m_hConnection;			// internet connection hadle
		HINTERNET	m_hRequest;				// internet request hadle
		
		DWORD		m_dwError;				// LAST ERROR CODE
		DWORD		m_dwResultSize;
	};
#endif 
	enum DOWMLOAD_END_CODE
	{
		DOWN_OK = 0,
		DOWN_ERROR,
		DOWN_CANCEL
	};

	class IFDownloadCallbackEvent
	{
	public:

		virtual void OnDownloadProgress( LPCTSTR URL, LPCTSTR Path, int nCurrent ) = 0;
		virtual void OnDownloadEnd( LPCTSTR URL, LPCTSTR Path, DOWMLOAD_END_CODE errorCode ) = 0;
	};

#if 0
	class CFHttpDownloader
	{
	public:
		CFHttpDownloader( void );
		~CFHttpDownloader( void );
		void	HttpDownAsync();
		BOOL	HttpDown();

		void	Cancel()
		{
			//m_bContinue = FALSE;
		}
	};
#endif

	class CFWebBrowserDumper : public CFInterfaceDumperBase<CFWebBrowserDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFWebBrowserDumper);
	public:
		FTLINLINE explicit CFWebBrowserDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFWebBrowserDumper>(pObj, pInfoOutput, nIndent){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	//����IE��ʵ��
	class CFWebBrowserAppDumper : public CFInterfaceDumperBase<CFWebBrowserAppDumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFWebBrowserAppDumper);
	public:
		FTLINLINE explicit CFWebBrowserAppDumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFWebBrowserAppDumper>(pObj, pInfoOutput, nIndent){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

	class CFWebBrowser2Dumper : public CFInterfaceDumperBase<CFWebBrowser2Dumper>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFWebBrowser2Dumper);
	public:
		FTLINLINE explicit CFWebBrowser2Dumper(IUnknown* pObj, IInformationOutput* pInfoOutput, int nIndent)
			:CFInterfaceDumperBase<CFWebBrowser2Dumper>(pObj, pInfoOutput, nIndent){}
	public:
		FTLINLINE HRESULT GetObjInfo(IInformationOutput* pInfoOutput);
	};

}

#ifndef USE_EXPORT
#  include "ftlNet.hpp"
#endif

#endif //FTL_NET_H