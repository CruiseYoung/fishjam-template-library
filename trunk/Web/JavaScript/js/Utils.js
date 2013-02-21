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