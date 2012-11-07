////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>
#include <vector>
using namespace std;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////



////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR4 color;
		D3DXVECTOR3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	void TransformObject(int, D3DXMATRIX);


	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();


private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	void DrawTriangle(VertexType, VertexType, VertexType);
    void DrawQuad(VertexType, VertexType);
	void DrawCircle(VertexType, float, int);
	void DrawSphere(VertexType, float, int, int);
    
	void HSVtoRGB(float[3], float[3]);

	bool LoadModel(char*);
	void ReleaseModel();

private:
	vector<VertexType> vertices_;
	vector<unsigned long> indices_;
    vector<unsigned long> objects_;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	ModelType* m_model;
};

#endif