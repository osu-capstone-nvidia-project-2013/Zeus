// shaderclass.h
#ifndef _SHADERCLASS_H
#define _SHADERCLASS_H
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <vector>
#include <string>
using namespace std;

struct SHADER
{
    D3D11_INPUT_ELEMENT_DESC *ied;         // the pointer to the input element desc
    ID3D11VertexShader *pVS;               // the pointer to the vertex shader
    ID3D11PixelShader *pPS;                // the pointer to the pixel shader
    string name;						   // name of shader
};

class ShaderClass
{
public:
    ShaderClass();
    ~ShaderClass();

	void SetShaders(ID3D11Device*, ID3D11DeviceContext*);

    vector<SHADER> shaders;
private:
	void InitShader(string, string, struct SHADER, ID3D11Device*, ID3D11DeviceContext*);
	
};


#endif