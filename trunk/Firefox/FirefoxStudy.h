#pragma once

#ifndef _DEBUG
���� -- AptanaStudio

:: ������
:: ע����Ϣ

%������:~��ʼ,����%  <== ��ָ�������л�ȡ�����ַ������� set _VAR=%~dp0 <CR> set _SIGN=%_VAR:~0,-1% �� Ϊ����"\"��׺��·��

set WORK_PATH=%~dp0 -- ��ǰĿ¼
set YEAR=%date:~0,4%
set MONTH=%date:~5,2%
set DAY=%date:~8,2%
set DATE=%YEAR%%MONTH%%DAY%
set XPI_NAME=win_%DATE%.xpi
rd /s /q Ŀ¼ <== �ݹ鰲����ɾ��Ŀ¼������Ŀ¼
7z.exe x xulrunner-xxxx.zip -o���Ŀ¼ -y <== ʹ��7z��ѹ xulrunner

������(���û�д���, ��ֵΪ 0)
if not "%ERRORLEVEL%" == "0" goto ERROR
goto OK
:ERROR
@exit /b 1
:OK

"%VS90COMNTOOLS%/../IDE/devenv.exe" xxx.sln /rebuild Release

set CURDIR=%cd% <== ���浱ǰ��Ŀ¼, ����Ŀ¼�󣬿���ͨ�� cd %CURDIR% ����ԭ����Ŀ¼

7z a -tzip %XPI_NAME% ./temp -r -mx=9 -xr@excludelist.txt  <== ѹ������ excludelist.txt ÿ��һ�����ˣ�֧��ͨ���(�� build.xml<CR>*.svn<CR>*.pdb ��)
unzip %XPI_NAME%

chcp 936 <== ���� codepage

sign\tools\signtool.exe(��dll��exeǩ��) sign /v /d "VietnamToolbar" /du http:/��վ/ /f "cert.pfx" /a /t http://timestamp.globalsign.com/scripts/timstamp.dll /p ���� signed\libraries\*.dll
����ʾ�������µ���Ϣ��
The following certificate was selected:
Issued to: Some Company Co.,Ltd.
Issued by: VeriSign Class 3 Code Signing 2010 CA
Expires:   2020/04/20 7:59:59
		   SHA1 hash: XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

nss-3.9\bin\signtool.exe -d ..\cert -k "ǩ��֤����ǳ�" -p "����" xpi��Ŀ¼  <== ��xpiǩ������,���� META-INFO 

ע�⣺ѹ�� xpi ʱҪ����XPI��Ŀ¼�����ѹ��������Ŀ¼�ṹ
zip  ..\xxxxx.xpi META-INF\zigbert.rsa				<== �Ȱ� zigbert.rsa ѹ����xpi��
zip -r -D ..\xxxxx.xpi * -x META-INF\zigbert.rsa	<== Ȼ��ѳ� zigbert.rsa ��������ļ�ѹ����xpi��


setlocal EnableDelayedExpansion  <== ���ñ���Ϊ�ӳ���չ����ʵҲ���ǣ��ӳٱ�����ȫ���ӳٻ���������չ, 

for /f "delims=" %%i in ('dir  /a-d/b *.xpi') do ( xxx )

if {%SIGN_PFX%} == {} goto NO_SIGN_PFX <== �жϱ����Ƿ�������ֵ
if exist "%_SIGN%\%_SIGN_PFX%" goto CHECK_SIGN_INFO   <== �ж��Ƿ����ָ���ļ�

call maxtools\uhura.bat -o update.rdf -k cert.pem -p =���� XXX.xpi http://xxx/xxx.xpi YYY.xpi http://xxx/yyy.xpi  <== ʲô��˼

#endif

//����Ƭ�Σ�https://developer.mozilla.org/en-US/docs/Code_snippets
//Helloworld: http://kb.mozillazine.org/Getting_started_with_extension_development (���� "download")
//firefox extension tutorial
//�ɰ汾��FF���أ�ftp://ftp.mozilla.org/pub/firefox/releases/, ��װ�ú���Ҫ ����->ѡ��->����->��������, ����鿴Aboutʱ���Զ�����

//��չ�����������ģ���չ���������������
//��������ҳ��ģ���ҳ�������������ݸ�ʽʱ������

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
*         ע�⣺DTD��peoperties �ļ���Ҫ����ΪUTF8��ʽ(VS��ʾΪ Unicode(UTF-8 without signature) - Codepage 65001)��������ܳ�������
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

