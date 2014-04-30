#ifndef FTL_DUI_H
#define FTL_DUI_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlDUI.h requires ftlbase.h to be included first
#endif

//Duilib���Ž̳� -- http://www.cnblogs.com/Alberl/tag/duilib%E5%85%A5%E9%97%A8%E6%95%99%E7%A8%8B/

//TODO:
//  1.duilib���沢û�ж�Capture�������ַ������Ϣ����Ӧ���ӿؼ��ϣ������ǻ�û�����Ƶ�ԭ��
//    ��ҪС���Ƿ�֧����ק
//  2.duilib�Զ��̵߳�֧�ֲ����Ǻܺ�? -- 
//  3.��֧�ֶ���
//  4.δʵ�� IAccessible �ӿ� 


/*************************************************************************************************************************
* DuiLib��һ�����ĵĿ�ԴDUI��֧ :http://duilib.googlecode.com/svn/trunk�� ��ѭBSDЭ�飬�������������ҵ��Ŀ
*   ��ȫ����ActiveX�ؼ�(��IE��Flash��)�����Ժ�MFC�Ƚ�������ʹ��
*   
* ������(�������κ��ⲿ�Ŀ⣬�����ڲ�ʵ���˺ܶ�����֧����Ŀ�Ļ�����)
*   CDelegate -- �¼�����ͨ�� OnXxx += MakeDelegate(this, &OnXxxHandle) �����¼��ʹ�����
*   CEventSource -- �¼�Դ,ͨ������ ���� CDelegateBase ��ʵ��������Ҫʱ�������μ������е��¼�����
*     �����һ����������false�����жϵ��ò�����false�����򼤷�ȫ���¼��󷵻�true
*   ��������CStdPtrArray / CStdValArray / CStdString / CStdStringPtrMap
*
* �ؼ������
*   CControlUI -- ���пؼ��Ļ��࣬�ؼ��������еĽڵ㶼��һ��CControlUI
*     DoPostPaint -- �ɹ��������صĻ��ƺ�Ļص���
*     Event/DoEevnt -- �����¼�
*     GetInterface(����ǿת) -- ���ݴ�����ַ���������ָ��ؼ���ָ�룬���û�ж�Ӧ�Ŀؼ����ͣ��򷵻�NULL
*     SetAttribute -- ��XML�н������ؼ��ĸ������ԣ������ַ������������ƺ�ֵ��������
*   CContainerUI -- �������࣬���ڲ���һ������(m_items)���������е�CControlUI�Ķ���
*       ���е�������֧�ֹ��������Լ��̺��������¼����д���(CContainerUI::DoEvent)�������ڲ�����Ԫ�ص���λ�ã�����ʱʵ�ֹ���Ч����
*       TODO�����ƿ���(������ʾ����Ĳ�����)���ؼ����жϻ�����������Լ�������ʾ�����򲻻���
*     FindControl -- �����ӿؼ�
*     Remove/RemoveAt -- �Ƴ��ӿؼ�
*     SetAutoDestroy -- �����Ƿ��Զ����٣�������ӳ����ٵĻ������� CPaintMangerUI ����?
*   
*   �����ؼ�(ControlĿ¼��,������ CControlUI) -- 
*     CActiveXUI -- ActiveX����ͨ����Ӧ DUI_MSGTYPE_SHOWACTIVEX �󣬵��� CActiveXUI::GetControl(IID_IUnknown,xxxx) ��ö�Ӧ�Ŀؼ��ӿ�
*     CButtonUI(Button)
*     CLabelUI
*     CListUI/CListTextElementUI -- 
*     CRichEditUI
*     CScrollBarUI --
*     CTextUI -- 
*     CWebBrowserUI -- ��Ƕ IWebBrowser2
*
*   Layout(IContainerUI -> CContainerUI) -- ���ֿ���
*     CChildLayoutUI
*     CDialogLayoutUI
*     CHorizontalLayoutUI/CVerticalLayoutUI/CTileLayoutUI -- �������У��������и�������  
*     CTabLayoutUI -- Tab ����
* 
*   CRenderClip -- �ü��������
*   CRenderEngine -- ��Ⱦ���棬�ܶ����ͨ�õľ�̬���ƴ��붼��������� �Ź�������ͼƬ�������������ֵ�
*   CPaintManagerUI -- �ؼ����ƹ�����Ӧ��һ��Windows��ԭ�����ڣ���Ҫ���� �ؼ�����Դ�Ĺ���ת�����ַ�Windowsԭ��������Ϣ
*     Init -- ͨ��ԭ��HWND���г�ʼ��
*     FindControl -- �������֡�λ�õȲ���ָ���Ŀؼ�
*     FindSubControlByName -- ��ָ�������и��������ҵ��ӿؼ���δ�ҵ��Ļ��᷵��NULL
*     MessageHandler -- �ڲ���Ժܶര����Ϣ���д���������ַ�����Ӧ�Ŀؼ���ȥ
*     SetResourcePath/SetResourceZip -- ����Ƥ����Դ��·��(ͨ���ǵ��԰汾)��Zip�ļ�(ͨ����Release�汾)
*
*   CWindowWnd -- ԭ�����ڿ���(Create/Subclass/HandleMessage��),ͨ�����ཫԭ�����ڵ���Ϣ�ַ����������࣬��󴫸������ؼ���ϵ��
*     HandleMessage -- ��������Ϣ�ַ�
*     OnFinalMessage -- ͨ��new�����Ĵ���ʵ����Ҫ����������� delete this
*   WindowImplBase -- DUIΪ�˼�ʹ�ö��ṩ��һ����װ�࣬�ṩ�˳��õ�һЩ���ܣ���ͨ���û��ĸ���ԭ������������ǴӸ���̳С
*     CPaintManagerUI m_PaintManager;  -- ������Ա�������Ըô���Ļ��ƽ��й���
*     HandleMessage -- ����һЩ��������Ϣ����(�������麯���ķ�ʽʵ����Ϣӳ�䣬���������Щ��Ϣ����Ȥ����Ҫ���ض�Ӧ����Ϣ������)
*     HandleCustomMessage -- ����������ش������Ȥ���Զ�������δ�������Ϣ
*     GetWindowClassName -- �����������ʵ�ֵĴ��麯��
* 
* ��Ϣ(�¼�)����ʽ -- ( Event + Notify )
*   ��Ϣӳ��(ͨ���ַ��������ֵģ���Ҫ�� CNotifyPump ��������̳�), ��򵥵���Ӧ������������ void CMyXXX::OnClick( TNotifyUI &msg );
*     ͷ�ļ��У�  DUI_DECLARE_MESSAGE_MAP ������Ϣӳ����صı����ͷ���
*     ʵ���ļ��У�DUI_BEGIN_MESSAGE_MAP(xxx, CNotifyPump) -> DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick) -> DUI_END_MESSAGE_MAP
*   IDialogBuilderCallback -- ���ݴ����Class��������ָ�����Ϳؼ���ʵ��?
*   INotifyUI -- ���������Ϣ����ӿڣ�ʹ�� TNotifyUI �ṹ������Ϣ�� ���Խ��俴�ɺ�WM_NOTIFY���Ĺ���?
*     TNotifyUI:
*       sType -- DUI_MSGTYPE_XXX, �� DUI_MSGTYPE_CLICK
*       sVirtualWnd -- ͨ�� AddVirtualWnd ����ʱ�����ⴰ������
*       pSender -- ����֪ͨ�Ŀؼ���һ��ͨ���� GetName ���ж�
*   IMessageFilterUI -- PreMessageFilter(MessageHandler ���鷽��)����ϢԤ����
*   ITranslateAccelerator -- ���ټ�ת���ӿڣ����鷽�� TranslateAccelerator
*   CEventSource -- �¼����� 
*     OnNotify += MakeDelegate(this, &CFrameWindowWnd::OnHChanged);
*   CNotifyPump -- ��Ϣӳ��Ļ��࣬��Ϣӳ��������鴰��ķ�ʽ(���� WindowImplBase ����Ƕ�Ǵ���� Pageҳ)��
*     ��ͨ�� AddVirtualWnd ���룬Ȼ��ͨ�� TNotifyUI::sVirtualWnd �ݹ��ҵ���Ӧ���鴰�������Ϣ����
*  ע�⣺
*    CPaintManagerUI::MessageHandler -- �ڲ���Ժܶര����Ϣ���д���������ַ�����Ӧ�Ŀؼ���ȥ
* 
* ��Դ��Ƥ������(Deubgʱʹ����ԴĿ¼��Releaseʱʹ��Zip��)��ͨ��XML���ý���ؼ����ֺͷ��֧�ִ�Alphaͨ����PNGͼƬ
*   CDialogBuilder -- ͨ�������ļ��Զ�����Ƥ��
*     Create -- ����Ƥ�������ļ��к�zip��ʽ���е�XML�ļ������������ֿؼ������ؿؼ����еĸ��ڵ�ؼ�
*
* ����֧�ֹ���
*   UIDesigner -- ���������õĿ������ߣ����ƺ�����������
* 
* XML��Դ(û�� DTD/Schema ����֤�ĵ�?)�����������пؼ��Ĳ��ֺ���ʽ
*   Window -- �����Ĵ���ؼ��������� Font��Default(VScrollBar/HScrollBar) ���ӽڵ㶨������
*   VerticalLayout/HorizontalLayout/TabLayout/TileLayout/ChildLayout -- ���ֿؼ������ӿؼ�����ָ���ķ�ʽ�Զ�����
*   Container -- �ɰ����ӿؼ���Ҳ�����������ռλ?(Ĭ�ϴ�С���Զ�����ʣ���λ��)
*   ���ֿؼ�
*     Edit
*     Label -- ��ǩ�ؼ�
*     List(CListUI) -- �� ListHeader/ListContainerElement ��϶���
*       TODO:��ζ�̬���� Item��ܰ�Elementд�ɿ����õķ�ʽ����
*       IListCallbackUI -- ����ʵ���������б�Ĺ��ܣ���ֻ�ܷ������֣����ܻ���ͼƬ��?
*     Option -- ����ʵ��Sheet�е��л�Tabҳ(�μ�RichListDemo)
*     Progress -- ������
*   ��������:
*     normalimage/hotimage/selectedimage/bkimage -- 
*************************************************************************************************************************/

/*************************************************************************************************************************
* DUI -- DirectUI
*   DirectUI and Duser Architecture
*   Resource Management and Build System
*   UI Files: Markup, Styles, and Macros
*   Layout, Rendering, and Animations
*   Notifications: Events, Listeners, Handlers, and Behaviors
*   Common Controls, Custom Controls, and Compound Controls
*   Best Practices and Future Work
*
* DUser -- Streamlined low level window manager (extending Window's User architecture. ) Includes painting coordination, 
*	gadget tree management, input management, messaging, and simple animations.
* DirectUI -- System above DUser including declarative layout, stylesheets, keyboard navigation, MSAA support, 
*	common controls, and content rendering	.
* Gadget -- Can be thought of as light weight HWNDs.  Can host HWNDs, or other Gadgets.
* DUI -- The whole of DUser and DirectUI as a platform.  Commonly used to refer to UX Platform.
* Parser -- Parser that takes UI markup in, and creates DirectUI visual tree.
* Layout -- Instructions given to element to allow for child positioning.
* Resource Management -- System designed to allow for build time processing and packaging of resources, 
*	runtime resource lookup, and extensions to the Win32 resource model.
* RCXML -- Build time resource processing tool.
*
*************************************************************************************************************************/

namespace FTL
{
}
#endif //FTL_DUI_H

#ifndef USE_EXPORT
#  include "ftlDUI.hpp"
#endif 