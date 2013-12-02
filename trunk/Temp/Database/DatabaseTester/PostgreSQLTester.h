#pragma once

/*****************************************************************************************************
* ���⣺
*   1.OID -- ֻҪ������ʱû��ʹ�� WITHOUT OIDS��PG������ÿһ�м�¼������һ��Ψһ��OID(4�ֽ�������������ظ�),
*     �ڲ�ϵͳ����ʹ�� OID �ڱ�֮�佨����ϵ
*   2.��ѯ work_mem -- ÿ�����̿�ʹ�õĹ����ڴ�ռ䣬����swap
*
* PostgreSQL -- ��ȫ��ѣ�����Դ��Ķ����ϵ�����ݿ����ϵͳ�����BSD��Ȩ��֧�ֶ�������(C/C++/Java/Perl/python �ȡ�
*   ��BSD������ POSTGRES �汾 4.2Ϊ����������
*   ֧�ִ󲿷�SQL��׼�����ṩ����������ִ����ԣ����Ӳ�ѯ�����������������ͼ�����������ԡ���汾�������ơ�
*   ���߳�֧�֣�--enable-thread-safety��Ĭ��ֵ������ͬ���߳̿���ͬʱʹ�� ��ͬ ������(PGConn) -- һ�����Ӳ�֧�ֶ��߳�
*   ֧��ȫ�ļ�������������еĵ��ʽ�������
* 
* ���밲װ(./configure �Chelp���鿴���õĲ���)��
*   1../configure --prefix=/usr/local/pgsql && make && make install
*   2.���� postgresql ���ݿ���û� postgres ����������
*     useradd postgres -m -d /export/postgres/ 
*     chown postgres:users postgres/ -R
*     passwd 
*   3.���û�������
*     PGHOME��
*   ��������:
*     a.configure: error: readline library not found   <== ����ʱ --without-readline �� ��װ yum install readline-devel
*     
*   ��Ҫ���� POSTGRES_HOME��PGLIB��PGDATA �Ȼ���������
*
* ʹ�ã�
*   ��ʼ����initdb -D <���ݿ��ַ>����ʼ���ɹ������»���һ�� postgresql.conf �������ļ�
*       ���� initdb ʱ�������ȱʡ�ı������� C locale -- ����ʹ��likeʱ�����޷�ʹ������������һ�������text_pattern_ops��������ܣ�
*     postgresql.conf -- ���� listen_address='*' ��port��ʹ�ÿ���Զ�̷���(����ֻ�� 127.0.0.1 �ϼ���)
*     pg_hba.conf -- �����ܷ��ʵ��û������ݿ⡢�ͻ���IP�ȣ� �� host all all 192.168.1.0/24 password
* 
*   ������pg_ctl -D <���ݿ��ַ--��/export/home/tsdata��> -l logfile start
*     ʵ�����е����ݿ������ postgres
*   ֹͣ��pg_ctl stop -m �ر�ģʽ(��smart��fast��immediate)
*   ����DB��createdb <DB����>
*   �� pgpool ͬ��
*   
* postgresql.conf
*   ��־Ŀ¼ͨ������ log_directory ���ã���"/var/postgres/pg_log" 
*   shared_buffers <== �����ڴ棬��ѯ����ʱ��˳��Ϊ �����ڴ� -> OS���� -> Ӳ�̣���˹����ڴ���OS�Ļ���һ�½ϰ�ȫ
*   checkpoint <== �ѹ����ڴ�����¹�������д�����̵�ʱ����ͨ�������־�е� "checkpoint"��ȷ��
*     checkpoint_segments -- ������С��һ����16M��Ĭ��Ϊ3����48M
*     checkpoint_timeout -- ��ʱ��Ĭ��5����
*
* ����
*   ����	    �ڲ�����	˵��
*   SERIAL                  ���У����Զ�ת��Ϊ INT4 NOT NULL DEFAULT nextval('����_�ֶ�_seq'),
*   VARCHAR(n)	varchar	    ָ������󳤶ȣ��䳤�ַ��������㶨�峤�ȵĲ��ֲ����룬ͨ���洢��������󳤶ȵı䳤�ַ�
*   CHAR(n)	    bpchar	    �����ַ�����ʵ�����ݲ��㶨�峤��ʱ���Կո��룬�ʺ��ڴ洢������ͬ���ַ���
*   TEXT	    text	    û���ر���������ƣ������е���󳤶����ƣ��������ڴ洢���ɴ�1G���ҵ�δ�������Ƴ��ȵ��ַ���
*   BYTEA	    bytea	    �䳤�ֽ����У������ڴ洢���������ݣ������ǰ��� NULL �ֽڵ�ֵ
*   "char"	    char	    �����ַ�
*
* 
* �ͻ���
*   Lin��psql -p �˿ں� -U �û��� -d ���ݿ���
*     �� -E ѡ������ psql,����ӡ������psql��������������ִ��ʱ���ڲ�ʵ��ʹ�õ�SQL��ѯ���
*     \q �˳�
*     \? ����
*     \dt -- ��ʾ���ݱ�Ķ��壻  \dn -- ��ʾSchema
*     �����û���  create user fujie password '123456';
*     �������ݿ⣺create database mydata owner fujie; 
*   Win��pgAdmin ,Ŀǰ�� 3-1.12����Ҫ�����listen_address, port
*   Ϊ�˷���ͻ��˵�ʹ�ã����԰�װ phpPgAdmin��ʹ�� B/S �ṹ���й�����Ҫapache��php4��php4-pgsql�������֧��
*
* ��������
*   PGʹ�ö�汾�������ƻ���(Multiversion Concurrency Control, MVCC) -- ÿ�������ڲ�ѯ����ʱ�������������ݵĿ���.
*   ֻ��������������ͼͬʱ����ͬһ��������ʱ���Ż��еȴ����֡�
*   PostgreSQLֻ�ṩ������������뼶��(����SET TRANSACTION������)
*     Read Committed(Ĭ��) -- ���񷢳���SELECT����ֻ�ܿ�����SELECT���ʼִ����ǰ�ύ�����ݣ�ʹ�ü򵥣��ٶȽϿ�
*     Serializable -- ���ϸ����в���ִ�е������ִ�н���͵�������һ��һ����ִ�еĽ����һ����
*
* �������������ܷ�����
*   1.ʹ��COPY��������Insert���(��Ҫ��admin�û�)��
*   2.�����SQL������һ�������Լ����ύ����Ŀ�����
*   3.��һ����������ȡ������¼ʱʹ��CLUSTER��
*   4.��һ����ѯ�����ȡ�����ּ�¼ʱʹ�� LIMIT n�������ִ�в�ѯ����ʱ��֪��ȷ�еļ�¼���� ��ʹ���α�(cursor)��FETCH���ܡ�
*   5.ʹ��ANALYZE�Ա��־�ȷ���Ż�ͳ�ƣ�
*   6.����ʹ�� VACUUM �� pg_autovacuum -- ����ʹ�� VACUUM ANALYZE���� ANALYZE ����йر��ͳ��ֵ
*   7.���д������ݸ���ʱ��ɾ��������Ȼ���ؽ�������
*
* ��ѯ��
*   ������ʽ����  <== ������ ~ ����������ʽƥ�䣬�� ~* �����Сд�޹ص�������ʽƥ�䣬��Сд�޹ص� LIKE ���ֳ�Ϊ ILIKE
*     ��Сд�޹صĵ�ʽ�Ƚ�ͨ��д�� WHERE lower(col) = 'abc';
*   ���� <== 
*     ��ͨ����(������)��
*     �������ӣ�SELECT * FROM t1 LEFT OUTER JOIN t2 ON (t1.col = t2.col); ���� t1 ������δ���ӵ���, 
*     �������ӣ�SELECT * FROM t1 RIGHT OUTER JOIN t2 ON (t1.col = t2.col); ���� t2 ��δ���ӵ���
*     ��ȫ�����ӣ�FULL OUTER JOIN��,���� t1 �� t2 ��δ���ӵ���
*
* ���ܲ���
*   ���������е� log_xxx ����������ѡ��
* 
* DDL����
*   CREATE INDEX ������ ON ����(�ֶ�)
*   CREATE SEQUENCE ���к��� -- ͨ������Ϊ ����_�ֶ�_seq
*   INSERT INTO ���� (�ֶ�) VALUES ('ֵ') RETURNING id;  -- ���Է���������ֵ
*
* Լ��
*   UNIQUE
*   CHECK
*
* ���
*   DBͬ�����ӣ�PQconnectdb �� PQsetdblogin�����ص� PGConn* �� PQfinish �ͷ�
*   DB�첽���ӣ�PQConnectStart �� PQconnectPool -- �ÿո����Ĳ������ؼ��� = ��ֵ -- ע��Ⱥ����ߵ��пո�
*     �ؼ����У�host(����)��hostaddr(IP��ַ)��port(�˿�)��dbname��user��password��connect_timeout��
*               options(����������������ѡ��)��sslmode(�Ƿ�ʹ��SSL���ӣ�ȱʡ��prefer)
*   ������  ��PQstatus(����)��PQerrorMessage
*   ״ֵ̬��ȡ��PQSocket(�ɻ�ȡ���ݿ������²���׽���������)
*               PQparameterStatus(���������������� server-version ��)
*               PQServerVersion
*   ������  ��PQtransactioStatus
*   ����ִ��  ��PQexec������ֺŷָ�������Զ���һ�������У������Ҫ�� PQclear �ͷ�
*               PQExecParams���ɴ���������Ҫ���ѯ������ı�(0)�������(1)��ʽ������������ $1,$2 �ȸ�ʽ
*               PQprepare/PQexecPrepared��׼��һ��ִ����䣬�ɱ���ÿ��ִ�ж����������صĽ����Ҫֱ�ӷ������е��ֶΣ�Ӧ���� PQresultStatus ������
*               PQntuples(�����и���)��PQnfields(�����и���)��PQfname(�ֶ�����)��PQftable(�����ֶ����ڵı��OID)
*   ��������֧�ֱ������м�����������(pg_advisory_lock) -- Ӧ�ó����ܹ���ȫ�����ؿ��Ƶõ����ͷ�����ʱ��
*****************************************************************************************************/

class CPostgreSQLTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( CPostgreSQLTester );
    //CPPUNIT_TEST( test_Time );
    CPPUNIT_TEST_SUITE_END();

private:
    DECLARE_DEFAULT_TEST_CLASS(CPostgreSQLTester);

    //void test_Time();

};