////////////////////////////////////////////////////////////////////////////////
// Filename: geometryclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GEOMETRYCLASS_H_
#define _GEOMETRYCLASS_H_
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <D3DX10math.h>
#include "objectclass.h"
#include "cameraclass.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct VERTEX
{
	D3DXVECTOR3 position;
    D3DXVECTOR4 color;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 texcord;
	D3DXVECTOR3 tangent;
};

////////////////////////////////////////////////////////////////////////////////
// Class name: GeometryClass
////////////////////////////////////////////////////////////////////////////////
class GeometryClass
{
public:


	GeometryClass();
	~GeometryClass();

	void Initialize();
	void LoadObject(ID3D11Device *, ID3D11DeviceContext *, string, D3DXVECTOR4);
	void CreatePlane(ID3D11Device *, ID3D11DeviceContext *, float y);
	void CreatePlane(ID3D11Device *, ID3D11DeviceContext *, float y, string name);
	void CreateQuad(ID3D11Device *, ID3D11DeviceContext *, float r, float g, float b, float size);
	void CreateQuad(ID3D11Device *, ID3D11DeviceContext *, float r, float g, float b, float size, string name);
    void CreateSphere(ID3D11Device *, ID3D11DeviceContext *, VERTEX, float, int, int);
    void CreateSphere(ID3D11Device *, ID3D11DeviceContext *, VERTEX, float, int, int, string name);
	void SetMatrices(MATRICES *mats, int objNum);
	void SetLight(LIGHT *light, int objNum);
    void SetAlpha(ID3D11Device *dev, LPCWSTR alphafile, int objNum);
    void SetTexture(ID3D11Device *dev, LPCWSTR texturefile, int objNum);
	void SetNormal(ID3D11Device *dev, LPCWSTR normalfile, int objNum);
	void SetReflect(ID3D11Device *dev, LPCWSTR reflectfile, int objNum);
    void SetMapping(float texture, float alpha, float normal, float reflect, float particle, int objNum);
    void Render(ID3D11Device *dev, ID3D11DeviceContext *devcon, ID3D11RenderTargetView *backbuffer, 
				IDXGISwapChain *swapchain, ID3D11Buffer *pCBuffer, ID3D11Buffer *vCBuffer,
				ID3D11Buffer *mCBuffer,
				ID3D11DepthStencilView *zbuffer, ID3D11ShaderResourceView *pTexture,
				ID3D11BlendState *pBS,ID3D11SamplerState *pSS, ID3D11RasterizerState *pRS, CameraClass *camera);
	void InitRenderTarget(ID3D11Device *dev, ID3D11DeviceContext *devcon, CameraClass *camera,ID3D11DepthStencilView *zbuffer);
	void RenderToTarget(ID3D11Device *dev, ID3D11DeviceContext *devcon, CameraClass *camera,ID3D11DepthStencilView *zbuffer,ID3D11Buffer *pCBuffer, ID3D11Buffer *vCBuffer,
									ID3D11Buffer *mCBuffer,ID3D11RenderTargetView *backbuffer, 
									IDXGISwapChain *swapchain);
	void Initblah(ID3D11Device *dev, ID3D11DeviceContext *devcon,ID3D11DepthStencilView *zbuffer,ID3D11RenderTargetView *backbuffer,IDXGISwapChain *swapchain);
	
	// Environment Map Pass 1:
	void CreateEnvironmentMapRenderTarget(ID3D11Device *dev);
	void setEnviroRenderTarget(ID3D11DeviceContext *devcon,ID3D11DepthStencilView *zbuffer);
	void ClearEnvRenderTargetDepthBuffer(ID3D11DeviceContext *devcon,ID3D11DepthStencilView *zbuffer);
	void setViewToReflection(CameraClass *camera, int objnum);
	void DrawEnvironment(ID3D11Device *dev, ID3D11DeviceContext *devcon,ID3D11RenderTargetView *backbuffer, 
									IDXGISwapChain *swapchain,ID3D11Buffer *pCBuffer, ID3D11Buffer *vCBuffer,
									ID3D11Buffer *mCBuffer);

	// Default Pass 2:
	void SetBackBufferasTarget(ID3D11DeviceContext *devcon,ID3D11DepthStencilView *zbuffer, ID3D11RenderTargetView *backbuffer);
	void ClearBackbuffRenderTargetDepthbuffer(ID3D11DeviceContext *devcon,ID3D11DepthStencilView *zbuffer, ID3D11RenderTargetView *backbuffer);
	void SetViewToCamera(CameraClass *camera);
	void SetTextureToObject(ID3D11DeviceContext *devcon);
	void DrawScene(ID3D11Device *dev, ID3D11DeviceContext *devcon,ID3D11RenderTargetView *backbuffer, 
									IDXGISwapChain *swapchain,ID3D11Buffer *pCBuffer, ID3D11Buffer *vCBuffer,
									ID3D11Buffer *mCBuffer);

	vector<ObjectClass*> objects;
	vector<ObjectClass*> sorted_objects;

	ID3D11Texture2D* renderTargetTextureMap;
	ID3D11RenderTargetView* renderTargetViewMap;
	ID3D11ShaderResourceView* shaderResourceViewMap;


private:
    void quicksort(int left, int right);
	int partition(int left, int right, int pivotIndex);
	float objdist(int index);

	void CopyObject( ObjectClass *, ObjectClass* );
    void CreateObject(ID3D11Device *, ID3D11DeviceContext *, vector<VERTEX>, vector<int>);
    void CreateObject(ID3D11Device *, ID3D11DeviceContext *, vector<VERTEX>, vector<int>, int, string);

	MATRICES *tempMat;
	ID3D11DepthStencilView *envzbuffer;
};

#endif