#ifndef COM_AUTOMATION_H
#define COM_AUTOMATION_H
#pragma once

/***************************************************************************************************
* �Զ����ӿڵĽ���
*   1.��Ҫ��һ��������ʵ����֧�ֶ��˫�ӿ�
*   2.��Ҫ�ѿ����õġ���̬��COM�ӿ���Ƴ�˫�ӿ�(����ͨ�ӿڲ�Ҫ��˫�ӿ�)
*   3.�����֧�ֽű��ͻ������鶨��һ��������˫�ӿ�����¶������й���
*   ����COM������ò�Ҫ�̳��Զ��IDispatch�ӿ�(���ɣ��ű��ͻ����ܻ��ȱʡ˫�ӿڵķ���������)
*  
* dispidӳ�䵽�麯����������� 3(IUnknown) + 4(IDispatch) + dispid(��Ӧ�ĺ���) - ƫ�ƻ���(ͨ���ǵ�һ��dispid��ֵ)
* 
* �Զ���(Automation) -- ͨ�� IDispatch �ӿ��ṩ���(Late Binding)��ʵ�ֶ�̬���ã���Ψһ�����ڽű����õĻ��ơ�
*   �������⹫������Ӧ�ó���Ĺ���(ͨ�����Ϳ� tlb �ṩ)�����а�����ʵ��IDispatch�ӿ������ȫ����Ϣ���� DispID�����ò����ȣ�
*   ����ʵ���� IDispatch �ӿڵ���������ڽű��ȡ���󶨡���ͨ���ĸ������ṩ���ܣ���ʵ�ֵ����Զ�����������еĺ�����
*     GetTypeInfoCount  -- ������ṩ�������Ϳ�(IDispatch �ӿڵĸ���)
*     GetTypeInfo   -- ȡ����Ҫ�����Ϳ�(ָ�� IDispatch �ӿڶ�Ӧ�� ITypeInfo,  )
*     GetIDsOfNames -- ���ݺ������֣�ȡ�ú������(DISPID)��Ϊ���� Invoke ��׼��
*     Invoke -- ���ݺ�����ţ�ִ�к�����ע�� DISPPARAMS ����Ĳ��������������˳���š��磺Object.Method(arg1, arg2, arg3)��
*       pDispParams�����У�cArgsΪ3(��3������)��rgvarg[0]��Ӧarg3��...��rgvarg[2]��Ӧarg1
*       ����ʹ����������(cNamedArgs��ʾ���������ĸ���)������ʱ�����ܴ����Լ����
*       �������ù������������쳣����Ϣ�ᱣ����pExcepInfo�С�������Invoke����ֱ�����pExcepInfo�ṹ��
*         Ҳ����ͨ���ص�����(pfnDeferredFillIn)�ɿͻ��������
*     RemoteInvoke -- Զ�̰溯��(�������)
*   ִ��Ч�ʱȽϵ�(������������ţ���ͨ����ŵ���)����ʹ�÷��㣬����ʹ�� CComDispatchDriver  ����ָ�����
*
* IDispatch -- �� VTable ��ʽ�Ľӿ��������ΪDispInterface(�ɷ��ӿ�)�ļ�Ӳ㣬ͨ����ѯ(GetIDsOfNames)
*   ��ú�������Ӧ�ķַ�ID(DispID)����ͨ���ɷ������(Invoke)����һ�����ֻ����һ��Ĭ�ϵ�IDispatch�ӿڣ�
*   �����Ҫʵ�ֶ������Ҫʹ�� COM_INTERFACE_ENTRY2(IDispatch,IMyMath) ��ķ�ʽ˵���� IMyMath �ṩĬ�ϵ� IDispatch �ӿڣ�
* ITypeInfo -- ����˵��һ������TypeLibrary��һ�����͵�COM������ӿڣ������Ի�ȡ��������Ϣ��
*   Invoke�ǰ�DispIDʹ��TypeLibrary�еĽӿڶ��壬������麯����Index��Ȼ��ͨ���麯����(Vtable)��ֱ�ӵ��ýӿڵĺ���
* 
* ͨ�����Զ���������� LPDispatch ���͵����ԣ������Զ������͵����ԣ����Ϳ���ͨ�����Ի�ȡ�������Զ�������
*   �����������ã��磺 obj.Document.Write <== Document ��obj��һ�� LPDispatch ���ԣ�ָ������ IDocumnet �Ľӿ�
*   IDL��IAAA��������[propget, id(1), helpstring("property BBB")] HRESULT BBB([out, retval] LPDISPATCH *pVal);
*   STDMETHODIMP CAAA::get_BBB(LPDISPATCH *pVal) //��IBBB��ΪIAAA�ӿڵ� LPDispatch �����������
*   {
*      QueryInterface(IID_IBBB,(LPVOID *)pVal);  //IBBB �ǵڶ���IDispatch�ӿ�(���ж�������Addһ��ķ���)
*      return S_OK;
*    }
*
* �Զ������ݵ��������ͣ�����Ҫ����� Proxy/Stub��ͨ����׼��ͨ�õ�����(OleAut32.dll)����Marshaling��
*   BSTR��CURRENCY��DATE��IUnknown*��IDispatch*��SafeArray��VARIANT��VARIANT_BOOL��CY(��������)
*   ������׼��������(�� short��long��float��double ��) -- ��ʹ�õ����;��� VARIANT �ṹ��֧�ֵ���Щ��
*
* VARIANT <== ���Ա�ʾ���洢���������͵����ݡ�����(vt)��ʾ�������ͣ������������������ union �ռ��С�
*   CComVariant(ATL��)��COleVariant(MFC��)��_variant_t(CRT?)
*   VariantInit��VariantClear��VariantCopy��VariantChangeType(Ex)-��������ת����
*   ר��ת��������Var<type>From<type>( �� VarR4FromI2 )
* 
* SafeArray(SAFEARRAY) <== CComSafeArray����ȫ���飬��ȫ����ı߽粻һ����0��1��ʼ
*   �����ж�ά��ͨ�����ü��������ڴ��������ֱ��ʹ�������ݳ�Ա��Ӧ��ʹ��API
*   SafeArrayCreate/SafeArrayDestroy/SafeArrayGetUBound/SafeArrayGetLBound
*   
* �ַ�ID(DISPID) -- �Զ����ӿ�ͨ���ַ�ID�����������ԣ�0�͸��������⺬��
*   �����ķַ�ID���京��
*     DISPID_VALUE	        0	�ӿڵ�ȱʡ��Ա������ڿͻ������в�ָ�����Ի򷽷������ô�ȱʡ��Ա��
*     DISPID_UNKNOWN	    -1	��GetIDsOfNames�����з��ش�ֵ������Ӧ�ĳ�Ա�������û���ҵ��ַ�ID��
*     DISPID_PROPERTYPUT	-3	���������ú�����ָʾһ������ֵ�Ĳ�����
*     DISPID_NEWENUM	    -4	���϶����_NewEnum������
*     DISPID_EVALUATE	    -5	�����Evaluate�������ڿ������Ľű������п����÷�����[]����ʾ��
*     DISPID_CONSTRUCTOR	-6	��ʾ�����빹�캯����ͬ���ܵķ�����
*     DISPID_DESTRUCTOR 	-7	��ʾ����������������ͬ���ܵķ�����
*
*   
* ��ص�API(P238)
*   CreateStdDispatch -- 
*   CreateDispTypeInfo --
*   LoadTypeLib(Ex) -- ͨ�����Ϳ��ļ��������Ϳ�
*   LoadRegTypeLib -- ͨ��ע���������Ϳ�
***************************************************************************************************/

//P238,
//P248 -- ͨ�� ITypeInfo �����Ϣ
//OLE�ĵ��ӿڵ����� P317
//����ҳ�ӿڵ�˵�� P324

#endif //COM_AUTOMATION_H