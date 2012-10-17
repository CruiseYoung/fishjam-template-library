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
*     MEDIASUBTYPE_Avi -- 
*     MEDIASUBTYPE_ARGB32
*     MEDIASUBTYPE_MPEG1Audio -- ����MEDIATYPE_Stream
*     MEDIASUBTYPE_MPEG1AudioPayload -- ���� MEDIATYPE_Audio
*     MEDIASUBTYPE_Mpeg2 -- ���� MEDIATYPE_Stream������ͬʱ��������Ƶ?
*     MEDIASUBTYPE_MPEG2_AUDIO -- 
*     MEDIASUBTYPE_MP3 -- 
*     MEDIASUBTYPE_PCM
*     MEDIASUBTYPE_RGB8 -- 
*     MEDIASUBTYPE_RGB565 -- 
*   formattype -- ��ʽϸ�ڣ�ͨ�� pbFormatָ��ָ���������ݽṹ(��СΪcbFormat)��
*                ���а�������Ƶͼ��Ĵ�С��֡�ʣ���Ƶ�Ĳ���Ƶ�ʡ��������ȵ�
*     FORMAT_AnalogVideo   -- 
*     FORMAT_DvInfo        --  DVINFO(DV stream)
*     FORMAT_MPEGVideo     --  MPEG1VIDEOINFO
*     FORMAT_MPEG2Video    --  MPEG2VIDEOINFO
*     FORMAT_MPEGStreams   
*     FORMAT_VideoInfo     --  VIDEOINFOHEADER, ͨ�� HEADER �����ֱ�ӷ������е�BITMAPINFOHEADER
*     FORMAT_VideoInfo2    --  VIDEOINFOHEADER2
*     FORMAT_WaveFormatEx
*
*************************************************************************************************/
/*************************************************************************************************
* ��̬��ʽ�ı�
*   �� Transform �ȷ����ڲ����ü�� MediaType ?
*   CBasePin �� CanReconnectWhenActive �������ж��Ƿ�����ڼ���(Paused/Running)״̬��������Ĭ�������Ϊ FALSE
*               BreakConnect
*   ��Ҫ������ -- 
*     IFilterGraph::Reconnect -- ������ָ����Pin
*     IFilterGraph2::ReconnectEx -- ��ָ����ý������������ָ����Pin
*************************************************************************************************/
