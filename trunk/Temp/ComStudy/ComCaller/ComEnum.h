#ifndef COM_ENUM_H
#define COM_ENUM_H
#pragma once

/******************************************************************************************************
* ö����(�� IEnumString��IEnumCLSID��IEnumVariant ��)
*   COM ͨ��ö����������Ϊ��Ŀ�嵥�Ĳ����ѭ���ṩ��׼����(Reset��Next��Skip��Clone)������չΪ����(Collection)��
*     Clone -- ������һ������ͬ��״̬(��λ��)��ö�ٶ�����Ϊ��ǰ����ĸ����������ڱ���λ�á�
*   ͨ���ͻ��˵�������ṩ��һ�����ⷽ��������ö�����ӿڵ�ָ��
*   ATL�ṩ��֧�֣�
*     CComEnum -- �� CComEnumImpl ʵ��ʵ�ֵ�ö��������
*     CComIEnum -- ��һ��ö�ٽӿڵĳ�����
*     CComEnumImpl -- ʵ��IEnumXXX�ӿڣ������ڲ����ݴ洢�������У�T* m_begin��
*     CComEnumOnSTL -- �ñ�׼ģ��ʵ��ö�����ӿ�
*   ����������һ�� IEnumString �ӿڣ���ӿڵ� GetEnum([out,retval] IEnumString **ppEnum);���� )
*     typedef CComObject< CComEnum< IEnumString, &IID_IEnumString,LPOLESTR,Copy<LPOLESTR> > > enumString;
*     enumString* pEnum = NULL;   //��ʹ�� new enumString;
*     enumString::CreateInstance(&pEnum);
*     pEnum->Init(m_vecStrings.begin(), m_vecStrings.end(),0,AtlFlagCopy);//���ڲ�vector�����г�ʼ��ö�������������
*       //���� std::vector<LPOLESTR> m_vecStrings;
*       //ָ���� AtlFlagCopy ��������������и�ֵ����ˣ��ͻ�����Ҫ�ͷŻ�ȡ�����ݣ���
*     pEnum->QueryInterface(IID_IEnumString,(void**)ppUnk); //ppUnk ���ظ��ͻ������ʧ�ܣ���Ҫ delete pEnum
*
* ����(Collectioin)�����ڼ��ϵĶ���Ӧ��ʵ��һ�±�׼�����Ժͷ�����IDispatch����ͨ����1Ϊ��ַ
*   Add(IMySubDispatchType* pSubObj) -- ͨ��
*   Remove
*   Count -- ֻ�����ԣ����ؼ�����Ŀ������
*   Item -- ֻ����Ĭ������(DISPID_VALUE)�����ؼ����е�ָ����Ŀ��ͨ������������ IDispatch* ������
*   NeuEnum -- ֻ������(DISPID_NEWENUM)������һ����ʹ�ͻ��Լ��Ͻ��е�����IEnumXXX�ӿ�
*   ���Ͽ���ʹ�� For Each XXX in colXXXs ����ö��
*   ע�⣺1.�����ڲ������ݲ����ɿͻ���ֱ�Ӵ�����Ӧ�����������������������IDL�в�����Ӧ��interface����coclass����
*         2.��������ڲ�����ͨ�� vector<CComVariant> ��������
******************************************************************************************************/

#endif //COM_ENUM_H