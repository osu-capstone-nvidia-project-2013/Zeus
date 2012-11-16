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

    vector<VERTEX> vertices;
    VERTEX vert1 = {D3DXVECTOR3(0.0f, 0.2f, 0.0f),D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.1f)};
    VERTEX vert2 = {D3DXVECTOR3(0.1f, 0.0f, 0.0f),D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.1f)};
    VERTEX vert3 = {D3DXVECTOR3(-0.1f, 0.0f, 0.0f),D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 0.1f)};
    vertices.push_back( vert1);
    vertices.push_back( vert2);
    vertices.push_back( vert3);
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
    memcpy(ms.pData, &vertices[0], sizeof(VERTEX) * vertices.size());                 // copy the data
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
    memcpy(ms.pData, &indices[0], sizeof(indices));                 // copy the data
    devcon->Unmap(obj->iBuffer, NULL);    

}

void GeometryClass::Initialize() 
{

}

void GeometryClass::LoadObject(ID3D11Device *dev, ID3D11DeviceContext *devcon, string obj_name)
{
    ifstream stream (obj_name);
    if(!stream)
    {
        return;
    }

    vector<VERTEX> vertices;
    vector<int> indices;
    char line[100];
    float x = 0, y = 0, z = 0;
    vector<D3DXVECTOR3> verts;
    vector<D3DXVECTOR3> norms;
    while( !stream.eof() ) {
        stream >> line;

        if( strcmp( line, "#" ) == 0 ) {
            // Read in the comment and do nothing
            stream.getline( line, 100);
        } else {
            if( strcmp( line, "f" ) == 0 ) {
                for(int i = 0; i < 3; i++) {
                    stream >> line;
                    indices.push_back( atoi( strtok( line, "/" ) ) - 1 );
                }
            } else if (strcmp( line, "v" ) == 0 ) {
                stream >> x >> y >> z;
                verts.push_back( D3DXVECTOR3( x, y, z ) );
            } else if (strcmp( line, "vn" ) == 0 ) {
                stream >> x >> y >> z;
                norms.push_back( D3DXVECTOR3( x, y, z) );
            }
        }
    }

    for(int i = 0; i < verts.size(); i++) {
        VERTEX vertex;
        vertex.position = verts[i];
        vertex.normal = norms[i];
        vertex.color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
        vertices.push_back(vertex);
    }

	CreateObject(dev, devcon, vertices, indices);
}

void GeometryClass::CreateSphere(ID3D11Device *dev, ID3D11DeviceContext *devcon, VERTEX center_point, float radius, int slices, int stacks) 
{
    float theta, phi;
	int i, j, u;

    vector<VERTEX> vertices;
    vector<int> indices;

    for(j=1; j < stacks - 1; j++ ) {
		for( i = 0; i < slices; i++) {
			theta = float(j) / (stacks - 1) * D3DX_PI;
			phi = float(i) / (slices - 1) * D3DX_PI * 2;
			VERTEX point;
			point.position.x = (radius) * (sinf(theta) * cosf(phi)) + center_point.position.x;
			point.position.y = (radius) * (cosf(theta)) + center_point.position.y;
			point.position.z = (radius) * (-sinf(theta) * sinf(phi)) + center_point.position.z;
			point.color = center_point.color;
			point.normal = point.position;
			vertices.push_back(point);
		}
	}
	int top_ind = vertices.size();
    VERTEX top;
    top.position = center_point.position;
    top.position.y += radius;
	top.color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
    top.normal = top.position;
    vertices.push_back(top);

    int bot_ind = vertices.size();
    VERTEX bottom;
    bottom.position = center_point.position;
    bottom.position.y -= radius;
    bottom.color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
    bottom.normal = bottom.position;
    vertices.push_back(bottom);

	for(i = 0; i < slices - 1; i++) {
		// Make top
		indices.push_back(top_ind);
		indices.push_back(i);
		indices.push_back(i + 1);
		// Make bottom
		indices.push_back(bot_ind);
		indices.push_back((top_ind - 1) - i);
		indices.push_back((top_ind - 1) - (i + 1));
	}
	// Make inbetween
	for(u = 0, j = 0; j < (stacks - 3); j++) {
		for( i = 0; i < (slices - 1); i++) {
			indices.push_back( (j) * slices + i);
			indices.push_back( (j + 1) * slices + i + 1);
			indices.push_back( (j) * slices + i + 1 );
			indices.push_back( (j) * slices + i );
			indices.push_back( (j + 1) * slices + i);
			indices.push_back( (j + 1) * slices + i + 1);
	    }
	}

    
    CreateObject(dev, devcon, vertices, indices);

	return;
}

// Creates the ObjectClass and pushes it into the objects vector.
void GeometryClass::CreateObject(ID3D11Device *dev, ID3D11DeviceContext *devcon, vector<VERTEX> vertices, vector<int> indices) 
{
    ObjectClass *obj = new ObjectClass();
    obj->numIndices = indices.size();
    

    // create the vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bd.ByteWidth = sizeof(VERTEX) * vertices.size();             // size is the vertices
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	dev->CreateBuffer(&bd, NULL, &obj->vBuffer);       // create the buffer

    // copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    devcon->Map(obj->vBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, &vertices[0], (sizeof(VERTEX) * vertices.size()));                 // copy the data
    devcon->Unmap(obj->vBuffer, NULL);    

	// create the index buffer
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bd.ByteWidth = sizeof(int) * indices.size();             // size is the indices
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;       // use as an index buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	dev->CreateBuffer(&bd, NULL, &obj->iBuffer);       // create the buffer

    // copy the vertices into the buffer
    devcon->Map(obj->iBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, &indices[0], sizeof(int) * indices.size());                 // copy the data
    devcon->Unmap(obj->iBuffer, NULL);

    objects.push_back(obj);
    return;
}

void GeometryClass::SetMatrix(D3DXMATRIX mat, int objNum)
{
	objects[objNum]->transMat = mat;
}

void GeometryClass::Render(ID3D11Device *dev, ID3D11DeviceContext *devcon, ID3D11RenderTargetView *backbuffer, IDXGISwapChain *swapchain, ID3D11Buffer *pCBuffer)
{

    // clear the back buffer to a deep blue
    devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

    // Draw it all
    for(int i = 0; i < objects.size(); i++ )
    {

		// update constant buffer
		devcon->UpdateSubresource(pCBuffer, 0, 0, objects[i]->transMat, 0, 0);	

        objects[i]->Render(dev, devcon, backbuffer, swapchain);
    }
	
    // switch the back buffer and the front buffer
    swapchain->Present(0, 0);
}