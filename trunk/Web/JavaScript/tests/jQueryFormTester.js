"use strict";

/******************************************************************************************************************************************
* jQueryForm(jquery.form.js) -- ������ʹ��ajax�����ύ��ʱ�����ݴ�������(����Ҫ�����JS�ķ�ʽ��ȡ�����Ե�ֵ��ƴ�ӳɲ�ѯ�ַ�����ʽ)
*   $("#myForm'). ����:
*     ajaxForm; -- ����������Ҫ���¼���������ΪAJAX�ύ������׼�����ú������ύ��,һ����� ready �С�
*       ����0��1������( function(response){} �ĳɹ��ص�����  �� Options ����)
*     ajaxSubmit -- �ύ��������ͬajaxForm���������¼�����(ֻҪ���¼����ھͿ���ʹ�ã�����Ҫ�ṩsubmit��ť).
*       ע�⣺һ���ڵ��õĵط�Ϊ�˱�����������б��ύ���ˢ�£���Ҫ return false;
*     formToArray
*     formSerialize -- �������л���һ����ѯ�ַ���������(�� & �ָ��� name=value )��Ȼ����� $.get��$.post��$.ajax�����ύ����
*     fieldSerialize -- �������ֶ�Ԫ�����л���һ����ѯ�ַ���������,ͨ������ֻ�Բ��ֱ��ֶν������л�ʱ
*       ʹ�÷�ʽ: $("#myForm .specialFields").fieldSerialize();
*     fieldValue -- ����ƥ����������еı�Ԫ��ֵ, �������������ʽ��������
*     clearForm -- �����Ԫ��(�����е� text, password, textarea, select, radio, checkbox ���ÿ�)
*     clearFields -- ���ָ�����ֶ�Ԫ��
*     resetForm -- ͨ�����ñ�Ԫ��ԭ�е�DOM�����������ָ�����ʼ״̬
*  
*   Options����
*     clearForm[] -- ������־����ʾ������ύ�ɹ��Ƿ����������
*     dataType[null] -- �ڴ����ص��������ͣ��� null(���ı�)/xml/script/json ��
*     resetForm[false] -- ������־����ʾ������ύ�ɹ��Ƿ��������
*     semantic[false] -- ������־����ʾ�����Ƿ�����ϸ�������˳���������ύ��TODO:һ�������� <input type="image"> ��Ԫ��?
*     target[null] -- ָ��ҳ�����ɷ�������Ӧ���и��µ�Ԫ�� -- TODO:ʲô��˼��ʾ��: '#divToUpdate'
*     type[form.method] -- ָ���ύ�����ݵķ���(method):"GET"��"POST"
*     url[form.action] -- ָ���ύ�����ݵ�URL
*     beforeSubmit(������ʽ�ı�����, jQuery������, Options����){ return boolean; }[null] -- 
*       ���ύǰ�����õĻص�����,��Ϊ���ӱ��ṩ������Ԥ�ύ�߼�����У������ݣ����ú�������false��������ύ
*       ������������·�ʽ������(TODO:�Ƿ��ܼ�?)��[ { name: 'username', value: 'fish' }, { name: 'password', value: 'secret' } ]
*     success(response){} -- ���ɹ��ύ����õĻص�����,������������Ӧ�󱻵���,response ��������dataType����
*   
******************************************************************************************************************************************/

module("jQueryFormTester", {
    setup: function() {
    //dump("in jQueryFormTester::setup\n");
    },
    teardown: function() {
    //dump("in jQueryFormTester::teardown\n");
    }
});

test("jQueryForm �Ĳ���", function() {
    equal(1, 1, "TODO:jQueryForm");
});
