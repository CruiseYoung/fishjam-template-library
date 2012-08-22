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
*******************************************************************************************/

/*******************************************************************************************
* �ӿ�
*   nsISupports -- �����ӿڣ����� IUnknown, ��Ҫ #include "nsISupports.idl"
*   
* �﷨
*   ��ԼID(����ProgID?) -- ���ڷ�������Ŀɶ�(to humen)���ַ�������"@"��ͷ?
*     refp = do_CreateInstance( "@mozilla.org/accessibleRetrieval;1", &rv);
*   ����ָ�룺nsCOMPtr��nsAutoPtr��nsAutoArrayPtr��nsRefPtr
*   ����nsCOMPtr<nsIPrefService> prefService = do_GetService (NS_PREFSERVICE_CONTRACTID);
*     ϵͳ�ṩ�ķ���
*     a.��ƽ̨�ļ������� XPCOM ������Ա�ṩͬһ��ǿ����ļ����ʹ���.
*     b.Ŀ¼����, �ṩӦ�ú��ض�ϵͳ��λ��Ϣ.
*     c.�ڴ����, ��֤���ж���ʹ��ͬ�����ڴ������. 
*     d.�¼�֪ͨ����, �����ݼ���Ϣ.
*
* ���÷�ʽ������ͨ�� XPCOM Viewer ��չ�� "Generate Code"�������ɵ��÷���Ĵ���)
*   ϵͳ���� <== ʹ�÷���: var myObj = Components.classes["��ԼID"].getService(�ӿ�����);
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
