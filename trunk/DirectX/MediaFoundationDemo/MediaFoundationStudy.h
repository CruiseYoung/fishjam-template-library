#pragma once
//Media Foundation ���� http://hi.baidu.com/neil_danky/item/79b5b15f5a735111da1635af

//�����ڴ���ִ���ʱ������������Ϣ��Ȼ�����
HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

//���е� mfh264enc.dll ���Խ��� H264 ����, mfAACEnc.dll ���� AAC ����?
/*************************************************************************************************
* Media Foundation -- ΢�����Ƴ���һ���Ļ���COM�Ķ�ý��Ӧ��ƽ̨��������� DirectShow, Windows Media SDK,DMO �ȣ�
*   ��Vista֮����ΪDirectShow����������룬��Ҫ .NET4��֧�� ??
* 
* ��� DirectShow �����ƣ�΢���鿪����Ա��д MFTs��ȡ�� DirectShow Transforms��
*    1.����չ�ĸ����������ݺ����ְ�Ȩ��������DRM-protected�������� -- ý�����ݱ���
*    2.����DirectX��Ƶ��������֮��DirectShow�Ļ�����ʩ��֧��DXVA 2.0, Win7��֧�ָ����ʽ����Ƶ�����豸��Ӳ�������ȡ�
*    3.MF�Ŀ���չ�ԣ�extensibility����ʹ��ͬ�����ݱ���ϵͳһ������
*    4.����ʹ�ö�ý�����ͼƻ��������MMCSS��
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
*
* ʹ��Ӳ��
*   Ӳ���豸��Media Foundation��չ��ʱʹ�ô�������ڹܵ���ʹ����������ʹ�û�����������һ��
*
*
* �ͼ�API�ӿ�(mfapi.h + mfidl.h)
*   
* �߼�API�ӿ�
*   MFPlay -- ������Ƶ����Ƶ�Ĳ��š�����֧�ֵĵ��Ͳ��Ų���(ֹͣ����ͣ�����ң����ۿؼ�����������ǰ)
*   Source Reader -- ��ý���ļ���ȡ��ԭʼ���ݻ��������(�����Ƶ�ļ��л�ȡ������ͼ�������������л�ȡ��ʵʱ����Ƶ֡)
*   Sink Writer -- ͨ��һЩδ��ѹ������������������ý���ļ�(��������±������һ����Ƶ�ļ�)
*   Transcode -- ��ɳ���������Ƶת������
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