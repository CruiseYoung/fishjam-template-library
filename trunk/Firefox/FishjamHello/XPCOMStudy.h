#pragma once

/*******************************************************************************************
* https://developer.mozilla.org/en-US/docs/How_to_build_a_binary_XPCOM_component_using_Visual_Studio
*
* XPCOM ����ͨ�� C/C++/JavaScript ������Ҳ������ C/C++/JavaScript ��ʹ��
*   https://developer.mozilla.org/en-US/docs/How_to_Build_an_XPCOM_Component_in_Javascript
*   https://developer.mozilla.org/en-US/docs/How_to_build_a_binary_XPCOM_component_using_Visual_Studio
* 
* TODO:
*   1.XPCOM �� ��������� ?
*   2.nsIServiceManager -- 
*   3.XPConnect -- Javascript ��XPConnect��֧���£�JavascriptҲ������������COM���������ʵ���κι���
*     ��չͨ��XPCOM��XPConnect API�������ʵ�ֽ���
*
* XPCOM(Cross Platform Component Object Module) -- ��ƽ̨�������ģ�ͣ���MS��COMʵ��
*   Mozilla�����Ŀ�ƽ̨����ṹ��ԭ��������΢���Com����������Եģ��ṩ��Ҫ���߼�����
*   XPCOM ����ֱ��֧�� C++ ���쳣����. �� XPCOM ��, ���е��쳣����������ڲ�����, �����ܿ�Խ�ӿڵı߽磬���� nsresult(���� HRESULT)
*   ��ĿǰMozilla �Ѳ���֧��XPCom
* 
* MXR(Mozilla Cross-Reference) -- 
*******************************************************************************************/

/*******************************************************************************************
* ��ȫ��
*   1.���ڰ�ȫ�Ե�ԭ�򣬺ܶ���Ҫ��Ȩ��XPCOM���������Ȩ��XUL�ļ��϶�����ʹ�á�Ψ��ʹ������ǩ��Ľű������д�����Ȩ -- ǩ����ô����
*   2.���ܼ���Զ�˵�XUL��DTD��RDF�ļ���һ����ͨ�� chrome:// �ķ�ʽʹ�ð�װ����ϵͳ�е�
*   Ȩ�ޣ����Զ����(autocomplete) ��Ҫ XPConnect Ȩ�� 
*   Ϊ��ʹ�� XPConnect���ļ���Ҫ�����UniversalXPConnectȨ��(��ͨ����ҳ�Լ������ļ�û�и�Ȩ��)
*     ����Ȩ��(�������Ҫ)��netscape.security.PrivilegeManager.enablePrivilege('UniversalXPConnect');
*
* �ӿ�(�ӿ�����ͨ��Ϊ nsIxxx ����ʽ)
*   nsIDOMWindowInternal -- �����Ĵ���? nsIWindowMediator.getEnumerator ���صĶ�������?(����QI)
*   nsIFile -- �������File�ӿ�
*     initWithPath("�ļ�·��URL") <== ʹ���ļ�·����ʼ������֮�����ʹ��
*   nsIFileInputStream -- �����ļ���
*     init <== ����nsIFile���󣬲������ https://developer.mozilla.org/en-US/docs/PR_Open#Parameters
*   nsILocalFile -- �������ļ���XPCOM�мȿ���ʹ��Զ����ԴҲ����ʹ�ñ����ļ�����������Ҫʹ��URI��Ϊ·��(�����ļ���Ҫת��Ϊ file:///C/temp/temp.txt �ĸ�ʽ)
*   nsISupports -- �����ӿڣ����� IUnknown, ��Ҫ #include "nsISupports.idl"
*   nsISimpleEnumerator -- ö����, �÷��� while(enum.hasMoreElements()) { var obj = getNext().QueryInterface(Ci.nsIxxxx); obj.someMethod(); }
*   nsIPrefBranch -- ��дFF����ѡ��(about:config)
*   nsIPrefService -- FF����ѡ� getBoolPref() �� getIntPref �ȷ���
*   nsIProperties -- �������ֻ�ȡXPCOM���󣬱��� propDir = get("ProfD", Components.interfaces.nsILocalFile); ��ȡ Profiles ��Ŀ¼
*     ��û����ö������ "����-����" �Եķ���?
*   nsIWebNavigation -- ���¶�������͡� document.getElementById('browser').webNavigation;
*   nsIWindowMediator -- ���Է���FF���д���(�˷�JSֻ�ܻ�ȡ�븸������صĴ��ڵ�����)
*     getMostRecentWindow('navigator:browser');  -- ��ȡ���������ڵ�ö����
*     getEnumerator('navigator:browser') -- ��ȡ���о����ض����͵Ĵ���(������������Ϊnull���ȡ�����Ի������ڵ����д���)��Ȼ�����ö��
* 
* �ַ�����ת��(�ڲ��ı����붼��Unicode)
*   0.ȷ�����ݵı��뷽ʽ������޷���ȡ�����Բ���ȱʡ�� "intl.charset.default"
*     a.nsIChannel -- �����п��Ի�ȡ contentCharset ����;
*     b.�ļ� -- ͨ��ʹ��ϵͳȱʡ�ı����"preferences"�е�ȱʡ����
*   1.ʹ�� nsIConverterInputStream ��ָ��������ı��ļ��ж���
*     var charset = 'Shift_JIS';
*     var converterStream = Cc['@mozilla.org/intl/converter-input-stream;1'].createInstance(Ci.nsIConverterInputStream);
*     converterStream.init(fileInputStream, charset, fileInputStream.available(), converterStream.DEFAULT_REPLACEMENT_CHARACTER);
*     var out = {};
*     var numChars = converterStream.readString(fileInputStream.available(), out);
*     if(numChars != 0) //EOF
*     { alert(out.value); }
*   2.ʹ�� nsIUnicharLineInputStream ֱ�Ӷ���Unicode������� ? 
*   3.ʹ�� nsIScriptableUnicodeConverter ���ڴ��еĸ��ֱ���֮��ת��
*     var converter = Cc["@mozilla.org/intl/scriptableunicodeconverter"].createInstance(Ci.nsIScriptableUnicodeConverter);
*     converter.charset = "UTF-8";  //�������ı����ʽ
*     var unicode_str = converter.ConvertToUnicode("SomeUTF8String");    
*     var utf8_str = converter.ConvertFromUnicode("SomeUnicodeString");
*   4.ʹ�� nsIUTF8ConverterService
*   5.ʹ�� JS ���� ( ���� ·�� )��
*     decodeURIComponent(escape("UTF-8String"))    <== �� UTF-8  ת��Ϊ UTF-16
*     unescape(encodeURIComponent("UTF-16String")) <== �� UTF-16 ת��Ϊ UTF-8
* 
* �ļ�IO��ע�⣺ʹ����Ϻ���Ҫ���� close �رգ�
*   ��Ϊ����ģʽ��
*   1.�����Ƶ���ģʽ
*     ���룺nsILocalFile + nsIFileInputStream(init) + nsIBinaryInputStream(setInputStream+readByteArray)
*     �����nsILocalFile(create) + nsIFileOutputStream(init) + nsIBinaryOutputStream(setOutputStream+writeByteArray)
*   2.�ı��ļ����ַ�ģʽ(�ڲ�ʹ��UTF-16���룬�ڶ�дʱ��Ҫ�����ļ��ı�����ַ�����ת��)
*     ���룺nsILocalFile + nsIFileInputStream(init) + nsIConverterInputStream(init+readString)
*     �����nsILocalFile(create) + nsIFileOutputStream(init) + nsIConverterOutputStream(init+writeString)
*           ע�⣺nsIConverterOutputStream::writeString �Ĳ�����Unicode������ַ���(��������UNICODEת�壬�Ƿ���ֱ�����ַ���?)
*
* �﷨
*   ��ԼID(contractID,����ProgID?) -- ���ڷ�������Ŀɶ�(to humen)��Ψһ��ʶ�ַ�����
*     ��ʽΪ"@domain_name/module_name/component_name;version_number"��
*     refp = do_CreateInstance( "@mozilla.org/accessibleRetrieval;1", &rv);
*   ����ָ�룺nsCOMPtr��nsAutoPtr��nsAutoArrayPtr��nsRefPtr
*   ����(��һʵ��)��nsCOMPtr<nsIPrefService> prefService = do_GetService (NS_PREFSERVICE_CONTRACTID);
*     ϵͳ�ṩ�ķ���
*     a.��ƽ̨�ļ������� XPCOM ������Ա�ṩͬһ��ǿ����ļ����ʹ���.
*     b.Ŀ¼����, �ṩӦ�ú��ض�ϵͳ��λ��Ϣ.
*     c.�ڴ����, ��֤���ж���ʹ��ͬ�����ڴ������. 
*     d.�¼�֪ͨ����, �����ݼ���Ϣ.
*
* ���÷�ʽ
*   ��ͨ���(ͨ�� createInstance()����ʵ��) <== 
*   ϵͳ����(ͨ�� getService() ���Ψһʵ��) <== var myObj = Components.classes["��ԼID"].getService(�ӿ�����);
*     ˵�����ڶ�Ӧ�� idl �ļ��У���ԼIDͨ��Ϊ NS_XXXXX_CONTRACTID
*   �Զ������
*      1.var myObj = Components.classes["@fishjam.com/helloworld-http-request;1"].createInstance(Components.interfaces.nsIHelloHttpRequest);
*      2.var myObj = Components.classes["@fishjam.com/helloworld-http-request;1"].createInstance(); myObj = myObj.QueryInterface(Components.interfaces.nsIHelloHttpRequest);
* ����ı�д
*   1.����ӿ��ļ�(idl?)
*   2.����ӿ��ļ�Ϊ XPT ��ʽ
*   3.ʵ�ֽӿ�
*   4.�������Ĺ���������ע��ģ�� -- ʵ�������ע�ἰ������صĹ���
*   5.VS����Ҫ���� XP_WIN��XP_WIN32 �ꣻ���� nspr4.lib xpcom.lib mozalloc.lib xpcomglue_s_nomozalloc.lib �ȿ�
*       xpcomglue_s.lib ?
*   6.�� chrome.manifest ��ע�����(interfaces��binary-component)
*******************************************************************************************/

/*******************************************************************************************
* IDL(���� idl �Ĺ����� xpidl.exe�����ɵ�ͷ�ļ��ж����� NS_IXXX_IID_STR �� NS_IXXX_IID �꣬���ɵĽӿ�һ��Ϊ)
*  -I Ŀ¼    <== ����ϵͳ�������idl��Ŀ¼���� -I %XPSDK_DIR%idl
*  -m typelib <== ������ .xpt Ϊ��׺�����Ϳ⣬���Ϳ����ͨ��XPConnect������JavaScript�������ṩ������ʵ�Wrapper
*  -m header  <== ����ͷ�ļ�
*  -w         <== �򿪾���
*  -v         <== verbose ģʽ
*  -e ����ļ��� <== ָ��������ļ���(�� XXX.xpt)
*
* Regxpcom <== ���ע�Ṥ�ߣ�
* xpt.py dump xxx.xpt <== �鿴���Ϳ����ϸ��Ϣ
*
* Python(?) -- 
*  �� idl ����ͷ�ļ� -- header.py -I  yourGecokSDKfolder \idl -o yourheader.h youridl.idl".
*  �� idl ���� typelib(.xpt) -- typelib.py -I  yourGecokSDKfolder \idl -o yourtypelib.xpt youridl.idl
*******************************************************************************************/

/*******************************************************************************************
* Gecko SDK(XulRunner SDK) -- https://developer.mozilla.org/en/Gecko_SDK
*     https://developer.mozilla.org/en/XPCOM/XPCOM_changes_in_Gecko_2.0
*     https://developer.mozilla.org/en/How_to_build_a_binary_XPCOM_component_using_Visual_Studio
*   һ����Դ��, ��ѭ��׼��, Ƕ��ʽ web ������� ���߰�
*   ��ѹ��Ҫʹ����������: 7z.exe x  XXX.win32.sdk.zip -oXXX.win32.sdk -y
*
*
*******************************************************************************************/


/*******************************************************************************************
* Ȼ����ʵ���ļ��� -- ���߻��Զ����� ������ģ�鶨�� ?
*******************************************************************************************/
