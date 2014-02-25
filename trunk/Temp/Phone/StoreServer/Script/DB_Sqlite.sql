BEGIN TRANSACTION;
CREATE TABLE [auction_user] (
[user_id] INTEGER  NOT NULL PRIMARY KEY,
[username] TEXT  UNIQUE NULL,
[userpass] TEXT  NULL,
[email] TEXT  NULL
);
INSERT INTO "auction_user" VALUES(1,'tomcat','tomcat','spring_test@163.com');
INSERT INTO "auction_user" VALUES(2,'mysql','mysql','spring_test@163.com');
CREATE TABLE [kind] (
[kind_id] INTEGER  NOT NULL PRIMARY KEY,
[kind_name] TEXT  NULL,
[kind_desc] TEXT  NULL
);
INSERT INTO "kind" VALUES(1,'����Ӳ��','���ﲢ���Ǻ������Ĳ�Ʒ�����۸���������Ķ�');
INSERT INTO "kind" VALUES(2,'����','�ṩ�ǳ�ϡȱ�ķ�Դ');
CREATE TABLE [state] (
[state_id] INTEGER  NOT NULL PRIMARY KEY,
[state_name] TEXT  NULL
);
INSERT INTO "state" VALUES(1,'������');
INSERT INTO "state" VALUES(2,'�����ɹ�');
INSERT INTO "state" VALUES(3,'����');
CREATE TABLE [item] (
[item_id] INTEGER  PRIMARY KEY NOT NULL,
[item_name] TEXT  NOT NULL,
[item_remark] TEXT  NULL,
[item_desc] TEXT  NULL,
[kind_id] INTEGER  NULL,
[addtime] TimeStamp  NOT NULL,
[endtime] TimeStamp  NOT NULL,
[init_price] FLOAT  NULL,
[max_price] FLOAT  NULL,
[owner_id] INTEGER  NULL,
[winer_id] INTEGER  NULL,
[state_id] INTEGER  NULL
);
INSERT INTO "item" VALUES(1,'����','��ʽ����','�����壬��������',1,'2014-02-25 13:44:15','2014-02-25 13:44:15',230.0,250.0,1,NULL,1);
INSERT INTO "item" VALUES(2,'�Կ�','��ʽ�Կ�','���Կ�����������',1,'2014-02-25 13:44:15','2014-02-25 13:44:15',210.0,210.0,2,NULL,3);
INSERT INTO "item" VALUES(3,'�Ϸ���','��ʽ����','40����Ϸ���',2,'2014-02-25 13:44:15','2014-02-25 13:44:15',21000.0,25000.0,2,1,2);
CREATE TABLE [bid] (
[bid_id] INTEGER  PRIMARY KEY NOT NULL,
[user_id] INTEGER  NOT NULL,
[item_id] INTEGER  NOT NULL,
[bid_price] FLOAT  NULL,
[bid_date] TimeStamp  NOT NULL
);
INSERT INTO "bid" VALUES(1,2,1,250.0,'2014-02-25 13:44:15');
INSERT INTO "bid" VALUES(2,1,3,25000.0,'2014-02-25 13:44:15');
COMMIT;
