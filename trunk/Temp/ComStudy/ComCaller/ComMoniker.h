#ifndef COM_MONIKER_H
#define COM_MONIKER_H
#pragma once

/******************************************************************************************************
* ͨ�� MkParseDisplayName �������Դ��ַ����õ���Ӧ�� IMoniker ����(�μ�amcap����)
* Moniker(���) -- ���ֶ���(Ҳ��COM����)Ϊ��������ṩ�˷��Ż��ı�ʾ��������ʾCOM��������֣�
*   ����ͨ�����ֽ��������������ͻ�ͨ�����ֶ�������com��������ӣ����ֶ����ǿͻ������֮�������
*   ��ʾ���ָ�������!������\������/������:����[��
*     Excel �е� Sheet!��� ��ʽ���� �������ֶ��� �����ӣ�
*   �󶨣�
*     �����״̬������״̬��������״̬������״̬
*     �󶨣�ʹ����ӱ���̬�Զ���������̬ ���� ������ӣ�����Ҳ����Ϊ���������ܶ���
*     ͨ���� �ͻ� -> IClassFactory -> COM ����
*     ʹ��Moniker���ͻ� -> IMoniker -> COM ����
*       IMoniker::BindToObject�󶨹��̣�
*         1.���ROT��
*         2.�ֽ�,pmkRight : pmkLeft�����ұ߲��ַ���
*         3.����,pmkRight->BindToObject(...,pmkLeft,...)
*         4.ִ��,pmkRight->BindToObject�����pmkLeftΪ�����ֶ��������ֹѭ����
*           ����,pmkRight����Ҫ����pmkLeft->BindToObject���Ӷ��γ����������ѭ���󶨹���
*           ��:File!Item1!Item2 �İ󶨺͹������
*              �ͻ�������� pComp->BindToObject ( �������ֶ��󱻲�ֳ�pmkFileItem1��pmlItem2������)
*                             => pmkItem2->BindToObject 
*                               => pmkFileItem1->BindToObject ( pmkFileItem1����ֳ�pmkFile��pmlItem1������ )
*                                 => pmkItem1->BindToObject
*                                   => pmkFile->BindToObject(�����ļ����󲢷�����IOleItemContainer�ӿ�ָ��)
*                                     => 
*  ʹ�÷�����
*    1.�ͻ��������ֶ��� -- CreateFileMoniker ��CreateItemMoniker
*    2.�󶨵����ֶ�����ָ�Ķ��� -- IMoniker::BindToObject / BindMoniker
*  �������ֶ��� -- CreateGenericComposite
*    �������ҵ�˳����ϣ��������ɣ��硰�ĵ��ڲ��ĵ��ӱ��
*  ROT(Running Object Table)�� -- COMʹ��ROT�����ǰϵͳ���������еġ��Ѿ���ע������ֶ���
*    �ͻ�����GetRunningObjectTable��������ROT�����͵���DirectShow��ע�ᵽ Graph Edit ��
*    IRunningObjectTable
*  �󶨻�������(IBindCtx) -- CreateBindCtx��Ҳ��COMʵ�ֵ�ϵͳ����
*    �����ѱ��󶨵Ķ��� -- RegisterObjectBound��RevokeObjectBound��ReleaseBoundObjects
*    ����󶨲�����(�ļ�����ģʽ����ʱ���õ�) -- SetBindOptions��GetBindOptions
*    ����󶨹����еĶ������ -- RegisterObjectParam��GetObjectParam��EnumObjectParam��RevokeObjectParam
*   
* IOleItemContainer -- ?
*
* COM���ֶ������
*   COM�ṩ��ϵͳ���ֶ��� -- 
*     �ļ����ֶ���(CreateFileMoniker)
*     �������ֶ���(CreateGenericComposite)
*     �������ֶ���(CreateItemMoniker)
*     ��-���ֶ���(CreateAntiMoniker)
*     ָ�����ֶ���(CreatePointerMoniker)
*     �����ֶ���(CreateClassMoniker)
*   URL���ֶ���
*     �첽���ֶ��� -- ʵ����IAsyncMoniker�ӿڣ������ӿڣ�IEnumFORMATETC��IBindStatusCallback��
*   �Զ������ֶ���(����ʹ��) -- ������Ҫʵ��IMoniker�ӿڵĳ�Ա������ͬʱ�ṩһ�������Զ������ֶ����;��
*
* ���ֶ����Ӧ���뷢չ
*   ��OLE���Ӷ������Ҫ��������
*   COM+���������µ����ֶ��󣬱���queue:��new:
*   ��Ϊ�ͻ���COM����֮�����ӵ�һ��ǿ�������ֶ�
*   VBScript�з��ʶ������Ҫ����
*
* MFC�����ֶ����֧��(VC�������ӣ�OClient��Scribble)
*   COleLinkingDoc
*
* ͳһ���ݴ��䣨UDT��
*   ���ݴ������
*     IDataObject -- ��ʾ��Ϣʵ������ݶ���
*     ����Э�� -- һ��Ӧ�ó�����εõ���һ��Ӧ�ó������ṩ�����ݶ���IDataObject�ӿ�ָ�룬������Ĵ���Э��Ϊ�����塢��-��
*   ͨ�������崫������
*   ��-�����ݴ���Э��
*     Դ��ʵ�� IDataObject/IDropSource �ӿڣ����� DoDragDrop �������ú����ڲ���һ��ѭ���������ϼ�����ͼ��̵�״̬��
*         �Լ�ѯ�ʡ��š�Ŀ�괰�ں͡�Դ�����ڣ���������Ӧ
*     Ŀ�꣺ ʵ�� IDropTarget �ӿڣ����� RegisterDragDrop �����Ѷ����봰����ϵ����
******************************************************************************************************/
#endif //COM_MONIKER_H
