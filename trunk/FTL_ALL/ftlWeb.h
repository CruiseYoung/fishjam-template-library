#ifndef FTL_WEB_H
#define FTL_WEB_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlWeb.h requires ftlbase.h to be included first
#endif

/*************************************************************************************************************************
* ���������
*   Gecko -- Firefoxʹ��,
*   Trident(Ҳ��MSHTML) -- IEʹ��
*   WebKit -- ����Safari�ȣ�ǰ����KHTML
*     WebKit Ports(��ֲ) -- Ŀǰ�� Safari/Chromium/QtWebKit/Android Browser �ȶ��Ports��
*        ��2D��ͼ��GPU���١�JavaScript���桢audio/video����ȣ����в�ͬ��ʵ�֡�
*     ����Port�Ĳ��ԣ�http://trac.webkit.org/browser/trunk/LayoutTests  
*                     http://testthewebforward.org/
*   KHTML -- WebKit��ǰ��?
*
* �ִ�����������
*   HTML��XML��CSS��JavsScript������
*   Layout
*   ���ֺ�ͼ����Ⱦ
*   ͼ�����
*   GPU����
*   �������
*   Ӳ������
*************************************************************************************************************************/


/*************************************************************************************************************************
* DHTML -- Dynamic HTML
* Nginx -- �����ܵ� HTTP �� ������� ��������Ҳ��һ�� IMAP/POP3/SMTP ���������������Ϊ���ؾ����������
*   ��֧��ʹ��һ���Թ�ϣ��ѡ����Memcache���(��ʹ�õ�����ģ��? ��ʹ�� Membase���MemCache ?)
* Rails -- ���ڿ������ݿ�����������Ӧ�ó����������ܣ�����MVC���ģʽ��ʹ��Ruby������
* MemCache -- �����ܵķֲ�ʽ���ڴ���󻺴�ϵͳ
*   ���ƣ����ü򵥣�nginx�ڽ���memcache��˵�֧�֣�֧��ʧЧ
*
*************************************************************************************************************************/

/*************************************************************************************************************************
* WebService -- ʵ���칹����֮�䷽�����õ�һ�ֻ���
*   ��Ӧ�ó���ͨ������Э��͹涨��һЩ��׼���ݸ�ʽ��Http,XML,Soap)������WebService,ͨ�����ڲ�ִ�еõ�������
*   ������ Web Services ƽ̨�� XML+HTTP��ʹ�� XML ����������ݣ���ʹ�� SOAP ����������
*   SOAP(Simple Object Access Protocol) -- �򵥶������Э�飬�ǻ���XML��Э�飬��Ҫ����Ӧ�ó���֮���ͨ��
*     ���ŷ�(<soap:Envelope>), SOAPͷ(<soap:Header>), ��Ϣ��(<soap:Body>) ��ɣ������쳣������body����Ҫ����<soap:Fault>
*   UDDI(Universal Description, Discovery and Integration) -- ͨ�����������ּ����ϣ���һ�ִ洢WebService��Ŀ¼����(ֻ��.NET��?)
*   WSDL(Web Services Description Language) -- Web�����������ԣ�����XML�����������Ͷ�λWebServices
* 
* App Speed Index(ȫ�������վ���ݷ���?) -- http://newrelic.com/asi
* 
* Web �������ļܹ�(�ص���Ҫ������չ)
*   1. Nginx + Memcache + N��Rails + Mysql  -- 
* 
* �û���Ϊ����(�ܰٵ�)�ͶԲ� -- https://github.com/qiushibaike/moumentei(�ܰٵľ�Դ��)
*   a.50%~80%�ÿͲ���½ -- δ��¼�û�ֱ�ӷ��ػ�������
*   b.��ҳ�ȼ���ҳ��ռ��50%���Ϸ�����
*   c.ҳ����80%�����ǲ���ģ�ʣ�µ���ҪΪ�û������Ϣ -- ��������Բ�����û������ݣ�Ȼ����ر仯����
*************************************************************************************************************************/


/************************************************************************************************************************
*   
* OSR(Off Screen Rendering) -- ������Ⱦ���������洰�ڣ�������ҳ����Ⱦ��һ��λͼ���档��Ŀǰ����ʹ��GPU����
*   ͨ��������Ⱦ + Windows��Layered Window�� ���������������ҳ����(��ҳ��͸��������ʲô��״�����ھ���ʲô��״)
*
* �����Ƕ�봰�ڳ���������
*   IE -- ͨ��ActiveX��ʽ����IWebBrowser2����Ƕ�뵽�����У�ʹ�����ӵ���������ڲ����ӣ���ȡ�ʹ����¼�
*     ȱ�㣺�û��Ĳ���ϵͳ���ָ��������ں�Ҳ�Ƕ���汾�Ҳ�����(IE9+)��֧��HTML5�������ȶ������ܿ�ƽ̨��
*   Firefox -- XULRunner��һ����ƽ̨���������ԴӦ�ÿ�ܣ�Ҳ��Firefox��ThunderBird�ĺ��ģ�ʹ��XPCOM��ʽʵ�֣����е�������Ŀ�����װ�ƿؼ�
*     https://developer.mozilla.org/en-US/docs/XULRunner
*   
*   Chrome(CEF:Chromium Embedded Framework) --  https://code.google.com/p/chromiumembedded/
*     ��õĽ��������֧��Windows��Linux��Mac��ƽ̨���ж������Ե���ֲ�汾��
*     �ṩ���������ԣ��Զ��������Զ���Э��(����Q+��qplus://)���Զ���JavaScript�������չ��֧��HTML5��
*     ȱ�㣺���̫�󣬻�����Ƴ�ֻ����һ�����̶�д(ÿ������ʵ��ָ��һ����ͬ�Ļ����ļ���)��
*     ���˼����汾��
*       CEF1 -- ʹ��Chromium WebKit API �ĵ�����ʵ�֣�QQ/�е��ȶ��õ�����汾�����ƺ��ᱻֹͣ��ֻ֧��NPAPI���
*       CEF2 -- ʹ��Chromium browser �Ķ����ʵ�֣�Ŀǰ��ֹͣά��
*       CEF3 -- ʹ��Chromium Content API �Ķ����ʵ�֣�ʹ���첽��Ϣ�ķ�ʽ����������̺��������̵�ͨ�ţ�֧��NPAPI��PPAPI���
*         TODO���ƺ��ܺã���Ϊʲôû����ʹ�ã�
*     http://blog.csdn.net/daoxwj/article/details/8786911
*     http://blog.csdn.net/milado_nju/article/details/7455373
*     CEF�ʼǣ�http://www.cnblogs.com/haippy/archive/2013/06/10/3131373.html
*   
*   Q+:��Web App�ĸ����£�ΪWebҳ���ṩ��һ�����л���(����˵���ǣ�Client��һ�����һЩ���õ�API)��֧��IE��Webkit�ںˡ�
*      
* 
*   QtWebkit �� CEF �Աȣ� http://www.magpcss.org/ceforum/viewtopic.php?f=6&t=395
*************************************************************************************************************************/

/*************************************************************************************************************************
* CEF3Դ�����
* ��ƽ̨֧�֣� ���� cef_win.h �ļ��ж���
*   CefWindowHandle -- ������(HWnd), 
* ���ü���
*   ����CefRefPtr<CefBase����>��ʵ�������ڹ������� CefBase���� ����ʵ������ͨ�� IMPLEMENT_REFCOUNTING ��ʵ�����ü����ĺ���
*  
* ������
*   CefApp -- ����Ľ����࣬ͨ��Ҫ�Ӹ���̳�?
*   CefBrowser -- ����һ����������󣬿��Ի�ȡ���еĸ���CefFrame,��ָ��browser/render���̷����ض���Ϣ��
*     ��browser�����и���ķ������Ա������̵߳��á���render������ֻ�������̱߳�����
*   CefBrowserHost -- ���������������������browser���̣���¶���������������صĽӿ�
*     CreateBrowser -- �������������
*   CefCommandLine -- �����У�ͨ���侲̬ CreateCommandLine ����������InitFromString(::GetCommandLineW()) ��ʼ��
*   CefDOMNode
*   CefDOMVisitor -- 
*   CefClient -- �ص������࣬��ͨ�������亯���ṩ�Զ���ص�, CefCreateBrowser/CefCreateBrowserSync ����ʵ��
*   CefFrame -- ��ʾ����������е�һ��frame���ɼ���URL����ȡҳ���Դ����ı�,URL,V8ִ��������,����ҳ���е�DOM ��
*   CefProcessMessage -- 
*   CefRequest -- 
*   CefSettings
*
* ���ִ���ص�(�����������ƹ۲��ߵĻص�Handler����������¼�)
*   CefBrowserProcessHandler
*   CefRenderProcessHandler -- ʵ��Render����Ļص�
*
* V8�ű�����
*   CefRegisterExtension("v8/app", ) -- ע����Ժ�ԭ��Ӧ�ó�����뽻����Javascript��չ(
*   CefV8Handler -- �û��Ӹ����м̳У�������name�Ƚ��д���
*   CefV8Context -- 
*   CefV8ValueList
*   CefV8Value
*   
* ����
*   �����в�����
*     -single-process -- ������ģʽ���������ܹ����öϵ㣿
*   CefTraceClient -- �ɼ�Trace֪ͨ
*   
* �߳�&����
*   1.��ǰ�߳����л����ж� -- CefCurrentlyOn(TID_XX)�� �� TID_UI ��ʾ��UI���߳���
*   2.CefPostTask -- ִ���첽��
*************************************************************************************************************************/

namespace FTL
{
}
#endif //FTL_WEB_H

#ifndef USE_EXPORT
#  include "ftlWeb.hpp"
#endif 