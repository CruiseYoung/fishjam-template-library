#ifndef NPAPI_STUDY_H
#define NPAPI_STUDY_H

//http://andylin02.iteye.com/blog/616098 -- Chrome�Ķ��߳�ģ��
// ?? Plugin��̬���ӿ����ֱ���ΪxxxWrapper.dll������webkit�����޷�ʶ��
//     Plugin��̬���ӿ��������������widget��������ͬһĿ¼��
// ?? Plugin��� nsPluginInstanceBase �̳�? ���� "pluginbase.h"�ļ�
// FF�в鿴�����about:pplugins
/*******************************************************************************************************
* http://mozilla.com.cn/post/21666/
* https://code.google.com/p/npapi-sdk/
* https://developer.mozilla.org/en-US/docs/Gecko_Plugin_API_Reference -- NPAPI
* 
* NPAPI(Netscape Plugin Application Programming Interface) -- �� Netscape Navigator��Firefox��Google Chrome��
*   Apple Safari��Opera ��Gecko�����������ʹ�õĲ���ӿڡ����Կ�����������Ĳ�������������������⡣
*   ����ɵ� XPCOM �� LiveConnect-based API����Ŀ����Ҫ�� np ��ͷ��Ϊ����Ӧ��ͬ�Ĳ���ϵͳ������ȫСд�����ȿ�����8�ַ��ڡ�
*   �����Ҫ������ָ����Ŀ¼�£���ÿ��������Դ���һ�ֻ����MIME��ʽ�����ݡ�
*   �����װ���Ժ󣬿���Ƕ��ʽ��ȫ��Ļ����ʽ���ֿ��ܵ���ʽ���֡�
*
* NPAPI�еĺ�������
*   NPN_XXX(Netscape Side API) -- ������ṩ��plugin�ĺ�����һ�㶼��NPNetscapeFuncs�ṹ����
*     NPN_InvokeDefault��NPN_CreateObject�� NPN_RetainObject��NPN_ReleaseObject 
*     NPN_UTF8FromIdentifier��NPN_GetStringIdentifier��NPN_GetIntIdentifier��NPN_Status
*     NPN_GetURL -- ����ָ��URL�����Խ����µ�������? ����ͨ�� "javascript:������()" �ķ�ʽ����JS�ĺ���
*     NPN_GetURLNotify / NPN_PostURL/NPN_PostURLNotify
*     NPN_GetProperty/NPN_SetProperty -- ��ȡ������ָ�������ָ������
*     NPN_GetValue/NPN_SetValue/NPN_InvalidateRect
*     NPN_PluginThreadAsyncCall -- �̰߳�ȫ��֪ͨ��������ò�������ķ�ʽ��ͨ�����ڶ��̻߳����£���Ҫ��ʼ�� NPNFuncs.pluginthreadasynccall ��
*   NPP_XXX(Plug-in Side API) -- plugin�ṩ��browser�ĺ�������Ҫ���ڲ�����һЩʵ�ʹ��ܣ�ʵ�ֺ����NPPluginFuncs���ṹ����(��browser�ص�)
*     NPP_NEW/NPP_Destroy -- ���Plugin����ĺ���
*     NPP_SetWindow -- ���ô��ھ�����������λ��
*     NPP_HandleEvent -- ����ƽ̨��ص��¼�, ���������ת��Ϊ NPEvent(��Windowsƽ̨�о�����Ϣ)��
*       TODO: NP_Initialize �� NPNetscapeFuncs->handleevent Ϊ NULL ? ��ô����?
*     NPP_URLNotify 
*     NPP_GetValue/NPP_SetValue -- ��plug-in���غ����������JS���������
*     NPP_NewStream/NPP_DestroyStream -- ����������һ�� NPStream ��ʹ��ʵ����plugin���������������ʱ����Ҫ�������ӿ�
*     NPP_WriteReady/NPP_Write -- ����Ҫ�������ݸ�plugin��ʱ����Ҫ�������ӿڣ��ֱ�Ϊ��ÿɴ������ݴ�С�ʹ������ݵĽӿڡ�
*   NP_XXX() -- ����ṩ���������Ĭ�ϵ��õķ�������Ҫ����ʶ�𡢳�ʼ������ֹ������ȣ���Ҫ��def�е���
*     NP_Initialize��NP_Shutdown �� -- ? npapi�Ĳ�����ṩ������������ϲ�ӿڣ����Plugin���͵ĺ���
*   NS_XXX() -- 
*
*
* ��дNPAPI���(��̬���ӿ�)
*   a.Ԥ����ı���꣺XP_WIN, _X86_
*   b.��Ҫ��def�ļ��е������º���(�������ǿ���ṩ)��
*     0.NP_GetMIMEDescription(������Ч?) -- browserͨ������ֵʶ��plugin�� ����ֵΪ "mime����:��չ��:����[; �����mime���� ]" ��ʽ���ַ�����
*       ��Ҫ���汾��Ϣ(.rc)StringFileInfo�е� ��BLOCK �� XXX ��Ϊ 040904E4(��������ΪӢ��,�ַ���Ϊ "Windows Multilingual", �����Ż������Դ��Ϣ),
*        ���Ļ�����������(����ʾ�� "about:plugins" ��)
*       "FileOpenName" -- MIME���͵���������(File Opent Template)���� "Fishjam Template Log(*.ftl)" (���ʱͨ��"|"�ָ�)
*       "FileExtents" -- MIME���͵ĺ�׺����(���ʱͨ�� "|" �ָ�)
*       "FileDescription" -- �ļ���������, ��"�汾"�·�
*       "FILEVERSION" -- �汾
*       "ProductName" -- ��Ʒ���Ʋ���(������Ĵ�ź�����)
*       "MIMEType" -- "MIME ����" ���֣��� "application/x-yyy"(ע�⣺����ȫСд, ���ʱͨ��"|"�ָ�)�� ��Ҫ�� NP_GetMIMEDescription ����ֵһ��
*       "Language(δ����)" -- ʹ�õ�����
*     1.NP_GetEntryPoints
*     2.NP_Initialize
*     3.NP_Shutdown
*     4.[NP_GetValue(������Ч?)], -- ���ز����һЩֵ(�� pluginName ��plugin Description ��)
*       ����һЩ�涨�ĺ���(?, NP_XXX, NPP_XXX )
*   c.ע�᣺HKLM\Software\MozillaPlugins\�������ֵ���(?)���ַ�������"Path"
*     TODO������ÿ���˵����
*
* �鿴Plugin�Ƿ���سɹ�: FF(about:plugins), 
* Firefox �� plugin ������ plugin-container.exe �����У�attach ���ý�����
* Google �Ƕ����������������˶ϵ㰤�����飿

*******************************************************************************************************/

/*******************************************************************************************************
* ���ͣ�
*   ScriptablePluginObject ?
*   NPClass -- һ���ṹ�壬���涨���˺ܶ���Ϊ NPXXXFunctionPtr �ĺ���ָ��(�� Construct��HasMethod��Invoke��SetProperty��)
*     һ��C��ʽʵ�ֵĲ���ӿڹ淶
*   NPError -- ����Ĵ����룬NPERR_NO_ERROR
*   NPIdentifier -- ���Ի򷽷����ȣ�һ��ͨ�� NPN_UTF8FromIdentifier �����õ� UTF8 ��ʽ�� string
*   NPMIMEType --
*   NPP m_pNPInstance; //�൱�ڲ����ʵ����������ڲ��� pdata(CPlugin*) �� ndata() ������ָ�룬���Էֱ�ת��Ϊ����������
*   NPStream -- ������Ͳ��ͨ����(������һ��URL�������ݵĶ���)�������˴˵����ݡ�
*     ���������(���������NPP_NewStream)���������¼���ģʽ
*       1.NP_NORMAL  -- �������ݿɷ���ʱ������Ͱ����ݷ��͸�plug-in����Щ���ݿ������Է�����˳�򵽴�ġ�
*         �����ͨ�� NPP_WriteReady �� NPP_Write �ȷ������ݣ�Ч�ʽϸߡ�
*       2.NP_SEEK -- �漴��ȡģʽ�� seekable Ϊ true ʱ�����е��������ɲ������ NPN_RequestRead ����ָ����Ҫ��ȡ�����ݵķ�Χ��
*         Ȼ�������ͨ�� NPP_WriteReady �� NPP_Write �ȷ������ݣ���ҪԶ�̷�������֧�ֻ�������Ƚ������ݴ浽���ص���ʱ�ļ��С�
*       3.NP_ASFILE -- ������Ƚ�����Url���ݴ浽һ�������ļ��У�Ȼ��ͨ�� NPP_StreamAsFile ���ļ����������
*     �������(�������NPN_NewStream)����ֻ��һ�ָ�ʽ
*       1.NP_NORMAL -- �����������������ͨ�����������෴���ڴ�����ʱ����ָ�������ݵ�Ŀ�ĵأ�Webҳ���е�һ��frame��һ���µ���������ڵ�)��
*         ͨ��Ŀ�ĵص�ָ����������Դﵽ����������޷����ӡ�
*   NPVariant -- һ��ͨ�� NPN_ReleaseVariantValue �Ⱥ�������
*   NPWindow -- ƽ̨��صĴ�����Ϣ(�������С��λ�õ�)
* 
* Npruntime �ӿ� -- JavaScript�ű��ӿ�?
*   Browser =>   => Plugin
*   Browser <=   <= Plugin
*
* 
*
*
* ʹ�÷�ʽ(HTML��)
*   ��  <object id="myPluginID" type="application/x-yyy" > <param name="xxx" value="yyy"/></object>
*   ���� <embed id="myPluginID" type="application/x-yyy" color="FFFF0000"></embed>
*   Ȼ����JS�м���ͨ�� "myPluginID.xxx" ���ʣ��ڲ����̣�
*   1.NPP_GetValue �õ� NPClass;
*   2.hasMethod/hasProperty ȷ���Ƿ��ж�Ӧ�ĺ���/����;
*   3.Invoke/GetProperty ���д���;
*   4.����ڲ���ʵ��;
*  
*   ��̬������
*     var newNode = document.createElement("embed");
*     newNode.setAttribute("type","application/x-yyy");
*     newNode.setAttribute("id", xxx"); //width, height
*     
* �����ص�(myPluginID.somecallback = func): ͨ�� SetProperty(�ص�ָ��) ���ã�����ڲ�ͨ�� 
*   NPN_DefaultInvoke(�ص�ָ��) �����ص�.�ص������� NPObject* ���͵ı���(m_playOver), 
*   ����:NPN_InvokeDefault(0, m_playOver, 0, 0, &result); 
*        NPN_ReleaseVariantValue(&result); 
*   ����ʱ��Ҫ :if(m_playOver){ NPN_ReleaseObject(m_playOver); }
*
* NPAPI Scriptable Plugin(JavaScript ���Խ� Plugin ���� JavaScript Object ��ʹ��) -- 
*   ��Ҫ���plugin�ĺ���������(static NPIdentifier ���͵�?ͨ�� NPN_GetStringIdentifier("����") ��ʼ��?)
*   
*   ��Ҫ֧������ö��(traceObj �� DOM Inspector��), ��Ҫ֧�����·���������
*     __iterator__
*     QueryInterface
*   �������Ի򷽷�(��û�л������ṩ?)
*     nodeName
*     nodeValue
*     nodeType
*     parentNode
*     parentElement
*     childNodes
*     firstChild
*     lastChild
*     previousSibling
*     nextSibling
*     attributes
*     ownerDocument
*     insertBefore()
*     replaceChild()
*     removeChild()
*     appendChild()
*     hasChildNodes()
*     ��
*******************************************************************************************************/

#endif //NPAPI_STUDY_H