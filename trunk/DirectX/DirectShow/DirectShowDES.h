#ifndef DIRECT_SHOW_DES_H
#define DIRECT_SHOW_DES_H
#pragma once

/*********************************************************************************************************
* DES(DirectShow Editing Services) -- ��Ƶ�����Ա༭������Ӧ�ó����DirectShow Core API֮�䡣��Ҫ�����¼������֣�
*   TimeLine -- ��֯����ý��Դ������ƵЧ�������ɴ���ȵ���Ϣ���ϣ����������յ���Ƶ�༭��Ʒ��
*   XML Parser -- ��TimeLine�ṹת��ΪXML��ʽ���ļ����棬���ߴ�XML��ʽ�ļ����ɶ�Ӧ��TimeLine��
*   Render Engine -- ��TimeLineʵ��ת��ΪDirectShow Filterʵ�ֵ������������
*   Media Locator -- ����ý���ļ��Ķ�λ
*
* DES ������ 100����SMPTE����(Transition)�����Լ�΢��IE�Դ��ĸ�������ƵЧ��(IAMTimelineEffect)��
*   ����(IAMTimelineTrans)���
*  
*   TimeLine(��ʶһ����Ƶ�༭����Ŀ -- CLSID_AMTimeline, IAMTimeline, Ȼ��ʹ��CreateEmptyNode��������DES����)
*     => Audio/Video Group(ÿ���������һ��ʽ��ý���������е��鹹��һ��TimeLine����Ƶ����Ƶ�Ƿֿ������ -- IAMTimelineGroup)
*       1=>n Composition(���Ͻڵ㣬���Զ��Ӹ��Ͻڵ��Track���и��ָ��ӵı༭ -- IAMTimelineComp)
*         1=>n Composition/Track(ÿ��Track��ͳһ��ý���ʽ�������Ƶ�����ȼ����ν���-0���,��Ƶֻ�Ǽ򵥺ϳ� -- IAMTimelineTrack) 
*           1=>n Source(��ý��Դ���ṩ��������ز� -- IAMTimelineSrc::SetMediaName)
*
* DES������ʱ�䣺
*   1.ʱ����ʱ��(TimeLine Time):���������ʱ������Ŀ��ʱ�䣬Ҳ��Ϊ��Ŀʱ�� -- IAMTimelineObj::SetStartStop(2)��
*     ע�⣺���ܸ�ͬһ��Track�ϵĲ�ͬý��Դ�����ص���ʱ����ʱ�䣬���������ȷ���Ĵ���
*   2.ý��ʱ��(Media Time):�����ý��Դ��ʱ�䣬���ý��Դ��һ���ļ�����ָ������ļ���ͷ��λ��ʱ�䣬
*     ��������(������ʼ�ͽ���) -- IAMTimelineSrc::SetMediaTimes(2)
*   3.������ʱ��(Parent Time):�����ʱ�������Ѿ����ŵ�ĳ�������ʱ�䣬
* �����ڸ����ȼ���Track�����ù��ɴ������Ĭ�ϵĹ��ɷ����Ǵӵ����ȼ����ɵ������ȼ���
*
* DES������֡�ʣ�
*   1.�����֡��:�������õ�֡��
*   2.ý��Դ֡��:ý��Դ�����֡�ʣ�һ�㲻�ܸ���
*   ��ý��Դ�����õ�ý��ʱ�䳤�Ⱥ�ʱ����ʱ�䳤�Ȳ���ʱ�����γɿ�Ż����ŵ�Ч�� 
*     �������� = (ý�����ʱ�� - ý�忪ʼʱ��)/(ʱ���߽���ʱ�� - ʱ���߿�ʼʱ��) -- ע�������֡���޹�
*
* DES��֡�߽紦��
*   DES���Զ������е�ʱ��ͽ�ȡ��ά֡�߽�(��30ftps�ģ���һ֡0.0333s�󣬵ڶ�֡��Ϊ 0.0666s����������λ
*   �� 0.05s ���������쳣)
*   �������:
*     1.����ý��Դ�ϵ� IAMTimelineObj::GetStartStop���ԭʼ�趨�Ŀ�ʼ������ʱ�䣻
*     2.����IAMTimelineObj::FixTimes��������ʱ��ת����֡�߽�ʱ�䣬���������֡�߽�ʱ����ж�λ��
*   
* DESԤ���������ģʽ��
*   1.RESIZEF_STRETCH(Ĭ��ģʽ) -- ������䡣
*   2.RESIZEF_CROP -- ���������š�Դ̫Сʱ����Χ��ɺ�ɫ��̫��ʱ�������Ĳ��ּ��õ���
*   3.RESIZEF_PRESERVEASPECTRATIO -- ���ֿ�߱������š����У����㲿�ּӺڱߡ�
*   4.RESIZEF_PRESERVEASPECTRATIO_NOLETTERBOX -- ���ֿ�߱������š����У��������ֱ��ü���
*
* ý��̽�������(Media Detector,CLSID_MediaDet)
*   ����Ϣ��ȡ(��ʼ״̬)��ץͼ(EnterBitmapGrabMode)����ģʽ��
*   
* DES֧���Զ����SourceFilter��������ΪDES��ý��Դ���뵽��Ƶ�༭��Ŀ�� -- Ҫ��֧���漴��λ(���Pinʵ��IMediaSeeking)
*
* ����ƵЧ���͹���
*   ʹ��Transform Object����(?)��
*********************************************************************************************************/

#endif //DIRECT_SHOW_DES_H