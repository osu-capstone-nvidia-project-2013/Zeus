// Author: Zander Clucas
// 2013 Capstone Project
// geometry.h
// Encapsulates the geometry for 3d models
#ifndef ZEUS_GEOMETRY_H
#define ZEUS_GEOMETRY_H

//Includes
#include <D3D10.h>
#include <D3DX10math.h>

class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR4 color;
		D3DXVECTOR3 normal;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D10Device*);
	void Shutdown();
	void Render(ID3D10Device*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D10Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D10Device*);
	void DrawCircle(VertexType, float, int, VertexType*&, unsigned long*&);
	void HSVtoRGB(float[3], float[3]);

private:
	ID3D10Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
};

#endif