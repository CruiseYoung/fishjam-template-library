package com.fishjam.android.study;
import junit.framework.TestCase;

/***************************************************************************************************************************************
 * 学习书籍
 *     Android应用开发详解.pdf -- P237, 
 *     欢迎来到Android世界 http://www.docin.com/p-31448231.html
 *     Android中可视化GUI布局拖拉开源工具程序  http://code.google.com/p/droiddraw , 或者通过 http://www.droiddraw.org/ 在线使用
 *     Google Android SDK开发范例大全  -- http://book.csdn.net/bookfiles/1060/100106031790.shtml
 * 
 * Android是Google发布的基于Linux平台的开源手机操作系统。
 *   Android Market(http://www.android.com/market/) -- 用户浏览、下载或购买应用软件。
 *   Google的基于Linux平台的开源手机操作系统,由Google与开放手机联盟(包括中国移动、摩托罗拉等)合作开发。由四部分组成:
 *     1.应用软件(Java)
 *     2.应用程序框架 -- 用户界面(Views),内容管理器(Content Providers), 资源管理器(Resource Manager),通知管理器(Notification Manager),活动管理器(Activity Manager).
 *        应用程序框架支持组件的重用与替换，如可替换系统中的打电话应用程序、文件管理器等
 *     3.中间件(哪些? 指Android程序库? -- ) C/C++库集，通过应用程序框架为开发者提供服务。
 *     4.操作系统(Linux) -- 核心系统服务依赖于Linux内核(2.6)，如安全性、内存管理、进程管理、网络协议栈和驱动模型等
 *   采用WebKit浏览器引擎，具备触摸屏、高级图形显示和上网功能，比iPhone等其他手机更强调搜索功能，界面更强大，可以说是一种融入全部Web应用的单一平台。
 *   开放手机联盟OHA (Open Handset Alliance)
 *   基于Android平台的SDK(Software Development Kit) -- m3-rc20a
 *   Android的目标是发展成为移动互联网领域终端设备的软件平台，就好比Windows在PC机领域的地位。
 * 
 * TODO: 
 *   1.SDK 版本 和 Google API 版本，两者有什么区别
 * 
 **************************************************************************************************************************************/

/**************************************************************************************************************************************
 * 环境搭建 （需要 Apache Ant 1.7 以上 ？) -- JDK + Eclipse + Android SDK + ADT
 *   TODO: 现在直接下载 adt-bundle-windows-x86-20131030 等的压缩包解压即可? 
 *      http://code.google.com/android/ 或 http://developer.android.com/
 *   1.下载安装JDK -- jdk-6u16-windows-i586.exe, 设置Java环境变量(%JAVA_HOME%, %CLASSPATH%值为".", 加入path)
 *   2.解压Ant, 将Ant路径加入path，(设置环境变量?)
 *   3.下载解压Eclipse -- http://www.eclipse.org/downloads/
 *   4.解压Android SDK(91M) -- http://developer.android.com/sdk/index.html，1.6以后只提供了在线安装
 *      并设置Android SDK的环境变量(如 ANDROID_SDK ，并将 tools 子目录加入path)；目录中最好不要带空格。
 *   5.本地或通过网络安装 Android for Eclipse 的插件(ADT -- Android Development Toolkit)：http://dl-ssl.google.com/android/eclipse/ (或使用 https:// ?).
 *      包括 Android Development Tools 和 Android Editors.
 *      ADT可以: 代码的自动生成、调试、编译、打包、拖拽式界面生成等
 *   6.重启Eclipse后，Window > Preferences > Android > SDK  -- 指定SDK的根目录。
 *   7.File -> New -> Project... -> Android -> Android Project 创建工程。
 *                  
 *   1.Eclipse 
 *     更改字体： Window-> Prefrences -> General -> Appearance -> Colors and Fonts -> Text Font, 
 *                     然后"Use System Font"，会自动使用 微软雅黑 
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * 工程的目录结构
 *   assets -- 原生的文件(如MP3等)，Android程序不能直接访问，必须通过 AssetManager类以二进制方式来读取
 *   src -- 保存源码
 *   res -- 可通过 R.java 中的资源类直接访问，被编译到应用程序中
 *     |-anim : 保存动画
 *     |-drawable : 保存图片，主要分为：BitmapFile, ColorDrawable(颜色), Nine-Patch Image(九片图片)
 *     |-layout :保存布局文件(xml格式)
 *     |-menu   :xml菜单文件, <menu> -> <group> -> <item> 。<group>表示一个菜单组，可以一起设置其属性(如visible,checkableBehavior 等)
 *     |-raw    :直接复制到设备中的原生文件
 *     |-values :保存一些常量值，如 strings.xml(字符串常量), arrays.xml(数组), colors.xml(颜色), dimens.xml(尺寸, 如文字尺寸、高宽等), styles.xml(样式)
 *     |-xml    :xml文件
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * Android应用程序有以下几种类型：
 *   1.前端Activity（Foreground Activities）。
 *   2.后台服务（Background Services） -- 系统服务（System Service）
 *      Service -- 长生命周期，没有用户界面的程序，如正在从播放列表中播放歌曲的后台媒体播放器。 
 *      Context.startService 启动一个Service; Context.bindService 连接到一个Service上(如还没有运行则启动)
 *      系统Broadcast（广播信息）与Receiver（广播信息）接收器 -- 如电量低的广播和接收器
 *   3.间隔执行Activity（Intermittent Activities）-- 类似Notification Manager等等
 *    
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * 常见问题
 *    1.多分辨率设备兼容性问题解决方案
 *       创建相应分辨率的资源文件夹和页面布局文件夹，在编译运行时会根据当前设备分辨率，自动选择相应的资源文件和页面布局文件。
**************************************************************************************************************************************/

public class AndroidStudy  extends TestCase{

}
