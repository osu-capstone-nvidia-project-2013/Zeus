//***************************************************************************************
// TextureHelper.cpp
//
//
//
//
//
//
//
//***************************************************************************************

#include "TextureHelper.h"

TextureHelper::TextureHelper() : md3dDevice(0)
{
}

TextureHelper::~TextureHelper()
{
	for(auto it = mTextureSRV.begin(); it != mTextureSRV.end(); ++it)
    {
		ReleaseCOM(it->second);
    }

	mTextureSRV.clear();
}

void TextureHelper::Init(ID3D11Device* device)
{
	md3dDevice = device;
}

ID3D11ShaderResourceView* TextureHelper::CreateTexture(std::wstring filename)
{
	ID3D11ShaderResourceView* srv = 0;

	// Does it already exist?
	if( mTextureSRV.find(filename) != mTextureSRV.end() )
	{
		srv = mTextureSRV[filename];
	}
	else
	{
		HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, filename.c_str(), 0, 0, &srv, 0 ));

		mTextureSRV[filename] = srv;
	}

	return srv;
}
 
