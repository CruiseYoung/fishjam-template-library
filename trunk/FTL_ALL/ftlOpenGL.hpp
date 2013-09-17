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
        OPENGL_VERIFY(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));    //���������Ļʱ���õ���ɫ(��ɫ����)
        
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
    CFOpenGLWndT<T>::CFOpenGLWndT()
    {
        m_hRC = NULL;
        m_hDC = NULL;
        m_bFullScreen = FALSE;
        m_iPixelFormat = 0;
    }

    template <class T>
    CFOpenGLWndT<T>::~CFOpenGLWndT()
    {

    }
    template <class T>
    LRESULT CFOpenGLWndT<T>::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        BOOL bRet = FALSE;
        FTLTRACE(TEXT("CFOpenGLWndT<T>::OnCreate\n"));
        //bHandled = FALSE;
        CREATESTRUCT* pCreateStruct = (CREATESTRUCT*)lParam;
        if (pCreateStruct)
        {
            //pCreateStruct->style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
        }
        
        int bits = 32;

        //���ظ�ʽ
        static	PIXELFORMATDESCRIPTOR pfd=					// /pfd ���ߴ���������ϣ���Ķ�����������ʹ�õ����ظ�ʽ
        {
            sizeof(PIXELFORMATDESCRIPTOR),					// ������ʽ�������Ĵ�С
            1,								// �汾��
            PFD_DRAW_TO_WINDOW |						// ��ʽ֧�ִ���
            PFD_SUPPORT_OPENGL |						// ��ʽ����֧��OpenGL
            PFD_DOUBLEBUFFER,						// ����֧��˫����
            PFD_TYPE_RGBA,							// ���� RGBA ��ʽ
            bits,								// ѡ��ɫ�����
            0, 0, 0, 0, 0, 0,						// ���Ե�ɫ��λ
            0,								// ��Alpha����
            0,								// ����Shift Bit
            0,								// ���ۼӻ���
            0, 0, 0, 0,							// ���Ծۼ�λ
            16,								// 16λ Z-���� (��Ȼ���)
            0,								// ���ɰ建��
            0,								// �޸�������
            PFD_MAIN_PLANE,							// ����ͼ��
            0,								// Reserved
            0, 0, 0								// ���Բ�����
        };

        m_hDC = GetDC();
        if (m_hDC)
        {
            //�����ҵ���Ӧ�����ظ�ʽ
            API_VERIFY((m_iPixelFormat = ChoosePixelFormat(m_hDC, &pfd))!= NULL);
            API_VERIFY(SetPixelFormat(m_hDC, m_iPixelFormat, &pfd));    //�������ظ�ʽ
            API_VERIFY((m_hRC = wglCreateContext(m_hDC)) != NULL);    //ȡ����ɫ������
            API_VERIFY(wglMakeCurrent(m_hDC, m_hRC));   //������ɫ������

            ReSizeGLScene(pCreateStruct->cx, pCreateStruct->cy);
        }
        return 0;
    }

    template <class T>
    BOOL CFOpenGLWndT<T>::DrawGLScene(GLvoid)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// �����Ļ����Ȼ���
        glLoadIdentity();							// ���õ�ǰ��ģ�͹۲����
        return TRUE;								//  һ�� OK
    }
    template <class T>
    LRESULT CFOpenGLWndT<T>::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        BOOL bRet = FALSE;
        PAINTSTRUCT ps = { 0 };
        HDC hDC = BeginPaint(&ps);
        if(hDC)
        {
            CRect rcClient;
            API_VERIFY(GetClientRect(rcClient));

            COLORREF clrOld = ::SetBkColor(hDC, RGB(255, 0, 0));
            ATLASSERT(clrOld != CLR_INVALID);
            if(clrOld != CLR_INVALID)
            {
                ::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rcClient, NULL, 0, NULL);
                ::SetBkColor(hDC, clrOld);
            }
            EndPaint(&ps);
        }
        return 0;
    }
    template <class T>
    LRESULT CFOpenGLWndT<T>::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));
        return 0;
    }
    template <class T>
    LRESULT CFOpenGLWndT<T>::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        BOOL bRet = FALSE;
        bHandled = FALSE;
        FTLTRACE(TEXT("CFOpenGLWndT<T>::OnDestroy\n"));

        if (m_bFullScreen)
        {
            //�л�������--ǿ��Windowsʹ��ע����е�ֵ(�ֱ��ʡ�ɫ�ˢ��Ƶ�ʵ�)���ָ�ԭʼ����
            ChangeDisplaySettings(NULL,0);		
            ShowCursor(TRUE);					//��ʾ���ָ��
        }
        if (m_hRC)  //�ͷ���Ⱦ������
        {
            API_VERIFY(wglMakeCurrent(m_hDC, m_hRC));
            API_VERIFY(wglDeleteContext(m_hRC));
            m_hRC = NULL;
        }
        if (m_hDC)
        {
            ReleaseDC(m_hDC);
            m_hDC = NULL;
        }
        return 0;
    }

    template <class T>
    GLenum CFOpenGLWndT<T>::ReSizeGLScene(GLsizei width, GLsizei height)
    {
        GLenum glErr = GL_NO_ERROR;

        if (height==0)  // ��ֹ�����
        {
            height=1;
        }
        OPENGL_VERIFY(glViewport(0, 0, width, height));             // ���õ�ǰ���ӿ�

        // Ϊ͸��ͼ������Ļ -- ԽԶ�Ķ���������ԽС
        glMatrixMode(GL_PROJECTION);						        // ѡ��ͶӰ����
        glLoadIdentity();							                // ����ͶӰ����

        //�˴�͸�Ӱ��ջ��ڴ��ڿ�Ⱥ͸߶ȵ�45���ӽ������㡣0.1f��100.0f�������ڳ��������ܻ�����ȵ������յ�
        gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);   // �����ӿڵĴ�С
        glMatrixMode(GL_MODELVIEW);						            // ѡ��ģ�͹۲����
        glLoadIdentity();							                // ����ģ�͹۲����

        return glErr;
    }
    
    template <class T>
    BOOL CFOpenGLWndT<T>::CreateGLWindow(char* title, INT width, INT height, INT bits, BOOL fullscreenflag)
    {
        BOOL bRet = FALSE;
        GLuint		PixelFormat; // �������ƥ��Ľ��

        return bRet;
    }

    template <class T>
    BOOL CFOpenGLWndT<T>::SetFullScreenMode(BOOL bFullScreen, BOOL *pOldFullScreenMode)
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
    BOOL CFOpenGLWndT<T>::OnInitScene()
    {
        return TRUE;
    }

    template <class T>
    BOOL CFOpenGLWndT<T>::OnFiniScene()
    {
        return TRUE;
    }
}

#endif //FTL_OPENGL_HPP