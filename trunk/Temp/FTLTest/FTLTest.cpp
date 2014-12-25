// FTLTest.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "FTLTest.h"

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/mfc/TestRunner.h>
//#include <cppunit/ui/qt/QtTestRunner.h>
//#include <cppunit/ui/text/TestRunner.h>
#include <ftlGdi.h>
#include <ftlGdiPlus.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFTLTestApp

BEGIN_MESSAGE_MAP(CFTLTestApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CFTLTestApp ����

CFTLTestApp::CFTLTestApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CFTLTestApp ����

CFTLTestApp theApp;


// CFTLTestApp ��ʼ��

BOOL CFTLTestApp::InitInstance()
{
    //FUNCTION_BLOCK_INIT();
    FTL::CFAutoGdiplus  autoGdiPlus;
    {
        //FAST_TRACE_OPTIONS traceOptions;
        //traceOptions.bWriteToFile = TRUE;
        //traceOptions.bDoTimings = TRUE;
        //FTL::CFFastTrace::GetInstance().SetTraceOptions(&traceOptions);

        // ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
        // ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
        //����Ҫ InitCommonControls()�����򣬽��޷��������ڡ�
        InitCommonControls();

        CWinApp::InitInstance();

        AfxEnableControlContainer();

        // ��׼��ʼ��
        // ���δʹ����Щ���ܲ�ϣ����С
        // ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
        // ����Ҫ���ض���ʼ������
        // �������ڴ洢���õ�ע�����
        // TODO: Ӧ�ʵ��޸ĸ��ַ�����
        // �����޸�Ϊ��˾����֯��
        SetRegistryKey(_T("Local AppWizard-Generated Applications"));

        CoInitialize(NULL);
        
        FTL::CFGdiUtil::SetProcessDPIAdware();

        CPPUNIT_NS::MfcUi::TestRunner runner;
        //CPPUNIT_NS::QtUi::TestRunner runner;
        //CPPUNIT_NS::TextUi::TestRunner runner;
        runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest() );
        runner.run();


        CoUninitialize();
    }

    //FUNCTION_BLOCK_UNINIT();

    // ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	// ����������Ӧ�ó������Ϣ�á�

	return FALSE;
}
