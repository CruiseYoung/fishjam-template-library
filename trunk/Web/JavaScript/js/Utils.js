function getCookie(cookieName) {
    var cookieString = document.cookie;
    var start = cookieString.indexOf(cookieName + '=');
    // ���ϵȺŵ�ԭ���Ǳ�����ĳЩ Cookie ��ֵ������ cookieName һ�����ַ�����
    if (start == -1) // �Ҳ���
    {
        return null;
    }
    start += cookieName.length + 1;
    var end = cookieString.indexOf(';', start);
    if (end == -1) return unescape(cookieString.substring(start));
    return unescape(cookieString.substring(start, end));
}