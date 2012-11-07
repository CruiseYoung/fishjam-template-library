#ifndef INTEL_MEDIA_SDK
#define INTEL_MEDIA_SDK
#pragma once

/*************************************************************************************************
* Intel��Media SDK��һ��ǿ�������Ƶ�����⣬��������Intel���Կ�ƽ̨�ϣ��ṩӲ�����٣������ṩ��Ӧ����������ʵ�֡�
*   ��ѡ��ǿ�Դ��
*   ������Ϣ��http://software.intel.com/en-us/articles/intel-media-software-development-kit-intel-media-sdk/
* 
* ע�⣺
*   1.Intel Media SDK Ŀǰֻ֧��NV12(Ӣ�ض��������Ƶ��ʽ������Ӣ�ض�Ӳ��ƽ̨����ԭ��̬֧��)
*     MSDK��ʽת��: http://software.intel.com/zh-cn/blogs/2010/07/26/msdkyv12nv12/
* 
* Media SDK֧�� VC1,H264 �� MPEG2 ��ʽ��Intel�Կ�ƽ̨��Ӳ�����١�
* ��Media SDk�У���������Ƶ��һЩ�ؼ����ĵı��롢�����㷨������Intel IPP�⣬���ڴ˻����϶Դ������ܽ������Ż�����ߡ�
* �ṩ�˷ḻ��API�Թ��ͻ����ο�������������չ���Ӷ�֧���κε�������ͼ��Ӳ�����������graphics hardware solutions���ͱ���뷽����
*   �ṩ�˱�׼��DShow Filters��
* 
* SDK�ı�̽ṹ��ͨ����ķַ��㣨Dispatcher layer����¶��Ӧ�ó���� -- �ṩ��ͨ�õ�������ƵԤ���������Ľӿڣ�
* �ò��ṩ�� �ض����ܡ�
*   1.�ַ�����dispatcher����λ��ǰ���Կ����豸��������
*   2.���ݶ�λȥSDK�в�������ʵ�ʵ�ַ�ʽ��
*   3.���û���ҵ��ʺϵ�ƽ̨��ص�ʵ�ַ�ʽ��dispatcher�ᶨλ��SDK�������ʵ�ֵķ�ʽ
*
* Media SDK ��ʼ����������Ĳ���(Ҳ�ṩ�˼򵥷�װ�õ�C++�ӿڣ��� MFXVideoSession/MFXVideoENCODE �� )��
*  1.�����ͳ�ʼ��һ������� mfxSession -- MFXInit ��������Ҫָ��������ʵ�ַ�ʽ���Զ�ѡ��Ӳ�����������
*    �����Autoʱ������ʹ�� MFXQueryIMPL ȷ�ϵ�ǰ��ʵ�ַ�ʽ��
*  2.�����ͳ�ʼ�������� -- MFXVideoDECODE_Init
*  3.�����ͳ�ʼ�������� -- MFXVideoENCODE_Init
*  n.ʹ����Ϻ�ʹ�� MFXClose �ر�Session
*
* Media SDK �ṩ���첽����(MFXVideoDECODE_DecodeFrameAsync��MFXVideoVPP_RunFrameVPPAsync��MFXVideoENCODE_EncodeFrameAsync)
*   �ȣ��Ƚ��ʺ�Ӳ������루һ���߳̾Ϳ�ʵ�ָ����ܣ�����������ʱ����ʹ����ˮ�߽ṹ��

*************************************************************************************************/



#endif //INTEL_MEDIA_SDK