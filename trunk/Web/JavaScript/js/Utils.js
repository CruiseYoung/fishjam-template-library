"use strict";

function getCookie(cookieName) {
    var cookieString = document.cookie;

    var start = cookieString.indexOf(cookieName + '=');
    // ���ϵȺŵ�ԭ���Ǳ�����ĳЩ Cookie ��ֵ������ cookieName һ�����ַ�����
    if (start == -1) // �Ҳ���
    {
        return null;
    }

    //��ȡ��cookieֵ�Ŀ�ʼλ��
    start += cookieName.length + 1;
    var end = cookieString.indexOf(';', start);  //�õ�ֵ�Ľ���λ��

    if (end == -1) {
        end = cookieString.length;��
    }

    return unescape(cookieString.substring(start, end));
}

//ʹ�÷���: setCookie('username','fishjam', 30)
function setCookie(c_name, value, expiredays) {
��  var exdate=new Date();
    exdate.setDate(exdate.getDate() + expiredays);
    document.cookie = c_name + "=" + escape(value) + ((expiredays==null) ? "" : ";expires="+exdate.toGMTString());
}

//�ñ��������б��ϸ��⺯�������ĸ��������� ���û�������ڵ��øú��������ÿһ�����������ͣ���������㣬���׳��쳣
//Releaseʱ��Ϊ ���� bool ?
//ʹ��ʾ���� funciton myFunc( prefix, num, user) {
//      strictParams( [String, Number, Array ] , arguments );
//      ������к�����ҵ���߼�
//  }
function strictParams(types, args ){
	if( types.length != args.length ) {
	    //throw "Invalid number of arguments. Expected " + types.length + ", received " + args.length + " instead.";
	    alert("Invalid number of arguments. Expected " + types.length + ", received " + args.length + " instead.");
	    return;
	}
	for (var i = 0; i < args.length; i++) {
	    //console.log("%o -- %o", args[i].constructor, types[i]);
	    if (args[i].constructor != types[i]) {
	        //throw "Invalid argument type for " + i + ". Expected " + types[i].name + ", received " + args[i].constructor.name + " instead.";
	        alert("Invalid argument type for " + i + ". Expected " + types[i].name + ", received " + args[i].constructor.name + " instead.");
	        return;
		}
	}
}

function checkParams(funName, args, logType) {
    var argInfo = funName + ": "
    for (var i = 0; i < args.length; i++) {
        argInfo = argInfo + "[" + i + "]=" + typeof args[i] + ",";
    }
    if (logType != undefined) {
        switch(logType) {
        case 1:
            console.debug(argInfo);
        	break;
        case 2:
            console.info(argInfo);
        	break;
        case 3:
        default:
            console.log(argInfo);
            break;
        }
    }else{
        console.debug(argInfo);
    }
    return argInfo;
}


//ͨ�õ����ü̳й�ϵ�ķ���?   -- Object-Oriented JavaScript.pdf �н���(P195)
function extend(Child, Parent) {
    var F = function() { };
    F.prototype = Parent.prototype;
    Child.prototype = new F();
    Child.prototype.constructor = Child;
    Child.uber = Parent.prototype;
}

String.prototype.format = function(args) {
    var result = this;
    if (arguments.length > 0) {    
        if (arguments.length == 1 && typeof (args) == "object") {
            for (var key in args) {
                if(args[key]!=undefined){
                    var reg = new RegExp("({" + key + "})", "g");
                    result = result.replace(reg, args[key]);
                }
            }
        }
        else {
            for (var i = 0; i < arguments.length; i++) {
                if (arguments[i] != undefined) {
                    var reg = new RegExp("({[" + i + "]})", "g");
                    result = result.replace(reg, arguments[i]);
                }
            }
        }
    }
    return result;
}

/*****************************************************************************************
* Url�п������� %\/?#&= ��������ţ����ֱ���������ϴ��ݻ�������ݿ⣬����Ϊ�������
*   ��˴�����ǰ��Ҫת����TODO: �Ƿ��б�׼��ϵͳ�������������?��
******************************************************************************************/
function handleUrlSpecialWord(quotReply) {
	if (null == quotReply) {
		return "";
	}
	
	return quotReply.replace(/%/g,"%25").replace(/\+/g,"%2B").replace(/\//g,"%2F")
			.replace(/\?/g,"%3F").replace(/#/g,"%23").replace(/&/g,"%26").replace(/=/g,"%3D");
}