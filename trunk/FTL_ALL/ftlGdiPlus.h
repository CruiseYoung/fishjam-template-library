
#ifndef FTL_GDIPLUS_H
#define FTL_GDIPLUS_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlGdiPlus.h requires ftlbase.h to be included first
#endif

#include <GdiPlus.h>

/*****************************************************************************************************
* ʹ��GDI+ǰ��Ҫ GdiplusStartup ��ʼ���� ʹ����Ϻ�ͨ�� GdiplusShutdown ��ֹ��
* Gdi+ �� 1.0 �� 1.1 �����汾(�����Ƿ��и��µ�?)
*
* GDI��GDI+������
*   ��GDI����״̬�ģ�GDI+����״̬�ģ� DC <==> Graphics ����
*   ��GDI����״�������ƺ����ʹ��ͬһ������������Rectangle(�����û��ʣ�����û�ˢ)��
*     ��GDI+�Ƿֿ��ģ��� DrawRectangle �� FillRectangle
*     GDI�����������ͣ���ת��ʱ�ᶪʧ���ȣ�GDI+�����������Բ���REAL(float)����������ŵȴ����в��ᶪʧ����
/*****************************************************************************************************/

/*****************************************************************************************************
* GDI+ ������ϵͳ -- GDI+ ʹ����������ռ�
*    ȫ�� -- DrawLine �ȷ�������������ֵλ��ȫ������ռ���
*    ҳ�� -- ԭ�����ڹ����������Ͻ�
*    �豸 -- ��ʾ�����ӡ�� ��?
*  ����ͨ������ϵ�в�����ʾ����Ļ��
*    ȫ������ =(ȫ�ֱ���,Graphics::XXXTransform)=> 
*       ҳ������ =(ҳ�����,PageUnit+PageScale)=> 
*         �豸����
*    PageUnit -- ָ���������������������λ
*    ȫ�ֱ���Ӧ���ڸ����� Graphics ������Ƶ�ÿ����Ŀ�ı��Σ�ͨ��������������ϵͳ��
*      GDI+�ṩ�� MultiplyTransform��RotateTransform��ScaleTransform �� TranslateTransform ��
*    ���ƶ����Ͽ������ֲ�����(�����Transform) -- ͨ��������ת��������������ϵͳ�ϻ��ƵĶ���
*    
*  ����Ҳ����ͨ��������ʽ���б�� -- GDI+��ͨ��Matrix������ξ���, ���β��� MatrixOrderAppend ?
*    ��ת(Rotate/RotateAt) -- ��λΪ��
*    ����(Multiply/Scale)��[x,y] X [�任����(2x2)]
*    ƽ��(Shear/Translate)�� [x,y] + [ƫ������(1x2)]
*    ������� -- ʹ��3x3�ľ���(�������һάʼ��Ϊ1)������ת,����,ƽ��һ�����(�ƺ�����ֵ�����岻��?)��
*                [ X ���� , X ͶӰ?, 0 ]
*      [x,y,1] X [ Y ͶӰ?, Y ���� , 0 ]
*                [ X ƽ�� , Y ƽ�� , 1 ]
*      ���ξ����У� [0,0]��[2x2]��Ϊ���Բ��֣� [3,1]��[3,2]��Ϊƽ�Ʋ���
*      ��Ϊ���һ�е�ֵ�̶���Matrix������ֻ��Ҫָ��ǰ���е�6��ֵ
*    ���ϱ��� -- �����������(����ת�����š�ƽ��)������˵õ���ע������˳���Ӱ����
*      
*  Ĭ������ϵͳ(GDI �� MM_TEXT) -- ԭ���������Ͻǣ����� x ��ָ���ұߣ�y ��ָ���±ߣ���λΪ����
*  ���� -- GDI+�ṩ��ȫ�ֱ��κ�ҳ����Σ��Ա�������ʹ���Ƶ���Ŀ���Σ���ת�����š�ƽ�ƣ��ȵȣ� 
*
* GDI+��GDI�ĺܶ����ܽ������Ż� -- ����˵gdi+ֱ��ʹ��CPU�������Ӳ���Ż����翪������ݵ�����»��Ƽ�����Ҳ��
*   ����Ч�����⡣�����Ҫ���־�̬����̬�����Լ����ػ�ʱ�Ĺ�������
*   Gdi+�е� DrawImage �����ܼ�����⣬���Ӧ GDI �е�BitBlt�ǿ��ٸ�Ч���ڴ濽��������
*     �������Ϊ��GDI+�Ļ�ͼ�������Ƶ��ڴ�DC�У�Ȼ��ʹ��λ�����ķ�ʽ���л�����ʾ��
*       WGF(windows graphic foundation)--����ʵ�ֵ�һ�� GDI/GDI+ ��˫�����ͼ����
*       http://www.pudn.com/downloads219/sourcecode/windows/bitmap/detail1030808.html
* GDI�п�Ӧ���������Ψһ������ƽ�ƣ���GDI+ ��ȫ�������д洢�����������������κοɴ洢�ڱ��ξ����еı��Σ��������ţ�?
* ���������� ? PointF 
* 
* ��������
*   2Dʸ��ͼ(2-D vector graphics)
*   ͼ��(Imaging)
*   ӡˢ��ʽ(Typography) -- ��ʽ��ϵ��ʹ�ø������塢�ֺź���ʽ����ʾ�ı�
* 
* �����:
*   Graphics(����)--���ڹ��캯����ͨ��HDC��̬��FromImage�ȳ�ʼ�����������ڻ���ֱ�ߡ����Ρ�·����ͼ�Ρ�ͼ����ı��ķ���
*     DrawPath -- ����·������(����һ���Ի��Ƴ�����·���������а�����ֱ�ߡ����Ρ�����κ����ߵ�)
*     DrawImage -- ����ͼ�񣬵������ƺ��ܲ�(�����GDI����?)
*     ��������
*       CompositingMode -- 
*       CompositingQuality --
*       InterpolationMode -- 
*       PixelOffsetMode -- 
*       SmoothingMode -- ����ƽ��ģʽ(�Ƿ�ʹ��������ݵļ���)���� AntiAlias �����
*       TextRenderingHint -- �ữ�ı��ľ�ݱ�Ե���� SystemDefault(ʹ���û��Ѿ����õ��κ�����ƽ������), SingleBitPerPixelGridFit(�ٶ���쵫�������),
*         AntiAliasGridFit(���õ��������ϵ͵��ٶ�), ClearTypeGridFit(��LCD��ʾ���Ͼ�����õ�����), 
*     ͼ������(BeginContainer/EndContainer) -- �� Graphics �����������κ�״̬���Ķ���������,
*       �����дGraphics���������״̬(�� �������򡢱��κ��������� ��)
*
*   Brush
*     SolidBrush(�̶�ɫ)/HatchBrush(��Ӱ)/TextureBrush(����ˢ--λͼ)/GradientBrush(����,������Linear��·��Path)
*   CachedBitmap -- ͨ��������Ƶ�ͼ�����Ż���ʾ��ע���ж� DrawCachedBitmap �����ķ���ֵ�������ΪOK˵����ʾ�����б仯�����磿����
*     ��Ҫ���´�������λͼ��
*   GraphicsPath -- ·�����󣬿ɴ�����������Ŀ���У�·������Ϊ�ɸ��ֻ�ͼ���������Ĺ켣
*     AddLine/AddEllipse/AddBezier/AddPath/AddString--��·��������·������
*     Flatten -- ��ƽ·�����ɽ�·���еĸ�������������ת��Ϊһ����ֱ��
*   Image -- �������
*     >DrawImage -- ���е� UnitPixel �Ȳ�������?
*     Bitmap -- ���ء�����ʹ����դͼ���� BMP��JPEG��PNG��ICON ��
*       Clone -- ������������ָ��ԭ���β�����ָ��Ҫ���Ƶ�ԭʼλͼ�Ĳ��֣����صĽ����Ҫdelete?
*       BitmapData��
*     Metafile -- ��¼����ʾ�ͱ���ͼԪ�ļ�(ͼ����������У�ʸ��ͼ��)���� EMF
*       MetafileHeader��MetaHeader -- ���ͼԪ�ļ�ͷ�д洢������
*       ע�⣺GDI+֧��EMF/EMF+������֧��WMF
*
*     GetRawFormat -- ��ø�ʽ��GUID
*   Matrix -- �洢������ Graphics ����ı��ξ����������ƽ�ơ���ת�����źͷ�ת�ȡ�ͨ�������ζ����Transform���á�
*   Pen
*     SetDashStyle--ָ��ֱ�ߵ�����
*     StartCap �� EndCap -- ����ָ���������˵����
*     LineJoin -- ���Կ�����ָ�����ӵ����໥����б�ӵģ�����ʱ�γ���ǣ���б�еġ�Բ�εĻ��ǽضϵ�
*   Region -- ������
*       1.����(���������Ƶ���ʾ�����һ���ض�����) -- ���� Graphics.Clip ����
*       2.���м��(ȷ��������갴ťʱ����Ƿ�����Ļ���ض�����)
*     Complement -- ? ���ڸ���������е�ļ���
*     Exclude -- �����������ڵ�һ�������ж����ڵڶ��������е����е�
*     Intersect -- ����,ͬʱ����������������е�ļ���
*     Union -- ������������������������һ�������е�ļ���
*     Xor -- ���ֻ����һ���������һ�����򵫲�ͬʱ����������������е㡣
*   StringFormat -- 
*   LineJoin -- ָ����������������������ʽ
* 
* ImageEncoder -- GetImageEncoders,�ɻ�ö�Ӧ��CLSID
* Gdi+�Ĵ����룬 Gdiplus::OK 
*
* ���GDI���¹���
*   Alpha Blending(���) -- ����ָ�������ɫ��͸���ȡ�͸��ɫ�뱳��ɫ���� ���ɫԽ͸��������ɫ��͸���̶Ⱦ�Խ�ߡ�
*   �������������(��¥��״����ʾֱ�ߵļ���) -- ����ʹ�ı��� LCD ��Ļ�ϳ���ʱ�ԵñȽ�ƽ��
*     ������� -- ͨ����·���ϵ�������Ϊ��ɫ��ǰ��ɫ�ͱ���ɫ�Ļ��ɫ��ȡ���ں�ֱ�ߵĽӽ��̶ȣ������������Ӿ��ϸ���ƽ����ֱ��
*   ���仭ˢ(GradientBrush) -- ���ͼ�Ρ�·������������Խ��仭�ʺ�·�����仭��
*     LinearGradientBrush -- ���Խ��仭��
*     PathGradientBrush -- ·�����仭��
*   ��������(Cardinal Splines,ľ�����ߵķ���) -- ����������һ�������������ߣ���Щ�������������γ�һ���ϴ�����ߡ�
*     ��������ƽ����(û�����)ͨ�������е�ÿһ���㣬��ͨ������ֱ�ߴ�����·������׼��
*     DrawCurve/DrawClosedCurve�������ɵ���������������(tension)ָ����tension ֵ:
*       0 -- ��ʾ��Ӧ�����޵���������(ֱ��)
*       1 -- û����������,����������С��ȫ������·��
*       1<tension -- ����ѹ���ĵ��ɣ�����ѹ�Ų��ýϳ���·����ֵԽ��·��Խ��
*   ����������(DrawBezier) -- ���ĸ���ָ�������ߣ������յ�(p1��p2)���������Ƶ�(c1��c2)��
*     ���Ƶ������һ������ĳЩ��������ק���߲�Ӱ�����������ķ�ʽ�������߲��������Ƶ㡣
* 
* ����
*   GDI+ �е� Region ���ṩһ������ Rectangle ����Ĺ��캯������һ������ GraphicsPath ����Ĺ��캯����
*   ���봴���Ǿ���(����Բ)������Ҫ����һ��������Ӧ��״��GraphicsPath����Ȼ���䴫���� Region ���캯����
*****************************************************************************************************/

namespace FTL
{

    # define GDIPLUS_VERIFY(x)   \
    sts = (x);\
    if(Gdiplus::Ok != sts)\
    {\
        REPORT_ERROR_INFO(FTL::CFGdiPlusErrorInfo, sts, x);\
    }


    FTLEXPORT class CFGdiPlusErrorInfo : public CFConvertInfoT<CFGdiPlusErrorInfo, Gdiplus::Status>
    {
        DISABLE_COPY_AND_ASSIGNMENT(CFGdiPlusErrorInfo);
    public:
        FTLINLINE explicit CFGdiPlusErrorInfo(Gdiplus::Status status);
        FTLINLINE virtual LPCTSTR ConvertInfo();
    };

    class CFAutoGdiplus
	{
	public:
		FTLINLINE CFAutoGdiplus();
		FTLINLINE ~CFAutoGdiplus();
	private:
		Gdiplus::GdiplusStartupInput	m_gdiplusStartupInput;
		ULONG_PTR						m_gdiplusToken;
	};

	class CFGdiPlusUtil
	{
	public:
		//ͨ���Ź���ͼƬ������Ч�����л��� -- TODO:��֤��ʵ��GDI�汾
		//strFileName.Format(_T("Chat_other_bubble_%d.png"), i);
		//m_pImgOtherBubble[i]  = new Image(GetImagePath(strFileName));

		FTLINLINE static BOOL DrawNineBlockImage(Gdiplus::Graphics* pGraphics, Gdiplus::Image* pImages[9], const Gdiplus::Rect* pRect);
	};
}

#endif //FTL_GDIPLUS_H

#ifndef USE_EXPORT
#  include "ftlGdiPlus.hpp"
#endif