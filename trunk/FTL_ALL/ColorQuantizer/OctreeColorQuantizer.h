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
#include <list>
#include <vector>

namespace FTL
{
    class CFOctreeNode;
    class CFOctreeColorQuantizer;
    
    typedef std::list<CFOctreeNode*>        OctreeNodeList;

    class CFOctreeNode{
    public:
        enum {
            MAX_NODE_COUNT  = 8,
        };
        CFOctreeNode(int level, CFOctreeColorQuantizer* pParent);
        ~CFOctreeNode();
        BOOL IsLeaf() const;
        COLORREF    GetColor() const;
        int GetActiveNodesPixelCount() const;
        int GetActiveNodes(OctreeNodeList& result) const;
        void AddColor(COLORREF color, int level, CFOctreeColorQuantizer* pParent);
        int GetPaletteIndex(COLORREF color, int level);
        int RemoveLeaves(int level, int activeColorCount, int targetColorCount, CFOctreeColorQuantizer* parent);

        int GetColorIndexAtLevel(COLORREF color, int level);
        void SetPaletteIndex(int index);
        bool operator < (const CFOctreeNode & other) const{
            return GetActiveNodesPixelCount() < other.GetActiveNodesPixelCount();
        }
    private:

        static BYTE s_MASK[8];// = new Byte[] { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
        int m_Red;
        int m_Green;
        int m_Blue;

        int m_nPixelCount;
        int m_nPaletteIndex;
        CFOctreeNode* m_pNodes[8];
    };

    class CFOctreeColorQuantizer : public CFColorQuantizerBase
    {
    public:
        CFOctreeColorQuantizer();
        virtual ~CFOctreeColorQuantizer();
    protected:
        virtual BOOL OnPrepare();
        virtual BOOL OnProcessQuantizer(UINT colorCount, UINT *pResultClrCount);
        virtual void OnFinish();
    public:
        int Leaves(OctreeNodeList& result);
        void AddLevelNode(int level, CFOctreeNode* octreeNode);
    private:
        int m_nLastColorCount;
        CFOctreeNode*   m_pRoot;
        OctreeNodeList*    m_pLevels;
    };
}
