#include "objectclass.h"
#include "geometryclass.h"

GeometryClass::GeometryClass()
{
}

void SetTriangle(ObjectClass *obj, ID3D11Device *dev, ID3D11DeviceContext *devcon)
{
    // create a triangle using the VERTEX struct
    VERTEX OurVertices[] =
    {
        {D3DXVECTOR3(0.0f, 0.2f, 0.0f),D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.1f)},
        {D3DXVECTOR3(0.1f, 0.0f, 0.0f),D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.1f)},
		{D3DXVECTOR3(-0.1f, 0.0f, 0.0f),D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.1f)}
    };

	vector<int> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	obj->numIndices = indices.size();

    // create the vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bd.ByteWidth = sizeof(VERTEX) * 3;             // size is the VERTEX struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	dev->CreateBuffer(&bd, NULL, &obj->vBuffer);       // create the buffer


    // copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    devcon->Map(obj->vBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, OurVertices, sizeof(OurVertices));                 // copy the data
    devcon->Unmap(obj->vBuffer, NULL);    

	// create the index buffer
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bd.ByteWidth = sizeof(indices);             // size is the VERTEX struct * 3
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;       // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	dev->CreateBuffer(&bd, NULL, &obj->iBuffer);       // create the buffer

    // copy the vertices into the buffer
    devcon->Map(obj->iBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, &indices[0], sizeof(OurVertices));                 // copy the data
    devcon->Unmap(obj->iBuffer, NULL);    

}