// Author: Zander Clucas
// 2013 Capstone Project
// geometry.cpp
// Encapsulates the geometry for 3d models
#include "geometry.h"

ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}


bool ModelClass::Initialize(ID3D10Device* device)
{
	bool result;


	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	return true;
}


void ModelClass::Shutdown()
{
	// Release the vertex and index buffers.
	ShutdownBuffers();

	return;
}


void ModelClass::Render(ID3D10Device* device)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(device);

	return;
}


int ModelClass::GetIndexCount()
{
	return m_indexCount;
}


bool ModelClass::InitializeBuffers(ID3D10Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D10_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D10_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	
	// Set the number of vertices in the vertex array.
	m_vertexCount = 0;

	// Set the number of indices in the index array.
	m_indexCount = 0;

	VertexType circle_center;
	circle_center.position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	circle_center.color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	circle_center.normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	DrawCircle(circle_center, 1.0f, 60, vertices, indices);


	// Set up the description of the vertex buffer.
    vertexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;

	// Now finally create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the index buffer.
    indexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}

// Draws a circle at the given center point with the given radius and resolution. The vertex and index arrays are passed by reference.
void ModelClass::DrawCircle(VertexType center_point, float radius, int resolution, VertexType* & vertices, unsigned long* & indices) {
	//  Check for resolution min
	if( resolution < 4 ) {
		return;
	}
	
	// Transfer existing vertices to a temp vertex array
	VertexType* temp_vertices;
	unsigned long* temp_indices;
	if(m_vertexCount > 0) {
		temp_vertices = new VertexType[m_vertexCount];
		for(int i = 0; i < m_vertexCount; i++) {
			temp_vertices[i] = vertices[i];
		}
		delete [] vertices;
		vertices = 0;
	}
	if(m_indexCount > 0) {
		temp_indices = new unsigned long[m_indexCount];
		for(int i = 0; i < m_indexCount; i++) {
			temp_indices[i] = indices[i];
		}
		delete [] indices;
		indices = 0;
	}

	int temp_vert_count = m_vertexCount;
	int temp_ind_count = m_indexCount;

	m_vertexCount += resolution + 1;
	m_indexCount += (resolution * 3); // resolution times for the center point, 2 times for each of the resolution points left 
	
	// Recreate the vertex and index array.
	vertices = new VertexType[m_vertexCount];
	for(int i = 0; i < temp_vert_count; i++) {
		vertices[i] = temp_vertices[i];
	}
	indices = new unsigned long[m_indexCount];
	for(int i = 0; i < temp_ind_count; i++) {
		indices[i] = temp_indices[i];
	}

	// Add the center point
	vertices[temp_vert_count] = center_point;
	
	// Add the circle's points
	for(int i = temp_vert_count + 1; i < m_vertexCount; i++) {
		float pos_x = cos( (i - (temp_vert_count + 1)) * ((2 * D3DX_PI) / resolution) ) ;
		float pos_y = sin( (i - (temp_vert_count + 1)) * ((2 * D3DX_PI) / resolution) );

		//Generate a rainbow circle!
		float hue = (i - (temp_vert_count + 1)) * (300.0f / resolution);
		float hsv[] = {hue, 1.0f, 1.0f};
		float rgb[] = {0.0f, 0.0f, 0.0f};
		HSVtoRGB(hsv, rgb);
		
		VertexType circle_point;
		circle_point.position = D3DXVECTOR3(pos_x, pos_y, 0.0f);
		circle_point.color = D3DXVECTOR4(rgb[0], rgb[1], rgb[2], 1.0f);
		circle_point.normal = center_point.normal;
		
		vertices[i] = circle_point;
	}

	// Rotate circle to align normals
    // -------------------------
    // Guys who are copying this, still need to look at center_point.normal
    // and rotate each point to the correct spot to align with the normal
    // --------------------------

	// Add the indices
	int curr_vert = temp_vert_count + 1;
	for(int i = temp_ind_count; i < m_indexCount; i++) {
		indices[i] = temp_vert_count; //The first point of each triangle is always the center point.
		i++;
		if(curr_vert + 1 < m_vertexCount) {
			indices[i] = curr_vert + 1;
		} else {
			indices[i] = temp_vert_count + 1; // Points back to the beginning point
		} 
		i++;
		indices[i] = curr_vert;
		
		// Don't need another i++ because the for loop takes care of it
		curr_vert++;
	}

	return;
}

// the HSV color model will be as follows
// h : [0 - 360]
// s : [0 - 1]
// v : [0 - 1]
// If you want it differently (in a 2 * pi scale, 256 instead of 1, etc, 
// you'll have to change it yourself.
// rgb is returned in 0-1 scale (ready for color3f)
void ModelClass::HSVtoRGB(float hsv[3], float rgb[3]) {
	float tmp1 = hsv[2] * (1-hsv[1]);
	float tmp2 = hsv[2] * (1-hsv[1] * (hsv[0] / 60.0f - (int) (hsv[0]/60.0f) ));
	float tmp3 = hsv[2] * (1-hsv[1] * (1 - (hsv[0] / 60.0f - (int) (hsv[0]/60.0f) )));
	switch((int)(hsv[0] / 60)) {
		case 0:
			rgb[0] = hsv[2] ;
			rgb[1] = tmp3 ;
			rgb[2] = tmp1 ;
			break;
		case 1:
			rgb[0] = tmp2 ;
			rgb[1] = hsv[2] ;
			rgb[2] = tmp1 ;
			break;
		case 2:
			rgb[0] = tmp1 ;
			rgb[1] = hsv[2] ;
			rgb[2] = tmp3 ;
			break;
		case 3:
			rgb[0] = tmp1 ;
			rgb[1] = tmp2 ;
			rgb[2] = hsv[2] ;
			break;
		case 4:
			rgb[0] = tmp3 ;
			rgb[1] = tmp1 ;
			rgb[2] = hsv[2] ;
			break;
		case 5:
			rgb[0] = hsv[2] ;
			rgb[1] = tmp1 ;
			rgb[2] = tmp2 ;
			break;
		default:
			break;
	}
}

void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void ModelClass::RenderBuffers(ID3D10Device* device)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
    stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	device->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
    device->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
    device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}