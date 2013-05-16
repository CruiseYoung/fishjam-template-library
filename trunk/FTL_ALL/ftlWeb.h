#ifndef FTL_WEB_H
#define FTL_WEB_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlWeb.h requires ftlbase.h to be included first
#endif


/*************************************************************************************************************************
* DHTML -- Dynamic HTML
* Nginx -- �����ܵ� HTTP �� ������� ��������Ҳ��һ�� IMAP/POP3/SMTP ���������������Ϊ���ؾ����������
*   ��֧��ʹ��һ���Թ�ϣ��ѡ����Memcache���(��ʹ�õ�����ģ��? ��ʹ�� Membase���MemCache ?)
* Rails -- ���ڿ������ݿ�����������Ӧ�ó����������ܣ�����MVC���ģʽ��ʹ��Ruby������
* MemCache -- �����ܵķֲ�ʽ���ڴ���󻺴�ϵͳ
*   ���ƣ����ü򵥣�nginx�ڽ���memcache��˵�֧�֣�֧��ʧЧ
*
*************************************************************************************************************************/

/*************************************************************************************************************************
* WebService
*   ��Ӧ�ó���ͨ������Э��͹涨��һЩ��׼���ݸ�ʽ��Http,XML,Soap)������WebService,ͨ�����ڲ�ִ�еõ�������
*   ������ Web Services ƽ̨�� XML+HTTP��ʹ�� XML ����������ݣ���ʹ�� SOAP ����������
*   SOAP(Simple Object Access Protocol) -- �򵥶������Э�飬�ǻ���XML��Э�飬��Ҫ����Ӧ�ó���֮���ͨ��
*   UDDI(Universal Description, Discovery and Integration) -- ͨ�����������ּ����ϣ���һ�ִ洢WebService��Ŀ¼����(ֻ��.NET��?)
*   WSDL(Web Services Description Language) -- ��������������ԣ�����XML�����������Ͷ�λWebServices
* 
* App Speed Index(ȫ�������վ���ݷ���?) -- http://newrelic.com/asi
* 
* Web �������ļܹ�(�ص���Ҫ������չ)
*   1. Nginx + Memcache + N��Rails + Mysql  -- 
* 
* �û���Ϊ����(�ܰٵ�)�ͶԲ� -- https://github.com/qiushibaike/moumentei(�ܰٵľ�Դ��)
*   a.50%~80%�ÿͲ���½ -- δ��¼�û�ֱ�ӷ��ػ�������
*   b.��ҳ�ȼ���ҳ��ռ��50%���Ϸ�����
*   c.ҳ����80%�����ǲ���ģ�ʣ�µ���ҪΪ�û������Ϣ -- ��������Բ�����û������ݣ�Ȼ����ر仯����
*************************************************************************************************************************/


namespace FTL
{
}
#endif //FTL_WEB_H

#ifndef USE_EXPORT
#  include "ftlWeb.hpp"
#endif 