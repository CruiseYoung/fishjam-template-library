#include "StdAfx.h"
#include "FTLGdiTester.h"
#include <WindowsX.h>
#include <ftlGdi.h>
#include <ftlDebug.h>

CFTLGdiTester::CFTLGdiTester()
{
    m_hDesktopDC = ::GetWindowDC(NULL);
    m_hDC = NULL;
    m_hBitmap = NULL;
    m_hOldBitmap = NULL;
}

CFTLGdiTester::~CFTLGdiTester()
{
    ::ReleaseDC(NULL, m_hDesktopDC);
}

void CFTLGdiTester::setUp()
{
    m_hDC = ::CreateCompatibleDC(m_hDesktopDC);
    API_ASSERT(NULL != m_hDC);
}

void CFTLGdiTester::tearDown()
{
    BOOL bRet = FALSE;
    API_VERIFY(::DeleteDC(m_hDC));
    m_hDC = NULL;
}

void CFTLGdiTester::setBitmap()
{
    m_hBitmap = :: CreateCompatibleBitmap(m_hDesktopDC, BITMAP_WIDTH, BITMAP_HEIGHT);
    API_ASSERT(NULL != m_hBitmap);
    m_hOldBitmap = (HBITMAP)::SelectObject(m_hDC, (HGDIOBJ)m_hBitmap);
}

void CFTLGdiTester::releaseBitmap()
{
    m_hBitmap = (HBITMAP)::SelectObject(m_hDC, (HGDIOBJ)m_hOldBitmap);
    ::DeleteObject(m_hBitmap);
    m_hBitmap = NULL;
}

void CFTLGdiTester::test_Clipping()
{
    BOOL bRet = FALSE;

    CRect rcClip;
    rcClip.SetRectEmpty();
    
    //�մ����õ�ʱ����һ�� [0,0] x [1,1] �ľ���
    const CRect rcInit(0,0,1,1);
    int nClipType = GetClipBox(m_hDC, rcClip);
    CPPUNIT_ASSERT(SIMPLEREGION  == nClipType);
    CPPUNIT_ASSERT(rcInit == rcClip);


    //ѡ��λͼ�󣬲ü�����λͼ�Ĵ�С [0,0] x [100,100]
    setBitmap();
    const CRect rcBitmap(0,0, BITMAP_WIDTH, BITMAP_HEIGHT);
    nClipType = GetClipBox(m_hDC, rcClip);
    CPPUNIT_ASSERT(SIMPLEREGION  == nClipType);
    CPPUNIT_ASSERT(rcBitmap == rcClip);

    //ͨ��ԭ�ü�����ָ�����εġ��롱���㣬�����µĲü���
    //��ʱ��Ϊ������ǿվ��� [0,0]x[0,0]�������µĲü���Ϊ�� -- 
    nClipType = ::IntersectClipRect(m_hDC, 0,0,0,0);
    CPPUNIT_ASSERT(COMPLEXREGION == nClipType);    
    nClipType = GetClipBox(m_hDC, rcClip);
    CPPUNIT_ASSERT(NULLREGION == nClipType);
    const CRect rcEmpty(0,0,0,0);
    CPPUNIT_ASSERT(rcEmpty == rcClip);

    //������λͼһ��������򣬲���Ϊ�ü��� -- �ü�����λͼ�Ĵ�С [0,0] x [100,100]
    HRGN hRgnBmp = ::CreateRectRgnIndirect(rcBitmap);
    nClipType = ::SelectClipRgn(m_hDC, hRgnBmp);
    CPPUNIT_ASSERT(SIMPLEREGION == nClipType);
    nClipType = GetClipBox(m_hDC, rcClip);
    CPPUNIT_ASSERT(SIMPLEREGION  == nClipType);
    CPPUNIT_ASSERT(rcBitmap == rcClip);

    //�Ӳü������ų������Ͻ� [0,0]x[50,50] �ľ���(����һ����)���γ�һ�����ӵĲü���
    nClipType = ExcludeClipRect(m_hDC, 0,0,50,50);
    CPPUNIT_ASSERT(COMPLEXREGION == nClipType);
    nClipType = GetClipBox(m_hDC, rcClip);      //����ʱ��Χ�ü����ľ�����ȻΪ rcBitmap
    CPPUNIT_ASSERT(COMPLEXREGION == nClipType);
    CPPUNIT_ASSERT(rcBitmap == rcClip);

    //�ٴӲü����м�ȥ���½ǵľ��� -- ���ʣ�µľ�ֻ���Ұ�ߵľ��βü�����
    nClipType = ExcludeClipRect(m_hDC, 0,50,50,100);
    CPPUNIT_ASSERT(COMPLEXREGION == nClipType); //��ʱ�ղü��꣬����ֵ��Ȼ�� ���Ӳü���
    nClipType = GetClipBox(m_hDC, rcClip);      //����ʱ��Χ�ü����ľ�����ȻΪ rcBitmap
    CPPUNIT_ASSERT(SIMPLEREGION == nClipType);  //�� ��ʱͨ�����¼��㣬�Ѿ����Լ�����Ǽ򵥾��� ��
    const CRect rcRightHalfBitmap(50,0,100,100);
    CPPUNIT_ASSERT(rcRightHalfBitmap == rcClip);

    //��ʣ�µ��Ұ�ߵľ��κ�ռ��λͼ�������ε��������XOR�����ʣ�����ߵľ���Ϊ�ü���
    nClipType = ::ExtSelectClipRgn(m_hDC, hRgnBmp, RGN_XOR);
    CPPUNIT_ASSERT(SIMPLEREGION == nClipType);
    nClipType = GetClipBox(m_hDC, rcClip);
    CPPUNIT_ASSERT(SIMPLEREGION == nClipType);
    const CRect rcLeftHalfBitmap(0,0,50,100);
    CPPUNIT_ASSERT(rcLeftHalfBitmap == rcClip);

    
    //�� ����˵ SelectClipRgn(xx,NULL)����ΪNULL����ʵ����� rcBitmap ?
    //   ʹ�� RGN_COPY �� NULL ѡΪ�ü��������µĲü���Ϊ�� -- Ϊʲô��������
    //::SelectClipRgn(m_hDC,NULL);
    //nClipType = ::ExtSelectClipRgn(m_hDC, NULL, RGN_COPY);
    //CPPUNIT_ASSERT(NULLREGION == nClipType);
    //nClipType = GetClipBox(m_hDC, rcClip);
    //CPPUNIT_ASSERT(NULLREGION == nClipType);
    //CPPUNIT_ASSERT(rcEmpty == rcClip);


    API_VERIFY(DeleteObject(hRgnBmp));
    releaseBitmap();
}

void CFTLGdiTester::test_GdiObjectInfoDump()
{
    BOOL bRet = FALSE;
    FTL::CFGdiObjectInfoDump gdiObjDump;

    //���� OBJ_BITMAP
    HBITMAP hBmp = ::CreateCompatibleBitmap(m_hDesktopDC, 100,100);
    CHECK_BITMAP_SELECTABLE(hBmp);

    //�ոմ�����λͼ��û�� Dimension ? ��Ҫ���õĻ�����ʲô�� ?
    SIZE szBitmapDimension = {0};
    API_VERIFY(::GetBitmapDimensionEx(hBmp, &szBitmapDimension));
    FTLTRACE(TEXT("%d,%d\n"), szBitmapDimension.cx, szBitmapDimension.cy);
    CPPUNIT_ASSERT(0 == szBitmapDimension.cx && 0 == szBitmapDimension.cy);

    CPPUNIT_ASSERT(OBJ_BITMAP == ::GetObjectType(hBmp));
   //�մ����ģ��ܻ�ȡ����Ϣ
    CPPUNIT_ASSERT(gdiObjDump.GetGdiObjectInfo(hBmp));
    FTLTRACE(TEXT("%s\n"), gdiObjDump.GetGdiObjectInfoString());
    API_VERIFY(::DeleteObject(hBmp));
    //ɾ���Ժ�ͻ�ȡ������
    CPPUNIT_ASSERT(!gdiObjDump.GetGdiObjectInfo(hBmp));

    //���� Canvas �е� CreateDIBSection ������ DIBSECTION
    CFCanvas canvas;
    API_VERIFY(canvas.Create(NULL, 100, 100, 32));
    HBITMAP hDibBitmap = canvas.GetMemoryBitmap();
    
#if 0
    //��Ϊ���λͼ�Ѿ���ѡ��canvas ��MemoryDC�У����Բ���"Selectable"�� -- ����������Ļ�������ֶ���
    CHECK_BITMAP_SELECTABLE(hDibBitmap);
#endif 

    CPPUNIT_ASSERT(OBJ_BITMAP == ::GetObjectType(hDibBitmap));
    CPPUNIT_ASSERT(gdiObjDump.GetGdiObjectInfo(hDibBitmap));
    FTLTRACE(TEXT("%s\n"), gdiObjDump.GetGdiObjectInfoString());
    canvas.Release();
    //�ͷ���DIB������Ϊ hDibBitmap ��ָ����ԭ����λ�ã���ȡ��Ϣ(���ܻ�ȡ��)
    FTLASSERT(hDibBitmap != NULL);
    CPPUNIT_ASSERT(!gdiObjDump.GetGdiObjectInfo(hDibBitmap));


    HPEN hStockPen = GetStockPen(NULL_PEN);
    CHECK_GDIOBJECT_VALIDATE(hStockPen);
    CPPUNIT_ASSERT(gdiObjDump.GetGdiObjectInfo(hStockPen));
    FTLTRACE(TEXT("%s\n"), gdiObjDump.GetGdiObjectInfoString());

    HBRUSH hStockBrush = GetStockBrush(NULL_BRUSH);
    CPPUNIT_ASSERT(gdiObjDump.GetGdiObjectInfo(hStockBrush));
    FTLTRACE(TEXT("%s\n"), gdiObjDump.GetGdiObjectInfoString());

    HFONT hStockFont = GetStockFont(DEVICE_DEFAULT_FONT);
    CPPUNIT_ASSERT(gdiObjDump.GetGdiObjectInfo(hStockFont));
    FTLTRACE(TEXT("%s\n"), gdiObjDump.GetGdiObjectInfoString());

    HPALETTE hStockPalette = (HPALETTE)GetStockObject(DEFAULT_PALETTE);
    CPPUNIT_ASSERT(gdiObjDump.GetGdiObjectInfo(hStockPalette));
    FTLTRACE(TEXT("%s\n"), gdiObjDump.GetGdiObjectInfoString());
   
}

void CFTLGdiTester::test_GdiObjectChecker()
{
    BOOL bRet = FALSE;

    HDC hMemoryDC = ::CreateCompatibleDC(m_hDesktopDC);
    HBITMAP hBmp = ::CreateCompatibleBitmap(m_hDesktopDC, 100,100);

    //�ոմ�����λͼ, ����Ч��, ����û��ѡ���κ�DC��
    FTL::CFGdiObjectChecker   checker(hMemoryDC);
    CPPUNIT_ASSERT(checker.IsValidate(hBmp));
    CPPUNIT_ASSERT(!checker.IsSelected(hBmp));

    //��λͼѡ�� MemoryDC ��
    HBITMAP hOldBmp = SelectBitmap(hMemoryDC, hBmp);
    CPPUNIT_ASSERT(checker.IsSelected(hBmp));

#ifdef CHECK_NORMAL
    //������������ -- �Ƚ� BITMAP ��MemoryDC��ѡ�����ٽ����ͷ�
    SelectBitmap(hMemoryDC, hOldBmp);

    CPPUNIT_ASSERT(checker.IsValidate(hBmp));
    //�Ѿ�ѡ������
    CPPUNIT_ASSERT(!checker.IsSelected(hBmp));

    //�ͷ��Ժ�λͼ������Ч
    API_VERIFY(::DeleteObject(hBmp));
    CPPUNIT_ASSERT(!checker.IsValidate(hBmp));

    API_VERIFY(::DeleteDC(hMemoryDC));

#else // defined( CHECK_DELETE_BEFORE_SELECTOUT)
	UNREFERENCED_PARAMETER(hOldBmp);

    //��û�д�MemoryDC��ѡ�� BITMAP ������£�ֱ�� DeleteObject, API�᷵��TRUE -- ��ʵ���ϸ� BITMAP ��δ�ͷš�
    CPPUNIT_ASSERT(::DeleteObject(hBmp));

    //��Ȼ������ ::DeleteObject, ����ΪBITMAPδ��MemoryDC��ѡ�������λͼ��Դ��δ�ͷţ���Ϊѡ��״̬
    CPPUNIT_ASSERT(checker.IsValidate(hBmp));  
    CPPUNIT_ASSERT(checker.IsSelected(hBmp));

    //�ͷ��ڴ�DC -- ��ʱϵͳ���Զ���BITMAP����Դ�ͷŵ������������Դй© (������һ�ֲ��õ�ʹ�÷�ʽ)
    API_VERIFY(::DeleteDC(hMemoryDC));
    CPPUNIT_ASSERT(!checker.IsValidate(hBmp));  
    CPPUNIT_ASSERT(0 == ::GetObjectType(hBmp));  //��ΪhBmp �Ѿ��ͷţ������޷��ٻ�ȡ��������
    //ERROR_ACCESS_DENIED
#endif
    

}

void CFTLGdiTester::test_Region()
{
    BOOL bRet = FALSE;
    CRect rcRegion;


    //[0,0] x [100,100] ���ε��ĸ���
    POINT ptPolygon[] = { {0,0}, {0,100}, {100,100}, {100,0}};

    HRGN hRgnPolygon = CreatePolygonRgn(ptPolygon, _countof(ptPolygon), ALTERNATE);
    int rgnType = ::GetRgnBox(hRgnPolygon, rcRegion);
    CPPUNIT_ASSERT(SIMPLEREGION == rgnType);
    CPPUNIT_ASSERT(0 == rcRegion.left);
    CPPUNIT_ASSERT(0 == rcRegion.top);
    CPPUNIT_ASSERT(100 == rcRegion.right);
    CPPUNIT_ASSERT(100 == rcRegion.bottom);

    //[0,0] x [100,100] ���ε��ĸ��� �� [150,150]->[150,200]->[200,150] ��������
    POINT ptPolyPolygon[] = { 
         {0,0}, {0,100}, {100,100}, {100,0},
         {150,150}, {150,200}, {200,150}
    };

    INT nPolyCount[] = {4, 3};
    HRGN hRgnPolyPolygon = ::CreatePolyPolygonRgn(ptPolyPolygon, nPolyCount, _countof(nPolyCount), ALTERNATE);
    rgnType = ::GetRgnBox(hRgnPolyPolygon, rcRegion);
    CPPUNIT_ASSERT(COMPLEXREGION == rgnType);
    //���صľ����ǰ�������(һ������ + һ������)����С���о���
    CPPUNIT_ASSERT(0 == rcRegion.left);
    CPPUNIT_ASSERT(0 == rcRegion.top);
    CPPUNIT_ASSERT(200 == rcRegion.right);
    CPPUNIT_ASSERT(200 == rcRegion.bottom);

    //�ͷŴ�����������Դ
    API_VERIFY(::DeleteObject(hRgnPolygon));
    API_VERIFY(::DeleteObject(hRgnPolyPolygon));
}

void CFTLGdiTester::test_CompatibleBitmap()
{
    //�ܽ᣺��������λͼʱ���ﲻ�ܡ�ʹ�òŴ����õ��ڴ����DC

    BOOL bRet = FALSE;
    HBITMAP hDesktopBmp = NULL;
    BITMAP bmpInfo = {0};

    FTL::HDCProperty prop;
    FTL::CFGdiUtil::GetHDCProperty(m_hDesktopDC, &prop);
    FTLTRACE(TEXT("%s\n"), prop.GetPropertyString(HDC_PROPERTY_GET_DEVICE_CAPS));

    //ֱ�ӻ�ȡ����DC�е�λͼ����
    hDesktopBmp = (HBITMAP)::GetCurrentObject(m_hDesktopDC,OBJ_BITMAP);
    API_ASSERT(NULL != hDesktopBmp);
    CPPUNIT_ASSERT(NULL != hDesktopBmp);

#pragma TODO(Why CAN NOT Get Desktop bitmap info)
    //��ȡ����DC��λͼ��������� -- Ϊʲô�ܻ�ȡ��λͼ����������ܻ�ȡ��������Ϣ��
    //  �����з��� ::GetObject ����ʧ�� -- ������Ϊ ERROR_INSUFFICIENT_BUFFER(122), �����ǲ��ܿ���̻�ȡ ?
#if 0
    //CHECK_BITMAP_SELECTABLE(hDesktopBmp);
    API_VERIFY( 0 != ::GetObject(hDesktopBmp, sizeof(bmpInfo), &bmpInfo));
    CPPUNIT_ASSERT(0 == bmpInfo.bmWidthBytes
        && 0 == bmpInfo.bmPlanes
        && 0 == bmpInfo.bmBitsPixel);
    hDesktopBmp = NULL;
#endif 

    //����������DC�ļ���λͼ -- ֧��32λ���
    hDesktopBmp = ::CreateCompatibleBitmap(m_hDesktopDC, 10,10);
    ::GetObject(hDesktopBmp, sizeof(bmpInfo), &bmpInfo);
    CPPUNIT_ASSERT(40 == bmpInfo.bmWidthBytes
        && 1 == bmpInfo.bmPlanes
        && 32 == bmpInfo.bmBitsPixel);

    //������������ݵ��ڴ�DC������������ϴ�������λͼ -- �����ڴ�DC��û��ѡ��λͼ����˼���λͼֻ֧��������ĺڰ���ɫ
    HDC hMemDcFromDesktop = ::CreateCompatibleDC(m_hDesktopDC);
    HBITMAP hMemDCBmp = ::CreateCompatibleBitmap(hMemDcFromDesktop, 10,10);
    ::GetObject(hMemDCBmp, sizeof(bmpInfo), &bmpInfo);
    CPPUNIT_ASSERT(2 == bmpInfo.bmWidthBytes
        && 1 == bmpInfo.bmPlanes
        && 1 == bmpInfo.bmBitsPixel);
    ::DeleteObject(hMemDCBmp);

    //�������õ�32λ����λͼѡ������ڴ�DC��Ȼ�����´�������λͼ -- ��ʱ����λͼҲ��֧��32λ���
    HBITMAP hOldBmp = (HBITMAP)::SelectObject(hMemDcFromDesktop, hDesktopBmp);
    hMemDCBmp = ::CreateCompatibleBitmap(hMemDcFromDesktop, 10,10);
    API_VERIFY(0 != ::GetObject(hMemDCBmp, sizeof(bmpInfo), &bmpInfo));
    CPPUNIT_ASSERT(40 == bmpInfo.bmWidthBytes
        && 1 == bmpInfo.bmPlanes
        && 32 == bmpInfo.bmBitsPixel);

	SelectObject(hMemDcFromDesktop, hOldBmp);
	API_VERIFY(::DeleteObject(hMemDCBmp));
    API_VERIFY(::DeleteDC(hMemDcFromDesktop));
    API_VERIFY(::DeleteObject(hDesktopBmp));
}

void CFTLGdiTester::test_CalcPointPosQuadrant()
{
	CPPUNIT_ASSERT_EQUAL(posTopLeft, CFGdiUtil::CalcPointPosQuadrant(CRect(1,1,5,3), CPoint(1, 0)));
	CPPUNIT_ASSERT_EQUAL(posTopRight, CFGdiUtil::CalcPointPosQuadrant(CRect(1,1,5,3), CPoint(5, 0)));
	CPPUNIT_ASSERT_EQUAL(posRightTop, CFGdiUtil::CalcPointPosQuadrant(CRect(1,1,5,3), CPoint(10, 0)));
}


LPCTSTR CFTLGdiTester::GetZoomModeString(INT nZoomMode)
{
	switch (nZoomMode)
	{
	case CFCalcRect::modeAutoFitIfBigSize:
		return TEXT("AutoFitIfBigSize");
	case CFCalcRect::modeAutoFit:
		return TEXT("AutoFit");
	default:
		ATLASSERT(FALSE);
		return NULL;
	}
}

void CFTLGdiTester::test_CalcRectCheckInfo(LPCTSTR pszPromptText, CalcRectCheckInfo* pCheckInfo, int nCount)
{
	ATLTRACE(TEXT("Enter Test %s\n"), pszPromptText);
	for (int index = 0; index < nCount; index++ )
	{
		CFCalcRect* pCalcRect = CFCalcRect::GetCalcRectObject(
			(FTL::CFCalcRect::ZoomMode)pCheckInfo[index].nZoomMode
			//pCheckInfo[index].pRcCurrent
			);
		if (pCalcRect)
		{
			CRect rcResult = pCalcRect->GetFitRect(
				pCheckInfo[index].rcClient,
				pCheckInfo[index].szImage
				//pCheckInfo[index].dZoomFactor,		
				//FALSE,										//bFirst
				//FALSE										//bFirstView
				);
			SAFE_DELETE(pCalcRect);

			FTL::CFRectDumpInfo	rcDumpInfoClient(pCheckInfo[index].rcClient);
			FTL::CFRectDumpInfo	rcDumpInfoCheck(pCheckInfo[index].rcCheck);
			FTL::CFRectDumpInfo	rcDumpInfoResult(rcResult);

			FTLTRACE(TEXT("\tCheck [%d] Mode=%s(%d), rcClient=%s, Size=%dx%d, rcCheck=%s, rcResult=%s\n"), 
				index,
				GetZoomModeString(pCheckInfo[index].nZoomMode),
				pCheckInfo[index].nZoomMode,
				rcDumpInfoClient.GetConvertedInfo(),
				pCheckInfo[index].szImage.cx, pCheckInfo[index].szImage.cy,
				rcDumpInfoCheck.GetConvertedInfo(),
				rcDumpInfoResult.GetConvertedInfo()
				);
			CPPUNIT_ASSERT(rcResult == pCheckInfo[index].rcCheck);
		}
	}

	FTLTRACE(TEXT("Leave Test %s\n"), pszPromptText);
}


void CFTLGdiTester::test_CalcRect_RectBigThanSize()
{
	CalcRectCheckInfo	calcRectCheckInfo[] = 
	{
		//Rect > Size
		{ 
			CFCalcRect::modeAutoFitIfBigSize,
			//NULL,
			CRect(100, 200, 1024 + 100, 768 + 200),
			CSize(100, 100),
			CRect(562, 534, 662, 634),	//100x100
			//1.0
		},

		{ 
			CFCalcRect::modeAutoFit,
			//NULL,
			CRect(100, 200, 1024 + 100, 768 + 200),
			CSize(100, 100),
			CRect(228, 200, 996, 968),	//768x768
			//1.0
		},
	};

	test_CalcRectCheckInfo(TEXT("RectBigThanSize"), calcRectCheckInfo, _countof(calcRectCheckInfo));
}

void CFTLGdiTester::test_CalcRect_RectSmallThanSize()
{

}


void CFTLGdiTester::test_CompareBitmapData()
{
    int nWidth = 100, nHeight = 100;

    BOOL bRet = FALSE;
    HWND hWndDesktop = GetDesktopWindow();
    HDC hDcDesktop = GetDC(hWndDesktop);

    int nBpps[] = {32, 24, 16 }; 
    int nCmpResultSize = nWidth * nHeight * 4;
    CFMemAllocator<byte> cmpResult(nCmpResultSize);

    for (int i = 0; i< _countof(nBpps); i++)
    {
        FTL::CFCanvas canvas1, canvas2;

        API_VERIFY(canvas1.Create(hWndDesktop, nWidth, nHeight, nBpps[i]));
        API_VERIFY(canvas2.Create(hWndDesktop, nWidth, nHeight, nBpps[i]));

        API_VERIFY(BitBlt(canvas1.GetCanvasDC(), 0, 0, nWidth, nHeight, hDcDesktop, 0, 0, SRCCOPY));
        API_VERIFY(BitBlt(canvas2.GetCanvasDC(), 0, 0, nWidth, nHeight, hDcDesktop, 0, 0, SRCCOPY));
        
        CDC memDC;
        memDC.Attach(canvas2.GetCanvasDC());
        memDC.FillSolidRect(0, 0, 5, 5, RGB(255, 0, 0));

        memDC.FillSolidRect(nWidth - 5, nHeight - 5, 5, 5, RGB(255, 0, 0));

        memDC.Detach();

        FTL::CFStringFormater formaterName1, formaterName2;
        formaterName1.Format(TEXT("Canvas1_%d.bmp"), nBpps[i]);
        formaterName2.Format(TEXT("Canvas2_%d.bmp"), nBpps[i]);

        FTL::CFGdiUtil::SaveBitmapToFile(canvas1.GetMemoryBitmap(), formaterName1.GetString());
        FTL::CFGdiUtil::SaveBitmapToFile(canvas2.GetMemoryBitmap(), formaterName2.GetString());
        RECT rcMinDiff = {0};
        int nCmpResult = CFGdiUtil::CompareBitmapData(nWidth, nHeight, nBpps[i], 
            canvas1.GetBuffer(), canvas2.GetBuffer(), &rcMinDiff,
            cmpResult.GetMemory(), nCmpResultSize, 0xFF);
        FTLTRACE(TEXT("nBpp=%d, nCmpResult=%d\n"), nBpps[i], nCmpResult);

        CPPUNIT_ASSERT(nCmpResult == 50);
    }
 
    ReleaseDC(hWndDesktop, hDcDesktop);
}

void CFTLGdiTester::test_LargetBitmap()
{
	BOOL bRet = FALSE;
	CFCanvas	canvas;
	CRect rcLarge;
	rcLarge.SetRect(0, 0, 5124, 5124);

	API_VERIFY(canvas.Create(NULL, rcLarge.Width(), rcLarge.Height(), 32));
	HBITMAP hDibBitmap = canvas.GetMemoryBitmap();
	CPPUNIT_ASSERT(hDibBitmap != NULL);

    
    CWindowDC wndDC(CWnd::FromHandle(GetDesktopWindow()));
    ::StretchBlt(canvas.GetCanvasDC(), 0, 0, rcLarge.Width(), rcLarge.Height(), wndDC.GetSafeHdc(), 0, 0, 1080, 1080, SRCCOPY);

	//CDC memDC;
	//API_VERIFY(memDC.Attach(canvas.GetCanvasDC()));
	//memDC.FillSolidRect(&rcLarge, RGB(255, 0, 0));
	//memDC.Detach();

    API_VERIFY(canvas.SaveToBmpFile(TEXT("LargeBitmap.bmp")));
	//API_VERIFY(CFGdiUtil::SaveBitmapToFile(canvas.GetMemoryBitmap(), TEXT("LargeBitmap.bmp")));


	canvas.Release();
}