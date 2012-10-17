#ifndef DIRECT3D_STUDY_H
#define DIRECT3D_STUDY_H
#pragma once

/*****************************************************************************************************
* ��ʱѧϰ�Ĳ���
*   ʸ����ͼ -- Ӧ�����ı���ͼ���Ч���ǳ��ã��������ɵķŴ���С�����ҿ��Էǳ���ȷ�Ŀ���ͼ��Ĵ�С��λ�á�
*   λͼ��ͼ -- ͼ�������ȡ����ͼ��ķֱ���(�ܵ���������)����ɫ��ɫ��(ÿ�����ص���Ϣ��)��һ��ʹ��ѹ��(JPEG/PNG��)�İ취�洢ͼƬ
*   
* ��������ҵ�����
*   �ӳ�/ͬ�� -- ��Ҫ���ֶ�̨PC��ʱ��ͬ��
*   ���ݶ�ʧ -- �ڴ�������У����ݲ��ɱ���Ļ��ж�ʧ
*   �ɿ��� -- ��������ͨѶ�п����Ǽ�ϵ�
* ��������ģ��
*   C/S -- 
*   P2P(peer-to-peer) -- ���пͻ��˶�����Ϸ���й�ͬ������Ϸ��״̬
*
* ����(��Ϸ����)--��ʾ��Ϸ�������˶��Ĳ��֣����԰��� ��ҡ����ˡ�������Ĳ��֡��ӵ���
*   ��Ҫ���������Ϣ(��ʱλ�á���ʱ�ٶȡ���ʱ���򡢿ɼ��� ��)��
*   ͨ��ʹ�� ����ľ���ԭ��������ʱ����ͬ���ľ���(clone ���ģʽ)
*
*****************************************************************************************************/

/*****************************************************************************************************
* ��������
*   D3DERR_INVALIDCALL -- ���ݵĲ�������Ҫ��
*****************************************************************************************************/


/*****************************************************************************************************
*
*
* ���� -- ��һ�����ͷ���������
* ���� -- D3D�п������Ķ����Լ��Ķ����ʽ��ʹ�ÿɱ䶥���ʽ(FVF--Flexible Vertex Format)����ϱ�־��������(��־˳�����Ͷ���ṹ˳����ͬ)��
*   Ȼ��ͨ���������㻺������������ȾԴ����ʾ������ͼ��
*   ����struct ColorVertex
*       {
*           float _x, _y, _z;     // λ��
*           DWORD _color;         // ��ɫ
*       };
*       #define FVF_COLOR (D3DFVF_XYZ | D3DFVF_DIFFUSE)   //NORMAL--���㷨�ߣ� TEX1 -- ��������
*         D3DFVF_XYZ -- �������3D���꣬(0,0)����Ļ�����ģ���λΪ��׼��λ����ΧΪ [-1,1]��Ĭ�Ϸǹ���
*         D3DFVF_XYZRHW -- ������Ǿ���ת�����2D���꣬ͨ������2D��ͼ��Reciprocal of Homogenous W()
*           (0,0)����Ļ�����Ͻ�,��λ�ƺ������أ���ΧΪ [�����,�����]��Ĭ�ϲ��ø����Ĺ���ģʽ
* 
* ���� -- Direct3D��Ĭ�϶�����˳ʱ�뷽���ڹ۲�����ϵ�У��γɵ�������Ϊ���棬����ʱ�뷽���γɵ�������Ϊ���档
* ����(Texturing) -- Ϊ�����θ���ͼ������(������ͼ)���ܹ���ϸ�����ʵ�ر��ֳ���
* �ڴ�� -- 
*   D3DPOOL_DEFAULT(������Դ�����ͺ���;�����Ƿ�������ʵĵط�, �������ܴ�����Դ���),
*     ���ڳ�������ʱ������豸��ʧ��������Ҫ����Ա���豸��ʧʱ�ͷ����ڴ棬���豸�ָ�ʱ���·����ڴ�
*   D3DPOOL_MANAGED(��Դ����Direct3D�����Ұ��豸����Ҫ��ָ������ϵͳ�ڴ滹�Ƿ���AGP�Դ���) -- ���ܲ���Ҫʱ�İ�ȫѡ��
*   D3DPOOL_SYSTEMMEM(��Դλ��ϵͳ�ڴ��У��ڳ����˳�֮ǰ��פ�ڴ棬��������豸��ʧ������)
*
* ������ -- Direct3Dͨ������2~3���������һ������(������)��ʹ��ҳ���л�����ʹ��֡����֮����ȸ�ƽ����
* ��Ȼ���(Z-Buffering) -- Ҳ��һ�����棬�����洢ͼ�����ݣ����Ǽ�¼���ص������Ϣ����ȷ����һ��������󱻻��Ƴ�����
*   ���ֵ�ĸ���=��x�ߡ�Ϊÿһ�����ؼ������ֵ����������Ȳ��ԣ��Ӷ�ֻ�����������������أ��������Ʊ���ס�����ء�
*   Direct3Dʹ�õ���Ȼ���ķ�Χ��0~1������SetViewport�еĲ�����˵������
* ���㴦�� -- ���Ա����(�ܱ�֧������Զ����)��Ӳ������(�Կ�Ӳ������֧�֣�Ҳ��Ϊ Ӳ������ת���͹�Դ����)��
* ��Ⱦ����(Rendering Pipeline) -- ����Ϊ3D������м���������2Dͼ�β��趨һ�����������ȷ�������������һ���ֽ���͸��ͶӰ����Ļ�ϡ�
*   ͨ����ͨ������任ʵ�ֵ�һϵ������任��ʹ�� IDirect3DDevice9::SetTranform �ṩ�任����
*   Local Space(��������ϵ�����˽�ģ�Ĺ��̣���3Dģ����ԭʼ�����꣬Ҳ�� Model Space--ģ�Ϳռ�) 
*     -> World Space(��������ϵ�������е����嶼ͨ��ƽ�ơ���ת�����ź�ŵ�ͬһ����������ϵ�У�D3DTS_WORLD)
*       -> View Space(��ͼ����ϵ���任������ĳ����λ�ã� ͨ�� D3DXMatrixLookAtLH ��ñ任������ͨ�� D3DTS_VIEW)
*         -> Backface Culling(�����ѡ��D3Dͨ��ɾ���������εĴ��������Ч�ʣ�ʹ�� SetRenderState ����)
*           -> Lighting(��Դ����������������ϵ��Ȼ�󱻱任����ͼ����ϵ�У������˳������������ʵ��)
*             -> Clipping(�ü�--��ѡ���������巶Χ�ļ���ͼ�εĹ��̣��ڿ�������Ĳ��ֲ�������)
*               -> Projection(ͶӰ--��3D����ͶӰΪ2Dͼ���ʾ��ʹ�� D3DXMatrixPerspectiveFovLH ���������ı任����Ȼ��ʹ�� D3DTS_PROJECTION) 
*                 -> Viewport Transform(�ӿڱ任��ת��ͶӰ���ڵ���ʾ��Ļ�ϣ�ʹ�� SetViewport ��������) 
*                   ->Rasterization(��դ����������Ҫ��ʾ��ÿ����������ÿ�������ɫֵ����2Dͼ����ʾ����ʾ���ϣ��ǳ����أ���Ӳ�����)
* ����������(Triangle Mesh) -- ʹ��������������Ʊ�ʾһ������(��ģ),�� XFile
*   D3DX�ṩ�� D3DXCreate(Box/Sphere/Cylinder/Teapot/Polygon/Torus)��6���������ɺ�����Ҳ����ʹ�� D3DXCreateMesh[FVF] ����һ����mesh
*   һ��mesh��һ���������Ӽ�(subset)��ɣ�һ���Ӽ���ʹ����ͬ����(���ʣ��������Ⱦ״̬��)��Ⱦ��һ�������Ρ���һ�����ӿ�����
*   �ذ塢ǽ�ڡ��컨�塢�����ȶ��subset��ɡ�ÿһ���Ӽ���һ�����Ӧ�Ĳ��ʺ�����
* ������ -- �����ζ����˳���Ǻ���Ҫ
* ���� -- ����ͼ���о������ظ��Ķ��㣬Ϊ�˼򻯱�ʾ������һ�������б�(�������в��ظ��Ķ���)��һ�������б�
*   (�ö������ж����ֵ����ʾÿһ�������εĹ��췽ʽ)
*   һ����������������������� -- Vertex rect[6] = { {v0, v1, v2},{ v0, v2, v3}};
*     �����б�Vertex vertexList[4] = {v0, v1, v2, v3};
*     �����б�WORD indexList[6] = { 0, 1, 2,  //������0
*                                     0, 2, 3}; //������1
* [����|����]���� -- һ�������Ĵ洢��[����|����]���ݵ��ڴ�,�ܱ��������Դ���(����)����IDirect3DVertexBuffer9�ӿڶ���
*   ��̬���� -- ͨ�����������Դ��У��ܸ�Ч�Ĵ��������ж�ȡ��д�������Ǻ����ģ�ͨ�����ڱ��治��Ҫ�������ı������(����κͽ���)
*   ��̬����(D3DUSAGE_DYNAMIC) -- ͨ��������AGP�ڴ��У��ܱ��ܿ�ĸ��£�����������Ⱦǰ��ת�Ƶ��Դ��С�ͨ�����ھ������µ�����(������ϵͳ)
* ����� -- ȷ��3D�����е��Ĳ����ǿɼ��ģ������Ҫ���Ĳ���ת��Ϊ2Dͼ�Σ�����������ϵ��Դ�㲢����Z��������
* ����� -- ʵ��һ��Camera�࣬ʹ���ǿ��Ը��õز������������������Ϊ����ģ��������͵�һ�˳��ӽ������ -- ����μ�����
* ������ -- �ɿ��ӽǶȺ�ǰ�ü��棨Near Plane�����ü��棨Far Plane�������һ����̨
* ��Ⱦ״̬ -- �ṩ�˶�����Ⱦ״̬����Ӱ�켸��������������Ⱦ
*
* XFile -- ʹ��3D��ģ����(�� 3DMax��LightWave3D��Maya ��)���츴�ӵ���ʵ��mesh��D3DX�ṩ�˶���дXFile�ĺ���
*   (D3DXLoadMeshFromX ��  )��DirectX SDK Extra ������� 3DMax��Maya ��������� .X File ��Direct3D ���߰���
*   ����ʹ�� MView.exe �鿴
*  
* �����ݻ�(Bounding Volumes) -- ��mesh�ı߽緶Χ�����õ����������ͣ���������򡣳��������ٿɼ��Բ��ԡ���ײ���ȡ�
*   D3DX�ṩ�� D3DXComputeBoundingSphere/D3DXComputeBoundingBox ��������߽��/�߽���
*
******************************************************************************************************/

/******************************************************************************************************
* �任����             ���ž���              ��ת����
*   1   0  0             sx 0  0               cos   x   sin
*   0   1  0             0  sy 0               -sin  x   cos
*   dx dy  0             0  0  1               0     0    1
* x2 = x1 + dx         x2=sx * x             
* y2 = y1 + dy         y2=xy * y
******************************************************************************************************/

/******************************************************************************************************
* C#������
*   ��Ŀ--���������-����Microsoft.DirectX��Microsoft.DirectX.Driect3D���������ռ���ӵ�������
*   using Microsoft.DirectX;
*   using Microsoft.DirectX.Driect3D;
* 
* ���� -- ���������������Ϣ��
*   ���� D3D_DEBUG_INFO �����Բ鿴������Ϣ������ IDirect3D9 �е� Version �ֶΣ�
* ����FPS -- ÿ��Լһ���ʱ�䣬����֡��/������Ϊ��ǰ��FPS��Ȼ��֡����������գ�Ϊ�´μ�����׼����
* 
* ��ѧ����
*   ��������ϵ(Z����)����������ϵ(Z����) -- ͨ��D3D��ʹ�����֣���OpenGLʹ�����֣� 
*   ����(D3DXVECTOR3) -- 3ά�ռ��е������߶Σ���׼�����������������ҳ���Ϊ1������(��λ����)��ʵ�ʾ���(x,y,z),��ʾ(0,0,0)���õ���߶�
*     Ҳ��2D����(D3DXVECTOR2)��4D����(D3DXVECTOR4����4����Աȱʡ��1[��ʾ����]��0[��ʾ����]�����ˣ�)��
*     ��׼������(D3DXVec3Normalize)�����ȵ���1��������
*     ���(D3DXVec3Dot) -- ���������ĳ˻�����ά�ֱ���ˣ�Ȼ����ӣ������һ������(��)����ʾ���������ĵ����
*       ���ǵ�ģ�ͼнǵ�����֮��(���ģ��Ϊ1�����Ǽн����ң�����н�)����������������Ϊ0��������������ֱ��
*     ���(D3DXVec3Cross) -- P = U x V = [(UyVz - UzVy), (UzVx - UxVz), (UxVy - UyVx)]��
*       �������Pͬʱ��ֱ��U��V��
*     ע�⣺�������(==)�Ǹ������Ƚϣ��ǽ�����ȣ�D3DXVECTOR3�ڲ�ʵ�֣�
*   �������� -- ��ˮƽ�����u�����ֱ����v��(����Ϊ��)���ɡ�  
*   ����(D3DXMATRIX -- 4x4�ľ���)
*     �˷� -- �ܱ任�������ҽ���ͬ����ת����һ��A�������������B����������A��B������ˡ�
*     ����(D3DXMatrixInverse) -- ֻ�з���������棬����������������˵õ�һ����λ����
*     ת��(D3DXMatrixTranspose) -- �໥����������к���
*     ��λ����(D3DXMatrixIdentity) -- ���˶Խǣ����ϵ����£��������г�Ա����0���ԽǶ���1�ķ�����
*       �õ�λ������Ծ��󲻻�ı����
*   
*   ����Ļ����任(ʹ�þ���-������˵ķ�ʽ)
*     ƽ��(D3DXMatrixTranslation) -- ע��ͨ��ƽ�Ʋ���ı�����
*     ��ת(D3DXMatrixRotationX��D3DXMatrixRotationY��D3DXMatrixRotationZ) -- ע����λ�ǻ���
*     ����(D3DXMatrixScaling) -- 
*     �任������(D3DXVec3TransformCoord) -- �任��ͬʱ����������4����ԱΪ1(����) �� D3DXVec3TransformCoordArray
*     �任��������(D3DXVec3TransformNormal) -- �任�����������õ�4����ԱΪ0(����) �� D3DXVec3TransformNormalArray
*     ͶӰ(���������ռ�ӳ�䵽3D�ռ�?) -- 
*     ��ͼ�ռ�任 -- ����������ϵ�н��м��α任�Ա㽫�����ƽ�Ʊ任������ϵ��Դ�㲢�����ķ�����ת������Z���������
*   �ۺϱ任 -- �Ѷ���任(ƽ�ơ���ת�����ŵ�)ͨ����λ����ϲ�Ϊһ�����󣬲�ʹ�þ���˷����м��㣬
*     ���Դ����߲�����ͬ��ʽ����������ʱ��Ч�ʡ�
*
*   ƽ��(D3DXPLANE) -- ͨ��һ��3D����n(������)��ƽ���ϵ�һ����p0��������ʹ��4D���� (n,d)��ʵ�֡�
*     ��p��ƽ��(n,d)�Ĺ�ϵ(D3DXPlaneDotCoord)��
*       1.n��p + d == 0 ; ��p��ƽ�湲��
*       2.n��p + d >  0 ; ��p��ƽ���ǰ������ƽ�������ռ���
*       3.n��p + d <  0 ; ��p��ƽ��ı�������ƽ��ĸ���ռ���
*     ����ƽ��(D3DXPlaneFromPointNormal) -- ��ָ�����ߺ͵㴴��ƽ��
*             (D3DXPlaneFromPoints) -- ͨ����ƽ���ϵ�3���㴴��һ��ƽ��
*     ��׼��ƽ��(D3DXPlaneNormalize) -- �������Ǳ�׼(����Ϊ1)��ƽ��
*     �任ƽ��(D3DXPlaneTransform) -- ��һ��ת���������㣨ƽ�ơ���ת�����ţ���ע�⣺��Ҫ�ȱ�׼��ƽ��
*   
*   ��ɫ -- ʹ��RGB��������������D3DCOLOR(UBTYE�͵�ARGB)��D3DCOLORVALUE/D3DXCOLOR(0-1float�͵�ARGB)
*     ͼԪ����ɫ���ɹ������Ķ������ɫ������
*     Ŀǰ��2����ɫ����ģʽ��ƽ����ɫ��flat����ɫ���ȣ��͸����[ƽ��]��ɫ��Gouraud�����Բ�ֵ�����м����ɫ����
*       ��SetRenderState(D3DRS_SHADEMODE...)����
*
*   �ƹ� -- ʹ�õƹ�ʱ������ָ���������ɫ���������㶼ͨ���ƹ����棬���ݵƹ����ɫ�����͡�
*     �������(Material)�����淽��(surface normal)�ȼ��㶥����ɫ��
*     �����ֵƹ⣺
*       �����⣨Ambient��
*       �����䣨Diffuse Reflection��������Ҫ���ǵƹⷽ��ͱ������̬
*       ���淴�䣨Specular Reflection���������������ϲ����߹�ĵط�
*
*   ��Դ��D3D֧���������͵Ĺ�Դ������ D3DLIGHT9 �ṹ��ʾ
*     ���Դ(point) -- ��������������һ��λ���������з����϶��������
*     �����Դ(direction) -- û��λ�õ�����ָ�����򷢳�ƽ�й���
*     �۹��(spotlight) -- ���ֵ�Ͳ�Ĺ����ƣ���λ�ò��ҷ����Ĺ���ָ�������ϰ���Բ׶������
*
*   ����(Texture) -- ͨ����ͼ���ļ��м��أ��� width,height,miplevel,pixel format �����ԣ�ʹ����������(D3DFVF_TEX1)�ͼ������ϣ�
*     �����귶ΧΪ��0.0 �� 1.0
*     D3DXCreateTextureFromFile -- ��BMP��ͼƬ�ļ��м�����������
*     ��������
*     m_pD3dDevice->SetTexture( 0, m_pD3dTexture );
*     m_pD3dDevice->SetTextureStageState( 0, D3DTSS_XXXX, YYYY);
* 
*     
*   ���ʲ���(D3DMATERIAL9) -- ������ɫ�������巴������ĵƹ���ɫ������
*     Diffuse -- ���淴����������ɫ
*     Ambient -- ���淴��Ļ�������ɫ
*     Specular -- ���淴��ľ������ɫ
*     Emissive -- ���������������ɫ����ʹ�����忴�������������Լ������Ĺ�һ��
*     Power -- ָ�������ľ���߹⣻����ֵ�Ǹ߹������ֵ
*       �����ɫ����Ĳ��ʣ�ֻ����������������ɫ�����й⣩
*         D3DMATERIAL9 red;
*         ::ZeroMemory(&red, sizeof(red));
*         red.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f); // red
*         red.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f); // red
*         red.Specular = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f); // red
*         red.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f); // no emission
*         red.Power = 5.0f;
*     ���㷨��(Vertex normals)/�淨��(face normal) -- �������������,D3D��Ҫ֪�����ߵķ�����ȷ���ƹ����䵽�������ĽǶȡ�
*       ע�⣺
*       1.���㷨�߲�һ�����淨����ͬ(������--�кܶ�С�����)��
*       2.���������ͬ��Ķ��㷨�߲�һ����ͬ��������׶�Ķ����ڲ�ͬ�����в�ͬ�Ķ��㷨�ߣ�
* 
*   ���� -- ����ӳ�䵽��Ļʱ��������Ļ�����κ����������β�һ����ʱ����������Ρ����ˣ�Filtering��������������Щ���α��ƽ���ļ�����
*     D3D�ṩ�����ֹ�����(Ʒ��Խ����Խ��)��
*       Nearest point sampling(D3DTEXF_POINT) -- ȱʡ��������Ч�����ٶ����
*       Linear filtering( D3DTEXF_LINEAR)) -- Ч���Ϻã��Ƽ�ʹ�á�
*       Anisotropic filtering( D3DTEXF_ANISOTROPIC) -- Ч����ã�������ʱ��Ҳ����������� D3DSAMP_MAXANISOTROPY �ȼ���
*   ��� -- ͨ�����Ѿ���դ�������ع�դ��ͬһλ�õ����أ����ּ�������������ɶ�����Ч��alpha����ܹ�����������ͼԪ��͸���ȡ�
*       OutputPixel=SourcePixel x SourceBlendFactor + DestPixel x DestBlendFactor
*       Ĭ�Ϲرգ�ͨ������ D3DRS_ALPHABLENDENABLE ����
*     ��ʹ�û��ʱ��Ҫ��ѭ���¹������Ȳ�ʹ�û�ϻ������壬Ȼ�����������������ľ���ʹ�û�϶������ѡ��
*     ����z����ʹ�ôӺ�ǰ��˳���ϻ������塣
*   
*   ͸���� -- ����ɫ�е�Alpha����ָ����0��ʾ��ȫ͸����255ʱ��ʾ��ȫ��͸�������뽫Դ���Ҫ������Ϊ D3DBLEND_SRCALPHA��
*     Ŀ�Ļ��Ҫ��Ϊ D3DBLEND_INVSRCALPHA��
*   Alphaͨ�� -- ���������ж��������λ������ÿһ�����alphaֵ������ʹ��DirectX�����ߴ���(DxTex.exe, �����������ļ���չ��Ϊ dds )
*
*   ģ��(Stencile)���� -- һ��Զ����Ļ�Ļ��棬���������һЩ��Ч(��ʵ�־��ӵķ���Ч����ͨ�� D3DXMatrixReflect �����������)
*     �ڴ�����Ȼ����ͬʱ���Դ���ģ�建�棬���Ƿ���ͬһ���������滺�棬����ÿ�����ر�ָ�������Ի����ڴ�Ƭ���С�
*     �ܹ�ʹ��ģ�滺�棬ͨ��ģ���������ֹ��Ⱦ�󻺴��е�ĳЩ����
*     �� D3DFMT_D24S8 ��ʾ����һ��32λ���/ģ�滺�棬����24λΪ��Ȼ��棬8λΪģ�滺��
*
*   ��Ӱ -- ������Ӱ����ĺ��� D3DXMatrixShadow
*   ���� -- ����ʹ�� ID3DXFont(ʹ��GDI���������֣����Ը�ʽ��)��CD3DFont(ʹ�����������κ�Direct3D���ƣ�����)�� D3DXCreateText ������
* Direct3D ����
*   Application 
*     -> Direct3D 
*       -> HEL(Ӳ��ģ���, ��Ӳ����֧��˵Ҫ�������ʱ��ʹ�����ģ�⣬D3DDEVTYPE_REF��D3DDEVTYPE_SW ��)
*       -> HAL(Hardware Abstraction Layer��Ӳ������㣬���豸������ʵ��,D3DDEVTYPE_HAL) 
*         -> Graphics Device��
*   Direct3D ʹ�� REF�豸(D3DDEVTYPE_REF)ģ�������е�Direct3D API�������д��������Щ�Կ���֧�ֵ�Direct3D���ԵĴ��룬
*   ��ʵ��������Ҫ����豸�Ƿ�֧�����蹦�ܡ�
*   
*   �豸���� -- Direct3D֧�ֵ�ÿһ�����Զ���Ӧ��D3DCAPS9�ṹ��һ�����ݳ�Ա
* 
* ����
*   Multisampling(D3DMULTISAMPLE_TYPE) -- ȫ�������(ʹ��ʾͼ������ؾ�������ƽ��)
* 
*
* ��ʼ��Direct3D
*   1.���һ�� IDirect3D9 �ӿ�ָ��  -- Direct3DCreate9(D3D_SDK_VERSION);
*   2.����豸���������Ƿ�֧��Ӳ�����㴦�� -- IDirect3D9::GetDeviceCaps
*   3.��ʼ�� D3DPRESENT_PARAMETERS �ṹʵ�� -- 
*   4.���� IDirect3DDevice9 ���� -- IDirect3D9::CreateDevice
******************************************************************************************************/


//���ӷ��� -- ������Ⱦ����ͼԪ��ģ�滺�棬Ȼ����Ⱦ��Щ�ܱ���Ⱦ��������ķ�����
#if 0
void RenderMirror()
{
    //����ģ�滺���������Ⱦ״̬
    Device->SetRenderState(D3DRS_STENCILENABLE, true);
    Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);  //������ģ����Զ�ͨ��
    Device->SetRenderState(D3DRS_STENCILREF, 0x1);
    Device->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
    Device->SetRenderState(D3DRS_STENCILWRITEMASK,0xffffffff);

    //������Ȳ���ʧ���ˣ�ָ��D3DSTENCILOP_KEEP������������ģ�滺����� -- ������Ⱦ����ģ�����ķ�������
    Device->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

    //����ģ�����ʧ����
    Device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);

    //������Ȳ��Ժ�ģ����Զ�ͨ���ˣ�����ģ�滺����ڣ�����ģ��ο�ֵΪ0x1
    Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);

    //��ֹ��Ⱦ���Ӵ���
    // disable writes to the depth and back buffers
    Device->SetRenderState(D3DRS_ZWRITEENABLE, false);
    Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
    Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
    // draw the mirror to the stencil buffer
    Device->SetStreamSource(0, VB, 0, sizeof(Vertex));
    Device->SetFVF(Vertex::FVF);
    Device->SetMaterial(&MirrorMtrl);
    Device->SetTexture(0, MirrorTex);
    D3DXMATRIX I;
    D3DXMatrixIdentity(&I);
    Device->SetTransform(D3DTS_WORLD, &I);
    Device->DrawPrimitive(D3DPT_TRIANGLELIST, 18, 2);
    // re-enable depth writes
    Device->SetRenderState(D3DRS_ZWRITEENABLE, true);

    Device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
    Device->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

    //�����ڳ����з���λ�õľ���
    // position reflection
    D3DXMATRIX W, T, R;
    D3DXPLANE plane(0.0f, 0.0f, 1.0f, 0.0f); // xy plane
    D3DXMatrixReflect(&R, &plane);
    D3DXMatrixTranslation(&T,
        TeapotPosition.x,
        TeapotPosition.y,
        TeapotPosition.z);
    W = T * R;

    //�����Ȼ��� -- ��Ϊ������Ĳ������ȱȾ��ӵ���ȴ�������Ҫ��ֹ���ӵ�ͼԪ�ѱ���������ͼԪŪģ��
    Device->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);

    //��ϱ�����Ĳ���;���
    Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
    Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);

    //׼�����Ʊ�����Ĳ��
    Device->SetTransform(D3DTS_WORLD, &W);
    Device->SetMaterial(&TeapotMtrl);
    Device->SetTexture(0, 0);
    Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
    Teapot->DrawSubset(0);

    //�ı䱳���ѡģʽ
    Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
    Device->SetRenderState( D3DRS_STENCILENABLE, false);
    Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
} // end RenderMirror()


//������� -- ʹ���ĸ������������Right��Up��Look��Position������Ϊ���������һ������ϵ�����������������еĶ�Ӧ��ϵ��
//��������ܹ������������ַ�ʽ�仯(�ܹ������������ƶ��Լ�������������ת)��
//  Χ��right������ת��pitch��б��
//  Χ��up������ת��yaw ƫ����
//  Χ��look������ת��roll ��ת��
//  ����right����ƽ�ƣ�strafe��
//  ����up�������У�fly��
//  ����look�����ƶ���move��

class Camera
{
public:
    enum CameraType { LANDOBJECT, AIRCRAFT };
    Camera();
    Camera(CameraType cameraType);
    ~Camera();

    void strafe(float units); // left/right
    void fly(float units);    // up/down
    void walk(float units);   // forward/backward	
    void pitch(float angle); // rotate on right vector
    void yaw(float angle);   // rotate on up vector
    void roll(float angle);  // rotate on look vector

    void getViewMatrix(D3DXMATRIX* V); 
    void setCameraType(CameraType cameraType); 
    void getPosition(D3DXVECTOR3* pos); 
    void setPosition(D3DXVECTOR3* pos); 
    void getRight(D3DXVECTOR3* right);
    void getUp(D3DXVECTOR3* up);
    void getLook(D3DXVECTOR3* look);
private:
    //���������ģʽ��LANDOBJECTģʽ(������ĳЩ���ϵ��˶�)��AIRCRAFTģʽ(�ռ�����ȫ���ɵ��ƶ�)
    CameraType  _cameraType;
    D3DXVECTOR3 _right;
    D3DXVECTOR3 _up;
    D3DXVECTOR3 _look;
    D3DXVECTOR3 _pos;
};

#endif  //#if 0

#endif //DIRECT3D_STUDY_H