#ifndef DIRECT_SHOW_TIME_H
#define DIRECT_SHOW_TIME_H
#pragma once

//ǳ��DirectShow����Ƶͬ��������� http://www.yesky.com/143/1805143.shtml
// Win8�������� http://msdn.microsoft.com/zh-cn/Hackathon 

//CBasePin::NewSegment -- ʲô�ô�?
//  ʱ������(���ʼֵ)һ���������Ϊ MAX_TIME
/****************************************************************************************************
* �ο�ʱ��(IReferenceClock) -- ���е�Filter����ͳһ�Ĳο�ʱ�ӣ��ο�ʱ�ӱ�֤����������ͬ����
*   1.����ʹ��Ӧ�ó���ͨ�� IMediaFilter:: SetSyncSource ���õĲο�ʱ�ӣ�������ΪNULL����ʾ�������ٶȴ���Sample
*   2.֧�� IReferenceClock �ӿڵ� Live Source
*   3.��Render������ʵ���� IReferenceClock ��Filter�����Ȳ���AudioRender(��������Ӳ����ʱ��);
*   4.���û�У���ʹ�û���ϵͳʱ���ȱʡʱ��( CLSID_SystemClock )
*     MS�ṩ�� CSystemClock ��
* 
* DirectShow �ж���������ʱ�䣺
*  1.�ο�ʱ��(Reference Time, CRefTime ��װ) -- �ο�ʱ�ӷ��صľ���ʱ��(IReferenceClock::GetTime),
*    ��100nsΪ��λ( RefTime.h�ж����˳��� UNITS = 10^7 )�����ü�ֵ����
*    ÿ֡��ƽ��ʱ�䷶Χ VIDEOINFOHEADER::AvgTimePerFrame = UNITS / FrameRate , �� 10^7 / 25.00 = 400000 (100ns)
*  2.��ʱ��(Stream Time) -- Sample�ϵ�ʱ���(���ʱ��)�����δӲο�ʱ�Ӷ�ȡ����ֵ�Ĳ����Graph��ͬ�����粥�ţ���
*    �� IMediaSame::SetTime ? ͨ����һ��Sample���Ϊ0,�����ʱ�������Sample����Ч���Ⱥͻط�������ȷ����
*    Ҳ�� Presentation time ? Renderer���Ǹ������ʱ��������Ʋ��ţ�ͨ�����Դ� CBaseFilter::StreamTime ȡ�á�
*    ����ʱ��ֵ = ��ǰ�ο�ʱ��ʱ�� - ����ʱ��ʱ��( IMediaFilter::Run ��������, ��ֵ������ CBaseFilter::m_tStart )
*    ��ͣʱ��ֵ = ������ͣ��һ�̵���ʱ��
*    Seek��ֵ = ��λΪ0
*    ֹͣʱ��ֵ = ��ȷ��
*  
*  IMediaSample��ʱ�������
*   1.��ģʽ��Source Filter -- ��Ҫ��ÿһ��Sample��ʱ���(SetTime)
*     TODO:�����ʵʱ�Ĳɼ���һ�㲻��ʱ���(ԭ����SDK����˵��) �� 
*   2.��ģʽ��Parser Filter -- ���ļ��н�����ʱ����Ϣ(��AVI��������)��ÿһ֡��ʱ���
*
* ʱ��ͬ��
*   1.����ͬ�� -- ��֤����ý�����ڵ�ʱ���ϵ���������֪Ҫ���簴�չ涨��֡�ʲ���һ����Ƶ
*   2.����ͬ�� -- ��֤��ͬý����֮���ʱ���ϵ��������Ƶ����Ļ��֮��Ĺ�ϵ
*     ����Ƶͬ�� -- latency (��ʱ��mostly used for configuring audio video sync -- IAMLatency ) 
*       һ��Filter����ʱ�����Filter����һ�����������ʱ���ܺͣ�����ʵʱԴ�У���ʱȡ���ڱ��������Ļ�������С��
*         �� Graph ��һ��33ms��ʱ����ƵԴ��һ������500ms��ʱ����ƵԴ����ÿ������Renderer����Ƶ֡Ҫ����֮ƥ�����Ƶ����������Ƶrenderer��470ms��
*            Graph ��Ҫ����������в���.
*       TODO?: AVIѹ��ʱ��ҪIConfigAviMux::MasterStream�������ĸ���(None[-1]��Video[0]��Audio[1])��Ϊʱ���׼�� 
*              �����Ƶ����Ƶ�ɼ�����ͬһ���豸���ɼ�һ��ʱ�����ܳ���ƫ���˱���ʱ��Ҫ��һ��Ϊ��׼��
*              ������һ����֡��(һ��������ƵΪ��׼?)��
* 
*   ���͵� Renderer Filter ��ÿ��Sample����Ҫ��ʱ���(TimeStamp)���ο�ʱ����100nsΪ��ʱ���ȡ�
*   Video Renderer������TimeStamp�����ź�ʱ������ʾ�����߶���һЩsample��������ȴ���������ӿ첥�Ż�����
*   ��Ϊ�����ܿ���(��ʵʱץȡ)����ʱ��Ҫͨ���������Ƶķ�ʽ��֪ͨSourceFilter������֡(����),
*   ��֡��Ϊ�˲����������񣬱���ѡ��ؼ�֡������֡��
*   Seek֮��Sample��ʱ��������ͻ�䣬��ʱ�����һ����̬������ȷ��Stream Time������ʼ��Sampleʱ������Ǻϣ�
*   һ���ȡ��ǰ�����Seek�ؼ�֡�������õ���Samples����Video Renderer�����ǡ��ٵ����ģ�
* 
* ʱ����Ϣ
*   1.�̶����ʵ�ý��(�̶�֡�ʵ���Ƶ��̶������ʵ���Ƶ),��ʱ����Ϣ��֡�ʻ�����ʣ������ļ��ײ����� AVI::hdrl, MP4::moov box
*   2.�����ʵ�ý��(MPEG TS��Real video),��ʱ����ϢǶ��ý�������ڲ������ҿ���Ч����ͬ�������е�ʱ��Ư��
* 
* IMediaSample
*   SetTime(Presentation time) -- ����ʱ���(ʹ�õ���Stream Time)������ͬ������Ƶ����ʾSample����Чʱ�䷶Χ(��ʼ -> ����)��
*     �������е�Sample��Ҫ���ʱ���(����ѹ������)��
*     ���RenderʱSample���˻�û��ʱ����������ϲ��Ÿ�Sample����������ˣ������ IReferenceClock::AdviseTime �ȴ�Sample�Ŀ�ʼʱ�䵽����ٲ��š�
*     Sample�ϵ�ʱ���һ����Source Filter��Parser Filter������
*     ��ʱ����������
*       1.�ļ��ط�(Playback) -- ��һ��Sample��ʱ�����0��ʼ���𣬺���Sample��ʱ�������Sample��Ч���ݵĳ��Ⱥͻط���������;
*       2.����Ƶ��׽(Capture) -- ԭ���ϣ��ɼ�����ÿһ��Sample�Ŀ�ʼʱ�䶼���ϲɼ�ʱ�̵�Stream time��
*                       ��Ƶ�ɼ���Preview Pin����Sample����ʱ���������ÿ��ʱ�����Render�������˵�(����Ϊ���������ճɶ�֡)��
*                       ��Ƶ�ɼ���Audio Capture Filter�����������������߸���ʹ���˲�ͬ�Ļ��棬��Ƶ�����Թ̶���ʱ����(��10ms)�����ɼ�filter�Ļ��塣
*                         ����Ƶ�����ϵ�ʱ�����ӳ�����������ɼ�filterʱ��ʱ�䣬��˻���ƫ���ý��ʱ����Ծ�ȷ�ط�ӳ����������Ƶ����������
*       3.�ϳ�(Mux) -- ȡ����Mux��������������ͣ����Դ�ʱ�����Ҳ���Բ���ʱ������� AVI�ļ���ʽʹ�ù̶���֡�ʶ�û��ʱ�����
*   SetMediaTime -- ���Ǳ���ģ�ȡ����ʵ�֡��ɴ��������ţ����ж��Ƿ���Sample��ʧ�������ڳ���ʵ�֡�
*                   �� ����Ƶ���У�media time��ʾ��Ƶ֡������������Ƶ���У�media time��ʾ���е�����������
* TIMECODE_SAMPLE(ʱ�����)
*   
*   
* �������� -- ����Ӧ�ķ������ƣ�
*   IQualityControl -- �����������ƣ�����֪ͨ���������ٶ��Ƿ���ʣ������ŵ��ٶȲ���ʱ������ҵ�OutputPin��
*     Notify(IBaseFilter * pSender, Quality q);  
*       //q.Type -- Famine:�ٶ�̫����Flood���ٶ�̫�죻
*       //q.Proportion -- ������ڵ��ٶȱ��ʣ���1000Ϊ��׼��m_iRepeatTime = m_iRepeatTime*1000 / q.Proportion;
*       //q.Late -- ��ǰ���µ�Sample�����ϵ�ʱ�������˶��٣��������絽��m_rtSampleTime += q.Late;(ֱ�������м䲿��)
*       //q.TimeStamp -- ���͸���Ϣʱ��ʱ�䣬ͨ����Sample�ϵĿ�ʼʱ�� -- ��ô�ã�
* 
* 
****************************************************************************************************/


#endif //DIRECT_SHOW_TIME_H