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
	float3x3 tbnmatrix : TBNMATRIX;
	float3 reflect : REFLECT;
};

Texture2D Texture;
Texture2D Alpha;
Texture2D NormalMap;
texture ReflectMap;

SamplerState ss;

VOut VShader(float4 position : POSITION, float4 color : COLOR, float3 normal : NORMAL, float2 texcord : TEXCORD, float3 tangent : TANGENT)
{
    VOut output;
	float3 tang;
    float4 worldPosition;

    output.position = mul(position, matWorld);
	output.position = mul(output.position, matView);
	output.position = mul(output.position, matProjection);

    output.color = color;
    
	output.texcord = texcord;

    // Calculate the normal vector against the world matrix only.
    output.normal = mul(normal, (float3x3)matWorld);
	tang = mul(tangent, (float3x3)matWorld);
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);

	float3 bitangent = normalize( cross(output.normal, tang));
	output.tbnmatrix = transpose(float3x3(tang,bitangent,output.normal));

    // Calculate the position of the vertex in the world.
    worldPosition = mul(position, matWorld);

    // Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
    output.viewDirection = cameraPosition.xyz - worldPosition.xyz;
	
    // Normalize the viewing direction vector.
    output.viewDirection = normalize(output.viewDirection);
	
	// Create reflection angle.
	float3 Incident = -output.viewDirection;
	output.reflect = normalize(reflect(Incident, output.normal));

    return output;
}