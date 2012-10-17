#pragma once

#if 0
//�������������꣬����ͨ��2D���ƣ�����ͨ��3D����
#define DRAW_FISHJAM_2D
#endif

#if 0
//�������������꣬����ͨ���� InitDirect3D �� hWnd ����Ϊһ��D3D����(��ȫ��)����ķ�ʽ����������н��� 
#define DX_SNAP
#endif

//�Զ���� FVF ����ṹ -- �о�����ʱ��û����� D3DXXXX ���ж��壿
struct FISHJAM_VERTEX
{
#ifdef DRAW_FISHJAM_2D
    FLOAT x, y, z, rhw; // The transformed position for the vertex
#else
    FLOAT x, y, z;      // The untransformed, 3D position for the vertex, 
                        //�ȼ��� D3DXVECTOR3 position

    D3DXVECTOR3 normal; // The surface normal for the vertex�����巨�߷���
#endif
    D3DCOLOR color;     // The vertex color
    //FLOAT       tu, tv;   // The texture coordinates
};

#ifdef DRAW_FISHJAM_2D
//RHW -- �����Ѿ���ת��(transformed)��������ϵ
#define D3DFVF_FISHJAM_VERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)
#else
#define D3DFVF_FISHJAM_VERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)
                              //����     |����         |������(��ɫ)  |����
#endif

class CMainFrame : public CFrameWnd
{
    DECLARE_DYNCREATE(CMainFrame)
public:
    CMainFrame(void);
    ~CMainFrame(void);
    DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg void OnPaint();

    /************************************************************************
    ע�⣺SDKʾ���в������µķ�ʽ����ͼ����� -- ����Ϣʱ��������Ϣʱ�����
          �˴�����Timer�ķ�ʽ����ģ�����
          ��һ�ָ��ã����߱�׼��ʽӦ����ʲô��
    if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }
    else
    {
        Render();
    }
    *************************************************************************/
    afx_msg void OnTimer(UINT_PTR nIDEvent);


    /*******************************************************************************************************
    *
    *******************************************************************************************************/
    HRESULT InitDirect3D(HWND hWnd);

    //��ʼ�� vertex buffer��Ȼ�����ͨ�� Lock/Unlock ����ʹ��
    HRESULT InitVertexBuffer();

    //�������硢��ͼ��ͶӰ��ת������
    HRESULT SetupMatrices();

    //���ó����еĵƹ�Ͳ���
    HRESULT SetupLights();

    HRESULT SetTexture();

    //�� XFile �м��� Mesh
    HRESULT SetupXFileMesh(LPCTSTR pszXFilePath);

    HRESULT Render();
private:
    IDirect3D9*                 m_pD3d;
    IDirect3DDevice9*           m_pD3dDevice;
	IDirect3DSurface9 *			m_pOffscreenPlainSurface;
    IDirect3DVertexBuffer9*     m_pD3dVertexBuffer;
    IDirect3DTexture9*          m_pD3dTexture;

    //ʹ��Ԥ�����Mesh(XFilee) -- ���п����ж�� Material��������Ҫ�� SetupXFileMesh ����FTL�У�
    ID3DXMesh*                  m_pD3dMesh;
    DWORD                       m_dwNumMaterials;
    D3DMATERIAL9*               m_pMeshMaterials; // Materials for our mesh -- ʹ��ʱ����һ������
    IDirect3DTexture9**         m_ppMeshTextures; // Textures for our mesh -- ���鷽ʽ��Mesh�п����ж��Material��ÿ����Ӧһ��Texture
public:
};
