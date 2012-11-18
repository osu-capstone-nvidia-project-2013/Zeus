cbuffer ConstantBuffer : register(cb0)
{
    float4x4 matWorld;
	float4x4 matView;
	float4x4 matProjection;
	float3 cameraPosition;
	float padding;
}



struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
	float3 normal : NORMAL;
	float3 viewDirection : VIEWDIRECTION;
	float2 texcord : TEXCORD;
};

VOut VShader(float4 position : POSITION, float4 color : COLOR, float3 normal : NORMAL, float2 texcord : TEXCORD)
{
    VOut output;
    float4 worldPosition;

    output.position = mul(position, matWorld);
	output.position = mul(output.position, matView);
	output.position = mul(output.position, matProjection);

    output.color = color;
    
	output.texcord = fmod(texcord, float2(1.0f, 1.0f));

    // Calculate the normal vector against the world matrix only.
    output.normal = mul(normal, (float3x3)matWorld);
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);

    // Calculate the position of the vertex in the world.
    worldPosition = mul(position, matWorld);

    // Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
    output.viewDirection = cameraPosition.xyz - worldPosition.xyz;
	
    // Normalize the viewing direction vector.
    output.viewDirection = normalize(output.viewDirection);
	

    return output;
}