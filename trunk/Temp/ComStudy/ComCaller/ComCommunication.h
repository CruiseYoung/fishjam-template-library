#ifndef COM_COMMUNICATION_H
#define COM_COMMUNICATION_H
#pragma once

/******************************************************************************************************
* ע�⣺
*   1.(ͨ������£�)Office(Word)������ IApplicationEvents2(�г��˷�����interface) �� ApplicationEvents2(�ͻ�����Ҫ����Evnets��dispinterface)��
*     ApplicationEvents2 �ͻ����� Invoke ʱ�� dispid ���� IApplicationEvents2 �з�����˳��(��1��ʼ����)��
*     �� IApplicationEvents2 �ĵڶ������� Quit ��Ӧ�� dispid Ϊ 2������μ� �� MSWORD.OLB import ���ɵ� msword.tlh �ļ�
*     ��������ʲô�ط����Եõ�����Events�� DISPID_XXXX -- ͨ�� OleView.exe ���Կ���ȫ����Ϣ����������������û�и��õĹ��ߣ�
*   2.������ IDispEventImpl ������ Invoke ��������ӡ�����ֲ���(����ֽӿڻ����)
*     ע�⣺pdispparams->rgvarg ��˳��ͺ���ǩ���в�����˳���Ƿ��ŵģ������Ҫ�Ӻ���ǰ�����Ϣ
*     	FTL::CFIExplorerDispidInfo IExplorerDispId(dispidMember); FTLTRACE(TEXT("%s:\n"), IExplorerDispId.GetConvertedInfo()); //IE�е�ID
*       for(UINT index = 0; index < pdispparams->cArgs ; index++) 
*       { 
*          FTL::CFVariantInfo varInfo(pdispparams->rgvarg[pdispparams->cArgs - index - 1]);
*          FTLTRACE(TEXT("\t %d, %s\n"), (index + 1), varInfo.GetConvertedInfo());
*       }
*       return __super::Invoke(xxxx);

*
* C������ʹ�ûص�֪ͨ�ͻ��ˣ�C++��ͨ��ʹ�� ������Ϣ���¼��ӿڡ�Ӳ����ص���������thunk�� ͳһ�¼�ģ�� ��
*
* ���������ͻ�����ͨ�ţ��ص��ͻ��ṩ�Ĺ��ܵĻ���
* 
* �ص��ӿڣ�ʹ���Զ���ӿڣ��ٶȿ죬�����ƴ� -- ֱ��ʹ�ûص�������ַ��ֻ�����ڱ��� DLL ����
*   ��COM����з����ض��¼�(��������ɡ���ʱ�����û�������)ʱ����Ҫ֪ͨ�����ߡ�
*   ͨ�� ���ص��������Ļ��ƣ������ص��ӿ�--�������ӿ�ISink�����õ� COM �������У������¼�ʱ��COM���øýӿ����֪ͨ��
*   1.COM�����������塱������֪ͨ�Ļص��ӿ� IMyMathCallBack��������֪ͨ����ĺ��� Fire_Result���ýӿڽ��ɿͻ��ˡ�ʵ�֡�
*     �ڽӿ��б�����Ҫʹ�� source �����ýӿ��ǻص��ӿڣ� [source,default] interface IMyMathCallBack;
*     ���� IMyMathCallBack �ӿ����ɿͻ���ʵ�ֵģ���˲���Ҫ���ӵ� BEGIN_COM_MAP ��
*   2.COM����������ͻ���Ҫ���õķ��� IMyMath::Sub������ͨ�������������ؽ��������ͨ�� Fire_Result ֪ͨ�����
*   3.�ͻ��˵��� IMyMath::Advise([in] IMyMathCallBack *pCallBack,[out] long *pdwCookie) �ѿͻ���ʵ�ֵĽ�����(sink)
*     �ӿ�ָ��(IMyMathCallBack *)���ݵ�����������У����ж���� dwCookie �������ֶ�� CallBack�������� Unadvise([in] long dwCookie)
*     ʱ֪��Ӧ�öϿ��ĸ��ص���
*
* ���ӵ� -- ʹ�ó��ӿ�(Outgoing Interface)���ɿͻ����¼�������(Sink)ʵ�֣��������������������¼�
*   ʹ��ϵͳԤ����ӿڣ��ٶ����������Կ���̡����磬��һ��α�첽(pseudo-asynchronous)����,COM+��֧���������첽
*   1.һ�� COM ����������ж�����ӵ����(IConnectionPoint)���������ж���������¼�����Դͷ��
*   2.������Щ���ӵ�ĽӿڽС����ӵ�������(IConnectionPointContainer)����2��������FindConnectionPoint��EnumConnectionPoints����
*   3.ÿһ�����ӵ㣬���Ա�����ͻ��˵Ľ�����(Sink)���� -- ʹ�� cookie ��������
*   �����Ӷ���(IConnectionPointContainer) 1 -> 1 ���ӵ�ö����(IEnumConnectionPoints) 1 -> n ���ӵ�(IConnectionPoint)
*   �򵼲��裺
*     1.ATL ��������ѡ��֧�����ӵ㡱,��Ҫ��˫�ء��ӿڣ��ᶨ��� dispinterface _IXXXEvents ���¼��ӿ�
*     2.idl�����ӽӿں���(IMyTimer::SetTimer)��
*     3.idl���������ӵ��¼��ӿ�(_IMyTimerEvents)��֪ͨ���� Fire_Timer -- ע�⣺ֻ��Ҫ���� Timer ���ɣ����ɺ���Ҫ����һ�£�
*     4.ѡ���������(CMyTimer)���Ҽ� -> ��� -> ������ӵ㣬�����Զ������¼�������������(CProxy_IMyTimerEvents::Fire_Timer)��
*     5.���ʵ���λ�õ���IDE���������ɵĴ��������룬�����¼����� Fire_Timer(v)��
*     6.�ڽӿ�ӳ��(BEGIN_COM_MAP)�м��� IConnectionPointContainer ��ӳ�� -- ���Զ����ɣ�
*     7.�򵼻������ӵ�ӳ�������ӳ�� CONNECTION_POINT_ENTRY(__uuidof(_IMyTimerEvents))��
*     8.�������ӵ���¼�ͨ�� Invoke ֪ͨ�ͻ��ˣ�Fire_Timer������˿ͻ��˽�����Ҫ�� IDispatch ���������� Invoke ��
*       �������ӵ㺯������������Ϣ��InvokeҪ�������ž��� Fire_XXX ��Ӧ��id���� Timer��
*   �ͻ���ʹ�ã�
*     1.COM���ʵ���� ���ӵ�����(IConnectionPointContainer) �� ���ӵ�(IConnectionPoint -- ���ýӿ�ͨ����ֱ�ӵ���) �ӿڣ����ԣ�
*       a.��ȡ IConnectionPointContainer �ӿں�ͨ�� FindConnectionPoint(__uuidof(_IMyTimerEvents),&m_spCPTimer)������ӵ�ӿڣ�
*     2.���� m_spCPTimer->Advise( &m_TimerSink, &m_dwCPCookie) �����¼�������(m_TimerSink)�����ӵ�����ӡ�
*     2.ʹ��MFC �� DECLARE_EVENTSINK_MAP() / BEGIN_EVENTSINK_MAP(...) -> On_EVENT(...) -> END_EVENTSINK_MAP() ������ӳ��
*       �� ON_EVENT(CTestAtlDlg, IDC_ATLCOMP1,     1    ,  OnAtlEvent, VTS_I4  VTS_BSTR)��
*         �����ֱ��ǣ��������,  �ؼ�ID,      Invoke �� ID, �����������������б�(������� long state,char * buf)
*     3.�ͻ�����Ҫʵ�� IDispatch �ӿڣ�����Invoke�д���ص��������ATLʵ�֣����� CComObject<CMyMathEvents> ��
*   ATL���ӵ�ͻ���
*     1.�� IDispEventSimpleImpl �� IDispEventImpl �̳�
*     2.ͨ�� BEGIN_SINK_MAP -> SINK_ENTRY(���Դ�����ΨһID, dispid, func)/SINK_ENTRY_EX(xxx) -> END_SINK_MAP �������¼�����ӳ��
*       �¼����պ���(��ĳ�Ա����)ǩ����ʽ���¼��ӿ��ж���ķ���ǩ��һ�£� 
*       �� HRESULT _IMyTimerEvents::Timer([in] VARIANT varDate) ��Ӧ�Ĵ�����Ӧ���� STDMETHOD(OnTimer)(VARIANT varDate);
*     3.����Ҫ������¼��ӿ�ͨ������ DispEventAdvise/DispEventUnadvise ����֪ͨ��ȡ��֪ͨ���ӵ�
*       ��������ǿ��Cast��ͨ�� QI �� IConnectionPointContainer��Ȼ�� FindConnectionPoint ָ��Event�ӿں� Advise/Unadvise
*       ע�⣺
*         a.DispSink�����ӳ����ʹ�÷�ʽΪ(IDispEventImpl<ID, Cxxx, DIID_IXXXEvents> ������)
*           CoCreateInstance(CLSID_XXX,xxx, &m_pIServ) <== ����COM��������ʵ��
*           AtlGetObjectSourceInterface(m_pIServ, &m_libid, &m_iid, &m_wMajorVerNum, &m_wMinorVerNum) <== ���Ĭ��������Ϣ����������
*           DispEventAdvise(m_pIServ, &m_iid) <== �������ӵ�
*         b.����ֱ���� AtlAdvise/AtlUnadvise�����ɣ�
*           IDispEventImpl��IDispEventSimpleImpl�������ಢû��ֱ�Ӽ̳�IDispatch���޷�ͨ��QueryInterface��ֱ�ӻ�ö�Ӧ�¼��ص��ӿڣ�
*           ͨ��DispEventAdvise�ڲ�ǿ��Cast��IUnknown��IDispEventSimpleImpl��Ȼ���Ǵ�IUnknown�̳У���������IUnknown��IDispatch�����з�����VTable�Ĳ���һ�£�

******************************************************************************************************/


/******************************************************************************************************
* ͳһ�¼�ģ�� ( ������й��첽���¼�������Ϣ���ݻؿͻ���, ���ڱ���C++(native)��COM ���й�����һ�µķ�ʽʵ��)
*   ��ģ��֧�ֵ��̺߳Ͷ��̵߳�ʹ�ã�����ֹ�����ݽ���ͬ�����̷߳��ʣ����Դ��¼�Դ����������������࣬��֧�ִ��������е���չ�¼�Դ/���ա�
*   event_source -- �¼�Դ,�����¼��Ͱ����¼��Ķ���ʹ�ùؼ��� __event �����¼�OnXXX(ȷ��ֻ������������Ҫʵ����)��__raise �����¼�
*   event_receiver -- �¼����������ǽ����¼��Ķ��󣬲�������ʵ�֡��¼��������
*   __hook/__unhook -- "�ҹ�/����ҹ�"�¼���ͨ���� �¼��������Ĺ��÷�����ʵ��
*   
*   ί�� -- �ɱ��ֶԷ��������õ��࣬�������ǩ������ֻ���ֶ�����ǩ����ƥ��ķ���������(��ͬ��������ĵط�)��
*  
*   ע�⣺
*   1.�ƺ����� event_source/event_receiver ����ָ��Ҳ������ȷ���У�
*   2.event_receiver �����һ���࣬���ܶ����ʹ�ó�Ա������������ø���Ĺ��캯����������ʵ����
*   3.(������)ͳһ�¼�ģ��֧�ֶ�̬�ҹ�������ѭ���йҹ�/����ҹ��¼������ڽ��ղ�����ʽ��Դ�����ͱ��ط����ĺ�����ʹ�������߼��ҹ�/����ҹ��¼�
*   4.��__hook �Ļ��ȱ����ã��˳�ʱ��Ҫ __unhook����������ڴ�й©
*   5.__hook(&CMyEventSource::OnValueChanged, pSource, &CMyEventReceiver::OnValueChangedHandler, this);
*     �������������´���(?): pSource->__AddEventHandle_CMyEventSource_OnValueChanged(XXX)
*  
******************************************************************************************************/

#endif //COM_COMMUNICATION_H