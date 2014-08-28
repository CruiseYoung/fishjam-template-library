#ifndef FTL_MULTI_MEDIA_H
#define FTL_MULTI_MEDIA_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlMultiMedia.h requires ftlbase.h to be included first
#endif

//ý���ʽ��鹤��
//  mediaInfo -- http://mediainfo.sourceforge.net/
//  KMP �Ȳ������Դ�
//  Fresh View ?

/*********************************************************************************************
* Զ�̼�غ���Ļ¼��ʱ��ֻ������Ļ�仯�Ĳ��֣�
*   1.�������� Mirror Driver(Tight VNC) -> ����ģʽgdi��
*     ��������������һ�ֱ�׼������Ϊ΢��˾���Ƽ���ֱ�Ӵ�������DIBλͼ�����棩�н���ȡ�����ͼ�����ݿ�����Ӧ�ó��������Ľ�����һ�ν�����ʱ�����ġ�
*     ȱ��:��Ҫ��װ��������ÿ�ν���ʱ����Ļ��������Ե�ˢ��
*   2.GDI+�µ��㷨��ʽ
*     a.�ֿ��������(�Ҹ�����Ļ����) -- ǰ�󱣴�����bmpλͼ������Ļ�ֳ����ɿ鲢��ţ�ǰ������λͼ�ֱ𰴶�Ӧ��ſ�������ص������(XOR)������
*       ������Ľ��ȫ����,֤���������ֿ��λͼ��ȣ���Ϊ�����������ֿ��λͼ����ȣ�
*       �����������Ľ������ѹ���������͡�
*     b.�̶������ɨ��(RadminӰ��Զ�̿���ϵͳ)--ǰ�󱣴�����bmpλͼ������Ļ�ֳ����ɿ鲢��ţ�ǰ������λͼ�ֱ𰴶�Ӧ��ſ�Աȣ�
*       ��������(һ����10��)�Ա�ǰ������ͼ���һ���е����ص��Ƿ���ͬ,����ͬ��ѹ�����͵�ǰ���е�ͼ��
*       Ϊ����ÿ��ɨ��̶����У�ÿ������ɨ��ʱ����ϴ�ɨ����б��+n��(��Ҷ������)
*     c.��̬�ֿ����ɨ��(Radmin���Ż�)--����Ļ�ı仯����̬ȷ��Ҫ���͵ľ���
*     d.�ȵ�׷��(��̬�ֿ����ɨ�� + �������Ĳ���)
*   3.RDP����Э��(Զ������)
* ѹ����ѹ��(TSPľ��ѡ�õ�LZW) -- JPEG��Huffman��RLE(Run Length Encode)��LZW��
*********************************************************************************************/

/*********************************************************************************************
* Ϊ��ʹ��OEM��Դ(�� LoadOEMIcon��)����Ҫ�� stdafx.h �Ŀ�ʼ�� #define OEMRESOURCE
* 
* BMP(Bitmap) -- λͼ(��դͼ��),BMP �ļ�ͨ����ѹ������˲�̫�ʺ�ͨ�� Internet ���䡣
* Exif(Exchangeable Image File) -- �ɽ���ͼ���ļ�����������������������Ƭ���ļ���ʽ��
*   ���и��� JPEG ���ѹ����ͼ�� �� �������ڡ������ٶȡ��ع�ʱ�䡢����ͺ� ����Ƭ���������Ϣ��
* Gif(Graphics Interchange Format) -- ��Ȩ���У�ʹ��ʱ��Ҫ��Ȩ(����Xx�����?)�������� Web ҳ����ʾͼ���ͨ�ø�ʽ,����������ɫ֮���������߽��ͼƬ��
*   ��������ѹ������ָ��һ����ɫΪ͸��ɫ(ͼ�񽫾�����ʾ�����κ� Web ҳ�ı���ɫ)��
*   �ڵ����ļ��д洢һϵ�� GIF ͼ������γ�һ������ GIF��������ɫ�б�ͼ�������Ƕ�Ӧ����ɫ�б������ֵ��ÿ��������ܴ洢8λ(256ɫ)��
*   �ڲ��ֳ����洢�飬�����洢���ͼ����Ǿ���ͼ�������Ϊ�Ŀ��ƿ顣ͨ�� LZW ѹ���㷨ѹ��ͼ�����ݡ�
* ICON() -- ͼ���Ǵ�С�̶���С����ͼ����ÿһ��ͼ����Դ�пɺ�Ϊ������ʾ�豸����;׼���ľ��в�ͬ��С��λ���Ķ��ͼ��
*   ͼ����һ��ͼƬ��һ�����ڲ���͸��������Ĥ��ɣ���׼��ͼ��Ϊ32��32���ص�256ɫͼ����չͼ��Ϊ 256��256���ص�32λɫ��
* JPEG(Joint Photographic Experts Group) -- ����ͼ��ר���飬���������Բ����(��Ƶ?)��Ϣ��ѹ���ж�����
*   �ɿ���ѹ�����𣬲�֧��͸���򶯻���JPEG ѹ��������������ͼ�Ρ���ɫ��������߽�(�߽紦��ģ��)��
*   JPEG ��һ��ѹ������������һ���ļ���ʽ��JPEG �ļ�������ʽ (JFIF)��һ���ļ���ʽ�������ڴ洢�ʹ����Ѹ���JPEG����ѹ����ͼ��
* PNG(Portable Network Graphics) -- ����ֲ����ͼ�Ρ���GIF��չ����������ѹ�����ɴ洢24λ��ʣ�
*   ֧��Alphaͨ�����ܹ�����ʾһ��ͼ��(������ʾʱԽ��Խ����)���ɰ���٤��У������ɫУ����Ϣ���Ա�ͼ����ڸ��ָ�������ʾ�豸�Ͼ�ȷ�س��֡�
* TIFF(Tagged Image File Format) -- ��ǩͼ���ļ���ʽ����һ�����ĺͿ���չ�ĸ�ʽ������ÿ��������λ���洢ͼ��
*   ������ʹ�ø��ָ�����ѹ���㷨�������Ķ�ҳ TIFF �ļ����Դ洢����ͼ�񣬿ɴ洢 �����̡�������ѹ������ ��Ԫ���ݡ�
*   ���԰���ͼ����ص���Ϣ(�������̡���ӡ�����)�洢���ļ��в�ʹ�ñ�ǩ��������Щ��Ϣ�����Ը�����Ҫͨ����׼������±�ǩ����չ TIFF ��ʽ��
*
* MetaFile -- ʸ��ͼ��,�洢Ϊһϵ�л�ͼ��������õ�ͼ������������ÿ��Դ洢���ڴ��л򱣴浽�ļ�������
*   WMF(Windows MetaFile)--
*   EMF(Enhanced MetaFile) -- ��ǿ��Ԫ�ļ�
*   EMF+ -- EMF ����չ���ɴ洢GDI+��¼
*********************************************************************************************/

/*********************************************************************************************
* λͼ--����ͼ���դͼ,�������������
*   �豸���λͼ(DDB--Device Dependent Bitmap) -- ͼ�����ʾ(CBitmap),������Ӳ���ĵ�ɫ��,ʹ��:LoadBitmap
*   �豸�޹�λͼ(DIB--Device Independent Bitmap) -- ͼ��Ĵ洢(CImage),�Լ�������ɫ��Ϣ����ɫģʽ���豸�޹ء�
*     ��һ���ļ���ʽ��ʹ��:CImage �� LoadImage+FromHandle�� 256�����µ�DIB���Լ�����ɫ�����ص���ɫ������ϵͳ��ɫ�塣
*     DIB��������ͼ����Ϣ��Ҫ�У���ɫ��ʽ���ֱ��ʡ���ɫ���ѹ����־.
*     ��Ӧ�Ľṹ -- BITMAPFILEHEADER��BITMAPINFO��BITMAPINFOHEADER
*     DIB���������SetDIBitsToDevice/StetchDIBits �ȣ�GDI�������DIBת����DDB ��
*   ��ɫ���(BPP -- bits-per-pixel) -- λͼ��ÿ�����������ɶ�����λ��ʾ��������λ�ĸ�����Ϊλͼ����ɫ��ȣ�
*     ��������ɫ���Ϊ 1,2,4,8,15,16,24(ȫ��),32(��Alphaͨ��)��
*   ���ڻ����8λ��256ɫ����λͼ����Ϊ����ɫ�������ɫ���е���ɫ���������ֵ,��Ҫ��ɫ�塣
* ͼ�� -- ��С�̶��ķ���Сλͼ����һ�������ͼ����Դ�У�һ����������ͬ��С��λ����ͼ��
* 
* GetBitmapBits
* BITMAP �ṹ�У�
*   bmWidth -- ���,���ظ���(�豸����?), ����>0
*   bmHeight -- �߶�,���ظ���(�豸����?), ����>0
*   bmWidthBytes -- ÿ��ɨ����(��ÿ������)���ֽ���,�����ܱ�2(4?)��������ÿ���ֶ��룬�����λ��0��
*   bmPlanes -- λ����, = ��ɫ���?
*   bmBitsPixel -- ��ɫ���
*   bmBits -- ָ����λͼ���ݵ��ַ�(�ֽ�)�����ָ��
*
*********************************************************************************************/

/*********************************************************************************************
* DIB(Device-independent bitmap),�豸�޹�λͼ����ͨ��API��ֱ�Ӹ�������λ���и���
*   λͼ�ļ���Ϣ -- BMP�ļ����ļ�ͷ��λͼ��Ϣͷ����ɫ��Ϣ(��������ɫ��ʱ�Ŵ���?)��λͼ�����Ĳ�����ɡ�
*   DIB �Ǳ�׼�� Windows λͼ�ṹ,BMP�ļ�������һ��DIB�� 
*   1.BMP�ļ�ͷ���ݽṹ����BMP�ļ������͡��ļ���С��λͼ��ʼλ�õ���Ϣ�� 
*     typedef struct tagBITMAPFILEHEADER
*     {
*       WORD  bfType;       //λͼ�ļ������ͣ�����Ϊ'BM'
*       DWORD bfSize;       //λͼ�ļ��Ĵ�С�����ֽ�Ϊ��λ������BITMAPFILEHEADER��14���ֽ�
*       WORD  bfReserved1;  //λͼ�ļ������֣�����Ϊ0
*       WORD  bfReserved2;  //λͼ�ļ������֣�����Ϊ0
*       DWORD bfOffBits;    //λͼ���ݵ���ʼλ�ã��������λͼ�ļ�ͷ��ƫ������ʾ�����ֽ�Ϊ��λ��
*                           //���������� BitMapInfoHeader �ṹ�ͺ������ɫ����������ĳߴ�
*     } BITMAPFILEHEADER;
*     
*   2.BMPλͼ��Ϣͷ��������˵���豸�޹�λͼ(DIB)�ĳߴ����ɫ��ʽ����Ϣ��
*       ���µ�ϵͳӦ���� BITMAPV4HEADER / BITMAPV5HEADER��
*     typedef struct tagBITMAPINFOHEADER{
*       DWORD biSize;       // ���ṹ��ռ���ֽ���
*       LONG  biWidth;      // λͼ�Ŀ�ȣ�������Ϊ��λ�����biCompression��BI_JPEG��BI_PNG����ʾ��ѹ��Ŀ�ȡ�
*       LONG  biHeight;     // λͼ�ĸ߶ȣ�������Ϊ��λ
*         //�����������λͼ�����µ����ϣ�
*         //����Ǹ���(��ʱ����ѹ��)��λͼ�����ϵ����¡�
*         //���biCompression��BI_JPEG��BI_PNG����ʾ��ѹ��ĸ߶ȡ�
*       WORD  biPlanes;     // Ŀ���豸��ƽ�����������Ϊ1
*       WORD  biBitCount;   // ÿ�����������λ������������������֮һ��
*         //0(JPEG��PNG��ʽ)
*         //1(RGB1,��ɫͼ),
*         //4(RGB4,16ɫ),
*         //8(RGB8,256ɫ),
*         //16(RGB565��ȫ�ֱ���bits565������Ϊ0x7C00|0x03E0|0x001F
*         //   RGB555,ȫ�ֱ���bits555������Ϊ 0xF800|0x07E0|0x001F)
*         //24(RGB24����ѹ��)
*         //32��RGB32������Ϊ 0x00FF0000|0x0000FF00|0x000000FF��֮һ��
*       DWORD biCompression;// λͼѹ�����ͣ�������λ����ء���24��32λ�����ز���Ҫѹ������������������֮һ��
*         //BI_RGB(��ѹ��)
*         //BI_RLE8(ѹ��)��BI_RLE4(ѹ��)
*         //BI_BITFIELDS(��ѹ������ɫ����Color-mask)֮һ��
*         //BI_JPEG��BI_PNG �������� JPEG �� PNG��
*       DWORD biSizeImage;  //λͼ�Ĵ�С(������ѹ����BI_RGB��ʽʱΪ0)�����ֽ�Ϊ��λ��
*         //ʵ�ʵ�λͼ����ռ�õ��ֽ��� biSizeImage=biWidth' * biHeight��
*         //  biWidth' ��biWidth ����4��������������Ľ��(��λ��)
*       LONG  biXPelsPerMeter;  // Ŀ���豸��ˮƽ�ֱ��ʣ����ظ���/�� -- MulDiv(PerMeter, 254, 10000) = 96 = GetDeviceCaps(hdc, LOGPIXELSX)
*       LONG  biYPelsPerMeter;  // Ŀ���豸�Ĵ�ֱ�ֱ��ʣ����ظ���/��
*       DWORD biClrUsed;    // λͼʵ��ʹ�õ���ɫ���е���ɫ����
*         //0��ʾ����biBitCountȷ�ϵ�������
*         //���biBitCount С��16��biClrUsed ����ʹ�õ���ɫ�������
*         //���biBitCount ���ڵ���16��biClrUsed��ʾΪ���Ż�����ʹ�õ�ϵͳ��ɫ�������
*       DWORD biClrImportant;   //λͼ����Ҫ����ɫ����0��ʾ������ɫ����Ҫ
*     } BITMAPINFOHEADER;
*       
*   3.��ɫ�� -- ����Ҫ��ɫ��16ɫ��256ɫ�������ʹ�ã����ڲ��Ե�ɫ�巽ʽ�洢��λͼ�����޴�����Ϣ��
*     ����˵��λͼ�е���ɫ������biClrUsed�����ÿһ��������һ��RGBQUAD���͵Ľṹ������һ����ɫ��
*     RGBQUAD�ṹ�Ķ�������: 
*     typedef struct tagRGBQUAD {
*       BYTErgbBlue;    // ��ɫ������(ֵ��ΧΪ0-255)
*       BYTErgbGreen;   // ��ɫ������(ֵ��ΧΪ0-255)
*       BYTErgbRed;     // ��ɫ������(ֵ��ΧΪ0-255)
*       BYTErgbReserved;// ����������Ϊ0 -- 32λ���ʱ��ʾͼ��͸������Ϣ��Alphaͨ����0��ȫ͸����255��͸��
*     } RGBQUAD;
*     ��ɫ����RGBQUAD�ṹ���ݵĸ�����biBitCount��ȷ��:
*       ��biBitCount=1,4,8ʱ���ֱ���2,16,256������;
*       ��biBitCount=24,32ʱ��û����ɫ���
*     λͼ��Ϣͷ����ɫ�����DIBλͼ��������Ϣ��BITMAPINFO�ṹ��������:
*     typedef struct tagBITMAPINFO {
*       BITMAPINFOHEADER bmiHeader; // λͼ��Ϣͷ
*       RGBQUAD bmiColors[1]; // ��ɫ��
*       //��ȡ��ɫ��ָ�룺pColor = ((BYTE*)pBitmapInfo + (WORD)(pBitmapInfo->bmiHeader.biSize));
*     } BITMAPINFO;
*   4.λͼ���� ImageDate����ά���飩
*     �����õ���ɫ���λͼ��ʵ�ʵ�ͼ������ImageDateΪ�����ص���ɫ�ڵ�ɫ���е�����ֵ��
*     �������ɫͼ��ͼ��������Ϊʵ�ʵ�R��G��Bֵ��
*     λͼ����ÿһ�е��ֽ�������Ϊ4����������������ǣ�����Ҫ���롣 
*       m_DIBWidthInBytes = ((m_Size.cx+31)>>3) & ~3; ���ߣ�(m_Size.cx + 31)/32 * 4
*     ��GDI�����У�λͼ�ļ��е������Ǵ��µ��ϡ������ҷ�ʽ�洢��(ͼ�񵹹���?) -- biHeight > 0��
*     ��DirectDraw�����У�λͼ�ļ��е������Ǵ��ϵ��¡������ҷ�ʽ�洢�� -- biHeight < 0��
*     ������Ҫ���Ƶĵ��λ�û�ȡ��λͼ�����е�ƫ����
*       BYTE *pData = pFrame + ( m_y * m_iImageWidth * iPixelSize) + m_x * iPixelSize; //����iPixelSize��ÿһ�����ص��ֽ���(1~4)
*
* DFB(Device-format bitmap) -- �豸��ʽλͼ  == DDB(Device-dependent bitmap) -- �豸���λͼ�������ϻ�ͼ֧��Ӳ������
*   ��Ӳ���豸�����λͼ�������̿�������ϣ���ķ�ʽ����ͼ�����ʽ���豸������ȷ����ͨ������ͨ��API����(����ֱ�Ӹ���ͼ���λ)
*   �� CreateBitmapXxx ������λͼ��
**********************************************************************************************/

/*********************************************************************************************
* ͼ����ԭ��
*   DIB/DDB -- �����ʹ�õ�ɫ��(4��8λɫ)��ϵͳ��˵�ģ���16��24��32λ��ɫʱ���洢����ʵ�ʵ���ɫ�������豸�޹ص�
*   �߼���ɫ��(CDC��)�޸ĺ���Ҫ realize ʹ������ɫӳ�䵽ϵͳ��ɫ��(Windows��)��������Ч����Ч��
*   ��ȡϵͳ��ɫ�����Ϣ��
*     PALETTEENTRY m_Palette[iPALETTE_COLORS];
*     GetSystemPaletteEntries(hdc, 0, iPALETTE_COLORS, (LPPALETTEENTRY) &m_Palette);
* 
* ͼ��͸��  -- (? TrueColor + Alpha, �� 32 bit )
*    ֧�ֵ��ļ�����ʽ��CUR��GIF(256ɫ)��ICO��PNG(�Ƽ�����GIF��չ����?)
*********************************************************************************************/

/*********************************************************************************************
* COM ��ʹ�� IPicture ����ͼƬ������ʹ�� CPictureHolder �ĸ�����
*   ���ļ����أ�hr = OleLoadPicturePath(T2OLE(szPath),NULL,0,RGB(255,255,255),IID_IPicture,(LPVOID*)&pPicture);
*   ���ڴ���أ�hr = OleLoadPicture(pStream, nSize, FALSE, IID_IPicture, (LPVOID *)&m_pPicture));
*   ����:
*     pPicture->get_Width(&hmWidth);
*     int nWidth  = MulDiv(hmWidth, pDC->GetDeviceCaps(LOGPIXELSX), HIMETRIC_PER_INCH); //MAP_LOGHIM_TO_PIX
*     int nHeight = MulDiv(hmHeight,pDC->GetDeviceCaps(LOGPIXELSY), HIMETRIC_PER_INCH);
*     hr = pPicture->Render(dcMem, 0, 0, nWidth, nHeight, 0, hmHeight, hmWidth, -hmHeight, NULL);
*********************************************************************************************/


/*********************************************************************************************
* ͼ�ο⣺
*   CxImage -- ��Դ����ȫ��ѵ�ͼ�������⣬�ɿ�ݵش�ȡ����ʾ��ת������ͼ��
*              ֧��ͼ��Ķ��ز���(�����˲�����ֵ�˲���ֱ��ͼ��������ת���š�����ѡȡ����ֵ����alpha��ϵ�)
*     CxImage�ı����У�pDib(����ͼ��)��pAlpha(͸����)��pSelection(��ѡ�еĲ�)��pLayers(����չ��ͨ�ò�)
*     ximacfg.h �ļ���ͨ�� CXIMAGE_SUPPORT_XXX �ĺ���Կ���CxImage��֧����Щͼ���ʽ
*     CxImage -- ͼ��Ļ��࣬�и��ָ�ʽͼ������ࣻCxFile <== CxIOFile/CxMemFile��CImageIterator
*       CxImageICO -- ֧�ֶ�VistaPNGͼ��Ķ�д
*       CxImageBMP -- ֧�ֶ�32λͼ��Ķ�д(֧��Alphaͨ��)
*       CxImageJPG -- �ṩJPEGͼ���ʽ�Ķ��β������� Encode_SubSample_422��
*     �������⣺
*       1.702 -- �ᱨ�� CCM_SETWINDOWTHEME û�ж���Ĵ���Ϊʲô��
*         �Զ���һ�����Ұ�stdafx.h�е�WINVER�Ⱥ궨��ע�;ͱ���OK��
*   OpenIL 
*   FreeImage
*   PaintLib
*   GdiPlus
*   
* 
* CMemDC -- Keith Rule ���������ڴ�DC�����Դ���ӿ��ͼ��Ч�ʣ�TextProgressCtrl.cpp ��ʹ�õ��Ѿ��ǽ��ϵİ汾
*********************************************************************************************/

/*********************************************************************************************
* giflib -- �鿴������gif�Ŀ�Դ��(gif�汾Ϊ 87a)
*   Gif�ж���汾(87a,89a �ȣ��μ� gif89.txt)���ɴ��һ֡���֡ͼ�����ݣ������Դ��ͼ�������Ϣ(����)����ͼ����ڵ�ɫ�壬���ֻ����255����ɫ��
*   ��ʽ:�ļ�ͷ(Header) + ���ݲ���( ���Ʊ�ʶ��[Control Block] + [ͼ�����ݿ�Data Sub-blocks] + [��չ���ݿ� -- 89a] ) + �ļ�β(Trailer, 0x3B[";"]���ַ�)
*         ÿ�����ݿ��СΪ[0..255],��һ���ֽ�Ϊ�����ݿ�Ĵ�С,�����Сʱ���������ֽ�(�����ݿ���һ���ֽڣ�����־��С�� 0x00),
*   
*   windows�±���: Cygwin + autogen.sh -- ���ٲ��ִ���TODO: MinGW�޷�����.
*   util�µ�demo����(һ��.c�ļ�����һ����ִ���ļ�)�����еĳ��򶼿�ͨ�� -h �鿴����
*     gifasm.c -- �ϲ����gif�ļ�Ϊһ�������ļ�����: gifasm.exe -A 100 src01.gif src02.gif > result.gif
*       ע�⣺��ʵ����ͨ������GlobalColorMapΪ��һ��ͼƬ��ֵ������������ͼƬ�����ݣ��ҵ���ӽ�����ɫ������������һ��ReMapColor��
*             ��������ÿ��ͼƬ���ɾֲ�ColorMap����Ч�����ļ������
*             ������������Compute the remapping of this image's color map to first one
*     gifflip.c -- ��תgif,�ɺ���(-x) �� ����(-y) ��ת��TODO: ����?
*     giftext.c -- ��ӡ��gif����Ϣ������giftext fire.gif�� ��֪������33��ͼƬ,
*     text2gif.c -- ����������ı�������gif�ļ�������text2gif -s 8 -c 255 0 0 -t "hello world" > text2gif.gif
*   ���ͺͽṹ:
*     ColorMapSize[1,8]: ɫ��� 6λɫ���ʾ�� 2^6=64 ����ɫ
*     ColorResolution - 8 ?
*     ColorMapObject(��ɫӳ�����ʵ���Ͼ��ǵ�ɫ��?) -- ��Ϊȫ��(GifFileType.SColorMap)�;ֲ�(GifFileType.Image.ColorMap)�������ͣ�
*       ������ ColorCount �� GifColorType(RGB��Ԫɫ)���͵�Colors�ڴ��
*     GifColorType -- ������ɫ�б��һ����Ŀ���������ֽ���ɣ���R��G��B��˳������
*     GifImageDesc -- ͼ�����ݿ飬���е� Interlace ��ʾ�Ƿ�ʹ�ý�֯��ʽ���У�����ǵĻ�����Ҫ 4 x ÿ��4�� �ķ�ʽ���д��� ?
*                     ���е�SortFlag(�ƺ�giflibδz֧��?)��ʾ�����ŵ���ɫ�б��Ƿ��������
*     GifFileType -- gif�ļ����͵ľ��ָ�룬�����д�С��ColorMap ����Ϣ����ÿ�ν��������ʱ��ͼ�����ݿ鶼���� Image �����ڡ�
*     GifPixelType -- ÿ�����ص����ͣ�Ϊ unsigned char(256ɫ)
*     GifRecordType--���ݿ�����ͣ�����ʱͨ�� DGifGetRecordType ��ȡ�������ͷ�Ϊ:
*       SCREEN_DESC_RECORD_TYPE
*       IMAGE_DESC_RECORD_TYPE(��','[0x2C]��ʼ)--ͼ�����ݿ�,��ͨ�� DGifGetImageDesc �� EGifPutImageDesc ������ÿ�е�������Ҫͨ�� DGifGetLine/EGifPutLine ����(ע�� Image.Interlace ��Ӱ��)
*       EXTENSION_RECORD_TYPE (��'!'[0x21]��ʼ)--��չ�飬ʵ��һЩ�������ܣ���Ӱ������ͼ�����ݽ��롣�Ƚ���Ҫ����: ͼ�������չ��(GRAPHICS_EXT_FUNC_CODE)
*       TERMINATE_RECORD_TYPE (�̶���';'[0x3B])--�������˽������λ�ã������� do{ DGifGetRecordType }while(RecordType != TERMINATE_RECORD_TYPE) ��ѭ��
*   ��������Ҫ��Ϊ����: Decode(��������D��ʼ) + Encode(��������E��ʼ)
*     DGifSlurp
*     EGifCloseFile -- �رմ򿪵��ļ�������ͷ���Դ
*     EGifOpenFileHandle -- �����ļ����������ļ����� (1) ��ʾstdout
*     EGifOpenFileName -- �����ļ���������ļ�
*     EGifOpen -- ͨ���ص��ķ�ʽ������ļ������������ʽ���������������ʽ�������� ��
*	  EGifPutScreenDesc -- �����߼���Ļ��ʶ������Ϣ��������С(SWidth * SHeight), ɫ��(ColorResolution[1..8])������ɫ����(SBackGroundColor)��
*       GlobalColorMap�����ؿ�߱�(Pixel Aspect Radio -- TODO:δֱ������?) �Ȳ�����Ϣ
*     EGifPutImageDesc -- ����һ��ImageͼƬ���ݵĲ�����Ϣ
*     EGifPutLine -- ���õ�ǰ�е�����ֵ?
*     EGifPutPixel -- ���õ�ǰλ�õ�����ֵ?
*     EGifSpew --
*     EGifPutCode|EGifPutCodeNext -- 
*   ��������:
*     QuantizeBuffer|GifQuantizeBuffer -- ��RGB����Ϊ��ɫ����ɫ���ο� QuantizeRGBBuffer ��ʵ�ַ�ʽ
*     
*   ������ɫ�б��ͼ������(Table-Based Image Data),����LZW�㷨ѹ��
*     LZW���볤��(LZW Minimum Code Size) -- Ҳ����Ҫѹ�������ݵ�λ������ֵһ�����ͼ���ɫ��,��Ҫ���ݸ�ֵ��ʼ�������
*       ͼ��������ѹ��ǰ���������и�ʽ��
*         ������--�����ҡ����ϵ��µ�˳������ͼ��Ĺ�դ���� 
*         ��֯��--�����ĸ�ͨ��(pass)�������ݣ�ÿ��ͨ����ȡ��ͬ�е����ݣ�
*           Pass1��ȡ�ӵ�0�п�ʼÿ��8�е����� -- [0, 8, 16, 24, ...]
*           Pass2��ȡ�ӵ�4�п�ʼÿ��8�е����� -- [4, 12, 20, 28,...]
*           Pass3��ȡ�ӵ�2�п�ʼÿ��4�е����� -- [2, 6, 10, 14, ...]
*           Pass4��ȡ�ӵ�1�п�ʼÿ��2�е����� -- [1, 3, 5, 7, 9,...]
*     ͼ������(Image Data) -- ��һ����DataSubBlock ���, ÿ��SubBlockΪ��һ�ֽڳ���+���255�ֽڵ�Data
*
*   GIF89 ��չ��(extension function), ����һ��ͼ���(ͼ���ʶ��)���ı���չ���ǰ�棬�������Ƹ���������ĵ�һ��ͼ��(���ı�)����Ⱦ(Render)��ʽ��
*     ��ͨ�� EGifPutExtensionFirst => EGifPutExtensionNext => EGifPutExtensionLast �� EGifPutExtension ���׺������д���
*     PLAINTEXT_EXT_FUNC_CODE(0x1--1) --ͼ���ı���չ,��������һ���򵥵��ı�ͼ�����������ƵĴ��ı�����(7-bit ASCII�ַ�)�Ϳ��ƻ��ƵĲ�������ɡ�
*       ���ı���(GridBox)���Ƶľ��η�Χ��ʹ��ȫ����ɫ�����ɫ���л��ơ�
*       unsigned char plain[12] = {���С, GridLeft(2byte), GridTop(2byte), GridWidth(2byte),GridHeight(2byte), CellWidth, CellHeight, ForegroundColorIndex, BackColorIndex};
*     GRAPHICS_EXT_FUNC_CODE(0xF9-249) --ͼ�������չ�顣unsigned char Extension[4] = { ����, ��־�ֽ�, delay��λ, delay��λ, ͸��ɫ }
*       ��־�ֽ�-- ���������� ���÷���(Disposal Method)���û������־(Use Input Flag)��͸��ɫ��־ ��
*       ��ʱ(delay time) -- ��λΪ 1/100 ��, ���ֵ��Ϊ1����ʾ��ͣ�涨��ʱ����ټ������´���������
*       ͸��ɫ����(transparent color index) -- ��ͼ�����ʱ������ͬ������ɫֵ����ʾΪ͸������? ��ô��͸��?
*     COMMENT_EXT_FUNC_CODE(0xFE-254)--ע����չ������������¼ͼ�Ρ���Ȩ���������κεķ�ͼ�κͿ��ƵĴ��ı�����(7-bit ASCII�ַ�),
*       ע����չ����Ӱ���ͼ���������Ĵ����Ƽ������������Ŀ�ʼ���β��ͨ�� EGifPutComment ����
*     APPLICATION_EXT_FUNC_CODE(0xFF-255)--Ӧ�ó�����չ,��Ӧ�ó����Լ�ʹ�á�Ӧ�ó�����������ﶨ���Լ��ı�ʶ����Ϣ��
*       unsigned char app[11] = {11, AppI(8byte), AuthCode(3byte) } + data
*
*   Bug����:
*     1.GifFilePrivateType �ĳ�ʼ��: memset(Private, '\0', sizeof(GifFilePrivateType));
*********************************************************************************************/

/*********************************************************************************************
* ������
*   waveOutGetNumDevs(); // ��ȡ�����ĸ���
*********************************************************************************************/
namespace FTL
{
	class CFMultiMediaUtil
	{
	public:
		//��ע��� HCRT �¸����ļ���չ������ȡ����, �� .jpg �� image/jpeg
		FTLINLINE static BOOL GetContentType(LPCTSTR pszFileName, LPTSTR pszContentType, DWORD dwCharSize);
	};

/**************************************************************************************
	//giflib �д��� GRAPHICS_EXT_FUNC_CODE ��ʾ������(��ʱ + ͸��ɫ):
	//TODO:��ʵ�����еĶ����ʵ�ַ�ʽ?
	UINT32 delay = 0;
    if( ExtCode == GRAPHICS_EXT_FUNC_CODE 
        && Extension[0] == GIF_CONTROL_EXT_SIZE) {   //#define GIF_CONTROL_EXT_SIZE 4
        delay = (Extension[3] << 8 | Extension[2]) * 10;
    }
     
    // handle transparent color
    if( (Extension[1] & 1) == 1 ) { trans_color = Extension[4]; } else{ trans_color = -1;}
**************************************************************************************/   
   
}//namespace FTL

#endif //FTL_FILE_H

#ifndef USE_EXPORT
#  include "ftlMultiMedia.hpp"
#endif