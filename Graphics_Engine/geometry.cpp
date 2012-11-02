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
	D3D10_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D10_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Set the number of vertices in the vertex array.
	m_vertexCount = 0;

	// Set the number of indices in the index array.
	m_indexCount = 0;

    // Draw a triangle
    VertexType triangle1;
    triangle1.position = D3DXVECTOR3(-2.0f, 2.0f, 0.0f);
    triangle1.color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
    triangle1.normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
    VertexType triangle2;
    triangle2.position = D3DXVECTOR3(-3.0f, 2.0f, 0.0f);
    triangle2.color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
    triangle2.normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
    VertexType triangle3;
    triangle3.position = D3DXVECTOR3(-2.5f, 3.0f, 0.0f);
    triangle3.color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
    triangle3.normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
    DrawTriangle(triangle1, triangle2, triangle3);

    // Draw a quad
    VertexType quad1;
    quad1.position = D3DXVECTOR3(-3.5f, -1.5f, 0.0f);
    quad1.color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
    quad1.normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
    VertexType quad2;
    quad2.position = D3DXVECTOR3(-1.5f, -3.5f, 0.0f);
    quad2.color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
    quad2.normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
    DrawQuad(quad1, quad2);

    // Draw a circle
	VertexType circle_center;
	circle_center.position = D3DXVECTOR3(2.0f, 2.0f, 0.0f);
	circle_center.color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	circle_center.normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	DrawCircle(circle_center, 1.0f, 40);

    // Draw a sphere
    VertexType sphere_center;
	sphere_center.position = D3DXVECTOR3(2.0f, 2.0f, 0.0f);
	sphere_center.color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	sphere_center.normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	DrawSphere(circle_center, 1.0f, 20, 20);

	// Set up the description of the vertex buffer.
    vertexBufferDesc.Usage = D3D10_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = &vertices_[0];

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
    indexData.pSysMem = &indices_[0];

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

// Draws a triangle object out of the three points, as long as all of the normals are the same.
// Draws in order of points given
void ModelClass::DrawTriangle(VertexType point1, VertexType point2, VertexType point3) {
    // Store the object
    objects_.push_back(m_vertexCount);

    vertices_.push_back(point1);
    vertices_.push_back(point2);
    vertices_.push_back(point3);

    indices_.push_back(m_vertexCount++);
    indices_.push_back(m_vertexCount++);
    indices_.push_back(m_vertexCount++);
    m_indexCount += 3;
}

// Draws a Quad object with two given corners. THe corners make up the opposite sides of the Quad.
void ModelClass::DrawQuad(VertexType corner1, VertexType corner3) {
    // Store the object
    objects_.push_back(m_vertexCount);

    // Corners are numbered clockwise
    VertexType corner2;
    corner2.position = D3DXVECTOR3(corner3.position.x, corner1.position.y, corner1.position.z);
    corner2.color = corner1.color;
	corner2.normal = corner1.normal;

    VertexType corner4;
    corner4.position = D3DXVECTOR3(corner1.position.x, corner3.position.y, 0.0f);
	corner4.color = corner3.color;
	corner4.normal = corner1.normal;

    vertices_.push_back(corner1);
    vertices_.push_back(corner2);
    vertices_.push_back(corner3);
    vertices_.push_back(corner4);

    indices_.push_back(m_vertexCount);
    indices_.push_back(m_vertexCount + 1);
    indices_.push_back(m_vertexCount + 2);
    indices_.push_back(m_vertexCount);
    indices_.push_back(m_vertexCount + 2);
    indices_.push_back(m_vertexCount + 3);

    m_vertexCount += 4;
    m_indexCount += 6;
}

// Draws a circle at the given center point with the given radius and resolution. The vertex and index arrays are passed by reference.
void ModelClass::DrawCircle(VertexType center_point, float radius, int resolution) {
	//  Check for resolution min
	if( resolution < 4 ) {
		return;
	}
	
	int temp_vert_count = m_vertexCount;
	int temp_ind_count = m_indexCount;

    //Save this object as a distinct object
    objects_.push_back(m_vertexCount);

	m_vertexCount += resolution + 1;
	m_indexCount += (resolution * 3); // resolution times for the center point, 2 times for each of the resolution points left 

	// Add the center point
	vertices_.push_back(center_point);
	
	// Add the circle's points
	float rotate_x = atan2(center_point.normal.y, center_point.normal.z);
	float rotate_y = atan2(center_point.normal.x, center_point.normal.z);
	D3DXMATRIX transform_x;
	D3DXMatrixRotationX(&transform_x, rotate_x);
	D3DXMATRIX transform_y;
	D3DXMatrixRotationY(&transform_y, rotate_y);
	D3DXMATRIX transform_inv = transform_x * transform_y;
	D3DXMatrixInverse(&transform_inv, NULL, &transform_inv);
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
		
        vertices_.push_back(circle_point);

		D3DXVECTOR4 transform_v = D3DXVECTOR4(vertices_[i].position.x, vertices_[i].position.y, vertices_[i].position.z, 1.0f);
		
		D3DXVec4Transform(&transform_v, &transform_v, &transform_inv);
		
		vertices_[i].position.x = transform_v.x;
		vertices_[i].position.y = transform_v.y;
		vertices_[i].position.z = transform_v.z;

		vertices_[i].position.x = vertices_[i].position.x + center_point.position.x;
		vertices_[i].position.y = vertices_[i].position.y + center_point.position.y;
		vertices_[i].position.z = vertices_[i].position.z + center_point.position.z;
	}

	// Add the indices
	int curr_vert = temp_vert_count + 1;
	for(int i = temp_ind_count; i < m_indexCount; i++) {
		indices_.push_back(temp_vert_count); //The first point of each triangle is always the center point.
		i++;
		if(curr_vert + 1 < m_vertexCount) {
            indices_.push_back(curr_vert + 1);
		} else {
            indices_.push_back(temp_vert_count + 1); // Points back to the beginning point
		} 
		i++;
		indices_.push_back(curr_vert);
		
		// Don't need another i++ because the for loop takes care of it
		curr_vert++;
	}

	return;
}

// Draws a circle at the given center point with the given radius and resolution. The vertex and index arrays are passed by reference.
void ModelClass::DrawSphere(VertexType center_point, float radius, int slices, int stacks) {
    //Save this object as a distinct object
    objects_.push_back(m_vertexCount);
        
    VertexType top;
    top.position = center_point.position;
    top.position.y += radius;
    top.color = center_point.color;
    top.normal = top.position;
    vertices_.push_back(top);

    VertexType bottom;
    bottom.position = center_point.position;
    bottom.position.y -= radius;
    bottom.color = center_point.color;
    bottom.normal = bottom.position;
    vertices_.push_back(bottom);

    // Add the bulk of the 
    vector< vector<VertexType> > sphere (slices, vector<VertexType> (stacks));
    for( int m = 0; m < slices; m++) {
        for( int n = 1; n < stacks - 1; n++) { // Reduced because of top and bottom points
            sphere[m][n].position.x = sin(D3DX_PI * (m / slices)) * cos(2 * D3DX_PI * (n / stacks)) + center_point.position.x;
            sphere[m][n].position.y = sin(D3DX_PI * (m / slices)) * sin(2 * D3DX_PI * (n / stacks)) + center_point.position.y;
            sphere[m][n].position.z = cos(D3DX_PI * (m / slices)) + center_point.position.z;
            sphere[m][n].color = center_point.color;
            sphere[m][n].normal = sphere[m][n].position;
            vertices_.push_back(sphere[m][n]);
        }
    }

    //Create the top and the bottom
    int top_ind = m_vertexCount;
    int bot_ind = m_vertexCount + 1;
    for(int i = 0; i < slices; i++) {
        indices_.push_back(top_ind);
        indices_.push_back( (top_ind + 1) + (i * (stacks - 1)) );
        indices_.push_back( (top_ind + 1) +  ((i + 1) * (stacks - 1)));
    }
    /*for(int i = 0; i < slices; i++) {
        indices_.push_back(bot_ind);
        indices_.push_back( i );
        indices_.push_back( i + 1);
    }*/


    m_vertexCount += (slices) * (stacks - 2) + 2;
    m_indexCount += (8 * (slices) * (stacks - 2)) + (2 * slices);

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