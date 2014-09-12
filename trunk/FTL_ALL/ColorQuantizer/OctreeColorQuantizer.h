#pragma once

#include "ColorQuantizerBase.h"

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

namespace FTL
{
    class CFOctreeColorQuantizer : public CFColorQuantizerBase
    {
    public:
        CFOctreeColorQuantizer();
        virtual ~CFOctreeColorQuantizer();
    protected:
        virtual BOOL OnPrepare();
        virtual BOOL OnProcessQuantizer(UINT colorCount, UINT *pResultClrCount);
        virtual void OnFinish();
    private:
        class CFOctreeNode{
        private:

            static BYTE s_MASK[8];// = new Byte[] { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
            unsigned char Red;
            unsigned char Green;
            unsigned char Blue;

            int pixelCount;
            int paletteIndex;

        };
    };
}
