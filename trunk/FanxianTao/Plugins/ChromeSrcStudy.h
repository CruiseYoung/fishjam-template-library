#ifndef CHROME_SRC_STUDY_H
#define CHROME_SRC_STUDY_H

/************************************************************************************************************
*ChromeԴ��ʹ�� GYP(Generate Your Projects) �����ɹ����ļ�
*  ��������ϰ�װ�˶���汾��VS���������� GYP_MSVS_VERSION ��������(�� 2010 )����ָ���汾�Ĺ���
* 
*Դ�����أ�
*  http://dev.chromium.org/developers/how-tos/get-the-code Ȼ���������� "source tarball"
*  ��ѹʱ��Ҫʹ�����µĽ�ѹ����(���� http://www.7-zip.org/��������Ȳ����Ƿ���һ�ν�ѹ)��������ܻᱨ��
*
* ����(Windows) -- http://www.chromium.org/developers/how-tos/build-instructions-windows
*   0.���������Win7_64 + 8G�ڴ� + 60G���ÿռ�
*   1.׼������(��ѡcygwin����ѡ����Ͳ��ܸ���):
*     ����Ҫ���� ��������Windows ��������(��ʼ������ "����ѡ��/Indexing Options") �ȡ�
*     VS2010(ѡ��X64������) + SP1(https://www.microsoft.com/en-us/download/details.aspx?id=23691)
*     Windows 8 SDK(http://msdn.microsoft.com/en-us/windows/hardware/hh852363.aspx)
*       ���û�а�װ��Ĭ��·������Ҫ����������������ָ��װ��·����GYP_DEFINES �� windows_sdk_path
*       �԰�װ��Windows 8 SDK �򲹶�(ȥ�� Include\winrt\asyncinfo.h 66�е�ö������ AsyncStatus ǰ�� class������ٷ�˵��)
*        ? http://download.microsoft.com/download/F/1/0/F10113F5-B750-4969-A255-274341AC6BCE/GRMSDKX_EN_DVD.iso
*     DirectX SDK 2010 June(http://www.microsoft.com/download/en/details.aspx?displaylang=en&id=6812)
*        ? http://download.microsoft.com/download/A/E/7/AE743F1F-632B-4809-87A9-AA1BB3458E31/DXSDK_Jun10.exe
*   ��ѡ��Google�ṩ���Զ��ļ򵥹�������װ�ű�(�Զ���װ����ĳ���)
*   2.���ذ�װ depot_tools(�����з�ʽ���Զ�������) -- http://www.chromium.org/developers/how-tos/install-depot-tools
*     TODO: gclient config https://src.chromium.org/chrome/trunk/src -- ʲôʱ��ִ��(�ƺ�ȱʡ�ľ�OK)���ᴴ��һ�� .gclient �ļ������Դ��λ��
*           svn ls https://src.chromium.org/chrome     -- ����SSL
*           ���ֹ����� .gclient �ļ���ͨ�� custom_deps ��ָ����ͬ���Ĵ���·��(���� test )���Ӷ����ٿռ�
*     a.����depot_tools.zip����ѹ��������·����PATH�����;
*     b.���ñ��� GYP_GENERATORS='ninja,msvs' ��ͬʱ��������VS���̺�ninja����? -- �����û����������� chromium.gyp_env ?
*     c.������������ gclient , ��һ������ʱ���Զ�����Ŀ¼�°�װ svn,python,git ��
*     d.�� src Ŀ¼���θ��� gclient sync [--force] �����Զ��ӷ�����ͬ�����µĴ���, ��ѡ�� --force ������ʲô��˼��
*   3.��ѡ�����ñ��� Shared Library ���������(��������������chromeʱ����ʹ�ý�С���ļ�,����ʧ��)��
*     ע������Ժ�һ�㶼��Ҫ clean �����֮ǰ�ı������������ַ���(�˴�ֻ����һ��)
*     a.�ں� .gclient ͬ����Ŀ¼������ chromium.gyp_env �ļ���������Ϊ��
*        {'GYP_DEFINES': 'component=shared_library'}
*     b.��� MS manifest ������
*       �༭ src\build\common.gypi �ļ������� _BIND_TO_CURRENT_CRT_VERSION=1 -- TODO:��ô���ӣ��ĵ���һ��
*     c.��Ԥ����ͷ(VS2010�Ѿ�Ĭ�ϴ�) -- 
*   4.���ɹ����ļ���gclient runhooks --force
*   5.����Google API keys(��ʱ������) -- ���ڱ���ʱָ����Ҳ��������ʱͨ����������ָ��
*   6.1.��VS2010�� chrome/chrome.sln �Ϳ��Ա��룬�����ǳ���
*   6.2.���ٱ��룺
*     a.�޸� src/build/some.gyp ��ֻ�����ע�Ĺ��� -- �ο� all.gyp �� all_android.gyp
*     b.���ٲ��������Ӷ�����ͬʱ����ʱ���ڴ����ġ�
*       MSVC�У�Tools\Options\Projects and Solutions\Build and Run -> ���� "maximum number of parallel project builds"
*       linxu��Cygwin�¿��Ը��� ~/include.gypi 
*       gclient��: -j=N
*     c1.ʹ�� ninja ���� -- ? ninja -C out\Debug chrome , ������Ľ��������VS�е���
*     c2.ʹ�� VS�������б���: �� src\chrome Ŀ¼��ִ�� devenv.exe /build Debug chrome.sln
*   7.����
*     src/build/Debug/chrome.exe �� src/build/Release/chrome.exe
*
* ChromeԴ�����
*   http://blog.csdn.net/milado_nju/article/details/7216067
* 
* ���ּ����������ֱ����Chrome������չ -- ����չ��ǿ��
************************************************************************************************************/

/************************************************************************************************************
* CEF(Chromium Embedded Framework) -- ��ԴWeb browser�ؼ������ ftlWeb.h
*
************************************************************************************************************/


#endif	//CHROME_SRC_STUDY_H