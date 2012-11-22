#pragma once
//Media Foundation ���� http://hi.baidu.com/neil_danky/item/79b5b15f5a735111da1635af
//http://blogs.msdn.com/b/mf/archive/2009/12/02/mfsimpleencode.aspx
//mfcopy http://blogs.msdn.com/b/mf/archive/2009/12/16/mfcopy.aspx

//�����ڴ���ִ���ʱ������������Ϣ��Ȼ�����
HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

//���е� mfh264enc.dll ���Խ��� H264 ����, mfAACEnc.dll ���� AAC ����?

//Ӳ�������: MF_READWRITE_ENABLE_HARDWARE_TRANSFORMS

/*************************************************************************************************
* ����
*   1.Sample ���� TopoEdit ����(���� GraphEdt ����)
*   2.MFTrace.exe  MyProgram.exe >> trace.txt
*     http://social.msdn.microsoft.com/Forums/zh-SG/mediafoundationdevelopment/thread/137b2208-8601-4633-88f9-30cc79cb7794
*   3.ParseTopologies.cmd  -- ��same url as up
*   4.TextAnalysisTool.NET
*     http://blogs.msdn.com/b/mf/archive/2010/09/09/analyzing-media-foundation-traces.aspx
*
*************************************************************************************************/


/*************************************************************************************************
*
* 
* Media Foundation -- ΢�����Ƴ���һ���Ļ���COM�Ķ�ý��Ӧ��ƽ̨��������� DirectShow, Windows Media SDK,DMO �ȣ�
*   ��Vista֮����ΪDirectShow����������룬��Ҫ .NET4��֧�� ??
* 
* ��� DirectShow �����ƣ�΢���鿪����Ա��д MFTs��ȡ�� DirectShow Transforms��
*    1.����չ�ĸ����������ݺ����ְ�Ȩ��������DRM-protected�������� -- ý�����ݱ���
*    2.����DirectX��Ƶ��������֮��DirectShow�Ļ�����ʩ��֧��DXVA 2.0 , Win7��֧�ָ����ʽ����Ƶ�����豸��Ӳ�������ȡ�
*    3.MF�Ŀ���չ�ԣ�extensibility����ʹ��ͬ�����ݱ���ϵͳһ������
*    4.����ʹ�ö�ý�����ͼƻ��������MMCSS��
* 
* MF�ܹ���
*   ���Ʋ�(Control layer)
*     Media Session
*     Topology
*     Sequencer
*   ���Ĳ�(Core layer)��Ҳ��Ϊ ý��ܵ�(pipeline,�첽����ý������)
*     ��������ɲ��֣�
*       a.ý����Դ(Media Source)
*       b.ý��ƽ̨�任(Media Foundation Transforms -- MFT)
*       c.ý�������(Media Sink) -- ��Ϊ renderer(��ʾ�����) �� archive(����) ����
*   ƽ̨��(Platform layer)
*     Media Streams
*     Presentation Clock
*
* MFT -- ʵ��IMFTransform��COM�������DMO��������MFT CLSID������ wmcodecdsp.h ��
*   Ӧ�ó��������pipeline��ʹ�ã���ֱ��ʹ��������Ϊ�����Ķ���
*   ����������(TopoEdit.exe -> Topology -> Add Transform �ɲ鿴):
*     ��Ƶ����Ƶ�������, �� "H264 Encoder MFT"
*     ��Ƶ����ƵЧ��
*     Multiplexers �� demultiplexers
*     ɫ�ʿռ�ת����
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
*     2.ͨ�� MFCreateMediaSession �� MFCreateTranscodeProfile �õ��յ� Session �� Profile�������ú��ʵ�ֵ
*     3.���� Encoding Session ���б���
*     
*   }
*   MFShutdown();
*************************************************************************************************/

//IMFAttributes *pMFAttributes = NULL;
//hr_SW = MFCreateAttributes( &pMFAttributes, 100 );
//hr_SW = pMFAttributes->SetGUID( MF_TRANSCODE_CONTAINERTYPE, MFTranscodeContainerType_MPEG4 );//video
//hr_SW = pMFAttributes->SetUINT32( MF_READWRITE_ENABLE_HARDWARE_TRANSFORMS, true ); //==> for hardware encode
//hr_SW = pMFAttributes->SetUINT32( MF_READWRITE_DISABLE_CONVERTERS , false );
  //==> for hardware encode, msdn says if I would like set MF_READWRITE_ENABLE_HARDWARE_TRANSFORMS true, this parameter should be false.

//hr_SW = MFCreateFile( MF_ACCESSMODE_READWRITE, MF_OPENMODE_DELETE_IF_EXIST, MF_FILEFLAGS_NONE, L"c:\\xxx.mp4", &pStream );
//hr_SW = MFCreateSinkWriterFromURL( NULL, pStream, pMFAttributes, &pSinkWriter ); // ==> Here, I got wrong return value.

//hr = MFTRegisterLocalByCLSID( CLSID_MF_H264EncFilter, MFT_CATEGORY_VIDEO_ENCODER, L"Intel\xae Media SDK H.264 Encoder MFT", NULL, 0, NULL, 0, NULL);