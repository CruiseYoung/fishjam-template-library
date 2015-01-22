/******************************************************************************************************************************
* �������Ƶİ�װ�������������У�Inno Setup,  wix(΢��)
* NSIS(Nullsoft Scriptable Install System) -- http://nsis.sf.net, ��Դ�� Windows ϵͳ����Ѱ�װ������������, ͨ���ű�����
* ���ʵ��
*   1.���� !define MUI_VERBOSE 4 , �����ڱ���ʱ�������ϸ�ı�����Ϣ(�����ǿ���!verbose), ������������ѧϰ��������ʱ���ȡ��, ����������������Ϣ
*   2.
* TODO��
*   0.��ô�鿴 !insertmacro ��չ���Ľ��?
*   1.Unicode�汾��
*   2.MUI_ABORTWARNING -- TODO: �� System.nsh ���Ǹ���, ���ܶ�ط���д !define MUI_ABORTWARNING , ��Ϊ��ȡ���궨�廹��˵��������ͬ��?
*   3.ж����Ϣ(һ�㶼д�� HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\ ��Ŀ¼��)
*
*   NSIS�����ĵ�(��ɫ����)��ͨ���鿴 System.nsh ��ϵͳԴ���ļ�, ����ѧ���ܶ��÷�
*   ���뷽ʽ��
*     makensis.exe [option | script.nsi ] -- �����еı�����
*     makensisw.exe -- GUI�ı�����
*     ��ѡ����:
*       /Vx -- ���Ʊ���ʱ�����Ϣ����ϸ�̶�(�ȼ��� !define MUI_VERBOSE ����). x����ֵ�� 4(all),3(no script),2(no info),1(no warnings),0(none).
*       /Ofile -- ָ���ı��ļ���¼��־, �� /OResult.log
*   Դ����룺ʹ�� python + scons ����
*   ע�⣺
*     1.����û�в������ݡ�
*       ����������ͨ�� ȫ�ֱ��� �� ��ջ���� Pop, Push, Exch �� 20 ���Ĵ������� $0��$9, $R0��$R9 ����
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
*     �������Ľű��������ɻ�����
*       HM NIS EDIT(�д��������ű�����, F9Ԥ�����, nisedit2.0.3.exe) -- http://sourceforge.net/projects/hmne/ (http://hmne.sourceforge.net/ -- ��?)
*         TODO: ��װ���Ժ�ִ�� NSIS-> ���� -> ���� nsis �İ�װ��ִ��·��
*         ʹ��ʱ�ǳ�����, ��ͨ�� Ctrl + ��� ������ת; ֱ�Ӳ鿴����İ���
*         ��Դ(�����޸Ĵ�����֧��): http://ftp.jaist.ac.jp/pub/sourceforge/h/hm/hmne/HM%20NIS%20Edit/2.0.3/nisedit203-src.zip
*           1.��֧�� Unicode �� UTF-8 ��ʽ���ļ�
*           2.SaveAs ʱ�����Ĭ�ϵ���չ��
*           3.���Ӵ�Ŀ¼���Ĺ���
*       Venis IX -- http://www.spaceblue.com/products/venis/
*       HM VNISEdit --
*       makensisw -- NSIS�ṩ��GUI�������, �ƺ�ֻ�� Unicode �汾����?
*     �﷨(�����ṹ���� ��װ������������, ҳ��, ����, ���� )
*       0.�����﷨
*         ע�� -- �Էֺ�(;) ��ʼ���в���
*         ת���ַ� -- $ǰ׺, �� $\n ��ʾ����,  $$ ��ʾ��Ԫ��"$", $\"��ʾ˫����
*         ����ܳ�ʱ, ������ "\" �����м���д, �� Messagebox MB_OK \<CR> "This is line breaks demo"
*         !define ������ [����ֵ]   -- ����ʱ��${������}, TODO: ������ζ�������, ����ĻḲ��?
*            ����ͨ�� !insertmacro MUI_SET Xxx �ķ�ʽ�����峣���ҷ�ֹ���ζ���
*         !macro ���� �����б� XXX !macroend -- �����,ʵ���п�ͨ�� ${������} �ķ�ʽ����, Ȼ��ͨ�� !insertmacro ���� ���� �ķ�ʽ����
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
*       0.5��װ�������ԣ��� , InstallDir ��)
*          ȷ����װ���������, ��ۺ�ϰ��, ������ʾ���ı�, ��װ���͵�������, �����������ʱֻ��,
*          InstallDir "$PROGRAMFILES\${COMPANYNAME}\${APPNAME_INNER}"  -- ����ȱʡ��װĿ¼
*          InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" "" <==
*          Name "MyPrgramName" -- ���ð�װ��������ʱ��ʾ�ı��������ֵȡ�Ȼ���ڽű����ͨ�� "$(^Name)" ����?
*          OutFile "MyProgramSetup.exe" -- ���ñ����İ�װ��������
*          ShowInstDetails|ShowUnInstDetails show -- �����Ƿ������û��鿴 ��װ|ж�� ʱ����ϸ��Ϣ
*          RequestExecutionLevel admin -- ��Vista��װʱ��Ҫ����ԱȨ��
*       1.ҳ�� -- �Ǿ�Ĭ��װ����Ҫ����ҳ��, ����ָ���û����а�װ��
*         Page/UninstPage/PageEx ҳ���� -- ָ���ض��İ�װ/ж��ҳ��, ��ͨ���ص�����������֤,��û�лص�, ���Ӧ��λ���� ""
*         ���ý���(??) -- uninstConfirm
*           Page <ҳ����> [pre_function] [show_function] [leave_function]
*         ʹ��Ԥ�����ҳ��(һ����ͨ�� !insertmacro �ķ�ʽ����):
*           ��װ|ж�� ���
*             MUI_PAGE_INIT <== TODO: ����ʱû�п��������ҳ��
*             MUI_PAGE_WELCOME|MUI_UNPAGE_WELCOME <== ��ӭҳ��
*             MUI_PAGE_LICENSE|MUI_UNPAGE_LICENSE licenseData <== ��Ȩҳ��, ��ͨ������ָ�� license �ļ�(�� ".\license.txt" )
*             MUI_PAGE_COMPONENTS | MUI_UNPAGE_COMPONENTS <== ѡ����Ҫ��װ�����(TODO: �������?)
*             MUI_PAGE_DIRECTORY|MUI_UNPAGE_DIRECTORY <== ѡ��װ·��
*             MUI_PAGE_STARTMENU ID $Var <== �û�ѡ��˵����λ�á�����:
*               ID  <== �� Application,
*               Var <== Ϊͨ�� var ����ı���(��ǰ��ͨ�� var ICONS_GROUP �����, $ICONS_GROUP ʹ��), ���ڻ�ȡ����ֵ���к�������?
*               ����, �ڲ����ǰ, �� !define ���Ƴ���:
*                 a.MUI_STARTMENUPAGE_NODISABLE
*                 b.MUI_STARTMENUPAGE_DEFAULTFOLDER
*                 c.MUI_STARTMENUPAGE_REGISTRY_ROOT
*             MUI_PAGE_INSTFILES | MUI_UNPAGE_INSTFILES -- ��ʾ��װ�ļ�����, Ҫ��ʾ���ҳ��ʱ, ��ǰһ��ҳ���"Next"��ť���Ϊ"Install"
*               TODO: ����һ����ʱ��Ĳ���, �Ƿ��ܿ����ܷ�ȡ��, �Լ�����Ĳ���?
*             MUI_PAGE_FINISH | MUI_UNPAGE_FINISH -- ����ҳ�档���Ƴ���:
*               MUI_FINISHPAGE_NOREBOOTSUPPORT -- ������ϵͳ(Delete,RMDir ��ָ����� /REBOOTOK ʱ, �������ɾ������ʾ����, ����������ɾ��)
*                 ������� <== �ж�(IfRebootFlag), ����(SetRebootFlag)
*               MUI_FINISHPAGE_RUN <== ָ���������Զ����еĳ���, һ��Ϊ "$INSTDIR\xxx.exe"
*           ��װ����
*
*           ж������
*              MUI_UNPAGE_CONFIRM <== ȷ���Ƿ�ж��
*           ���ƽ���
*             Page <ҳ����> [creator_function] [leave_function] [caption]
*             MUI_PAGE_FUNCTION_CUSTOM TYPE -- �Զ���ҳ��ĺ�, TYPE��Ϊ: PRE, SHOW, LEAVE,
*             0.ͨ�� Function Xxx ����ָ����ҳ��ص�����
*             1.�û� !define MUI_PAGE_CUSTOMFUNCTION_PRE, MUI_PAGE_CUSTOMFUNCTION_SHOW, MUI_PAGE_CUSTOMFUNCTION_LEAVE �ȳ���Ϊ�Զ���ĺ���
*
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
*           Function <������>
*             ������
*           FunctionEnd
*         b.NSIS Ԥ����Ļص�����(��װ��ж��) -- ע��:����һЩϵͳҳ����Ժ���Զ����ɶ�Ӧ�Ļص�����
*             .onGUIInit <== TODO: �ƺ�Ĭ������(������ʲô����ɵ�?)
*             .onInit -- ��װʱ��ʾ����ǰִ��
*             .onInstFailed
*             .onInstSuccess <== ��װ�ɹ���ִ��
*             .onGUIEnd <== ��װ�����GUI������ִ��
*             .onMouseOverSection -- Ŀǰʵ��,�� MUI_PAGE_COMPONENTS ҳ����,�����Section���ƶ�ʱ��ִ��(�ɽ���Ч������?)
*             .onRebootFailed
*             .onSelChange
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
*             File -- �ͷ��ļ�����ǰ���·��($OUTDIR) , ͨ����Ҫ���SetOutPathʹ��
*               ����: /r �ݹ�,  /x �ų�ָ���ļ�,  �� File /r *.dll
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
*       6.���(��չNSIS��װ�����DLL, ϵͳԤ��װ���� Plugins Ŀ¼��, �û����� !addplugindir ����Ŀ¼λ��)
*         ʹ���﷨�� DllName::FunctionName [ѡ��] "����1" ����2" "����3"
*           �����ʹ�ò�����ͨ�� popstring/popint �Ȼ�ȡ, ������ͨ�� pushstring/pushint ���ٷ����ջ
*           ��������ջ�Ĵ�����, �ڽű��п�ͨ�� Pop $0 ����з���
*         �磨�����ļ����� NSISdl::download http://download.nullsoft.com/winamp/client/winamp291_lite.exe $R0
*            (����DLL����): System::Call '������չ����Dll��::����() ����'
*         ����ѡ��:
*           /NOUNLOAD -- ����������ж��, �������ڲ���ڱ��������ݵ����(������ƣ�)
*       6.1�Լ���д���(ʾ���μ� NSIS\Examples\Plugin ) -- ���������ԭ���������£�
*          extern "C" __declspec(dllexport) void mypluginfunc(HWND hWndParent, int string_size, TCHAR *variables, stack_t **stacktop, extra_parameters *extra);
*            hWndParent -- ������װ�ĸ����ھ��
*            stacktop -- ������ջ, ��ͨ�� popstring/pushint �Ⱥ�������, �Ӷ��ͽű�����
*            extra -- ��������script����ʱʹ��, ʹ��ʾ��:
*            a.�ű��л�ȡ�ű�����DllInit�ĵ�ַ, Ȼ�󴫵ݸ����:
*              Function DllInit �ű��ĺ���ʵ�� FunctionEnd
*              GetFunctionAddress $2 DllInit
*              myPlugin::Init /NOUNLOAD $2
*            b.�����ͨ����ջ��ȡ����ַ��, ʹ�� ExecuteCodeSegment ���лص�:
*              EXDLL_INIT(); int nFunc=popint(); extra->ExecuteCodeSegment(nFunc-1,g_hwndParent);
*          ͨ����ջ��ȫ�ֱ���($0~$9,$R0~$R9 ��)���ݲ���, ���ؽ��
*
*       7.����Ĭ�ϵ�UI -- �� "MUI Settings" ��ͨ�� !define MUI_XXXX_YYY "���ֻ�λͼ��ַ" �ķ�ʽָ���ض�ʱ����ʹ�õ����ֻ�λͼ����Ϣ
*         MUI_WELCOMEPAGE_TITLE -- ��װ������(�ַ���)
*         MUI_WELCOMEFINISHPAGE_BITMAP --
*         MUI_WELCOMEPAGE_TEXT --
*       8.�����Կ��� -- �����Ҫ���¶������ֻ�����,�鿴 %NSIS%\Contrib\Language files\ �µ��ļ��������Ӧ�ĳ�������
*         !insertmacro MUI_LANGUAGE "����(��English, SimpChinese) <== ��Ҫ��Ӧ����Դ�ļ�(�� Contrib\Language files\English.nsh)
*
*       9.ϵͳ����(һ����ͨ�� !define �����һЩ�� ?  �鿴System.nsh ��ϵͳ�ļ�Դ��)
*         MUI_ABORTWARNING -- ���ư�װʱȡ���Ļ��Ƿ�����ʾ
*       10.ϵͳ�ṩ�ĳ��� .nsh �ļ�
*          FileFunc.nsh -- �ļ���صĸ�������,���ȡ��С����չ����
*          Library.nsh -- һЩdll, tlb ����صĸ�������
*          LogicLib.nsh -- �߼����ƺ�
*          WinMessages.nsh -- Windows ��Ϣ����
*          x64.nsh -- ������64λϵͳ�ϰ�װ�߼�
******************************************************************************************************************************/
