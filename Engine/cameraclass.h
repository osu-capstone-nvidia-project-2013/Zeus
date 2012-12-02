#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <vector>

class CameraClass
{
public:

	CameraClass();
	CameraClass(D3DXVECTOR3 up,D3DXVECTOR3 epeen,D3DXVECTOR3 at, float fieldOfView,
				float width, float height, float nearVP, float farVP);

	~CameraClass();


	D3DXVECTOR3 m_up, m_epeen, m_at;
	D3DXMATRIX matView, matProjection;
};

#endif