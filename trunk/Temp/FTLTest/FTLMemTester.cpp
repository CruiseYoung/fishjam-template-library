#include "StdAfx.h"
#include "FTLMemTester.h"
#include "ftlMem.h"
#include <atlfile.h>
#include <atlimage.h>
#include <GdiPlus.h>
using namespace Gdiplus;

class CMyMemLeak : public FTL::CFMemCheckBase
{
public:
    CMyMemLeak()
    {
        m_SomeValue = 10;
    }
private:
    int m_SomeValue;
    
};

void CFTLMemTester::_AllocLargeMeomories(int nBLockSize, int nBlockCount, UINT nFlags)
{
    BOOL bRet = FALSE;
    for (int i = 0; i < nBlockCount; i++)
    {
        HGLOBAL hGlobal = GlobalAlloc(nFlags, nBLockSize);
        API_VERIFY(hGlobal != NULL);
        GLOBAL_FLAGS_CHECK(hGlobal, 0, 0);
        CPPUNIT_ASSERT(nBLockSize == GlobalSize(hGlobal));
        m_AllocMemories.push_back(hGlobal);
    }
}

void CFTLMemTester::_ClearAllocMeomories()
{
    BOOL bRet = FALSE;
    int nCount = m_AllocMemories.size();
    for (int i = 0; i < nCount; i++)
    {
        HGLOBAL hGlobal = m_AllocMemories[i];
        GLOBAL_FLAGS_CHECK(hGlobal, 0, 0);
        API_VERIFY(NULL == GlobalFree(hGlobal));
    }
}


void CFTLMemTester::test_MemLeakBase()
{
    CMyMemLeak* pLeak = new CMyMemLeak();

    //�˴���û���ͷţ���˻��⵽�ڴ�й©
    //CHECK_OBJ_MEM_LEAK(CMyMemLeak);
    
    SAFE_DELETE(pLeak);
}

void CFTLMemTester::test_GlobalFunctions()
{
    const INT nBufferSize = 1024;
    BOOL bRet = FALSE;
    HGLOBAL hGlobalMoveable = NULL;
    HGLOBAL hGlobalFix = NULL;

    {
        //���� GMEM_FIXED ���͵��ڴ�
        hGlobalFix = GlobalAlloc(GMEM_FIXED| GMEM_ZEROINIT, nBufferSize);
        GLOBAL_FLAGS_CHECK(hGlobalFix, 0, 0);

        LPBYTE pBufFix = (LPBYTE)GlobalLock(hGlobalFix);
        HGLOBAL hGlobalGet = GlobalHandle(pBufFix);

        CPPUNIT_ASSERT((LPBYTE)hGlobalFix == pBufFix);      //���� Fix ���͵��ڴ�ʱ�����صľ������ָ��
        CPPUNIT_ASSERT((LPBYTE)hGlobalGet == pBufFix);

        StringCchCopy((LPTSTR) pBufFix, nBufferSize / sizeof(TCHAR), TEXT("FTLMemTester"));

        GLOBAL_FLAGS_CHECK(hGlobalFix, 0, 0);   //Fix �Ĳ�������Lock��,����Ƿ���� Lock/Unlock ������ν������Ϊ�˱�������������Bug�����黹�ǵ��ú�ʹ��

        API_VERIFY(GlobalUnlock(hGlobalFix));   //��ε���Ҳ������ɴ��󣬲������鱣��һ��
        //API_VERIFY(GlobalUnlock(hGlobalFix));

        GLOBAL_FLAGS_CHECK(hGlobalFix, 0, 0);


        API_VERIFY(NULL == GlobalFree(hGlobalFix)); 
#if 0
        //���� Fix ��HGLOBAL���ͷź����ٴ�ʹ�ã����������ڴ���ʴ���
        UINT nFlags = GlobalFlags(hGlobalFix);      
        CPPUNIT_ASSERT(GMEM_INVALID_HANDLE == nFlags);
        nFlags = GlobalFlags(hGlobalGet);           //ָ��ͬһ���ڴ�飬���Ҳ��Ч
        CPPUNIT_ASSERT(GMEM_INVALID_HANDLE == nFlags);
#endif 

    }

    {
        //���� GMEM_MOVEABLE ���͵��ڴ�
        hGlobalMoveable = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, nBufferSize);
        CPPUNIT_ASSERT(NULL != hGlobalMoveable);
        if (hGlobalMoveable)
        {
            GLOBAL_FLAGS_CHECK(hGlobalMoveable, 0, 0);      //�շ��䣬��δ���κ�����

            LPVOID pLock1 = GlobalLock(hGlobalMoveable);
            GLOBAL_FLAGS_CHECK(hGlobalMoveable, 1, 0);
            LPVOID pLock2 = GlobalLock(hGlobalMoveable);
            GLOBAL_FLAGS_CHECK(hGlobalMoveable, 2, 0);
            LPVOID pLock3 = GlobalLock(hGlobalMoveable);
            GLOBAL_FLAGS_CHECK(hGlobalMoveable, 3, 0);
            LPVOID pLock4 = GlobalLock(hGlobalMoveable);
            GLOBAL_FLAGS_CHECK(hGlobalMoveable, 4, 0);

            //������������صĵ�ַ��һ����
            CPPUNIT_ASSERT(pLock1 == pLock2 && pLock2 == pLock3 && pLock3 == pLock4);
            CPPUNIT_ASSERT(pLock1 != (hGlobalMoveable));    //���� Moveable ���͵��ڴ�ʱ�����ص��Ǿ��
            //API_VERIFY(NULL == GlobalFree(hGlobalMoveable));    //ע�⣺���������ط������ͷţ�����Զ�������ȫ����,����ָ�뽫���ٿ���

            //��ʼ����
            API_VERIFY(GlobalUnlock(hGlobalMoveable));    //��������3��, ����ֵΪTRUE(1)
            GLOBAL_FLAGS_CHECK(hGlobalMoveable, 3, 0);
            API_VERIFY(GlobalUnlock(hGlobalMoveable));    //��������2�Σ�����ֵΪTRUE(1)
            GLOBAL_FLAGS_CHECK(hGlobalMoveable, 2, 0);
            API_VERIFY(GlobalUnlock(hGlobalMoveable));    //��������1�Σ�����ֵΪTRUE(1)
            GLOBAL_FLAGS_CHECK(hGlobalMoveable, 1, 0);

            bRet = GlobalUnlock(hGlobalMoveable); 
            CPPUNIT_ASSERT((FALSE == bRet) && (GetLastError() == 0));  //ȫ������������ֵΪFALSE, ��GetLastErrorΪ0
            GLOBAL_FLAGS_CHECK(hGlobalMoveable, 0, 0);

            bRet = GlobalUnlock(hGlobalMoveable);                      //�Ѿ�ȫ���������ٴν����Ļ���������Ϊ ERROR_NOT_LOCKED
            CPPUNIT_ASSERT((FALSE == bRet) && (GetLastError() == ERROR_NOT_LOCKED));
            GLOBAL_FLAGS_CHECK(hGlobalMoveable, 0, 0);

            API_VERIFY(NULL == GlobalFree(hGlobalMoveable));    //�ͷ�
#if 1
            //���� Movable ��HGLOBAL���ͷź���Ի�ȡ���Ӧ�� Flags
            INT nFlags = GlobalFlags(hGlobalMoveable);
            CPPUNIT_ASSERT(GMEM_INVALID_HANDLE == nFlags);
#endif 
        }
    }

    {
        const INT ALLOC_COUNT = 100;
        LPBYTE pLockAddres[ALLOC_COUNT] = {0};

        //���� Moveable ���ڴ��Ƿ�����ƶ�
        HGLOBAL hGlobalMove = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT | GMEM_DISCARDABLE, 100 * 1024 * 1000); //�ȷ���100M
        for (int i = 0; i < ALLOC_COUNT; i++)
        {
            pLockAddres[i] = (LPBYTE)GlobalLock(hGlobalMove);
            GLOBAL_FLAGS_CHECK(hGlobalMove, 1, 0);
            _AllocLargeMeomories(1 * 1024 * 1000, 10, GMEM_FIXED | GMEM_ZEROINIT);
            API_VERIFY_EXCEPT1(GlobalUnlock(hGlobalMove), NO_ERROR);
            GLOBAL_FLAGS_CHECK(hGlobalMove, 0, 0);
        }

        BOOL bSameAddress = TRUE;
        LPBYTE pCheckAddress = pLockAddres[0];
        for (int i = 1; i < ALLOC_COUNT; i++)
        {
            if (pCheckAddress != pLockAddres[i])
            {
                bSameAddress = FALSE;
                CPPUNIT_ASSERT(FALSE); //��ô�ܵ�����?
                break;
            }
        }

        _ClearAllocMeomories();

        //CPPUNIT_ASSERT(FALSE == bSameAddress);      //���ƶ� -- ʵ��: Win7 + VS2008 ʱ��������1G���ڴ棬��Ȼ���ƶ�

        API_VERIFY(NULL == GlobalFree(hGlobalMove));
    }
}

void CFTLMemTester::test_CreateStreamOnHGlobal()
{
    BOOL bRet = FALSE;
    HRESULT hr = E_FAIL;

    const INT LOOP_COUNT = 1;
    const INT IMAGE_WIDTH = 100;
    const INT IMAGE_HEIGHT = 100;

    for(int i = 0; i < LOOP_COUNT; i++)
    {
        CAtlFile file;
        COM_VERIFY(file.Create(TEXT("res\\SimpleTest.bmp"), GENERIC_READ, 0, OPEN_EXISTING));
        if (SUCCEEDED(hr))
        {
            ULONGLONG nFileSize = 0;
            COM_VERIFY(file.GetSize(nFileSize));

            CPPUNIT_ASSERT(30054 == nFileSize); //ͼƬ�ļ��Ĵ�С

            HGLOBAL hBuffer = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, nFileSize);
            API_VERIFY(hBuffer != NULL);
            if (hBuffer)
            {
                GLOBAL_FLAGS_CHECK(hBuffer, 0, 0);
                BYTE* pBuffer = (BYTE*)::GlobalLock(hBuffer);
                API_VERIFY(pBuffer != NULL);
                GLOBAL_FLAGS_CHECK(hBuffer, 1, 0);

                COM_VERIFY(file.Read(pBuffer, nFileSize));
                file.Close();

                API_VERIFY(0 == GlobalUnlock(hBuffer)); //��������������Ϊ0

                CImage image;
                Bitmap* pGdiPlusBmp = NULL;
                {
                    CComPtr<IStream>    spStream;
                    GLOBAL_FLAGS_CHECK(hBuffer, 0, 0);
                    COM_VERIFY(CreateStreamOnHGlobal(hBuffer, TRUE, &spStream)); //����ΪTRUE,��ʾ IStream �ͷ�ʱ�Զ��ͷŶ�Ӧ�ڴ�
                    COM_VERIFY(image.Load(spStream));
                    GLOBAL_FLAGS_CHECK(hBuffer, 0, 0);
                    LPVOID pImageBits = image.GetBits();
                    CPPUNIT_ASSERT(pImageBits != pBuffer);
                    
                    //����ԭͼ����ɫ��������ĵ�Image
                    LPBYTE pBuffer2 = (BYTE*)::GlobalLock(hBuffer);
                    CPPUNIT_ASSERT(pBuffer == pBuffer2);  //����������λ�ò��䡣TODO: �ƺ�û�����ֱ�֤?
                    GLOBAL_FLAGS_CHECK(hBuffer, 1, 0);

                    BITMAPFILEHEADER* pBmpFileHeader = (BITMAPFILEHEADER*)pBuffer;
                    LPBYTE pSrcBits = pBuffer + pBmpFileHeader->bfOffBits;
                    for (int y = 0; y < IMAGE_HEIGHT; y++)
                    {
                        for(int x = 0; x < IMAGE_WIDTH; x++){
                            *pSrcBits++ = 0xFF;
                            *pSrcBits++ = 0;
                            *pSrcBits++ = 0;
                        }
                    }

                    {
                        //TODO:��������ⲿ�ִ��룬�� GlobalUnlock �᷵��FALSE.Ϊʲô?
                        pGdiPlusBmp = Bitmap::FromStream( spStream );
                        GLOBAL_FLAGS_CHECK(hBuffer, 1, 0);
                        CPPUNIT_ASSERT(NULL != pGdiPlusBmp);
                        if (pGdiPlusBmp)
                        {
                            CPPUNIT_ASSERT(pGdiPlusBmp->GetWidth() == IMAGE_WIDTH);
                            CPPUNIT_ASSERT(pGdiPlusBmp->GetHeight() == IMAGE_HEIGHT);
                        }
                        GLOBAL_FLAGS_CHECK(hBuffer, 1, 0);
                        delete pGdiPlusBmp;
                        pGdiPlusBmp = NULL;
                        GLOBAL_FLAGS_CHECK(hBuffer, 1, 0);

                        API_VERIFY(0 == GlobalUnlock(hBuffer));
                        GLOBAL_FLAGS_CHECK(hBuffer, 0, 0);

                        //spStream.Detach();
                    }
                }//���� IStream �ķ�Χ���ͷź� hBuffer Ҳ���ͷţ��� Image �����ݻᱣ��
                INT nFlags = GlobalFlags(hBuffer);
                CPPUNIT_ASSERT(GMEM_INVALID_HANDLE == nFlags);

                //GLOBAL_FLAGS_CHECK(hBuffer, 1, 0);

                //SIZE_T sizeCheck = GlobalSize(hBuffer);
                //CPPUNIT_ASSERT(sizeCheck == nFileSize);

                //API_VERIFY(0 == GlobalUnlock(hBuffer)); //��ȫ�ͷ�
                bRet = (NULL == GlobalFree(hBuffer));  //��� CreateStreamOnHGlobal ΪTRUE���˴������ͷŵĻ�����ʧ�ܣ�����ԭ��Ϊ ERROR_INVALID_HANDLE
                CPPUNIT_ASSERT((FALSE == bRet) && (GetLastError() == ERROR_INVALID_HANDLE));

                //SIZE_T sizeCheckAfterFree = GlobalSize(hBuffer);
                //CPPUNIT_ASSERT(sizeCheckAfterFree == 0);

                CPPUNIT_ASSERT(image.GetWidth() == IMAGE_WIDTH);
                CPPUNIT_ASSERT(image.GetHeight() == IMAGE_HEIGHT);

                //LPVOID pImageBuf = image.GetBits();

                //CPPUNIT_ASSERT(IsBadReadPtr(pBuffer, nFileSize));
                //CPPUNIT_ASSERT(FALSE == IsBadReadPtr(pImageBuf, nFileSize));

                COM_VERIFY(image.Save(TEXT("res\\saveAfterReleaseImage.bmp"), GUID_NULL));
            }
        }
    }
}