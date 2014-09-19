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
        virtual BOOL ProcessQuantizer(UINT nWantClrCount, UINT *pResultClrCount);
        virtual COLORREF*   GetPalette(UINT* pResultCount);
        virtual UCHAR* GetQuantizerResult(UINT* pBufferSize);
    protected:
        virtual BOOL OnPrepare();
        virtual void OnFinish();
        virtual BOOL OnProcessQuantizer(UINT nWantClrCount, UINT *pResultClrCount) = 0;

    protected:
        //�����ԵĹ��ܺ���
        BOOL _AnalyzeColorMeta();       //���� m_redList/m_greenList/m_blueList ����ԭɫ�б�

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
        std::vector<UCHAR> m_QuantizerResultIndexes;
        
        //����4�ֽڶ��� �� Bpp���������ɫ��COLORREF �б�
        typedef std::list<COLORREF>    ColorMetaList;
        ColorMetaList m_clrList;
    };
}
