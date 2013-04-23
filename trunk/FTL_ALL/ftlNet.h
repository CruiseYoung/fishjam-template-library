#ifndef FTL_NET_H
#define FTL_NET_H

#pragma once

/*************************************************************************************************************************
* �Ѷ�����
*   netds\http\AsyncServer(δ���꣬��Ҫ�ȿ��̳߳ء�IO��ɶ˿�) -- ʹ�� IO��ɶ˿� + �̳߳� ʵ�ֵ��첽HttpV2������
*   netds\http\httpauth -- HttpV2 �������������֤����(�򵥵�NTLM)������ʱ���� http://localhost:80/
*   netds\http\HttpV2Server -- HttpV2 ������(Postʱ�޷���ȡ�ϴ����ļ����ݣ�)
*   netds\http\server -- ʹ�� winhttp ʵ�ֵļ� HTPPV1 ��������֧��GET��POST
*   netds\uri -- ��ʾ��������ʾIUri�ӿ����Ե��÷�(��Ҫ���� _WIN32_IE=0x0700����ʹ�� 7.0 ���ϵ�SDK)
*   netds\winsock\mcastip -- �ಥ���������汾���ֱ�ʹ�� setsockopt �� WSAJoinLeaf ʵ��
*   web\Wininet\CacheEnumerate -- ʹ��Wininet��Cache APIö�١�ɾ��URL��ص�����(-d ������Σ�գ����ܻ��cookieɾ��)
*   web\Wininet\httpauth -- ͨ��HTTP����Webҳ��ʱ������Ƿ���Ҫ��֤��Ϣ(������ҳ��) -- ����û�в��Գ����
*************************************************************************************************************************/

/*************************************************************************************************************************
* POST�ϴ��ļ�ʱ����������
* ע�⣺PostArgument �Ĳ����� �� ����ֵ ֮�������� <CRLF>���ļ�����Ϣ(·��/Content-Type)���ļ�����֮��Ҳ������ <CRLF>
*       
*   Command: POST
*   URI: /api/file/upload.php
*   {RequestHeader} -- 
*     Content-Type: multipart/form-data; boundary=[Boundary]<CRLF> 
*     Content-Length: [�ܳ���]<CRLF>
*     User-Agent:FTL Translate<CRLF>
*     Host: upload.mytest.com <CRLF>
*     Cookie: xxxxx <CRLF>
*       ������ Accept��Cache-Control�� <CRLF>
*     <CRLF> -- ���һ�����һ�� <CRLF> ��ʾ RequestHeader �Ľ���, ʵ��������ط��༸�� <CRLF> �ƺ�Ҳû�����⣬ֻҪ���ȼ�����ȷ����
*   [PostArgument]
*     --[Boundary]<CRLF>Content-Disposition: form-data; name="������"<CRLF><CRLF>����ֵ<CRLF>
*   [LocalFile]
*     --[Boundary]<CRLF>Content-Disposition: form-data; name="file"; filename="�ļ�·��"<CRLF>
*                       Content-Type:application/octet-stream<CRLF>  -- ע�⣺�����ѡ
*                 <CRLF>�ļ�����<CRLF>
*   [����]
*     --[Boundary]--<CRLF>
* 
* �ܳ��ȼ��㷽ʽ: RequestHeader + PostArgument + �ļ���С
*************************************************************************************************************************/

/*************************************************************************************************************************
* Http �������ܲ���(�ļ���С��12M) -- ���������⣺1.���� �� д�ļ���һ����
*   ϵͳ����Buffer       ReadBuffer         ʱ��
*      4K(Ĭ��)             64K              42S
*      4K(Ĭ��)             16K              33S
*      4K(Ĭ��)             4K               29S
*      4K(Ĭ��)             0K               22S
*
*      16K                  0K               24S
*      16K                 16K               28S
*
*      64K                 0K/4K             49S
*************************************************************************************************************************/

#pragma TODO(wsock32.lib �� ws2_32.lib ������)
//CHttpFile ����һ�� m_pbWriteBuffer ��������Ҫ���͵�����, Write ʱ����һ���������ķ��ͳ�ȥ
//AfxParseURL -- MFC�н�������û��ATL�汾

//? InternetConnect + HttpOpenRequest(Get/Post ��) == InternetOpenUrl(����ֱ�Ӵ��� URL����Щ����ֻ������HTTP URL?)

#ifndef FTL_BASE_H
#  error ftlNet.h requires ftlbase.h to be included first
#endif

#include <list>
#include "ftlthread.h"
#include "ftlThreadPool.h"
#include "ftlSharePtr.h"
#include "ftlCom.h"
#include "ftlBuffer.h"

//Ŀǰ�����汾�г�ͻ������ͨ��ɾ�� winhttp.h �г�ͻ�Ĳ��֣������ĺ���ļ����ڹ����еķ��������
#ifdef USE_WIN_HTTP
#  include <winhttp.h>	//��ǿ�棬ͨ����д��Ҫ �ر��ܵĿͻ���(�磿) �� ������
#else
#  include <WinInet.h>	//�����棬ͨ����д�ͻ��ˣ�������Gopher��FTP��HTTPЭ��
#endif
#include <Iphlpapi.h>
#include <ws2tcpip.h>

#include <atlbase.h>
#include <atlstr.h>

//Ĭ�ϵ���������BufferSize
#ifndef INTERNET_BUFFER_SIZE
#  define	INTERNET_BUFFER_SIZE	4096
#endif //INTERNET_BUFFER_SIZE

/*************************************************************************************************************************
* RFC -- http://www.rfc-editor.org/info/rfcXxx
*  1889/3550 -- RTP(Real-time Transport Protocol)
*  2616 -- HTTP(Hypertext Transfer Protocol)
*  6443 -- RFB(Remote Framebuffer Protocol), ͨ������Զ�̿���

* Fiddler(http���Դ���) -- ��¼�����������ĵ��Ժͻ�����֮���httpͨѶ���鿴���н�������
* Httpwatch/Fiddler2 -- ר�ż��Http����Ĺ���?
* Wireshark(����ץ������) -- ���ˣ�Capture->Options->Capture Filter->HTTP TCP port(80) 
* IECookiesView(www.nirsoft.net): �鿴�༭Cookie
* netstat -- ϵͳ�ṩ�Ĳ�ѯ�������󶨵���Ϣ�Ĺ���
*
* CAsyncSocketEx -- ����MFC::CAsyncSocket���첽Socket�࣬����ͨ���� CAsyncSocketExLayer �̳����ಢAddLayer(xxx)
*   ��֧�ִ���(CAsyncProxySocketLayer) �� SSL(CAsyncSslSocketLayer) ��, Layerͨ������ṹ���棬��֧�ֶ����
*   http://www.codeproject.com/internet/casyncsocketex.asp
*   ע�⣺1.Ŀǰ����ʵ����ֻ�� SOCKS5/HTTP11 ֧�� �û���/���� ?
*         2.��������ʱ closesocket ������Bug? MFC�еİ汾�� KillSocket ��̬����
*         3.����ԭʼʵ���ÿһ����CAsyncSocketEx���̶߳������һ��CAsyncSocketExHelperWindow�����Թ��� 1~N ��CAsyncSocketEx��
*           ͨ���侲̬�� m_spAsyncSocketExThreadDataList �����������(��������Ϊ�����ܣ���������ϸ���ϵĹ����Ż�?--���ݴ��ݱ��߳�ͬ������ʱ�����)
*         4.pProxyUser ���ַ���������ͷ��������⣬Ӧ���� delete []
*
* AtlUtil.h ���в��ٸ�����
*************************************************************************************************************************/


/*************************************************************************************************************************
* ͨ�����Ҫ���ǵģ�����ĳ�ʼ���ͷֲ�,��������,������,������,�¼�ѭ���̳�,�ݴ��
* ����Ͽ��������Ӻ���������Э��ʱ����Ҫ�漰���ӳ�(latency)����������(scalability)���ɿ���(reliability)
*   ������Э�� -- ������Ϣ��ÿ����Ϣ����Ѱַ�ͷ��ͣ����õ���UDP��IP�������ڶ�ý��ֱ��������һ���̶����ݶ�ʧ����Ҫ��ʵʱ�Եĳ���
*   ��������Э�� -- �ṩ�ɿ������򡢲��ظ��ķ��ͷ���Ϊ��֤�ɿ��Բ����˶Դ��͵�ÿ�������ֽڱ�š�У��ͼ��㡢����ȷ��(ACK)��
*     ��ʱ�ش�����������(ͨ�����������ɽ��ܷ����Ʒ��ͷ����͵�������) ��һϵ�д�ʩ�������ڵ����ʼ��Ȳ��������ݶ�ʧ�ĳ��ϡ�
*     ��Ҫ�����������ѡ��
*       ���ݳ�֡����--������Ϣ(�� TP4��XTP)�������ֽ���(�� TCP)����������Ϣ�߽�
*       ���Ӷ�·���ò���--��·����(����̹߳���TCP���ӣ����ڱ��)���Ƕ�·����(ÿ���߳�ʹ�ò�ͬ�����ӣ�ͬ������С���������Բ���)
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
*
* ����(Proxy) -- ��Ϊ SOCKS4/5, HTTP/1.1 ��
*************************************************************************************************************************/

/*************************************************************************************************************************
* TCPճ��(Stick Package)
*   ���ͷ����͵����ɰ����ݵ����շ�����ʱճ��һ�������ջ������к�һ�����ݵ�ͷ������ǰһ�����ݵ�β���������ճ��һ������ݰ���������
*   �����������Ϊ���ṹ�����ݣ���Ҫ���ְ�����(������ļ�������������������Ͳ��طְ�)
*   ������˫������������, ������Ҫ�����Ӻ�һ��ʱ���ڷ��Ͳ�ͬ�����ݽṹ, ����Ҫ����ճ��������
*       TCP��Ϊ������ԶԶ���ϵĵ�Ŀ�ĵأ����շ����������UDP��Ϊ��Ϣ�����ݱ���һ��������������շ����������ճ�����⡣
*   
* ����ԭ�� ע�⣺������ʱ�����ܳ���ճ�������� Http Э�� ��)
*   ���ͷ���TCPΪ��ߴ���Ч�ʣ����ռ����㹻�����ݺ�ŷ���(Nagle�㷨[���ӳ�Ӧ��?] ��)
*   ���շ������շ��û����̲���ʱ��ϵͳ���ջ������������ݣ���һ�����ݵ���ʱ�ͽӵ�ǰһ������֮��
*   
* �ϺõĶԲߣ�
*   ���շ�����һԤ�����߳���Խ��յ������ݰ�����Ԥ����,��ճ���İ��ֿ�
*************************************************************************************************************************/

/*************************************************************************************************************************
* ARP(Address Resolution Protocol)--��ַת��Э�飬���ڶ�̬�����IP��ַ�������ַ��ת����
* BOOTP(BooTstrap Protocol)--��ѡ��ȫ����Э�飬ʹ��UDP��Ϣ���ṩһ���и�����Ϣ�����̹���վ��ͨ���ļ��������ϵ��ڴ�Ӱ����������
* Cookie(Cookies) -- ��վΪ�˱���û���ݡ�����session���ٶ��������û������ն����÷ֺ�(";")�ֿ��ļ�ֵ�����ݣ�ͨ���������ܣ���
*   ����Ϊ RFC2109���ѷ������� RFC2965������͵�Ӧ�����Զ���¼�����ﳵ��
*   ������������ -> ���͸�UserAgent(�����) -> �����������ı� -> �´η���ʱ���� -> ��������֤
*   �������� -- Cookie����ʱָ����Expire����������Cookie�ͻᱻ�������λΪ��������������Ϊ"0"��ֵ��ʾ�ر�ҳ��ʱ���������
*   ��ȫ���⣺1.ʶ�𲻾�ȷ����ͬһ̨�������ʹ��ͬһ������Ķ��û�Ⱥ��cookie�����������ǵ���ݣ�����ʹ�ò��õ��û�����¼OS��
*             2.��ҳ�����ͼƬ(͸����ֻ��һ������-�Ա�����)�������з��ʹ���ҳ��ļ����д��cookie��������վ���������ʼ�
* DHT(Distributed Hash Table ��ɢʽ�Ӵձ�) -- ����ʹP2P������ȫ��ʹ�÷����������͵���eMule��ʹ�õ� Kad��
* ICMP(Internet Control Message Protocol)--����������ϢЭ�飬��Ҫ������������·��������IP���ݱ��ش����п��ܳ��ֵĲ����������
* IP(Internet Protocol)--����Э�飬�涨�����ݴ���Ļ�����Ԫ(���ķ���)�Լ��������������ʴ���ʱ��ȷ�и�ʽ�淶��
* LANA(LAN Adapter)Number -- ��Ӧ������������Э���Ψһ���
* MSS() -- ����Ķγ��ȣ���ʾTCP������һ�˵��������ݵĳ��ȡ���������ʱ��
* MTU(Maximum Transmission Unit) -- ����䵥Ԫ 
* NetBios Name -- ΢�������еĻ��������õı���NetBIOS���֣���������ʱ���Ὣ����ע�ᵽ���ص�WINS��
*   ͨ�� nbtstat ��������г���Ϣ��
* OSI -- ����ϵͳ������Ӧ�ò㡢��ʾ�㡢�Ự�㡢����㡢����㡢������·�㡢����㣻
* OOB(Out Of Band) -- ��������(��������)��ͨ���������߼��ŵ������պʹ���������Ҫʹ�á�
* TCP(Transmission Control Protocol) -- �������Э�飬�������ӣ��ṩ˫���������ظ������޼�¼�߽������������
* RARP(Reverse Address Resolution Protocol)--���ڶ�̬��������ַ��IP��ַ��ת����
* RTP(Real-time Transport Protocol) -- ʵʱ����Э�飬Ŀǰ���ʵʱ��ý�崫��ı�׼Э��͹ؼ�������ͨ��ʹ��UDP���д��䣬
*   ��Դʵ���� LibRTP, JRtpLib �ȣ�ÿһ��RTP���ݱ�����ͷ��(Header)�͸���(Payload)��������ɡ�
* RTCP(Real-time Transport Control Protocol) -- ����ʵʱ��� RTP ���ݴ���ͷ���������
* SCTP(Stream Control Transmission Protocol) -- �����ƴ���Э��, 2000���¶�����������ӵĴ����Э�飬��TCP��ȱ�ݽ�����һЩ���ơ�
*     �������˵�֮���ṩ�ȶ�����������ݴ��ݷ��񣨷ǳ������� TCP����������Ϣ��, ���Ա���������Ϣ�߽�(�������ճ������)
*     ����Ǳ����������IP�ϴ���绰�������ӿ����Ƕ��������ӵ�(����˫�������������ַ������ǰ����ʧ�ܣ����л�����һ����ַ)��
*     
* TTL -- ����ʱ�䣬��ʾһ�����ݰ��ڶ���֮ǰ�����������ϴ��ڶ೤ʱ�䣬ֵΪ0ʱ����������
* UDP(User Datagram Protocol) -- �û����ݱ�Э�飬�����ӣ�֧��˫�������������������֤�ɿ����������ظ�
* WINS -- Windows����������������,ά������ע�������NetBIOS���ֵ�һ���б�
* WinSock -- �������̽ӿڣ�������Э�飬��Э���޹ء��������һ�־���Э�飨��IP��TCP��IPX��lrDA�ȣ������׽��֡�
*  ��������Ϣ����������Ϣ�߽�--ÿ�ζ�ȡ����һ����Ϣ����������Ϸ�Ŀ��ư����� ���������������������ݴ��䣬�ᾡ���ض�ȡ��Ч���ݣ�
*  ���������ӡ��͡������ӡ�
*   �������ӵ�Э��ͬʱҲ����ʽЭ�飬������Э�鼸�����ǻ�����Ϣ�ġ�
* XMPP(The Extensible Messaging and Presence Protocol, ����չͨѶ�ͱ�ʾЭ��) -- ͨ�������ڷ�����ʵʱͨѶ(�� QQ?)
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
* TODO: GetAdaptersAddresses ��Flags���� unicast/anycast/multicast �ȶ��ֶ���,�÷�����IP_ADAPTER_ADDRESSES�����ֺܶ�汾
* 
* �㲥�Ͷಥ����ʹ�� UDP �׽��ֲ���ʵ��
*   
* �ಥ/�鲥(multicast) -- һ(�ಥԴ)�Զ�(����ಥ���е�����)������Ӧ���Ƕ���(����Ƶ)���飬
*   �ಥ��ַ��
*     IPV4 -- D��(224.0.0.0 ~ 239.255.255.255)�������Ϊ���֣��ֲ����Ӷಥ��ַ��Ԥ���ಥ��ַ������Ȩ�޶ಥ��ַ
*       NTP(����ʱ��Э����) -- 224.0.1.1
*     IPV6 -- ǰ���� ff12:: ?
*   ��������
*   1.�����ಥ��Socket
*     WinSock1.1: socket(AF_INET[6], SOCK_DGRAM, IPPROTO_UDP);
*     WinSock2.2: WSASocket(AF_INET[6], SOCK_DGRAM, IPPROTO_UDP, NULL, 0, 
*        WSA_FLAG_MULTIPOINT_C_LEAF | WSA_FLAG_MULTIPOINT_D_LEAF); //WSA_FLAG_OVERLAPPED
*        �ڿ��Ʋ�������ݲ��涼��"�޸���"��ֻ����Ҷ�ڵ㣬�����������һ���ಥ�飻��һ��Ҷ�ڵ㷢�͵����ݻᴫ�͵�ÿһ��Ҷ�ڵ㣨�������Լ���
*   2.bind -- �������õ��׽����뱾�ص�ַ�ͱ��ض˿ڰ�����
*   3.ͨ�����ص�һ������ӿڼ���ָ���Ķಥ��
*     ����1:setsockopt -- ������һ��socket�ϼ������ಥ��
*       IPV4:ip_mreq   mreqv4;
*            mreqv4.imr_multiaddr.s_addr = ((SOCKADDR_IN *)group->ai_addr)->sin_addr.s_addr;
*		     mreqv4.imr_interface.s_addr = ((SOCKADDR_IN *)iface->ai_addr)->sin_addr.s_addr;
*            setsockopt(, IPPROTO_IP, IP_ADD_MEMBERSHIP, mreqv4); 
*       IPV6:ipv6_mreq mreqv6;
*			 mreqv6.ipv6mr_multiaddr = ((SOCKADDR_IN6 *)group->ai_addr)->sin6_addr;
*            mreqv6.ipv6mr_interface = ((SOCKADDR_IN6 *)iface->ai_addr)->sin6_scope_id;
*            setsockopt(, IPPROTO_IPV6, IPV6_ADD_MEMBERSHIP, mreqv6); 
*     ����2: WSAJoinLeaf -- ֻ�ܼ���һ���ಥ��(��˿���ֱ�� connect ?)
*            SOCKET rs = WSAJoinLeaf(xxx); �᷵��һ��socket(��ʲô��?)
*   4.���÷������ݵ�����ӿ� -- �� 4 �еı�������ӿ� ��ʲô����
*       IPV4: optval = (char *) &((SOCKADDR_IN *)iface->ai_addr)->sin_addr.s_addr;
*             optlen = sizeof(((SOCKADDR_IN *)iface->ai_addr)->sin_addr.s_addr);
*             setsockopt(, IPPROTO_IP, IP_MULTICAST_IF, optval, optlen);
*       IPV6: optval = (char *) &((SOCKADDR_IN6 *)iface->ai_addr)->sin6_scope_id;
*             optlen   = sizeof(((SOCKADDR_IN6 *)iface->ai_addr)->sin6_scope_id);
*             setsockopt(, IPPROTO_IPV6, IPV6_MULTICAST_IF, optval, optlen);
*   5.������ѡ�� -- setsockopt
*     IP_MULTICAST_LOOP|IPV6_MULTICAST_LOOP  -- ������ֹ�ಥͨ��ʱ���ͳ�ȥ��ͨ�������Ƿ�Ҳ�ܹ���ͬһ���׽����ϱ����գ����ಥ���أ�
*     IP_MULTICAST_TTL|IPV6_MULTICAST_HOPS -- ���öಥ�����ķ�Χ(TTL��ȱʡֵ��1 ?)
*   6.connect[��ѡ] -- ���ӵ��ಥ������������� 2.2 �������ӣ�Ȼ�����ͨ�� send/recv ���������ݣ�
*   7.sendto/recvfrom(1.1) �� send/recv(2.2) -- ��ಥ���ַ���ͻ��������
*     ע�⣺IPV6��sendtoʱ���ಥ���ַ��sin6_scope_id������0. ((SOCKADDR_IN6 *)resmulti->ai_addr)->sin6_scope_id = 0;
*   8.�˳��ಥ�飺setsockopt(, IP_DROP_MEMBERSHIP
*    
* �㲥(broadcast) -- ���͵��������еĽڵ㣬����㲥��ֱ���͵㲥������Ҫ������ʽ
*   �����㲥 -- �㲥�ÿ����������Ǽ�λ�ģ����ڶ���CIDR���·�ɣ�������ȫ1���ǹ㲥��ַ����Ϊ��·�㲥�Ͷ�·�㲥��
*    �������̣�
*    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, 1); -- ����������������㲥
*    sendto(xxx.255)
* 
* �ಥ·��Э�� -- �� PIM(Э������ಥ)��DVMRP��
*   ·�����ɽ�����ӶಥԴ�ڵ㵽����Ŀ�Ľڵ�Ķಥ·�ɱ��Ӷ�ʵ����������ת���ಥ���ݰ�
* Internet�����Э�� -- IGMP
*************************************************************************************************************************/


/*************************************************************************************************************************
* Socketģʽ -- �����ڶ�һ���׽��ֵ���ʱ����ЩWinSock��������Ϊ
*   ����ģʽ(ȱʡ)����I/O�������ǰ��ִ�в�����Winsock����(��send��recv)��һֱ����ȥ�������������س���
*   ������ģʽ��Winsock�������۳ɹ���(����0)����ʧ�ܶ����������أ�ʧ��ʱ����SOCKET_ERROR��������Ϊ WSAEWOULDBLOCK��
      ͨ����ҪƵ���ĵ����ж�(Ч�ʵͣ�Ӧ��ʹ��Socketģ�ͽ����첽)��
*     ioctlsocket(,FIONBIO,&1) -- ע�⣺ipmsg�Ķ˿ھ�ʹ���˸�ģʽ
*   IO���ã��� select/poll/WaitForMultiXXX ��ϵͳ����������������һ�εȴ�����ļ����˿ڵĶ�����(��/д/�쳣)   
*
* Socketģ�� -- ������һ��Ӧ�ó�����ζ��׽����Ͻ��е�I/O���й�������
*   select(ѡ��) -- ����select�����ж��׽������Ƿ�������ݣ������ܷ���һ���׽���д�����ݡ�
*     �����֣��ɶ��ԡ���д�ԡ����⡣�������������������׽ӿڵ���Ŀ��Ȼ��ͨ���ٴ��ж�socket�Ƿ��Ǽ��ϵ�һ������ȷ���Ƿ�ɶ�д��
*     ȱ�㣺1.���ܶ�̬�ĵ���(�����ӡ�ɾ��)Socket��
*           2.����̲���һ���źŲ����źŴ�����򷵻أ��ȴ�һ��ᱻ�жϣ������źŴ������ָ�� SA_RESTART ��ϵͳ֧�֣�
*           3.��Windows��ȱʡ���64��(FD_SETSIZE)��ͨ���ض�����������Linux���Ƿ�������?
*     ͨ�� FD_ZERO�� FD_SET �Ⱥ��socket������ü������¼��� Ȼ��select(socket+1,xxx) ���غ�ͨ�� FD_ISSET �Ⱥ��ж��Ƿ����¼�
*   WSAAsyncSelect(�첽ѡ��) -- ������Windows��ϢΪ�����������¼�֪ͨ(�������¼����˺�����Ϣ���д���)��MFC��CAsyncSocket�ķ�ʽ��ģʽ�Զ���Ϊ��������
*     ��IPMsg��tcp���ڽ������ӣ�UDP���ڶ�ȡ���ݣ�
*       ::WSAAsyncSelect(udp_sd, hWnd, WM_UDPEVENT, FD_READ)
*       ::WSAAsyncSelect(tcp_sd, hWnd, WM_TCPEVENT, FD_ACCEPT|FD_CLOSE)
*       Ȼ������Ϣѭ���д����Ӧ��Ϣʱ��ʹ�ú� WSAGETSELECTERROR �� WSAGETSELECTEVENT ��lParam��ȡ ErrCode �� Event(�� FD_READ)��
*         ���ж�Ӧ�Ĵ���(�� OnReceive )���ڽ���ǰ��Ҫ�ٴ��� 0��Ϊ lEvent �������� WSAAsyncSelect ��ȡ���¼�֪ͨ
*   WSAEventSelect(�¼�ѡ��) -- ���ÿһ���׽��֣�ʹ��WSACreateEvent����һ���¼�(Ĭ�����ֶ�����)�������й�����
*     ���� WSAWaitForMultipleEvents ���еȴ������֧��64�������� WSAEnumNetworkEvents ��ȡ�������¼�
*   Overlapped I/O(�ص�ʽI/O) -- ���нϺõ����ܣ�ʹ�� WSA_FLAG_OVERLAPPED ����Socket((Ĭ������)��
*     �������Socket�����м��� WSAOVERLAPPED �ṹ��ʹ��"�������"��
*     ���� WSAResetEvent �����¼�(���¼����ֶ����õ�)��ͨ�� WSAGetOverlappedResult ��ȡ
*     ִ�н��(�����Ͳ�ʹ��Overlapped����ReadFile�Ⱥ���ʱ���صĽ��һ��)��
*     ע�⣺����Overlapped�󣬺���ͨ������ʧ�ܣ�������ΪWSA_IO_PENDING�����������������Ѿ���Cache�У���ֱ�ӷ��سɹ�
*
*   IO Completion Port(��ɶ˿�) -- �� Overlapped I/O Эͬ������������һ���������ڶ�ľ����������I/O�ĳ���(��Web������)���л��������ܡ�
*     û��64��HANDLE�����ƣ�ʹ��һ���߳�(ͨ��������Ϊ CPU����*2+2 )����һ��Events�����ʣ��Զ�֧�� scalable��
*     ����ϵͳ���Ѿ���ɵ��ص�I/O�����֪ͨ������ɶ˿ڵ�֪ͨ����(һ���߳̽�һ��������ʱ��������)���ȴ���һ���߳�Ϊ����ʵ�ʷ���
*     �̳߳ص������߳� = Ŀǰ����ִ�е� + �������� + ����ɶ˿��ϵȴ��ġ�
*     ΢�����ӣ�web\Wininet\Async
*     ʹ�����̣�
*       1.����һ��I/O completion port -- hPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL...)������һ��û�к��κ��ļ�Handle�й�ϵ��port
*       2.�������ļ�handle�������� -- CreateIoCompletionPort(hFile,hPort...)��Ϊÿһ�������ӵ��ļ�Handle���й���
*       3.����һ���߳� -- for(CPU*2+2){ _beginthreadex }
*       4.��ÿһ���߳���Completion Port�ϵȴ������غ����������д���
*         GetQueuedCompletionStatus(,INFINITE)����ͨ�� CONTAINING_RECORD ��ȡ������ָ����չ�ṹ��ָ��
*       5.��ʼ�����Ǹ��ļ�handle����һЩoverlapped I/O���� -- �� ReadFile��WriteFile��DeviceIoControl�ȣ�
*       6.�����߳������Ҫ����(��֪ͨ�������)����Ҫͨ�� PostQueuedCompletionStatus ����ɶ˿ڷ����¼�֪ͨ���Ӷ����Ѹ����߳�,
*         Ȼ�� WaitForSingleObject(hPort, ��ʱʱ��)
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
*   SOCKADDR_STORAGE -- ���� sockaddr ?ӵ�и������Ϣ
*
* socket API���¿��Է�Ϊ���ࣺ
*   A.���ػ�����������Ϣͨ��������OS�ں˻�ϵͳ����
*     1.socket -- ����һ��socket����������ظ�������
*     2.bind -- ��һ��socket�����һ�����ػ�Զ�̵�ַ��������
*     3.getsockname -- ����socket�󶨵ı��ص�ַ
*     4.getpeername -- ����socket�󶨵�Զ�̵�ַ
*     5.close/closesocket -- �ͷ�socket�����ʹ��ɸ���
*   B.���ӵĽ�������ֹ
*     1.connect -- ������һ��socket����Ͻ������ӣ���������ó�ʱ������ioctlsocket(socket, FIONBIO, &1)��socket����Ϊ�Ƕ����ģ�
*         ��connectʱ���������أ�Ȼ������select�����ȴ� readfd �����ó�ʱֵ(TODO:�� writefd? )
*         fd_set rdevents; FD_ZERO(&rdevents); FD_SET(socket, &rdevents); struct timeval tv(nTimeOutSec, 0); 
*         nResult = select (socket+1, &rdevents, NULL, NULL);
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
*   D.ѡ�����  -- ��ô���д������ƣ�
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
*     2.ioctlsocket/WSAIoctl -- ����socket��IOģʽ��
*         FIONBIO -- �����Ƿ��������ģʽ, &1 �������ģʽ��
*         FIONREAD -- ȷ�ϵȴ���ȡ�����ݳ���
*         SIOCATMARK -- ȷ���Ƿ����е�OOB���ݶ��Ѿ���ȡ���
*   E.�����ַ -- �ڿɶ��Ե�����(������)�͵ͼ������ַ(��IP)֮�����ת��
*     1.getaddrinfo(��Ҫfreeaddrinfo�ͷ�)/getnameinfo -- HOST�� �� ��ַ(IPV4/IPV6) ֮�� Э���޹ص�ת��
*         struct addrinfo hints = { 0 }; hints.ai_family = AF_UNSPEC; hints.ai_socktype=SOCK_DGRAM; hints.ai_flags=AI_PASSIVE;
*         getaddrinfo("www.baidu.com", "http", &hints, &res);  // res ��ŷ��� addrinfo �ṹ�����ָ��,Ȼ���ͨ�� res->ai_family/ai_socktype �ȴ���socket
*       gethostbyname/gethostbyaddr -- ������������ IPV4 ��ַ֮��������ַӳ��, buffer�� ��󳤶�Ϊ MAXGETHOSTSTRUCT
*       WSAAsyncGetHostByName/WSAAsyncGetHostByAddr  -- �첽��ȡ ����/��ַ ��Ϣ�������߳�����
*         sockAddr.sin_addr.s_addr = ((LPIN_ADDR)((LPHOSTENT)pAsyncGetHostByNameBuffer)->h_addr)->s_addr;
*     2.getipnodebyname/getipnodebyaddr -- ������������ IPV6 ��ַ֮��������ַӳ��
*     3.getservbyname -- ͨ�����пɶ��Ե����Ʊ�ʶ����
*       getpeername -- 
*     4.inet_aton/inet_ntoa -- �ڵ��ʮ��������(��192.168.0.1)���Ӧ��32λ�����ֽ��������ֵ��ת����ֻ������IPv4
*       ע�� inet_ntoa ���ص��ַ����洢�ھ�̬�ڴ��У����̰߳�ȫ��
*     5.inet_pton/inet_ntop <== presentaion(�ַ������)<->numeric(�׽ӿڵ�ַ�ṹ�еĶ�����ֵ)��������IPv4��IPv6 -- Vista���֧��?
*         InetNtop(AF_INET,&cliaddr.sin_addr, buff, sizeof(buff));
*         InetPton/inet_pton(AF_INET,TEXT("192.168.0.1"), &addr.sin_addr, ...); 
*     6.(�Ƽ�)WSAAddressToString/WSAStringToAddress -- ���ַ��Ͷ����Ʒ�ʽ֮��ת����ַ��֧��Xp
* ע�⣺
*   1.Socket APIʹ��һ�ֺܻ����ĵ�ַ�ṹ(sockaddr),���ݵ�ַ�ز�ͬ���ṹ��Ա��ռλҲ��ͬ��
*     ��IPV4�� sockaddr_in,ʹ��ǰһ��Ҫȫ�����㣬�������׳��ִ���
*   2.ע�������ֽ���������ֽ��������
*     �����ֽ��򣺶˿�(addr.sin_port)��IP��ַ
*   3.Ĭ������£������TCP/IPʵ�ֻ�ʹ��Nagle�㷨�����ڷ��Ͷ��л����������η��͵����ݱ���ͨ���������
*     ��������ӵ���������������ӳٲ���������������ͨ�� TCP_NODELAY ѡ��رգ���ֹ���ݺϲ���
*     �����ھ���������С���ݱ�(���� IPMsg )
*       :setsockopt(info->sd, SOL_SOCKET, TCP_NODELAY, (char *)&flg, sizeof(flg));
*   4.�ӳ�Ӧ��(�����Ӵ�ACK) -- ͨ��TCP�ڽ��յ�����ʱ������������ACK��ʵ�����Ƴٷ��ͣ��Ա㽫ACK����Ҫ�ظ÷����͵�����һ���͡�
*     �������ʵ�ֲ��õ�ʱ��Ϊ200ms
*************************************************************************************************************************/

/*************************************************************************************************************************
* TransmitFile -- ���ں��и����ܵĴ����ļ�����
* TransmitPackets -- �����е�����socket�ϴ����ڴ�飬�ƺ������з�����ȡ����
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
*   ͨ��������TCP��TLS��SSL(��HTTPS��Ĭ�϶˿�443)Э���֮�ϣ�����Ӧ�ò�Э�飬��Ϊ POST(RFC1867)��GET(RFC???) ��
*   C#��ʹ�� HttpWebRequest request = (HttpWebRequest)WebRequest.Create("http://www.baidu.com");
*            request.AddRange //����ָ����Դ�ķ�Χ
*            HttpWebResponse response = (HttpWebResponse)request.GetResponse();
*            Stream stream = response.GetResponseStream();
*            StreamReader sr = new StreamReader(stream);
*            string content = sr.ReadToEnd();
*
*  Э������������ɣ�Э��ͷ�����������Լ�Э��β�� ������ASCII��ʽ��
*    Э�����ͣ�
*    ʹ�ö�����ͬʱ��?����������(HTTP POST-MultiPartFormData),���������д�ʼ�ʱ�Ķ��������
*      ��ʱ�������� HTTP_REQUEST::Flags ���� HTTP_REQUEST_FLAG_MORE_ENTITY_BODY_EXISTS
*      HTML ��ͨ���� <form action="http://xxx" method="post" enctype="multipart/form-data">
*                       <input type="file" name="uploadfile1"/>
*                       <input type="file" name="uploadfile2"/>
*                       <input type="submit" value="uploadfile"/>
*                    </form>
*    1. HttpAddRequestHeaders����
*       ʹ�� "Content-Type: multipart/form-data; boundary={boundary}" ����ʹ�ö���֣���ָ���ָ���в�ͬ�������ݵķ���
*       (���Զ�����漴��������һ��ʹ�õ��� --MULTI-PARTS-FORM-DATA-BOUNDARY )
*    2. Post�������֣�����������ݣ��� Content-Disposition  �Ϳ�ѡ�� [Content-Type]�� ��(������Ϣ��������ԭʼ��Ϣ)��
*       ÿһ������ {boundary} CRLF �� "Content-Disposition: form-data;" �ֿ�, ע��������ļ�β��Ҳ��Ҫ CRLF��(���������ݿ�ʼʱ��Ҫ���� CRLF ?)
*    3. ʹ�� {boundary}-- ��ʾ����(ע���������� "--" )
*    
*
*  ��������( HTTP/1.1 Ĭ��ʹ�ô���ˮ�ߵĳ־����� )
*    �ǳ־�����(NonPersistent Connection) -- HTTP/1.0��ÿһ�����ݴ��Ͷ���Ҫ���´�/�ر�TCP����
*    �־�����(Persistent Connection,������) -- �������ڷ�����Ӧ����TCP���ӳ����򿪣�ͬһ�Կͻ�/������֮���
*      �����������Ӧ��ͨ��������ӷ��ͣ�����������һ��ʱ���رա��ַ�Ϊ��
*      a.������ˮ�� -- �յ�ǰһ���������Ӧ��ŷ����µ�����
*      b.����ˮ�� �����汾
*
* HTML VERBS(�ַ���) -- �������ͣ���Э��ͷ���֣�����ָ��HTTP�汾��Ϣ
*   CONNECT(1.1) -- Ԥ�����ܹ������Ӹ�Ϊ�ܵ���ʽ�Ĵ��������
*   DELETE(1.1) -- ����ɾ��ָ������Դ
*   GET(1.0) -- �����ȡ�ض�����Դ��������������ύ����(INTERNET_REQFLAG_NO_HEADERS)��������һ��Webҳ�棬
*               ͨ��ֻ����URL�е� ��ҳ��ַ���֣��� GET /index.html HTTP/1.1\r\n
*               ��ѯ�ַ���������/ֵ�ԣ����� GET ����� URL �з��͵�(�ᱻ�������г�������)���� /test/demo_form.asp?name1=value1&name2=value2
*   HEAD(1.0) -- ������������ȡ��GET������һ�µ���Ӧ��ֻ������Ӧ�岻�ᱻ���ء�
*                �÷������ڲ��ش���������Ӧ���ݵ�����£��ͻ�ȡ��������Ӧ��Ϣͷ�е�Ԫ��Ϣ��
*                ͨ�����ڸ�������(�����������ȡ��Ϣ����ȫ��֤ʱ������֤��Ϣ�������ļ�ǰ�ж���Դ�Ƿ���Ч ��)
*   LINK(1.1)
*   OPTIONS(1.1) -- ���ط���������ض���Դ��֧�ֵ�HTTP���󷽷�
*   POST(1.0) -- ��ָ����Դ�ύ���ݣ�������д������ύ�����ϴ��ļ������������(�� ��ѯ�ļ�ֵ�ԡ��ϴ����ļ�����)�������������HTTP��Ϣ�����У�
*                �����󲻻ᱻ���棬���������������ʷ��¼�У����ܱ��ղ�Ϊ��ǩ�������ݳ���û��Ҫ��
*   PUT(1.1) -- ��ָ����Դλ���ϴ����������ݣ�������洢һ��Webҳ�棬 �����POST���ϴ��ļ���
*   TRACE(1.1) -- ���Է������յ�������
*   UNLINK(1.1)
*
* �ж����� HINTERNET �Ƿ���Ҫ��֤��Ϣ
*   1.HttpQueryInfo(hRequest, HTTP_QUERY_FLAG_NUMBER | HTTP_QUERY_STATUS_CODE, &dwStatus, &cbStatus, NULL);  //��ȡ״̬��
*   2.switch(dwStatus) {   //��ѯ״̬��
*       case HTTP_STATUS_DENIED(401):			// �ܾ����ʣ��ٲ�ѯ dwFlags = HTTP_QUERY_WWW_AUTHENTICATE
*       case HTTP_STATUS_PROXY_AUTH_REQ(407):	// ������Ҫ�����Ϣ���ٲ�ѯ dwFlags = HTTP_QUERY_PROXY_AUTHENTICATE
*     }
*     do  { bRet = HttpQueryInfo( hRequest, dwFlags, szScheme, &cbScheme, &dwIndex ); } while(bRet);  //�����ж����֤�������Ҫѭ����

* ʹ��HTTP��ȡ���ݵĲ���: 
*   1.�ͻ��˽�������(TCP)
*   2.�ͻ��˷�������(HTTP/HTTPS)
*     ���ݰ�����ͳһ��Դ��ʶ����URL����Э��汾�ţ������MIME��Ϣ�����������η����ͻ�����Ϣ�Ϳ��ܵ�����
*   3.������������Ӧ��Ϣ����ʽΪһ��״̬�У�������Ϣ��Э��汾�š�һ���ɹ������Ĵ��룬�����MIME��Ϣ������������Ϣ��ʵ����Ϣ�Ϳ��ܵ�����
* 
* ͷ��(Headers)����Ҫ����ǰ��ԭ��Ϣ(Meta Information)��������������ṩ��������������Ϣ����ʽΪ "����:��ֵ"
*   ÿ������������"CRLF"�ֿ���������������"CRLF"��ͨ�� HttpAddRequestHeaders ����?
*   [M]Accept: text/plain, x/x(ע�⣺�˴�������*��)����ʾClient�ܹ����յ����������
*   [O]Accept-Charset: GBK, utf-8
*   [O]Accept-Encoding: gzip, deflate
*   [O]Accept-Language: zh-cn
*   [O]Accept-Ranges: bytes
*   [O]Cache-Control: ָ���������Ӧ��ѭ�Ļ�����ƣ��� no-cache, max-age, private ��
*   [M]Content-Type: ����򷵻ص��������ͣ���Ӧ�� form �е� enctype
*      �����б� -- rfc1341( http://www.ietf.org/rfc/rfc1341.txt )
*        application/x-www-form-urlencoded <== 
*        application/octet-stream <== �����ϴ��ݶ���������ʱ
*        multipart/form-data; boundary=--{boundary} <== �����ݴ����ļ�����
*        video/avi   <== AVI 
*        image/jpeg  <== JPG
*        image/x-png <== PNG
*        image/bmp   <== BMP
*        image/tiff  <== TIF
*        image/gif   <== GIF
*        text/plain  <== TXT
*        text/xml    <== XML
*        text/html;charset=gb2312  <== HTML
*        
*   []Content-Disposition: -- ����HTTP��׼�����㷺ʵ�֡�
*     �ɴ���������ļ�(ǰ������� --{boundary}), ���ӱ���ʱ�Ľ����ļ�����
      Э��ͷʱ��form-data; name=\"attach_file\"; filename=\"xxxx\"    -- ���е� "attach_file" ��HTMLԪ����(�ͷ������й�)
      Э��βʱ��form-data; name=\"icount\" + CRLF + CRLF + _T("1") + CRLF +   -- ���е� icount Ҳ�Ƿ�������ص�?
      \"submitted\"
*   [M]Content-Length: ���ݳ��ȣ����� ͷ + ���� + β�������ֱ�ӷ����ļ���ַ�Ļ������ص����ļ���С��
*   [O]Content-Transfer-Encoding: binary | Chunked(��ʾ��������ݳ��Ȳ�ȷ��)
*   []Connection: Keep-Alive(�־�����), Close(�ǳ־�����)
*   []Cookie : �ֺ�";"�ֿ��Ķ�� "������=ֵ" �ļ�ֵ��
*   []Date: ��ʾ��Ϣ���͵�ʱ�䣬��RFC822���壬�������׼ʱ���� Sat, 26 May 2012 00:42:19 GMT
*   [O]Host: www.google.com[:80]������Ŀ�����վ���� GET ���������ͺ�ĵ�ַ����������Ŀ���ַ
*   []Referer: http://Դ��Դ��ַ�� �����������������ɻ���������������½���Ż���cache��
*   []User-Agent: Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0) ���Զ�������֣��Ƿ���������û������Ϣ
*                
* GET /�ļ���ַ HTTP/1.1 -- �ֿ�/�ϵ��������������ɹ��᷵�� 206(HTTP_STATUS_PARTIAL_CONTENT) OK
*   []Range: bytes=��ʼλ��-����λ��, ����ʵ���һ�����߶���ӷ�Χ����ͨ�����ŷָ������Χ
*   []Content-Range: bytes ��ʼλ��-����λ��/�ܴ�С, ��Range ����Ӧ
* 
* ��������(request-body)
*   ���пɰ�����������ݣ�
* 
* ������Ϣ: ״̬��(HTTP�汾�� + 3λ״̬�� + ״̬����) + ͷ + ��Ӧ��
*   ״̬��( HTTP_STATUS_OK �� )
*     1xx��Ϣ -- �����ѱ����������գ���������
*       100 Continue -- �����������յ��������󣬿ͻ���Ӧ�ü����������������
*       101 Switching Protocols -- ������ת��Э�飺����������ӿͻ�������ת��������һ��Э�顣
*     2xx�ɹ� -- �����ѳɹ������������ա���Ⲣ���ܣ��� 200(OK������Ƕ�GET��POST�����Ӧ���ĵ���
*       206(PARTIAL_CONTENT) -- ��ʾ�ɹ���ȡ���������ݣ�����Rangeָ����Χ����
*     3xx�ض��� -- ��Ҫ�����������������һ������ 304(NOT MODIFIED--����Դ���ϴ�����֮��û���κ��޸ģ�ͨ������������Ļ������)
*     4xx������� -- �������﷨������޷���ִ�У��� 401(UNAUTHORIZED),403(FORBIDDEN),404(NOT FOUND)
*     5xx���������� -- �������ڴ���ĳ����ȷ����ʱ���������� 501(Not Implemented--����������ʶ�������δʵ��ָ��������)
*   ���ص�״̬ͷ
*     []Server: Apache/2.2.11 (Unix)
*     []Last-Modified: Wed, 23 May 2012 21:38:23 GMT
*     []ETag: "1d2d6-5c896800-4c0baf45c91c0"
*     []Accept-Ranges: bytes
*     []Content-Encoding: gzip, ��Ӧ��ı��뷽ʽ����gzip��ʾ��������ʹ����gzipѹ�����������أ����Ǳ���client��֧��gzip�Ľ������
*       Vista֮��WinINet����֧�ֽ���( InternetSetOption(INTERNET_OPTION_HTTP_DECODING) )
*     []Content-Length: 1164371968
*     []Content-Range: bytes 388136960-1552508927/1552508928
*     []Expires: ����client���ԵĹ���ʱ�䣬�����ʱ����client�����Բ��÷��������ֱ�Ӵ�client��cache�л�ȡ��
*                ������js/css/image�Ļ�������
*     []Keep-Alive: timeout=5, max=100
*     []Last-Modified: ���һ���޸���Ӧ���ݵ����ں�ʱ��
*     []Server: BWS/1.0 ��Ӧ�ͻ��˵ķ����������Կ�����ʲô���͵�Web����
*
* ��װ����
*   URLDownloadToFile -- ָ��URLֱ�������ļ���ȱ�㣺ͬ�����ã��������ش��ļ���
*
* WinINet API ����(MFC ��װ: CHttpFile) -- ͨ��ֻ�����ڿͻ��˳��򣬷��������򿪷���Ҫ�� WinHTTP(������? �μ� "Porting WinINet Applications to WinHTTP" )
*   һ�������� HINTERNET(��ͨ�� GetHInternetHandleType ��������):
*     1.InternetOpen/WinHttpOpen ��ʼ���� WinINet ��������,�õ�Session���(��Ӧ CInternetSession ), 
*       ���Flags�� INTERNET_FLAG_ASYNC ��������첽���ӣ��������ӡ����ݽ�������Ҫ���첽��(��Ҫͨ�� InternetSetStatusCallback ���ûص� �� ͨ����ɶ˿������� ?)
*         ��Ҫ�� INTERNET_STATUS_REQUEST_COMPLETE �¼���Ӧ�У�����״̬���д���(���� )
*       �첽ʱ��HTTP �� InternetConnect ��ͬ�����أ�FTP�� InternetConnect ���첽����(ERROR_IO_PENDING)
*     2.InternetConnect(ָ��Server+Object+Port)/WinHttpConnect  
*       ���ӵ�ָ�� Server:Port �ϵ�Connect���(��Ӧ CHttpConnection )������ָ���û���������
*     3.HttpOpenRequest/WinHttpOpenRequest �����Ӿ���ϴ򿪵�Request���(��Ӧ CHttpFile ��)��Ҫָ���� POST/GET �ȣ��������ݽ����ڸþ���Ͻ���
*       �� lplpszAcceptTypes ������ָ��LPCTSTR�����ָ�룬������һ��NULLָ�������ָ���˳�����ܵ����ݵ�����(NULL �������κ����͵����ݣ����ַ����ȼ���"text/*")
*   ע�⣺������� InternetOpen + InternetOpenUrl �ķ�ʽ����ֻ��Ҫ�������
* 
*   �����ļ���������
*     HttpOpenRequest => HttpAddRequestHeaders => HttpSendRequestEx => loop InternetWriteFile( Fire OnProgress ) 
*       => HttpEndRequest => loop InternetReadFile(��ȡ��Ӧ)
*
*   ������������
*     HttpQueryInfo(HTTP_QUERY_CONTENT_LENGTH)     ��ȡ��С����̬��ҳ�ɻ�ȡ����С����asp/php�ȶ�̬��ҳ�޷���ȡ
*     HttpQueryInfo(HTTP_QUERY_STATUS_CODE)   ��ȡ��ǰ��״̬��
*     Loop InternetReadFile( Fire OnProgress ) ,���ȡ����С����ȡ��ָ����С�������ȡ�� ����� lpdwNumberOfBytesRead ���� 0
*     InternetQueryDataAvailable �ж��Ƿ������ݣ��������óɹ����ҷ���ֵΪ0 ��ʾ��ȡ��ϣ���Ҫ�� HttpSendRequest ֮����ܵ��ã�
* 
*   ����/״̬
*     InternetGetConnectedState()
*     InternetQueryOption -- 
*     InternetSetOption/WinHttpSetOption -- 
*     InternetGetCookie
*     InternetSetCookie[Ex] -- ����Cookie
*     HttpQueryInfo
*     InternetSetStatusCallback  -- ������������ʱ�����¼��Ļص��ӿ�
*     InternetErrorDlg -- ����������صĴ���Ի�������֤ʧ��(httpauth �����в���û�г���):
*       dwError = ERROR_INTERNET_INCORRECT_PASSWORD; //GetLastError()
*       dwFlags = FLAGS_ERROR_UI_FILTER_FOR_ERRORS | FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS | FLAGS_ERROR_UI_FLAGS_GENERATE_DATA;
*       if(InternetErrorDlg(GetDesktopWindow(), hRequest, dwError, dwFlags, NULL) == ERROR_INTERNET_FORCE_RETRY){ retry; }
*   ���ӿ���
*     InternetOpen -- ��ʼ��WinINet�����⣬�䷵�صľ�����ں�����Connect,ʹ����Ϻ���ҪClose
*     InternetConnect -- ָ��URL����һ������, ��ָ֪���� InternetSetStatusCallback ʹ�õĻص�����(��ƱȽϹ֣�Ϊʲô���� InternetSetStatusCallback ���ṩ?)
*     InternetAttemptConnect
*     InternetCloseHandle(hRequest,hConnect, hOpen)
*     HttpOpenRequest -- ����һ��HTTP���󣬲���������������Զ��������ͳ�ȥ����Ҫ���� HttpSendRequest[Ex] ������
*       HttpsʱdwFlags������Ҫ���� INTERNET_FLAG_SECURE�������Ժ������������ INTERNET_FLAG_IGNORE_CERT_CN_INVALID|INTERNET_FLAG_IGNORE_CERT_DATE_INVALID
*     InternetLockRequestFile/InternetUnlockRequestFile -- ����/������Դ�ļ�
*   ���ݴ���
*     InternetWriteFile -- ��򿪵�Http��������д�����ݣ�ͨ����ѭ���н���
*     InternetReadFile -- ��Http�������ж�ȡ���ݣ�ͨ��ѭ���� *lpdwNumberOfBytesRead Ϊ 0 �� ��������FALSE
*     InternetQueryDataAvailable -- ��ȡ�����ϻ��е�����������������ɹ��ҷ��صĴ�СΪ0����ʾû�������ˡ�
*     InternetSetFilePointer -- ����ָ����ȡ��λ��?
*     HttpAddRequestHeaders(xxx, HTTP_ADDREQ_FLAG_ADD) -- ��HTTP����������������ͷ��ÿ��ͷ��Ҫ��һ�� CRLF ����
*     HttpSendRequest -- ������������ (��һ������? -- ������Ҫ���͵����ݶ�Ҫ��һ��buffer����?)
*     WinHttpSendRequest
*     WinHttpReceiveResponse -- ������Ӧ
*     HttpSendRequestEx -- ͨ�� INTERNET_BUFFERS(ע��Ҫ���� dwStructSize) �ṹ�巢���������ݣ�
*       ͨ�������� POST �����ļ����� dwBufferTotal ����Ϊ Post����Ϣ���� + �ļ���С(�ȼ������� Content-Length )
*       ʹ�õ���������Ҫ�� INTERNET_FLAG_NO_CACHE_WRITE �������
*     HttpEndRequest -- ����HttpSendRequestEx��ʼ����HTTP����
*   Cache����
*     ��Ϊ�������ͣ�
*       Groups -- ?
*       Entries -- ���Բ��� visited:/cookie/""(CONTENT)/NULL(ȫ��) �ȸ���������ʹ������ݻ���
*     FindFirstUrlCacheGroup -> ����(�磺DeleteUrlCacheGroup) -> FindNextUrlCacheGroup -> FindCloseUrlCache
*     FindFirstUrlCacheEntry/FindFirstUrlCacheEntryEx( INTERNET_CACHE_ENTRY_INFO::dwStructSize ϵͳ���غ����õ� *lpdwEntryInfo ���õģ��ɱ�)
*       -> ����(�磺DeleteUrlCacheEntry) -> FindNextUrlCacheEntryEx -> FindCloseUrlCache
*     GetUrlCacheEntryInfo -> 
*
* HTTP����������(winHttp)��Ŀǰ�������汾��1.0(XPSP2/Win2003)��2.0(Vista/Win2008)
*   HttpInitialize(1.0/2.0) -- ��ʼ��������API������ѡ�� ���� �� ������ ��ʼ����
*   ����HTTP������еľ��������URL��ص���Դ(URL �������� FullyQualifie������Э�顢�˿ڡ�����б�ߵȣ��� http://localhost:80/ )���ְ汾��
*     1.0
*       HttpCreateHttpHandle -- ����������У�����ʱ��Ҫ CloseHandle
*       HttpAddUrl -- �����������ע����Ҫ�����URL���ɶ��
*     2.0:
*       HttpCreateServerSession -- ʹ����Ϻ���Ҫ HttpCloseServerSession
*       HttpCreateUrlGroup -- ����UrlGroup��ʹ����Ϻ���Ҫ HttpCloseUrlGroup
*       HttpAddUrlToUrlGroup -- ��UrlGroup��ע����Ҫ�����URL���ɶ��
*       HttpCreateRequestQueue -- ����������У�����ʱ��Ҫ HttpCloseRequestQueue
*       HttpSetUrlGroupProperty(,HttpServerBindingProperty,) -- ��UrlGroup��������а���������������������Ϣ(��Authentication��)
*   ���ݴ���ѭ��
*     HttpReceiveHttpRequest -- ����������л�ȡ���õ�HTTP����(��ѡ��ͬ�����첽)������ HTTP_REQUEST::Verb �Ƚ��д���
*       HttpReceiveRequestEntityBody -- ���չ�����HTTP�����ϵ�ʵ������(��ͻ����ϴ����ļ����ݣ���ͨ��htmlʵ�ⷢ��û�ж�ȡ���ļ����ݣ�)
*       HttpSendHttpResponse -- ������Ӧ������������и�������(���ļ�)��Ҫ���ͣ���Ҫָ�� HTTP_SEND_RESPONSE_FLAG_MORE_DATA
*       HttpSendResponseEntityBody -- ���͹�����HTTP��Ӧ�ϵ�ʵ������(��ͨ���ļ����ָ�����ļ�)
*   HttpRemoveUrl(1.0)/HttpRemoveUrlFromUrlGroup(2.0) -- �����������ֹͣURL�Ĵ���
*   HttpTerminate(1.0/2.0) -- ���HTTP������API����Դ
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
    #define NET_VERIFY_EXCEPT1(x, e1) \
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
        #define NET_VERIFY_EXCEPT1(x, e1) \
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

	//��HTTP��Ӧ������Ԥ����õ�ͷ(�� HttpHeaderContentType �� "text/html" )��Ϣ
	#ifndef ADD_HTTP_RESPONSE_KNOWN_HEADER
	#  define ADD_HTTP_RESPONSE_KNOWN_HEADER(Response, HeaderId, RawValue)\
		{\
			(Response).Headers.KnownHeaders[(HeaderId)].pRawValue = (RawValue);\
			(Response).Headers.KnownHeaders[(HeaderId)].RawValueLength = (USHORT) strlen(RawValue);\
		} 
	#endif //ADD_HTTP_RESPONSE_KNOWN_HEADER

    FTLEXPORT class CFNetErrorInfo : public CFConvertInfoT<CFNetErrorInfo,int>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFNetErrorInfo);
    public:
        FTLINLINE explicit CFNetErrorInfo(int err);
        FTLINLINE virtual LPCTSTR ConvertInfo();
    };

	//IPV4/IPV6 ���ݵĵ�ַ -- SOCKADDR_STORAGE
	class CFSocketAddress : public SOCKET_ADDRESS
	{
	public:
		FTLINLINE explicit CFSocketAddress();
		FTLINLINE explicit CFSocketAddress(const SOCKET_ADDRESS& addr);
		FTLINLINE explicit CFSocketAddress(const SOCKADDR_IN& addrv4);
		FTLINLINE explicit CFSocketAddress(const SOCKADDR_IN6& addrv6);
		FTLINLINE explicit CFSocketAddress(const SOCKADDR_STORAGE addrStorage);
		FTLINLINE explicit CFSocketAddress(LPCTSTR sAddr, USHORT usPort = 0);
		FTLINLINE ~CFSocketAddress();

        FTLINLINE BOOL   SetAddressPort(USHORT usPort);
		FTLINLINE BOOL 	 GetIPv4Address(in_addr& rAddrV4, USHORT& rPort);
		FTLINLINE BOOL	 GetIPV6Address(in6_addr& rAddrV6, USHORT& rPort);
		FTLINLINE LPCTSTR ToString(CFStringFormater& formater);
	protected:
		FTLINLINE VOID	  Init();
	};

	typedef std::map<tstring, tstring> CookieKeyValueMap;
    namespace FNetInfo
    {
		//FTLINLINE LPCTSTR GetSockAddrString(CFStringFormater& formater, SOCKADDR *sa, int len);

		//ͷ�е�CheckSum
		FTLINLINE USHORT CheckSum(USHORT *pBuffer, int size);

		FTLINLINE LPCTSTR GetCookieInfo(CFStringFormater& formater, LPCTSTR lpszUrl, LPCTSTR lpszCookieName);
		FTLINLINE DWORD GetCookieInfoMap(LPCTSTR pszCookies, CookieKeyValueMap& cookieMap);

        //��ȡЭ��ĵ�ַ���壺�� AF_INET / AF_INET6
        FTLINLINE LPCTSTR GetAddressFamily(int iAddressFamily);

        //��ȡ�׽�������
        FTLINLINE LPCTSTR GetSocketType(int iSocketType);

		//FTLINLINE LPCTSTR GetSocketAddrInfoString(CFStringFormater& formater, const PSOCKADDR_IN pSockAddrIn);
		//��ȡ ADDRINFO.ai_flags ��Ӧ���ַ���
		FTLINLINE LPCTSTR GetAddrInfoFlagsString(CFStringFormater& formater, int aiFlags);

		//FTLINLINE LPCTSTR GetAddressInfoString(CFStringFormater& formater, LPSOCKADDR pSockAddr, DWORD dwAddressLength);
		//FTLINLINE LPCTSTR GetAddressInfoString(CFStringFormater& formater, SOCKET_ADDRESS& socketAddress);
		FTLINLINE LPCTSTR GetAddressInfoString(CFStringFormater& formater, const ADDRINFO& addrInfo, int nLevel = 0);

        //��ȡָ����ַ�����Э�飺 �� AF_INETx �е� IPPROTO_IP/IPPROTO_TCP/IPPROTO_UDP ��
        FTLINLINE LPCTSTR GetProtocolType(int iAddressFamily,int iProtocol);

        FTLINLINE LPCTSTR GetServiceFlagsType(DWORD dwServiceFlags);
        FTLINLINE LPCTSTR GetProviderFlagsType(DWORD dwProviderFlags);

		FTLINLINE LPCTSTR GetPerConnOptionListInfo(CFStringFormater& formater, const INTERNET_PER_CONN_OPTION_LIST& optList);
		FTLINLINE LPCTSTR GetHInternetHandleType(CFStringFormater& formater, const ULONG& HandleType);
		FTLINLINE LPCTSTR GetProxyInfoString(CFStringFormater& formater, const INTERNET_PROXY_INFO& proxyInfo);
		FTLINLINE LPCTSTR GetDiagnosticSocketInfoString(CFStringFormater& formater, const INTERNET_DIAGNOSTIC_SOCKET_INFO& diagSocketInfo);
		FTLINLINE LPCTSTR GetCacheTimeStampsString(CFStringFormater& formater, const INTERNET_CACHE_TIMESTAMPS& cacheTimeStamps);

#ifdef __WINCRYPT_H__
		FTLINLINE LPCTSTR GetCertChainContextString(CFStringFormater& formater, const PCCERT_CHAIN_CONTEXT& certChainContext);
#endif 

		FTLINLINE LPCTSTR GetReqestFlagString(CFStringFormater& formater, DWORD dwRequestFlags);
		FTLINLINE LPCTSTR GetSecurityFlagsString(CFStringFormater& formater, DWORD dwSecurityFlags);

		//��ȡ HINTERNET ��������Ϣ, ��� dwOption ��-1�� ���ȡȫ��
		FTLINLINE LPCTSTR GetHInternetOption(CFStringFormater& formater, HINTERNET hInternet, DWORD dwOption = DWORD(-1));

		//ͨ�� HttpQueryInfo ��ȡHTTP��Ϣ
		FTLINLINE LPCTSTR GetHttpQueryInfoString(CFStringFormater& formater, HINTERNET hInternet, DWORD dwInfoLevel = DWORD(-1));

        //��ȡ���ص�ַ��Ĭ�ϻ�ȡIPV4��
        FTLINLINE LONG GetLocalAddress(std::list<CFSocketAddress*>& lstAddress, ULONG Family = AF_INET, ULONG Flags = 0);

		//����URLָ�����ļ���С������
		FTLINLINE BOOL GetUrlFileSizeAndFileName(
			__in LPCTSTR pszUrl, 
			__in LPCTSTR pszCookie, 
			__out PLONGLONG pFileSize, 
			__out LPTSTR pszOutFileName, 
			__in INT nMaxFileNameSize); 
    }

	//Ĭ��ʵ��ֻ�Ǵ�ӡ���¼���־������ı���������Ϊ InternetConnect(dwContext) ��������
	//template<class T>
	FTLEXPORT class CFInternetStatusCallbackImpl
	{
	public:
		FTLINLINE CFInternetStatusCallbackImpl();
		FTLINLINE virtual ~CFInternetStatusCallbackImpl();

		FTLINLINE void SetParam(DWORD_PTR param);
		FTLINLINE BOOL Attach(HINTERNET hInternet);
		FTLINLINE BOOL Detach();
	protected:
		DWORD_PTR					m_param;
		HINTERNET					m_hInternet;

		FTLINLINE virtual void OnResolvingName(HINTERNET hInternet, DWORD_PTR dwContext, LPCTSTR lpszName, DWORD dwLength);
		FTLINLINE virtual void OnNameResolved(HINTERNET hInternet, DWORD_PTR dwContext, LPCTSTR lpszName, DWORD dwLength);
		FTLINLINE virtual void OnConnectingToServer(HINTERNET hInternet, DWORD_PTR dwContext, SOCKADDR* pSockAddr, DWORD dwLength);
		FTLINLINE virtual void OnConnectedToServer(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnSendingRequest(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnRequestSent(HINTERNET hInternet, DWORD_PTR dwContext, 
			DWORD* pdwSend, DWORD dwLength);
		FTLINLINE virtual void OnReceivingResponse(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnResponseReceived(HINTERNET hInternet, DWORD_PTR dwContext,
			DWORD* pdwResponse, DWORD dwLength);
		FTLINLINE virtual void OnCtlResponseReceived(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnPrefetch(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnClosingConnection(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnConnectionClosed(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnHandleCreated(HINTERNET hInternet, DWORD_PTR dwContext,
			INTERNET_ASYNC_RESULT* pAsyncResult, DWORD dwLength);
		FTLINLINE virtual void OnHandleClosing(HINTERNET hInternet, DWORD_PTR dwContext, DWORD* pTmpValue, DWORD dwLength);
		FTLINLINE virtual void OnDetectingProxy(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnRequestComplete(HINTERNET hInternet, DWORD_PTR dwContext,
			INTERNET_ASYNC_RESULT* pAsyncResult, DWORD dwLength);
		FTLINLINE virtual void OnRedirect(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnIntermediateResponse(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnUserInputRequired(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnStateChange(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnCookieSent(HINTERNET hInternet, DWORD_PTR dwContext, DWORD* pTmpValue, DWORD dwLength);
		FTLINLINE virtual void OnCookieReceived(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnPrivacyImpacted(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnP3pHeader(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnP3pPolicyRef(HINTERNET hInternet, DWORD_PTR dwContext);
		FTLINLINE virtual void OnCookieHistory(HINTERNET hInternet, DWORD_PTR dwContext, 
			InternetCookieHistory* pCookieHistory, DWORD dwLength);
	private:
		INTERNET_STATUS_CALLBACK	m_pOldCallBack;
		FTLINLINE static VOID	CALLBACK _StatusCallbackProc(
			HINTERNET hInternet,
			DWORD_PTR dwContext,
			DWORD dwInternetStatus,
			LPVOID lpvStatusInformation,
			DWORD dwStatusInformationLength
			);
		FTLINLINE VOID	_InnerStatusCallbackProc(
			HINTERNET hInternet,
			DWORD dwInternetStatus, 
			LPVOID lpvStatusInformation, 
			DWORD dwStatusInformationLength);
	};


	//���TCPճ����һЩ��
	class IReceiveAdapter
	{
	public:
		//�����յ��İ��ĳ���
		virtual INT ReceiveData(BYTE* pBuffer, INT nLength) = 0;
	};
	class IPacketParserAdapter
	{
	public:
		//���ط������İ��ĳ��ȣ�ÿ�η���һ�������ɣ����� CFTCPReceiver::ParsePacket ��ѭ������
		//ע�⣺�ú��������� CFTCPReceiver���� Buffer ���´��(�ú����˳��󣬿��ܱ�����)
		//      �����Ҫ���촦����ϣ����������Ҫ���ݣ���Ҫ�Լ���������
		virtual INT ParsePacket(BYTE* pBuffer, INT nLength) = 0;
	};

	//TCP���壬��ÿ�����ݽ��ն�( �� TCP Socket )�󶨣���Ϊ�������ݵĻ��壬Ŀǰ�ṩ�����¹���
	//a. TCPճ�����⴦��
	//
	//ͨ����ʹ�÷�ʽ��?
	//  1.TCP�����������Ϊ��Ա��������ָ����Ӧ�� IReceiveAdapter��IPacketParserAdapter ʵ��
	//  2.if(tcpReceiver.ReceiveData() > 0 ) { tcpReceiver.ParsePacket(); } 
	class CFTCPReceiver
	{
	public:
		FTLINLINE CFTCPReceiver(IReceiveAdapter* pReceiveAdapter, IPacketParserAdapter* pParserAdapter, INT nBufferSize = INTERNET_BUFFER_SIZE);
		FTLINLINE ~CFTCPReceiver();

		FTLINLINE BOOL IsBufferAvalibe() const { return m_pBufferHeader ? TRUE : FALSE; }

		FTLINLINE INT ReceiveData();

		//ѭ������Packet��������Ϻ�����¿�������ʣ������ݵ�Bufferͷ��
		FTLINLINE INT ParsePacket();

		//TODO:����ָ�����ȵ��ֽڣ�������
		FTLINLINE INT SkipParseData(INT nLength); 
	private:
		CFCriticalSection	m_LockObject;		//�������λ�����ٽ���
		IReceiveAdapter*		m_pReceiveAdapter;
		IPacketParserAdapter*	m_pParserAdapter;
		INT					m_nBufferSize;
		BYTE*				m_pBufferHeader;//��������ʼλ��,����� nBufferSize ���ȵ�Buffer
		BYTE*				m_pRead;		//��ǰδ�������ݵ���ʼλ��
		BYTE*				m_pWrite;		//��ǰδ�������ݵĽ���λ��
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//ʹ���̳߳ؽ��� HTTP ͨ�ŵ����紫����
	enum FTransferJobType
	{
		tjtUnknown = 0,
		tjtGet = 1,		//Not Implement
		tjtUpload,
		tjtDownload,
	};

	enum FTransferParamType
	{
		tptUnknown,
		tptPostArgument,    //Post ������ÿһ������ʽ���� Content-Disposition: form-data; name=\"xxx\"\r\n\r\nValue �ֿ�
		tptRequestHeader,	//HttpAddRequestHeaders,ĿǰΨһ��Ҫ����ṩ���� Cookie,
		tptLocalFile,		//�����ļ���Value���ļ�·��, �ϴ�ʱ���� tptPostArgument������ʱ vlaue ���ļ��ı���·��
	};
	enum FTransferOrder
	{
		toNormal,			//will send as normal order(post -> localfile -> End)
		toNeedCallback,		//ticket in nTalk maybe expired, so must send after file send
	};
	struct FTransferParam
	{
		FTransferParamType	paramType;
		CAtlString			strName;
		CAtlString			strValue;
		FTransferOrder		transferOrder;
		UINT				CodePage;	//Change Code Page, default is CP_UTF8
		FTLINLINE FTransferParam()
		{
			paramType = tptUnknown;
			strName.Empty();
			strValue.Empty();
			transferOrder = toNormal;
			CodePage = CP_UTF8;
		}
	};

	typedef std::list<FTransferParam>	TransferParamContainer;
	struct FTransferJobInfo
	{
		DISABLE_COPY_AND_ASSIGNMENT(FTransferJobInfo);
	public:
		FTLINLINE FTransferJobInfo();
		FTLINLINE FTransferJobInfo(LPCTSTR pszServerName, 
			LPCTSTR pszObjectName, 
			USHORT nPort = INTERNET_DEFAULT_HTTP_PORT);
		FTLINLINE ~FTransferJobInfo();

		//TODO: �ο� CHttpFile::AddRequestHeaders("Accept: */*");
		FTLINLINE void AddTransferParam(FTransferParamType paramType, const CAtlString& strName, const CAtlString& strValue, 
			FTransferOrder transOrder = toNormal, UINT codePage = CP_UTF8)
		{
			FTransferParam param;
			param.paramType = paramType;
			param.strName = strName;
			param.strValue = strValue;
			param.transferOrder = transOrder;
			param.CodePage = codePage;
			m_transferParams.push_back(param);
		}
		FTransferJobType	m_TransferJobType;
		CAtlString	m_strServerName;
		CAtlString	m_strObjectName;
		USHORT		m_nPort;

		DWORD_PTR	m_dwUserParam;
		TransferParamContainer	m_transferParams;
		CAtlStringA		m_strResponseData;		//û��ת���������練��(��Upload��ʹ��, Download ����Ҫ������ļ�)
		CAtlString		m_strVerb;
	};

	typedef CFSharePtr<FTransferJobInfo> FTransferJobInfoPtr;

	class CFTransferJobBase 
		: public CFJobBase<FTransferJobInfoPtr>
		, public CFRWBufferT<CFTransferJobBase>
	{
	public:
		FTLINLINE CFTransferJobBase(const CAtlString& strAgent);
		FTLINLINE virtual ~CFTransferJobBase();
	public:
		//CFRWBufferT
		FTLINLINE BOOL ReadReal(PBYTE pBuffer, LONG nCount, LONG* pRead);
		FTLINLINE BOOL WriteReal(const PBYTE pBuffer, LONG nCount, LONG* pWrite);
	protected:
		//CFJobBase virtual function
		FTLINLINE virtual BOOL OnInitialize();
		FTLINLINE virtual BOOL Run();
		FTLINLINE virtual void OnFinalize();
		FTLINLINE virtual void OnCancelJob();
	protected:
		CAtlString				m_strAgent;

		HINTERNET	m_hSession;
		HINTERNET	m_hConnection;
		HINTERNET	m_hRequest;

		//�����֧�ֶϵ���������߳����أ���Ҫʹ�� SetFilePointer�����õ� LARGE_INTEGER::QuadPart �� LONGLONG
		LONGLONG		m_nTotalSize;
		LONGLONG		m_nCurPos;

	protected:
		FTLINLINE virtual BOOL _CheckParams() = 0;
		FTLINLINE virtual BOOL _SendRequest() = 0;
		FTLINLINE virtual BOOL _ReceiveResponse() = 0;
		FTLINLINE virtual void _OnClose(){}
	protected:
		FTLINLINE void _InitValue();

		FTLINLINE BOOL _Connect();
		FTLINLINE void _Close();

		//ѭ����֤����ָ���� N ���ֽ�����
		//TODO: �ο� CHttpFile ��Buffer����
		FTLINLINE BOOL _SendN(PBYTE pBuffer, LONG nCount, LONG* pSend);

	};

	class CFUploadJob : public CFTransferJobBase
	{
	public:
		FTLINLINE CFUploadJob(const CAtlString& strAgent);
	protected:
		FTLINLINE virtual BOOL _CheckParams();
		FTLINLINE virtual BOOL _SendRequest();
		FTLINLINE virtual BOOL _ReceiveResponse();
		FTLINLINE virtual void _OnClose();
	protected:
		//translate m_transferParams to m_postArgumentParams and m_postFileParams
		FTLINLINE virtual BOOL  _TranslatePostParams();
		FTLINLINE virtual void  _ClearPostParams();
		FTLINLINE virtual void	_AddRequestHeader(LPCTSTR pszName, LPCTSTR pszValue);

		FTLINLINE virtual LPCTSTR _GetMultiPartBoundary(BOOL bEnd) const;
		FTLINLINE virtual LONGLONG _CalcContentLength();
		FTLINLINE virtual BOOL _SetRequestHeader();
		FTLINLINE virtual BOOL _SendUploadData();

		FTLINLINE virtual BOOL _SendPostArgument(PBYTE pBuffer, DWORD dwBufferSize);
		FTLINLINE virtual BOOL _SendLocalFile(PBYTE pBuffer, DWORD dwBufferSize);
		FTLINLINE virtual BOOL _SendCallbackParam(PBYTE pBuffer, DWORD dwBufferSize);
		FTLINLINE virtual BOOL _SendEndBoundary(PBYTE pBuffer, DWORD dwBufferSize);

	protected:
		struct PostArgumentParam
		{
			DISABLE_COPY_AND_ASSIGNMENT(PostArgumentParam);
		public:
			LPSTR	pBuffer;
			INT		nBufferSize;
			FTransferOrder		transferOrder;
			PostArgumentParam()
			{
				pBuffer = NULL;
				nBufferSize = 0;
				transferOrder = toNormal;
			}
			~PostArgumentParam()
			{
				SAFE_DELETE_ARRAY(pBuffer);
			}
		};
		struct PostFileParam
		{
			DISABLE_COPY_AND_ASSIGNMENT(PostFileParam);
		public:
			//ע��: pBuffer����������NULL(�����������ʱ�鿴), ��nBufferSize����������NULL(��֤���͵����ݲ�����)
			LPSTR   pBuffer;
			DWORD	nBufferSize;	

			DWORD   dwFileSize;
			CAtlString strFilePath;
			PostFileParam()
			{
				pBuffer = NULL;
				nBufferSize = 0;
				dwFileSize = 0;
			}
			~PostFileParam()
			{
				SAFE_DELETE_ARRAY(pBuffer);
			}
		};
		std::list<PostArgumentParam*>	m_postArgumentParams;
		std::list<PostFileParam*>		m_postFileParams;
		PostArgumentParam*				m_pEndBoundaryPostParam;
	};

	//����ʱ����ͨ�� tptLocalFile ָ���ļ�����ı���·��
	class CFDownloadJob : public CFTransferJobBase
	{
	public:
		FTLINLINE CFDownloadJob(const CAtlString& strAgent);
	protected:
		CAtlString m_strLocalFilePath;
        FTLINLINE VOID SetDeleteWhenCancel(BOOL bDelete);
        FTLINLINE BOOL GetDeleteWhenCancel();
		FTLINLINE virtual BOOL _CheckParams();
		FTLINLINE virtual BOOL _SendRequest();
		FTLINLINE virtual BOOL _ReceiveResponse();
		FTLINLINE virtual BOOL _OnOpenTargetFile(HANDLE hFile) { UNREFERENCED_PARAMETER(hFile); return TRUE; }
    private:
        BOOL    m_bDeleteWhenCancel;
	};

	//��������
	class CFParallelDownloadJob : public CFDownloadJob
	{
	public:
		FTLINLINE CFParallelDownloadJob(const CAtlString& strAgent);
		FTLINLINE virtual BOOL _CheckParams();
	protected:
		LONG m_nBeginPos;
		LONG m_nEndPos;
		FTLINLINE virtual BOOL _OnOpenTargetFile(HANDLE hFile);


	};

	class IInternetTransferCallBack : public IFThreadPoolCallBack<FTransferJobInfoPtr>
	{
	public:
		//Download
		FTLINLINE virtual BOOL OnPromptSaveFile(__in LONG nJobIndex, __inout CFJobBase<FTransferJobInfoPtr>* pJob , 
			__in LONGLONG nTotalFileSize, __inout LPTSTR pszFileName, __in DWORD nBufferLenght) 
        {
            UNREFERENCED_PARAMETER(nJobIndex);
            UNREFERENCED_PARAMETER(pJob);
            UNREFERENCED_PARAMETER(nTotalFileSize);
            UNREFERENCED_PARAMETER(pszFileName);
            UNREFERENCED_PARAMETER(nBufferLenght);
            return TRUE; 
        }
	};

	class CFInternetTransfer
	{
	public:
		FTLINLINE CFInternetTransfer( );
		FTLINLINE virtual ~CFInternetTransfer( void );

		FTLINLINE BOOL IsStarted();
		FTLINLINE BOOL Start(IInternetTransferCallBack* pCallBack = NULL, // IFThreadPoolCallBack<FTransferJobInfoPtr>* pCallBack = NULL, 
			LONG nMinParallelCount = 1, 
			LONG nMaxParallelCount = 4, 
			LPCTSTR pszAgent = NULL);
		FTLINLINE BOOL Stop();
		FTLINLINE void Close();
		
		FTLINLINE LONG AddDownloadTask(FTransferJobInfoPtr pDownloadJobInfo);
		FTLINLINE LONG AddUploadTask(FTransferJobInfoPtr pUploadJobInfo);
		FTLINLINE LONG AddTask(CFTransferJobBase* pTransferJob);

		FTLINLINE BOOL CancelTask(LONG nJobIndex);
	protected:
		CAtlString		m_strAgent;
		CFThreadPool<FTransferJobInfoPtr>*		m_pThreadPool;
	};

	//////////////////////////////////////////////////////////////////////////

	//URL ��ַ�����ƺ�ֻ�� URL_COMPONENTS(����Ҫ�����Ա?) + InternetCrackUrl ����?
	//URL�в��ܰ���ĳЩ�����ַ��������Ҫת���� %ʮ���������� ����ʽ�� �磺"%" => "%25"; "?" => "%3F", "#" => "%23", "&" => "%26" ��
	class CFUrlComponents : public URL_COMPONENTS
	{
	public:
		FTLINLINE CFUrlComponents();
		FTLINLINE BOOL ParseUrl( LPCTSTR pstrURL, DWORD& dwServiceType,
			WORD& nPort, DWORD dwFlags );
	public:
		TCHAR m_szScheme[INTERNET_MAX_SCHEME_LENGTH];
		TCHAR m_szHostName[INTERNET_MAX_HOST_NAME_LENGTH];
		TCHAR m_szUserName[INTERNET_MAX_USER_NAME_LENGTH];
		TCHAR m_szPassword[INTERNET_MAX_PASSWORD_LENGTH];
		TCHAR m_szUrlPath[INTERNET_MAX_URL_LENGTH];
		TCHAR m_szExtraInfo[INTERNET_MAX_PATH_LENGTH];
	};

	//////////////////////////////////////////////////////////////////////////
#if 0
	#define __HTTP_VERB_GET			TEXT("GET")
	#define __HTTP_VERB_POST		TEXT("POST")
	#define __HTTP_ACCEPT_TYPE		TEXT("*/*")
	#define __HTTP_ACCEPT			TEXT("Accept: */*\r\n")
	#define __SIZE_BUFFER			1024

	//TODO:TimeOut
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