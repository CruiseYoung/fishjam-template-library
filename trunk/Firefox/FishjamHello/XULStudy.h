#ifndef FF_XUL_H
#define FF_XUL_H
#pragma once

//TODO: ���ֻ���XML�����ԣ���XHTML��SVG���е�Ԫ�ض�����Ƕ�뵽�����Ԫ������Ϊһ����չ���Եķ�����
//      ��ʱ��Ҫͨ�����ƿռ�������֣��� XHTML("http://www.w3.org/1999/xhtml") �� SVG("http://www.w3.org/1999/svg" ?)
//      XUL �����ƿռ�Ϊ���� http://www.mozilla.org/keymaster/gatekeeper/there.is.only.xul
//
//����ͨ�� XUL Gear �Զ����� ��ص�XUL�ļ�?
/*******************************************************************************************************
* ע�⣺
*   1.�������ô������<listbox>���ݵķ�ʽ��
*   2.���ݲ�ͬ����������UI�Ĳ���(�糤��) -- �磺style="width:&attachmentextractor.settings.width;; height: 55em;"
*     Ȼ���� dtd �и��ݲ�ͬ���������ö�Ӧ�Ŀ�ȡ�ע�� ����� "width" ���������ֺ�,�ֱ��� ��Դ���� �� style������
*     http://www.babelzilla.org/index.php?option=com_content&task=view&id=357
*   3.�������֧��"Զ��XUL"���� -- ��ͨ�� file:///C:/abc.xul �ȵķ�ʽ��ʾ�ⲿxul�� security.checkloaduri ��
*    
* ѧϰ��ַ��https://developer.mozilla.org/en-US/docs/XUL  / https://developer.mozilla.org/en-US/docs/XUL_Reference
*           http://wiki.woodpecker.org.cn/moin.cgi/LearningXul
*           http://blog.donews.com/limodou/archive/category/xul
* 
* XUL(XML User Interface Language) -- Ӧ��XML�������û�����ı�ʾ���ԣ����UI���ƣ��ȿ�����Զ��ִ�У�Ҳ���԰�װ������ִ�С�
*   ͨ�� Gecko ���������ʾ��ͨ��"XUL Periodic Table"���߿��Բ�ѯ���ָ����Ĳ���?
*   XUL�ṩ��һ�׼��׺Ϳ�ƽ̨��widget����,���Դ�������ͼ�ν�����������Ԫ��, ͬʱ�������¼��������(style) ���ַ������ʻ�����Ϣ������ֱ�ӱ�mozilla layout�������ִ�С�
*   XUL Explore -- ��������XUL���ɿ�������(https://developer.mozilla.org/en-US/docs/XUL_Explorer)�����ǲ��Ժ��񲻺���
*   XUL�ļ���Ҳ����дXHTML����(��������)������������XHTML���ֿռ䣨��window�ڵ��������� xmlns:html="http://www.w3.org/1999/xhtml"��,
*   �����е�XHTML��ǩ��Ҫ�� html:,����<p>Ҫд�� <html:p>
* 
* һ��XUL����һ����������������ݣ�
*   ����: ����ʹ���߽����XUL��
*   Skin: ���������۵�CSS��ͼ��
*   Locale: ��������ִ���DTD����Ϊ��������ػ���
*   
* Overlay -- ���Խ����XUL �ĵ�����������������һ��������XUL�ĵ�������FF�������Ը��ֹ��ܽ���ģ�黯��ʵ������չ��
*   ������<xul-overlay>����ָ��(����xml-stylesheet?)������Ҫ��ϲ�������XUL�ļ�?
*   �� <?xul-overlay href="overlayDocument.xul"?>�� ��Ӧ��"overlayDocument.xul"�ĵ���Ҫ�� <overlay>��Ϊ��Ԫ��
*   
* 
* ����
*   xulrunner.exe -- 
* 
* XUL(https://developer.mozilla.org/En/XUL)
*   ���ߵ�XUL�༭��(����ʵ�ⷢ�ֲ��ٿ��ã�) -- http://ted.mielczarek.org/code/mozilla/xuledit/xuledit.xul
*                                              http://www.hevanet.com/acorbin/xul/top.xul
*
* XUL ������ǶJS�ű�(����û���д�ڶ������ⲿ�ļ���)����Ҫ���������� CDATA ����
*   (���Ա����ɶ�ȡ&�������ַ���ͷ��ʵ������ʱ���ִ���)���磺
*   <script type="application/javascript"><![CDATA[
*     var nodes = gBrowser.mTabContainer.childNodes;
*     for (var i = 0; i < nodes.length; i++)
*     alert(nodes[i].label);
*   ]]></script>
*******************************************************************************************************/

/*******************************************************************************************************
* ������ͨ������
*   [align/pack] -- ָ��Ԫ�ز��֣�����ȱʡΪstretch(��չ�������ƥ�������ڵ�Ԫ��)�������� start(����|��˶���), 
*      center(����), end, baseline(ˮƽ�߶��룬ֻ������hbox��)��
*      align���Զ���Ϊ��orient�����ഹֱ�ķ��򣬶�pack������orient���ԵĲ�������һ�¡��������������9��λ�Ͻ��в��֡�
*   [class] -- ΪԪ�ط���
*   [collapsed] -- �������Ϊ"true" ��ʾ��һ��Ԫ�صĸߺͿ�����Ϊ0�� �������Ԫ�ػ��ǻᱻ��Ϊ�Ǵ��ڵģ��� CSS ������"visibility:collapse"Ч����ͬ��
*   [crop] -- �ı���ȴ����丸Ԫ�صĿ��ʱ��ʡ�Ժ�(��)����
*   [disabled] -- ���ö���ͨ����ʾ��ǳ��ɫ����͸����
*   [flex] -- ˵����һ��Ԫ��Ӧ����չ��ռ��һ�����ڵĸ߶ȺͿ�ȵı�����ֵΪ����������ʾ�ڸ�Ԫ�ص�orient���Է����ϵ�����������
*   [height/width] -- ����׼ȷ�ĳߴ�
*   [hidden] -- ����Ԫ���Ƿ���ʾ���� CSS ������ "display: none" Ч����ͬ��
*   [id] -- ����һ��Ԫ�ص�Ψһ��ʶ���Ƶ�
*   [minwidth/minheight/maxwidth/maxheight] -- ͨ����������flex���Ժ��ʾ��С���Ա仯��������С�����ߴ硣
*      TODO:CSS���� min-width ?
*   [ordinal] -- ���ò���˳��(Ĭ�ϰ��մ����г��ֵ�˳�򲼾�),
*   [orient] -- ��ֱ(vertical)��ˮƽ(horizontal)����
*   [persist] -- �־û�����("Persistent Data" )����Ҫ����Ԫ�ص�һЩ״̬����(�細�ڵ�λ�úʹ�С)ʱ������ persist ����Ϊ�ո�ֿ������������ɣ�
*      �磺��� window �� persist="screenX screenY width height"��  
*      ע�⣺1.Ҫ�������ݵĶ�������� "id" ����;
*            2.��Ӧ��ֵ�������û�����Ŀ¼�µ� localstore.rdf �ļ���
*   [tooltiptext] -- ������ʾ��ֻ��������ʾ�ı����������ḻ�����ݣ���Ҫʹ�� tooltip Ԫ�ء�
*   
*
* ����Widget
*   <arrowscrollbox> -- ����ͷ�Ĺ��������ڲ����Է����κ�Ԫ��(ϵͳ���Զ�ʹ�ã�)
*   <box> -- orient ���Կ�ѡ horizental(ˮƽ���ȼ��� hbox) ��  vertical(��ֱ, �ȼ��� vbox)
*   <browser> -- ���ܱ�<iframe>��ǿ�������֡��������ҳ����������л�������(ǰ��/����/��ֹ�ڲ��ű������ⲿ��Դ)
*     [type] <== ������Ϊ"content"�����÷�������(���д򿪵���ҳ��������Ϊ����֡�����ܷ��ʸ�֡?)
*                ������Ϊ"content-primary"����ʾ������XUL�ĵ���ͬ����������������ڶ����ʹ��"window.content"������
*   <button> -- ��ť������ͨ�� type="menu" ����Ƕ <menupopup> ��ʵ�־��������˵��İ�ť�����type����Ϊ menu-button ?
*      ͨ��[icon]��[image]������ʾͼ��?
*   <caption> -- ?
*   <checkbox> -- ��ѡ��ѡ��ʱ [checked]Ϊtrue
*   <command> -- ͳһ�������Զ��Դ(��˵����ݼ��� )�������Ҫ���� id���ԣ�Ȼ������Դ�� command ���ԡ�
*                ��������� "chrome://global/content/globalOverlay.js"�󼴿�ͨ�� goDoCommand ��������
*     [oncommand] -- �¼��������
*   <commandset> -- ���ɼ�Ԫ�أ���Ƕ <command>��ͨ������ �� <window id="main-window"> �� overlay ������һЩȫ�ֿɵ��õ� command
*   <conditions> -- ? rule ����������ƥ��ʹ�õ�����������������Ԫ�أ�content, member, triple��
*      <content> -- ֻ��дһ�Ρ�����������ģ���Ԫ���е�rel��ָ����ʼ����Ӧ��һ��������
*      <member>  -- ����ӳ��ÿ����Դ�ı���
*      <triple>  -- ��Ԫ�飬��������ƥ�����,һ��triple����Ҫ����ɣ�����(subject), ν��(predicate), object(Ŀ��)��
*   <context> -- �����Ĳ˵�����Ҫ���� <popupset> ��������Ԫ��ͨ�� [context] = contextId ��������
*   <dialog> -- �Ի��򴰿ڵĸ�Ԫ��
*      [buttons] <== �ö��ŷָ�����������ײ���ʾ�İ�ť( accept,cancel,help,disclosure--������Ϣ)�����Զ�����OS���в���(��Mac OS ��Cancel�������?)
*                    �������ⰴť, extra1 ʹ�ñ�ǩ buttonlabelextra1�� extra2 ʹ�ñ�ǩ buttonlabelextra2��
*                    �����¼�: ondialog<buttonname>����Ҫ "XPConnect privileges" ,
*   <deck> -- �����ɶ����Ԫ�أ���һ��ֻ��һ����Ԫ���ǿɼ��ġ�ͨ�� selectedindex �������л��ɼ�����Ԫ�أ���һ����Ԫ�ص�����Ϊ0��
*   <description> -- ���ε������ĵ�
*   <frame> -- ��Ƕ֡?
*   <grid> -- ��񲼾֣�����Ƕ <columns><column>(��¼ÿ����¼���ֶ���Ϣ��һ�㲻�����ݣ���Ҫ�����������,�� [flex]) 
*             �� <rows><row>(һ�����ڷ�������) �ؼ�����ʾ
*             ʹ��grid��Ƕ�׵�box�����ô��ǿ����ô����ĵ�Ԫ����ˮƽ�ʹ�ֱ����ͬʱ��չʱҪ����ö�
*   <groupbox> -- ��򣬿��԰ڷŶ��Ԫ�أ���ʾһ����ͱ���
*   <iframe> -- ��ʾ������ҳ��( �õĺ��٣�ʵ���ϳ���<browser> ?)
*     [src] <== ָ������ҳ���URL��ַ(�� http://xxx, chrome://xxx)
*   <image> -- ͨ�� src="image.jpg" ʹ��ͼƬ(���õķ�ʽ��ʹ��CSS?)
*     list-style-image <== ����Զ���ת������?(�� url("chrome://browser/skin/tabbrowser/loading.png") )
*     list-style-type: square <== ͼ�񲻿���ʱ?
*   <key> -- ���� window ȫ�ֵĿ�ݼ�����������<menu>��<menuitem>�� [key]Ϊ��Ԫ�ص�id
*     [command] <== ���� command Ԫ�ص�id���������Զ��Դͳһ���� Ҳ�������� [oncommand] ����
*     [modifiers] <== ���ù��ܼ����� "accel,alt", ���� accel ��ʾ����ϵͳ��ȷ�Ϲ��ܼ�(Win����Ctrl,Mac����command)
*     [key] <== ��Ӧ�İ������� "C",ע�⣺�����ִ�Сд
*     [keycode] <== ���ⰴ������������� "VK_UP","VK_DELETE" ��
*   <keyset> -- ���ɼ�Ԫ�أ���Ƕ <key>�������ÿ�ݼ�
*   <label> --  �������ı���ǩ��[value]="��ʾ�ַ�"����Ҫ��ʾ���ı�����Ҫ��������[flex]="1"����Ƕ�ı��ķ�ʽ(������[value]����)
*     [control] ="myControlId" <== ָ�������Ŀؼ������label��ʹ�ؼ������뽹�㼤��
*   <listbox> -- �б��ͨ�� <listhead><listheader label="XXX">(�����ͷ) �� <listcols><listcol>(�����е����) ������ۣ�ͨ�� <listitem> �������ݣ�
*      ���ڲ����԰����κ�Ԫ�ء�
*   <menu> -- ϵͳ�˵����е�һ��� "File",��Ƕ <menupopup> �ṩ�����˵��� Ҳ������Ϊ�����˵��� ��Ԫ��(���滻��Ӧ�� <menuitem>,Ȼ����Ƕ <menupopup>
*   <menubar> -- ϵͳ�˵�������Ƕ <menu><menupopup><menuitem> �ṩϵͳ�˵�����
*   <menuitem> -- �����˵��еĲ˵���丸Ԫ��ͨ���� <menupopup>��ͨ�� label �ṩ��ʾ���ı�
*      ��� class="menuitem-iconic", �����ͨ�� image �������ò˵�ͼ��
*      [type] <== checkbox(ѡ�б�ǣ�ѡ�����"checked"=true),radio(���name������ͬ��Ϊһ�飬ѡ�����selected="true")
*   <menulist> -- �����б������Ƕ menupopup ��
*     [editable] <== ������Ϊ"true"����Ҳ���Բ��������б���ѡ�񣬶��ֹ�����(���ı�ѡ����)
*   <menupopup> -- �����˵������ɼ�(������Ҫ���� label ����)����Ƕ <menuitem> �� <menuseparator> ��
*   <menuseparator> -- �˵��ķָ���
*   <overlay> -- ���Ժϲ�������XUL��ȥ��XUL�ļ��ĸ�Ԫ�أ�
*   <page> -- ������Լ������ڴ��ڿ���ڲ��򿪵�XUL�ĵ��ȵĸ�Ԫ�أ���Ҫ��Ƕ iframe ָ�� XUL �ļ� ���ṩ����
*   <popupset> -- �������ݣ����ڲ����� <menupopup>,<panel>,<tooltip> ����Ԫ�أ��ֱ��Ӧ �����Ĳ˵�([context]),��ͨ����(������),������ʾ([tooltip])��
*      ע�⣺�����õ�λ�ò��ɼ�����Ҫͨ����id����������ʽ������������ʱ����ʾ������
*   <prefwindow> -- ���ô���? ����ȷ����ȡ���İ�ť
*   <progressmeter> -- ��������ͨ�� mode="determined(ȷ����)|undetermined(��ȷ����)" ѡ������;
*      ��ģʽΪȷ��ʱ������ͨ�� value="50%" ���ý���λ��; ��Ϊ��ȷ����ʱ�����ȳ������±�ʾ�������ڼ���
*   <radio> -- ��ѡ��ť����ǰѡ���Ԫ��[selected]="true",������<radiogroup>��[value]����Ϊѡ��<radio>��Ӧ��ֵ
*   <radiogroup> -- ��Ƕ radio ��ť��ֻ��һ����ѡ��
*   <rule> -- �ڴ���ģ�����ɵ�����ʱָ�����򣿿�����Ƕ <conditions>,<bindings>,<action>������Ԫ�ء�
*      ģ�����������ȸ���conditions�е���������Դ����ƥ�䣬���ƥ��ɹ����������Ӧ��action�е����ݽ��и��ƴ���
*   <scale> -- ������(�����������ƻ���)
*   <scrollbar> -- ����������ͨ�� orient ���÷���
*      [slider?]
*   <spacer> -- ռλ�հס����flex����Ϊ"1"�ҵ�ǰ��Ԫ����ֻ��һ��Ԫ���и�����ʱ�򵱴��ڱ仯ʱ�Զ��仯��ͨ���ɷ�����Ҫ�Ҷ���İ�ťǰ��
*   <splitter> -- �ָ�����ͨ���϶����������ı䲢��Ԫ�صĴ�С��
*      [state] <== �ڳ�ʼ��ʾʱ��collapse��ָ����Ԫ���Ǵ򿪻��ǹرա���stateֵΪ"open"��Ϊ�򿪡�stateΪ"collapsed"ʱΪ�رա�
*      [collapse] <== ָ���ĸ�Ԫ�ؿ��Ա��ر�(����)����ѡֵ�� before|after|both|none 
*   <stack> -- ջ���������ڲ���Ԫ���ǰ�������˳���ص��ڷţ��´����ķ��������棬
*      ��;������ɫ��ͬ���ִ��ص�����ʵ��һЩ��ӰЧ����<progressmeter>��<label>һ����ʾ���ı��Ľ�����
*   <stringbundle> -- ͨ�� src="chrome://xxx/locale/xxxx.properties" �ķ�ʽָ�������ļ��������ļ���һ�� Key=Value �Ե��ı��ļ���
*      Ȼ�� JS �м���ͨ�����·��������Ӧ�ı����ַ���Դ��
*        var bundleString = document.getElementById("bundle��ID"); var strValue = bundleString.getString("��Ҫ����ԴKey");
*   <stringbundleset> -- ��Ƕ <stringbundle> Ԫ�أ���JavaScript���ṩ���ػ�
*   <tabbox> -- ��ǩҳ, ��Ҫ��� <tabs><tab label="��ǩҳ�ı���"/>  �� <tabpanels><tabpanel>��ǩҳ������</tabpanel> ������ʾ������
*      ���ߵĸ�����˳����Ҫ��Ӧ��TODO: ��û�а취�� tabpanel �����õ���� xul �ļ���
*      <tab> ʹ�� selected="true" ��ѡ��
*   <tabbrowser> -- ��<browser>��ǿ�󣬰����˱�ǩ�������ԵĻ������ݣ���Ҫ XPConnect Ȩ��
*   <textbox> -- �ı������
*     [multiline] <== Ϊ"true"ʱ������������ı�
*     [type] <== Ϊ"password"ʱ�����������, Ϊ"search"ʱ�������������չ��
*        Ϊ"number"ʱ����������������΢����ť��ͨ��[min][max][increment]���ø���ϸ����Ϣ
*        Ϊ"autocomplete" ��֧���Զ���ɣ�����Ҫ����[autocompletesearch]����ָ������Ŀ�꣬��"history"���ַ����ʷ,
*        "form-history"����������������ʷ(Ȼ����ô��?)��ע�⣺��Ҫ XPConnect Ȩ��
*     [oninput] <== �����û������ʵʱ�¼�
*   <toolbar id="xxx"> -- ���������ڲ���� <toolbarbutton> ��
*      [defaultset] -- ָ���ö��ŷָ����Ķ�� <toolbarpalette>
*      [grippyhidden] -- ����Ϊ true ��ʾ�رչ�������С��(toolbargrippy)
*   <toolbarbutton> -- ?���幤������ť��Ԫ��
*      [type] <== ��ѡ"checkbox","menu","menu-button(�����/ǰ����ť)" ��
*   <toolbargrippy> -- 
*   <toolbaritem> -- ������Ƕ��
*   <toolbarpalette> -- ����<toolbar>�е�һ����壬������Ƕ <toolbarbutton> �Ȱ�ť�������ɼ�������<toolbar>��������items����ʾ��
*      ���� overlay ��ͨ������ id Ϊ BrowserToolbarPalette ������Firefox�Ĺ�����
*   <toolbox> -- �Թ��������飬��Ҫ��� <toolbar><toolbarbutton> ��ʹ�ã��������һ��С�ۣ�����ɴ򿪻�رչ�����
*   <tooltip> -- ���幤����ʾ�����ڲ����԰����κ�Ԫ�أ����úú�ͨ��������Ҫ��ʾ��ʾ��Ԫ�ص� tooltip="myTooltipID" ������
*   <tree> -- �������ɷּ��ṹ�ͷּ����б�ṹ����ֻ�ܰ����ı���ͼƬ������ʹ������Դ������ģ�������ɡ�
*   <window> -- ������ͨ���ڣ��� Firefox��������ڡ���ǩ���������� ��
*     [windowtype] -- �������ͱ�ʶ������������������԰���ͬ���͵Ĵ��������һ���� ���������"navigator:browser", ѡ���"Browser:Preference" ��
*   <wizard> -- ��ʾ��ҳ
* 
*   
*   
* ģ����������(Mozilla�еķ�ʽ����֪���Ƿ��ʺ�Firefox?)
*   1.����Ԫ�ص� [datasources] �� [ref] ����ָ������Դ
*   2.�ṩһ��Ԫ�ص�ģ��(template)��ʾһ���������������԰�Ŀ��XML�ļ��е�����������Ԫ���ҵ� -- ���� xslt ?
*     ��ģ���е�Ԫ������ֵ�������rdf:��ͷ�ģ��������ֵӦ���Ǵ�����Դ���ġ�rdf:�����ֵ��ʾ����Դ�е���������
*   3.����ʹ�� rule Ԫ������һЩ��Դ���в�ͬ�Ĵ���
*   ���ӣ���bookmarks �е���ǩ���ɰ�ť(�Ƿ��ܴ�Firefox���ҵ�Դ��?)
*
*   ��ô�����Լ���RDF����Դ ?
*
*
* �¼�����
*   XUL���¼�����ʽ��HTML�еĻ������ơ�������Ҫ��Ӧ�¼���Ԫ�أ�ֻҪ��Ԫ����������Ӧ���¼����������ɡ�
*   XULʹ����DOM2 Events�������¼�ģ��һ��
*     һ���¼��ķ���������״̬������״̬��ð��״̬��
*     �ڲ���״̬��һ���¼��ȷ��͵�document��Ȼ����Ԫ�صķּ��ṹ��㴫�ݣ�ֱ�������¼���Ԫ�ء�
*     ð��״̬�Ƿ����򴫵ݡ�
*     �ڴ���·���ϣ�����¼���ĳ��Ԫ�ش�����ô�¼����ݼ����������û�б����������������һ��Ԫ�ء�
*     �����û�д�����ȱʡ�Ĵ���ִ�С������ζ�ţ���һ��Ҫ���¼�������ڴ����¼���Ԫ���ϣ����Ƿ������ĸ�Ԫ���н��д���
*     ���һ���¼�������������Ӧ�÷���true���������false���ʾδ��������ˣ���ʱ�����ڴ������¼�����Ȼ����false���Ӷ�ʹ�¼����Լ������ݡ�
*     ����ʹ��event�����target�������õ������¼���Ԫ�أ��� onclick="dump(event.target.tagName + '\n'); return false;"
*******************************************************************************************************/



#endif //FF_XUL_H