////////////////////////////////////////////////////////////////////////////////
// Filename: geometryclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GEOMETRYCLASS_H_
#define _GEOMETRYCLASS_H_

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include "objectclass.h"
#include <vector>

using namespace std;
// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

struct VERTEX
{
	D3DXVECTOR3 position;
    D3DXVECTOR4 color;
	D3DXVECTOR3 normal;
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
	void LoadObject();
	
};

#endif