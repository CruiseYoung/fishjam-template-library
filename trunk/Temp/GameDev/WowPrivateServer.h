#pragma once

/**********************************************************************************************************************
* ˵��(��ع�����mangos�� contrib �� )
*   1.mpqe.exe�ͻ���dbc�ļ� -- https://svn.code.sf.net/p/mangoscms/code/trunk/tools/MPQE/
*     ��ѩ��Ϸ��mpq(Mike O'Brien Pack--�����Ǳ�ѩ����ϯ��Ϸ���򿪷���Ա�ʹ�ʼ��)�ļ�����
*     ����ʹ�� MPQE_1.2.rar ���߽�����ȡ�����Խ� *.dbc(��Ϸ��Ϣ)��ȡ�� mangos/dbc ��
*     mpqe.exe������data(���� /data/zhCN ��)Ŀ¼�£�������ִ�� mpqe.exe /p dbc.MPQ  DBFilesClient/*.dbc, ��󿽱��� mangos/dbc ��
*     TODO:����Ӧ����? mpqe /p local-zhCN.mpq DBFilesClient/*.dbc
*   2.��ͼ��ȡ����ad.exe -- https://svn.code.sf.net/p/mangos/code/trunk/contrib/extractor
*     Ϊ֧�����ģ���Ҫ�޸�system.cpp�е����Դ��� char* langs[]={"zhCN", ... }
*     ������WOW��װĿ¼�У������յ�maps��Ŀ¼��Ȼ������ad.exe���ȴ�����ȡ��ͼ��Ϣ,
*     ����ѹ��ͼ������ mangos/maps ��
*   3.����Vmap(����"����"����) -- vmap_extract_assembler_bin
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
*     1.sql\create_mysql.sql -- �������еĿ����ݿ�
*       characters
*       mangos -- ͨ������YTDB�ķ�ʽ�ָ��� ��ҪԼ 5��6 ��Сʱ?
*                 Ȼ��˳����ִ��YTDB�����ִ� mangos ������������
*                 �鿴 db_version �������һ�е�����ǰ��ı��(�� 11169_02)��Ȼ���ٵ���sql\updates�ļ����¶�Ӧ��ź������sql�ļ�
*       realmd -- login ��Ϣ�����ݿ⣬main��ѭ������ ACE_Reactor::run_reactor_event_loop������ AuthSocket �Ľ���?
*     2.src\bindings\ScriptDev2\sql\scriptdev2_create_database.sql -- ���� scriptdev2 ��
*       scriptdev2 -- scriptdev2_script_full.sql �ű�������
*   �����ļ�
*     mangosd.conf -- ������Ʒ����Ǯ�ĵ����ʣ�����ֵ�������ʵ�
*       [list]
*         DataDir = "./"
*         WorldDatabaseInfo = "127.0.0.1;3306;root;[����];mangos"
*         LoginDatabaseInfo = "127.0.0.1;3306;root;[����];realmd"
*         CharacterDatabaseInfo = "127.0.0.1;3306;root;[����];characters"
*     realmd.conf -- 
*       LoginDatabaseInfo="127.0.0.1;3306;root;[����];realmd"
**********************************************************************************************************************/

/**********************************************************************************************************************
*
* �ħ��˽�� -- http://blog.csdn.net/lodger007/article/details/2279422 -- ˽���Ĵ����
*                 http://log4think.com/setup_wow_private_server/
*                 http://blog.csdn.net/xiadasong007/article/details/7522910
*                            
* ������̳(â���й�) -- http://www.mangoscn.com/forum.php?mod=viewthread&tid=6864
* 
* MaNGOS(Massive Network Game Object Server) -- ��Դ(GPL)��MMORPG��Ϸ������Դ����, ������֧���κοͻ��˵�������Ϸ, Ŀǰ������ħ��˽����
*   �°汾 -- https://github.com/mangos , ���Ͽ����ҵ���Ӧ����Wow�汾�ĳ���( Zero -> Four )
*     0.Mangos Zero  -- For Vanilla World of Warcraft
*     1.Mangos One   -- The Burning Crusade(ȼ�յ�Զ��)
*     2.Mangos Two   -- The Wrath of the Lich King(������֮ŭ)
*     3.Mangos Three -- The Cataclysm(���ֱ�)
*     4.Mangos Four  -- Mists of Pandaria(�˴����ǵ�����)
*
*     
*   
*  
*   ���Ĳ����Ǹ����ض���Ϸû�й�ϵ�ĺ��Ŀ�ܳ��� -- ���н��̵��ȣ��������磬�����������ƣ������������ȡ�
*   ����Ϸ�������ݿ⣬��Ϸ���ʱ�䣬����ű���������������ĳ�����֧�ֵ���չ�ű���ʵ�֡�
*    
*     ��Ϸ�������ݿ⣬��Ҫ��Ӧ��ͬ��Mangos�汾(CompleteSDB156-Mangos3462.rar)
* 
*   Linux �±���ǰ��Ҫ(�������ͨ��SVN���ص�) autoreconf --install --force
*   doc -- �������� Doxygen ��ʽ�İ����ļ�
* 
* �ű�ϵͳ(scriptdev2)���������� scriptdev2.dll�� ���WoW����Ϸ�ű��������˶���Ĺ��ＰBOSS��AI��С���ṩ��������ݿ��ṩ֧��
*   https://github.com/scriptdev2/scriptdev2 -- ���� ?
*   https://svn.code.sf.net/p/scriptdev2/code/, ��Ҫ���ص� mangos\trunk\src\bindings\ScriptDev2 Ŀ¼��(��ͨ�����·��ʹ��mangos�е��ļ�)
*   https://scriptdev2.svn.sourceforge.net/svnroot/scriptdev2
* 
* YTDB ���ݿ� -- ������ACID��scriptdev2�ű������ݿ⣬ ������ YTDB_0.14.1_R580_MaNGOS_R11000_SD2_R1946_ACID_R307_RuDB_R38.6.7z
*   http://svn2.assembla.com/svn/ytdbase/  -- �޷����ʣ� �����Է�����̳ http://ytdb.ru/
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
*     3.MySQL ( http://dev.mysql.com/downloads/mysql/ ) , �������ݿ�ʱ���� root �������Ϊ mangos(.conf �ļ��е�Ĭ��ֵ)
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

/**********************************************************************************************************************
* ħ��Ŀ¼����
*   Wow.exe -- ԭ���¼��
*   Launcher.exe -- ��¼����������վ���棬���¼�飬�޸��ȶ���� 
*   Data -- ���İ汾���ݣ�����3Dģ�͵�
*   WTK  -- ��Ϸ�����������ļ���
**********************************************************************************************************************/
