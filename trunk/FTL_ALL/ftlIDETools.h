#ifndef FTL_IDE_TOOLS_H
#define FTL_IDE_TOOLS_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlIDETools.h requires ftlbase.h to be included first
#endif

//�������Ա����� C++ ��Դ��ȫ -- http://blog.jobbole.com/78901/

/******************************************************************************************************************************
* BigBlueButton -- ��Դ��Ƶ����ϵͳ(sourceforge������)
*   ʹ�� ActionScript ������Զ�̽���ƽ̨, ���������õ�����Ŀ������ ActiveMQ,Asterisk,Nginx,Tomcat��
*   ��������(8.0)������Ubuntu 10.04 32-bit ���� 64-bit.  ����bbb�������������ַ�ʽ:
*     1.��װ����װ
*     2.��װbbb�����: VirtualBox + �Ž�, ��ʼ�û�/������ firstuser/default
******************************************************************************************************************************/

/***********************************************************************************************
* TODO:
*   ���ֱ༭���Ƽ�������ļ�����:
*   1.VS -- ��ǩ����UTF8
*   2.Eclipse -- ����ǩ����UTF8(Ant ��֧��ǩ��)
*   3.Go���� -- 
*
* ���뾲̬����(���뾯��)ʱ������ĵ���:
*   C4700 -- uninitialized local variable 'Xxx' used
*   C6001 -- Using uninitialized memory 'Xxx'
***********************************************************************************************/


/***********************************************************************************************
* �������ܵ� Tip --http://www.watch-life.net/visual-studio/visual-studio-2008-tip-day-index.html
*   ѡ����δ�������(��ģʽ)��Alt + ���
*   ��ʽ����ǰ�ĵ���Ctrl+K, Ctrl+D; ��ʽ��ѡ���ı���Ctrl+K, Ctrl+F -- ���⣺��ô���ø�ʽ���Ĺ���?
*   �۵������壺Ctrl+M, Ctrl+O, ���Կ���������еķ���
*   ��Tooltip����ʾ��ݼ����Զ���->������->����Ļ��ʾ����ʾ��ݼ�
*
* IDE -- ͳһ��������
*   VS6--Windows 9xƽ̨��VS�����汾
*   VS2002(7.0)--.NET Framework 1.0,�������йܴ���, ��.NET�����³������ΪMSIL��΢���м����ԣ������ǻ�������
*   VS2003(7.1)--֧���°汾��.NET 1.1, ��һ���ȶ��汾
*   VS2005(8.0)--.NET 2.0, ��ʱ, ΢���Ƶ������Ʒ�ġ�.NET��ǰ׺, ��Visual Studio��Visual Basic
*   VS2008(9.0)--��һ��֧�ֲ�ͬ��.NET�汾, ����2.0, 3.0��3.5, ֧��WPF, WCF��WF�Ŀ���, 
*     ���� LINQ, Silverlight ��������
*   VS2010()--
* �汾
* 
* Windows SDK
*   7.0 -- Microsoft Windows SDK for Windows 7 and .NET Framework 3.5 SP1
*          http://www.microsoft.com/en-us/download/details.aspx?id=18950
*   7.1 -- Microsoft Windows SDK for Windows 7 and .NET Framework 4
*          http://www.microsoft.com/en-us/download/details.aspx?id=8442
*
* ����IE��ʹ��MFC����ʱ�����ű����������:
*   http://blogs.msdn.com/b/vcblog/archive/2009/03/28/some-vs2005-and-vs2008-wizards-pop-up-script-error.aspx
*   �������(��Ҫ����)��
*     [HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Internet Settings\Zones\1000]
*        "1207"=dword:00000000
************************************************************************************************/

/************************************************************************************************
* ��̬��Ĳ鿴, �������� -- libdump(��Դ��)
* 
* How to: Deploy using XCopy -- http://msdn.microsoft.com/en-us/library/ms235291(VS.80).aspx
* 
* ������ CRT, MFC �ȶ���汾ʱ���ض��� -- ȱ�� DebugCRT.9.0.30729.6161 ���޷����г��������
* 1.������Ӧ�汾��  Microsoft.VC90.CRT.manifest,  msvcr90.dll, msvcp90.dll ���ļ��� ��ִ�г���Ŀ¼ -- �ɽ���󲿷�����
* 2.���� ������.exe.config �ļ�(�� Application Config File, Ӧ�ó�������ʱ��������Ϣ), ���ݿ�����£��������е� name , oldVersion, newVersion �ȣ���
*   <?xml version="1.0"?>
*   <configuration>
*     <windows>    <!-- ע��˴�ԭ���� runtime(ָ��  .NET assemblies ?) -->
*       <assemblyBinding xmlns="urn:schemas-microsoft-com:asm.v1">
*         <dependentAssembly>
*           <assemblyIdentity type="win32" name="Microsoft.VC90.DebugCRT" processorArchitecture="x86" publicKeyToken="1fc8b3b9a1e18e3b"  /> 
*             <bindingRedirect oldVersion="9.0.30729.6161" newVersion="9.0.21022.8"/>  
*             <bindingRedirect oldVersion="9.0.30729.1" newVersion="9.0.21022.8"/>  
*           </dependentAssembly>
*       </assemblyBinding>
*     </windows>
*   </configuration>
*
* ��ǰ���Թ���Ӱ����̫��ķ����Ǹ��� winsxs �е��ļ���(��Ҫ���� WinSxs �������߲����޸�)
* �޸� C:\Windows\winsxs\Manifests\x86_policy.9.0.microsoft.vc90.debugcrt_1fc8b3b9a1e18e3b_9.0.30729.1_none_61305e07e4f1bc01.manifest �ļ�, 
*   ԭ��ֻ�����У�
*  		<bindingRedirect  oldVersion="9.0.20718.0-9.0.21022.8" newVersion="9.0.30729.1"/>
*       <bindingRedirect  oldVersion="9.0.30201.0-9.0.30729.1" newVersion="9.0.30729.1"/>
*   �������µ� 
*       <bindingRedirect  oldVersion="9.0.30729.6161-9.0.30729.6161" newVersion="9.0.30729.1"/>
*   ע������ļ��Ǵ� SxsTrace ���ɵ���־�ļ����ҳ����ġ�
*       bindingRedirect ָ�� ���оɰ汾��Χ�Ŀͻ�����ӳ�䵽ָ�����°汾��, Ҳ�ɶԱ���ֶν���ӳ�䣿��ʽΪ old + �ؼ��� ?
* WinSxs(Windows Side-by-Side assembly, �Ӵ����г���)
*
* ���ؼ���
*   A.����ϵͳ�� manifest �ļ���Ӧ�ó��򱾵��ļ�����,����Ϊ <assemblyName>.manifest,  �� Microsoft.VC90.CRT.manifest, ����Ӧ�� DLL �ļ�(�� msvcr90.dll,msvcp90.dll �ȣ���������ͬ��Ŀ¼
*   B.Ҳ���Խ��� <assemblyName> �ļ���, ��������  <assemblyName>.manifest �� ��Ӧ�� DLL �ļ�
* 
* ManiFest -- ��������
*   �ͻ��� -- ���������Exe, Dll��, ָ�����ļ�������DLL��Ϣ, ��Ҫ�� type,version, processorArchitecture, publicKeyToken ����Ϣ
*   �������� -- ΢��ȿ����� DLL, ָ�����ļ���ǿ������Ϣ, ����ͨ�� file( hashalg/hash/DigestValue ��) ָ������ص������֤��Ϣ
*
* policy�������ļ���ȷ��ӳ���ϵ��-- 
*  
* ǿ��������Ϣ
*   type -- ϵͳ����, �� win32, win32-policy
*   name -- �� assemblyIdentity  ��������Ϣ,  �� Microsoft.VC80.CRT �� policy.8.0.Microsoft.VC80.CRT ��
*   version -- �汾�ţ�������Ƕ���汾�� DLL,  ���������
*   processorArchitecture -- ƽ̨����, �� x86, ia64 ��
*   publicKeyToken -- ��Կ(���ڶ����ǩ���ģ���ͬ����������ͬ? ΢��� ATL/CRT/OPENMP �� ���� "1fc8b3b9a1e18e3b", �� Common-Controls �� 6595b64144ccf1df)
*
*   hash -- �ļ���Hashǩ��, ÿ���ļ�Ψһ, ȷ���ļ��������� -- ��������汾��һ(bindingRedirect), �Ƿ������������
* 
* 
*   ����Ӧ�ó������, Manifest������һ����exe/dll�ļ�ͬһĿ¼�µ�. manifest �ļ�, 
*   Ҳ��������Ϊһ����ԴǶ���ִ���ļ���(Embed Manifest)��
*     Exe ����ԴIDΪ 1( CREATEPROCESS_MANIFEST_RESOURCE_ID )
*     Dll ����ԴIDΪ 2( ISOLATIONAWARE_MANIFEST_RESOURCE_ID )
*     ���⣺3( ISOLATIONAWARE_NOSTATICIMPORT_MANIFEST_RESOURCE_ID ) ��ʲô��?
*           24( RT_MANIFEST ) ��ʲô��?
*   
*   ��Դ(.rc)�ļ�����Ϣ
*     VarFileInfo
*        Translation -- �ļ���������ʾ������(Ӣ��Ϊ"0x0409 0x04B0", ����Ϊ"0x0804 0x04B0")
*     StringFileInfo
*        ÿ�����Զ�Ӧ����Ϣ(�� BLOCK "040904B0" ��ӦӢ��)
*     
*       
*
*   WinXP��ǰ��Windows����� manifest �����ݡ�
*   VS2010 �ֲ���ʹ��manifest�ˣ�
*
*   ע�⣺VC������������ atlassem.h, crtassem.h, MFCassem.h ���ļ�, ���ж����˵�ǰ���뻷���»�ʹ�õ� manifest,  
*         Ҳ�������ļ���ͨ�����µĴ���ָ�� -- #pragma comment(linker, "/manifestdependency  ...)
*           �� stdafx.h ��ָ�� Microsoft.Windows.Common-Controls
*   
* manifest �е� assemblyIdentity ��Ϣ�� winsxs ���ļ��Ĺ�ϵ( ǿ�ļ�����Ϣ -- ϵͳ�п����ж����ͬ�汾����ͬ�Ŀ⹲������ᷢ����ͻ)
*   xml�е���Ϣ��<assemblyIdentity type="win32" name="Microsoft.VC90.CRT" version="9.0.21022.8" processorArchitecture="x86" publicKeyToken="1fc8b3b9a1e18e3b"></assemblyIdentity>
*   ��Ӧ�ļ�Ϊ ��C:\Windows\winsxs\x86_microsoft.vc90.crt_1fc8b3b9a1e18e3b_9.0.21022.8_none_bcb86ed6ac711f91\msvcp90.dll
*            ����C:\Windows\winsxs\<processorArchitecture>_<name>_<publicKeyToken>_<version>_<???>_<???>\xxx.dll
* 
* "Unable to start program" ����Ľ��(ʹ�� SxsTrace ����, ֻ�� Vista/Win7 ���� ?)
*  1.�Թ���Ա�˺����� cmd.exe, ���л���Ŀ�ĳ����Ŀ¼��
*  2.SxsTrace Trace -logfile:SxsTrace.etl
*  3.ִ�� �����ĳ���
*  4.SxsTrace Parse -logfile:SxsTrace.etl -outfile:SxsTrace.txt,  ���ɶ� mainfest ��������־�ļ���
*  5.������־�ļ�, ���в��ҡ�����/Error�� �ȹؼ���, ������ʾ�Ĵ����ļ�ͨ�� EveryThing �ȹ��߽��в���ȷ��
*    ��(SxsTrace���������Ĵ�����Ϣ)
*    ����: �޷������ο� Microsoft.VC90.DebugCRT,processorArchitecture="x86",publicKeyToken="1fc8b3b9a1e18e3b",type="win32",version="9.0.30729.6161"��
*    ������Debug �� XXX.exe.embed.manifest �� XXX.exe.intermediate.manifest �������˱����ϲ����ڵ� Microsoft.VC90.DebugCRT �汾
*         (������ WinSXS ��ֻ�� 9.0.21022.8 �� 9.0.30729.1 �� x86_microsoft.vc90.debugcrt �汾, ���嵥�ļ��������� 9.0.30729.6161 �汾)
*    ԭ�������ӵľ�̬��ʹ���� 9.0.30729.6161(��ͨ�� dumpbin /all ���ض���������� manifestdependency ȷ��)
*      
*   VS90SP1-KB2538241-x86.exe(MFC Security Update) -- http://www.microsoft.com/zh-CN/download/details.aspx?id=26360
*   vcredist_x86.exe �ļ����汾������·��
*      9.0.30729.1(VS2008 SP1) -- http://www.microsoft.com/en-us/download/confirmation.aspx?id=5582
*      9.0.30729.6161(KB2538243/MS11-025) --  http://www.microsoft.com/en-us/download/details.aspx?id=26368
* 
* Fuslogvw.exe -- Assembly Binding Log Viewer,��ô�ã�
* PDBCopy.exe -- ���Ƶ��Է���
* SvcTraceViewer.exe -- �鿴ϵͳ��־�Ĺ���?
************************************************************************************************/

/******************************************************************************************************************************
* ���� SxsTrace ȷ���������ĸ���ԭ��?
* VS2008 ����
* 1.��������Ϊ manifest �ļ���������(processorArchitecture�ȳ�������, ����Ϊ0xc000007b), 
*   �����޷�����, ��Ҫ�ӹ�����ɾ��manifest�ļ�, ��stdafx.h��������´��룺
#ifdef _UNICODE
#  if defined _M_IX86
#    pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#  elif defined _M_IA64
#    pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#  elif defined _M_X64
#    pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#  else
#    pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#  endif
#endif
******************************************************************************************************************************/


//�ж�ָ��IP�Ļ�����Ϣ(�������)�� nbtstat -a IP

#pragma TODO(���ǲο�һ�� ATL_DLL_ ���÷�)
// ��������˸ĺ�, ���̽�ʹ��ATL.DLL������, ATLIMPL.CPP �ļ�����ʵ����Ϊģ����һ���ְ���

namespace FTL
{
    //Wp64 ����, �µ�IDE�ǲ���û����������ˣ�
    // Gotta love it.  I've NEVER found a way to cast away the /Wp64
    // warnings with the functions Microsoft tells you to use for
    // Win64 readiness!
#pragma warning ( disable : 4244 )
    //����(�� SetWindowLongPtr)
#pragma warning ( default: 4244 )

    /*************************************************************************************************************************
    * IDE��������͹���
    *   nmake
    *   dumpbin �����ִ���ļ�(EXE, DLL)����Ϣ��
    *     dumpbin /headers xxx.lib ��鿴 "FILE HEADER VALUES" ����Ϣ, ����֪����Ӧ���ļ��� x86/x64 ��
    *     ���� x86/x64 lib �����: dumpbin /headers *.lib > check.txt ��ȫ������ ��Dump of file", Ȼ��鿴���� machine ����
    *   mt.exe -- manifest �Ŀ��ƹ���
    *     mt.exe -inputresource:ExePath;#1 -out:extracted.manifest <== ���� Exe �е� manifest
    *   undname <== C++����������
    *   ϵͳ�豸ɨ�蹤�� -- gmer.exe
    *   Inspect -- MS�ṩ������Spy�Ĺ���, �ܿ�������, IAccessible ����Ϣ
    *   AccExplorer32�������Բ鿴����(����Spy++) -- �ɲ鿴DirectUI����(�� MSN), ���鲻��DUIlib �� QQ�ȵ�
    *   �����ܵ�ɨ�蹤�� -- pipelist.exe(http://technet.microsoft.com/zh-cn/sysinternals/dd581625)
    *   
    *************************************************************************************************************************/

    /*************************************************************************************************************************
    * 010Editor -- �༭�Ͳ鿴�������ļ��Ĺ���, ����ͨ��ģ������ļ�����������ʹ�ô��ض������ļ�ʱ�Զ�ʹ�ö�Ӧ��ģ�档
    *   �﷨
    *     0.��ͨ�� #include "Vector.bt" ���﷨����Ԥ�����ģ���ļ�, ʵ������
    *     1.ֻҪ��������, �����ͻ�ӳ�䵽�ļ���ǰλ�õ����ݡ�����ͨ���ؼ��� "local" ������
    *       char|wchar_t str[] �� string|wstring str; -- ��̬ƥ��NULL��β���ַ���(��ȡ�ַ�����ֱ������NULL)
    *     2.ʮ������: 0xff, 25h; ������: 0b011; 12u ���޷�������; -1L ��ʾint64; 0.2��8�ֽڵ�double; 0.2f ��4�ֽڵ�float 
    *     3.�����������ͨ�� <xxx=yyy> ָ���ض�������, �磺
    *       bgcolor=<color>, -- ָ����ǰ�����ı���ɫ, SetBackColor() ����ָ����ǰλ�ÿ�ʼ�ı���ɫ
    *       comment="xxxx", 
    *       fgcolor=<color>, -- ָ����ǰ������ǰ��ɫ��()
    *       format=hex|decimal|octal|binary,
    *       hidden=true|false,
    *       open=true|false|suppress, 
    *       read=<function_name>, 
    *     4.��ɫֵ����ͨ�� 0x0000FF(BGR��ʽ) ��ʮ�����Ƹ�ʽ �� cBlack|cLtGray|cRed|cNone �ȳ���ָ��
    *     5.��ͨ�� struct|union �ȶ���ṹ��, ���п��԰��� if|for|while �ȿ������, ����Ƕ�ײ�ʹ������(�μ� ?).
    *       ����ṹʱ����ָ������, �� typedef struct (int arraySize) { int id; int array[arraySize];}VarSizeStruct 
    *       Ȼ����������ʱ��ͨ������ָ����С��
    *     6.����λ�����ģʽ(bitfield modes) -- ��Ӱ��ṹ���λ��ģʽ( ushort r:5; ushort g:5; ushort b:5 }
    *       TODO:��Ϊλ��ģʽ���Զ����Ƚϸ���, ����ڶ��� �ṹ���λ������ʱ�ͽ�����λ�������, ������ϵͳ���
    *       [padded bitfields] | unpadded bitfields
    *     7.�Զ��庯��:
    *       <return type> <function name> ( <argument_list> ) { <statements> }
    *   ����:
    *     1.����ʹ��ָ��;
    *     2.��֧�ֶ�ά����(Ҳ����strings����), ����ͨ�� �ṹ��+���� �ķ�ʽģ��
    *   ���ú��� -- ����Ĭ��Ϊֵ����, ��ͨ�� & ��ʾ���ô���
    *     BigEndian|LittleEndian -- ���ô��(ȱʡΪ packed left-to-right)|С��(ȱʡΪ packed right-to-left)
    *     BitfieldRightToLeft|BitfieldLeftToRight -- ����λ��ģʽʱ�Ķ�ȡ��ʽ
    *     BitfieldDisablePadding|BitfieldEnablePadding -- �л�λ������ģʽ
    *     FSeek(pos) -- ��λ��ָ����ַ, ����0��ʾ�ɹ�
    *     FSkip(offset) -- ƫ��
    *     FTell() -- �����ĵĵ�ǰλ��
    *     FEof() -- �����Ƿ񵽴��ļ���β
    *     FileSize() -- �ļ���С
    *     Printf|Warning("string='%s', lenght=%d\n", str, Strlen(str)); -- 
    *     ReadBytes(�ڴ����,
    *     ReadByte|ReadShort|ReadInt|ReadFloat|ReadQuad -- ��ָ����ֱַ�Ӷ�ȡ����(��ͨ����������ķ�ʽ)
    *     SetBackColor|SetColor|SetForeColor -- ָ����ǰλ�ÿ�ʼ��Ĭ����ɫ
    *     Strchr|Strlen|SubStr -- �ַ�������
    *     SwapBytes(data_type) -- TODO:�����������ֽ�? ���� 0x1234 => 0x3412 ?
    *   �÷�:
    *     while(!FEof()){ �����ű� } -- �������ļ���β
    *   ���ʵ��:
    *     1.���ڳ�������, �����ں������� <optimize=true> �����Ż�(�μ� BMP )
    *   ʾ��:
    *      1.Bmp�������е�4�ֽڶ���:
    *        local int bytesPerLine = (int)Ceil( bmih.biWidth * bmih.biBitCount / 8.0 );
    *        local int padding      = 4 - (bytesPerLine % 4);
    *        if( padding == 4 )
    *          padding = 0;
    *************************************************************************************************************************/

    /*************************************************************************************************************************
    * Avalanche -- һ��(HTTP)ѹ�����Ե�Ӳ����
    * FileZilla -- ��Դ��FTP����˺Ϳͻ���, ͨ������(FileZilla Server Interface)Զ�̹���
    *   (TODO: �������ص���downloader, https://www.filezilla.cn/ �����ص���װ��) 
    *   
    *   
    *************************************************************************************************************************/

    /*************************************************************************************************************************
    * C++ Test,һ�����ɽ������, �ɽ���
    *   a.��̬����(Static Analysis) -- ��̬��������,�����ָ������淶�����һ����
    *   b.��Ԫ����(Unit Testing) -- �����Զ����ɴ�����Ԫ��������, ����ִ���쳣, ����, �ع�ȶ��ֲ���, 
    *     �ɲ������Ը����ʡ��ڲ��Կ����, �ṩ��˽�кͱ�����Ա�����Լ���Ա�����ķ��ʡ�
    *     ͨ��׮����(������ǰ׺CppTest_Stub_)�ṩ����������ռλ, ʹ�ÿ��Զ����ؽ��е�Ԫ���Զ��������ⲿ�ļ�������
    *   c.Ƕ��ʽ֧��(Embedded Support)
    *   d.���븴��(Code Review)
    *   e.�Զ�����򴴽�(Rule Wizard) -- ��ͼ�λ��ķ�ʽ��ͨ����������ͼ�η�ʽ�������Զ���������ͨ���ṩ��ʾ��������Υ���Ĵ��룩��
    *     ��Ҫ python ��֧��?
    *   f.�������?[ԭ�����Ե���?(Native Tests Import)]
    *   g.BugDetective(���������ṩ) -- ģ��Ӧ�ó���ִ��·��, ��ʶ����ܴ�������ʱȱ�ݵ�·��, �漰��ʶ��͸��ٸ���·��, 
    *   h.�����н���(Desktop/Extended Command Line Mode)
    *
    * TCM(Team Configuration Manager -- �Ŷ����ù�����),ȷ�������Ŷӳ�Ա���ܷ�����Ӧ�Ŷӵ� Test Configuration���������ã�, 
    *   suppressions(����), �����ļ��Ͳ��������ļ�, Ȼ���ɸ��������Ա����Import��
    *   �Ż����Ŷ����÷�ʽ�ǣ�
    *     ���Ŷӹ��������ϰ�װһ�� C++test��Server Edition��-- ÿ���Զ�����,��֤����Ĵ���,��������͵�GRS�������Ա.
    *     ��ÿ̨������Ա����վ�ϰ�װһ�� C++test��Developer Edition�� -- �����Լ���д���޸ĵĴ���,������Դ�������.
    *     �ڼܹ����ʦ�����ϰ�װһ�� C++test��Architect Edition��, 
    *     ���Ŷӹ�����������һ̨�Ŷӻ����ϰ�װһ�� Team Configuration Manager���Ŷ����ù���������
    *   ���裺
    *     1.������Ĭ��װ�����ã�����ʾ��, �������ý��а�װ�����ã�-- ��װһ�κ�, ������ setup.iss�ļ�, ����
    *       ��ֵ����Ҫ��װ�Ļ�����, ��ʹ�� -s ��־���� setup.exe
    *     2.������ C++test ������Դ�������(����ȷ����������,�Ӵ�����л�ȡͳһ�Ĵ���):
    *       Preferences->Scope and Authorship �� Source Control
    *     2.������ C++test ��װ������ TCM(�ṩ�ļ�������ļ�����):Preferences->Team
    *     3.�� C++test ������������ GRS(�ṩ�Ǳ��, ����Ͷ���ֵ,�ɰ���������Ŀ����������׼�����).
    *     4.�����ŶӲ������ú͹���
    *     5.������������
    *     6.���г�ʼ���Ժͼ�������
    *     7.ͨ��Դ�������������Ŀ�Ͳ�����Դ
    *     8.�����Զ�ҹ�����

    * �����������(�簲ȫ��, �Ż�, ��ʼ����),�������Էּ�.
    * �����ڼ���Test Configuration����ѡ��Ĺ������ܼ��, ��ƥ��Suppressions(����)�����Ľ����������ʾ����.
    * �ȿ���ͨ��IDE����,Ҳ����ͨ�������н���, ������ʽ���лع���ԡ�
    * ����֧��Ƕ��ʽƽ̨�ͽ��������, ���Ժ� Eclipse + CDT һ������
    * GRS(�鱨��ϵͳ)--��һ������֧��ϵͳ,ʹ������������̾��п����Ժ��ܿ���

    * ������(cpptestcli.exe)--������������,��������͸�GRS��TCM.
    * AEP(�Զ�����Ԥ��)--�Զ��������岽��:ʶ�����->�ҵ�����ԭ��->��λ��Ʒ��������ĵط�->�޸����е�ʵ��->��������
    *
    * C++Test����������Ĵ������⹲��Ϊ5�����س̶�:
    *   1.I   -- Informational
    *   2.PV  -- Possible Violation
    *   3.V	  -- Violation
    *   4.PSV -- Possible Severe Violation
    *   5.SV  -- Severe Violation
    *
    *************************************************************************************************************************/

    /*************************************************************************************************************************
    * ClearCase(CC) -- 
    *   IBM�İ汾����ϵͳ, ����C/S�ṹ, ֧�ִ���, ��������, ֧�ֿ�ƽ̨(Win/Linux)����, ��һ������eclipse��CCRC(CC Remote Control)����, 
    *   ��ʹ������Ϊ MVFS �������ļ�ϵͳ(Clear Case Virtual File System)
    *
    *   VOB(Versioned Object Base) -- �汾�����, �����˽��а汾���Ƶ��ļ���Ŀ¼(Element)�����������Ԫ����(Metadata), ������
    *     ��������, �û���VOB�����в�������Ҫͨ��View����ͼ����ʵ��
    *   View -- �ͻ��˲�������ͼ, �����֣�
    *     ��̬Dynamic : Զ�˾���, ʵʱ����, �ʺϹ�����Ա
    *     ��̬Snapshot : ���ؾ���, �ֶ�����, �ʺϿ�����Ա
    *   Version -- �ͻ���ÿ�� CheckOut->Edit->CheckIn ��������һ����Version
    *   Version Tree(�汾��) -- һ��Ԫ�ص����а汾��ɵ���״�ṹ, չʾ��Ԫ�ذ汾���ݻ����
    *   Label(��ǩ) -- ����ĳһ�汾�ϵı�ʶ, ͨ��ת����ʱ��Ҫ���ǩ����ǩ��ͬ�汾��һ�£�, �Ժ���Կ��ٶ�λ���뼯��
    *   Bracnch(��֧) -- ������VOB��������Ԫ���ض��汾�ļ���, ��ʵ�ֲ�ͬ�汾�Ĳ��п������÷����ʾ�� -- ע���Label������
    *   ���ù��(Config Spec) -- ȷ����VOB������View��Ԫ�ؼ���汾��ȡ��ϸ��, ������ÿ��profile��
    *     CCRC�У�����->��ʾClearCase��ͼ����->�汾ѡ����� -> �����Ӧ��Ԫ�ذ汾����, �����£�latest��
    *************************************************************************************************************************/

    /*************************************************************************************************************************
    * CppUnit --
    *    VS2008����ʱ���ִ���
    *      1.error C3505: cannot load type library '{80CC9F66-E7D8-4DDD-85B6-D9E6CD0E93E2}
    *        ����Ӧ������ (MsDevCallerListCtrl.cpp:67 ) �� version("7.0")  ��Ϊ version("9.0") ����
    *      2.TestRunnerDSPlugInD.dll.intermediate.manifest �б������ general error c1010070: Failed to load and parse the manifest
    *
    *************************************************************************************************************************/

    /*************************************************************************************************************************
    * curl-loader -- http://curl-loader.sourceforge.net
    *   ��Ƶ��http://www.boobooke.com/v/bbk1808/
    *         http://www.boobooke.com/v/bbk1809/
    *         http://www.boobooke.com/v/bbk1835/
    *   һ����C���Ա�д�� Web Ӧ�ò��Ժ�ģ�⸺�ع���, һ�����̿���ģ��2.5K~10K�Ŀͻ��˲�������, 
    *   ����ÿ���������Բ�ͬ��IP��ַ(����LoadRunner��IPαװ), ֧���û���֤, ��¼��Web�������̡�
    *   ��ͨ�� libcurl ��֧�� sftp, telnet, tftp, ssh, scp ��Э��
    * ��װ(����ʹ��root�û�)��
    *   tar -zxvf ��ѹ�� make ����
    *   ./curl-loader -f �����ļ� [ѡ��]
    * �����ļ�(conf-examples)
    * ѡ��
    *   -c :����ʱ�ĳ�ʱ
    *   -d :��ϸ��־(���������ݻᱣ�浽�ı�, �ʺ��ı�����)
    *   -i ����ʱ��, Ĭ��3��
    *   -f �����ļ�: 
    *   -r :��ֹ����TCP����(�رպ����´�)
    *   -t �߳���:�ڶ�CPU, ���CPUʱ���Բ�������ѹ��( cat /proc/cpuinfo )
    *   -v :verbose
    *   -u -- 
    * ѹ��ͼʾ
    *   D -- ��Ӧʱ��(ms)
    *   Response Time(��Ӧʱ��)
    *   Throughtput(������), ��ΪTi(nput)��To(utput)
    *************************************************************************************************************************/

    /*************************************************************************************************************************
    * Devpartner(VC6) -- ��̬���Թ���, ��ҪӦ���ڰ׺в���, �����ڵ�Ԫ���Խ׶κͼ��ɲ��Խ׶Ρ�
    * ֻ�ܼ�鱻ִ�й��Ĵ������Ƿ���ڴ���, �ڿ�ʼʹ��BoundsCheckerǰ, �ƶ�����ȫ��Ĳ�������
    * �ǳ���Ҫ(ʹ�� TrueCoverage )��
    * �������������ӹ��ܣ�
    *  1.BoundsChecker --Run-Time������
    *    ����ʹ��ģʽ��
    *    a.ActiveCheck -- ������, ���ܼ��Ĵ�����������(�ڴ����Դй¶, API����ʹ�ô���)
    *    b.FinalCheck()-- �������м����(�ڴ�������),�����������ٶȱ�����ʹ��BC�ı���������Rebuild����
    *  2.TrueCoverage -- (���)������ͳ�ƹ���, �����ڲ������������
    *    ���εĲ��Խ�����Ժϲ���
    *    ѡ�С�Instrument inline function��-- ���Բ������������ĸ�����(�����ڼ��ģ���Ч�ʣ�)
    *  3.TrueTime -- ���ܲ��Թ��ߡ�
    *    Ϊ���������ṩ�����ֲ�ͬ�ĵ�λ(Scale), �ֱ��ǣ�CPUʱ������, ΢��, ����, ��.
    *    ʹ��"Quantum"����, ֻ���㻨��Ӧ�ó�����߳��ϵ�ʱ��Ƭ��
    *    �䡰Real����ʾ��ʱ���ǡ�����ϵͳ��ͬʱ�����ŵ�����Ӧ�ó�����߳����ķѵ�ʱ��(ǽ��ʱ�䣿)��
    * 
    *
    *************************************************************************************************************************/


    /*******************************************************************************************************************************
    * �ĵ�����doxygen -- ������ĳ�ָ�ʽ��ע���Զ�ת��Ϊ��Ӧ���ĵ� http://www.stack.nl/~dimitri/doxygen/index.html
    * ���裺
    *   1.ΪProject ���������ĵ� -- ����ʹ�ø������� Doxygen Wizard ����(�������� ��doxygen -g �ĵ��� ����һ��ȱʡ�������ĵ�)
    *     һЩ Key ��ֵ���趨��ÿ���趨Ϊһ�С�����һ�п�ͷΪ"#" ��ʾ����Ϊ��ע��
    *     TAG = value [value, ...] �� TAG += value [value, ...]
    *     ���� TAG ѡ��:
    *       PROJECT_NAME -- Project ������, ��һ������Ϊ��, ���������ʹ��˫������ס
    *       PROJECT_VERSION -- Project�İ汾����
    *       OUTPUT_DIRECTORY -- ���·�������û�������·��, ������Ŀǰ����·������Ϊ���·��
    *       OUTPUT_LANGUAGE -- Ԥ��ΪEnglish, ����ʹ��Chinese-Traditional ��������ķ���
    *       INPUT -- ָ�����ػ���ѰҪ����ĳ�����뵵��·���������һ������ʽ����̬�����ҿ�ָ��������·����
    *         ������˵������a.c, b.c, c.c ��������������ʹ��INPUT = a.c, b.c, c.c �ķ�ʽ��
    *         ��������һ��Ŀ¼, ��Ŀ¼�������е������ᱻ����
    *       FILE_PATTERNS -- �ļ���չ����������� INPUT Tag ��ָ����Ŀ¼��������͸�����Tag��Ҫ��Doxygen�ڴ���ʱ, 
    *         ֻ����ض����ļ����ж��������磺��ϣ����Ŀ¼�µ���չ��Ϊ.c, .cpp��.h�ĵ���������
    *         �����趨 FILE_PATTERNS = *.c, *.cpp, *.h��
    *       RECURSIVE -- ����ֵ, ֻ����YES��NO�����趨ΪYESʱ, INPUT��ָ��Ŀ¼��������Ŀ¼���ᱻ����    
    *       EXCLUDE -- �������ĳ�����ض���������Ŀ¼, ��ϣ������Doxygen�������������Tag��ָ����
    *       EXCLUDE_PATTERNS -- ������FILE_PATTERNS���÷�, ֻ�����Tag�ǹ�EXCLUDE��ʹ�á�
    *       SOURCE_BROWSER -- ����趨ΪYES, �� Doxygen �������Դ�ļ����б�, �Թ����ġ�
    *       INLINE_SOURCES -- ����趨ΪYES , ��������Ҳ�ᱻǶ����˵���ļ��С�
    *       ALPHABETICAL_INDEX -- ����趨ΪYES, ��һ��������ĸ������б������ڲ������ļ��С�
    *       GENERATE_HTML -- ���趨ΪYES,�����HTML�汾��˵���ļ���HTML�ļ���DoxygenԤ������ĸ�ʽ֮һ
    *       HTML_OUTPUT -- HTML�ļ������Ŀ¼������һ�����·��, ����ʵ�ʵ�·��Ϊ OUTPUT_DIRECTORY ���� HTML_OUTPUT ������趨Ԥ��Ϊhtml��       
    *       HTML_FILE_EXTENSION -- HTML�ļ�����չ����Ԥ��Ϊ.html��
    *       HTML_HEADER -- Ҫʹ����ÿһҳHTML�ļ��е�Header�����û��ָ��, Doxygen��ʹ���Լ�Ԥ���Header�� 
    *       HTML_FOOTER -- Ҫʹ����ÿһҳHTML�ļ��е�Footer�����û��ָ��, Doxygen��ʹ���Լ�Ԥ���Footer�� 
    *       HTML_STYLESHEET -- ���ɸ���һ��CSS ���趨, ��HTML����������������
    *       GENERATE_HTMLHELP -- ���趨ΪYES, Doxygen�����һ�������ļ�����������ļ�������Ҫ���� windows �ϵ�HTML��ʽ��HELP����ʱ���õ��ϡ�
    *       GENERATE_TREEVIEW -- ���趨ΪYES, Doxygen���������һ����״�ṹ, �ڻ�����ࡣ�����״�ṹ����JavaScript��д�ɡ�������Ҫ�°��Browser������ȷ��ʾ��
    *       TREEVIEW_WIDTH -- �����趨��״�ṹ�ڻ����ϵĿ�ȡ�
    *       GENERATE_LATEX -- �趨ΪYES ʱ, �����LaTeX ���ļ�����������ϵͳ����Ҫ�а�װLaTeX ����ع��ߡ�  
    *       LATEX_OUTPUT -- LaTeX�ļ������Ŀ¼, ��HTML_OUTPUT�÷���ͬ, һ����ָ��OUTPUT_DIRECTORY֮�µ�·����Ԥ��Ϊlatex��                         
    *       LATEX_CMD_NAME -- LaTeX������������Ƽ��������ڡ�Ԥ��Ϊlatex 
    *       GENERATE_RTF -- ���趨ΪYES , ������RTF ��ʽ��˵������
    *       RTF_OUTPUT -- ��HTML_OUTPUT �÷���ͬ, ����ָ��RTF �������·����Ԥ��Ϊrtf��
    *       GENERATE_MAN -- ���趨ΪYES , ������Unix Man Page ��ʽ��˵���ļ���
    *       MAN_OUTPUT -- ��HTML_OUTPUT �÷���ͬ, ����ָ��Man Page�����Ŀ¼��Ԥ��Ϊman��
    *       GENERATE_XML -- ���趨ΪYES , ������XML ��ʽ��˵���ļ���
    *       ENABLE_PREPROCESSING -- ���趨ΪYES , ��Doxygen ������C ��ǰ�ô�����������ԭʼ����
    *       PREDEFINED -- �����������ж���һЩ�ꡣ������gcc �е�-Dѡ��
    *   2.�ڳ�������м��Ϸ���Doxygen��������ע��ʽ
    *     ԭ����, Doxygen �����������ṹ�йص�ע��, �� �ļ�, ��, �ṹ, ����, ����, �� �ȵ�ע�͡������Ժ����ڱ���, ����ȵ�ע�͡�
    *     ͨ��һ��ע�Ϳ����һ����Ҫ˵����brief����һ����ϸ˵����detailed��, ����ͷ�ļ��и������Ͷ���, �ؼ�����, ��Ķ���, 
    *     doxygen�ṩ��group�ĸ���, ���ɵ�ģ���ע�ͻᵥ������һ��ģ���ҳ����(defgroup)����
    *     �κ����������������, ����, �����ļ������Ҫ���뵽ĳ��ģ��, ��������doxygenע����ʹ��ingroup����ɡ�
    *     Group֮��ʹ��ingroup����, ���������״��ϵ��
    *
    *       ����ǰʹ�� @brief �������Ҫ˵��, ��һ�к����д����ϸ��ע�ͼ��ɡ�
    *     / **
    *      * @brief [some brief description ]
    *      *      [ brief description more. ]
    *      * @param
    *      * 
    *      * /
    *     �����¸�ʽ��
    *       /// ע�ͣ�
    *       //! ע��
    * 
    *     Doxygen����ע�����
    *        @addtogroup ������
    *        @author ����
    *        @brief  ��Ҫ˵��
    *        @bug ���ڵ�Bug˵��
    *        @code ע����˵���Ĵ��� -- �� @endcode ����
    *        @date ����
    *        @defgroup ���� ������
    *        @deprecated -- ˵���ǲ���֧�ֵ�
    *        @exception �쳣���� [�쳣˵��]
    *        @file �ļ���
    *        @ingroup [group_name] -- ����һ���ض���ע����
    *        @image html "��Ҫ�����ͼƬ�ļ���"'
    *        @name ����ע�͵�����
    *        @note ע������
    *        @param ������ [in|out|in,out] ����˵��
    *        @post ����֮���ʹ������
    *        @pre ǰ������
    *        @relates �ǳ�Ա���������� -- ͨ�������ѷǳ�Ա������ע���ĵ����������˵���ĵ���
    *        @see ��������
    *        @since �汾������ -- ͨ������˵����ʲô�汾, ʱ��д�˲��ִ���
    *        @version �汾
    *        @warning ������Ϣ
    *        @return ����ֵ˵��
    *        @retval value �ض�����ֵ������
    *        @todo Ҫ��������˵��
    *    ��չ��
    *        @par requirements  -- ��Ҫ�Ļ���
    *        @par license
    *    �б�
    *        @return 
    *         <dl>
    *           <dt>escSuccess</dt> <dd>������Ϣ</dd>
    *           <dt>escInvalidParameter</dt> <dd>������Ϣ</dd>
    *         </dl>
    *   3.ʹ��Doxygen�������ĵ�
    *     ���Ҫ����CHM��ʽ���ĵ�, �����ڡ�HHC_LOCATION����ָ�� HTML Help Workshop �� hhc.exe ��·��
    *
    * ����
    *   GhostDoc -- VS����Ѳ��, ����������Ա��дXML��ʽ��ע���ĵ�
    *******************************************************************************************************************************/

    /*******************************************************************************************************************************
    * http://www.cnblogs.com/TankXiao/archive/2012/02/06/2337728.html
    * Fiddler(http���Դ���)--��¼��������пͻ��˺ͷ�����֮���http/httpsͨѶ, 
    *   �鿴���н�������,���Լ���, ���öϵ�, �޸�����������ݵȡ�������һ��ǿ��Ļ����¼��ű�����ϵͳ, ��ʹ��JScript.NET���Խ�����չ
    *   ����ԭ���Դ���web����������ʽ����, ʹ��127.0.0.1:8888, ����ʱ�Զ����ô���, �˳�ʱ�Զ�ע������
    *   
    *   ��������
    *     1.����HTTPS(ȱʡ����) -- Tool->Fiddler Options->HTTPS tab->ѡ�ж�Ӧ�ĸ�ѡ��("Decrypt HTTPS traffic", "Ignore server certificate errors"��)
    *     2.����ض�����
    *       Firefox/Chrome -- �ֹ�������ʹ�� 127.0.0.1:8888 ��Http����
    *       VS����ASP.NET -- ��localhost����Ӹ���ż���, �� http://localhost:2391/Default.aspx => http://localhost.:2391/Default.aspx
    *     3.���öϵ�(�ж�ʱ���޸����е��κ�����)
    *       ����1: Rules->Automatic Breakpoint->Before Requests, ���ж����еĻỰ
    *       ����2: �� QuickExec �����д����� bpu URL ������ֻ����ָ��URLʱ�жϡ�ֱ������ bpu ������ϵ�
    *               ���� bpafter URL ����Responseʱ�ж�
    *     4.���á�Զ�����ӡ�������ʵ��Fiddle����Macϵͳ������HTTP/HTTPS(TODO: ���������, �������˵Ļ���Ҳ��?)
    *       a.Mac�а�װ�����(��VMware);
    *       b.������а�װWindowsϵͳ, ����װFiddle���;
    *       c.�����������ʹ�á�Bridged�� ģʽ -- ������е�Windowsӵ�ж�����IP��ַ��
    *       d.����Fiddle -- Tools->Options->Connections->Allow remote computers to connect
    *       e.����Mac, �Ѵ��������ָ��Fiddler���ڵ�Windows -- System Preferences ->Network->Advanced...->Proxies
    * 
    *   ��������
    *     AutoResponder -- ������ӱ��ط����ļ�, �����ý�http request ���͵���������
    *        ����Ҫ�滻�ɱ����ļ��ĻỰ�϶���AutoResponder, ��ѡ��Enable automatic reaponses ��Unmatched requests passthrough
    *     Composer -- ���Դ����ͷ����Զ����HTTP Request
    *     Filter -- ���˻Ự
    *     Script -- 
    *       1.��װSyntaxView:Inspectors tab->Get SyntaxView tab->Download and Install SyntaxView Now,
    *         ��װ�ɹ�������������һ��Fiddler Script Tab, �������б�д OnBeforeRequest ���¼�ʱ�Ľű�, 
    *         �ڽű������������Զ��޸�Request��Response������(CustomRules.js).
    *******************************************************************************************************************************/

    /******************************************************************************************************************************
    * Git -- ��������linux�ں˿����Ŀ�Դ�ķֲ�ʽ�汾����ϵͳ, ����Ҫ�������������֧��
    *   ����ʽ(SVN) -- �����Ƿ�����, �зǳ���ȷ��Ȩ�޹������(��֧��������), ���ʺϿ�Դ����
    *   �ֲ�ʽ(Git) -- �����߿��Ա������߹���(�ύ, �ϲ�, ������֧��, ÿ�������߻����϶���һ�������������ݿ�, clone -> pull/fetch),
    *     Ȼ�󱾵��޸ĺ����ɲ���ͨ���ʼ��ύ��������������, ��������ȷ�Ϻ�ϲ�������������( push/ pull )
    *   git��Ϊ��ɫ�������ĺϲ�����(merge tracing)����
    *   gitû���ṩȨ�޹�����, ��Ҫͨ��ssh����
    *   Git�е���ظ���
    *     ��֧(branch) -- master��Ĭ�ϵķ�֧,git������master�Ͽ�ʼ��չ������, ���Ǵ����Լ��ķ�֧
    *     ����(Ref) -- 
    *
    *   �������(git, ʹ�� git help ���� ������ʾ��ϸ��HTML����)
    *     add �ļ���Ŀ¼ [-A] -- �����ύ���ļ�add��, -A ��ʾ���δ���ٵ��ļ�
    *     branch <��֧��>-- ��֧����, �� ������֧[Ĭ��], �鿴��֧(--list), ɾ����֧[-d]
    *     checkout [��֧��] -- ���л���֧��, ����� branch ������ temp ��֧,  ��Ĭ�ϵ� master ��֧
    *     clone Զ��Ŀ¼ ����Ŀ¼ -- �ӷ������Ͽ�¡���ݿ⣨��������Ͱ汾��Ϣ����������
    *     commit -m "ע����Ϣ" -- �ύ���������ݿ�
    *     config -- ���������Ϣ, ���� user.email �� user.name, commit������ǰ����Ҫ����"--global"��Χ��������Ϣ
    *     diff [��֧1] [��֧2] > patch -- �Ƚ�������֧, ��ͨ���ض������ɲ���
    *     fetch -- �ӷ������������ݿ�, ���ŵ��·�֧, �����Լ������ݿ�ϲ�, ͨ�����ڽ����ͻǰ�ȱȽ�һ��
    *     init -- ����һ�����ݿ�(����Ŀ¼�´���һ�� .git ����Ŀ¼)
    *     log -- �鿴�汾��ʷ
    *     merge -- �ϲ���֧, ��Ŀ���֧�ϲ�����ǰ��֧
    *     mv -- �������ļ������ļ���
    *     patch -- �򲹶�?
    *     pull IP��ַ:���ݿ��ļ��� -- �ӷ������������ݿ�, �����Լ������ݿ�ϲ�
    *     push [IP:���ݿ��ļ���] [�汾] -- ���������ݿ������Լ������ݿ⣨һ�����������߲����ύ��
    *     reset -- �ָ���֮ǰ�İ汾
    *     rm -- ɾ���ļ������ļ���
    *     show -- �鿴�汾�Ŷ�Ӧ�汾����ʷ�����������HEAD�鿴���°汾
    *     status -- ��ʾ��ǰ��״̬(���� ���ڷ�֧, �ļ�����)
    *     update-index -- �ڲ�����, �� git add ��Ӧ�� git update-index --add
    *     whatchanged [��֧1] [��֧2] -- �鿴������֧�ı仯
    * ������ع��� -- �뿪��һ�� nGit(�ο� Git �� Gemit��ʹ��, ����VS���IE View)
    *   Git -- http://git-scm.com/download/win
    *   EGit -- Eclipse �µ� Git Plugin
    *   Gerrit -- ����Web�Ĵ������󹤾ߺ���Ŀ������
    *   GitHub -- ���� Hudson ����?
    *   TortoiseGit -- �ṩ Git �� Windows ��Դ������������(ͼ�λ���������)
    *   Msysgit -- Window�µ�Git�ͻ��˳���, http://code.google.com/p/msysgit/
    * 
    * Git����ͨ��SSH����(ssh-keygen -t rsa -C "xxx" ����)
    *******************************************************************************************************************************/

	/******************************************************************************************************************************
    * GNU ����(Windows �µ�),
    *   MinGW(Minimalist GNU For Windows) + MSYS(����linux�������л���) , һ�׾����Windowsƽ̨������, ���Cygwin���С�ܶࡣ
    *     �ṩ�������Ŀ�Դ���빤�߼�, ������û�е�������̬���ӿ�������ʹ��GCC����Win32����
    *     C/C++ ���뻷����binutils/mingw-runtime-dev/mingw-runtime/w32api/gcc-core/gcc-c++/libstdc++ ��
    *     ��װ��
    *       0.ͨ�� mingw-get-setup.exe ѡ����Ҫ��װ�����, ���߰�װ
    *       1.���� MINGW_HOME ��������ָ�����Ŀ¼, �� %MINGW_HOME%\bin ����Path
    *       2.�� %MINGW_HOME%\bin\mingw32-make.exe ����Ϊ make.exe ��eclipseʹ��
    *     ����������(msys\1.0\msys.bat)��
    *       1.���к�ͨ�� mount �ɼ��ر����ϵ����д���, ��ֱ�ӷ��ʣ����� / �¿�����), Ҳ�����޸� MinGW\msys\1.0\etc\fstab �ļ�ָ��Ŀ¼
    *       2.����ͨ�� ln -s �ķ�ʽ���� mingw Ϊ msys �µ�mingw Ŀ¼(TODO: ʵ�ⷢ�ֻ´��Ŀ¼, ���һ�ݹ飿)
    *       3.��GTK+ for Windows���� pkg-config.exe �� GLib DLL �� MinGW/bin
    *         http://ftp.gnome.org/pub/gnome/binaries/win32/dependencies/pkg-config_0.23-3_win32.zip
    *         http://ftp.gnome.org/pub/gnome/binaries/win32/glib/2.28/glib_2.28.1-1_win32.zip
    *         TODO:1.��Щdll�� VMWare ����; 2.���� copy libintl-8.dll initl.dll
    *       4.������һЩMingW�����úͳ�������ı��뷽ʽ: http://www.gaia-gis.it/gaia-sins/mingw_how_to.html
    *
    *   yasm -- �������ص����ĸ��汾, �������Ϊ yasm.exe,���� bin ��
    ******************************************************************************************************************************/

    /******************************************************************************************************************************
    * Google Performance Tools(google-perftools)
    * 
    *******************************************************************************************************************************/

    /*******************************************************************************************************************************
    * Google Code(http://code.google.com)
    *  Project Hosting(��Ŀ�й�)-> Project Hosting on Google Code

    *******************************************************************************************************************************/


    /******************************************************************************************************************************
    * GUI Design Studio -- ���ԭ����ƹ��ߡ������������� AxureRP-Pro(��Ҫ����Web?),Balsamiq mockup,
    *   UIDesigner(��Ѷ���Ĵ���ԭ�����, �����ִ���ļ�)
    * ����
    *   ��Ļ���� -- ��ɫ���ο�
    *   ��Ԫ��(Representative Element) -- ��ʾ����ʼԪ��
    *   ���°�(StoryBoard)��� -- �ṩ���ڹ�������ĵ��п������̵�Ԫ����ʵ��һ��ģ��ԭ��
    * �������ͣ�
    *   Modal Popup -- ģ̬����
    *   Modal Choice Popup -- ��������������ⴰ�ڻ��Զ��رյ�ģ̬����, ������Ƕ�׼����˵�����������ʽѡ�񴰿�
    *   Show Window -- ��ģ̬����, �����ڲ����滻����
    *   Hide Window
    *   Toggle Window -- �л���ʾ
    *   Toggle Overlay -- ͬToggle Window, ��Ŀ�괰�ڲ��Ǳ�����¼�, ��������Ӧ��꾭���¼���ͬʱ, ��ť, ѡ�������Ԫ����Ȼ��Ҫ�����л������
    ******************************************************************************************************************************/

    /******************************************************************************************************************************
    * .pn9 Ŀ¼���д����ļ�, ʲô���ɵ�?
    *
    * CI�Ļ�������(C:\hudson\tools\apache-tomcat-5.5.35\bin)
    *   ANT_HOME=C:\hudson\tools\apache-ant-1.8.2
    *   classpath=.;%JAVA_HOME%\lib\tools.jar;%JAVA_HOME%\lib\dt.jar
    *   COVFILE=C:\Users\hudson\Documents\test.cov
    *   GIT_HOME=C:\Program Files (x86)\Git
    *   GTEST_HOME=C:\gtest-1.6.0
    *   HUDSON_HOME=C:\hudson\workspace
    *   JAVA_HOME=C:\Program Files\Java\jdk1.6.0_30
    *   MAVEN_HOME=C:\hudson\tools\apache-maven-2.2.1
    *   Path=C:\Program Files (x86)\BullseyeCoverage\bin;
    *        C:\hudson\util\klocwork\bin;C:\Program Files\Java\jdk1.6.0_30\bin;
    *        %MAVEN%\bin;%ANT%\bin;C:\Program Files (x86)\Cppcheck\;
    *        C:\hudson\util\nsiqcppstyle\nsiqcppstyle;C:\hudson\util\bullshtml;
    *        C:\hudson\tools\apache-ant-1.8.2\bin;
    *        C:\Program Files (x86)\Git\binC:\Program Files (x86)\PHP\;C:\Windows\system32;
    *        C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;
    *        c:\Program Files (x86)\Microsoft SQL Server\100\Tools\Binn\;
	         c:\Program Files\Microsoft SQL Server\100\Tools\Binn\;
    *        c:\Program Files\Microsoft SQL Server\100\DTS\Binn\;C:\Program Files (x86)\Git\cmd;
    *        C:\Program Files\TortoiseSVN\bin;C:\Python27;c:\Program Files (x86)\Microsoft SQL Server\90\Tools\binn\;
    *        C:\Program Files\Microsoft Windows Performance Toolkit\;
    *        C:\Program Files (x86)\Mozilla Firefox;C:\cygwin\bin;
    *   PHPRC=C:\Program Files (x86)\PHP\
    *   PROCESSOR_ARCHITECTURE=AMD64
    *   PROCESSOR_IDENTIFIER=Intel64 Family 6 Model 44 Stepping 2, GenuineIntel
    *   PSModulePath=%SystemRoot%\system32\WindowsPowerShell\v1.0\Modules\
    *   PYTHON_HOME=C:\Python27
    *   VS100COMNTOOLS=C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\Tools\
    *   VS90COMNTOOLS=C:\Program Files (x86)\Microsoft Visual Studio 9.0\Common7\Tools\
    *
    *
    * Hudson -- ��������(Continuous Integration)�Ĺ���,  �����ķ������У�CruiseControl ��
    *   NHN�İ�����http://devcafe.nhncorp.com/QPTalk/529396
    * ִ�����̣�Svn Update->Build(batch/shell)->PostBuild(����gtest���Ա����)
    * 
    * ���ذ�װ��
    *   1.http://jdk6.java.net/download.html, �����ذ�װJDK(6.0),���� JAVA_HOME 
    *   2.http://tomcat.apache.org/ <== 60, Binaray Distribution, Core zip, ��ѹ���� TOMCAT_HOME, 
    *     a. ���� conf\server.xml �ļ�, ��ÿ�� "<Connector port=" �������� URIEncoding="UTF-8" ������, �ɱ���Jobs���а�����ASCII�ַ�ʱ����
    *     b. ��ô��?
    *     JAVA_OPTS="$JAVA_OPTS -Xms2048m -Xmx2048m -Dfile.encoding=UTF-8 -server -Dnet.mdns.interface=0.0.0.0"
    *     JAVA_OPTS="$JAVA_OPTS -server -Xms256m -Xmx2048m -XX:PermSize=600m -XX:MaxPermSize=2048m -Dcom.sun.management.jmxremote"
    *     JAVA_OPTS="$JAVA_OPTS -DHUDSON_HOME=C:\hudson\workspace -Xmx2048m"
    *     CATALINA_OPTS="-DHUDSON_HOME=C:\hudson\workspace -Xmx2048m"
    *   3.http://hudson-ci.org/downloads/war/  <== �� 1.395�汾�Ժ��Ϊ hudson �� JeXXXX(http://jenkins-ci.org/) ? ������֧, Ŀǰ���µ��� 1.395.1
    *     �� hudson.war ���� TOMCAT �� webapps Ŀ¼��, Ҳ�ɽ�ѹ�� hudson ��Ŀ¼
    *     ���� HUDSON_HOME ������ָ���Ĺ���Ŀ¼(�������ص�Դ����б���), Ĭ��Ϊ CSIDL_APPDATA
    *   4.���ؽ�ѹ apache-maven ? ,  ���� MAVEN_HOME 
    *   5.��װ Python27, ���� PYTHON_HOME ��������
    *   ͨ�� startup.bat ����������Tomcat,Ȼ����� http://localhost:8080/hudson/,  ֹͣʱ�� shutdown.bat
    *     ������(ѡ��Jenkins�汾),  һ���Ȱ�װ��Ҫ��Plugins, Ȼ����ϵͳ�����н�������
    *     ע�⣺��Ҫ��Plugin Manager��Advanced����Ҫ�� Update Site��Ϊ�� http://updates.hudson-labs.org/update-center.json
    *           ԭ����Ĭ�ϵ�ַ�ǣ�http://updates.jenkins-ci.org/update-center.json
    *     a.ȫ��������, ����"Environment variables"�ļ�ֵ��
    *       gcovr -- ${PYTHON_HOME}\Scripts\gcovr (��Ҫ��ѹ gcovr-2.3.1.tar.gz, Ȼ�� scripts\gcovr ���� python��·����ŵ� Python27 ��)
    *       GTEST_HOME -- C:\gtest-1.6.0
    *       NSIQCPPSTYLEPATH -- C:\hudson\util\nsiqcppcheckstyle\nsiqcppcheckstyle  <== �ƺ�·������
    *       path -- C:\hudson\util\nsiqcppcheckstyle\nsiqcppcheckstyle;${path}      <== �ƺ�·������
    *
    * CI����(tomcat5.5.36,��װ��������5.5.35?)
    *   Build Triggers -- schedule �����α�ʾ ����/Сʱ/��/��/��
    *     Commit Build -- ��ѯ(��10����), �б䶯ʱ�����͵�Ԫ����, ��֤��ʱ���ֱ�������, X/10 * * * * ��ʾÿ10���Ӵ���һ��(ע��10ǰ�����Ǻ�*)
    *     Integration Build -- ��ʱ(��ÿ��),�������������ȫ�����(UT, ��̬, CC��), 30 0 * * 1-5 ��ʾ��һ������ÿ�� 0:30 �ִ��� 
    *     Publishing Build -- ���ɰ�װ�ļ�(ע��Ӧ��ͬʱ������ص�PDB)
    *   NHN Project Plugin(Assign NHN Project) -- ����NHN��Project�����ã�
    *     ��Ŀ���������淶?
    *   �������п��õ�ϵͳ����
    *     %BUILD_NUMBER% -- ��ǰ��������
    *     %WORKSPACE% -- ��ǰ��Ŀ�Ĺ����ռ�, �� C:\jenkins\workspace\jobs\XXXX_I\workspace
    *
    * �ļ�·��: ** ��ʾĿ¼����Ŀ¼, �� JUnit ����ļ�Ӧ������Ϊ xx/test_detail.xml ��ʾ������ǰĿ¼����Ŀ¼�еĸ��ļ�
    * 
    * ����Plugin(��ѡ����������ѡ�� Install, ���Զ����� *.hpi�ļ�����װ) -- http://wiki.hudson-ci.org/display/HUDSON/Plugins
    *   ��װ�Ժ����ͨ�� http://localhost:8080/hudson/restart ����(����Windows��������, ���������ɹ�)
    *   SimpleUpdateSite(NHN) -- ���ƻ���Plugin��������,http://devcode.nhncorp.com/projects/updatecenter/downoad,
    *     �� Advanced ��ѡ�� simpleupdatesite.hpi �ļ�, �ϴ���װ, Ȼ��������ã�
    *     New RSS URLΪ http://devcafe.nhncorp.com/QPTalk/notice/rss
    *     UpdateSiteURL Ϊ http://hudson02.nhncorp.com:9080/update/simpleupdatesite.json
    *   Publish JUnit test result report -- ���Բ鿴 JUnit/GTest���Ժ�� xml(�� gtestĬ�� ��test_detail.xml)
    *   locale <== ���ػ�(��ֹ������ʾ�к���, ������ʾӢ�ĵ�), ����Ϊ en_us ��ѡ��"Ignore XXX"
    *   E-mail Notification -- ���򼯳�������ʱ�����ʼ�֪ͨ����Ҫ�������ʼ���������,Ȼ�����е�ַ�Կո�����(��ͨ�� D0478@XXX �ķ�ʽ�����ʼ������)
    *     SMTP server:mexcn01.nhncorp.cn
    *     Default user e-mail suffix: @nhn.com
    *     System Admin E-mail Address: xxxx@nhn.com
    *   Quality Dashboard(NHN) -- ͨ��plugin�Ӹ���Hudson�������ϱ�ָ�굽ͳһ��QD������, ����ͳһ��չʾ��
    *     http://nsiq.nhncorp.com/, ÿ����Ŀ��Ψһ��API KEY��������
    *     ѡ�������ʱ��
    *       Analysis Defect Density ѡ�� klocwork
    *       Coding Standard Conformence Rate ѡ�� NSIQ cppstyle
    *       CC ѡ�� NSIQ Collector for Complexity
    *   JUnit test result report-- ���Բ鿴JUint, Gtest�����ɵĽ��XML(��GTest��Ӧ�� xx/test_detail.xml--ע:���Ǻ�)
    *   CovComplPort(NHN) -- ͬʱ��ʾ�Ƚϸ��ӶȺ͸����ʵĲ��,Խ���ӵ�ԽҪ����, Ŀǰֻ֧��Clover, 
    *     ���ʱΪ Publish Coverage/Complexity Scatter Plot
    *   analysis-core -- �����Ժ���ʾ Static Analysis Utilities, ��̬��������ĺ���(Core)���, Ҫ���о�̬����(���� klocwork/CheckStyle ��ʱ���밲װ��Plugin)
    *     http://wiki.jenkins-ci.org/x/CwDgAQ ,http://hudson-ci.org/download/plugins/analysis-core/ 
    *   checkstyle-- �������ʾ"Checkstyle Plug-in", �ռ�CheckStyle�ķ��������������ʾ
    *     http://wiki.jenkins-ci.org/x/GYCGAQ
    *   Clover plugin -- integrates Clover code coverage reports to Jenkins.
    *     http://wiki.jenkins-ci.org/display/JENKINS/Clover+Plugin
    *   MSBuild Plugin -- ֧��ʹ�� MSBuild ����Visual Studio�� .proj and .sln, ��Ҫ��������ָ�������Ϣ(Ȼ������Ŀ����ôʹ��?)
    *     Name: MSBuild, Path: C:\Windows\Microsoft.NET\Framework\v4.0.30319\MSBuild.exe
    *     Name: MSBuild_64, Path: C:\Windows\Microsoft.NET\Framework64\v4.0.30319\MSBuild.exe
    *     http://wiki.hudson-ci.org/display/HUDSON/MSBuild+Plugin
    *   MSTest plugin -- �� MSTest TRX���Ա����ʽת��ΪJUnit XML��ʽ
    *     http://wiki.hudson-ci.org/display/HUDSON/MSTest+Plugin
    * ��������ά�ȼ�ͳ�Ʋ�� -- �������õĴ�����ͳ�ƹ����� SourceCounter
    *   Coding Convention(����淶) -- N'SIQ CppStyle(http://nsiqcppstyle.appspot.com)
    *   Static Analysis(��̬����) -- klocwork
    *      1.ͨ�� kw-user-installer.9.2.2.10679.windows.exe ��װ�����Ȱ�װ�ͻ������;
    *      2.ͨ�� SimpleUpdateSite ��װ���, http://devcafe.nhncorp.com/QPTalk/205688
    *      3.Hudsonϵͳ���������������Ϣ
    *        Version:9.0, Path:�ͻ�������İ�װ·��
    *        Host:klocwork9.nhncorp.com, Port:27000
    *      4.������Ŀ������klocwork�ļ��
    *        Linux:����Config FileName Ϊ���������ű�����ɾ�������е�����, ����ظ�����?��
    *        Windows:����Config FileName Ϊ xxx.sln, Build Parameter ������ --config Release|Win32 ��
    *      ע�⣺ʵ��Ч������, ������߷ǳ���ʱ��(һ����ͨ�Ĺ��̺�ʱ40��������), �з����Ż���
    *   Code Coverage(���븲����,Ҫ�����>50%,��֧>40%) -- BullsEye/GCov, �ռ������� Clover/Cobertura
    *     ʵ�ֻ��������֣�1.���������(GCov); 2.����Դ���ٽ��б���(BullsEye Coverage)
    *     1.GCov(Linux) -- ��������ѡ���м���: -fprofie-arcs -ftest-coverage -lgcov
    *       ��Cobertura�ɼ����, ��������������Ҫ�� gcovr -r src --xml > coverage.xml ��������Ҫ���ļ�
    *     2.Bullseye Coverage(Win/Linux), BullseyeCoverage-8.4.2-Windows.exe�İ�װLicense : 0z6RNW3dGR0x0lVSwC0JzlmD4dE3Q4116NMw
    *       Ҫ���� COVBUILDZONE=${WORKSPACE}; COVFILE=${WORKSPACE}/test.gov,
    *       VS2008 �п���ͨ�� Tools->Enable/Disable Bullseye Coverage Build ����򿪻�ر��书��
    *       cov01 -1(��ѡ��); make clean all; cov01 -0(�ر�); ִ�в��Գ���; bullshtml target/coverage(��ָ��Ŀ¼�����ɸ����ʽ��);
    *       ��Clover�ɼ����, ��report directory ���� target/coverage, file name Ϊ clover.xml
    *       Bullseye ����ͨ�� COVFILE ��������ָ��ȫ�ֵĸ����ʷ����ļ�(��չ��Ϊ .gov )
    *       ע��:VS2008������Bullseye�Ժ��п��ܳ���"LNK1000: Internal error during LinkerMain", 
    *         ԭ��MS��bug(KB948127), http://www.bullseye.com/help/trouble-lnk1000.html ; 
    *         �����1.��װ����(�ƺ���Ч); 2.
    *   LOC/CC(Ȧ���Ӷ�,Ҫ��<30) -- N'SIQ Collector, ע����Ҫ�ų�����������, Lex/Yacc���Զ�����, Windows����Ϣӳ���
    *     http://dev.naver.com/projects/nsiqCollector, ���غ�������ʱ�����ִ���ļ�(nsiqcollector.exe)��·��, �е�����Build Step
    *   CQ -- Code Quality
    *   Code Duplication Analysis(�����ظ�����) -- CPD
    *   Defect density(ȱ���ܶ�,Ҫ�� < 4/KLOC)
    ******************************************************************************************************************************/

    /******************************************************************************************************************************
    * Intel C++��������9.0 ����߰汾�� -- ֧�� OpenMP* �淶 2.5 ��
    *   �Ż�������
    *   Linux	Windows	      ����
    *    -O0	  /Od	    ��ֹ�Ż�
    *    -g	      /Zi	    ���ɱ��
    *    -O1	  /O1	  �Ż������ƴ���
    *    -O2	  /O2	  �Ż��ٶȣ�Ĭ�ϣ�
    *    -O3	  /O3	   �Ż����ݻ���
    *   
    * 
    * Intel VTune ���ܷ����� 7.2 ����߰汾
    *   ��Ҫ���ܣ�
    *     a.ȡ������, ��������ͼ����, �������������Լ������Ľ���Intel��������;
    *       ȡ������ -- ���԰��������߷ֱ������������ʱ��ĺ�����ģ��, ����������ϵͳ��Ӧ�ó������ϸ��ͼ��
    *       ��������ͼ -- ͨ��������������ʱ��������ڵ�ͳ��ڵ�, ����һ�ŵ�������ͼ����ȷ������˳�����ʾ�ؼ�·����
    *       ����������� -- ������ʱ����ϵͳ�, ȷ���Ƿ����Ϊ�����ڴ���ٻ��ļ�����/����������������Ӧ�ó����ٶȱ�����
    *       Intel��������(ǿ���Ƽ�, �úÿ���) -- ���ݷḻ��֪ʶ��ֱ���������, �Զ��Ƽ�����Ľ��취, ������߿����ߵĹ���Ч�ʡ�
    *         ���ṩ�˻������ܼ���������ͼ, �Լ�����Windows����ϵͳ��Ӣ�ض����´������Ľ���, �Լ�Դ����ĸĽ����顣
    *     b.��Windows��ͬʱ�ṩͼ�λ��������н���, Linux �£�
    *     ����ʶ������̹߳���, ����������, ����������, ��ͨ��ʶ��Դ����ʶ������ƿ��, ʶ����Դ����, ʶ���Ч�߳���Ƶ�,
    *     ��VTune Performance Analyzer Essentials��
    * Intel Thread Checker �̼߳����2.2 ����߰汾
    *    �ɿ��ٲ��Һ��޸�Win*��OpenMP*�߳�����е�bug��
    *    ��س���ִ�й����е��߳���Ϊ, �������д��ڵľ�������, �߳������Լ�Ǳ�ڵ��߳���������, 
    *    ��ʾͬ�̴߳�����ص�Դ����λ��, ��Ȩ�����Լ���ջ���ٵȡ�
    * 
    * Intel Thread Profiler �̵߳����� 2.2 ����߰汾
    *    ���԰�������Win32*��OpenMP*�̻߳���������ܡ�
    *    ͨ����س��������������߳����ܵ��������, �����̹߳��غ�ͬ����ͻ, �ܹ��������Ҹ���ƽ��, ͬ���������߳��������⡣
    *    ���Խ��йؼ�·��������
    *
    * Intel ��ѧ���ĺ�����( MKL ) -- ��һ�׾����߶��Ż���, �̰߳�ȫ����ѧ�����⡣�������������
    *     a. ���Դ��� ����BLAS, LAPACK
    *     b. ScaLAPACK
    *     c. PARDISOϡ����������
    *     d. ��ɢ����Ҷ�任��Discrete Fourier transforms, DFTs��
    *     e. ʸ����ѧ�⣨Vector Math Library, VML��
    *     f. ʸ��ͳ�ƿ⣨Vector Statistical Library, VSL�����������������
    * 
    *******************************************************************************************************************************/

    /*******************************************************************************************************************************
    * Klocwork -- ������+�ͻ���, BS�ܹ�, �� Windows/linux/Unix �ȶ���汾
    *   ����ģ��
    *     Klocwork for C/C++ And Java
    *     Klocwork Architectural Analysis
    *     Klocwork Metrics and Trending -- ���������Ʒ���
    *     Klocwork Project Central -- �鿴���������Web����
    *   ����(server_name)
    *     lm(license������, �˿ں�27000) -- 
    *     tomcat(Web������, �˿ں�8068) -- 
    *     mysql(���ݿ������, �˿ں�3311) --
    *     rmimanager(����������ȷ�����, �˿ں�1103) -- 
    *   �����
    *     kwservice [check|start|stop] <projects_root> <server_name> -- �鿴|����|ֹͣ ����
    *     KMC -- 
    *   ���÷�����ȱ�ݺͰ�ȫ©�����ͣ�
    *     1.System configuration -> problems_default.pconf.xml -> Edit in advanced editor -> Klocwork Configuration Editor;
    *     2.<projects_root>\projects\workspace\rules �µ� metrics_default.mconf, usagerules_default.uconf ��
    *   ��Klocwork����C/C++��Ŀ(���ַ���)
    *     1.���� kwinforce �� kwinforce plugin ���� -- ͨ�����ڿ�����Ա���������з���һ���򼸸��ļ�
    *     2.��Source Root Directories���� -- ����ָ��Ŀ¼������ָ����׺��Դ�ļ������Ƽ������ֹ������ļ�����, ����ȷ��ͷ�ļ����õ���ȷ˳��
    *     3.��Build Specification Files���� -- ��ѡ��������ʵ�����˹����Ĺ��̺�����ѡ�
    *         VS20XX -- kwVcprojParser<vcproj/slnfile> -c <build_config> -o <spec_name>.out
    *         make -- kwinject-o <spec_name>.out <make/build batch cmmand>
    *       Ȼ��ʹ�� kwinject ���ɵ� .out �ļ���Ϊ����
    *     4.��Visual Studio Project,Solutionor Workspace���� -- VS������ѡ, ����Ϊ dsp/dsw/vcproj/sln ��
    *     5.��Tables���� -- ���м��ļ����з���, ������û�и���Դ��, �����ı���Ҫ������ȱ�ݺͰ�ȫ©�����͵ķ���, �ɽ�ʡ����ʱ��
    *   Options
    *     Generate problem filter������һ�������ļ��������ز�ϣ��������ȱ��
    *   ����ʧ�� -- ������Ϣ"parse error","syntax error","can not include"
    *   �������õ�֪ʶ�ⶨ��(*.kb �ļ�)
    *     BPS -- ����������������Ϣ����һ����������޸Ļ���������
    *     NPD -- ����һ�������ڲ�����NULL���������, �ᷢ����ָ������
    *     DMEM -- �����Լ����ڴ��������������η�����ͷ��ڴ棩
    *     FREE -- �����ͷ��ڴ�ĺ���
    *******************************************************************************************************************************/

    /******************************************************************************************************************************
    * LoadRunner -- HP��վ����, ֧��Win��Linux, ���ð�ֻ��10��
    *   9.5�ƽ�汾��License(δ����)��globa100 -- AEAMAUIKY-AFEKEKJJKEEA-BCJGI
    *                                 web10000 -- AEABEXFR-YTIEKEKJJMFKEKEKWBRAUNQJU-KBYGB
    *   
    *   Ԥ��ϵͳ��Ϊ�����ܵĸ��ز��Թ���, ͨ����ģ����ǧ���û�ʵʩ�������ؼ�ʵʱ���ܼ��ķ�ʽ��ȷ��
    * �Ͳ�������, LoadRunner �ܹ���������ҵ�ܹ����в��ԡ���Ҫ��������Ҫ������ɣ�
    *   Controller -- ��֯����û��Ĳ��Է���, �����Թ����Զ���
    *   VuGen(Virtual User Generator) -- ���� Vuser �ű�����Ҫ����, ����¼�ƺ����нű�, �ܼ��ش�����ϵͳ����
    *   Analysis -- ���������Թ�����, ��¼�����в����ĺ������ӵ�����, ������ͼ��
    *
    ******************************************************************************************************************************/

    /******************************************************************************************************************************
    * NuGet -- VS��չ, �ܼ���VS��Ŀ�����, ���º�ɾ����(����Ϊ�����)�Ĳ�����
    *   VS2012��ʹ��: 
    *     1.����NuGet: Tools -> Extensions and Updates -> Updates -> Visual Studio Gallery
    *     2.�½�C++����,  
    * 
    *   NuGet������Ǵ����һ���ļ����ļ���, ��չ���� .nupkg, ʹ�ÿ��Ŵ��Լ�� (OPC) ��ʽ
    *   NuGet�����, ���Դ����Nuget�ṩ�ķ�������, Ҳ���Դ���ڹ�˾�Խ��ķ�������, ����ֱ�Ӵ������ĿĿ¼��
    * 
    * ����C++��.nupkg ��, ��:
    *   http://blogs.msdn.com/b/vcblog/archive/2013/04/26/nuget-for-c.aspx 
    *   http://coapp.org/pages/tutorials.html
    *
    * ��NuGet�������������ĿĿ¼��
    *   http://beginor.github.io/2013/03/31/nuget-custom-package-repository-location.html
    ******************************************************************************************************************************/
 
	
    /******************************************************************************************************************************
    * PC-Lint(Windows)/FlexeLint(Linux) -- http://www.gimpel.com/ ,  ��̬�����⹤��,Ŀǰ��߰汾��9
    *   ���ص�ַ��http://download.csdn.net/source/2258242  8.x
    *             http://download.csdn.net/download/liuchang5/3005191  9.0e(������ pc_lint����.pdf)
    *   �� C/C++ Դ����о�̬�����Ĺ���, ��Ҫ���ָ��, ����Ƚ��з���, ���޷�����
    *   ����ʱ������߼�����֧�� VC, gcc, Intel C/C++ ��, ���Ա�һ���C/C++���������и�������������
    *   ����ƽ̨�� Windows, Dos, Linux�¿����� FlexeLint, ����������Բμ� gcc-readme.txt��
    *   ֧��16/32/64��ƽ̨����, ���Լ�ⵥ���ļ�����������
    *   ʹ��ǰ��Ҫͨ�� Config.exe �����������ļ�(std.lnt),���涨���˼���������Ϣ
    *
    * �������⼰�����ʽ:
    *   1."Error -- Unable to open include file 'xxxx' "
    *      ��Ҫͨ�� -i ָ��lint���ʱ��Ҫ�����Ŀ��ļ���·��(ϵͳ�Ŀ��Է��� std.lnt ������, ��Ŀ��صĿ��Է��� options.lnt ��)
    *   2.���θ��ֿ�(�� MFC/ATL ��)�ľ���(std.lnt �����õĻᱻoptions.lnt�е�ͬ�����ø���?)
    *     �� options.lnt �м��� -wlib(0)
    *   3.Ĭ�ϵ��������ܶ�, ���ѿ�������������(Ĭ��Ϊ -w3, ��� Error + Warning + Info + Note)
    *     �� options.lnt �м��� -w2  //ֻ��� Error + Warning
    *   4.Lint ��֧�� UTF16, ���Դ������ñ���ɴ�ǩ����UTF8��ʽ
    *   5.��ʹ�� dir ����Ҫ�����ļ��б�, Ȼ�� lint-nt [ѡ��] filelist.lnt �ķ�ʽ���м��
    *     �ļ��б��ļ������ɣ�dir d:\MyWork\*.cpp /s/b/a:-d > d:\pc-lint\filelist.lnt
    *
    * �ļ����ܣ�
    *   lint-nt.exe [ѡ��] [ѡ���ļ�(*.lnt)] �����ļ�(֧��ͨ���*.cpp)���ļ��б��ļ� 
    *     +ffn -- ����ļ�ȫ·����, ȷ����IDE�����п���˫�����ڲ�ͬ���ļ����л�
    *     -i -- ָ��lint���ʱ��Ҫ������·��
    *     -u -- ���е�Ԫ���, �������ģ�������澯
    *        ?? -u xxx.lnt -- ʹ��ָ����xxx.lnt�����ļ�
    *     -os(����ļ�·��)   
    *   
    * 
    *   LIN.bat -- ͨ�������ɵ��������ļ�, �ܰ����û�ָ��������(һ���� std.lnt, 
    *     �����ж��, ͨ�� _a �� _z �ĺ�׺����)����pclint, ���Էŵ�ϵͳ��Path·����
    *   LSET.BAT -- �ὫPC-lint��Ŀ¼���뵱ǰ%PATH%��������, ���˸о����罫Lint��Ŀ¼����PATH����
    *   std.lnt -- ͨ�������ɵ������ļ�, ���л���õ� .lnt �ļ���Ҫ��lntĿ¼������ͬһĿ¼��, ������ xxx ����
    *   options.lnt -- �������δ�����Ϣ�������ļ�, ���Դ򿪻�ر�ĳЩ����, �Ӷ����������ĳЩ����
    *     (�� if(b = fun()),  �����޷��Ÿ��Ƶ�)
    *
    *   au-xxx.lnt -- C++�ش����߽����ѡ��
    *   co-xxx.lnt -- �������澯����ѡ��������ļ�, �� gnu,mscX0,
    *   env-xxx.lnt --  ֧�ֶ�Ӧ��IDE������������Ҫ�������ļ�, ���Ұ����˶�ӦIDE���õ���ϸ˵��
    *   lib-xxx.lnt -- ���ֿ��ļ��澯����ѡ��������ļ�, ��stl, atl, mfc��
    *   xxx.h/xxx.c(pp) -- ����ʱ��Ҫ��ͷ�ļ���Դ�ļ�, ���������ļ�����һЩ���������еĹؼ��ֺ�����
    *     �÷�, ��һ������c/c++��׼, PC-lintΪ֧����Щ��׼, ͨ��ʹ����Щͷ�ļ���Դ�ļ�������б�ʶ�滻
    *     �����ļ����Ӧ�Ĵ���
    *   xxx.lob -- lint��������ʱ���м��ļ�(Lint Object Module)
    * �������ɵ�һ�� std.lnt (������VC2008)
    *   au-sm123.lnt au-ds.lnt                              //Effective C++, Dan Saks, MISRA 2004 ��
    *   co-msc90.lnt                                        //VC2008
    *   lib-atl.lnt lib-mfc.lnt lib-stl.lnt lib-w32.lnt     //���õĿ� ATL,MFC,STL,Win32 
    *   options.lnt  -si4 -sp4 -wlib(1) -w3                 //��ʹ���Զ����options.lnt�е�����
    *   -i"C:\Program Files\Microsoft SDKs\Windows\v6.0A\Include"
    *   -i"D:\Microsoft Visual Studio 9.0\VC\include"
    *   -i"D:\Microsoft Visual Studio 9.0\VC\atlmfc\include"
    *   -i"E:\FJSDK\Windows\FTL"
    * �Զ����options.lnt����
    *   -wlib(0)                    //���ο����ļ��ĸ澯
    *   
    *  
    * ��VC�Ļ�����ʹ��, ͨ�������ⲿ����ķ�ʽʹ��, ִ�м���ͨ����ݼ� F8 �ڸ�������֮���л�
    *   ��鵱ǰ���ڱ༭�ĵ����ļ���
    *     1.����Ϊ: PC-LINT File Check
    *     2.����Ϊ: D:\lint\lint-nt.exe
    *     3.����Ϊ: +v -i"D:\lint"  std.lnt env-vc9.lnt "$(ItemPath)"  -- �ƺ����Բ�Ҫ��env-vc9.lnt��?
    *     4.��ʼĿ¼Ϊ: "$(ItemDir)"
    *     5.ѡ�С�ʹ��������ڡ�, ���Զ�ѡ�С��˳�ʱ�رա�
    *   ��鵱ǰ���ڱ༭�Ĺ���, ������������������ص�.lnt -> ����.lnt
    *     TODO:ʵ�ⲻ��, �����д������, �����ļ��б������
    *     1.����Ϊ��PC-LINT Project Creation
    *     2.����Ϊ: D:\lint\lint-nt.exe
    *     3.����Ϊ: +v -os($(TargetName).lnt) $(ProjectFileName)
    *     4.��ʼĿ¼Ϊ: $(ProjectDir)
    *     5.ѡ�С�ʹ��������ڡ�, ���Զ�ѡ�С��˳�ʱ�رա�
    *     6.����Ϊ: PC-LINT Check Project, 
    *     7.����Ϊ: D:\lint\lint-nt.exe
    *     8.����Ϊ: +v -i"D:\lint" std.lnt env-vc9.lnt $(TargetName).lnt
    *     9.��ʼĿ¼Ϊ��$(ProjectDir)
    *     10.ѡ�С�ʹ��������ڡ�, ���Զ�ѡ�С��˳�ʱ�رա�
    *   ����һ����ν�� unit check (�ڴ����� Project.lnt ��), ������ʲô�ã�
    *     ������-i"D:\lint" std.lnt env-vc9.lnt --u $(TargetName).lnt $(ItemPath)
    *     ��ʼ��Ŀ¼��$(ProjectDir)
    *     
    * format ˵��(�� format=%f %l %t %n:%m )
    *   %f -- �ļ�
    *   %l -- �к�
    *
    * �澯����(���ָ澯����, ������ʱ��������, �����ڴ�����ʹ�������ע�͸�ʽ��������), 
    *   �������������εķ�ʽ, 
    *     //lint !e�澯��1 -e�澯��2 -- ���н�ֹ���ָ���ĸ澯, һ�������β,��ѡ���Կո�ֿ�
    *   ���лᱨ�澯��Ĵ���, �� -save �� -restore ����������, �Ͳ����ٱ�ָ���Ĵ���(�˴�Ϊ813)��
    *     /��lint -save -e813 ��/ ��Ҫ���Ĵ��� /��lint -restore ��/    -- ע�⣺����� �� ʵ����Ӧ���� *
    * options.lnt �е�����
    *   "-" -- ��ֹ�����Ӧ�Ĵ�����Ϣ
    *   "+" -- ���������Ӧ�Ĵ�����Ϣ
    *   "#" -- ����ʹ��ͨ��� "?" �� "*"
    *   -e(�澯��1,[�澯��2]) -- ��ֹ��һ�����ʽ�ĸ澯
    *   --e(�澯��1,�澯��2) -- �Ե�ǰ�������ʽ��ֹ����澯
    *   -e{�澯��} -- ���ʽ��������(����һ��������)
    *   -efile(�澯��,�ļ��б�) -- ��ָֹ�����ļ��Ը澯
    *   -efunc(�澯��,�������б�) --
    *   -elibsym(�澯��,�������б�) --
    *   -emacro(�澯��,����) -- ��չ��ʱ��ֹ�澯, �� -emacro(*,END_MSG_MAP)
    *   -esym(�澯��,������) -- �ɶ�ָ��������(������, ��������) ���и澯����, �� -esym(534, printf,scanf)
    *   -etemplate(�澯��) -- ģ��չ��ʱ��ֹ�澯
    *   -s�����ֽ��� -- ���÷��ŵ��ֽ���, �� -sb8(�ֽ�Ϊ8����),  -sc1(charΪ1�ֽ�), -sl4(longΪ4�ֽ�)
    *   -function(�Ƚϻ�������,�º���) -- ����ģ�£�����"�º���"���ú�"�Ƚϻ�������"��ͬ�ıȽ����ý��м��, ��
    *     -function(strcpy,myStrCpy) ��char buf[4]; myStrCpy(buf,"abcde"); �ر�419 �ľ���(bufԽ��)
    *   +libdir(���������ļ�Ŀ¼) -- ��ָ��Ŀ¼�µ��ļ�����Ϊlib, ֮������� -elib/-wlib �Ƚ�������
    *   -v -- verbosity 
    *   -wlib(�ȼ�) -- �Ժ������ļ�(��-i�а�����?)���ø澯����ȼ�(0�������Ϣ, 1+����,2+�澯, 3+������ʾ, 4������Ϣ)
    *   -w����ȼ� -- ���ø澯����ȼ�
    *
    * ������������(�ڽ�������� "error xxx" ), <�ظ�>, [�������], {�ɺ���}
    *   [ 19] -- Useless Declaration, ͨ�������������ŵķֺ�, һ���Ǻ걾�����˷ֺ�, ��ʹ��ʱ���ڽ�β���˷ֺ�
    *   { 40} -- Undeclared identifier 'xxx', 
    *   <415> -- access of out-of-bounds pointer, �������Խ��
    *   <423> -- Creation of memory leak in assignment to variable 'Symbol', �����б�����ָ�븳ֵ, ��������ڴ�й¶
    *   [429] -- Custodial pointer 'Symbol' (Location) has not been freed or returned
    *            ָ��û���ͷŻ򷵻�, ���ܳ����ڴ�й¶����Ҳ�����Ǽ������ض��������й���
    *   [438] -- Last value assigned to variable 'Symbol' not used
    *   [529] -- Symbol 'Symbol' (Location) not subsequently referenced, �����Ժ��û����ʹ��
    *            һ����԰ѱ�������͸���ɾ��(ע�⣺���ܼ򵥵ذ����д���ɾ��, �������Ͳ��������)
    *   <530> -- Symbol 'xxx' not initialized, ����δ��ʼ��
    *   <533> -- �������е�·���������˽����function 'Xxx' should return a value
    *   [534] -- Ignoring return value of function 'xxx', �����˺����ķ���ֵ
    *   {537} -- Repeated include file 'FileName', �ظ���������ͬ��ͷ�ļ�?
    *   <540> -- Excessive size, �ַ�����ʼ��ʱ��������Ļ�������С
    *   <604> -- Returning address of auto variable 'xxx', ���ؾֲ������ĵ�ַ
    *   <616> -- control flows into case/default, swtich...case ��û��break, ͨ���Ǵ���
    *   <644> -- Variable 'xxx' may not have been initialized, ��Ϊִ��·��������, ��������δ��ʼ��
    *   <661> -- possible access of out-of-bounds pointer, ���������, ����ʱԽ��
    *   {830} -- "Location cited in prior message"��ͨ������һ���ض��Ĵ������, ָ���ô���ָ��ĺ���ԭ��, ͨ��ֻ���ĵ��õĵط����ɡ�
    *   <1401>(���Ƶ���1402,1403��) -- ��Ա����δ�ڹ��캯���г�ʼ��, member 'Xxx' not initialized by constructor
    *     BUG? �����û�й��캯��, ���鲻����
    *   <1411> -- Member with different signature hides virtual member 'Symbol', ������ͬ����ͬ��, ������ǩ����ͬ�ĺ���
    *   <1509> -- base class destructor for class 'Name' is not virtual, ������ĸ���������������������
    *   <1510> -- base class 'Name' has no destructor, ��������������, ������û�ж�������, һ����Ҫ��������������
    *   <1516> -- Member declaration hides inherited member 'Symbol', ���ඨ���˺͸���ͬ���ֵĳ�Ա����,
    *             ע��:ͨ���궨�����Ϣӳ��Ȼ�����ͬ�ı�����
    *   <1540> -- pointer member 'Symbol' (Location) neither freed nor zero'ed by destructor. ָ�������������û���ͷŻ�ֵΪNULL
    *   <1541> -- member 'Symbol' (Location) possibly not initialized by constructor, ���캯���п���û�г�ʼ��ָ������
    *   [1551] -- function 'Symbol' may throw an exception in destructor 'Symbol', 
    *      �����������е��ÿ����׳��쳣�Ĵ���, ��Ҫͨ�� try...catch �����쳣
    *   [1579] -- Pointer member ��Symbol�� (Location) might have been freed by a separate function, ����ָ�����͵ı���û����������������ȷ�ͷ�(����ͨ���ڲ������ͷ���),
    *             �������� /x -sem( CMyClass::myCleanMethod, cleanup ) x/ ��ѡ�������ˡ�
    *   TODO: û�б��ʵ�����������±�; δ����ʼ���ı���; ʹ�ÿ�ָ��; �������
    *
    * �����Ĺ���
    *   ALOA(A Lint Output Analyzer ) -- �ɶԽ��ͳ�Ʋ�����
    *   lintProject -- ��Դ�Ĺ���, ���Խ� sln/vcproj �ȹ����ļ�ת���� �ļ��б��ļ�(lnt)
    *   RuleChecker -- �����ڴ������淶�ļ��, �������������ʽ, case�����д�淶, ���������Ͳ������ʽ�ı�д����
    ******************************************************************************************************************************/

    /******************************************************************************************************************************
    * Purify
    ******************************************************************************************************************************/

    /******************************************************************************************************************************
    * ReviewBoard -- Web Code Review Tool( http://www.reviewboard.org/screenshots )
    *   ���: http://code.google.com/p/reviewboardvsx/
    ******************************************************************************************************************************/

	/******************************************************************************************************************************
    * scons(http://www.scons.org/) -- ��Ҫ�Ȱ�װPython,��װ��Scons��, ��PATH�м��� scons.bat ����·��
    *   Pythonд�Ŀ�ƽ̨(Windows/Linux/Mac OS��)����һ���Զ�����������(���� GNU make, ������Ч�Ϳɿ�), ������������ NSIS/jsoncpp �ȵ�Դ��
    *   ��װ:
    *     Linux: python setup.py install -- ��Ҫ����ԱȨ��, �ƺ��� --install-lib|--install-dir ����, ������δ�ɹ�
    *   SConstruct -- python ��ʽ�ı���˵���ļ�,scons���Ը��ݸ��ļ��Զ����������ϵ���Ƶ����������ӵȹ���
    *   ����ʹ�ö��ε� SConscript(['��Ŀ¼��SConscript�ű�']) �ű��ļ��γɶ��Ŀ¼����
    *     Library|StaticLibrary('foo',['f1.c','f2.c','f3.c']) -- ���ļ�f1.c,f2.c��f3.c����ɾ�̬��libfoo.a
    *     Java(xxx) -- 
    *     Object -- ֻ�����Ŀ���ļ�(.o �� .obj)
    *     Program('hello', ['hello.c'], LIBS='m', LIBPATH=['/usr/lib','/usr/local/lib'],CCFLAGS='-DHELLO'); 
    *       --  ��hello.c(.cpp?) ����ɿ�ִ���ļ�hello, ����ʹ�õ��� libm.so|.a ��(�� -lm),
    *       �﷨Ϊ:Program( target='program',source=src_files )
    *     SharedLibrary('foo',['f1.c','f2.c','f3.c']) -- ����ɾ�̬��libfoo.so
    *   ����:
    *       LIBS=[�����ľ�̬, ��̬��]
    *       LIBPATH='../lib'  -- ָ������lib��·��
    *       CCFLAGS='-D_DEBUG' -- �������, �綨�� _DEBUG ��
    *       CPPPATH=['include', '/home/project/inc'] -- ͷ�ļ�����, ����ʱʹ�� -I
    *       CPPDEFINES -- �궨��?
    *       LINKFLAGS -- ���Ӳ���?
    *   �﷨(����ʹ�� Python ��׼�� import ���﷨)��
    *     ԭʼ�ļ��б�:
    *       1.ʹ������ķ�ʽ�г�������Ҫ�����Դ�ļ� -- �� ['f1.c','f2.c','f3.c'] �� Split('f1.c f2.c f3.c')
    *       2.ʹ�� Glob ������������ʽ -- �� Glob('*.c')
    *     �ڵ����(nodes)--SCons������֪�����ļ���Ŀ¼��ʾΪ�ڵ�,builder�����ķ���ֵ���ǽڵ����List,�������(+)
    *       File(['f1.c']) -- �����ļ��ڵ�
    *       Dir('classes') -- ����Ŀ¼�ڵ�
    *     ���ú���|����?
    *       Decider('MD5|timestamp-newer') -- ȷ���ļ��Ƿ��޸ĵķ�ʽ, Ĭ��Ϊ MD5
    *       Entry(xxx) -- ͳһ�Ľڵ����ʽ(�ļ�, Ŀ¼��)
    *       Environment -- ��������, ��Ҫ import os?
    *         Dictionary() -- ��ȡ�����������еļ�ֵ��, Ȼ���ͨ�� for key in [xxx] �ȷ�ʽ��ʾ
    *         Install('Ŀ�ĵ�','�ļ�')
    *       SetOption('������',����ֵ) -- 
    *     
    *     ��������:
    *        print "��Ϣ1", "��Ϣ2" -- �����Ϣ�ö��ŷָ�?
    *     ʾ��:
    *       1.�ж��ļ��Ƿ���ڣ�import os.path<CR> if not os.path.exists(program_name):<CR> print program_name, "does not exist!"
    *   scons �������
    *     -c|--clean -- ִ����������
    *     --debug=TYPE -- ��ӡ������Ϣ, ��ѡ���� includes,objects,pdb ��
    *     -h|--help -- ��ӡ������Ϣ
    *     -j N -- ͬʱִ��N��Job
    *     -Q -- ����"Reading/Building"�ȵĽ�����Ϣ
    ******************************************************************************************************************************/

	/******************************************************************************************************************************
    * �汾���ƹ��� SVN��CVS�����, ���ڶ������ļ�ֻ�������һ�汾��֮ͬ����, SVNһ��������Ŀ¼��branches, tgs, trunk
    *   ������(Subversion/VisualSVN-Server ��)  -- http://www.visualsvn.com/downloads/( http://subversion.tigris.org/)
    *     http://www.collab.net/downloads/subversion
    *     �Դ������е� svn.exe ��Ϊ�ͻ���
    *   �ͻ���(TortoiseSVN) -- http://tortoisesvn.net/downloads
    *   VS ���(AnkhSvn)    -- http://ankhsvn.open.collab.net/
    * 
    * ȫ�������ļ��� ~/.subversion/config
    *   global-ignores -- ����ȫ�ֹ����б�(Ŀ¼����չ����)
    * 
    * ��������
    *   SVN_EDITOR <==
    *   VISUAL, EDITOR
    *
    * svnadmin.exe 
    *   create �յ�Ŀ¼�� -- �����汾��, �ȼ���TortoiseSVN�� �Ҽ�->TortoiseSVN->Create Repository here
    * svnserve.exe <== ���������ڶ˿�3690�ȴ�����
    *   --daemon : ���ػ����̷�ʽ����, �ֶ���ֹ֮ǰ�����˳�
    *   --root ����Ŀ¼��: ���ø�λ�������Ʒ������ķ���Ŀ¼
    *   ������ʽ����svn��
    *     set CUR_PATH=%~dp0�� svnserve --daemon --root %CUR_PATH% 
    *   ��������(��������, ע��ÿһ���Ⱥź��涼Ҫ��һ���ո�, ����ᱨ��)��
    *     sc create SVN binpath= "E:\Subversion\bin\svnserve.exe --service --root E:\fishjam\Code_SVN" displayname= "SVN Service" depend= Tcpip start= auto
    *
    * �����ļ�(�汾��Ŀ¼��conf��Ŀ¼��)
    *   svnserve.conf
    *     password-db=passwd -- 
    *   passwd
    *     [users] -- ���ÿһ�ж���һ���û�, ���� �û���=���� �ķ�ʽ
    *
    * SVN��������( svn.exe )
    *   add �����ļ���Ŀ¼ <== ����
    *   blame �ļ� <== �鿴�ļ����޶������ߵ���Ϣ��ÿ�е�����޸��κΰ汾��, ��ͨ�� | sort �鿴�����޸�
    *   checkout https://xxxxx ����·�� --username �û���  <== �����
    *   cleanup <== ����������������������Ĵ���, ��������ô�����ģ��Ƿ�������
    *   copy SRC DST <== ��<-->���� �� ����֮�俽����ע����֧�ֿ�֮�䣩
    *   export SRC DST <== �ɴӿ�, �����޸��е��������汾������Ϣ�Ľṹ
    *   import PATH URL <== ���ް汾���ļ��������絽��Ŀ�洢����
    *   info <== �鿴��·����Ϣ
    *   list <== �г����е�����(��ǰĿ¼�µ��ļ���Ŀ¼)
    *   log <== ������ʷ
    *   merge <== �Կ��е����°汾���ļ��ϲ�������������
    *   resolved �ļ� <== �ѽ����ͻ
    *   status(st) <== �鿴���ص��ļ��޸����, ͨ������ svn st | grep ^[^?] �ķ�ʽ�鿴����
    *   switch <== ���¸�����URL
    *   proplist Ŀ¼ -R <== �г�ָ��Ŀ¼�����е�����(�� svn:executable)
    *   propedit ������(�� propedit) �汾·�� [ --editor-cmd notepad ]  
    *     <== ʹ�� notepad ��������(���������SVN_EDITOR ���������Ļ�, ������ָ���ı༭�����б༭���� )
    *   ��������( propset )
    *       svn:externals "[-r�汾] ������URL��ַ ������"  <== ʵ���ļ�, Ŀ¼����
    *         ע��, URL�еĿո������%20����, �������Ŀո������˫���š�"^/"�����URL, ʾ��:
    *         ����·����ʽ: swanfalconapp https://....    (TODO: ��Ҫȷ��)
    *         ���·����ʽ: ^/trunk/Temp/FJDriverDemo/InlineHook.h Temp/nComic/ComicHelper/InlineHook.h
    *       svn:executable <== ���ÿ�ִ������(����linux��so�ȣ�)
    *       svn:ignore  "*.obj *.exe *.bak" Ҫ���õ�Ŀ¼(��.) <== ����SVN���Ե��ļ�(����ͨ�� * ָ���������չ��)
    *    update <== ���´���, ���ܻᷢ����ͻ
    *    del --keep-local MyConfig.xml  <== �������ظ���, ÿ���в�ͬ�ĸ�������δ�����Ƿ����У�
    * 
    * ͨ��SVN�������, �����ñ��ش���Ŀ¼--ʹ�ñ��ص� Local Repository ��Ϊ����, ��ת��������������
    *   1.svnadmin create ���زֿ�Ŀ¼ <== 
    *   2.svn co file:///���زֿ�Ŀ¼ . <== �������ֿ�Ŀ¼checkout����ʱĿ¼��, ֮����һ����������
    *   3.������Ҫ��Ŀ¼�ṹ, ������Ҫ���ļ�, Ȼ��ʹ�� svn add ����;
    *   4.$SVN_EDITOR=gedit svn pe svn:externals ������ <== ���� svn ����������
    *     �ڵ����� gedit �������ⲿ���ӵ�·�����ⲿ������ SVN��ַ, ��:
    *   5.svn commit -m "ע��" <== ���Ѿ����úõı���Ŀ¼�ṹ commit �� ���ص� Local Repository ��
    *   6.Eclipse ����� Resposity location, ��ʹ�� URL ָ�򱾵ص� Local Repository, Checkout��
    *     �Ϳ���ֱ����Eclipse�й��������
    *
    * ��ͻ���
    *   p(ostpone) <== �Ƴ�, �ϲ����벢���û����, ��ͻ�ĵط����� <=== �ȷָ[Windows�µ�Ĭ�Ͻ����ʽ]
    *   mc(�ҵİ汾)
    *   tc(���˵İ汾)
    *   df(diff-full)��ʾȫ������
    *   edit <== �༭, ����Ҫ���� SVN_EDITOR �Ȼ�������
    *   r(esolved) <== �ѽ��
    *   dc()
    *   mf <== ȫ���ҵ��ļ�
    *   tf <== ȫ�����ǵ��ļ�(��tc������)
    ******************************************************************************************************************************/
    
    /******************************************************************************************************************************
    * UltraEdit(�����滻ʱ��������ʽ), �����֣�P(erl), U(nix), (Ultra)E(dit),  L(ex,ͨ��ͬU)
    *   ��string              L<== ֱ������string
    *   ��"a+b"��(����)��   U<== "a+b"
    *   ת���ַ���          U<== \              E<== ^
    *   �滻���ʽ(����)��  U<== (KEYWORD)      E<== ^(KEYWORD^)
    *   �滻��ǩ(1-9)��     U<== \n             E<== ^n
    *   �߼���(����A��B)��  U<== (A|B)          E<== ^{A^}^{B^} 
    *   ab��1�λ��Σ�     U<== (ab)+
    *   ���ף�              U<== ^              E<== %
    *   ��β(r$�ȼ���r/\n)��U<== $              E<== $
    *   ���������ַ���                          E<== *
    *   ǰ�ַ�0��1�Σ�      U<== ?
    *   ǰ�ַ�0���Σ�     U<== *              E<== ++
    *   ǰ�ַ�1���Σ�     U<== +              E<== +
    *   ǰ�ַ�N��M�Σ�      U<== \{N, M\}
    *   ���ִ���A����N��M)��  L<== A{N,M}   <==˵����Lex���﷨, �� [a-z]{5,} ��ʾ���ȴ���5�� a-z ��ɵ��ַ�
    *   ��\n���κε��ַ���  U<== .              E<== ?
    *   ��ҳ����            U<== \f             E<== ^b
    *   0D0A(DOS)��         U<== \p             E<== ^p
    *   0D(MAC)��           U<== \r             E<== ^r
    *   ���з�OA(UNIX)��    U<== \n             E<== ^n
    *   �ո��Ʊ�          U<== \s
    *   �ǿ��ַ���          U<== \S
    *   �Ʊ�(TAB)��         U<== \t             E<== ^t
    *   ��ֱ�Ʊ�          U<== \v
    *   �»��߻��ַ���      U<== \w
    *   ���ַ���            U<== \W
    *   ��Χ(��Сд��ĸ)    U<== [a-z]          E<== [a-z]
    *   �ų�(��0-9)��       U<== [^0-9]         E<== [~0-9]
    *     ��ab��������ַ�   <== [^ab]
    *   ��ǰƥ��(β�����)��   L<== r/s ƥ��������s��r���� ABC/D ƥ�� ABCD ����ƥ�� ABCE, ���ҵ���ģʽΪ ABC
    * ע�⣺
    *   *��+ ֻƥ��֮ǰ�����Ԫ��, ������С���Ž����Ԫ��������, ��ͨ����Ϳ���ƥ���������ŵ�������
    *   "^" ���ڷ����ŵ��ײ���ʾ"����"����˼, �������ط�û���ر�����
    *   "|"���ڷ������б�ʾ�����ߡ�, �ڷ�������ֻ��ʾ"|" ����
    * ���ӣ�
    *   1.���� create poa ok �� create orb ok, Ȼ���򣨼�Ϊ ok poa create��
    *     Unix<== (create) (orb|poa) (ok), �滻�ַ����� \3 \2 \1  -- ע�⣺��BUG
    *     UltraEdit<== ^(create^) ^(^{orb^}^{poa^}^) ^(ok^),  �滻�ַ�����^3 ^2 ^1
    ******************************************************************************************************************************/

    /******************************************************************************************************************************
    * ��Ԫ����(Unit Testing) --
    *   �쳣����--ȷ�ϴ���Ľṹ�ɿ���, �ܴ������п��е������Լ��������, �������Ԥ��֮����쳣��
    *   ���ܲ���--��֤������ģ���Ƿ��������͹����Ƿ�����������
    *   �ع����--��֤���д�����Ϊ�������Ŵ����Ľ�������,�ڴ������֮������, �Բ�����Ĵ�����Ϊ�е�ƫ��
    *
    ******************************************************************************************************************************/

    /******************************************************************************************************************************
    * Visual Unit -- �����ĵ�Ԫ���Թ���
    *   
    ******************************************************************************************************************************/

    /******************************************************************************************************************************
    * Visual Studio
    *   �༭ UTF-8��ʽ���ĵ���Linuxͬ����������
    *     Options->Text Editor->General->Auto-detect UTF-8 encoding without signature
    * 
    * ���������� ":a+" ��ѡ��"������ʽ", �������� �ĵ���Ӳ������ַ���
    * ����������ʽȥ�� .ini �ļ��� = ǰ��Ŀո��Tab�� ���ң� ^{[^ \t=]*}{[ \t]*}={[ \t]*}{.*}$ , �滻�ɣ�\1=\4 
    * ���������VS2010�е�.vcxproj.filters�ļ����ù���Ŀ¼(EmEditor��):
    *   ����: <(\w+) Include=\"(((\w*\\)*).*)\"\sx/>$           <== ע��: ���� "sx/>$" ���е� x Ӧ���� *
    *   �滻: <\1 Include=\"\2">\n        <Filter>\3</Filter>\n    </\1> 
    *   EditPlus�е��﷨(֧������): <(?<tag>\w+) Include=\"(?<wholename>(?<path>(\w*\\)*).*)\"\sx/>$
    *                                       <$+{tag} Include=\"$+{wholename}\">\n        <Filter>$+{path}</Filter>\n    </$+{tag}>
    *   TODO:Ŀǰ���滻�������һ�� "\" ����
    *
    * Visual Studio Team System(VSTS)
    *   ���ù��ߺ͹��̹�����
    * 
    * Visual Studio Team Explorer(����, ���� CodePlex �ϴ���Ĺ���)
    * 
    * Profiler(���ܷ������� -- ��ҵ���в���): 
    *   Tools->Performance Tools->Performance Wizard
    *  
    * ���뾲̬���
    *   Analyze -> Run Code Analysis On Only
    *
    * ��������, ���Ӿ���ʹ���
    *   C1859 -- unexpected precompiled header error, ΢���BUG, http://code.msdn.microsoft.com/KB976656, TODO:�ƺ��������ó���Ҳ�ᱨ�����
    *   C4005 -- ���ض���, macro redefinition
    *   C4101 -- δ���õı��ر��� unreferenced local variable
    *   C4390 -- �տ������(������if��ֱ�Ӹ��˷ֺ�),empty controlled statement found; is this the intent
    *   C4715 -- �������е�·�����з���ֵ, not all control paths return a value
    *   C4930 -- ������������﷨д���˺����������﷨��prototyped function not called (was a variable definition intended?)
    *   C4065 -- switch ���������default����û��case��switch statement contains 'default' but no 'case' labels
    *   D9038 -- /ZI is not supported on this platform; enabling /Zi instead
    *     ���: �� x86 ֧�� /ZI, �����x64�����þͻᱨ���������: C/C++ => General =>Debug Information Format
    *   LNK2026:module unsafe for SAFESEH image -- VS2012�������ļ�ʱ, ����Ĵ���, /SAFESEH
    *     �����(�Ƽ�)ʹ�� ml(64).exe ����ʱ, ����ָ�� /safeseh ����(set the Use Safe Exception Handler MASM property to Yes)
    *            ��    ������������ѡ��Command������ /SAFESEH:NO
    *   LNK4222: exported symbol 'DllCanUnloadNow' should not be assigned an ordinal
    *     ���: DllCanUnloadNow �Ⱥ�������ͨ����ŵ���, ɾ�� .def �ж�Ӧ����ż���
    * ���д���:
    *   64λ����������32λ�ĳ���ʱ,����0xc000007b(STATUS_INVALID_IMAGE_FORMAT) -- 
    * 
    * �����ϵ㣺��ʽΪ ???  -- 
    *   {,,USER32.DLL}_MessageBeep@4 ��Ӧ WinDbg�е� bp USER32!MessageBeep
    *   TODO:Ϊʲô�еĺ������ò����������ҹ�����ʲô��@4 ��Ӧʲô -- ����ͨ�� dumpbin �鿴�����ĺ���, ��ȷ���Ƿ������öϵ�
    ******************************************************************************************************************************/

    /******************************************************************************************************************************
    * VC++Ĭ�������, ���԰汾�д����쳣, �����汾�в��������� try {*NULL = 100;} catch(...){����ֻ�� Debug ����Ч} 
    * ͬ���쳣ģ��(/Gx �ı�������), ������첽�쳣ģ��, �κδ���ָ�������쳣(/Eha)
    * C++�쳣ֻ���� throw ����׳�, �ṹ���쳣(���� __try ... __except ����) 
    * 
    * 
    * C++ �쳣�ͽṹ���쳣�ıȽ� -- P139
    * VC++ ��֧���쳣�淶(C4290) -- ����֧���� throw �������������׳����쳣����
    *
    * ���������ã�
    *   1.��֤���а汾(���������汾)�ж����е��Է���PDB�ļ���
    *     �Ż���ķ����汾����ʼ���ܹ�ƥ��Դ�����ִ����, ���׽��е������٣�
    *     �Զ�ջ�Ĵ����������Ż�, �޷����������ĵ��ö�ջ��Ϣ�����ڻ����ļ�����ӱ�ʶPDB�ļ��ĵ�����
    *       (section)�ַ���, ����΢�����ļ��Ĵ�С��
    *       a..NET(C#/VB) �У�   Build -> Generate Debugging Infomation => True
    *       b.��������(C/C++)�У�C/C++ -> General -> Debug Information Format => Program Database(/Zi)
    *         Linker ->General -> Enable Incremental Linking => No(/INCREMENTAL:NO), ���������ӿ�����ΪNo, 
    *         Linker -> Debugging -> Generate Debug Info => Yes(/DEBUG),���������Ϣ, ��ʹ���������Զ���
    *           �����к���, ���������Ƿ����ã���Ҫ /OPT:REF ɾ����
    *         Linker -> Debugging -> Generate Program Database File => $(OutDir)/$(ProjectName).pdb, 
    *         Linker -> Debugging -> Strip Private Symbols => $(OutDir)/$(ProjectName)_Stripped.pdb, 
    *         Linker -> Optimization -> References => Eliminate Unreferenced Data(/OPT:REF), ֻ������Щ������ֱ�ӵ��õĺ���
    *         Linker -> Optimization -> Enable COMDAT Folding => Remove Redundant COMDATs(OPT:ICF), ��Ҫ��ʱ��
    *           �ϲ���ͬ������COMDAT��¼, ʹ����������ĳ����ֵ�ĵط�ֻ��ʹ��Ψһ�ĳ������ݱ�����
    *   2.���йܴ����еı��뾯����Ϊ�������/WARASERROR+��, ���󲿷ֱ�������ı��뾯����Ϊ�������
    *
    *******************************************************************************************************************************/

    /******************************************************************************************************************************
    * Visual Assist -- VC����(���ƵĻ��� DPack / Resharper ��)
    * 1.���Ļ����Ĭ��λ��( %APPDATA%\Visual Assist )
    *   HKCU\Software\Whole Tomato ��������Ϊ "UserDataDir" ���ַ�����, ��ֵΪĿ¼, ��"F:\Visual Assist X\Cache"
    * 1.AutoText �еĺ� �� ������������ϵͳ�ж��壩
    *   %FILE%              Full filename with path 
    *   %FILE_PATH%         Path of file
    *   %FILE_BASE%         Filename without path or extension
    *   %FILE_EXT%          Filename extension
    *   %DATE%              Year/month/day formatted as %04d/%02d/%02d
    *   %YEAR%              Year formatted as %d
    *   %MONTH%             Month formatted as %d
    *   %MONTH_02%          Month formatted as %02d
    *   %MONTHNAME%         Three-character abbreviation of month
    *   %DAY%               Day of month formatted as %d
    *   %DAY_02%            Day of month formatted as %02d
    *   %DAYNAME%           Three-character abbreviation of day
    *   %HOUR%              Hour formatted as %d
    *   %HOUR_02%           Hour formatted as %02d
    *   %MINUTE%            Minute formatted as %02d
    *   %SECOND%            Second formatted as %02d
    *   %%                  %
    *   %USER_NAME%         USERNAME
    *   %0                  Current selection
    *   %1 through %8       User input field
    ******************************************************************************************************************************/

}

#endif //FTL_IDE_TOOLS_H

//#ifndef USE_EXPORT
//#  include "ftlIDETools.hpp"
//#endif
