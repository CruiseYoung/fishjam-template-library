package com.fishjam.android.study;
import android.annotation.TargetApi;
import android.os.Build;
import android.test.AndroidTestCase;

//Android_Tutor的专栏 -- http://blog.csdn.net/android_tutor/article/category/605362
	
/***************************************************************************************************************************************
 * TODO: 
 *   1.GridLayout 是4.0以后版本中新增的，如果希望在更早的平台上使用，则需要“导入相应的支撑库” -- 具体是什么？怎么导入?
 *   2.高版本程序改成低版本后运行失败 -- 新创建的低版本程序 比较
 *     a. .settings\org.eclipse.jdt.core.prefs
 *     b. AndroidManifest.xml 中的 <uses-sdk> 部分
 *     c. .classpath 文件 
 *     d. project.properties 文件
 *     e. 高版本可能会增加 crunch、res/values-v11、res/values-v14 等 资源目录
 *   3.以库方式给其他项目使用 -- project.properties 中增加 android.library=true， 然后其他项目可以加入
 *     Windows中要求两个工程在同一个盘符下，否则会关联失败(显示添加成功，但再次打开时显示红叉)，此为ADT 的bug
 *     其他问题(R.id 的final问题)：http://blog.sina.com.cn/s/blog_ac843e3301017p2t.html
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * 学习书籍
 *     疯狂Android讲义 -- P292 (Intent)
 *     Android应用开发详解.pdf -- 311, 
 *     欢迎来到Android世界 http://www.docin.com/p-31448231.html
 *     Android中可视化GUI布局拖拉开源工具程序  http://code.google.com/p/droiddraw , 或者通过 http://www.droiddraw.org/ 在线使用
 *     Google Android SDK开发范例大全  -- http://book.csdn.net/bookfiles/1060/100106031790.shtml
 * 
 * Android是Google发布的基于Linux平台的开源手机操作系统。
 *   Android Market(http://www.android.com/market/) -- 用户浏览、下载或购买应用软件。支持集合发布，系统自动根据用户手机选择合适的apk。
 *   Google的基于Linux平台的开源手机操作系统,由Google与开放手机联盟(包括中国移动、摩托罗拉等)合作开发。由四部分组成:
 *     1.应用软件(Java) -- 包括一些系统的核心应用程序，如 SMS程序、日历、地图、联系人等。
 *     2.应用程序框架 -- 用户界面(Views),内容管理器(Content Providers), 资源管理器(Resource Manager),通知管理器(Notification Manager),活动管理器(Activity Manager).
 *        应用程序框架支持组件的重用与替换，如可替换系统中的打电话应用程序、文件管理器等
 *     3.中间件，即函数库? -- C/C++库集(从BSD系统派生出的libc系统库)，通过应用程序框架为开发者提供服务。
 *     4.操作系统(Linux) -- 核心系统服务依赖于Linux内核(2.6)，如安全性、内存管理、进程管理、网络协议栈和驱动模型等
 *   采用WebKit浏览器引擎，具备触摸屏、高级图形显示和上网功能，比iPhone等其他手机更强调搜索功能，界面更强大，可以说是一种融入全部Web应用的单一平台。
 *   开放手机联盟OHA (Open Handset Alliance)
 *   基于Android平台的SDK(Software Development Kit) -- m3-rc20a
 *   Android的目标是发展成为移动互联网领域终端设备的软件平台，就好比Windows在PC机领域的地位。
 * 
 * 每个Android应用程序都运行在单独的Dalvik虚拟机内，可以方便的实现对应用程序的隔离。
 *   注意：Dalvik并未完全遵守JVM规范，二者也不兼容。
 *        JVM运行的是Java字节码，JVM直接从.class文件或JAR包中加载字节码然后运行；
 *        Dalvik运行的是其专有的dex(Dalvik Executable)文件，需要通过DX工具将应用程序的所有.class文件编译成.dex文件后运行(性能更高、文件更小，基于寄存器实现)
 * 
 * TODO: 
 *   1.SDK 版本 和 Google API 版本，两者有什么区别
 *   2.版本兼容
 *     2.1. 使用 Android Support Library package 来保证来高版本sdk开发的向下兼容性，
 *           使得高版本开发的程序在低版本OS上也能运行，但功能可能会退化(如 fragement ?) 
 *           android-support-v4.jar -- 支持 1.6 及以后
 *           android-support-v7.jar -- 支持 2.1 及以后，依赖于 -v4.jar 包(即两者同时包含)
 *           android-support-v13.jar --支持3.2 及以后，通常在平板开发中才会使用
 *     2.2. 使用 @TargetApi(版本号) 使高版本API的代码在低版本SDK不报错(即避免低版本中找不到定义的错误)，
 *           在代码中还需要加一个版本判断是否执行该代码, 参见 testVersionCompat
 *           TODO: 这是编译时行为还是运行时行为？
 *   3.平台版本 和 API Level ( Build.VERSION_CODES )
 *      获得API Level 的代码： Integer apiLevel = Integer.parseInt(VERSION.SDK);
 *      编译时指定版本：在函数前增加如下声明 --   表明只针对 14(及之后?)的有效？
 *      平台版本                API Level                功能
 *     Android 4.4                19(KITKAT)
 *     Android 4.3                18(JELLY_BEAN_MR2)
 *     Android 4.2                17(JELLY_BEAN_MR1)
 *     Android 4.1                16
 *     Android 4.0.3             15
 *     Android 4.0                14(ICE_CREAM_SANDWICH)
 *     Android 3.2                13
 *     Android 3.1                12
 *     Android 3.0                11
 *     Android 2.3.3             10(GINGERBREAD_MR1)
 *     Android 2.3                9
 *     Android 2.2                8(FROYO)
 *     Android 2.1                7(ECLAIR_MR1)
 *     Android 2.0.1             6(ECLAIR_0_1)
 *     Android 2.0               5(ECLAIR)
 *     Android 1.6               4(DONUT)
 *     Android 1.5               3(CUPCAKE)
 *     Android 1.1               2(BASE_1_1)
 *     Android 1.0               1(BASE)
 **************************************************************************************************************************************/

/**************************************************************************************************************************************
 * TODO: 
 *   1.现在直接下载 adt-bundle-windows-x86-20131030 等的压缩包解压即可(安装了ADT插件的 Eclipse + SDK)?
 *     然后将 Android SDK 目录下的tools、platform-tools 等子目录加入PATH环境变量(或创建设置环境变量的批处理) 
 *      http://code.google.com/android/ 或 http://developer.android.com/
 *   2.可以设置 ANDROID_SDK_HOME 环境变量，告诉SDK保存 AVD 等配置信息的地方？否则默认会保存在  %HOMEPATH%\.android 路径下?
 *   
 * 环境搭建 （需要 Apache Ant 1.7 以上 ？) -- JDK + Eclipse + Android SDK + ADT
 *   1.下载安装JDK -- jdk-6u16-windows-i586.exe/jdk-7u45-windows-i586.exe, 
 *     设置Java环境变量(%JAVA_HOME%, %CLASSPATH%值为  ".;%JAVA_HOME%\lib\tools.jar;%JAVA_HOME%\lib\dt.jar", 加入path)
 *   2.解压Ant, 设置 ANT_HOME 环境变量，将 %ANT_HOME%\bin 路径加入 PATH，(设置环境变量?)
 *   3.下载解压Eclipse -- http://www.eclipse.org/downloads/, 一般可以下载 Eclipse for Java EE Developers
 *   4.解压Android SDK(91M) -- http://developer.android.com/sdk/index.html，1.6以后只提供了在线安装
 *      并设置Android SDK的环境变量(如 ANDROID_SDK ，并将 tools 子目录加入path)；目录中最好不要带空格。
 *   5.本地或通过网络安装 Android for Eclipse 的插件(ADT -- Android Development Toolkit)：
 *     http://dl-ssl.google.com/android/eclipse/ (或使用 https:// ?).
 *      包括 Android Development Tools 和 Android Editors.
 *      ADT可以: 代码的自动生成、调试、编译、打包、拖拽式界面生成等
 *   6.重启Eclipse后，Window > Preferences > Android > SDK  -- 指定SDK的根目录。
 *   7.File -> New -> Project... -> Android -> Android Project 创建工程。
 *   
 *   TODO:
 *     1.需要安装 Tomcat(使用解压缩的安装方式),安装完毕后将 jsp-api.jar,servlet-api.jar 两个文件添加到 %CLASSPATH% 变量后;
 *       Web应用复制到 %TOMCAT_HOME%\webapps 目录下，然后进入 build.xml 所在路径，执行 ant compile 来编译应用。
 *       启动Tomcat服务器，通过浏览器即可访问该应用
 *     
 *   1.Eclipse 
 *     更改字体： Window-> Prefrences -> General -> Appearance -> Colors and Fonts -> Text Font, 
 *                     然后"Use System Font"，会自动使用 微软雅黑 
 * 
 * 导入已存在的工程
 *   方法1(项目目录已在workspace目录中): Import -> Existing Projects into Workspace
 *   方法2. 新建Android程序 -> Create project form existing source
 *
 * 发布 -- Android 项目以包名作为唯一标识，如同一手机上安装两个包名相同的应用，后安装的可以覆盖前面安装的。
 *   1.通过DX工具对*.class文件进行转换和打包，得到.dex文件;
 *   2.通过AAPT工具打包所有资源文件，得到 .ap_ 文件
 *   3.通过 apkbuilder 工具把 .dex 和 .ap_ 文件打包成 APK包
 *   4.签名(确定发布者身份和确保应用的完整性) 
 *      开发、调试阶段 -- ADT插件会自动生成调试证书对Android应用签名(在真机上会怎么样？)
 *      真实发布时 -- 必须使用合适的数字证书来给应用程序签名，
 *      方法：
 *        a.右键项目 -> Android Tools -> Export Signed Application Package
 *        b.使用 keytool + jarsigner.exe + zipalign.exe 工具，最后得到经过 签名 + 优化 的APK安装包，即可对外发布了 
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * SDK的目录结构
 *   add-ons -- 第三方公司为Android平台开发的附加功能系统，初始时为空。可加入 ???
 *   extras -- 存放Google提供的USB驱动、Intel提供的硬件加速等附加工具包 
 *   platforms -- 存放不同版本的Android系统,通过 Android SDK Manager 在线下载安装(如 android-7, android-19 等)
 *   platform-tools -- 存放了Android平台相关工具
 *   
 * 工程的目录结构
 *   assets -- 原生的文件(如MP3等)，Android程序不能直接访问，必须通过 AssetManager类以二进制方式来读取
 *   src -- 保存源码
 *   res -- 可通过 R.java 中的资源类直接访问，被编译到应用程序中的各种资源，aapt工具自动扫描生成资源清单文件(R.java)
 *     |-anim : 保存动画
 *     |-drawable : 保存图片，主要分为：BitmapFile, ColorDrawable(颜色), Nine-Patch Image(九片图片)
 *     |---drawable-ldpi,drawable-mdpi,drawable-hdpi,drawable-xhdpi 分别存放低分辨率、中分辨率、高分辨率、超高分辨率的4种图片文件
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
  *       
 * NDK开发 ?
 *   有专用的 NDK Plugins,需要依赖 C/C++开发插件
**************************************************************************************************************************************/

public class AndroidStudy  extends AndroidTestCase{
    public void testAndriod()
    {
        
    }
    
    @TargetApi(14)  //(Build.VERSION_CODES.ICE_CREAM_SANDWICH)
    public void testVersionCompat(){
    	//Build.VERSION_CODES.xxxxx
    	
        if(Build.VERSION.SDK_INT >= 14){ 
            //版本大于 14 时执行以下代码(可以使用14中新的API)
        }
    }
}
