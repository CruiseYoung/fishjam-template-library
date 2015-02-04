#include "StdAfx.h"
#include "StorageFileUtil.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
#endif

CStorageFileUtil::CStorageFileUtil(void)
{
}

CStorageFileUtil::~CStorageFileUtil(void)
{
}

HRESULT CStorageFileUtil::DumpStorageFileInfo(LPCTSTR pszFilePath)
{
    HRESULT hr;

    USES_CONVERSION;
    LPCOLESTR lpwFileName = T2COLE( pszFilePath );	// ת��T����Ϊ���ַ�

    CComPtr<IStorage>   spStorage;
    COM_VERIFY(::StgIsStorageFile( lpwFileName ));// �Ǹ����ļ���
    if( FAILED(hr) )
    {
        return hr;
    }

    // �򿪸����ļ�
    COM_VERIFY(::StgOpenStorage(lpwFileName,NULL,STGM_READ | STGM_SHARE_DENY_WRITE,0,0,&spStorage));

    CComPtr<IEnumSTATSTG> spEnum;
    COM_VERIFY(spStorage->EnumElements( 0, NULL, 0, &spEnum ));

    STATSTG statstg;
    while( NOERROR == spEnum->Next( 1, &statstg, NULL) )
    {
        // statstg.type �����Ŷ������� STGTY_STREAM �� STGTY_STORAGE
        // statstg.pwcsName �����Ŷ�������
        // ...... ����ʱ�䣬���ȵȺܶ���Ϣ����鿴 MSDN
        TRACE(TEXT("type = %d, Name = %S\n"),statstg.type,statstg.pwcsName);

        ::CoTaskMemFree( statstg.pwcsName );	// �ͷ�������ʹ�õ��ڴ棨ע6��
    }
    
    spEnum.Release();
    spStorage.Release();
    
    return hr;
}

HRESULT CStorageFileUtil::CreateDemoStorageFile(LPCTSTR pszFilePath)
{
    HRESULT hr;		// ����ִ�з���ֵ
    IStorage *pStg = NULL;	// ���洢�ӿ�ָ��
    IStorage *pSub = NULL;	// �Ӵ洢�ӿ�ָ��
    IStream *pStm = NULL;	// ���ӿ�ָ��

    USES_CONVERSION;
    LPCOLESTR lpwFileName = T2COLE( pszFilePath );	// ת��T����Ϊ���ַ�

    hr = ::StgCreateDocfile(	// ���������ļ�
        lpwFileName,	// �ļ�����
        STGM_CREATE | STGM_WRITE | STGM_SHARE_EXCLUSIVE,	// �򿪷�ʽ
        0,		// ��������
        &pStg);		// ȡ�ø��洢�ӿ�ָ��
    ASSERT( SUCCEEDED(hr) );	// Ϊ��ͻ���ص㣬�򻯳���ṹ������ʹ���˶��ԡ�
    // ��ʵ�ʵĳ�������Ҫʹ�������жϺ��쳣����

    hr = pStg->CreateStorage(	// �����Ӵ洢
        L"SubStg",	// �Ӵ洢����
        STGM_CREATE | STGM_WRITE | STGM_SHARE_EXCLUSIVE,
        0,0,
        &pSub);		// ȡ���Ӵ洢�ӿ�ָ��
    ASSERT( SUCCEEDED(hr) );

    hr = pSub->CreateStream(	// ������
        L"Stm",		// ������
        STGM_CREATE | STGM_WRITE | STGM_SHARE_EXCLUSIVE,
        0,0,
        &pStm);		// ȡ�����ӿ�ָ��
    ASSERT( SUCCEEDED(hr) );

    hr = pStm->Write(		// ������д������
        "Hello",		// ���ݵ�ַ
        5,		// �ֽڳ���(ע�⣬û��д���ַ�����β��\0)
        NULL);		// ����Ҫ�õ�ʵ��д����ֽڳ���
    ASSERT( SUCCEEDED(hr) );

    if( pStm )	pStm->Release();// �ͷ���ָ��
    if( pSub )	pSub->Release();// �ͷ��Ӵ洢ָ��
    if( pStg )	pStg->Release();// �ͷŸ��洢ָ��
    
    return hr;
}