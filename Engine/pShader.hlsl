cbuffer Lighting : register(cb0)
{
	float4 ambientColor;
	float4 diffuseColor;
	float4 specularcolor;
	float specularPower;
	float3 lightDirection;
}

struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
	float3 normal : NORMAL;
	float3 viewDirection : VIEWDIRECTION;
	float2 texcord : TEXCORD;
};

Texture2D Texture;
Texture2D Alpha;
SamplerState ss;

float4 PShader(VOut input) : SV_TARGET
{
	float4 textureColor;
    float4 alphaColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;
    float3 reflection;
    float4 specular;
	float resAlpha;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
	if(input.color.x < 0.0)
	{
		textureColor = Texture.Sample(ss, input.texcord);
        alphaColor = Alpha.Sample(ss, input.texcord);
        textureColor.a = (alphaColor.r + alphaColor.g + alphaColor.b ) / 3.0;
        if(textureColor.a < 0.1f)
        {
            textureColor.a = 0.0f;
        }
	}

    // Set the default output color to the ambient light value for all pixels.
    color = ambientColor;

    // Initialize the specular color.
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // Invert the light direction for calculations.
    lightDir = -lightDirection;

	lightDir = normalize(lightDir);
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

	if(input.color.x >= 0)
	{
		color = color * input.color;
	}
	else
	{
		color = color * textureColor;
	}
    // Add the specular component last to the output color.
    color = saturate(color + specular);


    return color;
}
