#ifndef DIRECT3D_INTERFACE_H
#define DIRECT3D_INTERFACE_H
#pragma once

/*****************************************************************************************************
* ����Direct3D�Ľӿ�
*   IDirect3D9 -- ������Ľӿڣ����ڻ�������豸����Ϣ�ʹ��� IDirect3DDevice9 �Ƚӿڡ�
*     CheckDeviceMultisampleType -- ����Ƿ�֧��ȫ�������
*     GetDeviceCaps -- ����豸������(�� D3DCAPS9::DevCaps &  D3DDEVCAPS_HWTRANSFORMANDLIGHT 
*       -- �ж��Ƿ�֧��Ӳ�����㴦��[Ӳ�����α任�͹�Դ����])
*   IDirect3DDevice9 -- ������ʾ3Dͼ�ε������豸
*     BeginScene/EndScene -- ��ʼ�ͽ������Ƴ���(���л��Ʒ���������������������֮��)
*     Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0) -- ��պ󱸱�������/ģ�滺��
*     CreateIndexBuffer -- ������������
*     CreateVertexBuffer -- �������㻺��
*     DrawPrimitive/DrawIndexedPrimitive --����ͼԪ(�㡢�ߡ������ε�)��ͨ�����ͼ�����Ϣ������Ⱦ����ʹ�ö���/����������л��ơ�
*     LightEnable -- �򿪻�رյƹ�
*     Present(0, 0, 0, 0) -- ���к󱸱���Ľ���
*     SetFVF -- ���ö����ʽ����Ҫ�����Զ���Ķ���ṹ�����ã�����  D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1
*     SetIndices -- �����������棬ÿ��ֻ��ʹ��һ��
*     SetLight -- ע��ƹ�
*     SetRenderState -- ������Ⱦ״̬(�� Cull���ƹ⡢�����⡢��Ȼ��塢͸�������)��ֱ����һ�θı���Ⱦ״̬Ϊֹ
*       ��ʹ�ÿ�ܽ�������ģʽ,SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME)
*     SetStreamSource -- ������Դ����һ�����㻺��ҹ�����������һ��������Ⱦ���ߵļ�����Ϣ������
*     SetTexture -- �������������ð˸����������ܹ����������������ϸ�ڵ�ͼ��(��������)
*     SetTextureStageState -- ���������״̬(����ʹ��Alphaͨ��)
*     SetTranform -- ���ñ任����
*     SetViewport -- �����ӿ�(����ģʽ����ʱ����ʾ���ֿ���ֻռ��Ļ��һ���֣��ɴ�������ϵ����)
*     SetTexture -- ��������
*     SetSamplerState -- �������������
*   ID3DXBaseMesh -- ����һ�������洢���񶥵�Ķ��㻺���һ������������Щ��������������һ��������������ε��������档
*     DrawSubset -- ����ͼ��(������Ӽ�)
*     GetAttributeTable/SetAttributeTable -- ��ȡ/���� ���Ա�
*     GetFVF -- ��ö���ĸ�ʽ
*     GetNumVertices -- ���ض��㻺���еĶ�����
*     GetNumBytesPerVertex -- ����һ��������ռ���ֽ���
*     GetNumFaces -- ������mesh�е��棨�����Σ���
*     GetVertexBuffer/GetIndexBuffer -- ��ö���/��������
*     LockVertexBuffer/LockIndexBuffer -- ��������/�������棬���ڶ�д����
*     LockAttributeBuffer/UnlockAttributeBuffer -- ����/�������Ի���
*     OptimizeInplace/Optimize -- ��Mesh�Ķ�������������������Ż���
*   ID3DXBuffer -- �����ݴ洢���ڽ��ڴ���У�����ʹ�� D3DXCreateBuffer ��������
*     GetBufferPointer -- ����һ��ָ��ʼ���ݵ�ָ��
*     GetBufferSize -- �����ڻ����е��ֽڴ�С

*   ID3DXFont -- ʹ��GDI����������(�ܹ������������͸�ʽ������)��ͨ�� D3DXCreateFontIndirect��D3DXCreateFont �Ⱥ���������
*     DrawText -- 
*   ID3DXMesh -- ��������
*   ID3DXPMesh -- ����(Progressive)������������ͨ���򻯱�����ת����edge collapse transformations��ECT������mesh��
*     ÿִ��һ��ECT���Ƴ�һ�������һ��2���棬�ù��̿���(�������)�����Կ��Ʋ�ͬ�ľ�ϸ����(��Զ��ʱ�;��ȣ������߾���)��
*     ʹ�� D3DXGeneratePMesh �������������ԭʼmesh������
*     GetMaxFaces/GetMinFaces -- ���ؽ��������ܹ������õ� ���/��С ����
*     GetMaxVertices/GetMinVertices -- ���ؽ��������ܹ������õ� ���/��С ������
*     SetNumFaces/SetNumVertices -- ������������ ��/���� �ĸ������Ա���mesh�򻯻��ӻ���ע����������/���������ܲ������趨������
*     TrimByFaces/TrimByVertices -- �����µ���С�������/��������ע����ֵ������[GetMinFaces��GetMaxFaces]��֮��
*   IDirect3DSurface9 -- ����(���ص���)����Ҫ�����洢2Dͼ������
*     LockRect/UnlockRect -- ���/�ͷ�ָ������ڴ��ָ�룬����һ�����ص���ж���д����
*     GetDesc -- ͨ�����D3DSURFACE_DESC�ṹ�����ر����������Ϣ
*   IDirect3DSwapChain -- ��������DX9ʱ���Ը��Ĵ���(�����ܲʹ���ڴ��)��DX10�н������ڴ���ʱ�Ͱ󶨵�����
*   IDirect3DTexture9 -- ����ͨ���� D3DXCreateTextureFromFile(BMP,DDS,DIB,JPG,PNG,TGA) �Ⱥ���������������
*   IDirect3DVertexBuffer9/IDirect3DIndexBuffer9 -- ���㻺��/��������
*     Lock -- ��÷��ʻ��������ָ�룬ʹ����Ϻ������ Unlock ����
*     GetDesc -- ���������Ϣ(ͨ��CreateVertexBuffer/CreateIndexBuffer����ʱ�������Ϣ)
* 
*****************************************************************************************************/



#endif //DIRECT3D_INTERFACE_H