#ifndef DIRECT_SHOW_WINDOWS_SHOW_H
#define DIRECT_SHOW_WINDOWS_SHOW_H
#pragma once

/**********************************************************************************************
* Windows Media Files--WMF(ASF,WMV,WMA)���� WMVCore.dll �ṩPlayback��Authoring�Ĺ��ܣ���Dll��
*   Windows Media Player 9 �� Windows Media Format Runtime ���ṩ��
*  
* 
* ͨ����Ҫʹ�� Windows Media Format SDK(WMFS) ������DirectShow���ṩ�˲���Filter����֧��
*   1.Windows Media Player 6.4 �У�����Ϊ Windows Media Source ��Filter֧��ASF�ļ��Ĳ���
*   2.DirectShow 8.0 ������������Filter��WM ASF Reader �� WM ASF Write�������ڲ�ʹ�� WMFS
*
* �����֤ -- ���ž��� DRM �������ļ�,ͨ����IServiceProvider�ӿڵ�COM���ʵ��(����һ����Կ�ṩ��������)�� 
*   Windows Media �ļ����ʹ����DRM�������������ͱ����ṩDRM�����(WMStub.lib)��
*   WMF9��ǰ��Ҫʹ�� Windows Media certificate handler�� unkey(IServiceProvider����), 
*   ��Ҫʹ�� Windows Media Format SDK (WMStub.LIB -- ע��DirectShow ���ṩ�İ汾��֧��DRM),
*   �μ� DS Sample AudioBox �� PlayWndASF
* 
*   CertHandler :
*     #include <dshowasf.h>���� IServiceProvider::QueryService ��ʵ���� 
*     if( siid == __uuidof(IWMReader) && riid == IID_IUnknown ) ʱ��
*     ���� WMCreateCertificate ����certificate�����ء�
*   ����ʱʹ�� WM ASF Reader filter��CLSID_WMAsfReader),���GraphBuilder��IObjectWithSite��
*     ͨ�� SetSite((IUnknown *) (IServiceProvider *) &prov); ������Ҫʹ�õ� Cert Handler
*   ����ý���ļ�ʱ��������NS_E_LICENSE_REQUIRED(0xC00D0BBEL,�ȴ���֤)/NS_E_PROTECTED_CONTENT(0xC00D0BBDL,��֤ʧ��)�ȴ��� 
*     ����ý���ļ���DRM������������ҪLicense���ܲ��ţ�(must acquire a DRM stub library from Microsoft
*     and link it with this application. the default version of WMStub.lib does not support DRM)
*     IMediaEventEx::GetEvent Ϊ EC_WMT_EVENT ʱ��Ҫ���� DRM ��֤����
* 
* ����(Profile��SDK����ProfileEnum.exe����) -- ����ASF�ļ�ʱ�����趨 WM ASF Writer(CLSID_WMAsfWriter) 
*   �м�������Pin�������ļ��ı����ʡ�ѹ�������ȡ�
*   IConfigAsfWriter -- ע�⣺�����ȼ���FilterGraph��Ȼ�����ã�����ٽ�������
*   IWMProfileManager(2) -- ö��ASF����
*
* ע�⣺
*   1.ASF������
*   2.ASF�ļ���������ƵΪ��׼�����ţ�����ASFWriter������Ҫһ����Ƶ������
**********************************************************************************************/

#endif //DIRECT_SHOW_WINDOWS_SHOW_H