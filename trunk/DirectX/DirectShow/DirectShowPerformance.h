#ifndef DIRECTSHOW_PERFORMANCE_H
#define DIRECTSHOW_PERFORMANCE_H
#pragma once

/*****************************************************************************************************
* ���ܶ���(performance measurement),�������ж����� DXMPERF/PERF ��ʱ������ amfilter.cpp ���Զ����� <dxmperf.h>
*   Ȼ��ʹ�� PERFLOG_CTOR��PERFLOG_RUN �Ⱥ��ڸ����ؼ��׶δ�ӡ��־��
* 
*   CVideoTransformFilter::RegisterPerfId
*     m_idSkip -- Video transform skip frame
&     m_idFrameType -- Video transform frame type
*     m_idLate -- Video transform lateness
*     m_idTimeTillKey -- Video transform estd. time to next key
*
* ��������(quality management) -- ����Ӧ�ķ������ƣ�
*   IQualityControl -- �����������ƣ�����֪ͨ���������ٶ��Ƿ����
*     Notify(IBaseFilter * pSender, Quality q);  
*       //q.Type -- Famine:�ٶ�̫����Flood���ٶ�̫�죻
*       //q.Proportion -- ������ڵ��ٶȱ��ʣ���1000Ϊ��׼��m_iRepeatTime = m_iRepeatTime*1000 / q.Proportion;
*       //q.Late -- ��ǰ���µ�Sample�����ϵ�ʱ�������˶��٣��������絽��m_rtSampleTime += q.Late;(ֱ�������м䲿��)
*       //q.TimeStamp -- ���͸���Ϣʱ��ʱ�䣬ͨ����Sample�ϵĿ�ʼʱ�� -- ��ô�ã�

*****************************************************************************************************/



#endif //DIRECTSHOW_PERFORMANCE_H