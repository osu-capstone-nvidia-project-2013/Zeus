struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
	float4 normal : NORMAL;
};

VOut VShader(float4 position : POSITION, float4 color : COLOR, float3 normal : NORMAL)
{
    VOut output;

    output.position = position;
    output.color = color;

    return output;
}


float4 PShader(float4 position : SV_POSITION, float4 color : COLOR, float4 normal : NORMAL) : SV_TARGET
{
    return color;
}
