#ifndef FTL_P2P_H
#define FTL_P2P_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlP2P.h requires ftlbase.h to be included first
#endif

#include <p2p.h>
#include <pnrpns.h>

namespace FTL
{
	/*************************************************************************************************************************
	* TODO:
	*   1.eMule ��Ϊʲô��ͨ��IP��ַ�Ĺ�ϵ�����ɽڵ�ID(�������)�������ڽ���XOR��ʱ��IP��ַ�ȽϽӽ��Ľڵ���߼�����Ҳ��Ƚ϶�
	*     CPrefs::m_uClientID
	*   2.CreateSearchExpressionTree �����Ǵ����������ʽ���Ǵ����ڵ�Ķ������ṹ��
	*   3.������ʱ��Ҫ�ҵĵ�һ���ڵ�(�����Ʒ������Ľڵ㣿)
	*************************************************************************************************************************/

	/*************************************************************************************************************************
	* �Ѷ�����
	*   (δ��)netds\peertopeer\graphchat\xp
	*   (δ��)netds\peertopeer\graphchat\vista -- Vista/Win7�汾��P2P�������
	*************************************************************************************************************************/

	/*************************************************************************************************************************
	*
	* P2P(Peer-to-Peer) -- �ԵȻ���/��Ե㼼������һ�ּ���������Э�顣ÿ���ڵ��ṩ�Ĺ��ܶԵȣ����÷ֲ�ʽ����洢�ṹ
	*   ����һ̨���������һ̨�����ֱ�ӽ������ݣ�������Ҫͨ������������
	*   ���ƣ����ؾ��⣬���ͷ�����ѹ�������Լ۱ȣ�����չ��ǿ���ɺ����洢�ͼ���
	*   ���⣺֪ʶ��Ȩ���������簲ȫ
	* ���ģʽ��
	*   ������P2P�ܹ� -- û������������������ڵ�֮��ֱ�ӽ�����Ϣ������Ҫ�û��ֹ�ָ���Է���IP:�˿ڣ��޷��Զ�������չ
	*   �����P2P�ܹ�(NetpSter) -- ��һ����������������𵽴ٳɸ��ڵ�Э������չ�Ĺ��ܡ�ÿ���ڵ㶼����������������ע��ͷ�ע�ᣬ
	*     �����ڵ�������������ȡ�����Ϣ�������Ӧ�Ľڵ�ֱ�����ӽ�����
	* ������P2P�㷨����Gnutella��FreeNet,Napster,Groove,Magi ��
	* �ѵ㣺
	*   1.ά��Ӧ�ò������ -- ���⣨�ػ��������⣨���Ż�����Ͽ���ʹP2PȺ���鷢��Ƶ���仯
	*   2.��ȫ�� -- 
	*   3.��Ҫ����������Ϣ������ר��Э�飻
	*   4.������Ҳ�����������"�����ַת��"(NAT) �����ǽ�����ĳӦ�ó����ʵ��
	*************************************************************************************************************************/
	

	/*************************************************************************************************************************
	* eMule(��Դ��P2P)
	*   ���غͱ���(����������Ҫ�û��Լ�����)������ʱע�����ѡ��һ��
	*     1.Crypto++(http://www.eskimo.com/~weidai/cryptlib.html), using namespace CryptoPP��
	*       CxImage
	*       id3lib -- ����дID3v1��ID3v2��ǩ
	*       pnglib(http://www.libpng.org/pub/png/libpng.html)
	*       miniupnpc -- ���������������е�UPNP�豸
	*       ResizableLib(http://sourceforge.net/projects/resizablelib/  -- ����ʱ�Զ�����UI�Ŀ�
	*       zlib(http://www.gzip.org/zlib/ -- ����ѹ��
	*       AtlServer -- �ƺ���Ҫ��SendMail.cpp ��ʹ�������е� atlsmtpconnection.h
	*   ����:
	*     1.ͨ���ļ�hash�������ļ�(���жϳ��ļ�����ͬ��������ͬ���ļ�) -- ��ͬ���ݵ��ļ�����ͬ��hashֵ������
	*     2.���÷ֿ����ʵ�ָ�Ч�����������֤(CKnownFile::CreateFromFile + CAICHRecoveryHashSet ), �������������ȵķֿ鷽ʽ,
	*       (TODO:��ʲô�ô���)�ȰѴ��ļ��ָ������� 9500K �Ŀ飬����Щ����֯��һ����״�ṹ���ٰ�ÿһ�������Ŀ��ַֽ�����ɸ� 180K �Ŀ�,��Ȼ������״�ṹ��֯
	*     3.AICH(Advanced Intelligent Corruption Handling,�߼������𻵴���) -- �ֿ����hash�����ļ�����ʱ����ʹ��������Ҳ���Բ����ش������ļ�
	*     4.ThrottledControlSocket + UploadBandwidthThrottler -- ʵ������ȫ�����ٵĹ��ܣ������е��׽��ֱ���ɷ��Ͷ��У����й�����RunInternal�е��߼���
	*       ����ÿ��socket�������(�ܷ��Ͷ����ֽ�)->���㱾��ѭ��Ӧ��˯�߶���ʱ��(ͨ��˯�߽�������)->����������Ϣ���з��Ͷ����е�����
	*     5.ͨ��Э��(����Ƴ���������ĸ�ʽ)����������zlib����ѹ���ͽ�ѹ��
	*       TCP -- Header_Struct(Э��� + ������ + ������ + ��������Ľṹ����)
	*       UDP -- UDPPack
    *     
	* Kad����(eMule��ʹ��,��Ҫ������ CKademlia) -- ��һ��DHT(Distributed Hash Table ��ɢʽ�Ӵձ�)��Э�飬����ʹ�ڵ�֮�以�ౣ��һЩ�����ڵ����ϵ��Ϣ��
	*   ������������һ��"��ϵ��"Ѱ�ҵ����������е��κ�һ���ڵ��Լ��������Դ���������̲���Ҫ�κ����ķ�������ÿ���ͻ��˸�����һС����
	*   search��source finding�Ĺ��������乤����ԭ���ǻ��ڿͻ��˵�ΨһID��search��source��hash֮���ƥ����ȷ���ģ�Ȼ���û����ɶ�λ
	*   ����������ʱ��Ҫ֪ͨ����ʵ��û�ID����ַ -- ͨ���û�id���ֵ��������֮����߼����룬��������·�ɲ�ѯ�ٶ�
	*   Kad�����У����нڵ�(CContact)��������һ�ö�������Ҷ�ӣ����ҽڵ��λ�ö�����IDֵ�����ǰ׺Ψһ��ȷ�����Լ��Ƕ������ĸ��ڵ㡣
	*   ����һ����N ���ڵ��Kad ���磬���ֻ��Ҫ����logN ����ѯ���Ϳ���׼ȷ��λ��Ŀ��ڵ㡣
	*   ����Զ��RPC ����:
	*     PING  -- ̽��һ���ڵ㣬�����ж����Ƿ���Ȼ����
	*     STORE -- ֪ͨһ���ڵ�洢һ��<key, value>��
	*     FIND_NODE -- ��Ӧemule�в��ҽڵ�Ĳ���
	*     FIND_VALUE -- ��Ӧemule���ļ������Ĳ���
	*   �������(���Һͷ���ʱ���У����ݾ�����ȷ�����ҵĵݽ����Լ�Ӧ�÷�������Щ�ڵ�)
	*     CRoutingZone::GetClosestTo -> CRoutingBin::GetClosestTo -> CUInt128::Xor
	*************************************************************************************************************************/

	/*****************************************************************************************************
	* ΢���ṩ�� P2P ��SDK(p2p.h) -- ������IPV6�ϣ�Ŀǰ��ʹ���нϴ�����
	*   PNRP --  ������������Ҫ�������������ַ,�������һ�����÷�����ʽ��P2PӦ�ó���
	*     WinXP�ϰ�װ�����/ɾ�� Windows ��� -> ������� -> �Ե����磬Ȼ���ڷ��������� "�Ե�����" ���� 
	*     ͨ�� netsh ��PNRPע��һ���µ�PNRP����netsh -> p2p phrp peer -> add 0.justtest(P2P��)
	*   PNM(People Near Me) -- Peer�����ָ�ʽ�� [authority].[classifier]
	*     ��PNM��ע������û���¼PNM��p2phost.exe����PNM�����в��������ڵ㣬�ҵ����������, 
	*   
	*   �������̣�
	*     1.PeerGraphStartup -- ��ʼ������P2P����
	*     2.PeerGraphCreate  -- 
	*   ��ظ���
	*     Group
	*     Graph
	*     Identity
	*     PNRP(Peer Name Resolution Protocol) -- �Ե����ƽ���Э�飬�ж�Ӧ�� PNRPsvc ����
	*       PeerPnrpGetCloudInfo -- ��ȡ���ýڵ��������б��� "LinkLocal_ff00::%11/8" ��
	*     Collaboration -- 
	*
	* Vista + .NET Framework 3.0 ��WCF��������PeerChannel,
	*****************************************************************************************************/

	//΢��P2P�����ļ򵥷�װ

	class CFP2PUtil
	{
	public:
		static void FreeData(LPCVOID pvData);
	};

	FTLEXPORT class CFP2PEnvInit
	{
	public:
		CFP2PEnvInit(BOOL bInitGraph = TRUE, BOOL bInitPnrp = TRUE);
		~CFP2PEnvInit();
	protected:
		//BOOL m_bStartup;
		BOOL m_bInitGraph;
		BOOL m_bInitPnrp;
		PEER_VERSION_DATA	m_PeerVersion;
	};

	FTLEXPORT class CFPeerPnrp
	{
	public:
		//need call FreeData to free ppCloudInfo
	};

	FTLEXPORT class CFPeerGraph
	{
	public:
		FTLINLINE CFPeerGraph();
		FTLINLINE virtual ~CFPeerGraph();
		FTLINLINE HRESULT CreateOrOpen(PCWSTR pwzGraphId, PCWSTR pwzPeerId, PCWSTR pwzDatabaseName, PPEER_SECURITY_INTERFACE pSecurityInterface);

		FTLINLINE HRESULT Create(PPEER_GRAPH_PROPERTIES pGraphProperties, PCWSTR pwzDatabaseName, PPEER_SECURITY_INTERFACE pSecurityInterface);
		FTLINLINE HRESULT Open(PCWSTR pwzGraphId, PCWSTR pwzPeerId, PCWSTR pwzDatabaseName, PPEER_SECURITY_INTERFACE pSecurityInterface,
			ULONG cRecordTypeSyncPrecedence, const GUID *pRecordTypeSyncPrecedence, PHGRAPH phGraph);

		FTLINLINE HRESULT Close();
	protected:

	private:
		HGRAPH	m_hGrpah;
	};


	FTLEXPORT class CFPeerIdentity
	{
	public: 
		FTLINLINE CFPeerIdentity();
		FTLINLINE virtual ~CFPeerIdentity();

		FTLINLINE HRESULT Create(PCWSTR pwzClassifier, PCWSTR pwzFriendlyName, HCRYPTPROV hCryptProv);
		FTLINLINE HRESULT Delete();

		FTLINLINE PWSTR  GetIdentity();
		//FTLINLINE PCWSTR GetFriendlyName();
		//FTLINLINE HRESULT SetFriendlyName(PCWSTR pwzFriendlyName);
		//FTLINLINE HCRYPTPROV GetCryptKey();
		
	private:
		PWSTR	m_pwzIdentity;

	};
}

#endif //FTL_P2P_H

#ifndef USE_EXPORT
#  include "ftlP2P.hpp"
#endif