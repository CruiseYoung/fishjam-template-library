#pragma once

#include <ftlBase.h>
#include "ColorQuantizer.h"

#include <list>
namespace FTL
{
    class CFColorQuantizerBase : public IFColorQuantizer{
    public:
        CFColorQuantizerBase();
        virtual ~CFColorQuantizerBase();
    public:
        //IFColorQuantizer
        virtual BOOL SetBmpInfo(UINT nWidth, UINT nHeight, UINT nBpp, BYTE* pBmpData, UINT nBmpDataSize, BOOL bCopyMem);
        virtual BOOL ProcessQuantizer(UINT colorCount, UINT *pResultClrCount);
        virtual COLORREF*   GetPalette(UINT* pResultCount);
        virtual int* GetQuantizerBuffer(INT* pSize);
    protected:
        virtual BOOL OnPrepare();
        virtual void OnFinish();
        virtual BOOL OnProcessQuantizer(UINT colorCount, UINT *pResultClrCount) = 0;

    protected:
        //�����ԵĹ��ܺ���
        void _AnalyzeColorMeta();       //���� m_redList/m_greenList/m_blueList ����ԭɫ�б�

    protected:
        UINT m_nWidth;
        UINT m_nHeight;
        UINT m_nBpp;
        UINT m_nBmpDataSize;    //�ܵ��ֽ���
        UINT m_nBmpLineBytes;   //ÿ�е��ֽ���(4�ֽڶ���)
        UINT m_nPaletteItemCount;

        BOOL        m_bCopyMem;
        BYTE*       m_pBmpData;
        COLORREF*   m_pResultPalette;
        UCHAR*      m_pResultBuffer;
        std::vector<int> m_indices;
        
        //��ԭɫ���б� -- ����4�ֽڶ���
        typedef std::list<unsigned char>    ColorMetaList;
        ColorMetaList   m_redList;
        ColorMetaList   m_greenList;
        ColorMetaList   m_blueList;
    };
}
