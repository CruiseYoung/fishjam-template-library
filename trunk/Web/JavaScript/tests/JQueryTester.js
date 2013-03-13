/********************************************************************************************
* http://jquery.com/
* jQuery JQuery ��ѡ���Դ(ʹ��MIT���Э��)����һ����������JavaScript��(�����ֻ��20��K)�����ݸ��������,����ؼ��� JavaScript ���
*   �ܷ���ش���HTMLԪ�ء��¼��ȣ�ʵ�ֶ���Ч����Ϊ��վ�ṩAJAX������ʹ��HTMLҳ���ִ����ҳ�����(���ò���js���е��ã�ֻ�趨��id����)
*
* ʹ�ã�head�а��� <script type="text/javascript" src="jquery.js"></script>
*       jquery.js ���Ա��浽���ػ��߷�����ֱ�����ã�Ҳ����ֱ��ʹ��CDN�ṩ��(�ŵ�:�û��Ѿ����ʹ�ʱ�Ỻ��;ȱ��:��һ������?)
*       Google CDN: http://ajax.googleapis.com/ajax/libs/jquery/1.8.0/jquery.min.js  -- ע��1.8.0 ����ʡ�Գ� 1.8 �� 1 ��(�Զ�ѡ������Ҫ�����߰汾)
*   �����汾��Production version -- �ѱ������ѹ��������ʵ�ʵ���վ
*             Development version -- δѹ�������ڲ��ԺͿ���
*     
* �����﷨��$(selector).action() -- ��Ԫ���Ŷ��� jQuery; ѡ���(selector)��ѯ�Ͳ���HTML Ԫ��; action() ִ�ж�Ԫ�صĲ���
*   $(document).ready(function(){ xxx }); -- 
*   .ready(function() { xxx });
*   .click(function() { xxx });
*  
* ���(ģ�黯��ʹ�÷�ʽ)��
*       
* ѡ����(Selectors)
*   $(this). -- ѡ��ǰԪ��
*   $("#test") -- ѡ��  id="test" ��Ԫ��
*   $(".test") -- ѡ������ class="test" ��Ԫ��
*   $("����") -- ѡ�����и����͵�Ԫ�أ� ������ <p> �� $("p")
*   
    

********************************************************************************************/

$(document).ready(function() {
    $("button").click(function() {
        $("p").hide();
    });
});

module("JQueryTester", {
    setup: function() {
        //dump("in JQueryTester::setup\n");
    },
    teardown: function() {
        //dump("in JQueryTester::teardown\n");
    }
});

test("TODO: JQuery", function() {
    equal(1, 1, "TODO:");
});
