// Author: Zander Clucas
// 2013 Capstone Project
// camera.h
// Keeps track of the 'camera' of the world
#ifndef ZEUS_CAMERA_H_
#define ZEUS_CAMERA_H_

#include <d3dx10math.h>

// Keeps track of the camera and controls it
class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void Render();
	void GetViewMatrix(D3DXMATRIX&);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	D3DXMATRIX m_viewMatrix;
};

#endif