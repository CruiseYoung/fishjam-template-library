#pragma once

/**********************************************************************************************************************
* http://blog.csdn.net/lodger007/article/details/2279422 -- ˽���Ĵ����
* ˵��(��ع�����mangos�� contrib �� )
*   1.mpqe.exe�ͻ���dbc�ļ� -- https://svn.code.sf.net/p/mangoscms/code/trunk/tools/MPQE/
*     ��ѩ��Ϸ��mpq(Mike O'Brien Pack--�����Ǳ�ѩ����ϯ��Ϸ���򿪷���Ա�ʹ�ʼ��)�ļ�����
*     ����ʹ�� MPQE_1.2.rar ���߽�����ȡ�����Խ� *.dbc(��Ϸ��Ϣ)��ȡ�� mangos/dbc ��
*     mpqe.exe������data(���� /data/zhCN ��)Ŀ¼�£�������ִ�� mpqe.exe /p dbc.MPQ  DBFilesClient/*.dbc, ��󿽱��� mangos/dbc ��
*     TODO:����Ӧ����? mpqe /p local-zhCN.mpq DBFilesClient/*.dbc
*   2.��ͼ��ȡ����ad.exe -- https://svn.code.sf.net/p/mangos/code/trunk/contrib/
*     Ϊ֧�����ģ���Ҫ�޸�system.cpp�е����Դ��� char* langs[]={"zhCN", ... }
*     ������WOW��װĿ¼�У������յ�maps��Ŀ¼��Ȼ������ad.exe���ȴ�����ȡ��ͼ��Ϣ,
*     ����ѹ��ͼ������ mangos/maps ��
*   3.����Vmap(����"����"����)
*     ִ�� mangos �е� makevmaps_SIMPLE.bat��������Ŀ¼ vmaps/*.vmap(?) �ļ�������mangos/vmaps Ŀ¼��
*   4.ʹ��UDB���ݷḻ��Ϸ���� -- (��վ���ɷ���)http://www.udbforums.org/
*     �����ݵ��� mangos ���ݿ�
*   5.����MaNGOS�Ϳͻ���
*     5.1.�ڿͻ��˵�wtf��Ŀ¼�´������ realmlist.wtf �ļ���д��"set realmlist 127.0.0.1"
*     5.2.�޸� mangosd.conf �� realmd.conf �����ļ�
*   6.���з�������
*     start realmd.exe + start mangosd.exe
*   7.ֱ�����пͻ��� Wow.exe 
*   8.��������⣬��鿴log�ļ�����sql/update�е����ݽű����³�����������ݱ�
* 
*   ��ص����ݿ�(MySQL), Restore from SQL Dump -> ѡ��sqlĿ¼�¶�Ӧ��sql�ļ�
*     characters
*     mangos 
*     realmd
*     scriptdev2 -- ���WoW����Ϸ�ű��������˶���Ĺ��ＰBOSS��AI
*   �����ļ�
*     mangosd.conf -- ������Ʒ����Ǯ�ĵ����ʣ�����ֵ�������ʵ�
*       [list]
*         DataDir = ""
*         WorldDatabaseInfo = "127.0.0.1;3306;root;[����];mangos"
*         LoginDatabaseInfo = "127.0.0.1;3306;root;[����];realmd"
*         CharacterDatabaseInfo = "127.0.0.1;3306;root;[����];characters"
*     realmd.conf -- 
*       LoginDatabaseInfo="127.0.0.1;3306;root;[����];realmd"
**********************************************************************************************************************/

/**********************************************************************************************************************
*
* ʹ��Trinity�ħ��˽�� -- http://log4think.com/setup_wow_private_server/
*                            http://blog.csdn.net/xiadasong007/article/details/7522910
* 
* MaNGOS(Massive Network Game Object Server) -- ��Դ(GPL)��MMORPG��Ϸ������Դ����
*   ( https://github.com/mangos �� https://svn.code.sf.net/p/mangos/code/trunk )
*   ������֧���κοͻ��˵�������Ϸ, Ŀǰ������ħ��˽����
*  
*   ���Ĳ����Ǹ����ض���Ϸû�й�ϵ�ĺ��Ŀ�ܳ��� -- ���н��̵��ȣ��������磬�����������ƣ������������ȡ�
*   ����Ϸ�������ݿ⣬��Ϸ���ʱ�䣬����ű���������������ĳ�����֧�ֵ���չ�ű���ʵ�֡�
*    
*     ��Ϸ�������ݿ⣬��Ҫ��Ӧ��ͬ��Mangos�汾(CompleteSDB156-Mangos3462.rar)
* 
*   Linux �±���ǰ��Ҫ(�������ͨ��SVN���ص�) autoreconf --install --force
*   doc -- �������� Doxygen ��ʽ�İ����ļ�
* 
* �ű�ϵͳ(scriptdev2)���������� scriptdev2.dll
*   https://svn.code.sf.net/p/scriptdev2/code/
*  
*   ����ϵͳ
*   ����/����ϵͳ
*   �ű�ϵͳ
*   ����ϵͳ
*
* 
* Trinity -- ����MaNGOS�Ĵ��뿪��(https://github.com/TrinityCore/TrinityCore.git)
*   ���밲װ
*     0.Git (��װ��Ϻ���Ҫ����Path) + TortoiseGid�������ظ���Դ��
*     1.OpenSSL ( http://slproweb.com/products/Win32OpenSSL.html ,16M���ҵ�, Ϊ�β������ع�����?)
*     2.CMake ( http://www.cmake.org/cmake/resources/software.html ),
*     3.MySQL ( http://dev.mysql.com/downloads/mysql/ ) 
*       MySQL GUI Tools ( http://dev.mysql.com/downloads/gui-tools/5.0.html )��������Ҳ������ SQLyog
*       ע��:a.����ʱ��¼�˺ŵĻ�����Ҫ��IE���ĵ��������
*            b.��Ҫ�����µ� 5.0(������), �� 4.0.2 ����?
*     4.ͨ�� CMake-Gui ������VS�Ĺ����ļ���
*       Configure ��ѡ�� ��Use default native compilers�� -> Visual Studio 9 2008 -> Finish ����Զ�������
*       ����������ѡ����� SCRIPTS��SERVERS��TOOLS��USE_COREPCH��USE_SCRIPTPCH, ��Ҫѡ�� USE_MYSQL_SOURCES��
*       ��Generate���ͻ���ָ��Ŀ¼������VS�Ĺ����ļ�
*     5.ʹ��VS�򿪹����ļ���Rebuild��
*     6.���Զ����� libmySQL.dll(MySQL)�� libeay32.dll,ssleay32.dll(OpenSSL)���������Ŀ¼����汾���ԣ������п���
**********************************************************************************************************************/


/**********************************************************************************************************************
* ģ��ħ���������Ŀ -- 
* 
**********************************************************************************************************************/
