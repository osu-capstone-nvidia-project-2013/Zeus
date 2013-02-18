//***************************************************************************************
// Effects.h
//
//
//
//
//
//
//
//***************************************************************************************

#ifndef EFFECTS_H
#define EFFECTS_H

#include "d3dUtil.h"
#include <vector>
using namespace std;

#pragma region Effect
class Effect
{
public:
	Effect(ID3D11Device* device, const std::wstring& filename);
	virtual ~Effect();

private:
	Effect(const Effect& rhs);
	Effect& operator=(const Effect& rhs);

protected:
	ID3DX11Effect* mFX;
};
#pragma endregion

#pragma region BasicEffect
class BasicEffect : public Effect
{
public:
	BasicEffect(ID3D11Device* device, const std::wstring& filename);
	~BasicEffect();

	void SetWorldViewProj(CXMMATRIX M)                  { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M)                          { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(CXMMATRIX M)              { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetShadowTransform(CXMMATRIX M)                { ShadowTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetShadowTransform2(CXMMATRIX M)               { ShadowTransform2->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetTexTransform(CXMMATRIX M)                   { TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const XMFLOAT3& v)                  { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetFogColor(const FXMVECTOR v)                 { FogColor->SetFloatVector(reinterpret_cast<const float*>(&v)); }
	void SetFogStart(float f)                           { FogStart->SetFloat(f); }
	void SetFogRange(float f)                           { FogRange->SetFloat(f); }
	void SetDirLights(const DirectionalLight* lights)   { DirLights->SetRawValue(lights, 0, 3*sizeof(DirectionalLight)); }
	void SetPointLights(const PointLight* lights)		{ PointLights->SetRawValue(lights, 0, sizeof(PointLight)); }
	void SetMaterial(const Material& mat)               { Mat->SetRawValue(&mat, 0, sizeof(Material)); }
	void SetDiffuseMap(ID3D11ShaderResourceView* tex)   { DiffuseMap->SetResource(tex); }
	void SetShadowMap(ID3D11ShaderResourceView* tex)    { ShadowMap->SetResource(tex); }
	void SetShadowMap2(ID3D11ShaderResourceView* tex)   { ShadowMap2->SetResource(tex); }
	void SetCubeMap(ID3D11ShaderResourceView* tex)      { CubeMap->SetResource(tex); }
	void SetTextureArray(vector<ID3D11ShaderResourceView*> texVec) { TextureArrayPtr->SetResourceArray(&texVec[0], 0, texVec.size());}

	void SetOmniShadowMaps(ID3D11ShaderResourceView* M0, ID3D11ShaderResourceView* M1,
        ID3D11ShaderResourceView* M2, ID3D11ShaderResourceView* M3, ID3D11ShaderResourceView* M4, 
        ID3D11ShaderResourceView* M5)	   { OmniShadowMap0->SetResource(M0);
                                             OmniShadowMap1->SetResource(M1);
                                             OmniShadowMap2->SetResource(M2);
                                             OmniShadowMap3->SetResource(M3);
                                             OmniShadowMap4->SetResource(M4);
                                             OmniShadowMap5->SetResource(M5);}
	void SetShadowTransforms(CXMMATRIX M0, CXMMATRIX M1, 
		CXMMATRIX M2, CXMMATRIX M3, 
		CXMMATRIX M4, CXMMATRIX M5)						   { ShadowTransCube0->SetMatrix(reinterpret_cast<const float*>(&M0)); 
															 ShadowTransCube1->SetMatrix(reinterpret_cast<const float*>(&M1)); 
															 ShadowTransCube2->SetMatrix(reinterpret_cast<const float*>(&M2)); 
															 ShadowTransCube3->SetMatrix(reinterpret_cast<const float*>(&M3)); 
															 ShadowTransCube4->SetMatrix(reinterpret_cast<const float*>(&M4)); 
															 ShadowTransCube5->SetMatrix(reinterpret_cast<const float*>(&M5)); }

	ID3DX11EffectTechnique* Light1Tech;
	ID3DX11EffectTechnique* Light2Tech;
	ID3DX11EffectTechnique* Light3Tech;

	ID3DX11EffectTechnique* Light0TexTech;
	ID3DX11EffectTechnique* Light1TexTech;
	ID3DX11EffectTechnique* Light2TexTech;
	ID3DX11EffectTechnique* Light3TexTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipTech;

	ID3DX11EffectTechnique* Light1FogTech;
	ID3DX11EffectTechnique* Light2FogTech;
	ID3DX11EffectTechnique* Light3FogTech;

	ID3DX11EffectTechnique* Light0TexFogTech;
	ID3DX11EffectTechnique* Light1TexFogTech;
	ID3DX11EffectTechnique* Light2TexFogTech;
	ID3DX11EffectTechnique* Light3TexFogTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipFogTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipFogTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipFogTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipFogTech;

	ID3DX11EffectTechnique* Light1ReflectTech;
	ID3DX11EffectTechnique* Light2ReflectTech;
	ID3DX11EffectTechnique* Light3ReflectTech;

	ID3DX11EffectTechnique* Light0TexReflectTech;
	ID3DX11EffectTechnique* Light1TexReflectTech;
	ID3DX11EffectTechnique* Light2TexReflectTech;
	ID3DX11EffectTechnique* Light3TexReflectTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipReflectTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipReflectTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipReflectTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipReflectTech;

	ID3DX11EffectTechnique* Light1FogReflectTech;
	ID3DX11EffectTechnique* Light2FogReflectTech;
	ID3DX11EffectTechnique* Light3FogReflectTech;

	ID3DX11EffectTechnique* Light0TexFogReflectTech;
	ID3DX11EffectTechnique* Light1TexFogReflectTech;
	ID3DX11EffectTechnique* Light2TexFogReflectTech;
	ID3DX11EffectTechnique* Light3TexFogReflectTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipFogReflectTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipFogReflectTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipFogReflectTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipFogReflectTech;

	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectMatrixVariable* ShadowTransform;
	ID3DX11EffectMatrixVariable* ShadowTransform2;
	ID3DX11EffectMatrixVariable* TexTransform;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVectorVariable* FogColor;
	ID3DX11EffectScalarVariable* FogStart;
	ID3DX11EffectScalarVariable* FogRange;
	ID3DX11EffectVariable* DirLights;
	ID3DX11EffectVariable* PointLights;
	ID3DX11EffectVariable* Mat;

	ID3DX11EffectShaderResourceVariable* DiffuseMap;
	ID3DX11EffectShaderResourceVariable* ShadowMap;
	ID3DX11EffectShaderResourceVariable* ShadowMap2;
	ID3DX11EffectShaderResourceVariable* CubeMap;

	ID3DX11EffectShaderResourceVariable* TextureArrayPtr;

	ID3DX11EffectShaderResourceVariable* OmniShadowMap0;
    ID3DX11EffectShaderResourceVariable* OmniShadowMap1;
    ID3DX11EffectShaderResourceVariable* OmniShadowMap2;
    ID3DX11EffectShaderResourceVariable* OmniShadowMap3;
    ID3DX11EffectShaderResourceVariable* OmniShadowMap4;
    ID3DX11EffectShaderResourceVariable* OmniShadowMap5;
	ID3DX11EffectMatrixVariable* ShadowTransCube0;
	ID3DX11EffectMatrixVariable* ShadowTransCube1;
	ID3DX11EffectMatrixVariable* ShadowTransCube2;
	ID3DX11EffectMatrixVariable* ShadowTransCube3;
	ID3DX11EffectMatrixVariable* ShadowTransCube4;
	ID3DX11EffectMatrixVariable* ShadowTransCube5;
};
#pragma endregion

#pragma region NormalMapEffect
class NormalMapEffect : public Effect
{
public:
	NormalMapEffect(ID3D11Device* device, const std::wstring& filename);
	~NormalMapEffect();

	void SetWorldViewProj(CXMMATRIX M)                  { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M)                          { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(CXMMATRIX M)              { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetShadowTransform(CXMMATRIX M)                { ShadowTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetShadowTransform2(CXMMATRIX M)               { ShadowTransform2->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetTexTransform(CXMMATRIX M)                   { TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const XMFLOAT3& v)                  { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetFogColor(const FXMVECTOR v)                 { FogColor->SetFloatVector(reinterpret_cast<const float*>(&v)); }
	void SetFogStart(float f)                           { FogStart->SetFloat(f); }
	void SetFogRange(float f)                           { FogRange->SetFloat(f); }
	void SetDirLights(const DirectionalLight* lights)   { DirLights->SetRawValue(lights, 0, 3*sizeof(DirectionalLight)); }
	void SetPointLights(const PointLight* lights)		{ PointLights->SetRawValue(lights, 0, sizeof(PointLight)); }
	void SetMaterial(const Material& mat)               { Mat->SetRawValue(&mat, 0, sizeof(Material)); }
	void SetDiffuseMap(ID3D11ShaderResourceView* tex)   { DiffuseMap->SetResource(tex); }
	void SetCubeMap(ID3D11ShaderResourceView* tex)      { CubeMap->SetResource(tex); }
	void SetNormalMap(ID3D11ShaderResourceView* tex)    { NormalMap->SetResource(tex); }
	void SetShadowMap(ID3D11ShaderResourceView* tex)    { ShadowMap->SetResource(tex); }
	void SetShadowMap2(ID3D11ShaderResourceView* tex)   { ShadowMap2->SetResource(tex); }
	void SetTextureArray(vector<ID3D11ShaderResourceView*> texVec) { TextureArrayPtr->SetResourceArray(&texVec[0], 0, texVec.size());}
	void SetNormalArray(vector<ID3D11ShaderResourceView*> texVec) { NormalArrayPtr->SetResourceArray(&texVec[0], 0, texVec.size());}

	void SetOmniShadowMaps(ID3D11ShaderResourceView* M0, ID3D11ShaderResourceView* M1,
        ID3D11ShaderResourceView* M2, ID3D11ShaderResourceView* M3, ID3D11ShaderResourceView* M4, 
        ID3D11ShaderResourceView* M5)	   { OmniShadowMap0->SetResource(M0);
                                             OmniShadowMap1->SetResource(M1);
                                             OmniShadowMap2->SetResource(M2);
                                             OmniShadowMap3->SetResource(M3);
                                             OmniShadowMap4->SetResource(M4);
                                             OmniShadowMap5->SetResource(M5);}
	void SetShadowTransforms(CXMMATRIX M0, CXMMATRIX M1, 
		CXMMATRIX M2, CXMMATRIX M3, 
		CXMMATRIX M4, CXMMATRIX M5)						   { ShadowTransCube0->SetMatrix(reinterpret_cast<const float*>(&M0)); 
															 ShadowTransCube1->SetMatrix(reinterpret_cast<const float*>(&M1)); 
															 ShadowTransCube2->SetMatrix(reinterpret_cast<const float*>(&M2)); 
															 ShadowTransCube3->SetMatrix(reinterpret_cast<const float*>(&M3)); 
															 ShadowTransCube4->SetMatrix(reinterpret_cast<const float*>(&M4)); 
															 ShadowTransCube5->SetMatrix(reinterpret_cast<const float*>(&M5)); }

	ID3DX11EffectTechnique* Light1Tech;
	ID3DX11EffectTechnique* Light2Tech;
	ID3DX11EffectTechnique* Light3Tech;

	ID3DX11EffectTechnique* Light0TexTech;
	ID3DX11EffectTechnique* Light1TexTech;
	ID3DX11EffectTechnique* Light2TexTech;
	ID3DX11EffectTechnique* Light3TexTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipTech;

	ID3DX11EffectTechnique* Light1FogTech;
	ID3DX11EffectTechnique* Light2FogTech;
	ID3DX11EffectTechnique* Light3FogTech;

	ID3DX11EffectTechnique* Light0TexFogTech;
	ID3DX11EffectTechnique* Light1TexFogTech;
	ID3DX11EffectTechnique* Light2TexFogTech;
	ID3DX11EffectTechnique* Light3TexFogTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipFogTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipFogTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipFogTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipFogTech;

	ID3DX11EffectTechnique* Light1ReflectTech;
	ID3DX11EffectTechnique* Light2ReflectTech;
	ID3DX11EffectTechnique* Light3ReflectTech;

	ID3DX11EffectTechnique* Light0TexReflectTech;
	ID3DX11EffectTechnique* Light1TexReflectTech;
	ID3DX11EffectTechnique* Light2TexReflectTech;
	ID3DX11EffectTechnique* Light3TexReflectTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipReflectTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipReflectTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipReflectTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipReflectTech;

	ID3DX11EffectTechnique* Light1FogReflectTech;
	ID3DX11EffectTechnique* Light2FogReflectTech;
	ID3DX11EffectTechnique* Light3FogReflectTech;

	ID3DX11EffectTechnique* Light0TexFogReflectTech;
	ID3DX11EffectTechnique* Light1TexFogReflectTech;
	ID3DX11EffectTechnique* Light2TexFogReflectTech;
	ID3DX11EffectTechnique* Light3TexFogReflectTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipFogReflectTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipFogReflectTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipFogReflectTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipFogReflectTech;

	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectMatrixVariable* ShadowTransform;
	ID3DX11EffectMatrixVariable* ShadowTransform2;
	ID3DX11EffectMatrixVariable* TexTransform;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVectorVariable* FogColor;
	ID3DX11EffectScalarVariable* FogStart;
	ID3DX11EffectScalarVariable* FogRange;
	ID3DX11EffectVariable* DirLights;
	ID3DX11EffectVariable* PointLights;
	ID3DX11EffectVariable* Mat;

	ID3DX11EffectShaderResourceVariable* TextureArrayPtr;
	ID3DX11EffectShaderResourceVariable* NormalArrayPtr;

	ID3DX11EffectShaderResourceVariable* DiffuseMap;
	ID3DX11EffectShaderResourceVariable* CubeMap;
	ID3DX11EffectShaderResourceVariable* NormalMap;
	ID3DX11EffectShaderResourceVariable* ShadowMap;
	ID3DX11EffectShaderResourceVariable* ShadowMap2;

	ID3DX11EffectShaderResourceVariable* OmniShadowMap0;
    ID3DX11EffectShaderResourceVariable* OmniShadowMap1;
    ID3DX11EffectShaderResourceVariable* OmniShadowMap2;
    ID3DX11EffectShaderResourceVariable* OmniShadowMap3;
    ID3DX11EffectShaderResourceVariable* OmniShadowMap4;
    ID3DX11EffectShaderResourceVariable* OmniShadowMap5;
	ID3DX11EffectMatrixVariable* ShadowTransCube0;
	ID3DX11EffectMatrixVariable* ShadowTransCube1;
	ID3DX11EffectMatrixVariable* ShadowTransCube2;
	ID3DX11EffectMatrixVariable* ShadowTransCube3;
	ID3DX11EffectMatrixVariable* ShadowTransCube4;
	ID3DX11EffectMatrixVariable* ShadowTransCube5;
};
#pragma endregion

#pragma region DisplacementMapEffect
class DisplacementMapEffect : public Effect
{
public:
	DisplacementMapEffect(ID3D11Device* device, const std::wstring& filename);
	~DisplacementMapEffect();

	void SetViewProj(CXMMATRIX M)                       { ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldViewProj(CXMMATRIX M)                  { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M)                          { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(CXMMATRIX M)              { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetShadowTransform(CXMMATRIX M)                { ShadowTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetShadowTransform2(CXMMATRIX M)               { ShadowTransform2->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetTexTransform(CXMMATRIX M)                   { TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const XMFLOAT3& v)                  { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetFogColor(const FXMVECTOR v)                 { FogColor->SetFloatVector(reinterpret_cast<const float*>(&v)); }
	void SetFogStart(float f)                           { FogStart->SetFloat(f); }
	void SetFogRange(float f)                           { FogRange->SetFloat(f); }
	void SetDirLights(const DirectionalLight* lights)   { DirLights->SetRawValue(lights, 0, 3*sizeof(DirectionalLight)); }
	void SetPointLights(const PointLight* lights)		{ PointLights->SetRawValue(lights, 0, sizeof(PointLight)); }
	void SetMaterial(const Material& mat)               { Mat->SetRawValue(&mat, 0, sizeof(Material)); }
	void SetHeightScale(float f)                        { HeightScale->SetFloat(f); }
	void SetMaxTessDistance(float f)                    { MaxTessDistance->SetFloat(f); }
	void SetMinTessDistance(float f)                    { MinTessDistance->SetFloat(f); }
	void SetMinTessFactor(float f)                      { MinTessFactor->SetFloat(f); }
	void SetMaxTessFactor(float f)                      { MaxTessFactor->SetFloat(f); }

	void SetDiffuseMap(ID3D11ShaderResourceView* tex)   { DiffuseMap->SetResource(tex); }
	void SetCubeMap(ID3D11ShaderResourceView* tex)      { CubeMap->SetResource(tex); }
	void SetNormalMap(ID3D11ShaderResourceView* tex)    { NormalMap->SetResource(tex); }
	void SetShadowMap(ID3D11ShaderResourceView* tex)    { ShadowMap->SetResource(tex); }
	void SetShadowMap2(ID3D11ShaderResourceView* tex)   { ShadowMap2->SetResource(tex); }

	void SetOmniShadowMaps(ID3D11ShaderResourceView* M0, ID3D11ShaderResourceView* M1,
        ID3D11ShaderResourceView* M2, ID3D11ShaderResourceView* M3, ID3D11ShaderResourceView* M4, 
        ID3D11ShaderResourceView* M5)	   { OmniShadowMap0->SetResource(M0);
                                             OmniShadowMap1->SetResource(M1);
                                             OmniShadowMap2->SetResource(M2);
                                             OmniShadowMap3->SetResource(M3);
                                             OmniShadowMap4->SetResource(M4);
                                             OmniShadowMap5->SetResource(M5);}
	void SetShadowTransforms(CXMMATRIX M0, CXMMATRIX M1, 
		CXMMATRIX M2, CXMMATRIX M3, 
		CXMMATRIX M4, CXMMATRIX M5)						   { ShadowTransCube0->SetMatrix(reinterpret_cast<const float*>(&M0)); 
															 ShadowTransCube1->SetMatrix(reinterpret_cast<const float*>(&M1)); 
															 ShadowTransCube2->SetMatrix(reinterpret_cast<const float*>(&M2)); 
															 ShadowTransCube3->SetMatrix(reinterpret_cast<const float*>(&M3)); 
															 ShadowTransCube4->SetMatrix(reinterpret_cast<const float*>(&M4)); 
															 ShadowTransCube5->SetMatrix(reinterpret_cast<const float*>(&M5)); }

	ID3DX11EffectTechnique* Light1Tech;
	ID3DX11EffectTechnique* Light2Tech;
	ID3DX11EffectTechnique* Light3Tech;

	ID3DX11EffectTechnique* Light0TexTech;
	ID3DX11EffectTechnique* Light1TexTech;
	ID3DX11EffectTechnique* Light2TexTech;
	ID3DX11EffectTechnique* Light3TexTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipTech;

	ID3DX11EffectTechnique* Light1FogTech;
	ID3DX11EffectTechnique* Light2FogTech;
	ID3DX11EffectTechnique* Light3FogTech;

	ID3DX11EffectTechnique* Light0TexFogTech;
	ID3DX11EffectTechnique* Light1TexFogTech;
	ID3DX11EffectTechnique* Light2TexFogTech;
	ID3DX11EffectTechnique* Light3TexFogTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipFogTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipFogTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipFogTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipFogTech;

	ID3DX11EffectTechnique* Light1ReflectTech;
	ID3DX11EffectTechnique* Light2ReflectTech;
	ID3DX11EffectTechnique* Light3ReflectTech;

	ID3DX11EffectTechnique* Light0TexReflectTech;
	ID3DX11EffectTechnique* Light1TexReflectTech;
	ID3DX11EffectTechnique* Light2TexReflectTech;
	ID3DX11EffectTechnique* Light3TexReflectTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipReflectTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipReflectTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipReflectTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipReflectTech;

	ID3DX11EffectTechnique* Light1FogReflectTech;
	ID3DX11EffectTechnique* Light2FogReflectTech;
	ID3DX11EffectTechnique* Light3FogReflectTech;

	ID3DX11EffectTechnique* Light0TexFogReflectTech;
	ID3DX11EffectTechnique* Light1TexFogReflectTech;
	ID3DX11EffectTechnique* Light2TexFogReflectTech;
	ID3DX11EffectTechnique* Light3TexFogReflectTech;

	ID3DX11EffectTechnique* Light0TexAlphaClipFogReflectTech;
	ID3DX11EffectTechnique* Light1TexAlphaClipFogReflectTech;
	ID3DX11EffectTechnique* Light2TexAlphaClipFogReflectTech;
	ID3DX11EffectTechnique* Light3TexAlphaClipFogReflectTech;

	ID3DX11EffectMatrixVariable* ViewProj;
	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectMatrixVariable* ShadowTransform;
	ID3DX11EffectMatrixVariable* ShadowTransform2;
	ID3DX11EffectMatrixVariable* TexTransform;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVectorVariable* FogColor;
	ID3DX11EffectScalarVariable* FogStart;
	ID3DX11EffectScalarVariable* FogRange;
	ID3DX11EffectVariable* DirLights;
	ID3DX11EffectVariable* PointLights;
	ID3DX11EffectVariable* Mat;
	ID3DX11EffectScalarVariable* HeightScale;
	ID3DX11EffectScalarVariable* MaxTessDistance;
	ID3DX11EffectScalarVariable* MinTessDistance;
	ID3DX11EffectScalarVariable* MinTessFactor;
	ID3DX11EffectScalarVariable* MaxTessFactor;
 
	ID3DX11EffectShaderResourceVariable* DiffuseMap;
	ID3DX11EffectShaderResourceVariable* CubeMap;
	ID3DX11EffectShaderResourceVariable* NormalMap;
	ID3DX11EffectShaderResourceVariable* ShadowMap;
	ID3DX11EffectShaderResourceVariable* ShadowMap2;

	ID3DX11EffectShaderResourceVariable* OmniShadowMap0;
    ID3DX11EffectShaderResourceVariable* OmniShadowMap1;
    ID3DX11EffectShaderResourceVariable* OmniShadowMap2;
    ID3DX11EffectShaderResourceVariable* OmniShadowMap3;
    ID3DX11EffectShaderResourceVariable* OmniShadowMap4;
    ID3DX11EffectShaderResourceVariable* OmniShadowMap5;
	ID3DX11EffectMatrixVariable* ShadowTransCube0;
	ID3DX11EffectMatrixVariable* ShadowTransCube1;
	ID3DX11EffectMatrixVariable* ShadowTransCube2;
	ID3DX11EffectMatrixVariable* ShadowTransCube3;
	ID3DX11EffectMatrixVariable* ShadowTransCube4;
	ID3DX11EffectMatrixVariable* ShadowTransCube5;
};
#pragma endregion

#pragma region BuildShadowMapEffect
class BuildShadowMapEffect : public Effect
{
public:
	BuildShadowMapEffect(ID3D11Device* device, const std::wstring& filename);
	~BuildShadowMapEffect();

	void SetViewProj(CXMMATRIX M)                       { ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldViewProj(CXMMATRIX M)                  { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M)                          { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(CXMMATRIX M)              { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetTexTransform(CXMMATRIX M)                   { TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const XMFLOAT3& v)                  { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	
	void SetHeightScale(float f)                        { HeightScale->SetFloat(f); }
	void SetMaxTessDistance(float f)                    { MaxTessDistance->SetFloat(f); }
	void SetMinTessDistance(float f)                    { MinTessDistance->SetFloat(f); }
	void SetMinTessFactor(float f)                      { MinTessFactor->SetFloat(f); }
	void SetMaxTessFactor(float f)                      { MaxTessFactor->SetFloat(f); }

	void SetDiffuseMap(ID3D11ShaderResourceView* tex)   { DiffuseMap->SetResource(tex); }
	void SetNormalMap(ID3D11ShaderResourceView* tex)    { NormalMap->SetResource(tex); }

	ID3DX11EffectTechnique* BuildShadowMapTech;
	ID3DX11EffectTechnique* BuildShadowMapAlphaClipTech;
	ID3DX11EffectTechnique* TessBuildShadowMapTech;
	ID3DX11EffectTechnique* TessBuildShadowMapAlphaClipTech;

	ID3DX11EffectMatrixVariable* ViewProj;
	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectMatrixVariable* TexTransform;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectScalarVariable* HeightScale;
	ID3DX11EffectScalarVariable* MaxTessDistance;
	ID3DX11EffectScalarVariable* MinTessDistance;
	ID3DX11EffectScalarVariable* MinTessFactor;
	ID3DX11EffectScalarVariable* MaxTessFactor;
 
	ID3DX11EffectShaderResourceVariable* DiffuseMap;
	ID3DX11EffectShaderResourceVariable* NormalMap;
};
#pragma endregion

#pragma region SkyEffect
class SkyEffect : public Effect
{
public:
	SkyEffect(ID3D11Device* device, const std::wstring& filename);
	~SkyEffect();

	void SetWorldViewProj(CXMMATRIX M)                  { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetCubeMap(ID3D11ShaderResourceView* cubemap)  { CubeMap->SetResource(cubemap); }

	ID3DX11EffectTechnique* SkyTech;

	ID3DX11EffectMatrixVariable* WorldViewProj;

	ID3DX11EffectShaderResourceVariable* CubeMap;
};
#pragma endregion

#pragma region TerrainEffect
class TerrainEffect : public Effect
{
public:
	TerrainEffect(ID3D11Device* device, const std::wstring& filename);
	~TerrainEffect();

	void SetViewProj(CXMMATRIX M)                       { ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const XMFLOAT3& v)                  { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetFogColor(const FXMVECTOR v)                 { FogColor->SetFloatVector(reinterpret_cast<const float*>(&v)); }
	void SetFogStart(float f)                           { FogStart->SetFloat(f); }
	void SetFogRange(float f)                           { FogRange->SetFloat(f); }
	void SetDirLights(const DirectionalLight* lights)   { DirLights->SetRawValue(lights, 0, 3*sizeof(DirectionalLight)); }
	void SetPointLights(const PointLight* lights)		{ PointLights->SetRawValue(lights, 0, sizeof(PointLight)); }
	void SetMaterial(const Material& mat)               { Mat->SetRawValue(&mat, 0, sizeof(Material)); }
	void SetShadowTransform(CXMMATRIX M)                { ShadowTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetShadowTransform2(CXMMATRIX M)               { ShadowTransform2->SetMatrix(reinterpret_cast<const float*>(&M)); }

	void SetMinDist(float f)                            { MinDist->SetFloat(f); }
	void SetMaxDist(float f)                            { MaxDist->SetFloat(f); }
	void SetMinTess(float f)                            { MinTess->SetFloat(f); }
	void SetMaxTess(float f)                            { MaxTess->SetFloat(f); }
	void SetTexelCellSpaceU(float f)                    { TexelCellSpaceU->SetFloat(f); }
	void SetTexelCellSpaceV(float f)                    { TexelCellSpaceV->SetFloat(f); }
	void SetWorldCellSpace(float f)                     { WorldCellSpace->SetFloat(f); }
	void SetWorldFrustumPlanes(XMFLOAT4 planes[6])      { WorldFrustumPlanes->SetFloatVectorArray(reinterpret_cast<float*>(planes), 0, 6); }

	void SetLayerMapArray(ID3D11ShaderResourceView* tex)   { LayerMapArray->SetResource(tex); }
	void SetBlendMap(ID3D11ShaderResourceView* tex)        { BlendMap->SetResource(tex); }
	void SetHeightMap(ID3D11ShaderResourceView* tex)       { HeightMap->SetResource(tex); }
	void SetShadowMap(ID3D11ShaderResourceView* tex)       { ShadowMap->SetResource(tex); }
	void SetShadowMap2(ID3D11ShaderResourceView* tex)      { ShadowMap2->SetResource(tex); }

	void SetOmniShadowMaps(ID3D11ShaderResourceView* M0, ID3D11ShaderResourceView* M1,
        ID3D11ShaderResourceView* M2, ID3D11ShaderResourceView* M3, ID3D11ShaderResourceView* M4, 
        ID3D11ShaderResourceView* M5)	   { OmniShadowMap0->SetResource(M0);
                                             OmniShadowMap1->SetResource(M1);
                                             OmniShadowMap2->SetResource(M2);
                                             OmniShadowMap3->SetResource(M3);
                                             OmniShadowMap4->SetResource(M4);
                                             OmniShadowMap5->SetResource(M5);}
	void SetShadowTransforms(CXMMATRIX M0, CXMMATRIX M1, 
		CXMMATRIX M2, CXMMATRIX M3, 
		CXMMATRIX M4, CXMMATRIX M5)						   { ShadowTransCube0->SetMatrix(reinterpret_cast<const float*>(&M0)); 
															 ShadowTransCube1->SetMatrix(reinterpret_cast<const float*>(&M1)); 
															 ShadowTransCube2->SetMatrix(reinterpret_cast<const float*>(&M2)); 
															 ShadowTransCube3->SetMatrix(reinterpret_cast<const float*>(&M3)); 
															 ShadowTransCube4->SetMatrix(reinterpret_cast<const float*>(&M4)); 
															 ShadowTransCube5->SetMatrix(reinterpret_cast<const float*>(&M5)); }

	ID3DX11EffectTechnique* Light1Tech;
	ID3DX11EffectTechnique* Light2Tech;
	ID3DX11EffectTechnique* Light3Tech;
	ID3DX11EffectTechnique* Light1FogTech;
	ID3DX11EffectTechnique* Light2FogTech;
	ID3DX11EffectTechnique* Light3FogTech;

	ID3DX11EffectMatrixVariable* ViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;	
	ID3DX11EffectMatrixVariable* ShadowTransform;
	ID3DX11EffectMatrixVariable* ShadowTransform2;
	ID3DX11EffectMatrixVariable* TexTransform;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVectorVariable* FogColor;
	ID3DX11EffectScalarVariable* FogStart;
	ID3DX11EffectScalarVariable* FogRange;
	ID3DX11EffectVariable* DirLights;
	ID3DX11EffectVariable* PointLights;
	ID3DX11EffectVariable* Mat;
	ID3DX11EffectScalarVariable* ScreenWidth;
	ID3DX11EffectScalarVariable* ScreenHeight;
	ID3DX11EffectScalarVariable* MinDist;
	ID3DX11EffectScalarVariable* MaxDist;
	ID3DX11EffectScalarVariable* MinTess;
	ID3DX11EffectScalarVariable* MaxTess;
	ID3DX11EffectScalarVariable* TexelCellSpaceU;
	ID3DX11EffectScalarVariable* TexelCellSpaceV;
	ID3DX11EffectScalarVariable* WorldCellSpace;
	ID3DX11EffectVectorVariable* WorldFrustumPlanes;

	ID3DX11EffectShaderResourceVariable* LayerMapArray;
	ID3DX11EffectShaderResourceVariable* BlendMap;
	ID3DX11EffectShaderResourceVariable* HeightMap;
	ID3DX11EffectShaderResourceVariable* ShadowMap;
	ID3DX11EffectShaderResourceVariable* ShadowMap2;

	ID3DX11EffectShaderResourceVariable* OmniShadowMap0;
    ID3DX11EffectShaderResourceVariable* OmniShadowMap1;
    ID3DX11EffectShaderResourceVariable* OmniShadowMap2;
    ID3DX11EffectShaderResourceVariable* OmniShadowMap3;
    ID3DX11EffectShaderResourceVariable* OmniShadowMap4;
    ID3DX11EffectShaderResourceVariable* OmniShadowMap5;
	ID3DX11EffectMatrixVariable* ShadowTransCube0;
	ID3DX11EffectMatrixVariable* ShadowTransCube1;
	ID3DX11EffectMatrixVariable* ShadowTransCube2;
	ID3DX11EffectMatrixVariable* ShadowTransCube3;
	ID3DX11EffectMatrixVariable* ShadowTransCube4;
	ID3DX11EffectMatrixVariable* ShadowTransCube5;
};
#pragma endregion

#pragma region DebugTexEffect
class DebugTexEffect : public Effect
{
public:
	DebugTexEffect(ID3D11Device* device, const std::wstring& filename);
	~DebugTexEffect();

	void SetWorldViewProj(CXMMATRIX M)              { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetTexture(ID3D11ShaderResourceView* tex)  { Texture->SetResource(tex); }

	ID3DX11EffectTechnique* ViewArgbTech;
	ID3DX11EffectTechnique* ViewRedTech;
	ID3DX11EffectTechnique* ViewGreenTech;
	ID3DX11EffectTechnique* ViewBlueTech;
	ID3DX11EffectTechnique* ViewAlphaTech;

	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectShaderResourceVariable* Texture;
};
#pragma endregion

#pragma region SpriteEffect
class SpriteEffect : public Effect
{
public:
	SpriteEffect(ID3D11Device* device, const std::wstring& filename);
	~SpriteEffect();

	void SetSpriteMap(ID3D11ShaderResourceView* tex)   { SpriteMap->SetResource(tex); }

	ID3DX11EffectTechnique* SpriteTech;
	ID3DX11EffectShaderResourceVariable* SpriteMap;
};
#pragma endregion

#pragma region ParticleEffect
class ParticleEffect : public Effect
{
public:
	ParticleEffect(ID3D11Device* device, const std::wstring& filename);
	~ParticleEffect();

	void SetViewProj(CXMMATRIX M)                       { ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }

	void SetGameTime(float f)                           { GameTime->SetFloat(f); }
	void SetTimeStep(float f)                           { TimeStep->SetFloat(f); }

	void SetEyePosW(const XMFLOAT3& v)                  { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetEmitPosW(const XMFLOAT3& v)                 { EmitPosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetEmitDirW(const XMFLOAT3& v)                 { EmitDirW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }

	void SetTexArray(ID3D11ShaderResourceView* tex)     { TexArray->SetResource(tex); }
	void SetRandomTex(ID3D11ShaderResourceView* tex)    { RandomTex->SetResource(tex); }
	
	ID3DX11EffectTechnique* StreamOutTech;
	ID3DX11EffectTechnique* DrawTech;

	ID3DX11EffectMatrixVariable* ViewProj;
	ID3DX11EffectScalarVariable* GameTime;
	ID3DX11EffectScalarVariable* TimeStep;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVectorVariable* EmitPosW;
	ID3DX11EffectVectorVariable* EmitDirW;
	ID3DX11EffectShaderResourceVariable* TexArray;
	ID3DX11EffectShaderResourceVariable* RandomTex;
};
#pragma endregion

#pragma region Effects
class Effects
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	static BasicEffect* BasicFX;
	static NormalMapEffect* NormalMapFX;
	static DisplacementMapEffect* DisplacementMapFX;
	static BuildShadowMapEffect* BuildShadowMapFX;
	static SkyEffect* SkyFX;
	static TerrainEffect* TerrainFX;
	static DebugTexEffect* DebugTexFX;
	static SpriteEffect* SpriteFX;
	static ParticleEffect* FireFX;
	static ParticleEffect* RainFX;
};
#pragma endregion

#endif // EFFECTS_H