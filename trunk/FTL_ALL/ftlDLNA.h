#ifndef FTL_DLNA_H
#define FTL_DLNA_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlDLNA.h requires ftlbase.h to be included first
#endif

//TODO: ��androidϵͳ�п���DLNA -- http://blog.csdn.net/gebitan505/article/details/19497545
//DLNA ���豸���ֺͿ��Ʋ�ʹ�õ�UPnPЭ���׼�� http://upnp.org/sdcps-and-certification/standards/device-architecture-documents/
//UPnPЭ����ʵ��(һ) http://www.ibm.com/developerworks/cn/linux/other/UPnP/part1/
//����CyberGarage���dlna���� --  http://blog.csdn.net/lancees/article/details/8477513
//DLNA�������� -- http://download.csdn.net/download/geniuseoe2012/4969961


/*************************************************************************************************************************
* ��������
*   ���ļ��� -- �Ѹ�����Ļ���߰Ѹ����豸����������Э�飬��Ҫ�� Airplay, DLNA, Miracast
*   �첥�����Ƶҵ�񣬿����� SmartPlay Э�飬����Ĵ�ͨ�˸����豸
*************************************************************************************************************************/

/*************************************************************************************************************************
* ��Դʵ�ֿ�
*   MiniDLNA -- https://github.com/azatoth/minidlna
*   SlickDLNA -- https://github.com/KernelCrap/android-dlna
*   UPnP SDK(�����俪����DLNA) -- http://upnp.org/sdcps-and-certification/resources/sdks/
*     �кܶ�ѡ�񣬿�֧�� C,C++,Java�ȣ����бȽϺõģ�
*       Plutinosoft -- 
*       CyberGarage(��˼Ҳ�����) -- 
*     Androidƽ̨��: NDK�������� jni�� => Ӧ�ò㿪��
*       ע�⣺Androidƽ̨�ϵ� MediaPlayer ����Ƶ�Ĵ��������൱����������Ը�����Ƶ�����õ�֧�֣���Ҫ��ֲ��Ƶ��������
*              �磺ffmpeg��gstream��vlc ��
*************************************************************************************************************************/


/*************************************************************************************************************************
* Airplay -- ƻ�����еĲ���ϵͳ��֧�ָ�Э�顣ƻ��ԭ����DLNA�ĳ�Ա�����˳������Ż���Э���DLNA�Ƚ����ơ�
*   �����ˣ�������(�ֻ��ϵ�����ֱ����ʾ��Apple TV�ϣ�������Ϸ)��˫����Ϸ����
*************************************************************************************************************************/

/*************************************************************************************************************************
* DLNA(Digital Living Network Alliance) -- ���������������ˣ����ᡢӢ�ض���΢��ȷ���http://www.dlna.org/
*   Ŀ���ǹ������ںͻ���Ŀ��Ի��������ѵ���(CE)��PC���ƶ��豸�ȹ��ɵ����Ⱥ��������磬�ٽ����ֶ�ý�����ݺͷ���Ĺ���
*   ֻ��������Ƶ����Ƶ��ͼƬ���ͣ�����ý���ļ�ͨ�����ߵķ�ʽ���͵����ӻ���ͶӰ�����档
*
* DLNA �����豸������ָ������(Networked Device Interoperability Guidelines -- 500$), �������� 1.5 �汾
*   �����˿ɹ��ƽ̨����������Ŀ��Խ����Ļ���ģ�顣
*
* �豸����(Device Category)�ͽ�ɫ(Roles)��
*   HND(Home Network Devices) -- ָ��ͥ�豸�����бȽϴ�ĳߴ缰��ȫ��Ĺ���
*     DMS(Digital Media Server) -- ����ý����������ṩý���ȡ����¼���洢��������ܡ�DMS�ܰ���DMP�Ĺ��ܣ�
*       �洢ý���ļ������ͥ�����е�DMP��DMR�ṩý�����ݵ��б�����ݡ��� PC�������е�
*     DMP(Digital Media Player) -- ����ý�岥���豸���ܴ�DMS/M-DMS�ϲ��Ҳ���ȡý�����ݲ����ź���Ⱦ��ʾ�������ܵ��ӵ�
*     DMR(Digital Media Renderer) -- ����ý����Ⱦ�豸��ͨ�������豸���ú󣬿��Բ��Ŵ�DMS�ϵ����ݡ���DMP����������û�в��ҡ����ý��Ĺ��ܣ��� ��ʾ���������
*     DMC(Digital Media Controller) -- ����DMS�����ݲ�����DMS��DMR֮������Ӳ�����ý��Ĳ��ţ���ң����
*     DMPr(Digital Media Printer) -- ����ý���ӡ������ �����ӡ����һ�廯��ӡ��
*   MHD(Mobile Handled Devices) -- �ֳ��豸��������Լ�һЩ��֧�ֵ�ý���ʽҲ��ͬ��
*     M-DMS(Mobile DMS), M-DMP, M-DMC
*     M-DMD(Mobile Digital Media Downloader) -- �ƶ���ý�������豸�����������ֲ��������������ֲ����������ܵ�������
*     M-DMU(Mobile Digital Media Uploader) -- �ƶ���ý�������豸���������豸���ֻ��ȡ�
*   HID(Home Interoperability Devices)| NID(Networked Infrastructure Devices) -- ����֧���豸��Ϊ�����豸�����Ļ������ṩ֧��
*     MIU(Media Interoperability Unit) -- ý�彻���豸���ṩý���ʽ��ת����֧�ָ����豸(HND �� MHD)��Ҫ
*     M-NCF(Mobile Network Connectivity Function) -- �ƶ��������ӹ����豸���ṩ�����豸�����ƶ������������ʣ�DLNA��ϣ����ȫ��ʵ�����߻���
*   ������ʽ�� HND <=> [ HID <=> ] MHD
*     Ҫ����HND��MHD�豸֮��Ľ�������ҪHND��MHD����Է���Ҫ�󡣵������������޷�����������������������ҪHID�豸�ṩ�ŽӺ�����ת������

* �豸����(Device Capabilities)�ͽ�ɫ(Roles) -- �ù����ܲ�ѯ����
*   +PU+(Push Controller) -- ���ͱ������ݵ�DMR
*   +PR1+/+PR2+(Printing Controller-1/-2) -- ����DMPr����ͼ�����ݴ�ӡ
*   +UP+(Upload Controller) -- ����Upload���ܣ���DMS��M-DMS��������
*   +DN+(Download Controller) -- ��DMS��M-DMS�������ݵ�����
*
* �豸���ܣ�
*   UPnP CP(Control Point) -- ����UPnP�豸�ܹ����豸���ֺͿ���
*   MSD(UPnP AV MediaServers) -- �ṩ����ý�����ݵķ�ʽ
*   MSCP(UPnP AV MediaServer Control Point) -- ���������ѡ��MSD�ṩ��ý�����ݵĿ�����
*   MRD(UPnP AV MediaRenderer) -- �ṩ����ý�����ݵķ�ʽ
*   MRCP(UPnP AV MediaRenerer Control Point) -- ����ѡ��MRD�������ݵĿ�����
*   PrD(UPnP Printer Device) -- �ṩ��ӡͼ�������
*   PrCP(UPnP Printer Control Point) -- ������ӡ����Ŀ�������ѡ����Ҫ��PrD��ӡ������
*   MTS(Media Transport Server) -- 
*   MTC(Media Transport Client)
*
* �ܹ�(DLNA Architecture)
*   1.ý���ʽ(Media Formats)���涨����ζ���Ҫ����Ͳ��ŵ����ݽ��б��룬��ÿһ���豸���࣬DLNAý���ʽģ��
*     ���3��ý������(ͼƬ����Ƶ����Ƶ)������һ��ǿ��<>�Ϳ�ѡ[Optional]��ý���ʽ��
*       Home Devices -- Imaging<JPEG>[GIF,TIFF,PNG], Audio<LPCM>[MP3,AAC,WMA9,AC-3��], Video<MPEG2>[MPEG1,MPEG4,WMV9]
*       Mobile/Handheld Devices -- Imaging<JPEG>[GIF,TIFF,PNG], Audio<MP3,MPEG4 AAC>[MPEG4,WMA��], Video<MPEG4 AVC>[VC1,H263,MPEG4 AVC��]
*     ���DMS/M-DMS �豸�����ý���ʽ���Ա������豸��֧�ֵ�����£���ֱ�Ӵ��ͣ��������ת���ɽ��ն���֧�ֵ�ý���ʽ��
*   2.ý�崫��(Media Transport)ģʽ��
*     a.�����䷽ʽ -- ��ý�����ݰ������õ���Ҫƥ���ʱ�������̱�DMP/M-DMP/DMR ��Ⱦ����ʱ����ý���������Թ̶�������ʵʱ����ʱ��
*     b.�������䷽ʽ -- ���������ڲ�ʱ����Ϣ��ý�����ݱ����������û�ʵʱ����ʱ������Ҫ������ʾ���û�ͼƬ��
*     c.��̨���䷽ʽ -- ��ý�����ݲ���Ҫ������Ⱦ���Ż��û����䴫������ȼ���Ҫ���ʱ��(��Ҫ����M-DMD/M-DMU�����ϴ������ط�ʵʱ���ɵ�����ʱ)
*     DLNA�豸����֧���� ��HTTP�� ��Ϊ�������䷽ʽ�����ý�����ݣ�RTP��Ϊ��ѡ��ý�崫�䷽ʽ���ṩ�������Ż�ý�崫���System Usage��չ��
*   3.ý�����(Media Management)��ʵ���豸��Ӧ�öԼ�ͥ�����е�ý�����ݵ�ʶ�𡢹����ַ��ͼ�¼���� UPnP AV Architecture �� UPnP Printer Architecture �����ĵ�˵����δ�����ЩӦ������
*     ��ΪDLNA�豸��ý�������������
*   4.�豸���ֺͿ��ƣ��豸�����ܹ����ּ�ͥ�����ϴ��ڵ������豸�ͷ��񣬲�ʶ�����úͿ������ǵĹ��ܺ���Ӧ��������
*     ʹ�� UpnPTM �����豸Э��ܹ�(DCP Framework) ��Ϊ�豸���ֺͿ��ƵĽ��������������������Щ�������������豸�Ļ�����
*   5.Network Connectivity(���绥��) -- ����IPv4(/v6?)�ĸ������ӣ�
*     802.3(Ethernet), 802.11a/g(WiFi), 802.11n(OFDM��MIMO), 802.15(Bluetooth)��,
*     ͨ�� QoS�Ż������������Դ��ʹ����Ƶ������Ƶ����������
*
*   1.���ְ�Ȩ�������ݱ��� -- ������·����(����֧��DTCP-IP����ѡ֧��WMDRM-ND)��DRM��DLNA�����ܱ�����ҵ�������ݲ���δ��Ȩ��ʹ�úͿ�����
*************************************************************************************************************************/

/*************************************************************************************************************************
* Miracast -- ��Wi-Fi������2012�����ƶ�����Wi-Fiֱ��Ϊ������������ʾ��׼��֧�ִ˱�׼���豸��ͨ�����߷�ʽ������Ƶ���档
*   ȱ�㣺�������£�֧���豸���٣����������졣
* 
*************************************************************************************************************************/


/*************************************************************************************************************************
* UPnP(Universal Plug and play) -- ͨ�ü��弴��(http://upnp.org/)����DLNA�ĺ���
*   UPnP��׼��http://upnp.org/resources/upnpresources.zip
* 
*   UPnP��һ������ PC ���������豸(������)�ĳ����Ե��������ӵ���ϵ�ṹ���������ڼ�ͥ�С�
*     ��Internet ��׼�ͼ���(TCP/IP��HTTP��XML)Ϊ������
*       1.ʹ�������豸�˴˿��Զ����Ӻ�Эͬ������
*         ʾ����Win7����������UPnPѡ����ڼ�ͥ�����й���һ��װ����Ƶ���ļ��У���SmartTV�����Զ��ҵ����ļ��У���֧�ֲ��š�
*       2.����ͥ���ڵ�devices���Զ���NAT�Ͷ˿�ӳ��(Port Mapping)--��Ϊ��ͥ������û����ô��IP��
*   �ĵ���
*     UPnP Device Architecture 1.0 -- ˵���豸������ͨ��UPnP���໥���ֺͿ��ƣ��Լ�������Ϣ��
* 
*   AV�淶������豸
*     MS(UPnP AV MediaServer)
*     MR(UPnP AV MediaRender)
*   AV����
*     CDS(Content Directory Service) -- ����Ŀ¼����ö�ٲ��г��ɷ��ʵ����ݣ�����Ƶ�����ֺ�ͼƬ��
*     CMS(Connection Manager Service) -- ����ý�����ݿ���ͨ�����ַ�ʽ�� UPnP AV MediaServer ������ UPnp AV MediaRenderer
*     AVT(AV Transport Service) -- ����ý�����ݵĴ��䣬�粥�š�ֹͣ����ͣ�����ҵ�
*     RCS(Rendering Control Service) -- �����Ժ��ַ�ʽ�������ݣ��� ���������������ȵ�
*   �����ʲô��
*     SRS(Scheduled Recording Service) -- 
*   Printer �ܹ�
*     �����˴�ӡ�豸�͹����Ŀ��Ƶ�Ӧ��֮��Ľ���ģ��
*
* UPnP�Ĺ�������
*   0.Ѱַ(Addressing) -- ÿ���豸��Ӧ����DHCP�Ŀͻ�,���豸�״������罨�����Ӻ�,�õ�һ��IP��ַ��һ����˵·�������иù���
*   1.����(Discovery) -- ��һ���豸����ӵ������UPnP�ķ���Э��������豸�������ϵ�Control Points(CPs)֪ͨ(advise)�Լ�ӵ�еķ���
*     ͬ����һ��CP����ӵ������UPnP����Э�������CP���������Ͽ��õ��豸��
*     һ��ͨ��SSDPЭ���鲥���� �豸�ͷ���Ļ�����Ϣ(���ͣ�Ψһ��ʶ������ǰ״̬������)��
*     �鲥��ַΪ��HOST: 239.255.255.250:1900�� ? Response ��UDP����
*   2.����(Description) -- CP���豸����������Ϣ������XML��ʽ������(device + service),�����и���ϸ��Ϣ��URL
*   3.����(Control) -- ͨ�����豸�������ָ�����Control URL���Ϳ�����Ϣ������Device�������ݷ������д���
*     ��ͨ��Ϣ����SOAP�ĸ�ʽ��д(���ڵİ汾�� SOAP 1.1 UPnP Profile)
*   4.�¼�(Eventing) -- �ڷ�����е�����ʱ���ڣ���״̬�����˸ı䣬���Բ���һ���¼�����������������жಥ(multicast);
*     CPҲ�����������¼�����������(subscribe)�¼���Ϣ����֤����CP����Ȥ���¼���ʱ׼ȷ�ص������͹���(unicast)��
*     �¼��Ķ��ĺ�����ʹ�õ�Э���� GENA��
*   5.���(Presentation) -- ң������UI��
*     ֻҪ�õ����豸��URL���Ϳ���ȡ�ø��豸����URL��ȡ�ø��豸����HTML��Ȼ����Խ���HTML����CP�ı���������ϡ�
*
* UPnP AV(Audio/Video) Architecture -- �ṩ�˽����Ļ��ơ�ģ�ͣ�����û�й涨���ú��ּ�����ʵ��(���й㷺����Ӧ��)��
*   AV�ܹ���CP(Actionͨ����CP����)�Ĺ���Ҫ����10����
*     1.����AV�豸
*     2.�������������б�
*     3.�����Ⱦ��֧�ֵ�Э��/��ʽ
*     4.�Ƚ�/ƥ��Э��/��ʽ
*     5.���÷�����/��Ⱦ��
*     6.ѡ�����������
*     7.��ʼ���ݴ���
*     8.������Ⱦ����
*     9.�ظ���ѡ����һ������
*     10.�Ͽ�����������Ⱦ�����ӡ�
*   CP������MediaServer��MediaRenderer��RemoteControl������CP�Ľ�ɫ�����ɳ��������ֳ�����AV�豸����ģ�ͣ�
*     2-Box Pull Model -- CP��MediaRenderer(�������ֻ�)��������MediaServer��ȡý�����ݣ��������֮�󲥷�ý��
*     2-Box Push Model -- CP��MediaServer(��PC)��CP������MediaRenderer����(push)ý�塣
*     3-box model -- CP������Ϊһ��ң������Ҳ��Ϊpull��push���ַ�ʽ��
*       a -- pull��ʽʱ,CP��Renderer����Server��Server������ý�����ݵ�URL����Rendererȥȡ
*       b -- push��ʽʱ��CP��Server��Renderer��URL����Serverȥ��Renderer����ý������
*************************************************************************************************************************/



namespace FTL
{
}
#endif //FTL_DLNA_H

#ifndef USE_EXPORT
#  include "ftlDLNA.hpp"
#endif 