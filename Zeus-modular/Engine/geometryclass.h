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
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

struct VERTEX
{
	D3DXVECTOR3 position;
    D3DXVECTOR4 color;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 texcord;
};


void SetTriangle(ObjectClass *, ID3D11Device *, ID3D11DeviceContext *);

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
    void CreateSphere(ID3D11Device *, ID3D11DeviceContext *, VERTEX, float, int, int);
	void SetMatrices(MATRICES *mats, int objNum);
	void SetLight(LIGHT *light, int objNum);
    void Render(ID3D11Device *dev, ID3D11DeviceContext *devcon, ID3D11RenderTargetView *backbuffer, 
				IDXGISwapChain *swapchain, ID3D11Buffer *pCBuffer, ID3D11Buffer *vCBuffer,
				ID3D11DepthStencilView *zbuffer, ID3D11ShaderResourceView *pTexture);
	
private:
    vector<ObjectClass*> objects;
    void CreateObject(ID3D11Device *, ID3D11DeviceContext *, vector<VERTEX>, vector<int>);
};

#endif