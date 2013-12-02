#pragma once

/*****************************************************************************************************
* ִ�� sql ����
* sqlplus username/pwd@ServerName  [ < xxx.sql ]
*****************************************************************************************************/

/*******************************************************************************************************
* �����Ż���
*   8i��9iʱ����ʹ�� StatSpack ���ߣ��˹�����������ݣ��ҵ�����ƿ���������
*   10i ʱ������ ADDM ���棬�������ܵ������飬�޸�һ�³�ʼ��������Ӽ������������߸���SQL Tunning Advisorʹ��
*     SQL Profile����SQL����ִ�мƻ���������ɴ󲿷������Ż���
*
*
*******************************************************************************************************/

/*******************************************************************************************************
* Oracle�ͷŴ���(Undo��Temp��)�ռ�
*   1.�鿴����ռ����ƣ�
*     select name from v$tablespace
*   2.�鿴ĳ����ռ���Ϣ(��С)��ȷ���ͷ��ĸ���
*     select file_name,bytes/1024/1024 from dba_data_files where tablespace_name like 'UNDOTBS1';
*   3.�鿴�Ƿ���������ʹ�ûع���(��ֹ���û�����ʹ��)
*     select s.username, u.name from v$transaction t,v$rollstat r, v$rollname u, v$session s
* ����where s.taddr=t.addr and  t.xidusn=r.usn and r.usn=u.usn order by s.username;
*   4.�����µ�UNDO��ռ䣬�������Զ���չ����
*   5.create undo tablespace undotbs2 datafile 'D:\ORACLE\PRODUCT\10.1.0\ORADATA\ORCL\UNDOTBS02.DBF' 
*     size 10m reuse autoextend on next 100m maxsize unlimited;
*   6.��̬����spfile�����ļ�
*     alter system set undo_tablespace=undotbs2 scope=both;
*   7.�ȴ�ԭUNDO��ռ�����UNDO SEGMENT OFFLINE
*     select usn,xacts,status,rssize/1024/1024/1024,hwmsize/1024/1024/1024,shrinks from v$rollstat order by rssize;
*   8.��ִ�п�UNDO��ռ�����UNDO SEGMENT ONLINE(��7һ���������ͬ��)
*     select usn,xacts,status,rssize/1024/1024/1024,hwmsize/1024/1024/1024,shrinks from v$rollstat order by rssize;
*   9.ɾ��ԭ�е�UNDO��ռ�
*     drop tablespace undotbs1 including contents;
*   10.ȷ��ɾ���Ƿ�ɹ�
*     select name from v$tablespace;
*   11.�������ݿ��������������󵽴洢�����ļ���·����ɾ�������ļ�(����Ĳ���ֻɾ����undo��ռ���߼���ϵ��δɾ���ļ�)
*******************************************************************************************************/

class COracleTester : public CPPUNIT_NS::TestFixture
{
public:
    CPPUNIT_TEST_SUITE( COracleTester );

    CPPUNIT_TEST_SUITE_END();

public:
    COracleTester();
    ~COracleTester();
    virtual void setUp();
    virtual void tearDown();
private:
};