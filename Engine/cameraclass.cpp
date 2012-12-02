#include "cameraclass.h"
#include "main.h"
CameraClass::CameraClass()
{	
	m_epeen = D3DXVECTOR3(0.0f, 1.0f, 5.5f);
	m_at = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(&matView,
                        &m_epeen,    // the camera position
                        &m_at,		 // the look-at position
                        &m_up);		 // the up direction

	// create a projection matrix
	D3DXMatrixPerspectiveFovLH(&matProjection,
								(FLOAT)D3DXToRadian(45),                    // field of view
								(FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
								1.0f,                                       // near view-plane
								100.0f);                                    // far view-plane
}

CameraClass::CameraClass(D3DXVECTOR3 up,D3DXVECTOR3 epeen,D3DXVECTOR3 at, float fieldOfView, float width, float height, float nearVP, float farVP)
{
	m_epeen = epeen;
	m_at = at;
	m_up = up;

	D3DXMatrixLookAtLH(&matView,
                        &m_epeen,    // the camera position
                        &m_at,		 // the look-at position
                        &m_up);		 // the up direction

	// create a projection matrix
	D3DXMatrixPerspectiveFovLH(&matProjection,
								(FLOAT)D3DXToRadian(fieldOfView),                    // field of view
								(FLOAT)width / (FLOAT)height, // aspect ratio
								nearVP,                                       // near view-plane
								farVP);                                    // far view-plane

}
