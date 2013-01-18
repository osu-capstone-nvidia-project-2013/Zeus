//***************************************************************************************
// TextureHelper.h
//
//
//
//
//
//
//
//***************************************************************************************

#ifndef TextureHelper_H
#define TextureHelper_H

#include "d3dUtil.h"
#include <map>

///<summary>
/// Simple texture manager to avoid loading duplicate textures from file.  That can
/// happen, for example, if multiple meshes reference the same texture filename. 
///</summary>
class TextureHelper
{
public:
	TextureHelper();
	~TextureHelper();

	void Init(ID3D11Device* device);

	ID3D11ShaderResourceView* CreateTexture(std::wstring filename);

private:
	TextureHelper(const TextureHelper& rhs);
	TextureHelper& operator=(const TextureHelper& rhs);
	
private:
	ID3D11Device* md3dDevice;
	std::map<std::wstring, ID3D11ShaderResourceView*> mTextureSRV;
};

#endif // TextureHelper_H