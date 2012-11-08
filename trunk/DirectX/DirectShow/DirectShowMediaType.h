#pragma once

/*************************************************************************************************
* CMediaType(���������ý�����ͣ�������GUID˵��) -- ע��ʹ�ú���Ҫ FreeMediaType/DeleteMediaType �ͷ�����ڴ棺
*   �������GUID��ָ���ض���ֵ�����Ϊ����ȫָ����ý�����͡���
*     ֻҪ��һ��ָ��ΪGUID_NULL(MEDIATYPE_NULL/MEDIASUBTYPE_NULL)�����Ϊ������ȫָ����ý�����͡�
*
* ���� CMediaType ��ֵ�� AM_MEDIA_TYPE ʱĬ����ǳ��������ֵ��Ϻ� CMediaType �� pbFormat/cbFormat/pUnk ��ֵΪNULL
*   ���Խ�������Ȩ��ֵ�� AM_MEDIA_TYPE���ұ������ʱ��������ʧ���μ� CEnumMediaTypes::Next ��
*   ������ CopyMediaType �������
*
*   ˵������ЩGUID���Ӧһ�� FOURCCMap������ 
*     FOURCCMap fccMap = FCC('AVI '); pMediaType->subtype = static_cast<GUID>(fccMap);
* 
*   majortype  -- ý�����ͣ�
*     MEDIATYPE_Audio��
*     MEDIATYPE_AnalogVideo�� 
*     MEDIATYPE_Interleaved -- ����Ƶ���������?���� DV ¼�Ƶ�����? 
*     MEDIATYPE_Stream -- ������������ MPEG2?
*     MEDIATYPE_Video -- ��Video, 
*   subtype    -- ��ʽ���ͣ�
*     MEDIASUBTYPE_AVC1 -- H.264 bitstream without start codes
*     MEDIASUBTYPE_Avi -- 
*     MEDIASUBTYPE_ARGB32 -- ÿ��������32λ��ʾ��RGB������ʹ��8λ��ʣ�µ�8λ���ڱ�ʾAlphaͨ��ֵ����
*       ���ڴ���RGB������������˳��Ϊ��BGRA,ͨ������ʹ��RGBQUAD���ݽṹ������һ������
*     MEDIASUBTYPE_H264 -- H.264 bitstream with start codes.
*     MEDIASUBTYPE_MPEG1Audio -- ����MEDIATYPE_Stream
*     MEDIASUBTYPE_MPEG1AudioPayload -- ���� MEDIATYPE_Audio
*     MEDIASUBTYPE_Mpeg2 -- ���� MEDIATYPE_Stream������ͬʱ��������Ƶ?
*     MEDIASUBTYPE_MPEG2_AUDIO -- 
*     MEDIASUBTYPE_MP3 -- 
*     MEDIASUBTYPE_PCM
*     MEDIASUBTYPE_RGB24 -- �ڴ���RGB������������˳��Ϊ BGR�� ͨ������ʹ��RGBTRIPLE���ݽṹ������һ������
*     MEDIASUBTYPE_RGB8 -- 
*     MEDIASUBTYPE_RGB565 -- 
*     MEDIASUBTYPE_UYVY --  
*     MEDIASUBTYPE_AYUV --  ����һ��Alphaͨ��������Ϊÿ�����ض���ȡYUV����(4:4:4)��ͼ�����ݸ�ʽΪ A0 Y0 U0 V0 
*   formattype -- ��ʽϸ�ڣ�ͨ�� pbFormatָ��ָ���������ݽṹ(��СΪcbFormat)��
*                ���а�������Ƶͼ��Ĵ�С��֡�ʣ���Ƶ�Ĳ���Ƶ�ʡ��������ȵ�
*     FORMAT_AnalogVideo   -- 
*     FORMAT_DvInfo        -- DVINFO(DV stream)
*     FORMAT_MPEGVideo     -- MPEG1VIDEOINFO
*     FORMAT_MPEG2Video    -- MPEG2VIDEOINFO
*     FORMAT_MPEGStreams   --   
*     FORMAT_VideoInfo     -- VIDEOINFOHEADER, ͨ������ MEDIATYPE_Video + MEDIASUBTYPE_RGBXXXʱ��ͨ�� HEADER �����ֱ�ӷ������е�BITMAPINFOHEADER
*     FORMAT_VideoInfo2    -- VIDEOINFOHEADER2�������� dwPictAspectRatioX x dwPictAspectRatioY����ʾ�������� 16x9�� ��,
*     FORMAT_WaveFormatEx  -- PCM��Ƶʹ��
*
*   bFixdSizeSamples -- ��ΪTRUE����ʾISampleSize��Ч����ÿ��Sample�Ĵ�С�̶����������lSampleSizeֵ
*   lSampleSize -- ��ʾÿ��Sample�Ĵ�С����Ϊ0��ʾ��С�ɱ�
*   bTemporalCompression -- ��ֵΪFALSE����ʾ����֡���ǹؼ�֡
*   pUnk -- ?
*
*************************************************************************************************/

/*************************************************************************************************
* ��̬��ʽ�ı�(Dynamic Format Change)
*   �� Transform �ȷ����ڲ����ü�� MediaType ?
*   CBasePin �� CanReconnectWhenActive �������ж��Ƿ�����ڼ���(Paused/Running)״̬��������Ĭ�������Ϊ FALSE
*               BreakConnect
* ���� -- 
*   IFilterMapper::EnumMatchingFilters -- ��ϵͳ��ö�ٳ�����Ҫ��(Merit,Major,Sub��)��Filter
*   IGraphBuilder::AddSourceFilter -- 
*   IGraphBuilder::Render -- ��ĳ�����Pin����ʼ������µ�graph�������÷������Զ������pin������ӱ����filter��ֱ��renderer filterΪֹ
*   IGraphBuilder::RenderFile -- ����һ���������ļ��ط�graph
*   IFilterGraph::ConnectDirect -- ������pin֮�����ֱ�����ӣ��������ʧ�ܣ��򷵻�ʧ��
*   IFilterGraph::Connect��GraphEdt�ϵķ�ʽ?�� -- ��������Pin��������ܵĻ���ֱ���������ǣ��������м����������filter���������
*   IFilterGraph::Reconnect -- ������ָ����Pin
*   IFilterGraph2::ReconnectEx -- ��ָ����ý������������ָ����Pin
*************************************************************************************************/
