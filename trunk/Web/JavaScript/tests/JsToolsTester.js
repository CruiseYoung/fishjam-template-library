"use strict";

/*************************************************************************************************************************
* JavaScript��صĹ���
*   TODO: ��JS�ļ����кϲ������Լ���http�����Ż������ʱ��������
* 
* ���� -- JS��������������ص��ͻ��ˣ��䱣���ǳ����ѡ�һ��ͨ�� ����+ѹ�� �ķ�ʽ���ӽ������Ѷȡ�
*   Javascript ��һ�ֻ���ԭ�͵����ԣ�û���ϸ�����Ͷ��塣
*     ���Զ�������У�������Ҫ�ⲿ���ʵ����Ժͷ���(���ⲿ������ȫ�ֱ�����)�����ܽ��л�����
*     �����ڲ����ʵ����Ժͷ�������Ҫ���л���
*     
* ��������
*   jasob -- ��ͬʱ����HTML��CSS��JS��Ч���ܺã����շѡ�
*   Javascript Obfuscator -- �շѣ���֧��ͬʱ����HTML
*   YUI Compressor(https://github.com/yui/yuicompressor) -- ���,���Ի���JS����֧��ͬʱ����HTML��
*   JSCompress(������Ʒ) -- http://www.zhaixiaowai.com/JSCompress/
*   UglifyJS -- jQuery 1.5�Ժ�ʹ�õĹ��ߡ����� NodeJS ��Javascript�﷨����/ѹ��/��ʽ�����ߡ�
*
* ���������� -- һ����Դ���ʽ������
*   Chrome�п���ͨ�� Sources -> Pretty print ����ʽ��������Ĵ��롣
*   WEBSTORM --  
*************************************************************************************************************************/


module("ToolsTester", {
    setup: function() {
    //dump("in ToolsTester::setup\n");
    },
    teardown: function() {
    //dump("in ToolsTester::teardown\n");
    }
});

test("JavaScript��صĹ���", function() {
    equal(1, 1, "TODO:����");
});


test("JUnit����", function() {
    var objNull;
    equal(objNull, null, "jUnit�� equal ���� == �����ж�");
    ok(objNull !== null, "֤�� objNull !== null");
});
