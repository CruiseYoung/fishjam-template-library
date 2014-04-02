#ifndef FTL_DLNA_H
#define FTL_DLNA_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlDLNA.h requires ftlbase.h to be included first
#endif

//TODO:
//  1.AllCast�� app��VCR���� -- ʲô��˼?

//����ע��:
//0.��������:C/C++(��ƽ̨�����Ĵ��������); Windows(DLL); Android(.so); iOS(Framework)
//1.��Ҫ֧�� Naver Media Player �� NaverTV����ƽ̨Ϊ Windows/Android/IOS ��
//  �ڸ���ƽ̨��ʹ�õĶ�ý��� -- ��ȡý�� ��С������ ����Ϣ
//2.֧�ֶ����豸��Э�� -- DLNA/AirPlay/ChromeCast ��Э�� �� SmartTV/XBox/ChromeCast/AppleTV ���豸
//  �ص���Э�飬�豸ֻ��Ҫ������ϸ����
//3.֧�ֵ�ý������ -- Photo/Audio/Video
//4.֧�ֵ����� -- ����ý��(DMS)��Զ��ý��(DMC)�����ز���(DMR)
//                 local �ļ�, HTTP URL, HLS VOD URL, HLS LIVE URL
//5.��ʽ -- ��ת��Դ�ļ��ĸ�ʽ,ֱ�Ӵ��͡�ֻ�� HLS Source ��Ҫת��Ϊ���Բ��ŵĸ�ʽ
//6.���紫��Э�� -- ����֧�� HTTP �� HLS��������Ҫ������չ�Ľӿ�


//DLNA ���豸���ֺͿ��Ʋ�ʹ�õ�UPnPЭ���׼�� http://upnp.org/sdcps-and-certification/standards/device-architecture-documents/
//UPnPЭ����ʵ��(һ) http://www.ibm.com/developerworks/cn/linux/other/UPnP/part1/
//����CyberGarage���dlna���� --  http://blog.csdn.net/lancees/article/details/8477513
//DLNA�������� -- http://download.csdn.net/download/geniuseoe2012/4969961
//������ص�һЩ֪ʶ -- http://www.h3c.com.cn/MiniSite/Technology_Circle/Net_Reptile/The_Five/

/*************************************************************************************************************************
* ����ʱ��Э�����
*   1.UDP -- ���ҡ�����ý�������ʱ, ʹ�� SSDP
*   2.HTTP -- ���ƽ��������� Action ʱ��ʹ�� SOAP
*   3.RTSP(?) -- ��ѡ��ý�崫��Э��
*
* DLNA��ý�岥�Ź���(���Ƶ�� MediaServer ��ѡ���ļ����� MediaRender �ϲ���)
*   1.ͨ��SSDP�ҵ� MediaServer �� MediaRender
*     ʾ��:a.ʹ�� M-SEARCH ����; b.���յ��豸��ʱ���͵� ssdp:alive 
*   2.ʹ�� ContentDirectory::Browse �� Search �� MediaServer �����ý����Ϣ������ֵΪ DIDL-Lite �淶��XML
*     ʾ��:ObjectID(0);BrowseFlag(BrowseDirectChildren);Filter(*);StartingIndex(0);RequestedCount(0);SortCriteria(��)
*     ���п��Ի�ȡ�� ����Э��(protocolInfo), ���ӵ�ַURL ����Ϣ
*   3.ʹ�� ConnectionManager::GetProtocolInfo �ֱ��ȡServer��Render�Ĵ���Э��(protocolInfo)��֧�ֵ����ݸ�ʽ�б���Ϊ Source��Sink,
*     ����ֵΪ���ŷֿ��� http-get:*:image/jpeg:DLNA.ORG_PN=JPEG_SM �� http-get:*:audio/mp3:* ��
*   4.ƥ��ý���ļ��Ͳ�����֧�ֶ�֧�ֵĴ���Э������ݸ�ʽ -- һ����˵�����������б���Ҫ��������ʵ�
*   5.ͨ�� ConnectionManger::PrepareForConnection ����Render���õ� AVTransport::InstanceId, ֮��ͨ����ֵ����״̬����
*     ʾ��(ִ�гɹ���᷵�� InstanceId, ��ֵ��ͨ�� ConnectionManger::GetCurrentConnectionIDs ȷ��)
*          RemoteProtocolInfo(http-get:*:video/x-ms-wmv:*);PeerConnectionManager(���ַ���);PeerConnectionID(-1);Direction(Input)
*   6.ʹ�� AVTransport::SetAVTransportURI �� MediaRender ����Ҫ����ý���ļ�����Ϣ
*     ʾ��:
*       InstanceID(ǰ��ķ���ֵ);CurrentURI(ָ�� "Wildlife in HD" ��Ӧ��URL);
*   7.ͨ�� AVTransport::play/stop �� RenderingControl::SetVolume ��������ý�岥��
*     ʾ��:���ţ���ͣ�����������ȵ���
*       Seek: Unit(REL_TIME); Target(00:00:05) -- ��λ����5��
*       GetPositionInfo -- ���Ի���ܳ��ȡ���ǰλ�õ�
*   8.������Ϻ���Ҫͨ�� ConnectionManager::ConnectionComplete �ر�����
*************************************************************************************************************************/

/*************************************************************************************************************************
* ��������
*   ���ļ��� -- �Ѹ�����Ļ���߰Ѹ����豸����������Э�飬��Ҫ�� Airplay, Chromecast, DLNA, Miracast, [ Wi-Fi Direct, WiDi ?]
*   �첥�����Ƶҵ�񣬿����� SmartPlay Э�飬����Ĵ�ͨ�˸����豸
* ������Ӳ��
*   AllCast -- Android�ϵ���������豸�ĸ�����Ҳʮ�ָ�(֧��Chromecast, SmartTV, XBox, AppleTV ��)
*   EzCast -- ����ChromeCast��Ӳ��
*   AllShare(����), Smart Share(LG)
*
* ע�⣺
*   1.ý�������Ϊ��������Ƶ����ʱ�Ļ���ʱ�䣬ͨ����ѳ���Ƶ�ֳ�С�Σ���Ҫ���ٷֶ���Ƶ֮���л���ɵ�ͣ��ʱ���ƽ������
*   2.ע����Ҫ����"AV follow me"������ -- ��Ƶ�ڵ��Ӻ��ֻ����������л�(�����л�ʱ��Ҫ���㲥��)
*************************************************************************************************************************/

/*************************************************************************************************************************
* ��Դʵ�ֿ� -- �ṩUPnPջ
*   MiniDLNA -- https://github.com/azatoth/minidlna
*   SlickDLNA -- https://github.com/KernelCrap/android-dlna
*   UPnP SDK(�����俪����DLNA) -- http://upnp.org/sdcps-and-certification/resources/sdks/
*     �кܶ�ѡ�񣬿�֧�� C,C++,Java�ȣ����бȽϺõģ�
*       Cling -- http://4thline.org/projects/cling, Android�ϱȽϺõĿ������ߣ�֧�ֿͻ��˺ͷ������ˡ����Э��ΪLGPL��CDDL
*       CyberGarage/CyberLink(��˼Ҳ�����) -- http://www.cybergarage.org/net/upnp,
*         ����ʱ��Ƚ��磬֧��C/C++/Java/ObjectC �ȶ�ƽ̨(ÿ��ƽ̨�е�����ʵ��)��������֧�ֲ��ã�����������
*         ʹ��BSDЭ�飬֧����ҵ������
*       Intel �� openTools -- http://opentools.homeip.net/
*       Platinum -- ��ƽ̨��C++ʵ��,֧�� Windows,Linux,Mac,iPhone,Android �ȣ�XBMC��ʹ�øÿ⡣˫lincense
*         http://blog.csdn.net/lancees/article/details/9865411
*       Plutinosoft -- 
*     Androidƽ̨��: NDK�������� jni�� => Ӧ�ò㿪��
*       ע�⣺Androidƽ̨�ϵ� MediaPlayer ����Ƶ�Ĵ��������൱����������Ը�����Ƶ�����õ�֧�֣���Ҫ��ֲ��Ƶ��������
*              �磺ffmpeg��gstream��vlc ��
*************************************************************************************************************************/


/*************************************************************************************************************************
* Airplay -- ƻ�����еĲ���ϵͳ��֧�ָ�Э�顣ƻ��ԭ����DLNA�ĳ�Ա�����˳������Ż���Э���DLNA�Ƚ����ơ�
*   �����ˣ�������(�ֻ��ϵ�����ֱ����ʾ��Apple TV�ϣ�������Ϸ)��˫����Ϸ����
*   �ٷ�SDK -- https://developer.apple.com/library/ios/documentation/AudioVideo/Conceptual/AirPlayGuide/Introduction/Introduction.html
*************************************************************************************************************************/

/*************************************************************************************************************************
* Chromecast -- https://developers.google.com/cast/
*   Google�Ƴ��ĵ��Ӱ���Ʒ(HDMI+WiFi)���������м򻯵�Chrome����ϵͳ������HDMI�ӿ��ϣ�ͨ���ֻ����ƣ�
*     ����:1.��Google����������URL;2.��������ȡ��Դ(����Ƶ����ҳ);3.����Դ����Chromecast;4.Chromecast����Դ�����������
*   ������Google Cast SDK
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
*
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
*
*   
*************************************************************************************************************************/

/*************************************************************************************************************************
* Miracast(WiFi Display) -- ��Wi-Fi������2012�����ƶ�����Wi-Fiֱ��Ϊ������������ʾ��׼��֧�ִ˱�׼���豸��ͨ�����߷�ʽ������Ƶ���档
*   ȱ�㣺�������£�֧���豸���٣����������졣
* 
*************************************************************************************************************************/

/*************************************************************************************************************************
* Platinum UPnP SDK -- http://www.plutinosoft.com/platinum
*   �ŵ�:
*     1.��ƽ̨(ͨ�� Neptune ��ʵ��)��ͳһ���룬
*   ȱ��:
*     1.���ܲ�ȫ -- ����ȱ�� ConnectionManger::PrepareForConnection
*     2.�ƺ���֧��IPV6
*   ���룺Neptune �� Platinum ������ɾ�̬��
*     1.Windows:Platinum\Build\Targets\x86-microsoft-win32-vs2008\Platinum.sln
*     2.Android JNI: NDK + Cygwin
*       2.1 ���� ANDROID_NDK_ROOT ��������;
*       2.2 ���� Neptune\Source\Core\NptConfig.h �ļ����� __CYGWIN__ �������м��� #define NPT_CONFIG_HAVE_GETADDRINFO
*           ����ᱨ���޷����� NPT_NetworkNameResolver::Resolve �Ĵ���
*       2.3 ���� Platinum Ŀ¼��ִ�� scons target=x86-unknown-cygwin build_config=Debug, ������AndroidʱӦ�� arm-android-linux
*           ��ѡ��target���� Neptune\Build\Boot.scons �в鿴(Ҳ��ͨ�� scons --help �鿴) 
*       2.4 ���� Platinum/Source/Platform/Android/module/platinum�� ִ�� ndk-build NDK_DEBUG=1
*************************************************************************************************************************/

/*************************************************************************************************************************
* XBMC(http://xbmc.org/) -- XBOX Media Center (Դ�� -- git://github.com/xbmc/xbmc.git )
*   ��Դý���������(GPL), ���ΪXBox��ƣ������ѿ���Linux��OSX��Windows��Android4.0ϵͳ����, ֧�� DLNA,Airplay ��Э��
*   ��ع��ߵ�����(�ϲ�Ŀ¼��ÿ�ձ����): http://www2.frugalware.org/mirror/xbmc.org/build-deps/win32/
*   Windows����( http://wiki.xbmc.org/index.php?title=HOW-TO:Compile_XBMC_for_Windows )��
*     1.����: VS2010 + DirextX2009_08�Ժ� + Git + Nullsoft + JRE
*     2.project\BuildDependencies\DownloadBuildDeps.bat -- �Զ��������ر��������������
*     3.project\BuildDependencies\DownloadMingwBuildEnv.bat -- ���ر���ffmpeg�����������
*     4.project\Win32BuildSetup\buildmingwlibs.bat -- ���� ffmpeg ��
*     5.��ѡһ�ı���(һ����Կ���ʱѡ�ڶ���)
*       5.1.project\Win32BuildSetup\BuildSetup.bat -- ֱ�ӱ���һ������ļ�
*       5.2.project\Win32BuildSetup\extract_git_rev.bat -- ��������ϵͳ��Ϣ��������ʾgit�汾
*     6.project\VS2010Express\XBMC for Windows.sln -- VS2010���б��룬ѡ��DirectX�汾(OpenGL�汾�Ѿ�������)
*   Ŀ¼�ṹ����:
*     addons -- ����Ԫ������XBMC��Ƥ��,��Ļ����,���ӻ�Ч����
*     project -- ��Ŀ�����ļ�����Ҫ��
*************************************************************************************************************************/


/*************************************************************************************************************************
* 
* ���ͼܹ���
*   +-Root Device > ������߼�Device > Service > Action(Arguments) �� Variable
*   +-Control Point
*
* UPnP(Universal Plug and play) -- ͨ�ü��弴��(http://upnp.org/)����Ҫ����ʵ���豸�����ܻ�����ͨ����DLNA�ĺ���
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
*     CDS(Content Directory Service) -- ����Ŀ¼����ö�ٲ��г��ɷ��ʵ����ݣ�����Ƶ�����ֺ�ͼƬ�ȣ�
*       Browser -- ���ý���ļ�
*     CMS(Connection Manager Service) -- ���ӹ������(Server��Render������֧��)
*     AVT(AV Transport Service) -- ����ý�����ݵĴ��䣬�粥�š�ֹͣ����ͣ�����ҵȣ����ܵ��Ӿ���Ҫʵ�������
*       GetPositionInfo -- ���Ի�ȡ��ý��ĳ��ȡ���ǰλ�õȣ�TODO: Windows Media Service �� GetMediaInfo ���ܻ�ȡ������
*       SetAVTransportURI -- ����Ҫ���ŵ�ý���URL
*       ֧�ֵĶ���Event:LastChange(����״̬�ı�),
*     RCS(Rendering Control Service) -- �����Ժ��ַ�ʽ�������ݣ��� ���������������ȵ�
*   �����ʲô��
*     SRS(Scheduled Recording Service) -- 
*   Printer �ܹ�
*     �����˴�ӡ�豸�͹����Ŀ��Ƶ�Ӧ��֮��Ľ���ģ��
*
* UPnP�Ĺ�������
*   0.Ѱַ(Addressing) -- ÿ���豸��Ӧ����DHCP�Ŀͻ�,���豸�״������罨�����Ӻ�,ͨ�� DHCP Discover + DHCP Offer �õ�һ��IP��ַ��
*     һ����˵·�������иù��ܡ���û����Ӧ����ʹ�� Auto-IP ���IP��ַ������ (169.254/169.16)��Ҳ����ʹ�þ�̬���õ�IP��ַ��
*   1.����(Discovery) -- ��һ���豸����ӵ������UPnP�ķ���Э��������豸�������ϵ�Control Points(CPs)֪ͨ(advise)�Լ�ӵ�еķ���
*     ͬ����һ��CP����ӵ������UPnP����Э�������CP���������Ͽ��õ��豸��
*     һ��ͨ��SSDPЭ���鲥���� �豸�ͷ���Ļ�����Ϣ(���ͣ�Ψһ��ʶ������ǰ״̬������)��
*     ����: "HOST: 239.255.255.250:1900" �� "M-SEARCH",
*     ֪ͨ: "HOST: 239.255.255.250:1900" �� "NOTIFY", ������Ӧ����ʹ��UDP�ĵ�������
*     �ò�����֪�豸������UPnP���ͣ�UUID���豸������URL��
*   2.����(Description.xml) -- CP���豸����������Ϣ������XML��ʽ������(device + service),�����и���ϸ��Ϣ��URL��ͨ�����εݹ��ѯ��
*     �����ĵ� -- ��Ҫ���豸���������������ȡ���������������Ϣ,�汾�ȣ��ɱ��豸���õ�ͼ���URL��ַ���豸�б������б�ȣ�
*     �������ˣ��豸��Ҫ����һ����׼��HTTP����(��������ȫ��Web������Ҳ�������������������Ҫ���ṩHTTP����)
*   3.����(Control) -- ͨ�����豸�������ָ�����Control URL���Ϳ�����Ϣ������Device�������ݷ������д���
*     ��ͨ��Ϣ����SOAP�ĸ�ʽ��д(���ڵİ汾�� SOAP 1.1 UPnP Profile), ʹ��HTTP��POST��M-POST�������XML�ĵ�
*   4.�¼�(Eventing) -- �ڷ�����е�����ʱ���ڣ���״̬�����˸ı䣬���Բ���һ���¼�����������������жಥ(multicast);
*     CPҲ�����������¼�����������(SUBSCRIBE)�¼���Ϣ����֤����CP����Ȥ���¼���ʱ׼ȷ�ص������͹���(unicast)��
*     �¼��Ķ��ĺ�����ʹ�õ�Э���� GENA��
*   5.��������(Presentation) -- ң������UI��
*     ֻҪ�õ����豸��URL���Ϳ���ȡ�ø��豸����URL��ȡ�ø��豸����HTML��Ȼ����Խ���HTML����CP�ı���������ϡ�
*     �������豸��Ӧ���ṩ������������û�����ͱ�̿��ƽӿڣ�Ҳ��������Ա�����Լ����豸���档
* 
* Control Point��Device����ʱʹ�õ�Э�飺
*   SOAP:   Control
*   HTTP:   Description
*   HTTPMU: Notify/Search
*   HTTPU:  Search Response
*   GENA:   Event
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
*
*
* ��ʽ
*   �豸���� -- ������ʽһ��Ϊ urn:schemas-upnp-org:device:uuid-device������ uuid-device ΪUPnP����ίԱ�ᶨ��ı�׼�豸���͡�
*     uuid ֵΪ 1 ��ʾ:
*     deviceֵΪ 0 ��ʾ:
*     �豸������Ҳ����ָ�����������֣�һ��Ϊ urn:domain-name:device:uuid-device������ uuid-deviceΪ�����̶���ı�׼�豸���ͣ�domain-name�ֶ�Ϊ�豸������ע�������
*     �����豸����(deviceType):InternetGatewayDevice(һ����·�����������豸)
*   �������� -- ��ʾ�����ͳһ��Դ����һ���ʽΪ urn:schemas-upnp-org:service:serviceType:version
*     UPnP�豸�����̿���ָ�����ӷ��񣬸�ʽΪ urn:domain-name:service:serviceType:version
*     �������� ɨ����(scanner),
*
* UPnP��NAT�е�Ӧ�� 
*   �û�ͨ��NAT�������� + ͬʱ��Ҫʹ�� BitComment������(eMule)��P2P�������UPnP���Զ��İ�P2P��������Ķ˿ں�ӳ�䵽�����ϣ�
*     �Ա㹫���ϵ��û��ܶ�NAT˽���෢�����ӣ������������Ч�����ٶ�
*   ǰ��������
*     1.NAT�����豸�����UPnP����
*     2.����ϵͳ(��WinXP)�����UPnP���ܣ�
*       a.���ɾ������->Windows���->�������->UPnP�û�����;
*       b.Windows����ǽ->����->UPnP���
*       c.����-> ���� "SSDP Discovery Service" �� ��Universal Plug and Play Device Host�� ����
*       Ȼ����ڡ��������ӡ��п�������Internet���أ���Ӧ�ó�������ʱ��Ӷ˿ں������п��Կ����������Ϣ��
*     3.Ӧ����������UPnP����,�� BitComment ��:ѡ��->�߼�->��������->����ʹ��UPnP�Զ��˿�ӳ��
*       Ȼ����"ȫ��ͳ�ơ���ǩҳ�п��Կ���NAT�˿�ӳ�������(����IP: TCP�����˿�:25118, UDP�����˿�:25118)
*   
*************************************************************************************************************************/

/*************************************************************************************************************************
* UPnPЭ�����
-------------------------------------------------------------------------------------
|                      UPnP�豸�����̶���                                    |
|                          UPnP��̳����                                       |
|                       UPnP�豸����Ľṹ                                   |
|      SSDP, GENA              ||    SOAP(�豸����,�豸����), GENA(�豸�¼�)     |
|   HTTPU/HTTPMU(�豸����)    ||                                                   |
|                              UDP                                                  |
|                              IP                                                   |
-------------------------------------------------------------------------------------
*  HTTPU/HTTPMU -- HTTPЭ��ı���,��ʽ��Ϯ��HTTPЭ�飬��ͨ��UDP����TCP�����أ����ҿ������鲥����ͨ��
*
* SSDP(CyberLink �е� SSDPPacket):
*   ����֪ͨ(NOTIFY * HTTP/1.1\r\n): �豸���뵽������ʱ���������ϵĿ��Ƶ��֪���ṩ�ķ��񣬲��Ҷ��ڷ���
*     HOST:239.255.255.250:1900\r\n -- ��ʾ�ǹ㲥
*     LOCATION: ���豸������URL���� http://192.168.1.1:2800/InternetGatewayDevice.xml\r\n
*     NT: �������ͣ��� upnp:rootdevice\r\n
*     NTS:ssdp:alive\r\n
*     USN:uuid:aaaabbbb-cccc-dddd-eeeeffffgggghhhh::upnp:rootdevice\r\n -- ��ʾ��ͬ�����ͳһ����������ʾ��ͬ���ͷ�������
*   ��ѯ(M-SEARCH * HTTP/1.1\r\n): ���Ƶ���뵽������ʱ��Ѱ�������ϸ���Ȥ���豸,��Ӧ SSDPPacket::isDiscover()
*     HOST: 239.255.255.250:1900\r\n -- �ಥ��ַ
*     MAN:"ssdp:discover"\r\n -- �̶���ʽ�����������ţ�û�в�������ʾ��ѯ
*     MX:3\r\n -- �����豸��Ӧ��ȴ�ʱ�䣬�豸��Ӧ��0�����ֵ֮�����ѡ����Ӧ�ӳٵ�ֵ
*     ST: ����Ŀ��(Search Target)����ѡ�������£�
*         ssdp:all -- �����豸�ͷ���
*         upnp:rootdevice -- �����������еĸ��豸
*         uuid:device-UUID -- ��ѯUUID��ʶ���豸
*         urn:schemas-upnp-org:device:device-type:version -- ��ѯdevice-Type�ֶ�ָ�����豸���ͣ��豸���ͺͰ汾��UPNP��֯����
*         urn:schemas-upnp-org:service:service-type:version -- ��ѯservice-Type�ֶ�ָ���ķ������ͣ��������ͺͰ汾��UPNP��֯����
*   ��Ӧ(HTTP/1.1 200 OK) -- �豸�����Լ�������Ƶ�������Ŀ�꣬������Ӧ
*     EXT:\r\n -- �սڵ㣬����Ƶ�ȷ��MANͷ���Ѿ����豸���
*     LOCATION:http://xxxx -- �������豸������URL��ַ
*     SERVER:Wireless N Router WR845N, UPnP/1.0\r\n -- ��������ϵͳ�����汾����Ʒ���Ͳ�Ʒ�汾��Ϣ��
*     ST: -- ����Ӧ��Ӧ������Ŀ�꣬���ݺ��������ѯ�������Ӧ�ֶ���ͬ
*     USN: -- uuid:xxxx, 
*   NTS(?) -- �����ڹ����е���Ϣ?
*     ssdp:alive --
*     ssdp:byebye
*     upnp:propchange
*
*   ���Ʊ���: ���Ƶ��ȷ���һ��������Ϊ����Ҫ���豸��ʼ����Ȼ���ٰ��豸��URL������Ӧ�Ŀ�����Ϣ
*     1.POST /EmWeb/UPnP/Control/4 HTTP/1.1 -- ��Ŀ�ĵ�ַ��POST Soap ��ʽ������
*       SOAPAction: "urn:schemas-upnp-org:service:WANIPConnection:1#GetSpecificPortMappingEntry"
*       <soap:Envelope> + <soap:Header> + <soap:Body>
*
*   �¼����ĺ�֪ͨ -- ����XML�У�ʹ��GENA��ʽ
*     NT: upnp:event -- 
*     SEQ
*
*   TODO:
*     
*************************************************************************************************************************/

/*************************************************************************************************************************
* UPnP�ĳ���ʵ������
*   1.AllShare
*     �豸: 
*       <deviceType>urn:schemas-upnp-org:device:MediaServer:1</deviceType>
*       ����: 
*         a.<serviceType>urn:schemas-upnp-org:service:ConnectionManager:1</serviceType> 
*         Action�б�(3��):
*           a1.GetCurrentConnectionIDs
*           a2.GetCurrentConnectionInfo
*           a3.GetProtocolInfo
*         b.<serviceType>urn:schemas-upnp-org:service:ContentDirectory:1</serviceType>
*         Action�б�(31��):
*           b1.Browse
*           b2.GetFeatureList -- 
*   2.Windows7 �е� "SSDP Discovery" + "Windows Media Player Network Sharing Service"
*     ������ʽ:
*       a.������Ӧ�ķ���
*       b.�������\����� Internet\����͹������� �����õ�ǰ���������Ϊ��ͥ����(���豸���԰�ȫ�Ļ������),
*         "��������"����"��ͥ��"�в鿴; ��ͥ����,ѡ��"���ҵ�ͼƬ�����ֺ���Ƶ������ҵļ�ͥ�����ϵ������豸";
*         "����͹�������\ý����ѡ��\��ý��ѡ��"�������豸����
*       c.Windows Media Player -- "ý����"�˵��й�ѡ"����Զ�̿����ҵĲ�����"��"�Զ������豸�����ҵ�ý��",
*         �����ֳ�������С�
*   3.֧��DLNA���������ܵ���
*     �豸:
*       <deviceType>urn:schemas-upnp-org:device:MediaRenderer:1</deviceType>  -- ��׼��Render��һ���ֻ�������ʱ������Ŀ��������
*       ����:
*       <deviceType>urn:samsung.com:device:MainTVServer2:1</deviceType>
*       <deviceType>urn:samsung.com:device:MainTVServer2:1</deviceType>
*************************************************************************************************************************/

/*************************************************************************************************************************
* Cling -- Android ��
*  ����:
*    1.(TODO:����ֻ��ģ����������)Android��֧�ֽ���UDP�鲥,�����Է���UDP�鲥����ˣ����Է��������е��豸�������ܷ�����Ѱ���¿������豸��
*  ��Ҫ�ӿں���
*    interface AndroidUpnpService -- 
*************************************************************************************************************************/

/*************************************************************************************************************************
* CyberLink () -- �ŵ㣺�򵥣�ȱ�㣺Ч�ʵͣ��ڲ�ʹ��string(��UTF-8), ���ܲ����ƣ��н϶�bug
*   Ĭ�������ͬʱ֧�� IPV4/IPV6, ��ͨ�� UPnP::SetEnable() ��������
*   C++ �汾
*     1.Դ��: CyberLinkForCC + HttpEngineForCC + expat
*     2.����:
*       a. linux: ./boostrap -enable-libxml2<CR> ./configure<CR> make
* 
*   ControlPoint�Ļص��ӿ�(��Ҫ���ö�Ӧ�� addXxxxListener ����ע��������Ч):
*     DeviceChangeListener -- �� deviceAdded/deviceRemoved ����������֪�� Device ������
*     EventListener -- �� eventNotifyReceived ���������Խ��յ�ע������¼�֪ͨ(����+ֵ)
*     NotifyListener -- �� deviceNotifyReceived ���������Խ��յ��豸��SSDP֪ͨ��Ϣ
*     SearchResponseListener -- �� deviceSearchResponseReceived ���������Խ��յ�������������Ӧ��SSDP��Ϣ
*   
* �߳�:
*   HttpServer -- ���ص�Http������
*   SSDPNotifySocket -- �� HttpMusocket ����
*   SSDPSearchResponseSocket -- 
*   Disposer -- ÿ60��ѭ��һ�Σ��Ƴ���ʱ�豸(removeExpiredDevices)
*************************************************************************************************************************/

/*************************************************************************************************************************
* Intel openTools
*   Device Builder -- �������ɹ��ߣ�
*   Device Sniffer -- SSDP���Թ��ߣ���������ʾ�����ϵ�����SSDP��Ϣ
*   Device Spy -- ��һ�� Control Point�������������������豸���ṩ���÷����ͷ����ͨ�÷���
*************************************************************************************************************************/

/*************************************************************************************************************************
* Android ƽ̨����
*   ����NDK��SDK�����JNI��
*************************************************************************************************************************/

namespace FTL
{
}
#endif //FTL_DLNA_H

#ifndef USE_EXPORT
#  include "ftlDLNA.hpp"
#endif 