  /*
	��������
	1,�Զ�����
	2,��Ԫ����Ӷ���
  */

$(document).ready(function() {
    //��ַ��������е���ʾ��TODO�������������ɫ����(�����ǻ�ɫ)
    //var addrPromptText = "�����������ַ";
    //var passwdPromptText = "��������������";
    
    $("#address").focus(function() {         // ��ַ������꽹��
        var txt_value = $(this).val();   // �õ���ǰ�ı����ֵ
        if (txt_value == this.defaultValue) {
            $(this).val("");              // �������������������ı�������
            //$(this).css("color", "red");
        }
    });
    $("#address").blur(function() {		  // ��ַ��ʧȥ��꽹��
        var txt_value = $(this).val();   // �õ���ǰ�ı����ֵ
        if (txt_value == "") {
            $(this).val(this.defaultValue); // �����������������������
           // $(this).css("color", "#7f7f7f");
        }
    })

    //TODO: �ƺ����������ʾ����? -- ��ô����?
    $("#password").focus(function() {
        var txt_value = $(this).val();
        if (txt_value == this.defaultValue) {
            $(this).val("");
        }
    });
    $("#password").blur(function() {
        var txt_value = $(this).val();
        if (txt_value == "") {
            $(this).val(this.defaultValue);
        }
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