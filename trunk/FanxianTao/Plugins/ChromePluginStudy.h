#pragma once

//�����Եĵ�ַ��C:\Documents and Settings\fujie\Local Settings\Application Data\Google\Chrome\User Data\Default\Extensions\keigpnkjljkelclbjbekcfnaomfodamj\3.5.5_0
//TODO: JSON �ĵ��ĸ�ʽ -- ���еļ�ֵ�Զ�Ҫ�����Ű�����? ÿһ�Դ�����������һ����涼û�ж��ţ������ÿ��֮������ö��ż��?
// ������� -- ?
// ͼƬ��ͼ���ԵӦ��ʹ��alpha͸���������Ļ������ںϵ����ֲ�ͬ�������������

/***************************************************************************************************************
* Chrome -- ����Webkit����һ�������
* ����ģʽ -- ��֤�ڸô���������������ºۼ���ȱʡ����£���չ�ǲ��������ڸ�ģʽ�µ�
*
***************************************************************************************************************/

/***************************************************************************************************************
* 
* �ٷ���չ�ĵ���http://code.google.com/chrome/extensions/dev/devguide.html 
* 360������ĵ���http://open.chrome.360.cn/html/dev_doc.html
*                http://open.chrome.360.cn/extension_dev/overview.html
* ����IDE�� Notepad++
* 
* Chrome��չ��ʽ�ж�������
*   browser_action -- �������Ϊ��չ, �����������������Ͻ���ʾһ��icon�����icon������Ӧ�û��������(default_popup).
*   page_action -- ҳ����Ϊ��չ���ò������ƥ��ҳ����������ַ�����ұ���ʾһ��icon������ֻ���ض���ҳ��������
*   theme -- �������������ṩ�ı���������ⱳ����һ������������HTML��JS���룬��Ҫ�Ǹ���ͼƬ����ɫ�ȣ���ͨ�� crxtheme ��������
*   app -- ��WebApp����chromeӦ�ó���ҳ����ʾһ��ͼ�꣬����󵼺���ָ����url����򵥵�һ�����
*
* ��չ(Extension)�� WebApp��Ƥ�� ��
*   
***************************************************************************************************************


/***************************************************************************************************************
* 
* browser action
* page action -- ����չ��ͼ���Ƿ���ʾ������ȡ���ڵ�����ҳ��ʱ
* Content Script -- ���ݽű����������ҳ������ͨ����JS��д��������ҳ������ɺ���ã��ɿ�����ҳ��һ����(������չ��һ����)��
*   ��һЩ����(�粻��ֱ�ӷ�����չ�ж���ı���)����ͨ��message���ƽ����
* 
* ����
*   �򿪲��ҳ(chrome://settings/extensions) -> ��ѡ"������ģʽ" -> ����������λ���ļ���
*
* ���
*   ������ߴ��
*
* ��ȫ����
*   Content Security Policy(CSP)--
*
* ������Google����չ����(ѹ���� zip ��ʽ�ύ��� )
*   https://chrome.google.com/webstore/developer/update
***************************************************************************************************************/


/***************************************************************************************************************
manifest.json -- ���úͿ��������������Դ, JSON��ʽ��������UTF-8��ʽ���档 http://open.chrome.360.cn/extension_dev/manifest.html
  "manifest_version": 2,     //���ڿ����Ķ��� 2.0�ģ�1.0 ���Ѿ���ʱ��

  "app": {			//APP ��������Ϣ
	 "launch": {
	   "web_url": "http://www.xxxx"
	 },
	 "urls" : [ "��������ַ, �� http://xxxx" ]
  },
  "default_locale" : "zh_CN",	//ȱʡ�ı��ػ�Ŀ¼���� en �� zh_CN���� _locales Ŀ¼����Ҫ�ж�Ӧ����Ŀ¼"
  "description" : "������Ϣ�����ܳ���132���ַ��������Ǵ��ı���TODO: ��ʾ������",
  "icons" : {  "16":"icon16.png", "128":"icon128.png" },  //��С+ͼ���ļ��ķ�ʽ
  "name" : "FanxianTao",						//��չ������",
  "version" : "1.0",							//�汾��Ϣ
  "permissions": "tabs, [http://xxx/xxx/]",		//Ȩ����Ϣ����Ȩ���ܷ��ʵĵ�ַ(�������ܿ������)��������Ǻ�(*)���������Է����κ���վ

  //Ӧ�����ͣ���ѡһ�����߲�ѡ
  "browser_action" : { "default_icon": "icon.png", "default_popup": "popup.html"}, 
     //ע�⣺�ܶ��ĵ����� popup, ʵ��Ҫ default_popup ���С���ʹ��default_popup��background�ű���ע���onclick�¼���ʧЧ
  "page_action" : { },
  "theme" : { "images":{xxx}, "colors":{xxx}, "tints":{xxx}, "properties":{xxx} },
  "app" : {},

  //��̨�߼����¼�������򣬻���һ�������ĺ�̨�������У�����ÿ��������������߲�����¼���ʱ�����Զ�ִ�У�
  //����Ҫ��������ָ������backgroundʱ�Ľű��ļ�, ע�����������Լ����������¼��������Ӷ��ﵽ����������������߼������ã�
  //ͬʱҲֻ��������ָ���Ľű��вſ���ʹ��chrome����ṩ��API�ӿڣ�Ҳ������ҳ����ͨ��
  //ֻ��������������ز��ʱ����һ�Σ�����ֱ����ʾ���������ں�̨���С��ռ�����Ľ�����Ա��浽ȫ�ֱ���localStorage�д��ݸ�popupҳ��
    "background": {
      "page": "assets/views/background.html"
      //"scripts": [ "jquery-1.8.0.min.js", "background.js" ],
      //"matches": [ "http://x/x", "https://x/x" ], 
      //"css": [ "plug_style.css" ]
    },
  "plugins": [ �����ƵĲ�� ],
  "update_url" : "http://path/to/updateInfo.xml"
  "homepage_url" : "��ҳ",

  "content_scripts" : [ { all_frames, matches,run_at }, {} ]  -- ������ݽű���Ϣ������

  //ʹ���Զ���ҳ�渲��ChromeĬ��ҳ��
  "chrome_url_overrides" : {   
	"newtab" : "newtab.html"		//�±�ǩҳ
  },

popup.html -- 

***************************************************************************************************************/

/***************************************************************************************************************
��չAPI(chrome.), ͨ����Ҫ�� mainfest.json �����ö�Ӧ��Ȩ��(permissions)
  bookmarks -- BookmarkTreeNode(�ڵ㣬��ǩ���ļ���)
  extension
    .onMessage -- �¼�, addListener(function (request, sender, sendResponse){ });
    .getURL('manifest.json') -- ����ָ����URL
    .sendMessage({json param}, function(response){ }); -- ������Ϣ��Ȼ����� onMessage �Ĵ����¼��д���

  tab
    .url -- 
  tabs
    .create({url: "www.baidu.com"}) -- ��URLҳ��
	.executeScript(null, {code:"document.body.style.backgroundColor='red'"});
    .getSelected(null, function(tab){ tab.url; }); -- ��ȡ��ǰ�����ҳ��?

BrowserPage ��صĺ���(��ô����?)
  setIcon,setTitle,setBadgeText(ͼ���ǣ���δ���ʼ���),setBadgeBackgroundColor,setPopup

��ͬҳ���Ľ���
  chrome.extension methods such as getViews() and getBackgroundPage()
***************************************************************************************************************/