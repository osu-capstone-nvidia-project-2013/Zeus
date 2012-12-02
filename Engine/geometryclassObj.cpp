#include "objectclass.h"
#include "geometryclass.h"

using namespace std;

void GeometryClass::LoadObject(ID3D11Device *dev, ID3D11DeviceContext *devcon, string obj_name,D3DXVECTOR4 color)
{
    vector<VERTEX> vertices;
    vector<int> indices;
	int fileExists = -1;

	objects.size();
	

	for( int i = 0; i < objects.size(); i++ )
	{
		//Check to see if we have this loaded already
		if( objects[i]->name.compare(obj_name) == 0)
		{
			fileExists = i;
			break;
		}
	}
	if( fileExists == -1 )
	{
		char line[100];
		float x = 0, y = 0, z = 0;
		vector<D3DXVECTOR3> verts;
		vector<D3DXVECTOR3> norms;

		ifstream stream (obj_name);
		if(!stream)
		{
			return;
		}
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
	}


	CreateObject(dev, devcon, vertices, indices, fileExists, obj_name);
}


void GeometryClass::CreatePlane(ID3D11Device *dev, ID3D11DeviceContext *devcon, float y)
{
	CreatePlane(dev, devcon, y, "null");
}

void GeometryClass::CreatePlane(ID3D11Device *dev, ID3D11DeviceContext *devcon, float y, string name)
{
	vector<VERTEX> vertices;
    vector<int> indices;

	D3DXCOLOR ground = D3DXCOLOR(0.8f,0.8f,0.8f,1.0f);
    
	VERTEX vert1 = {D3DXVECTOR3(-15.0f, y, -15.0f),ground, D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR2(2.0f,2.0f)};
    VERTEX vert2 = {D3DXVECTOR3(-15.0f, y, 15.0f),ground, D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR2(2.0f,0.0f)};
    VERTEX vert3 = {D3DXVECTOR3(15.0f, y, 15.0f),ground, D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR2(0.0f,0.0f)};
	VERTEX vert4 = {D3DXVECTOR3(15.0f, y, -15.0f),ground, D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR2(0.0f,2.0f)};
    vert1.tangent = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vert2.tangent = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vert3.tangent = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	vert4.tangent = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
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

	CreateObject(dev, devcon, vertices, indices, -1, name);
}

void GeometryClass::CreateQuad(ID3D11Device *dev, ID3D11DeviceContext *devcon, float r, float g, float b, float size)
{
	CreateQuad(dev, devcon, r, g, b, size, "null");
}

void GeometryClass::CreateQuad(ID3D11Device *dev, ID3D11DeviceContext *devcon, float r, float g, float b, float size, string name)
{
	vector<VERTEX> vertices;
    vector<int> indices;

	D3DXCOLOR color = D3DXCOLOR(r,g,b,1.0f);
    
	VERTEX vert1 = {D3DXVECTOR3(-size, -size, 0.0f),color, D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.999f,0.999f)};
    VERTEX vert2 = {D3DXVECTOR3(-size, size, 0.0f),color, D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.999f,0.0f)};
    VERTEX vert3 = {D3DXVECTOR3(size, size, 0.0f),color, D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f,0.0f)};
	VERTEX vert4 = {D3DXVECTOR3(size, -size, 0.0f),color, D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f,0.999f)};
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

	CreateObject(dev, devcon, vertices, indices, -1, name);
}

void GeometryClass::CreateSphere(ID3D11Device *dev, ID3D11DeviceContext *devcon, VERTEX center_point, float radius, int slices, int stacks) 
{
	CreateSphere( dev, devcon, center_point, radius, slices, stacks, "null");
}

void GeometryClass::CreateSphere(ID3D11Device *dev, ID3D11DeviceContext *devcon, VERTEX center_point, float radius, int slices, int stacks, string name) 
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

    
    CreateObject(dev, devcon, vertices, indices, -1, "null");

	return;
}