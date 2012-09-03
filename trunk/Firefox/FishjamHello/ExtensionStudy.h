#pragma once

/*******************************************************************************************************
* ��չ������������(������)��https://addons.mozilla.org/en-US/developers/tools/package/
* ������չ��
*   All-in-One Gestures -- �������
*   GreaseMonkey -- ��Firefox ���ṩ��һ�����������û��ű�(JavaScript)�Ļ���������ű�����ָ��Ŀ����վ��
*   
* ע�⣺
*   1.? -- dtd��properties ����Դ�ļ������Ϊ "without signature" ��UTF-8���룬��������޷���ʾ
*   2. -- Ϊ��д�Ĵ�������Ȩ���Կ˷���ȫ���Ƶ�����
*   3. -- ��ͨ��xpcom������ʹ�� js-ctypes ��addon��ֱ�ӵ���dll�еĺ���(http://mozilla.com.cn/post/21692/)
*         ���԰���xpcom���������İ汾����������������֧�� Chrome(��û����� npapi ?)
*   
* ��չ�Ļ���Ŀ¼�ṹ(��չ��xpi��zipѹ����) -- https://developer.mozilla.org/en/Building_an_Extension
*   chrome.manifest -- Chrome ע����嵥�ļ�,���ø���XUL�ļ�·���Լ�������һЩ��Դ�ļ�·�����﷨���£�
*     content ��չ���� λ�� <== ������չ���������ö�Ӧ��Chrome���������ַ(jar����Ŀ¼�ȣ�ע�⣬���Ҫ��б�߱�ʾĿ¼)�Ķ�Ӧ��ϵ
*     overlay chrome://browser/content/browser.xul	chrome://helloworld/content/overlay.xul	<== ��FF�ṩ��browser.xul�Ϻϲ��Զ����UI��ͨ������չ����UI
*       ˵����1.FFͨ��"Chrome URIs"����ȡ��ص��ļ�����"chrome://browser/content/browser.xul", ��ֱ���ڵ�ַ������ȷ��,
*               FF�Ὣ��ת��Ϊ���̻�JAR����ʵ�ʵ��ļ�·����
*             2.����չ�п���ʹ�ø��ǣ�Overlay���������еĽ��������Ԫ��(��ϵͳ Tools�˵� <menupopup id="menu_ToolsPopup"> ),
*               �������Ը��ǵ�ϵͳxul�� chrome://browser/content/preferences/preferences.xul ��
*             3.FFϵͳʹ�õ� browser.xul ���ļ��ڰ�װĿ¼�µ� omni.ja ѹ������
*     locale ��չ���� ����ID λ�� <== ���ò�ͬ����ID(��zh-CN)��Ӧ����ԴĿ¼�����⣺��ô�л�����?
*       �� locale helloworld zh-CN chrome/locale/zh-CN/
*     skin ��չ�� classic/1.0 λ�� <== 
*     style chrome://global/content/customizeToolbar.xul xxx.css  <== ��������(xul)�ͽ�����(css)֮��Ĺ��� ?
*
*     XPCOMע��
*       C++���͵�XPCOMע�᣺
*         interfaces ���Ϳ�(.xpt)��λ�� <== ָ��XPCOM��ʵ�ֵĽӿ����Ϳ�
*         binary-component XPCOM��·�� ABI=WINNT_x86-msvc appversion<15 <== 
*       JavaScript���͵�XPCOMע��
*         component ���ID xxx.js <CR> contract ��ԼID ���ID <== ���� js �� XPCOM ֮�������(ͨ�����ID?)
*       Python���͵�XPCOMע��(PyXPCOM)
*         ��ô��???
* 
*   install.rdf -- ��������չ��ע����Ϣ�͸�����Ϣ�ȣ���Ҫ�����������ݣ�
*     <em:id> -- �� "{UUID}" �� "name@��ַ" �ķ�ʽ��ʾ 
*     <em:name> -- 
*     <em:version> -- 
*     <em:targetApplication> -- ָ��Ŀ��ϵͳ����Ϣ����
*        <em:id>��{ec8030f7-c20a-464f-9b0e-13a3a9e97384} -- Firefox App ID
*        <em:minVersion>,<em:maxVersion>
*    <em:updateKey> -- �Զ�����ʱ��"Public Key",
*
*   update.rdf -- ������ǰ��չ���°汾��Ϣ(�� ���URL?),����֧���Զ�����
*     0.���ز���װ HashTab ����,�������ļ������в鿴�� CRC32/MD5/SHA-1 �ȹ�ϣֵ;
*       ���ز���װ McCoy ����(https://developer.mozilla.org/en-US/docs/McCoy) -- ǩ����GUI���򣬶�Ӧ Uhura �������еĹ��ܣ�
*     <em:updateURL> -- update.rdf �ļ���Web�������ϵĵ�ַ ? �ǿͻ��˷�ʲô?
*     <em:updateKey> -- 
*
*   [chrome] -- Chromeָ����Ӧ�ó��򴰿ڵ���������֮����û�����Ԫ�صļ��ϣ��������������˵����������ʹ��ڵı������ȡ�
*       contents.rdf -- (�����ƺ�����Ҫ����ļ��ˣ�ÿ����Ŀ¼�¶���һ��������ͳһ�� chrome.manifest ��)�������Ŀ¼����Ϣ�嵥��Mozilla����ȡ��ʹ����������ע�����
*       �������������һ��chrome URL�����û�и��ļ����Ͳ���ͨ�� chrome:// �����ʣ�ʵ����Է���
*     ���ֻ�����chrome�ṩ��
*     [content] -- ��չ����������� XUL �ļ���������Ϊ�� JavaScript �ļ���
*       �磺content	myExtension	chrome/content/ �� content	myExtension	jar:chrome/myExtension.jar!/content/
*       �������ļ��������������ͬ(��editor��������һ������editor.xul���ļ�)���ϵİ汾������������ƣ������ƺ��Ѿ�����Ҫ�ˣ�
*     [locale] -- ��ŵ��Ǳ��ػ���ص��ļ�, ��  en-US��zh-CN��ko-KR��vi-VN ��Ŀ¼�������� dtd��properties ���ļ�
*        ע�⣺1.���뱣��Ϊ"without signature"��UTF-8��ʽ��2. ����properties���������FF������Ч(��û�������� ?)
*     [skin] -- ��� CSS��PNG����Դ�ļ�������������չ����ۣ����԰��������֧�ֲ�ͬ������?
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
* ������(�� about:config �������Լ�����������������Ŀ¼����ʽ)
*   ʹ�� pref("����", ֵ) �ķ�ʽ�������Լ�Ŀ¼�µ� defaults\preferences\xxx.js �ļ���
* 
*
* ���ǩ���������ٷ��ĵ�(�����ȽϾ�) ��https://developer.mozilla.org/en/Signing_a_XPI
*   1.mccoy ???(Я�̵Ĺ���?) -> �����ѹ���ļ�����չ����Ϊ xpi;
*   2.sha1sum.exe xxx.xpi �õ�У���룬copy�� update.rdf;
*   3.sign �� verify (mccopy �ϵİ�ť��ʵ������ô��?)
*   
*   ǩ�����˵��( META-INF Ŀ¼��)
*     manifest.mf -- ����XPI��ÿһ���ļ��� MD5/SHA1 ��֤��
*     zigbert.sf --  ����XPI��ÿһ���ļ��� MD5/SHA1 + XPI���� ��֤��(ֵ�� manifest.mf �еĲ�һ��)
*     zigbert.rsa -- ǩ����Ϣ(�繫˾����ַ�ȣ����Ƕ������ļ�����ʲô�鿴?
*
*   NSS(Network Security Services) -- ���أ�https://ftp.mozilla.org/pub/mozilla.org/security/nss/releases/NSS_3_12_4_RTM/msvc9/WINNT5.1_OPT.OBJ/nss-3.12.4.zip)
*   NSPR(Netscape Portable Runtime) -- ʹ�� NSS 3.12.4 ���ɲ���������ߣ�
*     ����߽���( -H ��ʾ��ϸ������ע�⣺MS�Լ��� certutil.exe�ȹ��ߣ������Ҫ���� %PATH% �������� )
*       certutil.exe -- ֤��������ߣ����� �½�֤�����ݿ�(-N), ����֤��(-A), �г�֤��(-L)
*         -L �鿴֤��ʱ�� "Trust Attributes -- SSL,S/MIME,JAR/XPI"
*            u, u, Cu -- ?
*       pk12util.exe -- ��Կ�������ߣ�
*         -o "֤���ļ���" -n "֤���ǳ�" -d Ŀ¼
*       pvk2pfx.exe -- .pvk + .spc(˫֤���ļ�) => .pfx() �� �� mycredentials.spc + myprivatekey.pvk => cert.pfx
*       signtool.exe(nss�汾) -- ǩ�����ߣ� ����ʹ��ָ����֤����ǩ���ض����ļ���Ŀ¼
*         -l -d . <== �鿴��ǰĿ¼���Ѿ������֤����Ϣ
*
*     ���裺
*       ˵������Ȼ PFX ��ʽ֤���б����Ѿ�����֤��䷢�ߵĸ�֤����м���֤�飬������NSSǩ������ Signtool ��֧��֤����(Bug),
*             ���Ա���Ҳ�����֤���м�֤��(���� 2 ��)���������ڹ���Ŀ¼��(һ���������Ϊ rootCA.cer? )
*       1.�ڵ�ǰĿ¼����nss֤�����ݿ⣬��Ҫ����"֤��洢���������(NSS Certificate DB)"��Ȼ������� cert8.db/key3.db/secmod.db �����ļ�
*         certutil -N -d .
*       1.5.���ɲ����õ���ǩ�� pfx ֤��(һ����ҵ������Ӧ�����Ѿ����ɵ�)
*         ��������ʲô�ļ��� -- mycredentials.spc, myprivatekey.pvk, cert.cert, cert.pfx, cert2.cert,cert.pem, cert.p12
*         signtool -G myTestCert -d . -p"֤��洢���������"  <== ������ "x509.raw" and "x509.cacert"��
*           ��Ҫ�ṩ"certificate common name", "organization","organization unit","state or province","country","username","email address" ��֤����Ϣ.
*           ���ɺ��ͨ�� certutil -d . -L �鿴, ����Ϊ "u,u,Cu"
*       1.6 �������õ�֤���������
*           x509.cacert ����Ϊ x509.cert => FF14�У�����->ѡ��->�߼�->����->�鿴֤��->֤�����-->����->�༭����->�༭CA����(���)
*       2.����֤��( .cer/.pfx -- .pfx���? )��XPI�ļ�ǩ��ʱʹ�õ�֤��(������Կ)���뵽NSS���ݿ���
*         certutil -A -n "Intermediate CA Certificate" -t "TC,TC,TC" -d . -i "cert.cert"
*         certutil -A -n "Code Signing Certificate" -t "c,c,C" -d . -i "cert2.cert"
*           -A ����֤��
*             -n "֤����ǳ�"
*             -t "���β���"
*             -i "֤���ļ�"
*           -L �г�֤�飬���Բ鿴������
*       3.�������ǩ��֤�� [TODO:������Կ?]( .pfx �� pkcs12 ���ļ�)����Ҫ����"֤��洢���������" + "����PFX֤��ʱ���õ�����"
*         pk12util -i "XPICA.pfx" -d .
*       4.����֤��ʹ���ǩ��֤���Ƿ�ɹ�����֤��洢����(��ǩ��ʹ�õ�֤�����Ƿ�����)
*         certutil -L -d .  <== ��ʾ  "u,u,u"
*       5.ʹ��֤����ǩ��ָ��Ŀ¼�µ��ļ�������ָ��Ŀ¼������ META-INF ����Ӧ�� zigbert.sf ���ļ�)
*         signtool -d . -k "ǩ��֤����ǳ�" -p "�������ݿ�ʱ������"  directory   //-X -Z "file.xpi"
*       6.ѹ��������Ϊ zip ������������չ��Ϊ xpi��ѹ�����е�һ���ļ������Ǻ���ǩ����Ϣ��zigbert.rsa�������Ҫ������
*         zip  ..\xxxxx.xpi META-INF\zigbert.rsa				<== �Ȱ� zigbert.rsa ѹ����xpi��
*         zip -r -D ..\xxxxx.xpi * -x META-INF\zigbert.rsa		<== Ȼ��ѳ� zigbert.rsa ��������ļ�ѹ����xpi��
*       7.����֤�鵼�뵽 Firefox ��֤��ϵͳ�к󣬽� xpi �ϵ� Firefox��װʱ������"δ��֤"����Ϊ"����֤"
*       
* ֤�飺
*   ����:  
********************************************************************************************************/


