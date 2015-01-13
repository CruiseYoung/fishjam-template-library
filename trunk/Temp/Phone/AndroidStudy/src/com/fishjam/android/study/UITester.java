package com.fishjam.android.study;
import android.app.Activity;
import android.content.Intent;
import android.graphics.Rect;
import android.test.ActivityUnitTestCase;
import android.util.Log;
import android.view.Display;

/***************************************************************************************************************************************
TODO: 
	1.使用 Adobe Edge 工具可以使用 HTML5、CSS和JavaScript制作网页动画，无需Flash，可制作类似支付宝付钱时金币掉下的效果
	
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * HVGA( Half-size VGA) -- VGA(640x480)的一半，分辨率为480x320, iPhone,第一款gPhone等手机都是。
 * WVGA() -- 800x480
 *
 * 常用距离单位
 *    dip/dp(density-independent pixels) -- 密度无关的像素, 基于屏幕密度的抽象单位，程序用它来定义界面元素。
 *      像素(px)=dip * (密度 / 160) , 如 密度为160dpi的屏幕上，1dip等于1x; 密度240dpi的屏幕上,1dip=1.5px
 *    in(英寸) -- 基于屏幕的物理尺寸
 *    mm(毫米) -- 
 *    pt(磅)-- 1/72英寸
 *    px -- 像素，对应屏幕上的一个点
 *    sp(scaled pixels) -- 比例像素，和精度无关的像素。主要处理字体的大小，可以根据用户的字体大小首选项进行缩放

 *
 * 尺寸：屏幕的物理尺寸，指屏幕的对角线长度，如 2.8英寸、3.7英寸
 * 比例：屏幕的物理长度和物理宽度比，16:9或16:10的是宽屏，4:3的为窄屏
 * 分辨率：屏幕上拥有的像素的总数，常使用"宽度x长度"表达，？通常应用程序并不直接处理分辨率？
 * 密度：以屏幕分辨率为基础，沿屏幕长宽方向排列的像素，密度较低的屏幕，在长宽方向都只有比较少的像素。
 * 视网膜(Retina)显示屏：一种超高像素和越高密度的液晶屏，相对于以前的iPhone，增加了4倍的像素数量，
 *       将960*640的分辨率压缩到和前代iPhone相同物理尺寸(3.5英寸)的显示屏内。屏幕密度达到 326像素/英寸
 *  
 * 多分辨率规格的界面设计 -- 真正需要区别的是OS版本(1.5和1.6及以上)而不是屏幕分辨率
 *   各种屏幕和UI类型：
 *     HTC Sense UI, SamSung TouchWiz UI, Motorola MOTO blur UI, Xiaomi MIUI, OMS(Ophone), LEOS(LePhone), Alibaba云OS 等
 *   A.1.5以前不支持高分屏，所有图片放在同一个drawable目录下;
 *     1.6开始支持 高(drawable-hdpi)、中(drawable-mdpi)、低(drawable-ldpi)三种密度。
 *     3.0以后支持 超高(drawable-xhdpi)，
 *     为了电视? 支持 (drawable-xxhdpi)
 * 
 *  交互设计建议：
 *    1.以中密度(mdpi)的正常屏幕(HVGA, 320x480)为设计基准进行界面布局(此时 dip和px为1:1，能更充分理解空间并合理使用)；
 *    2.程序可以为各种尺寸的屏幕提供不同的资源(主要是布局),也可以为各种密度的屏幕提供不同的资源(主要是位图);
 *    3.使用与密度无关的像素(dip)来定义应用程序的界面布局，而不是px -- 这样可保证应用程序的界面能在各种分辨率的屏幕上都可以正常显示。
 *    4.执行时，平台会根据屏幕本身的尺寸与密度特性，自动载入对应的资源，并把它们从与密度无关的像素转换成屏幕上的物理像素
 *    5.在处理屏幕方向变化时，为其优化而不是单纯地改变方向
 *    6.挑选具有代表性的特性界面在真机预览效果
 *    7.处理按钮的放大缩小时，尽可能使用九宫格图片，而不是单纯绘图
 *  
 *  在 layout 中可以通过 <include> 和 <merge> 标签重用已有的layout等 -- TODO：细节？
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * 风格(Style)
 *   自定义Style(指定颜色、大小等)
 *     1.编写style.xml文件,放在values目录下
 *       <?xml version="1.0" encoding="utf-8"?>
 *       <resources>
 *         <style name="DavidStyleText1" [parent=“父Style名”]>
 *           <item name="android:textSize">18sp</item>
 *           <item name="android:textColor">#EC9237</item>
 *         </style>    可以使用 <style></style> 定义多个,
 *       </resources>
 *     2.main.xml 中 <TextView style="@style/DavidStyleText1" ...>
 *     
 * 主题(Theme)
 *   配置中：android:theme="@android:style/Theme.Dialog"
 *   
 * view  -- 所有UI类都建立在View和ViewGroup两个类的基础之上，采用组合(Composite)设计模式，需要通过 Activity.setContentView 设置显示
 *   View --基本控件,  其子类称为 Widget
 *     Button( setOnClickListener(new Button.OnClickListener(){public void onClick(View v) {...}}); )
 *     EditText
 *     TextView
 *     RelativeLayout
 *     lists,grids,text boxes,buttons,
 *   ViewGroup -- 布局容器的抽象父类，子类称为Layout(布局管理器)，控制其子组件的分布依赖于LayoutParams,MarginLayoutParams 
 *     两个内部类，其提供了一些XML属性(如 layout_heidth/layout_width)。ViewGroup的子类通过指定这些属性来进行布局？
 *   
 * 自绘
 *   相关组件: Canvas + Bitmap + Paint 
 *   在View的子类中重载 onDraw(Canvas canvas) 函数，并在其中使用 进行绘制。
 *   
 * 手势识别 -- 如 缩放图片或者缩放网页，在 android.gesture 包中还提供了很多辅助类，
 *   +-GestureDetector(v1)/GestureDetectorCompat(sv4，推荐，可以使更多的Action适配到API4)，可简单的将 View.onTouchEvent 事件转发进去处理。
 *      OnDoubleTapListener -- 
 *   +-ScaleGestureDetector -- 检测两个手指在屏幕上缩放的手势，
 *       http://www.cnblogs.com/over140/archive/2010/12/08/1899839.html
 *       OnScaleGestureListener -- 用户提供的回调接口，一般在各个回调函数中调用 Detector 的方法进行处理。
 *         onScaleBegin(返回true表明开始缩放) -> 循环onScale -> onScaleEnd
 *       getScaleFactor() -- 返回缩放比例，注意：onScale 中如果返回true会更新factor的基数, 返回false会以旧值计算factor.
 *   实现手势识别的方法( TODO: 需要先 setFocusable(true) ? )：
 *     1.实现 OnGestureListener 和 OnDoubleTapListener 接口，实现所有方法
 *       onDown --按下 
 *       onFling(快速移动并松开)
 *       onLongPress -- 长按且未松开，此后松开也不会出现 Up 事件?
 *       onScroll -- 单指滑动, 在onDown后立即移动(不触发 onShowPress), 包括缩放? 
 *       onShowPress -- 按下到激发长按之前
 *       onSingleTapUp(手指离开触摸屏)
 *       onDoubleTap(双击)--什么区别？为什么GestureImageView中会发生对次？
 *       onDoubleTapEvent -- 通知DoubleTap手势中的事件，包含down、up和move事件(指在双击之间发生的事件)
 *       onSingleTapConfirmed -- 用来判定该次点击是SingleTap而不是DoubleTap
 *       
 *     2.继承已有的 SimpleOnGestureListener 类，只需重载需要的部分方法即可
 *   自定义手势识别：
 *     1.使用 Sample 中的 GestureBuilder 建立自定义的手势信息(会保存成 sdcard\gestures 文件)， 其中使用了 GestureOverlayView 
 *     2.拷贝 gestures 文件到本工程的 raw 目录下，代码中：
 *       GestureLibrary glib = GestureLibraries.fromRawResource(this, R.raw.gestures); glib.load();
 *       GestureOverlayView gov = (GestureOverlayView)findViewById(xxxx);
 *       gov.addOnGesturePerformedListener(){ new OnGesturePerformedListener(){
 *         public void onGesturePerformed(GestureOverlayView overlay, Gesture gesture){ 
 *           ArrayList<Prediction> list = gl.recognize(gesture); //识别手势，返回一个类型为Prediction的列表
 *           Prediction pre = list.get(0); 
 *            if(pre.score > 1){  //如果匹配度大于1，表示可以识别，否则提示无法识别
 *               if(pre.name.equals("myGesture")) { ...  }  //判断名字是否与手势库的名字相同  
 *            }
 *         } 
 *       }
 * 重力感应
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * View事件
 *     onTouchEvent() -- 当发生触摸屏事件时的回调，注意重载以后的返回值(1. 直接返回true； 2(更好？).onCreate时 setClickable(true)，然后返回super.onTouchEvent)
 *           int action = MotionEventCompat.getActionMasked(event);  然后 switch(action) {...}
 *       或 switch(event.getAction() & MotionEvent.ACTION_MASK) 来判断按键状态(down,move,up 等), 
 *     getX(相对于自身左上角的坐标)/getRawX(相对于屏幕左上角的坐标) 等方法获取位置。
 *       单点：ACTION_DOWN -> ACTION_MOVE -> ACTION_UP
 *       多点：ACTION_DOWN -> ACTION_POINTER_DOWN(getPointerCount == 2) -> ACTION_MOVE -> ACTION_POINTER_UP -> ACTION_UP
 * View属性
 *   tag -- 为组件设置I个字符串类型的tag值，然后可通过 getTag()获取该值 或通过 findViewWithTag 找到该组件
 *   
 * View --
 * +-AbsListView --  列表视图的抽象基类，有 GridView(多列)、ListView(一列) 等子类
 *    choiceMode -- 选择行为，如 none(不显示任何选中项), singleChoice(单选), multipleChoice(多选)
 *    divider -- 设置列表项的分隔条(即可用颜色、也可用Drawable)
 *    entries -- 指定一个数组资源，将根据该数组资源来生成ListView, 如指定 "@array/books"
 *    fastScrollEnabled -- 设置是否允许快速滚动，如为true，则会显示滚动图标，并允许用户拖动该滚动图标进行快速滚动
 *    scrollingCache -- 如设为true，在滚动时将会使用绘制缓存
 *    textFilterEnabled -- 设置是否对列表项进行过滤，需要对应的Adapter实现了Filter接口时才起作用
 * +-ActionBar -- 从Android3.0( targetSdkVersion >= 11 )开始，并不要求手机设备上必须提供 MENU 按键，推荐使用 ActionBar 来代替菜单。通过 Activity.getActionBar() 获取
 *      位于显示的屏幕的顶部(标题)，可显示: 菜单项(Action Item), 应用的图标和Activity标题(作为返回Home主屏的导航操作), 提供交互式的ActionView,基于Tab的导航方式，用于切换多个Fragment.
 *      当选项很多时，ActionBar无法同时显示所有的选项菜单项。对有MENU按键的手机，单击MENU即可看到剩余的菜单项；没有MENU的手机，最后显示一个折叠图标，点击时显示。
 *      若要关闭ActionBar: android:theme="@android:style/Theme.Holo.NoActionBar" 
 *      添加ActionView: a.定义ActionItem时使用 actionViewClass 属性指定其实现类(如 "android.widget.SearchView"); 
 *                             b.定义ActionItem时使用 actionLayout 属性指定对应的视图资源(如 "@layout/clock" )
 *      Tab导航: setNavigationMode(_TABS) + addTab + new Fragment() + 响应 ActionBar.TabListener
 *      下拉式(DropDown)导航: setNavigationMode(_LIST) + setListNavigationCallbacks(添加列表项和事件监听, 在 onNavigationItemSelected 中创建并设置UI界面，通常是 Fragment) 
 *      setDisplayHomeAsUpEnabled() -- 设置是否将应用程序图标转变成可点击的图标，并在图标上添加一个向左的箭头(返回)
 *      setHomeButtonEnabled -- 设置是否将应用程序图标转变成可点击的按钮
 *      setNavigationMode(NAVIGATION_MODE_TABS / NAVIGATION_MODE_LIST) -- 设置使用使用 Tab / DropDown 导航方式      
 * +-AdapterView -- 抽象基类，可以包括多个"列表项"并将其以合适的形式显示出来，其显示的列表项由 Adapter 提供(通过 setAdapter 方法设置).
 *     AdapterView 的各子类负责采用合适的方式显示Adapter提供的每个"列表项"组件。
 *     setOnItemClickListener()/setOnItemSelectedListener() -- 添加单击、选中事件
 * +-AdapterViewFlipper -- 一般用于幻灯播放。一次显示一个Adapter提供的View组件，可以通过 showPrevious()/showNext() 方法来控制显示上一个、下一个。
 *    inAnimation/outAnimation -- 控制 显示/隐藏 时使用的动画
 *    loopViews -- 设置是否循环
 *    flipInterval -- 设置自动播放的时间间隔
 * +-AnalogClock -- 模拟时钟，显示 小时、分钟
 *    dial -- 设置表盘使用的图片
 *    hand_hour/hand_minute -- 设置时针、分针使用的图片
 * +-AutoCompleteTextView -- 自动完成文本框，通过设置想要显示资源的适配器(setAdapter, 一般用最简单的文本类型的ArrayAdapter即可)来实现
 *    completionHint/completionHintView -- 下拉菜单中的 提示标题 / 提示标题视图 
 *    completionThreshold -- 用户至少输入几个字符才会显示提示
 * +-BaseAdapter -- 
 * +-Button
 *     text
 * +-CalendarView -- 日历视图，显示和选择日期，通过 setOnDateChangeListener 监控日期改变
 *     dateTextAppearance -- 设置日期文字的样式
 *     minDate/maxDate -- 设置该日历组件支持的 最小/最大 日期，以 mm/dd/yyyy 格式指定
 *     showWeekNumber/showWeekCount -- 设置是否显示 第几周/总共显示几周
 * +-CheckBox -- 复选按钮
 * +-Chronmeter -- 计时器，显示从某个起始时间开始，一共过去了多长时间
 *     format -- 指定计时器的计时格式，
 *     setBase(long) -- 设置计时器的起始时间，单位为毫秒。如 SystemClock.elapsedRealtime()
 * +-DatePicker/TimePicker -- 日期/时间选择器
 *     calendarViewShown -- 是否显示 CalendarView 组件
 *     minDate/maxDate -- 以 mm/dd/yyyy 格式指定 最小/最大 日期， 
 *     spinnersShown -- 设置日期选择器是否显示 Spinner 日期选择组件
 *     startYear/endYear -- 设置日期选择器允许选择的 第一年/最后一年
 * +-DigitalClock -- 数字时钟，可以显示当前的秒数
 * +-EditText -- 编辑框或密码框
 *     hint -- 没有输入内容时的提示信息
 *     inputType -- 指定输入组件的类型(类似 HTML 中 <input> 的type属性)，如 date(日期), number(数值), numberPassword(只接受数字密码), 等
 *     numeric -- 设置关联的数值输入法，如 integer(整数), signed(允许输入符号的数值)， decimal(允许输入小数点的数值)
 *     password -- true
 *     textSize -- 18sp
 * +-ExpandableListView -- 可展开的列表组件(类似数控件?)，把列表项分为几组，每组里可包含多个列表项，
 *       其数据由 ExpandableListAdapter接口的实现类提供，如 BaseExpandableListAdapter(可扩展性最强), SimpleCursorTreeAdapter, SimpleExpandableListAdapter 等
 *       childDivider -- 指定各组内各子列表项之间的分隔条
 *       childIndicator -- 显示在子列表项旁边的Drawable对象
 *       groupIndicator -- 显示在组列表项旁边的Drawable对象 
 * +-ExtractEditText -- 是EditText组件的底层服务类，负责提供全屏输入法支持
 * +-Fragment --3.0(11)里为了适应大屏幕的平板(如网易新闻左侧显示列表，右侧显示内容)而增加的，可以重用Activity等, 相当于Activity片段(模块化区域)，必须嵌入 Activity 中使用。
 *         拥有自己的生命周期，但被其所属的Activity的生命周期控制。Fragment向Activity传递数据时可在Fragment中定义回调接口，由Activity实现。
 *         从 android.app.Fragment 继承，3.0 以前的平台使用: 从 android.support.v4.app.Fragment 继承，并通过 FragmentActivity.getSupportFragmentManager() 获取管理器
 *     常见子类 -- DialogFragment, ListFragment, PreferenceFragment, WebViewFragment
 *     FragmentManager
 *       add/remove/replace -- 动态的增加、删除、替换Fragment
 *       addToBackStack() -- 在调用commit()前调用，可将事务添加到back栈，该栈由Activity负责管理，允许用户按BACK按键返回到前一个Fragment状态。
 *       findFragmentById 或 findFragmentByTag -- 获取 Fragment
 *     通常用法：
 *       静态: 在布局文件中使用 <fragment> 元素添加，通过 name 属性指定实现类;
 *       动态: 1. 设置要替换的占位元素Id：myContrainLayout.setId( ROOT_CONTAINER_ID);
 *               2. 将占位元素的内容换成自定义的Fragment( myGetFragment  可以由子类重载 )：
 *                  FragmentTransaction ft = getFragmentManager().beginTransaction();
 *                  ft.replace(ROOT_CONTAINER_ID , myGetFragment());	//使用自定义的fragment代替 container 组件
 *                  ft.commit();	//提交事务
 *      getActivity() -- 获取所在的Activity
 *      onActivityCreated() -- 当所在的Activity被启动完成后回调该方法
 *      onAttach() -- 当该Fragment被添加到Activity时，回调该方法，只会被调用一次。
 *      onCreate() -- 创建时回调，只会被调用一次。可初始化想要在Fragment中保持的必要组件，当fragment被暂停或停止后可以恢复
 *      onCreateView() -- 每次创建绘制界面组件时回调该方法，其返回值就是Fragment所显示的View。 一般重写该方法进行初始化。会调用多次?
 *      onDetach() -- 当该Fragment从它所属的Activity中被删除、替换完成时回调该方法，只被调用一次
 *      onDestroy() -- 销毁Fragment时被回调，只调用一次
 *      onDestroyView() -- 销毁该Fragment所包含的View组件时调用
 *      onPause()|onResume() -- 暂停 | 恢复 Fragment时被回调 
 *      onStart() | onStop() -- 启动 | 停止 Fragment时被回调
 *      常见场景的执行顺序(未实测):
 *         启动时:
 *         转到后台时:
 *         重回前台时:
 *         结束时:
 * +-Gallery(图库) -- 能水平方向显示其内容，而且用户可以拖动来切换列表项。一般和 ImageSwitcher 一起用来浏览图片，被选中的项位于中间。
 *      通常使用 Gallery 显示缩略图，在点击时通过 ImageSwitcher::setImageResource 切换到大图
 *      TODO: 已不再推荐该控件。推荐使用 HorizontalScrollView + ViewPager 来代替Gallery。
 *      animationDuration -- 设置图片切换时的动画持续时间
 *      unselectedAlpha -- 设置没有选中的图片的透明度，如 "0.6"
 *      ImageSwitcher::setOutAnimation(AnimationUtils.loadAnimation(this, android.R.anim.fade_out)); // 设置动画效果
 * +-Gesture -- 手势识别 
 * +-GridView -- 按照行列的方式显示内容，一般适合显示图片的缩略图)
 *     horizontalSpacing/verticalSpacing -- 各元素之间的 水平/垂直 间距
 *     numColumns -- 设置列数，默认为1。注意：行数是根据Adapter动态改变的
 *     stretchMode -- 设置拉伸模式，如 NO_STRETCH(不拉伸)， STRETCH_COLUMN_WIDTH(拉伸元素表格元素本身), STRETCH_SPACING_UNIFORM(表格元素、间距一起拉伸)
 * +-ImageButton -- 图片按钮(ImageView的子类，设置text无效)
 *     src -- "@drawable/iconempty"
 *     setImageResource(R.drawable.iconempty);
 * +-ImageView -- 显示图片等Drawable对象
 *     adjustViewBounds -- 设置是否调整自己的边界来保持所显示的图片的长宽比
 *     cropToPadding -- 设置是否裁剪到保留padding
 *     scaleType -- 设置显示的图片如何缩放或移动以适应ImageView的大小，
 *       center(图片放在中间，不进行缩放), centerCrop(保持纵横比缩放，使得图片能完全覆盖ImageView), centerInside(保持纵横比缩放，使得能完全显示图片)
 *       fitCenter(保持纵横比缩放，并将缩放后的图片放在中央) , fitXY(横向纵向独立缩放，图片完全适应该ImageView)，
 *       matrix(使用图像矩阵方式缩放),
 *     setOnTouchListener -- 设置Touch事件监听器 
 *     设置显示的内容
 *       setImageDrawable(getResources().getDrawable(R.drawable.right));   //设置显示的图片, 或通过 src 属性设置
 *       setImageResource(xxx); setImageBitmap(); setImageURI()
 * +-ListView/ListActivity -- 列表视图，以垂直列表的方式列出需要显示的列表项(如联系人名单、系统设置项等)
 *     关键点：设置Adapter。 若 ListActivty 中要使用自定义的界面布局文件，则其中必须有一个id为 "@+id/android:list" 的ListView。
 *     setTextFilterEnabled()/setFilterText()/clearTextFilter() -- 根据文本对列表项进行过滤显示  
 * +-MapView -- 显示Google地图
 * +-MultiAutoCompleteTextView -- 允许输入多个提示项(多个提示项以分隔符分隔)的自动提示，
 *    setTokenizer -- 设置分隔符，如  myAuto.setTokenizer(new MultiAutoCompleteTextView.CommaTokenizer()); //此处设置的分隔符是逗号?
 * +-Notification -- 显示在手机状态栏的通知(如 网络状态、电池状态、时间等)，程序通过 NotificationManager 服务来发送通知。
 *     setContentIntent -- 设置通知将要启动程序的 PendingIntent 信息
 * +-NumberPicker -- 数值选择器，用户即可通过键盘输入，也可通过拖动来选择
 *    focusable/focusableInTouchMode -- 似乎是通用的属性?
 * +-PagerTitleStrip -- 与 ViewPager 结合使用，在其上显示 "导航条" 
 * +-PopupWindow -- 可以创建类似对话框风格的窗口（浮动显示）, 在关闭按钮中调用 popup.dismiss() 来关闭 
 *    showAsDropDown(View v) -- 作为 v 组件的下拉组件显示出来;
 *    showAtLocation -- 在指定位置显示出来 
 * +-ProgressBar -- 进度条，有两个子类: SeekBar 和 RatingBar,  并且ProgressBar也有多种:
 *     1.ProgressDialog(对话框进度条)： a. new + show; b.覆盖Activity的onCreateDialog方法，并在其中创建进度条并返回, 调用showDialog方法显示; c.直接调用静态的 show 函数;
 *     2.ProgressBarIndeterminate(标题栏进度条)：调用 Activity.requestWindowFeature(Window.FEATURE_PROGRESS 或 FEATURE_INDETERMINATE_PROGRESS) 方法设置窗口有进度条的特征;
 *       调用 Activity.setProgressBarVisibility(带进度)/setProgressBarIndeterminateVisibility(不带进度) 显示进度条， Activity.setProgress 设置进度
 *     3.ProgressBar(普通进度条) -- 布局文件中声明ProgressBar，获取到变量后调用 incrementProgressBy 等方法设置进度
 *       style -- 指定风格, 如 : @android:style/Widget.ProgressBar.Horizontal--水平进度条; @android:style/Widget.ProgressBar.Inverse -- 普通大小的环形进度条; 
 *       indeterminate/indeterminateDuration -- 设置为true，表示不精确显示 进度 / 进度的持续时间
 *       progressDrawable -- 设置进度条的轨道对应的Drawable对象(为 LayerDrawable 对象, 通过 <layer-list><item id="@android:id/background" android:drawalbe="@drawable/no" /> 元素进行配置)
 *       max/progress -- 最大值/当前值
 * +-QuickContactBadge -- 显示关联到特定联系人的图片，单击图片时会打开相应联系人的联系方式界面
 *     assignContactFromEmail()/assignContactPhone()/assignContactUri() 
 * +-RadioButton(单选按钮) + RadioGroup(管理一组RadioButton) 
 *     checkedButton -- "@+id/sex1"
 *     orientation -- vertical,horizontal
 * +-RatingBar -- 星级评分条，从ProgressBar继承，通过星星表示进度，通过 OnRatingBarChangeLister 响应评分的改变
 *     isIndicator -- 设置为true表示不允许用户改变
 *     numStars -- 设置总共有多少个星级
 *     stepSize -- 设置每次最少需要改变多少个星级，如 "0.5"表示最少变化半个星级
 * +-SearchView -- 搜索框，允许用户在文本框内输入文字，当输入完成提交时，可通过监听器执行实际的搜索。在其下方放置一个ListView，并通过setFilterText设置显示的列表项，则可模拟自动完成的功能
 *       setIconifiedByDefault() -- 设置默认是否最懂缩小为图标
 *       setSubmitButtonEnabled -- 设置是否显示搜索按钮
 *       setQueryHint -- 设置搜索框内默认显示的提示文本
 * +-SeekBar -- 拖动条，继承自ProgressBar，通过滑块来标识数值，并且允许用户拖动滑块来改变值(响应 OnSeekBarChangeListener)
 *     thumb -- 指定一个Drawable对象，可自定义滑块的外观。 
 * +-StackView -- 以堆叠方式来显示多个列表项
 * +-Spinner -- 下拉列表，相当于弹出一个菜单供用户选择，如开发时已知可选内容，可直接设置entries属性为数组资源; 如需要在运行时设置，可通过Adapter提供
 *     entries -- 使用数组资源设置该下拉列表框的列表项目
 *     prompt -- 设置提示信息
 * +-Switch -- 开关
 *     switchTextAppearance -- 该开关图标上的文本样式
 *     thumb -- 指定使用自定义Drawable控制该开关的开关按钮
 *     track -- 指定使用自定义Drawable控制该开关的开关轨道
 * +-TabHost / TabActivity -- 选项卡，可显示多个标签页并进行切换。TabHost(选项卡) -> [ TabSpec(标签页面) + TabWidget(标签条) ]
 *     TODO: 新版本推荐使用 Fragment 代替 TabHost/TabActivity  
 * +-TextView -- 文本视图，显示字符串
 *     autoLink -- all(可以显示链接，如 http://)， 或 "email|phone" -- 对邮件、电话增加链接
 *     editable -- 控制是否允许编辑
 *     ellipsize -- 设置当显示的文本超过长度时如何处理文本内容，如 end(在文本结尾处截断显示省略号), marquee(滚动动画显示文本)
 *     text -- @string/str_id
 *     textColor -- @drawable/darkgray
 *    +-CheckedTextView -- 增加了checked状态
 * +-ToggleButton -- 状态开关按钮，拥有 checked 属性
 *     textOn/textOff -- 当按钮的状态 打开/关闭 时显示的文本
 * +-VideoView --
 * +-ViewPager-- Fragment容器，可以同时管理多个Fragment，并允许多个Fragment切换时提供动画效果, 可通过 FragmentPagerAdapter 提供多个Fragment  
 * +-WebView -- 内置浏览器控件，可直接加载网页。为响应超链接功能，调用 setWebViewClient 方法设置自定义的 WebViewClient 子类实例
 *     getSettings().setJavaScriptEnabled(true) -- 更改设置
 *     loadUrl -- 加载指定的URL地址网页
 * +-Window
 *     setFormat -- 设置窗口特征，如 PixelFormat.TRANSLUCENT(半透明)， 需要在 setContentView 之前调用
 * +-ZoomButton -- 放大/缩小 按钮，Android系统提供了 @android:drawable/btn_minus,btn_plus 两个资源，只需指定其 src 属性即可
 * +-ZoomControls -- 同时组合了 放大、缩小 两个按钮，并可分别绑定不同的事件监听器
***************************************************************************************************************************************/

/***************************************************************************************************************************************
 * Layout -- Android 通过 LayoutInflater/MenuInflater  等类将 XML 格式的布局文件中的组件解析为可视化的视图组件。
 *   会根据运行平台调整其中组件的位置、大小。其过程分为两步：measure(计算位置) -> layout(根据计算结果进行布局)
 *   布局文件中的 <requestFocus/> 项代表什么意思?
 * ViewGroup -- 虚拟父类
 *   gravity -- 控制布局管理器内组件的对齐方式，如 top, bottom, left, right, center, center_vertical, 等，多个属性可通过 "|" 组合
 *   layout_width/layout_height -- fill_parent(减去填充空白后等同于父容器),match_parent(等价于fill_parent?推荐) wrap_content(根据内容指定高宽),320px,80dip
 *   +-AbsoluteLayout(TODO:已废弃 ) -- 绝对位置定位，降低兼容性，维护成本高
 *       layout_x, layout_y -- 30px， 指定子组件的 X、Y 坐标
 *   +-FrameLayout -- 帧布局，组件从屏幕的左上角开始布局，多个组件层叠排序，后面的组件覆盖前面的组件。
 *                             为每个加入其中的组件创建一个空白的区域(称为一帧),每个子组件占据一帧。这些帧会根据gravity属性自动对齐。
 *                             可通过设置多个 layout_gravity="center" 的逐渐减小的子元素达到霓虹灯的效果(如同心圆环)
 *         foreground -- 设置该帧布局容器的前景图像(Drawable类型)
 *         foregroundGravity -- 定义绘制前景图像的gravity属性
 *     +-ViewAnimator--为其中的View切换提供动画效果，可通过 setInAnimation/setOutAnimation 设置进入和退出屏幕时使用的动画
 *        animateFirstView -- 设置显示第一个View组件时是否使用动画
 *        inAnimation/outAnimation -- 设置 显示/隐藏 组件时使用的动画
 *       +-ViewFlipper--指定多个View之间的切换效果，通过 addView 添加多个组件
 *       +-ViewSwitcher--在两个View之间切换(如Launcher界面的左右滚动切换)。可通过指定 ViewSwitcher.ViewFactory 来创建需要显示的View(重载其 makeView 方法)
 *         +-ImageSwitcher -- 使用更简单。1.提供一个ViewFactory,其生成的View必须是ImageView; 2.切换图片时，通过 setImageDrawable, setImageResource, setImageURI 等更换图片即可 
 *         +-TextSwitcher -- 显示文本内容，可在文本切换时动画效果。1.ViewFactory的makeView方法必须返回一个 TextView 组件； 切换文本时，通过 setText 设置即可
 *   +-GridLayout -- 4.0新增的网格布局,将容器划分为 rowsXcolumns 个网格，每个网格可以防止一个组件
 *       columnCount/rowCount -- 设置网格的 列数 、行数
 *       layout_column/layout_row -- 设置该子组件在GridLayout的第几列、第几行
 *       layout_columnSpan/layout_rowSpan -- 设置该子组件在GridLayout 横向、纵向上 跨几列、几行
 *   +-LinearLayout -- 线型布局，按照垂直或者水平方向布局组件。注意：水平排列时不会换行显示多余的组件。
 *       background -- "@drawable/testpic",
 *       divider -- 布局时的分隔条(Drawable对象，如 "?android:attr/dividerHorizontal" )，然后通过 showDividers= "middle"  等来显示
 *       layout_gravity -- 设置该子元素在父容器中的对齐方式，如 center_horizontal
 *       layout_weight -- 权重
 *       layout_alignParentLeft/layout_alignParentTop/ -- true 或 false
 *       layout_marginRight -- 80dip, 
 *       layout_below/layout_alignTop -- "@+id/ExitButton", 设置相对位置的元素ID
 *       layout_centerHorizontal -- true
 *       measureWithLargestChild -- 设置为true时，所有带权重的子元素都会具有最大子元素的最小尺寸
 *       orientation -- 排列方式, 如   vertical(垂直排列，默认值),horizontal
 *       
 *     +-TableLayout  -- 表格布局，继承自LinearLayout，按照行列方式布局组件( 采用 > TableRow > 元素 的方式)。
 *         若直接将元素放入TableLayout，则该元素将占满一行
 *           collapseColumns -- 设置需要被隐藏的列的列序号，多个列序号之间用逗号隔开
 *           shrinkColumns -- 设置允许被收缩的列的列序号，多个列序号之间用逗号隔开
 *           stretchColumns -- 设置允许被拉伸的列的列序号，多个列序号之间用逗号隔开，可保证组件能完全填满表格空余空间
 *   +-RelativeLayout -- 相对布局，相对其他组件的布局方式(如在其上下左右等)。可以拉伸自动适应，但会造成图像变形。
 *        layout_below, layout_alignParentBottom, layout_toRightOf
 *        ignoreGravity -- 指定哪个组件不受gravity属性的影响
 *   +-ScrollView/HorizontalScrollView -- 滚动视图, 为普通组件添加 垂直/水平 滚动条的组件，其中最多只能包含一个组件
 *   +-TableRow
 *
 * LayoutInflater infater = (LayoutInflater)getSystemService(Context.LAYOUT_INFLATER_SERVICE); //或LayoutInflater.from(getApplicationContext())
 * convertView = infater.inflate(R.layout.baseadapter_provider,null);
 * 
 * ViewTreeObserver -- View变化监听器(当View Tree 发生变化时会通知该监听器)，不能自定义实例，而必须通过 View.getViewTreeObserver()  获得
 *   
***************************************************************************************************************************************/

/**************************************************************************************************************************************
 * 对话框(Dialog) -- 也可以将 Activity 的风格设置为对话框风格( <activity android:theme="@android:style/Theme.Dialog"> )模拟对话框，需要在关闭按钮处理中调用 finish();
 *   AlertDialog(警告对话框，功能最丰富，实际应用最广)，其对话框有如下区域：图表区、标题区、内容区、按钮区，均可定制
 *     setPositiveButton(确定)、setNegativeButton(取消)、setNeutralButton(装饰性按钮) -- 设置按钮
 *     setItems(简单列表项)/setSingleChoiceItems(单选列表项)/setMultiChoiceItems(多选列表项) -- 设置对话框内容
 *     setAdapter -- 设置内容为自定义列表项，由该Adapter负责提供多个列表项组件
 *     setView -- 设置对话框内容为自定义View，可任意定制对话框内容(如 制作登录对话框 ):  
 *     示例: 
 *       AlertDialog.Builder dlgBuilder = new AlertDialog.Builder(this);  //创建Builder，然后可以依次设置各种参数
 *       dlgBuilder.setMessage(msg).setCancelable(false).setPositiveButton("确定", new DialogInterface.OnClickListener() { ...onClick(...){finish();} }); //设置builder
 *       AlertDialog alert = dlgBuilder.create();  //创建对话框实例
 *       alert.show(); //显示
 *       //TableLayout loginForm =(TableLayout)getlayoutInflater().inflate( R.layout.login, null);  xxx.setView(loginForm);  
 *  ColorPickerDialog -- 选择颜色对话框
 *  DatePickerDialog/TimePickerDialog(日期/时间 选择对话框) --  
 *    1.new 以后 show() 或 重载 Activity.onCreateDialog(int id) { switch(id) {...}  } 方法创建对话框;
 *    2.在 OnDateSetListener 和 OnTimeSetListener 的对应事件方法中响应日期和时间的设置;
 *    3.调用 Activity.showDialog 显示对话框
 *  
 * Toast -- 提示信息(没有交互功能，只是提示，一段时间后自动消失), 静态的makeText设置显示文本和时长
 *    1.Toast.makeText(getApplicationContext(), "提示信息", Toast.LENGTH_LONG).show;
 *    2.可通过构造函数创建实例后，调用 setView 设置要显示的内容，来自定义显示。
**************************************************************************************************************************************/

/***************************************************************************************************************************************
 * Activity  :  表示用户界面中的一个屏幕，显示由几个Views控件组成的用户接口，并对事件做出响应，可以在xml文件中定义所需要的Views或在代码中定义，
 *     Context <- ContextWrapper <- ContextThemeWrapper <- Activity
 *     注意：Activity必须在清单文件中声明才能使用
 *              配置时若指定其 exported 属性为true，则可被其他应用启动。可在 <action android:name> 中指定能响应Action为指定字符串的 Intent 
 *   每个Activity的状态由它所在Activity栈中的位置所决定，所有当前正在运行的Activity将遵循后进先出的原则。
 *   当一个新的Activity启动，当前的Activity将移至堆栈的顶部，如果用户使用Back按钮，或在前台Activity被关闭，下一个Activity将被激活并且移至到堆栈的顶部。
 *   如在 startActivity 时 setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP) 标志可清空已有的栈(即新起的Activity将成为最后一个，finish时将结束程序)
 *     TODO：API版本小于8？否则用 FLAG_ACTIVITY_NEW_TASK
 *     
 *   Activity状态 -- 状态过渡具有不确定性并且由Runtime完全管理
 *     Active状态：这时候Activity处于栈顶，且是可见的，有焦点的，能够接收用户输入前景Activity。
 *     Paused状态：Activity是可见的，但没有焦点。例如，如果有一个透明或非全屏幕上的Activity在你的Activity上面。
 *     Stopped状态：完全被遮住（不可见）时，将继续保留在内存中保持当前的所有状态和成员信息，但会是被回收对象的主要候选。
 *     Inactive状态：Activity被杀掉以后或者被启动以前，已被从Activity堆栈中移除，需要重新启动才可以显示和使用。
 *   监测Activity的状态变化
 *     onCreate -- 在Activity生命周期开始时被调用，初始化用户界面，分配引用类变量，绑定数据控件，并创建服务和线程 -- 通常需要 findViewById 将成员变量和控件建立关联。
 *     onRestore -- onCreate完成后被调用，用来回复UI状态
 *     onRestart -- 当activity从停止状态重新启动时调用(onStop->onRestart->onStart)
 *     onStart -- 当activity对用户即将可见的时候调用，可注册更新用户界面Intent接收者
 *     onResume -- 当activity将要与用户交互时调用此方法，此时activity在activity栈的栈顶，用户输入已经可以传递给它 
 *     onSave -- Activity即将移出栈顶保留UI状态时调用此方法
 *     onPause -- 当系统要启动一个其他的activity(如 接到电话 )时调用，这个方法被用来提交那些持久数据的改变、停止动画、和其他占用 CPU资源的东西。
 *     onStop -- 当另外一个activity恢复并遮盖住此activity,导致其对用户不再可见时调用。一个新activity启动、其它activity被切换至前景、当前activity被销毁时都会发生这种场景。可注销更新用户界面Intent接收者
 *     onDestroy -- 在activity被销毁前所调用的最后一个方法，有可能在某些情况下，一个Activity被终止时并不调用onDestroy方法。
 *   状态保存和恢复( Bundle savedInstanceState ) -- TODO: 什么时候调用?
 *     onSaveInstanceState --
 *     onRestoreInstanceState -- 
 *   常见场景的执行顺序(TODO: BackPress 和 Home 不一样 ?)，具体的生命周期可以参见图 ActivityLifeCycle.gif
 *     初次启动:    onApplyThemeResource -> {onCreate} -> onStart -> onPostCreate -> onResume -> onPostResume -> onAttachedToWindow
 *     退出:    [onBackPressed ->] onPause(切换时触发)  -> onStop(后台由系统选择时机触发) -> onDestroy -> onDetachedFromWindow
 *     Home到后台：onUserLeaveHint -> onSaveInstanceState(保存内容) -> onPause(另一个到前台) -> onCreateDescription -> onStop(不可见)
 *     到前台: {onRestart} ->onStart -> onResume -> onPostResume
 *     锁屏时(TODO: 未测试确认): onSaveInstanceState -> onPause
 *   加载方式( android:launchMode )
 *     standard(默认) -- 标准模式，启动时会创建新的Activity实例，并添加到当前Task栈中，不启动新的Task( Activity.getTaskId() 相同), 点击"返回"键时会逐一从栈顶删除Activity实例返回
 *     singleTop -- Task顶的单例模式, 当要被启动的目标Activity已经位于Task栈顶时，系统直接复用已有的实例。若目标Activity不在栈顶时，系统创建目标实例并加载到Task栈顶。
 *     singleTask -- Task内单例模式， 目标Activty在同一个Task内只有一个实例。a.目标不存在=>创建并加入栈顶;b.已经在栈顶=>同singleTop; c.存在且不在栈顶=>将其上所有Activity移出栈
 *     singleInstance -- 全局单例模式，系统保证无论从哪个Task中启动目标Activity，只会创建一个目标实例，其所在Task也只包含该Activity。
 *   常见属性
 *     ConfigChanges -- 可以使Activity捕捉设备状态变化, 如 orientation(设备旋转) 等
 *   常见方法
 *     findViewById() -- 根据ID查找组件的实例
 *     finish() -- 结束Activity, 通常用法为 MyActivity.this.finish();
 *     finishActivity(requestCode) -- 结束以 startActivityForResult 方法启动的Activity(具体意思 ?)
 *     getWindow() -- 返回该Activity所包含的窗口，如不调用 setContentView 来设置该窗口显示的内容，则显示为一个空窗口 
 *     setContentView -- 通常在重载的 onCreate 中调用来设置要显示的视图，从而实现与用户交互的功能
 *     setThemem -- 设置窗口风格(如 不显示ActionBar、以对话框形式显示窗口等)
 *     startActivity -- 启动并导航到另一个Activity，参数被封装在Intent中,
 *       Intent intent = new Intent(FirstActivity.this, SecondActivity.class);  通过Bundle设置参数;  startActivity(intent);  //在FirstActivity中启动SecondActivity
 *       在 SecondActivity::onCreate 中: Intent intent = getIntent(); Bundle bundle = intent.getExtras(); ...
 *     startActivityForResult -- 以等待返回结果的方式启动并导航到另一个Activity， 需要重载 onActivityResult 处理返回的 Intent 等结果。
 *                                       在SecondActivity中通过 setResult(resultCode, intent); 设置返回值， 然后 finish() 结束
 *   常见子类
 *     AccountAuthenticatorActivity -- 账户管理界面
 *     ExpandableListActivity -- 可展开列表，通过 ExpandableListAdapter 提供数据
 *     ListActivity
 *     LauncherActivity -- 应用启动列表, 每个列表项对应一个Intent，单击时自动启动对应的Activity, 需要重写 intentForPosition 方法来返回对应的 Intent
 *        示例: Class<?>[] classes = { AAA.class, BBB.class };  public Intent intentForPosition(int position) { return new Intent(MyLauncherActivy.this, classes[position]); } 
 *     MapActivity(还是 MainActiviy ?) --
 *     TabActivity 
 *     PreferenceActivity -- 显示设置选项参数并进行保存，需结合 PreferenceScreen + PreferenceFragment子类(3.0以后) 实现参数设置界面，然后自动通过 Preferences 进行保存
 *       1.UI配置xml文件: <preference-headers> <header fragment="xxx">  
 *       2.<PreferenceScreen><PreferenceCategory>
 *       3.Fragment 继承 PreferenceFragment, 在 onCreate 中调用 addPreferencesFromResource 方法加载指定的界面布局文件
 **************************************************************************************************************************************/

/**************************************************************************************************************************************
 * 按钮交互
 *   1. implements OnClickListener
 *      Button ResolutionTest = (Button)findViewById(R.id.ResolutionTest); 
 *      ResolutionTest.setOnClickListener(this);  //必须在 onClick中比较 getId才能处理多个按钮的点击情况
 *      public void onClick(View arg0) { 
 *          if (arg0.getId() == R.id.ResolutionTest)  { startTest(); } 
 *      } 
 *   2. 使用成员变量
 *       ResolutionTest.setOnClickListener(m_myButtonClick);
 *       Button.OnClickListener m_myButtonClick =  new Button.OnClickListener() {...}
 *   3. 使用匿名事件
 *       ResolutionTest.setOnClickListener(new Button.OnClickListener() {
 *           //@Override -- 是否需要这行
 *           public void onClick(View v) {
 *              startTest();  //不用比较getId,每个Button都可以有自己的OnClick
 *           }
 *       });
**************************************************************************************************************************************/

/**************************************************************************************************************************************
 * 1. 隐藏TitleBar 和 Status Bar -- TODO：需要在 setContentView(R.layout.start_test) 之前调用，否则会出现 force close 错误。
 *    在当前Activity的Java源文件引入android.view.Window和android.view.WindowManager，并在oncreate()方法中调用:
 *    requestWindowFeature(Window.FEATURE_NO_TITLE);   //Hide title bar;
 *    getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);  //全屏显示
 *    //getWindow().setBackgroundDrawableResource(R.color.red_bg); //设置背景色
 * 2.获取屏幕高宽
 *     	WindowManager windowManager = getWindowManager()
 *      Display display = windowManager.getDefaultDisplay();
 *      DisplayMetrics metrics = new DisplayMetrics();
 *      display.getMetrics(metrics);  
 *    	display.getWidth(), display.getHeight(); metrics.widthPixels; metrics.heightPixels;   //TODO: 区别 ?
 * 3. 通过资源中定义的id找到对应的对象 -- findViewById
 *    TextView ResultText = (TextView)findViewById(R.id.ResultText);
 *    
**************************************************************************************************************************************/

public class UITester extends ActivityUnitTestCase<Activity>{
	private final static String TAG = UITester.class.getSimpleName();
	private Intent mStartIntent;
	
	public UITester() {
		super(Activity.class);
	}


    @Override
	protected void setUp() throws Exception {
		super.setUp();
		mStartIntent = new Intent(Intent.ACTION_MAIN);
	}


	//界面切换 -- 有什么区别?标准应该是使用第二种方法（虽然麻烦，但更合理和）
    //  1.切换layout -- 定义不同的layout xml文件,通过 setContentView方法 进行切换; 还在同一个Activity中，成员变量不变。
    //  2.切换Activity -- 定义不同的layout xml，实现不同的Activity子类(显示对应的layout),通过 Intent类 进行切换。
    //  注意需要在AndroidManifest.xml中定义新的activity，并设置那么属性，否则无法编译。
    public void TestSwitchUI() {
        /*************************************************************************************************
         * Intent intent = new Intent();// new一个Intent对象
         * intent.setClass(EX03_09.this, EX03_09_1.class); //指定要启动的class
         * startActivity(intent);// 调用一个新的Activity -- 类似非模态？
         * //startActivityForResult(intent,0); // 被调用Activity要返回值的依据,
         * 启动新的Activity并等待其结束， // 并通过重载 onActivityResult 函数判断返回值(resultCode ==
         * RESULT_OK), 可以通过 data.getExtras 获取数据， 具有层次关系 // 子Activity中通过如下方式返回结果:
         * // EX03_09_1.this.setResult(RESULT_OK, intent); //
         * 返回result回上一个activity // EX03_09_1.this.finish();// 结束这个activity
         * EX03_09.this.finish(); // 关闭原本的Activity(根据具体情况)
         **************************************************************************************************/
    }
    
    public void testTabHost(){
        /********************************************************************************************
         * 方法1:
         *   1. 在布局文件中使用 FrameLayout 列出Tab组件及Tab中的内容组件
         *   2. Activity 要继承 TabActivity;
         *   3.调用 TabActivity::getTabHost 获得 TabHost 对象，然后创建 Tab 选项页
         * 方法2:
         *   1.实现接口 TabHost.TabContentFactory 的 createTabContent 方法来指定
         *     public class MyActivity extends TabActivity implements TabHost.TabContentFactory { 
         *          onCreate 中:  getTabHost().addTab(getTabHost().newTabSpec("all").setContent(this);
         *          public View createTabContent(String tag) { ...; if(tag.equals("all")) {  设置all页面对应的内容  }  } 
         ********************************************************************************************/
        
        /*******************************************************************************************
        //代码部分
            TabHost tabHost = getTabHost();
            LayoutInflater.from(this).infalte(R.layout.main, tabHost.getTabContentView(), true);
            tabHost.addTab(tabHost.newTabSpec("all").setIndicator("所有通话记录").setContent(R.id.TextViewAll));
            tabHost.addTab(tabHost.newTabSpec("ok").setIndicator("已接来电").setContent(R.id.TextViewReceived));
            tabHost.addTab(tabHost.newTabSpec("cancel").setIndicator("未接来电").setContent(R.id.TextViewUnreceived));

         //布局部分
           <FrameLayout ....>
                 <TabHost>
                   <TextView android:id="@+id/TextViewAll" android:layout_width="wrap_content" android.text="所有通话记录">
                   </TextView>
                 </TabHost>
           </FrameLayout>
         *******************************************************************************************/
    }
    
    public void testTouchImage(){
    	//setOnTouchListener -- 在图片上通过触摸更改位置、大小等(不完整， 而且可能会有性能问题？)
    	/*
    	public boolean onTouchEvent(View view, MotionEvent event){
	    	ImageView image1 = new ImageView();
	    	BitmapDrawable bitmapDrawable = (BitmapDrawable)image1.getDrawable();
	    	Bitmap bitmap = bitmapDrawable.getBitmap();  //获取位图
	    	double scale  = bitmap.getWidth() / 320.0; // 获得缩放比例
	    	//计算要显示的图片的开始点（注意增加范围检查）
	    	int x = (int)(event.getX() * scale);
	    	int y = (int)(event.getY() * scale);
	    	image2.setImageBitmap(Bitmap.createBitmap(bitmap, x, y, 120, 120));  //从指定开始位置创建要显示的子位图，并显示
	    	return false;
    	}
    	*/
    }
    
    public void GridViewDisplayImageTester(){
        //使用 BaseAdapter 类，重写其中的方法( 如 getView 来设置图片显示格式 )
        /********************************************************************************************
        //自定义的 BaseAdapter 类，根据需要返回并图片的数据信息
            class MyAdapter extends BaseAdapter{
                private Integer [] imgs = {
                        R.drawable.ic_launcher
                };
                Context context;
                MyAdapter(Context context){
                    this.context = context;
                }
                @Override
                public View getView(int position, View convertView, ViewGroup parent) {
                    ImageView imageView;
                    if(convertView == null){
                        imageView = new ImageView(context);
                        imageView.setLayoutParams(new GridView.LayoutParams(45, 45));    //设置ImageView对象布局
                        imageView.setAdjustViewBounds(false);         //设置边界对齐
                        imageView.setPadding(8,  8,  8,  8);                //设置间距
                    }
                    else{
                        imageView = (ImageView)convertView;
                    }
                    imageView.setImageResource(imgs[position]);    //为 ImageView 设置图片资源
                    return imageView;
                }
            }
        //Activity 子类中，使用自定义的 Adapter 在 GridView 中显示图片
            GridView gridView = (GridView)findViewById(R.id.xxx);
            gridView.setNumColumns(4);     //设置列数
            gridView.setAdapter(new MyAdapter(this));
        *********************************************************************************************/
    }
    
	// 全屏显示窗口并获取屏幕高宽等, Activity.onCreate 中
    public void testFullScreen(){
    	
    	/*
    	// 隐去标题栏（程序的名字）, 等价于 android:theme="@android:style/Theme.NoTitleBar"
    	requestWindowFeature(Window.FEATURE_NO_TITLE);
    	  
    	//隐去电池等图标和一切修饰部分（状态栏部分）， 等价于 android:theme="@android:style/Theme.NoTitleBar.Fullscreen"
    	getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);  
    		
    	Display display = getWindowManager().getDefaultDisplay();
    	display.getWidth(), display.getHeight();
    	
    	setContentView(xxxx);
   		//*/
    }
    
    public void testDisplayInfo(){
    	startActivity(mStartIntent, null, null);
    	Activity activity = getActivity();
    	assertNotNull(activity);
    	
    	Display display = activity.getWindowManager().getDefaultDisplay();
    	assertNotNull(display);
    	Log.i(TAG, "Display=" + LogHelper.FormatDisplay(display));
    	
    	Rect frame = new Rect();
    	getActivity().getWindow().getDecorView().getWindowVisibleDisplayFrame(frame);
    	
    	//frame.top 是状况栏高度
		Log.i(TAG, "DecorViewSize = " + LogHelper.FormatRect(frame) );
    }
}

