#ifndef COM_PERSIST_H
#define COM_PERSIST_H
#pragma once

/******************************************************************************************************
* IPersistXXXX -- ����ĳ�����(�־û�)����һ���ǳ����ã����ҹ���ǿ��Ľӿڼ��塣
*   �����ߣ��������ṩ�洢���ʣ����ļ����ڴ桢ע������ȣ���
*     ��Ҫ�����ʱ�򣬵�����ͨ�� IPersistXXX::Save() �ӿں��������ȥ�Լ��洢������Ϣ���������߸������ù��Ĵ洢��ʽ�ʹ洢���ݣ�
*     ��Ҫ��ԭ��ʱ�򣬵����ߴ򿪴洢���ʣ�ͨ�� IPersistXXX::Load() �ӿں���������Լ�ȥ��ȡ������Ϣ����ɳ�ʼ�������á�
*   COM����ֻҪ�� IPersistStreamInitImpl��IPersistStorageImpl��IPersistPropertyBagImpl �����ӿ�ʵ��������һ���̳У�
*     ����������Ϊ m_bRequiresSave �ĳ�Ա�������ٰѽӿ���ӵ� COM_MAP �У�COM�����֧�ֳ־û� -- �Զ��Զ������Ա�(PROP_MAP)�е����ݽ��г־û�
*   ΢����ĳ����Խӿ�(ֻҪ���Լ�д������и�����Ҫʵ������һ���򼸸������Խӿڼ���)��
*     IPersist -- ���г����Խӿڵĸ���ֻ��һ������ GetClassID ���ڷ�������� CLSID ��(�� GetObjectCLSID )��
*       �Ա�������ܱ��������Ϊ���� CoCreateInstance() ��������á�
*       +- IPersistStream -- ����(IStream)�ж�д���������Ϣ
*         +- IMoniker
*       +- IPersistStreamInit(���) -- ������ IPersistStream������������һ������ InitNew() �������һ��Ĭ�ϵ�������Գ�ʼ����
*       +- IPersistMemory -- ʹ���ڴ��
*       +- IPersistStorage -- ʹ�ô洢 IStorage(һ�� IStorage �п����ж�� IStream)
*       +- IPersistFile -- ʹ���ļ�
*       +- IPersistPropertyBag -- ʹ�����԰�������-ֵ �ԣ����ı���ʽ������Ϣ���� <param name="��������" value="ֵ"> ��
*         �� IE ������У�HTML Ƕ�� ActiveX �ؼ�ͨ��ʹ��������������Ա���COM����е�����ֵ��
*         ��Ҫ�ڵ�����(����)��ʵ�� IPropertyBag �ӿڡ�
*       +- IPersistPropertyBag2 -- ��չ�� IPersistPropertyBag �ӿڡ��ṩ�˸��ḻһЩ�����Թ����ú�����
*       +- IPersistHistory -- ������ IE �ϣ������û���� WEB ҳ��ʱ�洢�Ͷ�ȡ״̬�ĳ����Խӿڡ�
*     IPersistMoniker -- ��������(moniker)�洢�Ͷ�ȡ״̬�ĳ����Խӿڡ�
*
*******************************************************************************************************/
#endif //COM_PERSIST_H