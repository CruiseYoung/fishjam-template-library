#ifndef FTL_OPENGL_H
#define FTL_OPENGL_H

#pragma once

#ifndef FTL_BASE_H
#  error ftlOpenGL.h requires ftlbase.h to be included first
#endif

namespace FTL
{
	/*****************************************************************************************************
    * OpenGL�Ŀ���SDK����(VC6�汾��)��http://download.csdn.net/detail/jinxiu/274113
    * OpenGL(NeHe)����ѧϰ���ϣ�http://www.yakergong.net/nehe/
    *
    * ��������������WS_CLIPSIBLINGS �� WS_CLIPCHILDREN
    *****************************************************************************************************/

    /*****************************************************************************************************
    * glMatrixMode(GL_PROJECTION ��)
    *   Modelview Matrix(ģ�͹۲����) -- ��������ǵ�����ѶϢ
    *   Projection Matrix(ͶӰ����) -- ����Ϊ���ǵĳ�������͸��
    *****************************************************************************************************/

    /*****************************************************************************************************
    * OpenGL ��ƽ̨������2D/3D ͼ��API�����ɸ�����ʾ���̶���Ĺ�ͨ�����⣬�������ǻ�ͼ�Ĺ�ҵ��׼����Ϊ�侫ȷ�Ⱥ�
    *   ���ܶ��ϸߣ���ҪӦ����CAD/CAM ��ģ������������ģ�����Ϳ�ѧ��������
    * 
    * ����Ƕ��ʽϵͳ���ص���ѡ��OpenGL �е�һЩ����(Command)��״̬(State)��������һЩ����(Profile Extension)
    *   �õ���OpenGL ES(Open Graphics Library for Embedded System)��Ƕ��ʽϵͳ�������ƶ��ֳ��豸���ҵ硢�����ȣ�
    *   �����ڻ���2D/3D ͼ�ε�һ�����˰����ƽ̨��ͼ�ο⡣
    *   OpenGL ES �淶������OpenGL ES API ����������(Profile)��Common ����� Common-Lite ���档Common-Lite ����
    *   �е�API ��Ϊ��֧�ָ����ܸ�������Ļ�ͼϵͳ����Ƶģ��������������е�����(Command���൱�ں���)��Ҫ����
    *   ��CM Profile ֧�ָ������㣬CL Profile ֻ֧�ֶ������㣬
    *
    * OpenGL�Ŀ⺯������C���Է������OpenGL�������������¸�ʽ����������
    *   <��ǰ׺><������><��ѡ�Ĳ�������><��ѡ�Ĳ�������>
    *   ��ǰ׺��gl��glu��aux��glut��wgl��glx��agl �ȵȣ��ֱ��ʾ�ú�������OpenGL�Ǹ�������
    * OpenGL��������ص�API�к��Ŀ�(gl)��ʵ�ÿ�(glu)��������(aux)��ʵ�ù��߿�(glut)��
    *   ��Բ�ͬ����ϵͳ�Ĵ��ڿ�(glx��agl��wgl)����չ������ȡ����� ���Ŀ�(gl)��ʵ�ÿ�(glu)���������е�ƽ̨��ʹ�á�
    * 
    *   A.���Ŀ�(gl) -- ���ڳ���ġ����ĵ�ͼ�δ�����Ҫ��Ϊ��
    *     1.���ƻ�������ͼԪ�ĺ��� -- glBegin��glEnd��glNormal*��glVertex* �ȣ�
    *     2.������������α任��ͶӰ�任�ĺ��� 
    *         ���������glPushMatrix��glPopMatrix��glLoadMatrix��glMultMatrix��
    *         ���α任��glTranslate*��glRotate*��glScale*��
    *         ͶӰ�任��glOrtho��glFrustum��
    *         �ӿڱ任��glViewport
    *     3.��ɫ�����պͲ��ʵĺ���
    *         ������ɫģʽ��glColor*��glIndex*��
    *         ���ù���Ч����glLight*��glLightModel*
    *         ���ò���Ч����glMaterial��
    *     4.��ʾ�б��� -- ����(glNewList)������(glEndList)������(glGenLists)��ɾ��(glDeleteLists)�͵���(glCallList)��ʾ�б�
    *     5.����ӳ�亯��
    *         һ��ά����������glTexImage*
    *         �����������(glTexParameter*)����������glTexEnv*������������(glTetCoord*);
    *     6.����Ч������
    *         �ںϺ���glBlendFunc������������glHint����Ч��glFog*
    *     7.��դ�������ز�������
    *         ����λ��glRasterPos*�����Ϳ��glLineWidth������λ���ģʽglPolygonMode����ȡ����glReadPixel����������glCopyPixel
    *     8.ѡ���뷴������
    *         ��ȾģʽglRenderMode��ѡ�񻺳���glSelectBuffer������������glFeedbackBuffer
    *     9.����������Ļ��ƺ���
    *         �������߻�����ĺ��� glMap*��glMapGrid*��
    *         ��ֵ�� glEvalCoord* glEvalMesh*��
    *     10.״̬�������ѯ����
    *        glGet*()��glEnable()��glGetError()
    *  
    *   B.OpenGLʵ�ÿ�(The OpenGL Utility Library -- GLU),���нϸ��ӵĻ�ͼ������ӵ㡢�ߡ��濪ʼ��ͨ��glu��װһЩ��Ϊ���ӵĲ�����
    *     1.����������ͼ����
    *         gluScaleImage() ��gluBuild1Dmipmaps()��gluBuild2Dmipmaps()
    *     2.����ת����ͶӰ�任����
    *         ����ͶӰ��ʽ���� gluPerspective()��gluOrtho2D() ��gluLookAt()
    *         ʰȡͶӰ�Ӿ��庯�� gluPickMatrix()��
    *         ͶӰ������� gluProject() �� gluUnProject()
    *     3.�������Ƕ����
    *         gluNewTess()��gluDeleteTess()��gluTessCallback()��gluBeginPolygon()��gluTessVertex()��gluNextContour()��gluEndPolygon()��
    *     4.����������ƹ��ߣ���Ҫ�л������桢׶�桢���桢Բ���� 
    *         gluNewQuadric()��gluSphere()��gluCylinder()��gluDisk()��gluPartialDisk()��gluDeleteQuadric()
    *     5.�Ǿ�������B�������ƹ���
    *         ��Ҫ��������ͻ���Nurbs���ߺ����棬���� gluNewNurbsRenderer��gluNurbsCurve��gluBeginSurface��gluEndSurface��
    *         gluBeginCurve��gluNurbsProperty
    *     6.����������
    *         ��ȡ������Ϣ���ַ��� gluErrorString
    *   
    *   C.OpenGL������(aux)��ע�⣺Windowsʵ���кܴ����⣬�ܴ�̶����Ѿ��� glut ��ȡ��
    *     1.���ڳ�ʼ�����˳�������auxInitDisplayMode()��auxInitPosition()
    *     2.���ڴ����ʱ�����뺯����auxReshapeFunc()��auxKeyFunc()��auxMouseFunc()�� 
    *     3.��ɫ����װ�뺯����auxSetOneColor()�� 
    *     4.��ά������ƺ����������� ��״���ʵ���� ������ʽ��
    *         �磺������auxWireCube()�� auxSolidCube()��
    *     5.�������̹�������auxIdleFunc()��
    *     6.�������к�����auxMainLoop()��
    * 
    *   D.OpenGL���߿�(glut) OpenGL Utility Toolkit -- ������X-Window, Windows NT, OS/2��ϵͳ������
    *   �����������ڴ���ƽ̨��OpenGL���߰���Ŀ�������ز�ͬ����ƽ̨API�ĸ��Ӷȡ� ��Ϊaux�⹦�ܸ�ǿ�����Ʒ���ṩ��Ϊ���ӵĻ��ƹ��ܣ�
    *     1.���ڲ������������ڳ�ʼ�������ڴ�С������λ�ú�����[glutInit()��glutInitDisplayMode()��glutInitWindowSize()��glutInitWindowPosition()]��
    *     2.�ص���������Ӧˢ����Ϣ��������Ϣ�������Ϣ����ʱ������[GlutDisplayFunc()��glutPostRedisplay()��glutReshapeFunc()��glutTimerFunc()��glutKeyboardFunc()��glutMouseFunc()]��
    *     3.�������ӵ���ά���壺��Щ��aux��ĺ���������ͬ��
    *     4.�˵�������������Ӳ˵��ĺ���GlutCreateMenu()��glutSetMenu()��glutAddMenuEntry()��glutAddSubMenu()��glutAttachMenu()��
    *     5.�������к�����glutMainLoop()��
    *  
    *   E.Windowsר�ÿ�(wgl) -- 
    *     ��������OpenGL��Windows �����ֲ�OpenGL���ı�����Ĳ���
    *     1.��ͼ��������غ�����wglCreateContext��wglDeleteContext��wglGetCurrentContent��wglGetCurrentDC��wglDeleteContent
    *     2.���ֺ��ı���������wglUseFontBitmaps��wglUseFontOutlines
    *     3.���ǲ㡢�ز����ƽ��㴦������wglCopyContext��wglCreateLayerPlane��wglDescribeLayerPlane��wglReakizeLayerPlatte
    *     4.����������wglShareLists()��wglGetProcAddress()��
    *   
    *   F.Win32 API������ -- ��������ר��ǰ׺���� Win32 ��չ��������Ҫ���ڴ������ش洢��ʽ��˫֡���档
    *   
    *   G.X����ר�ÿ�(glX) -- �����Unix��Linux����չ����
    *     1.��ʼ����glXQueryExtension
    *     2.��Ⱦ�����ĺ�����glXCreateContext��glXDestroyContext��glXCopyContext��glXMakeCurrent��glXCreateGLXPixmap
    *     3.ִ�У�glXWaitGL��glXWaitX
    *     4.�����������壺glXSwapBuffers��glXUseXFont
    *
    *   H.������չ�� -- �������µ�OpenGL��������û���ڱ�׼OpenGL����ʵ�֣�������չ�Ѵ��ڵ�OpenGL�����Ĺ��ܡ�
    *     ͨ����Ӳ������Ϊʵ��Ӳ����������OpenGL����չ���ƿ����ĺ�����OpenGL��չ(OpenGL Extention)�����˴�������չAPI������
    *     ���� glext.h��glxext.h��wglext.h ��ͷ�ļ���
    *****************************************************************************************************/

    #define OPENGL_VERIFY(x) \
        {\
            (x);\
            if(GL_NO_ERROR != (glErr = glGetError()))\
            {\
                REPORT_ERROR_INFO(FTL::CFOpenGLErrorInfo, glErr, x);\
            }\
        }

    FTLEXPORT class CFOpenGLErrorInfo : public CFConvertInfoT<CFOpenGLErrorInfo, GLenum>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFOpenGLErrorInfo);
    public:
        FTLINLINE explicit CFOpenGLErrorInfo(GLenum glErr);
        FTLINLINE virtual LPCTSTR ConvertInfo();
    };


    FTLEXPORT class CFOpenGLApp
    {
    public:
        FTLINLINE BOOL Initialize();
    };

    template <class T>
    FTLEXPORT class /*ATL_NO_VTABLE*/ CFOpenGLWndT : public ATL::CWindowImpl<T, CWindow, CFrameWinTraits>
    {
    public:
        DECLARE_WND_CLASS_EX(TEXT("FTLOpenGLWindow"), CS_VREDRAW | CS_HREDRAW | CS_OWNDC, -1)

        BEGIN_MSG_MAP(CFOpenGLWndT)
            MESSAGE_HANDLER(WM_CREATE, OnCreate)
            MESSAGE_HANDLER(WM_PAINT, OnPaint)
            MESSAGE_HANDLER(WM_SIZE, OnSize)
            MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        END_MSG_MAP()

    public:
        CFOpenGLWndT();
        ~CFOpenGLWndT();
        BOOL CreateGLWindow(char* title, INT width, INT height, INT bits, BOOL fullscreenflag);
        BOOL SetFullScreenMode(BOOL bFullScreen, BOOL *pOldFullScreenMode);
        BOOL DrawGLScene(GLvoid);
        GLenum ReSizeGLScene(GLsizei width, GLsizei height);

    protected:
        HGLRC   m_hRC;          //������ɫ������(Rendering Context)����������е�OpenGL�����������ӵ�Device Context(�豸������)��
        HDC     m_hDC;    //OpenGL��Ⱦ�豸��������
        BOOL    m_bFullScreen;  //ȫ����־
        int     m_iPixelFormat;

        BOOL    OnInitScene();
        BOOL    OnFiniScene();
    private:
        LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    };
}

#endif //FTL_OPENGL_H

#ifndef USE_EXPORT
#  include "ftlOpenGL.hpp"
#endif