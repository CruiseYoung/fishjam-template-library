#pragma once


/**********************************************************************************************************************
* ʹ��Trinity�ħ��˽�� -- http://log4think.com/setup_wow_private_server/
*                            http://blog.csdn.net/xiadasong007/article/details/7522910
* 
* MaNGOS��Massive Network Game Object Server�� -- ��Դ(GPL)��MMORPG��Ϸ������Դ����(https://github.com/mangos)��������ħ��˽��
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