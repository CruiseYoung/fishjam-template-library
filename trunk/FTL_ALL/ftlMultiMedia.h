#ifndef FTL_MULTI_MEDIA_H
#define FTL_MULTI_MEDIA_H
#pragma once

#ifndef FTL_BASE_H
#  error ftlMultiMedia.h requires ftlbase.h to be included first
#endif

/*********************************************************************************************
* Ϊ��ʹ��OEM��Դ(�� LoadOEMIcon��)����Ҫ�� stdafx.h �Ŀ�ʼ�� #define OEMRESOURCE
* 
* BMP(Bitmap) -- λͼ(��դͼ��),BMP �ļ�ͨ����ѹ������˲�̫�ʺ�ͨ�� Internet ���䡣
* Exif(Exchangeable Image File) -- �ɽ���ͼ���ļ�����������������������Ƭ���ļ���ʽ��
*   ���и��� JPEG ���ѹ����ͼ�� �� �������ڡ������ٶȡ��ع�ʱ�䡢����ͺ� ����Ƭ���������Ϣ��
* Gif(Graphics Interchange Format) -- ������ Web ҳ����ʾͼ���ͨ�ø�ʽ,����������ɫ֮���������߽��ͼƬ��
*   ��������ѹ������ָ��һ����ɫΪ͸��ɫ(ͼ�񽫾�����ʾ�����κ� Web ҳ�ı���ɫ)��
*   �ڵ����ļ��д洢һϵ�� GIF ͼ������γ�һ������ GIF����ÿ��������ܴ洢8λ��ֻ����ʾ256����ɫ��
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
* DIB λͼ�ļ���Ϣ -- BMP�ļ����ļ�ͷ��λͼ��Ϣͷ����ɫ��Ϣ(��������ɫ��ʱ�Ŵ���?)��λͼ�����Ĳ�����ɡ�
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
**********************************************************************************************/

/*********************************************************************************************
* ͼ����ԭ��
*   DIB/DDB -- �����ʹ�õ�ɫ��(4��8λɫ)��ϵͳ��˵�ģ���16��24��32λ��ɫʱ���洢����ʵ�ʵ���ɫ�������豸�޹ص�
*   �߼���ɫ��(CDC��)�޸ĺ���Ҫ realize ʹ������ɫӳ�䵽ϵͳ��ɫ��(Windows��)��������Ч����Ч��
*
* ͼ��͸��  -- (? TrueColor + Alpha, �� 32 bit )
*    ֧�ֵ��ļ�����ʽ��CUR��GIF(256ɫ)��ICO��PNG(�Ƽ�����GIF��չ����?)
*********************************************************************************************/

/*********************************************************************************************
* COM ��ʹ�� IPicture ����ͼƬ������ʹ�� CPictureHolder �ĸ�����
*   hr=OleLoadPicturePath(T2OLE(szPath),NULL,0,RGB(255,255,255),IID_IPicture,(LPVOID*)&pPicture);
*********************************************************************************************/


/*********************************************************************************************
* ͼ�ο⣺
*   CxImage -- ��Դ����ȫ��ѵ�ͼ�������⣬�ɿ�ݵش�ȡ����ʾ��ת������ͼ��
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
* CMemDC -- Keith Rule ���������ڴ�DC�����Դ���ӿ��ͼ��Ч�ʣ�TextProgressCtrl.cpp ��ʹ�õ��Ѿ��ǽ��ϵİ汾
*********************************************************************************************/

/*********************************************************************************************
* ffdshow -- һ����ѵı�����������װ����DirectShow�ı�׼�����
*   ʹ�� libavcodec library �Լ��������ֿ���Դ����������(�� DivX��)����֧��H.264��FLV��MPEG-4�ȸ�ʽ��Ƶ
*  
* ffmpeg -- ��Դ�ҿ�ƽ̨������Ƶ�����������һ�ױ����Ŀ�ܣ����вɼ������롢�����ȹ��ܣ�http://www.ffmpeg.org/��
*   http://www.ffmpeg.com.cn, http://ffmpeg.zeranoe.com/builds/
* ģ��
*   ffmpeg -i �����ļ� ����ļ� -- ��Ƶ�ļ�ת�������й���,Ҳ֧�־���ʵʱ���ӿ�ץȡ�ͱ������Ƶ�ļ�
*   ffserver -- ����HTTP(RTSP���ڿ�����)����ʵʱ�㲥�Ķ�ý�������.Ҳ֧��ʱ��ƽ��(Time-Shifting)
*   ffplay -- �� SDL��FFmpeg�⿪����һ���򵥵�ý�岥����(��Ҫ�Ȱ�װ SDL ����ܱ���)
*   libavcodec -- ����������FFmpeg����Ƶ��������Ŀ�.Ϊ�˱�֤�������ܺ͸߿ɸ�����,��������������ͷ������
*   libavformat -- ���������е���ͨ���Ӹ�ʽ�Ľ������Ͳ������Ŀ�
* 
* ����͵���(�޷�ʹ��VC���룿)��������MSys+MinGW���룬���Ǳ��������DLL�ǿ��Ա�VCʹ�õ�
*   http://www.cnblogs.com/mcodec/articles/1659671.html -- ʹ��VC����ķ���
*   http://ffmpeg.org/trac/ffmpeg/wiki/How%20to%20setup%20Eclipse%20IDE%20for%20FFmpeg%20development -- ����eclipse�Ļ���
*     1.�� http://www.eclipse.org/downloads/ ���� "Eclipse Classic"����ֱ�����ش� C/C++ �İ汾
*     2.���߰�װCDT���: Help -> Install New Software -> http://download.eclipse.org/releases/indigo -> ����CDT
*     3.���߰�װGit���(http://www.vogella.de/articles/EGit/article.html), 
*     4.��װMinGW
*     4.Clone FFMpeg ��Git�ֿ�: File->Import->Git->Projects From Git(git://source.ffmpeg.org/ffmpeg.git)
*     5.��������: File->New->Project-> C/C++ -> Makefile Project with Existing Code 
* 
*   http://ffmpeg.zeranoe.com/blog/  -- �Զ����غͱ���ffmpeg�Ľű�
*   ����ѡ��
*     ./configure --enable-shared --disable-static --enable-memalign-hack --disable-yasm
*     --disable-debug			���õ�����Ϣ,���뷢���汾
*     --disable-optimizations	
*     --enable-shared			���붯̬��
*     --disable-static			���þ�̬��
*     --enable-gpl				��ѭgplЭ��,����ָ������Ϊx264��gplЭ��
*     --enable-libfaac     
*     --enable-libx264			ʹ��x264��Ϊh264������,��ʾҪʹ�õ�3����x264,��ʱmingw include �� lib�ڱ����Ѿ���x264.h��libx264.a
*     --enable-libmp3lame		ʹ��mp3lame��Ϊmp3��������http://sourceforge.net/projects/lame/files/��
*     --enable-memalign-hack    �����ڴ���䣬Windows�±���
*     --enable-small			�����ļ���С����
*  ����
*    av_log_set_callback
*    
*  �����⣨ffmpeg ֻ��һ����ܣ���ͬ�ı������ڲ�ͬ����վ��http://ffmpeg.zeranoe.com/builds/����
*    ��ͨ�� --extra-cflags ��ָ��ͷ�ļ����ļ���?
*    libavfilter -- ���libswscale��������ͼ���pixel formatת��������DirectShow ���ƣ�
*      AVFilterGraph -- ������ȫ��ͬ��directShow�е�fitlerGraph
*      AVFilter -- ����һ��Filter
*      AVFilterPad -- ����һ��filter�����������ڣ���ͬ��DShow�е�Pin
*      AVFilterLink -- �����������ӵ�fitler֮���ճ����
*    libx264 
*    libfaac 
*    libmp3lame -- linux ��֧��mp3
*    libogg 
*    libspeex 
*    libswscale -- 
*    libvorbis 
*    libtheora  
* 
*  Դ�����
*    Demuxer -- ý���ʽ������
*    struct AVCodec -- codec �����ݽṹ��ȫ�ֱ��� *first_avcodec, avcodec_register_all ����ע��ȫ�� codec
*    ���벥�ţ�Media -> ByteIOContext -> Demuxer -> Decode -> YUV -> Display
*    ��ʽת����Media -> ByteIOContext -> Demuxer -> Decode -> YUV -> Encode -> Muxer -> ByteIOContex -> Media
*    ��ʽת������ �ļ����ʹ� .flv -> mp4
*    �������ͣ��� codec �� mpeg4 -> H264
*    ý��¼�ƣ�YUV -> Encode -> Muxer -> ByteIOContex -> Media
*
* ��������
*   av_register_all();
*   avcodec_register_all();
*   AVFrame *m_pRGBFrame = avcodec_alloc_frame();  //RGB֡����
*   AVCodec *pCodecH264 = avcodec_find_encoder(CODEC_ID_H264); //����h264������
*   AVCodecContext *pContext = avcodec_alloc_context3(pCodecH264); //��ȡ��ʼ�ı����������ģ�������Ը���
*   pContext->bit_rate = 3000000; pContext->width=nWidth;
*   AVRational rate; rate.num = 1; rate.den = 25;  pContext->time_base = rate; //ÿ��25֡
*   pContext->gop_size = 10; //ÿ10֡��һ��GOP
*   pContext->thread_count = 1; //���̸߳���(TODO?��ʲô����)
*   avcodec_open2(pContext, pCodecH264, NULL); //�򿪱����
*   SwsContext * scxt = sws_getContext(pContext->width, pContext->height, PIX_FMT_BGR24, 
*      pContext->width, pContext->height, PIX_FMT_YUV420P, SWS_POINT, NULL, NULL, NULL);
*   avpicture_fill((AVPicture*)m_pRGBFrame, (uint8_t*)rgb_buff, PIX_FMT_RGB24, nWidth, nHeight); 
*   avcodec_encode_video2(c, &avpkt, m_pYUVFrame, &got_packet_ptr); //����
*
*
* libavFilter ʹ������
*   AVFilterGraph *graph = avfilter_graph_alloc();
*   AVFilterContext *filt_src = NULL;
*   avfilter_graph_create_filter(&filt_src, &input_filter, "src",NULL, is, graph); //input_filter�����ɵ�����ʵ��
*   AVFilterContext *filt_out = NULL;
*   avfilter_graph_create_filter(&filt_out, avfilter_get_by_name("buffersink"), "out", NULL, pix_fmts, graph);
*   avfilter_link(filt_src, 0, filt_out, 0); //����source��sink
*   avfilter_graph_config(graph, NULL); //
*   ʵ��input_filter
*     static AVFilter input_filter = { .name="ffplay_input", .priv_size=sizeof(FilterPriv), .init=input_init, xxx};
*     
*   
*********************************************************************************************/


namespace FTL
{
}//namespace FTL

#endif //FTL_FILE_H

#ifndef USE_EXPORT
#  include "ftlMultiMedia.hpp"
#endif