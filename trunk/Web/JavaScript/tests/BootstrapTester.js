"use strict";

/******************************************************************************************************************************************
* ѧϰ��: http://www.w3cschool.cc/bootstrap/bootstrap-forms.html
*
* ���ʵ��
*   1.���ߵĿ��ӻ�����: http://www.w3cschool.cc/try/bootstrap/layoutit/
*     Pingendo(http://www.pingendo.com/): ����Java����
*     TODO: a.�����Ƿ��й���? b.�Ƿ��б��صĹ���?  http://www.w3cschool.cc/bootstrap/bootstrap-ui-editor.html
*       
*     bootstrap ui �༭��,��ֱ�ӳ�bootstrap���ִ���
*   2. -- 
*     
* ��������:
*   1.data-toggle -- TODO: ��������״̬���л����� button, buttons, collapse(), dropdown(�����˵�),modal(), tab(), pill
*
* �ٷ���վ --  http://getbootstrap.com/ 
* ������վ -- http://www.bootcss.com/indexAction.java
* ��Ƶ ?  http://www.icoolxue.com/album/show/78
*
* TODO:
*   2.
*   3.Ŀǰ���°汾��3.2(������ bootstrap.css �� bootstrap-theme.css); ���ϵ�2.3�汾������ bootstrap.css �� bootstrap-responsive.css
*     
******************************************************************************************************************************************/

/******************************************************************************************************************************************
* �ƶ��豸���Ȳ���
*   ���� -- ����ʲô������Ҫ��
*   ��Ӧʽ����(responsive) -- �ɸ�����Ļ����Զ�����UI����
*     ������ CSS ���ƶ��豸���ȣ�ý���ѯ�������ƽ����ԡ�̨ʽ����,��������CSS����,��������ӿڴ�С�ƶ�����ʾ���������ݡ�
*     �﷨: @media (min-width: @screen-sm-min) and (max-width: @screen-sm-max) { ... }
*       ��:  @media (min-width: @screen-xs-min) { ��С���豸: <768px }
*            @media (min-width: @screen-sm-min) { С���豸: ƽ����ԣ�>= 768px }
*            @media (min-width: @screen-md-min) { �����豸: ̨ʽ���ԣ�>= 992px }
*            @media (min-width: @screen-lg-min) { �����豸: ��̨ʽ���ԣ�>=1200px }
*          
*   ������ǿ -- ������Ļ��С�����Ӷ����Ԫ��
******************************************************************************************************************************************/

/******************************************************************************************************************************************
*   
******************************************************************************************************************************************/

/******************************************************************************************************************************************
* Bootstrap
*   Twitter�����Ŀ�ԴWebǰ�� CSS/HTML ���, ����HTML5��CSS3����, ���ݴ󲿷�jQuery�����
*   ��Bootstrap 3 ��ʼ, ���Ŀ�����ƶ��豸���ȣ�Ȼ����������豸�����������µ�����ϵͳ���ƶ���������кܺõĹ���(��ҪIE8������)
*      
*
* �����ṹ -- �ṩ��һ����������ϵͳ��������ʽ�������Ļ����ṹ
* Bootstrap Less -- ��̬CSS����,��һ�� CSS Ԥ������,�� CSS ���ж�̬��, ����CSS��ʹ��Less����,mixins(���),nesting(Ƕ��)
* ��Ӧʽ���(V3.0�Ժ�ɾ����?) -- ��Ӧʽ CSS �ܹ�����Ӧ��̨ʽ����ƽ����Ժ��ֻ�
* ����(V3.0�Ժ�����?) -- bootstrap-theme
* ֧�ֶ��� -- ���Զ��� Bootstrap �������LESS ������ jQuery ������õ����Լ��İ汾
******************************************************************************************************************************************/

/******************************************************************************************************************************************
* ʹ��:
*   0.������ص���Ԥ�������ѹ���汾,����ֱ��ʹ��. ������ص���Դ�ļ�,��Ҫͨ��Recess����LESS�ļ������ɿ����õ�CSS�ļ�.
*     Recess(http://twitter.github.io/recess/)
*   1.Bootstrapʹ����HTML5Ԫ�غ�CSS,�����Ҫʹ�� HTML5 �ĵ�����(Doctype),������ܻ��������ʾ��һ�¡����ļ�ͷ����:
*     <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
*   2.�� <head> ���ְ���������Ϣ
*     a.�� Bootstrap ��������վ���ƶ��豸�Ѻã�ȷ���ʵ��Ļ��ƺʹ������� 
*       <meta name="viewport" content="width=device-width, initial-scale=1.0">
*       ע��: ���ƶ��豸������ϣ�ͨ��Ϊ viewport meta ��ǩ��� maximum-scale=1.0, user-scalable=no ���Խ���������(zooming)����
*              ʵ����Chrome�ϲ���Ч(��Ȼ������)
*     b.���� css �ļ� -- bootstrap.min.css, bootstrap-responsive.min.css(��Ӧʽ����)
*       <link href="bootstrap.min.css" rel="stylesheet">
*   3.���� jQuery �� bootstrap �� JavaScript �ļ�. TODO:JavaScript �������ĵ���������ʹҳ������ٶȸ���
*       <script src="jquery.min.js"/>
*       <script src="bootstrap.min.js"/>
*   4.TODO:
*     Ȼ������� HTML Ԫ����Ӧ�� class="xxxx" , �� <div class="table-responsive"> ?
******************************************************************************************************************************************/

/******************************************************************************************************************************************
* Bootstrap CSS -- Ԥ�����˺ܶ�Ĭ�ϵ�����(��ֻҪ������ bootstrap.css, �ͻḲ��Ĭ�ϵ�����)
*   class
*     btn|btn-* : ָ����ť�ĸ��ַ��ʹ�С
*     caret: ʹ�ò������ʾ�������ܺͷ����� <p>�����˵�<span class="caret"></span></p>
*     center-block�� ����Ԫ��Ϊ display:block ��������ʾ
*     checkbox-inline: ���Ƹ�ѡ����ͬһ��
*     clearfix: �������
*     col-*-* : ���ڱ��ϵͳ,���е��Ǻ�(*)����ָ�� �豸����(С��sm, ����md, ����lg)������,�� col-sm-3 col-md-6, col-lg-8 ��,
*        ���col-* ���Ϳ�������ͬһ��Ԫ��, ϵͳ����ݴ����С�Զ�Ӧ�ú��ʵ����͡�һ����˵��ͬһ���е�������Ӧ��Ϊ12?
*     container|container-fluid : ����,���ڰ���ҳ���ϵ����ݣ�ȷ�����к�����ȡ�ע��: Ĭ������²���Ƕ��.
*     has-success|has-error|has-warning : ��֤״̬
*     img-responsive : ��Ӧʽͼ��, ������ʽ����(display:inline-block), ͼ�񰴱�������,�������丸Ԫ�صĳߴ�
*     img-rounded|img-circle: ͼƬԲ��(border-radius:6px)|Բ��ͼƬ(border-radius:500px)
*     img-thumbnail:���һЩ�ڱ߾ࣨpadding����һ����ɫ�ı߿�
*     input-*-* : �������ñ��������ڱ��ϵͳ�еĸ߶ȺͿ��
*     navbar-left|navbar-right :���뵼�����е����
*     pull-left|pull-right: �ظ��������|�ұ�
*     radio-inline: ���Ƶ�ѡ����ͬһ��
*     row|row-fluid : ����ϵͳ�е���
*     small : �����ı�Ϊ���ı���С�� 85%���������� h1~h6 �ı����ڲ�����ɫ��ǳһЩ��Ҳ����ͨ�� <small> ��ǩָ����Ԫ��
*     text-success|text-info|text-warning|text-danger: �����ı���֤״̬,
*     text-muted|text-primary: ����������ɫ
*     table|table-xxx : ���ڱ���, �� table(������ڱ߾��ˮƽ�ָ���), table-striped(�������), table-bordered(�߿�), table-hover(�����ͣ), table-condensed(���ձ��,�ڱ߾����)
*     table-responsive: ��Ӧʽ���,һ�����ڰ���<table>��<div>��, �����ñ��ˮƽ�������Զ���ӦС���豸(<768px),�ڴ���768px����豸�ϲ鿴ʱ���������
*     visible-*-*|hidden: ���ƶԲ�ͬ��Ļ��С�Ŀɼ���, ��һ���Ǻ�(*)����Ļ����(xs|sm|md|lg), �ڶ����Ǻ�(*)ָ��display����(block|inline|inline-block)
******************************************************************************************************************************************/


/******************************************************************************************************************************************
* ����ϵͳ(Grid System) -- ��Ӧʽ���ƶ��豸���ȵ���ʽ����ϵͳ,������Ļ���ӿڣ�viewport���ߴ�����ӣ�ϵͳ���Զ���Ϊ���12�С�
*   1.�б�������� .container �ڣ��Ա����ʵ��Ķ���(alignment)���ڱ߾�(padding)
*   2.����Ӧ�÷��������ڣ���Ψ���п������е�ֱ����Ԫ��
*
* ��(Form)
*   1.�� <form> Ԫ����� role="form";
*     ����form: ָ�� class .form-inline , ������룬��ǩ����;
*     ˮƽform: ָ�� class .form-horizontal, ���ǩ��� class .control-label
*   2.�ѱ�ǩ�Ϳؼ�����һ������ class .form-group �� <div> �л�ȡ��Ѽ��
*   3.�����е��ı�Ԫ�� <input>��<textarea> �� <select> ��� class .form-control
*
* ����ͼ��(Glyphicons) -- ��Web��Ŀ��ʹ�õ�ͼ������,�� fonts Ŀ¼�µ� *.eot,*.svg,*.ttf, *.woff ���ļ�
*   http://www.w3cschool.cc/try/demo_source/bootstrap3-glyph-icons.htm
*   
*
* ���ò������(��Ҫ�Ƕ���������� CSS class):
*   �����˵�(class=dropdown): ���б��ʽ��ʾ���ӵ������Ĳ˵���
*     �� class=dropdown �ڼ������²˵�����,һ���� button(class=dropdown-toggle) + ul(class=dropdown-menu, role=menu) + li(role=menuitem)
*   ��ť��(btn-group|btn-group-vertical|btn-toolbar): �����ť���ѵ���ͬһ�л����ϡ�
*     class=btn-toolbar + class=btn-group + class=btn 
*   ��ť�����˵�: .btn-group + button(.caret, data-toggle="dropdown") + .dropdown-menu
*   �������:����������ı�������������Ϊǰ׺�ͺ�׺���ı���ť
*     div(.input-group) + span(.input-group-addon) + input
*   ����: ��ǩʽ�ĵ����˵� ul(.nav .nav-tabs[���ʽ]|.nav-pills[����ʽ]|.nav-stacked[��ջʽ] ) + li
*     ����� .nav-justified  �����˶���
*   ������: nav(.navbar .navbar-default role=navigation) + div(.navbar-header) + a(navbar-brand) + ul(.nav .navbar-nav) + li(
*   ���м����(Breadcrumbs):������վ�����Ϣ����ʾ��ʽ
*     ol(.breadcrumb) + li
*   ��ҳ: ul(.pagination) + li
*   ��ҳ: ul(.pager) + li
*   �Ű�
*   ����ͼ
*   ����Ի���
*   ������
*   ý�����
* 
* ����jQuery���
*   ģʽ�Ի���
*   ��ǩҳ
*   ������
*   ������
******************************************************************************************************************************************/

/******************************************************************************************************************************************
* Bootstrap Դ�����
*   normalize.less -- ͨ������Ĭ����ʽ(�� body { margin: 0;} )���������������һ����
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
