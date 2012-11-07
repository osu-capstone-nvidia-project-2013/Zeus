////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"


ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_model = 0;
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}


bool ModelClass::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename)
{
	bool result;


	// Load in the model data,
	result = LoadModel(modelFilename);
	if(!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}


	return true;
}


void ModelClass::Shutdown()
{
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	return;
}


void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int ModelClass::GetIndexCount()
{
	return m_indexCount;
}


bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Set the number of vertices in the vertex array.
	m_vertexCount = 0;

	// Set the number of indices in the index array.
	m_indexCount = 0;

    // Draw a triangle
    VertexType triangle1;
    triangle1.position = D3DXVECTOR3(-2.0f, 2.0f, -1.0f);
    triangle1.color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
    triangle1.normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
    VertexType triangle2;
    triangle2.position = D3DXVECTOR3(-3.0f, 2.0f, -2.0f);
    triangle2.color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
    triangle2.normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
    VertexType triangle3;
    triangle3.position = D3DXVECTOR3(-2.5f, 3.0f, -1.0f);
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
	circle_center.position = D3DXVECTOR3(2.0f, 2.0f, -1.0f);
	circle_center.color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	circle_center.normal = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	DrawCircle(circle_center, 1.0f, 40);

    // Draw a sphere
    VertexType sphere_center;
	sphere_center.position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	sphere_center.color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	sphere_center.normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	DrawSphere(sphere_center, 1.5f, 20, 20);
	// Draw a sphere
    VertexType sphere_center2;
	sphere_center2.position = D3DXVECTOR3(2.0f, -2.0f, 2.0f);
	sphere_center2.color = D3DXVECTOR4(0.5f, 0.0f, 0.5f, 1.0f);
	sphere_center2.normal = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	DrawSphere(sphere_center2, 0.5f, 20, 20);

	

	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = &vertices_[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = &indices_[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

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

	float theta, phi;
	int i, j, t, u;

	for(t=0,j=1; j < stacks - 1; j++ ) {
		for( i = 0; i < slices; i++) {
			theta = float(j) / (stacks - 1) * D3DX_PI;
			phi = float(i) / (slices - 1) * D3DX_PI * 2;
			VertexType point;
			point.position.x = (radius) * (sinf(theta) * cosf(phi)) + center_point.position.x;
			point.position.y = (radius) * (cosf(theta)) + center_point.position.y;
			point.position.z = (radius) * (-sinf(theta) * sinf(phi)) + center_point.position.z;
			point.color = center_point.color;
			point.normal = point.position;
			t++;
			vertices_.push_back(point);
		}
	}
	int top_ind = m_vertexCount + t;
    VertexType top;
    top.position = center_point.position;
    top.position.y += radius;
	top.color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
    top.normal = top.position;
    vertices_.push_back(top);
	t++;

	int bot_ind = m_vertexCount + t;
    VertexType bottom;
    bottom.position = center_point.position;
    bottom.position.y -= radius;
    bottom.color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
    bottom.normal = bottom.position;
    vertices_.push_back(bottom);
	t++;

	
	for(i = 0; i < slices - 1; i++) {
		// Make top
		indices_.push_back(top_ind);
		indices_.push_back(m_vertexCount + i);
		indices_.push_back(m_vertexCount + i + 1);
		m_indexCount += 3;
		// Make bottom
		indices_.push_back(bot_ind);
		indices_.push_back((top_ind - 1) - i);
		indices_.push_back((top_ind - 1) - (i + 1));
		m_indexCount += 3;
	}
	// Make inbetween
	for(u = 0, j = 0; j < (stacks - 3); j++) {
		for( i = 0; i < (slices - 1); i++) {
			indices_.push_back( (j) * slices + i + m_vertexCount);
			indices_.push_back( (j + 1) * slices + i + 1 + m_vertexCount);
			indices_.push_back( (j) * slices + i + 1 + m_vertexCount);
			m_indexCount += 3;
			indices_.push_back( (j) * slices + i + m_vertexCount);
			indices_.push_back( (j + 1) * slices + i + m_vertexCount);
			indices_.push_back( (j + 1) * slices + i + 1 + m_vertexCount);
			m_indexCount += 3;
		}
	}
	

	m_vertexCount += t;

	return;
}

// Transforms the object at the index with the given matrix
void ModelClass::TransformObject(int index, D3DXMATRIX matFinal) {
    int object_size = 0;
    if(objects_.size() > index) {
        if(objects_.size() < (index + 1)) {
            object_size = objects_[index+1] - objects_[index];
        } else {
            object_size = vertices_.size() - objects_[index];
        }
    } else {
        return;
    }
    
    for( int i = 0; i < object_size; i++) {
        D3DXVec3TransformCoord(&vertices_[objects_[index] + i].position, &vertices_[objects_[index] + i].position, &matFinal) ;
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


void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}


bool ModelClass::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	int i;


	// Open the model file.
	fin.open(filename);
	
	// If it could not open the file then exit.
	if(fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if(!m_model)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for(i=0; i<m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// Close the model file.
	fin.close();

	return true;
}


void ModelClass::ReleaseModel()
{
	if(m_model)
	{
		delete [] m_model;
		m_model = 0;
	}

	return;
}