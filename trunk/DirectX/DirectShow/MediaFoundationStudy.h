#pragma once

//�����ڴ���ִ���ʱ������������Ϣ��Ȼ�����
HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

//���е� mfh264enc.dll ���Խ��� H264 ����, mfAACEnc.dll ���� AAC ����?
/*************************************************************************************************
* Media Foundation -- ΢�����Ƴ���һ���Ļ���COM�Ķ�ý��Ӧ��ƽ̨��������� DirectShow, Windows Media SDK,DMO �ȣ�
*   ?? ��Vista֮�����ʹ�ã���Ҫ .NET4��֧�� ??
* 
* ��� DirectShow �����ƣ�΢���鿪����Ա��д MFTs��ȡ�� DirectShow Transforms��
*    1.����չ�ĸ����������ݺ����ְ�Ȩ��������DRM-protected��������
*    2.����DirectX��Ƶ��������֮��DirectShow�Ļ�����ʩ��֧��DXVA 2.0
*    3.MF�Ŀ���չ�ԣ�extensibility����ʹ��ͬ�����ݱ���ϵͳһ������
*    4.����ʹ�ö�ý�����ͼƻ��������MMCSS��
*    
* 
* MF�ܹ���
*   ���Ʋ�(Control layer)
*     Media Session
*     Topology
*     Sequencer
*   ���Ĳ�(Core layer)��Ҳ��Ϊ ý��ܵ�(pipeline)?
*     ��������ɲ��֣�
*       a.ý����Դ(Media Source)
*       b.ý�������(Media Sink)
*       c.ý��ƽ̨�任(Media Foundation Transforms -- MFT)
*   ƽ̨��(Platform layer)
*     Media Streams
*     Presentation Clock
*
* MFT -- Ӧ�ó������ʹ��MFTs�ڵĹܵ�����ֱ��ʹ��������Ϊ�����Ķ�������������:
*   ��Ƶ����Ƶ�������
*   ��Ƶ����ƵЧ��
*   Multiplexers �� demultiplexers
*   ɫ�ʿռ�ת����
*************************************************************************************************/

/************************************************************************************************
* ���� -- ����Ҫ���ƺ��� IMFMediaSession��IMFTopology��IMFTranscodeProfile��IMFMediaEvent �ȼ����ӿ�?
*   #include <mfapi.h> �� <mfidl.h>
*   CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
*   MFStartup(MF_VERSION);
*   {
*     1.�õ��ļ���Ӧ�� IMFMediaSource
*       MFCreateSourceResolver -> IMFSourceResolver::CreateObjectFromURL -> QueryInterface �õ� IMFMediaSource
*     2.ͨ�� MFCreateMediaSession �� MFCreateTranscodeProfile �õ��յ� Session �� Profile
*       
*     
*   }
*   MFShutdown();
*************************************************************************************************/