#pragma once

/**********************************************************************************************************************
* ˵��
*   ��ѩ��Ϸ��mpq(Mike O'Brien Pack--�����Ǳ�ѩ����ϯ��Ϸ���򿪷���Ա�ʹ�ʼ��)�ļ�����
*     ����ʹ�� MPQE_1.2.rar ���߽�����ȡ�����Խ� *.dbc(��Ϸ��Ϣ)��ȡ�� mangos/dbc ��
*     mpqe.exe������dataĿ¼�£�������ִ�� mpqe.exe /p dbc.MPQ  DBFilesClient/*.dbc, ��󱦱��� mangos/dbc ��
*   ��ͼ��ȡ����ad.exe -- ������WOW��װĿ¼�У������յ�maps��Ŀ¼��Ȼ������ad.exe���ȴ�����ȡ��ͼ��Ϣ,
*     ����ѹ��ͼ������ mangos/maps ��
*   ��ص����ݿ�(MySQL)
*     mangos
*     realmd
*   �����ļ�
*     mangosd.conf -- ������Ʒ����Ǯ�ĵ����ʣ�����ֵ�������ʵ�
*     realmd.conf -- 
*     
**********************************************************************************************************************/

/**********************************************************************************************************************
*
* ʹ��Trinity�ħ��˽�� -- http://log4think.com/setup_wow_private_server/
*                            http://blog.csdn.net/xiadasong007/article/details/7522910
* 
* MaNGOS(Massive Network Game Object Server) -- ��Դ(GPL)��MMORPG��Ϸ������Դ����
*   ( https://github.com/mangos �� http://svn.code.sf.net/p/mangos/code/trunk )
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
*       MySQL GUI Tools ( http://dev.mysql.com/downloads/gui-tools/5.0.html )
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
