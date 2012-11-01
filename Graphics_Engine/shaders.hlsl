cbuffer ConstantBuffer
{
    float4x4 matFinal;
}

struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VOut VShader(float4 position : POSITION, float4 color : COLOR)
{
    VOut output;

    output.position = mul(matFinal, position);    // transform the vertex from 3D to 2D
    output.color = color;

    return output;
}

float4 PShaderInterp(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
    return color;
}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
	color.r = 0.25f;
	color.g = 0.0f;
	color.b = 0.75f;
    return color;
}
