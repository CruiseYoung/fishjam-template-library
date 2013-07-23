"use strict";
/*************************************************************************************************************************
* HTML5 �������˺ܶ��µ�Ԫ��
*  canvas -- 
*    getContext("2d") -- ��ȡ2D�����Ļ����ĺ���
*    width/height -- �߿�����
*    toDataURL() -- ��Canvas�е�����ת����URL������ֱ�Ӹ�ֵ�� img.src 
* 
*  context
*    createImageData
*    getImageData(left, top, width, height) -- ��ȡ��ǰͼ���imageData
*    putImageData(imgData, left, top)
*    fillStyle -- �����ɫ������,�� #ff0000 ��ʾ��ɫ
*
*  imageData -- CanvasԪ��2D�����Ļ����ﶨ�����������
*    data -- �����Ƶ�RGBA�ֽ��������? ������ length ���ԣ���ͨ�� [] ��ȡÿһ���ֽڵ�ֵ
* 
*  �ļ���ק֧��(drag and drop)
* 
*  WebSocket
*    connection.send(byteArray.buffer);
*************************************************************************************************************************/


module("HTML5Tester", {
    setup: function() {
        //dump("in MathTester::setup\n");
    },
    teardown: function() {
        //dump("in MathTester::teardown\n");
    }
});

test("CanvasTester", function() {
    var canvasWidth = 100;
    var canvasHeight = 100;

    var c = document.createElement("canvas");
    var ctx = c.getContext("2d");
    var imgData = ctx.createImageData(canvasWidth, canvasHeight);

    equal(imgData.data.length, canvasWidth * canvasHeight * 4, "data ������һ���ֽ����飬ÿһ�����ض��� RGBA");

    //��imageDataת��Ϊ���������Ի�ȡ�����ִ���ٶ�
    var buf = new ArrayBuffer(imgData.data.length);
    var data = new Uint32Array(buf);
    for (var y = 0; y < canvasHeight; ++y) {
        for (var x = 0; x < canvasWidth; ++x) {
        
            //TODO: �ƺ����ԣ�
            var value = x * y & 0xff;
            data[y * canvasWidth + x] =
                (255 << 24)|        // alpha
                (value << 16)|      // blue
                (value <<  8)|      // green
                value;              // red
        }
    }
});

test("DragDropTester", function() {
    /*
    document.addEventListener("drop", function(e) {
    e.preventDefault();
    e.stopPropagation();
    var files = e.dataTransfer.files;
    if (files.length > 0) selected(files[0]);
    }, false);
    //Ȼ�� selected �м��ɴ���File, File �� size ������
    */
    equal(1, 1, "TODO: DragDrop Tester");
});