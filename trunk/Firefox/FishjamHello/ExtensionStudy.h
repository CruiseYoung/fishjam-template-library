#pragma once

/*******************************************************************************************************
* ��չ������������(������)��https://addons.mozilla.org/en-US/developers/tools/package/
* ������չ��
*   All-in-One Gestures -- �������
*   GreaseMonkey -- ��Firefox ���ṩ��һ�����������û��ű�(JavaScript)�Ļ���������ű�����ָ��Ŀ����վ��
*   
* ע�⣺
*   1.? -- dtd����Դ�ļ������ΪUTF-8���룬��������޷���ʾ
*   2. -- Ϊ��д�Ĵ�������Ȩ���Կ˷���ȫ���Ƶ�����
*   3. -- ��ͨ��xpcom������ʹ�� js-ctypes ��addon��ֱ�ӵ���dll�еĺ���(http://mozilla.com.cn/post/21692/)
*         ���԰���xpcom���������İ汾����������������֧�� Chrome(��û����� npapi ?)
*   
* ��չ�Ļ���Ŀ¼�ṹ(��չ��xpi��zipѹ����) -- https://developer.mozilla.org/en/Building_an_Extension
*   chrome.manifest -- Chrome ע����嵥�ļ�,���ø���XUL�ļ�·���Լ�������һЩ��Դ�ļ�·�����﷨���£�
*     overlay chrome://browser/content/browser.xul	chrome://helloworld/content/overlay.xul	<== ��FF�ṩ��browser.xul�Ϻϲ��Զ����UI��ͨ������չ����UI
*       ˵����1.FFͨ��"Chrome URIs"����ȡ��ص��ļ�����"chrome://browser/content/browser.xul", ��ֱ���ڵ�ַ������ȷ��,
*               FF�Ὣ��ת��Ϊ���̻�JAR����ʵ�ʵ��ļ�·����
*             2.����չ�п���ʹ�ø��ǣ�Overlay���������еĽ��������Ԫ��(��ϵͳ Tools�˵� <menupopup id="menu_ToolsPopup"> ),
*               �������Ը��ǵ�ϵͳxul�� chrome://browser/content/preferences/preferences.xul ��
*             3.FFϵͳʹ�õ� browser.xul ���ļ��ڰ�װĿ¼�µ� omni.ja ѹ������
*     content ��չ���� λ�� <== ������չ���������ö�Ӧ��Chrome���������ַ(jar����Ŀ¼�ȣ�ע�⣬���Ҫ��б�߱�ʾĿ¼)�Ķ�Ӧ��ϵ
*     locale ��չ���� ����ID λ�� <== ���ò�ͬ����ID(��zh-CN)��Ӧ����ԴĿ¼�����⣺��ô�л�����?
*       �� locale helloworld zh-CN chrome/locale/zh-CN/
*     skin ��չ�� classic/1.0 λ�� <== 
*     style chrome://global/content/customizeToolbar.xul xxx.css  <== ��������(xul)�ͽ�����(css)֮��Ĺ��� ?
*     C++���͵�XPCOMע�᣺
*       interfaces ���Ϳ�(.xpt)��λ�� <== ָ��XPCOM��ʵ�ֵĽӿ����Ϳ�
*       binary-component XPCOM��·�� ABI=WINNT_x86-msvc appversion<15 <== 
*     JavaScript���͵�XPCOMע��
*       component ���ID xxx.js <CR> contract ��ԼID ���ID <== ���� js �� XPCOM ֮�������(ͨ�����ID?)
*     Python���͵�XPCOMע��(PyXPCOM)
*       ��ô��???
* 
*   install.rdf -- ��������չ��ע����Ϣ�͸�����Ϣ�ȣ���Ҫ�����������ݣ�
*     <em:id> -- �� "{UUID}" �� "name@��ַ" �ķ�ʽ��ʾ
*     <em:name> -- 
*     <em:version> -- 
*     <em:targetApplication> -- ָ��Ŀ��ϵͳ����Ϣ����
*        <em:id>��{ec8030f7-c20a-464f-9b0e-13a3a9e97384} -- Firefox App ID
*        <em:minVersion>,<em:maxVersion>
*   update.rdf -- ������ǰ��չ���°汾��Ϣ ?
*  
*   [chrome] -- Chromeָ����Ӧ�ó��򴰿ڵ���������֮����û�����Ԫ�صļ��ϣ��������������˵����������ʹ��ڵı������ȡ�
*       contents.rdf -- (�����ƺ�����Ҫ����ļ��ˣ�ÿ����Ŀ¼�¶���һ��������ͳһ�� chrome.manifest ��)�������Ŀ¼����Ϣ�嵥��Mozilla����ȡ��ʹ����������ע�����
*       �������������һ��chrome URL�����û�и��ļ����Ͳ���ͨ�� chrome:// �����ʣ�ʵ����Է���
*     ���ֻ�����chrome�ṩ��
*     [content] -- ��չ����������� XUL �ļ���������Ϊ�� JavaScript �ļ���
*       �磺content	myExtension	chrome/content/ �� content	myExtension	jar:chrome/myExtension.jar!/content/
*       �������ļ��������������ͬ(��editor��������һ������editor.xul���ļ�)���ϵİ汾������������ƣ������ƺ��Ѿ�����Ҫ�ˣ�
*     [locale] -- ��ŵ��Ǳ��ػ���ص��ļ�, ��  en-US��zh-CN��zh-TW��ko-KR��vi-VN �� Ŀ¼�������� dtd��properties ���ļ�
*     [skin] -- ��� CSS��PNG����Դ�ļ�������������չ�����
*   [plugins] -- 
*   [components] -- ����Զ����XPCOM���(xpt��dll)������ʹ�� C++��JavaScript��Python��ʵ�֡�
*     cmdline.js
*   [defaults] -- ���һЩĬ�ϵ���������
*   
* ��װ��ʽ��
*   1.xpi
*   2.����ʱ����չ�����ļ�
*   3.ע���(������չ�����ļ�)��HKCR �� HKLM �� Software\Mozilla\Firefox\Extensions 
*
* ���ǩ������
*   1.mccoy ???(Я�̵Ĺ���?) -> �����ѹ���ļ�����չ����Ϊ xpi;
*   2.sha1sum.exe xxx.xpi �õ�У���룬copy�� update.rdf;
*   3.sign �� verify (mccopy �ϵİ�ť��ʵ������ô��?)
********************************************************************************************************/

