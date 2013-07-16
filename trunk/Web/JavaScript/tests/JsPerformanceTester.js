/*************************************************************************************************************************
* Typed Array -- ��������(http://www.khronos.org/registry/typedarray/specs/latest/)
*   JS���Բ���ԭʼ����������(��ǰ�����ԭʼ���ݵ����ַ�������������ʹ��charCodeAt�����������ݻ������ж�ȡ�ֽ�)
*   ���������ڳ�ʼ�����ܸı��С�� 
*   �������У� Int32Array
*     �����Ǹ������͵� ArrayBuffer ����ͼ���������������ͼ����ָ��ͬһ��arrayBuffer�����ò�ͬ�����͡���ͬ�ĳ����Լ���ͬ��λ�ơ�

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
    var b = new ArrayBuffer(8);

    // ����һ��ָ��b����ͼv1������Int32���ͣ���ʼ��Ĭ�ϵ��ֽ�����0��ֱ����������ĩβ
    var v1 = new Int32Array(b);

    // ����һ��ָ��b����ͼv2������Uint8���ͣ���ʼ���ֽ�����2��ֱ����������ĩβ
    var v2 = new Uint8Array(b, 2);

    // ����һ��ָ��b����ͼv3������Int16���ͣ���ʼ���ֽ�����2������Ϊ2
    var v3 = new Int16Array(b, 2, 2);
    
	equal(1, 1, "TODO: ��������");
});
