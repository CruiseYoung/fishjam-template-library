#pragma once

������Ĵ��룺
CMediaType VideoType; //Strmbase.lib
VideoType.SetType( &MEDIATYPE_Video);
VideoType.SetSubtype(&MEDIASUBTYPE_RGB24);
hr = m_pGrabber->SetMediaType( &VideoType ); // shouldn't fail
hr = m_pCaptureBuilder2->SetOutputFileName(&MEDIASUBTYPE_Avi, L"C:\\test.avi", &pMux, &pSink); 
hr = m_pCaptureBuilder2->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Interleaved, pSmart, NULL, pMux);
                                                             &MEDIATYPE_Video,
/*****************************************************************************************************
* g_hInst -- Filter DLL�ľ���������ڼ���DLL�е���Դ
* NAME -- ����Filter��Pin�ȵ����֣������汾ʱ���Զ�����ΪNULL(��֤��й��)
*
* IMediaFilter
* +-CBaseMediaFilter //�������
* +-IBaseFilter
*   +-CBaseFilter    
*     +-CSource
*     +-CTransformFilter
*       +-CVideoTransformFilter
*       +-CTransInPlaceFilter
*     +-CBaseRenderer  //��֧����������
*       +-CBaseVideoRenderer //֧����������
* 
* 1.������ʽ��
*   ��Filter���϶��徲̬�� CreateInstance ����
*     static CUnknown * WINAPI CreateInstance(LPUNKNOWN punk, HRESULT *pHr);
*   ��ʵ���ļ��ж�������Ҫʹ�õ�ȫ�ֱ����������а��� Filter �� Pin �������Ϣ�ʹ�������
*     CFactoryTemplate g_Templates[] = {L"FilterNameXXX",&CLSID_FILTER_XXXX,CXXXX::CreateInstance,
*       NULL, &amSetupFilter}};  //���� amSetupFilter ���� AMOVIESETUP_FILTER ���͵ı���
*     int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);
* 
*
* CBaseMediaFilter
*   �����������ڹ���� m_pLock �ٽ���ָ�루�ɹ��캯�����룩
*
* CBaseFilter��ע�ⲻ�Ǵ�CBaseMediaFilter�̳У�������Ӧ�ü̳У�
*   1.��Ҫ˵��
*   2.�������ص�(��)�麯��
*     a.GetPinCount / GetPin <== ����Pin����Ϣ
*   3.��ѡ�����غ���
*     a.Run(REFERENCE_TIME tStart)/Pause/Stop -- ״̬�л�ʱ�ĺ���������ʱ��Ҫע��:
*       1).CAutoLock cObjectLock(m_pLock); -- ����������
*       2).���û���ķ�����ɻ���Ĺ��ܣ�
*       3).״̬Ǩ��Ϊ��Stopped -> Paused(Active) -> Running -> Paused(Inactive) -> Stopped
*          ״̬ת��ʱ������ѭ���������ϡ���˳��(Render->Transform->Source)��������ܶ�ʧSample������
*   4.���о�̬�������������ʣ�static CUnknown * WINAPI CreateInstance(LPUNKNOWN lpunk, HRESULT *phr);
*     ������ʹ�� new ˽�еĹ��캯����������Filter
*   5.���õķ���
*     StreamTime -- ��Filter�ϻ�ȡ��ǰ��stream time������ǰ��Ҫ�� CAutoLock cObjectLock(m_pLock);
* 
* SourceFilter -- 
*   1.��Ҫ˵��
*     �� CSource �̳�(LiveFilter����֧��Seeking)
*     ��Ҫ�������Pinָ�����飨m_paStreams��-- ������Ĺ��캯���д������ɻ��ฺ���ͷ�
*       m_paStreams = (CSourceStream **) new CBallStream*[1];
*       m_paStreams[0] = new CBallStream(phr, this, L"A Bouncing Ball!");
*   2.�������ص�(��)�麯��
*   3.��ѡ�����غ���
*   4.�ᱻQI�Ľӿ�(�� IBaseFilter������)
*     IReferenceClock --
*     IFileSourceFilter
*     IFileSinkFilter
*     IMediaPosition
*     IAMFilterMiscFlags
*     IVideoWindow
*     IBasicAudio
*     IAMDeviceRemoval
*     IAMOpenProgress
* 
* CTransformFilter
*   1.��Ҫ˵�� -- ʹ�õ�������������뻺����(copy-transform filter)������һ����������󣬸�д�µ����ݵ�һ���������
*     ���Ҵ��ݵ���һ��filter���б����ĳ�Ա���� m_pInput��m_pOutputָ�����롢���Pin������ж��PinӦ����Σ�����
*     Ĭ��ʵ����GetPin��������������ھͻᴴ���������Ҫʹ���Լ���Pin�������ڹ����GetPin�д�������ֵ�� m_pInput/m_pOuput��
*   2.�������ص�(��)�麯��
*     Transform(IMediaSample * pIn, IMediaSample *pOut);   //����ý���ת�����ɿ�ܵ��߳�ѭ�����ã��緵��S_FALSE�����һ֡��
*        ��Receive�������ڲ�ͬ���������������filter�������κι����߳��������������(ͨ��TranformFilter����ͬ����?)
*     CheckInputType(const CMediaType* mtIn); //����Ƿ�֧��mtIn������
*     CheckTransform(const CMediaType* mtIn, const CMediaType* mtOut); //����Ƿ�֧�ִ� mtIn ת��Ϊ mtOut
*     DecideBufferSize(IMemAllocator * pAllocator,ALLOCATOR_PROPERTIES *pprop);//����SetProperties�����ʵ��Ĳ���
*       ���Ȼ�ȡ����Pin�ϵķ�����(m_pInput->GetAllocator)�������л�ȡ�Ѿ����úõĲ���
*     // GetMediaType(int iPosition, CMediaType *pMediaType);// �������Pin��֧�ֵĸ�ʽ
*   3.��ѡ�����غ���
*     SetMediaType(PIN_DIRECTION direction,const CMediaType *pmt); //�����ӳ��Գɹ�ʱ���������Pin��
*       �Ƿ�֧�����Pin�Ѿ�ȷ����ý�����ͣ�˵���ʹ����Ƿ��ˣ����������֧�֣���Ҫ�����������µ�ʾ�����룺
*       CTransformFilter::SetMediaType(direction, pmt); //���û���ķ����������
*       if( m_pInput->IsConnected() && m_pOutput->IsConnected() ) //�������Pin�����Pin�������ϵ�
*       {
*         if (direction == PINDIR_OUTPUT && *pmt != m_pInput->CurrentMediaType())
*         {
*            FILTER_INFO fInfo;
*            QueryFilterInfo( &fInfo );           //��ȡFilterInfo����Ҫ�� IFilterGraph �ӿ�
*            fInfo.pGraph->Reconnect( m_pInput ); //��������
*            QueryFilterInfoReleaseGraph( fInfo );//�ͷŻ�ȡ�� IFilterGraph �ӿ�
*         }
*       }
*     Receive(IMediaSample *pSample); //
*     StartStreaming/StopStreaming  -- ����Ӧ���߼��ĳ�ʼ������ֹ��
*     
* CVieoTransformFilter
*   1.��Ҫ˵��
*     ��ҪΪ��Ƶ��������Ƶģ�������downstram renderer�ڽ�����ʧ�ܽ���֡�Ĺ��ܵ�
*  
* CTransInPlaceFilter
*   1.��Ҫ˵��
*     Ϊ�޸���ԭʼ�������е����ݶ���Ƶ�filter������һ�����������ڲ��ı�������ݲ��Ҵ��ݸò�������һ����
*     ���롢���Pinʹ����ͬ��ý�����͡�ͨ����Ҫ��Pin˵��ΪFilter����Ԫ��
*     ���Filter����������(FALSE==m_bModifiesData)�����롢���Pin��ʹ����ͬ��Allocator
*   2.�������ص�(��)�麯��
*     Transform(IMediaSample *pSample);     //�͵ش�������(�����޸�)
*     CheckInputType(const CMediaType *mtIn);   //����Ƿ�֧��mtIn������
*   3.��ѡ�����غ���
*   4.ע��
*     ��Renderʱ�������������� -- ��Ҫ�ڻ������ϵ�read-modify-write���������ڴ�λ���Դ��ϣ����ǳ���
*
* CBaseRenderer
*   1.��Ҫ˵��
*     �򵥵�Renderer���࣬��������Pin
*   2.�������ص�(��)�麯��
*     DoRenderSample(IMediaSample *pMediaSample);
*     CheckMediaType(const CMediaType *pmt);
*   3.��ѡ�����غ���
*
* Video Render
*   1.Video Render(��ͳ��,Merit=0x00400000) -- ��ʾʱ���Ǵ���һ����Ƶ���ڣ���Ҫ��ʾ��ָ����UI�����У�
*     ��Ҫ�޸���Ƶ���ڵķ�񣬲�����ָ��ΪUI���ڵ��Ӵ��ڡ�
*   2.Overlay Mixer -- 
*   VMR(Video Mixing Renderer) -- ͨ��DirectXʹ�����Կ���ͼ�δ�������(��Ƶ�ϳɺ���ʾʱ��ռ��CPU),
*     ��Щ�Կ�������Ϊ24λɫ������²���ִ��Direct3D������
*     3.VMR-7 -- ʹ��DirectDraw7����������WinXP��(��)��ã�WinXP��Ĭ����Ƶ��ʾRenderer(Merit=0x00800001)��
*       ʵ�����������޴���ģʽ��ʾ
*     4.VMR-9 -- ʹ��Direct3D 9���������κΰ�װ��DirectX9�Ĳ���ϵͳ�϶���ʹ�ã�����Ĭ�ϵ�Renderer
*       (Merit=0x00200000)����֧��Video Port(��Ƶ�������ں�ģʽ��ֱ�Ӵ��͵��Կ����Դ�)��ʹ��
* 
* Muxer Filter -- һ��Muxer�����ӣ������п����õ� BaseMux��svn://dev.monogram.sk/public/libmonodshow/trunk
*   http://blog.monogram.sk/janos/2008/08/23/how-to-make-a-directshow-muxer-filter-part-1/
*   �������Pin��һ��Muxing Thread�����ӵ�����Pin�� Interleaver unit��Queue �л�ȡ���ݣ�������䵽���Pin��
*   ���Pin����ʵ��IStream�ӿ�(���ڷ�������ļ�ʱ�����漴����)
* 
*   ʵ��ʱ���ǣ�Pause/Resume �� -- ��δȷ��
*   ǰ�᣺Muxer Filter���յ�Sampleֻ��Audio�����˾���ʱ�����Video������Ե�  -- ���Ժ���ԣ�
*   1.Video Sample�ȵ���Muxer��û����Audio�еõ���ȷ��ƫ�ƣ�
*   2.Audio Sample�ȵ���Muxer����Resume���һ֡��Video Sample��ʱ������е��������򲥷Ÿ��ļ�ʱ�����ĳ������ס������
*
* Splitter Filter
*   
* 
* Filter����ʵ�ֵ�ϵͳ�ӿڣ�����GE��ϵͳ�����QI�Ľӿڣ�
*   #.IAMDeviceRemoval
*   #.IAMFilterMiscFlags
*   #.IAMOpenProgress
*   #.IBaseFilter
*   #.IBasicAudio
*   #.IBasicVideo
*   #.IFileSinkFilter  -- д�ļ���GE����Ҫѡ�񱣴��ļ�
*   #.IFileSinkFilter2
*   #.IFileSourceFilter -- ���ļ���GE����Ҫѡ�������ļ�
*   #.IKsPropertySet
*   #.IMediaPosition
*   #.IMediaSeeking
*   #.IPersistPropertyBag
*   #.IReferenceClock
*   #.ISpecifyPropertyPages
*   #.IVideoWindow
*
* ϵͳ�ṩ����ֱ��ʹ�õ� Filter��һ�����ͨ�� AddFilterByCLSID ���� GraphBuilder 
*   wmcodecdsp.h ͷ�ļ��а�����ϵͳĬ�Ͽ��õı������, ��Ӧ�� CLSID_XXX ����ͨ�� GraphEdt �ҵ��� UuidFromString �õ�
*     UuidFromString(TEXT("205769BC-B121-4CA8-A4E6-84A20EF253B7"), &CLSID_DebugInfoFilter);
*
*   CLSID_AviDest(AVI Mux Filter) -- ���ն����������ת��ΪAVI��ʽ�����������Pin
*   CLSID_AviSplitter -- ���ļ���Դ��������File Source(Async)�������ݣ�Ȼ��ֽ⵽��Ƶ����Ƶ����
*   CLSID_CMP3DecMediaObject -- Windows Media MP3 Decoder ?
*   CLSID_CMPEG2AudioEncoderMFT -- MPEG2 Audio Encoder ?
*     http://msdn.microsoft.com/en-us/library/windows/desktop/hh162908(v=VS.85).aspx
*   CLSID_Colour -- Colour space convertor, RGB��ɫ�ռ�ת��?
*   CLSID_FileWriter -- �ļ����������ʵ���� IFileSinkFilter
*   CLSID_MP3ACMCodecWrapper -- MS��MP3������,Win8��֧��
*     http://msdn.microsoft.com/en-us/library/windows/desktop/hh162907(v=vs.85).aspx
*   CLSID_SampleGrabber 
*   CLSID_SmartTee -- ��·Filter��һ�����ڶ�·ͬʱ������� Ԥ�� + �ļ�����
*   CLSID_VideoRenderer -- ��Ƶ������ƺ�ѡ���?
*   CLSID_VideoRendererDefault -- TODO: ���������� 
*   CLSID_Colour -- Colour space convertor, ��ɫ�ؼ�ת��?(������֧������ҳ?)
******************************************************************************************************/
//Render uses Direct3D or DirectDraw for rendering samples

