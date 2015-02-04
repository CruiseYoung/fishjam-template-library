#ifndef COM_MFC_STUDY_H
#define COM_MFC_STUDY_H

/*****************************************************************************************************
* COM�ĳ�ʼ��: AfxOleInit / CoInitialize(EX)
*
* ��MFCʵ��LPDispatch�Զ����ӿ�����
*   1.����һ��֧���Զ���(Automation)�ĳ������ӳ����У�ʹ�õ���DLL�� 
*   2.���һ���Զ����� CAAA ��������CCmdTarget��ѡ��Createable by type ID��ע�⣬ֻ��ѡ��������ܱ��ⲿ����(VB)CreateObject��(VC)CreateDispatch�ȷ����������� 
*   3.�����һ���Զ�����CBBB��������CCmdTarget��ѡ��Automation 
*   4.��CAAA�У���ӵĳ�Ա����CBBB m_bbb 
*   5.��IAAA�У��������ΪBBB������ΪLPDispatch�����ԣ�ʹ��Get/Set���� 
*   6.���BBB���Ե�Get/Set���� 
*     LPDISPATCH CAAA::GetBBB()
*     {
*       return m_bbb.GetIDispatch(TRUE);
*     }
*     void CAAA::SetBBB(LPDISPATCH newValue)
*     {
*       SetNotSupported();
*     }
*   7.��IBBB�У���ӷ������� Add
*   8.�޸�CBBB��������������protected�ƶ���public�� 
*
* ʹ���Զ��������
*   ����1��Add Class => From a type Library(�� C:\Program Files\Microsoft Office\Office\MSWord.olb )
*   ����2��#import 
*
* ���ӵ�
*   MFCͨ��CCmdTarget��ʵ����IConnectionPointContainer��IEnumConnectionPoints�ӿ�
*      ͨ��CConnectionPoint��ʵ����IConnectionPoint�ӿ�
*   ��Ҫ���� EnableConnections ���� �� METHOD_PROLOGUE ��
*
* ʹ�õ�ӳ���
*   1.BEGIN_INTERFACE_MAP -> INTERFACE_PART -> END_INTERFACE_MAP
*     �ӿ�ӳ��
*   2.BEGIN_INTERFACE_PART -> STDMETHOD -> END_INTERFACE_PART => DECLARE_INTERFACE_MAP
*   3.BEGIN_CONNECTION_PART -> CONNECTION_IID -> END_CONNECTION_PART => DECLARE_CONNECTION_MAP
*   4.BEGIN_CONNECTION_MAP -> CONNECTION_PART -> END_CONNECTION_MAP
*     ���ӵ�ӳ��
*   4.IMPLEMENT_OLECREATE -> DECLARE_OLECREATE
*
* ���÷�������
*   COleDispatchDriver -- MFC ʹ�ø���������� IDispatch �ӿ�
*   CWnd ���п���ֱ�ӵ��� CreateControl(CLSID_WebBrowser, NULL, WS_VISIBLE|WS_CHILD, xxxx) 
*     ������ActiveX����Ȼ��ͨ�� GetControlUnknown ��ȡ��Ӧ�ӿ�
* 
******************************************************************************************************/

#endif //COM_MFC_STUDY_H