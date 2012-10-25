#ifndef DIRECT_SHOW_STUDY_H
#define DIRECT_SHOW_STUDY_H
#pragma once

//http://xbox360.tgbus.com/zixun/gzpj/201203/20120316134756.shtml
//http://www.cppblog.com/lovedday/archive/2007/09/24/32815.html

//COM => Filters => Pins => MediaTypes

/*************************************************************************************************
* ��ǰ���� ActiveMovie, ���Ժܶ�ṹ����Ϊ AM_XXX
* TODO: CSourceStream ���е� pObjectName �� pName �ֱ����ʲô?
*
* ���Թ��ߣ�
*   1. GraphEdit(graphedt.exe) <== DirectShow Graph Tool, �� Vista��װ��Vista SDK ����Ҫע��SDK�� proppage.dll ����Զ�����ӡ�
*      TODO: �Ƿ���Ҫ����ע�� System32 �µ� quartz.dll ?
*   2. DXCapsViewer.exe -- �鿴��ǰϵͳ��DirectX�豸(�Կ���DSound������DInput��) ������
*   3. VideoInspector(http://www.kcsoftwares.com) -- ���Բ鿴����ý�����Ϣ
*
* DirectShow��һ����ȫ����COM��Ӧ��ϵͳ��SDK����Դ�������Ѿ�ʵ����Filter�е�COM��������
* DirectShow ���п�ĺ��Ŀ��� quartz.dll�����а���������Ƶ����Ƶ���ŵ�����Ҫ�� COM �ӿں� coclass��
* 
* ���ýӿ�
*   IPin -- ����Pin�����ӡ��Ͽ�����Ϣ��ȡ��Stream����(??)��
*     NewSegment(������һ���µ�Segment��??���� CRefTime �� m_tStart �� m_tStop)
*
*   IMediaFilter -- �� IPersist �̳У����� ֹͣ����ͣ�����ŵ�״̬ת�� �� ͬ��ʱ�ӵĹ���
*   IBaseFilter  -- �� IMediaFilter �̳У�����ö�ٺͲ������ϵ�Pin����ȡFilter����Ϣ���ܼ���IFilterGraph
*   IFilterGraph -- ����Filter����ɾ�飩��ֱ������
*   IPinFlowControl -- ֻ��һ�� Block ����(???)
*   IMediaSample -- ����ʵ���ڴ洫��͹������а��� ý�����͡�ʱ��(Time)��ý��ʱ��(MediaTime)��
*     SyncPoint(ͬ����) -- IsSyncPoint ���� S_OK �� TRUE��S_FALSE �� FALSE)
*     Preroll(Ԥת��) -- ������������������ʾ��Ҫ�������ٶȲ�ƥ���������buffer����
*     Discontinuity -- �Ƿ�����������,���ж��Ƿ����Seek����?
*   IMemAllocator -- ���� IMediaSample ���ڴ�� 
*   IMemInputPin -- ��ע�⣺���Ǵ�IPin�ϼ̳У����� IMemAllocator�����ܽ��� IMediaSample

*   IEnumPins -- ö��Filter�ϵ�Pin
*   IEnumMediaTypes -- ö��Pin֧�ֵ�ý������(ע�⣺Filter��û��ö�ٵĸ���?�μ� CEnumMediaTypes �Ķ���)
* 
* DirectShow ��Ҫ���� DllEntryPoint ������
* Filter Graph 
*   IFilterGraph���������е�Filter���ɽ���ֱ������
*   IGraphBuilder:������������
*     AddSourceFilter -- ����Դ�ļ�·�������ܼ�����Ӧ��Source Filter
*       1.���·������Э����(��http)���� HKEY_CLASSES_ROOT\{protocol}\Extensions �в�ѯ��չ�������
*         ���㣬ʹ�ö�ӦCLSID��Filter������ʹ��"Source Filter"ָ����CLSID -- File Source (URL)��
*       2.���������Э������������չ����HKEY_CLASSES_ROOT\Media Type\Extensions ƥ����չ����
*       3.�����ļ���У���ֽ��жϣ�HKEY_CLASSES_ROOT\Media Type\{MEDIATYPE_Stream}\{subtype}��
*         ���ʽΪ"offset,cb,mask,val" -- �� offset ��ʼ�� cb ���ֽڣ��� mask ����λ���ͬ val �Ƿ����
*       4.������Ϊ File Source(Async.) ��Filter���� MediaType ʹ�� { MEDIATYPE_Stream, MEDIASUBTYPE_None }
*     Render -- ��ָ�����Pin��ʼ������������
*       1.�������Pin֧�� IStreamBuilder �ӿڣ����� IStreamBuilder::Render �����������
*       2.ʹ�� IGraphConfig::AddFilterToCache �����ڲ������е�Filter����������;
*       3.ʹ�õ�ǰ Filter Graph �л�û����ȫ���ӵ�Filter���������ӣ�
*       4.ʹ�� IFilterMapper2::EnumMatchingFilters �������� MERIT_DO_NOT_USE ������Filter���������ӣ�
*     RenderFile --
* 
* Filter: SourceFilters(ֻ�����Pin) => TransformFilters => RenderingFilters(ֻ������Pin)
*   Filter ������뵽FilterGraph�ҽ��뵽������·�в��ܷ������ã����������ֲ��DMO
*   IBaseFilter��
* Pin��
*   IPin��
*
* Filter ��ע��(�� AMovieDllRegisterServer2 ��������)��
*  ������COM��Ϣ(ThreadingModeΪBoth) 
*  Filter���е���Ϣ(ע�������Ŀ¼��Pin������֧�ֵ�ý�����͵ȣ�
*    �������²�ζ���ע�����õĽṹ����Ϣ
*      CFactoryTemplate
*       => AMOVIESETUP_FILTER -- ��ͨ������ CBaseFilter::GetSetupData ���أ���֧�ֶ�̬ע��(IAMovieSetup::Register)?
*        => AMOVIESETUP_PIN
*         => AMOVIESETUP_MEDIATYPE 
* 
*    ͨ��ע���� CLSID_LegacyAmFilterCategory(DirectShow Filters) ������
*    ���е�Category��HKEY_CLASSES_ROOT\CLSID\{DA4E3DA0-D07D-11d0-BD50-00A0C911CE86}\Instance -- CLSID_ActiveMovieCategories
*    ָ��Category�µ�����Filter��HKEY_CLASSES_ROOT\CLSID\{Category}\Instance -- ��"{083863F1-70DE-11d0-BD40-00A0C911CE86}"
*    ��δ����Լ�������Ŀ¼��������
*
*  Meritֵ��Filter����������ʱ��ʹ�õ����ȼ���ֻ�д��� MERIT_DO_NOT_USE(0x200000) ʱ���ſ��ܱ���������ʹ�á�
*
* 
* ��������(Intelligent Connect)
*   ����ʹ��Cache�е�(IGraphConfig::AddFilterToCache �� Filter ��FilterGraph������Cache���������ӳɹ��Ļ����ƻ� FilterGraph)
*     -> ����ʹ�� Filter Graph �е� 
*       -> ���ͨ��ע�����Ҹ��� MERIT_DO_NOT_USE ��
*   ���Pin -> ����Pin����Ҫ��CBasePinʵ��)��  TryMediaTypes��QueryAccept ?
*   IPin(��Pin)::Connect(��Pin)
*     -> ����Ƿ��Ѿ����� �� �Ƿ����ڷ�ͣ״̬�¶�̬����
*     -> ��Pin::AgreeMediaType(Э��˫����֧�ֵ�ý�����ͣ��粻��������ͨ������ VFW_E_TYPE_NOT_ACCEPTED )��ʧ�ܵĻ�BreakConnect
*       -> �������ȫָ����ý�����ͣ����� ��Pin::AttemptConnection ֱ�����ӣ�
*          ���� ����Pin�����Pin��˳�� ö��Pin�ϵ�ý������(EnumMediaTypes)��Ȼ�������� TryMediaType -- �ڲ����� AttemptConnection )
*          ��ö��ʱ������� ��Pin::GetMediaType �������鿴֧�ֵ����ͣ�Ӧ�ð���˳���ſ�֧�ֵĸ�ʽ(�����ȵ���0λ��)
*         ->��Pin::AttemptConnection�ڲ�����˳�򣨿��Կ�������CBasePin���麯�����ã���
*           ->��Pin::CheckConnect(�������Pin�Ƿ�������ӣ��緽����벻ͬ)[->ʧ�ܵĻ�BreakConnect]
*           ->��Pin::CheckMediaType(����������أ�����Ƿ��ܽ�������ý������)����ɹ�������m_Connected(���ӵ�Pin)����
*           ->��Pin::SetMediaType(����������أ�����Ҫ�ȵ��û���ķ������������ȷ����ý������m_mt��Ȼ����������)
*           ->��Pin::ReceiveConnection(�鿴���ӵ�Pin�Ƿ��ܽ��ձ�Pin�Ͷ�Ӧ�����ͣ��ڲ��߼��ͳ�Pin��AttemptConnection���)
*             ->��Pin::CheckConnect(�ȼ�����ӷ���)
*             ->��Pin::CheckMediaType(����������أ�����Ƿ��ܽ�������ý������)
*             ->��Pin::SetMediaType(����������أ�����Ҫ�ȵ��û���ķ������������ȷ����ý�����ͣ�Ȼ���ٸ�������m_mt������)
*             ->��Pin::CompleteConnect������Pin�ϵ�Complete)
*           ->��Pin::CompleteConnect(�������Pin�ϵ�CompleteConnectЭ��ʹ�õ�MediaSample)������ CBaseOutputPin����
*
* Pinʹ�õĵ�MediaSample����Э��
*   CBaseOutputPin::CompleteConnect
*     ->DecideAllocator(Э���ڴ������)
*       ->InputPin::GetAllocatorRequirements(��ȡ����Pin�ϵ�����)
*       ->InputPin::GetAllocator (���Ȼ�ȡ����Pin�ķ�����������ɹ��򷵻�)
*       ->DecideBufferSize(���Pin����������أ���������m_mt��ȷ��ʹ�õķ���������--�ڴ��С��ʹ�õ�
*         Sample������--ͨ�� IMemAllocator::SetProperties ����)
*       ->InputPin::NotifyAllocator(�ɹ�������������Pin��NotifyAllocator֪ͨ����ʹ�õķ�����)
*       �������Pin���ṩ�������������÷�����ʧ��
*       ->InitAllocator(�����Pin������׼�ķ���������ִ����ͬ���߼�)
*         DecideBufferSize->InputPin::NotifyAllocator)
*
* ԴFilter��ʼ���ݴ��䣨��ģʽ�� CSourceStream��
*   CAMThread::InitialThreadProc //ԴFilter���̺߳������
*     ->CSourceStream::ThreadProc //�������ݴ���Ĺ����̺߳���
*       ->GetRequest(��ʼ�����ȴ� CMD_INIT ����)
*       ->OnThreadCreate������������ظ÷��������г�ʼ����������Դ�ȣ�
*       ->ѭ��ִ�� GetRequest ,��������ִ�в�ͬ�Ĳ���(���յ� CMD_EXIT �¼����˳�ѭ��):
*         ->CMD_PAUSE(��ʼ����ʱ��״̬ת��Ϊ Stop->Pause->Run ,Ȼ������״̬��ʼ���ڱ�������) => DoBufferProcessingLoop
*           ->OnThreadStartPlay��֮�����ѭ����ֱ�����յ� CMD_STOP �¼�
*           ->GetDeliveryBuffer(��ȡ�յ�Sample Buffer)
*             ->m_pAllocator->GetBuffer
*           ->FillBuffer(����������أ�����ý��������Video��Audio���������)
*           ->CBaseOutputPin::Deliver(�����Pin����Sample)
*             ->m_pInputPin->Receive(���������ӵ�����Pin�ϵ�Receive��������������)
*       ->OnThreadDestroy (ֹͣʱ������ֹ�����ͷŷ������Դ)
* 
* ���ݴ������
*   CBaseOutputPin::DeliverEndOfStream
*     ->CBaseInputPin::EndOfStream
*
* ���⣺BeginFlush/EndFlush/Receive �ȵĹ�ϵ��˳��
* 
* IMediaSample�����ݴ��͵�Ԫ���ɷ�����(Allocator����)
* ���ݴ��ͣ�
*   ��ģʽ(Live Source��) -- CAMThread, Down Filter ������Pin��ʵ�� IMemInputPin �ӿ�
*   ��ģʽ(File Source��) -- CPullPin�����Pin��ʵ�� IAsyncReader �ӿڣ�ͨ���� Splitter������Pin�ϵ��̡߳�����
*   ͨ�� IOverlay/IOverlayNotify �ӿ�(��ͳ��Video Renderer) -- û��ʵ�ʵ����ݴ��䣿
*
* ����
*   COutputQueue -- ʵ����Sample�Ķ���ʽ���ͣ����Թ�����ͬ��ģʽ��Ҳ���Թ������첽ģʽ(ʹ��һ���������߳�)
*     �����Pin�ϴ���ʵ���������乹����ָ��������Pin�����ŵ�����Pinָ�롣���ݴ���ʱ����COutputQueue����ĺ���
* 
* ʵ���Զ���ӿ�(��֧��ϵͳ�ṩ�������ӿ�-- ISpecifyPropertyPages ) -- 
*   ��ע�⣺Ҳ����ʹ��IDL���ɽӿ����� -- ���뽫 .idl �ļ������е� "MkTypLib Compatible"��Ϊ No ���ܱ��룩
*   1.�����ӿڵ�ͷ�ļ�(ע����Ҫʹ�� #ifdef __cplusplus extern "C" { #endif �������������)��
*     ʹ�� guidgen �������ɶ���IID��DEFINE_GUID(IID_IXXXX,xxx,xxxx,xxx);��
*   2.ʹ�� DECLARE_INTERFACE_(IXXXX, IUnknown) �궨��ӿ�
*     STDMETHOD(������) (THIS_      //ע�⣺THIS ������û�в��������
*       CMediaType *pmt  //��������
*       ) PURE;
*   3.ʵ����̳иýӿڣ������� DECLARE_IUNKNOWN �� ��ʵ�ֽӿڵ� QueryInterface ��Ĭ�Ϸ���
*   4.���� STDMETHODIMP CXXX::NonDelegatingQueryInterface(REFIID riid, void ** ppv); �������������ж�QI�Ľӿڽ��з��أ�
*     ���򷵻ػ���� NonDelegatingQueryInterface ����
*       if (IID_IXXXX == riid) {return GetInterface((IXXXX *) this, ppv);}
*       else { return ����::NonDelegatingQueryInterface(riid,ppv); }
*     
* ����ҳ -- ͨ������ҳֻ��ֱ�ӻ�ȡ�� IBaseFilter �Ľӿڣ�������ȡ����Ƹ��࣬Filter��Ҫʵ���Զ���ӿڣ�������QI
*   1.��������ҳ��Ҫ�ı����ַ����ͶԻ�����Դ -- ע���������ԣ�Border--None��Style--Child��TitleBar--False��
*   2.�� CBasePropertyPage �̳У��ڵ��û���Ĺ��캯���У�ָ�� DialogId ����
*   3.���岢ʵ��static CUnknown * WINAPI CreateInstance(LPUNKNOWN lpunk, HRESULT *phr)������ȫ��
*     ���� g_Templates ������ -- ��Filterһ��
*   4.ͨ����Ҫ�������·����������Դ���
*     OnReceiveMessage -- ��Ϣ����
*       �ڸ������ú󣬿���ͨ�� m_bDirty = TRUE;m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY) ֪ͨApply��Ч
*     OnConnect(IUnknown *pUnknown) -- ͨ��IUnknown�ӿڲ�����ȡFilter����Ϣ�������Զ���ӿڣ�,���г�ʼ��
*     OnDisconnect -- �ر����Դ���ʱ�ͷ������Դ(���OnConnect ʧ�ܷ��ش���Ҳ���Զ����øú���)
*     OnActivate -- ����ҳ����ʱ
*     OnDeactivate -- �л�����ҳʱ���ɵ�����ҳ��ȡ�����ע�⣺��ʱԭ����Wnd�ᱻDestroy����Property���󲻻�����
*     OnApplyChanges -- ��鲢��������ֵ������m_bDirty��ΪFALSE
*   5.����ʵ���Զ���ӿڵķ�ʽ�� ��Filter��ʵ�ֲ����� ISpecifyPropertyPages �ӿ�
*       STDMETHODIMP CXXX::GetPages(CAUUID *pPages){
*         //��������ҳ����Ϣ(ҳ����ÿһҳ��GUID)
*         pPages->cElems = 1;  
*         pPages->pElems = (GUID *) CoTaskMemAlloc(sizeof(GUID) * 1);
*         pPages->pElems[0] = CLSID_MYPropertyPage; //ָ������ҳ��GUID
*       }
* 
* �־û� -- Filter ������ Graph �б��沢�ָ����Ե���Ϣ
*   1.�� pstream.h �ļ��е� CPersistStream ��̳� -- ��ʵ�� IPersistStream �ӿ�;
*   2.ͨ�� NonDelegatingQueryInterface ���� IPersist �� IPersistStream �ӿ�
*   3.���� GetClassID �������ṩ Filter ��CLSID��
*   4.���� ReadFromStream �� WriteToStream ���� ���г־û���
*     ���Ե��� WriteInt �� ReadInt ��������������UNICODE�ķ���д�루ÿ������ռ��12��WCHAR��
*   5.���� SizeMax ����������Ҫ������ֽڸ���(UNICODEʱ��ע��TCHAR��2���ֽ�)��
*   6.�����ݱ䶯�󣬵��� CPersistStream::SetDirty(TRUE) ����
*
*
* Ӳ���豸(Device Filter)
*   һ�㶼Ҫ��ö��������Graph, ������һ������ö��ʱ��ͨ��IPropertyBag::Read(L"FriendlyName",&varName, 0)��á�
*   ��ȡ"Description"���Կ��Ը���ϸ�صõ�������Ϣ
* 
* ���ſ��ƣ�
*   ��λ(Seeking) -- IMediaSeeking(��λ������ Renderer->Source�����ݶ����� Source->Renderer)
*     һ������ģʽ��SourceFilter�ϵ����Pin����ģʽ��Parser/Splitter Filter �����Pinʵ�������Ķ�λ������
*     ϵͳ���� CSourceSeeking��CPosPassThru��CRendererPosPassThru ���ṩ��λ���ܡ�
*     CTransformOutputPin::NonDelegatingQueryInterface ������ IMediaSeeking �� IMediaPosition(���ݵ�Upper Filter)
*   ���/������֧�� ͨ��IMediaSeeking::SetRate����IMediaPosition::put_Rate��ʵ�֡�
*     ����Rate�ı����Ӧ����Ҫ�����ڸ����ʱ�����Filter�ϣ��п�����Parser/Splitter Filter,
*     Ҳ�п�����Pushģʽ�µ�Source Filter;��ЩFilter��Ҫ����CBaseOutputPin::DeliverNewSegment ������
*     ��ʾһ���¸�ʽ�����Ŀ�ʼ,Ȼ������µ�Rate���´��ʱ�����
*   ��֡���� -- ͨ��Filter Graph Manager�ϻ�õ�IVideoFrameStep�ӿ���ʵ�֡���Step�������óɹ���
*     Filter Graph Manager����Ӧ�ó�����һ��EC_STEP_COMPLETE�¼���Filter GraphҲ�Զ�ת��Paused״̬
*
* ��Filtr Graph����״̬�£����Զ�̬�����µ�Filter��������ɾ��Filter��Ҳ���ܶϿ�Filter֮���Pin���ӡ�
*   Filter��Filter Graph��ɾ��������Stopped״̬��ɾ��ǰ�����Filter��Pin�Ͽ����Ǳ�Ҫ�ģ��������ŵ�Pin
*   ����ͨ��IFilterGraph::Disconnect�Ͽ�������������ͷ��Pin��Ҫ����һ�Σ���
* ��̬�ؽ����� -- ���� Filter Graph ����״̬��ͬʱʵ���ؽ�
*   1.ý�����͵Ķ�̬�ı䣺
*   2.���ӻ�ɾ��Filter�����½������Filter֮������ӣ�
*   3.��һ��Filter��·(Filter Chain)���в�����
*   
*
* ѹ��/��ѹ -- ACM��Audio Compression Manager����VCM��Video Compression Manager��������ͨ�� Windows Multimedia API ���ã�
*   ��DirectShow�ж���ͨ����װFilter(Wrapper Filter)Ӧ�õģ���Ϊѹ����ʱ��ֻ��ͨ��ϵͳö��ȡ�á�
*   ACM ��Ϊ��ѹ��ʱע����"DirectShow Filters"Ŀ¼�£���Ϊѹ����ʱע����"Audio Compressors"Ŀ¼
*     <msacm.h> ͷ�ļ����� acmStreamOpen ��API����ֱ��ʹ��
*   VCM ��Ϊѹ����ʱ����ѹ����ע����"Video Compressors"Ŀ¼
*
* ���ܶ���
*   1.ʹ�� <measure.h> �ļ��е� MSR_XXX(��ôʹ�ã�)
*   
* ����
*   1.ΪʲôBaseClass����Ҫ���⴦�� UNICODE �ꣿ��UNICODEʱ���ǳ����ظ����壿
*   
*************************************************************************************************/
GetSubtypeName
TODO:����ϵͳ�ṩ��Filter����Щ
 
#endif //DIRECT_SHOW_STUDY_H