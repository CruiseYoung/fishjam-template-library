#pragma once
/**************************************************************************************************************
* ������ͣ�
*   �ٶ�: tn=xxxx
*   
**************************************************************************************************************/

/************************************************************************************************
* ��������
*   Internet Explorer Developer Toolbar(http://www.microsoft.com/en-us/download/details.aspx?id=18359)
*     ΢���Ƴ��Ŀ��������������� ������޸�Webҳ���ĵ�����ģ�� �ȡ�
*     ��װ���Ժ�Tools-> Developer Tool����ݼ��� F12
*   
* 
* mshtml.h(IE ���棬Trident)
*
*
* IE������ṩ�˿ɱ�̵Ķ���ӿڣ����������Զ���Ĺ��ܵĽ���ӿ� 
*   ʹ�� RBDeskBand Wizard���Լ��ٿ��� -- http://www.codeproject.com/KB/atl/rbdeskband.aspx
*
* ��������չ����ӿڣ�
*   1.��Ӷ��Ʋ˵��IE�����Ĳ˵� -- �� ����ӵ�QQ���顱
*     ���ע����� -- HKCU\Software\Microsoft\Internet Explorer\MenuExt\ 
*       �����һ���Ӽ�(�Ӽ�������ʾ�������Ĳ˵��ϵ�����)���Ӽ���Ĭ��ֵΪһ����ҳ�ļ���·����
*       ���а����˲˵��������ʱ��ִ�в����Ľű����򣬿������� External.menuArguments �յ���ǰIE����״̬�Ĳ���
*       �� External.menuArguments.document.links 
*       ���Ҫ���ƶ��Ʋ˵��������ҳ�ϵ���ʾ��Χ�������ڸ��Ӽ������һ��Context������ֵ(�߼���):
*         Ĭ��(0x1);ͼ��(0x2);�ؼ�(0x4);��(0x8);��ѡ����(0x10);ê��(0x20)
*   2.��Ӷ��Ʋ˵��<����>�˵��£��� NetAnts, FalshGet��MSN Message Service 
*     a.����Ψһ��GUID��������Ϊ���Ƽ��뵽 HKLM\SOFTWARE\Microsoft\Internet Explorer\Extensions\ ��,
*     b.�ڸ��Ӽ��´���һ����Ϊ CLSID ���ַ���ֵ��{1FBA04EE-3024-11D2-8F1F-0000F87ABD16}��-- IE�����������ʶ��
*     c.MenuText--�˵�����ʾ�ִ���MenuStatusBar--�˵�����ʱ״̬����ʾ�İ����ִ�
*     d.Ҫ��Ӧ�˵�����������ʵ�ַ�ʽ: Exe�ļ����ű��ļ���COM����
*       (1).Exe -- ���Ӽ��������ΪExec��ֵΪExe�ļ���·����
*       (2).�ű��ļ� -- �����Ϊscript��ֵΪ�ű��ļ���·����
*       (3).COM���� -- �����Ϊ ClsidExtension �ַ���ֵΪ��Ϊ����չ��������COM�� GUID,COM ����ʵ�� IOleCommandTarget �ӿ�,
*           COm������ÿ���˵���ť�����������ʶ(OLECMDID_XXX)�����������У���������һ��Ψһ��GUID��ʶ�����������������ʶִ����Ӧ�Ĳ�����
*   3.��Ӷ��ư�ť����׼���������� NetAnts, FalshGet -- HKLM\SOFTWARE\Microsoft\Internet Explorer\Extensions
*     a.����ע����ͬ���Ʋ˵����� CLSID���ַ���ֵ��ͬ
*     b.BandCLSID -- ������Toolbar��CLSID
*       ButtonText--��ť���ı���ǩ
*       Clsid -- �̶�Ϊ {E0DD6CAB-2D10-11D2-8F1A-0000F87ABD16} -- 
*       Hot Icon --Hot ͼ���·��;
*       Icon--��ɫͼ��·��;
*       'Default Visible'--�Ƿ�Ĭ�Ͽɼ�(YES/NO);
*       MenuStatusBar-- ��ʾ���ı�
*       MenuText -- ��ʾ���ı�
*   4.��Ӷ��Ƶ�������������� -- ʵ�����������ӿ� IDeskBand, IObjectWithSite, IpersistStream��������չʵ��IContextMenu
*     �����ֿɶ��Ƶ���(���������������ע�ᣬ����ͨ�� IMPLEMENTED_CATEGORY ��ע�᣿)��
*       ˮƽ����� -- CATID_CommBand
*       ��ֱ����� -- CATID_InfoBand
*         <IE5�Ժ�CommBand �� InfoBand ͳ��Ϊ Explorer Bars��������IE����Դ������>
*       ���湤���� -- CATID_DeskBand
*       IE������ -- �� HKLM\Software\Microsoft\Internet Explorer\Toolbar\ ����COM���CLSIDΪ������һ���ַ���ֵ
*
*   5.��������(ATL DLL) -- Browser Helper Objects(BHO)�����Լ�⵽IE�ĸ����¼�(���ڲ������ӵ�ķ�ʽ֪ͨ����˱���ʵ�� IDispatch �ӿڣ�
*     ������Invoke, ��ӦIE���͵��¼�)��BHO������Windowsϵͳ�µ�IE�����(CLSID_InternetExplorer)���ļ������(CLSID_ShellDesktop)
*     a.ATL DLL => New ATL Object => Internet Explorer Object(�� Simple Object, ����ʱѡ�� ConnectionPoint �� IObjectWithSite)

*     a.HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\Browser Helper Objects ����COM���GUIDΪ���ƴ���һ���Ӽ�
*     b.����ʵ�� IObjectWithSite �ӿڣ��� SetSite �У����Ի�ȡIEʵ���� IWebBrowser2 �ӿڣ�����ȡ��ǰIE��������Ϣ��
*       Ҫ�ػ��¼������ȡ IConnectionPointerContainer �ӿڣ��� FindConnectionPoint (�� DIID_DWebBrowserEvents2),
*       �ҵ�ĳһ�¼���Ӧ�ĳ��ӿڵ����ӵ���󣬲���Adviseע��(Ȼ��ɽػ�������Ĵ����¼���������µ�ַ��ǰ�������ˡ������´��ڵ�)
*       SetSite �д���� IUnknown �ӿڿ���QI��: 
*         IBandSite -- 
*         IDropTarget -- 
*         IInputObject/IInputObjectSite -- , 
*         IOleWindow -- �ɻ�ȡIE��������, 
*         IOleCommandTarget -- ,
*         IPersist/IPersistStream
*         IServiceProvider -- �ɽ�һ����ȡ�Ľӿ�:
*            IWebBrowserApp, IShellBrowser, IHTMLWindow2, IWebBrowserEventsService, IHTMLOMWindowServices, 
*            IShdocvwBroker, ITargetFrame, IProfferService
*         
*  
************************************************************************************************/

/**************************************************************************************************************
* BHO(Browser Helper Object) -- �������������������Ե���������Ա���Ž����ӿڵ�ҵ���׼��
*   ������BHO�����Լ������������ӣ������ᵼ��IE���������������д��DLL 
*     Ҫ��BHO��ʵ��Winsock���ֻ����IE��Ϣ��ʱ��ſ��� ?
* 
* ��дBHO����
*   1.ʵ�� IObjectWithSite �ӿڣ����� COM ����
*   2.�� HKLM\Software\Microsoft\Windows\CurrentVersion\Explorer\Browser Helper Objects\ ��ע��BHO����
*
* ������ٳ� -- ������ҳ,����(���������������ܵ�),Ю����ת�û������URL����ַ�ֶ��滻,�޸ĵ�ǰ��ҳ����,ͳ�ƺ�̨
*   ����SetSite����ǿ��IE��ת������Ա�趨��ҳ��ȥ
*   ���� about:blank �۸���ҳ�ġ�������ٳ֡�����
*
* IURLSearchHook -- �ȴ�IE����IURLSearchHook�������������������ת��һ��δ֪��URLЭ���ַ
*   ���������ͼȥ��һ��δ֪Э���URL��ַʱ����������ȳ��Դ������ַ�õ���ǰ��Э�飬
*   ������ɹ����������Ѱ��ϵͳ������ע��Ϊ��URL Search Hook������Դ�������ӣ�USH���Ķ��󲢰����IE�������ĵ�ַ���͹�ȥ
* 
* Winsock LSP(Windows Socket Layered Service Provider) -- �ֲ�����ṩ��, Winsock2.0���ṩ��
*   ֧��SPI(Service Provider Interface, �����ṩ�̽ӿ�)��������ϵͳ���Ѿ����ڵĻ���Э���ṩ��(��TCP/IPЭ��),
*   ����ЩЭ�����ɷֳ�����Э�鼴Ϊ���ֲ�Э�顱����SSL��ͨ�� LSP�����Ը��򵥵ķ�������������(�� Sniffer )
*   
**************************************************************************************************************/
