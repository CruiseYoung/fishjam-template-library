#pragma once
/*****************************************************************************************************
*
* CBasePin   //�� IPin �� IQualityControl�̳�
* +-CBaseInputPin  //ʵ�� IMemInputPin
*   +-CRenderedInputPin
*   +-CTransformInputPIn
*     +-CTransInPlaceInputPin
* +-CBaseOutputPin //���Pin��ͨ��IMemInputPin�ӿڰ����ݴ��ݸ����ӵ�InputPin
*   +-CSourceStream
*   +-CTransformOutputPin
*     +-CTransInPlaceOutputPin
*   +-CDynamicOutputPin //ʵ�� IPinFlowControl �ӿڣ���������(Block)?
*       �����������������Զ�̬�ı����? ChangeOutputFormat/DynamicReconnect
*       �и����� CAutoUsingOutputPin(������StartUsingOutputPin,������StopUsingOutputPin)
*
* CBasePin
*   1.��Ҫ˵��
*     ͨ����Ա���� m_mt �����˵�ǰ����ʹ�õ� CMediaType,
*   2.�������ص�(��)�麯��
*     a.CheckMediaType(const CMediaType *pMediaType); -- ����Ƿ�֧��ָ����ý�����ͣ�
*       �Ƚ�Type��SubType��Format(������ֵת��Ϊ������Ϣ,��VIDEOINFO)�� 
*   3.��ѡ�����غ���
*     Active/Inactive/Run -- ״̬ת��ʱ��֪ͨ����
*     EndOfStream(); //û�����ݴ���ʱ���ã�һ��������ڽ�������(�� д������־���� EC_COMPLETE ��Ϣ�� ?)
*       ֻ��InputPin�ϵ���,OutputPin�ϵ��� DeliverEndOfStream 
*     GetMediaType(int iPosition,CMediaType*); //���ؿ�֧�ֵ�ý�����ͣ������Ĭ��ʵ�ַ��ش���
*     Notify(IBaseFilter* pSender, Quality); //IQualityControl�ӿ�֪ͨ���������ٶ�
*     SetMediaType(const CMediaType *); //Filter֮��Э�̳ɹ���ͨ���ú���֪ͨPin��
*       ע���������ȵ��û���ķ����������ȷ����ý�����ͣ�Ȼ���ٸ�������(m_mt)������
*   4.�ᱻQI�Ľӿ�
*     IPin
*     IQualityControl
* 
* CBaseInputPin  
*   1.��Ҫ˵��
*       �� IMemInputPin �ӿڼ̳У��� CBaseOutputPin ʹ�� ��
*       GetAllocator --���� IMemAllocator �ӿڣ���û�лᴴ��ȱʡ���ڴ������
*       NotifyAllocator -- ֪ͨʹ�õ�IMemAllocator,
*       Receive -- OutputPin���ø÷�����������
*   2.�������ص�(��)�麯��
*   3.��ѡ�����غ���
*     
*
* CRenderedInputPin
*   1.��Ҫ˵�� -- ���� Render Filter ������Pin
*   2.�������ص�(��)�麯��
*     a.CheckMediaType(const CMediaType *);
*   3.��ѡ�����غ���
*
* CBaseOutputPin  
*   1.���� IMemAllocator �� IMemInputPin �ӿڣ�������Pin���ṩ�ڴ�������������Pin�ṩ
*   2.�������ص�(��)�麯��
*     DecideBufferSize(IMemAllocator * pAlloc,ALLOCATOR_PROPERTIES * ppropInputRequest);
*       ����Э�̺õ�ý������(m_mt)��ȷ��ʹ�õķ���������--�ڴ��С(cbBuffer)��ʹ�õ�Sample����(cBuffers)�ȣ�
*       ��ͨ�� IMemAllocator::SetProperties ���ã�
*       (ע�⣺�������ܷ��سɹ�����û�гɹ�������Ҫ���ڴ棬����������Ժ���Ҫ�����ʵ���������Ƿ�����Ҫ��)
*   3.��ѡ�����غ���
*     CompleteConnect -- �������ʱ����ã�������Э�̷�����
*     DecideAllocator -- ��ʹ���Զ���� IMemAllocator
*
* CSourceStream
*   1.��Ҫ˵�� --  ʹ����ģʽ���ɶ����߳��������ݡ�SourceFilter�϶�Ӧ�����Pin
*   2.�������ص�(��)�麯��
*     a.FillBuffer(IMediaSample *pms) -- ��� IMediaSample ���ṩ���������
*       pms->GetPointer(&pData); //��ȡ����Ҫ��� pms->GetSize() ���ֽڵ� pData ���������������ݺ�
*       pms->GetActualDataLength();
*       ������� ...
*         Videoʱ -- pData �� BITMAPINFOHEADER::biSizeImage ��С���ڴ�飬������Ҫ������λͼ��Ϣ��
*         Audioʱ -- pData �� WAVEFORMATEX ��ʲô��
*       pms->SetTime(...);pms->SetSyncPoint(TRUE); //(��ѹ����Ƶ)����ͬ����Ϣ������ÿһ֡����Чʱ�䷶Χ
*     b.DecideBufferSize(IMemAllocator * pAlloc,ALLOCATOR_PROPERTIES * ppropInputRequest); -- ��CBaseOutputPin
*     c.GetMediaType(int iPosition, CMediaType *pMediaType) -- �������Pin����֧�ֵ�ý�����ͣ�Ӧ�ð����ȴ����ṩ(0���)
*       iPosition < 0 ��return E_INVALIDARG; 
*       iPosition > ���֧�ָ�����return VFW_S_NO_MORE_ITEMS; ������ܾͲ����ٴε��øú�����
*       iPosition ����Чֵ��
*         1).����ָ�� *pti = pMediaType->AllocFormatBuffer(sizeof(������Ϣ)); //����ý������(��VIDEOINFO)��Ҫ���ڴ�
*         2).pti->XXXX = .... ; //������Ҫ�����͸�ֵ
*         3).pMediaType->SetType/SetSubtype/SetFormatType/SetSampleSize ���ÿ���ʹ�õĸ�ʽ��Ϣ
*     d.CheckMediaType(const CMediaType *pMediaType) -- 
*     e.SetMediaType(const CMediaType *pMediaType); -- ����Э�̺õ�ý�����ͣ������ñ�Ҫ�Ĳ������˺����Ӽ�����
*   3.��ѡ�����غ���
*     a.OnThreadCreate -- ���̴߳���ʱ
*       OnThreadDestroy
*       OnThreadStartPlay
*     b.Notify(IBaseFilter * pSender, Quality q); -- ������������֪ͨ�����Ե�������Դ��λ�ã�
*   4.�ᱻQI�Ľӿ�(�μ� CBaseOutputPin)
*       IMediaSeeking
*       IMediaPosition
* 
* CTransformInputPin
*   ͨ������PINû����ѡ��ý�����͡���ȫ��������һ��filter��ʹ�õĵ�ý�����͡�
*   
* CTransformOutputPin
*   1.��Ҫ˵��
*     �ڴ��������£�Transform Filter �����������������ǰ������������
*   2.�������ص�(��)�麯��
*   3.��ѡ�����غ���
*
* CDynamicOutputPin -- ���Զ�̬�ı������
*   1.��Ҫ˵��
*   2.�������ص�(��)�麯��
*   3.��ѡ�����غ���
*      
* CPullPin
*   1.��Ҫ˵�� -- ��������Pinʵ�֡�����ģʽ���߳��࣬��CAMThread�̳У�������������һ�������Pin����ʵ��
*     IAsyncReader�ӿڡ���Ϊ����CBasePin����ĳ�Ա����ʹ��(Active�������������̣߳�Inactive��ֹͣ�߳�)
*   2.�������ص�(��)�麯��
*     Receive(IMediaSample*) -- ����Sample������Sampleͨ�����Pin���´���
*     BeginFlush/EndFlush/EndOfStream
*     OnError(HRESULT hr)
*   3.��ѡ�����غ���
*****************************************************************************************************/

