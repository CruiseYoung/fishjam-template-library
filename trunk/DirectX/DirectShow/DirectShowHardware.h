#ifndef DIRECT_SHOW_HARDWARE_H
#define DIRECT_SHOW_HARDWARE_H
#pragma once

//ע�⣺һЩ��������Filter����Debug��ö��ʱ�׳��쳣(int 3)����ֹ�ƽ�
//  
/*********************************************************************************************
* �ں���
*    WDM�豸֧���ں��������ں������������ں�ģʽ�±�������������Զ����Ҫ�л����û�ģʽ��ȥ��
*    �Ӷ����������ں�ģʽ���û�ģʽ֮���л��ľ޴������ں�������ߵı����ʶ�������CPU��ʱ�䡣
*    ����:Ӳ���ɼ���Ƶ -> Ӳ��ѹ�� -> D3DӲ��������ʾ (�������е����ݲ��ᴫ�뵽�û�ģʽ)
*
* �Լ�������Filter������ DllRegisterServer ����ʵ����ͨ�� CLSID_FilterMapper2 �� 
*   IFilterMapper2::RegisterFilter ע�ᵽϵͳ��Category��
*   ��Ҫ���� REGFILTER2 rf2FilterReg ����
*
* 
* DirectShow ͨ����Ӳ���ṩWrapperFilter(CLSID_AVICo/CLSID_ACMWrapper ��)��ʹ���û�ģʽ(Ring3)�µ�Ӧ�ó����ܲ����ں�ģʽ(Ring0)��Ӳ����
*   DirectShow�ṩһ����KsProxy��filter��������ʵ���κ����͵�WDM������,Ӳ����ͨ���ṩһ��Ksproxy plug-in(���ۺϵ�COM����
*     ͨ�� IKsPropertySet �ӿ���֧���������Զ�������)����չKsProxy,��ʹ��֧���Լ��Ĺ���
*
* CLSID_CaptureGraphBuilder2 -- ��������Ƶ�ɼ�,���ICaptureGraphBuilder2�ӿڣ�ʹ��SetFilterGraphg������IGraphBuilder��.
*   1.ʹ�� SetOutputFileName ��������ļ���·�������Եõ� multiplexer �� FileWriter ��Filter
*     pCapture->SetOutputFileName(&MEDIASUBTYPE_Avi, strFileName, &pOutMutex, &pOutSink);
*   2.����ʹ��RenderStream��ָ�����͵�Pin��ɺ�������(�� PIN_CATEGORY_PREVIEW ָ�� Preview PIn�� 
*       _CAPTURE ָ������ Codec ָ���������� _CC��_VBI ָ��CC );
*     g_pCapture->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, pSrcCaptureFilter, pVideoCodecFilter, pMultiplexer);
*   3.���ܹ���PreviewPin��
*     a.��VideoPort Pin ʱ������ Overlay Mixer -> Video Renderer
*     b.��PreviewPinʱ��ֱ������ Video Renderer��
*     c.ֻ��Capture Pin ʱ���Զ�����һ��SmartTee Filter ���з�·��Ȼ�����ӵ� Video Renderer.
*   4.�Զ�����ɼ���Filter֮ǰ�ĸ���Filter(������ѡ����Ƶ������ӵ�Analog Video Crossbar Filter),
*     ��������ӡ���ЩFilter֮������Ӳ���ͨ��ý����������ʶ�ģ�����ͨ��ý��(Medium)��GUID����ʶ�ġ�
*   5.������������FindPin��FindInterface(ͨ�� IKsPropertySet::QuerySupported ?�鿴��Ҳ����˵ Filter ��Ҫ֧������ӿڣ� )
*        ����Filter���Ƿ�֧��CC��Pin
*          FindPin(pVideoCap, PINDIR_OUTPUT, 
*            &PIN_CATEGORY_CC,   // &PIN_CATEGORY_VBI
*            NULL, FALSE, 0, &pPin);
*        ����CapFilter�ϵ�ѹ���ӿ�(IAMVideoCompression, ͨ��Ӳ�� WDM TVTuners ���ṩ?) �� ���ýӿ�(IAMStreamConfig)
*        FindInterface(&PIN_CATEGORY_CAPTURE,
*          &MEDIATYPE_Interleaved,  //&MEDIATYPE_Video,
*          pVideoCap,
*          IID_IAMVideoCompression, (void **).pVideoCompress); // IID_IAMStreamConfig, IID_IAMDroppedFrames
*        ���ø÷���֮����Զ�������ص�Filer ?
*   6.��ʼ����(���ַ�ʽ��ControlStream �� IMediaControl::Run )
*        g_pCapture->ControlStream(&PIN_CATEGORY_CAPTURE, NULL, NULL, NULL, &stop, 0, 0);
*   7.�Զ����� EC_REPAINT ���⣨IMediaEvent::CancelDefaultHandling(EC_REPAINT)����ֹд�ļ��쳣��
*     
* KsProxy.ax -- �������ʹ��WDM(Windows Drive Mode)����������Ƶ�豸������USB������ͷͨ��WDM Video Capture Filter��װ��
* 
* �ڳ����п���ͨ��ϵͳö������ö��ϵͳ�а�װ������Ӳ����������Щ����Ӳ����Filter��Ҳ��Ҫͨ��ö��(����ֱ�Ӵ���)��
* ö�ٹ��� [ HKEY_CURRENT_USER\Software\Microsoft\ActiveMovie\devenum ]
*   1.����ϵͳö�����������(CLSID_SystemDeviceEnum),����� ICreateDevEnum �ӿڣ��緵��S_FALSE��ʾ����������Ϊ��(�򲻴���?)
*   2.ʹ���� CreateClassEnumerator ������Ϊָ����Filterע������Ŀ¼����һ��ö���������IEnumMoniker�ӿڣ�
*     ������ע������Ŀ¼�кܶ࣬�磺
*     CLSID_ActiveMovieCategories -- ����ע�����͵�Ŀ¼(GraphEdt��������)
*     CLSID_LegacyAmFilterCategory -- ����ע���DirectShowFilter
*     CLSID_AudioCompressorCategory -- ��Ƶѹ��
*     CLSID_AudioInputDeviceCategory -- ��Ƶ��׽�豸
*     CLSID_AudioRendererCategory -- ��Ƶ���
*     CLSID_LegacyAmFilterCategory -- DirectShow Filters
*     CLSID_VideoCompressorCategory -- ����ѹ��
*     CLSID_VideoInputDeviceCategory -- ������׽�豸
*   3.ʹ��IEnumMoniker::Next �ӿڷ���ö���豸��ʶ(Device Moniker)�����IMoniker�ӿڣ���һ���õ��Ľ��ǿ�������е���ѡ�豸��
*   4.ʹ�� IMoniker::BindToStorage(�� NULL,NULL,IID_IPropertyBag,&pPropertyBag)��
*     ���Է����豸��ʶ�����Լ�(�� L"FriendlyName", L"CLSID", L"FilterData", ��); IMoniker::GetDisplayName
*   5.ʹ�� IMoniker::BindToObject,���Խ��豸��ʶ����һ��IBaseFilter��Ȼ�����FilterGraph;
*     5.1.Ҳ����ͨ��IFilterGraph2::AddSourceFilterForMoniker ֱ�Ӵ���Filter������FilterGraph
*   ���Ӵ���
*     CComPtr<ICreateDevEnum> pSysDevEnum;
*     CoCreateInstance(CLSID_SystemDeviceEnum,NULL,CLSCTX_INPROC,IID_ICreateDevEnum,(void**)pSysDevEnum);
*     CComPtr<IEnumMoniker> pEnumCat;
*     pSysDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,&pEnumCat,0);
*     pEnumCat->Reset();
*     CComPtr<IMoniker> pMoniker;
*     while (pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK) //���һ��(���)�᷵�� S_FALSE
*     {
*       CComPtr<IPropertyBag> pPropBag;
*       pMoniker->BindToStorage(NULL,NULL,IID_IPropertyBag,(void**)&pPropBag);
*       if(SUCCEEDED(hr){
*         VARIANT varName; 
*         VariantInit(&varName); //varName.vt = VT_BSTR;
*         pPropBag->Read(L"FriendlyName",&varName,NULL); //��� FriendlyName ���� varName.bstrVal
*         VariantClear(&varName);
*       }
*       CComPtr<IBaseFilter> pFilter;
*       pMoniker->BindToObject(NULL,NULL,IID_IBaseFilter,(void**)&pFilter); //
*       pMoniker.Release();
*     }
*********************************************************************************************/

/*********************************************************************************************
* ��Ƶ�ɼ� -- Audio Capture Filter,�ڲ�ʹ�� waveInXXX �ĺ�����
*   �� CLSID_AudioInputDeviceCategory ö�٣�����ͨ�� InfTee ��������ָ������Ԥ���ͱ���������,
*     ��ʹ��ICaptureGraphBuilder2::RenderStream(&PIN_CATEGORY_PREVIEW,...)�������Ҫ�Զ�����һ��SmartTeeFilter
*   ����Pin��Filter��ʹ�� IAMAudioInputMixer �ӿڽ��в������ã����������ߵ�������
*   ���Pin�Ͽ���ʹ�� IAMStreamConfig �ӿڻ�ȡ������ý����Ϣ(�� ����Ƶ�ʡ���������)
*   ͨ�����Pin�ϵ� IAMBufferNegotiation �ӿ�������Ƶ�ɼ�����Ĵ�С���Լ����������ӳ١�
*
*********************************************************************************************/

/*********************************************************************************************
* ��Ƶ�ɼ� -- ��Ƶ�ɼ��������ӽ��տ�������ͷ��¼���(VTR)��
*   ͨ����CapturePin(д�ļ�,MediaSample����ʱ���)��PreviewPin(Ԥ�������Զ���֡������ʱ���)
*     ����ͨ�� ICaptureGraphBuilder2::FindInterface ���Եõ�
*   ����ʹ����������Ĳ�ͬ����VFW(��ʱ��CLSID_VfwCapture)��WDM(����,)���ֿ���
*   VFW 
*     IAMVfwCaptureDialogs --��ʾ���öԻ���(ֻ�ܸ��û���ʾԤ����ĶԻ���)
*   WDM -- ��capture��Preview��Pin�Ͽ��Էֱ����ò���
*     IAMFilterMiscFlags
*     IAMCrossbar -- �����ж�����롢���Pin�������ø���pin�����ӷ�ʽ?
*     IAMVideoProcAmp
*     IAMAnalogVideoDecoder
*     ���(��������?)Pin��Ҫʵ��
*      IKsPropertySet -- amCap������ȷ���Ƿ���ģ����Ƶ����Pin(ANALOGVIDEOIN)��ʹ��Crossbar:
*        Get(AMPROPSETID_Pin,AMPROPERTY_PIN_CATEGORY, NULL, 0, &guid, sizeof(GUID), &dw); 
*        if(guid == PIN_CATEGORY_ANALOGVIDEOIN) {  } 
*      IAMStreamConfig -- ��ȡ���õ�����Ƶ��ʽ(GetFormat)����ȡ�����õ�ǰ��ý���ʽ
*   ģ����ӽ��� -- ʹ�ýӿ� IAMTVTuner
*     TV Tuner -- ���ڵ���Ƶ����������գ�
*     TV Audio -- ���ڵ��������Ľ��������
*     Crossbar -- ���ڲɼ���������ӵ�ѡ��(�������� Video Tuner��Video Composite �� S-Video)
*                 �����롢������˿���ѡ�����ӵ���·?
*********************************************************************************************/

/*********************************************************************************************
* ����Ƶ������һ�������֣�
*   1.Native codecs -- �� MP3 �� LAME DirectShow wrapper
*   2.ACM/VCM codecs
*   3.DMO(DirectX Media Object) codecs
*
* ��Ƶ����ѹ��(VideoCodec) -- ͨ�� CLSID_VideoCompressorCategory ö��
*     Microsoft RLE -- MSΪAVI��ʽ������һ�ֱ��룬ʹ��RLE����ѹ���㷨������8λ�ı��뷽ʽ��ֻ��֧�ֵ�256ɫ��
*       ͨ��ֻ����ѹ�����������Ǽ�����ϳɵ�ͼ��Ⱦ��д����ɫ����ز�
*     Microsoft Video 1 -- ���ڶ�ģ����Ƶ����ѹ������һ������ѹ����������߽��ﵽ256ɫ,һ�㲻���ڱ���
*   ��������Ƶѹ������
*     IAMStreamConfig -- 
*     IAMVideoCompression(���Pin֧��)�� ������֡�ʣ�ѹ��������
*       GetInfo -- ���FilterĬ�ϵ�֡�ʺ�������, pCapabilities ��ֵ�� CompressionCaps
*     IConfigInterleaving -- ���ý���ģʽ
*     IConfigAviMux -- ��������(SetMasterStream)��AVI�����Ե�����(SetOutputCompatibilityIndex,�����ɳ�����ܿ��ٶ�λ)
*       AviMuxĬ�ϴ���AVI 2.0��index(indx),��ΪTRUE���Դ���AVI 1.0��index(idx1)
* ������(δȷ��):
*   Xvid -- AVI
*   h264 -- MOV
*   wmv9 -- WMV
*********************************************************************************************/

#endif //DIRECT_SHOW_HARDWARE_H