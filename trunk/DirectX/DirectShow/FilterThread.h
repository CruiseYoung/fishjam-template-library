#ifndef FILTER_THREAD_H
#define FILTER_THREAD_H

#pragma once

/**************************************************************************************************
* ͨ����˵��һ��FilterӦ����ͣ(����)����������λ��:
*   IMemAllocator::GetBuffer(��Filterʹ�õ�MediaSample��ռ����) 
*   IMemInputPin::Receive(��ģʽ���¼�Filter���ܽ�������)
*   IAsyncReader::Request?(��ģʽ���Ƿ������Ƶ�?)
*
* Filter Ĭ�϶���Both�͵�COM�������ͨ�� COINIT_MULTITHREADED ��ʼ��COM( AMovieSetupRegisterServer )
* ʹ�� CAutoLock + CCritSec �ķ�ʽ����ͬ������
*   CBaseFilter��CBasePin ���б�����Ա���� CCritSec *m_pLock; �ñ���ͨ�������ඨ���ͨ�����캯������ʹ��
*   CTransformFilter ӵ�� m_csFilter(״̬��)��m_csReceive(������) ������������
*     ���� m_csFilter ��������� m_pLock��m_csFilter���������� => m_csReceive(������)
*     ״̬��(���߳�)��Pause��SetPositions��Connect��BeginFlush ��
*     ������(�����߳�)��Receive��GetBuffer ��
*   CBasePin �� m_pLock  ͨ��ʹ�ö�ӦFilter����
* 
* ʹ�� DbgLockTrace �� ���Խ� CCritSec ����Ϊ����״̬�����԰汾�������ҵ�����
* 
* ����Լ������ݣ�ֻ�����Լ�ʱ��������Լ��� CCritSec��������ú�ϵͳ�ĳ�ͻ���������Ч�ʣ�
*   ʹ��Filter�ģ�CAutoLock cAutoLock(m_pFilter->pStateLock());
*     CBallStream::GetMediaType
*     CBallStream::DecideBufferSize
*     CBallStream::SetMediaType
*   ʹ���Լ���(m_Ball��m_rtSampleTime)��CAutoLock cAutoLockShared(&m_cSharedState);
*     CBallStream::FillBuffer��
*     CBallStream::OnThreadCreate
*   
* Filter ͨ��m_State��������״̬(FILTER_STATE)������ͨ�����ص� CBaseFilter::Stop��Pause��Run �ȷ�����֧����ͣ��������.
*   Filter Graph Manager��������Filter������Filter�Ĵ�����������е�״̬ת��(����Render��ʼ��Source ����)������˳����Է�ֹ���ݶ�ʧ��Graph����
*   1. State_Stopped
*   2. State_Paused
*   3. State_Running
*      ���� Running/Paused ͳ�� Active(CBaseFilter::IsActive����TRUE)��//��ʱһ�㲻���Զ�̬�ı����ԣ�
*
*      Control                     Filter                     
*         Run                      Pause ����, �� State_Stopped(0) => State_Paused(1)�������������ŵ�Pin���� Active ����
*                                    ����Transform������� StartStreaming
*                                  Run   ����, �� State_Paused(1) => State_Running(2)�������������ŵ�Pin���� Run ����
*         Pause                    Pause �������� State_Running(2) => State_Paused(1)
*         Resume(ʵ����Run?)       Run   �������� State_Paused(1) => State_Running(2)
*         Stop                     Pause �������� State_Running(2) => State_Paused(1)
*                                  Stop  �������� State_Paused(1) => State_Stopped(0)�������������ŵ�Pin���� Inactive ����
*                                    ����Transform������� StopStreaming
**************************************************************************************************/

#endif //FILTER_THREAD_H