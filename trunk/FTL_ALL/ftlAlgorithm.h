#ifndef FTL_ALGORITHM_H
#define FTL_ALGORITHM_H

/*******************************************************************************************************
* �����㷨
*   ͼ�����ƶ��㷨��C#ʵ��: http://www.doc88.com/p-34967244171.html
*     http://wenku.baidu.com/view/b80f3ef8770bf78a6529542b.html
*******************************************************************************************************/

/*******************************************************************************************************
* ѹ������ѹ�� �㷨
*   LZW(Lempel Ziv Compression)�㷨 -- 
*     ԭ����ȡԭʼͼ�������еĲ�ͬͼ����������Щͼ������һ�������Ȼ���ñ�����е�ͼ��������
*            ���ԭʼ��դ�����е���Ӧͼ��������ԭʼ���ݴ�С�����Ƶ�ɫ��ͼ���ʵ��ԭ��
*     ������Ǹ���ԭʼͼ�����ݶ�̬�����ģ�����ʱ��Ҫ���ѱ���������л�ԭ��ԭ���ı����
*     ������(CharStream,ͼ��Ĺ�դ��������) ͨ�� �����(String Table)���㣬��� ������(CodeStream, ����ѹ������ı�������)
*******************************************************************************************************/

/*******************************************************************************************************
* �����Ӿ���RGB����ɫ�������ȸ�����ͬ�������������п��Ը��ݲ�ͬ��ɫ����ʹ�ò�ͬ���Ե�ѹ���任��������ȡ���������Ч����
* ������ɫ������Ӱ��ϵ��Ϊ R:G:B=0.299:0.597:0.114, ͨ�������Ӱ��ϵ�� 2:1:4
*
* ��ɫ�����㷨(�� 24λ���ת256ɫ ) -- https://github.com/jsummers/imageworsener/issues/2
*   �����㷨(Clustering Algorithm) -- 
*     ԭ��:ѡ��������ɫ��Ϊ�������ģ����վ���׼��ʹÿ����ɫ��������ľۼ����Ӷ��õ����ࡣ
*     �ŵ�:��һ�����ɫͼ���������������ܵõ��Ϻõ�����Ч��
*     ȱ�㣺�������̵Ŀռ俪���ϴ󣬶��ҶԳ�ʼ�����ĵ�ѡȡ�ǳ���Ҫ���ر��ɫ�ʷֲ������ȵ�ͼ������Ч���������롣
*            ʵ��ʱ��Ҫ���ϾۺϺ͵����������ģ���������Ч�ʽϵ͡�
*   ��λ�з��㷨(Median Cut Algorithm)��TODO:ԭ���CodeProject�Ͻ��ܵ��ƺ���һ��
*     ԭ��:��RGB��ɫ�ռ��У�R��G��B����ɫ��Ӧ�ڿռ�����������ᣬ��ÿһ�����ᶼ����Ϊ0��255,
*           �γ�һ���߳�Ϊ256�Ĳ�ɫ�����壬���п��ܵ���ɫ����Ӧ���������ڵĵ㡣ȡÿ��С����������ĵ���ɫ��
*     CodeProject�ϵ�ԭ��:���ط�����RGB�ռ��У��ҳ�����������ɫ����Сbox; Ȼ��ѡȡ����ᣬȡ�м��ֳ�����box�����ݹ�ֱ���õ�
*           ��Ҫ��N��box�������ƽ��ֵ��
*     �ŵ�:ͼ���������Խ���
*     ȱ��:�ڴ����Ĵ��ٶ���
*   NeuQuant�㷨
*     ԭ��:ʹ�� Kohonen Neural Network(������)
*     �ŵ㣺������������Ч��֮���������ѡ��֧�ָ�����ɫλ��
*   �˲�����ɫ�����㷨(Octree Color Quantization Algorithm) 
*     ԭ��:��ͼ����ʹ�õ�RGB��ɫֵ�ֲ�����״�İ˲����С��˲�������ȿɴ�9�㣬�����ڵ����Ϸֱ��ʾ8λ��R��G��Bֵ��ÿһλ��8��ڵ㡣
*          �ϵ͵Ľڵ���Ӧ�ڽϲ���Ҫ��RGBֵ��λ(�ұߵ�λ)��Ϊ�����Ч�ʺͽ�ʡ�ڴ棬����ȥ����Ͳ���2 ~ 3�㣬���������������̫��Ӱ�졣
*     �ŵ�:Ч�ʸߡ�Ч����
*     ȱ��:�ڴ����Ĵ�
*   �Ż��ĵ�ɫ���㷨(Optimized Palette Algorithm) -- 
*     ԭ��:ʹ��ϵͳ��ɫ��?
*     �ŵ�:�ǳ��죬����û���ڴ�����
*     ȱ��:ͼ�������ܲ�,ֻ֧��256ɫ
*   ����ɫ�㷨(Popularity Algorithm)
*     ԭ��:�Բ�ɫͼ��������ɫ�ʳ��ֵĴ�������ͳ�Ʒ������Ӷ�ѡȡƵ����ߵ�N(256)����ɫ��Ϊ��N(256)����ɫ������ɫ�壬
*          �������ɫ����С����ԭ��ӳ�䵽���ٽ��ĵ�ɫ����ɫ�ϡ�Ϊ�����Ч�ʣ�ͨ��Ҳ�Ƿֲ�С�������ͳ��
*     �ŵ�:�㷨������ʵ��,�Բ�ɫ����С��ͼ��任Ч����
*     ȱ��:һЩ����Ƶ�ʽϵͣ����������Ӿ�Ч�����Ե���Ϣ����ʧ(������Ȱߵ�)
*   �ռ�ɫ������(Spatial Color Quantization) -- ���µ��㷨? 
*     ԭ��:����˶���(dithering),http://moonflare.com/code/scolorq/ -- ���۶����ȵı仯������,�����Ҫ���ٶ����ȵ�����Ч��?����Ҫ�������ȼ���仯�Ĳ���(��Ե)
*     �ŵ�:ͼ�������ܺ�
*     ȱ��:�ٶ���
*   Uniform Quantization(ֱ��������)
*     ԭ��:ת16λɫ��ֱ�Ӽ���RGB������ɫ������ʾ��λ����������ɫ�����ĸ�λ��ȥ����λ����555��565 �� 444 �ַ�,
*          ת256ɫ����ɫ�����ֳɼ����̶��Σ���  8-8-4(255ɫ), 6-6-7(252ɫ)��ÿ����ɫ�����õ���Ӧ�Ķ��У�Ȼ��ÿ�����е�������ɫ��Ӽ���ƽ��ֵ��
*     �ŵ�:�򵥿��٣��ڴ����ĵ�
*     ȱ��:û��ѡ���ԣ��Ƚ�äĿ������������Ŀռ�����
*   Wu's Color Quantizer(Xiaolin Wu)
*     �ŵ�:ͼ���������
*     ȱ��:�������ڴ����Ĵ�
*******************************************************************************************************/
#include <set>
#include <list>
namespace FTL{
    
    
    FTLEXPORT class IFColorQuantizer{
    public:
        virtual ~IFColorQuantizer();
        virtual BOOL SetBmpInfo(UINT nWidth, UINT nHeight, UINT nBpp, BYTE* pBmpData, UINT nBmpDataSize) = 0;
        virtual COLORREF* GetPalette(UINT* pResultCount) = 0;
    };

    FTLEXPORT class CFColorQuantizerBase : public IFColorQuantizer{
    public:
        FTLINLINE CFColorQuantizerBase();
        FTLINLINE virtual ~CFColorQuantizerBase();
    public:
        //IFColorQuantizer
        FTLINLINE virtual BOOL SetBmpInfo(UINT nWidth, UINT nHeight, UINT nBpp, BYTE* pBmpData, UINT nBmpDataSize);
        FTLINLINE virtual COLORREF*   GetPalette(UINT* pResultCount);
    protected:
        UINT m_nWidth;
        UINT m_nHeight;
        UINT m_nBpp;
        UINT m_nBmpDataSize;    //�ܵ��ֽ���
        UINT m_nBmpLineBytes;   //ÿ�е��ֽ���(4�ֽڶ���)
        UINT m_nPaletteItemCount;

        BYTE*       m_pBmpData;
        COLORREF*   m_pResultPalette;
    };

    /*******************************************************************************************************
    * a.giflib �е������㷨?
    * 2.http://www.codeproject.com/Articles/109133/Octree-Color-Palette
    *
    * �˲�����ɫ�����㷨
    *   1.ɨ��ͼ����������أ������ǵ������ۼӵ���Ӧ�Ľڵ��У������µ���ɫ�򴴽�һ��Ҷ�ӽڵ㣬���������ص���ɫ���ݴ������С�
    *   2.���Ҷ�ӽڵ���Ŀ����Ŀ���ɫ����Ҫ����ɫ�����ͽ�����Ҷ�ӽڵ�ϲ������ڵ��У��������ڵ�ת��ΪҶ�ӽڵ㣬
    *     �����д����ɫ�������ۼ�ֵ�������س��ֵĴ�����ͬʱɾ��ԭ�����ϲ���Ҷ�ӽڵ㡣
    *   3.�������ش�������������˲�������Ҷ�ӽڵ�ĸ���ɫֵ��ƽ��ֵ��Ϊ�ڵ����ɫ����ֵ����������Ŀ���ɫ�塣
    *   4.�ٴα����������أ�ͨ��ÿ�����ص���ɫֵ���ɫ����ѡ�е�256ɫ���㣬���һ����ӽ�������ɫֵ�ĵ�ɫ����ɫ���Ѹ����ػ���Ӧ�ĵ�ɫ����ɫ����
    *******************************************************************************************************/
    FTLEXPORT class CFOctreeColorQuantizer : public CFColorQuantizerBase{

    };

    //Xiaolin Wu -- http://www.ece.mcmaster.ca/~xwu/cq.c
    FTLEXPORT class CFWuColorQuantizer : public CFColorQuantizerBase{
    public:

    private:
        enum {
            MAX_COLOR = 512,
            DIR_RED = 2,
            DIR_GREEN = 1,
            DIR_BLUE = 0,
            SIDE_SIZE = 33,
            MAX_SIDE_INDEX = 32,
            MAX_VOLUME = SIDE_SIZE * SIDE_SIZE * SIDE_SIZE,
        };

        //template<typename T>
        //using MomentType = std::vector<std::vector<std::vector<T> > >; //long moment[SIDE_SIZE][SIDE_SIZE][SIDE_SIZE]);
        typedef std::vector<std::vector<std::vector<long> > > ColorMomentType;
        typedef std::vector<std::vector<std::vector<float> > > VolumeMomentType;

        struct ColorCube{
            int RedMinimum;
            int RedMaximum;
            int GreenMinimum;
            int GreenMaximum;
            int BlueMinimum;
            int BlueMaximum;
            int Volume;
        };
        
        FTLINLINE long Bottom(ColorCube &cube, int direction, ColorMomentType& moment);
        FTLINLINE long Top(ColorCube& cube, int direction,  int position, ColorMomentType& moment);

        FTLINLINE long Volume(ColorCube& cube, ColorMomentType& moment);
        FTLINLINE float VolumeFloat(ColorCube& cube, VolumeMomentType& moment);
        FTLINLINE float CalculateVariance(ColorCube& cube);
        float Maximize(ColorCube& cube, int direction, int first, int last, 
            int* cut, long wholeRed, long wholeGreen, long wholeBlue, long wholeWeight);
        BOOL Cut(ColorCube& first, ColorCube& second);
        void Mark(ColorCube& cube, int label, std::vector<int>& tag);
    private:
        ColorMomentType weights;
        ColorMomentType momentsRed;
        ColorMomentType momentsGreen;
        ColorMomentType momentsBlue;
        VolumeMomentType moments;
    };
}

#endif //FTL_ALGORITHM_H

#ifndef USE_EXPORT
#  include "ftlAlgorithm.hpp"
#endif