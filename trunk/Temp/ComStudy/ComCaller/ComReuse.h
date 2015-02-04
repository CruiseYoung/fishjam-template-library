#ifndef COM_REUSE_H
#define COM_REUSE_H
#pragma once

/***********************************************************************************************************
* C++  -- Դ�������ã������޶������ֿ��ܳ�ͻ���������ô�������¹������ͣ�
* DLLs -- �����Ƽ��������,ֻ�ܱ�C�����C���ù淶������ʹ��(��ƽ���)
* COM  -- �����Ʊ�׼�� ͨ���ӿڽ������廯��ʵ�����ö����Ǵ������á�
* 
* �������(����) -- �� FinalConstruct �д����ڲ��������FinalRelease���ͷ�.
*   1.�ۺ�(Aggregation) -- �ڲ�����Ľӿ�ֱ�����⹫��(����ֱ��QueryInterface���ڲ�����Ľӿ�)���ⲿ����ڲ�
*       ����Ҫ��ʹ���ڲ�����Ĺ��ܣ���������������ڶ���ͨ���ⲿ����� IUnknown ��ɡ�
*       ATLĿǰֻ֧�ֽ���������ľۺϡ�
*     ����ͨ��ѡ��֧��(����ҳ�� Aggregation)��
*       Yes  -- �� CComCoClass Ĭ��֧�֣��������ر�ĺ�(DECLARE_AGGREGATABLE��)
*       No   -- ���� DECLARE_NOT_AGGREGATABLE ��
*       Only -- ���� DECLARE_ONLY_AGGREGATABLE ��
*     ���������ַ�ʽ��
*       a.��ѡ��ؼ���(Selective Aggregation) -- ֻ��QIָ���ӿ�ʱ�������ڲ��������(�Ƽ�)
*           if(IID_XXX == riid) {return m_pInnerUnk->QueryInterface(...);}
*       b.äĿ����(Blind Aggregation) -- ��QI����δ֪�Ľӿ�ʱ���������ڲ��������
*           if(IID_YYY == riid){...} else {return m_pInnerUnk->QueryInterface(...);}
*     �����ڲ����ʱ
*       a.�����ⲿ�����IUnknown�ӿڣ�ͨ������ DECLARE_GET_CONTROLLING_UNKNOWN ���
*         ���� GetControllingUnknown ������ã�;
*       b.�����ڲ����������������IUnknow�ӿڣ�������Ϊ�ⲿ����ĳ�Ա����(IUnknown * m_pInnerUnk)
*       c.�ӿ�ӳ���м������µ�ӳ�䣬ʹ�ÿͻ���ͨ���ⲿ�������QIʱ���ܻ���ڲ�����Ľӿڣ�
*         COM_INTERFACE_ENTRY_AGGREGATE -- ��ѡ��ؼ���ָ���ӿ�
*         COM_INTERFACE_ENTRY_AGGREGATE_BLIND -- äĿ���������κνӿ�(ֻ����һ���ڲ��������äĿ���ϣ�)
*         COM_INTERFACE_ENTRY_AUTOAGGREGATE(_BLIND) -- ������ӿ�ʱ�ٴ����ڲ����(��������FinalContruct�д������ͷ��أ�)
*   2.����(Containment) -- ������Ƕ��һ���ָ����Ϊ��Ա�������ڲ�����Ľӿ��Լ�ӵķ�ʽչʾ���ⲿ������ڲ����
*       �Ŀͻ��˳���(COM�Ļ���Ĭ��֧��)��
*       ͨ���ⲿ����ķ���(���Ժ��ڲ�����ķ���ͬ����Ҳ���Ը��ĳ��Լ��ķ���)�����ڲ������ʵ�����ṩ���ܡ�
*     �����ڲ����ʱ
*       a.�ڼ̳��б��м����ڲ�����Ľӿ� -- ֱ��ʹ���ڲ��ӿڷ����ķ�ʽ��Ҳ�����Զ��������ӿڣ�
*       b.IDL�м����ڲ������������
*       c.�ӿ��б�
*       d.����ʵ��ʱ��ͨ���ڲ��ӿڵ�ָ�������ʵ�֡�
*
* CComAggObject �� CComObjectRootEx �̳������Σ��ṩ��IUnknown������ʵ��
*  1.ֱ�Ӽ̳� -- IUnknownʵ�ְѵ���ת�����ⲿ���ƶ��󣬰����������ں�ʵ����ݷ���
*  2.ͨ�������û���CComContainedObject���������ʵ����Ӽ̳� -- IUnknown���ⲿ���ƶ�������˽����;��
*    ��������ά���ڲ����������ںͽӿڲ�ѯ
*
*
* ���⣺
*   �ͻ��˵���ʹ���˾ۺϹ��ܵ������Ӧ�ð����ĸ���ͷ�ļ�����������Ķ���Ҫ��
***********************************************************************************************************/

#if 0
���� CComAggObjecct ����������IUnknownʵ�֣��Զ����CMyComClass������ڲ��޸�Դ�������¼�֧�ֶ����ķ�����Ҳ֧�־ۺϼ���
CMyComClass::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppv)
{
    *ppv = NULL;
    if (pUnkOuter)
    {
        CComAggObject<CMyComClass>* pObj = new CComAggObject<CMyComClass>(pUnkOuter);
    }
    else
    {
        CComObject<CMyComClass>* pObj = new CComObject<CMyComClass>;
    }
}
#endif

#endif //COM_REUSE_H