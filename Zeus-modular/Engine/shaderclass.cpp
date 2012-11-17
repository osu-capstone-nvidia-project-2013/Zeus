#include "shaderclass.h"

ShaderClass::ShaderClass() 
{
	// init all shaderrrzzz
}

ShaderClass::~ShaderClass() 
{

}

void ShaderClass::SetShaders(ID3D11Device *dev, ID3D11DeviceContext *devcon) {

	struct SHADER pew;

	InitShader("stuff", "stuff", pew, dev, devcon);
}

void ShaderClass::InitShader(string vsname, string psname, struct SHADER shader,ID3D11Device *dev, ID3D11DeviceContext *devcon)
{
	// load and compile the two shaders
    ID3D10Blob *bVS, *bPS;
	ID3D11InputLayout *pLayout;            // the pointer to the input layout
    D3DX11CompileFromFile(L"vShader.hlsl", 0, 0, "VShader", "vs_5_0", 0, 0, 0, &bVS, 0, 0);
    D3DX11CompileFromFile(L"pShader.hlsl", 0, 0, "PShader", "ps_5_0", 0, 0, 0, &bPS, 0, 0);

	// encapsulate both shaders into shader objects
    dev->CreateVertexShader(bVS->GetBufferPointer(), bVS->GetBufferSize(), NULL, &shader.pVS);
    dev->CreatePixelShader(bPS->GetBufferPointer(), bPS->GetBufferSize(), NULL, &shader.pPS);


	// set the shader objects
    devcon->VSSetShader(shader.pVS, 0, 0);
    devcon->PSSetShader(shader.pPS, 0, 0);

	// create the input layout object
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    dev->CreateInputLayout(ied, 2, bVS->GetBufferPointer(), bVS->GetBufferSize(), &pLayout);
    devcon->IASetInputLayout(pLayout);
}


