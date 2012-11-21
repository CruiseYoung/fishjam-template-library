#ifndef DIRECT_SHOW_DMO_H
#define DIRECT_SHOW_DMO_H
#pragma once

/*************************************************************************************************
* DMO(DirectX Media Objects)����һ��������������������ʵ���� IMediaObject �ӿڡ�
*   ʹ����(Stream)�����뵽�����ת����ʽ��"����--IMediaObject::ProcessInput/ProcessOutput"
*   ��"�͵�--IMediaObjectInPlace::Process"�������֡�
*   DMO��ʹ���߷��������ڴ棬�������ݺ��͸�DMO(ͬ��)������DMO���������ȡ������������ݡ�
* 
* �ŵ� -- ΢���Ƽ�ʹ��DMO�滻Filter(�������ͽ��������Ƽ��������)
*   1.���Բ��ü���FilterGraph����ʹ���书�ܡ�
*   2.��DirectShow�п���ʹ��DMO Wrapper Filter ��װ�����DMO���뵽FilterGraph�С�
*   3.����DirectSound��Ӧ�ó���Ҳ����ʹ�ø�������Ч����DMO(�μ� dmodemo ����)
* 
* ����ý������ע�⣺
*   1.���ڽ���DMO��һ���������������ϵ�ý�����ͣ�Ȼ�����������ѡ��һ�������ʽ��
*     ���ڱ���DMO��һ��������������ϵ�ý�����ͣ�Ȼ����������������һ�������ʽ��
*
* ��DirectShow��ʹ��DMO -- ϵͳ�ṩWrapper���ۺ�DMO
*   1.ʹ��CoCreateInstance����DMO Wrapper Filterʵ��(CLSID_DMOWrapperFilter),
*     ���IBaseFilter�ӿڣ��Ժ�ͨ���ýӿں�Graph������
*   2.��ȡ IDMOWrapperFilter �ӿڣ��������� Init ������ָ��DMO��CLSID������Ŀ¼
*     (�� DMOCATEGORY_VIDEO_ENCODER / DMOCATEGORY_AUDIO_EFFECT )
*   ���ƣ�
*     a.DMOWrapperFilterֻ֧�ֵ����롢�������Transform��DMO;
*     b.DMOWrapperFilter������Pinֻ֧��ʹ��IMemInputPin�ӿڴ�������(��ģʽ);
*     c.��ǰ��DES(DirectShow Editing Services)��֧��DMO��Ч��(Effect)�͹���(Transition)
*
* ͨ��ϵͳö�ٵķ�������DMO -- DMOEnum
*   ͨ�� IDMOWrapperFilter �ӿ�ʹ��(<dmodshow.h>)
*   
* DMO�Ŀ��� -- ATL��ʽ��COM���(��û�и����������ࣿ)
*  1.֧�־ۺ�(���Ա�DMOWrapperFilter�ۺ�)���߳�ģ��ΪBoth;
*  2.ʵ��IMediaObject�ӿ� -- �Ѿ���IMediaObjectImplʵ��,�ӿ���Ҫ���صĺ������ڲ�������ΪInternal##XXX -- �ɺ�INTERNAL_CALLʵ��
*    class CMyDMO : public IMediaObjectImpl<CMyDMO, 1(����������),2(���������)>
*                 , public IMediaObjectInPlace
*    ʹ�� LockIt lck(static_cast<_DERIVED_ *>(this)); �����Զ���Χ��
*  3.�������ЧDMO������ʵ��IMediaObjectInPlace�ӿ�(������DirectMusic��DirectSound��ʹ��)
*
* ���ԣ�dmotest.exe -- û���ҵ���
* ע�᣺DllRegisterServer �е��� DMORegister ����ע��DMO��������Ϣ��_Module.RegisterServer ע��COM��Ϣ
* 
* //1.����
*   IMediaObject* pDMO = NULL;
*   CoCreateInstance(CLSID_XXX,...(void**)&pDMO);
* //2.�Ը����������������ý�����͵�����--��ȡ�ø������ϵ���ѡ(Preferred)ý������
*   DMO_MEDIA_TYPE mt = {0}; //ý������
*   MoInitMediaType(&mt,sizeof(WAVEFORMATEX));
*   mt.majortype = MEDIATYPE_Audio;
*   mt.subtype = MEDIASUBTYPE_PCM;
*   mt.formattype = FORMAT_WaveFormatEx;
*   WAVEFORMATEX* pWave = reinterpret_cast<WAVEFORMATEX*>(mt.pbFormat);
*   //...
*   pDMO->SetInputType(0,&mt,0); //����0����ý������
*   MoFreeMediaType(&mt);
**************************************************************************************************/


#endif //DIRECT_SHOW_DMO_H