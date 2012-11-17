cbuffer Lighting
{
	float4 ambientcolor;
	float4 diffusecolor;
	float4 specularcolor;
	float specularpower;
	float3 lightdirection;
}

struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

float4 PShader(VOut input) : SV_TARGET
{
    return ambientcolor;
}
