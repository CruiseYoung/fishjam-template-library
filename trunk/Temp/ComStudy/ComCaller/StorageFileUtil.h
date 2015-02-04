#pragma once


/***********************************************************************************************
* �����ĵ�-- WORD�ļ����Ǹ����ĵ���ʹ�� VC6�еĸ����ļ������ DFView.exe �鿴����֧������·������
* 
* �ļ��洢���ͷ�Ϊ��
*   �ǽṹ���ļ�������±�����
*   ��׼�ṹ���ļ�������ӱ��
*   �Զ���ṹ���ļ���
*   �����ļ���ʹ�������ļ�ϵͳ�ķ�ʽ�����ļ������洢 -> �Ӵ洢 -> ���������ص㣺
*     1.ʹ��ָ�빹���һ�������й�������ʹ�õ��ǵ���ָ�룬��˵�����λ������ʱ�����λ����ǰ��λҪ�졣
*     2.�������������������ݵĿռ䣬�洢��λΪ512�ֽڡ�
*     3.��ͬ�Ľ��̣���ͬһ�����̵Ĳ�ͬ�߳̿���ͬʱ����һ�������ļ��Ĳ�ͬ���ֶ��������ţ�
*     4.�����ļ����ṩ�˷ǳ�����ġ��������ʡ����� -- �ܷ�������ӡ�ɾ����
*     5.ɾ���ļ��е����ݺ���Ҫ��������
*     6.���������Թ���ͬһ�������ļ� -- �����ĵ�
*   ��������
*     ���洢��������������ļ�ϵͳ������Լ��
*     ���Ȳ�����32���ַ�
*     ���ַ�ʹ�ô���32���ַ���С��32���ַ���Ϊ���ַ�����������
*     ����ʹ���ַ���\������/������:���͡�!��
*     ���֡�.���͡�..��������
*     ���ֱ�����Сд�����Ƚϲ�����Сд�޹�
*   �ײ���ƣ�ILockBytes���󣬰Ѵ洢����������һ�㻯���ֽ�����
* 
* �����ͽӿ�
*   StgCreateDocfile    <== ����һ�������ļ����õ����洢����
*   StgCreateDocfileOnILockBytes <== ?
*   StgOpenStorage      <== ��һ�������ļ����õ����洢���� 
*   StgIsStorageFile    <== �ж�һ���ļ��Ƿ��Ǹ����ļ� 
*   WriteClassStg       <== дCLSID���洢�У�ͬIStorage::SetClass() 
*   ReadClassStg        <== ����WriteClassStg()д���CLSID���൱�ڼ򻯵���IStorage::Stat() 
*   WriteClassStm       <== дCLSID�����Ŀ�ʼλ�� 
*   ReadClassStm        <== ����WriteClassStm()д���CLSID 
*   WriteFmtUserTypeStg <== д���û�ָ���ļ������ʽ�����Ƶ��洢�� 
*   ReadFmtUserTypeStg  <== ����WriteFmtUserTypeStg()д�����Ϣ������Ӧ�ó�������ж��Ƿ�������Ҫ�ĸ�ʽ���ݡ� 
*   CreateStreamOnHGlobal<== �ڴ��� HGLOBAL ת��Ϊ������ 
*   GetHGlobalFromStream<== ȡ��CreateStreamOnHGlobal()������ʹ�õ��ڴ��� 
*
*   IStorage 
*     CreateStorage     <== �ڵ�ǰ�洢�н����´洢���õ��Ӵ洢���� 
*     CreateStream      <== �ڵ�ǰ�洢�н����������õ������� 
*     OpenStorage       <== ���Ӵ洢���õ��Ӵ洢���� 
*     OpenStream        <== �������õ������� 
*     CopyTo            <== ���ƴ洢�µ����ж���Ŀ��洢�У��ú�������ʵ�֡������ļ����ͷ���Ƭ�ռ䡱�Ĺ��� 
*     MoveElementTo     <== �ƶ�����Ŀ��洢�� 
*     DestoryElement    <== ɾ������ 
*     RenameElement     <== ���������� 
*     EnumElements      <== ö�ٵ�ǰ�洢�����еĶ��� 
*     SetElementTimes   <== �޸Ķ����ʱ�� 
*     SetClass          <== �ڵ�ǰ�洢�н���һ���������������������CLSID���Ժ���Ը���CLSID���ҹ����ĳ�����Word�б���Excel��
*     Stat              <== ȡ�õ�ǰ�洢�е�ϵͳ��Ϣ 
*     Release           <== �رմ洢���� 
*  
*   IStream
*     Read              <== �����ж�ȡ���� 
*     Write             <== ������д������ 
*     Seek              <== ��λ��дλ�� 
*     SetSize           <== �������ߴ硣���Ԥ��֪����С����ô�ȵ����������������������� 
*     CopyTo            <== ���������ݵ���һ���������� 
*     Stat              <== ȡ�õ�ǰ���е�ϵͳ��Ϣ 
*     Clone             <== ��¡һ�������󣬷�������еĲ�ͬģ�����ͬһ�������� 
*     Release           <== �ر������� 
*
*   IPersistStorage -- �ø����ļ��Ĵ洢(Storage)����������/��ȡ����
*   IPersistStreamInit -- �ø����ļ�����(Stream)����������/��ȡ����
*
* �������( Commit��Revert )
*   ��STGM_TRANSACTED��־Ϊ���������������Ҫ���Ľ϶�ϵͳ��Դ
*   Commit������
*     STGC_DEFAULT
*     STGC_OVERWRITE
*     STGC_ONLYIFCURRENT
*     STGC_DANGEROUSLYCOMMITMERELYTODISKCACHE
***********************************************************************************************/
class CStorageFileUtil
{
public:
    CStorageFileUtil(void);
    ~CStorageFileUtil(void);
public:
    //�������ĵ��Ľṹ��ʾ��������δʵ����
    HRESULT DumpStorageFileInfo(LPCTSTR pszFilePath);
    HRESULT CreateDemoStorageFile(LPCTSTR pszFilePath);
};
