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