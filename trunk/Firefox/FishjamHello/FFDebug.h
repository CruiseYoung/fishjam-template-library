#pragma once

/*******************************************************************************************
* TODO:
*   1.Firefox ����ʱ���ʵ������� FireFox\omni.ja(zip��ʽ��ѹ����)�����Բ鿴ȷ��ϵͳ��һЩʵ�ַ�ʽ?
*   2.������ DOM Inspector ������ chrome://xxx/xxx.xul ���е���(���Կ���xul�еĶ�����������Ч��)
*   3.about:support(�� �ļ�->����->�����ų���Ϣ) ���Բ鿴��ǰ��FF��Ϣ(�����������չ���ڴ�ʹ���������Ĺ�����ѡ���İ汾��)
*   4.��ʹ��һ����չ���������ӵ���������ڵ�ʱ����javascript ������κε��ӵ�browser.xul ������ȫ�ֱ����ͺ��������Ϊ
*     browser.xul ���ڶ�������ԣ����ֱ���������������һ�������÷�װ�����С�
*
* Firefox ����ʱ���������( firefox.exe -h | more -- ����ͨ���ض��������ɣ�Ҳ���� > help.txt)
*   -console    <== �򿪵���Console��������ʾ dump �ȵ���Ϣ
*   -jsconsole  <== ����ʱ����"�������̨",Ҳ���������"����->Web������"�д�
*   -no-remote  <== �����ջ���Զ������(remote commands), �������������ʵ������������
*   -private    <== ������˽���ģ(����¼���λỰ���κ���ʷ��¼)
*   -ProfileManager <== �����ù������ķ�ʽ����������ѡ�������ļ���Windows �µȼ��� -P ?
*   -safe-mode  <== 
*
* ������չ�����ļ�(�ı��ļ���������Ϊָ����չ�ļ���Ŀ¼ -- �޸Ĵ��������Firefox������Ч�����ô��ѹ����xpi)
*   1.��λ����չ��װ��·���� %USERPROFILE%\Application Data\Mozilla\Firefox\Profiles\<profile_id>\extensions\
*     �� Win7: C:\Users\%USERNAME%\AppData\Roaming\Mozilla\Firefox\Profiles\XXXX\extensions\
*       WinXP: C:\Documents and Settings\%USERNAME%\Application Data\Mozilla\Firefox\Profiles\XXXX\extensions
*     TODO:�µ�FF�汾��Ҫ����Ϊ %FIREFOX_HOME%\browser\extensions\ Ŀ¼
*   2.����һ���ı��ļ�������չID(��install.rdf�ļ��е�<em:id> )��Ϊ�ļ����������ļ���û���ļ���չ�������ʹ��ID��GUIDֵ����ô�ļ���Ҳ���������{}����
*   3.�ı��ļ������뿪��Ŀ¼��ȫ·��(�磺c:\extensions\my_extension\)��·�������ԡ�\����β����֮�����пո�
*   4.����Firefox��Firefox����⵽�ı�������չĿ¼������װ��չ��
*   ���䣺
*     a.��jar(���ձ�Toolbar��FFJpToolbar.jar)�Ļ������԰�jar��ѹ�������� chrome.manifest ����ص�·��(content,skin,locale��)
*     b.(δȷ��)��װ ��Ϊ Remote XUL Manager ����չ��FF�˵��� Web Developer -> Remote XUL Manager -> ���LocalFiles, ˫��xul�ļ���ʹ��FF�򿪣��Ϳ�ִ��XUL�Ĳ��ֺ͹�����
*       https://addons.mozilla.org/en-US/firefox/addon/remote-xul-manager/
*     c.Ϊ�˵���ͨ�� overlay ���ص�FF�������ϵĶ���(����ϵͳ�˵�����������),����ͨ��������д� "browser.xul"�ķ�ʽ���ԣ����ɲ�����FF�������ĺ��Ч����
*       ��: chrome://browser/content/browser.xul
*
* FF����(��ַ������ about:config�������Ǹ�������Ŀ¼�µ� prefs.js )
*   ����ѡ��:
*     browser.dom.window.dump.enabled��	= true //����ʹ�� dump() ������׼����̨�����Ϣ���ű������ͨ�� Interface("nsIConsoleService")
*     devtools.chrome.enabled			= true //(ToolsMenu��������ع��ܰ�ť?)����ͨ��"Tools menu"�е�"Scratchpad"����JavaScript����Ƭ��
*     extensions.logging.enabled		= true //������չ��־
*     javascript.options.showInConsole	= true //�� JavaScript �ĳ�����Ϣ��ʾ�ڴ������̨
*     javascript.options.strict��������	= true //�ڴ������̨�������ϸ�� JavaScript ������Ϣ���������Ϣ����ͨ�� Console2 �������
*     network.http.use-cache			= false //����ҳ��ʹ�û���
*   Ϊ��ͨ���ϵ����plugin(NPAPI)��һ����Ҫ������������
*     dom.ipc.plugins.timeoutSecs		= 999	//xx��ʱ?
*     dom.ipc.plugins.processLaunchTimeoutSecs	= 999;	//����ʱ��ʱ
*     dom.ipc.plugins.parentTimeoutSecs	= 999	//xx��ʱ?
*   �Ҽ�->�½�ѡ��:
*     dom.report_all_js_exceptions		= true  //�鿴JavaScript�쳣����ϸ��Ϣ
*     extensions.checkCompatibility		= false //����������(�ƺ�����?)
*     nglayout.debug.disable_xul_cache	= true  //���� XUL ���棬ʹ�öԴ��ںͶԻ�����޸Ĳ���Ҫ����(�� ���� dtd��)
*     nglayout.debug.disable_xul_fastload = true
*  TODO:
*     signed.applets.codebase_principal_support = true //����Ϊtrue�Ժ���Դ� UniversalXPConnect Ȩ��? ��δ���Գ���.
*     general.useragent.locale  = zh-CN		//���ñ����������ͣ����Բ��Զ�����
*
* �������������Ե���չ 
*   DOM Inspector <== �鿴�ͱ༭��ҳ�ĵ���XULӦ�ó����DOM������ͨ���������е�"Find a node to inspect by clicking on it"��������Ԫ�غ�λ��
*     ��װ"Element Inspector"����� ͨ�� Shift + �Ҽ� ֱ�Ӷ�λ��
*     ���⣺��λ��һ��Ԫ�غ���ôȷ�����ĸ���չ�ṩ�ģ�����ô֪����Ӧ�� xul ���ļ���
*   Venkman(����Ϊ JavaScript Debugger) <== JavaScript �ĵ��Ի������������öϵ㡢�鿴���ö�ջ��
*     https://addons.mozilla.org/en-US/firefox/addon/javascript-debugger/
*     ע�⣺��Ҫȡ�� "Debug -> Exclude Application Files" ��ѡ�񣬷���ϵ㲻����Ч
*   XPCOMViewer <== �鿴��ǰ��װ��XPCOM�ؼ����鿴����Ӧ�Ŀؼ��󣬿�ͨ���Ҽ���"Generate Code"�Զ�������صĴ��롣
*		ע�⣺�����ɵĴ����д���--����getInstance,���� createInstance �� getService
*   Extension Developer 
*   Console2 
*   Chrome List
*   Chromebug(��ô��? ��������Ҫ -chromebug) -- ����ʹ����������Firefox����Ҳ���������ѧϰFirefox ��UI(chrome)����ι��ɵġ�
*     http://getfirebug.com/wiki/index.php/Chromebug_User_Guide
*   Developer Assistant(��ô��?) -- ��չ��������
*     https://addons.mozilla.org/en-US/firefox/addon/extension-developer/
*   Firebug -- һ����Ϊ���ӵ����ڼ��Ӻ͵���CSS,HTML �Լ�javascript �����翪������
*   Greasemonkey -- ��ʹ���߰�װһЩ�ű�ʹ�󲿷�HTMLΪ������ҳ��ʹ���߶�ֱ�Ӹı�ø��������á�
*   Scrapbook -- һ����ҳ����������
*
* ���Է����ͼ���
*   JavaScript
*     1.dump("string") �����������ն��������Ϣ, ���� FF ʱ��Ҫ���Ӳ��� -console
*       console.log("xxxx") ?
*   XPCOM
*******************************************************************************************/
