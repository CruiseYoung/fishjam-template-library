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
* IE 7 �ı���ģʽ(Protected Mode) -- Ҳ��IE��Ȩ��(Low Rights)��ͨ�� Process Explorer �鿴���̵�"��ȫ"���ԣ��ɿ�����
*   "Mandatory Label\Low Mandatory Level"��SID(��Ӧ�ı�־λ��"Integrity")�������IE���̵������Լ�����"Low"��
*   "Virtualized" Ϊ Yes
*   ����ģʽ�·��ʵ��ļ�Ŀ¼(�����Լ�����"Low")��
*     1.IE��ʱ�ļ�: %UserProfile%\AppData\Local\Microsoft\Windows\Temporary Internet Files\Low
*     2.ϵͳ��ʱĿ¼: %UserProfile%\AppData\Local\Temp\Low
*     3.Cookies: %UserProfile%\AppData\Roaming\Microsoft\Windows\Cookies\Low
*     4.��ʷ: %UserProfile%\AppData\Local\Microsoft\Windows\History\Low
*     5.�ղؼ�: �ղؼ�Ĭ������ͼ����IE���̷��ʣ���˲���Ҫ������Ŀ¼��
*   ������������⣺
*     1.ͨ������NTFS������(Soft Link)�ķ��������IE����ģʽ�ļ��������⣺linkd ԴĿ¼ ��Ȩ�޵�Ŀ��Ŀ¼
*       ʵ�ʣ���NTFS�ļ�����һ���ؽ�����(REPARSE)������
*       Linkd ��ִ�г����� Windows Server 2003 Resource Kit Tools���߰� ��
*     2.SHGetSpecialFolderPath(NULL, szSavePath, CSIDL_LOCAL_APPDATA , TRUE); //ȡ�� %UserProfile%\AppData\Local
*       CString strFilePath(szSavePath); 
*       strFilePath += TEXT("\\Temp\\Low\\nCapture\\IEToobar\\");  -- ֮��ķ��ʶ��ھ��� Low �����Եȼ���Ŀ¼��
*   
* �������(��Ȩ����˼��) -- �Ѵ�ͳ��IE����һ��Ϊ�����ֱ���ͬ��ȫ�ȼ�������.
*   IE���̱��� -- �����õġ�������Ȩ����ҳ�������
*   IeUser.exe(�û����������) -- ����е���Ȩ������(���籣��ͼƬ��)
*   IeInstal.exe(�����������) -- ��ɸ���Ȩ������(���簲װ������)��
* 
* IE�еĶ���̿���
*   HKLM��HKCR \SOFTWARE\Microsoft\Internet Explorer\MAIN\TabProcGrowth  -- REG_SZ, Ϊ 0 ��ʾ�� Ϊ Medium ��ʾ
*
********************************************************************************************/

namespace FTL
{
    class CFIEUtils
    {
    public:
		//��Window��������ȡIE�ڵ�Docuemnt�ӿ�
		FTLINLINE static HRESULT GetIEDocumentFromHWnd(HWND hWnd, IHTMLDocument** ppDocument);

		FTLINLINE static BOOL IsProtectedModeProcess();
    };

}//namespace FTL

#ifndef USE_EXPORT
#  include "ftlIEHelper.hpp"
#endif

#endif //FTL_IE_HELPER_H
