#ifndef COM_ACTIVEX_H
#define COM_ACTIVEX_H

/***********************************************************************************************************
* MFC��֧�֣� COleControl
*
* ActiveX -- һ��ʵ����һЩ��׼�ӿ�(��IOleObject��IOleControl��)��COM���
*   ��Ϊ����ģʽ(UserMode): ���ʱ(designtime)������ʱ(runtime)
* 
* Ambient -- �����Ļ������ԣ���������IDispatch�ӿ��ṩ,����ATL�Ŀؼ����Ե��� GetAmbient* �������������Ļ������ԡ�
*   UserMode:TRUE��������ģʽ��FALSE�������ģʽ
*   DisplayName:����ָ���Ŀؼ�����
*   LocalID:�����ı��ر�ʶ��
*   MessageReflect��ΪTRUEʱ�������������Խ���Ϣ�������ؼ�
*   UIDead�����߿ؼ��������û����������û������벻���κ���Ӧ
*   SupportsMnemonics���������ڿؼ���ʹ�ü��似��(mnemonics)�ṩ֧��
*
* ʵ�ؼ���(in-place activation/in-place editing ʵ�ر༭) -- ����ֱ���ڰ����������ڲ����б༭
*   ʵ�ֵĽӿ�
*     ��������ʵ�֣�IOleInPlaceFrame��
*     �ĵ����ڶ���ʵ�֣�IOleInPlaceUIWindow
*     վ�����ʵ�֣�IOleClientSite��IAdviseSink��IOleInPlaceSite��IOleControlSite��IPropertyNotifySink(���⣺�⼸���ӿ�Ӧ����˭ʵ�֣�)
*     IOleInPlaceObject��IOleInPlaceActiveObject
*   ʵ��Ҫ�㣺
*     ���洦�� -- �˵��ϲ�������������ݼ�
*     ��Ϣ���͡�������� -- �����Ϣ��������Ϣ
*     ���ڴ�С��λ�õ���
*
* ��׼�ַ�ID -- ��׼���ԡ���׼��������׼�¼����������ı�׼��������
* 
* ��׼�ؼ�����(stock property�����͡����֡�DISPID_XXX ��ֵ���ǹ̶���) -- �ṩͨ�õĿؼ����ܣ���Font��BackColor ��
*   ͨ����Ҫ�ڿؼ��Ĺ��캯���н��г�ʼ��
* ���Եĳ����� -- IPersistStream��IPersistStreamInit��IPersistPropertyBag���ṩ����-ֵ�Եķ�ʽ��
*
* ����ҳ--ÿһ������ҳ����Ҳ��һ��COM�������������Ӧ�ó��򵥶�ʵ������ϵͳ�ṩ�˼�����׼������ҳ(����Font����ɫColor��ͼ��Picture)
*   ��������ҳ��ATL����(��������WTL��������ҳ�Ի������Ϣ��ӳ��)����ͨ�� PROP_ENTRY �����Ժ�����ҳ��CLSID����
*     �����CLSID_NULL������û��ȱʡ������ҳ��ͨ����Ҫ�Լ���������ҳ��������ӳ��
*   ϵͳԤ��������Զ������� $(VSInstallDir)\Vc7\VCWizards\ClassWiz\ATL\Control\Templates\1033\control.h �ļ��в鵽
*   ������ҳ�����У�
*     1.OnInitDialog �л�ȡ�ؼ�������ֵ�����ڳ�ʼ����
*          if(m_nObjects >0){       //���⣺m_nObjects �� m_ppUnk ��ʲô��
*            CComQIPtr<IXXX,&IID_IXXX> pXXX(m_ppUnk[0]); 
*            pXXX->get_YYYY(...) 
*          }
*     2.���Զ���� �༭�� ��UI�ؼ��� EN_CHANGE ���¼������� OnPropertyChange ������ Apply ��ť
*       SetDirty(TRUE);  //����Dirty��־
*       m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY | PROPPAGESTATUS_VALIDATE);//֪ͨ���������������ֵ�Ѹı�
*     3.�� Apply �¼�������
*       CComQIPtr<IXXX,&IID_IXXX> pXXX(m_ppUnk[0]); //��ȡ�ؼ���ָ��
*       ������ҳ��UI�ϻ�ȡ�û��������Ϣ�󣬵��� pXXX->put_YYYY �ȷ���������ֵ���ݵ��ؼ�
*       m_bDirty = FALSE;   //�ָ�Dirty״̬
*       �����������(��ֵ��Χ)����Ҫͨ�� GetErrorInfo(0,&pIErrorInfo) ��ȡ������Ϣ�ӿڣ�����ȡ��ϸ���������ʾ��
*     4.���Ա仯֪ͨ -- ʹ�ÿ����Ӷ�����ƣ�����ӿ�Ϊ IPropertyNotifySink 
*   ԭ�����ܲ���ȷ�������ͻ�ͨ�� OleCreatePropertyFrame �������Ա���ܣ����а����˶�� IPropertyPageSite <-> IPropertyPage��
*         ���ͨ�� ISpecifyPropertyPages �������е�����ҳ
*
* �¼� -- ʹ�����ӵ㼼��(�����ӵ��ṩClick�¼���DISPID_CLICK����WM_LBUTTONDOWN ��Ϣ�� Fire_Click)
* 
* ActiveX�ؼ��ķ���
*   1.�ͻ�-������Э�����ƣ�<OBJECT CLASSID="..." codebase="http://webserver/Polygon.dll" ...>
*       <PARAM NAME="Sides" VALUE=5></OBJECT>
*       IE�Զ����ص���Downloaded Program Files��Ŀ¼�в�ע�ᵽ�ͻ�����
*   2.ʹ��CAB����ѹ������ߴ���Ч�ʲ��ҿɰ�����������ļ���<OBJECT ... codebase="http://webserver/Polygon.cab" ... ></OBJECT>
*     CAB�ļ���������cabarc.exe N polygon.cab atl.dll polygon.dll \ polygon.inf
*       ֧������ǩ��
*
*
* CComControl -- ATLͨ��CComControl�ṩ�����ActiveX�ؼ����ܣ���������ԡ����Գ����ԡ����ڹ��ܡ���Ϣӳ��ȣ�
*   ������������Ա仯�����ܻ������ۣ������Ե��� FireViewChange ֪ͨ����
* CStockPropImpl -- �ṩ��ATL�Ĺ�������ʵ��
* IOleObjectImpl -- �ṩ�˿ؼ��ĺܶ�Ĭ��ʵ��,�������Ҫ��ֻ��Ҫ��д��Ӧ�ĺ������ɣ������ƿؼ���С�� SetExtent��
*
***********************************************************************************************************/

#endif //COM_ACTIVEX_H