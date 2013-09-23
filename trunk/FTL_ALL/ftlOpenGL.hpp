#ifndef FTL_OPENGL_HPP
#define FTL_OPENGL_HPP
#pragma once

#ifdef USE_EXPORT
#  include "ftlOpenGL.h"
#endif

namespace FTL
{
    CFOpenGLErrorInfo::CFOpenGLErrorInfo(GLenum glErr) : CFConvertInfoT<CFOpenGLErrorInfo,GLenum>(glErr)
    {

    }
    LPCTSTR CFOpenGLErrorInfo::ConvertInfo()
    {
        if (NULL == m_bufInfo[0])
        {
            switch (m_Info)
            {
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), GL_INVALID_ENUM);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), GL_INVALID_VALUE);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), GL_INVALID_OPERATION);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), GL_STACK_OVERFLOW);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), GL_STACK_UNDERFLOW);
                HANDLE_CASE_TO_STRING(m_bufInfo, _countof(m_bufInfo), GL_OUT_OF_MEMORY);
            default:
                StringCchPrintf(m_bufInfo,_countof(m_bufInfo),
                    TEXT("Unknown Error:%d(0x%x)"), m_Info, m_Info);
                break;
            }
        }
        return m_bufInfo;
    }

    BOOL CFOpenGLApp::Initialize()
    {
        GLenum glErr = GL_NO_ERROR;
        OPENGL_VERIFY(glClearColor(0.0f, 1.0f, 0.0f, 0.5f));    //���������Ļʱ���õ���ɫ(��ɫ����)
        
        OPENGL_VERIFY(glClearDepth(1.0f));  //��Ȼ���--�����������Ļ�ڲ��ж�����и��٣��������������Ŀɼ���
        OPENGL_VERIFY(glEnable(GL_DEPTH_TEST)); //������Ȳ���
        OPENGL_VERIFY(glDepthFunc(GL_LEQUAL));  //������Ȳ��Ե�����

        OPENGL_VERIFY(glShadeModel(GL_SMOOTH)); //����smooth shading(��Ӱƽ��) -- ͨ������ξ�ϸ�Ļ��ɫ�ʣ������ⲿ�����ƽ��
        
        //����ϵͳ��͸�ӽ�����õ����� -- Ч���ã���Ӱ������
        OPENGL_VERIFY(glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST));
        //ResizeDraw(true); // �ı�OpenGL���ڴ�С��ֱ�ӵ�������ĺ���

        return TRUE;
    }

    template <class T>
    CFOpenGLWindowImpl<T>::CFOpenGLWindowImpl()
    {
        m_hRC = NULL;
        //m_hDC = NULL;
        m_bFullScreen = FALSE;
        m_iPixelFormat = 0;
    }

    template <class T>
    CFOpenGLWindowImpl<T>::~CFOpenGLWindowImpl()
    {

    }
    template <class T>
    LRESULT CFOpenGLWindowImpl<T>::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        FTLTRACE(TEXT("CFOpenGLWindowImpl<T>::OnCreate\n"));

        BOOL bRet = FALSE;
        T* pT = static_cast<T*>(this);
        CClientDC dc(pT->m_hWnd);

        //���ô���ʹ�õ����ظ�ʽ
        PIXELFORMATDESCRIPTOR pfd = {0};
        pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR); 
        pfd.nVersion   = 1;                 //�汾��
        pfd.dwFlags    = PFD_DOUBLEBUFFER   //����֧��˫����
            | PFD_SUPPORT_OPENGL            //��ʽ����֧��OpenGL 
            | PFD_DRAW_TO_WINDOW;           //��ʽ֧�ִ���
        pfd.iPixelType = PFD_TYPE_RGBA;     //���� RGBA ��ʽ
        pfd.cColorBits = 24;                //ѡ��ɫ�����
        //pfd.cRedBits ~ cAlphaShift -- �ȶ���0����ʾ����ɫ��λ����Alpha����
        pfd.cAccumBits = 0;                 //���ۼӻ���
        //pfd.cAccumRedBits ~ cAccumAlphaBits = 0; //���Ծۼ�λ
        pfd.cDepthBits = 32;  //16 ?         //Z-���� (��Ȼ���)
        pfd.cStencilBits = 0;               //���ɰ建��
        pfd.cAuxBuffers = 0;                //�޸�������
        pfd.iLayerType = PFD_MAIN_PLANE ;   //����ͼ��
        pfd.dwLayerMask = 0;                //���Բ�����
        pfd.dwVisibleMask = 0;
        pfd.dwDamageMask = 0;

        //�����ҵ���Ӧ�����ظ�ʽ
        m_iPixelFormat = dc.ChoosePixelFormat(&pfd);
        API_ASSERT(m_iPixelFormat != 0);

        API_VERIFY(dc.SetPixelFormat (m_iPixelFormat, &pfd));   //�������ظ�ʽ
        API_VERIFY((m_hRC = dc.wglCreateContext()) != NULL);    //ȡ����ɫ������
        API_VERIFY(dc.wglMakeCurrent(m_hRC));                   //������ɫ������

        API_VERIFY(pT->OnInit());

        API_VERIFY(dc.wglMakeCurrent(NULL));

        bHandled = FALSE;

        return 0;
    }

    template <class T>
    LRESULT CFOpenGLWindowImpl<T>::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        FTLTRACE(TEXT("CFOpenGLWindowImpl<T>::OnPaint\n"));

        BOOL bRet = FALSE;
        T* pT = static_cast<T*>(this);
        CPaintDC dc(pT->m_hWnd);
        API_VERIFY(dc.wglMakeCurrent(m_hRC));
        API_VERIFY(pT->OnRender());
        API_VERIFY(dc.SwapBuffers());
        API_VERIFY(dc.wglMakeCurrent(NULL));

        //BOOL bRet = FALSE;
        //PAINTSTRUCT ps = { 0 };
        //HDC hDC = BeginPaint(&ps);
        //if(hDC)
        //{
        //    CRect rcClient;
        //    API_VERIFY(GetClientRect(rcClient));
        //    COLORREF clrOld = ::SetBkColor(hDC, RGB(255, 0, 0));
        //    ATLASSERT(clrOld != CLR_INVALID);
        //    if(clrOld != CLR_INVALID)
        //    {
        //        ::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rcClient, NULL, 0, NULL);
        //        ::SetBkColor(hDC, clrOld);
        //    }
        //    EndPaint(&ps);
        //}
        //bHandled = FALSE;
        return 0;
    }
    template <class T>
    LRESULT CFOpenGLWindowImpl<T>::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        FTLTRACE(TEXT("CFOpenGLWindowImpl<T>::OnSize\n"));

        //ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));
        T* pT = static_cast<T*>(this);
        int cx = LOWORD(lParam);
        int cy = HIWORD(lParam);

        if(0 == cx || 0 == cy || NULL == pT->m_hWnd || NULL == m_hRC)
        {
            return 0;
        }
        BOOL bRet = FALSE;
        CClientDC dc(pT->m_hWnd);
        API_VERIFY(dc.wglMakeCurrent(m_hRC));
        API_VERIFY(pT->OnResize(cx, cy));
        API_VERIFY(dc.wglMakeCurrent(NULL));

        bHandled = FALSE;
        return 0;
    }

    template <class T>
    LRESULT CFOpenGLWindowImpl<T>::OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        return 1;
    }

    template <class T>
    LRESULT CFOpenGLWindowImpl<T>::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        BOOL bRet = FALSE;
        bHandled = FALSE;
        FTLTRACE(TEXT("CFOpenGLWndT<T>::OnDestroy\n"));

        if (m_hRC) 
        {
            API_VERIFY(wglDeleteContext(m_hRC));
            m_hRC = NULL;
        }

        //if (m_bFullScreen)
        //{
        //    //�л�������--ǿ��Windowsʹ��ע����е�ֵ(�ֱ��ʡ�ɫ�ˢ��Ƶ�ʵ�)���ָ�ԭʼ����
        //    ChangeDisplaySettings(NULL,0);		
        //    ShowCursor(TRUE);					//��ʾ���ָ��
        //}
        return 0;
    }

    template <class T>
    GLenum CFOpenGLWindowImpl<T>::ReSizeGLScene(GLsizei width, GLsizei height)
    {
        GLenum glErr = GL_NO_ERROR;

        if (0 == height)  // ��ֹ�����
        {
            height = 1;
        }
        OPENGL_VERIFY(glViewport(0, 0, width, height));             // ���õ�ǰ���ӿ�

        // Ϊ͸��ͼ������Ļ -- ԽԶ�Ķ���������ԽС
        OPENGL_VERIFY(glMatrixMode(GL_PROJECTION));						        // ѡ��ͶӰ����
        OPENGL_VERIFY(glLoadIdentity());							                // ����ͶӰ����

        //�˴�͸�Ӱ��ջ��ڴ��ڿ�Ⱥ͸߶ȵ�45���ӽ������㡣0.1f��100.0f�������ڳ��������ܻ�����ȵ������յ�
        OPENGL_VERIFY(gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f));   // �����ӿڵĴ�С
        OPENGL_VERIFY(glMatrixMode(GL_MODELVIEW));						            // ѡ��ģ�͹۲����
        OPENGL_VERIFY(glLoadIdentity());							                // ����ģ�͹۲����

        return glErr;
    }
    
    template <class T>
    BOOL CFOpenGLWindowImpl<T>::CreateGLWindow(char* title, INT width, INT height, INT bits, BOOL fullscreenflag)
    {
        BOOL bRet = FALSE;
        GLuint		PixelFormat; // �������ƥ��Ľ��

        return bRet;
    }

    template <class T>
    BOOL CFOpenGLWindowImpl<T>::SetFullScreenMode(BOOL bFullScreen, BOOL *pOldFullScreenMode)
    {
        BOOL bRet = FALSE;
        if (pOldFullScreenMode)
        {
            *pOldFullScreenMode = m_bFullScreen;
        }
        if (m_bFullScreen != bFullScreen)
        {
            DEVMODE dmScreenSettings = {0};						// �豸ģʽ
            dmScreenSettings.dmSize = sizeof(dmScreenSettings);			// Devmode �ṹ�Ĵ�С
            RECT WindowRect = {0};
            GetClientRect(&WindowRect);
            if (bFullScreen)
            {
                dmScreenSettings.dmPelsWidth = GetSystemMetrics(SM_CXSCREEN);				// ��ѡ��Ļ���
                dmScreenSettings.dmPelsHeight = GetSystemMetrics(SM_CYSCREEN);				// ��ѡ��Ļ�߶�
                dmScreenSettings.dmBitsPerPel = 32;					// ÿ������ѡ��ɫ�����
            }
            else
            {

            }
            dmScreenSettings.dmFields= DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
            API_VERIFY(DISP_CHANGE_SUCCESSFUL == ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN));
            if (bRet)
            {
                DWORD dwStyle = 0;
                DWORD dwExStyle = 0;
                if (bFullScreen)
                {
                    dwExStyle = WS_EX_APPWINDOW; //WS_EX_APPWINDOW����ǿ�����ǵĴ���ɼ�ʱ������ǰ��
                    dwStyle = WS_POPUP; //����û�б߿�ʹȫ��ģʽ����������ʾ
                    //ShowCursor(FALSE);						// �������ָ��
                }
                else
                {
                    dwExStyle = WS_EX_APPWINDOW|WS_EX_WINDOWEDGE; //��չ������
                    dwStyle = WS_OVERLAPPEDWINDOW; //�����������ɱ��С�ı߿򡢲˵������/��С����ť�Ĵ���
                }

                //���ݴ����Ĵ������͵�������
                API_VERIFY(AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle));		// �������ڴﵽ����Ҫ��Ĵ�С
                SetWindowPos(WindowRect, SWP_SHOWWINDOW);
            }

        }
        if (bRet)
        {
            m_bFullScreen = bFullScreen;
        }

        return bOldFullScreen;
    }

    template <class T>
    BOOL CFOpenGLWindowImpl<T>::OnInit()
    {
        return TRUE;
    }

    template <class T>
    BOOL CFOpenGLWindowImpl<T>::OnRender()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// �����Ļ����Ȼ���
        glLoadIdentity();							// ���õ�ǰ��ģ�͹۲����
        return TRUE;								//  һ�� OK
    }

    template <class T>
    BOOL CFOpenGLWindowImpl<T>::OnResize(int cx, int cy)
    {
        return TRUE;
    }
    template <class T>
    BOOL CFOpenGLWindowImpl<T>::OnFini()
    {
        return TRUE;
    }
}

#endif //FTL_OPENGL_HPP