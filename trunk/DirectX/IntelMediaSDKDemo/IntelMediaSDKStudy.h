#ifndef INTEL_MEDIA_SDK
#define INTEL_MEDIA_SDK
#pragma once

/*************************************************************************************************
* ����
*   MVC(Multi-View Video Coding)
*   VPP(Video Processing)��ָ Pre-Post processing ?
*
* Intel��Media SDK��һ��ǿ�������Ƶ����롢����⣬��������Intel���Կ�ƽ̨�ϣ��ṩӲ�����٣���
*   ���ṩ��Ӧ����������ʵ�֣���ѡ��ǿ�Դ��
*   ������Ϣ��http://software.intel.com/en-us/articles/intel-media-software-development-kit-intel-media-sdk/
* 
* ע�⣺
*   1.Intel Media SDK Ŀǰֻ֧��NV12(Ӣ�ض��������Ƶ��ʽ������Ӣ�ض�Ӳ��ƽ̨����ԭ��̬֧��)
*     MSDK��ʽת��: http://software.intel.com/zh-cn/blogs/2010/07/26/msdkyv12nv12/
*
*
* Media SDK֧�� VC1,H264 �� MPEG2 ��ʽ��Intel�Կ�ƽ̨��Ӳ�����١�
* ��Media SDk�У���������Ƶ��һЩ�ؼ����ĵı��롢�����㷨������Intel IPP�⣬���ڴ˻����϶Դ������ܽ������Ż�����ߡ�
* �ṩ�˷ḻ��API�Թ��ͻ����ο�������������չ���Ӷ�֧���κε�������ͼ��Ӳ�����������graphics hardware solutions���ͱ���뷽����
*   �ṩ�˱�׼��DShow Filters��
* 
* SDK�ı�̽ṹ��ͨ����ķַ��㣨Dispatcher layer,�� Encode/Decode/VPP �ȣ���¶��Ӧ�ó����
*   �ṩ��ͨ�õ�������ƵԤ���������Ľӿڣ��ò��ṩ���ض����ܡ�
*   1.�ַ�����dispatcher����λ��ǰ���Կ����豸��������
*   2.���ݶ�λȥSDK�в�������ʵ�ʵ�ַ�ʽ(��Ӳ������)�����ڵ������ض���(����ָ��?)�����ʵĺ�������
*   3.���û���ҵ��ʺϵ�ƽ̨��ص�ʵ�ַ�ʽ��dispatcher�ᶨλ��SDK�������ʵ�ֵķ�ʽ
*
*
* Media SDK ��ʼ����������Ĳ���(Ҳ�ṩ�˼򵥷�װ�õ�C++�ӿڣ��� MFXVideoSession/MFXVideoENCODE �� )��
*  1.�����ͳ�ʼ��һ������� mfxSession -- MFXInit ��������Ҫָ��������ʵ�ַ�ʽ���Զ�ѡ��Ӳ�����������
*    �����Autoʱ������ʹ�� MFXQueryIMPL ȷ�ϵ�ǰ��ʵ�ַ�ʽ��
*  2.�����ͳ�ʼ�������� -- MFXVideoDECODE_Init
*  3.�����ͳ�ʼ�������� -- MFXVideoENCODE_Init
*  n.ʹ����Ϻ�ʹ�� MFXClose �ر�Session
*
* Media SDK �ṩ���첽����(MFXVideoDECODE_DecodeFrameAsync��MFXVideoVPP_RunFrameVPPAsync��MFXVideoENCODE_EncodeFrameAsync)
*   �ȣ��Ƚ��ʺ�Ӳ������루һ���߳̾Ϳ�ʵ�ָ����ܣ�����������ʱ����ʹ����ˮ�߽ṹ��
*
* ͨ�� CodecPreset �ṩ��Ԥ�����һЩ�����������
* 
* ע���� HKCU\Software\Intel\Media SDK Sample Filters �лᱣ��Ĭ��ֵ��Ҳ����ͨ�������������
*   IsHWMfxlib -- 1��ʾʹ��ƽ̨��ص�MediaSDKʵ��
*   IsDecodePartiallyAccelerated/IsEncodePartiallyAccelerated -- �Ƿ��м���?(MFX_WRN_PARTIAL_ACCELERATION)
*   FramesEncoded -- (��?)�����֡��(���ƻ���ͳ��ֵ��)
*   RealBitrate -- �������е���ʵ������
**************************************************************************************************/

/**************************************************************************************************
* Դ�����
*   1.mfxSession -- MediaSDK�ĺ���,ͨ�� MFXInit��ʼ��,ʹ����Ϻ�ͨ��MFXClose�ر�,��ͨ��MFXVideoSession�������
*     �亯����Ϊ���¼������֣������ֱ���װ�ɶ�Ӧ�� MFXVideoXXX ����м򻯴���
*     ��������:   �� QueryXXX, ��Session�������ȼ�(mfxPriority)����
*     VideoCore:  ���� BufferAllocator��FrameAllocator; ���û��ȡ Handle(?)��ͬ������(SyncPoint) ��
*     VideoEncode:����Encode�Ĳ�����ͨ�� mfxVideoParam ������ز�����ͨ�� EncodeFrameAsync �����첽���룿
*     VideoDecode:����Decode�Ĳ�����ͨ�� DecodeHeader �Ƚ���ͷ����ͨ�� DecodeFrameAsync �����첽���룿
*       �������֧�� ����֡����֡���֡���������е�time stamp������֡���ݵĵ�һ���ֽ�[��λ��]
*     VideoVPP:   ͨ�� RunFrameVPPAsync �����첽 VPP?  
*   2.mfxHandleType/mfxHDL -- �ƺ��ǿ����Ƿ�ʹ��Ӳ������Ӳ�����͵Ķ���?
*   3.mfxSyncPoint -- 
*   4.mfxBitstream -- ������?
**************************************************************************************************/




#endif //INTEL_MEDIA_SDK