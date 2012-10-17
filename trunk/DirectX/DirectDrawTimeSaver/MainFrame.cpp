#include "Stdafx.h"
#include <ddraw.h>
#include ".\mainframe.h"


#pragma comment(lib, "ddraw.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    ON_WM_CREATE()
    ON_WM_TIMER()
    ON_WM_DESTROY()
    ON_WM_KEYDOWN()
END_MESSAGE_MAP()


CMainFrame::CMainFrame(void)
{
    m_pDDraw            = NULL;
    m_pDDSPrimary       = NULL;
#ifdef FULLSCREEN_RUNNING
    m_pDDSBack          = NULL;
#endif 
    m_pDDSBackground    = NULL;
} 

CMainFrame::~CMainFrame(void)
{
    SAFE_RELEASE(m_pDDSBackground);
#ifdef FULLSCREEN_RUNNING
    SAFE_RELEASE(m_pDDSBack);
#endif
    SAFE_RELEASE(m_pDDSPrimary);
    SAFE_RELEASE(m_pDDraw);
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    HRESULT hr = E_FAIL;
    //��ʼ�������
    srand(GetTickCount());

    //RECT rcClient = { 0,0,800,600 };
    //AdjustWindowRectEx( &rcClient, GetWindowStyle(hWnd), GetMenu(hWnd) != NULL, GetWindowExStyle(hWnd));
    //lpCreateStruct->cx = 800;
    //lpCreateStruct->cy = 600;

    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

#if 0
    //ʹ��COM API �ķ�ʽ���� DDraw
    DX_VERIFY(CoCreateInstance(CLSID_DirectDraw, NULL, CLSCTX_ALL, IID_IDirectDraw, (LPVOID *)&m_pDDraw));
    DX_VERIFY(m_pDDraw->Initialize(NULL));
#endif 

    //����DirectDraw���� -- �����Ĵ����� ��DDERR_INVALIDDIRECTDRAWGUID
    //DX_VERIFY(DirectDrawCreate(NULL,//�Կ�������GUID��NULL����Ĭ��Ӳ��
    //    &m_pDDraw,
    //    NULL));

    DX_VERIFY(DirectDrawCreateEx(NULL, (LPVOID *)&m_pDDraw, IID_IDirectDraw7, NULL));
    if (SUCCEEDED(hr))
    {
        //��鲻�������ӿڣ�DirectX����ʹ�� QI ��
        //COM_DETECT_INTERFACE_FROM_REGISTER(m_pDDraw);
        COM_DETECT_INTERFACE_FROM_LIST(m_pDDraw);

        //CComPtr<IDirectDraw7> spDdClipper;
        //DX_VERIFY(m_pDDraw->QueryInterface(IID_IDirectDraw7, (LPVOID *)&spDdClipper));
    }
    else
    {
        return -1;
    }

#ifdef FULLSCREEN_RUNNING
    DWORD dwCooperativeLevel = DDSCL_FULLSCREEN | DDSCL_EXCLUSIVE | DDSCL_ALLOWMODEX | DDSCL_ALLOWREBOOT;
#else
    DWORD dwCooperativeLevel = DDSCL_NORMAL;
#endif
    //����Э���������Դ�����ʾ����ȫ��
    ASSERT(::IsWindow(m_hWnd));
    COM_VERIFY(m_pDDraw->SetCooperativeLevel(m_hWnd,dwCooperativeLevel));

    //������ʾģʽ -- ͸����(���漰Ӳ��)������Ƶģʽת��
    COM_VERIFY(m_pDDraw->SetDisplayMode(1280,   //dwWidth
        1024,                                   //dwHeight
        32,                                     //dwBPP( bits per pixel, 8,16,24,32)
        0,                                      //dwRefreshRte(ˢ��Ƶ�ʣ�0 ��Ĭ��ֵ)
        0                                       //dwFlags
        ));


    DDSURFACEDESC2              ddsd = {0};
    ddsd.dwSize = sizeof(ddsd);
    // ddsd.lPitch <== ����ÿ�е��ֽ��������ڰ�ȫ�ķ����ڴ棬ʹ�÷�ʽΪ�� ddsd.lpSurface[x+y*ddsd.lPitch]=color;
    //    ( ͨ���� ��� * (dpp/8)��4�ֽڶ��룿 )
    // ddsd.lpSurface <== ȡ��ָ�������ڵ��ڴ�(ϵͳ�ڴ���Դ�)��ָ��
    // ddsd.ddckCKDestBlt <== ����Ŀ����ɫ�������ڽ���λͼת������ʱ����д�����ɫ
    // ddsd.ddckCKSrcBlt <== ��ʾԭ������ɫ���ɶ�λͼ�趨͸����ɫ��
    // ddsd.ddpfPixelFormat <== ��������ظ�ʽ

#ifdef FULLSCREEN_RUNNING
    //ָ��ddsd����Ч���Ĳ�������������һ����̨����������ҳ��(���ֻ�����Ǹ���-COMPLEX-����)
    ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;// | DDSD_HEIGHT | DDSD_WIDTH;
    //ddsd.dwHeight = 1024;
    //ddsd.dwWidth = 1280;

    //ָ����һ����ҳ��(DDSCAPS_PRIMARYSURFACE)��һ����תҳ��(DDSCAPS_FLIP)��һ������ҳ�� (DDSCAPS_COMPLEX)��
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
    //ddsd.ddsCaps.dwCaps2 <== ��������3Dͼ��
    ddsd.dwBackBufferCount = 1; //��̨����ҳ�ĸ�������������Ϊ1�������������Ҫ����һ���������һ����̨����ķ�ҳ��
    DX_VERIFY(m_pDDraw->CreateSurface(&ddsd, &m_pDDSPrimary, NULL));

    //��ȡ�����ĺ�̨�����ҳ�� -- ������ʱ��ô�ã�����˵Ӳ����ѭ���ķ�ʽ�������Կ���͸��
    DDSCAPS2                    ddscaps = {0};
    ddscaps.dwCaps = DDSCAPS_BACKBUFFER; 
    DX_VERIFY(m_pDDSPrimary->GetAttachedSurface(&ddscaps, &m_pDDSBack)); 
#else
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    DX_VERIFY(m_pDDraw->CreateSurface(&ddsd, &m_pDDSPrimary, NULL));
#endif 
    
    //��������ҳ��
    ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
    ddsd.dwHeight = 480; //��������ҳ��Ĵ�С���������ó���ͼƬ��С��ͬ
    ddsd.dwWidth = 640;
    DX_VERIFY(m_pDDraw->CreateSurface( &ddsd, &m_pDDSBackground, NULL ));

    //����λͼ  ���� ע�⣺�˴��Ĵ�������д���
    //hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), szBitmap,IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
    //DeleteObject(hbm);
    //DDCopyBitmap(lpDDSOne, hbm, 0, 0, 640, 480);
    //lpDDPal = DDLoadPalette(lpDD, szBackground);	//SDK��Ddutil.h�ĺ�������Back.bmp�ļ��д���һ��DirectDrawPalette����
    //lpDDSPrimary->SetPalette(lpDDPal);	//���õ�ɫ��
    //DDReLoadBitmap(m_pDDSBackground, szBackground);  // ��λͼ�����̨������������DDCopyBitmap�������ļ���������̨����������չΪ�ʵ��ĳߴ磩

    //��ʱͨ����Ҫ����ҳ���ϻ��Ƴ�ʼ����Ϣ
    m_TimerID = SetTimer(0,1000, NULL);
    return 0;
}

void CMainFrame::OnTimer(UINT nIDEvent)
{
    HRESULT hr = E_FAIL;
    HDC hdc = NULL;
    //��ȡ�豸�����ĵľ���������ñ���
    //����������ڴ棨�������������滹�����е�һ���֣���ȷ�����Ӧ�ó����ϵͳ����ͬʱ��������ڴ档
    //  m_pDDSPrimary->GetDC() <==> m_pDDSPrimary->ReleaseDC()
    //  m_pDDSPrimary->Lock() <==> m_pDDSPrimary->Unlock()  -- ֮����Խ����ڴ�Ķ�д����
    //    DDLOCK_READONLY -- ֻ����ʽ��ס
    //    DDLOCK_SURFACEMEMORYPTR -- Ӧ����һ��ָ��RECT���˵���Чָ��(lpSurface[x+y*ddsd.lPitch])
    //      lPitch�� 1280x1024,32λɫ ʱ�� 5120, ddsd.ddpfPixelFormat.dwRGBBitCount �� 32
    //      ���ԣ���ֵ��ʱ���Ƿ������ (DWORD*)lpSurface[x+y*ddsd.lPitch/(dwRGBBitCount/8)] = RGB32() ?
    //    DDLOCK_WRITEONLY -- ��д�ķ�ʽ��ס  
#ifdef FULLSCREEN_RUNNING
    DDSURFACEDESC2 ddsd = {0};
    ddsd.dwSize = sizeof(ddsd);
    DX_VERIFY(m_pDDSBack->Lock(NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL));
    if (DDERR_SURFACELOST == hr)
    {
        //���Իָ�
        DX_VERIFY_EXCEPT1(m_pDDSBack->Restore(), DDERR_IMPLICITLYCREATED);
        if (FAILED(hr))
        {
            return ;
        }
        DX_VERIFY_EXCEPT1(m_pDDSBack->Lock(NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT, NULL), DDERR_SURFACELOST);
        if (FAILED(hr))
        {
            //���Իָ����ɹ������˳����λ���
            return ;
        }
    }
    DWORD* pSurfaceColor = ((DWORD*)ddsd.lpSurface);
    //ע�⣺dwRGBBitCount �п�����15(XRGB555)����ˣ����ַ�������
    DWORD dwOffsetSize = ddsd.lPitch / (ddsd.ddpfPixelFormat.dwRGBBitCount / 8);
    for (DWORD y = 0; y < ddsd.dwHeight; y++)
    {
        for (DWORD x = 0; x < ddsd.dwWidth; x++)
        {
            //pSurfaceColor[x + y * dwOffsetSize] = RGB( rand() % 256 , rand() %256, rand() % 256);

            //С��ϵͳ�е���ɫ��A B G R ? 
            pSurfaceColor[x + y * dwOffsetSize] = RGB( 255,//  -- ��ɫ
                //255 * (x+1) / ddsd.dwWidth , 
                //255 * (y+1)/ddsd.dwHeight , 
                0,   //��ɫ
                0);  //��ɫ
        }
    }

    //DX_VERIFY(m_pDDSBack->GetDC(&hdc));
    //SetBkColor( hdc, RGB( 0, 0, 255 ) );
    //SetTextColor( hdc, RGB( 255, 255, 0 ) );
    //SYSTEMTIME st = {0};
    //GetSystemTime(&st);
    //TCHAR buf[20] = {0};
    //StringCchPrintf(buf,_countof(buf),TEXT("%02d:%02d:%02d"),st.wHour,st.wMinute,st.wSecond);
    //RECT rect;
    //GetClientRect(&rect);
    //DrawText(hdc,buf,-1,&rect,DT_CENTER|DT_VCENTER);
    ////�������沢�ͷž����
    //COM_VERIFY(m_pDDSBack->ReleaseDC(hdc));

    DX_VERIFY(m_pDDSBack->Unlock(NULL));

    //���������λλ�鴫�䵽��̨������
    //m_pDDSBack->BltFast( 0, 0, m_pDDSBackground, &rcRect, DDBLT_WAIT | DDBLT_KEYSRC );

    //������ͺ���潻��
    DX_VERIFY(m_pDDSPrimary->Flip(NULL,DDFLIP_WAIT));
#else
    //����ģʽ

#endif 

    CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnDestroy()
{
    KillTimer(m_TimerID);
    SAFE_RELEASE(m_pDDSBackground);
#ifdef FULLSCREEN_RUNNING
    SAFE_RELEASE(m_pDDSBack);
#endif 
    SAFE_RELEASE(m_pDDSPrimary);
    SAFE_RELEASE(m_pDDraw);

    CFrameWnd::OnDestroy();
}

void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (KEY_DOWN(VK_ESCAPE))
    {
        PostMessage(WM_CLOSE,0,0);
    }

    CFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}
