#ifndef F_DRIVER_DEBUG_H
#define F_DRIVER_DEBUG_H

/******************************************************************************************************************
* 调试的Checked版本中有 define DBG 1 的宏定义
* 打印日志：
*   DbgPrint("x=%d\n", 100) -- 
*     %s -- ANSI 字符串
*     %S/%ws -- UnicodeString.Buffer
*     %Z -- ANSI_STRING 变量的地址
*     %wZ -- UNICODE_STRING 变量的地址
*   KdPrint(("x=%d\n", 100)) -- DbgPrint 的宏，注意为了利用格式化功能，一般需要使用两重括号
*
* 驱动可以从注册表中提取相应参数，来提高灵活性。
******************************************************************************************************************/

/******************************************************************************************************************
* 一般的过滤等非硬件驱动安装完毕后，会以"隐藏设备"的形式加在设备管理器的 "Not-Plug and Play Drivers"(NT式？)中
*
* SrvInstW.exe(http://www.onlinedown.net/soft/36059.htm)
*   将驱动作为服务的方式安装，然后可以通过 net start/stop 来启动/停止 驱动程序
*    卸载驱动：sc delete xxxx
* 
* WinObj(http://technet.microsoft.com/en-us/sysinternals/bb896657.aspx)
*   可以显示所有的内核对象
*
* Win2K 时代的产品
*   DriverStudio工具包
*     DriverWorks -- Win2K时代支持WDM开发的辅助工具(是否有最新版本?)，提供了非常完善的向导
*     DriverMonitor -- 可以安装卸载驱动，并可以直接显示调试语句
*     EzDriverInstaller -- 可以很方便的通过 inf 安装卸载WDM驱动的工具
*
* Win7时代的产品( http://www.osronline.com 网站似乎有不少好东西？)
*   DeviceTree -- 显示所有的Pnp设备树，包括各个设备的关系(设备堆栈 等)
*   IrpTrace -- 可以方便地最终IRP的各种操作
*
* 通过DebugView查看打印的日志信息，需要选中 Capture Kernel 才能看到内核输出
* BusHound -- 调试USB驱动程序
* IRPTrace -- 查看IRP
* EzDriverInstaller -- 加载WDM式驱动
* DeviceTree -- 查看设备对象
* 
* 虚拟机调试
*   VMWare
*     共享目录：Options -> Shared Foloders -> 增加共享目录, 该共享目录会出现在虚拟机中的“网络驱动器”里(老版本VMWare需要通过 \\.host\ 访问)
*     用管道虚拟串口：Hardware -> Add -> Serial Port -> Output to named pipe -> 修改第三个选项为"The other end is an application"
*       调试机上启动windbg(可设置快捷方式的属性)：windbg.exe -b -k com:port=\\.\pipe\com_1,baud=115200,pipe，设置好符号文件和源码路径，
*       刚连接上时，虚拟机里的Windows系统会被中断(貌似时机),需要在WinDbg的命令提示符后执行 "g" 命令允许其继续运行，
*       可在源码中调试版本时 增加 _asm int 3 的软件断点
*
*
******************************************************************************************************************/
/******************************************************************************************************************
* WinDbg(http://msdn.microsoft.com/en-us/windows/hardware/gg463009)
*   调试内核时必须是双机调试？通过串口相连？网络？
*   设置被调试机器为调试执行方式，WinDbg会把内核视为一个整体进行调试
*     WinXP：boot.ini 文件中拷贝并增加一个启动项 /debug /debugport=com1 /baudrate=115200
*     Vista：
*       bcdedit /enum OSLOADER <== 先枚举出本机上所有的"操作系统加载器"
*       bcdedit /copy {current} /d "Windows Debug Model"  <== 拷贝出一个调试模式的配置项
*       TODO: 设置刚拷贝出来的配置项为当前配置项？否则会更改到当前的正常配置(或者拷贝出一个 Original ?)
*       bcdedit /debug ON  <== 设置为调试
*       bcdedit /bootdebug ON 
*       bcdedit /dbgsetting  <== 查看当前的调试配置
*
*   常用命令(在中断状态下，通过F9键可以设置/取消断点)
*     g -- 中断的程序继续运行
*     u XXXXXXXX -- 反汇编指定的地址(不需要 0x 前缀 ?)
*     dt Xxxx -- 显示变量内容？ 如 dt _EPROCESS
******************************************************************************************************************/



#endif //F_DRIVER_DEBUG_H