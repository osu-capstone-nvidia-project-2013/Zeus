////////////////////////////////////////////////////////////////////////////////
// Filename: objectclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _OBJECTCLASS_H_
#define _OBJECTCLASS_H_
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <string>

using std::string;

struct LIGHT
{
	D3DXVECTOR4 ambientcolor;
	D3DXVECTOR4 diffusecolor;
	D3DXVECTOR4 specularcolor;
	float specularpower;
	D3DXVECTOR3 lightdirection;
};

struct MATRICES
{
	D3DXMATRIX matWorld;
	D3DXMATRIX matView;
	D3DXMATRIX matProjection;
	D3DXVECTOR3 cameraPosition;
	float padding;
};

struct MAPPING
{
	float textureflag;
	float alphaflag;
	float normalflag;
	float particle;
	float reflective;
	float padding;
};
////////////////////////////////////////////////////////////////////////////////
// Class name: ObjectClass
////////////////////////////////////////////////////////////////////////////////
class ObjectClass
{
public:
	ObjectClass();
	~ObjectClass();

	void Render(ID3D11Device *dev, ID3D11DeviceContext *devcon, ID3D11RenderTargetView *backbuffer, IDXGISwapChain *swapchain, ID3D11ShaderResourceView *pTexture);

	string name;
	MATRICES *matrices;
	unsigned int numIndices;
	ID3D11Buffer *vBuffer;
	ID3D11Buffer *iBuffer;	
	LIGHT *light;
	MAPPING *mapping;

	ID3D11ShaderResourceView *texturemap;
	ID3D11ShaderResourceView *alphamap;
	ID3D11ShaderResourceView *normalmap;

	float x0, y0, z0;	// starting location	
	float vx0, vy0, vz0;	// starting velocity		
	float r0, g0, b0;	// starting color		
	float x, y, z;		// current location	
	float vx, vy, vz;	// current velocity		
	float r, g, b;		// current color
};

#endif