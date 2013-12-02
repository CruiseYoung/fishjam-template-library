#pragma once

/*****************************************************************************************************
* http://www.sqlite.org ��http://www.sqlite.com.cn
*   ע�⣺���� sqlite3.c ��C���ԵĴ��룬���ֱ�ӹ����ڹ�����Ļ���Ӧ�ò��� extern "C" { #include "sqlite3.h" }; �ķ�ʽ
*
* SQLite��һ�����͵ġ���Դ�ġ����� ACID �Ĺ���ʽ�ļ������ݿ����ϵͳ�������Ŀ����Ƕ��ʽ��ռ����Դ�ǳ��ͣ�
*   �����ڵ�һ�����ļ��е�һ�����������ݿ�,֧�����ݿ��С��2TB
* SQLite��������(Typelessness)�ģ�����Ȼ������Create Table�����ָ����������
*   ֧�ֵ����������У�VARCHAR��NVARCHAR��TEXT��INTEGER��FLOAT��BOOLEAN��CLOB��BLOB��TIMESTAMP��NUMERIC��
* 
* ֧�ֵ�SQL����
*   comment��expression 
*   TRANSACTION��BEGIN��COMMIT��END��ROLLBACK 
*   CREATE��INDEX��TABLE��TRIGGER��VIEW
*     ������create table table_name( ID INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, field2 ����2, ...);
*       �������ڲ���NULLֵʱ���Զ����ӣ�ͨ�� sqlite3_last_insert_rowid ������ȡ�����IDֵ
*     ����������create index index_name on table_name(field_to_be_indexed);
*   DATABASE��ATTACH��DETACH
*   COPY��DELETE��INSERT��SELECT��UPDATE��EXPLAIN��PRAGMA��REPLACE��
*     �������ݣ�insert into ���� [(�ֶ��б�)] values(data1, data2, data3, ...); 
*       �ַ����õ�����(')����������ֶ�û�����ݣ���������NULL
*     ��ѯ���ݣ�select �ֶ� from ���� where ���� order by �ֶ��� (desc) limit ����;
*       like '�����ֶ�%' -- ͨ������
*       count(*) -- ͳ������
*     �������ݣ�update ���� set �ֶ���='ֵ' where ����;
*     ɾ�����ݣ�delete from ���� where ����;
*   DROP��INDEX��TABLE��TRIGGER��VIEW
*   ON CONFLICT clause
*     

* ϵͳ��
*   sqlite_master��
*
* ������������ʱ��ʹ��"begin;"��Ȼ��insert���ݺ�ͨ��"commit;"��������ύ
* 
* ���� ( sqlite3 ���ݿ���.db -- �򿪻򴴽����ݿ⣬���֮ǰû�����ݿ⣬���� �ֺ�";" ��ȷ�ϼ����������ݿ�)
*   SQLָ���Էֺ�(;)��������������(--)�����ע�ͣ�Ctrl+C ���Խ�����ǰ����
*   .help -- ��ʾ������Ϣ
*   .quit -- �˳�
*   .backup [���ݿ�����Ĭ����main] FILE -- �������ݿ⵽�ļ������� .restore ���лָ�
*   .bail ON|OFF -- ���������Ǽ���������ֹѡ��,Ĭ������ֹ.
*   .databases -- ��ʾ�ļ������ݿ��б�,���û�и������ݿ������£��ļ���ֻ��main���ݿ�
*   .dump [����] -- ��SQL���ķ�ʽ������е�����
*   .echo ON|OFF -- �����Ƿ���Բ������Ĭ������
*   .explain ON|OFF -- ���������ʽΪcolumn 
*   .header(s) ON|OFF -- �����Ƿ� ���|�����ֶ���
*   .import FILE TABLE -- ���ļ��е����ݵ��뵽���У����ֶ���separator��Ĭ����"|"����ֵΪ�ָ���
*   .indices TABLE -- �г��ض�������е�����
*   .mode MODE ?TABLE? �����ʽѡ��� csv(���ŷָ�)��column(ָ�����)��html��insert(SQL���)��
*      line(field=value ��ʽ)��list(��.separator�ָ�)��tabs��tcl ��
*   .output FILENAME -- ��������(�� select��)������ļ���Ĭ���� stdout(�������Ļ)
*   .read �ļ��� -- ִ���ļ��е�SQL���,�ļ��е����һ��Ҫ���Ϸֺ�(;).
*   .restore [DB] FILE -- �ӱ����ļ���ԭ����
*   .schema ?TABLE? -- ��ʾ���ڴ�����ǰ���ݿ�� CREATE TABLE �� CREATE INDEX ���
*   .show -- ��ʾ������Ϣ(echo��explain��headers��mode��nullvalue��output ��)
*   .tables ?PATTERN? -- ��ʾ���еı�
*   
* ����˵��
*   sqlite3_exec -- ����ͨ�� sqlite3_callback ָ����sql���ִ�к���õĻص���ͨ��ֻ�� select ʱ��ʹ�ã�
*     ��ص���int LoadMyInfo(void *param, int n_column, char** column_value, char** column_name)��ÿ�鵽һ����¼�͵���һ�Ρ�
*     �����ֱ���� ����Ĳ�����һ����¼�ж����ֶΣ��ֶ����ݵ�һά���飬�ֶ�����һά����
*
* ���õ���ѹ�����
*   SQLite Expert �C- http://www.sqliteexpert.com/features.html
*   Sqliteadmin Administrator -- http://sqliteadmin.orbmu2k.de/
*   SQLite Database Browser -- http://www.oschina.net/p/sqlitebrowser
*   SQLiteSpy -- http://www.oschina.net/p/sqlitespy
*   SQLite Manager 0.8.0 Firefox Plugin -- https://addons.mozilla.org/en-US/firefox/addon/sqlite-manager/
*****************************************************************************************************/
