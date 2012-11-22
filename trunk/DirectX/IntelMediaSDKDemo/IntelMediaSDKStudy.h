#ifndef INTEL_MEDIA_SDK
#define INTEL_MEDIA_SDK
#pragma once
IMemAllocator
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
*   1.Intel Media SDK �ƺ�ֻ֧��NV12(Ӣ�ض��������Ƶ��ʽ������Ӣ�ض�Ӳ��ƽ̨����ԭ��̬֧��)
*     MSDK��ʽת��: http://software.intel.com/zh-cn/blogs/2010/07/26/msdkyv12nv12/
*                   http://software.intel.com/en-us/articles/using-unsupported-color-formats-with-the-intel-media-software-development-kit/
*   2.��Filter �� ALLOCATOR_PROPERTIES Ҫ��Ƚϸߣ����� cBuffers һ�㶼Ҫ10����(Ϊ���첽ʱ�ܶദ����)��
*     cbBuffer һ�㶼Ҫ�������������ֵ�󣬷���͹����� CheckInput��
*     �����ƺ�Sample��Enc�л��Զ����ĸ߿�(32�ֽڶ��룬���� 320x240=> 320x256)
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
*
* �����չ
*   ͨ�� mfxPlugin(C++��MFXPlugin)���û��ṩ��չ�Ļ���,Sample ���ṩ�� CSCPlugin(�� RGB32 => NV12 ת��)
*   
**************************************************************************************************/

/**************************************************************************************************
* ʹ������� MFX_TIME_STAMP, ͨ�� ConvertReferenceTime2MFXTime / xxxx ת����
*   ������ MFX_TIME_STAMP_FREQUENCY(90000) ����
*   
* CSCProcessor -- Color Space Converter - converts frames from RGB4 to NV12,
* 
* Դ�����
*   1.mfxSession -- MediaSDK�ĺ���,ͨ�� MFXInit��ʼ��,ʹ����Ϻ�ͨ��MFXClose�ر�,��ͨ��MFXVideoSession�������
*     �亯����Ϊ���¼������֣������ֱ���װ�ɶ�Ӧ�� MFXVideoXXX ����м򻯴���
*     ��������:   �� QueryXXX, ��Session�������ȼ�(mfxPriority)����
*     VideoCore:  ���� BufferAllocator��FrameAllocator; ���û��ȡ Handle(?)��ͬ������(SyncPoint) ��
*     VideoEncode:����Encode�Ĳ�����ͨ�� mfxVideoParam ������ز�����ͨ�� EncodeFrameAsync �����첽���룿
*     VideoDecode:����Decode�Ĳ�����ͨ�� DecodeHeader �Ƚ���ͷ����ͨ�� DecodeFrameAsync �����첽���룿
*       �������֧�� ����֡����֡���֡���������е�time stamp������֡���ݵĵ�һ���ֽ�[��λ��]
*     VideoVPP:   ͨ�� RunFrameVPPAsync �����첽 VPP���᷵��һ��ͬ����  
*   n.mfxBitstream -- ������?
*   n.mfxHandleType/mfxHDL -- �ƺ��ǿ����Ƿ�ʹ��Ӳ������Ӳ������(D3D9,D3D11,VA_DISPLAY)�Ķ���?
*     mfxIMPL ��Ӧ�� MFX_IMPL_XXX ö��ֵ
*   n.mfxInfoMFX -- �Ǻܶ����ñ������Ϣ�Ľṹ��(���а����� H264��MPEG-2��JPEG���õ�union)
*       CodecId -- ������ID
*       CodecProfile -- MFX_PROFILE_AVC_STEREO_HIGH | xxx;
*       TargetUsage --
*       RateControlMethod -- MFX_RATECONTROL_CBR|
*       mfxU32  reserved[3];
*   n.mfxInfoVPP -- ���ڲ����������� mfxFrameInfo �ṹ�壬�ֱ��ʾ����������ý��֡��Ϣ��
*   n.mfxFrameInfo -- ý���֡��Ϣ(��������DS�е�Format?)
*       AspectRatioW/AspectRatioH -- ����?
*       ChromaFormat -- MFX_CHROMAFORMAT_YUV420
*       FourCC -- MFX_FOURCC_NV12
*       FrameRateExtN/FrameRateExtD -- ֡�ʵķ�����ʾ������ͨ��SDK���ṩ�� ConvertFrameRate ��������
*       Height/Width -- MSDK_ALIGN16 �� MSDK_ALIGN32 �� ������ĸ߶ȺͿ��
*       PicStruct -- ?
*   n.mfxSyncPoint -- ͬ���㣬��һ��ָ�룬
*   n.mfxVideoParam
*       IOPattern -- ָ���ڴ�ģ�ͣ�
*         ������D3D�Ļ���һ���ǣ�MFX_IOPATTERN_IN_VIDEO_MEMORY|MFX_IOPATTERN_OUT_VIDEO_MEMORY;
*         û������D3D�Ļ���һ���ǣ�MFX_IOPATTERN_IN_SYSTEM_MEMORY|MFX_IOPATTERN_OUT_SYSTEM_MEMORY;
*
* Pipeline
*   Init{
*     FileRead::Init -> InitFileWrite -> Session::init -> CreateAllocator -> InitMfxEncParams 
*     -> InitMfxVppParams -> new  MFXVideoENCODE -> [new MFXVideoVPP(�����Ҫ���Ĵ�С��FourCC) -> ] 
*     -> ResetMFXComponents { MFXVideoENCODE::Init -> MFXVideoVPP::Init -> CEncTaskPool::Init }
*   }
*   for(...) 
*   {
*     Run{
*       GetFreeTask -> GetFreeSurface(����Ŀ���surface) -> FileReader::LoadNextFrame(�����ļ�����)
*       -> MFXVideoVPP::RunFrameVPPAsync(&m_pVppSurfaces[nVppSurfIdx],����VPP����) -> MFXVideoENCODE::EncodeFrameAsync(����)
*       -> MFXVideoVPP::RunFrameVPPAsync(NULL,�ٴε���VPP����) -> MFXVideoENCODE::EncodeFrameAsync(�ٴα���)
*     }
*   }
*   Close{
*     DeleteFrames -> DeleteAllocator -> Close TaskPool/Session/FileReader
*   }
**************************************************************************************************/




#endif //INTEL_MEDIA_SDK