"use strict";

/*************************************************************************************************************************
* TODO: 
*   1.imageDataʹ�ö������ֽڵ����飬���к�TypedArray�ӽ������ܣ�
*     �����Ȱ�imageDataת��Ϊ���������Ի�ȡ�����ִ���ٶ�
*   
*
* Typed Array -- ��������(http://www.kuqin.com/webpagedesign/20120407/319733.html)
*   JS���Բ���ԭʼ����������(��ǰ�����ԭʼ���ݵ����ַ�������������ʹ��charCodeAt�����������ݻ������ж�ȡ�ֽ�)
*   ���������ڳ�ʼ�����ܸı��С�� 
*   �������У� IntNArray/UintNArray(N Ϊ 8/16/32), Float32Array/Float64Array ��
*     ���캯��(buf [, ��ʼλ��] [,����])
*     
*     �����Ǹ������͵� ArrayBuffer ����ͼ,�������������ͼ����ָ��ͬһ��arrayBuffer�����ò�ͬ�����͡���ͬ�ĳ����Լ���ͬ��λ�ơ�
*     TODO:Ϊʲôһ������������ֵ����һ����ͼ�����ֲ�������

*   �ŵ㣺
*   1.��������(һ���̶����ڴ��)�ȴ�ͳ����(ʹ��Hash���ҷ�ʽ)��Ķ࣬���зǳ��õ�����; http://blog.n01se.net/blog-n01se-net-p-248.html
*   2.��������ʹ��WebӦ�ÿ���ʹ�ö��ֶ������ļ���ʽ��ֱ�Ӳ����ļ��Ķ��������ݣ���������е�ý���ļ�����ȡ����
*     ��ʾ�������ļ������ݣ�http://ie.microsoft.com/testdrive/HTML5/TypedArrays/
*************************************************************************************************************************/


module("JsPerformanceTester", {
    setup: function() {
        //dump("in JsPerformanceTester::setup\n");
    },
    teardown: function() {
        //dump("in JsPerformanceTester::teardown\n");
    }
});

test("TypedArrayTester", function() {
    // ����һ��8�ֽڵ�ArrayBuffer -- Ȼ������ĸ�����ͼ��ָ�������
    var buf = new ArrayBuffer(8);
    equal(buf.byteLength, 8, "ArrayBuffer::ByteLength");

    // ����һ��ָ��buf����ͼv1������Int32���ͣ���ʼ��Ĭ�ϵ��ֽ�����0��ֱ����������ĩβ
    var v1 = new Int32Array(buf);
    equal(v1.byteLength, 8, "Int32Array::ByteLength");
    equal(v1.length, 2, "Int32Array::length");
    v1[0] = 0x01020304;
    v1[1] = 0x05060708;
    //���ú���ڴ�ṹ: 04 03 02 01 | 08 07 06 05

    // ����һ��ָ��buf����ͼv2������Uint8���ͣ���ʼ���ֽ�����2��ֱ����������ĩβ
    var v2 = new Uint8Array(buf, 2);

    // ����һ��ָ��buf����ͼv3������Int16���ͣ���ʼ���ֽ�����2������Ϊ2(���� 3~6 �ֽڷ�Χ )
    var v3 = new Int16Array(buf, 2, 2);
    equal(v3.byteLength, 4, "Int16Array::ByteLength");
    equal(v3.length, 2, "Int16Array::length");
    equal(v3[0], 0x0102, "��Int16������ȡ����һ������");
    equal(v3[1], 0x0708, "��Int16������ȡ���ڶ�������");
});
