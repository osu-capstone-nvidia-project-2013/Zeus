//***************************************************************************************
// FBXObj.cpp
//***************************************************************************************
#include "FBXObj.h"

UINT mIndexCount;
UINT mVertexCount;
vector<int> mIndices;
vector<XMFLOAT3> mVertices;

ID3D11Buffer* mVB = 0;
ID3D11Buffer* mIB = 0;

vector<ID3D11ShaderResourceView*> mTextureArray;
vector<ID3D11ShaderResourceView*> mNormalArray;

FBXImporter mFBXImporter;

FBXObj::FBXObj() /*: 
    mVB(0),
    mIB(0),
    mIndexCount(0),
    mVertexCount(0)*/
{

}

FBXObj::~FBXObj()
{
    /*if(mVB)
    {
        mVB->Release();
    }

    if(mIB)
    {
        mIB->Release();
    }

    mTextureArray.clear();
    mNormalArray.clear();*/
}


void FBXObj::Import(char* filename, ID3D11Device* dev)
{
    vector<XMFLOAT3> positions;
    vector<int> indices;
    vector<XMFLOAT3> norms;
	vector<XMFLOAT2> texVec;
    vector<int> texNum;

    mFBXImporter.Import(filename, &positions, &indices, &norms, &texVec, &texNum);

    std::vector<Vertex::PosNormalTexTan> vertices;
    Vertex::PosNormalTexTan tempVert;

	XMVECTOR p;
	XMVECTOR n;
	for(int i = 0; i < positions.size(); i++)
	{
		p = XMVectorSet(positions[i].x, positions[i].y,positions[i].z, 1.0 );
		n = XMVectorSet(norms[i].x, norms[i].y, norms[i].z, 1.0 );
		XMVECTOR cross = XMVector3Cross(p, n);
		XMStoreFloat3(&tempVert.TangentU, cross);

		tempVert.Pos = positions[i];
		tempVert.Normal = norms[i];
		tempVert.Tex = texVec[i];
		tempVert.TexNum = texNum[i];

		vertices.push_back(tempVert);
	}

    mIndexCount = indices.size();
    mVertexCount = positions.size();
    mVertices = positions;
    mIndices = indices;

    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex::PosNormalTexTan) * positions.size();
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &vertices[0];
    HR(dev->CreateBuffer(&vbd, &vinitData, &mVB));

    //
    // Pack the indices of all the meshes into one index buffer.
    //

    D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(UINT) * mIndexCount;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &indices[0];
    HR(dev->CreateBuffer(&ibd, &iinitData, &mIB));
}

void FBXObj::LoadTexture(ID3D11Device* dev, wchar_t* filename)
{
	ID3D11ShaderResourceView* texture;
	HR(D3DX11CreateShaderResourceViewFromFile(dev, 
        filename, 0, 0, &texture, 0 ));
	mTextureArray.push_back(texture);
}

void FBXObj::LoadNormal(ID3D11Device* dev, wchar_t* filename)
{
	ID3D11ShaderResourceView* texture;
	HR(D3DX11CreateShaderResourceViewFromFile(dev, 
        filename, 0, 0, &texture, 0 ));
	mNormalArray.push_back(texture);
}


void FBXObj::LoadTextures(ID3D11Device* dev, vector<wchar_t*> filenames)
{
	for(int i = 0; i < filenames.size() ; i++)
	{
		ID3D11ShaderResourceView* texture;
		HR(D3DX11CreateShaderResourceViewFromFile(dev, 
			filenames[i], 0, 0, &texture, 0 ));
		mTextureArray.push_back(texture);
	}
}

void FBXObj::LoadNormals(ID3D11Device* dev, vector<wchar_t*> filenames)
{
	for(int i = 0; i < filenames.size() ; i++)
	{
		ID3D11ShaderResourceView* texture;
		HR(D3DX11CreateShaderResourceViewFromFile(dev, 
			filenames[i], 0, 0, &texture, 0 ));
		mNormalArray.push_back(texture);
	}
}

UINT FBXObj::GetIndexCount()
{
    return mIndexCount;
}

UINT FBXObj::GetVertexCount()
{
    return mVertexCount;
}

vector<int> FBXObj::GetIndices()
{
    return mIndices;
}

vector<XMFLOAT3> FBXObj::GetVertices()
{
    return mVertices;
}

ID3D11Buffer* FBXObj::GetVB()
{
    return mVB;
}

ID3D11Buffer* FBXObj::GetIB()
{
    return mIB;
}

vector<ID3D11ShaderResourceView*> FBXObj::GetTextureArray()
{
    return mTextureArray;
}

vector<ID3D11ShaderResourceView*> FBXObj::GetNormalArray()
{
    return mNormalArray;
}