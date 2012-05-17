#ifndef FTL_IE_HELPER_H
#define FTL_IE_HELPER_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlIEHelper.h requires ftlbase.h to be included first
#endif

#ifndef FTL_COM_H
#  error ftlIEHelper.h requires ftlCom.h to be included first
#endif


/********************************************************************************************
* ��ȫ����
*   Security Principal -- Windows���εİ�װ��ȫ����
* 
*   DEP -- ����ִ�б���
*   MIC(Mandatory Integrity Control) -- ǿ�������Կ��ơ�Vista�е����а�ȫ�Զ���ͽ��̶���һ�������Լ���
*     �����Լ���(IL)�͵Ľ��̲����޸ļ���ߵ��ļ���ע�����
*     1.�����μ��� -- �����ø����������ӻỰ
*     2.Low(��) -- ����ģʽ�µ�IE�����, �� Untrusted ��ϵͳȨ�����г���ֻ�ܴ�ȡ�ʹ�ȡ�����·��λ�ã��� "Temporary Internet Files\Low"��
*       ע�⣺δ���û�ͬ������ص���Ϊ�����ڵͼ���
*             �û��ֶ������ػ�����ִ�м���һ������ �������еȼ���
*             ��һ������Ȩ�޵��û���ͬ�������(���簲װһ��ActiveX�ؼ�)�������ڸ������Լ���
*     3.Medium(�е�) -- Ĭ������£��û�����Ĵ��룬��Windows Explorer�����������, ʹ��"User"Ȩ�ޣ��ܶ�д�û����ļ���ע�����
* ����4.High(��) -- ������ administrator�û� ������Ȩ�޺�ĳ���
* ����5.System(ϵͳ) -- �ں˼����Windows�ļ�
*     6.�������̼��� -- ������ߣ�ֻ����ϵͳ��Ҫ��ʱ��Żᱻʹ��
*   Ӧ���ļ���ע�����⻯ -- ��ָ��λ�ý��ж�д��ʱ��ᱻ�ض���ÿ���û������⻯��������
*     C:\Users\%USER_NAME%\AppData\Local\Microsoft\Windows\Temporary Internet Files\Virtualized\XXXX -- XXX���ּ���Ӧ�õ� C:\ProgramData\MyProgramData ��
*     ����ע�����п��ƣ���HKLM\SYSTEM\CurrentControlSet\services\luafv
*   SFP -- ϵͳ�ļ�����, Win2K ��ǰ���ļ���������
*   UAC(User Account Control) -- �û��ʻ�����
*   User Interface Privilege Isolation -- �û�������Ȩ����
*   WFP -- Windows�ļ�����, Win2Kʱ���룬ֻ�����ļ�
*   WRP -- Windows��Դ�������Թؼ��ļ����ļ��к�ע���ֵ���б���
*
* IE 7 �ı���ģʽ(Protected Mode)
********************************************************************************************/

namespace FTL
{
    class CFIEUtils
    {
    public:
		FTLINLINE static BOOL IsProtectedModeProcess();
    };

}//namespace FTL

#ifndef USE_EXPORT
#  include "ftlIEHelper.hpp"
#endif

#endif //FTL_IE_HELPER_H
