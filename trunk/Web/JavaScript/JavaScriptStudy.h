
//<a href="javascript:self.close()"> �ر� </a>

/***********************************************************************************************
* ������MSDN -> Development Tools and Languages -> Visual Studio -> JScript
*
* �﷨��
*   1.�������ִ�Сд��ʹ��ǰ��Ҫͨ�� var <������> [ = <��ʼֵ> ] �ķ�ʽ������Ĭ�ϸ�ֵΪ"null"��JavaScript ���������͵�Ҫ���ϸ�
*     null -- һ������Ŀ�ֵ��������δ���壬���߶���֮��û�ж�������κθ�ֵ����ʱ��ֵ��
*     Number.NaN(Not a Number) -- �������޷�������ȷ����ֵʱ�����ظ�ֵ(�� parseInt("��ĸ"))���κ���(����NaN)�����������
*     Infinity -- ����󣬶�Ӧ���� Number.NEGATIVE_INFINITY �� Number.POSITIVE_INFINITY
*   2.����(Array) -- ����ļ��ϣ���ߵĶ�������ǲ�ͬ���͵ģ�ͨ�� [�±�] ����, var myArray = new Array(��ѡ�ĳ�ʼ�б�);
*       JSֻ֧��һά���顣����ģ���ά����Ҫ���� new Array(new Array(), xxx) �ķ�ʽ������ʱͨ�� myArray[2][3] = xxx;
*       join(�ָ���) -- ʹ�÷ָ����������еĸ���Ԫ�ش�������������Ӧ���ַ�����ͨ�������ڵ������?
*       sort([������]) -- ��ָ�������������������簴�����������ֵ��������� function myGreater(a, b){return a-b;}
*   3.Date -- �ɴ洢 0001��9999������һ�����ڣ���ȷ�����룬�ڲ�ʵ����һ������(1970-01-01 00:00:00:000 ��ʼ�ĺ�����)��
*     �粻ָ��ʱ����Ĭ�ϲ���"UTC"���� get/set[UTC]XXXX �ķ���������ȡ�����ö�Ӧ��ʱ��ֵ��
*     getTimezoneOffset() -- �������ڶ�����õ�ʱ�����������ʱ������ķ��������й�(GMT+0800)���� -480��
*   4.�쳣����
*     try { xxxx } catch(e) { alert('some error'); }
*   5.һЩ���õ����⺯��
*     eval() -- �������ڵ��ַ���������׼������ʽ������
*     escape() -- ���������е��ַ����������������ַ������ñ���Ӧ����URL��Ҳ���ǰѿո�д��"%20"���ָ�ʽ
*     parseInt()/parseFloat() -- ���ذ������ڵ�����ת��������/������֮���ֵ�����������ָ�� radix �Է�ֹ���������ַ���(��0xxx);
***********************************************************************************************/

/***********************************************************************************************
* ���󻯱��
*   ����ʱ��className.funName1();
*   var className = {
*     funName1 : function(param1, param2) {
*       this.initialized = true;
*       return [����ֵ];
*     },
*     funName2 : function(param..) {
*     }
*   };
*
*   ����
*   ����
*     �κζ����� toString() ���������ؿɶ����ַ�����ʽ
*   �¼�--��Ӧ�����ڶ����ϵ�����
*
*   
* �ĵ����ո����¼� <== document.captureEvent()
***********************************************************************************************/



//Jscript�ṩSendKeys�����������ڽ�һ�������������͵�����ڡ�

var CFUtil = {
    //��һ���µĴ��ڣ�����ʹ���Ѿ������Ĵ��ڻ�ý���
    openWindow: function(windowName, url, flags, params) {
        var windowsMediator = Components.classes["@mozilla.org/appshell/window-mediator;1"].getService(Components.interfaces.nsIWindowMediator);
        var aWindow = windowsMediator.getMostRecentWindow(windowName);
        if (aWindow) {
            aWindow.focus();
        }
        else {
            aWindow = window.openDialog(url, windowName, flags, params);
        }
        return aWindow;
    },
    
    
   openProgram : function () {
   	    var WshShell = WScript.CreateObject("Wscript.Shell");	//����WshShell����
	    WshShell.Run("notepad",3); //����WshShell�������������±�,���������
	    WshShell.AppActivate("���±�"); //������Ӧ�ó����������ȵ���һ��AppActivate������ȷ��Ӧ�ó���Ĵ��ڱ�����
	    WScript.Sleep(1000);//��ЩGUI�Ľ�����Ӧ�ٶȻ�Ƚ���������ڸ�����֮������ܲ���ʱ�仺��

	    WshShell.SendKeys("ABCD...");	//���Ͱ���������������Ӧ�ó���Ĺ���
	    WScript.Sleep(1000);

	    WshShell.SendKeys("%OF");   //������Ϲ��ܼ�(ALT+O+F���������±����������ô���)
   }
};

//CFUtil.openProgram();
