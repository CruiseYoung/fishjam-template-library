#ifndef F_DRIVER_64_H
#define F_DRIVER_64_H

/******************************************************************************************************************
* 64λƽ̨����������
* 
* ���
*   64λ����������֧����Ƕʽ�Ļ�����(__asm) -- ���Խ���������ȡ������д�ڵ����Ļ���ļ��У������obj��ͨ�� TARGETLIBS ����
*   MS�ı������� ml64.exe���������� GoASM(GoASM������ + GoLINK������ + GoRC��Դ������)��
*
*
* ��������£�ETHREAD�е�ServiceTable��Ҫôָ��KeServiceDescriptorTable,Ҫôָ�� KeServiceDescriptorTableShadow
* SSDT���� -- KiServiceTable ?
*   1.64λϵͳ������ KeServiceDescriptorTable, ������ LNK2001: unresolved external symbol �Ĵ���
*     ʵ�������ڴ����Ǵ��ڵģ��������ͨ�� dq KeServiceDescriptorTable �� ln KeServiceDescriptorTable �鿴(ʵ�ʵ�ַ���)
*     0: kd> dp KeServiceDescriptorTable
*        fffff800`02ab1840  fffff800`0287bb00 00000000`00000000     <== KeServiceDescriptorTable(KiServiceTabe, 32λϵͳ�е�����64λϵͳ��δ����)
*        fffff800`02ab1850  00000000`00000191 fffff800`0287c78c     <== SSDT�ĸ�����0x191��
*        fffff800`02ab1860  00000000`00000000 00000000`00000000
*        fffff800`02ab1870  00000000`00000000 00000000`00000000
*        fffff800`02ab1880  fffff800`0287bb00 00000000`00000000		<== KeServiceDescriptorTableShadow, �����ݺ� KeServiceDescriptorTable һ��(������ʲôϵͳ��δ����)
*        fffff800`02ab1890  00000000`00000191 fffff800`0287c78c		<== SSDT�ĸ�����0x191��
*        fffff800`02ab18a0  fffff960`00131c00 00000000`00000000		<== Shadow SSDT
*        fffff800`02ab18b0  00000000`0000033b fffff960`0013391c		<== Shadow SSDT������0x33b��
*     0: kd> ln KeServiceDescriptorTable -- �������� fffff800`02ab1840 �� fffff800`02ab1880
*        (fffff800`02ab1840)   nt!KeServiceDescriptorTable   |  (fffff800`02ab1880)   nt!KeServiceDescriptorTableShadow
*     0: kd> ln KeServiceDescriptorTableShadow  -- ��ʾ Shadow SSDT ? -- Ϊʲô���ַ�͵ڶ��� KeServiceDescriptorTable һ����
*        (fffff800`02ab1880)   nt!KeServiceDescriptorTableShadow   |  (fffff800`02ab18c0)   nt!KeUserCallbackDispatcher
*     Ŀǰֻ��ͨ��������������(�� KeAddSystemServiceTable)�������������ҳ� KeServiceDescriptorTable �ĵ�ַ
*
*   2.����� PatchGuard ��������Hook SSDT ����ʱ�� BSOD
*   
* ��64λϵͳ���ں˺�����ͷ��ַ�ĵ���λһ����0������:xxxxxxxx`xxxxxxx0(����ʱ�������ã�)
*
* �����м��64λϵͳ
*   1.IA32_EFER.LMA
*   2.if(sizeof(ULONG_PTR)==4)  32λ 
*   
******************************************************************************************************************/

/******************************************************************************************************************
* (LDE64) Length Disassemble Engine) -- X64���������
******************************************************************************************************************/


/*
[notes]x64�Ĺ��������ASM�ļ�(R0��R3)   -- http://hi.baidu.com/ithurricane
��һ����¼���ѡ�����
x64�������޷����������䣬���Ա���ר��дasm�ļ������������ԣ�
���ܽ�һ��R0������α��룺
1. ����MASM64��������Tesla.Angela�޸İ棩�����ص�ַ��http://good.gd/1399004.htm
Ȼ��дһ�������bat�ļ�������װ��C�����棬��Ŀ��D:\Devlopment\test��asm�ļ���x64.asm��
@Set ProgName=64bit
@Color 0E
@Title %ProgName%
@call C:\Masm64\Env.Cmd
@set Directory="D:\Devlopment\test"
@Set SrcName=x64
@cd %Directory%
@del "%Directory%\%SrcName%.obj"
@C:\Masm64\BIN\x64\ml64 /c "%Directory%\%SrcName%.asm"
@pause   
2.�޸�sources�ļ�
��� AMD64_SOURCES=x64.asm��TARGETLIBS = x64.obj������
3.����޸�һ��mybuild.bat�ļ�
ddkbuild.cmd -WLHX64 chk . -cZ -WDF����64λ������
������R3�Ĺ��̱���ASM�ļ����ұ��������趨����㶨�ģ�����һֱ�޷��ɹ���ֻ��ֱ���޸Ĺ����ļ�
1. VS2008�Ĺ����ļ���
�޸�**.PowerTool64.vcproj
���һ��Filter�ڵ㣬��������
<Filter Name="ASM" >
<FileRelativePath=".\ASM\x64.asm">
<FileConfiguration Name="Release|x64">
<Tool Name="VCCustomBuildTool"
CommandLine="cd "$(IntDir)" ml64.exe /D_WIN32 /c /Cx "$(InputPath)""

*/

#endif //F_DRIVER_64_H