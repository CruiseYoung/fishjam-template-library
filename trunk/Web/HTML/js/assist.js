  /*
	��������
	1,�Զ�����
	2,��Ԫ����Ӷ���
  */

$(document).ready(function() {

    //�Զ����Tooltip��ʾ(ϵͳ����ʾЧ����Ӧ�Ƚ���,����û�ж���Ч��)
    var tooltipOffsetX = 10;
    var tooltipOffsetY = 20;
    $("a.tooltip").mouseover(function(e) {
        this.myOldTitle = this.title;      //����һ���µı���������ԭ���� title ֵ
        this.title = "";
        var tooltip = "<div id='tooltip'>" + this.myOldTitle + "<\/div>"; //���� div Ԫ��
        $("body").append(tooltip); //����׷�ӵ��ĵ���
        $("#tooltip")
			    .css({
			        "top": (e.pageY + tooltipOffsetY) + "px",
			        "left": (e.pageX + tooltipOffsetX) + "px"
			    }).show("fast");   //����x�����y���꣬������ʾ
    }).mouseout(function() {
        this.title = this.myOldTitle;
        $("#tooltip").remove();   //�Ƴ� 
    }).mousemove(function(e) {
        $("#tooltip")
			    .css({
			        "top": (e.pageY + tooltipOffsetY) + "px",
			        "left": (e.pageX + tooltipOffsetX) + "px"
			    });
    });




    //��ַ��������е���ʾ��TODO�������������ɫ����(�����ǻ�ɫ)
    //var addrPromptText = "�����������ַ";
    //var passwdPromptText = "��������������";

    $("#jqueryEval, #address, #password").focus(function() {         //��ý���
        var txt_value = $(this).val();   // �õ���ǰ�ı����ֵ
        if (txt_value == this.defaultValue) {   //ע��ʹ�õ� DOM ��������
            $(this).val("").removeClass("translucence");              // �������������������ı�������
            //$(this).css("font-color", "red");
        }
    });
    $("#jqueryEval, #address, #password").blur(function() {		  // ʧȥ����
        var txt_value = $(this).val();   // �õ���ǰ�ı����ֵ
        if (txt_value == "") {
            $(this).val(this.defaultValue).addClass("translucence"); // �����������������������
            // $(this).css("color", "#7f7f7f");
            //$(this).css("opacity", "0.5");
        }
    })

    $("#jqueryCheckBtn").click(function() {
        //alert("aaaaaaa");
        //alert($("#jqueryEval").val());
        alert($(this).offset().left + ", " + $(this).offset().top);
    })

    //�ֶ�����ҳ��Ԫ��
    $("#reset").click(function() {
        $("*").removeAttr("style");
        $("div[class=none]").css({ "display": "none" });
    });

    //�ж��Ƿ��Զ�����
    $("input[type=button]").click(function() {
        //if($("#realodwindow").is(":checked")){
        //   window.location.reload();
        //}else 
        if ($("#isreset").is(":checked")) {
            $("#reset").click();
        }
    });

    //TODO: �¼ӵ� -- ������Ϻ����ϼ��� click, ��֤��Ĭ��Ϊѡ��״̬(�� HTML �������� checked="checked" �޷����ó�ʼֵ)
    $("#isreset").trigger('click');

    //��idΪmover��Ԫ����Ӷ���.
    function animateIt() {
        $("#mover").slideToggle("slow", animateIt);
    }
    animateIt();
})