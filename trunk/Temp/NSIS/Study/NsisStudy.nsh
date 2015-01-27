/******************************************************************************************************************************
* ���ʵ��
*   1.���� !define MUI_VERBOSE 4 , �����ڱ���ʱ�������ϸ�ı�����Ϣ(�����ǿ���!verbose), ������������ѧϰ��������ʱ���ȡ��, ����������������Ϣ
*   2.
*
* TODO��
*   0.��ô�鿴 !insertmacro ��չ���Ľ��?
*   1.Unicode�汾��
*   2.MUI_ABORTWARNING -- TODO: �� System.nsh ���Ǹ���, ���ܶ�ط���д !define MUI_ABORTWARNING , ��Ϊ��ȡ���궨�廹��˵��������ͬ��?
*   3.ж����Ϣ(һ�㶼д�� HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\ ��Ŀ¼��)
*   4.�ƺ�����ͨ�� !insertmacro INSTALLOPTIONS_WRITE "ioSpecial.ini" "Field 3" "Bottom" "185" �ķ�ʽ(�μ� MUI_WELCOMEPAGE_TITLE_3LINES)����ȫ���ƽ��沼��
*
* �����ĵ�
*   NSIS�����ĵ�(��ɫ����)��ͨ���鿴 System.nsh ��ϵͳԴ���ļ�, ����ѧ���ܶ��÷�
******************************************************************************************************************************/


/******************************************************************************************************************************
* NSIS(Nullsoft Scriptable Install System) -- http://nsis.sf.net, ��Դ�� Windows ϵͳ����Ѱ�װ������������, ͨ���ű�����
*   Դ����룺ʹ�� python + scons ����
*
* �������Ƶİ�װ�������������У�Inno Setup,  wix(΢��)
*
* �������Ľű��������ɻ�����
*   HM NIS EDIT(�д��������ű�����, F9Ԥ�����, nisedit2.0.3.exe) -- http://sourceforge.net/projects/hmne/ (http://hmne.sourceforge.net/ -- ��?)
*     TODO: ��װ���Ժ�ִ�� NSIS-> ���� -> ���� nsis �İ�װ��ִ��·��
*     ʹ��ʱ�ǳ�����, ��ͨ�� Ctrl + ��� ������ת; ֱ�Ӳ鿴����İ���
*     ��Դ(�����޸Ĵ�����֧��): http://ftp.jaist.ac.jp/pub/sourceforge/h/hm/hmne/HM%20NIS%20Edit/2.0.3/nisedit203-src.zip
*       1.��֧�� Unicode �� UTF-8 ��ʽ���ļ�
*       2.SaveAs ʱ�����Ĭ�ϵ���չ��
*       3.���Ӵ�Ŀ¼���Ĺ���
*   Venis IX -- http://www.spaceblue.com/products/venis/
*   HM VNISEdit -- ���� HM NIS EDIT �ĸĽ��汾�����ܸ�ǿ�󣬵�Ҳ��֧��Unicode? http://restools.hanzify.org/article.asp?id=43
*   makensisw -- NSIS�ṩ��GUI�������, �ƺ�ֻ�� Unicode �汾����?
*   NotePad2 -- ��˵�� nsi ���﷨�����ȽϺ�( http://foobar.nease.net/nsis/np2.exe -- �ڿ�Դ�������޸ĵ�?)
******************************************************************************************************************************/

/******************************************************************************************************************************
* ���뷽ʽ��
*   makensis.exe [option | script.nsi ] -- �����еı�����
*   makensisw.exe -- GUI�ı�����
*   ��ѡ����:
*     /Vx -- ���Ʊ���ʱ�����Ϣ����ϸ�̶�(�ȼ��� !define MUI_VERBOSE ����). x����ֵ�� 4(all),3(no script),2(no info),1(no warnings),0(none).
*     /Ofile -- ָ���ı��ļ���¼��־, �� /OResult.log
*
*   ע�⣺
*     2.����ѹ���㷨(ZLib, BZip2, LZMA) -- LZMA ѹ�����б�����ͨ��ѹ���������õ�Ч��, ͨ�� SetCompressor ����
*     3.��ͨ��"XXXX"ʵ���Զ���Ի���ͽ���
*   ��������:
*     1.SetOverwrite -- �����Ƿ񸲸��Ѿ����ڵ��ļ�����ѡֵ:[on(ֱ�Ӹ���,�粻�ܸ��ǵĻ���ʾ)],off(������),try(��������),ifnewer(����)
*     2.������ҳ���ӵĿ�ݷ�ʽ -- WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
*     3.��Ҫ�Զ�����棬���Բ鿴 Contrib\Graphics �µ� Graphics �� Language files Ŀ¼���ݣ�ȷ��ʹ�����ĸ���Դ��
*       Ȼ���� System.nsh �в����÷�����Ӧ������صĳ������
*   ��Ĭ��װ(silent installations) -- ����ȫ������NSIS�����Ĭ�����ý��а�װ, ��װ����Ҳ����ʾ, ȫ����̨����
*     �������� /silent ? �� /M ?
*
*   �ű�(.nsh/.nsi -- ��� makensis.exe ͬĿ¼���� nsisconf.nsh ͷ�ļ�, ���Զ�����)
*     �﷨(�����ṹ���� ��װ������������, ҳ��, ����, ���� )
*       0.�����﷨
*         ע�� -- �Էֺ�(;) ��ʼ���в���
*         ת���ַ� -- $ǰ׺, �� $\n ��ʾ����,  $$ ��ʾ��Ԫ��"$", $\"��ʾ˫����
*         ����ܳ�ʱ, ������ "\" �����м���д, �� Messagebox MB_OK \<CR> "This is line breaks demo"
*         !define ������ [����ֵ]   -- ����ʱ��${������}, TODO: ������ζ�������, ����ĻḲ��?
*            ����ͨ�� !insertmacro MUI_SET Xxx �ķ�ʽ�����峣���ҷ�ֹ���ζ���
*         !echo "��Ϣ" -- ����ʱ�ڱ��봰���������ָ����Ϣ(TODO:�ƺ����ѿ���?)
*         !macro ���� �����б� XXX !macroend -- �����,ʵ���п�ͨ�� ${������} �ķ�ʽ����, Ȼ��ͨ�� !insertmacro ���� ���� �ķ�ʽ����
*            ע��:���ں�ʵ���п���ͨ�������ӵķ�ʽƴ�ӳ���Ӧ�ĳ��������������ȣ������Դ��������ȫ�ĵĻ��������޷�������.
*                 ( ���� MUI_PAGE_CUSTOMFUNCTION_SHOW = MUI_PAGE_CUSTOMFUNCTION_ + SHOW )
*         !ifdef|!ifndef MY_DEFINE  xxxxx !endif -- �ж��Ƿ���������ĺ�, ��ͨ�� !define �� makensis.exe ����ʱͨ�� /D"MY_DEFINE" �ķ�ʽ����
*         !include ͷ�ļ��� -- ����ָ����ͷ�ļ�, Ȼ�󼴿�ʹ�����еĳ���, ����, ��ȡ��� !include "LogicLib.nsh" �� !include "MUI.nsh" ��
*         !insertmacro ���� -- �ڵ�ǰλ�ò��붨��ĺ꣨��ϵͳԤ���� MUI_PAGE_WELCOME �ȸ���Pageҳ�涼�Ǻ� -- \Modern UI\System.nsh �ļ��У�
*         !verbose -- ���������Ϣ�ĵȼ�, Ĭ��Ϊ3(no script), Ϊ�˵õ�����ϸ����Ϣ, ����ͨ�� !define MUI_VERBOSE 4 �ķ�ʽ����Ϊ all
*         var [/GLOBAL] ������, ����ʱ $������, �������ַ�����ʽ�洢����1024�ֽڵ�����, ����ı�������ȫ�ֵ�(��ʹ�����λ����ඨ�壿)
*             ��ɫ -- ����HTML�е�RGB��ʾ��, �����þ���"#"
*         ���õ�Ԥ����ϵͳ������
*           $INSTDIR(��װĿ¼), $OUTDIR(Ŀ��ϵͳ�Ĺ���·��), $CMDLINE, $LANGUAGE
*           $PROGRAMFILES[64], $COMMONFILES, $DESKTOP(��ǰ�û�����), $STARTMENU(��ʼ�˵�Ŀ¼), $SMPROGRAMS(��ʼ�˵����ProgramsĿ¼),
*           $MUSIC, $FONTS, $APPDATA, $TEMP(ϵͳ��ʱĿ¼)
*           $RESOURCES, $RESOURCES_LOCALIZED
*         64λϵͳ��װ�ж�: !include x64.nsh => ${If} ${RunningX64} xxxxx ${Else} yyyyy ${EndIf}
*    ��װ�ű�
*       2.���� -- һ�������һ�鹦�ܻ����໥�����Ĺ���, ����һ��������, �����Ը�����ȡ������,
*                 �������齨ҳ��Ϳ�����ʾ������ֹ��û�ѡ��(������������û�����ֻ���ǰ׺"-",��ʾ��һ�����صĶ�(�û����ܸ���).
*                 �����ǰ׺ "un." ���ʾ��ж��ʱ�Ķ�
*         Section "[un.]������"
*            ��������, ��Ӧĳ�ְ�װ/ж��ѡ��Ĵ����߼�, �� ��ѹ�ļ�, ��ȡ��д��ע���, INI�ļ�����ͨ�ļ�, ����Ŀ¼, ��ݷ�ʽ��
*         SectionEnd
*         ���������η�, �� RO
*         �����Σ�SubSection
*       3.�û��Զ��庯��(ʹ�� Call ָ������) �� �ص������� �� .onInit )
*         a.�û��Զ��庯����
*           Function <������> ������ FunctionEnd
*           ע��: ����û�в������ݡ�����������ͨ�� ȫ�ֱ��� �� ��ջ���� Pop, Push, Exch �� 20 ���Ĵ������� $0��$9, $R0��$R9 ����
*         b.NSIS Ԥ����Ļص�����(��װ��ж��) -- ע��:����һЩϵͳҳ����Ժ���Զ����ɶ�Ӧ�Ļص�����
*             .onGUIInit <== TODO: �ƺ�Ĭ������(������ʲô����ɵ�?)
*             .onInit -- ��װʱ��ʾ����ǰִ��
*             .onInstFailed
*             .onInstSuccess <== ��װ�ɹ���ִ��
*             .onGUIEnd <== ��װ�����GUI������ִ��
*             .onMouseOverSection -- Ŀǰʵ��,�� MUI_PAGE_COMPONENTS ҳ����,�����Section���ƶ�ʱ��ִ��(�ɽ���Ч������?)
*             .onRebootFailed
*             .onSelChange -- ���ѡ��ҳ�������ʱ
*             .onUserAbort
*             .onVerifyInstDir -- MUI_PAGE_DIRECTORY ҳ��ÿ���û��������İ�װĿ¼ʱִ�У�һ�����ڼ��Ŀ¼�Ϸ���
*             un.onGUIInit
*             un.onInit <== ж��ʱ��ʼ����һ���������û�ȷ���Ƿ�Ҫж��
*             un.onUninstFailed
*             un.onUninstSuccess <== ж�سɹ�
*             un.onGUIEnd
*             un.onRebootFailed
*             un.onUserAbort
*       4.ָ�� -- �ɴ�����, �� /REBOOTOK . TODO: ������?
*           ����ָ�Delete, Exec, ExecShell, ExecWait, Rename, RMDir
*             Abort -- �жϵ�ǰ����, ���� .onInit �б�ʾֹͣ��װ���˳�
*             Delete -- ɾ��ָ���ļ����������ɾ������ʾ����, ����ָ���� /REBOOTOK
*             File -- �ͷ��ļ�����ǰ���·��($OUTDIR) , ͨ����Ҫ���SetOutPathʹ�á����ļ����������ڱ���ʱ��ǰϵͳ���ڵ��ļ�
*               ����: /r �ݹ�, /oname=Ŀ���ļ���, /x �ų�ָ���ļ�,  �� File /r *.dll
*             SetOutPath -- �������·��(������$OUTDIR��ֵ?), ��·��������ʱ���Զ����������õ�·����  $INSTDIR, $SYSDIR,
*             SetRegView -- ����ע�����������·��(64λϵͳ�ϵ�32λWOW64����ֻ�ܷ��� 32λ��View����Ҫͨ�� SetRegView 64 ���������64λ��View)
*           ��ţ�ʹ��Goto, IfErrors�������г������, �﷨Ϊ"���: ���", ��ű��붨���ں�����������, �����÷�Χ�����ڶ����������λ�����
*              �Ե��"."��ͷ�ı����ȫ�ֱ�ţ��� .onInit ��Ԥ����Ļص�����?��
*           �����ת���﷨Ϊ "[+-][1-9]", �Ӻű�ʾ�ӵ�ǰλ��(Ϊ0)��ǰ��ת, �������ʾ�ӵ�ǰλ��������ת�����ֱ�ʾ��ת�����������
*              ����Ķ���ʱ��ͨ����Ҫ�޸������ת, ���׳�������Ƽ�ʹ�� LogicLib.nsh ���档
*              �� Goto +4
*           ��֧�жϣ�IfErrors, IfSilent, IfFileExists, StrCmp,
*                     LogicLib.nsh �ļ����ṩ�ܶ�򵥵ĺ���ڹ��츴�ӵ��߼��ṹ(�����������߼��ж�),����Ҫ�������ת���(���Ƽ��÷���), �磺
*                       ${If} $0 == 'some string' ${ElseIf} xxx ${Else} yyy {EndIf}
*                       ${Switch} $0 ${Case} 'some string' xxx ${Break} ${Default} yyy ${Break} ${EndSwitch}
*           ע���ReadRegStr
*           ���Ժ�����
*              MessageBox MB_YESNO "��ʾ��Ϣ" IDYES ��ת��� �� MessageBox MB_OK "��ʾ��Ϣ". ��ͨ�� /SD ����ֵ ָ��silentģʽʱ�ķ���ֵ
*              DetailPrint �ַ�����Ϣ����� -- �ڰ�װ�Ľ��ȴ�����ʾ��Ϣ, �� DetailPrint $R0
*              Dumpstate
*
*
*       8.�����Կ��� -- �����Ҫ���¶������ֻ�����,�鿴 %NSIS%\Contrib\Language files\ �µ��ļ��������Ӧ�ĳ�������
*         !insertmacro MUI_LANGUAGE "����(��English, SimpChinese) <== ��Ҫ��Ӧ����Դ�ļ�(�� Contrib\Language files\English.nsh �� English.nlf)
*
*       9.ϵͳ����(һ����ͨ�� !define �����һЩ�� ?  �鿴System.nsh ��ϵͳ�ļ�Դ��)
*         MUI_ABORTWARNING -- ���ư�װʱȡ���Ļ��Ƿ�����ʾ
*       10.ϵͳ�ṩ�ĳ��� .nsh �ļ�
*          FileFunc.nsh -- �ļ���صĸ�������,���ȡ��С����չ����
*          Library.nsh -- һЩdll, tlb ����صĸ�������
*          LogicLib.nsh -- �߼����ƺ�
*          nsDialogs.nsh --
*          Util.nsh --
*          WinMessages.nsh -- Windows ��Ϣ����
*          x64.nsh -- ������64λϵͳ�ϰ�װ�߼�
******************************************************************************************************************************/
     
/******************************************************************************************************************************
* 1.ҳ�� -- �Ǿ�Ĭ��װ����Ҫ����ҳ��, ����ָ���û����а�װ��
*   ���ý���(??) -- uninstConfirm
*
* NSIS 2.0 �汾֧�ֶ��Ƶ��û�����(Modern UI, �� MUI)-- ģ��Windows������MUIʹ�ú���ָ����������, �ı���NSIS�ű��ı�дϰ��
*   (�����ٱ���ͨ�� LicenseText, Icon, CheckBitmap, InstallColors ��ָ��)
*
* ������ҳ�� (һ����ͨ�� !insertmacro �ķ�ʽ����, ���ڲ���ǰ����ͨ�� !define MUI_XXXX_YYY �ķ�ʽָ���ض�ʱ����ʹ�õ����ֻ�λͼ����Ϣ ):
*   MUI_PAGE_INIT <== TODO: ����ʱû�п��������ҳ��
*   MUI_PAGE_WELCOME|MUI_UNPAGE_WELCOME -- ��ӭҳ�档���Ƴ���:
*     MUI_WELCOMEPAGE_TITLE -- ��ӭ��ҳ�ϵı���(�ַ���)
*     MUI_WELCOMEPAGE_TEXT -- ��ϸ������Ϣ,����ͨ�� "\r\n" ָ���ֶε����֡�$_CLICK" ��ʾ "����[��һ��(N)]����"
*     MUI_WELCOMEFINISHPAGE_BITMAP <== ��ӭ��������λͼ
*     MUI_WELCOMEPAGE_TITLE_3LINES <== ʵ��ʱ���ֶ����˸ñ�����Title �� Text ֮��ļ������(���еľ��룬Ĭ��Ϊ����)
*   MUI_UNPAGE_CONFIRM <== ȷ���Ƿ�ж�أ�����ж�����С�
*     MUI_UNCONFIRMPAGE_TEXT_TOP <== ��ʾ��ҳ�涥��������
*     MUI_UNCONFIRMPAGE_TEXT_LOCATION <== ��ʾ�� uninstall location �Ե�����(ж��Ŀ¼)
*   MUI_PAGE_LICENSE|MUI_UNPAGE_LICENSE licenseData <== ��Ȩҳ��, ��ͨ������ָ�� license �ļ�(�� ".\license.txt" �� ".\license.rtf" )
*     ������Ȩ��ʽ: ��ť(Ĭ��); MUI_LICENSEPAGE_CHECKBOX; MUI_LICENSEPAGE_RADIOBUTTONS
*       ע��: Ҳ��ͨ�� LicenseForceSelection ָ��ֱ��ָ�����е���ϸ��Ϣ
*     MUI_LICENSEPAGE_TEXT_TOP <== ��ʾ��ҳ�涥��������
*     MUI_LICENSEPAGE_TEXT_BOTTOM <== ��ʾ��ҳ��ײ�������
*     MUI_LICENSEPAGE_BUTTON <== ��ť��ʽʱ��ʾ�ڰ�ť�ϵ�����
*     MUI_LICENSEPAGE_CHECKBOX_TEXT <== CheckBox��ʽʱ��ʾ�ڰ�ť�Ե�����
*     MUI_LICENSEPAGE_RADIOBUTTONS_TEXT_ACCEPT | MUI_LICENSEPAGE_RADIOBUTTONS_TEXT_DECLINE <== radio��ʽʱ��ʾ�� ����|�ܾ� ������
*   MUI_PAGE_COMPONENTS | MUI_UNPAGE_COMPONENTS <==(Components.nsh) ѡ����Ҫ��װ�����(TODO: �������?)
*     MUI_COMPONENTSPAGE_TEXT_TOP <== ��ʾ��ҳ�涥��������
*     MUI_COMPONENTSPAGE_TEXT_COMPLIST <== ��ʾ������б��Աߵ�����
*     MUI_COMPONENTSPAGE_TEXT_INSTTYPE <== ��ʾ�ڰ�ת�����������Աߵ�����
*     MUI_COMPONENTSPAGE_TEXT_DESCRIPTION_TITLE <== ��ʾ�������򶥲�������
*     MUI_COMPONENTSPAGE_TEXT_DESCRIPTION_INFO <== ��û�а�װ���ѡ��ʱ����ʾ���������е�����
*     TODO:�ƺ�û�п���"����ռ�" ���ֵĳ���(ͨ����ѯ Components.nsh Դ�룬�ƺ��� mui.ComponentsPage.SpaceRequired ��������?)
*   MUI_PAGE_DIRECTORY | MUI_UNPAGE_DIRECTORY <== ѡ��װ·��, ���Ƴ���:
*     MUI_DIRECTORYPAGE_TEXT_TOP <== ��ʾ��ҳ�涥��������
*     MUI_DIRECTORYPAGE_TEXT_DESTINATION <== ��ʾ��Ŀ��Ŀ¼ѡ����ϵ�����
*     MUI_DIRECTORYPAGE_VERIFYONLEAVE <== ���뿪��ҳ��ʱ��֤Ŀ¼����Ч�ԣ�������"��һ��"�����ť(TODO:�ƺ����ã�������Ҫ�Լ�������Ч����֤, ��ͨ�� GetInstDirError ��ȡ��װĿ¼����Ϣ)
*     MUI_DIRECTORYPAGE_VARIABLE $PLUGINS_FOLDER <== ��Ŀ¼ѡ��ҳ�����û�ѡ���Ŀ¼λ�ô�ŵ�ͨ�� var PLUGINS_FOLDER ����ı�����ȥ,Ĭ���� $INSTDIR
*     TODO:�ƺ�û�п��� "����ռ�" �� "���ÿռ�" ���ֵĳ���
*   MUI_PAGE_STARTMENU ID $Var <== �û�ѡ��˵����λ�á����˰�װ���С�����:
*     ID  <== �� Application,
*     Var <== Ϊͨ�� var ����ı���(��ǰ��ͨ�� var ICONS_GROUP �����, $ICONS_GROUP ʹ��), ���ڻ�ȡ����ֵ���к�������?
*     ���Ƴ���:
*       MUI_STARTMENUPAGE_TEXT_TOP <== ��ʾ��ҳ�涥��������
*       MUI_STARTMENUPAGE_TEXT_CHECKBOX <== ��ʾ�� checkbox �Եı�ʾ��ֹ��ݷ�ʽ����������
*       MUI_STARTMENUPAGE_NODISABLE <== ��ʵ�����ڽ�ֹ��ݷ�ʽ������CheckBox
*       MUI_STARTMENUPAGE_DEFAULTFOLDER folder <== Ĭ�ϵĿ�ʼ�˵�Ŀ¼
*       MUI_STARTMENUPAGE_REGISTRY_ROOT + MUI_STARTMENUPAGE_REGISTRY_KEY + MUI_STARTMENUPAGE_REGISTRY_VALUENAME
*         <== ָ����ʼ�˵��趨��ע����еļ���ֵ�����ڼ�¼�û���ƫ�á���ж��ʱӦ��ɾ��֮
*     ע��: 1.������ݷ�ʽ�Ĵ���Ӧ������ MUI_STARTMENU_WRITE_BEGIN �� MUI_STARTMENU_WRITE_END ֮��.
*           2.ж�س�������ú�MUI_STARTMENU_GETFOLDER��ÿ�ʼ�˵�Ŀ¼
*   MUI_PAGE_INSTFILES | MUI_UNPAGE_INSTFILES -- ��ʾ��װ|ж�ص��ļ�����, Ҫ��ʾ���ҳ��ʱ, ��ǰһ��ҳ���"Next"��ť���Ϊ"Install"
*     TODO: ����һ����ʱ��Ĳ���, �Ƿ��ܿ����ܷ�ȡ��, �Լ�����Ĳ���?
*     MUI_INSTFILESPAGE_FINISHHEADER_TEXT <== ��ʾ��װ����ҳ��ͷ�ϵ����֣������ڽ���ҳ�治��MUI_(UN)FINISHPAGE_NOAUTOCLOSE)����ʾ
*     MUI_INSTFILESPAGE_FINISHHEADER_SUBTEXT <== ��ʾ��װ����ҳ��ͷ�ϵĲ���˵������
*     MUI_INSTFILESPAGE_ABORTHEADER_TEXT <== ��ʾ��װ����ҳ��ͷ�ϵı�ʾ��װ���̷�������ֹ������
*     MUI_INSTFILESPAGE_ABORTHEADER_SUBTEXT <== ��������ֹʱ�Ĳ���˵������
*   MUI_PAGE_FINISH | MUI_UNPAGE_FINISH --(Finish.nsh)����ҳ�档���Ƴ���:
*     MUI_FINISHPAGE_TITLE_3LINES <== ҳ��������Ķ���ո�
*     MUI_FINISHPAGE_TITLE <== ҳ�����
*     MUI_FINISHPAGE_TITLE   "MUI_FINISHPAGE_TITLE"
*     MUI_FINISHPAGE_TEXT_LARGE <== ���ʹ���� checkbox ʱ����Ҫ�趨���ı����Ķ���ռ�
*     MUI_FINISHPAGE_TEXT <== ��ʾ�����ҳ�������֣���\r\n����
*     MUI_FINISHPAGE_TEXT_REBOOT <== ��������ϵͳ��ʾ�����֣���\r\n����
*     MUI_FINISHPAGE_TEXT_REBOOTNOW <== "��������"����ʾ����
*     MUI_FINISHPAGE_TEXT_REBOOTLATER <== "�Ժ�����"����ʾ����
*     MUI_FINISHPAGE_LINK <== �û��ɼ�����ĳ����ϵ�����
*     MUI_FINISHPAGE_LINK_LOCATION file/url <== ָ���û��Ƿ�����ó����鿴��վ
*     MUI_FINISHPAGE_LINK_COLOR color <== ���������ֵ���ɫ(RRGGBB)��Ĭ��Ϊ 000080
*     MUI_FINISHPAGE_RUN "Notepad.exe" <== ָ���������е�Ӧ�ó���
*     MUI_FINISHPAGE_RUN_TEXT <== �������г������ʾ����
*     MUI_FINISHPAGE_RUN_PARAMETERS 
*     MUI_FINISHPAGE_RUN_FUNCTION funName <== ָ����װ��ɺ�Ҫִ�е�nsi�������ڸú����п������ж��Ӧ�ó���
*     MUI_FINISHPAGE_RUN_NOTCHECKED <== ָ������Ӧ�ó��� checkbox Ϊ��ѡ��״̬
*     MUI_FINISHPAGE_SHOWREADME "$INSTDIR\ReadMe.txt"  <== ָ��Readme�ļ�(ָ���û������� checkbox ѡ���Ƿ�鿴���ļ�����վ)
*     MUI_FINISHPAGE_SHOWREADME_TEXT <== ReadMeʱ��������Ϣ
*     MUI_FINISHPAGE_SHOWREADME_NOTCHECKED <== ָ�� 'Show Readme' checkbox Ϊ��ѡ��״̬
*     MUI_FINISHPAGE_SHOWREADME_FUNCTION funName <== ָ����װ��ɺ�Ҫִ�е� nsis �������ڸú����п�����ʾ����ļ�����վ
*     MUI_FINISHPAGE_BUTTON <== ��ɰ�ť����ʾ����
*     MUI_FINISHPAGE_NOREBOOTSUPPORT -- ������������ϵͳ�Ĺ���(Delete,RMDir ��ָ����� /REBOOTOK ʱ, �������ɾ������ʾ����, ����������ɾ��)
*       ������� <== �ж�(IfRebootFlag), ����(SetRebootFlag)
*     MUI_FINISHPAGE_RUN <== ָ���������Զ����еĳ���, һ��Ϊ "$INSTDIR\xxx.exe"
*     TODO:�ƺ���������ѡ���ʱ�� Run �� ShowReadme ������� ?
*
* Header -- ��������ͨ�õ�Header���ƣ���Ϊÿ�� Ԥ����Ԥ����� MUI_PAGE_ �������ʱ�򶼻� undef ��Щ�꣬����ڲ����ǰ�ɷֱ��岻ͬ��ֵ
*   MUI_HEADERIMAGE <== ���� Header �����Ƿ���ʾ����ͼƬ
*   MUI_HEADERIMAGE_BITMAP "Header.bmp"    <== ָ��Header��ͼƬ
*   MUI_PAGE_HEADER_TEXT text <== ��ʾ����ҳͷ�ϵ�����
*   MUI_PAGE_HEADER_SUBTEXT text <== ��ʾ����ҳͷ�ϵ����֣�����ͨ����ʾ�� MUI_PAGE_HEADER_TEXT ֮�£���ʾ�� MUI_PAGE_HEADER_TEXT �Ľ�һ������
*      TODO: ����ʹ�� InstallOptions �Ķ��ƽ��棬ʹ�ú� !insertmacro MUI_HEADER_TEXT "$(TEXT_IO_TITLE)" "$(TEXT_IO_SUBTITLE)" ʵ�����ƹ���
******************************************************************************************************************************/

/******************************************************************************************************************************
* ���ƽ���(InstallOptions) -- һ��NSIS�������������Ա�������Ƶ���ҳ�������������ҳ�治���ṩ���û����롣
*   ������Ƕ��NSIS�����еĶԻ�����ؼ�ͨ�� ini(�� WordFunc.ini ) ���������Ϳ��ƣ���ͨ�� HM NIS EDIT �ȿ��ӻ����߱༭
*   1.��ƶ�����ҳ���棬������������ini�ļ�
*     [Settings] <== ���ÿؼ��������������崰�������
*        NumFields=�ؼ���Ŀ
*     [Field #] <== ÿ���ؼ�����"Field #"ģʽ������"#"�ű�ʾ���֣���1��ʼ
*        ���� Button��Link �ȿؼ�������ͨ�� Flags ָ�� NOTIFY��ʹInstallOptions�����¼������ö���ҳ���validation/leave����
*   2.��дnsi�ű�
*     2.1.���ʹ����lzma,gzip��ѹ���㷨����Ҫ������Fileָ��ǰreserve���ƽ����ini�ļ���ReserveFile ".ini"
*     2.2.�� Function .onInit ��ʹ�ú� !insertmacro MUI_INSTALLOPTIONS_EXTRACT ".ini"
*         TODO:û���ҵ��ú��ʾ���������� ? File /oname=$INI "WordFunc.ini"
*   3.��д���ƶԻ������ʾ���� -- �μ� useInstallOptionsDemo
*   4.��Pageָ���ں��ʵ�λ�ò������ҳ�棬��:
*     Page custom useInstallOptionsDemo "" "--��ʾinstallOptionsDemo�Ի���"
*   TODO:
*     a.PhotoViewer �� !insertmacro MUI_INSTALLOPTIONS_INITDIALOG "InstEnd.ini"
******************************************************************************************************************************/

/******************************************************************************************************************************
*   Page/UninstPage/PageEx ҳ���� -- ָ���ض��İ�װ/ж��ҳ��, ��ͨ���ص�����������֤,��û�лص�, ���Ӧ��λ���� ""
*
*   �޸�����ҳ����Ϊ:
*     Page <internal_page_type> [pre_function] [show_function] [leave_function]
*       internal_page_type: license, components, directory, instfiles, uninstConfirm
*     �ṩ���������Կ��Զ��ƻ��Ľӿڣ�Pre|Show|Leave, �����ַ�ʽ���ж���:
*       1.Page <internal_page_type> ָ��(TODO: �в������ν���);
*       2.ͨ�� MUI_PAGE_FUNCTION_CUSTOM TYPE ��ʽָ��: TYPE��Ϊ: PRE|SHOW|LEAVE���÷�:
*         a.ͨ�� Function Xxx ����ָ����ҳ��ص�����ʵ��
*         b.�� !insertmacro MUI_PAGE_XXX ǰ !define MUI_PAGE_CUSTOMFUNCTION_PRE, MUI_PAGE_CUSTOMFUNCTION_SHOW,
*           MUI_PAGE_CUSTOMFUNCTION_LEAVE �ȳ���Ϊ�Զ���ĺ����������Զ�����
*
*   �Զ���ҳ��:
*     Page <custom> [creator_function] [leave_function] [caption]
*
*
*   nsDialogs.nsh(TODO:�д�ȷ��) -- ��̬�����ؼ�������UI(��ѯ�ĵ������ϸ��˵��) -- CreateXxx �Ľ����Ҫ Pop ��������?
*     �����ؼ�
*       ${NSD_CreateBitmap}      0u 0u 109u 193u ""
*       ${NSD_CreateCheckbox}    120u ${MUI_FINISHPAGE_RUN_TOP}u 195u 10u "${MUI_FINISHPAGE_RUN_TEXT}"
*       ${NSD_CreateLabel}       120u 10u 195u ${MUI_FINISHPAGE_TITLE_HEIGHT}u "${MUI_FINISHPAGE_TITLE}"
*       ${NSD_CreateLink}        120u 175u 195u 10u "${MUI_FINISHPAGE_LINK}"
*       ${NSD_CreateRadioButton} 120u ${MUI_FINISHPAGE_REBOOTNOW_TOP}u 195u 10u "${MUI_FINISHPAGE_TEXT_REBOOTNOW}"
*     ��д����
*       ${NSD_FreeImage}         $mui.FinishPage.Image.Bitmap
*       ${NSD_GetText} <== ?
*       ${NSD_SetStretchedImage} $mui.FinishPage.Image $PLUGINSDIR\modern-wizard.bmp $mui.FinishPage.Image.Bitmap
*       ${NSD_SetImage}          $mui.FinishPage.Image $PLUGINSDIR\modern-wizard.bmp $mui.FinishPage.Image.Bitmap
*       ${NSD_SetFocus}          $mui.FinishPage.RebootNow
*     �����¼�:
*       ${NSD_OnClick}           $mui.FinishPage.Link "${LINK}"
*
*
*   �޸�������װ��ʽ(TODO: δ���Ժ�ȷ��)
*     ʹ�� Resource Hacker ���� NSIS\Contrib\UIs\modern.exe �ļ��Ĳ��֣�֮�����еİ�װ����ȫ������� -- �����޸� NSIS ��Դ����Դ?
******************************************************************************************************************************/
      
/******************************************************************************************************************************
* ��װ�������ԣ��� , InstallDir ��) -- ȷ����װ���������, ��ۺ�ϰ��, ������ʾ���ı�, ��װ���͵�������, �����������ʱֻ��,
*   (�ƺ������κεط����У���һ����� "MUI end" ע�ͺ�λ��ͳһ�ȽϺ�?)
*   BrandingText "�����̡����ߵ���Ϣ" <== ���� ҳ������ �� ���ư�ť֮�����ʾ��Ϣ(Ĭ��Ϊ Nullsoft Install System v2.xx")
*   InstallDir "$PROGRAMFILES\${COMPANYNAME}\${APPNAME_INNER}"  -- ����ȱʡ��װĿ¼
*   InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" "" <==
*   Name "MyPrgramName" -- ���ð�װ��������ʱ��ʾ�ı��������ֵȡ�Ȼ���ڽű����ͨ�� "$(^Name)" ����?
*   OutFile "MyProgramSetup.exe" -- ���ñ����İ�װ��������
*   RequestExecutionLevel admin -- ��Vista��װʱ��Ҫ����ԱȨ��
*   ShowInstDetails|ShowUnInstDetails show -- �����Ƿ������û��鿴 ��װ|ж�� ʱ����ϸ��Ϣ
******************************************************************************************************************************/

/******************************************************************************************************************************
* ���(��չNSIS��װ�����DLL, ϵͳԤ��װ���� Plugins Ŀ¼��, �û����� !addplugindir ����Ŀ¼λ��)
*   ʹ���﷨�� DllName::FunctionName [ѡ��] "����1" ����2" "����3"
*     �����ʹ�ò�����ͨ�� popstring/popint �Ȼ�ȡ, ������ͨ�� pushstring/pushint ���ٷ����ջ
*     ��������ջ�Ĵ�����, �ڽű��п�ͨ�� Pop $0 ����з���
*     �磨�����ļ����� NSISdl::download http://download.nullsoft.com/winamp/client/winamp291_lite.exe $R0
*       (����DLL����): System::Call '������չ����Dll��::����() ����'
*   ����ѡ��:
*     /NOUNLOAD -- ����������ж��, �������ڲ���ڱ��������ݵ����(������ƣ�)
*
* ����ϵͳ���������
*   bgimage::Sound, Clear
*
* �Լ���д���(ʾ���μ� NSIS\Examples\Plugin ) -- ���������ԭ���������£�
*   extern "C" __declspec(dllexport) void mypluginfunc(HWND hWndParent, int string_size, TCHAR *variables, stack_t **stacktop, extra_parameters *extra);
*     hWndParent -- ������װ�ĸ����ھ��
*     stacktop -- ������ջ, ��ͨ�� popstring/pushint �Ⱥ�������, �Ӷ��ͽű�����
*     extra -- ��������script����ʱʹ��, ʹ��ʾ��:
*       a.�ű��л�ȡ�ű�����DllInit�ĵ�ַ, Ȼ�󴫵ݸ����:
*         Function DllInit �ű��ĺ���ʵ�� FunctionEnd
*         GetFunctionAddress $2 DllInit
*         myPlugin::Init /NOUNLOAD $2
*       b.�����ͨ����ջ��ȡ����ַ��, ʹ�� ExecuteCodeSegment ���лص�:
*         EXDLL_INIT(); int nFunc=popint(); extra->ExecuteCodeSegment(nFunc-1,g_hwndParent);
*   ͨ����ջ��ȫ�ֱ���($0~$9,$R0~$R9 ��)���ݲ���, ���ؽ��
******************************************************************************************************************************/

/******************************************************************************************************************************
* ������Ч��ʾ
*   Splash��ʾЧ�� -- ��װ������ʾǰ����ʾ��������Ϣ��( �μ� .onInit ����ʹ�� splash.bmp )
******************************************************************************************************************************/
