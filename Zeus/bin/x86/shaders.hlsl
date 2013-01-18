cbuffer ConstantBuffer
{
    float4x4 matFinal;
	float4x4 matWorld;
}

cbuffer CameraBuffer
{
    float3 cameraPosition;
}

cbuffer LightBuffer
{
    float4 ambientColor;
	float4 diffuseColor;
	float3 lightDirection;
	float specularPower;
	float4 specularColor;
}

struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
	float3 normal : NORMAL;
	float3 viewDirection : VIEWDIRECTION;
};

VOut VShader(float4 position : POSITION, float4 color : COLOR, float3 normal : NORMAL)
{
    VOut output;
	float4 worldPosition;

    output.position = mul(matFinal, position);
    output.color = color;
	output.normal = mul(normal, (float3x3)matWorld);
	output.normal = normalize(output.normal);

	worldPosition = mul(output.position, matWorld);
	output.viewDirection = cameraPosition.xyz - worldPosition.xyz;
	output.viewDirection = normalize(output.viewDirection);

    return output;
}


float4 PShader(VOut input) : SV_TARGET
{
//	float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;
    float3 reflection;
    float4 specular;


    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
//    textureColor = shaderTexture.Sample(SampleType, input.tex);

    // Set the default output color to the ambient light value for all pixels.
    color = ambientColor;

    // Initialize the specular color.
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // Invert the light direction for calculations.
    lightDir = -lightDirection;

    // Calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(input.normal, lightDir));

    if(lightIntensity > 0.0f)
    {
        // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
        color += (diffuseColor * lightIntensity);

        // Saturate the ambient and diffuse color.
        color = saturate(color);

		 // Calculate the reflection vector based on the light intensity, normal vector, and light direction.
        reflection = normalize(2 * lightIntensity * input.normal - lightDir); 

		 // Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
        specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);
    }

    // Multiply the texture pixel and the input color to get the textured result.
    color = color * input.color;

	// Add the specular component last to the output color.
    color = saturate(color + specular);

    return color;
}
