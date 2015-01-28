"use strict";

/******************************************************************************************************************************************
* 学习到: http://www.w3cschool.cc/bootstrap/bootstrap-forms.html
*
* 最佳实践
*   1.在线的可视化布局: http://www.w3cschool.cc/try/bootstrap/layoutit/
*     TODO: a.官网是否有工具? b.是否有本地的工具?  http://www.w3cschool.cc/bootstrap/bootstrap-ui-editor.html
*     bootstrap ui 编辑器,可直接成bootstrap布局代码
*   2. -- 
*     
*   
*
* 官方网站 --  http://getbootstrap.com/ 
* 中文网站 -- http://www.bootcss.com/
* 视频 ?  http://www.icoolxue.com/album/show/78
*
* TODO:
*   2.响应式布局(responsive) --
*   3.目前最新版本是3.2(里面是 bootstrap.css 和 bootstrap-theme.css); 而老的2.3版本里面是 bootstrap.css 和 bootstrap-responsive.css
*     
******************************************************************************************************************************************/

/******************************************************************************************************************************************
* 移动设备优先策略
*   内容 -- 决定什么是最重要的
*   布局 -- 优先设计更小的宽度
*            基础的 CSS 是移动设备优先，媒体查询是针对于平板电脑、台式电脑,有条件的CSS规则,允许基于视口大小移动、显示并隐藏内容。
*            语法: @media (min-width: @screen-sm-min) and (max-width: @screen-sm-max) { ... }
*              如: @media (min-width: @screen-sm-min) { 小型设备: 平板电脑，>= 768px }
*                   @media (min-width: @screen-md-min) { 中型设备: 台式电脑，>= 992px }
*                   @media (min-width: @screen-lg-min) { 大型设备: 大台式电脑，>=1200px }
*          
*   渐进增强 -- 随着屏幕大小的增加而添加元素
******************************************************************************************************************************************/

/******************************************************************************************************************************************
* Bootstrap
*   Twitter开发的开源Web前端 CSS/HTML 框架, 基于HTML5和CSS3开发, 兼容大部分jQuery插件。
*   从Bootstrap 3 开始, 设计目标是移动设备优先，然后才是桌面设备，可以在最新的桌面系统和移动端浏览器中很好的工作(需要IE8及更高)
*      
*
* 基本结构 -- 提供了一个带有网格系统、链接样式、背景的基本结构
* Bootstrap Less -- 动态CSS语言,是一个 CSS 预处理器,让 CSS 具有动态性, 可在CSS中使用Less变量,mixins(混合),nesting(嵌套)
* 响应式设计(V3.0以后删除了?) -- 响应式 CSS 能够自适应于台式机、平板电脑和手机
* 主题(V3.0以后新增?) -- bootstrap-theme
* 支持定制 -- 可以定制 Bootstrap 的组件、LESS 变量和 jQuery 插件来得到您自己的版本
******************************************************************************************************************************************/

/******************************************************************************************************************************************
* 使用:
*   0.如果下载的是预编译过的压缩版本,可以直接使用. 如果下载的是源文件,需要通过Recess编译LESS文件来生成可重用的CSS文件.
*     Recess(http://twitter.github.io/recess/)
*   1.Bootstrap使用了HTML5元素和CSS,因此需要使用 HTML5 文档类型(Doctype),否则可能会浏览器显示不一致。在文件头加入:
*     <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
*   2.在 <head> 部分包含以下信息
*     a.让 Bootstrap 开发的网站对移动设备友好，确保适当的绘制和触屏缩放 
*       <meta name="viewport" content="width=device-width, initial-scale=1.0">
*       注意: 在移动设备浏览器上，通过为 viewport meta 标签添加 maximum-scale=1.0, user-scalable=no 可以禁用其缩放(zooming)功能
*              实测在Chrome上不生效(仍然能缩放)
*     b.引入 css 文件 -- bootstrap.min.css, bootstrap-responsive.min.css(响应式布局)
*       <link href="bootstrap.min.css" rel="stylesheet">
*   3.引入 jQuery 和 bootstrap 的 JavaScript 文件. TODO:JavaScript 放置在文档最后面可以使页面加载速度更快
*       <script src="jquery.min.js"/>
*       <script src="bootstrap.min.js"/>
*   4.TODO:
*     然后就是在 HTML 元素上应用 class="xxxx" , 如 <div class="table-responsive"> ?
******************************************************************************************************************************************/

/******************************************************************************************************************************************
* Bootstrap CSS -- 预定义了很多默认的类型(即只要包含了 bootstrap.css, 就会覆盖默认的属性)
*   class
*     col-*-* : 用于表格系统,其中的星号(*)用于指定 设备类型(小型sm, 中型md, 大型lg)和列数,如 col-sm-3 col-md-6, col-lg-8 等,
*        多个col-* 类型可以修饰同一个元素, 系统会根据窗体大小自动应用合适的类型。一般来说，同一行中的列数和应该为12?
*     container|container-fluid : 容器,用于包裹页面上的内容，确保居中和最大宽度。注意: 默认情况下不可嵌套.
*     img-responsive : 响应式图像, 内联形式呈现(display:inline-block), 图像按比例缩放,不超过其父元素的尺寸
*     pagination
*     row|row-fluid : 网格系统中的行
*     small : 设置文本为父文本大小的 85%。可以用于 h1~h6 的标题内部，颜色会浅一些。也可以通过 <small> 标签指定子元素
*     text-success|text-info|text-warning|text-danger -- 用于文本
*     table|table-xxx : 用于表格的, 如 table(表格有内边距和水平分割条), table-striped(间隔条纹), table-bordered(边框), table-hover(鼠标悬停), table-condensed(紧凑表格,内边距减半)
*     table-responsive: 响应式表格,一般用在包含<table>的<div>中, 可以让表格水平滚动以自动适应小型设备(<768px),在大于768px宽的设备上查看时看不到差别
******************************************************************************************************************************************/


/******************************************************************************************************************************************
* 网格系统(Grid System) -- 响应式、移动设备优先的流式网格系统,随着屏幕或视口（viewport）尺寸的增加，系统会自动分为最多12列。
*   1.行必须放置在 .container 内，以便获得适当的对齐(alignment)和内边距(padding)
*   2.内容应该放置在列内，且唯有列可以是行的直接子元素
*
*
* 内置Web组件:
*   下拉菜单
*   按钮组
*   按钮下拉菜单
*   导航
*   导航条
*   面包屑
*   分页
*   排版
*   缩略图
*   警告对话框
*   进度条
*   媒体对象
* 
* 内置jQuery插件
*   模式对话框
*   标签页
*   滚动条
*   弹出框
******************************************************************************************************************************************/

/******************************************************************************************************************************************
* Bootstrap 源码分析
*   normalize.less -- 通过设置默认样式(如 body { margin: 0;} )来建立跨浏览器的一致性
******************************************************************************************************************************************/


module("BootstrapTester", {
    setup: function() {
        //dump("in BootstrapTester::setup\n");
    },
    teardown: function() {
        //dump("in BootstrapTester::teardown\n");
    }
});

test("BootstrapTester", function() {
	equal(1, "1", "BootstrapTester");
});
