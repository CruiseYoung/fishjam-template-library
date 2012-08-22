#pragma once

//����Ƭ�Σ�https://developer.mozilla.org/en-US/docs/Code_snippets
//Helloworld: http://kb.mozillazine.org/Getting_started_with_extension_development (���� "download")
//firefox extension tutorial
//�ɰ汾��FF���أ�ftp://ftp.mozilla.org/pub/firefox/releases/, ��װ�ú���Ҫ ����->ѡ��->����->��������, ����鿴Aboutʱ���Զ�����

//��չ�����������ģ���չ���������������//��������ҳ��ģ���ҳ�������������ݸ�ʽʱ������
/************************************************************************
* ���������������(https://developer.mozilla.org/en-US/docs/Setting_up_extension_development_environment)
* a.Firefox ����汾���� -- �����ļ�����(ע�⣺�Ϻõķ�ʽ��ͨ�� -no-remote -p ������Ȼ��ͨ���Ի���ѡ�������ļ�)
*   1.���������ļ���%USERPROFILE%\Application Data\Mozilla��%USERPROFILE%\Local Settings\Application Data\Mozilla
*   2.��װ�µ�FF�汾���µ�Ŀ¼�����ܲ���������װ�ķ�ʽ������Ḳ����ǰ�İ汾?)
*   3.�޸Ŀ�ݷ�ʽ���Ҽ�->����->��Ŀ�ꡱ��Ϊ "{��װ Firefox ��Ŀ¼}\firefox.exe" -P {�汾��} -no-remote
*     -no-remote <== ����ͬʱ���ж��ʵ��
*     -purgecaches <== ��ֹJavaScript�Ļ���(fastload)
*   4.��������޸Ĺ��Ŀ�ݷ��̣�ѡ�񡰴��������ļ���������Ϊ {�汾��}
*     Firefox ���û��ĸ�����Ϣ���������á��Ѱ�װ����չ�ȣ���������һ�������ļ��У�Ĭ����ʹ����Ϊ default �������ļ���
*   5.���飺һ��������°汾�������������ҳ, ���ϰ汾ֻ�������� 
*     ���Ա������ú�װ�°汾���Ը���ԭ�汾������ (��Ϊ����Ҫ�̳���ǰ������, �Է���ʹ��)��
*              Ȼ����ȥ��װһ���ϰ汾�� Firefox �����������Ķ�汾���ݲ���
* ��������
*   aptana
*   spket <== http://spket.com/(���� JS �� XML����Ҫ�ƽ� com.spket.ui.internal.License )
*   XULBooster <== 
*   XPComViewer <== ���Լ��ע�ᵽFF�е�XPCom
*
************************************************************************/

/************************************************************************
* ������չ��ʽ
*   1.�����Plugins) -- ��Firefox�ܹ���ʾ�����ʽ���ļ�������Ӧ�ĵ�����Ӧ�ó����ṩ�Ķ������ļ�
*     (����ʾFlash��Adobe shockwave flash��) -- ʹ���Լ���View��չʾ
*   2.��չ��Extensions��-- �޸Ļ�����ǿFirefox����Ĺ��ܶ��ṩ��һ�ִ����ʽ,ͨ���ɰ������ܴ����js�ű������������xul
*     �ļ��Լ�����Ƥ����css�ļ��͸���ͼ���ļ���ɡ���չ��xpi��zipѹ������
*   2.1.��ͳ�Ļ���JavaScript��XUL�ķ�ʽ����ͳ�ģ�ʹ��XUL����չ������ǿ�󣬵��Ƚϸ��ӣ�����Ҫ��������װ
*       CSS(Cascading Style Sheets) -- �����ʽ�������������ַ�������⣬�����߿��Զ����仯��
*         ��FF�л����԰�XBL��XUL��������������ɶ�XUL����չ��
*       DTD(Document Type Definition) -- �ĵ����Ͷ��壬����DTD�е�ʵ�� (Entity)չ�����ƣ�����ַ����ı��ػ�
*         ���÷�ʽ��<!DOCTYPE window [<!ENTITY % brandDTD SYSTEM "chrome://branding/locale/brand.dtd" > %brandDTD; ]>
*         ʹ��ʱ�� label="&backCmd.label;"
*         ע�⣺DTD�ļ���Ҫ����ΪUTF8��ʽ(VS��ʾΪ Unicode(UTF-8 without signature) - Codepage 65001)��������ܳ�������
*       JavaScript -- FF�ں˵��ñ����Գ����ճ�ͼ������ҵ���߼���ͨ���������ñ����Եĳ��򣨱���c++�Ķ�̬�⣬API��
*         ���÷�ʽ��<script type="application/x-javascript" src="chrome://global/content/nsDragAndDrop.js"/>
*       XBL(Extensible Binding Language) -- ��չ�����ԣ���XUL�п���ֻ�������й��ԵĲ��֣�����XBL����������չ��
*          XBL ����Ϻͼ̳л��ƣ��������˿������ԡ�
*       XUL(XML User Interface Language) -- Ӧ��XML������ʹ���߽���ı�ʾ���ԣ����UI���ƣ��ȿ�����Զ��ִ�У�Ҳ���԰�װ������ִ�С�
*       properties�ļ� -- �� nsIStringBundle �ӿڵİ����£�javascript���Է���Ĵ�property�ļ���ȡ����Ҫ���ַ���������JavaScript���ַ����ı��ػ���
*          �ļ���ÿһ�е������� "key=Value" �ĸ�ʽ
*          ���÷�ʽ��<stringbundleset id="stringbundleset">
*                      <stringbundle id="minimo_properties" src="chrome://minimo/locale/minimo.properties"/>
*                    </stringbundleset>
*                    ��ͨ�� nsIStringBundleService/nsIStringBundle ʹ��
*       RDF(Resource Description Framework) -- ��Դ�������
*       SVG(Scalable Vector Graphics) -- ��ʾʸ��ͼ?
*
*   2.2. FF4 �Ժ� Add-on Builder �� Add-on SDK �ķ�ʽ, ����XPCOM��Ŀǰ���Ƶķ�ʽ��FF->JS->XpCom->C++ Dll
*        XPCOM(Cross Platform Component Object Module) -- ��ƽ̨�������ģ�ͣ���MS��COMʵ��
*          Mozilla�����Ŀ�ƽ̨����ṹ��ԭ��������΢���Com����������Եģ��ṩ��Ҫ���߼�����
*          ���÷�ʽ��ͨ����JS��ͨ��DOM�ӿ�ȡ�� -- gBrowser = document.getElementById("content"));
*            ʹ�ã�gBrowser.webNavigation.goForward();
*        ע�⣺��Ϊÿһ��FF�汾ʹ�ò�ͬ�汾��SDK��ͨ����Ʒ�Ĳ�����ֻ֧�������N���汾������ά���ϵ�����
*        
*    �˴����������⣿
*        �ص㣺restartless(bootstrapped extensions )����װʱ����Ҫ���������������ޣ�ʹ�� Add-on SDK �� Add-on Builder ��������
*        https://addons.mozilla.org/en-US/developers/tools/builder
*
*
* FF�б��ػ��������֣�
*   XUL �еı��ػ�ͨ��DTDʵ�֣�
*   JavaScript �еı��ػ�ͨ�� properties �ļ�ʵ��
*
*
************************************************************************/

/************************************************************************
* theme -- ����?

************************************************************************/

