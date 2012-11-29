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

void GeometryClass::CreatePlane(ID3D11Device *dev, ID3D11DeviceContext *devcon, float y)
{
	vector<VERTEX> vertices;
    vector<int> indices;

	D3DXCOLOR ground = D3DXCOLOR(0.8f,0.8f,0.8f,1.0f);
    
	VERTEX vert1 = {D3DXVECTOR3(-30.0f, y, -30.0f),ground, D3DXVECTOR3(0.0f, 1.0f, 0.0f)};
    VERTEX vert2 = {D3DXVECTOR3(-30.0f, y, 30.0f),ground, D3DXVECTOR3(0.0f, 1.0f, 0.0f)};
    VERTEX vert3 = {D3DXVECTOR3(30.0f, y, 30.0f),ground, D3DXVECTOR3(0.0f, 1.0f, 0.0f)};
	VERTEX vert4 = {D3DXVECTOR3(30.0f, y, -30.0f),ground, D3DXVECTOR3(0.0f, 1.0f, 0.0f)};
    vertices.push_back( vert1);
    vertices.push_back( vert2);
    vertices.push_back( vert3);
    vertices.push_back( vert4);
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);

	CreateObject(dev, devcon, vertices, indices);
}

void GeometryClass::CreateQuad(ID3D11Device *dev, ID3D11DeviceContext *devcon, float r, float g, float b, float size)
{
	vector<VERTEX> vertices;
    vector<int> indices;

	D3DXCOLOR color = D3DXCOLOR(r,g,b,1.0f);
    
	VERTEX vert1 = {D3DXVECTOR3(-1.0f, -1.0f, 0.0f),color, D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.99f,0.99f)};
    VERTEX vert2 = {D3DXVECTOR3(-1.0f, 1.0f, 0.0f),color, D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.99f,0.01f)};
    VERTEX vert3 = {D3DXVECTOR3(1.0f, 1.0f, 0.0f),color, D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.01f,0.01f)};
	VERTEX vert4 = {D3DXVECTOR3(1.0f, -1.0f, 0.0f),color, D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.01f,0.99f)};
	vert1.tangent = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vert2.tangent = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vert3.tangent = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	vert4.tangent = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
    vertices.push_back( vert1);
    vertices.push_back( vert2);
    vertices.push_back( vert3);
    vertices.push_back( vert4);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(2);

	CreateObject(dev, devcon, vertices, indices);
}

void GeometryClass::LoadObject(ID3D11Device *dev, ID3D11DeviceContext *devcon, string obj_name,D3DXVECTOR4 color)
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
        vertex.color = color;
		vertex.texcord = D3DXVECTOR2((norms[i].x/2.0f) + 0.5f, (norms[i].y)/2 + 0.5f);
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
			point.tangent = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			float texcoordx = fmodf(float(i) / ((float)slices), 1.0f);
			float texcoordy = fmodf(float(j) / ((float)stacks), 1.0f);
			point.texcord = D3DXVECTOR2(texcoordx, texcoordy);
			vertices.push_back(point);
		}
	}
	int top_ind = vertices.size();
    VERTEX top;
    top.position = center_point.position;
    top.position.y += radius;
	top.color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
    top.normal = top.position;
	top.tangent = D3DXVECTOR3(-top.normal.y, top.normal.x ,top.normal.z );
	top.texcord = D3DXVECTOR2(fmodf(theta/(2.0f * D3DX_PI), 1.0f), fmodf(phi/D3DX_PI, 1.0f));
    vertices.push_back(top);
	
    int bot_ind = vertices.size();
    VERTEX bottom;
    bottom.position = center_point.position;
    bottom.position.y -= radius;
    bottom.color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
    bottom.normal = bottom.position;
	bottom.tangent = D3DXVECTOR3(-bottom.normal.y, bottom.normal.x ,bottom.normal.z );
	bottom.texcord = D3DXVECTOR2(fmodf(theta/(2.0f * D3DX_PI), 1.0f), fmodf(phi/D3DX_PI, 1.0f));
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

			// Determine surface orientation by calculating triangles edges
			D3DXVECTOR3 edge1 = vertices[(j + 1) * slices + i + 1].position - vertices[(j) * slices + i].position;
			D3DXVECTOR3 edge2 = vertices[ (j) * slices + i + 1 ].position - vertices[(j) * slices + i].position;
			D3DXVec3Normalize(&edge1, &edge1);
			D3DXVec3Normalize(&edge2, &edge2);

			// Do the same in texture space
			D3DXVECTOR2 texEdge1 = vertices[(j + 1) * slices + i + 1].texcord - vertices[(j) * slices + i].texcord;
			D3DXVECTOR2 texEdge2 = vertices[ (j) * slices + i + 1 ].texcord - vertices[(j) * slices + i].texcord;
			D3DXVec2Normalize(&texEdge1, &texEdge1);
			D3DXVec2Normalize(&texEdge2, &texEdge2);

			// A determinant returns the orientation of the surface
			float det = (texEdge1.x * texEdge2.y) - (texEdge1.y * texEdge2.x);
			det = 1.0f / det;

			// Averages out tangents for each face
			vertices[(j) * slices + i].tangent.x += (texEdge2.y * edge1.x - texEdge1.y * edge2.x) * det;
			vertices[(j) * slices + i].tangent.y += (texEdge2.y * edge1.y - texEdge1.y * edge2.y) * det;
			vertices[(j) * slices + i].tangent.z += (texEdge2.y * edge1.z - texEdge1.y * edge2.z) * det;
			D3DXVec3Normalize(&vertices[(j) * slices + i].tangent, &vertices[(j) * slices + i].tangent);

			vertices[(j + 1) * slices + i + 1].tangent.x += (texEdge2.y * edge1.x - texEdge1.y * edge2.x) * det;
			vertices[(j + 1) * slices + i + 1].tangent.y += (texEdge2.y * edge1.y - texEdge1.y * edge2.y) * det;
			vertices[(j + 1) * slices + i + 1].tangent.z += (texEdge2.y * edge1.z - texEdge1.y * edge2.z) * det;
			D3DXVec3Normalize(&vertices[(j + 1) * slices + i + 1].tangent, &vertices[(j + 1) * slices + i + 1].tangent);

			vertices[(j) * slices + i + 1].tangent.x += (texEdge2.y * edge1.x - texEdge1.y * edge2.x) * det;
			vertices[(j) * slices + i + 1].tangent.y += (texEdge2.y * edge1.y - texEdge1.y * edge2.y) * det;
			vertices[(j) * slices + i + 1].tangent.z += (texEdge2.y * edge1.z - texEdge1.y * edge2.z) * det;
			D3DXVec3Normalize(&vertices[(j) * slices + i + 1].tangent, &vertices[(j) * slices + i + 1].tangent);

			indices.push_back( (j) * slices + i);
			indices.push_back( (j + 1) * slices + i + 1);
			indices.push_back( (j) * slices + i + 1 );

			// Determine surface orientation by calculating triangles edges
			D3DXVECTOR3 edge3 = vertices[(j + 1) * slices + i].position - vertices[(j) * slices + i].position;
			D3DXVECTOR3 edge4 = vertices[ (j + 1) * slices + i + 1 ].position - vertices[(j) * slices + i].position;
			D3DXVec3Normalize(&edge3, &edge3);
			D3DXVec3Normalize(&edge4, &edge4);

			// Do the same in texture space
			D3DXVECTOR2 texEdge3 = vertices[(j + 1) * slices + i].texcord - vertices[(j) * slices + i].texcord;
			D3DXVECTOR2 texEdge4 = vertices[(j + 1) * slices + i + 1 ].texcord - vertices[(j) * slices + i].texcord;
			D3DXVec2Normalize(&texEdge3, &texEdge3);
			D3DXVec2Normalize(&texEdge4, &texEdge4);

			// A determinant returns the orientation of the surface
			det = (texEdge3.x * texEdge4.y) - (texEdge3.y * texEdge4.x);
			det = 1.0f / det;

			vertices[(j) * slices + i].tangent.x += (texEdge4.y * edge3.x - texEdge3.y * edge4.x) * det;
			vertices[(j) * slices + i].tangent.y += (texEdge4.y * edge3.y - texEdge3.y * edge4.y) * det;
			vertices[(j) * slices + i].tangent.z += (texEdge4.y * edge3.z - texEdge3.y * edge4.z) * det;
			D3DXVec3Normalize(&vertices[(j) * slices + i].tangent, &vertices[(j) * slices + i].tangent);

			vertices[(j + 1) * slices + i].tangent.x += (texEdge4.y * edge3.x - texEdge3.y * edge4.x) * det;
			vertices[(j + 1) * slices + i].tangent.y += (texEdge4.y * edge3.y - texEdge3.y * edge4.y) * det;
			vertices[(j + 1) * slices + i].tangent.z += (texEdge4.y * edge3.z - texEdge3.y * edge4.z) * det;
			D3DXVec3Normalize(&vertices[(j + 1) * slices + i].tangent, &vertices[(j + 1) * slices + i].tangent);

			vertices[(j + 1) * slices + i + 1].tangent.x += (texEdge4.y * edge3.x - texEdge3.y * edge4.x) * det;
			vertices[(j + 1) * slices + i + 1].tangent.y += (texEdge4.y * edge3.y - texEdge3.y * edge4.y) * det;
			vertices[(j + 1) * slices + i + 1].tangent.z += (texEdge4.y * edge3.z - texEdge3.y * edge4.z) * det;
			D3DXVec3Normalize(&vertices[(j) * slices + i + 1].tangent, &vertices[(j + 1) * slices + i + 1].tangent);

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
	obj->matrices = new MATRICES();
	obj->light = new LIGHT();
	obj->mapping = new MAPPING();
	obj->mapping->textureflag = 0.;
	obj->mapping->alphaflag = 0.;
	obj->mapping->normalflag = 0.;
	obj->texturemap = NULL;
	obj->alphamap = NULL;
	obj->normalmap = NULL;
	
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

void GeometryClass::SetMatrices(MATRICES *mats, int objNum)
{
	D3DXMatrixTranspose(&objects[objNum]->matrices->matProjection, &mats->matProjection);
	D3DXMatrixTranspose(&objects[objNum]->matrices->matView, &mats->matView);
	D3DXMatrixTranspose(&objects[objNum]->matrices->matWorld, &mats->matWorld);
	objects[objNum]->matrices->cameraPosition = mats->cameraPosition;
}


void GeometryClass::SetLight(LIGHT *light, int objNum)
{
	objects[objNum]->light = light;
}

void GeometryClass::SetMapping(float texture, float alpha, float normal, int objNum) 
{
	objects[objNum]->mapping->textureflag = texture;
	objects[objNum]->mapping->alphaflag = alpha;
	objects[objNum]->mapping->normalflag = normal;
}

void GeometryClass::SetAlpha(ID3D11Device *dev, LPCWSTR alphafile, int objNum) 
{
    D3DX11CreateShaderResourceViewFromFile(dev,                             // the Direct3D device
                                           alphafile,                       // load the alphamap in the local folder
                                           NULL,                            // no additional information
                                           NULL,                            // no multithreading
                                           &objects[objNum]->alphamap,      // address of the shader-resource-view
                                           NULL);                           // no multithreading
}

void GeometryClass::SetTexture(ID3D11Device *dev, LPCWSTR texturefile, int objNum) 
{
    D3DX11CreateShaderResourceViewFromFile(dev,                             // the Direct3D device
                                           texturefile,                     // load the alphamap in the local folder
                                           NULL,                            // no additional information
                                           NULL,                            // no multithreading
                                           &objects[objNum]->texturemap,    // address of the shader-resource-view
                                           NULL);                           // no multithreading
}

void GeometryClass::SetNormal(ID3D11Device *dev, LPCWSTR normalfile, int objNum) 
{
    D3DX11CreateShaderResourceViewFromFile(dev,                             // the Direct3D device
                                           normalfile,                     // load the alphamap in the local folder
                                           NULL,                            // no additional information
                                           NULL,                            // no multithreading
                                           &objects[objNum]->normalmap,    // address of the shader-resource-view
                                           NULL);                           // no multithreading
}



void GeometryClass::Render(ID3D11Device *dev, ID3D11DeviceContext *devcon, ID3D11RenderTargetView *backbuffer, 
							IDXGISwapChain *swapchain, ID3D11Buffer *pCBuffer, ID3D11Buffer *vCBuffer,
							ID3D11Buffer *mCBuffer,
							ID3D11DepthStencilView *zbuffer, ID3D11ShaderResourceView *pTexture,
							ID3D11BlendState *pBS,ID3D11SamplerState *pSS, ID3D11RasterizerState *pRS)
{

    // clear the back buffer to a deep blue
    devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
	// clear the depth buffer
	devcon->ClearDepthStencilView(zbuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);
	
    // set the various states
    devcon->RSSetState(pRS);
    devcon->PSSetSamplers(0, 1, &pSS);
    devcon->OMSetBlendState(pBS, 0, 0xffffffff);
    // Draw it all
    for(int i = 0; i < objects.size(); i++ )
    {

		// update constant buffer
		devcon->UpdateSubresource(vCBuffer, 0, 0, objects[i]->matrices, 0, 0);	
		devcon->UpdateSubresource(pCBuffer, 0, 0, objects[i]->light, 0, 0);
		devcon->UpdateSubresource(mCBuffer, 0, 0, objects[i]->mapping, 0, 0);
		objects[i]->Render(dev, devcon, backbuffer, swapchain, pTexture);
    }
	
    // switch the back buffer and the front buffer
    swapchain->Present(0, 0);
}