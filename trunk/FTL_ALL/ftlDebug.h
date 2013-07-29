#ifndef FTL_DEBUG_H
#define FTL_DEBUG_H

#pragma once

namespace FTL
{
    //WinDBG�÷����.pdf -- P19,  30.5.7 Զ�̵���
    /*********************************************************************************************************
    * TODO: 
    *  1.˫���ں˵��� �� �����ں˵���
    *  2.alias -- 
    *  3.ͨ�������ں˵��ԶԻ��������ں˵�����������
    *  4.�����е�ע�ͣ� * �� $$ -- ʹ��ǰӦ����ǰһ���������Ϸֺ���Ϊ�ָ�
    *  5.poi ��ʲô�� "j (poi(MyVar)>5) ' -- J �����жϱ���MyVar ��ֵ�Ƿ����5��
	*  6.�������û�̬��ת���ļ�ʱ������ʹ��.ecxr ���ת���ļ��б�����쳣����������Ϊ�Ĵ��������ġ�
	*  7.VC ������ȱʡ�����ͷ��ŷ���VCx0.PDB �ļ��У�WinDBG���ܺܺõĴ��������ʾ�ֲ�����ʱ������ʾ�ܶ�no type information ����
	*    ��������������Ÿ�ʽ����ΪC7 Compatable��Settings>C++ > General > Debug Info��
	*  8.���û�������_NT_SYMBOL_PATH ��_NT_ALT_SYMBOL_PATH
	*  9.WinDBG ȱʡʹ����ν������ʽ���ż��ز��ԣ�ģ�����ʱ������ط��ţ���ʾΪ deferred(�ӳټ���)��
	*    ͨ�� .reload ǿ��ȫ�����ػ� .load �ֶ����� ��
    *********************************************************************************************************/

    /*********************************************************************************************************
    * OllyDbg -- ǿ��Ĵ�������������޷��ź�Դ��ʱҲ�ܺܺõĽ��е���(���Գ���������ƽ⡢�������������򹤳�)
    * WinDbg  -- ������������������GUI����ƫ������Դ��ʱ���ԱȽ����ѣ�֧��dump���ԣ���������ǿ��
    *            ͨ�� SOS.dll ֧��.NET���ԣ����и߼����ԱȽϷ���
    * 
	* WinDbg(http://msdn.microsoft.com/en-us/windows/hardware/gg463009)
    *   ֧�ֽű��Ͳ������������������ṩ�˴����ǳ����õĲ�����ɱ�д������չģ�������ƺͲ�����Թ���
    *   .hh �ؼ��� -- �򿪰����ĵ�����λ��ָ���ؼ���
	*
    * �����ռ�(Workspace) -- �����ʹ洢һ��������Ŀ�����ԡ��������Լ����������õ���Ϣ������IDE�е���Ŀ�ļ���
    *   ������Ϣ������ HKCU\Software\Microsoft\Windbg\Workspaces��һ������ĸ��Ӽ����ֱ��Ӧ���಻ͬ�Ĺ����ռ䡣
    *     User(�û�̬)
    *     Kernel(�ں�̬)
    *     Dump(ת���ļ�)
    *     Explicit(�û����Ϊ����ʽ�����ռ�)
    *
    * ����(Themes) -- WinDbg����Ŀ¼�е�Themes��Ŀ¼�а��������־������ƵĹ����ռ�����(��Ҫ�Ǵ��ڲ���)����ͨ����.reg����ע����ʹ�á�
    * ����(Alias) -- �����ڼ�д������д�������ȣ�
    *   ������(��ͨ�� .echo �鿴)��
    *     �̶����Ʊ���( $u0 ~ $u9 ) -- �޸ķ�ʽ: r $.u<0~9>=<����ʵ��>�� �� r $.u9=nt!KiServiceTable
    *     WinDBG �Զ�����ı��� -- 
    *       $ntnsym(NT �ں˻���NT DLL�ķ�����), 
    *       $ntwsym(��64λϵͳ�ϵ���32λĿ��ʱ��NTϵͳDLL������), 
    *       $ntsym(�뵱ǰ����Ŀ��Ļ���ģʽƥ���NT ģ������)
    *     �û��������� -- �޸ķ�ʽ��as �������� ����ʵ�塣 ���÷�ʽ��${�û�����} �� ֱ������
    * ��־�ļ�(LogFile) -- ���԰����������������ִ�н����¼һ���ı��ļ�(��־�ļ�)�С�
    * ������:
	*   �Ự(Session)������ -- ֻ���ں˵���ʱ��������, !session ��չ����, -s n ����, !sprocess -- �г�ĳ���Ự�����н���
	*   ����(Process)������ -- ͨ�� .process ��ʾ���л� ?
	*   �Ĵ���(Register)������ -- �Ĵ����д�ŵ��ǵ�ǰ����ִ���̵߳ļĴ���ֵ��ͨ�� .thread ��ʾ�������üĴ�������������Ե��߳�
    *   �ֲ�(Local)������ -- �ֲ����������ڵ��ﾳ���漰����ջ
	*  
    * ����������(������༭�������� "." �� "!" Ȼ�� Tab������˳����ʾ���ֿ��õ����� )��
    *   ��׼���� -- һ�����ַ����߷��ţ������ṩ�����ڸ��ֵ���Ŀ�����������Թ���
    *   Ԫ����(Meta-Command���Ե�"."��ʼ) -- �ṩ��׼����û���ṩ�ĵ��Թ���
    *   ��չ����(Extension Command���Ը�̾��"!"��ʼ) -- ������չĳһ����ĵ��Թ��ܣ���ʵ���ڶ�̬���ص���չģ��(DLL)�ļ��еģ�
    *     ���� w2kchk/w2kfre/winext/winxp �ȸ�����Ŀ¼�У���ͨ����SDK�ɱ�д�Լ�����չģ�顣
    *     ������Ŀ�걻����ʱ��WinDBG ����ݵ���Ŀ������ͺ͵�ǰ�Ĺ����ռ䣬�Զ�����ĳЩ��չģ�顣Ҳ��ͨ�� .load Ԫ����������
    * 
    * ��������(���ж�״̬�£�ͨ��F9����������/ȡ���ϵ�), ֱ�������ʺ� "?" ������ʾ��һ����׼������б�ͼ򵥽���
    *   ~ -- ��ʾ���л��߳�
    *     ~<thread_index> s -- �л���ǰ�̣߳� �� "~6 s" �л�����6���߳�
    *   | -- ��ʾ����
    *     |<process_index> s -- �л���ǰϵͳ�еĵ�ǰ����
    *     ||<n> s -- ����ǰϵͳ�л���<n>�ţ���ͬʱ���Զ��ϵͳ(�� ���ء�Զ�̡�dump��)ʱʹ��
    *   ? -- ��ʾ���ʽ
    *   ?? -- ��ʾC++���ʽ
    *   $ -- ִ�������ļ�
    *   a -- ���
    *   al -- �г�Ŀǰ����������û���������
    *   ad -- ɾ��ָ����ȫ��(ad *) �û�����
    *   as �������� ����ʵ�� -- ��������޸��û������������� as SST nt!KiServiceTable
    *   b -- �ϵ�(F9 �����ڹ������Դ�����趨�ϵ㣿)��bu ���õĶϵ�ᱣ����Workspace��?
    *     ba -- Ӳ���ϵ�
    *     bc/bd/be -- ���/ ����/����  �ϵ㣬�� bc 1 �� bc *
    *     bl -- �г����жϵ�
	*     bu -- ��û�м��ط��ű��ģ��ʹ���ӳ����öϵ㣬�����ڵ��Զ�̬����ģ�����ں������߳�ʼ�����롣
	*        �� bu HelloWDM!DriveEntry
    *     bp [Options] [Address [Passes]] ["CommandString"] -- ��ָ�����������ַ�����öϵ㣬
	*        �� bp HelloWDM!DriveEntry 
	*           bp MSVCR80D!printf+3 2 "kv;da poi(ebp+8)" -- ��printf ƫ��3�ĵ�ַ���öϵ㣬�ڶ���ʱ���жϣ����Զ�ִ��ָ�����
	*              da poi(ebp+8)������ʾprintf �ĵ�һ��������ָ�����ַ���������ƫ��3��ԭ����ڴ���ջ֡��������ִ�кú�ebp+8����ָ���һ������
	*           bp my.cpp:122 100
    *   dds ��ַ -- ����ʾָ��λ�õĺ�����?
    *   d �۲��ڴ�����, ��ͨ�� ln ָ������Ϊn, 
    *     dd -- ���� DWORD �ķ�ʽ��ʾ�� ���Ƶ��� du(Unicode Char?)
    *     dg -- ��ʾ��ѡ����
    *     dt Xxxx -- ��ʾ�������ݣ� �� dt _EPROCESS
    *     dv -- ��ʾ���������;ֲ���������Ϣ(Ctrl+Alt+V �л�������ϸ����ʾģʽ)��ע�⣺�Ż����Ĵ�����ʾ���ܲ�׼ȷ��
    *   e -- �༭�ڴ�����
    *   g -- �ָ�����
	*     gu -- ִ�е�����������
    *   j <�������ʽ> [Command1>] ; [Command2>] -- �ж�һ��������Ȼ��ѡ���Ե�ִ�к����������� if..else�����õ����Ű���һ������
    *     ���� r ecx; j (ecx<2) 'r ecx';'r eax' ��ʾ����ʾ�Ĵ���ecx ��ֵ��Ȼ��ִ��j�����ж�ecx�Ƿ�С��2��ִ�в�ͬ���֡�
    *          �ȼ��ڣ�r ecx; .if (ecx>2) {r ecx} .else {r eax}
    *   k -- �۲���ö�ջ
    *     k/kb/kn -- ��ӡ���ö�ջ�� k* ��ʾ��ǰ�̵߳Ķ�ջ; ~*kb ��ʾ�����̵߳Ķ�ջ
	*   ld ģ�� -- ����ָ��ģ��ĵ��Է���(�ɼ��ص�ģ������ͨ�� lm �ܿ�����)
    *   lm -- �г���ǰ���صķ��ű��ɲ鿴�Ƿ���ȷ��������Ҫ���Եĳ��������ķ��ű����� M ��ǣ���ʾ�����ļ���ִ��ӳ���ļ����ڲ�ƥ��
	*     lm v -- ������ʾ��ϸ��Ϣ
	*     lm e -- ֻ��ʾ�з��������ģ��
    *     lmm -- list module match
    *   ln -- ��������ָ����ַ����ķ���
    *   p -- step over������ִ���꺯������
	*     pa -- ������ָ����ַ���������Ӻ���
	*     pc -- ����ִ�е���һ����������(Call ָ��)
    *   q -- �������ԻỰ
    *   r -- �۲���޸ļĴ������� r ecx=2 ��ʾ��ecx����Ϊ2
    *   s -- �����ڴ�����
    *   sx -- ���õ����¼�����ʽ
    *   t -- trace into�������������������ã����뱻���ú���
	*     ta -- ׷�ٵ�ָ����ַ�������Ӻ���
	*     tc -- ׷��ִ�е���һ����������(Call ָ��)
    *   u XXXXXXXX -- �����ָ���ĵ�ַ(����Ҫ 0x ǰ׺ ?)�� �� lkd> u KeAddSystemServiceTable l 40
    *   version -- ��ʾ�汾���Ѿ����ص���չģ�����Ϣ
	*   wt -- ����ִ�в�����һ�ݱ���(����÷�����������?)
    *   x -- �������Է��ţ��� x /a win32k!NtUser* �г�win32kģ���������� NtUser ��ͷ�ķ��Ų�����ַ��������
	*     /t -- ��ʾ���ŵ��������͡� /v -- ��ʾ���ŵķ������ͺʹ�С
    *   
    * Ԫ���� -- ��һ����(.)��ʼ
    *   .abandon -- �����û�̬����Ŀ�����
    *   .asm -- �����ѡ��
    *   .attach -- ���ӵ�ָ������
    *   .chain -- �г���ǰ���ص�������չģ��
    *   .childdbg -- �����ӽ��̵ĵ��ԣ����ͬʱ���Լ������̣�����ʹ�� "|" �������л�
	*   .context -- ���û�����ʾ���������û�̬��ַ��ҳĿ¼����ַ(Base of Page Directory)
    *   .create -- �����½���
    *   .dbgdbg -- ������һ�������������Ե�ǰ������
    *   .detach -- �������Ŀ��
    *   .dump -- ��ʾ����ת������Ŀ������
    *      .dump /ma Dump�ļ���   -- �ֶ�����Dump�ļ�
    *   .echo -- ��ʾ ����(alias) �ȵ���Ϣ
    *   .endpsrv -- ����Զ�̽��̷�����
    *   .endsrv -- �������������
    *   .exepath -- ��ִ���ļ�
    *   .expr -- ���Ʊ��ʽ������
    *   .extpath -- ��չ����ģ��·��
    *   .extmatch -- ƥ����չ����
	*   .frame -- �۲쵱ǰ�ľֲ�������
    *   .fromats -- �Բ�ͬ��ʽ��ʾ����
    *   .help -- �г�����Ԫ�����ÿ������ļ�˵��
    *   .kill -- ɱ������
    *   .ld -- �ֶ����ط��ű��� .ld ????
    *   .load ģ������·�� -- ������չ����ģ��
    *   .loadby ģ������ -- WinDBG ���Զ�����ǰ�����ж������չģ������·��������ƥ���ģ�鲢����
    *   .logfile -- ��ʾ��������־�ļ�
    *     .logopen(��)/.logappend(׷��)/.logclose(�ر�)
    *   .lsrcpath -- Զ�� Դ��·��
    *   .opendump -- ��ת���ļ�
	*   .process [EPROCESS] -- ��ʾ���л����������ģ� ʹ�� !process 0 0 �г�ϵͳ�е����н��̵Ļ�����Ϣ
    *   .reload -- ���¼��ط��ű� /i ������ز��ϸ�ƥ��ķ����ļ�
    *   .remote -- ���� remote.exe ֧��Զ�̵���
    *   .restart -- ���¿�ʼ���ԻỰ
    *   .send_file -- ��Զ�̷����������ļ�
    *   .server -- �������������
    *   .servers -- �г����÷�����
    *   .srcpath/.srcnoise/.srcfix -- ���ñ���Դ��·��(����Ǳ���������ԵĻ���������)
	*   .suspend_ui -- ��ʱֹͣˢ����Ϣ���ڣ���ֹ�������̫Ƶ��
    *   .symopt -- ��ʾ���޸ķ���ѡ�ʹ��һ��32λ��DWORD ����¼����ѡ�ÿλ����һ��ѡ�+������,-��ȡ��
    *   .sympath -- �������ӡ��޸ġ�������ʾ��������·��(���ش���Ŀ¼ + ���ŷ����������λ��֮���÷ֺŷָ�)
	*   .symfix -- ���÷��ŷ�����
	*   .thread [ETHREAD] -- 
    *   .time -- ��ʾ���ԻỰʱ��
    *   .tlist -- ��ʾ�����б�(task list)
    *   .ttime -- ��ʾ�߳�ʱ��
    *   .unload/.unloadall -- ж����չ����ģ��
    *   .writemem -- ��ԭʼ�ڴ�����д���ļ�
    *   ��д�������(.if��.else��.elsif��.foreach��.do��.while��.continue��.catch��.break��.continue��.leave��.printf��.block ��)  
    * 
    * ��չ���� -- �Ը�̾��(!)��ʼ������﷨Ϊ�� ![��չģ����.]<��չ������> [����]
    *     TODO: !for_each_XXXX ����ѭ��ִ��
    *   acpikd.dll -- ����ACPI ���ԣ�׷�ٵ���ASL ����Ĺ��̣���ʾACPI ����
    *   ext.dll -- �����ڸ��ֵ���Ŀ��ĳ�����չ����
    *   exts.dll -- ���ڶ�(!heap)������/ �߳̽ṹ(!teb/!peb)����ȫ��Ϣ(!token��!sid��!acl)����Ӧ�ó�����֤(!avrf)�ȵ���չ���
    *   fltkd.dll -- ���ڵ����ļ�ϵͳ�Ĺ�����������(FsFilter)
    *   kdexts.dll -- �����˴��������ں˵��Ե���չ����
    *   kext.dll -- �ں�̬����ʱ�ĳ�����չ����
    *   ks.dll -- ���ڵ����ں���(Kernel Stream)
    *   logexts.dll -- ���ڼ��Ӻͼ�¼API ����(Windows API Logging Extensions)
    *   minipkd.dll -- ���ڵ���AIC78xx С�˿�(miniport)��������
    *   ndiskd.dll -- ���ڵ��������й���������
    *   ntsdexts.dll -- ʵ����!handle��!locks��!dp��!dreg(��ʾע���)������
    *   rpcexts.dll -- ����RPC ����
    *   scsikd.dll -- ���ڵ���SCSI �йص���������
    *   sos.dll -- ���ڵ����йܴ����.Net ����
    *   traceprt.dll -- ���ڸ�ʽ�� ETW ��Ϣ
    *   uext.dll -- �û�̬����ʱ�ĳ�����չ����
    *   vdmexts.dll -- ���������� VDM �е� DOS ����� WOW ����
    *   wdfkd.dll -- ����ʹ��WDF(Windows Driver Foundation)��д����������
    *   wmitrace.dll -- ��ʾ WMI ׷���йص����ݽṹ������������־�ļ�
    *   wow64exts.dll -- ����������64 λWindows ϵͳ�е�32 λ����
    *   WudfExt.dll -- ���ڵ���ʹ��UMDF ��д���û�̬��������
    * 
    * α�Ĵ���(ʹ�÷�ʽ @α�Ĵ�����, �� ln @$exentry �� ? @$pagesize)
    *   $csp -- ָ֡�롣x86 �м�ESP �Ĵ�����x64 ��RSP������ΪBSP��
    *   $dbgtime -- ��ǰʱ��
    *   $ea -- ����Ŀ����ִ����һ��ָ�����Ч��ַ(effective address)
    *   $exentry -- ��ǰ���̵���ڵ�ַ
    *   $exp -- ���ʽ����������������һ�����ʽ
    *   $ip -- ָ��ָ��Ĵ�����x86 �м�EIP��x64 ��eip
    *   $pagesize -- ����Ŀ������ϵͳ���ڴ�ҳ�ֽ���
    *   $peb -- ��ǰ���̵Ľ��̻�����(PEB)�ĵ�ַ
    *   $ptrsize -- ����Ŀ������ϵͳ��ָ�����Ϳ��
    *   $proc -- ��ǰ���̵�EPROCESS �ṹ�ĵ�ַ
    *   $ra -- ��ǰ�����ķ��ص�ַ(retrun address)
    *   $retreg -- ��Ҫ�ĺ�������ֵ�Ĵ�����x86 �ܹ�ʹ�õ���EAX��x64 ��RAX��������ret0
    *   $retreg64 -- 64 λ��ʽ����Ҫ��������ֵ�Ĵ�����x86 ����edx:eax �Ĵ�����
    *   $teb -- ��ǰ�̵߳��̻߳�����(TEB)�ĵ�ַ
    *   $thread -- ��ǰ�̵߳�ETHREAD �ṹ�ĵ�ַ
    *   $tid -- ��ǰ�̵߳��߳�ID
    *   $tpid -- ӵ�е�ǰ�̵߳Ľ��̵Ľ���ID(PID)
    * 
    * Զ�̵��� -- ͨ����Զ�̹��ߡ�ת������ϣ�ʵ�ָ�������Զ�̵��Է�ʽ����֧�ֲ�ͬ�����绷��
    *   1.DbgSvr/KdSvr + remote.exe ?
    *   2.����ˣ�ʹ����Ϊadvdbg�������ܵ��� windbg �Cserver npipe:pipe=advdbg �� ִ�� .server npipe:pipe=advdbg ����
	*     �ͻ��ˣ����Ӹ������ܵ���windbg -remote npipe:server=FJPC,pipe=advdbg �� "Connect to Remote Session" ������ npipe:Pipe=advdbg,Server=FJPC
	*
    * �����в���
    *   -k -- �����ں�(Kernel)���ԣ� -kl �Ǳ����ں˵���
	*   -pe PID -- ����һ����Detach�����ĵ��Խ��̣��� -p ���������ڿ�������"DebugPort��Ϊ��"�Ľ���
    *   -z -- ָ��ת���ļ�
    *
    * ���Լ���
    *   1.�� Watch ���������� "@err,hr" �����Լ��� API ���Ƿ���� ������ԭ�� -- GetLastError
    *   2.ʹ�÷ֺţ�;������Ϊ�ָ�����������ͬһ������������
    *   3.ʹ�� Ctrl+Break ����ֹһ����ʱ��δ��ɵ�����
    *   4.ʹ��Windbg��ʾϵͳ�����Ϣ(��ʾSession1 �е� WinSta0): lkd>!object Sessions\1\Windows\WindowStations\WinSta0 
    *   5.�����ں�Ŀ�� -- 
    *
    * WinDbg����dmp�ļ�(�������ز����ز�ƥ��ķ��� -- VS2008��������)
	*   1.���÷���·�� SRV*E:\OSSymbols*http://msdl.microsoft.com/download/symbols
	*   2.��dmp�ļ�
	*   3.!sym noisy  -- �൱��.symopt+0x80000000����������ν�ġ����ӡ�ʽ���ż��أ���ʾ���ż��صĵ�����Ϣ��
	*   4.!analyze -v
	*********************************************************************************************************/

    /*********************************************************************************************************
    * ��Release�汾�����öϵ�: _asm int 3
	*   
	* ʹ�� TRACE/ATLTRACE ��ӡ�������պ�������ʱ�����ܻᱨ" _CrtDbgReport: String too long or IO Error "�Ĵ��󣬶��޷������־��
	*   ԭ��: wprintf_s ������ȷ������պ��� UNICODE ����(�ƺ�VS2010���޸������Bug��)
	*   ��������(��δ��ϸ�о�)��http://www.cnblogs.com/hnrainll/archive/2011/05/07/2039700.html
	*   ���������
	*     1.ʹ��FTLTRACE(�ڲ�ʹ�� StringCchVPrintfEx + OutputDebugString )
	*     2.#include <locale.h>
	*       TCHAR* old_locale = _tsetlocale( setlocale(LC_ALL,NULL) );
	*       _tsetlocale( LC_ALL, _T("chs") );    �� "", "korean"
	*       TRACE(XXXX);
	*       _tsetlocale( LC_ALL, old_locale); 
	*       free(old_locale); 
    *********************************************************************************************************/

    //printf �� Ctrl Code -- printf("\033[30m" "�������Ʒ�" "\033[0m" , �����б�)
    #define CTRL_COLOR_RESET        TEXT("\033[0m")

    #define CTRL_COLOR_BOLD         TEXT("\033[1m")
    #define CTRL_COLOR_GREY         TEXT("\033[5m")
    #define CTRL_COLOR_UNDERLINE    TEXT("\033[4m")
    #define CTRL_COLOR_BLINK        TEXT("\033[5m")
    #define CTRL_COLOR_BLACKBLOCK   TEXT("\033[7m")
    #define CTRL_COLOR_DELETELINE   TEXT("\033[9m")

    #define CTRL_COLOR_BLACK        TEXT("\033[30m")
    #define CTRL_COLOR_RED          TEXT("\033[31m")
    #define CTRL_COLOR_GREEN        TEXT("\033[32m")
    #define CTRL_COLOR_YELLOW       TEXT("\033[33m")
    #define CTRL_COLOR_BLUE         TEXT("\033[34m")
    #define CTRL_COLOR_MAGENTA      TEXT("\033[35m")
    #define CTRL_COLOR_CYAN         TEXT("\033[36m")


	FTLEXPORT class CFRectDumpInfo : public CFConvertInfoT<CFRectDumpInfo, const RECT&, 64>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFRectDumpInfo);
	public:
		FTLINLINE explicit CFRectDumpInfo(const RECT& rect);
		FTLINLINE virtual LPCTSTR ConvertInfo();
	};

	FTLEXPORT class CFFileTimeDumpInfo : public CFConvertInfoT<CFFileTimeDumpInfo, const FILETIME&, 64>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFFileTimeDumpInfo);
	public:
		FTLINLINE explicit CFFileTimeDumpInfo(const FILETIME& fileTime);
		FTLINLINE virtual LPCTSTR ConvertInfo();
	};

	//˫��ʾ��ʱ: dwFlags = 01(MONITORINFOF_PRIMARY) ������
	//  rcMonitor ��ʾ������ʾ����
	//  rcWork    �ų�������������
	//��������ߵ�����(�·���������)��cbSize= 40,dwFlags=0x1, rcMonitor=(0,0)-(1440,900), 1440x900, rcWork=(0,0)-(1440,869), 1440x869
	//�������ұߵĴ��ԣ�              cbSize= 40,dwFlags=0x0, rcMonitor=(1440,-124)-(2720,900), 1280x1024, rcWork=(1440,-124)-(2720,900), 1280x1024

	FTLEXPORT class CFMonitorInfoDumpInfo: public CFConvertInfoT<CFMonitorInfoDumpInfo, const MONITORINFO &>
	{
		DISABLE_COPY_AND_ASSIGNMENT(CFMonitorInfoDumpInfo);
	public:
		FTLINLINE explicit CFMonitorInfoDumpInfo(const MONITORINFO& monitorInfo);
		FTLINLINE virtual LPCTSTR ConvertInfo();
	};
}

#endif //FTL_DEBUG_H

#ifndef USE_EXPORT
#  include "ftlDebug.hpp"
#endif
