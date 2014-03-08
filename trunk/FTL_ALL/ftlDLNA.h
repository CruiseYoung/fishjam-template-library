#ifndef FTL_DLNA_H
#define FTL_DLNA_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlDLNA.h requires ftlbase.h to be included first
#endif

/*************************************************************************************************************************
* DLNA(Digital Living Network Alliance) -- ���������������ˣ�Ŀ���ǹ������ںͻ���Ŀ��Ի��������ѵ���(CE)��PC���ƶ��豸�ȹ��ɵ����Ⱥ��������磬
*   �ٽ����ֶ�ý�����ݺͷ���Ĺ���
* DLNA �����豸������ָ������(Networked Device Interoperability Guidelines) �����˿ɹ��ƽ̨����������Ŀ��Խ����Ļ���ģ�顣
*
* �豸����(Device Category)�ͽ�ɫ(Roles)��
*   HND(Home Network Devices)
*     DMS(Digital Media Server) -- ����ý����������洢ý���ļ������ͥ�����е�DMP��DMR�ṩý�����ݵ��б������
*     DMP(Digital Media Player) -- ����ý�岥���豸����������Ͳ��������ϵ�����ý������
*     DMR(Digital Media Renderer) -- ����ý������豸
*     DMC(Digital Media Controller)
*     DMPr(Digital Media Printer)
*   MHD(Mobile Handled Devices)
*     M-DMS(Mobile DMS), M-DMP, M-DMC
*     M-DMD(Mobile Digital Media Downloader)
*     M-DMU(Mobile Digital Media Uploader)
*   HID(Home Interoperability Devices) �����ǣ� Home Infrastructure Device��Ϊ�����豸�����Ļ������ṩ֧��
*     MIU(Media Interoperability Unit) -- �Ž�ý���ʽ�㣬��������ת����ý�彻����Ԫ���ṩ HND �� MHD ���ý���ʽת����
*     M-NCF(Mobile Network Connectivity Function) -- �����Ž��������Ӳ�
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
* �ܹ�
*   1.Networking and Connectivity(�������ͨ��) -- ����IPv4(/v6?)�ĸ������ӣ�802.3/802.11/������,
*     ͨ�� QoS�Ż������������Դ��ʹ����Ƶ������Ƶ����������
*   2.�豸���ֺͿ��ƣ��豸�����ܹ����ּ�ͥ�����ϴ��ڵ������豸�ͷ��񣬲�ʶ�����úͿ������ǵĹ��ܺ���Ӧ��������
*     ʹ�� UpnPTM �����豸Э��ܹ�(DCP Framework) ��Ϊ�豸���ֺͿ��ƵĽ��������������������Щ�������������豸�Ļ�����
*   3.ý�����ʵ���豸��Ӧ�öԼ�ͥ�����е�ý�����ݵ�ʶ�𡢹���ͷַ����� UPnP Audio/Video �� UPnP Printer ������ЩӦ������
*     ��ΪDLNA�豸��ý�������������
*   4.ý���ʽ���涨����ζ���Ҫ����Ͳ��ŵ����ݽ��б��룬��ÿһ���豸���࣬DLNAý���ʽģ�����3��ý������(ͼƬ����Ƶ����Ƶ)������һ��ǿ�ƺͿ�ѡ��ý���ʽ��
*       Home Devices -- Imaging<JPEG>, Audio<LPCM>, Video<MPEG2>
*       Mobile/Handheld Devices -- Imaging<JPEG>, Audio<MP3,MPEG4>, Video<MPEG4 AVC>
*     ���DMS/M-DMS �豸�����ý���ʽ���Ա������豸��֧�ֵ�����£���ֱ�Ӵ��ͣ��������ת���ɽ��ն���֧�ֵ�ý���ʽ��
*   5.ý�崫���ʽ��
*     a.�����䷽ʽ -- ��ý�����ݰ������õ���Ҫƥ���ʱ�������̱�DMP/M-DMP/DMR ��Ⱦ����ʱ����ý���������Թ̶�������ʵʱ����ʱ��
*     b.�������䷽ʽ -- ���������ڲ�ʱ����Ϣ��ý�����ݱ����������û�ʵʱ����ʱ������Ҫ������ʾ���û�ͼƬ��
*     c.��̨���䷽ʽ -- ��ý�����ݲ���Ҫ������Ⱦ���Ż��û����䴫������ȼ���Ҫ���ʱ��(��Ҫ����M-DMD/M-DMU�����ϴ������ط�ʵʱ���ɵ�����ʱ)
*     DLNA�豸����֧���� ��HTTP�� ��Ϊ�������䷽ʽ�����ý�����ݣ�RTP��Ϊ��ѡ��ý�崫�䷽ʽ���ṩ�������Ż�ý�崫���System Usage��չ��
*   6.���ְ�Ȩ�������ݱ��� -- ������·����(����֧��DTCP-IP����ѡ֧��WMDRM-ND)��DRM��DLNA�����ܱ�����ҵ�������ݲ���δ��Ȩ��ʹ�úͿ�����
*
* UPnP
*   AV�淶������豸
*     UPnP AV MediaServer
*     UPnP AV MediaRender
*   AV����
*     CDC(Content Directory Service) -- ����Ŀ¼����ö�ٿɷ��ʵ����ݣ�����Ƶ�����ֺ�ͼƬ��
*     Connection Manager Service -- ����ý�����ݿ���ͨ�����ַ�ʽ�� UPnP AV MediaServer ������ UPnp AV MediaRenderer
*     AV Transport Service -- ����ý�����ݵĴ��䣬�粥�š�ֹͣ����ͣ�����ҵ�
*     Rendering Control Service -- �����Ժ��ַ�ʽ�������ݣ��� ���������������ȵ�
*   Printer �ܹ�
*     �����˴�ӡ�豸�͹����Ŀ��Ƶ�Ӧ��֮��Ľ���ģ��
*************************************************************************************************************************/


namespace FTL
{
}
#endif //FTL_DLNA_H

#ifndef USE_EXPORT
#  include "ftlDLNA.hpp"
#endif 