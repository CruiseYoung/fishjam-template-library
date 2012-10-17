#ifndef FILTER_DEBUG_H
#define FILTER_DEBUG_H

#pragma once


/***************************************************************************************************
* DbgLog -- ϵͳ�ṩ����־������������úͼ�� Type/Level(DbgSetModuleLevel/DbgCheckModuleLevel)
*   Type -- LOG_TIMING, LOG_TRACE, LOG_MEMORY, LOG_LOCKING , LOG_ERROR
*   Level:DWORD, 0 ����Ҫ(�������)��ÿ�����Ͷ��ж�Ӧ��Level,�� CONNECT_TRACE_LEVEL
* 
* ע���� HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\DirectShow\Debug ��¼��ÿ��ģ��ÿ�����͵���־����ȼ������Էֱ�����,
*   �� GLOBAL ��������ȫ�����ã����õ�ֵ������ڶ�Ӧ��Level������Ч
*   CUSTOMx -- 
*   ERROR
*   LogToFile -- ����Ĭ�������Ŀ�ĵ�
*     1.ΪNULLʹ�� OutputDebugString ���
*     2.Ϊ"Console"ʱ���ڿ���̨��������������⣿��
*     3.Ϊ�ļ�·��ʱ��ʹ���ļ���ʽ���
*   TIMEOUT -- ���� DbgWaitForSingleObject �Ⱥ����ĳ�ʱֵ�����Է��㷢������(ע���û���ͣ����)
*     Ҳ����ͨ�� DbgSetWaitTimeout �����趨
*
*
*   DbgDumpObjectRegister -- ��ʾ��ǰ��������Ϣ�����Լ���ڴ�й©),Debug״̬���Զ����� ?
*   DbgSetModuleLevel(LOG_TRACE|LOG_MEMORY , 5);  -- ������־��������� �� �ȼ�
*   DbgSetAutoRefreshLevels -- �����Ƿ��Զ����ע���ı仯(Ĭ�ϲ���飬���Ļ���Ӱ������)
*   DisplayTypeInfo -- ��ʾý������,Terse at level 2, verbose at level 5
*   DisplayPinInfo -- ��ʾPin��Ϣ
*   DumpGraph(pGraph, 1) -- ��ʾ Filter Graph ����Ϣ
*   ϵͳ�ṩ�� DbgLog �����������Ϣ
*   CAutoTrace -- �Զ���ÿ���������и��٣�Ӧ��ʹ�� AMTRACEFN / AMTRACE ��
*
*   Sample �� NullInPlace �ж����� CTextMediaType �࣬���Խ�ý�����͵�GUIDת��Ϊ�׶��� CLSID_XXX 
*     -- ʹ��ȫ�ֵ�GuidNames[yourGUID]�������
***************************************************************************************************/

#endif //FILTER_DEBUG_H